#ifndef _MAINFRAME_DLG_H
#define _MAINFRAME_DLG_H
    
#include "ui/adapter/FirendListItemAdapter.h"
#include "ui/adapter/IListItemCallBack.h"
 

#pragma once 


class CMainDlg : public SOUI::SHostWnd, 
	public live::IListItemCallback 
{ 
	enum EListType
	{
		EFriendList=0, 
		ENewChannel, 
		EHotChannel,
	};
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, SOUI::CSize size);
	void OnLoginButtonClick();

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);
	 
	BOOL OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct);


	bool OnJoinChannel();
	bool OnQuitChannel();
	 

	int  GetReturnCode() { return m_nRetCode; }
protected:    

	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)

		EVENT_NAME_COMMAND(SOUI::R.name.btn_friends, OnSwitchFriends)
		EVENT_NAME_COMMAND(SOUI::R.name.btn_hot_channel, OnSwitchHotChannel)
		EVENT_NAME_COMMAND(SOUI::R.name.btn_new_channel, OnSwitchNewChannel) 
		EVENT_NAME_COMMAND(SOUI::R.name.btn_user_header, OnClickAvatar)
	EVENT_MAP_END()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)
		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize) 

		MSG_WM_COMMAND(OnCommand)
		MSG_WM_COPYDATA(OnCopyData)
		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()
	 
private:  
	//implement IListItemCallback
	virtual void OnItemDBClick(int data, int position, void *pData);
	 
	void OnSwitchFriends();
	void OnSwitchHotChannel();
	void OnSwitchNewChannel(); 
	void OnClickAvatar();

	void SyncFriendList(); //获取好友列表 
	void GetHotChannelList(); //获取热门频道列表
	void GetNewChannelList();//获取最新频道列表 
private:  
	void OnCommand(UINT uNotifyCode, int nID, HWND wndCtl); 
private:  
	SOUI::SWindow *m_p_tab_friends;
	SOUI::SWindow *m_p_tab_hotchannel;
	SOUI::SWindow *m_p_tab_newchannel; 

	SOUI::STabCtrl *m_p_TabMain;


	SOUI::SStatic *m_p_Title;

	SOUI::SMCListView  *m_lv_friends;
	SOUI::SMCListView  *m_lv_hot_channel;
	SOUI::SMCListView  *m_lv_new_channel;

    SOUI::SEdit    *m_pSearchInput;
    BOOL            m_bLayoutInited;

	int             m_nRetCode;  
};

#endif //_MAINFRAME_DLG_H