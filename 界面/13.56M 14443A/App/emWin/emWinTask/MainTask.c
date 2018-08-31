/*
******************************************************************
**                      uCGUIBuilder                            **
**                  Version:   4.0.0.0                          **
**                     2012 / 04                               **
**                   CpoyRight to: wyl                          **
**              Email:ucguibuilder@163.com                        **
**          This text was Created by uCGUIBuilder               **
******************************************************************/

#include <stddef.h>
#include "GUI.h"
#include "DIALOG.h"

#include "WM.h"
#include "BUTTON.h"
#include "CHECKBOX.h"
#include "DROPDOWN.h"
#include "EDIT.h"
#include "FRAMEWIN.h"
#include "LISTBOX.h"
#include "MULTIEDIT.h"
#include "RADIO.h"
#include "SLIDER.h"
#include "TEXT.h"
#include "PROGBAR.h"
#include "SCROLLBAR.h"
#include "LISTVIEW.h"
#include "BSP_ST16C554.h"
#include "BSP_Delay.h"
#include "MESSAGEBOX.h"

extern GUI_CONST_STORAGE GUI_FONT GUI_FontFontSong;

uint8_t chuankou[20] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x08, 0x01, 0x41, 0x48};
uint8_t xunka[20] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x01, 0x02, 0x52, 0x51};
uint8_t fangchongzhuang[20] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x02, 0x02, 0x04, 0x04};
uint8_t xuanzemingling[20] = {0xAA, 0xBB, 0x09, 0x00, 0x00, 0x00, 0x03, 0x02};
uint8_t xiumianxunka[20] = {0xAA, 0xBB, 0x06, 0x00, 0x00, 0x00, 0x01, 0x02, 0x26, 0x25};
uint8_t xiumian[20] = {0xaa, 0xbb, 0x05, 0x00, 0x00, 0x00, 0x04, 0x02, 0x06};
uint8_t huanxing[20] = {0xaa, 0xbb, 0x06, 0x00, 0x00, 0x00, 0x0c, 0x01, 0x00, 0x0d};
uint8_t shanqu[20] = {0xaa, 0xbb, 0x0d, 0x00, 0x00, 0x00, 0x07, 0x02, 0x61, 0x00, 0xff, 0xff, 0xff, 0xff, 0xff, 0xff};
uint8_t kuai[20] = {0xaa, 0xbb, 0x0a, 0x00, 0x00, 0x00, 0x0a, 0x02};
uint8_t dukakuai[20] = {0xaa, 0xbb, 0x06, 0x00, 0x00, 0x00, 0x08, 0x02};
uint8_t xiekakuai[20] = {0xaa, 0xbb, 0x16, 0x00, 0x00, 0x00, 0x09, 0x02};

volatile uint8_t drbuf[100]= {0x00};
volatile uint8_t rxflag = 0;
volatile uint8_t len = 0;
volatile uint8_t dataLen = 0;
volatile int recSta = 0x01;
int flag1 = 0;
int flag2 = 0;
int flag3 = 0;
int flag4 = 0;
int flag5 = 0;
int flag6 = 0;//初始化
int flag7 = 0;//操作窗口2
int flag8 = 0;//操作窗口1
int flag9 = 0;//读余额
int flag10 = 0;//充值
int flag11 = 0;//扣款
char key[20];
char jine16[20], jine10[20];
int xianshijine = 0;//显示的金额
int neicunjine = 0;//存入内存的金额
uint16_t Data[10] = {0};//发送的16进制数据

char tran(uint16_t n)
{
    char ans;
    if(n > 9) ans = n % 9 +'A' - 1;
    else ans = n + '0' - 0;
    return ans;
}

uint8_t Xunka()
{
    uint8_t i, cnt, kahao[20], status;
    status = 0;
    for(i = 0; i < 10; i++)//选择14443
    {
        BSP_ST16C554_CS1D_Write(chuankou[i]);
        BSP_Delay_ms(1);
    }
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)
        status = 1;
    else status = 2;
    rxflag = 0;
    if(status == 1)
    {
        for(i = 0; i < 10; i++)//选卡
        {
            BSP_ST16C554_CS1D_Write(xunka[i]);
            BSP_Delay_ms(1);
        }
        BSP_Delay_ms(30);
        if(rxflag && drbuf[8] == 0x00) status = 1;
        else status = 2;
        rxflag = 0;
        if(status == 1)
        {
            for(i = 0; i < 10; i++)
            {
                BSP_ST16C554_CS1D_Write(fangchongzhuang[i]);
                BSP_Delay_ms(1);
            }

            BSP_Delay_ms(30);
            if(rxflag && drbuf[8] == 0x00) status = 1;
            else status = 2;
            rxflag = 0;
            if(status == 1)
            {
                cnt = 0;
                for(i = 9; i < 3 + drbuf[2]; i++)//9+drbuf[2]-6
                {
                    kahao[cnt++] = drbuf[i];
                }

                for(i = 0; i < 8; i++)//选择命令
                {
                    BSP_ST16C554_CS1D_Write(xuanzemingling[i]);
                    BSP_Delay_ms(1);
                }
                cnt = xuanzemingling[4];
                for(i = 5; i < 8; i++) cnt ^= xuanzemingling[i];
                for(i = 9; i < 3+drbuf[2]; i++)
                {
                    BSP_ST16C554_CS1D_Write(kahao[i-9]);
                    cnt ^= kahao[i-9];
                    BSP_Delay_ms(1);
                }
                BSP_ST16C554_CS1D_Write(cnt);
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) status = 1;
                else status = 2;
                rxflag = 0;
                if(status == 1) return 1;
            }
        }
    }
    return 0;
}

uint8_t Shanqu(int sq)
{
    uint8_t i, cnt;
    int res;
    cnt = 0;
    for(i = 0; i < 16; i++)
    {
        if(i == 9)
        {
            res = sq * 4;
            BSP_ST16C554_CS1D_Write(res);
        }
        else
        {
            BSP_ST16C554_CS1D_Write(shanqu[i]);
            res = shanqu[i];
        }
        if(i >= 4 && i <= 15) cnt ^= res;
        BSP_Delay_ms(1);
    }
    BSP_ST16C554_CS1D_Write(cnt);
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)
    {
        rxflag = 0;
        return 1;
    }
    rxflag = 0;
    return 0;
}

