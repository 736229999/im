// im\SAPageHotkey.cpp : 实现文件
//

#include "stdafx.h"
#include "SAPageHotkey.h"
#include "Localdata.h"
#include ".\sapagehotkey.h"
#include "..\MainFrame.h"


// CSAPageHotkey 对话框

IMPLEMENT_DYNAMIC(CSAPageHotkey, CSABasePage)
CSAPageHotkey::CSAPageHotkey(CWnd* pParent /*=NULL*/)
	: CSABasePage(CSAPageHotkey::IDD, pParent)
{
	Create(CSAPageHotkey::IDD,pParent);
}

CSAPageHotkey::~CSAPageHotkey()
{
}

void CSAPageHotkey::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_HOTKEY_MSG, m_hotkey);
}


BEGIN_MESSAGE_MAP(CSAPageHotkey, CSABasePage)
//	ON_BN_SETFOCUS(IDC_HOTKEY_CUSTOM, OnBnSetfocusHotkeyCustom)
//	ON_BN_SETFOCUS(IDC_HOTKEY_DEFAULT, OnBnSetfocusHotkeyDefault)
ON_BN_CLICKED(IDC_HOTKEY_CUSTOM, OnBnClickedHotkeyCustom)
ON_BN_CLICKED(IDC_HOTKEY_DEFAULT, OnBnClickedHotkeyDefault)
ON_BN_CLICKED(IDC_ENTER, &CSAPageHotkey::OnBnClickedEnter)
END_MESSAGE_MAP()


// CSAPageHotkey 消息处理程序

BOOL CSAPageHotkey::OnInitDialog()
{
	CSABasePage::OnInitDialog();


	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();

	CButton* pEnter=(CButton*) GetDlgItem(IDC_ENTER);
	CButton* pCtrlEnter=(CButton*) GetDlgItem(IDC_CTRLENTER);
	CButton * pHotkeyDefault = (CButton *) GetDlgItem(IDC_HOTKEY_DEFAULT);
	CButton * pHotkeyCustom = (CButton *) GetDlgItem(IDC_HOTKEY_CUSTOM);

	WORD wVK;
	WORD wMod;

	g_LocalData.GetMsgHotKey(wVK,wMod);
	m_hotkey.SetHotKey(wVK,wMod);

	if(wVK == 0 && wMod == 0)
	{
		m_hotkey.EnableWindow(FALSE);
		pHotkeyDefault->SetCheck(TRUE);
	}
	else
	{
		pHotkeyCustom->SetCheck(TRUE);
	}

	if(cfg->sendMsgKey==SMK_CTRLENTER)
	{
		pCtrlEnter->SetCheck(TRUE);
		pEnter->SetCheck(FALSE);
	}
	else
	{
		pCtrlEnter->SetCheck(FALSE);
		pEnter->SetCheck(TRUE);;
	};

	return TRUE;  
}


void CSAPageHotkey::SaveConfig()
{
	HOTKEY_CONFIG *cfg;
	cfg=g_LocalData.GetHotKeyConfig();


	CButton* pEnter=(CButton*) GetDlgItem(IDC_ENTER);
	CButton* pCtrlEnter=(CButton*) GetDlgItem(IDC_CTRLENTER);

	if(pCtrlEnter->GetCheck())
	{
		cfg->sendMsgKey=SMK_CTRLENTER;
	}
	else
	{
		cfg->sendMsgKey=SMK_ENTER;
	};
	WORD wVK;
	WORD wMod;
	WORD wMod2 = 0;
	
	CButton * pHotkeyDefault = (CButton *) GetDlgItem(IDC_HOTKEY_DEFAULT);

	if(pHotkeyDefault->GetCheck())
	{
		wVK = 0;
		wMod = 0;
	}
	else
	{
		m_hotkey.GetHotKey(wVK,wMod);
	}
	
	g_LocalData.SaveMsgHotKey(wVK,wMod);

	HWND hWnd = CMainFrame::GetInstance()->GetHWND();
	BOOL b;
	if(hWnd)
	{
		int nHotkeyID = GlobalAddAtom("mini139_show_message");
		b = UnregisterHotKey(hWnd,nHotkeyID);
		if(wVK == 0 && wMod == 0)
		{
			b = RegisterHotKey(hWnd,nHotkeyID,MOD_ALT,'Z');
		}
		else
		{
			if(HOTKEYF_ALT & wMod)
				wMod2 |= MOD_ALT;
			if(HOTKEYF_CONTROL & wMod)
				wMod2 |= MOD_CONTROL;
			if(HOTKEYF_SHIFT & wMod)
				wMod2 |= MOD_SHIFT;
			b = RegisterHotKey(hWnd,nHotkeyID,wMod2,wVK);
		}
	}
	
}

//void CSAPageHotkey::OnBnSetfocusHotkeyCustom()
//{
//	GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(TRUE);
//
//}

//void CSAPageHotkey::OnBnSetfocusHotkeyDefault()
//{
//	GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(FALSE);
//}


void CSAPageHotkey::OnBnClickedHotkeyCustom()
{
	GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(TRUE);
}

void CSAPageHotkey::OnBnClickedHotkeyDefault()
{
	GetDlgItem(IDC_HOTKEY_MSG)->EnableWindow(FALSE);
}


void CSAPageHotkey::OnBnClickedEnter()
{
	// TODO: 在此添加控件通知处理程序代码
}
