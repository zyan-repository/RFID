#ifndef __BSP_DEBUGUART_H
#define __BSP_DEBUGUART_H

#include <stdio.h>
#include "stm32f4xx.h"

void BSP_DebugUART_Init(uint32_t BaudRate);

void ISRprintf(char* string,uint8_t num);

#endif
