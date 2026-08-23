#include "y_lpuart.h"
#include "stm32g031xx.h"

YLPUART_returnStatus ylpuart_setConfig(YLPUART_typeDef* handler)
{
    /* TODO: implement */

    return YLPUART_error;
}

YLPUART_returnStatus ylpuart_transmit(YLPUART_typeDef* handler, uint8_t* pData, uint16_t dataSize)
{
    /* no transfers if controller is busy */
    if (handler->status == YLPUART_busy)
    {
        return YLPUART_error;
    }

    handler->status = YLPUART_busy;

    for (; dataSize > 0; --dataSize)
    {
        /* wait for last transaction to finish */
        while (!(handler->instance->ISR & USART_ISR_TXE_TXFNF))
            ;

        handler->instance->TDR = *pData;
        ++pData;
    }

    /* wait for transaction to complete before releasing status */
    while (!(handler->instance->ISR & USART_ISR_TC))
        ;
    handler->status = YLPUART_ready;

    return YLPUART_ok;
}

YLPUART_returnStatus ylpuart_receive(YLPUART_typeDef* handler, uint8_t* pData, uint16_t dataSize)
{
    /* no transfers if controller is busy */
    if (handler->status == YLPUART_busy)
    {
        return YLPUART_error;
    }

    handler->status = YLPUART_busy;

    for (; dataSize > 0; --dataSize)
    {
        /* wait for data to be available */
        while (!(handler->instance->ISR & USART_ISR_RXNE_RXFNE))
            ;

        /* mask is used to remove parity bit from data */
        *pData = handler->instance->RDR & handler->mask;
        ++pData;
    }

    handler->status = YLPUART_ready;

    return YLPUART_ok;
}
