#include "stdafx.h"
#include "MainDlg.h"
#include "ui/adapter/HotChannelListItemAdapter.h"
#include "ui/adapter/NewChannelListItemAdapter.h"  
#include "ui/drag/DragWnd.h"
#include "ui/ChooseAvatar.h"

#define SUBSCRIBE(x,y,z) (x)->GetEventSet()->subscribeEvent(y,Subscriber(&z,this))
#define UNSUBSCRIBE(x,y,z) (x)->GetEventSet()->unsubscribeEvent(y,Subscriber(&z,this))

using namespace SOUI;
using namespace live;

CMainDlg::CMainDlg() : SHostWnd(UIRES.LAYOUT.dlg_main)
, m_pSearchInput(NULL)
{ 
	m_nRetCode = -1;
	m_bLayoutInited = FALSE;


	m_p_tab_friends = NULL;
	m_p_tab_hotchannel = NULL;
	m_p_tab_newchannel = NULL;

	m_p_TabMain =NULL; 
}

CMainDlg::~CMainDlg()
{
}
 
BOOL CMainDlg::OnCopyData(HWND wnd, PCOPYDATASTRUCT pCopyDataStruct)
{ 
	return TRUE;
}

int CMainDlg::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	#ifdef DWMBLUR	//win7毛玻璃开关
	MARGINS mar = {5,5,30,5};
	DwmExtendFrameIntoClientArea ( m_hWnd, &mar );
	#endif

	SetMsgHandled(FALSE);
	return 0;
}

BOOL CMainDlg::OnInitDialog(HWND hWnd, LPARAM lParam)
{
	m_bLayoutInited = TRUE;

	
	m_p_Title = this->FindChildByName2<SStatic>(L"txt_title");
	m_p_TabMain = this->FindChildByName2<STabCtrl>(L"tab_main");
	m_p_tab_friends = this->FindChildByName2<SWindow>(L"btn_friends");
	m_p_tab_hotchannel = this->FindChildByName2<SWindow>(L"btn_hot_channel");
	m_p_tab_newchannel = FindChildByName2<SWindow>(L"btn_new_channel");


	m_pSearchInput = FindChildByName2<SEdit>(L"edt_search_input");

	m_lv_friends = FindChildByID2<SMCListView>(R.id.friends_list);
	m_lv_hot_channel = FindChildByID2<SMCListView>(R.id.hot_channel_list);
	m_lv_new_channel = FindChildByID2<SMCListView>(R.id.new_channel_list);

	  

	SyncFriendList(); 

	GetHotChannelList();
	GetNewChannelList();
	  

	return 0;
}
 
