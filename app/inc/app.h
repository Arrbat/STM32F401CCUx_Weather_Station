#ifndef APP_H
#define APP_H

#include "gcc.h"

enum app_status
{
    APP_OK = 0,
    APP_ERR_UART,
    APP_ERR_LOGGER_INIT,
    APP_ERR_LOGGER,
};

/* Thread mode only. Safe to retry with the same UART handle. */
WS_MUST_CHECK enum app_status app_init(void *uart_handle);

#endif /* APP_H */
