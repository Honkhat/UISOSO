// soui_helloworld.cpp : 定义应用程序的入口点。
//

#include "stdafx.h"
#include "soui_helloworld.h"
//#include "com-loader.hpp"
#include "MainWnd.h"
#include "SToolbox.h"
#include "SImageSwitcher.h"

#ifdef _DEBUG
#define	COM_IMGDECODER	_T("imgdecoder-wicd.dll")
#define COM_RENDER_GDI	_T("render-gdid.dll")
#define	SYS_NAMED_RESOURCE	_T("soui-sys-resourced.dll")
#else
	//...
#endif

/*
int APIENTRY _tWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPTSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	int nRet = 0;

	SComLoader imgDecLoader;
	SComLoader renderLoader;
	SComLoader transLoader;

	//将程序的运行路径修改为项目所在目录;
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	::GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir)*sizeof(TCHAR));
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos + 1, _T(""));
	SetCurrentDirectory(szCurrentDir);

	//PathAppend()可比这里写的方便多了!! jian.he;

	{
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;

		imgDecLoader.CreateInstance(COM_IMGDECODER, (IObjRef**)&pImgDecoderFactory);
		renderLoader.CreateInstance(COM_RENDER_GDI, (IObjRef**)&pRenderFactory);
		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

		SApplication *theApp = new SApplication(pRenderFactory, hInstance);

		HMODULE hSysRes = LoadLibrary(SYS_NAMED_RESOURCE);
		if (hSysRes)
		{
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hSysRes, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
		}

		//加载窗体资源;
		CAutoRefPtr<IResProvider> pResProvider;
		CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
		if (!pResProvider->Init((WPARAM)_T("uires"), 0))
		{
			SASSERT(0);
			return 1;
		}
		theApp->AddResProvider(pResProvider);

		//创建主窗口;
		{
			CMainWnd wndMain;
			wndMain.Create(GetActiveWindow(), 0, 0, 800, 600);
			wndMain.SSendMessage(WM_INITDIALOG);//不用手动吧?
			wndMain.CenterWindow(wndMain.m_hWnd);
			wndMain.ShowWindow(SW_SHOWNORMAL);
			nRet = theApp->Run(wndMain.m_hWnd);
		}
		delete theApp;
	}

	::OleUninitialize();

	return nRet;
}
*/

int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR lpstrCmdLine, int /*nCmdShow*/)
{
	HRESULT hRes = OleInitialize(NULL);
	SASSERT(SUCCEEDED(hRes));

	int nRet = 0;

	SComMgr *pComMgr = new SComMgr;

	//将程序的运行路径修改到项目所在的目录
	TCHAR szCurrentDir[MAX_PATH] = { 0 };
	GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
	LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
	_tcscpy(lpInsertPos + 1, _T(""));
	SetCurrentDirectory(szCurrentDir);
	{
		BOOL bLoaded = FALSE;
		CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
		CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
		bLoaded = pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [render] failed!"));
		bLoaded = pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);
		SASSERT_FMT(bLoaded, _T("load interface [%s] failed!"), _T("imgdecoder"));

		pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);
		SApplication *theApp = new SApplication(pRenderFactory, hInstance);

		theApp->RegisterWindowClass<SImageSwitcher>();
		theApp->RegisterWindowClass<SToolbox>();

		//从DLL加载系统资源
		HMODULE hModSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
		if (hModSysResource)
		{
			CAutoRefPtr<IResProvider> sysResProvider;
			CreateResProvider(RES_PE, (IObjRef**)&sysResProvider);
			sysResProvider->Init((WPARAM)hModSysResource, 0);
			theApp->LoadSystemNamedResource(sysResProvider);
			FreeLibrary(hModSysResource);
		}
		else
		{
			SASSERT(0);
		}

		CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
		CreateResProvider(RES_FILE, (IObjRef**)&pResProvider);
		if (!pResProvider->Init((LPARAM)_T("uires"), 0))
		{
			SASSERT(0);
			return 1;
		}
#else 
		CreateResProvider(RES_PE, (IObjRef**)&pResProvider);
		pResProvider->Init((WPARAM)hInstance, 0);
#endif


		theApp->AddResProvider(pResProvider);


		// BLOCK: Run application
		{
			CMainWnd dlgMain;
			dlgMain.Create(GetActiveWindow());
			dlgMain.SendMessage(WM_INITDIALOG);
			dlgMain.CenterWindow(dlgMain.m_hWnd);
			dlgMain.ShowWindow(SW_SHOWNORMAL);
			nRet = theApp->Run(dlgMain.m_hWnd);
		}

		delete theApp;
	}

	delete pComMgr;

	OleUninitialize();
	return nRet;
}
