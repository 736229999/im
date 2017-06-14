// SysCommonVoice.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SysCommonVoice.h"
#include "afxdialogex.h"
	enum LIST_LINE {VOICE_CHECK = 1, WAVE_PLAY = 2, FILE_PATH = 3 };

// CSysCommonVoice �Ի���

using namespace DuiLib;

void CSysCommonVoice::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pBtnRecvOnce =  static_cast<CButtonUI*>(m_PaintManager.FindControl("RecvOnce"));;  ////����
	m_pBtnOnline =  static_cast<CButtonUI*>(m_PaintManager.FindControl("Online"));;  ////����
	m_pBtnSysMsg =  static_cast<CButtonUI*>(m_PaintManager.FindControl("SysMsg"));;  ////����
	m_pBtnShake =  static_cast<CButtonUI*>(m_PaintManager.FindControl("Shake"));;  ////����

	m_pCheckRecvBtn =  static_cast<COptionUI*>(m_PaintManager.FindControl("CheckRecvBtn"));;  ////Ĭ��״̬
	m_pCheckOnlineBtn =  static_cast<COptionUI*>(m_PaintManager.FindControl("CheckOnlineBtn"));;  ////Ĭ��״̬
	m_pCheckSysBtn =  static_cast<COptionUI*>(m_PaintManager.FindControl("CheckSysBtn"));;  ////Ĭ��״̬
	m_pCheckShakeBtn =  static_cast<COptionUI*>(m_PaintManager.FindControl("CheckShakeBtn"));;  ////Ĭ��״̬

	m_pEditRecvPath =  static_cast<CEditUI*>(m_PaintManager.FindControl("RecvPath"));;  ////·�����
	m_pEditRecvPath->SetReadOnly(TRUE);
	m_pEditOnlinePath =  static_cast<CEditUI*>(m_PaintManager.FindControl("OnlinePath"));;  ////·�����
	m_pEditOnlinePath->SetReadOnly(TRUE);
	m_pEditSysPath =  static_cast<CEditUI*>(m_PaintManager.FindControl("SysPath"));;  ////·�����
	m_pEditSysPath->SetReadOnly(TRUE);
	m_pEditShakePath =  static_cast<CEditUI*>(m_PaintManager.FindControl("ShakePath"));;  ////·�����
	m_pEditShakePath->SetReadOnly(TRUE);

	m_pBtnVoiceChose =  static_cast<CButtonUI*>(m_PaintManager.FindControl("VoiceChose"));;  ////ѡ���ļ�
	m_pBtnOnlineChose =  static_cast<CButtonUI*>(m_PaintManager.FindControl("OnlineChose"));;  ////ѡ���ļ�
	m_pBtnSysChose =  static_cast<CButtonUI*>(m_PaintManager.FindControl("SysChose"));;  ////ѡ���ļ�
	m_pBtnShakeChose  =  static_cast<CButtonUI*>(m_PaintManager.FindControl("ShakeChose"));;  ////ѡ���ļ�


    ///////ϵͳ����
    m_pEditRecvPath->SetText(g_LocalData.GetVoicePath(EVOICE_RECEIVE_MSG).c_str());
	m_pEditOnlinePath->SetText(g_LocalData.GetVoicePath(EVOICE_FRIEND_ONLINE).c_str());
	m_pEditSysPath->SetText(g_LocalData.GetVoicePath(EVOICE_SYS_MSG).c_str());
	m_pEditShakePath->SetText(g_LocalData.GetVoicePath(EVOICE_SHOCK_SCREEN).c_str());
	
	m_pCheckRecvBtn->Selected(g_LocalData.VoiceIsEnable(EVOICE_RECEIVE_MSG));
	m_pCheckOnlineBtn->Selected(g_LocalData.VoiceIsEnable(EVOICE_FRIEND_ONLINE));
	m_pCheckSysBtn->Selected(g_LocalData.VoiceIsEnable(EVOICE_SYS_MSG));
	m_pCheckShakeBtn->Selected(g_LocalData.VoiceIsEnable(EVOICE_SHOCK_SCREEN));
}

