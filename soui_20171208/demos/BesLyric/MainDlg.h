/*
	BesLyric  一款 操作简单、功能实用的 专门用于制作网易云音乐滚动歌词的 歌词制作软件。
    Copyright (C) 2017  BensonLaur

    This program is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    This program is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with this program.  If not, see <http://www.gnu.org/licenses/>.
*/

/**
* @file       MainDlg.h
* @version    v1.0      
* @author     BensonLaur   
* @date       2017/01/08
* 
* Describe    主窗口类，提供界面的消息处理接口； 以及部分页面业务逻辑的接口和实现
*/

#pragma once
#include "LyricMaker.h"
#include "LyricPlayer.h"
#include "BSMessageBox.h"

//在制作歌词页面 和 滚动预览 页面中，需要选择音乐路径，歌词路径，输出路径等信息
//该结构用于存储 路径名 以及 是否已被选择的状态
struct _PATH_STATE
{	
	TCHAR nameOfPath[_MAX_PATH];
	bool isInited;
}; 

//制作歌词页面 的路径 与选择状态
static struct _PATH_STATE PATH_STATE_1[] = 
{
	{_T("音乐文件"),false},
	{_T("歌词文件"),false},
	{_T("输出路径"),false}
};

//滚动预览页面 的路径 与选择状态
static struct _PATH_STATE PATH_STATE_2[] = 
{
	{_T("音乐文件"),false},
	{_T("歌词文件"),false}
};


/* 程序的主窗口类 */
class CMainDlg : public SHostWnd
{
public:
	CMainDlg();
	~CMainDlg();

	void OnClose();
	void OnMaximize();
	void OnRestore();
	void OnMinimize();
	void OnSize(UINT nType, CSize size);

	void OnBtnMsgBox();
	int OnCreate(LPCREATESTRUCT lpCreateStruct);
	BOOL OnInitDialog(HWND wndFocus, LPARAM lInitParam);

	//接受键盘输入
	void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);

	//各个页面按钮的响应函数
	
	void OnBtnSelectMusic1();		//三个路径的选择
	void OnBtnSelectLyric1();
	void OnBtnSelectOutput();
	void OnBtnLoad1();				//制作歌词：加载 (初始化 LyricMaker)
	void OnBtnRestart();			//我要重制
	void OnBtnStartMaking();		//开始制作
	void OnBtnPreview();
	void OnBtnOpenOutput();			//打开生成的文件

	void OnBtnSelectMusic2();		//两个路径的选择
	void OnBtnSelectLyric2();	
	void OnBtnLoad2();
	void OnBtnEndPreview();
	void OnBtnStartPlaying();		//开始播放歌词
	void OnBtnManualAdjust();		//手动打开文件调整时间

	//接收音乐消息
	void OnMusicCommand(UINT lParam, UINT wParam);

	//接收定时器消息，用于滚动歌词动画的实现
	void OnTimer(UINT_PTR nIDEvent);

	//相应2个页面共四个静音按钮
	void OnBtnSoundOpen1();
	void OnBtnSoundOpen2();
	void OnBtnSoundClose1();
	void OnBtnSoundClose2();

	//实际相应操作
	void OnBtnSoundOpen();
	void OnBtnSoundClose();
	
	//处理声音slider 位置的变化
	void OnSliderPos1(EventArgs *pEvt);
	void OnSliderPos2(EventArgs *pEvt);
	void OnSliderPos(bool isPos1);

protected:
	//soui消息
	EVENT_MAP_BEGIN()
		EVENT_NAME_COMMAND(L"btn_close", OnClose)
		EVENT_NAME_COMMAND(L"btn_min", OnMinimize)
		EVENT_NAME_COMMAND(L"btn_max", OnMaximize)
		EVENT_NAME_COMMAND(L"btn_restore", OnRestore)
		
		//添加各个页面按钮的消息映射
		EVENT_ID_COMMAND(R.id.btn_select_music_1 , OnBtnSelectMusic1)
		EVENT_ID_COMMAND(R.id.btn_select_lyric_1 , OnBtnSelectLyric1)
		EVENT_ID_COMMAND(R.id.btn_select_output  , OnBtnSelectOutput)
		EVENT_ID_COMMAND(R.id.btn_load_1 , OnBtnLoad1)
		EVENT_ID_COMMAND(R.id.btn_start_making , OnBtnStartMaking)
		EVENT_ID_COMMAND(R.id.btn_restart , OnBtnRestart)
		EVENT_ID_COMMAND(R.id.btn_preview , OnBtnPreview)
		EVENT_ID_COMMAND(R.id.btn_open_output , OnBtnOpenOutput)

		EVENT_ID_COMMAND(R.id.btn_select_music_2 , OnBtnSelectMusic2)
		EVENT_ID_COMMAND(R.id.btn_select_lyric_2 , OnBtnSelectLyric2)
		EVENT_ID_COMMAND(R.id.btn_load_2 , OnBtnLoad2)
		EVENT_ID_COMMAND(R.id.btn_end_preview , OnBtnEndPreview)
		EVENT_ID_COMMAND(R.id.btn_start_playing , OnBtnStartPlaying)
		EVENT_ID_COMMAND(R.id.btn_manual_adjust , OnBtnManualAdjust)
		
		
		EVENT_ID_COMMAND(R.id.btn_sound_open_1 , OnBtnSoundOpen1)
		EVENT_ID_COMMAND(R.id.btn_sound_open_2 , OnBtnSoundOpen2)
		EVENT_ID_COMMAND(R.id.btn_sound_close_1 , OnBtnSoundClose1)
		EVENT_ID_COMMAND(R.id.btn_sound_close_2 , OnBtnSoundClose2)
		
		EVENT_NAME_HANDLER(R.name.slider_sound_1, EVT_SLIDER_POS, OnSliderPos1)
		EVENT_NAME_HANDLER(R.name.slider_sound_2, EVT_SLIDER_POS, OnSliderPos2)

	EVENT_MAP_END()

	//HostWnd真实窗口消息处理
	BEGIN_MSG_MAP_EX(CMainDlg)

		MSG_WM_CREATE(OnCreate)
		MSG_WM_INITDIALOG(OnInitDialog)
		MSG_WM_CLOSE(OnClose)
		MSG_WM_SIZE(OnSize)

		//处理键盘按键消息
		MSG_WM_KEYDOWN(OnKeyDown)

		//接收 mciSendCommand 函数 执行音乐播放 相关操作时 发出的通知消息
		if (uMsg == MM_MCINOTIFY ) 
		{ 
			SetMsgHandled(TRUE); 
			OnMusicCommand((UINT)lParam, (UINT)(wParam)); 
			lResult = 0; 
			if(IsMsgHandled()) 
				return TRUE; 
		}

		MSG_WM_TIMER(OnTimer)

		CHAIN_MSG_MAP(SHostWnd)
		REFLECT_NOTIFICATIONS_EX()
	END_MSG_MAP()


