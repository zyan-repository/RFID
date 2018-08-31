/*
***************************************************************************
*    模块：LF_125K 
*    描述：低频125K模块 应用测试
*    作者：Zyan
*    时间：2018.06.01
*    版本：
***************************************************************************
*/
#include "Test_emWin.h"

/*
***************************************************************************
*	函 数 名: Test_TouchPad
*	功能说明: TouchPad 应用函数
*	形    参: 无
*	返 回 值: 无
***************************************************************************
*/
void Test_TouchPad(void)
{
	while(1)
	{
		/* 建议使用 定时器扫描！*/
//		if(!BSP_TouchPad_Callback())
//		{
//			if(BSP_TouchPad_MoveValid(PENX,PENY,PENX_PRE,PENY_PRE))
//				printf("有效点位！\n");
//			else
//				printf("无效点位！\n");
//		}
	}
}

void Test_TFTLCD(void)
{
	uint16_t x,y;
	FONT_T tFont;			/* 定义一个字体结构体变量，用于设置字体参数 */

	/* 用于LCD界面标题 */
	//#define VER_INFO	"博创智联 魔法师创意竞赛平台 STM32开发板(V1.0h)"
	char VER_INFO[] =	"博创智联 魔法师创意竞赛平台 STM32开发板(V1.0h)";

	LCD_ClrScr(CL_WHITE);	/* 清屏，显示全白 */
	LCD_DrawLine(0,240,800,240,CL_BLACK);
	LCD_DrawLine(400,0,400,480,CL_BLACK);
	LCD_DrawBMP(0,0,240,400,(uint16_t*)0x0800F000);
	for(y=0;y<240;y++)
		for(x=0;x<400;x++)
		{
			LCD_PutPixel(x,y+240,LCD_GetPixel(x,y));
		}
//	LCD_DrawRect(450,60,100,120,CL_BLACK);
//	LCD_Fill_Rect(650,60,100,120,CL_BLACK);
	LCD_DrawRoundRect(450,60,100,120,15,CL_BLACK);
	LCD_FillRoundRect(650,60,100,120,15,CL_BLACK);
	LCD_DrawCircle(500,360,60,CL_BLACK);
	LCD_FillCircle(700,360,60,CL_BLACK);
	
	/* 设置字体参数 */
	{
		tFont.FontCode = FC_ST_16;		/* 字体代码 16点阵 */
		tFont.FrontColor = CL_WHITE;	/* 字体颜色 */
		tFont.BackColor = CL_RED;		/* 文字背景颜色 */
		tFont.Space = 0;				/* 文字间距，单位 = 像素 */
	}
	LCD_DispStr(400, 0, VER_INFO, &tFont);	/* 显示软件版本信息 */

	while(1)
	{
		Test_SRAM();
	}
}

void Test_SRAM(void)
{
	uint32_t Count,ErrCount=0;
	uint16_t Buffer,Temp;
	
	printf("SRAM Test\r\n");
	while (1)
	{
		/* 写入 0xFFFF 再读出并校验 */
		Buffer=0xFFFF;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0xFFFF)
				ErrCount++;
		}

		/* 写入 0xFF00 再读出并校验 */
		Buffer=0xFF00;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0xFF00)
				ErrCount++;
		}

		/* 写入 0x00FF 再读出并校验 */
		Buffer=0x00FF;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0x00FF)
				ErrCount++;
		}

		/* 写入 0x0000 再读出并校验 */
		Buffer=0x0000;
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_WriteBuffer(&Buffer,0x0000,1);
		}
		for(Count=0;Count<512*1024;Count++)
		{
			BSP_SRAM_ReadBuffer(&Temp,0x0000,1);
			if(Temp != 0x0000)
				ErrCount++;
		}
		
		/* 输出测试结果 */
		if(ErrCount)
			printf("Error!!! ErrCount=%d \r\n",ErrCount);
		else
			printf("Success!!! \r\n");
	}
}

