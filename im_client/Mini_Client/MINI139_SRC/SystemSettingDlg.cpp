// SystemSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemSettingDlg.h"
#include "afxdialogex.h"

using namespace DuiLib;
//

CSystemSettingDlg::CSystemSettingDlg()
{

}

CSystemSettingDlg::~CSystemSettingDlg()
{

}

///////////////////////////////////////////////////////////////////////////////////////////////
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
std::tr1::shared_ptr<CSystemSettingDlg> CSystemSettingDlg::pInstance = NULL;
std::tr1::shared_ptr<CSystemSettingDlg> CSystemSettingDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
	    BringWindowToTop(pInstance->GetHWND());
	    ::ShowWindow(pInstance->GetHWND(),SW_NORMAL);
    }
	else
	{
		pInstance = std::tr1::shared_ptr<CSystemSettingDlg>(new CSystemSettingDlg());
		pInstance->Create(GetDesktopWindow(), _T("ϵͳ����"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}

void CSystemSettingDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////��ȡ�ؼ�����

	m_pRadioBasic = static_cast<COptionUI*>(m_PaintManager.FindControl("basicSettingBtn"));
	m_pRadioSecurity = static_cast<COptionUI*>(m_PaintManager.FindControl("securitySettingBtn"));

	m_pTabLayout = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));

    m_pBtnClose  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("close"));
	m_pBtnOK     = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnOK"));
	m_pBtnCancel = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnCancel"));
	InitTabCtrl();
	UpdateBk();
}

void CSystemSettingDlg::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}

void  CSystemSettingDlg::InitTabCtrl()
{
	////Ĭ����ʾ��һ��
   m_SystemCommon.ShowWindow(SW_SHOW);
   m_SystemSecurity.ShowWindow(SW_HIDE);
}
//
 ////����Tabҳ��ʾ
void CSystemSettingDlg::ShowTab(int nMainTabIndex, int nChildTabIndex)
{
	if(nMainTabIndex == 0)
	{
		m_SystemCommon.ShowWindow(SW_SHOW);
		m_SystemCommon.ShowTab(nChildTabIndex);
	}
	if(nMainTabIndex == 1)
	{
		m_SystemSecurity.ShowWindow(SW_SHOW);
		m_SystemSecurity.ShowTab(nChildTabIndex);
	}
}


void CSystemSettingDlg::OnBnClickedOk()
{
	m_SystemCommon.Save();
	m_SystemSecurity.Save();
	g_LocalData.SaveSystemSetting();//���浽����
	Close(IDOK);
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SYSTEM_SETTING_MSG, 0, 0);	
}


void CSystemSettingDlg::OnBnClickedCancel()
{
	Close(IDOK);	
	::SendMessage(CMainFrame::GetInstance()->GetHWND(), WM_SYSTEM_SETTING_MSG, 0, 0);	
}


LRESULT CSystemSettingDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

    return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

// ��Ϣ
void CSystemSettingDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(_tcsicmp(msg.sType, _T("selectchanged")) == 0)
	{	
		DuiLib::COptionUI *pOption = static_cast<DuiLib::COptionUI*>(msg.pSender);
		string strGroup = pOption->GetGroup();
		if(strGroup == "mainTab")
		{	//  tabѡ��ı�
			if(m_pRadioBasic->IsSelected())
			{
		    	m_pTabLayout->SelectItem(0);
			}
			else if(m_pRadioSecurity->IsSelected())
		    {
			   m_pTabLayout->SelectItem(1);
			}
		}
	}
    else if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	    if(msg.pSender == m_pBtnOK)////ȷ��  
		{
			OnBnClickedOk();
		}
		else if(msg.pSender == m_pBtnCancel)////ȡ��  
		{
            OnBnClickedCancel();
		}
		else if(msg.pSender == m_pBtnClose)////�ر�
		{
			OnBnClickedCancel();
		}
	}
}

void CSystemSettingDlg::CloseDlg()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		pInstance->Close(IDOK);
	}
}

