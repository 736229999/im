// SystemSettingCommonDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SystemSettingCommonDlg.h"
#include "afxdialogex.h"


// CSystemSettingCommonDlg 对话框

IMPLEMENT_DYNAMIC(CSystemSettingCommonDlg, CDialog)

CSystemSettingCommonDlg::CSystemSettingCommonDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CSystemSettingCommonDlg::IDD, pParent)
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
	m_colBasicSetting.SetDirectMode(CColumnCtrl::EN_Y_DIRE);
	m_colBasicSetting.SetFont(&m_font);
	m_colBasicSetting.AddItem("基本设置");
	m_colBasicSetting.AddItem("消息设置");
	m_colBasicSetting.AddItem("声音");
	m_colBasicSetting.AddItem("语音视频");
	m_colBasicSetting.AddItem("文件管理");
	m_colBasicSetting.AddItem("自动回复");
	//m_colBasicSetting.AddItem("快捷短语");
	m_colBasicSetting.AddItem("软件更新");
	m_colBasicSetting.SetBackgroundColor(RGB(235, 235, 235), RGB(255, 255, 255), RGB(245, 245, 245));
	m_colBasicSetting.SetItemTextColor(0x595959, 0x595959, 0x595959);
	m_colBasicSetting.SetFrameModol(FRAME_DRAW_NULL);
}

CSystemSettingCommonDlg::~CSystemSettingCommonDlg()
{
	if(m_pSysComBasic != NULL)
	{
		delete m_pSysComBasic;
		m_pSysComBasic = NULL;
	}

	if(m_pSysComMsg != NULL)
	{
		delete m_pSysComMsg;
		m_pSysComMsg = NULL;
	}

	if(m_pSysComVoice != NULL)
	{
		delete m_pSysComVoice;
		m_pSysComVoice = NULL;
	}
	if(m_pSysComAudio != NULL)
	{
		delete m_pSysComAudio;
		m_pSysComAudio = NULL;
	}
	if(m_pSysComFile != NULL)
	{
		delete m_pSysComFile;
		m_pSysComFile = NULL;
	}

	if(m_pSysSoftUpdate != NULL)
	{
		delete m_pSysSoftUpdate;
		m_pSysSoftUpdate = NULL;
	}

	if(m_pSysAutoReply != NULL)
	{
		if(::IsWindow(m_pSysAutoReply->GetSafeHwnd()))
		{
			m_pSysAutoReply->DestroyWindow();
		}
		delete m_pSysAutoReply;
		m_pSysAutoReply = NULL;
	}
	//if(m_pSysFastWord != NULL)
	//{
	//	delete m_pSysFastWord;
	//	m_pSysFastWord = NULL;
	//}
	if(::IsWindow(GetSafeHwnd()))
	{
		DestroyWindow();
	}
}

void CSystemSettingCommonDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_FRAME_COMMON, m_ctlFrameCommon);
	DDX_Control(pDX, IDC_STATIC_BASIC_SETTING, m_colBasicSetting);
}


BEGIN_MESSAGE_MAP(CSystemSettingCommonDlg, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_DESTROY()
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
END_MESSAGE_MAP()


// CSystemSettingCommonDlg 消息处理程序

BOOL CSystemSettingCommonDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
	CreateCommonDlg();
	m_colBasicSetting.SetCurSel(0);
	return TRUE;
}

