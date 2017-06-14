// SystemSettingSecurityDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemSettingSecurityDlg.h"
#include "afxdialogex.h"


// CSystemSettingSecurityDlg 对话框

IMPLEMENT_DYNAMIC(CSystemSettingSecurityDlg, CDialog)

CSystemSettingSecurityDlg::CSystemSettingSecurityDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemSettingSecurityDlg::IDD, pParent)
{
	m_font.CreateFont(
		17,                        // nHeight
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
	m_colSecuritySetting.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colSecuritySetting.SetFont(&m_font);
	m_colSecuritySetting.AddItem("密码");
	m_colSecuritySetting.AddItem("验证设置");
	m_colSecuritySetting.AddItem("消息记录");
	m_colSecuritySetting.AddItem("文件传输");
	m_colSecuritySetting.SetBackgroundColor(RGB(235, 235, 235), RGB(255, 255, 255), RGB(245, 245, 245));
	m_colSecuritySetting.SetItemTextColor(0x595959, 0x595959, 0x595959);
	m_colSecuritySetting.SetFrameModol(FRAME_DRAW_NULL);
}

CSystemSettingSecurityDlg::~CSystemSettingSecurityDlg()
{
	if(m_pSysSecPwd != NULL)
	{
		delete m_pSysSecPwd;
		m_pSysSecPwd = NULL;
	}
	if(m_pSysSecVerify != NULL)
	{
		delete m_pSysSecVerify;
		m_pSysSecVerify = NULL;
	}
	if(m_pSysSecMsglog != NULL)
	{
		delete m_pSysSecMsglog;
		m_pSysSecMsglog = NULL;
	}
	if(m_pSysSecFileTransfer != NULL)
	{
		delete m_pSysSecFileTransfer;
		m_pSysSecFileTransfer = NULL;
	}
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}
}

void CSystemSettingSecurityDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FRAME_SECURITY, m_ctlFrameSecurity);
	DDX_Control(pDX, IDC_STATIC_SECURITY_SETTING, m_colSecuritySetting);
}


BEGIN_MESSAGE_MAP(CSystemSettingSecurityDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
END_MESSAGE_MAP()


// CSystemSettingSecurityDlg 消息处理程序


BOOL CSystemSettingSecurityDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CreateSecurityDlg();
	m_colSecuritySetting.SetCurSel(0);
	return TRUE;
}

void  CSystemSettingSecurityDlg::CreateSecurityDlg()
{
	CRect rect, rtScroll;
	m_ctlFrameSecurity.GetClientRect(&rect);
	m_ctlFrameSecurity.MapWindowPoints(this, rect);
	//////////8个页面子框架布局//////////////////////
	 ////密码
	m_pSysSecPwd = new CSysSecurityPwd;
	m_pSysSecPwd->Create(GetSafeHwnd(), _T("DUISysSettingModPsw"), UI_WNDSTYLE_CHILD, 0, rect.left, rect.top+5, 382, 270); 
	m_pSysSecPwd->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecPwd);

    ////验证设置
	m_pSysSecVerify = new CSysSecurityVerify;
	m_pSysSecVerify->Create(GetSafeHwnd(), _T("DUISysSettingVerifyCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecVerify->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecVerify);

	////消息记录
	m_pSysSecMsglog = new CSysSecurityMsglog;
	m_pSysSecMsglog->Create(GetSafeHwnd(), _T("DUISysSettingSecurityMsgLog"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecMsglog->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecMsglog);

	////文件传输
	m_pSysSecFileTransfer = new CSysSecurityFileTransfer;
	m_pSysSecFileTransfer->Create(GetSafeHwnd(), _T("DUISysSettingSecurityLevelCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecFileTransfer->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecFileTransfer);
}

void CSystemSettingSecurityDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: 在此处添加消息处理程序代码
}

HBRUSH CSystemSettingSecurityDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


// 菜单栏选着改变事件
LRESULT CSystemSettingSecurityDlg::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	if(pCtrl == &m_colSecuritySetting)
	{
		int nCurSel = HIWORD(lParam);

		if(nCurSel >=0 && nCurSel <= 3)
		{
			for(int i = 0; i < m_vecpChildWnd.size(); ++i)
			{
				if(i == 0)
				{
					m_pSysSecPwd->ShowWindow(i == nCurSel);
					continue;
				}
				else if(i == 1)
				{
					m_pSysSecVerify->ShowWindow(i == nCurSel);
					continue;
				}
				else if(i == 2)
				{
					m_pSysSecMsglog->ShowWindow(i == nCurSel);
					continue;
				}
				else if(i == 3)
				{
					m_pSysSecFileTransfer->ShowWindow(i == nCurSel);
					continue;
				}
				CWnd *pChild = m_vecpChildWnd[i];
				pChild->ShowWindow(i == nCurSel ? SW_SHOW : SW_HIDE);
			}
		}
	}

	return 0;
}

// 控制Tab页显示
void CSystemSettingSecurityDlg::ShowTab(int nTabIndex)
{
	m_colSecuritySetting.SetCurSel(nTabIndex);
}

// 保存配置
void CSystemSettingSecurityDlg::Save()
{
	m_pSysSecVerify->SaveConfig();
	m_pSysSecMsglog->Save();
	m_pSysSecFileTransfer->SaveConfig();
}