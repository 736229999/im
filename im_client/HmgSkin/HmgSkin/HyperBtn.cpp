// im\HyperBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "HyperBtn.h"

// CHyperBtn
IMPLEMENT_DYNAMIC(CHyperBtn, CStatic)
CHyperBtn::CHyperBtn()
{
	m_bOver     = false;
	m_bAutoSize = true;
	m_clrLink   = RGB(0,0,255);
	m_clrNormal = 0;
}

CHyperBtn::~CHyperBtn()
{
}


BEGIN_MESSAGE_MAP(CHyperBtn, CStatic)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CHyperBtn 消息处理程序


BOOL CHyperBtn::PreCreateWindow(CREATESTRUCT& cs)
{
	return CStatic::PreCreateWindow(cs);
}

void CHyperBtn::SetAutoSize(bool bAutoSize)
{
	m_bAutoSize = bAutoSize;
}

void CHyperBtn::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0, SS_NOTIFY, TRUE);
	CStatic::GetWindowText(m_strText);
	CStatic::PreSubclassWindow();
}

void CHyperBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;

	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	_TrackMouseEvent(&tme);

	CStatic::OnMouseMove(nFlags, point);
}

LRESULT CHyperBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	Invalidate();
	return 1;
}

LRESULT CHyperBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		m_bOver = TRUE;
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND]
	
		Invalidate();
	}

	return 1;
}

BOOL CHyperBtn::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if ( m_bOver )
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
	
	return TRUE;
}

BOOL CHyperBtn::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CHyperBtn::OnPaint()
{
	CPaintDC dc(this); 

	int nSave = dc.SaveDC();

	CFont font;
	LOGFONT lf;
	GetFont()->GetLogFont(&lf);


	CRect rcClient;
	GetClientRect(rcClient);

	if ( IsWindowEnabled() )
	{
		if ( m_bOver )
		{
			lf.lfUnderline=1;
			dc.SetTextColor(m_clrLink);
		}
		else
		{
			
			dc.SetTextColor(m_clrNormal);
		}

		dc.SelectObject(&font);
	}
	else
	{
		lf.lfUnderline=0;
		dc.SetTextColor(0xBBBBBB);
	}

	
	font.CreateFontIndirect(&lf);
	dc.SelectObject(&font);

	CRect rc;
	GetClientRect(&rc);

	dc.SetBkMode(TRANSPARENT);
	dc.DrawText(m_strText,&rc,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

	CSize sz = dc.GetTextExtent(m_strText);

	if (  m_bAutoSize  )
	{
		int cx = rc.Width();

		if ( cx>sz.cx )
		{
			ClientToScreen(&rc);
			GetParent()->ScreenToClient(&rc);
			rc.right=rc.left+sz.cx+1;
			MoveWindow(&rc,false);
		}
	}
		
	dc.SelectObject(GetStockObject(DEFAULT_GUI_FONT));
	dc.RestoreDC(nSave);

}

void CHyperBtn::SizeToContent()
{
	CClientDC dc(this);

	CSize sz = dc.GetTextExtent(m_strText);

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();

	ClientToScreen(&rc);
	GetParent()->ScreenToClient(&rc);
	rc.right=rc.left+sz.cx+1;
	MoveWindow(&rc,false);
}

void CHyperBtn::SetWindowText(LPCSTR lpszString)
{
	m_strText = lpszString;
	Invalidate();
}

void CHyperBtn::GetWindowText(CString &s)
{
	s=m_strText;
}

void CHyperBtn::Invalidate()
{	
	CWnd *pParent = GetParent();

	if ( pParent && pParent->m_hWnd )
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);

		pParent->ScreenToClient(&rc);
		pParent->InvalidateRect(&rc);
	}
}
