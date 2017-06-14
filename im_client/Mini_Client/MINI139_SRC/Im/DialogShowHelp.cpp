// E:\hechang\MiniApollo\Mini_Client\Mini_Client\MINI_CLIENT\Mini139_Src\Im\DialogShowHelp.cpp : 实现文件
//

#include "stdafx.h"
//#include "Mini139_src.h"
#include "DialogShowHelp.h"
#include ".\dialogshowhelp.h"
#include "LocalData.h"
#include "..\IMClient.h"

extern CIMClientApp theApp;
// CDialogShowHelp 对话框

CDialogShowHelp* CDialogShowHelp::m_pInstance = NULL;
IMPLEMENT_DYNAMIC(CDialogShowHelp, CXSkinDlg)
CDialogShowHelp::CDialogShowHelp(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CDialogShowHelp::IDD, pParent)
{
	m_pInstance = this;

}

CDialogShowHelp::~CDialogShowHelp()
{

}

void CDialogShowHelp::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_HELP, m_ieHelp);
	DDX_Control(pDX, IDC_CHECK_NEED_RUN, m_wndCheckNeedRun);
}


BEGIN_MESSAGE_MAP(CDialogShowHelp, CXSkinDlg)
	ON_WM_GETMINMAXINFO()
	ON_WM_CTLCOLOR()
	ON_BN_CLICKED(IDC_CHECK_NEED_RUN, OnBnClickedCheckNeedRun)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_DESTROY()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// CDialogShowHelp 消息处理程序

void CDialogShowHelp::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	lpMMI->ptMinTrackSize.x = 585;
	lpMMI->ptMinTrackSize.y = 385;


	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

BOOL CDialogShowHelp::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	// TODO:  在此添加额外的初始化
	
	SetWindowText("帮助演示");
	
	CString strHelpPath;
	strHelpPath =  GetModulePath();
	strHelpPath += "MiniDemo.swf";
	m_ieHelp.Navigate(strHelpPath,NULL,NULL,NULL,NULL);

	m_wndCheckNeedRun.SetCheck(BST_CHECKED);


	CRect rtMain;
	GetWindowRect(rtMain);
	//ClientToScreen(rtMain);
	//m_ieHelp.ModifyStyle(WS_BORDER ,NULL );

	rtMain.right = rtMain.left + 585;
	rtMain.bottom= rtMain.top  + 385 + 50;
	MoveWindow(rtMain);

	CenterWindow();
		

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

HBRUSH CDialogShowHelp::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	if( CTLCOLOR_STATIC == nCtlColor )
	{
		pDC->SetBkMode(TRANSPARENT);
		return (HBRUSH)GetStockObject(NULL_BRUSH);//使背景透明
	}

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CDialogShowHelp::PostNcDestroy()
{
	// TODO: 在此添加专用代码和/或调用基类

	CXSkinDlg::PostNcDestroy();


}

void CDialogShowHelp::Show()
{
	if( m_pInstance )
	{
		//delete m_pInstance;
		m_pInstance->ShowWindow(SW_SHOW);
		m_pInstance->BringWindowToTop();
	}
	else
	{
		m_pInstance = new CDialogShowHelp;
		m_pInstance->Create(CDialogShowHelp::IDD,NULL);
		m_pInstance->ShowWindow(SW_SHOW);
	}
}

void CDialogShowHelp::Exit()
{
	if( m_pInstance )
	{

		m_pInstance->DestroyWindow();
	//	delete m_pInstance;
	//	m_pInstance = NULL;
	}
}

void CDialogShowHelp::OnBnClickedCheckNeedRun()
{
	// TODO: 在此添加控件通知处理程序代码

	if( BST_CHECKED == m_wndCheckNeedRun.GetCheck())
	{
		g_LocalData.SetShowHelpDemo(true);
	}
	else
	{
		g_LocalData.SetShowHelpDemo(false);
	}
}


void CDialogShowHelp::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();

	// TODO: 在此处添加消息处理程序代码
	Exit();
}

void CDialogShowHelp::OnDestroy()
{
	//this->m_ieHelp.DestroyWindow();
	CXSkinDlg::OnDestroy();

	// TODO: 在此处添加消息处理程序代码

}

void CDialogShowHelp::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	// TODO: 在此处添加消息处理程序代码
	CRect rtCheck;
	

	if( m_wndCheckNeedRun.GetSafeHwnd())
	{
		m_wndCheckNeedRun.GetWindowRect(rtCheck);
		m_wndCheckNeedRun.MoveWindow(16,cy-rtCheck.Height()-8,rtCheck.Width(),rtCheck.Height());
		
		if( m_ieHelp.GetSafeHwnd() )
		{
			int nTop = 24;
			m_ieHelp.MoveWindow(0,nTop,cx,cy - nTop - (rtCheck.Height()+16));
		}
	}


}
