//////////////////////////////////////////////////////////////////////////
//  Class Name: SShellNotifyIcon
// Description:show ShellNotifyIcon
//     Creator: 359501300
//     Version: 2014.9.19 - 1.0 - Create
//////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "SShellNotifyIcon.h"

	SShellNotifyIcon::SShellNotifyIcon()
	{
		memset(&nid,0,sizeof(nid));
	}
	
	SShellNotifyIcon::~SShellNotifyIcon()
	{
		Hide();
	}
	
	void SShellNotifyIcon::Create(HWND hOwner,HICON hIcon,UINT uFlags, UINT uCallbackMessage, UINT uId, LPCWSTR szTip)
	{
		IniNotifyIconData(hOwner,hIcon,uFlags, uCallbackMessage, uId, szTip);
	}
	

	BOOL SShellNotifyIcon::Show()
	{
		//未初使化NotifyIconData
		if (nid.cbSize != sizeof(NOTIFYICONDATA))
			return FALSE;
		return Shell_NotifyIcon(NIM_ADD, &nid);
	}
	
	BOOL SShellNotifyIcon::Hide()
	{
		return Shell_NotifyIcon(NIM_DELETE, &nid);
	}
	
	void SShellNotifyIcon::IniNotifyIconData(HWND hOwner,HICON hIcon,UINT flags, UINT callbackmsg, UINT ID, LPCWSTR szTip)
	{
		nid.cbSize = sizeof(NOTIFYICONDATA);
		nid.hWnd = hOwner;
		nid.uID = ID;
		nid.uFlags = flags;//NIF_ICON | NIF_MESSAGE | NIF_TIP;
		nid.uCallbackMessage = callbackmsg;
		nid.hIcon = hIcon;
		_tcscpy_s(nid.szTip, szTip);
	}
