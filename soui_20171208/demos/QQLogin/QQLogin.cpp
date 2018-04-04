// dui-demo.cpp : main source file
//

#include "stdafx.h"

#include "MainDlg.h"
#include "GroupChatFrame.h"

#include "../controls.extend/SChromeTabCtrl.h"
#include "../controls.extend/simagemaskwnd.h"
#include "../controls.extend/SListboxex.h"

#include "extend.ctrls/SButtonEx.h"
#include "extend.ctrls/ExtendCtrls.h"
#include "extend.ctrls/imre/SImRichedit.h"
#include "extend.ctrls/SSplitBar.h"
#include "extend.ctrls/SImageView.h"

#include "extend.skins/ExtendSkins.h"
#include "extend.skins/SAntialiasSkin.h"
#include "extend.skins/SVscrollbar.h"

//#define RES_TYPE 0   //从文件中加载资源
#define RES_TYPE 1   //从PE资源中加载UI资源

#ifdef _DEBUG
#define SYS_NAMED_RESOURCE _T("soui-sys-resourced.dll")
#else
#define SYS_NAMED_RESOURCE _T("soui-sys-resource.dll")
#endif


int WINAPI _tWinMain(HINSTANCE hInstance, HINSTANCE /*hPrevInstance*/, LPTSTR /*lpstrCmdLine*/, int /*nCmdShow*/)
{
    HRESULT hRes = OleInitialize(NULL);
    SASSERT(SUCCEEDED(hRes));

    int nRet = 0;

    SComMgr *pComMgr = new SComMgr(_T("imgdecoder-gdip"));

    {
        TCHAR szCurrentDir[MAX_PATH] = { 0 };
        GetModuleFileName(NULL, szCurrentDir, sizeof(szCurrentDir));
        LPTSTR lpInsertPos = _tcsrchr(szCurrentDir, _T('\\'));
        _tcscpy(lpInsertPos+1,_T("..\\demos\\qqlogin"));
        SetCurrentDirectory(szCurrentDir);

        // 加载自带的riched20.DLL,其实是msftedit.dll.
        // 如果使用系统的riched20.dll，在计算缩进等方面有问题,原因未查
        SStringW re = szCurrentDir;
        re += L"\\riched20.dll";
        LoadLibrary(re);

        CAutoRefPtr<SOUI::IImgDecoderFactory> pImgDecoderFactory;
        CAutoRefPtr<SOUI::IRenderFactory> pRenderFactory;
        pComMgr->CreateRender_GDI((IObjRef**)&pRenderFactory);
        pComMgr->CreateImgDecoder((IObjRef**)&pImgDecoderFactory);

        pRenderFactory->SetImgDecoderFactory(pImgDecoderFactory);

        SApplication *theApp = new SApplication(pRenderFactory, hInstance);

        HMODULE hSysResource = LoadLibrary(SYS_NAMED_RESOURCE);
        if (hSysResource)
        {
            CAutoRefPtr<IResProvider> sysSesProvider;
            CreateResProvider(RES_PE, (IObjRef**)&sysSesProvider);
            sysSesProvider->Init((WPARAM)hSysResource, 0);
            theApp->LoadSystemNamedResource(sysSesProvider);
        }


        CAutoRefPtr<IResProvider>   pResProvider;
#if (RES_TYPE == 0)
        //将程序的运行路径修改到项目所在目录所在的目录

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
        theApp->RegisterWindowClass<STurn3dView>();

        // extened controls
        theApp->RegisterWindowClass<SChromeTabCtrl>();//注册ChromeTabCtrl
        theApp->RegisterWindowClass<SImageMaskWnd>();//注册SImageMaskWnd
        theApp->RegisterWindowClass<SButtonEx>();
        theApp->RegisterWindowClass<SWindowEx>();
        theApp->RegisterWindowClass<SImRichEdit>();
        theApp->RegisterWindowClass<SImageView>();
        theApp->RegisterWindowClass<SSplitBar>();
        theApp->RegisterWindowClass<SListBoxEx>();

        //extened skins
        theApp->RegisterSkinClass<SColorMask>();
        theApp->RegisterSkinClass<SAntialiasSkin>();
        theApp->RegisterSkinClass<SSkinVScrollbar>();

        theApp->AddResProvider(pResProvider);

        // BLOCK: Run application
        {
            CMainDlg dlgMain;
            dlgMain.Create(GetActiveWindow(), 0, 0, 0, 0);
            dlgMain.SendMessage(WM_INITDIALOG);
            dlgMain.CenterWindow(dlgMain.m_hWnd);
            dlgMain.AnimateHostWindow(200, AW_CENTER);
            nRet = theApp->Run(dlgMain.m_hWnd);

            if (dlgMain.GetReturnCode() == 0)
            {
                GroupChatFrame chatFrame;
                chatFrame.Create(GetActiveWindow(), 0, 0, 0, 0);
                chatFrame.SendMessage(WM_INITDIALOG);
                chatFrame.CenterWindow(chatFrame.m_hWnd);

                chatFrame.ShowWindow(SW_SHOWNORMAL);
                nRet = theApp->Run(chatFrame.m_hWnd);
            }
        }

        delete theApp;
    }

    delete pComMgr;
    OleUninitialize();
    return nRet;
}
