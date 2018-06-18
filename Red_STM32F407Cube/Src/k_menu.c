#include "DIALOG.h"

#define ID_FRAMEWIN_0  (GUI_ID_USER + 0x04)
#define ID_ICONVIEW_0  (GUI_ID_USER + 0x06)
#define ID_TEXT_0  (GUI_ID_USER + 0x07)

static const GUI_WIDGET_CREATE_INFO _aDialogCreate[] = {
	{ FRAMEWIN_CreateIndirect, "Framewin", ID_FRAMEWIN_0, 0, 0, 320, 480, 0, 0x64, 0 },
	{ ICONVIEW_CreateIndirect, "Iconview", ID_ICONVIEW_0, 0, 80, 320, 480, 0, 0x00400040/*xSizeItem-ySizeItem*/, 0 },
	{ TEXT_CreateIndirect, "Text", ID_TEXT_0, 15, 10, 280, 20, 0, 0x64, 0 },
};

extern GUI_CONST_STORAGE GUI_BITMAP bm01;
extern GUI_CONST_STORAGE GUI_BITMAP bm02;
extern GUI_CONST_STORAGE GUI_BITMAP bm03;
extern GUI_CONST_STORAGE GUI_BITMAP bm04;
extern GUI_CONST_STORAGE GUI_BITMAP bm05;
extern GUI_CONST_STORAGE GUI_BITMAP bm06;
extern GUI_CONST_STORAGE GUI_BITMAP bm07;
extern GUI_CONST_STORAGE GUI_BITMAP bm08;
extern GUI_CONST_STORAGE GUI_BITMAP bm09;

static void _cbDialog(WM_MESSAGE * pMsg) {
	WM_HWIN hItem;
	int     NCode;
	int     Id;

	switch (pMsg->MsgId) {
	case WM_INIT_DIALOG://初始化消息,创建窗口/控件时有效,比如在这里设置一些控件的初始参数
		hItem = pMsg->hWin;
		FRAMEWIN_SetText(hItem, "neqee.com");
		FRAMEWIN_SetFont(hItem, GUI_FONT_20_1);
		FRAMEWIN_SetTextColor(hItem, (0x00FF0000));

		hItem = WM_GetDialogItem(pMsg->hWin, ID_TEXT_0);
		TEXT_SetTextAlign(hItem, GUI_TA_HCENTER | GUI_TA_VCENTER);
		TEXT_SetTextColor(hItem, (0x008000FF));
		TEXT_SetFont(hItem, GUI_FONT_20_1);
		TEXT_SetText(hItem, "neqee");

		hItem = WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_0);
		ICONVIEW_SetIconAlign(hItem, ICONVIEW_IA_TOP);
		ICONVIEW_SetTextColor(hItem, ICONVIEW_CI_UNSEL, GUI_BLUE);
		ICONVIEW_SetTextColor(hItem, ICONVIEW_CI_SEL, GUI_GREEN);
		ICONVIEW_SetFrame(hItem, GUI_COORD_X, 0);//设置图标到IconView边框的间距
		ICONVIEW_SetFrame(hItem, GUI_COORD_Y, 0);
		ICONVIEW_SetSpace(hItem, GUI_COORD_X, (320-64*4)/3);//设置图标和图标之间的间距
		ICONVIEW_SetSpace(hItem, GUI_COORD_Y, (400-64*5)/4);
		ICONVIEW_SetFont(hItem, GUI_FONT_16_1);
		ICONVIEW_AddBitmapItem(hItem, &bm01, "neqee");//添加图标项
		ICONVIEW_AddBitmapItem(hItem, &bm02, "SOS");
		ICONVIEW_AddBitmapItem(hItem, &bm03, "Phone");
		ICONVIEW_AddBitmapItem(hItem, &bm04, "Camera");
		ICONVIEW_AddBitmapItem(hItem, &bm05, "Email");
		ICONVIEW_AddBitmapItem(hItem, &bm06, "NoteBook");
		ICONVIEW_AddBitmapItem(hItem, &bm07, "JTV");
		ICONVIEW_AddBitmapItem(hItem, &bm08, "Dollar");
		//ICONVIEW_AddBitmapItem(hItem, &bm09, "Jobbet");
		break;
	case WM_NOTIFY_PARENT://操作触发消息处理(操作屏幕程序会跑到这里),比如点击按键、点击编辑框(任何的操作)等等......
		Id    = WM_GetId(pMsg->hWinSrc);
		NCode = pMsg->Data.v;
		switch(Id) {
		case ID_ICONVIEW_0:
			switch(NCode) {
			case WM_NOTIFICATION_CLICKED://点击图标
				switch(ICONVIEW_GetSel(WM_GetDialogItem(pMsg->hWin, ID_ICONVIEW_0)))
				{
				case 0: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "neqee"); break;
				case 1: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "SOS"); break;
				case 2: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Phone"); break;
				case 3: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Camera"); break;
				case 4: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Email"); break;
				case 5: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "NoteBook"); break;
				case 6: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "JTV"); break;
				case 7: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Dollar"); break;
				//case 8: TEXT_SetText(WM_GetDialogItem(pMsg->hWin, ID_TEXT_0), "Jobbet"); break;
				}
				break;
			case WM_NOTIFICATION_RELEASED:
				break;
			case WM_NOTIFICATION_MOVED_OUT:
				break;
			case WM_NOTIFICATION_SCROLL_CHANGED:
				break;
			case WM_NOTIFICATION_SEL_CHANGED:
				break;
			}
			break;
		}
		break;
	default:
		WM_DefaultProc(pMsg);
		break;
	}
}

WM_HWIN CreateFramewin(void);
WM_HWIN CreateFramewin(void) {
	WM_HWIN hWin;

	hWin = GUI_CreateDialogBox(_aDialogCreate, GUI_COUNTOF(_aDialogCreate), _cbDialog, WM_HBKWIN, 0, 0);
	return hWin;
}





void    k_InitMenu(void)
{
    CreateFramewin();
}























