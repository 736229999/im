
// SysCommonMsg.cpp : 实现文件
//
#include "stdafx.h"
#include "SysCommonMsg.h"
#include "afxdialogex.h"
#include "..\Im\LocalData.h"
#include "MainFrame.h"
using namespace DuiLib;


void CSysCommonMsg::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pCheckBanShake = static_cast<COptionUI*>(m_PaintManager.FindControl("BanShake"));
	m_pRadioEnterKey = static_cast<COptionUI*>(m_PaintManager.FindControl("EnterKey"));
	m_pRadioCtrlEnterKey = static_cast<COptionUI*>(m_PaintManager.FindControl("CtrlEnterKey"));
	m_pCheckViewMsg = static_cast<COptionUI*>(m_PaintManager.FindControl("ViewMsg"));
	m_pCheckSnapShot = static_cast<COptionUI*>(m_PaintManager.FindControl("SnapShot"));

	///初始化设置///////////////////
	////字体修改
	m_hotkeyMsglog.SetFont(&g_font12);
	m_hotkeySnapShot.SetFont(&g_font12);

	m_pRadioEnterKey->Selected(TRUE);
	m_pCheckViewMsg->Selected(TRUE);
	m_pCheckSnapShot->Selected(TRUE);

	WORD wVK;
	WORD wMod;
	g_LocalData.GetMsgHotKey(wVK,wMod);
	m_hotkeyMsglog.SetHotKey(wVK,wMod);//消息记录快捷键
	
	g_LocalData.GetSnapShotHotKey(wVK,wMod);
	m_hotkeySnapShot.SetHotKey(wVK,wMod);//截图快捷键
	
	WORD nVK = 0;
	g_LocalData.GetShakeWinConfig(nVK);
	m_pCheckBanShake->Selected(nVK == 1);
	
	//((CComboBox*)GetDlgItem(IDC_COMBO_STATUS))->AddString(_T("离开"));
	//((CComboBox*)GetDlgItem(IDC_COMBO_STATUS))->AddString(_T("忙碌"));
	//
	//((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TIME))->SetRange(0, 60);

	/////忙时状态切换
	//WORD nStatus = 0;
	//g_LocalData.GetWakeupConfig(nVK,nStatus);
	//if(nVK > 0)////支持空闲时状态切换
	//{
	//    ((CButton*)GetDlgItem(IDC_CHECK_STATUS))->SetCheck(TRUE);
	//	CString strTxt = _T("");
	//	strTxt.Format(_T("%d"),nVK);
	//	m_cEdit.SetWindowText(strTxt);
	//}
	//
	//m_cComboStatus.SetCurSel(nStatus);
	
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();
	if(cfg->sendMsgKey == SMK_ENTER)
	{
		m_pRadioEnterKey->Selected(TRUE);
		m_pRadioCtrlEnterKey->Selected(FALSE);
	}
	else
	{
		m_pRadioEnterKey->Selected(FALSE);
		m_pRadioCtrlEnterKey->Selected(TRUE);
	}
}


// 消息
void CSysCommonMsg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(m_pCheckViewMsg->IsSelected())   ////消息热键
	{
	     m_hotkeyMsglog.EnableWindow(TRUE);
	}
	else
	{
		 m_hotkeyMsglog.EnableWindow(FALSE);
	}

	if(m_pCheckSnapShot->IsSelected())  ////截图热键
	{

		m_hotkeySnapShot.EnableWindow(TRUE);
	}
	else
	{
		m_hotkeySnapShot.EnableWindow(FALSE);
	}
}

