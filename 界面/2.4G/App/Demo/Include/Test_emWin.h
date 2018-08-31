#ifndef __TEST_EMWIN_H
#define __TEST_EMWIN_H

#include "stm32f4xx.h"
#include "BSP_DebugUART.h"
#include "BSP_Delay.h"
#include "Queue.h"
#include "BSP_MyIIC.h"
#include "BSP_GSL268x.h"
#include "BSP_Timer.h"
#include "BSP_TouchPad.h"
#include "BSP_ParallelBus.h"
#include "BSP_RA8875_Port.h"
#include "BSP_RA8875.h"
#include "fonts.h"
#include "BSP_TFTLCD.h"
//#include "StartBMP.h"  //首次包含，以后可以不再包含！
#include "Test_emWin.h"
#include "BSP_SRAM.h"
#include "BSP_UART.h"

void Test_TouchPad(void);

void Test_TFTLCD(void);

void Test_SRAM(void);

#endif