uint8_t DukaKuai(int sq, int k)
{
    uint8_t i, cnt;
    int res;
    cnt = 0;
    for(i = 0; i < 8; i++)
    {
        BSP_ST16C554_CS1D_Write(dukakuai[i]);
        if(i >= 4 && i <= 7) cnt ^= dukakuai[i];
        BSP_Delay_ms(1);
    }
    res = sq * 4 + k;
    BSP_ST16C554_CS1D_Write(res);
    cnt ^= res;
    BSP_Delay_ms(1);
    BSP_ST16C554_CS1D_Write(cnt);
    BSP_Delay_ms(30);
    if(rxflag && drbuf[8] == 0x00)
    {
        rxflag = 0;
        return 1;
    }
    rxflag = 0;
    return 0;
}

uint8_t Key(int sq)//扇区 默认keyB
{
    uint8_t i, cnt;
    char pt[40];
    if(!Xunka())
        return 0;
    if(!Shanqu(sq))
        return 0;
    if(!DukaKuai(sq, 3))
        return 0;
    cnt = 0;
    for(i = 19; i < 25; i++)
    {
        pt[cnt++] = tran(drbuf[i]/16);
        pt[cnt++] = tran(drbuf[i]%16);
    }
    for(i = 0; i < 12; i++)
        if(pt[i] != key[i])
            return 0;
    for(i  = 0; i < 12; i++)
        key[i] = '\0';
    return 1;
}

uint16_t trannum(char c)
{
    if(c >= 'A' && c <= 'F')
        return 10 + c - 'A';
    else
        return c - '0';
}

static WM_HWIN _CreateMessageBox(const char * sMessage, const char * sCaption, int Flags, const GUI_FONT * pFont)
{
    WM_HWIN hWin;
    WM_HWIN hItem;
    GUI_RECT Rect;
    hWin = MESSAGEBOX_Create(sMessage, sCaption, Flags);
//
// Change font of message box window
//
    FRAMEWIN_SetFont(hWin, pFont);
//
// Adjust size
//
    WM_GetWindowRectEx(hWin, &Rect);
    WM_SetWindowPos(hWin, Rect.x0 - 15,
                    Rect.y0 - 40,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 30);
//
// Change font of button widget
//
    hItem = WM_GetDialogItem(hWin, GUI_ID_OK);
    BUTTON_SetFont(hItem, pFont);
//
// Adjust size of button widget
//
    WM_GetWindowRectEx(hItem, &Rect);
    WM_SetWindowPos(hItem, Rect.x0,
                    Rect.y0 + 10,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 5);
//
// Change font of text widget
//
    hItem = WM_GetDialogItem(hWin, GUI_ID_TEXT0);
    TEXT_SetFont(hItem, pFont);
//
// Adjust size text widget
//
    WM_GetWindowRectEx(hItem, &Rect);
    WM_SetWindowPos(hItem, Rect.x0,
                    Rect.y0,
                    Rect.x1 - Rect.x0 + 1 + 30,
                    Rect.y1 - Rect.y0 + 1 + 12);
    return hWin;
}

/*
************************************************************************************************
* 添加到多页控件第一个页面窗口的对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
#define ID_WINDOW_01 (GUI_ID_USER + 0x41)
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage1[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_01,          0, 0, 780, 210, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "块号：",             ID_TEXT_3,             10, 70, 50, 40, 0, 0},
    { TEXT_CreateIndirect,      "扇区号：",           ID_TEXT_4,             10, 20, 70, 40, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_1,          75, 70,100, 80, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_0,          75, 20, 100, 80, 0, 0},
    { BUTTON_CreateIndirect,    "初始化",         		 ID_BUTTON_chushihua,    291, 140, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "充值",          		 ID_BUTTON_chongzhi,     421, 140, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "扣款",          		 ID_BUTTON_koukuan,      547, 140, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "读余额",          	 ID_BUTTON_duyue,        675, 140, 100, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_2,              291, 90, 220, 40, 0, 220},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_3,              527, 90, 220, 40, 0, 220},
    { TEXT_CreateIndirect,      "金额（十进制）",     ID_TEXT_5,              291, 50, 150, 40, 0, 0},
    { TEXT_CreateIndirect,      "金额（十六进制）",   ID_TEXT_6,              527, 50, 150, 40, 0, 0},
};

/*********************************************************************
*
* Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
// USER END
/*********************************************************************
*
* _cbDialog
*/

int shanqu1 = 0;
int kuai1 = 1;

static void _cbDialogPage1(WM_MESSAGE * pMsg) {
    uint8_t i, cnt;
    uint16_t res;
    WM_HWIN hItem;
    char pt[40];
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        //ID_TEXT_3
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_3), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_3), &GUI_FontFontSong);
        //
        //ID_TEXT_4
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_4), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_4), &GUI_FontFontSong);

        //ID_DROPDOWN_0
        //
