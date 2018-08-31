#include "MainTask.h"

#define ID_HEADER_0 GUI_ID_USER+1
#define ID_WINDOW_0 GUI_ID_USER+2

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

/*
*********************************************************************************************************
*                                         segger的logo
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,   NULL,                      ID_WINDOW_0,            0,  0,  800, 480, 0, 0, 0},
    { BUTTON_CreateIndirect,    "打开",                   GUI_ID_BUTTON0,          600, 200, 150, 50, 0, 0},
    { BUTTON_CreateIndirect,    "关闭",                   GUI_ID_BUTTON1,          600, 300, 150, 50, 0, 0},
    { TEXT_CreateIndirect, 			"模块", 						    GUI_ID_TEXT0, 					50, 100, 101, 41, 0, 0},
    { TEXT_CreateIndirect, 			"执行器测试界面", 	        GUI_ID_TEXT1, 					480, 30, 400, 50, 0, 0},
    { TEXT_CreateIndirect, 			"地铁通道界面", 						GUI_ID_TEXT2, 					50, 220, 250, 100, 0, 250}
};

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    WINDOW_SetBkColor(hWin,0xaacd66);

    GUI_UC_SetEncodeUTF8();

    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_FontFontSong);


    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_WHITE);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontFontSong);

    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_BLUE);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontFontSong);

    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT2), GUI_BLACK);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT2),&GUI_FontFontSong);
}

char tran(uint16_t n)
{
    char ans;
    if(n > 9)
    {
        ans = n % 9 +'A' - 1;
    }
    else
    {
        ans = n + '0'-0;
    }
    return ans;
}

uint8_t zhuangtai[15] = {0xfe, 0x03, 0xd1, 0x00, 0x00, 0x00, 0x0a};
uint8_t dakai[15] = {0xfe, 0x03, 0x52, 0x02, 0x01, 0x00, 0x0a};
uint8_t guanbi[15] = {0xfe, 0x03, 0x52, 0x02, 0x00, 0x00, 0x0a};

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    int NCode, Id;
    uint8_t i;
    switch (pMsg->MsgId)
    {
    case WM_INIT_DIALOG:
        InitDialog(pMsg);
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch (Id)
        {
        case GUI_ID_BUTTON0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i  = 0; i < 7; i++)
                {
                    BSP_ST16C554_CS2B_Write(dakai[i]);
                    BSP_Delay_ms(1);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case GUI_ID_BUTTON1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i  = 0; i < 7; i++)
                {
                    BSP_ST16C554_CS2B_Write(guanbi[i]);
                    BSP_Delay_ms(1);
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        }
        break;
    default:
        WM_DefaultProc(pMsg);
    }
}

WM_HWIN CreateFramewin(void)
{
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbCallback, WM_HBKWIN, 0, 0);
    return hWin;
}

/*********************************************************************
*
*       MainTask
*
**********************************************************************
*/
void MainTask(void)
{
    GUI_Init();
    WM_SetDesktopColor(GUI_BLACK);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    CreateFramewin();
    while(1)
    {
        GUI_Delay(1);
    }
}
