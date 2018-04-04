/*
*	Copyright (C) 2017  BensonLaur
*	note: Looking up header file for license detail
*/

// MainDlg.cpp :  主窗口类的实现	
//
/////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "MainDlg.h"
#include "FileHelper.h"
#include <helper/SDibHelper.h>
#include "BSMessageBox.h"

//经过个人测试，音乐播放设备，在毫秒级访问时，（比如获取歌曲当前位置）会有一定的延迟，导致声音已经被设备播放，
//但是在获取实时信息来显示时存在延迟，在此定义设备可能的延迟（单位：毫秒）
#define MUSIC_DIVICE_DELAY 300
	
#ifdef DWMBLUR	//win7毛玻璃开关
#include <dwmapi.h>
#pragma comment(lib,"dwmapi.lib")
#endif


//发送自定义图标的消息
int SOUI::_MessageBox(HWND hwnd,LPCTSTR content, LPCTSTR tiltle, UINT uType)
{
	BSMessageBox m;
	return m.MessageBoxW(hwnd,content,tiltle,uType);
}

CMainDlg::CMainDlg() : SHostWnd(_T("LAYOUT:XML_MAINWND"))
{
	m_bLayoutInited = FALSE;
}
CMainDlg::~CMainDlg()
{
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
	//设置背景图片
	setBackSkin();

	m_bLayoutInited = TRUE;
	
	//初始化 m_bIsLyricMaking 状态为 “未在制作中”，该状态可以锁定（忽略）键盘消息
	m_bIsLyricMaking =FALSE;
	//未在“播放预览”中
	m_bIsLyricPlaying = FALSE;
	
	//音量的初值为最大值1000
	this->m_nVolumn = 1000;

	//初始化记录页面播放足迹，详看变量的说明
	FootPrintPage = -1;

	return 0;
}

//TODO:消息映射
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
//设置程序窗口的背景图片
void CMainDlg::setBackSkin()
{
    SSkinImgList * pSkin = sobj_cast<SSkinImgList>(GETSKIN(_T("skin_bkimg"),GetScale()));
    SASSERT(pSkin);
    FindChildByID(R.id.img_skin_layer)->SetAttribute(L"skin",_T("skin_bkimg"));
	
	
    //COLORREF crAvg = SDIBHelper::CalcAvarageColor(pSkin->GetImage());
    //DoColorize(crAvg);
}

//键盘消息
void CMainDlg::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	int curPage;

	//获取当前所处页面
	STabCtrl *mainTab = FindChildByID2<STabCtrl>(R.id.tab_main);
	if(mainTab)
		curPage = mainTab->GetCurSel();

	if(curPage == 0)//来自第一页
	{
		//不在制作歌词的状态中，则不响应按键的状态
		if(!this->m_bIsLyricMaking) return;
		switch(nChar)
		{
			case VK_UP:
				maker.m_nCurLine += 1;
			
				if(maker.m_nCurLine <= maker.m_nTotalLine)
				{
					//更新显示的歌词
					//第一次第一行没有数据
					if(maker.m_nCurLine != 1)
					{
						SStatic *text1 = FindChildByID2<SStatic>(R.id.name_1_line_1);
						if(text1)
							text1->SetWindowTextW(maker.m_vLyricOrigin[maker.m_nCurLine-1-1]);
					}
					SStatic *text2 = FindChildByID2<SStatic>(65000);
					if(text2)
						text2->SetWindowTextW(maker.m_vLyricOrigin[maker.m_nCurLine-1]);
				
					SStatic *text3 = FindChildByID2<SStatic>(R.id.name_1_line_3);
					if(maker.m_nCurLine != maker.m_nTotalLine)
					{
						if(text3)
							text3->SetWindowTextW(maker.m_vLyricOrigin[maker.m_nCurLine-1+1]);
					}
					else//最后行后面数据为空
					{
						if(text3)
							text3->SetWindowTextW(_T(""));
					}

					//隐藏空行提示
					SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block);
					emptyTip->SetVisible(FALSE,TRUE);

					//标记生成下一行歌词
					maker.markNextLine();

					break ;
				}
				//else 执行“left”空出一行的操作

			case VK_LEFT:

				//更新页面显示
				if(maker.m_nCurLine == 0)
				{
					SStatic *text2 = FindChildByID2<SStatic>(65000);
					if(text2)
						text2->SetWindowTextW(_T(""));
				
					//显示空行提示
					SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block);
					emptyTip->SetVisible(TRUE,TRUE);
				}
				else if(maker.m_nCurLine <= maker.m_nTotalLine )  
				{
					//如果上一行不是空行，则需要更新显示面板的第一行和第二行
					if(!maker.isLastLineSpace())
					{
					
						SStatic *text1 = FindChildByID2<SStatic>(R.id.name_1_line_1);
						if(text1)
							text1->SetWindowTextW(maker.m_vLyricOrigin[maker.m_nCurLine-1]);
						SStatic *text2 = FindChildByID2<SStatic>(65000);
						if(text2)
							text2->SetWindowTextW(_T(""));

						//显示空行提示
						SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block);
						emptyTip->SetVisible(TRUE,TRUE);
				
					}
					//else 保持原来页面的显示
				}else if(maker.m_nCurLine == maker.m_nTotalLine+1)//此条件需要考虑到：在最后一行时按下VK_UP，则m_nCurLine m_nTotalLine +1
				{
					//如果上一行不是空行，则需要更新显示面板的第一行和第二行
					if(!maker.isLastLineSpace())
					{
						SStatic *text1 = FindChildByID2<SStatic>(R.id.name_1_line_1);
						if(text1)
							text1->SetWindowTextW(maker.m_vLyricOrigin[maker.m_nCurLine-1-1]);
						SStatic *text2 = FindChildByID2<SStatic>(65000);
						if(text2)
							text2->SetWindowTextW(_T(""));
				
						//显示空行提示
						SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block);
						emptyTip->SetVisible(TRUE,TRUE);
					}
				}
			
				//更新插入数据
				maker.markSpaceLine();

					break;
			case VK_RETURN:
				this->m_bIsLyricMaking = FALSE;

				//取得输出的文件名
				TCHAR outFileName[_MAX_FNAME];
				maker.getOutputFileName(outFileName);

				//提示用户是否保存
				int ret = _MessageBox(this->m_hWnd,SStringT().Format(_T("结束了制作!\\n\
																		是否保存到文件:\\n%s？ \
							"),outFileName),_T("保存提示"),MB_OKCANCEL|MB_ICONQUESTION);

				if(ret == IDOK)
				//如果确定保存，则保存入目标文件，并给出提示
				{
					maker.makingEnd();
				
					//取得输出的 路径文件名
					TCHAR outPathName[_MAX_FNAME];
					maker.getm_szOutputPathName(outPathName);

					//给出提示预览提示
					_MessageBox(this->m_hWnd,SStringT().Format(_T("歌词已保存到：\\n%s  \\n\
							效果预览方法：\\n点击本页面左下角的“效果预览”\\n\
							或 在左边切换页面“滚动预览” 自己选择文件。"),outPathName),_T("预览提示"),MB_OK|MB_ICONINFORMATION);
				}

				//重置状态
				backToInit_1();
			
				//改变预览按钮的状态
				SButton *btn_preview = FindChildByID2<SButton>(R.id.btn_preview);
				btn_preview->EnableWindow(TRUE,TRUE);
				SButton *btn_open_output = FindChildByID2<SButton>(R.id.btn_open_output);
				btn_open_output->EnableWindow(TRUE,TRUE);
		
				break;
		}
	}
	else //来自第二页
	{
		//不在预览歌词的状态中，则不响应按键的状态
		if(!this->m_bIsLyricPlaying) return;
		switch(nChar)
		{
			case VK_LEFT:
				//后退十秒
				player.m_musicPlayer.shift(-10*1000);
				player.m_musicPlayer.playAfterSeek();

				break;
			case VK_RIGHT:
				//前进十秒

				player.m_musicPlayer.shift(10*1000);
				player.m_musicPlayer.playAfterSeek();

				break;
			case VK_SPACE:
				
				DWORD status = player.m_musicPlayer.getModeStatus();
				if(status == MCI_MODE_PLAY)
				{
					player.m_musicPlayer.pause();
					//this->KillTimer(102);
				}
				else if(status == MCI_MODE_PAUSE)
				{
					player.m_musicPlayer.resume();
					//this->SetTimer(102,1);
				}

				break;
		}
	}

}


