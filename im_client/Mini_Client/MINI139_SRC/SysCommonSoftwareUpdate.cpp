// SysCommonSoftwareUpdate.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "SysCommonSoftwareUpdate.h"
#include "afxdialogex.h"
#include "..\Im\LocalData.h"
using namespace DuiLib;

void CSysCommonSoftwareUpdate::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pRadioAutoInstall = static_cast<COptionUI*>(m_PaintManager.FindControl("AutoInstall"));
	m_pRadioOnlyMention = static_cast<COptionUI*>(m_PaintManager.FindControl("OnlyMention"));

	WORD nVK = 0;
	g_LocalData.GetSoftUpdateConfig(nVK);
	if(nVK != 0) //�и���ʱ������
	{
		m_pRadioAutoInstall->Selected(FALSE);
		m_pRadioOnlyMention->Selected(TRUE);
	}
	else
	{
		m_pRadioAutoInstall->Selected(TRUE);
        m_pRadioOnlyMention->Selected(FALSE);
	}
}


// ��Ϣ
void CSysCommonSoftwareUpdate::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	/*if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
	if(m_pRadioAutoInstall->IsSelected())
	{
	Save();
	}
	else if(m_pRadioAutoInstall->IsSelected())
	{
	Save()
	}
	}*/
}


void CSysCommonSoftwareUpdate::Save()
{
	WORD wVKMention;
	if(!m_pRadioAutoInstall->IsSelected())////����
	{
		wVKMention = 1;
		g_LocalData.SaveSoftUpdateConfig(wVKMention);
	}
	else
	{
		wVKMention = 0;
		g_LocalData.SaveSoftUpdateConfig(wVKMention);
	}
}


////////// CSysCommonSoftwareUpdate �Ի���
////////
////////IMPLEMENT_DYNAMIC(CSysCommonSoftwareUpdate, CDialog)
////////
////////CSysCommonSoftwareUpdate::CSysCommonSoftwareUpdate(CWnd* pParent /*=NULL*/)
////////	: CDialog(CSysCommonSoftwareUpdate::IDD, pParent)
////////{
////////
////////}
////////
////////CSysCommonSoftwareUpdate::~CSysCommonSoftwareUpdate()
////////{
////////}
////////
////////void CSysCommonSoftwareUpdate::DoDataExchange(CDataExchange* pDX)
////////{
////////	CDialog::DoDataExchange(pDX);
////////	DDX_Control(pDX, IDC_RADIO_AUTO, m_radioAuto);
////////	DDX_Control(pDX, IDC_RADIO_MENTION, m_radioMention);
////////}
////////
////////
////////BEGIN_MESSAGE_MAP(CSysCommonSoftwareUpdate, CDialog)
////////	ON_WM_CTLCOLOR()
////////	ON_BN_CLICKED(IDC_RADIO_AUTO, &CSysCommonSoftwareUpdate::OnBnClickedRadioAuto)
////////END_MESSAGE_MAP()
////////
////////
////////// CSysCommonSoftwareUpdate ��Ϣ�������
////////
////////BOOL CSysCommonSoftwareUpdate::OnInitDialog()
////////{
////////	CDialog::OnInitDialog();
////////	
////////	// ���������ӿؼ������ÿؼ�����
////////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////////	while(hWndChild)  
////////	{  
////////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////////		if(GetDlgItem(IDC_STATIC_SOFT_UPDATE)->GetSafeHwnd() == hWndChild)
////////		{
////////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
////////		}
////////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////////	}
////////
////////	WORD nVK = 0;
////////	g_LocalData.GetSoftUpdateConfig(nVK);
////////	if(nVK != 0) //�и���ʱ��Ҫ��װ����������
////////	{
////////		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(FALSE);
////////		((CButton*)GetDlgItem(IDC_RADIO_MENTION))->SetCheck(TRUE);
////////	}
////////	else
////////	{
////////		((CButton*)GetDlgItem(IDC_RADIO_AUTO))->SetCheck(TRUE);
////////		((CButton*)GetDlgItem(IDC_RADIO_MENTION))->SetCheck(FALSE);
////////	}
////////	return TRUE;
////////}
////////
////////HBRUSH CSysCommonSoftwareUpdate::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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
////////
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
////////
////////void CSysCommonSoftwareUpdate::OnBnClickedRadioAuto()
////////{
////////	// TODO: �ڴ���ӿؼ�֪ͨ����������
////////}
////////
////////
////////void CSysCommonSoftwareUpdate::Save()
////////{
////////	WORD wVKMention;
////////	CButton * pShake = (CButton *) GetDlgItem(IDC_RADIO_MENTION);
////////	if(pShake->GetCheck())
////////	{
////////		wVKMention = 1;
////////		g_LocalData.SaveSoftUpdateConfig(wVKMention);
////////	}
////////	else
////////	{
////////		wVKMention = 0;
////////		g_LocalData.SaveSoftUpdateConfig(wVKMention);
////////	}
////////}
