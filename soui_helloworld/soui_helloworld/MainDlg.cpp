// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "SImageSwitcher.h"

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}

CMainDlg::~CMainDlg()
{
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	// 		MARGINS mar = {5,5,30,5};
	// 		DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;
	return 0;
}


//TODO:消息映射
void CMainDlg::OnClose()
{
	DestroyWindow();
}

void CMainDlg::OnMaximize()
{
	//SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);

}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnDesktop()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(0);
}
void CMainDlg::OnUcenter()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(1);
}
void CMainDlg::OnGuard()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(2);
}
void CMainDlg::OnDiagnosescan()
{
	SImageSwitcher *pViewSwitch = FindChildByName2<SImageSwitcher>(L"vw_switch");
	if (pViewSwitch)
		pViewSwitch->Switch(3);
}