//各个页面的响应函数
/*
*	歌词制作页面的响应函数
*/

//三个路径的选择
void CMainDlg::OnBtnSelectMusic1()
{
	SButton *btn = FindChildByID2<SButton>(R.id.btn_select_music_1);
    if(btn)
    {
        CFileDialogEx openDlg(TRUE,_T("mp3"),0,6,_T("音频文件(mp3,mp2,wma,wav,wv,ape,flac)\0*.mp3;*.mp2;*.wma;*.wav;*.wv;*.ape;*.flac;\0手机录音(amr)/手机铃声(mmf)\0*amr;*.mmf\0\0"));
        if(openDlg.DoModal()==IDOK)
		{
			if( CFileDialogEx::checkPathName(_T("*.mp3"),openDlg.m_szFileName) 
			 || CFileDialogEx::checkPathName(_T("*.mp2"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.wma"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.wav"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.wv"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.ape"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.flac"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.amr"),openDlg.m_szFileName)
			 || CFileDialogEx::checkPathName(_T("*.mmf"),openDlg.m_szFileName))
			{	
				; //使用“或”条件判断( || )而 不用“且”条件判断（&&），以减少 checkPathName 调用的次数
			}
			else 
			{
				_MessageBox(this->m_hWnd,_T("格式不支持\\n请确定文件格式为【选择对话框指定的文件类型】"),_T("提示"),MB_OK|MB_ICONINFORMATION);
				return;
			}

			//显示新选择的文件
			SWindow *musicPath = FindChildByID(R.id.edit_music_1);
			musicPath->SetWindowTextW(openDlg.m_szFileName);

			PATH_STATE_1[0].isInited = true;

			//加入歌词制作器
			maker.setMusicPath(openDlg.m_szFileName,this->m_hWnd);
		}
    }
}

void CMainDlg::OnBtnSelectLyric1()
{
	SButton *btn = FindChildByID2<SButton>(R.id.btn_select_lyric_1);
    if(btn)
    {
        CFileDialogEx openDlg(TRUE,_T("txt"),0,6,_T("文本文件(*.txt)\0*.txt\0\0"));
        if(openDlg.DoModal()==IDOK)
		{
			if(!CFileDialogEx::checkPathName(_T("*.txt"),openDlg.m_szFileName))
			{
				_MessageBox(this->m_hWnd,_T("格式不支持\\n请确定文件格式为【*.txt】"),_T("提示"),MB_OK|MB_ICONINFORMATION);
				return;
			}
			
			//显示新选择的文件
			SWindow *lyricPath = FindChildByID(R.id.edit_lyric_1);
			lyricPath->SetWindowTextW(openDlg.m_szFileName);
			
			PATH_STATE_1[1].isInited = true;
			
			//加入歌词制作器
			maker.setLyricPath(openDlg.m_szFileName);
		}
    }

}

void CMainDlg::OnBtnSelectOutput()
{
	SButton *btn = FindChildByID2<SButton>(R.id.btn_select_output);
    if(btn)
    {
        CFileDialogEx openDlg(TRUE,_T("txt"),0,6,_T("文件夹\0..\0\0"),NULL,TRUE);//bFloder设置为TRUE，选择文件夹
        if(openDlg.DoModal()==IDOK)
		{
			if(!CFileDialogEx::checkPathName(_T(".."),openDlg.m_szFileName))
			{
				_MessageBox(this->m_hWnd,_T("格式不支持\\n请确定您选择【文件夹】"),_T("提示"),MB_OK|MB_ICONINFORMATION);
				return;
			}
			
			//显示新选择的文件
			SWindow *m_szOutputPath = FindChildByID(R.id.edit_output);
			m_szOutputPath->SetWindowTextW(openDlg.m_szFileName);
			
			PATH_STATE_1[2].isInited = true;
			
			//加入歌词制作器
			maker.setm_szOutputPath(openDlg.m_szFileName);
		}
    }
}

