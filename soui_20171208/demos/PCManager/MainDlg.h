// MainDlg.h : interface of the CMainDlg class
//
/////////////////////////////////////////////////////////////////////////////
#pragma once

class CMainDlg : public SHostWnd
{
	friend class SWindow;
public:
	
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);
	void OnLButtonDown(UINT nFlags, CPoint point);
	void OnTimer(char cTimerID);
	void OnTimer2(UINT_PTR idEvent);

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
protected:
private:
	void Home_OnRadioCmd0(); //Tool工具区Radio
	void Home_OnRadioCmd1();
	void Home_OnRadioCmd2();

	void Home_SwitchRadio(BYTE trId);
	void Home_MoveRadio();
protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		EVENT_NAME_COMMAND(L"home_radiobg_0", Home_OnRadioCmd0)
		EVENT_NAME_COMMAND(L"home_radiobg_1", Home_OnRadioCmd1)
		EVENT_NAME_COMMAND(L"home_radiobg_2", Home_OnRadioCmd2)
	EVENT_MAP_END()
	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
		MSG_WM_TIMER(OnTimer2) //....
	END_MSG_MAP()

	SOUI_MSG_MAP_BEGIN()
		MSG_WM_LBUTTONDOWN(OnLButtonDown)
		MSG_WM_TIMER_EX(OnTimer)    //定时器消息
	SOUI_MSG_MAP_END()
	
private:
	BOOL			m_bLayoutInited;
	BYTE			m_nowHomeRadio, m_tarHomeRadio; //当前和目标Radio状态
	static const char  m_HomeTimerID = 100;		
};
