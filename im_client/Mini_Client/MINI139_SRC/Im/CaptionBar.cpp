// CaptionBar.cpp : 实现文件
//

#include "stdafx.h"
#include ".\captionbar.h"
#include "impublic.h"
#include "..\resource.h"

// CCaptionBar

IMPLEMENT_DYNAMIC(CCaptionBar, CStatic)
CCaptionBar::CCaptionBar()
{
	m_bOver		= false;
	m_clrTrans	= 0;
	m_clrBk     = IM_SPACE_COLOR;
}

CCaptionBar::~CCaptionBar()
{
}


BEGIN_MESSAGE_MAP(CCaptionBar, CStatic)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()



// CCaptionBar 消息处理程序


BOOL CCaptionBar::OnEraseBkgnd(CDC* pDC)
{
	//return CStatic::OnEraseBkgnd(pDC);
	return FALSE;
}

void CCaptionBar::OnPaint()
{
	CPaintDC dcPaint(this);

	CRect rc;
	GetClientRect(&rc);

	int cx = rc.Width();
	int cy = rc.Height();

	CDC dc;
	dc.CreateCompatibleDC(&dcPaint);

	int nSavedDC = dc.SaveDC();

	CBitmap bmp_back;
	bmp_back.CreateCompatibleBitmap(&dcPaint,cx,cy);
  
	dc.SelectObject(&bmp_back);
	//dc.FillSolidRect(&rc,m_clrBk);

	CResBitmap bmp_fill;
	bmp_fill.LoadBitmap(IDB_TALK_TITLE_BACK);

	HorizStretchBlt(&dcPaint,0,0,cx,&bmp_fill);

	int xoffset = 1;

	dc.SetBkMode(TRANSPARENT);

	CPoint pt(3,2);
	g_imgHead20.Draw(&dc,GET_HEAD_IMAGE(m_nFaceId,m_nStatus),pt,ILD_NORMAL);

	xoffset += 20;

	CFont font;
	font.CreateFont(12, 
		0,
		0, 
		90,
		FW_NORMAL,
		0,
		m_bOver,//italic
		0,
		DEFAULT_CHARSET,
		OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,
		DRAFT_QUALITY,
		(DEFAULT_PITCH|FF_MODERN),
		"宋体");

	if ( m_bOver )
	{
		dc.SetTextColor(RGB(0,0,255));
	}
	else
	{
		dc.SetTextColor(RGB(0,0,0));
	}

	dc.SelectObject(&font);

	CString s;
	GetWindowText(s);

	CSize sz = dc.GetTextExtent(s);

	int yoffset = ( cy-sz.cy )/2;
	dc.TextOut(xoffset,yoffset,s);

	m_rcHover.left = 0;
	m_rcHover.top  = 0;
	m_rcHover.right = xoffset+sz.cx;
	m_rcHover.bottom = cy;

	CPen penWhite(PS_SOLID,1,RGB(255,255,255));
	CPen penLine(PS_SOLID,1,RGB(125,167,217));

	dc.SelectObject(&penWhite);

	dc.MoveTo(0,cy-1);
	dc.LineTo(cx,cy-1);

	dc.MoveTo(0,cy-3);
	dc.LineTo(cx,cy-3);

	dc.SelectObject(&penLine);

	dc.MoveTo(0,cy-2);
	dc.LineTo(cx,cy-2);

	dcPaint.BitBlt(0,0,cx,cy,&dc,0,0,SRCCOPY);

	dc.RestoreDC(nSavedDC);
}


LRESULT CCaptionBar::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		POINT pt;
		GetCursorPos(&pt);
		ScreenToClient(&pt);

		if ( m_rcHover.PtInRect(pt) )
		{
			m_bOver=true;

			LOGFONT lf;
			GetFont()->GetLogFont(&lf);
			lf.lfUnderline=true;
			CFont font;
			font.CreateFontIndirect(&lf);
			SetFont(&font);
			Invalidate();
		}
	}

	return 1;
}

LRESULT CCaptionBar::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = false;

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	lf.lfUnderline=false;

	CFont font;
	font.CreateFontIndirect(&lf);
	SetFont(&font);

	Invalidate();
	return 1;
}

void CCaptionBar::OnMouseMove(UINT nFlags, CPoint point) 
{
	POINT pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		_TrackMouseEvent(&tme);


	if ( m_bOver==true && !m_rcHover.PtInRect(pt) )
	{
		m_bOver=false;
		Invalidate();
	}
	
	if ( m_bOver )
	{
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
	}	

	CStatic::OnMouseMove(nFlags, point);
}

void CCaptionBar::SetFaceId(int nId, int nStatus)
{
	m_nFaceId=nId;
	m_nStatus=nStatus;
	if (m_hWnd)
		Invalidate();
}

void CCaptionBar::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类
	ModifyStyle(0,SS_NOTIFY);
	CStatic::PreSubclassWindow();
}

/*
void CCaptionBar::SetCaptionRgn(CRgn &rgn,CRect rect)
{
	rect.right+=1;

	CRgn rgnWnd; rgnWnd.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
	CRgn rgnTop; rgnTop.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom/2);
	CRgn rgnRound; rgnRound.CreateRoundRectRgn(rect.left,rect.top,rect.right,rect.bottom,14,14);

	CRgn rgnTemp;
	rgnTemp.CreateRectRgn(0,0,50,50);
	rgnTemp.CombineRgn(&rgnTop,&rgnRound,RGN_DIFF);

	rgn.CreateRectRgn(0,0,50,50);
	rgn.CombineRgn(&rgnWnd,&rgnTemp,RGN_XOR);	
}
*/
