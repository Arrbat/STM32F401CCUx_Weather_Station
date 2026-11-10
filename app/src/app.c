#include "app.h"
#include "driver_uart.h"
#include "logger.h"

enum app_status
app_init(void *uart_handle)
{
    enum drv_uart_status uart_status = drv_uart_init(uart_handle);
    if (uart_status != DRV_UART_OK && uart_status != DRV_UART_ERR_ALREADY_INIT)
    {
        return APP_ERR_UART;
    }

    enum logger_status logger_status = logger_init();
    if (logger_status != LOGGER_OK && logger_status != LOGGER_ERR_ALREADY_INIT)
    {
        return APP_ERR_LOGGER_INIT;
    }

    if (logger_log(LOG_LEVEL_INFO, "Initialized main components") != LOGGER_OK)
    {
        return APP_ERR_LOGGER;
    }

    return APP_OK;
}