// ��Ϣ
void CSysCommonVoice::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnRecvOnce) ////�յ���Ϣ
		{
			OnBnClickedBtnReceiveMsgVoice();
		}
		else if(msg.pSender == m_pBtnOnline) ////����
		{
			OnBnClickedBtnFriendOnlineVoice();
		}
		else if(msg.pSender == m_pBtnSysMsg) ////ϵͳ
		{
			OnBnClickedBtnSysMsgVoice();
		}
		else if(msg.pSender == m_pBtnShake) ////����
		{
			OnBnClickedBtnShockScreenVoice();
		}
		else if(msg.pSender == m_pBtnVoiceChose) ////��ʱѡ��
		{
			OnBnClickedBtnReceiveMsgBrowse();
		}
		else if(msg.pSender == m_pBtnOnlineChose) ////����ѡ��
		{
			OnBnClickedBtnFriendOnlineBrowse();
		}
		else if(msg.pSender == m_pBtnSysChose) ////ϵͳѡ��
		{
			OnBnClickedBtnSysMsgBrowse();
		}
		else if(msg.pSender == m_pBtnShakeChose) ////����ѡ��
		{
			OnBnClickedBtnShockScreenBrowse();
		}
	}
}

void CSysCommonVoice::OnBnClickedBtnReceiveMsgVoice()
{
	CString strWavFile;
	strWavFile = m_pEditRecvPath->GetText();
	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
}

void CSysCommonVoice::OnBnClickedBtnFriendOnlineVoice()
{
	CString strWavFile;
	strWavFile = m_pEditOnlinePath->GetText();
	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
}


void CSysCommonVoice::OnBnClickedBtnSysMsgVoice()
{
	CString strWavFile;
	strWavFile = m_pEditSysPath->GetText();
	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
}


void CSysCommonVoice::OnBnClickedBtnShockScreenVoice()
{
	CString strWavFile;
	strWavFile = m_pEditShakePath->GetText();
	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
}


void CSysCommonVoice::OnBnClickedBtnReceiveMsgBrowse()
{
	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||",NULL);

	if ( dlg.DoModal() == IDOK )
	{
		CString strFile = dlg.GetPathName();
		m_pEditRecvPath->SetText(strFile);
	}
}


void CSysCommonVoice::OnBnClickedBtnFriendOnlineBrowse()
{
	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", NULL);

	if ( dlg.DoModal() == IDOK )
	{
		CString strFile = dlg.GetPathName();
		m_pEditOnlinePath->SetText(strFile);
	}
}


void CSysCommonVoice::OnBnClickedBtnSysMsgBrowse()
{
	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", NULL);

	if ( dlg.DoModal() == IDOK )
	{
		CString strFile = dlg.GetPathName();
		m_pEditSysPath->SetText(strFile);
	}
}


void CSysCommonVoice::OnBnClickedBtnShockScreenBrowse()
{
	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", NULL);//this

	if ( dlg.DoModal() == IDOK )
	{
		CString strFile = dlg.GetPathName();
		m_pEditShakePath->SetText(strFile);
	}
}

// ��������
void CSysCommonVoice::Save()
{
	g_LocalData.EnableVoice(EVOICE_RECEIVE_MSG, m_pCheckRecvBtn->IsSelected());
	g_LocalData.EnableVoice(EVOICE_FRIEND_ONLINE, m_pCheckOnlineBtn->IsSelected());
	g_LocalData.EnableVoice(EVOICE_SYS_MSG, m_pCheckSysBtn->IsSelected());
	g_LocalData.EnableVoice(EVOICE_SHOCK_SCREEN,m_pCheckShakeBtn->IsSelected());

	CString strPath;
	strPath = m_pEditRecvPath->GetText();
	g_LocalData.SetVoicePath(EVOICE_RECEIVE_MSG, strPath);
	strPath = m_pEditOnlinePath->GetText();
	g_LocalData.SetVoicePath(EVOICE_FRIEND_ONLINE, strPath);
	strPath = m_pEditSysPath->GetText();
	g_LocalData.SetVoicePath(EVOICE_SYS_MSG, strPath);
	strPath = m_pEditShakePath->GetText();
	g_LocalData.SetVoicePath(EVOICE_SHOCK_SCREEN, strPath);
}