void  CSystemSettingCommonDlg::CreateCommonDlg()
{
	CRect rect, rtScroll;
	m_ctlFrameCommon.GetClientRect(&rect);
	m_ctlFrameCommon.MapWindowPoints(this, rect);
	//////////8个页面子框架布局//////////////////////
	////基本信息
	m_pSysComBasic= new CSysCommonBasic;
	m_pSysComBasic->Create(GetSafeHwnd(), _T("DUISysSettingBasicCfg"), UI_WNDSTYLE_CHILD, 0, rect.left, rect.top+5, 382, 270); 
	m_pSysComBasic->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysComBasic);

	////消息设置
	m_pSysComMsg = new CSysCommonMsg;
	m_pSysComMsg->Create(GetSafeHwnd(), _T("DUISysSettingMsgCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysComMsg->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysComMsg);
	
	////声音
	m_pSysComVoice = new CSysCommonVoice;
	m_pSysComVoice->Create(GetSafeHwnd(), _T("DUISysSettingVoiceCfg"), UI_WNDSTYLE_CHILD, 0, rect.left, rect.top+5, 382, 270); 
	m_pSysComVoice->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysComVoice);
	
	////语音视频
	m_pSysComAudio = new CSysCommonAudio;
	m_pSysComAudio->Create(GetSafeHwnd(), _T("DUISysSettingAudioCfg"), UI_WNDSTYLE_CHILD, 0, rect.left, rect.top+5, 382, 270);  
	m_pSysComAudio->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysComAudio);

    ////文件管理
	m_pSysComFile = new CSysCommonFile;
	m_pSysComFile->Create(GetSafeHwnd(), _T("DUISysSettingFileCfg"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysComFile->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysComFile);

	////自动回复
	m_pSysAutoReply = new CSysCommonAutoReply;
	m_pSysAutoReply->Create(CSysCommonAutoReply::IDD, this);
	m_pSysAutoReply->MoveWindow(rect.left, rect.top, 400, 270);
	m_pSysAutoReply->ShowWindow(SW_HIDE);
	m_vecpChildWnd.push_back(m_pSysAutoReply);

	////快捷短语
	//m_pSysFastWord = new CSysCommonFastWord;
	//m_pSysFastWord->Create(GetSafeHwnd(), _T("DUISysSettingFastWord"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	//m_pSysFastWord->ShowWindow(FALSE); 
	//m_vecpChildWnd.push_back((CWnd*)m_pSysFastWord);

	////软件更新
	m_pSysSoftUpdate = new CSysCommonSoftwareUpdate;
	m_pSysSoftUpdate->Create(GetSafeHwnd(), _T("DUISysSettingSoftwareUpdate"), UI_WNDSTYLE_CHILD, 0,  rect.left, rect.top+5, 382, 270); 
	m_pSysSoftUpdate->ShowWindow(FALSE); 
	m_vecpChildWnd.push_back((CWnd*)m_pSysSoftUpdate);
}


// 菜单栏选着改变事件
LRESULT CSystemSettingCommonDlg::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	if(pCtrl == &m_colBasicSetting)
	{
		int nCurSel = HIWORD(lParam);
		
		if(nCurSel >=0 && nCurSel <= 6)
		{
			for(int i = 0; i < m_vecpChildWnd.size(); ++i)
			{
				if(i == 0)
				{
					m_pSysComBasic->ShowWindow(i == nCurSel);
					continue;
				}
				if(i == 1)
				{
					m_pSysComMsg->ShowWindow(i == nCurSel);
					continue;
				}
				if(i == 2)
				{
					m_pSysComVoice->ShowWindow(i == nCurSel);
					continue;
				}
				if(i == 3)
				{
					m_pSysComAudio->ShowWindow(i == nCurSel);
					continue;
				}
				if(i == 4)
				{
					m_pSysComFile->ShowWindow(i == nCurSel);
					continue;
				}
				//if(i == 6)
				//{
				//	m_pSysFastWord->ShowWindow(i == nCurSel);
				//	continue;
				//}
				if(i == 6)
				{
					m_pSysSoftUpdate->ShowWindow(i == nCurSel);
					continue;
				}
				CWnd *pChild = m_vecpChildWnd[i];
				pChild->ShowWindow(i == nCurSel ? SW_SHOW : SW_HIDE);
			}
		}
	}

	return 0;
}

HBRUSH CSystemSettingCommonDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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


void CSystemSettingCommonDlg::OnDestroy()
{
	m_pSysComBasic->Close(IDOK);
	m_pSysComMsg->Close(IDOK);
	m_pSysComVoice->Close(IDOK);
	m_pSysComAudio->Close(IDOK);
	m_pSysComFile->Close(IDOK);
	//m_pSysFastWord->Close(IDOK);
	m_pSysAutoReply->SendMessage(WM_NCDESTROY);
	CDialog::OnDestroy();
}

// 保存配置
void CSystemSettingCommonDlg::Save()
{
	m_pSysComBasic->SaveLoginSetting();///登陆设置
	m_pSysComMsg->SaveConfig();///快捷键设置
	m_pSysAutoReply->Save();
	//m_pSysFastWord->Save();
	m_pSysComFile->Save();
	m_pSysComVoice->Save();
	m_pSysSoftUpdate->Save();
	m_pSysComAudio->Save();
}

// 控制Tab页显示
void CSystemSettingCommonDlg::ShowTab(int nTabIndex)
{
	m_colBasicSetting.SetCurSel(nTabIndex);
}