//制作歌词：加载 (初始化 LyricMaker)
void CMainDlg::OnBtnLoad1()
{
	//确保三个路径的数据都已经初始化
	if(!isPathReady_1())
	{
		//CMainDlg::getPathNotReady_1() 此时返回的正是 未初始化的路径 在 PATH_STATE 中对应的下标
		_MessageBox(this->m_hWnd,SStringT().Format(_T("您还没选择【%s】"),PATH_STATE_1[CMainDlg::getPathNotReady_1()].nameOfPath),
			_T("提示"),MB_OK|MB_ICONASTERISK);
		return;
	}

	//确保 歌词文件 和 音乐文件有效
	File lyricFile(maker.m_szLyricPathName,_T("r"));

	if(!lyricFile.isValidFile()) 
	{
		_MessageBox(this->m_hWnd,SStringT().Format(_T("文件打开失败:\\n【%s】\\n!请确保文件有效"),maker.m_szLyricPathName),
			_T("失败提示"),MB_OK|MB_ICONWARNING);
		return;
	}

	//更新 页面的 当前音乐 和 当前歌词的信息
	SStatic *text_music = FindChildByID2<SStatic>(R.id.line_music_1);
	if(text_music)
		text_music->SetWindowTextW(maker.m_szMusicPathName);
	SStatic *text_lyric = FindChildByID2<SStatic>(R.id.line_lyric_1);
	if(text_lyric)
		text_lyric->SetWindowTextW(maker.m_szLyricPathName);
	
	//重置 LyricMaker的 歌词数据为空
	maker.reloadMaker();
	
	char _line[MAX_CHAR_COUNT_OF_LINE+1]; 
	wchar_t line[MAX_WCHAR_COUNT_OF_LINE+1];
	
	// 从文件中读取歌词，并将非空行加入到 maker.m_vLyricOrigin 向量中
	while(fgets(_line,MAX_WCHAR_COUNT_OF_LINE,lyricFile.m_pf))
	{
		_line[MAX_CHAR_COUNT_OF_LINE]='\0';//保证在最后一个字符处截断字符串

		//由变长字符转换为 unicode 宽字节字符
		MultiByteToWideChar(CP_ACP,MB_COMPOSITE,_line,strlen(_line)+1,line,MAX_WCHAR_COUNT_OF_LINE+1);
		SStringT aLine(line);
		aLine.Trim('\n');
		aLine.Trim(' ');
		aLine.Trim('\t');
		if(aLine.GetLength() ==0) 
			continue;

		maker.m_vLyricOrigin.push_back(aLine);
	}
	 
	//读取完毕，清空显示面板，显示第一条非空歌词
	SStatic *text1 = FindChildByID2<SStatic>(R.id.name_1_line_1);
	if(text1)
		text1->SetWindowTextW(_T(""));
	SStatic *text2 = FindChildByID2<SStatic>(65000);
	if(text2)
		text2->SetWindowTextW(_T(""));
	SStatic *text3 = FindChildByID2<SStatic>(R.id.name_1_line_3);
	if(text3)
		text3->SetWindowTextW(maker.m_vLyricOrigin[0]);

	//改变按钮的状态
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_making);
	btn_start->EnableWindow(TRUE,TRUE); //第一个参数bEnable为 是否启用，第二个参数bUpdate为是否在改变状态后更新显示
	//btn_start->Invalidate();			//如果没有指定第二个参数bUpdate，其默认值为 FALSE，要刷新则需要自己Invalidate()
	
	//改变预览按钮的状态
	SButton *btn_preview = FindChildByID2<SButton>(R.id.btn_preview);
	btn_preview->EnableWindow(FALSE,TRUE);
	SButton *btn_open_output = FindChildByID2<SButton>(R.id.btn_open_output);
	btn_open_output->EnableWindow(FALSE,TRUE);
}

//播放预览刚刚制作完成的带时间时间轴歌词
void CMainDlg::OnBtnPreview()
{
	//切换到预览页面
	STabCtrl* tab = FindChildByID2<STabCtrl>(R.id.tab_main);
	if(tab)
		tab->SetCurSel(1);

	Sleep(500);

	//设置player的状态
	player.setMusicPath(maker.m_szMusicPathName,this->m_hWnd);
	player.setLyricPath(maker.m_szOutputPathName);
	
	PATH_STATE_2[0].isInited = true;
	PATH_STATE_2[1].isInited = true;

	//设置页面 路径名，和当前选择的音乐和歌词（“载入准备”按下之后的状态）
	//显示新选择的文件
	SWindow *musicPath = FindChildByID(R.id.edit_music_2);
	musicPath->SetWindowTextW(player.m_szMusicPathName);
	//显示新选择的文件
	SWindow *lyricPath = FindChildByID(R.id.edit_lyric_2);
	lyricPath->SetWindowTextW(player.m_szLyricPathName);

	

	OnBtnLoad2();
}

//打开生成的文件
void CMainDlg::OnBtnOpenOutput()
{
	if(_tcslen(maker.m_szOutputPath)!=0)
	{
		//打开文件夹
		WinExec(S_CW2A(SStringT().Format(_T("explorer \"%s\""),maker.m_szOutputPath)),SW_SHOWNORMAL);
		//用window默认编辑器打开歌词文件
		WinExec(S_CW2A(SStringT().Format(_T("notepad \"%s\""),maker.m_szOutputPathName)),SW_SHOWNORMAL);
	}
}

