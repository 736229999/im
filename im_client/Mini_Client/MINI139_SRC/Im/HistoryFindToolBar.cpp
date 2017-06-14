// HistoryFindToolBar.cpp : implementation file
//

#include "stdafx.h"
#include "HistoryFindToolBar.h"
#include "afxdialogex.h"


// CHistoryFindToolBar dialog

IMPLEMENT_DYNAMIC(CHistoryFindToolBar, CDialog)

CHistoryFindToolBar::CHistoryFindToolBar(CWnd* pParent /*=NULL*/)
	: CDialog(CHistoryFindToolBar::IDD, pParent)
{

}

CHistoryFindToolBar::~CHistoryFindToolBar()
{
}

void CHistoryFindToolBar::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_BTN_FIND, m_btnFind);
	DDX_Control(pDX, IDC_EDIT_KEYWORD, m_editKeyword);
	DDX_Control(pDX, IDC_COMBO1, m_comboRange);
}


BEGIN_MESSAGE_MAP(CHistoryFindToolBar, CDialog)
END_MESSAGE_MAP()


// CHistoryFindToolBar message handlers