//				DROPDOWN_SetAutoScroll(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 1);
//				DROPDOWN_SetScrollbarWidth(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 16);
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_0), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 85);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "3");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "4");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "5");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "6");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "7");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "8");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "9");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "10");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "11");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "12");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "13");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "14");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "15");

        //
        //ID_DROPDOWN_1
        //
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_1), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_1), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), 75);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "2");
        DROPDOWN_SetSel(WM_GetDialogItem(hItem,ID_DROPDOWN_1), 1);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_chushihua),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_chongzhi),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_koukuan),&GUI_FontFontSong);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_duyue),&GUI_FontFontSong);

        //
        //ID_EDIT_2
        //
        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_2),GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_2),&GUI_Font16B_ASCII);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_2), 100, 1);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_2), 9);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_2),"");
        //
        //ID_EDIT_3
        //
        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_3),GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_3),&GUI_Font16B_ASCII);
        EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_3), 0);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_3),"");

        //
        //ID_TEXT_5
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_5),GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_5),&GUI_FontFontSong);
        //
        //ID_TEXT_6
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_6),GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_6),&GUI_FontFontSong);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_DROPDOWN_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                shanqu1 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_0));
                break;
            }
            break;
        case ID_DROPDOWN_1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                kuai1 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_1));
                break;
            }
            break;
        case ID_BUTTON_chushihua:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu1))
                {
                    flag6 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag6 = 2;
                    break;
                }
                if(!Shanqu(shanqu1))
                {
                    flag6 = 2;
                    break;
                }
                cnt = 0;
                for(i = 0; i < 8; i++)
                {
                    BSP_ST16C554_CS1D_Write(kuai[i]);
                    if(i >= 4 && i <= 7) cnt ^= kuai[i];
                    BSP_Delay_ms(1);
                }
                res = shanqu1 * 4 + kuai1 ;
                BSP_ST16C554_CS1D_Write(res);
                cnt ^= res;
                BSP_Delay_ms(1);
                //发送4个16进制数 数据位（倒序）
                for(i = 0; i < 4; i++)
                {
                    res = Data[i*2+1] + Data[i*2] * 16;
                    BSP_ST16C554_CS1D_Write(res);
                    cnt ^= res;
                    BSP_Delay_ms(1);
                    if(res == 0xaa)
                    {
                        BSP_ST16C554_CS1D_Write(0x00);
                        BSP_Delay_ms(1);
                    }
                }
                //
                BSP_ST16C554_CS1D_Write(cnt);
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag6 = 1;
                else flag6 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_chongzhi:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu1))
                {
                    flag10 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag10 = 2;
                    break;
                }
                if(!Shanqu(shanqu1))
                {
                    flag10 = 2;
                    break;
                }
                if(!DukaKuai(shanqu1, kuai1))
                {
                    flag10 = 2;
                    break;
                }
                cnt = 0;
                for(i = 9; i < 13; i++)//25
                {
                    pt[cnt++] = tran(drbuf[i]/16);
                    pt[cnt++] = tran(drbuf[i]%16);
                }
                pt[cnt] = '\0';
                neicunjine = trannum(pt[1]) + trannum(pt[0]) * 16 + trannum(pt[3]) * 16 * 16 + trannum(pt[2]) * 16 * 16 * 16
                             + trannum(pt[5]) * 16 * 16 * 16 * 16 + trannum(pt[4]) * 16 * 16 * 16 * 16 * 16
                             + trannum(pt[7]) * 16 * 16 * 16 * 16 * 16 * 16 + trannum(pt[6]) * 16 * 16 * 16 * 16 * 16 * 16 * 16;
                neicunjine += xianshijine;
                if(!Shanqu(shanqu1))
                {
                    flag10 = 2;
                    break;
                }
                cnt = 0;
                for(i = 0; i < 8; i++)
                {
                    BSP_ST16C554_CS1D_Write(kuai[i]);
                    if(i >= 4 && i <= 7) cnt ^= kuai[i];
                    BSP_Delay_ms(1);
                }
                res = shanqu1 * 4 + kuai1 ;
                BSP_ST16C554_CS1D_Write(res);
                cnt ^= res;
                BSP_Delay_ms(1);
                //发送4个16进制数 数据位（倒序）
                for(i = 0; i < 8; i++) Data[i] = 0;
                i = 0;
                while(neicunjine)
                {
                    Data[i+1] = neicunjine % 16;
                    neicunjine /= 16;
                    Data[i] = neicunjine % 16;
                    neicunjine /= 16;
                    i += 2;
                }
                for(i = 0; i < 4; i++)
                {
                    res = Data[i*2+1] + Data[i*2] * 16;
                    BSP_ST16C554_CS1D_Write(res);
                    cnt ^= res;
                    BSP_Delay_ms(1);
                    if(res == 0xaa)
                    {
                        BSP_ST16C554_CS1D_Write(0x00);
                        BSP_Delay_ms(1);
                    }
                }
                //
                BSP_ST16C554_CS1D_Write(cnt); 
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag10 = 1;
                else flag10 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_koukuan:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu1))
                {
                    flag11 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag11 = 2;
                    break;
                }
                if(!Shanqu(shanqu1))
                {
                    flag11 = 2;
                    break;
                }
                if(!DukaKuai(shanqu1, kuai1))
                {
                    flag11 = 2;
                    break;
                }
                cnt = 0;
                for(i = 9; i < 13; i++)//25
                {
                    pt[cnt++] = tran(drbuf[i]/16);
                    pt[cnt++] = tran(drbuf[i]%16);
                }
                pt[cnt] = '\0';
                neicunjine = trannum(pt[1]) + trannum(pt[0]) * 16 + trannum(pt[3]) * 16 * 16 + trannum(pt[2]) * 16 * 16 * 16
                             + trannum(pt[5]) * 16 * 16 * 16 * 16 + trannum(pt[4]) * 16 * 16 * 16 * 16 * 16
                             + trannum(pt[7]) * 16 * 16 * 16 * 16 * 16 * 16 + trannum(pt[6]) * 16 * 16 * 16 * 16 * 16 * 16 * 16;
                neicunjine -= xianshijine;
                if(!Shanqu(shanqu1))
                {
                    flag11 = 2;
                    break;
                }
                cnt = 0;
                for(i = 0; i < 8; i++)
                {
                    BSP_ST16C554_CS1D_Write(kuai[i]);
                    if(i >= 4 && i <= 7) cnt ^= kuai[i];
                    BSP_Delay_ms(1);
                }
                res = shanqu1 * 4 + kuai1 ;
                BSP_ST16C554_CS1D_Write(res);
                cnt ^= res;
                BSP_Delay_ms(1);
                //发送4个16进制数 数据位（倒序）
                for(i = 0; i < 8; i++) Data[i] = 0;
                i = 0;
                while(neicunjine)
                {
                    Data[i+1] = neicunjine % 16;
                    neicunjine /= 16;
                    Data[i] = neicunjine % 16;
                    neicunjine /= 16;
                    i += 2;
                }
                for(i = 0; i < 4; i++)
                {
                    res = Data[i*2+1] + Data[i*2] * 16;
                    BSP_ST16C554_CS1D_Write(res);
                    cnt ^= res;
                    BSP_Delay_ms(1);
                    if(res == 0xaa)
                    {
                        BSP_ST16C554_CS1D_Write(0x00);
                        BSP_Delay_ms(1);
                    }
                }
                //
                BSP_ST16C554_CS1D_Write(cnt);
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag11 = 1;
                else flag11 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_duyue:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu1))
                {
                    flag9 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag9 = 2;
                    break;
                }
                if(!Shanqu(shanqu1))
                {
                    flag9 = 2;
                    break;
                }
                if(!DukaKuai(shanqu1, kuai1))
                {
                    flag9 = 2;
                    break;
                }
                flag9 = 1;
                cnt = 0;
                for(i = 9; i < 13; i++)//25
                {
                    pt[cnt++] = tran(drbuf[i]/16);
                    pt[cnt++] = tran(drbuf[i]%16);
                }
                pt[cnt] = '\0';
                EDIT_SetText(WM_GetDialogItem(hItem, ID_EDIT_3), pt);
                neicunjine = trannum(pt[1]) + trannum(pt[0]) * 16 + trannum(pt[3]) * 16 * 16 + trannum(pt[2]) * 16 * 16 * 16
                             + trannum(pt[5]) * 16 * 16 * 16 * 16 + trannum(pt[4]) * 16 * 16 * 16 * 16 * 16
                             + trannum(pt[7]) * 16 * 16 * 16 * 16 * 16 * 16 + trannum(pt[6]) * 16 * 16 * 16 * 16 * 16 * 16 * 16;
                sprintf(jine10, "%d", neicunjine);
                EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_2),jine10);
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
// USER START (Optionally insert additional code for further Ids)
// USER END
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/*********************************************************************
*
* CreateWindowPage1
*/
WM_HWIN CreateWindowPage1(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage1, GUI_COUNTOF(_aDialogCreatePage1), _cbDialogPage1,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*
************************************************************************************************
* 添加到多页控件第二个页面窗口的对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
#define ID_WINDOW_02 (GUI_ID_USER + 0x43)
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage2[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_02,          0, 0, 780, 210, 0, 0x0, 0},
    { TEXT_CreateIndirect,      "块号：",             ID_TEXT_3,            10, 70, 50, 40, 0, 0},
    { TEXT_CreateIndirect,      "扇区号：",           ID_TEXT_4,            10, 20, 70, 40, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_1,         75, 70, 100, 80, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_0,         75, 20, 100, 80, 0, 0},
    { TEXT_CreateIndirect,       "块0",               ID_TEXT_7,            190, 10, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块1",               ID_TEXT_8,             190, 50, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块2",               ID_TEXT_9,             190, 90, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块3",               ID_TEXT_10,            190, 130, 30, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_8,             220, 10, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_9,             220, 50, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_10,            220, 90, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_11,            220, 130, 540, 38, 0, 540},
};
/*********************************************************************
*
* Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
// USER END

int shanqu2 = 0;
int kuai2 = 0;

/*********************************************************************
*
* _cbDialog
*/
static void _cbDialogPage2(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        //ID_TEXT_3
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_3), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_3), &GUI_FontFontSong);
        //
        //ID_TEXT_4
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_4), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_4), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_7), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_7), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_8), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_8), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_9), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_9), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_10), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_10), &GUI_FontFontSong);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_8), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_8), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_8), 32);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_8), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_8), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_9), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_9), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_9), 32);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_9), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_9), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_10), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_10), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_10), 32);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_10), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_10), "");

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_11), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_11), &GUI_Font16B_ASCII);
        EDIT_SetMaxLen(WM_GetDialogItem(hItem,ID_EDIT_11), 32);
        EDIT_EnableBlink(WM_GetDialogItem(hItem,ID_EDIT_11), 100, 1);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_11), "");

        //
        //
        //ID_DROPDOWN_0
        //
