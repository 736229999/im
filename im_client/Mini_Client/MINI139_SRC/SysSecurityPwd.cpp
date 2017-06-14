// SysSecurityPwd.cpp : 实现文件
//

#include "stdafx.h"
#include "SysSecurityPwd.h"
#include "afxdialogex.h"
#include "..\resource.h"
#include "CModPSW.h"

using namespace DuiLib;

void CSysSecurityPwd::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pBtnModpwd = static_cast<CButtonUI*>(m_PaintManager.FindControl("Modpwd"));////修改密码
}

// 消息
void CSysSecurityPwd::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnModpwd) ////修改密码
		{
			OnBnClickedBtnPwd();
		}
	}
}

void CSysSecurityPwd::OnBnClickedBtnPwd()
{
	// TODO: 在此添加控件通知处理程序代码
#ifndef _CS_PLAT_EX_
	#ifdef _WORK_EX_
			OpenHtmlPage(WORK_FORGET_PSW_URL);
	#elif defined _BUSINESS_VESION
			OpenHtmlPage(SELLER_FORGET_PSW_URL);
	#else
			OpenHtmlPage(BUYER_FORGET_PSW_URL);
	#endif
#else
	std::tr1::shared_ptr<CModPassWordDlg> pWnd = CModPassWordDlg::Show();
	pWnd->ShowModal();
#endif
}

// CSysSecurityPwd 对话框

//////IMPLEMENT_DYNAMIC(CSysSecurityPwd, CDialog)
//////
//////CSysSecurityPwd::CSysSecurityPwd(CWnd* pParent /*=NULL*/)
//////	: CDialog(CSysSecurityPwd::IDD, pParent)
//////{
//////
//////}
//////
//////CSysSecurityPwd::~CSysSecurityPwd()
//////{
//////}
//////
//////void CSysSecurityPwd::DoDataExchange(CDataExchange* pDX)
//////{
//////	CDialog::DoDataExchange(pDX);
//////	DDX_Control(pDX, IDC_BTN_PWD, m_btnPWD);
//////}
//////
//////
//////BEGIN_MESSAGE_MAP(CSysSecurityPwd, CDialog)
//////	ON_BN_CLICKED(IDC_BTN_PWD, &CSysSecurityPwd::OnBnClickedBtnPwd)
//////	ON_WM_CTLCOLOR()
//////END_MESSAGE_MAP()
//////
//////
//////// CSysSecurityPwd 消息处理程序
//////
//////BOOL CSysSecurityPwd::OnInitDialog()
//////{
//////	CDialog::OnInitDialog();
//////
//////	// 遍历所有子控件，设置控件字体
//////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//////	while(hWndChild)  
//////	{  
//////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//////		if(GetDlgItem(IDC_STATIC_PWD)->GetSafeHwnd() == hWndChild)
//////		{
//////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
//////		}
//////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//////	}
//////
//////	m_btnPWD.SetPng(IDB_PNG_SENIOR_BTN_HOVER, IDB_PNG_SENIOR_BTN_NORMAL, IDB_PNG_SENIOR_BTN_PRESSED, IDB_PNG_SENIOR_BTN_PRESSED);
//////	
//////	return TRUE;
//////}
//////
//////
//////void CSysSecurityPwd::OnBnClickedBtnPwd()
//////{
//////	// TODO: 在此添加控件通知处理程序代码
//////#ifdef _BUSINESS_VESION
//////		OpenHtmlPage(SELLER_FORGET_PSW_URL);
//////#else
//////		OpenHtmlPage(BUYER_FORGET_PSW_URL);
//////#endif
//////	ShowWindow(SW_MINIMIZE);
//////}
//////
//////
//////HBRUSH CSysSecurityPwd::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//////{
//////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//////
//////	if ( nCtlColor==CTLCOLOR_STATIC )
//////	{
//////		UINT nId = pWnd->GetDlgCtrlID();
//////
//////		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
//////		{
//////
//////		}
//////		else
//////		{
//////			pDC->SetBkMode(TRANSPARENT);
//////			hbr = g_hWhiteBrush;
//////		}
//////
//////		if ( nId==IDC_STATIC_GROUP )
//////		{
//////			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
//////		}
//////		else
//////		{
//////			pDC->SetTextColor(0x2e2e2e);
//////		}
//////	}
//////
//////	if ( nCtlColor==CTLCOLOR_DLG  )
//////	{
//////		hbr = g_hWhiteBrush;
//////	}
//////
//////	return hbr;
//////}
