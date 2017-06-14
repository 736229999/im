// SystemSettingSecurityDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SystemSettingSecurityDlg.h"
#include "afxdialogex.h"


// CSystemSettingSecurityDlg �Ի���

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
		_T("΢���ź�"));

	// tab����
	m_colSecuritySetting.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colSecuritySetting.SetFont(&m_font);
	m_colSecuritySetting.AddItem("����");
	m_colSecuritySetting.AddItem("��֤����");
	m_colSecuritySetting.AddItem("��Ϣ��¼");
	m_colSecuritySetting.AddItem("�ļ�����");
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


// CSystemSettingSecurityDlg ��Ϣ�������


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
	//////////8��ҳ���ӿ�ܲ���//////////////////////
	 ////����
	m_pSysSecPwd = new CSysSecurityPwd;
	m_pSysSecPwd->Create(GetSafeHwnd(), _T("DUISysSettingModPsw"), UI_WNDSTYLE_CHILD, 0, rect.left, rect.top+5, 382, 270); 
	m_pSysSecPwd->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecPwd);

    ////��֤����
	m_pSysSecVerify = new CSysSecurityVerify;
	m_pSysSecVerify->Create(GetSafeHwnd(), _T("DUISysSettingVerifyCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecVerify->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecVerify);

	////��Ϣ��¼
	m_pSysSecMsglog = new CSysSecurityMsglog;
	m_pSysSecMsglog->Create(GetSafeHwnd(), _T("DUISysSettingSecurityMsgLog"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecMsglog->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecMsglog);

	////�ļ�����
	m_pSysSecFileTransfer = new CSysSecurityFileTransfer;
	m_pSysSecFileTransfer->Create(GetSafeHwnd(), _T("DUISysSettingSecurityLevelCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSecFileTransfer->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSecFileTransfer);
}

void CSystemSettingSecurityDlg::OnDestroy()
{
	CDialog::OnDestroy();
	// TODO: �ڴ˴������Ϣ����������
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
			pDC->SetTextColor(RGB(255,0,0));//��255,0��0�� //(74,123,211)
		}

	}

	if ( nCtlColor==CTLCOLOR_DLG  )
	{
		hbr = g_hWhiteBrush;
	}

	return hbr;
}


// �˵���ѡ�Ÿı��¼�
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

// ����Tabҳ��ʾ
void CSystemSettingSecurityDlg::ShowTab(int nTabIndex)
{
	m_colSecuritySetting.SetCurSel(nTabIndex);
}

// ��������
void CSystemSettingSecurityDlg::Save()
{
	m_pSysSecVerify->SaveConfig();
	m_pSysSecMsglog->Save();
	m_pSysSecFileTransfer->SaveConfig();
}