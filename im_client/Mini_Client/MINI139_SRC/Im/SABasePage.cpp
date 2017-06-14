// SABasePage.cpp : 实现文件
//

#include "stdafx.h"
#include "SABasePage.h"
#include ".\sabasepage.h"


// CSABasePage 对话框

IMPLEMENT_DYNAMIC(CSABasePage, CDialog)
CSABasePage::CSABasePage(UINT nID,CWnd* pParent /*=NULL*/)
	: CDialog(nID, pParent)
{
	m_hBrushBackGround = CreateSolidBrush(RGB(255,255,255));
	m_hBrushDisable = CreateSolidBrush(RGB(200,200,200));
	m_bModified   = false;
	m_bModifyFlag = false;
}

CSABasePage::~CSABasePage()
{
	DeleteObject(m_hBrushBackGround);
	DeleteObject(m_hBrushDisable);
}


BEGIN_MESSAGE_MAP(CSABasePage, CDialog)
	ON_WM_CTLCOLOR()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CSABasePage::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}
// CSABasePage 消息处理程序

void CSABasePage::OnOK()
{
}

HBRUSH CSABasePage::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	hbr=m_hBrushBackGround;

	if ( nCtlColor==CTLCOLOR_EDIT )
	{
		if ( pWnd->IsWindowEnabled()==FALSE )
			hbr=m_hBrushDisable;
	}
	
	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
	}

	return hbr;
}

LRESULT CSABasePage::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( m_hWnd )
	{
		if ( ( m_bModifyFlag==false ) &&
			( message>= WM_MOUSEFIRST && message<=WM_MOUSELAST ) ||
			( message>= WM_KEYFIRST && message<=WM_KEYLAST ) )
		{
			m_bModifyFlag=true;
		}

		if ( m_bModifyFlag && message==WM_COMMAND )
		{
			WORD w=HIWORD(wParam);

			switch (w)
			{
			case EN_CHANGE:
			case BN_CLICKED:
			case CBN_SELCHANGE:
			case CBN_EDITCHANGE:
				m_bModified=true;
				break;
			}
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

BOOL CSABasePage::PreTranslateMessage(MSG* pMsg)
{
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE:
			return TRUE;
		default:
			break;
		}
	}

	return CDialog::PreTranslateMessage(pMsg);
}

BOOL CSABasePage::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,RGB(255,255,255));

	return TRUE;
}
