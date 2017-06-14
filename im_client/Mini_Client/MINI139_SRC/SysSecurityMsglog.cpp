// SysSecurityMsglog.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SysSecurityMsglog.h"
#include "afxdialogex.h"
#include "..\Im\MsgDlg.h"

using namespace DuiLib;

void CSysSecurityMsglog::InitWindow()
{
	WindowImplBase::InitWindow();

	m_pCheckLoginShowLog = static_cast<COptionUI*>(m_PaintManager.FindControl("LoginShowLog"));////��¼��ͬ��
	m_pCheckExitDelMsgLog = static_cast<COptionUI*>(m_PaintManager.FindControl("ExitDeleteLog"))  ;////�˳�ɾ����Ϣ
	m_pBtnOpenMsgLog =  static_cast<CButtonUI*>(m_PaintManager.FindControl("msgview")) ;     ///����Ϣ������
	
	BASIC_SYSTEM_SETTING *cfg = g_LocalData.GetBasicSystemSetting();
	m_pCheckLoginShowLog->Selected(cfg->bAutoSyncMsgRecord);
	m_pCheckExitDelMsgLog->Selected(cfg->bAutoClearMsgRecord);
}


// ��Ϣ
void CSysSecurityMsglog::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	   if(msg.pSender == m_pBtnOpenMsgLog)
	   {
	      OnBnClickedBtnMsglog();
	   }
	}
}


void CSysSecurityMsglog::OnBnClickedBtnMsglog()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CDuiMsgDlg::Show();
}

// ��������
void CSysSecurityMsglog::Save()
{
	BASIC_SYSTEM_SETTING *cfg = g_LocalData.GetBasicSystemSetting();
	if ( m_pCheckLoginShowLog->IsSelected())
	{
		cfg->bAutoSyncMsgRecord = 1;
	}
	else 
	{
		cfg->bAutoSyncMsgRecord = 0;
	}

	if ( m_pCheckExitDelMsgLog->IsSelected())
	{
		cfg->bAutoClearMsgRecord = 1;
	}
	else 
	{
		cfg->bAutoClearMsgRecord = 0;
	}
}


// CSysSecurityMsglog �Ի���
//
//IMPLEMENT_DYNAMIC(CSysSecurityMsglog, CDialog)
//
//CSysSecurityMsglog::CSysSecurityMsglog(CWnd* pParent /*=NULL*/)
//	: CDialog(CSysSecurityMsglog::IDD, pParent)
//{
//
//}
//
//CSysSecurityMsglog::~CSysSecurityMsglog()
//{
//}
//
//void CSysSecurityMsglog::DoDataExchange(CDataExchange* pDX)
//{
//	CDialog::DoDataExchange(pDX);
//	DDX_Control(pDX, IDC_BTN_MSGLOG, m_btnMsglog);
//	DDX_Control(pDX, IDC_CHECK_SYNC, m_checkSync);
//	DDX_Control(pDX, IDC_CHECK_CLEAR_MSG, m_checkClearMsg);
//}
//
//
//BEGIN_MESSAGE_MAP(CSysSecurityMsglog, CDialog)
//	ON_WM_CTLCOLOR()
//	ON_BN_CLICKED(IDC_BTN_MSGLOG, &CSysSecurityMsglog::OnBnClickedBtnMsglog)
//END_MESSAGE_MAP()
//
//
//// CSysSecurityMsglog ��Ϣ�������
//
//BOOL CSysSecurityMsglog::OnInitDialog()
//{
//	CDialog::OnInitDialog();
//
//	// ���������ӿؼ������ÿؼ�����
//	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//	while(hWndChild)  
//	{  
//		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//		if(GetDlgItem(IDC_STATIC_MSG_LOG)->GetSafeHwnd() == hWndChild)
//		{
//			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
//		}
//		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//	}
//	m_btnMsglog.SetPng(IDB_PNG_SENIOR_BTN_HOVER_L, IDB_PNG_SENIOR_BTN_NORMAL_L, IDB_PNG_SENIOR_BTN_PRESSED_L, IDB_PNG_SENIOR_BTN_PRESSED_L);
//	BASIC_SYSTEM_SETTING *cfg = g_LocalData.GetBasicSystemSetting();
//	((CButton*)GetDlgItem(IDC_CHECK_SYNC))->SetCheck(cfg->bAutoSyncMsgRecord);
//	((CButton*)GetDlgItem(IDC_CHECK_CLEAR_MSG))->SetCheck(cfg->bAutoClearMsgRecord);
//	return TRUE;
//}
//
//HBRUSH CSysSecurityMsglog::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//{
//	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//
//	if ( nCtlColor==CTLCOLOR_STATIC )
//	{
//		UINT nId = pWnd->GetDlgCtrlID();
//
//		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
//		{
//
//		}
//		else
//		{
//			pDC->SetBkMode(TRANSPARENT);
//			hbr = g_hWhiteBrush;
//		}
//
//		if ( nId==IDC_STATIC_GROUP )
//		{
//			pDC->SetTextColor(RGB(255,0,0));//��255,0��0�� //(74,123,211)
//		}
//		else
//		{
//			pDC->SetTextColor(0x2e2e2e);
//		}
//	}
//
//	if ( nCtlColor==CTLCOLOR_DLG  )
//	{
//		hbr = g_hWhiteBrush;
//	}
//
//	return hbr;
//}
//
//
//void CSysSecurityMsglog::OnBnClickedBtnMsglog()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	CMsgDlg * pDlg = CMsgDlg::GetInstance();
//	pDlg->BringWindowToTop();
//}
//
//// ��������
//void CSysSecurityMsglog::Save()
//{
//	BASIC_SYSTEM_SETTING *cfg = g_LocalData.GetBasicSystemSetting();
//	cfg->bAutoSyncMsgRecord = ((CButton*)GetDlgItem(IDC_CHECK_SYNC))->GetCheck();
//	cfg->bAutoClearMsgRecord = ((CButton*)GetDlgItem(IDC_CHECK_CLEAR_MSG))->GetCheck();
//}