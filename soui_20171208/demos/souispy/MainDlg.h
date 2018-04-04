// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

#include "SwndFrame.h"

class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose()
	{
		AnimateHostWindow(200,AW_CENTER|AW_HIDE);
        DestroyWindow();
	}
	void OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND,SC_MAXIMIZE);
	}
	void OnRestore()
	{
		SendMessage(WM_SYSCOMMAND,SC_RESTORE);
	}
	void OnMinimize()
	{
		SendMessage(WM_SYSCOMMAND,SC_MINIMIZE);
	}
    
    void OnBtnSearch();
protected:
	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);
		if(!m_bLayoutInited) return;
		if(nType==SIZE_MAXIMIZED)
		{
			FindChildByName(L"btn_restore")->SetVisible(TRUE);
			FindChildByName(L"btn_max")->SetVisible(FALSE);
		}else if(nType==SIZE_RESTORED)
		{
            FindChildByName(L"btn_restore")->SetVisible(FALSE);
            FindChildByName(L"btn_max")->SetVisible(TRUE);
		}
	}
        
	void OnShowWindow(BOOL bShow, UINT nStatus);
    BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

    BOOL OnEventCaptureHost(EventArgs *pEvt);
    BOOL OnEventCaptureHostFinish(EventArgs *pEvt);

    BOOL OnEventCaptureSwnd(EventArgs *pEvt);
    BOOL OnEventCaptureSwndFinish(EventArgs *pEvt);
    
    BOOL OnEventTreeSelChanged(EventArgs *pEvt);
    
    BOOL OnCopyData(HWND hSend, PCOPYDATASTRUCT pCopyDataStruct);

    void EnumSwnd(HWND hHost ,DWORD dwSwnd, HSTREEITEM hItem);
protected:

	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close",OnClose)
        EVENT_NAME_COMMAND(L"btn_min",OnMinimize)
        EVENT_NAME_COMMAND(L"btn_max",OnMaximize)
        EVENT_NAME_COMMAND(L"btn_restore",OnRestore)
        EVENT_NAME_HANDLER(L"img_capture_host",EventCapture::EventID,OnEventCaptureHost)
        EVENT_NAME_HANDLER(L"img_capture_host",EventCaptureFinish::EventID,OnEventCaptureHostFinish)
        EVENT_NAME_HANDLER(L"img_capture_swnd",EventCapture::EventID,OnEventCaptureSwnd)
        EVENT_NAME_HANDLER(L"img_capture_swnd",EventCaptureFinish::EventID,OnEventCaptureSwndFinish)
        EVENT_NAME_HANDLER(L"tree_swnd",EventTCSelChanged::EventID,OnEventTreeSelChanged)
        EVENT_NAME_COMMAND(L"btn_search",OnBtnSearch)
	EVENT_MAP_END()	

	BEGIN_MSG_MAP_EX(CMainDlg)
        MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		MSG_WM_SHOWWINDOW(OnShowWindow)
        MSG_WM_COPYDATA(OnCopyData)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
private:
	BOOL			m_bLayoutInited;

    STreeCtrl     * m_pTreeCtrl;

    SwndFrame       m_wndFrame;
};
