#ifndef _BSP_RA8875_PORT_H
#define _BSP_RA8875_PORT_H

#include "stm32f4xx.h"
#include "BSP_ParallelBus.h"

/* 软件未使用WAIT引脚。可以通过读取寄存器状态寄存器判断芯片忙 */
//#define USE_WAIT_PIN		

#ifdef USE_WAIT_PIN
	/* RA8875芯片的WAIT引脚 */
	#define RCC_WAIT	RCC_AHB1Periph_GPIOG
	#define PORT_WAIT	GPIOG
	#define PIN_WAIT	GPIO_Pin_11
#endif

/*
	魔法师STM32F103开发板接线方法：

	PD0/FSMC_D2
	PD1/FSMC_D3
	PD4/FSMC_NOE		--- 读控制信号，OE = Output Enable ， N 表示低有效
	PD5/FSMC_NWE		--- 写控制信号，WE = Output Enable ， N 表示低有效
	PD8/FSMC_D13
	PD9/FSMC_D14
	PD10/FSMC_D15
	PD14/FSMC_D0
	PD15/FSMC_D1

	PF1/FSMC_A1		    --- 地址 RS
	PF5/FSMC_A5		    --- 3-8译码器
	PE12/FSMC_A6		--- 3-8译码器
	PE13/FSMC_A7		--- 3-8译码器

	PE7/FSMC_D4
	PE8/FSMC_D5
	PE9/FSMC_D6
	PE10/FSMC_D7
	PE11/FSMC_D8
	PE12/FSMC_D9
	PE13/FSMC_D10
	PE14/FSMC_D11
	PE15/FSMC_D12

	PG12/FSMC_NE4		--- 主片选（TFT, DM9000A）

	PG11/WAIT			--- WAIT引脚
*/
#define RA8875_BASE		((uint32_t)(Bank1_SRAM4_ADDR | 0x00000400))

#define RA8875_REG		*(__IO uint16_t *)(RA8875_BASE +  (1 << (0 + 2)))	/* FSMC 16位总线模式下，FSMC_A0口线对应物理地址A1 */
#define RA8875_RAM		*(__IO uint16_t *)(RA8875_BASE)

#define RA8875_RAM_ADDR		RA8875_BASE


void BSP_RA8875_Port_Init(void);
void RA8875_Delaly1us(void);
void RA8875_Delaly1ms(void);
uint16_t RA8875_ReadID(void);
void RA8875_WriteCmd(uint8_t _ucRegAddr);
void RA8875_WriteData(uint8_t _ucRegValue);
uint8_t RA8875_ReadData(void);
void RA8875_WriteData16(uint16_t _usRGB);
uint16_t RA8875_ReadData16(void);
uint8_t RA8875_ReadStatus(void);
uint32_t RA8875_GetDispMemAddr(void);

uint8_t RA8875_ReadBusy(void);

#endif
