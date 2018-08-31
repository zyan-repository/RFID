/*
***************************************************************************
*    模块：Test_TouchPad 
*    描述：TouchPad 应用测试
*    作者：Huo
*    时间：2017.12.13
*    版本：UP-Magic-Version 1.0.0
***************************************************************************
*/
//#include "stm32f4xx.h"
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
#include "BSP_SRAM.h"
#include "MainTask.h"
#include "BSP_UART.h"
#include "BSP_ST16C554.h"

#include "Test_emWin.h"

/* 主函数 */
int main(void)
{
	RCC_AHB1PeriphClockCmd(RCC_AHB1Periph_CRC, ENABLE);
	/* 优先级分组设置为 4，不使用默认分组方案 */
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_4);
	BSP_DebugUART_Init(115200);
	BSP_Delay_Init();
	BSP_MyIIC_Init();
	BSP_GSL268x_Init();			//触摸屏初始化，本实验默认使用定时扫描
	BSP_PBus_Init();
	BSP_TFTLCD_Init();	/* 初始化显示器硬件(配置GPIO和FSMC,给LCD发送初始化指令) */
	BSP_SRAM_Init();
	BSP_UART_Init(115200);
	BSP_ST16C554_Init();
	BSP_Timer_Init(1*1000);	//定时 1*1000us = 1ms
		
	printf("         \t 触摸屏测试 演示 \r\n"
        "----------------------------------------------------\r\n"
        "\t打印触摸屏上的触点位置信息 \n\r"
        "----------------------------------------------------\n\r"
			);
	
	//Test_TouchPad();
	//Test_TFTLCD();
	
	MainTask();
}
