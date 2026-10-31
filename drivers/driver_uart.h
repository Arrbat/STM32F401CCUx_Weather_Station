#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#include <stddef.h>
#include <stdint.h>

enum
{
    DRV_UART_TX_BUFFER_SIZE = 128,
};

typedef enum
{
    DRV_UART_OK = 0,
    DRV_UART_ERR_PARAM,
    DRV_UART_ERR_NOT_INIT,
    DRV_UART_ERR_ALREADY_INIT,
    DRV_UART_ERR_BUSY,
    DRV_UART_ERR_HAL,
} drv_uart_status_t;

drv_uart_status_t drv_uart_init(void *handle);

/* Thread mode only */
drv_uart_status_t drv_uart_write(const char *buffer, size_t buffer_length);

#endif /* DRIVER_UART_H */