//第一个页面(歌词制作)：回到“加载按钮”按下后的状态
void CMainDlg::backToInit_1()
{
	maker.m_vLyricOutput.clear();
	maker.stopMusic();

	//重置显示面板，显示第一条非空歌词
	SStatic *text1 = FindChildByID2<SStatic>(R.id.name_1_line_1);
	if(text1)
		text1->SetWindowTextW(_T(""));
	SStatic *text2 = FindChildByID2<SStatic>(65000);
	if(text2)
		text2->SetWindowTextW(_T(""));
	SStatic *text3 = FindChildByID2<SStatic>(R.id.name_1_line_3);
	if(text3)
		text3->SetWindowTextW(maker.m_vLyricOrigin[0]);

	//隐藏空行提示
	SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block);
	if(emptyTip->IsVisible())
		emptyTip->SetVisible(FALSE,TRUE);
	
	//改变按钮的状态
	SButton *btn_load = FindChildByID2<SButton>(R.id.btn_load_1);
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_making);
	SButton *btn_restart = FindChildByID2<SButton>(R.id.btn_restart);
	
	btn_load->EnableWindow(TRUE,TRUE);
	btn_start->EnableWindow(TRUE,TRUE);
	btn_restart->EnableWindow(FALSE,TRUE);
	
	//取消歌词显示面板中的“当前行”的高亮背景
	SWindow *highLight = FindChildByID(R.id.highlight_bkgnd);
	if(highLight)
		highLight->SetVisible(FALSE,TRUE);

	//重置时间标签和进度条
	SStatic* timeLabel_1 = FindChildByID2<SStatic>(R.id.text_time_1);
	SProgress* timeProgress_1 = FindChildByID2<SProgress>(R.id.progress_music_1);
	if(timeLabel_1)
		timeLabel_1->SetWindowTextW(_T("00:00.000"));
	if(timeProgress_1)
		timeProgress_1->SetValue( 0 );//设置千分数值

	//改变状态  锁定按键信息
	this->m_bIsLyricMaking = FALSE;

	this->KillTimer(101);
}

//我要重制
void CMainDlg::OnBtnRestart()
{
	//重置状态
	backToInit_1();
}

