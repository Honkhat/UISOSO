/**
 * Copyright (C) 2014-2050 359501300
 * All rights reserved.
 * 
 * @file       SShellNotifyIcon.h
 * @brief      
 * @version    v1.0      
 * @author     359501300      
 * @date       2014-09-19
 * 
 * Describe     
 */
#pragma once

#define WM_ICONNOTIFY (WM_USER + 1101) 

#define ID_TASKBARICON	100

#include <ShellAPI.h>
/**
 * @class      SShellNotifyIcon
 * @brief      ShellNotifyIcon
 * 
 * Describe    ShellNotifyIcon
 */
class SShellNotifyIcon
{
public:
    /**
     * SShellNotifyIcon::SShellNotifyIcon
     * @brief    构造函数
     *
     * Describe  构造函数  
     */
	SShellNotifyIcon();

    /**
     * SShellNotifyIcon::~SShellNotifyIcon
     * @brief    析构函数
     *
     * Describe  析构函数  
     */
	virtual ~SShellNotifyIcon();
	
    void Create(HWND hOwner,HICON hIcon,UINT uFlags = (NIF_ICON | NIF_MESSAGE | NIF_TIP), UINT uCallbackMessage = WM_ICONNOTIFY, UINT uId = ID_TASKBARICON, LPCWSTR szTip=L"Soui 托盘演示");

    BOOL Show();

    BOOL Hide();
protected:
	void IniNotifyIconData(HWND hOwner,HICON hIcon,UINT flags, UINT callbackmsg, UINT ID, LPCWSTR szTip);

	NOTIFYICONDATA nid;
};

