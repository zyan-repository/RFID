#ifndef __BSP_GSL268X_H
#define __BSP_GSL268X_H

#include "stm32f4xx.h"
#include "BSP_DebugUART.h"
#include "BSP_MyIIC.h"

#define RCC_GSL268x_INT			(RCC_AHB1Periph_GPIOG)
#define PORT_GSL268x_INT 		GPIOG
#define PIN_GSL268x_INT 		(GPIO_Pin_14)

#define GSL268x_ADDR	0x80
#define TOUCH_CODE_LEN	5049	

struct fw_data
{
    uint8_t offset;
    uint32_t val;
};

void BSP_GSL268x_Init(void);
uint8_t BSP_GSL268x_Callback(uint16_t PEN[2]);

#endif
