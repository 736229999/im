// RegBasePage.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "WizardPage.h"
#include "impublic.h"

#include "..\Resource.h"
// CWizardPage 对话框

IMPLEMENT_DYNAMIC(CWizardPage, CDialog)
CWizardPage::CWizardPage(UINT nID, CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	m_hBrushBackGround = CreateSolidBrush(IM_SPACE_COLOR);
}

CWizardPage::~CWizardPage()
{
	DeleteObject(m_hBrushBackGround);
}

void CWizardPage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CWizardPage, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CWizardPage 消息处理程序

HBRUSH CWizardPage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		hbr = g_hWhiteBrush;
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

BOOL CWizardPage::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,0xFFFFFF);
	return FALSE;
}

BOOL CWizardPage::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case 0x0D:
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

void CWizardPage::OnOK()
{

}
