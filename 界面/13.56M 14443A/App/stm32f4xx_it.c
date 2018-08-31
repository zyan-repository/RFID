/**
  ******************************************************************************
  * @file    Project/STM32F4xx_StdPeriph_Templates/stm32f4xx_it.c
  * @author  MCD Application Team
  * @version V1.4.0
  * @date    04-August-2014
  * @brief   Main Interrupt Service Routines.
  *          This file provides template for all exceptions handler and
  *          peripherals interrupt service routine.
  ******************************************************************************
  * @attention
  *
  * <h2><center>&copy; COPYRIGHT 2014 STMicroelectronics</center></h2>
  *
  * Licensed under MCD-ST Liberty SW License Agreement V2, (the "License");
  * You may not use this file except in compliance with the License.
  * You may obtain a copy of the License at:
  *
  *        http://www.st.com/software_license_agreement_liberty_v2
  *
  * Unless required by applicable law or agreed to in writing, software
  * distributed under the License is distributed on an "AS IS" BASIS,
  * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
  * See the License for the specific language governing permissions and
  * limitations under the License.
  *
  ******************************************************************************
  */

/* Includes ------------------------------------------------------------------*/
#include "stm32f4xx_it.h"

/** @addtogroup Template_Project
  * @{
  */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/
/* Private variables ---------------------------------------------------------*/
/* Private function prototypes -----------------------------------------------*/
/* Private functions ---------------------------------------------------------*/

/******************************************************************************/
/*            Cortex-M4 Processor Exceptions Handlers                         */
/******************************************************************************/

/**
  * @brief  This function handles NMI exception.
  * @param  None
  * @retval None
  */
void NMI_Handler(void)
{
}

/**
  * @brief  This function handles Hard Fault exception.
  * @param  None
  * @retval None
  */
void HardFault_Handler(void)
{
    /* Go to infinite loop when Hard Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Memory Manage exception.
  * @param  None
  * @retval None
  */
void MemManage_Handler(void)
{
    /* Go to infinite loop when Memory Manage exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Bus Fault exception.
  * @param  None
  * @retval None
  */
void BusFault_Handler(void)
{
    /* Go to infinite loop when Bus Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles Usage Fault exception.
  * @param  None
  * @retval None
  */
void UsageFault_Handler(void)
{
    /* Go to infinite loop when Usage Fault exception occurs */
    while (1)
    {
    }
}

/**
  * @brief  This function handles SVCall exception.
  * @param  None
  * @retval None
  */
void SVC_Handler(void)
{
}

/**
  * @brief  This function handles Debug Monitor exception.
  * @param  None
  * @retval None
  */
void DebugMon_Handler(void)
{
}

/**
  * @brief  This function handles PendSVC exception.
  * @param  None
  * @retval None
  */
void PendSV_Handler(void)
{
}

/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{

}

/******************************************************************************/
/*                 STM32F4xx Peripherals Interrupt Handlers                   */
/*  Add here the Interrupt Handler for the used peripheral(s) (PPP), for the  */
/*  available peripheral interrupt handler's name please refer to the startup */
/*  file (startup_stm32f4xx.s).                                               */
/******************************************************************************/

/**
  * @brief  This function handles PPP interrupt request.
  * @param  None
  * @retval None
  */
/*void PPP_IRQHandler(void)
{
}*/

/*
***************************************************************************
*	函 数 名: USART1_IRQHandler
*	功能说明: STM32 USART1 中断服务函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void USART1_IRQHandler(void)
{
    uint16_t Res;
    if(USART_GetITStatus(USART1, USART_IT_RXNE) != RESET)  //接收中断
    {
        Res = USART_ReceiveData(USART1);	//读取接收到的数据
        QueueMemDataInsert((uint8_t)Res);
    }
}

//void USART3_IRQHandler(void)
//{
//	uint8_t clear = clear;
//	if(USART_GetITStatus(USART3, USART_IT_RXNE) != RESET)  //接收中断
//	{
//		drbuf[rxcounter++] = USART3->DR;
//	}
//	else if(USART_GetITStatus(USART3,USART_IT_IDLE) != RESET)
//	{
//		clear = USART3->DR;//先读取接受数据寄存器
//    clear = USART3->SR;//再读取发送数据寄存器解可以清除空闲中断标志位
//    rxflag = 1;
//	}
//}

extern volatile GUI_TIMER_TIME OS_TimeMS;
GUI_PID_STATE State;

/*
********************************************************************************
*	函 数 名: TIM2_IRQHandler
*	功能说明: STM32 TIM2中断服务函数
*	形    参: 无
*	返 回 值: 无
********************************************************************************
*/
void TIM2_IRQHandler(void)
{
//    static u8 count = 0;
    if (TIM_GetITStatus(TIM2, TIM_IT_Update) != RESET)
    {
        OS_TimeMS++;
//        count++;
//        if(count == 10) {
//            count = 0;
            if(!BSP_TouchPad_Callback()) {
                State.x = PENX;
                State.y = PENY;
                State.Pressed = 1;
                GUI_PID_StoreState(&State);
            }
            else {
                State.x = -1;
                State.y = -1;
                State.Pressed = 0;
                GUI_PID_StoreState(&State);
//            }
        }
    }
    TIM_ClearITPendingBit(TIM2, TIM_IT_Update);  //清除TIMx更新中断标志
}

/*
***************************************************************************
*	函 数 名: EXTI0_IRQHandler
*	功能说明: STM32 EXTI0 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void EXTI0_IRQHandler(void)
{
    BSP_ST16C554_CS2C_ISR();
}

/*
***************************************************************************
*	函 数 名: EXTI1_IRQHandler
*	功能说明: STM32 EXTI1 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void EXTI1_IRQHandler(void)
{
    BSP_ST16C554_CS2A_ISR();
}

/*
***************************************************************************
*	函 数 名: EXTI4_IRQHandler
*	功能说明: STM32 EXTI4 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void EXTI4_IRQHandler(void)
{
    BSP_ST16C554_CS2D_ISR();
}

/*
***************************************************************************
*	函 数 名: EXTI9_5_IRQHandler
*	功能说明: STM32 EXTI9_5 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void EXTI9_5_IRQHandler(void)
{
    BSP_ST16C554_CS1A_ISR();
    BSP_ST16C554_CS1B_ISR();
    BSP_ST16C554_CS1C_ISR();
    BSP_ST16C554_CS1D_ISR();
}

/*
***************************************************************************
*	函 数 名: EXTI15_10_IRQHandler
*	功能说明: STM32 EXTI15_10 中断服务功能函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void EXTI15_10_IRQHandler(void)
{
    BSP_ST16C554_CS2B_ISR();
}
/**
  * @}
  */


/************************ (C) COPYRIGHT STMicroelectronics *****END OF FILE****/