//        DROPDOWN_SetAutoScroll(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 1);
//				DROPDOWN_SetScrollbarWidth(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 16);
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_0), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 85);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "3");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "4");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "5");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "6");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "7");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "8");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "9");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "10");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "11");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "12");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "13");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "14");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "15");
        //
        //ID_DROPDOWN_1
        //
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_1), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_1),34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), 75);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "3");

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_DROPDOWN_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                shanqu2 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_0));
                break;
            }
            break;
        case ID_DROPDOWN_1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                kuai2 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_1));
                break;
            }
            break;
// USER START (Optionally insert additional code for further Ids)
// USER END
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/*********************************************************************
*
* CreateWindowPage2
*/
WM_HWIN CreateWindowPage2(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage2, GUI_COUNTOF(_aDialogCreatePage2), _cbDialogPage2,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*
************************************************************************************************
* 添加到多页控件第三个页面窗口的对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
#define ID_WINDOW_03 (GUI_ID_USER + 0x44)
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePage3[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_02,          0, 0, 780, 210, 0, 0x0, 0 },
    { TEXT_CreateIndirect,      "块号：",             ID_TEXT_3,            10, 70, 50, 40, 0,0},
    { TEXT_CreateIndirect,      "扇区号：",           ID_TEXT_4,            10, 20, 70, 40, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_1,         75, 70, 100, 80, 0, 0},
    { DROPDOWN_CreateIndirect,   NULL,               ID_DROPDOWN_0,         75, 20, 100, 80, 0, 0},
    { TEXT_CreateIndirect,       "块0",               ID_TEXT_7,            190,10, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块1",               ID_TEXT_8,             190, 50, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块2",               ID_TEXT_9,             190, 90, 30, 40, 0, 0},
    { TEXT_CreateIndirect,      "块3",               ID_TEXT_10,            190, 130, 30, 40, 0, 0},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_4,              220, 10, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_5,              220, 50, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_6,              220, 90, 540, 38, 0, 540},
    { EDIT_CreateIndirect,       NULL,               ID_EDIT_7,              220, 130, 540, 38, 0, 540},
};
/*********************************************************************
*
* Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
// USER END

int shanqu3 = 0;
int kuai3 = 0;

/*********************************************************************
*
* _cbDialog
*/
static void _cbDialogPage3(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        //
        //ID_TEXT_3
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_3), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_3), &GUI_FontFontSong);
        //
        //ID_TEXT_4
        //
        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_4), GUI_TA_VCENTER|GUI_TA_CENTER);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_4), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_7), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_7), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_8), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_8), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_9), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_9), &GUI_FontFontSong);

        TEXT_SetTextAlign(WM_GetDialogItem(hItem,ID_TEXT_10), GUI_TA_VCENTER|GUI_TA_LEFT);
        TEXT_SetFont(WM_GetDialogItem(hItem,ID_TEXT_10), &GUI_FontFontSong);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_4), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_4), &GUI_Font16B_ASCII);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_4), "");
        EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_4), 0);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_5), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_5), &GUI_Font16B_ASCII);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_5), "");
        EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_5), 0);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_6), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_6), &GUI_Font16B_ASCII);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_6), "");
        EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_6), 0);

        EDIT_SetTextAlign(WM_GetDialogItem(hItem,ID_EDIT_7), GUI_TA_VCENTER|GUI_TA_LEFT);
        EDIT_SetFont(WM_GetDialogItem(hItem,ID_EDIT_7), &GUI_Font16B_ASCII);
        EDIT_SetText(WM_GetDialogItem(hItem,ID_EDIT_7), "");
        EDIT_SetFocussable(WM_GetDialogItem(hItem,ID_EDIT_7), 0);

        //
        //ID_DROPDOWN_0
        //
