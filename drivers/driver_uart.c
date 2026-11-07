#include "driver_uart.h"
#include "usart.h"
#include <stdint.h>
#include <string.h>

static UART_HandleTypeDef **
drv_uart_handle_storage(void)
{
    static UART_HandleTypeDef *handle;
    return &handle;
}

/*
 * gState is used directly instead of HAL_UART_GetState(): GetState returns
 * gState | RxState.
 * gState can only go BUSY_TX -> READY asynchronously (TX ISR). READY ->
 * BUSY_TX happens only in HAL_UART_Transmit_IT(), called from thread mode.
 * So a "READY" reading cannot become stale before memcpy().
 */
static enum drv_uart_status
uart_status_from_state(const UART_HandleTypeDef *handle)
{
    switch (handle->gState)
    {
    case HAL_UART_STATE_READY:
        return DRV_UART_OK;
    case HAL_UART_STATE_RESET:
        return DRV_UART_ERR_NOT_INIT;
    case HAL_UART_STATE_BUSY:
    case HAL_UART_STATE_BUSY_TX:
    case HAL_UART_STATE_BUSY_RX:
    case HAL_UART_STATE_BUSY_TX_RX:
        return DRV_UART_ERR_BUSY;
    case HAL_UART_STATE_TIMEOUT:
    case HAL_UART_STATE_ERROR:
    default:
        return DRV_UART_ERR_HAL;
    }
}

static enum drv_uart_status
drv_uart_transmit(UART_HandleTypeDef *handle, const char *buffer,
                  size_t buffer_length)
{
    HAL_StatusTypeDef hal_ret = HAL_OK;

    if (handle == NULL || buffer == NULL || buffer_length == 0
        || buffer_length > DRV_UART_TX_BUFFER_SIZE)
    {
        return DRV_UART_ERR_PARAM;
    }

    enum drv_uart_status status = uart_status_from_state(handle);
    if (status != DRV_UART_OK)
    {
        return status;
    }

    hal_ret = HAL_UART_Transmit_IT(handle, (uint8_t *)buffer,
                                   (uint16_t)buffer_length);

    if (hal_ret == HAL_BUSY)
    {
        return DRV_UART_ERR_BUSY;
    }

    if (hal_ret != HAL_OK)
    {
        return DRV_UART_ERR_HAL;
    }

    return DRV_UART_OK;
}

enum drv_uart_status
drv_uart_init(void *handle)
{
    UART_HandleTypeDef **handle_storage = drv_uart_handle_storage();

    if (handle == NULL)
    {
        return DRV_UART_ERR_PARAM;
    }

    if (*handle_storage != NULL)
    {
        return DRV_UART_ERR_ALREADY_INIT;
    }

    *handle_storage = (UART_HandleTypeDef *)handle;

    return DRV_UART_OK;
}

enum drv_uart_status
drv_uart_write(const char *buffer, size_t buffer_length)
{
    static char uart_tx_buffer[DRV_UART_TX_BUFFER_SIZE];
    UART_HandleTypeDef *handle = *drv_uart_handle_storage();

    if (buffer == NULL || buffer_length == 0
        || buffer_length > DRV_UART_TX_BUFFER_SIZE)
    {
        return DRV_UART_ERR_PARAM;
    }

    if (handle == NULL)
    {
        return DRV_UART_ERR_NOT_INIT;
    }

    /* Must precede memcpy(): the TX ISR may still be reading the buffer. */
    enum drv_uart_status status = uart_status_from_state(handle);
    if (status != DRV_UART_OK)
    {
        return status;
    }

    memcpy(uart_tx_buffer, buffer, buffer_length);

    return drv_uart_transmit(handle, uart_tx_buffer, buffer_length);
}
