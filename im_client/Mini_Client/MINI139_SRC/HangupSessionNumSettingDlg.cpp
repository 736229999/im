// HangupSessionNumSettingDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "HangupSessionNumSettingDlg.h"
#include "afxdialogex.h"
#include "resource.h"



// CHangupSessionNumSettingDlg 对话框

IMPLEMENT_DYNAMIC(CHangupSessionNumSettingDlg, CXSkinDlg)

CHangupSessionNumSettingDlg::CHangupSessionNumSettingDlg(int nNum, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(IDD_DLG_HANGUP_NUM_SETTING, pParent)
{
	////////添加颜色设置///////////////
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


// CHangupSessionNumSettingDlg 消息处理程序
BOOL CHangupSessionNumSettingDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("自定义挂起");

	m_editHangupNum.SubclassWindow(GetDlgItem(IDC_EDIT_SESSION_NUM)->m_hWnd);
	m_editHangupNum.LimitText(3);
	m_editHangupNum.SetBorderColor(RGB(88,88,88));

	// 设置控件字体
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

// 相应控件颜色绘制事件，解决控制背景色保持原有颜色的问题
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
// 确定按钮
void CHangupSessionNumSettingDlg::OnOK()
{
	CString strNum;
	m_editHangupNum.GetWindowText(strNum);
	int nNum = atoi(strNum);
	if(nNum <= 0)
	{
		CMessageBox::Model(GetSafeHwnd(),"设置会话数不能小于1！");
		m_editHangupNum.SetFocus();
		return;
	}

	m_nNum = nNum;
	if(m_nNum > 200)
	{
		CMessageBox::Model(GetSafeHwnd(),"挂起人数不能超过200人！");
		return;
	}
	CXSkinDlg::OnOK();
}
