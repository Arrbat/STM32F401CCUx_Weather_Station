#include "logger.h"
#include "driver_uart.h"
#include <stddef.h>
#include <stdint.h>

_Static_assert(MAX_LOG_LENGTH <= DRV_UART_TX_BUFFER_SIZE,
               "MAX_LOG_LENGTH exceeds the UART driver TX buffer");

static uint8_t is_initialized;

static const char *const LEVEL_NAME[] = {
    [LOG_LEVEL_NONE] = "NONE",       [LOG_LEVEL_ERROR] = "ERROR",
    [LOG_LEVEL_WARNING] = "WARNING", [LOG_LEVEL_INFO] = "INFO",
    [LOG_LEVEL_DEBUG] = "DEBUG",
};

/* Appends src to dst starting at pos, never writing at index >= limit. */
static size_t
append(char *dst, size_t pos, size_t limit, const char *src)
{
    while (*src != '\0' && pos < limit)
    {
        dst[pos++] = *src++;
    }

    return pos;
}

enum logger_status
logger_init(void)
{
    if (is_initialized == 1)
    {
        return LOGGER_ERR_ALREADY_INIT;
    }

    is_initialized = 1;
    return LOGGER_OK;
}

enum logger_status
logger_log(enum log_level level, const char *message)
{
    char frame[MAX_LOG_LENGTH];
    /* One byte is reserved for the trailing '\n'. */
    const size_t text_limit = MAX_LOG_LENGTH - 1;
    size_t i = 0;

    if (is_initialized == 0)
    {
        return LOGGER_ERR_NOT_INIT;
    }

    if (message == NULL || (unsigned int)level > (unsigned int)LOG_LEVEL_DEBUG)
    {
        return LOGGER_ERR_PARAM;
    }

    /* "[LEVEL] message\n" */
    i = append(frame, i, text_limit, "[");
    i = append(frame, i, text_limit, LEVEL_NAME[level]);
    i = append(frame, i, text_limit, "] ");
    i = append(frame, i, text_limit, message);
    frame[i++] = '\n';

    switch (drv_uart_write(frame, i))
    {
    case DRV_UART_OK:
        return LOGGER_OK;
    case DRV_UART_ERR_BUSY:
        return LOGGER_ERR_BUSY;
    default:
        return LOGGER_ERR_DRIVER;
    }
}
