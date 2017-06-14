// AutoAcceptDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "AutoAcceptDlg.h"
#include "afxdialogex.h"
// CAutoAcceptDlg �Ի���
////
using namespace DuiLib;

CAutoAcceptDlg::CAutoAcceptDlg()
	: m_pfontOnline(&m_reTextOnline, &g_ImFont, CWnd::FromHandle(GetHWND())),
	  m_pfontOffline(&m_reTextOffline, &g_ImFont, CWnd::FromHandle(GetHWND()))
{

}
void CAutoAcceptDlg::InitWindow()
{
	WindowImplBase::InitWindow();

	//////ϵͳ����
	g_LocalData.GetAutoAccept_Online(m_AutoAcceptOnline);
	g_LocalData.GetAutoAccept_Offline(m_AutoAcceptOffline);

	CVerticalLayoutUI* pFontBar = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	{
		pFontBar->SetVisible(true);
	}

	//CVerticalLayoutUI* pFontOfflineBar = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("OfflineFontBar")));
	//{
	//    pFontOfflineBar->SetVisible(true);
	//}

	/////����
	m_reTextOnline.SetText(m_AutoAcceptOnline.strText);
	m_reTextOnline.SetXFont(m_AutoAcceptOnline.font);
	m_reTextOnline.SetLimitWordSize(200);//������������Ϊ200��
	m_reTextOnline.SetEditType(EDIT_TYPE_AUTOACCEPT);//����edit�����Զ��ظ�

	////����
	m_reTextOffline.SetText(m_AutoAcceptOffline.strText);
	m_reTextOffline.SetXFont(m_AutoAcceptOffline.font);

	// ��ʱ���ص������Զ�Ӧ��������Ŀ
	m_pfontOffline.ShowWindow(SW_HIDE);
	m_reTextOffline.ShowWindow(SW_HIDE);
}

// ��ȡ�Զ��Ӵ�������ߣ�
void CAutoAcceptDlg::GetAutoAccept_Online(AUTO_REPLY &online)
{
	m_reTextOnline.GetText(m_AutoAcceptOnline.strText);
	online = m_AutoAcceptOnline;
}

// ��ȡ�Զ��Ӵ�������ߣ�
void CAutoAcceptDlg::GetAutoAccept_Offline(AUTO_REPLY &offline)
{
	m_reTextOffline.GetText(m_AutoAcceptOffline.strText);
	offline = m_AutoAcceptOffline;
}

void CAutoAcceptDlg::Save()
{
	AUTO_REPLY online, offline;
	GetAutoAccept_Online(online);
	GetAutoAccept_Offline(offline);

	g_LocalData.SetAutoAccept_Online(online);
	g_LocalData.SetAutoAccept_Offline(offline);
}


