// SysCommonAutoReply.cpp : 实现文件
//

#include "stdafx.h"
#include "SysCommonAutoReply.h"
#include "afxdialogex.h"

enum TAB_CTRL_ID
{
	TAB_CTRL_AUTO_REPLY   = 0,
	TAB_CTRL_AUTO_ACCEPT = 1,
};
// CSysCommonAutoReply 对话框

IMPLEMENT_DYNAMIC(CSysCommonAutoReply, CDialog)

CSysCommonAutoReply::CSysCommonAutoReply(CWnd* pParent /*=NULL*/)
	: CDialog(CSysCommonAutoReply::IDD, pParent)
{
	m_font.CreateFont(
		19,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	// tab设置
	m_colCtrl.SetDirectMode(CColumnCtrl::EN_X_DIRE);
	m_colCtrl.SetFont(&m_font);
	m_colCtrl.AddItem("自动回复");
#if defined(_BUSINESS_VESION) && !defined(_WORK_EX_)
	m_colCtrl.AddItem("自动接待");
#endif
	m_colCtrl.SetBackgroundColor(RGB(255, 255, 255), RGB(231, 231, 231), RGB(240, 240, 240));
	m_colCtrl.SetItemTextColor(0x2E2E2E, 0x2E2E2E, 0x2E2E2E);
	m_colCtrl.SetFrameModol(FRAME_DRAW_NULL);

	m_pAutoReply  = new CUIAutoReply();
	m_AutoAcptDlg = new CAutoAcceptDlg();
}

CSysCommonAutoReply::~CSysCommonAutoReply()
{
	if(::IsWindow(GetSafeHwnd()))
	{
		m_pAutoReply->SendMessage(WM_NCDESTROY);
		DestroyWindow();
	}

	if(m_pAutoReply != NULL)
	{
		delete m_pAutoReply;
		m_pAutoReply = NULL;
	}

	if(m_AutoAcptDlg != NULL)
	{
		delete m_AutoAcptDlg;
		m_AutoAcptDlg = NULL;
	}
}

void CSysCommonAutoReply::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COL, m_colCtrl);
}


BEGIN_MESSAGE_MAP(CSysCommonAutoReply, CDialog)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
END_MESSAGE_MAP()

BOOL CSysCommonAutoReply::OnInitDialog()
{
	CDialog::OnInitDialog();
	InitTabCtrl();
	m_colCtrl.SetCurSel(0);

	g_LocalData.GetAllAutoReplyItem(m_pAutoReply->m_vecItem);
	CRect rcTab;
	m_colCtrl.GetWindowRect(rcTab);
	ScreenToClient(&rcTab);
	m_pAutoReply->Create(GetSafeHwnd(), _T("DUISysSettingAudoReply"), UI_WNDSTYLE_CHILD, 0, rcTab.left, rcTab.bottom+2, 382, 247); 
	m_pAutoReply->ShowWindow(TRUE); 

	return TRUE;
}
// CSysCommonAutoReply 消息处理程序
void  CSysCommonAutoReply::InitTabCtrl()
{
	//m_AutoAcptDlg.Create(CAutoAcceptDlg::IDD,this);  

	////////默认显示第一个
	//CRect rcTab;
	//m_colCtrl.GetWindowRect(rcTab);
	//ScreenToClient(&rcTab);
	//m_AutoAcptDlg.MoveWindow(rcTab.left, rcTab.bottom+2, 382, 247);

	CRect rcTab;
	m_colCtrl.GetWindowRect(rcTab);
	ScreenToClient(&rcTab);
	m_AutoAcptDlg->Create(GetSafeHwnd(), _T("DUISysSettingAutoAccept"), UI_WNDSTYLE_CHILD, 0, rcTab.left, rcTab.bottom+2, 382, 247); 

#ifdef BUSINESS_VESION
	m_AutoAcptDlg->ShowWindow(SW_HIDE);
#endif
}

HBRUSH CSysCommonAutoReply::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nId = pWnd->GetDlgCtrlID();

		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
		{

		}
		else
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = g_hWhiteBrush;
		}

		if ( nId==IDC_STATIC_GROUP )
		{
			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
		}

	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}

// 保存配置
void CSysCommonAutoReply::Save()
{
	m_AutoAcptDlg->Save();
	m_pAutoReply->Save();
}

// 菜单栏选着改变事件
LRESULT CSysCommonAutoReply::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	if(pCtrl == &m_colCtrl)
	{
		int nCurSel = HIWORD(lParam);
		if(nCurSel == 0)
		{
			if(::IsWindow(m_pAutoReply->GetHWND()))
				m_pAutoReply->ShowWindow(true);
			m_AutoAcptDlg->ShowWindow(SW_HIDE);
		}

		if(nCurSel == 1)
		{
			if(::IsWindow(m_pAutoReply->GetHWND()))
				m_pAutoReply->ShowWindow(false);
			m_AutoAcptDlg->ShowWindow(SW_SHOW);
		}

#ifdef BUSINESS_VESION
		m_AutoAcptDlg->ShowWindow(SW_HIDE);
#endif
	}

	return 0;
}