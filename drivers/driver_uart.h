#ifndef DRIVER_UART_H
#define DRIVER_UART_H

#include <stddef.h>
#include <stdint.h>

enum
{
    DRV_UART_TX_BUFFER_SIZE = 128,
};

enum drv_uart_status
{
    DRV_UART_OK = 0,
    DRV_UART_ERR_PARAM,
    DRV_UART_ERR_NOT_INIT,
    DRV_UART_ERR_ALREADY_INIT,
    DRV_UART_ERR_BUSY,
    DRV_UART_ERR_HAL,
};

enum drv_uart_status drv_uart_init(void *handle);

/* Thread mode only */
/* 128 bytes frame takes around 11 ms at 115200 baudrate to transmit the message 
   if logs' delay is less than 11 ms TX line will return BUSY state and message will be dropped */
enum drv_uart_status drv_uart_write(const char *buffer, size_t buffer_length);

#endif /* DRIVER_UART_H */
