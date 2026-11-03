#include "app.h"
#include "driver_uart.h"
#include "logger.h"

enum app_status
app_init(void *uart_handle)
{
    if (drv_uart_init(uart_handle) != DRV_UART_OK)
    {
        return APP_ERR_UART;
    }

    if (logger_init() != LOGGER_OK)
    {
        return APP_ERR_LOGGER_INIT;
    }

    if (logger_log(LOG_LEVEL_INFO, "boot done") != LOGGER_OK)
    {
        return APP_ERR_LOGGER;
    }

    return APP_OK;
}