//开始制作
void CMainDlg::OnBtnStartMaking()
{
	//如果正在：播放预览歌词 的状态，则不允许制作歌词
	if(m_bIsLyricPlaying == TRUE)
	{
		_MessageBox(this->m_hWnd,_T("请先结束【歌词滚动预览】！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
		return;
	}

	//改变相应的数据状态
	this->m_bIsLyricMaking = TRUE;
	maker.makingStart();
	
	//改变按钮的状态
	SButton *btn_load = FindChildByID2<SButton>(R.id.btn_load_1);
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_making);
	SButton *btn_restart = FindChildByID2<SButton>(R.id.btn_restart);

	btn_load->EnableWindow(FALSE,TRUE);
	btn_start->EnableWindow(FALSE,TRUE);
	btn_restart->EnableWindow(TRUE,TRUE);

	
	//改变预览按钮的状态
	SButton *btn_preview = FindChildByID2<SButton>(R.id.btn_preview);
	btn_preview->EnableWindow(FALSE,TRUE);
	SButton *btn_output_path = FindChildByID2<SButton>(R.id.btn_open_output);
	btn_output_path->EnableWindow(FALSE,TRUE);

	//显示歌词显示面板中的“当前行”的高亮背景
	SWindow *highLight = FindChildByID(R.id.highlight_bkgnd);
	if(highLight)
		highLight->SetVisible(TRUE,TRUE);

	this->SetTimer(101,1);

	//记录页面播放足迹，详看变量的说明
	this->FootPrintPage = 0;
}


/*
*	滚动预览页面的响应函数
*/

//两个路径的选择
void CMainDlg::OnBtnSelectMusic2()
{
	SButton *btn = FindChildByID2<SButton>(R.id.btn_select_music_2);
    if(btn)
    {
        CFileDialogEx openDlg(TRUE,_T("mp3"),0,6,_T("MP3 文件(*.mp3)\0*.mp3\0\0"));
        if(openDlg.DoModal()==IDOK)
		{
			if(!CFileDialogEx::checkPathName(_T("*.mp3"),openDlg.m_szFileName))
			{
				_MessageBox(this->m_hWnd,_T("格式不支持\\n请确定文件格式为【*.mp3】"),_T("提示"),MB_OK|MB_ICONINFORMATION);
				return;
			}

			//显示新选择的文件
			SWindow *musicPath = FindChildByID(R.id.edit_music_2);
			musicPath->SetWindowTextW(openDlg.m_szFileName);

			PATH_STATE_2[0].isInited = true;

			//歌词播放器
			player.setMusicPath(openDlg.m_szFileName, this->m_hWnd);
		}
    }

}

void CMainDlg::OnBtnSelectLyric2()
{
	SButton *btn = FindChildByID2<SButton>(R.id.btn_select_lyric_2);
    if(btn)
    {
        CFileDialogEx openDlg(TRUE,_T("txt"),0,6,_T("LRC文本文件(*.lrc)\0*.lrc\0\0"));
        if(openDlg.DoModal()==IDOK)
		{
			if(!CFileDialogEx::checkPathName(_T("*.lrc"),openDlg.m_szFileName))
			{
				_MessageBox(this->m_hWnd,_T("格式不支持\\n请确定文件格式为【*.lrc】"),_T("提示"),MB_OK|MB_ICONINFORMATION);
				return;
			}
			
			//显示新选择的文件
			SWindow *lyricPath = FindChildByID(R.id.edit_lyric_2);
			lyricPath->SetWindowTextW(openDlg.m_szFileName);
			
			PATH_STATE_2[1].isInited = true;
			
			//加入歌词播放器
			player.setLyricPath(openDlg.m_szFileName);
		}
    }
}

void CMainDlg::OnBtnLoad2()
{
	//确保两个个路径的数据都已经初始化
	if(!isPathReady_2())
	{
		//CMainDlg::getPathNotReady_2() 此时返回的正是 未初始化的路径 在 PATH_STATE 中对应的下标
		_MessageBox(this->m_hWnd,SStringT().Format(_T("您还没选择：\\n【%s】"),PATH_STATE_2[CMainDlg::getPathNotReady_2()].nameOfPath),
			_T("提示"),MB_OK|MB_ICONINFORMATION);
		return;
	}

	//确保 歌词文件 和 音乐文件 路径有效
	File lyricFile(player.m_szLyricPathName,_T("r"));

	if(!lyricFile.isValidFile()) 
	{
		_MessageBox(this->m_hWnd,SStringT().Format(_T("文件打开失败:\\n【%s】\\n请确保文件有效!"),player.m_szLyricPathName),
			_T("失败提示"),MB_OK|MB_ICONWARNING);
		return;
	}

	//重置 LyricPlayer的 歌词数据为空
	player.reloadPlayer();

	char _line[MAX_CHAR_COUNT_OF_LINE+1];
	wchar_t line[MAX_WCHAR_COUNT_OF_LINE+1];
	
	// 从文件中读取歌词，并将非空行加入到 player.m_vTimeLyric 向量中
	while(fgets(_line,MAX_WCHAR_COUNT_OF_LINE,lyricFile.m_pf))
	{
		_line[MAX_CHAR_COUNT_OF_LINE]='\0';//保证在最后一个字符处截断字符串

		//由变长字符装换为 unicode 宽字节字符
		MultiByteToWideChar(CP_ACP,MB_COMPOSITE,_line,strlen(_line)+1,line,MAX_WCHAR_COUNT_OF_LINE+1);
		SStringT aLine(line);
		aLine.Trim('\n');
		aLine.Trim(' ');
		aLine.Trim('\t');
		if(aLine.GetLength() ==0)
			continue;

		//生成一个行信息对象，储存到player.m_vLineInfo向量中
		TimeLineInfo newInfo(aLine);
		player.m_vLineInfo.push_back(newInfo);
	}

	//没有歌词，不能播放
	if(player.m_vLineInfo.empty())
	{
		//清空 （可能存在的）页面的 当前音乐 和 当前歌词的信息
		SStatic *text_music = FindChildByID2<SStatic>(R.id.line_music_2);
		if(text_music)
			text_music->SetWindowTextW(_T(""));
		SStatic *text_lyric = FindChildByID2<SStatic>(R.id.line_lyric_2);
		if(text_lyric)
			text_lyric->SetWindowTextW(_T(""));
		//清空 （可能存在的）第三行歌词
		SStatic *text3 = FindChildByID2<SStatic>(R.id.name_2_line_3);
		if(text3)
			text3->SetWindowTextW(_T(""));

		//禁用 （可能启用的）开始按钮
		SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_playing);
		btn_start->EnableWindow(FALSE,TRUE); //第一个参数bEnable为 是否启用，第二个参数bUpdate为是否在改变状态后更新显示

		//禁用 （可能启用的）手动微调时间轴
		SButton *btn_adjust = FindChildByID2<SButton>(R.id.btn_manual_adjust);
		btn_adjust->EnableWindow(FALSE,TRUE); 

		_MessageBox(this->m_hWnd,SStringT().Format(_T("当前歌词文件没有可播放内容！\\n文件：\\n【%s】"),player.m_szLyricPathName),
			_T("失败提示"),MB_OK|MB_ICONWARNING);
		return;
	}

	//更新 页面的 当前音乐 和 当前歌词的信息
	SStatic *text_music = FindChildByID2<SStatic>(R.id.line_music_2);
	if(text_music)
		text_music->SetWindowTextW(player.m_szMusicPathName);
	SStatic *text_lyric = FindChildByID2<SStatic>(R.id.line_lyric_2);
	if(text_lyric)
		text_lyric->SetWindowTextW(player.m_szLyricPathName);


	//更新化页面的显示
	//读取完毕，清空显示面板，显示第一条非空歌词
		SStatic *text1 = FindChildByID2<SStatic>(R.id.name_2_line_1);
	if(text1)
		text1->SetWindowTextW(_T(""));
	SStatic *text2 = FindChildByID2<SStatic>(65001);
	if(text2)
		text2->SetWindowTextW(_T(""));
	SStatic *text3 = FindChildByID2<SStatic>(R.id.name_2_line_3);
	if(text3)
		text3->SetWindowTextW(player.m_vLineInfo[0].m_strLine);

	//改变按钮的状态
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_playing);
	btn_start->EnableWindow(TRUE,TRUE); //第一个参数bEnable为 是否启用，第二个参数bUpdate为是否在改变状态后更新显示

	//启用用 “手动微调时间轴”按钮
	SButton *btn_adjust = FindChildByID2<SButton>(R.id.btn_manual_adjust);
	btn_adjust->EnableWindow(TRUE,TRUE); 

}

//第二个页面(滚动预览)：回到“加载按钮”按下后的状态
void CMainDlg::backToInit_2()
{
	//结束播放和歌词滚动
	player.playingEnd(this);

	//重置显示面板，显示第一条非空歌词
	SStatic *text1 = FindChildByID2<SStatic>(R.id.name_2_line_1);
	if(text1)
		text1->SetWindowTextW(_T(""));
	SStatic *text2 = FindChildByID2<SStatic>(65001);
	if(text2)
		text2->SetWindowTextW(_T(""));
	SStatic *text3 = FindChildByID2<SStatic>(R.id.name_2_line_3);
	if(text3)
		text3->SetWindowTextW(player.m_vLineInfo[0].m_strLine);

	//隐藏空行提示
	SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block_2);
	if(emptyTip->IsVisible())
		emptyTip->SetVisible(FALSE,TRUE);

	//改变按钮的状态
	SButton *btn_load = FindChildByID2<SButton>(R.id.btn_load_2);
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_playing);
	SButton *btn_end_preview = FindChildByID2<SButton>(R.id.btn_end_preview);
	
	btn_load->EnableWindow(TRUE,TRUE);
	btn_start->EnableWindow(TRUE,TRUE);
	btn_end_preview->EnableWindow(FALSE,TRUE);
	
	//取消歌词显示面板中的“当前行”的高亮背景
	SWindow *highLight = FindChildByID(R.id.highlight_bkgnd_2);
	if(highLight)
		highLight->SetVisible(FALSE,TRUE);

	//重置时间标签和进度条
	SStatic* timeLabel_2 = FindChildByID2<SStatic>(R.id.text_time_2);
	SProgress* timeProgress_2 = FindChildByID2<SProgress>(R.id.progress_music_2);
	if(timeLabel_2)
		timeLabel_2->SetWindowTextW(_T("00:00.000"));
	if(timeProgress_2)
		timeProgress_2->SetValue( 0 );//设置千分数值

	//结束“播放预览”状态
	m_bIsLyricPlaying = FALSE;
}