//void  CSystemSettingDlg::InitTabCtrl()
//{
//	//CRect rc;
//	//GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rc);
//	//ScreenToClient(&rc);
//	////�����������̶�
//
//	m_SystemCommon.Create(IDD_WIZARD_CLUSTER_SYS_COMMON,this);    //��TAB�ϴ���
//	m_SystemCommon.MoveWindow(&rc);
//	m_SystemSecurity.Create(IDD_WIZARD_CLUSTER_SYS_SECURITY,this);  
//	m_SystemSecurity.MoveWindow(&rc);
//
//	//////Ĭ����ʾ��һ��
//	m_SystemCommon.ShowWindow(SW_SHOW);
//	m_SystemSecurity.ShowWindow(SW_HIDE);
//}
//
//std::tr1::shared_ptr<CSystemSettingDlg> CSystemSettingDlg::Show()
//{
//	pInstance = std::tr1::shared_ptr<CSystemSettingDlg>(new CSystemSettingDlg(GetDesktopWindow()));
//	pInstance->Create(CSystemSettingDlg::IDD,GetDesktopWindow());
//	pInstance->ShowWindow(SW_SHOW);
//	return pInstance;
//}
//
////void CSystemSettingDlg::UpdateUserInfo()
////{
////	m_UserInfo = g_ImUserInfo;
////	m_SystemSecurity.m_pSysSecVerify->SetUserInfo(m_UserInfo);///��֤��Ϣ
////	
////	//m_pPage1->SetUserInfo(m_UserInfo);
////	//m_pPage2->SetUserInfo(m_UserInfo);
////	//m_pPage3->SetUserInfo(m_UserInfo);
////}
//
//// �˵���ѡ�Ÿı��¼�
//LRESULT CSystemSettingDlg::OnMainTabSelChange(WPARAM wParam, LPARAM lParam)
//{
//	int nCurSel = int(wParam);
//	if(nCurSel == 0)
//	{	// ϵͳ����
//		m_SystemCommon.ShowWindow(SW_SHOW);
//		m_SystemSecurity.ShowWindow(SW_HIDE);
//	}
//	if(nCurSel == 1)
//	{	// ��ȫ����
//		m_SystemCommon.ShowWindow(SW_HIDE);
//		m_SystemSecurity.ShowWindow(SW_SHOW);
//	}
//
//	return 0;
//}
//
//void CSystemSettingDlg::OnBnClickedOk()
//{
//	m_SystemCommon.Save();
//	m_SystemSecurity.Save();
//
//	// �򸸴��ڷ�����Ϣ
//	// wParam = 0����ʾ�����ȷ��
//	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SYSTEM_SETTING_MSG, 0, 0);
//
//	m_pMainTab->SendMessage(WM_NCDESTROY);
//	DestroyWindow();	
//}
//
//
//void CSystemSettingDlg::OnBnClickedCancel()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_pMainTab->SendMessage(WM_NCDESTROY);
//	DestroyWindow();
//}
//
//// ����Tabҳ��ʾ
//void CSystemSettingDlg::ShowTab(int nMainTabIndex, int nChildTabIndex)
//{
//	if(nMainTabIndex == 0)
//	{
//		m_pMainTab->SetCurSel(CUIMainTab::en_sel_bask);
//		m_SystemCommon.ShowTab(nChildTabIndex);
//	}
//
//	if(nMainTabIndex == 1)
//	{
//		m_pMainTab->SetCurSel(CUIMainTab::en_sel_security);
//		m_SystemSecurity.ShowTab(nChildTabIndex);
//	}
//}
//
//BOOL CSystemSettingDlg::PreTranslateMessage(MSG* pMsg)
//{
//	if  ( pMsg->message  ==  WM_KEYDOWN  )  
//	{  
//		switch(  pMsg->wParam  )  
//		{  
//		case VK_ESCAPE:
//		case VK_RETURN:
//			return TRUE;
//			break;
//		}
//	}
//
//	return CXSkinDlg::PreTranslateMessage(pMsg);
//}
//
//DuiLib::UILIB_RESOURCETYPE CUIMainTab::GetResourceType() const
//{
//	return UILIB_ZIPRESOURCE;
//}
//
//LPCTSTR CUIMainTab::GetResourceID() const
//{
//	return MAKEINTRESOURCE(IDR_ZIPRES1);
//}
//
//void CUIMainTab::Notify(DuiLib::TNotifyUI& msg)
//{
//	WindowImplBase::Notify(msg);
//	if(msg.sType == "selectchanged")
//	{
//		::SendMessage(::GetParent(m_hWnd), WM_MAINTAB_SEL_CHANGE, msg.pSender->GetName() == "baskSettingBtn" ? en_sel_bask : en_sel_security, 0);
//	}
//}
//
//void CUIMainTab::SetCurSel(EN_SEL en)
//{
//	COptionUI* pOptionBask = static_cast<COptionUI*>(m_PaintManager.FindControl("baskSettingBtn"));
//	COptionUI* pOptionSecurity = static_cast<COptionUI*>(m_PaintManager.FindControl("securitySettingBtn"));
//	if(pOptionBask != NULL && pOptionSecurity != NULL) 
//	{
//		pOptionBask->Selected(en == en_sel_bask);
//		pOptionSecurity->Selected(en == en_sel_security);
//	}
//}