////IMPLEMENT_DYNAMIC(CAutoAcceptDlg, CDialog)
////
////CAutoAcceptDlg::CAutoAcceptDlg(CWnd* pParent /*=NULL*/)
////	: CDialog(CAutoAcceptDlg::IDD, pParent)
////{
////	m_pfontOnline = NULL;
////	m_pfontOffline = NULL;
////
////	g_LocalData.GetAutoAccept_Online(m_AutoAcceptOnline);
////	g_LocalData.GetAutoAccept_Offline(m_AutoAcceptOffline);
////
////	m_hBkBrush = CreateSolidBrush(0xFAFAFA);
////}
////
////CAutoAcceptDlg::~CAutoAcceptDlg()
////{
////	if(m_pfontOnline != NULL)
////	{
////		delete m_pfontOnline;
////		m_pfontOnline = NULL;
////	}
////
////	if(m_pfontOffline != NULL)
////	{
////		delete m_pfontOffline;
////		m_pfontOffline = NULL;
////	}
////
////	DeleteObject(m_hBkBrush);
////}
////
////void CAutoAcceptDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CDialog::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_RICHEDIT_TEXT_ONLINE, m_reTextOnline);
////	DDX_Control(pDX, IDC_RICHEDIT_TEXT_OFFLINE, m_reTextOffline);
////}
////
////
////BEGIN_MESSAGE_MAP(CAutoAcceptDlg, CDialog)
////	ON_WM_CTLCOLOR()
////	ON_WM_SIZE()
////END_MESSAGE_MAP()
////
////
////// CAutoAcceptDlg ��Ϣ�������
////
////
////HBRUSH CAutoAcceptDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
////			hbr = m_hBkBrush;
////		}
////
////		if ( nId==IDC_STATIC_GROUP )
////		{
////			pDC->SetTextColor(RGB(255,0,0));//��255,0��0�� //(74,123,211)
////		}
////
////	}
////
////	if ( nCtlColor==CTLCOLOR_DLG  )
////	{
////		hbr = m_hBkBrush;
////	}
////
////	return hbr;
////}
////
////BOOL CAutoAcceptDlg::OnInitDialog()
////{
////	CDialog::OnInitDialog();
////
////	// ���������ӿؼ������ÿؼ�����
////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////	while(hWndChild)  
////	{  
////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////	}
////
////	// ���幤����
////	CRect rcFontOnline;
////	GetDlgItem(IDC_STATIC_FONT_ONLINE)->GetWindowRect(&rcFontOnline);
////	ScreenToClient(&rcFontOnline);
////	m_pfontOnline = new CP2PTalkToolBar(&m_reTextOnline, &m_AutoAcceptOnline.font, this);
////	m_pfontOnline->Create(CP2PTalkToolBar::IDD,this);
////	m_pfontOnline->MoveWindow(rcFontOnline);
////	m_pfontOnline->ShowWindow(SW_SHOW);
////
////	m_reTextOnline.SetText(m_AutoAcceptOnline.strText);
////	m_reTextOnline.SetXFont(m_AutoAcceptOnline.font);
////	m_reTextOnline.SetLimitWordSize(200);//������������Ϊ200��
////	m_reTextOnline.SetEditType(EDIT_TYPE_AUTOACCEPT);//����edit�����Զ��ظ�
////
////	CRect rcFontOffline;
////	GetDlgItem(IDC_STATIC_FONT_OFFLINE)->GetWindowRect(&rcFontOffline);
////	ScreenToClient(&rcFontOffline);
////	m_pfontOffline = new CP2PTalkToolBar(&m_reTextOffline, &m_AutoAcceptOffline.font, this);
////	m_pfontOffline->Create(CP2PTalkToolBar::IDD,this);
////	m_pfontOffline->MoveWindow(rcFontOffline);
////	m_pfontOffline->ShowWindow(SW_SHOW);
////
////	m_reTextOffline.SetText(m_AutoAcceptOffline.strText);
////	m_reTextOffline.SetXFont(m_AutoAcceptOffline.font);
////
////	// ��ʱ���ص������Զ�Ӧ��������Ŀ
////	m_pfontOffline->ShowWindow(SW_HIDE);
////	m_reTextOffline.ShowWindow(SW_HIDE);
////
////	return TRUE;
////}
////
////// ��ȡ�Զ��Ӵ�������ߣ�
////void CAutoAcceptDlg::GetAutoAccept_Online(AUTO_REPLY &online)
////{
////	m_reTextOnline.GetText(m_AutoAcceptOnline.strText);
////	online = m_AutoAcceptOnline;
////}
////
////// ��ȡ�Զ��Ӵ�������ߣ�
////void CAutoAcceptDlg::GetAutoAccept_Offline(AUTO_REPLY &offline)
////{
////	m_reTextOffline.GetText(m_AutoAcceptOffline.strText);
////	offline = m_AutoAcceptOffline;
////}
////
////void CAutoAcceptDlg::OnSize(UINT nType, int cx, int cy)
////{
////	CDialog::OnSize(nType, cx, cy);
////}