void CSysCommonMsg::SaveConfig()
{
	/////////////////////////电脑处于空闲/////////////////////////////////////
	//CButton * pWakeup = (CButton *) GetDlgItem(IDC_CHECK_STATUS);
	//if(pWakeup->GetCheck())
	//{
    //    CString strTxt  = _T("");
	//    ((CEdit*)GetDlgItem(IDC_EDIT_TIME))->GetWindowText(strTxt);///空闲时间设置
	//    int nTime = _ttol(strTxt);
	//	  int nStatus = m_cComboStatus.GetCurSel();
	//    if(nTime>0)
	//    {
	//	   g_LocalData.SaveWakeupConfig(nTime,nStatus);
	//    }
	//}
	//else
	//{
	//	   g_LocalData.SaveWakeupConfig(0,0);
	//}
	/////////////////////////振屏设置/////////////////////////////////////////
	WORD wVKShake;
	if(m_pCheckBanShake->IsSelected())
	{
		wVKShake = 1;		// 1是支持振屏
		g_LocalData.SaveShakeWinConfig(wVKShake);
	}
	else
	{
		wVKShake = 0;		// 0是不支持振屏	
		g_LocalData.SaveShakeWinConfig(wVKShake);
	}

	/////////////////////////发送消息/////////////////////////////////////
	HOTKEY_CONFIG *cfg = NULL;
	cfg=g_LocalData.GetHotKeyConfig();
	if(m_pRadioCtrlEnterKey->IsSelected())
	{
		cfg->sendMsgKey=SMK_CTRLENTER;
	}
	else
	{
		cfg->sendMsgKey=SMK_ENTER;
	};
	/////////////////////////查看消息保存////////////////////////////////////
	WORD wVKMsg;
	WORD wModMsg;
	WORD wModMsg2;
	BOOL b = FALSE;
	if(m_pCheckViewMsg->IsSelected())
	{
		m_hotkeyMsglog.GetHotKey(wVKMsg,wModMsg);////当前设置属性值
		g_LocalData.SaveMsgHotKey(wVKMsg,wModMsg);
	}

	//////////////////////////截屏保存//////////////////////////////////////
	WORD wVKSnapshot;
	WORD wModSnapshot;
	WORD wModSnapshot2;
	if(m_pCheckSnapShot->IsSelected())
	{
		m_hotkeySnapShot.GetHotKey(wVKSnapshot,wModSnapshot);////当前设置属性值
		g_LocalData.SaveSnapShotHotKey(wVKSnapshot,wModSnapshot);
	}
	::SendMessage(AfxGetMainWnd()->GetSafeHwnd(), WM_SYSTEM_SETTING_MSG,1, 0);		
}

