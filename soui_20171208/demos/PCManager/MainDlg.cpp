// MainDlg.cpp : implementation of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"

#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
using namespace SOUI;

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND")), m_nowHomeRadio(0)
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
	SendMessage(WM_SYSCOMMAND, SC_MAXIMIZE);
}
void CMainDlg::OnRestore()
{
	SendMessage(WM_SYSCOMMAND, SC_RESTORE);
}
void CMainDlg::OnMinimize()
{
	SendMessage(WM_SYSCOMMAND, SC_MINIMIZE);
}

void CMainDlg::OnSize(UINT nType, CSize size)
{
	SetMsgHandled(FALSE);
	
	SWindow *pBtnMax = FindChildByName(L"btn_max");
	SWindow *pBtnRestore = FindChildByName(L"btn_restore");
	if(!pBtnMax || !pBtnRestore) return;
	
	if (nType == SIZE_MAXIMIZED)
	{
		pBtnRestore->SetVisible(TRUE);
		pBtnMax->SetVisible(FALSE);
	}
	else if (nType == SIZE_RESTORED)
	{
		pBtnRestore->SetVisible(FALSE);
		pBtnMax->SetVisible(TRUE);
	}
}

void CMainDlg::OnLButtonDown( UINT nFlags, CPoint point )
{
	HWND hHost=GetContainer()->GetHostHwnd();
	if (WS_MAXIMIZE == (::GetWindowLong(hHost,GWL_STYLE) & WS_MAXIMIZE))
	{
		//m_bIsMaxDown = TRUE;
		return;
	}
	::SendMessage(hHost,WM_SYSCOMMAND, SC_MOVE | HTCAPTION,0);
}

void CMainDlg::Home_OnRadioCmd0()
{
	Home_SwitchRadio(0);
}

void CMainDlg::Home_OnRadioCmd1()
{
	Home_SwitchRadio(1);
}

void CMainDlg::Home_OnRadioCmd2()
{
	Home_SwitchRadio(2);
}

void CMainDlg::Home_SwitchRadio( BYTE trId )
{
	m_tarHomeRadio = trId;
	CSimpleWnd::SetTimer(m_HomeTimerID,20);
}

void CMainDlg::OnTimer( char cTimerID )
{

}

void CMainDlg::OnTimer2( UINT_PTR idEvent )
{
	SWindow *pCon = FindChildByName(L"home_left_con");
	SWindow *pLw = FindChildByName(L"home_left_window_0");
	BOOL bKill = FALSE;
	switch (idEvent)
	{
	case m_HomeTimerID:
		if (pCon && pLw)
		{
			float ftar = (float)-m_tarHomeRadio;
			float fnow = 0.0f;
			if (ftar > fnow)
			{
				fnow +=(float)0.2;
				if (fnow >= ftar)
				{
					bKill = TRUE;
					fnow = ftar;
				}
			}
			else if (ftar < fnow)
			{
				fnow -= (float)0.2;
				if (fnow <= ftar)
				{
					bKill = TRUE;
					fnow = ftar;
				}
			} else 
				bKill = TRUE;
			if (bKill){
				CSimpleWnd::KillTimer(idEvent);
				Home_MoveRadio();
			}
			wchar_t buf[12] = {0};
			int n = swprintf(buf, sizeof(buf), L"%f,0", fnow);
			pLw->SetAttribute(L"offset",buf);
			pCon->Invalidate();
		}
		break;
	default:
		SetMsgHandled(FALSE);
	}
}

void CMainDlg::Home_MoveRadio()
{
	SWindow  *pRa = FindChildByName(L"home_radio");
	if (pRa)
	{
		switch (m_tarHomeRadio)
		{
		case 0:
			pRa->SetAttribute(L"pos",L"|-20,-30");
			pRa->SetAttribute(L"offset",L"-1,0");
			break;
		case 1:
			pRa->SetAttribute(L"pos",L"|,-30");
			pRa->SetAttribute(L"offset",L"0,0");
			break;
		case 2:
			pRa->SetAttribute(L"pos",L"|20,-30");
			pRa->SetAttribute(L"offset",L"1,0");
			break;
		}
	}
}