////////IMPLEMENT_DYNAMIC(CSysCommonVoice, CDialog)
////////
////////CSysCommonVoice::CSysCommonVoice(CWnd* pParent /*=NULL*/)
////////	: CDialog(CSysCommonVoice::IDD, pParent)
////////{
////////
////////}
////////
////////CSysCommonVoice::~CSysCommonVoice()
////////{
////////	//m_cListVoice.release();
////////}
////////
////////void CSysCommonVoice::DoDataExchange(CDataExchange* pDX)
////////{
////////	CDialog::DoDataExchange(pDX);
////////	DDX_Control(pDX, IDC_BTN_RECEIVE_MSG_VOICE, m_btnVoice_ReceiveMsg);
////////	DDX_Control(pDX, IDC_BTN_SYS_MSG_VOICE, m_btnVoice_SysMsg);
////////	DDX_Control(pDX, IDC_BTN_FRIEND_ONLINE_VOICE, m_btnVoice_FriendOnline);
////////	DDX_Control(pDX, IDC_BTN_SHOCK_SCREEN_VOICE, m_btnVoice_ShockScreen);
////////
////////	DDX_Control(pDX, IDC_BTN_RECEIVE_MSG_BROWSE, m_btnBrowse_ReceiveMsg);
////////	DDX_Control(pDX, IDC_BTN_SYS_MSG_BROWSE, m_btnBrowse_SysMsg);
////////	DDX_Control(pDX, IDC_BTN_FRIEND_ONLINE_BROWSE, m_btnBrowse_FriendOnline);
////////	DDX_Control(pDX, IDC_BTN_SHOCK_SCREEN_BROWSE, m_btnBrowse_ShockScreen);
////////
////////	DDX_Control(pDX, IDC_EDIT_RECEIVE_MSG_VOICE, m_pEditRecvPath->);
////////	DDX_Control(pDX, IDC_EDIT_SYS_MSG_VOICE, m_pEditSysPath->);
////////	DDX_Control(pDX, IDC_EDIT_SHOCK_SCREEN_VOICE, m_pEditShakePath->);
////////	DDX_Control(pDX, IDC_EDIT_FRIEND_ONLINE_VOICE, m_pEditOnlinePath->);
////////
////////	DDX_Control(pDX, IDC_CHECK_RECEIVE_MSG_VOICE, m_check_ReceiveMsg);
////////	DDX_Control(pDX, IDC_CHECK_FRIEND_ONLINE_VOICE, m_check_FriendOnline);
////////	DDX_Control(pDX, IDC_CHECK_SYS_MSG_VOICE, m_check_SysMsg);
////////	DDX_Control(pDX, IDC_CHECK_SHOCK_SCREEN_VOICE, m_check_ShockScreen);
////////}
////////
////////BEGIN_MESSAGE_MAP(CSysCommonVoice, CDialog)
////////	ON_WM_CTLCOLOR()
////////	ON_WM_DESTROY()
////////	ON_WM_ERASEBKGND()
////////	ON_WM_PAINT()
////////	ON_WM_SIZE()
////////	ON_BN_CLICKED(IDC_BTN_RECEIVE_MSG_VOICE, &CSysCommonVoice::OnBnClickedBtnReceiveMsgVoice)
////////	ON_BN_CLICKED(IDC_BTN_FRIEND_ONLINE_VOICE, &CSysCommonVoice::OnBnClickedBtnFriendOnlineVoice)
////////	ON_BN_CLICKED(IDC_BTN_SYS_MSG_VOICE, &CSysCommonVoice::OnBnClickedBtnSysMsgVoice)
////////	ON_BN_CLICKED(IDC_BTN_SHOCK_SCREEN_VOICE, &CSysCommonVoice::OnBnClickedBtnShockScreenVoice)
////////	ON_BN_CLICKED(IDC_BTN_RECEIVE_MSG_BROWSE, &CSysCommonVoice::OnBnClickedBtnReceiveMsgBrowse)
////////	ON_BN_CLICKED(IDC_BTN_FRIEND_ONLINE_BROWSE, &CSysCommonVoice::OnBnClickedBtnFriendOnlineBrowse)
////////	ON_BN_CLICKED(IDC_BTN_SYS_MSG_BROWSE, &CSysCommonVoice::OnBnClickedBtnSysMsgBrowse)
////////	ON_BN_CLICKED(IDC_BTN_SHOCK_SCREEN_BROWSE, &CSysCommonVoice::OnBnClickedBtnShockScreenBrowse)
////////END_MESSAGE_MAP()
////////
////////
////////// CSysCommonVoice ��Ϣ�������
////////
////////
////////HBRUSH CSysCommonVoice::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////////{
////////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
////////
////////	if ( nCtlColor==CTLCOLOR_STATIC )
////////	{
////////		UINT nId = pWnd->GetDlgCtrlID();
////////
////////		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
////////		{
////////
////////		}
////////		else
////////		{
////////			pDC->SetBkMode(TRANSPARENT);
////////			hbr = g_hWhiteBrush;
////////		}
////////
////////		if ( nId==IDC_STATIC_GROUP )
////////		{
////////			pDC->SetTextColor(RGB(255,0,0));//��255,0��0�� //(74,123,211)
////////		}
////////		else
////////		{
////////			pDC->SetTextColor(0x2e2e2e);
////////		}
////////	}
////////
////////	if ( nCtlColor==CTLCOLOR_DLG  )
////////	{
////////		hbr = g_hWhiteBrush;
////////	}
////////
////////	return hbr;
////////}
////////
////////BOOL CSysCommonVoice::OnInitDialog()
////////{
////////	CDialog::OnInitDialog();
////////
////////	CWnd *pStatic = GetDlgItem(IDC_STATIC_VOICE);
////////	pStatic->SetFont(&g_font12B);
////////
////////	CalcTablePosData();
////////
////////	m_btnVoice_ReceiveMsg.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);
////////	m_btnVoice_SysMsg.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);
////////	m_btnVoice_FriendOnline.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);
////////	m_btnVoice_ShockScreen.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);
////////
////////	m_btnBrowse_ReceiveMsg.SetPng(IDB_PNG_FILES_HOVER, IDB_PNG_FILES_NORMAL, IDB_PNG_FILES_PRESSED, IDB_PNG_FILES_DISABLE);
////////	m_btnBrowse_SysMsg.SetPng(IDB_PNG_FILES_HOVER, IDB_PNG_FILES_NORMAL, IDB_PNG_FILES_PRESSED, IDB_PNG_FILES_DISABLE);
////////	m_btnBrowse_FriendOnline.SetPng(IDB_PNG_FILES_HOVER, IDB_PNG_FILES_NORMAL, IDB_PNG_FILES_PRESSED, IDB_PNG_FILES_DISABLE);
////////	m_btnBrowse_ShockScreen.SetPng(IDB_PNG_FILES_HOVER, IDB_PNG_FILES_NORMAL, IDB_PNG_FILES_PRESSED, IDB_PNG_FILES_DISABLE);
////////
////////	m_pEditRecvPath->.SetBackColor(RGB(255,255,255));
////////	m_pEditRecvPath->.SetBorderColor(RGB(255,255,255));
////////	m_pEditSysPath->.SetBackColor(RGB(255,255,255));
////////	m_pEditSysPath->.SetBorderColor(RGB(255,255,255));
////////
////////	m_pEditOnlinePath->.SetBackColor(RGB(236,236,236));
////////	m_pEditOnlinePath->.SetBorderColor(RGB(236,236,236));
////////	m_pEditShakePath->.SetBackColor(RGB(236,236,236));
////////	m_pEditShakePath->.SetBorderColor(RGB(236,236,236));
////////
////////	m_pEditRecvPath->.SetWindowText(g_LocalData.GetVoicePath(EVOICE_RECEIVE_MSG).c_str());
////////	m_pEditOnlinePath->.SetWindowText(g_LocalData.GetVoicePath(EVOICE_FRIEND_ONLINE).c_str());
////////	m_pEditSysPath->.SetWindowText(g_LocalData.GetVoicePath(EVOICE_SYS_MSG).c_str());
////////	m_pEditShakePath->.SetWindowText(g_LocalData.GetVoicePath(EVOICE_SHOCK_SCREEN).c_str());
////////
////////	m_check_ReceiveMsg.SetCheck(g_LocalData.VoiceIsEnable(EVOICE_RECEIVE_MSG));
////////	m_check_FriendOnline.SetCheck(g_LocalData.VoiceIsEnable(EVOICE_FRIEND_ONLINE));
////////	m_check_SysMsg.SetCheck(g_LocalData.VoiceIsEnable(EVOICE_SYS_MSG));
////////	m_check_ShockScreen.SetCheck(g_LocalData.VoiceIsEnable(EVOICE_SHOCK_SCREEN));
////////
////////	return TRUE;
////////}
////////
////////void CSysCommonVoice::OnDestroy()
////////{
////////	CDialog::OnDestroy();
////////
////////	// TODO: �ڴ˴������Ϣ����������
////////	//m_cListVoice.release();
////////}
////////
////////// ������ˢΪ��ɫ
////////BOOL CSysCommonVoice::OnEraseBkgnd(CDC* pDC)
////////{
////////	pDC->SetBkColor(RGB(255, 255, 255));
////////
////////	// �����
////////	for(int j = 0; j < 4; ++j)
////////	{
////////		pDC->FillSolidRect(m_arrRcCell[0][j], RGB(217,228,234));
////////		pDC->FillSolidRect(m_arrRcCell[1][j], RGB(255,255,255));
////////		pDC->FillSolidRect(m_arrRcCell[2][j], RGB(236,236,236));
////////		pDC->FillSolidRect(m_arrRcCell[3][j], RGB(255,255,255));
////////		pDC->FillSolidRect(m_arrRcCell[4][j], RGB(236,236,236));
////////	}
////////
////////	// д�ı�
////////	CFont *pOldFont = pDC->SelectObject(&g_font12);	// ѡ������
////////
////////	pDC->SetBkColor(RGB(217,228,234));		// �������屳��ɫ
////////	pDC->SetTextColor(RGB(0, 0, 0));		// ����������ɫ
////////	pDC->DrawText(" ��������", &(m_arrRcCell[0][0]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->DrawText(" ����", &(m_arrRcCell[0][1]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->DrawText(" ״̬", &(m_arrRcCell[0][2]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->DrawText(" �����ļ�", &(m_arrRcCell[0][3]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////
////////	pDC->SetBkColor(RGB(255,255,255));		// �������屳��ɫ
////////	pDC->DrawText(" �յ���ʱ��Ϣ", &(m_arrRcCell[1][0]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->DrawText(" ϵͳ��Ϣ", &(m_arrRcCell[3][0]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////
////////	pDC->SetBkColor(RGB(236,236,236));		// �������屳��ɫ
////////	pDC->DrawText(" ��ϵ������", &(m_arrRcCell[2][0]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->DrawText(" ������", &(m_arrRcCell[4][0]), DT_SINGLELINE | DT_LEFT | DT_VCENTER);
////////	pDC->SelectObject(pOldFont);
////////
////////	int nTop = m_arrRcCell[0][0].top + m_arrRcCell[0][0].Height()/4;
////////	int nBottom = m_arrRcCell[0][0].top + m_arrRcCell[0][0].Height()*3/4;
////////	CPen pen(PS_SOLID, 1, RGB(101, 101, 101));
////////	CPen *pOldPen = pDC->SelectObject(&pen);
////////	pDC->MoveTo(m_ptTop1.x, nTop);
////////	pDC->LineTo(m_ptTop1.x, nBottom);
////////	pDC->MoveTo(m_ptTop2.x, nTop);
////////	pDC->LineTo(m_ptTop2.x, nBottom);
////////	pDC->MoveTo(m_ptTop3.x, nTop);
////////	pDC->LineTo(m_ptTop3.x, nBottom);
////////	pDC->SelectObject(pOldPen);
////////
////////	return FALSE;
////////}
////////
////////// ���Ʊ��
////////void CSysCommonVoice::OnPaint()
////////{
////////	CDialog::OnPaint();
////////}
////////
////////// ������ڷ�λ������
////////void CSysCommonVoice::CalcTablePosData()
////////{
////////	// �������λ��Ϊ��������static�·�20���س�
////////	CRect rcClient;
////////	GetClientRect(&rcClient);
////////	CWnd *pStatic = GetDlgItem(IDC_STATIC_VOICE);
////////	if(pStatic == NULL || pStatic->m_hWnd == NULL)
////////		return;
////////	CRect rcStatic;
////////	pStatic->GetWindowRect(&rcStatic);
////////	ScreenToClient(&rcStatic);
////////	m_rcTable.left = rcStatic.left;
////////	m_rcTable.right = rcClient.right - 0;
////////	m_rcTable.bottom = rcClient.bottom - 70;
////////	m_rcTable.top = rcStatic.bottom + 10;
////////
////////	// �������ָ����ڱ���Ϸ�������
////////	int nWidth = m_rcTable.Width();
////////	int nHeight = m_rcTable.Height();
////////	m_ptTop1.y = m_ptTop2.y = m_ptTop3.y = m_rcTable.top;
////////	m_ptTop1.x = m_rcTable.left + nWidth * 0.25;
////////	m_ptTop2.x = m_rcTable.left + nWidth * 0.333;
////////	m_ptTop3.x = m_rcTable.left + nWidth * 0.416;
////////
////////	// �������ָ����ڱ���·�������
////////	m_ptBottom1.y = m_ptBottom2.y = m_ptBottom3.y = m_rcTable.bottom;
////////	m_ptBottom1.x = m_ptTop1.x;
////////	m_ptBottom2.x = m_ptTop2.x;
////////	m_ptBottom3.x = m_ptTop3.x;
////////
////////	// ������ָ����ڱ����ߵ�����
////////	m_ptLeft1.x = m_ptLeft2.x = m_ptLeft3.x = m_ptLeft4.x = m_rcTable.left;
////////	m_ptLeft1.y = m_rcTable.top + nHeight/5;
////////	m_ptLeft2.y = m_rcTable.top + nHeight*2/5;
////////	m_ptLeft3.y = m_rcTable.top + nHeight*3/5;
////////	m_ptLeft4.y = m_rcTable.top + nHeight*4/5;
////////
////////	// ������ָ����ڱ���ұߵ�����
////////	m_ptRight1.x = m_ptRight2.x = m_ptRight3.x = m_ptRight4.x = m_rcTable.right;
////////	m_ptRight1.y = m_ptLeft1.y;
////////	m_ptRight2.y = m_ptLeft2.y;
////////	m_ptRight3.y = m_ptLeft3.y;
////////	m_ptRight4.y = m_ptLeft4.y;
////////
////////	// ��¼���Cell��λ��
////////	m_arrRcCell[0][0].top = m_arrRcCell[0][1].top = m_arrRcCell[0][2].top = m_arrRcCell[0][3].top = m_rcTable.top;
////////	m_arrRcCell[0][0].bottom = m_arrRcCell[0][1].bottom = m_arrRcCell[0][2].bottom = m_arrRcCell[0][3].bottom = m_ptLeft1.y;
////////
////////	m_arrRcCell[1][0].top = m_arrRcCell[1][1].top = m_arrRcCell[1][2].top = m_arrRcCell[1][3].top = m_ptLeft1.y;
////////	m_arrRcCell[1][0].bottom = m_arrRcCell[1][1].bottom = m_arrRcCell[1][2].bottom = m_arrRcCell[1][3].bottom = m_ptLeft2.y;
////////
////////	m_arrRcCell[2][0].top = m_arrRcCell[2][1].top = m_arrRcCell[2][2].top = m_arrRcCell[2][3].top = m_ptLeft2.y;
////////	m_arrRcCell[2][0].bottom = m_arrRcCell[2][1].bottom = m_arrRcCell[2][2].bottom = m_arrRcCell[2][3].bottom = m_ptLeft3.y;
////////
////////	m_arrRcCell[3][0].top = m_arrRcCell[3][1].top = m_arrRcCell[3][2].top = m_arrRcCell[3][3].top = m_ptLeft3.y;
////////	m_arrRcCell[3][0].bottom = m_arrRcCell[3][1].bottom = m_arrRcCell[3][2].bottom = m_arrRcCell[3][3].bottom = m_ptLeft4.y;
////////
////////	m_arrRcCell[4][0].top = m_arrRcCell[4][1].top = m_arrRcCell[4][2].top = m_arrRcCell[4][3].top = m_ptLeft4.y;
////////	m_arrRcCell[4][0].bottom = m_arrRcCell[4][1].bottom = m_arrRcCell[4][2].bottom = m_arrRcCell[4][3].bottom = m_rcTable.bottom;
////////
////////	m_arrRcCell[0][0].left = m_arrRcCell[1][0].left = m_arrRcCell[2][0].left = m_arrRcCell[3][0].left = m_arrRcCell[4][0].left = m_rcTable.left;
////////	m_arrRcCell[0][0].right = m_arrRcCell[1][0].right = m_arrRcCell[2][0].right = m_arrRcCell[3][0].right = m_arrRcCell[4][0].right = m_ptTop1.x;
////////
////////	m_arrRcCell[0][1].left = m_arrRcCell[1][1].left = m_arrRcCell[2][1].left = m_arrRcCell[3][1].left = m_arrRcCell[4][1].left = m_ptTop1.x;
////////	m_arrRcCell[0][1].right = m_arrRcCell[1][1].right = m_arrRcCell[2][1].right = m_arrRcCell[3][1].right = m_arrRcCell[4][1].right = m_ptTop2.x;
////////
////////	m_arrRcCell[0][2].left = m_arrRcCell[1][2].left = m_arrRcCell[2][2].left = m_arrRcCell[3][2].left = m_arrRcCell[4][2].left = m_ptTop2.x;
////////	m_arrRcCell[0][2].right = m_arrRcCell[1][2].right = m_arrRcCell[2][2].right = m_arrRcCell[3][2].right = m_arrRcCell[4][2].right = m_ptTop3.x;
////////
////////	m_arrRcCell[0][3].left = m_arrRcCell[1][3].left = m_arrRcCell[2][3].left = m_arrRcCell[3][3].left = m_arrRcCell[4][3].left = m_ptTop3.x;
////////	m_arrRcCell[0][3].right = m_arrRcCell[1][3].right = m_arrRcCell[2][3].right = m_arrRcCell[3][3].right = m_arrRcCell[4][3].right = m_rcTable.right;	
////////}
////////
////////void CSysCommonVoice::OnSize(UINT nType, int cx, int cy)
////////{
////////	CDialog::OnSize(nType, cx, cy);
////////	if(m_hWnd != NULL)
////////	{
////////		CalcTablePosData();
////////
////////		if(::IsWindow(m_btnVoice_ReceiveMsg.GetSafeHwnd()))
////////		{
////////			// �ƶ�������ť
////////			m_btnVoice_ReceiveMsg.MoveWindow(m_arrRcCell[1][1].left + (m_arrRcCell[1][1].Width()-22)/2, 
////////											m_arrRcCell[1][1].top + (m_arrRcCell[1][1].Height()-22)/2,
////////											22, 22);
////////			m_btnVoice_FriendOnline.MoveWindow(m_arrRcCell[2][1].left + (m_arrRcCell[2][1].Width()-22)/2, 
////////											m_arrRcCell[2][1].top + (m_arrRcCell[2][1].Height()-22)/2,
////////											22, 22);
////////			m_btnVoice_SysMsg.MoveWindow(m_arrRcCell[3][1].left + (m_arrRcCell[3][1].Width()-22)/2, 
////////											m_arrRcCell[3][1].top + (m_arrRcCell[3][1].Height()-22)/2,
////////											22, 22);
////////			m_btnVoice_ShockScreen.MoveWindow(m_arrRcCell[4][1].left + (m_arrRcCell[4][1].Width()-22)/2, 
////////											m_arrRcCell[4][1].top + (m_arrRcCell[4][1].Height()-22)/2,
////////											22, 22);
////////
////////			// �ƶ�״̬��ť
////////			CRect rcCheck;
////////			GetDlgItem(IDC_CHECK_RECEIVE_MSG_VOICE)->GetWindowRect(rcCheck);
////////			GetDlgItem(IDC_CHECK_RECEIVE_MSG_VOICE)->MoveWindow(m_arrRcCell[1][2].left + (m_arrRcCell[1][2].Width()-rcCheck.Width())/2,
////////														m_arrRcCell[1][2].top + (m_arrRcCell[1][2].Height()-rcCheck.Height())/2,
////////														rcCheck.Width(), rcCheck.Height());
////////			GetDlgItem(IDC_CHECK_FRIEND_ONLINE_VOICE)->MoveWindow(m_arrRcCell[2][2].left + (m_arrRcCell[2][2].Width()-rcCheck.Width())/2,
////////														m_arrRcCell[2][2].top + (m_arrRcCell[2][2].Height()-rcCheck.Height())/2,
////////														rcCheck.Width(), rcCheck.Height());
////////			GetDlgItem(IDC_CHECK_SYS_MSG_VOICE)->MoveWindow(m_arrRcCell[3][2].left + (m_arrRcCell[3][2].Width()-rcCheck.Width())/2,
////////														m_arrRcCell[3][2].top + (m_arrRcCell[3][2].Height()-rcCheck.Height())/2,
////////														rcCheck.Width(), rcCheck.Height());
////////			GetDlgItem(IDC_CHECK_SHOCK_SCREEN_VOICE)->MoveWindow(m_arrRcCell[4][2].left + (m_arrRcCell[4][2].Width()-rcCheck.Width())/2,
////////														m_arrRcCell[4][2].top + (m_arrRcCell[4][2].Height()-rcCheck.Height())/2,
////////														rcCheck.Width(), rcCheck.Height());
////////
////////			//// �ƶ�����ļ���ť
////////			m_btnBrowse_ReceiveMsg.MoveWindow(m_arrRcCell[1][3].right - 28, 
////////												m_arrRcCell[1][3].top + (m_arrRcCell[1][3].Height()-25)/2,
////////												25, 25);
////////			m_btnBrowse_FriendOnline.MoveWindow(m_arrRcCell[2][3].right - 28, 
////////												m_arrRcCell[2][3].top + (m_arrRcCell[2][3].Height()-25)/2,
////////												25, 25);
////////			m_btnBrowse_SysMsg.MoveWindow(m_arrRcCell[3][3].right - 28, 
////////												m_arrRcCell[3][3].top + (m_arrRcCell[3][3].Height()-25)/2,
////////												25, 25);
////////			m_btnBrowse_ShockScreen.MoveWindow(m_arrRcCell[4][3].right - 28, 
////////												m_arrRcCell[4][3].top + (m_arrRcCell[4][3].Height()-25)/2,
////////												25, 25);
////////
////////			// �����ļ�·��
////////			CRect rcEdit;
////////			m_pEditRecvPath->.GetWindowRect(&rcEdit);
////////			m_pEditRecvPath->.MoveWindow(m_arrRcCell[1][3].left+3, m_arrRcCell[1][3].top+(m_arrRcCell[1][3].Height()-rcEdit.Height())/2, m_arrRcCell[1][3].Width()-3-28, rcEdit.Height());
////////			m_pEditOnlinePath->.MoveWindow(m_arrRcCell[2][3].left+3, m_arrRcCell[2][3].top+(m_arrRcCell[2][3].Height()-rcEdit.Height())/2, m_arrRcCell[2][3].Width()-3-28, rcEdit.Height());
////////			m_pEditSysPath->.MoveWindow(m_arrRcCell[3][3].left+3, m_arrRcCell[3][3].top+(m_arrRcCell[3][3].Height()-rcEdit.Height())/2, m_arrRcCell[3][3].Width()-3-28, rcEdit.Height());
////////			m_pEditShakePath->.MoveWindow(m_arrRcCell[4][3].left+3, m_arrRcCell[4][3].top+(m_arrRcCell[4][3].Height()-rcEdit.Height())/2, m_arrRcCell[4][3].Width()-3-28, rcEdit.Height());
////////		}		
////////	}
////////}
////////
////////void CSysCommonVoice::OnBnClickedBtnReceiveMsgVoice()
////////{
////////	CString strWavFile;
////////	m_pEditRecvPath->.GetWindowText(strWavFile);
////////	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
////////}
////////
////////void CSysCommonVoice::OnBnClickedBtnFriendOnlineVoice()
////////{
////////	CString strWavFile;
////////	m_pEditOnlinePath->.GetWindowText(strWavFile);
////////	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnSysMsgVoice()
////////{
////////	CString strWavFile;
////////	m_pEditSysPath->.GetWindowText(strWavFile);
////////	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnShockScreenVoice()
////////{
////////	CString strWavFile;
////////	m_pEditShakePath->.GetWindowText(strWavFile);
////////	PlaySound(strWavFile,NULL,SND_FILENAME | SND_ASYNC);
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnReceiveMsgBrowse()
////////{
////////	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", this);
////////
////////	if ( dlg.DoModal() == IDOK )
////////	{
////////		CString strFile = dlg.GetPathName();
////////		m_pEditRecvPath->.SetWindowText(strFile);
////////	}
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnFriendOnlineBrowse()
////////{
////////	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", this);
////////
////////	if ( dlg.DoModal() == IDOK )
////////	{
////////		CString strFile = dlg.GetPathName();
////////		m_pEditOnlinePath->.SetWindowText(strFile);
////////	}
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnSysMsgBrowse()
////////{
////////	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", this);
////////
////////	if ( dlg.DoModal() == IDOK )
////////	{
////////		CString strFile = dlg.GetPathName();
////////		m_pEditSysPath->.SetWindowText(strFile);
////////	}
////////}
////////
////////
////////void CSysCommonVoice::OnBnClickedBtnShockScreenBrowse()
////////{
////////	CFileDialog dlg(TRUE, "wav", "", OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT, "�����ļ�(*.wav)|*.wav||", this);
////////
////////	if ( dlg.DoModal() == IDOK )
////////	{
////////		CString strFile = dlg.GetPathName();
////////		m_pEditShakePath->.SetWindowText(strFile);
////////	}
////////}
////////
////////// ��������
////////void CSysCommonVoice::Save()
////////{
////////	g_LocalData.EnableVoice(EVOICE_RECEIVE_MSG, m_check_ReceiveMsg.GetCheck());
////////	g_LocalData.EnableVoice(EVOICE_FRIEND_ONLINE, m_check_FriendOnline.GetCheck());
////////	g_LocalData.EnableVoice(EVOICE_SYS_MSG, m_check_SysMsg.GetCheck());
////////	g_LocalData.EnableVoice(EVOICE_SHOCK_SCREEN, m_check_ShockScreen.GetCheck());
////////
////////	CString strPath;
////////	m_pEditRecvPath->.GetWindowText(strPath);
////////	g_LocalData.SetVoicePath(EVOICE_RECEIVE_MSG, strPath);
////////	m_pEditOnlinePath->.GetWindowText(strPath);
////////	g_LocalData.SetVoicePath(EVOICE_FRIEND_ONLINE, strPath);
////////	m_pEditSysPath->.GetWindowText(strPath);
////////	g_LocalData.SetVoicePath(EVOICE_SYS_MSG, strPath);
////////	m_pEditShakePath->.GetWindowText(strPath);
////////	g_LocalData.SetVoicePath(EVOICE_SHOCK_SCREEN, strPath);
////////}