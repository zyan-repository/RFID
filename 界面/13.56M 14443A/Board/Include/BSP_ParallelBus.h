#ifndef __BSP_PBUS_H
#define __BSP_PBUS_H

#include "stm32f4xx.h"

#define Bank1_SRAM4_ADDR    ((uint32_t)0x6C000000)

void BSP_PBus_Init(void);
void BSP_PBus_WriteBuffer(uint16_t* pBuffer, uint32_t WriteAddr, uint32_t NumHalfwordToWrite);
void BSP_PBus_ReadBuffer(uint16_t* pBuffer, uint32_t ReadAddr, uint32_t NumHalfwordToRead);

#endif
