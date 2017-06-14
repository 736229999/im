// SplitterControl.cpp : implementation file
//

#include "stdafx.h"
#include "resource.h"
#include "SplitterControl.h"
#include "skinbitmap.h"

/////////////////////////////////////////////////////////////////////////////
// CSplitterControl

// hCursor1 is for vertiacal one
// and hCursor2 is for horizontal one
static HCURSOR SplitterControl_hCursor1 = NULL;
static HCURSOR SplitterControl_hCursor2 = NULL;

CSplitterControl::CSplitterControl()
{
	// Mouse is pressed down or not ?
	m_bIsPressed = FALSE;	
	m_bButtonDown = false;
	m_bSetButton  = false;
	m_bMouseOverButton = false;
	m_nLimitSize = 30;
}

CSplitterControl::~CSplitterControl()
{
}


BEGIN_MESSAGE_MAP(CSplitterControl, CStatic)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_SIZE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CSplitterControl message handlers

/****************************************************
*	Create(DWORD dwStyle, const CRect& rect, CWnd* pParent, nID)
*	Use this function instead of the CStatic::Create function
* Parameters: No need to explain (see MSDN (:-) )
*
****************************************************/
void CSplitterControl::Create(DWORD dwStyle, const CRect &rect, CWnd *pParent, UINT nID)
{
	CRect rc = rect;
	dwStyle |= SS_NOTIFY;
	
	// Determine default type base on it's size.
	m_nType = (rc.Width() < rc.Height())?
					SPS_VERTICAL:
					SPS_HORIZONTAL;
	
	if (m_nType == SPS_VERTICAL)
		rc.right = rc.left + 5;
	else // SPS_HORIZONTAL
		rc.bottom = rc.top + 5;
	
	CStatic::Create(_T(""), dwStyle, rc, pParent, nID);
	
	if (!SplitterControl_hCursor1)
	{
		SplitterControl_hCursor1 = AfxGetApp()->LoadStandardCursor(IDC_SIZEWE);
		SplitterControl_hCursor2 = AfxGetApp()->LoadStandardCursor(IDC_SIZENS);
	}
}

void CSplitterControl::SetButton(bool bSet)
{
	m_bSetButton = bSet;
}
// Set style for splitter control
// nStyle = SPS_VERTICAL or SPS_HORIZONTAL
int CSplitterControl::SetStyle(int nStyle)
{
	int m_nOldStyle = m_nType;
	m_nType = nStyle;
	return m_nOldStyle;
}
int CSplitterControl::GetStyle()
{
	return m_nType;
}

void CSplitterControl::OnPaint() 
{
	CPaintDC dc(this); // device context for painting

	CRect rc;
	GetClientRect(&rc);
	dc.FillSolidRect(&rc,RGB(202,235,248));

	if ( m_nType==SPS_HORIZONTAL && m_bSetButton )
	{
		DrawButton(&dc);
	}
}