void CMainDlg::OnClose()
{ 
	CSimpleWnd::DestroyWindow(); 
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
	if (!m_bLayoutInited) return;
	
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
   
void CMainDlg::OnSwitchFriends()
{
	m_p_tab_friends->SetAttribute(L"skin", L"skin.tabbtn_hover", FALSE);
	m_p_tab_friends->SetAttribute(L"class", L"font:tab_button_hover", FALSE);


	m_p_tab_hotchannel->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_hotchannel->SetAttribute(L"class", L"font:tab_button_normal", FALSE);

	m_p_tab_newchannel->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_newchannel->SetAttribute(L"class", L"font:tab_button_normal", FALSE);


	m_p_TabMain->SetCurSel(_T("friend"));
	m_p_TabMain->GetItem(m_p_TabMain->GetPageIndex(_T("friend"), TRUE))->Invalidate(); 
}

void CMainDlg::OnSwitchHotChannel()
{
	m_p_tab_friends->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_friends->SetAttribute(L"class", L"font:tab_button_normal", FALSE);


	m_p_tab_hotchannel->SetAttribute(L"skin", L"skin.tabbtn_hover", FALSE);
	m_p_tab_hotchannel->SetAttribute(L"class", L"font:tab_button_hover", FALSE);

	m_p_tab_newchannel->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_newchannel->SetAttribute(L"class", L"font:tab_button_normal", FALSE);

	m_p_TabMain->SetCurSel(_T("hot"));
	m_p_TabMain->GetItem(m_p_TabMain->GetPageIndex(_T("hot"), TRUE))->Invalidate();
	 
}


void CMainDlg::OnSwitchNewChannel()
{
	m_p_tab_friends->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_friends->SetAttribute(L"class", L"font:tab_button_normal", FALSE);


	m_p_tab_hotchannel->SetAttribute(L"skin", L"skin.tabbtn_normal", FALSE);
	m_p_tab_hotchannel->SetAttribute(L"class", L"font:tab_button_normal", FALSE);

	m_p_tab_newchannel->SetAttribute(L"skin", L"skin.tabbtn_hover", FALSE);
	m_p_tab_newchannel->SetAttribute(L"class", L"font:tab_button_hover", FALSE);



	m_p_TabMain->SetCurSel(_T("new"));
	m_p_TabMain->GetItem(m_p_TabMain->GetPageIndex(_T("new"), TRUE))->Invalidate();
	 
}
     
//获取好友列表
void CMainDlg::SyncFriendList()
{
	CFriendListAdapter *pLVAdapter = (CFriendListAdapter *)m_lv_friends->GetAdapter();
	if (NULL == pLVAdapter)
	{
		pLVAdapter = new CFriendListAdapter(EFriendList, this);
	}

	pLVAdapter->RemoveAll();

	std::wstring title = L"我的好友";
	unsigned int  uid = 0;
	bool has_child = false;

	{
		Node<FriendItemData>* root_parent = pLVAdapter->AddGroupNode(title, true, NULL);

		title = L"MPF掌柜";
		uid = 1001;
		pLVAdapter->AddNode(title, uid, has_child, root_parent);

		title = L"SOUI库";
		uid = 1020;
		pLVAdapter->AddNode(title, uid, has_child, root_parent);


		title = L"wacr2008";
		uid = 1002;
		pLVAdapter->AddNode(title, uid, has_child, root_parent);

	}


	{
		title = L"黑名单";
		uid = 2011;
		Node<FriendItemData>* root_parent_block = pLVAdapter->AddGroupNode(title, true, NULL);

		title = L"传销";
		uid = 2012;
		pLVAdapter->AddNode(title, uid, has_child, root_parent_block);

		title = L"广告";
		uid = 2013;
		pLVAdapter->AddNode(title, uid, has_child, root_parent_block);

		title = L"猎头";
		uid = 2015;
		pLVAdapter->AddNode(title, uid, has_child, root_parent_block);

	}


	m_lv_friends->SetAdapter(pLVAdapter);
	pLVAdapter->Release();
} 
void CMainDlg::GetHotChannelList()
{

	CHotChannelListItemAdapter *pLVAdapter = (CHotChannelListItemAdapter *)m_lv_hot_channel->GetAdapter();
	if (NULL == pLVAdapter)
	{
		pLVAdapter = new CHotChannelListItemAdapter(EHotChannel, this);
	}

	pLVAdapter->RemoveAll();

	std::wstring title = L"热门游戏";
	unsigned int  channel_id = 100;
	unsigned int uid = 1001;
	bool has_child = false;

	Node<HotChannelInfo>* root_parent = pLVAdapter->AddGroupNode(title, true, NULL);

	{
		title = L"斗地主";
		uid = 1002;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent);

		title = L"比大小";
		uid = 1003;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent);
	}
	
	title = L"娱乐频道";
	Node<HotChannelInfo>* root_parent2 = pLVAdapter->AddGroupNode(title, true,  NULL);

	{
		title = L"新浪";
		uid = 1004;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent2);

		title = L"网易";
		uid = 1005;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent2);
	}


	title = L"广东区域";
	Node<HotChannelInfo>* root_parent3 = pLVAdapter->AddGroupNode(title, true,  NULL);

	{
		title = L"广州";
		uid = 1008;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent3);

		title = L"深圳";
		uid = 1009;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent3);

		title = L"佛山";
		uid = 1010;
		pLVAdapter->AddNode(title, channel_id, uid, has_child, root_parent3);
	}

	pLVAdapter->ExpendNode(0, false);


	m_lv_hot_channel->SetAdapter(pLVAdapter);

	pLVAdapter->notifyDataSetChanged();

	OnSwitchHotChannel();
}

void CMainDlg::GetNewChannelList()
{

	CNewChannelListItemAdapter *pLVAdapter = (CNewChannelListItemAdapter *)m_lv_new_channel->GetAdapter();
	if (NULL == pLVAdapter)
	{
		pLVAdapter = new CNewChannelListItemAdapter(ENewChannel, this);
	}

	pLVAdapter->RemoveAll();

	std::wstring title = L"最新";
	unsigned int  uid = 0;
	bool has_child = false;

	Node<NewChannelInfo>* root_parent = pLVAdapter->AddGroupNode(title, true, NULL);

	title = L"欢乐斗地主";
	uid = 1301;

	pLVAdapter->AddNode(title, uid, has_child, root_parent);

	title = L"长沙麻将";
	uid = 1501;
	pLVAdapter->AddNode(title, uid, has_child, root_parent);

	title = L"广东麻将";
	uid = 1501;
	pLVAdapter->AddNode(title, uid, has_child, root_parent);

	m_lv_new_channel->SetAdapter(pLVAdapter);

	pLVAdapter->notifyDataSetChanged();
} 
  
      
void CMainDlg::OnItemDBClick(int data, int position,void *pData)
{
	switch (data)
	{
	case EFriendList:
		{
			//unsigned int  uid = *(unsigned int  *)pData;
			//OpenChatWindow(uid);
		}
		break;
	case ENewChannel:
		{
			//unsigned int  channel_id = *(unsigned int  *)pData;

			//OpenChannelWindow(channel_id);
		}
		break;
	case EHotChannel:
		{ 
			//unsigned int  channel_id = *(unsigned int  *)pData;

			//OpenChannelWindow(channel_id);
		}
		break;
	}
}
 
void CMainDlg::OnCommand(UINT uNotifyCode, int nID, HWND wndCtl)
{
	if (uNotifyCode == 0)
	{
		switch (nID)
		{
			case 100:
			{
				GetNewChannelList();//获取最新频道列表 
			}
			break;
			case 101:
			{
				GetHotChannelList(); //获取热门频道列表
				 
			}
			break;
		}
	}
}

void CMainDlg::OnClickAvatar()
{
	ChooseAvatarDlg chooseAvatarDlg;
	chooseAvatarDlg.DoModal(m_hWnd);
}

