/*
***************************************************************************
*    模块：LF_125K 
*    描述：低频125K模块 应用测试
*    作者：Zyan
*    时间：2018.06.01
*    版本：
***************************************************************************
*/

#include "MainTask.h"

//volatile
volatile uint8_t drbuf[100]= {0x00};
volatile uint8_t rxflag = 0;
volatile uint8_t len = 0;
volatile int recSta = 0x01;
int ct = 0;

struct node {
    char ID[20];
    int num;
};

struct node card[100];

uint8_t same(char *a, char *b)
{
    uint8_t i;
    for(i = 0; i < 10; i++)
        if(a[i] != b[i])
            return 0;
    return 1;
}

char num_to_char(uint16_t n)
{
    char ans;
    if(n > 9) ans = n % 9 +'A' - 1;
    else ans = n + '0'-0;
    return ans;
}

char prtnum[10];

/*
*********************************************************************************************************
*                                         segger的logo
*********************************************************************************************************
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    NULL,                   0,                       0,  0,  800, 480, 0, 0, 0},
    { BUTTON_CreateIndirect,    "计数清零",             GUI_ID_BUTTON0,          660, 348, 101, 37, 0, 0},
    { BUTTON_CreateIndirect,    "卡号清除",             GUI_ID_BUTTON1,          660, 403, 101, 35, 0, 0},
    { TEXT_CreateIndirect, 			"标签信息", 						GUI_ID_TEXT0, 					 18, 20, 101, 41, 0, 0},
    { TEXT_CreateIndirect, 			"低频125KHZ测试界面", 	GUI_ID_TEXT1, 					 340, 30, 450, 50, 0, 0},
    { LISTVIEW_CreateIndirect,  "Listview",             GUI_ID_LISTVIEW0,           18, 77, 200, 341, 0, 0x0, 0 },

};

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    uint8_t i;
    SCROLLBAR_Handle hScroll;
    WM_HWIN hItem;
    WM_HWIN hWin = pMsg->hWin;

    WINDOW_SetBkColor(hWin,0xaacd66);

    GUI_UC_SetEncodeUTF8();
    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON0),&GUI_FontFONT3);

    BUTTON_SetFont(WM_GetDialogItem(hWin,GUI_ID_BUTTON1),&GUI_FontFONT3);


    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT0), GUI_WHITE);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT0),&GUI_FontFONT1);

    TEXT_SetTextColor(WM_GetDialogItem(hWin,GUI_ID_TEXT1), GUI_BLUE);
    TEXT_SetFont(WM_GetDialogItem(hWin,GUI_ID_TEXT1),&GUI_FontFONT2);

    HEADER_Handle hHeader;

    hItem = WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0);
    hHeader = LISTVIEW_GetHeader(WM_GetDialogItem(pMsg->hWin, GUI_ID_LISTVIEW0));
    HEADER_SetFont(hHeader, &GUI_FontFONT3);
    LISTVIEW_SetHeaderHeight(hItem, 40);
    LISTVIEW_SetRowHeight(hItem, 40);
    LISTVIEW_AddColumn(hItem, 30, "", GUI_TA_HCENTER | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 100, "卡号", GUI_TA_LEFT | GUI_TA_VCENTER);
    LISTVIEW_AddColumn(hItem, 55, "计数", GUI_TA_LEFT | GUI_TA_VCENTER);
		LISTVIEW_AddColumn(hItem, 15, "", GUI_TA_HCENTER | GUI_TA_VCENTER);
		HEADER_SetTextAlign(hHeader, 1, GUI_TA_HCENTER | GUI_TA_VCENTER);
		HEADER_SetTextAlign(hHeader, 2, GUI_TA_HCENTER | GUI_TA_VCENTER);
    for(i = 0; i < 20; i++)
        LISTVIEW_AddRow(hItem, NULL);
    LISTVIEW_SetGridVis(hItem, 1);
    //LISTVIEW_SetAutoScrollV(hItem, 1);
    hScroll = SCROLLBAR_CreateAttached(hItem, GUI_ID_VSCROLL);
    SCROLLBAR_SetWidth(hScroll, 15);
		LISTVIEW_SetFont(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), &GUI_Font16B_ASCII);
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbCallback(WM_MESSAGE * pMsg)
{
    uint8_t i;
    int NCode, Id;
    WM_HWIN hWin = pMsg->hWin;
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
                for(i = 0; i < ct; i++)
                {
                    if(card[i].num) card[i].num = 0;
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, i, "0");
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
                for(i = 0; i < ct; i++)
                {
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 0, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 1, i, "");
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, i, "");
                    card[i].num = 0;
                }
                ct = 0;
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
    uint8_t i, cnt, flg;
    char temp[15];
    WM_HWIN hWin;
    GUI_Init();
    WM_SetDesktopColor(GUI_BLACK);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    hWin = CreateFramewin();
    for(i = 0; i < 100; i++)
        card[i].num = 0;
    ct = 0;
    while(1)
    {
        if(rxflag)                                  //读到新卡
        {
            cnt = 0;
            flg = 0;
            for(i = 0; i < 5; i++)
            {
                temp[cnt++] = num_to_char(drbuf[i]/16);
                temp[cnt++] = num_to_char(drbuf[i]%16);
            }
            temp[10] = '\0';
            for(i = 0; i < ct; i++)                 //遍历刷出来的所有卡，出现卡号相同的在原基础上累加计数
            {
                if(same(temp, card[i].ID))
                {
                    card[i].num++;
										sprintf(prtnum, "%d", card[i].num);
                    LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, i, prtnum);
                    flg = 1;
                }
            }
            if(!flg)                                //遍历所有卡没有出现相同卡号的就新增一行
            {
                strcpy(card[ct].ID, temp);
                card[ct].num++;
								sprintf(prtnum, "%d", ct+1);
                LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 0, ct, prtnum);
                LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 1, ct, card[ct].ID);
								sprintf(prtnum, "%d", card[ct].num);
                LISTVIEW_SetItemText(WM_GetDialogItem(hWin,GUI_ID_LISTVIEW0), 2, ct, prtnum);
                ct++;
            }
            rxflag = 0;
        }
        GUI_Delay(1);                              
    }
}