//        DROPDOWN_SetAutoScroll(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 1);
//        DROPDOWN_SetScrollbarWidth(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 16);
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_0), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_0), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), 85);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "3");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "4");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "5");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "6");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "7");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "8");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "9");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "10");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "11");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "12");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "13");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "14");
//        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_0), "15");
        //
        //ID_DROPDOWN_1
        //
        DROPDOWN_SetTextAlign(WM_GetDialogItem(hItem,ID_DROPDOWN_1), GUI_TA_HCENTER | GUI_TA_VCENTER);
        DROPDOWN_SetTextHeight(WM_GetDialogItem(hItem,ID_DROPDOWN_1), 34);
        DROPDOWN_SetListHeight(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), 75);
        DROPDOWN_SetFont(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), GUI_FONT_16B_ASCII);
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "0");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "1");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "2");
        DROPDOWN_AddString(WM_GetDialogItem(pMsg->hWin, ID_DROPDOWN_1), "3");

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(Id) {
        case ID_DROPDOWN_0:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                shanqu3 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_0));
                break;
            }
            break;
        case ID_DROPDOWN_1:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:

                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_SEL_CHANGED:
                kuai3 =  DROPDOWN_GetSel(WM_GetDialogItem(hItem, ID_DROPDOWN_1));
                break;
            }
            break;
// USER START (Optionally insert additional code for further Ids)
// USER END
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/*********************************************************************
*
* CreateWindowPage3
*/
WM_HWIN CreateWindowPage3(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePage3, GUI_COUNTOF(_aDialogCreatePage3), _cbDialogPage3,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*
************************************************************************************************
* 键盘对话框
************************************************************************************************
*/
/*********************************************************************
*
* Defines
*
**********************************************************************
*/
#define ID_WINDOW_04 (GUI_ID_USER + 0x53)
// USER START (Optionally insert additional static data)
// USER END
/*********************************************************************
*
* _aDialogCreate
*/
static const GUI_WIDGET_CREATE_INFO _aDialogCreatePageKeyboard[] = {
    { WINDOW_CreateIndirect,    "Window",            ID_WINDOW_04,                 8, 305, 530, 165, 0, 0x0, 0},
    { BUTTON_CreateIndirect,    "4",                 ID_BUTTON_4,                  2, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "7",                 ID_BUTTON_7,                  2, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "8",                 ID_BUTTON_8,                  93, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "9",                 ID_BUTTON_9,                  189, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "0",                 ID_BUTTON_0,                  274, 104, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "5",                 ID_BUTTON_5,                  93, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "6",                 ID_BUTTON_6,                  189, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "Backspace",         ID_BUTTON_Backspace,          363, 104, 160, 40, 0, 0},
    { BUTTON_CreateIndirect,    "D",                 ID_BUTTON_D,                  274, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "E",                 ID_BUTTON_E,                  363, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "F",                 ID_BUTTON_F,                  453, 58, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "1",                 ID_BUTTON_1,                  2, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "2",                 ID_BUTTON_2,                  93, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "3",                 ID_BUTTON_3,                  189, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "A",                 ID_BUTTON_A,                  274, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "B",                 ID_BUTTON_B,                  363, 3, 70, 40, 0, 0},
    { BUTTON_CreateIndirect,    "C",                 ID_BUTTON_C,                  453, 3, 70, 40, 0, 0},
};
/*********************************************************************
*
* Static code
*
**********************************************************************
*/
// USER START (Optionally insert additional static code)
// USER END
/*********************************************************************
*
* _cbDialog
*/
static void _cbDialogPageKeyboard(WM_MESSAGE * pMsg) {
    WM_HWIN hItem;
    int NCode;
    int Id;
    int Pressed = 0;
    hItem = pMsg->hWin;
// USER START (Optionally insert additional variables)
// USER END
    switch (pMsg->MsgId) {
    case WM_INIT_DIALOG:
        WINDOW_SetBkColor(hItem, 0x578b2e);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_4), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_4), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_7), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_7), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_8), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_8), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_9), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_9), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_0), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_0), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_5), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_5), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_6), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_6), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_Backspace), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_Backspace), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_D), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_D), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_E), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_E), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_F), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_F), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_1), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_1), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_2), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_2), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_3), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_3), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_A), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_A), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_B), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_B), 0);

        BUTTON_SetFont(WM_GetDialogItem(hItem,ID_BUTTON_C), &GUI_FontFontSong);
        BUTTON_SetFocussable(WM_GetDialogItem(hItem,ID_BUTTON_C), 0);

// USER START (Optionally insert additional code for further widget initialization)
// USER END
        break;
    case WM_NOTIFY_PARENT:
        Id = WM_GetId(pMsg->hWinSrc);
        NCode = pMsg->Data.v;
        switch(NCode) {
        case WM_NOTIFICATION_CLICKED:
            Pressed = 1;
        case WM_NOTIFICATION_RELEASED:
            if ((Id >= GUI_ID_USER + 10 && Id <= GUI_ID_USER + 18) || (Id >= GUI_ID_USER + 0 && Id <= GUI_ID_USER + 7)) {
                int Key;
                if (Id != GUI_ID_USER + 7) {
                    char acBuffer[10];
                    BUTTON_GetText(pMsg->hWinSrc, acBuffer, sizeof(acBuffer)); /* Get the text of the button */
                    Key = acBuffer[0];
                } else {
                    Key = GUI_KEY_BACKSPACE; /* Get the text from the array */
                }
                GUI_SendKeyMsg(Key, Pressed); /* Send a key message to the focussed window */
            }
            break;
// USER START (Optionally insert additional code for further Ids)
// USER END
        }
        break;
    default:
        WM_DefaultProc(pMsg);
        break;
    }
}
/*********************************************************************
*
* CreateWindowPage3
*/
WM_HWIN CreateWindowPageKeyboard(void) {
    WM_HWIN hWin;
    hWin = GUI_CreateDialogBox(_aDialogCreatePageKeyboard, GUI_COUNTOF(_aDialogCreatePageKeyboard), _cbDialogPageKeyboard,
                               WM_HBKWIN, 0, 0);
    return hWin;
}

/*********************************************************************
*
*       static data
*
**********************************************************************
*/