//结束预览播放，重置状态
void CMainDlg::OnBtnEndPreview()
{	
	//重置状态
	backToInit_2();
}

//开始播放歌词
void CMainDlg::OnBtnStartPlaying()
{
	//如果正在：制作歌词 的状态，则不允许播放预览歌词
	if(m_bIsLyricMaking == TRUE)
	{
		_MessageBox(this->m_hWnd,_T("请先结束【歌词制作】！"),_T("提示"),MB_OK|MB_ICONINFORMATION);
		return;
	}

	//开始播放和歌词滚动
	player.playingStart(this);

	//改变按钮的状态
	SButton *btn_load = FindChildByID2<SButton>(R.id.btn_load_2);
	SButton *btn_start = FindChildByID2<SButton>(R.id.btn_start_playing);
	SButton *btn_end_preview = FindChildByID2<SButton>(R.id.btn_end_preview);
	
	btn_load->EnableWindow(FALSE,TRUE);
	btn_start->EnableWindow(FALSE,TRUE);
	btn_end_preview->EnableWindow(TRUE,TRUE);
	
	//显示歌词显示面板中的“当前行”的高亮背景
	SWindow *highLight = FindChildByID(R.id.highlight_bkgnd_2);
	if(highLight)
		highLight->SetVisible(TRUE,TRUE);

	//进入“播放预览”状态
	m_bIsLyricPlaying = TRUE;

	//记录页面播放足迹，详看变量的说明
	this->FootPrintPage = 1;
}

void CMainDlg::OnBtnManualAdjust()
{	
	if(_tcslen(player.m_szLyricPathName)!=0)
	{
		//用window默认编辑器打开歌词文件
		WinExec(S_CW2A(SStringT().Format(_T("notepad \"%s\""),player.m_szLyricPathName)),SW_SHOWNORMAL);
	}
}

//接收音乐消息
//参数说明：https://msdn.microsoft.com/en-us/library/dd757358(v=vs.85).aspx
//wParam = (WPARAM) wFlags 
//lParam = (LONG) lDevID
void CMainDlg::OnMusicCommand(UINT lParam, UINT wParam)
{
	switch(wParam)
	{
	case MCI_NOTIFY_ABORTED:	//The device received a command that prevented the current conditions for initiating the callback function from being met. If a new command interrupts the current command and it also requests notification, the device sends this message only and not MCI_NOTIFY_SUPERSEDED
		//MB(_T("MCI_NOTIFY_ABORTED"));
		//播放中的音乐被stop时会发送该消息

		//不做处理
		break;
	case MCI_NOTIFY_FAILURE	://A device error occurred while the device was executing the command.
		break;
	case MCI_NOTIFY_SUCCESSFUL:	//The conditions initiating the callback function have been met.
		//MB(_T("MCI_NOTIFY_SUCCESSFUL"));
		//成功执行完动作后会发送该消息

		//FootPrintPage 里保留着最后播放时所在的页面；由此决定当前 音乐结束是来自哪个页面

		if(FootPrintPage == 0) //第1页（来自制作歌词页面）
		{
			//发送回车按键消息
			SendMessage(WM_KEYDOWN, VK_RETURN );  //本程序只处理 wParam 参数，wParam置为 VK_RETURN
		}
		else if(FootPrintPage == 1) //第1页（来自制作歌词页面）
		{
			//发送 “结束预览”时所触发的事件
			SButton* btn = FindChildByID2<SButton>(R.id.btn_end_preview);//这一句不是必要的，且当结束时刻页面不在第二页则会获取失败
			SOUI::EventCmd Evt(btn);				//初始化EventCmd需要一个参数
			Evt.idFrom = R.id.btn_end_preview;		//在这里只要事件对象是EventCmd，idFrom是按钮id，即可模拟调用按钮响应函数

			//自己调用 soui 消息处理函数
			this->_HandleEvent(&Evt);
		}

		break;
	case MCI_NOTIFY_SUPERSEDED: //The device received another command with the "notify" flag set and the current conditions for initiating the callback function have been superseded.
		break;
	}
}


