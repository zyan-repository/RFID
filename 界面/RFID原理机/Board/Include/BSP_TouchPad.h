#ifndef __BSP_TOUCHPAD_H
#define __BSP_TOUCHPAD_H

#include "stm32f4xx.h"
#include "BSP_DebugUART.h"
#include "BSP_GSL268x.h"

extern volatile uint16_t PENX,PENY;
extern volatile uint16_t PENX_PRE,PENY_PRE;

uint8_t BSP_TouchPad_Callback(void);
uint8_t BSP_TouchPad_InRect(uint16_t _usX, uint16_t _usY,
	uint16_t _usRectX, uint16_t _usRectY, uint16_t _usRectH, uint16_t _usRectW);
uint8_t BSP_TouchPad_MoveValid(uint16_t _usX1, uint16_t _usY1,
                               uint16_t _usX2, uint16_t _usY2);

#endif