/*********************************************************************
*
*       Dialog resource
*
* This table conatins the info required to create the dialog.
* It has been created by ucGUIbuilder.
*/

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
    { WINDOW_CreateIndirect,    NULL,                 0,                       0, 0, 800, 480, 0, 0, 0},
    { TEXT_CreateIndirect,      "卡号：",             ID_TEXT_0,               0, 18, 80, 30, 0, 0},
    { EDIT_CreateIndirect,       NULL,                ID_EDIT_0,               84, 19, 160, 30, 0, 160},
    { TEXT_CreateIndirect,      "密钥：",             ID_TEXT_1,               250, 19, 80, 30, 0, 0},
    { EDIT_CreateIndirect,       NULL,                ID_EDIT_1,               336, 19, 160, 30, 0, 160},
    { TEXT_CreateIndirect,      "ISO14443协议",       ID_TEXT_2,               558, 18, 217, 30, 0, 0},
    { BUTTON_CreateIndirect,    "休眠",           	  ID_BUTTON_xiumian,       570, 409, 45, 40, 0, 0},
    { BUTTON_CreateIndirect,    "唤醒",           	  ID_BUTTON_huanxing,      625, 409, 45, 40, 0, 0},
    { BUTTON_CreateIndirect,    "14443",              ID_BUTTON_14443,         685, 409, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "寻卡.IDL",           ID_BUTTON_xunkaIDL,      570, 363, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "写卡",          		  ID_BUTTON_xieka,         685, 363, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "寻卡.ALL",           ID_BUTTON_xunkaALL,      570, 308, 100, 40, 0, 0},
    { BUTTON_CreateIndirect,    "读卡",          		  ID_BUTTON_duka,          685, 308, 100, 40, 0, 0},
    { MULTIPAGE_CreateIndirect, NULL,          ID_MULTIPAGE_0,          10, 80, 780, 210, 0, 0x0, 0}
};

WM_HWIN hWin1, hWin2, hWin3;

/*****************************************************************
**      FunctionName:void InitDialog(WM_MESSAGE * pMsg)
**      Function: to initialize the Dialog items
**
**      call this function in _cbCallback --> WM_INIT_DIALOG
*****************************************************************/

void InitDialog(WM_MESSAGE * pMsg)
{
    WM_HWIN hWin = pMsg->hWin;

    GUI_UC_SetEncodeUTF8();
    //
    //WINDOW
    //
    WINDOW_SetBkColor(hWin, 0x578b2e);
    //
    //ID_TEXT_0
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin, ID_TEXT_0), 0x578b2e);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_0), 0xffffff);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, ID_TEXT_0), GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_0), &GUI_FontFontSong);
    //
    //ID_EDIT_0
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin, ID_EDIT_0), GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_0), &GUI_Font16B_ASCII);
    EDIT_SetFocussable(WM_GetDialogItem(hWin, ID_EDIT_0), 0);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_0), "");
    //
    //ID_TEXT_1
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin, ID_TEXT_1), 0x578b2e);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_1), 0xffffff);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, ID_TEXT_1), GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin, ID_TEXT_1), &GUI_FontFontSong);
    //
    //ID_EDIT_1
    //
    EDIT_SetTextAlign(WM_GetDialogItem(hWin, ID_EDIT_1), GUI_TA_VCENTER|GUI_TA_LEFT);
    EDIT_SetFont(WM_GetDialogItem(hWin, ID_EDIT_1), &GUI_Font16B_ASCII);
    EDIT_EnableBlink(WM_GetDialogItem(hWin, ID_EDIT_1), 100, 1);
    EDIT_SetMaxLen(WM_GetDialogItem(hWin, ID_EDIT_1), 12);
    EDIT_SetText(WM_GetDialogItem(hWin, ID_EDIT_1), "");
    //
    //ID_TEXT_2
    //
    TEXT_SetBkColor(WM_GetDialogItem(hWin, ID_TEXT_2), 0x578b2e);
    TEXT_SetTextColor(WM_GetDialogItem(hWin, ID_TEXT_2), 0x8b0000);
    TEXT_SetTextAlign(WM_GetDialogItem(hWin, ID_TEXT_2), GUI_TA_VCENTER|GUI_TA_CENTER);
    TEXT_SetFont(WM_GetDialogItem(hWin,ID_TEXT_2), &GUI_FontFontSong);

    //
    //BUTTON
    //
    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_xiumian), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_huanxing), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_14443), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_xunkaIDL), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_xieka), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_xunkaALL), &GUI_FontFontSong);


    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_duka), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_qianbaogongneng), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_caozuochuangkou1), &GUI_FontFontSong);

    BUTTON_SetFont(WM_GetDialogItem(hWin, ID_BUTTON_caozuochuangkou2), &GUI_FontFontSong);

    hWin = WM_GetDialogItem(pMsg->hWin, ID_MULTIPAGE_0);

    hWin1 = CreateWindowPage1();
    MULTIPAGE_SetFont(hWin, &GUI_FontFontSong);
    MULTIPAGE_AddEmptyPage(hWin, hWin1, "钱包功能");

    hWin2 = CreateWindowPage2();
    MULTIPAGE_AddEmptyPage(hWin, hWin2, "操作窗口1");

    hWin3 = CreateWindowPage3();
    MULTIPAGE_AddEmptyPage(hWin, hWin3, "操作窗口2");
}

