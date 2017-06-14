// SysSecurityVerify.cpp : 实现文件
//

#include "stdafx.h"
#include "SysSecurityVerify.h"
#include "afxdialogex.h"
#include "..\Im\IMProtocol.h"

// CSysSecurityVerify 对话框
using namespace DuiLib;

void CSysSecurityVerify::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pRadioAnyAllow = static_cast<COptionUI*>(m_PaintManager.FindControl("AnyAllow"));////允许任何人
	m_pRadioVerify = static_cast<COptionUI*>(m_PaintManager.FindControl("Verify"))  ;////需要验证

	switch ( g_ImUserInfo.rule )
	{
	  case 1:
		 m_pRadioVerify->Selected(TRUE);
		 break;
	  case 0:
		 m_pRadioAnyAllow->Selected(TRUE);
		 break;
	  default :
		 break;
	}
}


// 消息
void CSysSecurityVerify::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	/*if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	if(m_pRadioAutoInstall->IsSelected())
	{
	Save();
	}
	else if(m_pRadioAutoInstall->IsSelected())
	{
	Save()
	}
	}*/
}



void CSysSecurityVerify::SaveConfig()
{
	//认证方式
	if ( m_pRadioAnyAllow->IsSelected() )
	{
		g_ImUserInfo.rule = 0;//允许任何人
	}
	if ( m_pRadioVerify->IsSelected() )
	{
		g_ImUserInfo.rule = 1;//需要验证
	}
	g_pProtocol->SendReq(g_ImUserInfo);
}
//

//////
//////IMPLEMENT_DYNAMIC(CSysSecurityVerify, CDialog)
//////
//////CSysSecurityVerify::CSysSecurityVerify(CWnd* pParent /*=NULL*/)
//////	: CDialog(CSysSecurityVerify::IDD, pParent)
//////{
//////
//////}
//////
//////CSysSecurityVerify::~CSysSecurityVerify()
//////{
//////}
//////
//////void CSysSecurityVerify::DoDataExchange(CDataExchange* pDX)
//////{
//////	CDialog::DoDataExchange(pDX);
//////	DDX_Control(pDX, IDC_RADIO_ANY, m_radioAny);
//////	DDX_Control(pDX, IDC_RADIO_VERIFY, m_radioVerify);
//////}
//////
//////
//////BEGIN_MESSAGE_MAP(CSysSecurityVerify, CDialog)
//////	ON_WM_CTLCOLOR()
//////	ON_BN_CLICKED(IDC_RADIO_VERIFY, &CSysSecurityVerify::OnBnClickedRadioVerify)
//////	ON_BN_CLICKED(IDC_RADIO_ANY, &CSysSecurityVerify::OnBnClickedRadioAny)
//////END_MESSAGE_MAP()
//////
//////
//////// CSysSecurityVerify 消息处理程序
//////BOOL CSysSecurityVerify::OnInitDialog()
//////{
//////	CDialog::OnInitDialog();
//////	// 遍历所有子控件，设置控件字体
//////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//////	while(hWndChild)  
//////	{  
//////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//////		if(GetDlgItem(IDC_STATIC_VERIFY_SETTING)->GetSafeHwnd() == hWndChild)
//////		{
//////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
//////		}
//////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//////	}
//////
//////	((CButton*)GetDlgItem(IDC_RADIO_VERIFY))->SetCheck(TRUE);
//////
//////	CButton *pRadioAny = (CButton *)GetDlgItem(IDC_RADIO_ANY);
//////	pRadioAny->SetCheck(false);
//////
//////	CButton *pRadioVerify = (CButton *)GetDlgItem(IDC_RADIO_VERIFY);
//////	pRadioVerify->SetCheck(false);
//////
//////	switch ( g_ImUserInfo.rule )
//////	{
//////	case 1:
//////		pRadioVerify->SetCheck(1);
//////		break;
//////	case 0:
//////		pRadioAny->SetCheck(1);
//////	}
//////
//////	return TRUE;
//////}
//////
//////HBRUSH CSysSecurityVerify::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
//////
//////
//////void CSysSecurityVerify::OnBnClickedRadioVerify()
//////{
//////	// TODO: 在此添加控件通知处理程序代码
//////}
//////
//////void CSysSecurityVerify::SetUserInfo(const XT_USER_INFO &info)
//////{
//////	CButton *pRadioAny = (CButton *)GetDlgItem(IDC_RADIO_ANY);
//////	pRadioAny->SetCheck(false);
//////
//////	CButton *pRadioVerify = (CButton *)GetDlgItem(IDC_RADIO_VERIFY);
//////	pRadioVerify->SetCheck(false);
//////
//////	switch ( info.rule )
//////	{
//////	  case 1:
//////		pRadioVerify->SetCheck(1);
//////		break;
//////	  case 0:
//////		pRadioAny->SetCheck(1);
//////	}
//////}
////
////void CSysSecurityVerify::OnBnClickedRadioAny()
////{
////	// TODO: 在此添加控件通知处理程序代码
////}
////
////
////void CSysSecurityVerify::SaveConfig()
////{
////	//认证方式
////	CButton *pRadioAny = (CButton *)GetDlgItem(IDC_RADIO_ANY);
////	CButton *pRadioVerify = (CButton *)GetDlgItem(IDC_RADIO_VERIFY);
////	if ( pRadioAny->GetCheck() )
////	{
////		g_ImUserInfo.rule = 0;//允许任何人
////	}
////	if ( pRadioVerify->GetCheck() )
////	{
////		g_ImUserInfo.rule = 1;//需要验证
////	}
////	g_pProtocol->SendReq(g_ImUserInfo);
////
////	//if (GetUserInfo(m_UserInfo))
////	//{
////	//		if ( strcmpi(g_ImUserInfo.nickname,m_UserInfo.nickname)==0 )
////	//		{
////	//			strcpy(m_UserInfo.nickname,"");
////	//			g_pProtocol->SendReq(m_UserInfo);
////	//			strcpy(m_UserInfo.nickname,g_ImUserInfo.nickname);
////	//		}
////	//		else////已修改昵称（此处不改变)
////	//		{
////	//			g_pProtocol->SendReq(m_UserInfo);
////	//		}
////	//}
////}
//////
//////int  CSysSecurityVerify::GetUserInfo(XT_USER_INFO &info)
//////{
//////	CButton *pRadioAny = (CButton *)GetDlgItem(IDC_RADIO_ANY);
//////	CButton *pRadioVerify = (CButton *)GetDlgItem(IDC_RADIO_VERIFY);
//////	if ( pRadioAny->GetCheck() )
//////	{
//////		info.rule = 0;//允许任何人
//////	}
//////	if ( pRadioVerify->GetCheck() )
//////	{
//////		info.rule = 1;//需要验证
//////	}
//////	return 1;
//////}