//接收定时器消息，用于滚动歌词动画的实现
void CMainDlg::OnTimer(UINT_PTR nIDEvent)
{
	static int curMSecond, totalLength;
	static int minute,ms; 
	static double second;
	static TCHAR timeBuf[20];
	SStatic* timeLabel_1 = FindChildByID2<SStatic>(R.id.text_time_1);
	SProgress* timeProgress_1 = FindChildByID2<SProgress>(R.id.progress_music_1);
	SStatic* timeLabel_2 = FindChildByID2<SStatic>(R.id.text_time_2);
	SProgress* timeProgress_2 = FindChildByID2<SProgress>(R.id.progress_music_2);

	STabCtrl* tab = FindChildByID2<STabCtrl>(R.id.tab_main);

	switch(nIDEvent)
	{
	case 101:
		if(tab)
			if(tab->GetCurSel() == 0) //在歌词制作页面
			{
				if(this->m_bIsLyricMaking)
				{
					curMSecond = maker.m_musicPlayer.getPosition();  //毫秒时间
				
					//更新页面时间显示标签
					minute = (curMSecond)/60000;  
					ms  = (curMSecond)%60000;//由于歌词的内容的显示 提前了 MUSIC_DIVICE_DELAY 毫秒，虽然看上去滚动不会有延迟了，但是暂停时却暴
														 //露了这种 MUSIC_DIVICE_DELAY 毫秒 的差距（已显示歌词却时间没跟上），故在此加  MUSIC_DIVICE_DELAY 毫秒 显示
					second = ms *1.0/1000;
					_stprintf(timeBuf,_T("[%02d:%06.3lf]"),minute,second);

					if(timeLabel_1)
						timeLabel_1->SetWindowTextW(timeBuf);
	
					//更新进度条的显示
					if(timeProgress_1)
						timeProgress_1->SetValue( int(curMSecond * 1.0 / maker.m_musicPlayer.getLength() * 1000) ); //设置千分数值
				}

			}
	case 102:
		if(tab)
			if(tab->GetCurSel() == 1) //在歌词预览的页面
			{
				if(this->m_bIsLyricPlaying)
				{
					curMSecond = player.m_musicPlayer.getPosition();  //毫秒时间

					player.updateCurLine(); //如果快进或者后退都会导致，当前行发生变化，故需要先更新再判断

					if(player.m_nCurLine==0)//下面循环需要m_nCurLine从1开始
						player.m_nCurLine=1;

					while(player.m_nCurLine <= player.m_nTotalLine ) //准备循环读取所有未被 显示处理的行
					{
						if(player.m_vLineInfo[player.m_nCurLine -1 ].m_nmSesonds -MUSIC_DIVICE_DELAY > curMSecond  ) //设备访问会有延迟 故提前 MUSIC_DIVICE_DELAY 毫秒显示歌词
							break;	//歌词时间 大于 实际经过时间，还不用处理，跳出处理循环
					
						//执行面板滚动到 m_nCurLine
						scrollToLyricCurLine();

						//滚动完毕，自增一行
						player.m_nCurLine ++;
					}

					//更新页面时间显示标签
					minute = (curMSecond+ MUSIC_DIVICE_DELAY )/60000;  
					ms  = (curMSecond+ MUSIC_DIVICE_DELAY )%60000;//由于歌词的内容的显示 提前了 MUSIC_DIVICE_DELAY 毫秒，虽然看上去滚动不会有延迟了，但是暂停时却暴
														 //露了这种 MUSIC_DIVICE_DELAY 毫秒 的差距（已显示歌词却时间没跟上），故在此加  MUSIC_DIVICE_DELAY 毫秒 显示
					second = ms *1.0/1000;
					_stprintf(timeBuf,_T("[%02d:%06.3lf]"),minute,second);

					if(timeLabel_2)
						timeLabel_2->SetWindowTextW(timeBuf);
	
					//更新进度条的显示
					if(timeProgress_2)
						timeProgress_2->SetValue( int(curMSecond * 1.0 / player.m_musicPlayer.getLength() * 1000) );//设置千分数值
				}

			}
		break;
	default:
		SetMsgHandled(FALSE);
		//SHostWnd::OnTimer(nIDEvent);
	}
}

//在滚动预览的页面，执行歌词显示 m_nCurLine
void CMainDlg::scrollToLyricCurLine()
{
	//player.m_nCurLine 的范围是1 ~ player.m_nTotalLine

	//这里每个条件里的第二行本来可以拿出来先执行，但是由于还我不了解更新是依次刷新还是 一次全刷新，
	//为保证显示顺序从上到下能依次刷新，这里选择累赘一点的分别写在各个条件中
	if(player.m_nCurLine == 1)
	{
		//更新第二行和第三行
		SStatic *text2 = FindChildByID2<SStatic>(65001);
		if(text2)
			text2->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine -1].m_strLine);

		SStatic *text3 = FindChildByID2<SStatic>(R.id.name_2_line_3);
		if(text3)
			text3->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine+1 -1].m_strLine);
	}
	else if(player.m_nCurLine < player.m_nTotalLine)
	{
		//更新三行
		SStatic *text1 = FindChildByID2<SStatic>(R.id.name_2_line_1);
		if(text1)
			text1->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine-1 -1].m_strLine);

		SStatic *text2 = FindChildByID2<SStatic>(65001);
		if(text2)
			text2->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine -1].m_strLine);

		SStatic *text3 = FindChildByID2<SStatic>(R.id.name_2_line_3);
		if(text3)
			text3->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine+1 -1].m_strLine);
	}
	else // player.m_nCurLine == player.m_nTotalLine
	{
		//更新第一行和第二行
		SStatic *text1 = FindChildByID2<SStatic>(R.id.name_2_line_1);
		if(text1)
			text1->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine-1 -1].m_strLine);

		SStatic *text2 = FindChildByID2<SStatic>(65001);
		if(text2)
			text2->SetWindowTextW(player.m_vLineInfo[player.m_nCurLine -1].m_strLine);
	}


	//根据歌词是否为空，显示或隐藏空行提示
	SStatic *emptyTip = FindChildByID2<SStatic>(R.id.empty_tip_block_2);
	if(emptyTip)
		emptyTip->SetVisible(player.m_vLineInfo[player.m_nCurLine -1].m_bIsEmptyLine,TRUE);
}



//相应2个页面共四个静音按钮
void CMainDlg::OnBtnSoundOpen1()
{
	OnBtnSoundOpen();
}

void CMainDlg::OnBtnSoundOpen2()
{
	OnBtnSoundOpen();
}

void CMainDlg::OnBtnSoundClose1()
{
	OnBtnSoundClose();
}

void CMainDlg::OnBtnSoundClose2()
{
	OnBtnSoundClose();
}