/*********************************************************************
*
*       Dialog callback routine
*/
static void _cbDialog(WM_MESSAGE * pMsg)
{
    uint8_t i, cnt, kahao[20];
    int lendata;
    char temp[20];
    char buf[40] = {0};
    uint16_t res;
    uint16_t dataxieka[40];
    char pt[40];
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
        case ID_MULTIPAGE_0: // Notifications sent by 'Multipage'
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED: //--------------（13）
// USER START (Optionally insert code for reacting on notification message)
// USER END
                break;
            case WM_NOTIFICATION_RELEASED: //--------------（14）
// USER START (Optionally insert code for reacting on notification message)
// USER END
                break;
            case WM_NOTIFICATION_MOVED_OUT: //--------------（15）
// USER START (Optionally insert code for reacting on notification message)
// USER END
                break;
            case WM_NOTIFICATION_VALUE_CHANGED: //--------------（16）
// USER START (Optionally insert code for reacting on notification message)
// USER END
                break;
// USER START (Optionally insert additional code for further notification handling)
// USER END
            }
            break;
        case ID_BUTTON_14443://选择14443A
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 10; i++)
                {
                    BSP_ST16C554_CS1D_Write(chuankou[i]);
                    BSP_Delay_ms(1);
                }
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00)
                    flag1 = 1;
                else flag1 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xunkaALL:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 10; i++)//选卡
                {
                    BSP_ST16C554_CS1D_Write(xunka[i]);
                    BSP_Delay_ms(1);
                }
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag3 =1;
                else flag3 = 2;
                rxflag = 0;
                if(flag3 == 1)//防冲撞
                {
                    for(i = 0; i < 10; i++)
                    {
                        BSP_ST16C554_CS1D_Write(fangchongzhuang[i]);
                        BSP_Delay_ms(1);
                    }

                    BSP_Delay_ms(30);
                    if(rxflag && drbuf[8] == 0x00) flag3 = 1;
                    else flag3 = 2;
                    rxflag = 0;
                    if(flag3 == 1)
                    {
                        cnt = 0;
                        for(i = 9; i < 3 + drbuf[2]; i++)//9+drbuf[2]-6
                        {
                            temp[cnt++] = tran(drbuf[i]/16);
                            temp[cnt++] = tran(drbuf[i]%16);
                            kahao[cnt/2-1] = drbuf[i];
                        }
                        temp[cnt] = '\0';
                        EDIT_SetFont(WM_GetDialogItem(hWin,ID_EDIT_0),&GUI_Font16B_ASCII);
                        EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_0), GUI_TA_LEFT | GUI_TA_VCENTER);
                        EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_0),temp);
                        for(i = 0; i < 8; i++)//选择命令
                        {
                            BSP_ST16C554_CS1D_Write(xuanzemingling[i]);
                            BSP_Delay_ms(1);
                        }
                        cnt = xuanzemingling[4];
                        for(i = 5; i < 8; i++) cnt ^= xuanzemingling[i];
                        for(i = 9; i < 3+drbuf[2]; i++)
                        {
                            BSP_ST16C554_CS1D_Write(kahao[i-9]);
                            cnt ^= kahao[i-9];
                            BSP_Delay_ms(1);
                        }
                        BSP_ST16C554_CS1D_Write(cnt);
                        BSP_Delay_ms(30);
                        rxflag = 0;
                    }
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xiumian://休眠
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 9; i++)
                {
                    BSP_ST16C554_CS1D_Write(xiumian[i]);
                    BSP_Delay_ms(1);
                }
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag4 = 1;
                else flag4 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_huanxing://唤醒
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 10; i++)
                {
                    BSP_ST16C554_CS1D_Write(huanxing[i]);
                    BSP_Delay_ms(1);
                }
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00) flag5 = 1;
                else flag5 = 2;
                rxflag = 0;
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xunkaIDL://寻未休眠的卡
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                for(i = 0; i < 10; i++)
                {
                    BSP_ST16C554_CS1D_Write(xiumianxunka[i]);
                    BSP_Delay_ms(1);
                }
                BSP_Delay_ms(30);
                if(rxflag && drbuf[8] == 0x00)
                    flag2 = 1;
                else flag2 = 2;
                rxflag = 0;
                if(flag2 == 1)
                {
                    for(i = 0; i < 10; i++)
                    {
                        BSP_ST16C554_CS1D_Write(fangchongzhuang[i]);
                        BSP_Delay_ms(1);
                    }
                    BSP_Delay_ms(30);
                    if(rxflag && drbuf[8] == 0x00) flag2 = 1;
                    else flag2 = 2;
                    rxflag = 0;
                    if(flag2 == 1)
                    {
                        cnt = 0;
                        for(i = 9; i < 3 + drbuf[2]; i++)//9+drbuf[2]-6
                        {

                            temp[cnt++] = tran(drbuf[i]/16);
                            temp[cnt++] = tran(drbuf[i]%16);
                            kahao[cnt/2-1] = drbuf[i];
                        }
                        temp[cnt] = '\0';
                        EDIT_SetFont(WM_GetDialogItem(hWin,ID_EDIT_0),&GUI_Font16B_ASCII);
                        EDIT_SetTextAlign(WM_GetDialogItem(hWin,ID_EDIT_0), GUI_TA_LEFT | GUI_TA_VCENTER);
                        EDIT_SetText(WM_GetDialogItem(hWin,ID_EDIT_0),temp);
                        for(i = 0; i < 8; i++)//选择命令
                        {
                            BSP_ST16C554_CS1D_Write(xuanzemingling[i]);
                            BSP_Delay_ms(1);
                        }
                        cnt = xuanzemingling[4];
                        for(i = 5; i < 8; i++) cnt ^= xuanzemingling[i];
                        for(i = 9; i < 3+drbuf[2]; i++)
                        {
                            BSP_ST16C554_CS1D_Write(kahao[i-9]);
                            cnt ^= kahao[i-9];
                            BSP_Delay_ms(1);
                        }
                        BSP_ST16C554_CS1D_Write(cnt);
                        BSP_Delay_ms(30);
                        rxflag = 0;
                    }
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_duka:
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu3))
                {
                    flag7 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag7 = 2;
                    break;
                }
                if(!Shanqu(shanqu3))
                {
                    flag7 = 2;
                    break;
                }
                if(!DukaKuai(shanqu3, kuai3))
                {
                    flag7 = 2;
                    break;
                }
                flag7 = 1;
                cnt = 0;
                for(i = 9; i < 25; i++)
                {
                    pt[cnt++] = tran(drbuf[i]/16);
                    pt[cnt++] = tran(drbuf[i]%16);
                }
                pt[cnt] = '\0';
                EDIT_SetText(WM_GetDialogItem(hWin3, GUI_ID_USER + 49 + kuai3), pt);
                for(i = 49; i <= 52; i++)
                {
                    if(i != 49 + kuai3)
                        EDIT_SetText(WM_GetDialogItem(hWin3, GUI_ID_USER + i), "");
                }
                break;
            case WM_NOTIFICATION_RELEASED:
                break;
            case WM_NOTIFICATION_MOVED_OUT:
                break;
            }
            break;
        case ID_BUTTON_xieka:
            buf[0] = '\0';
            for(i = 0; i < 40; i++) dataxieka[i] = 0;
            EDIT_GetText(WM_GetDialogItem(hWin2,GUI_ID_USER+55+kuai2), buf, 32);
            lendata = EDIT_GetNumChars(WM_GetDialogItem(hWin2,GUI_ID_USER+55+kuai2));
            if(lendata & 1)
            {
                buf[lendata] = buf[lendata-1];
                buf[lendata-1] = '0';
                for(i = 0; i < lendata+1; i+=2)
                {
                    dataxieka[i/2] = trannum(buf[i]) * 16 + trannum(buf[i+1]);
                }
            }
            else
            {
                for(i = 0; i < lendata; i+=2)
                {
                    dataxieka[i/2] = trannum(buf[i]) * 16 + trannum(buf[i+1]);
                }
            }
            switch(NCode)
            {
            case WM_NOTIFICATION_CLICKED:
                if(!Key(shanqu2))
                {
                    flag8 = 3;
                    break;
                }
                if(!Xunka())
                {
                    flag8 = 2;
                    break;
                }
                if(!Shanqu(shanqu2))
                {
                    flag8 = 2;
                    break;
                }
                cnt = 0;
                for(i = 0; i < 8; i++)
                {
                    BSP_ST16C554_CS1D_Write(xiekakuai[i]);
                    if(i >= 4 && i <= 7) cnt ^= xiekakuai[i];
                    BSP_Delay_ms(1);
                }
                res = shanqu2 * 4 + kuai2;
                BSP_ST16C554_CS1D_Write(res);
                cnt ^= res;
                BSP_Delay_ms(1);
                //
                for(i = 0; i < 16; i++)
                {
                    res = dataxieka[i];
                    BSP_ST16C554_CS1D_Write(res);
                    cnt ^= res;
                    BSP_Delay_ms(1);
                    if(dataxieka[i] == 0xaa)
                    {
                        BSP_ST16C554_CS1D_Write(0x00);
                        BSP_Delay_ms(1);
                    }
                }
                //
                BSP_ST16C554_CS1D_Write(cnt);
                BSP_Delay_ms(30);
                if(drbuf[8] == 0x00) flag8 = 1;
                else flag8 = 2;
                rxflag = 0;
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
    hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
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
    uint8_t i;
    int res;
    char buf[40] = {0};
    WM_HWIN hWin;
    GUI_Init();
    WM_SetDesktopColor(GUI_WHITE);      /* Automacally update desktop window */
    WM_SetCreateFlags(WM_CF_MEMDEV);  /* Use memory devices on all windows to avoid flicker */
    hWin = CreateFramewin();
    CreateWindowPageKeyboard();
    while(1)
    {
        //14443模式选择
        if(flag1 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("14443 OK!", "", 0, &GUI_Font16B_ASCII));
            flag1 = 0;
        }
        else if(flag1 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("14443 failed!", "", 0, &GUI_Font16B_ASCII));
            flag1 = 0;
        }

        //寻卡.IDL
        if(flag2 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag2 = 0;
        }
        else if(flag2 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag2 = 0;
        }

        //寻卡.ALL
        if(flag3 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag3 = 0;
        }
        else if(flag3 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag3 = 0;
        }

        //休眠
        if(flag4 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag4 = 0;
        }
        else if(flag4 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag4 = 0;
        }

        //唤醒
        if(flag5 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag5 = 0;
        }
        else if(flag5 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag5 = 0;
        }

        //初始化
        if(flag6 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag6 = 0;
        }
        else if(flag6 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag6 = 0;
        }
        else if(flag6 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag6 = 0;
        }

        //读卡
        if(flag7 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag7 = 0;
        }
        else if(flag7 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag7 = 0;
        }
        else if(flag7 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag7 = 0;
        }

        //写卡
        if(flag8 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag8 = 0;
        }
        else if(flag8 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag8 = 0;
        }
        else if(flag8 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag8 = 0;
        }

        //读余额
        if(flag9 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag9 = 0;
        }
        else if(flag9 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag9 = 0;
        }
        else if(flag9 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag9 = 0;
        }

        //充值
        if(flag10 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag10 = 0;
        }
        else if(flag10 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag10 = 0;
        }
        else if(flag10 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag10 = 0;
        }

        //扣款
        if(flag11 == 1)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("OK!", "", 0, &GUI_Font16B_ASCII));
            flag11 = 0;
        }
        else if(flag11 == 2)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("failed!", "", 0, &GUI_Font16B_ASCII));
            flag11 = 0;
        }
        else if(flag11 == 3)
        {
            GUI_ExecCreatedDialog(_CreateMessageBox("key wrong!", "", 0, &GUI_Font16B_ASCII));
            flag11 = 0;
        }

        //10进制转16进制输出
        EDIT_GetText(WM_GetDialogItem(hWin1,ID_EDIT_2), buf, sizeof(buf));
        if(buf[0] != '\0')
        {
            sscanf(buf, "%d", &res);
            xianshijine = res;
            for(i = 0; i < 4; i++)
            {
                jine16[i*2+1] = tran(res%16);
                Data[i*2+1] = res % 16;
                res /= 16;
                jine16[i*2] = tran(res%16);
                Data[i*2] = res % 16;
                res /= 16;
            }
        }
        else
        {
            for(i = 0; i < 8; i++)
            {
                jine16[i] = 0;
                Data[i] = 0;
            }
            xianshijine = 0;
        }
        jine16[8] = '\0';
        EDIT_SetText(WM_GetDialogItem(hWin1,ID_EDIT_3),jine16);
        buf[0] = '\0';

        //获取密钥
        EDIT_GetText(WM_GetDialogItem(hWin,ID_EDIT_1), key, sizeof(key));
				
				res = MULTIPAGE_GetSelection(WM_GetDialogItem(hWin, ID_MULTIPAGE_0));
				if(res == 0)
				{
					WM_DisableWindow(WM_GetDialogItem(hWin,ID_BUTTON_duka));
					WM_DisableWindow(WM_GetDialogItem(hWin,ID_BUTTON_xieka));
				}
				else if(res == 1)
				{
					WM_DisableWindow(WM_GetDialogItem(hWin,ID_BUTTON_duka));
					WM_EnableWindow(WM_GetDialogItem(hWin,ID_BUTTON_xieka));
				}
				else if(res == 2)
				{
					WM_DisableWindow(WM_GetDialogItem(hWin,ID_BUTTON_xieka));
					WM_EnableWindow(WM_GetDialogItem(hWin,ID_BUTTON_duka));
				}

        BSP_Delay_ms(100);
        GUI_Delay(1);
    }
}
