// EditNoticeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "EditNoticeDlg.h"
#include "afxdialogex.h"
#include "resource.h"


// CEditNoticeDlg 对话框

IMPLEMENT_DYNAMIC(CEditNoticeDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CEditNoticeDlg)

CEditNoticeDlg::CEditNoticeDlg(CWnd* pParent, string strOldNotice, const CPoint &ptPos)
	: CXSkinDlg(IDD_DLG_EDIT_NOTICE, pParent),
	m_wndTalkToolBar(&m_editNotice, &m_font)
{
	m_colorBackground = RGB(248, 248, 248);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);

	SetDlgStyle(CXSkinDlg::NONE);

	m_ptPos = ptPos;
	m_pParentWnd = pParent;
	m_clrBorder = RGB(213, 213, 213);

	m_strNotice = strOldNotice;
}

CEditNoticeDlg::~CEditNoticeDlg()
{
}

BOOL CEditNoticeDlg::Create()
{
	BOOL bRlt = CXSkinDlg::Create(IDD_DLG_EDIT_NOTICE, m_pParentWnd);
	ShowWindow(SW_SHOW);

	return bRlt;
}

void CEditNoticeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EDIT_NOTICE, m_editNotice);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_BTN_FACE, m_btnFace);
	DDX_Control(pDX, IDC_BTN_PIC, m_btnPic);
}


BEGIN_MESSAGE_MAP(CEditNoticeDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	SET_HMG_SCROLLBAR(IDC_EDIT_NOTICE)
	ON_EN_SETFOCUS(IDC_EDIT_NOTICE, &CEditNoticeDlg::OnEnSetfocusEditNotice)
END_MESSAGE_MAP()


// CEditNoticeDlg 消息处理程序
BOOL CEditNoticeDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	// 移动窗口
	CRect rc;
	GetClientRect(&rc);

	CRect newPos;
	newPos.left = m_ptPos.x;
	newPos.right = m_ptPos.x + rc.Width();
	newPos.top = m_ptPos.y;
	newPos.bottom = m_ptPos.y + rc.Height();
	MoveWindow(&newPos);

	m_btnFace.SetBitmap(IDB_TALK_EMOTION_HOT,IDB_TALK_EMOTION_NORMAL,IDB_TALK_EMOTION_DOWN,IDB_TALK_EMOTION_DISABLE);
	m_btnFace.SetToolTip("表情");

	m_btnPic.SetBitmap(IDB_TALK_SENDPIC_HOT,IDB_TALK_SENDPIC_NORMAL,IDB_TALK_SENDPIC_DOWN,IDB_TALK_SENDPIC_DISABLE);
	m_btnPic.SetToolTip("图片");

	

	// 调整字体Bar的位置
	CRect rcEdit;
	m_editNotice.GetWindowRect(&rcEdit);
	m_editNotice.SetReadOnly(FALSE);

	CRect rcTalkBar;
	rcTalkBar.left = rcEdit.left;
	rcTalkBar.right = rcEdit.right;
	rcTalkBar.top = rcEdit.bottom;
	rcTalkBar.bottom = rcTalkBar.top + 31;
	ScreenToClient(&rcTalkBar);
	m_wndTalkToolBar.Create(CP2PTalkToolBar::IDD,this);
	m_wndTalkToolBar.MoveWindow(rcTalkBar);
	m_wndTalkToolBar.ShowWindow(SW_SHOW);

	// 调整字体
	GetDlgItem(IDC_STATIC_NOTICE_TEXT)->SetFont(&g_font12B);

	// 添加滚动条
	AttachHMGScrollbar(m_editNotice.m_hWnd);

	
	m_editNotice.LimitText(MAX_FAMILYNAME_LEN);

	if(m_strNotice.empty())
	{
		m_font.fontColor = RGB(95, 95, 95);
		m_editNotice.SetXFont(m_font);
		m_editNotice.SetText("请输入群公告！");
	}
	else
	{
		m_editNotice.SetXFont(m_font);
		m_editNotice.SetText(m_strNotice.c_str());
	}

	// 隐藏字体，图片，表情按钮，以后待完善
	if(1)
	{
		ScreenToClient(&rcEdit);
		rcEdit.bottom += 62;
		m_editNotice.MoveWindow(&rcEdit);

		m_wndTalkToolBar.ShowWindow(SW_HIDE);
		m_btnFace.ShowWindow(SW_HIDE);
		m_btnPic.ShowWindow(SW_HIDE);
	}	

	return TRUE;
}

void CEditNoticeDlg::OnPaint()
{
	CXSkinDlg::OnPaint();

	CPaintDC dc(this);	
	
}

BOOL CEditNoticeDlg::OnEraseBkgnd(CDC* pDC)
{
	CXSkinDlg::OnEraseBkgnd(pDC);

	// 绘制边框
	CRect rcBorder;
	m_editNotice.GetWindowRect(&rcBorder);
	ScreenToClient(&rcBorder);
	rcBorder.InflateRect(1, 1, 1, 1);
	pDC->Draw3dRect(&rcBorder,m_clrBorder,m_clrBorder);
	int nTemp = rcBorder.bottom;

	// 隐藏字体，图片，表情按钮，以后待完善
	if(0)
	{
		CRect rcEdit;
		m_editNotice.GetWindowRect(&rcEdit);
		rcBorder.left = rcEdit.left;
		rcBorder.right = rcEdit.right;
		rcBorder.top = rcEdit.bottom;
		rcBorder.bottom = rcBorder.top + 31;
		ScreenToClient(&rcBorder);
		rcBorder.InflateRect(1, 0, 1, 1);
		pDC->Draw3dRect(&rcBorder,m_clrBorder,m_clrBorder);

		nTemp = rcBorder.bottom;
		rcBorder.bottom += rcBorder.Height();
		rcBorder.top = nTemp - 1;
		pDC->FillSolidRect(rcBorder,RGB(255, 255, 255));
		pDC->Draw3dRect(&rcBorder,m_clrBorder,m_clrBorder);
	}	

	return FALSE;
}

void CEditNoticeDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);
	if(::IsWindow(GetSafeHwnd()))
	{

	}
}

// 绘制标题时什么都不执行
void CEditNoticeDlg::DrawCaptionBar()
{

}

HBRUSH CEditNoticeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		if ( pWnd->GetDlgCtrlID()!=IDC_EDIT_BULITIN )
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_hBkGround;
		}
	}

	return hbr;
}

void CEditNoticeDlg::OnOK()
{
	CXSkinDlg::OnOK();
	CString strText;
	m_editNotice.GetText(strText);
	m_strNotice = (LPCTSTR)strText;

	::PostMessage(m_pParentWnd->GetSafeHwnd(), EDIT_NOTICE_SELF_MSG, IDOK, 0);
}

void CEditNoticeDlg::OnCancel()
{
	CXSkinDlg::OnCancel();
	::PostMessage(m_pParentWnd->GetSafeHwnd(), EDIT_NOTICE_SELF_MSG, IDCANCEL, 0);
}

void CEditNoticeDlg::OnEnSetfocusEditNotice()
{
	CString strText;
	m_editNotice.GetText(strText);
	if(m_strNotice.empty() && strText == "请输入群公告！")
	{
		m_editNotice.SetText("");
		if(m_font.fontColor == RGB(95, 95, 95))
		{
			m_font.fontColor = RGB(0, 0, 0);
		}
	}
}
