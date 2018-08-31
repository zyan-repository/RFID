#ifndef __BSP_UART_H
#define __BSP_UART_H

#include <stdio.h>
#include "stm32f4xx.h"

void BSP_UART_Init(uint32_t BaudRate);
void USART_printf( USART_TypeDef * USARTx, char * Data, ... );

#endif
