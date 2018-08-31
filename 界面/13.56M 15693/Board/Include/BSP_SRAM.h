#ifndef __BSP_SRAM_H
#define __BSP_SRAM_H

#include "stm32f4xx.h"

#define Bank1_SRAM2_ADDR    ((u32)0x64000000)

void BSP_SRAM_Init(void);
void BSP_SRAM_WriteBuffer(u16* pBuffer, u32 WriteAddr, u32 NumHalfwordToWrite);
void BSP_SRAM_ReadBuffer(u16* pBuffer, u32 ReadAddr, u32 NumHalfwordToRead);

#endif
