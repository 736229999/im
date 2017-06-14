// HangupSessionNumSettingDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "HangupSessionNumSettingDlg.h"
#include "afxdialogex.h"
#include "resource.h"



// CHangupSessionNumSettingDlg �Ի���

IMPLEMENT_DYNAMIC(CHangupSessionNumSettingDlg, CXSkinDlg)

CHangupSessionNumSettingDlg::CHangupSessionNumSettingDlg(int nNum, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(IDD_DLG_HANGUP_NUM_SETTING, pParent)
{
	////////�����ɫ����///////////////
	m_colorBackground = RGB(255, 255, 255);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	if(nNum <= 0)
	{
		m_nNum = 20;
	}
	else
	{
		m_nNum = nNum;
	}

	SetDlgStyle(CXSkinDlg::CLOSE);
}

CHangupSessionNumSettingDlg::~CHangupSessionNumSettingDlg()
{
}

void CHangupSessionNumSettingDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CHangupSessionNumSettingDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_NCPAINT()
END_MESSAGE_MAP()


// CHangupSessionNumSettingDlg ��Ϣ�������
BOOL CHangupSessionNumSettingDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("�Զ������");

	m_editHangupNum.SubclassWindow(GetDlgItem(IDC_EDIT_SESSION_NUM)->m_hWnd);
	m_editHangupNum.LimitText(3);
	m_editHangupNum.SetBorderColor(RGB(88,88,88));

	// ���ÿؼ�����
	CWnd* pChild = GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		pChild->SetFont(&g_font12);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}

	CString strNum;
	strNum.Format("%d", m_nNum);
	m_editHangupNum.SetWindowText(strNum);

	return TRUE;
}

void CHangupSessionNumSettingDlg::OnPaint()
{
	CPaintDC dc(this);
	CXSkinDlg::DrawCaptionBar();
	CXSkinDlg::DrawBorder();
}

// ��Ӧ�ؼ���ɫ�����¼���������Ʊ���ɫ����ԭ����ɫ������
HBRUSH CHangupSessionNumSettingDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if (nCtlColor == CTLCOLOR_STATIC)
	{
		DWORD ID = GetWindowLong(pWnd->GetSafeHwnd(), GWL_ID);
		if (ID == IDC_STATIC)
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_hBkGround;
		}
	}

	////////////////////////////////////////////////////////////////////
	return hbr;
}

#include "im\MessageBox.h"
// ȷ����ť
void CHangupSessionNumSettingDlg::OnOK()
{
	CString strNum;
	m_editHangupNum.GetWindowText(strNum);
	int nNum = atoi(strNum);
	if(nNum <= 0)
	{
		CMessageBox::Model(GetSafeHwnd(),"���ûỰ������С��1��");
		m_editHangupNum.SetFocus();
		return;
	}

	m_nNum = nNum;
	if(m_nNum > 200)
	{
		CMessageBox::Model(GetSafeHwnd(),"�����������ܳ���200�ˣ�");
		return;
	}
	CXSkinDlg::OnOK();
}
