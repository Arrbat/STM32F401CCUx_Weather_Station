#include "driver_uart.h"
#include "usart.h"
#include <stdbool.h>
#include <stdint.h>
#include <string.h>

typedef struct
{
    const char *buffer;
    size_t buffer_length;
    UART_HandleTypeDef *handle;
} uart_ctx_t;

static char uart_tx_buffer[DRV_UART_TX_BUFFER_SIZE];
static uart_ctx_t uart_ctx;

/*
 * gState is used directly instead of HAL_UART_GetState(): GetState returns
 * gState | RxState.
 * gState can only go BUSY_TX -> READY asynchronously (TX ISR). READY ->
 * BUSY_TX happens only in HAL_UART_Transmit_IT(), called from thread mode.
 * So a "READY" reading cannot become stale before memcpy().
 */
static bool
uart_is_busy(const UART_HandleTypeDef *handle)
{
    return handle->gState != HAL_UART_STATE_READY;
}

static drv_uart_status_t
drv_uart_transmit(uart_ctx_t *ctx)
{
    HAL_StatusTypeDef hal_ret = HAL_OK;

    if (ctx == NULL || ctx->handle == NULL || ctx->buffer == NULL
        || ctx->buffer_length == 0
        || ctx->buffer_length > DRV_UART_TX_BUFFER_SIZE)
    {
        return DRV_UART_ERR_PARAM;
    }

    if (uart_is_busy(ctx->handle))
    {
        return DRV_UART_ERR_BUSY;
    }

    hal_ret = HAL_UART_Transmit_IT(ctx->handle, (uint8_t *)ctx->buffer,
                                   (uint16_t)ctx->buffer_length);

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

drv_uart_status_t
drv_uart_init(void *handle)
{
    if (handle == NULL)
    {
        return DRV_UART_ERR_PARAM;
    }

    if (uart_ctx.handle != NULL)
    {
        return DRV_UART_ERR_ALREADY_INIT;
    }

    uart_ctx.buffer = uart_tx_buffer;
    uart_ctx.buffer_length = 0;
    uart_ctx.handle = (UART_HandleTypeDef *)handle;

    return DRV_UART_OK;
}

drv_uart_status_t
drv_uart_write(const char *buffer, size_t buffer_length)
{
    if (buffer == NULL || buffer_length == 0
        || buffer_length > DRV_UART_TX_BUFFER_SIZE)
    {
        return DRV_UART_ERR_PARAM;
    }

    if (uart_ctx.handle == NULL)
    {
        return DRV_UART_ERR_NOT_INIT;
    }

    /* Must precede memcpy(): the TX ISR may still be reading the buffer. */
    if (uart_is_busy(uart_ctx.handle))
    {
        return DRV_UART_ERR_BUSY;
    }

    memcpy(uart_tx_buffer, buffer, buffer_length);

    uart_ctx.buffer = uart_tx_buffer;
    uart_ctx.buffer_length = buffer_length;

    return drv_uart_transmit(&uart_ctx);
}