//实际相应操作
void CMainDlg::OnBtnSoundOpen()
{
	//MB(SStringT().Format((_T("volumn: %d")),player.m_musicPlayer.getVolumn()));

	this->m_nVolumnBeforeMute = this->m_nVolumn;

	//设置音乐播放器的音量
	maker.m_musicPlayer.setVolumn(0);
	player.m_musicPlayer.setVolumn(0);

	// 更新两个页面的静音按钮的显示状态
	SButton * btnOpen_1 = FindChildByID2<SButton>(R.id.btn_sound_open_1);
	SButton * btnOpen_2 = FindChildByID2<SButton>(R.id.btn_sound_open_2);
	SButton * btnClose_1 = FindChildByID2<SButton>(R.id.btn_sound_close_1);
	SButton * btnClose_2 = FindChildByID2<SButton>(R.id.btn_sound_close_2);

	if(btnOpen_1)
		btnOpen_1->SetVisible(FALSE,TRUE);
	if(btnOpen_2)
		btnOpen_2->SetVisible(FALSE,TRUE);
	if(btnClose_1)
		btnClose_1->SetVisible(TRUE,TRUE);
	if(btnClose_2)
		btnClose_2->SetVisible(TRUE,TRUE);
	
	//更新音量sound bar的位置显示状态
	SSliderBar* bar1 = FindChildByID2<SSliderBar>(R.id.slider_sound_1);
	if(bar1)
		bar1->SetValue(0);
	SSliderBar* bar2 = FindChildByID2<SSliderBar>(R.id.slider_sound_2);
	if(bar2)
		bar2->SetValue(0);
}

void CMainDlg::OnBtnSoundClose()
{
	//设置音乐播放器的音量
	maker.m_musicPlayer.setVolumn(this->m_nVolumn);
	player.m_musicPlayer.setVolumn(this->m_nVolumn);

	// 更新两个页面的静音按钮的显示状态
	SButton * btnOpen_1 = FindChildByID2<SButton>(R.id.btn_sound_open_1);
	SButton * btnOpen_2 = FindChildByID2<SButton>(R.id.btn_sound_open_2);
	SButton * btnClose_1 = FindChildByID2<SButton>(R.id.btn_sound_close_1);
	SButton * btnClose_2 = FindChildByID2<SButton>(R.id.btn_sound_close_2);

	if(btnOpen_1)
		btnOpen_1->SetVisible(TRUE,TRUE);
	if(btnOpen_2)
		btnOpen_2->SetVisible(TRUE,TRUE);
	if(btnClose_1)
		btnClose_1->SetVisible(FALSE,TRUE);
	if(btnClose_2)
		btnClose_2->SetVisible(FALSE,TRUE);

	//更新音量sound bar的位置显示状态
	SSliderBar* bar1 = FindChildByID2<SSliderBar>(R.id.slider_sound_1);
	if(bar1)
		bar1->SetValue(this->m_nVolumnBeforeMute * 100 /1000);
	SSliderBar* bar2 = FindChildByID2<SSliderBar>(R.id.slider_sound_2);
	if(bar2)
		bar2->SetValue(this->m_nVolumnBeforeMute  * 100 /1000);

	//恢复声音的值
	maker.m_musicPlayer.setVolumn(this->m_nVolumnBeforeMute);
	this->m_nVolumn = player.m_musicPlayer.setVolumn(this->m_nVolumnBeforeMute);
}


//处理声音slider 位置的变化
void  CMainDlg::OnSliderPos1(EventArgs *pEvt)
{
	OnSliderPos(true);
}

void  CMainDlg::OnSliderPos2(EventArgs *pEvt)
{
	OnSliderPos(false);
}

void  CMainDlg::OnSliderPos(bool isPos1)
{
	int volumn = this->m_nVolumn;
	int value = this->m_nVolumn * 100 / 1000;

	//同步两个音量条的显示
	if(isPos1)
	{
		//获得  slider_sound_1 的值，决定设置的音量
		SSliderBar* bar1 = FindChildByID2<SSliderBar>(R.id.slider_sound_1);
		if(bar1)
			 value = bar1->GetValue();

		//更新 slider_sound_2 的位置
		SSliderBar* bar2 = FindChildByID2<SSliderBar>(R.id.slider_sound_2);
		if(bar2)
			  bar2->SetValue(value);
	}
	else
	{
		//获得  slider_sound_2 的值，决定设置的音量
		SSliderBar* bar2 = FindChildByID2<SSliderBar>(R.id.slider_sound_2);
		if(bar2)
			 value = bar2->GetValue();

		//更新 slider_sound_1 的位置
		SSliderBar* bar1 = FindChildByID2<SSliderBar>(R.id.slider_sound_1);
		if(bar1)
			  bar1->SetValue(value);
	}
	
	//两个音乐播放器都更新音量
	volumn = value  * 1000 / 100;
	maker.m_musicPlayer.setVolumn(volumn);
	this->m_nVolumn = player.m_musicPlayer.setVolumn(volumn);
	
	//根据当前音量是否为0，改变静音按钮的显示
	if(value==0)
	{
		// 更新两个页面的静音按钮的显示状态
		SButton * btnOpen_1 = FindChildByID2<SButton>(R.id.btn_sound_open_1);
		SButton * btnOpen_2 = FindChildByID2<SButton>(R.id.btn_sound_open_2);
		SButton * btnClose_1 = FindChildByID2<SButton>(R.id.btn_sound_close_1);
		SButton * btnClose_2 = FindChildByID2<SButton>(R.id.btn_sound_close_2);

		if(btnOpen_1)
			btnOpen_1->SetVisible(FALSE,TRUE);
		if(btnOpen_2)
			btnOpen_2->SetVisible(FALSE,TRUE);
		if(btnClose_1)
			btnClose_1->SetVisible(TRUE,TRUE);
		if(btnClose_2)
			btnClose_2->SetVisible(TRUE,TRUE);
	}
	else
	{
		// 更新两个页面的静音按钮的显示状态
		SButton * btnOpen_1 = FindChildByID2<SButton>(R.id.btn_sound_open_1);
		SButton * btnOpen_2 = FindChildByID2<SButton>(R.id.btn_sound_open_2);
		SButton * btnClose_1 = FindChildByID2<SButton>(R.id.btn_sound_close_1);
		SButton * btnClose_2 = FindChildByID2<SButton>(R.id.btn_sound_close_2);

		if(btnOpen_1)
			btnOpen_1->SetVisible(TRUE,TRUE);
		if(btnOpen_2)
			btnOpen_2->SetVisible(TRUE,TRUE);
		if(btnClose_1)
			btnClose_1->SetVisible(FALSE,TRUE);
		if(btnClose_2)
			btnClose_2->SetVisible(FALSE,TRUE);
	}
}