/////////////////////////////////////////////////////////////////////////////////////////////
//// CSystemSettingDlg �Ի���
//enum TAB_CTRL_ID
//{
//	TAB_CTRL_COMMON_ID   = 0,
//	TAB_CTRL_SECRUITY_ID = 1,
//};
//std::tr1::shared_ptr<CSystemSettingDlg> CSystemSettingDlg::pInstance = NULL;
//IMPLEMENT_DYNAMIC(CSystemSettingDlg, CXSkinDlg)
//
//CSystemSettingDlg::CSystemSettingDlg(CWnd* pParent /*=NULL*/)
//	: CXSkinDlg(CSystemSettingDlg::IDD, pParent)
//{
//	//////�����ɫ����///////////////
//	SetDlgStyle(CXSkinDlg::CLOSE);
//	m_colorBackground = RGB(235, 235, 235);
//	m_colorBorder = CHAT_BKG_COLOR;
//	DeleteObject(m_hBkGround);
//	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
//
//	m_pMainTab = new CUIMainTab();
//}
//
//CSystemSettingDlg::~CSystemSettingDlg()
//{
//	if(::IsWindow(GetSafeHwnd()))
//	{
//		m_pMainTab->SendMessage(WM_NCDESTROY);
//		DestroyWindow();
//	}
//	if(m_pMainTab != NULL)
//	{
//		delete m_pMainTab;
//		m_pMainTab = NULL;
//	}
//}
//
//void CSystemSettingDlg::DoDataExchange(CDataExchange* pDX)
//{
//	CXSkinDlg::DoDataExchange(pDX);
//	DDX_Control(pDX, IDOK, m_btnOK);
//	DDX_Control(pDX, IDCANCEL, m_btnCancel);
//}
//
//
//BEGIN_MESSAGE_MAP(CSystemSettingDlg, CXSkinDlg)
//	ON_BN_CLICKED(IDOK, &CSystemSettingDlg::OnBnClickedOk)
//	ON_BN_CLICKED(IDCANCEL, &CSystemSettingDlg::OnBnClickedCancel)
//	ON_MESSAGE(WM_MAINTAB_SEL_CHANGE, OnMainTabSelChange)
//END_MESSAGE_MAP()
//
//
//// CSystemSettingDlg ��Ϣ�������
//BOOL CSystemSettingDlg::OnInitDialog()
//{
//	CXSkinDlg::OnInitDialog();
//	SetWindowText(_T("ϵͳ����"));
//
//	// ���������ӿؼ������ÿؼ�����
//	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//	while(hWndChild)  
//	{  
//		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//	}
//
//	m_btnOK.SetPng(IDB_PNG_SENIOR_BTN_HOVER, IDB_PNG_SENIOR_BTN_NORMAL, IDB_PNG_SENIOR_BTN_PRESSED, IDB_PNG_SENIOR_BTN_PRESSED);
//	m_btnCancel.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
//	m_btnCancel.SetTextColor(0x2E2E2E);
//
//	// ����ؼ�λ�õ���
//	CRect rcClient1;
//	GetClientRect(&rcClient1);
//	m_pMainTab->Create(GetSafeHwnd(), _T("DUISysSettingMainTab"), UI_WNDSTYLE_CHILD, 0, 1, rcClient1.top+TITLE_BAR_CY+1, rcClient1.Width()-2, 46); 
//	m_pMainTab->ShowWindow(TRUE); 
//
//	CRect rcClient;
//	GetWindowRect(&rcClient);
//	CWnd *pWorkArea = GetDlgItem(IDC_STATIC_AREA);
//	CRect rcWorkArea;
//	pWorkArea->GetWindowRect(&rcWorkArea);
//	int nWorkAreaHeight = rcWorkArea.Height();
//	rcWorkArea.top = rcClient.top + 46 + TITLE_BAR_CY+1;
//	rcWorkArea.bottom = rcWorkArea.top + nWorkAreaHeight;
//	rcWorkArea.left = rcClient.left + 1; 
//	rcWorkArea.right = rcClient.right - 1;
//	pWorkArea->MoveWindow(&rcWorkArea);
//
//	InitTabCtrl();
//	return TRUE;
//}
//
//void  CSystemSettingDlg::InitTabCtrl()
//{
//	//CRect rc;
//	//GetDlgItem(IDC_STATIC_AREA)->GetWindowRect(&rc);
//	//ScreenToClient(&rc);
//	////�����������̶�
//
//	m_SystemCommon.Create(IDD_WIZARD_CLUSTER_SYS_COMMON,this);    //��TAB�ϴ���
//	m_SystemCommon.MoveWindow(&rc);
//	m_SystemSecurity.Create(IDD_WIZARD_CLUSTER_SYS_SECURITY,this);  
//	m_SystemSecurity.MoveWindow(&rc);
//
//	//////Ĭ����ʾ��һ��
//	m_SystemCommon.ShowWindow(SW_SHOW);
//	m_SystemSecurity.ShowWindow(SW_HIDE);
//}
//
//std::tr1::shared_ptr<CSystemSettingDlg> CSystemSettingDlg::Show()
//{
//	pInstance = std::tr1::shared_ptr<CSystemSettingDlg>(new CSystemSettingDlg(GetDesktopWindow()));
//	pInstance->Create(CSystemSettingDlg::IDD,GetDesktopWindow());
//	pInstance->ShowWindow(SW_SHOW);
//	return pInstance;
//}
//
////void CSystemSettingDlg::UpdateUserInfo()
////{
////	m_UserInfo = g_ImUserInfo;
////	m_SystemSecurity.m_pSysSecVerify->SetUserInfo(m_UserInfo);///��֤��Ϣ
////	
////	//m_pPage1->SetUserInfo(m_UserInfo);
////	//m_pPage2->SetUserInfo(m_UserInfo);
////	//m_pPage3->SetUserInfo(m_UserInfo);
////}
//
//// �˵���ѡ�Ÿı��¼�
//LRESULT CSystemSettingDlg::OnMainTabSelChange(WPARAM wParam, LPARAM lParam)
//{
//	int nCurSel = int(wParam);
//	if(nCurSel == 0)
//	{	// ϵͳ����
//		m_SystemCommon.ShowWindow(SW_SHOW);
//		m_SystemSecurity.ShowWindow(SW_HIDE);
//	}
//	if(nCurSel == 1)
//	{	// ��ȫ����
//		m_SystemCommon.ShowWindow(SW_HIDE);
//		m_SystemSecurity.ShowWindow(SW_SHOW);
//	}
//
//	return 0;
//}
//
//void CSystemSettingDlg::OnBnClickedOk()
//{
//	m_SystemCommon.Save();
//	m_SystemSecurity.Save();
//
//	// �򸸴��ڷ�����Ϣ
//	// wParam = 0����ʾ�����ȷ��
//	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SYSTEM_SETTING_MSG, 0, 0);
//
//	m_pMainTab->SendMessage(WM_NCDESTROY);
//	DestroyWindow();	
//}
//
//
//void CSystemSettingDlg::OnBnClickedCancel()
//{
//	// TODO: �ڴ���ӿؼ�֪ͨ����������
//	m_pMainTab->SendMessage(WM_NCDESTROY);
//	DestroyWindow();
//}
//
//// ����Tabҳ��ʾ
//void CSystemSettingDlg::ShowTab(int nMainTabIndex, int nChildTabIndex)
//{
//	if(nMainTabIndex == 0)
//	{
//		m_pMainTab->SetCurSel(CUIMainTab::en_sel_bask);
//		m_SystemCommon.ShowTab(nChildTabIndex);
//	}
//
//	if(nMainTabIndex == 1)
//	{
//		m_pMainTab->SetCurSel(CUIMainTab::en_sel_security);
//		m_SystemSecurity.ShowTab(nChildTabIndex);
//	}
//}
//
//BOOL CSystemSettingDlg::PreTranslateMessage(MSG* pMsg)
//{
//	if  ( pMsg->message  ==  WM_KEYDOWN  )  
//	{  
//		switch(  pMsg->wParam  )  
//		{  
//		case VK_ESCAPE:
//		case VK_RETURN:
//			return TRUE;
//			break;
//		}
//	}
//
//	return CXSkinDlg::PreTranslateMessage(pMsg);
//}
//
//DuiLib::UILIB_RESOURCETYPE CUIMainTab::GetResourceType() const
//{
//	return UILIB_ZIPRESOURCE;
//}
//
//LPCTSTR CUIMainTab::GetResourceID() const
//{
//	return MAKEINTRESOURCE(IDR_ZIPRES1);
//}
//
//void CUIMainTab::Notify(DuiLib::TNotifyUI& msg)
//{
//	WindowImplBase::Notify(msg);
//	if(msg.sType == "selectchanged")
//	{
//		::SendMessage(::GetParent(m_hWnd), WM_MAINTAB_SEL_CHANGE, msg.pSender->GetName() == "baskSettingBtn" ? en_sel_bask : en_sel_security, 0);
//	}
//}
//
//void CUIMainTab::SetCurSel(EN_SEL en)
//{
//	COptionUI* pOptionBask = static_cast<COptionUI*>(m_PaintManager.FindControl("baskSettingBtn"));
//	COptionUI* pOptionSecurity = static_cast<COptionUI*>(m_PaintManager.FindControl("securitySettingBtn"));
//	if(pOptionBask != NULL && pOptionSecurity != NULL) 
//	{
//		pOptionBask->Selected(en == en_sel_bask);
//		pOptionSecurity->Selected(en == en_sel_security);
//	}
//}