// CSysCommonMsg 对话框
////
////IMPLEMENT_DYNAMIC(CSysCommonMsg, CDialog)
////
////CSysCommonMsg::CSysCommonMsg(CWnd* pParent /*=NULL*/)
////	: CDialog(CSysCommonMsg::IDD, pParent)
////{
////
////}
////
////CSysCommonMsg::~CSysCommonMsg()
////{
////}
////
////void CSysCommonMsg::DoDataExchange(CDataExchange* pDX)
////{
////	CDialog::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_HOTKEY_MSG, m_hotkeyMsglog);
////	DDX_Control(pDX, IDC_HOTKEY_SNAPSHOT, m_hotkeySnapShot);
////	DDX_Control(pDX, IDC_EDIT_TIME, m_cEdit);
////	DDX_Control(pDX, IDC_COMBO_STATUS, m_cComboStatus);
////	DDX_Control(pDX, IDC_CHECK_SHAKE, m_checkShake);
////	DDX_Control(pDX, IDC_RADIO_ENTER, m_radioEnter);
////	DDX_Control(pDX, IDC_RADIO_CTRL_ENTER, m_radioCtrlEnter);
////	DDX_Control(pDX, IDC_CHECK_LOG, m_checkLog);
////	DDX_Control(pDX, IDC_CHECK_SNAPSHOT, m_checkSnapshot);
////}
////
////
////BEGIN_MESSAGE_MAP(CSysCommonMsg, CDialog)
////	ON_WM_CTLCOLOR()
////	ON_BN_CLICKED(IDC_RADIO_ENTER, &CSysCommonMsg::OnBnClickedRadioEnter)
////	ON_BN_CLICKED(IDC_CHECK_LOG, &CSysCommonMsg::OnBnClickedCheckLog)
////	ON_BN_CLICKED(IDC_CHECK_SNAPSHOT, &CSysCommonMsg::OnBnClickedCheckSnapshot)
////	ON_BN_CLICKED(IDC_CHECK_STATUS, &CSysCommonMsg::OnBnClickedCheckStatus)
////END_MESSAGE_MAP()
////
////
////// CSysCommonMsg 消息处理程序
////
////BOOL CSysCommonMsg::OnInitDialog()
////{
////	CDialog::OnInitDialog();
////
////	// 遍历所有子控件，设置控件字体
////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////	while(hWndChild)  
////	{  
////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////		if(GetDlgItem(IDC_STATIC_MSG_SETTTING)->GetSafeHwnd() == hWndChild 
////			|| GetDlgItem(IDC_STATIC_MSG_SEND_TYPE)->GetSafeHwnd() == hWndChild
////			|| GetDlgItem(IDC_STATIC_SHORTCUT_KEY)->GetSafeHwnd() == hWndChild)
////		{
////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
////		}
////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////	}
////
////	((CButton*)GetDlgItem(IDC_RADIO_ENTER))->SetCheck(TRUE);
////	((CButton*)GetDlgItem(IDC_CHECK_LOG))->SetCheck(TRUE);
////	((CButton*)GetDlgItem(IDC_CHECK_SNAPSHOT))->SetCheck(TRUE);
////	
////	((CComboBox*)GetDlgItem(IDC_COMBO_STATUS))->AddString(_T("离开"));
////	((CComboBox*)GetDlgItem(IDC_COMBO_STATUS))->AddString(_T("忙碌"));
////
////	((CSpinButtonCtrl *)GetDlgItem(IDC_SPIN_TIME))->SetRange(0, 60);
////
////	WORD wVK;
////	WORD wMod;
////	g_LocalData.GetMsgHotKey(wVK,wMod);
////	m_hotkeyMsglog.SetHotKey(wVK,wMod);//消息记录
////
////	g_LocalData.GetSnapShotHotKey(wVK,wMod);
////	m_hotkeySnapShot.SetHotKey(wVK,wMod);//截图
////
////	WORD nVK = 0;
////	g_LocalData.GetShakeWinConfig(nVK);
////	if(nVK != 0)////支持振屏
////	{
////	   ((CButton*)GetDlgItem(IDC_CHECK_SHAKE))->SetCheck(TRUE);
////	}
////
////	WORD nStatus = 0;
////	g_LocalData.GetWakeupConfig(nVK,nStatus);
////	if(nVK > 0)////支持空闲时状态切换
////	{
////        ((CButton*)GetDlgItem(IDC_CHECK_STATUS))->SetCheck(TRUE);
////		CString strTxt = _T("");
////		strTxt.Format(_T("%d"),nVK);
////		m_cEdit.SetWindowText(strTxt);
////	}
////
////	m_cComboStatus.SetCurSel(nStatus);
////
////	HOTKEY_CONFIG *cfg;
////	cfg=g_LocalData.GetHotKeyConfig();
////	CButton* pEnter=(CButton*) GetDlgItem(IDC_RADIO_ENTER);
////	CButton* pCtrlEnter=(CButton*) GetDlgItem(IDC_RADIO_CTRL_ENTER);
////	if(cfg->sendMsgKey == SMK_ENTER)
////	{
////		pEnter->SetCheck(1);
////		pCtrlEnter->SetCheck(0);
////	}
////	else
////	{
////		pEnter->SetCheck(0);
////		pCtrlEnter->SetCheck(1);
////	}
////
////	m_hotkeyMsglog.SetBorderColor(0xc8c8c8);
////	m_hotkeySnapShot.SetBorderColor(0xc8c8c8);
////
////	return TRUE;
////}
////
////
////HBRUSH CSysCommonMsg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
////void CSysCommonMsg::OnBnClickedRadioEnter()
////{
////	// TODO: 在此添加控件通知处理程序代码
////}
////
////void CSysCommonMsg::SaveConfig()
////{
////	/////////////////////////电脑处于空闲/////////////////////////////////////
////	CButton * pWakeup = (CButton *) GetDlgItem(IDC_CHECK_STATUS);
////	if(pWakeup->GetCheck())
////	{
////         CString strTxt  = _T("");
////	    ((CEdit*)GetDlgItem(IDC_EDIT_TIME))->GetWindowText(strTxt);///空闲时间设置
////	    int nTime = _ttol(strTxt);
////		int nStatus = m_cComboStatus.GetCurSel();
////	    if(nTime>0)
////	    {
////		   g_LocalData.SaveWakeupConfig(nTime,nStatus);
////	    }
////	}
////	else
////	{
////		   g_LocalData.SaveWakeupConfig(0,0);
////	}
////	/////////////////////////振屏设置/////////////////////////////////////////
////	WORD wVKShake;
////	CButton * pShake = (CButton *) GetDlgItem(IDC_CHECK_SHAKE);
////	if(pShake->GetCheck())
////	{
////		wVKShake = 1;
////		g_LocalData.SaveShakeWinConfig(wVKShake);
////	}
////	else
////	{
////		wVKShake = 0;
////		g_LocalData.SaveShakeWinConfig(wVKShake);
////	}
////
////	/////////////////////////发送消息/////////////////////////////////////
////	HOTKEY_CONFIG *cfg;
////	cfg=g_LocalData.GetHotKeyConfig();
////
////	CButton* pEnter=(CButton*) GetDlgItem(IDC_RADIO_ENTER);
////	CButton* pCtrlEnter=(CButton*) GetDlgItem(IDC_RADIO_CTRL_ENTER);
////
////	if(pCtrlEnter->GetCheck())
////	{
////		cfg->sendMsgKey=SMK_CTRLENTER;
////	}
////	else
////	{
////		cfg->sendMsgKey=SMK_ENTER;
////	};
////	/////////////////////////查看消息////////////////////////////////////
////	WORD wVKMsg;
////	WORD wModMsg;
////	WORD wModMsg2;
////	CButton * pHotkeyMsglog = (CButton *) GetDlgItem(IDC_CHECK_LOG);
////	HWND hWnd = AfxGetMainWnd()->GetSafeHwnd();
////	BOOL b = FALSE;
////	if(pHotkeyMsglog->GetCheck())
////	{
////		m_hotkeyMsglog.GetHotKey(wVKMsg,wModMsg);////当前设置属性值
////		g_LocalData.SaveMsgHotKey(wVKMsg,wModMsg);
////		if(hWnd)
////		{
////			int nHotkeyID = GlobalAddAtom("mini139_show_message");
////			b = UnregisterHotKey(hWnd,nHotkeyID);
////			if(wVKMsg == 0 && wModMsg == 0)
////			{
////				b = RegisterHotKey(hWnd,nHotkeyID,MOD_SHIFT|MOD_ALT,'A');
////			}
////			else
////			{
////				if(HOTKEYF_ALT & wModMsg)
////					wModMsg2 |= MOD_ALT;
////				if(HOTKEYF_CONTROL & wModMsg)
////					wModMsg2 |= MOD_CONTROL;
////				if(HOTKEYF_SHIFT & wModMsg)
////					wModMsg2 |= MOD_SHIFT;
////				b = RegisterHotKey(hWnd,nHotkeyID,wModMsg2,wVKMsg);
////			}
////		}
////	}
////
////	//////////////////////////截屏//////////////////////////////////////
////	WORD wVKSnapshot;
////	WORD wModSnapshot;
////	WORD wModSnapshot2;
////	CButton * pHotkeySnapShot = (CButton *) GetDlgItem(IDC_CHECK_SNAPSHOT);
////	if(pHotkeySnapShot->GetCheck())
////	{
////		m_hotkeySnapShot.GetHotKey(wVKSnapshot,wModSnapshot);////当前设置属性值
////		g_LocalData.SaveSnapShotHotKey(wVKSnapshot,wModSnapshot);
////		hWnd = AfxGetMainWnd()->GetSafeHwnd();
////		if(hWnd)
////		{
////			int nHotkeyID = GlobalAddAtom("mini139_show_snapshot");
////			b = UnregisterHotKey(hWnd,nHotkeyID);
////			if(wVKSnapshot == 0 && wModSnapshot == 0)
////			{
////				b = RegisterHotKey(hWnd,nHotkeyID,MOD_SHIFT|MOD_ALT,'Z');
////			}
////			else
////			{
////				if(HOTKEYF_ALT & wModSnapshot)
////					wModSnapshot2 |= MOD_ALT;
////
////				if(HOTKEYF_CONTROL & wModSnapshot)
////					wModSnapshot2 |= MOD_CONTROL;
////
////				if(HOTKEYF_SHIFT & wModSnapshot)
////					wModSnapshot2 |= MOD_SHIFT;
////
////				b = RegisterHotKey(hWnd,nHotkeyID,wModSnapshot2,wVKSnapshot);
////			}
////		}
////
////	}
////	
////}
////
////void CSysCommonMsg::OnBnClickedCheckLog()
////{
////	// TODO: 在此添加控件通知处理程序代码
////	CButton * pHotkeyMsglog = (CButton *) GetDlgItem(IDC_CHECK_LOG);
////	if(pHotkeyMsglog->GetCheck())
////	{
////       GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(TRUE);
////	}
////	else
////	{
////		GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(FALSE);
////	}
////	
////}
////
////
////void CSysCommonMsg::OnBnClickedCheckSnapshot()
////{
////	// TODO: 在此添加控件通知处理程序代码
////	CButton * pHotkeySnapShot = (CButton *) GetDlgItem(IDC_CHECK_SNAPSHOT);
////	if(pHotkeySnapShot->GetCheck())
////	{
////		GetDlgItem(IDC_HOTKEY_SNAPSHOT)->EnableWindow(TRUE);
////	}
////	else
////	{
////		GetDlgItem(IDC_HOTKEY_SNAPSHOT)->EnableWindow(FALSE);
////	}
////}
////
////void   CSysCommonMsg::OnVScroll(UINT   nSBCode,   UINT   nPos,   CScrollBar*   pScrollBar)     
////{   
////	switch(pScrollBar->GetDlgCtrlID())   
////	{   
////	case   IDC_SPIN_TIME:     
////		{   
////			float   nStep=0.1;   
////			CString   str;   
////			str.Format("%f",nPos*nStep);
////			m_cEdit.SetWindowText(str);   
////		}   
////		break;   
////	}   
////}
////
////void CSysCommonMsg::OnBnClickedCheckStatus()
////{
////	// TODO: 在此添加控件通知处理程序代码
////}
