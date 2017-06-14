// SysSecurityFileTransfer.cpp : 实现文件
//

#include "stdafx.h"
#include "SysSecurityFileTransfer.h"
#include "afxdialogex.h"
#include "./im/LocalData.h"

// CSysSecurityFileTransfer 对话框
////
using namespace DuiLib;

void CSysSecurityFileTransfer::InitWindow()
{
	WindowImplBase::InitWindow();

	m_pRadioHighLevel = static_cast<COptionUI*>(m_PaintManager.FindControl("Highlevel"));////安全级别高
	m_pRadioMidLevel = static_cast<COptionUI*>(m_PaintManager.FindControl("Midlevel"))  ; ////安全级别中等
	m_pRadioLowLevel = static_cast<COptionUI*>(m_PaintManager.FindControl("Lowlevel"))  ;  ////安全级别低

	WORD nVKTrans;
	g_LocalData.GetFileTransConfig(nVKTrans);
	switch(nVKTrans)
	{
	case 1:
		m_pRadioHighLevel->Selected(TRUE);
		break;
	case 3:
	    m_pRadioLowLevel->Selected(TRUE);
		break;
	case 2:
	default:
	    m_pRadioMidLevel->Selected(TRUE);
		break;
	}
}


// 消息
void CSysSecurityFileTransfer::Notify(DuiLib::TNotifyUI& msg)
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



void CSysSecurityFileTransfer::SaveConfig()
{
	WORD wVKFileTrans = 2;
	if(m_pRadioHighLevel->IsSelected())
	{
		wVKFileTrans = 1;
	}
	else if(m_pRadioLowLevel->IsSelected())
	{
		wVKFileTrans = 3;
	}
	g_LocalData.SaveFileTransConfig(wVKFileTrans);
	
}
////IMPLEMENT_DYNAMIC(CSysSecurityFileTransfer, CDialog)
////
////CSysSecurityFileTransfer::CSysSecurityFileTransfer(CWnd* pParent /*=NULL*/)
////	: CDialog(CSysSecurityFileTransfer::IDD, pParent)
////{
////
////}
////
////CSysSecurityFileTransfer::~CSysSecurityFileTransfer()
////{
////}
////
////void CSysSecurityFileTransfer::DoDataExchange(CDataExchange* pDX)
////{
////	CDialog::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_RADIO_HIGH, m_radioHigh);
////	DDX_Control(pDX, IDC_RADIO_LOW, m_radioLow);
////	DDX_Control(pDX, IDC_RADIO_MID, m_radioMid);
////}
////
////BEGIN_MESSAGE_MAP(CSysSecurityFileTransfer, CDialog)
////	ON_WM_CTLCOLOR()
////	ON_BN_CLICKED(IDC_RADIO_MID, &CSysSecurityFileTransfer::OnBnClickedRadioMid)
////END_MESSAGE_MAP()
////
////
////// CSysSecurityFileTransfer 消息处理程序
////
////BOOL CSysSecurityFileTransfer::OnInitDialog()
////{
////	CDialog::OnInitDialog();
////
////	// 遍历所有子控件，设置控件字体
////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////	while(hWndChild)  
////	{  
////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////		if(GetDlgItem(IDC_STATIC_FILE_TRAN)->GetSafeHwnd() == hWndChild)
////		{
////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
////		}
////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////	}
////
////	WORD nVKTrans;
////	g_LocalData.GetFileTransConfig(nVKTrans);
////	switch(nVKTrans)
////	{
////		case 1:
////			((CButton*)GetDlgItem(IDC_RADIO_HIGH))->SetCheck(TRUE);
////			break;
////		case 3:
////			((CButton*)GetDlgItem(IDC_RADIO_LOW))->SetCheck(TRUE);
////			break;
////		case 2:
////		default:
////			((CButton*)GetDlgItem(IDC_RADIO_MID))->SetCheck(TRUE);
////			break;
////	}
////	return TRUE;
////}
////
////HBRUSH CSysSecurityFileTransfer::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
////
////	if ( nCtlColor==CTLCOLOR_STATIC )
////	{
////		UINT nId = pWnd->GetDlgCtrlID();
////
////		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
////		{
////
////		}
////		else
////		{
////			pDC->SetBkMode(TRANSPARENT);
////			hbr = g_hWhiteBrush;
////		}
////
////		if ( nId==IDC_STATIC_GROUP )
////		{
////			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
////		}
////		else
////		{
////			pDC->SetTextColor(0x2e2e2e);
////		}
////	}
////
////	if ( nCtlColor==CTLCOLOR_DLG  )
////	{
////		hbr = g_hWhiteBrush;
////	}
////
////	return hbr;
////}
////
////
////void CSysSecurityFileTransfer::OnBnClickedRadioMid()
////{
////	// TODO: 在此添加控件通知处理程序代码
////}
////
////
////void CSysSecurityFileTransfer::SaveConfig()
////{
////	bool bHighCheck = ((CButton*)GetDlgItem(IDC_RADIO_HIGH))->GetCheck();
////	bool bMidCheck = ((CButton*)GetDlgItem(IDC_RADIO_MID))->GetCheck();
////	bool bLowCheck = ((CButton*)GetDlgItem(IDC_RADIO_LOW))->GetCheck();
////
////	WORD wVKFileTrans = 2;
////	if(bHighCheck)
////	{
////		wVKFileTrans = 1;
////	}
////	else if(bLowCheck)
////	{
////		wVKFileTrans = 3;
////	}
////	g_LocalData.SaveFileTransConfig(wVKFileTrans);
////
////	
////}
////
