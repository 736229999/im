// SpaceManagement.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "SpaceManagement.h"
#include "SpaceManagementDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CSpaceManagementApp

BEGIN_MESSAGE_MAP(CSpaceManagementApp, CWinApp)
	ON_COMMAND(ID_HELP, CWinApp::OnHelp)
END_MESSAGE_MAP()


// CSpaceManagementApp 构造

CSpaceManagementApp::CSpaceManagementApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CSpaceManagementApp 对象

CSpaceManagementApp theApp;


// CSpaceManagementApp 初始化

BOOL CSpaceManagementApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControls()。否则，将无法创建窗口。
	InitCommonControls();

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	SetRegistryKey(_T("应用程序向导生成的本地应用程序"));

	::CoInitialize(0);

	CSpaceManagementDlg dlg;
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();
	if (nResponse == IDOK)
	{
		// TODO: 在此放置处理何时用“确定”来关闭
		//对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//对话框的代码
	}


	::CoUninitialize();

	return FALSE;
}
