// CHistoryToolBar dialog

#include "stdafx.h"
#include "HistoryToolBar.h"
#include "PopupHistoryDlg.h"
#include "HistoryFindToolBar.h"


IMPLEMENT_DYNAMIC(CHistoryToolBar, CDialog)

	CHistoryToolBar::CHistoryToolBar(CWnd* pHitory, CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryToolBar::IDD, pParent)
	, m_pHitory(pHitory)
{

}

CHistoryToolBar::~CHistoryToolBar()
{
}

void CHistoryToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BUTTON_FIRST, m_btnFirst);
	DDX_Control(pDX, IDC_BUTTON_NEXT, m_btnNext);
	DDX_Control(pDX, IDC_BUTTON_LAST, m_btnLast);
	DDX_Control(pDX, IDC_BUTTON_PREV, m_btnPrev);
	DDX_Control(pDX, IDC_BUTTON_HISTORY_FIND, m_btnSearch);
	DDX_Control(pDX, IDC_STATIC_HISTORY_PAGE_INFO, m_staticHistoryPage);
}

BOOL CHistoryToolBar::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_btnFirst.SetPng(IDB_PNG_FIRST_HOVER, IDB_PNG_FIRST_NORMAL, IDB_PNG_FIRST_PRESSED, IDB_PNG_FIRST_DISABLE);
	m_btnFirst.SetToolTip(_T("第一页"));
	m_btnPrev.SetPng(IDB_PNG_PREV_HOVER, IDB_PNG_PREV_NORMAL, IDB_PNG_PREV_PRESSED, IDB_PNG_PREV_DISABLE);
	m_btnPrev.SetToolTip(_T("上一页"));
	m_btnNext.SetPng(IDB_PNG_NEXT_HOVER, IDB_PNG_NEXT_NORMAL, IDB_PNG_NEXT_PRESSED, IDB_PNG_NEXT_DISABLE);
	m_btnNext.SetToolTip(_T("下一页"));
	m_btnLast.SetPng(IDB_PNG_LAST_HOVER, IDB_PNG_LAST_NORMAL, IDB_PNG_LAST_PRESSED, IDB_PNG_LAST_DISABLE);
	m_btnLast.SetToolTip(_T("最后一页"));
	m_btnSearch.SetPic(IDB_PNG_SEARCH_PRESSED, IDB_PNG_SEARCH_HOVER, IDB_PNG_SEARCH_PRESSED, IDB_PNG_SEARCH_NORMAL, AfxGetInstanceHandle());
	m_btnSearch.SetToolTip(_T("查找"));

	m_btnFirst.EnableWindow(FALSE);
	m_btnLast.EnableWindow(FALSE);
	m_btnNext.EnableWindow(FALSE);
	m_btnPrev.EnableWindow(FALSE);

	m_staticHistoryPage.SetFont(&g_font12);

	return TRUE;  // return TRUE unless you set the focus to a control
	// EXCEPTION: OCX Property Pages should return FALSE
}


BEGIN_MESSAGE_MAP(CHistoryToolBar, CDialog)
	ON_BN_CLICKED(IDC_BUTTON_HISTORY_FIND, &CHistoryToolBar::OnBnClickedButtonHistoryFind)
	ON_BN_CLICKED(IDC_BUTTON_FIRST, &CHistoryToolBar::OnBnClickedButtonFirst)
	ON_BN_CLICKED(IDC_BUTTON_PREV, &CHistoryToolBar::OnBnClickedButtonPrev)
	ON_BN_CLICKED(IDC_BUTTON_NEXT, &CHistoryToolBar::OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_LAST, &CHistoryToolBar::OnBnClickedButtonLast)
END_MESSAGE_MAP()



void CHistoryToolBar::OnBnClickedButtonHistoryFind()
{
	::PostMessage(m_pHitory->m_hWnd, WM_SHOW_HISTORY_FIND_TOOL, 0, 0);
}


void CHistoryToolBar::OnBnClickedButtonFirst()
{
	::PostMessage(m_pHitory->m_hWnd, WM_HISTORY_PAGE, PAGE_FIRST, 0);
}


void CHistoryToolBar::OnBnClickedButtonPrev()
{
	::PostMessage(m_pHitory->m_hWnd, WM_HISTORY_PAGE, PAGE_PREV, 0);
}


void CHistoryToolBar::OnBnClickedButtonNext()
{
	::PostMessage(m_pHitory->m_hWnd, WM_HISTORY_PAGE, PAGE_NEXT, 0);
}


void CHistoryToolBar::OnBnClickedButtonLast()
{
	::PostMessage(m_pHitory->m_hWnd, WM_HISTORY_PAGE, PAGE_LAST, 0);
}

void CHistoryToolBar::SetPage(INT32 iTotal, INT32 iCurrent)
{
	CString str;
	str.Format("共%d页，当前%d页", iTotal, iCurrent);
	m_staticHistoryPage.SetWindowText(str);
}
BOOL CHistoryToolBar::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_KEYDOWN )
	{
		switch (pMsg->wParam)  
		{ 
		case VK_ESCAPE:
		case VK_RETURN : 
			{
				return TRUE;		// 不处理回车和esc
			}
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}