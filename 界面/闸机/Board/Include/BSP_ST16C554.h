#ifndef __BSP_ST16C554_H
#define __BSP_ST16C554_H

#include "stm32f4xx.h"
#include "BSP_DebugUART.h"
#include "BSP_ParallelBus.h"

/* ST16C554 CS Base Address */
#define ST16C554_CS1A_ADD        0x0800
#define ST16C554_CS1B_ADD        0x0900
#define ST16C554_CS1C_ADD        0x0A00
#define ST16C554_CS1D_ADD        0x0B00
#define ST16C554_CS2A_ADD        0x0C00
#define ST16C554_CS2B_ADD        0x0D00
#define ST16C554_CS2C_ADD        0x0E00
#define ST16C554_CS2D_ADD        0x0F00

/* ST16C554 REGISTER LIST */
#define ST16C554_REG_RHR        0x00
#define ST16C554_REG_THR        0x00
#define ST16C554_REG_IER        0x01
#define ST16C554_REG_ISR        0x02
#define ST16C554_REG_FCR        0x02
#define ST16C554_REG_LCR        0x03
#define ST16C554_REG_MCR        0x04
#define ST16C554_REG_LSR        0x05
#define ST16C554_REG_MSR        0x06
#define ST16C554_REG_SPR        0x07
#define ST16C554_REG_DLL        0x00
#define ST16C554_REG_DLM        0x01

/* œ‡πÿ∫Í…Ë÷√ */
#define ST16C554_IER_VAL       0x01
#define ST16C554_LCR_DIVVAL    0x83
#define ST16C554_LCR_DATVAL    0x03
//#define ST16C554_DLL_VAL       0x0C //57600bps
//#define ST16C554_DLM_VAL       0x00
//#define ST16C554_DLL_VAL       0x06 //115200bps
#define ST16C554_DLL_VAL 0x06//115200
#define ST16C554_DLM_VAL       0x00

void BSP_ST16C554_Init(void);

void BSP_ST16C554_CS1A_ISR(void);
uint16_t BSP_ST16C554_CS1A_Read(void);
void BSP_ST16C554_CS1A_Write(uint16_t BSP_ST16C554_CS1A_Val);
void BSP_ST16C554_CS1B_ISR(void);
uint16_t BSP_ST16C554_CS1B_Read(void);
void BSP_ST16C554_CS1B_Write(uint16_t BSP_ST16C554_CS1B_Val);
void BSP_ST16C554_CS1C_ISR(void);
uint16_t BSP_ST16C554_CS1C_Read(void);
void BSP_ST16C554_CS1C_Write(uint16_t BSP_ST16C554_CS1C_Val);
void BSP_ST16C554_CS1D_ISR(void);
uint16_t BSP_ST16C554_CS1D_Read(void);
void BSP_ST16C554_CS1D_Write(uint16_t BSP_ST16C554_CS1D_Val);
void BSP_ST16C554_CS2A_ISR(void);
uint16_t BSP_ST16C554_CS2A_Read(void);
void BSP_ST16C554_CS2A_Write(uint16_t BSP_ST16C554_CS2A_Val);
void BSP_ST16C554_CS2B_ISR(void);
uint16_t BSP_ST16C554_CS2B_Read(void);
void BSP_ST16C554_CS2B_Write(uint16_t BSP_ST16C554_CS2B_Val);
void BSP_ST16C554_CS2C_ISR(void);
uint16_t BSP_ST16C554_CS2C_Read(void);
void BSP_ST16C554_CS2C_Write(uint16_t BSP_ST16C554_CS2C_Val);
void BSP_ST16C554_CS2D_ISR(void);
uint16_t BSP_ST16C554_CS2D_Read(void);
void BSP_ST16C554_CS2D_Write(uint16_t BSP_ST16C554_CS2D_Val);

#endif