void CSplitterControl::OnMouseMove(UINT nFlags, CPoint point) 
{
	if (m_bIsPressed)
	{
		CWindowDC dc(NULL);
		DrawLine(&dc, m_nX, m_nY);
		
		CPoint pt = point;
		ClientToScreen(&pt);
		GetParent()->ScreenToClient(&pt);

		CRect rcParent;
		GetParent()->GetClientRect(&rcParent);

		if ( m_nType==SPS_HORIZONTAL )
		{
			rcParent.top+=m_nLimitSize;
			rcParent.bottom-=m_nLimitSize;

			if (pt.y < rcParent.top)
				pt.y = rcParent.top;
			
			if (pt.y > rcParent.bottom)
				pt.y = rcParent.bottom;

		}
		else
		{
			rcParent.left+=m_nLimitSize;
			rcParent.right-=m_nLimitSize;

			if (pt.x < rcParent.left)
				pt.x = rcParent.left;


			if (pt.x > rcParent.right)
				pt.x = rcParent.right;
		}

		GetParent()->ClientToScreen(&pt);
		m_nX = pt.x;
		m_nY = pt.y;
		DrawLine(&dc, m_nX, m_nY);
	}
	else
	{
		if ( m_bSetButton ) 
		{
			if ( m_bMouseOverButton==false )
			{
				if ( m_rcButton.PtInRect(point) )
				{
					m_bMouseOverButton=true;
					SetTimer(0x01,10,NULL);
					CDC * pdc = GetDC();
					DrawButton(pdc);
					ReleaseDC(pdc);
				}
			}
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}

BOOL CSplitterControl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	
	if (nHitTest == HTCLIENT )
	{
		if (m_bSetButton && m_rcButton.PtInRect(pt) )
		{
			return CStatic::OnSetCursor(pWnd, nHitTest, message);
		}

		(m_nType == SPS_VERTICAL)?(::SetCursor(SplitterControl_hCursor1))
			:(::SetCursor(SplitterControl_hCursor2));

		return 0;
	}
	else
		return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

void CSplitterControl::OnLButtonDown(UINT nFlags, CPoint point) 
{
	CStatic::OnLButtonDown(nFlags, point);
	
	SetCapture();

	if ( m_bSetButton ) 
	{
		if ( m_rcButton.PtInRect(point) )
		{
			m_bButtonDown = true;


			CDC *pDC = GetDC();
			DrawButton(pDC);
			ReleaseDC(pDC);
			return ;
		}
	}

	m_bIsPressed = TRUE;
	CRect rcWnd;
	GetWindowRect(rcWnd);
	
	if (m_nType == SPS_VERTICAL)
		m_nX = rcWnd.left + rcWnd.Width() / 2;	
	
	else
		m_nY = rcWnd.top  + rcWnd.Height() / 2;
	
	if (m_nType == SPS_VERTICAL)
		m_nSavePos = m_nX;
	else
		m_nSavePos = m_nY;

	CWindowDC dc(NULL);
	DrawLine(&dc, m_nX, m_nY);
	
}

void CSplitterControl::OnLButtonUp(UINT nFlags, CPoint point) 
{
	ReleaseCapture();

	if ( m_bButtonDown )
	{
		//post message
		SPC_NMHDR nmsp;

		nmsp.hdr.hwndFrom = m_hWnd;
		nmsp.hdr.idFrom   = GetDlgCtrlID();
		nmsp.hdr.code     = 0;
		nmsp.delta = 0;

		m_bButtonDown=false;

		CDC *pDC = GetDC();
		DrawButton(pDC);
		ReleaseDC(pDC);

		GetOwner()->SendMessage(WM_NOTIFY, nmsp.hdr.idFrom, (LPARAM)&nmsp);
	}
	else
	if (m_bIsPressed)
	{
		ClientToScreen(&point);
		CWindowDC dc(NULL);

		DrawLine(&dc, m_nX, m_nY);
		CPoint pt(m_nX, m_nY);
		m_bIsPressed = FALSE;
		CWnd *pOwner = GetOwner();
		if (pOwner && IsWindow(pOwner->m_hWnd))
		{
			CRect rc;
			int delta;
			pOwner->GetClientRect(rc);
			pOwner->ScreenToClient(&pt);
			//MoveWindowTo(pt);

			if (m_nType == SPS_VERTICAL)
				delta = m_nX - m_nSavePos;
			else
				delta = m_nY - m_nSavePos;
			
			
			SPC_NMHDR nmsp;
		
			nmsp.hdr.hwndFrom = m_hWnd;
			nmsp.hdr.idFrom   = GetDlgCtrlID();
			nmsp.hdr.code     = SPN_SIZED;
			nmsp.delta = delta;

			pOwner->SendMessage(WM_NOTIFY, nmsp.hdr.idFrom, (LPARAM)&nmsp);
		}
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CSplitterControl::DrawLine(CDC* pDC, int x, int y)
{
	int nRop = pDC->SetROP2(R2_NOTXORPEN);

	CRect rcWnd;
	int d = 1;
	GetWindowRect(rcWnd);
	CPen  pen;
	pen.CreatePen(0, 1, RGB(50, 100, 100));
	CPen *pOP = pDC->SelectObject(&pen);
	
	if (m_nType == SPS_VERTICAL)
	{
		pDC->MoveTo(m_nX - d, rcWnd.top);
		pDC->LineTo(m_nX - d, rcWnd.bottom);

		pDC->MoveTo(m_nX , rcWnd.top);
		pDC->LineTo(m_nX , rcWnd.bottom);

		pDC->MoveTo(m_nX + d, rcWnd.top);
		pDC->LineTo(m_nX + d, rcWnd.bottom);
	}
	else // m_nType == SPS_HORIZONTAL
	{
		pDC->MoveTo(rcWnd.left, m_nY - d);
		pDC->LineTo(rcWnd.right, m_nY - d);
	
		pDC->MoveTo(rcWnd.left, m_nY );
		pDC->LineTo(rcWnd.right, m_nY );

		pDC->MoveTo(rcWnd.left, m_nY + d);
		pDC->LineTo(rcWnd.right, m_nY + d);
	}
	pDC->SetROP2(nRop);
	pDC->SelectObject(pOP);
}

void CSplitterControl::MoveWindowTo(CPoint pt)
{
	CRect rc;
	GetWindowRect(rc);
	CWnd* pParent;
	pParent = GetParent();
	if (!pParent || !::IsWindow(pParent->m_hWnd))
		return;

	pParent->ScreenToClient(rc);
	if (m_nType == SPS_VERTICAL)
	{	
		int nMidX = (rc.left + rc.right) / 2;
		int dx = pt.x - nMidX;
		rc.OffsetRect(dx, 0);
	}
	else
	{	
		int nMidY = (rc.top + rc.bottom) / 2;
		int dy = pt.y - nMidY;
		rc.OffsetRect(0, dy);
	}
	MoveWindow(rc);
}

void CSplitterControl::ChangeWidth(CWnd *pWnd, int dx, DWORD dwFlag)
{
	CWnd* pParent = pWnd->GetParent();
	if (pParent && ::IsWindow(pParent->m_hWnd))
	{
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		pParent->ScreenToClient(rcWnd);
		if (dwFlag == CW_LEFTALIGN)
			rcWnd.right += dx;
		else if (dwFlag == CW_RIGHTALIGN)
			rcWnd.left -= dx;
		pWnd->MoveWindow(rcWnd);
	}
}

void CSplitterControl::ChangeHeight(CWnd *pWnd, int dy, DWORD dwFlag)
{
	CWnd* pParent = pWnd->GetParent();
	if (pParent && ::IsWindow(pParent->m_hWnd))
	{
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		pParent->ScreenToClient(rcWnd);
		if (dwFlag == CW_TOPALIGN)
			rcWnd.bottom += dy;
		else if (dwFlag == CW_BOTTOMALIGN)
			rcWnd.top -= dy;
		pWnd->MoveWindow(rcWnd);
	}
}

void CSplitterControl::ChangePos(CWnd* pWnd, int dx, int dy)
{
	CWnd* pParent = pWnd->GetParent();
	if (pParent && ::IsWindow(pParent->m_hWnd))
	{
		CRect rcWnd;
		pWnd->GetWindowRect(rcWnd);
		pParent->ScreenToClient(rcWnd);
		rcWnd.OffsetRect(-dx, dy);

		pWnd->MoveWindow(rcWnd);
	}	
}


void CSplitterControl::OnSize(UINT nType, int cx, int cy)
{
	CStatic::OnSize(nType, cx, cy);

	m_rcButton.left = (cx-120)/2;
	m_rcButton.top  = 0;
	m_rcButton.bottom = m_rcButton.top+8;
	m_rcButton.right = m_rcButton.left+120;

	Invalidate();
}

void CSplitterControl::DrawButton(CDC *pDC)
{
	CSkinBitmap bmp;

	if ( m_bButtonDown )
		bmp.LoadBitmap(IDB_UPDOWN_NORMAL );
    else
	if ( m_bMouseOverButton )
		bmp.LoadBitmap(IDB_UPDOWN_HOT);
	else
		bmp.LoadBitmap(IDB_UPDOWN_NORMAL);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap *p = dcMem.SelectObject(&bmp);

	int cx = m_rcButton.Width();
	int cy = m_rcButton.Height();

	pDC->TransparentBlt(m_rcButton.left,m_rcButton.top,cx,cy,&dcMem,0,0,cx,cy,RGB(0,255,0));
	dcMem.SelectObject(p);
}

void CSplitterControl::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{			
		if ( m_bMouseOverButton )
		{
			CPoint point;
			GetCursorPos(&point);
			ScreenToClient(&point);

			if ( m_rcButton.PtInRect(point)==FALSE )
			{
				m_bMouseOverButton=false;
				KillTimer(0x01);
				CDC * pdc = GetDC();
				DrawButton(pdc);
				ReleaseDC(pdc);
			}
		}
	}

	CStatic::OnTimer(nIDEvent);
}
