#pragma once

#include "stdafx.h"

class CMainWnd: public SHostWnd
{
public:
	CMainWnd():SHostWnd(_T("LAYOUT:XML_MAINWND"))
	{
		m_bLayoutInited = false;
	}

	void OnClose()
	{
		PostMessage(WM_QUIT);
	}

	void OnMaximize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
	}

	void OnRestore()
	{
		SendMessage(WM_SYSCOMMAND, SC_RESTORE);
	}

	void OnMinimize()
	{
		SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
	}

	void OnSize(UINT nType, CSize size)
	{
		SetMsgHandled(FALSE);

		if (!m_bLayoutInited)
			return;

		if (nType == SIZE_MAXIMIZED)
		{
			FindChildByName(L"btn_restore")->SetVisible(TRUE);
			FindChildByName(L"btn_max")->SetVisible(FALSE);
		}
		else if (nType == SIZE_RESTORED)
		{
			FindChildByName(L"btn_restore")->SetVisible(FALSE);
			FindChildByName(L"btn_max")->SetVisible(TRUE);
		}
	}

	void OnBtnMsgBox()
	{
		SMessageBox(NULL, _T("This is a message box"), _T("haha"), MB_OK | MB_ICONEXCLAMATION);
	}

	BOOL OnInitDialog(HWND hWnd, LPARAM lParam)
	{
		m_bLayoutInited = true;

		return 0;//S_OK;
	}

protected:
	//SOUI�¼�����ӳ���;
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"btn_msgbox", OnBtnMsgBox)
	EVENT_MAP_END()

	//Windows��Ϣ����ӳ���;(WTL)
	BEGIN_MSG_MAP_EX(CMainWnd)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()

private:
	bool m_bLayoutInited;
};