public:
	//判断第一个页面(歌词制作) 的 三个路径是否都选择完毕 */
	static bool isPathReady_1()				
	{
		bool ret = true;
		for(int i=0; i< sizeof(PATH_STATE_1)/sizeof(PATH_STATE_1[0]); i++)
			if(!PATH_STATE_1[i].isInited)
			{
				ret = PATH_STATE_1[i].isInited;
				break;
			}
		return ret;
	}
	/* 获得当前未初始化的第一个路径 在PATH_STATE_1 中对应的下标;都初始化了则返回-1 */
	static int getPathNotReady_1()			
	{
		int index = -1;
		for(int i=0; i< sizeof(PATH_STATE_1)/sizeof(PATH_STATE_1[0]); i++)
			if(!PATH_STATE_1[i].isInited)
			{
				index = i;
				break;
			}
		return index;
	}

	//判断第二个页面(滚动预览) 的 两个路径是否都选择完毕 */
	static bool isPathReady_2()				
	{
		bool ret = true;
		for(int i=0; i< sizeof(PATH_STATE_2)/sizeof(PATH_STATE_2[0]); i++)
			if(!PATH_STATE_2[i].isInited)
			{
				ret = PATH_STATE_2[i].isInited;
				break;
			}
		return ret;
	}
	/* 获得当前未初始化的第一个路径 在PATH_STATE_2 中对应的下标;都初始化了则返回-1 */
	static int getPathNotReady_2()			
	{
		int index = -1;
		for(int i=0; i< sizeof(PATH_STATE_2)/sizeof(PATH_STATE_2[0]); i++)
			if(!PATH_STATE_2[i].isInited)
			{
				index = i;
				break;
			}
		return index;
	}


protected:
	//第一个页面(歌词制作)：回到“加载按钮”按下后的状态
	void backToInit_1();						/* 回到载入完成后的状态，此函数是为了复用而特意作为一个函数来使用 */
	//第二个页面(滚动预览)：回到“加载按钮”按下后的状态
	void backToInit_2();						

	//在滚动预览的页面，执行歌词显示 m_nCurLine
	void scrollToLyricCurLine();
private:
	//设置程序的背景图片
	void setBackSkin();

public:
	LyricMaker maker;						/* 歌词制作器 */
	LyricPlayer player;						/* 歌词滚动播放器 */

private:
	BOOL			m_bLayoutInited;
	BOOL			m_bIsLyricMaking;		/* 是否正在制作歌词当中，觉得是否处理键盘消息 ，在制作歌词过程中不能预览歌词*/
	BOOL			m_bIsLyricPlaying;		/* 是否正在预览歌词当中，在预览歌词过程中，不能制作歌词 */
	
	int m_nVolumn;							/* 当前音量 0 ~ 1000 */
	int m_nVolumnBeforeMute;				/* 储存静音前的音量*/

	// 程序的两个页面都需要播放音乐，在音乐自然结束时， void OnMusicCommand(UINT lParam, UINT wParam) 会被调用
	// 然而， lParam, wParam 都无法提供更多的关于 消息发送者的信息
	// 故在此定义FootPrintPage。
	// FootPrintPage 里保留着最后播放时所在的页面；由此决定当前 音乐结束是来自哪个页面
	// FootPrintPage 分别会在 歌词制作页面“开始制作”按钮，和滚动预览“开始播放”按钮被按下之后被记录页面状态,分别是 [0,1]
	int FootPrintPage;

};

namespace SOUI{

	//发送自定义图标的消息
	int _MessageBox(HWND hwnd,LPCTSTR content, LPCTSTR tiltle, UINT uType);

};