#ifndef __BSP_MYIIC_H
#define __BSP_MYIIC_H

#include "stm32f4xx.h"

/* 定义连接的GPIO端口, 用户只需要修改下面4行代码即可任意改变SCL和SDA的引脚 */
#define RCC_I2C_PORT 	RCC_AHB1Periph_GPIOB		/* GPIO端口时钟 */

#define PORT_I2C_SCL	GPIOB			/* GPIO端口 */
#define PIN_I2C_SCL		GPIO_Pin_6		/* GPIO引脚 */

#define PORT_I2C_SDA	GPIOB			/* GPIO端口 */
#define PIN_I2C_SDA		GPIO_Pin_7		/* GPIO引脚 */


/* 定义读写SCL和SDA的宏 */
#define I2C_SCL_1()  PORT_I2C_SCL->BSRRL = PIN_I2C_SCL				/* SCL = 1 */
#define I2C_SCL_0()  PORT_I2C_SCL->BSRRH = PIN_I2C_SCL				/* SCL = 0 */

#define I2C_SDA_1()  PORT_I2C_SDA->BSRRL = PIN_I2C_SDA				/* SDA = 1 */
#define I2C_SDA_0()  PORT_I2C_SDA->BSRRH = PIN_I2C_SDA				/* SDA = 0 */

#define I2C_SDA_READ()  ((PORT_I2C_SDA->IDR & PIN_I2C_SDA) != 0)	/* 读SDA口线状态 */
#define I2C_SCL_READ()  ((PORT_I2C_SCL->IDR & PIN_I2C_SCL) != 0)	/* 读SCL口线状态 */

#define I2C_WR	0		/* 写控制bit */
#define I2C_RD	1		/* 读控制bit */

void BSP_MyIIC_Init(void);
void BSP_MyIIC_Start(void);
void BSP_MyIIC_Stop(void);
void BSP_MyIIC_SendByte(uint8_t _ucByte);
uint8_t BSP_MyIIC_ReadByte(void);
uint8_t BSP_MyIIC_WaitAck(void);
void BSP_MyIIC_Ack(void);
void BSP_MyIIC_NAck(void);
uint8_t BSP_MyIIC_CheckDevice(uint8_t _Address);

#endif
