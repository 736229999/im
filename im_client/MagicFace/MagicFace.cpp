// MagicFace.cpp : 定义应用程序的类行为。
//

#include "stdafx.h"
#include "MagicFace.h"
#include "MagicFaceDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMagicFaceApp

BEGIN_MESSAGE_MAP(CMagicFaceApp, CWinApp)
	ON_COMMAND(ID_HELP, &CWinApp::OnHelp)
END_MESSAGE_MAP()


// CMagicFaceApp 构造

CMagicFaceApp::CMagicFaceApp()
{
	// TODO: 在此处添加构造代码，
	// 将所有重要的初始化放置在 InitInstance 中
}


// 唯一的一个 CMagicFaceApp 对象

CMagicFaceApp theApp;


// CMagicFaceApp 初始化

BOOL CMagicFaceApp::InitInstance()
{
	// 如果一个运行在 Windows XP 上的应用程序清单指定要
	// 使用 ComCtl32.dll 版本 6 或更高版本来启用可视化方式，
	//则需要 InitCommonControlsEx()。否则，将无法创建窗口。
	INITCOMMONCONTROLSEX InitCtrls;
	InitCtrls.dwSize = sizeof(InitCtrls);
	// 将它设置为包括所有要在应用程序中使用的
	// 公共控件类。
	InitCtrls.dwICC = ICC_WIN95_CLASSES;
	InitCommonControlsEx(&InitCtrls);

	CWinApp::InitInstance();

	AfxEnableControlContainer();

	CString strCmdLine = GetCommandLine();

	int pos1 = strCmdLine.Find("(-");
	int pos2 = strCmdLine.Find("-)");

	if ( pos2>pos1 && pos1>0 )
	{
		g_strMagicFace = strCmdLine.Mid(pos1+2,pos2-pos1-2);
	}

	CMagicFaceDlg dlg(CWnd::GetDesktopWindow());
	m_pMainWnd = &dlg;
	INT_PTR nResponse = dlg.DoModal();

	if (nResponse == IDOK)
	{
		// TODO: 在此处放置处理何时用“确定”来关闭
		//  对话框的代码
	}
	else if (nResponse == IDCANCEL)
	{
		// TODO: 在此放置处理何时用“取消”来关闭
		//  对话框的代码
	}

	// 由于对话框已关闭，所以将返回 FALSE 以便退出应用程序，
	//  而不是启动应用程序的消息泵。
	return FALSE;
}
