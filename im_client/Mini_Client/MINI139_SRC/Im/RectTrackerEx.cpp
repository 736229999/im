#include "StdAfx.h"
#include "RectTrackerEx.h"
#include "ScreenCtrl.h"
AFX_STATIC_DATA HCURSOR _afxCursors[10] = 
	{ 
		::LoadCursor(NULL,MAKEINTRESOURCE(32642)), 
		::LoadCursor(NULL,MAKEINTRESOURCE(32643)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32642)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32643)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32645)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32644)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32645)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32644)),
		::LoadCursor(NULL,MAKEINTRESOURCE(32646)),
		::LoadCursor(NULL,IDC_CROSS),
	};
AFX_STATIC_DATA HBRUSH _afxHatchBrush = 0;
AFX_STATIC_DATA HPEN _afxBlackDottedPen = 0;
AFX_STATIC_DATA int _afxHandleSize = 0;

CRectTrackerEx::CRectTrackerEx(void)
{
	m_bIsRefresh = FALSE;
	m_bIsDrawPointCross = FALSE;
	m_nTrackerType = 0;
	m_dwColor = RGB(51,255,255);
}

CRectTrackerEx::~CRectTrackerEx(void)
{
}

void CRectTrackerEx::SetTrackerType(int nType)
{
	m_nTrackerType = nType;
}

void CRectTrackerEx::SetTrackerColor(DWORD dwColor)
{
	m_dwColor = dwColor;
}
void CRectTrackerEx::SetScreenCtrl(CScreenCtrl* pScreenCtrl)
{
	m_pScreenCtrl = pScreenCtrl;
}

void CRectTrackerEx::Draw(CDC* pDC) const
{
	// set initial DC state
	VERIFY(pDC->SaveDC() != 0);

	pDC->SetMapMode(MM_TEXT);
	pDC->SetViewportOrg(0, 0);
	pDC->SetWindowOrg(0, 0);

	// get normalized rectangle
	CRect rect = m_rect;
	rect.NormalizeRect();

	//	rect.InflateRect(2,2);

	CPen* pOldPen = NULL;
	CBrush* pOldBrush = NULL;
	CGdiObject* pTemp;
	int nOldROP;

	// draw lines
	if ((m_nStyle & (dottedLine|solidLine)) != 0)
	{
		if (m_nStyle & dottedLine)
			pOldPen = pDC->SelectObject(CPen::FromHandle(_afxBlackDottedPen));
		else

			pOldPen = (CPen*)pDC->SelectStockObject(BLACK_PEN);

 		CPen penNew;   
 		penNew.CreatePen(PS_SOLID,1,m_dwColor); 
 		pOldPen = (CPen*)pDC->SelectObject(&penNew);

		pOldBrush = (CBrush*)pDC->SelectStockObject(NULL_BRUSH);
		nOldROP = pDC->SetROP2(R2_COPYPEN);

		if(m_nTrackerType != TRACKER_TYPE_HEAD)
			rect.InflateRect(+1, +1);   // borders are one pixel outside

		pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// if hatchBrush is going to be used, need to unrealize it
	if ((m_nStyle & (hatchInside|hatchedBorder)) != 0)
		UnrealizeObject(_afxHatchBrush);

	// hatch inside
	if ((m_nStyle & hatchInside) != 0)
	{
		pTemp = pDC->SelectStockObject(NULL_PEN);
		if (pOldPen == NULL)
			pOldPen = (CPen*)pTemp;

		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;

		pDC->SetBkMode(TRANSPARENT);
		nOldROP = pDC->SetROP2(R2_MASKNOTPEN);
// 		if(m_nTrackerType == TRACKER_TYPE_HEAD)
// 			pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
// 		else
			pDC->Rectangle(rect.left+1, rect.top+1, rect.right, rect.bottom);
		pDC->SetROP2(nOldROP);
	}

	// draw hatched border
	if ((m_nStyle & hatchedBorder) != 0)
	{
		pTemp = pDC->SelectObject(CBrush::FromHandle(_afxHatchBrush));
		if (pOldBrush == NULL)
			pOldBrush = (CBrush*)pTemp;

		pDC->SetBkMode(OPAQUE);

		CRect rectTrue;
		GetTrueRect(&rectTrue);

		pDC->PatBlt(rectTrue.left, rectTrue.top, rectTrue.Width(),
			rect.top-rectTrue.top, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.bottom,
			rectTrue.Width(), rectTrue.bottom-rect.bottom, 0x000F0001 /* Pn */);
		pDC->PatBlt(rectTrue.left, rect.top, rect.left-rectTrue.left,
			rect.Height(), 0x000F0001 /* Pn */);
		pDC->PatBlt(rect.right, rect.top, rectTrue.right-rect.right,
			rect.Height(), 0x000F0001 /* Pn */);
	}

	// draw resize handles
	if ((m_nStyle & (resizeInside|resizeOutside/*|resizeMidside*/)) != 0)
	{
		UINT mask = GetHandleMask();
		for (int i = 0; i < 8; ++i)
		{
			if (mask & (1<<i))
			{
				//pTemp=pDC->SelectStockObject(BLACK_PEN);

				CPen penNew;   
				penNew.CreatePen(PS_SOLID,1,m_dwColor); 
				pTemp = (CPen*)pDC->SelectObject(&penNew);

				if(pOldPen!=NULL)
					pOldPen=(CPen *)pTemp;

				CBrush br;
				br.CreateSolidBrush(m_dwColor);

				pTemp = pDC->SelectObject(&br);
				if (pOldBrush == NULL)
					pOldBrush = (CBrush*)pTemp;

				GetHandleRect((TrackerHit)i, &rect);
				if(m_nTrackerType != TRACKER_TYPE_HEAD)
					rect.InflateRect(+1, +1);
				pDC->Rectangle/*Ellipse*/(rect);
			}
		}
	}

	// cleanup pDC state
	if (pOldPen != NULL)
		pDC->SelectObject(pOldPen);
	if (pOldBrush != NULL)
		pDC->SelectObject(pOldBrush);

	VERIFY(pDC->RestoreDC(-1));
}

BOOL CRectTrackerEx::Track(CWnd* pWnd, CPoint point, BOOL bAllowInvert,
	CWnd* pWndClipTo)
{
	// perform hit testing on the handles
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
	{
		// didn't hit a handle, so just return FALSE
		return FALSE;
	}

	// otherwise, call helper function to do the tracking
	m_bAllowInvert = bAllowInvert;
	return TrackHandle(nHandle, pWnd, point, pWndClipTo);
}

int CRectTrackerEx::HitTestHandles(CPoint point) const
{
	//AfxMessageBox("1234");
	if(m_bIsDrawPointCross)
	{
		return (TrackerHit)9;
	}
	CRect rect;
	UINT mask = GetHandleMask();

	// see if hit anywhere inside the tracker
	GetTrueRect(&rect);
	CRect rcTrue;
	GetTrueRect(&rcTrue);
	if (!rect.PtInRect(point))
		return hitNothing;  // totally missed

	// see if we hit a handle
	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if (rect.PtInRect(point))
			{
				return (TrackerHit)i;
			}
		}
	}

	for (int i = 0; i < 8; ++i)
	{
		if (mask & (1<<i))
		{
			GetHandleRect((TrackerHit)i, &rect);
			if( i == hitTop || i == hitBottom)
			{
				rect.left = rcTrue.left;
				rect.right = rcTrue.right;
			}
			if( i == hitRight || i == hitLeft )
			{
				rect.top = rcTrue.top;
				rect.bottom = rcTrue.bottom;
			}
			if (rect.PtInRect(point))
				return (TrackerHit)i;
		}
	}

	// last of all, check for non-hit outside of object, between resize handles
	if ((m_nStyle & hatchedBorder) == 0)
	{
		CRect rect = m_rect;
		rect.NormalizeRect();
		if ((m_nStyle & dottedLine|solidLine) != 0)
			rect.InflateRect(+1, +1);
		if (!rect.PtInRect(point))
			return hitNothing;  // must have been between resize handles
	}
	return hitMiddle;   // no handle hit, but hit object (or object border)
}

BOOL CRectTrackerEx::SetCursor(CWnd* pWnd, UINT nHitTest) const
{
	// trackers should only be in client area
	if (nHitTest != HTCLIENT)
		return FALSE;

	// convert cursor position to client co-ordinates
	CPoint point;
	GetCursorPos(&point);
	pWnd->ScreenToClient(&point);

	// do hittest and normalize hit
	int nHandle = HitTestHandles(point);
	if (nHandle < 0)
		return FALSE;

	// need to normalize the hittest such that we get proper cursors
	if(nHandle != 9)
		nHandle = NormalizeHit(nHandle);

	// handle special case of hitting area between handles
	//  (logically the same -- handled as a move -- but different cursor)
	if (nHandle == hitMiddle && !m_rect.PtInRect(point))
	{
		// only for trackers with hatchedBorder (ie. in-place resizing)
		if (m_nStyle & hatchedBorder)
			nHandle = (TrackerHit)9;
	}

	ENSURE(nHandle < _countof(_afxCursors));
	::SetCursor(_afxCursors[nHandle]);
	//::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(_afxCursors[nHandle])));//IDC HAND
	return TRUE;
}

BOOL CRectTrackerEx::TrackHandle(int nHandle, CWnd* pWnd, CPoint point,
	CWnd* pWndClipTo)
{
	ASSERT(nHandle >= 0);
	ASSERT(nHandle <= 8);   // handle 8 is inside the rect

	// don't handle if capture already set
	if (::GetCapture() != NULL)
		return FALSE;

	AfxLockTempMaps();  // protect maps while looping

	ASSERT(!m_bFinalErase);

	// save original width & height in pixels
	int nWidth = m_rect.Width();
	int nHeight = m_rect.Height();

	// set capture to the window which received this message
	pWnd->SetCapture();
	ASSERT(pWnd == CWnd::GetCapture());
	pWnd->UpdateWindow();
	if (pWndClipTo != NULL)
		pWndClipTo->UpdateWindow();
	CRect rectSave = m_rect;

	// find out what x/y coords we are supposed to modify
	int *px, *py;
	int xDiff, yDiff;
	GetModifyPointers(nHandle, &px, &py, &xDiff, &yDiff);
	xDiff = point.x - xDiff;
	yDiff = point.y - yDiff;

	// get DC for drawing
	CDC* pDrawDC;
	if (pWndClipTo != NULL)
	{
		// clip to arbitrary window by using adjusted Window DC
		pDrawDC = pWndClipTo->GetDCEx(NULL, DCX_CACHE);
	}
	else
	{
		// otherwise, just use normal DC
		pDrawDC = pWnd->GetDC();
	}
	ENSURE_VALID(pDrawDC);

	CRect rectOld;
	BOOL bMoved = FALSE;

	// get messages until capture lost or cancelled/accepted
	for (;;)
	{
		MSG msg;
		VERIFY(::GetMessage(&msg, NULL, 0, 0));

		if (CWnd::GetCapture() != pWnd)
			break;

		switch (msg.message)
		{
			// handle movement/accept messages
		case WM_LBUTTONUP:
			//m_pScreenCtrl->Invalidate();
			//m_pScreenCtrl->SendMessage(WM_LBUTTONUP);
		case WM_MOUSEMOVE:
			rectOld = m_rect;
			// handle resize cases (and part of move)
			if (px != NULL)
				*px = GET_X_LPARAM(msg.lParam) - xDiff;
			if (py != NULL)
				*py = GET_Y_LPARAM(msg.lParam) - yDiff;

			// handle move case
			if (nHandle == hitMiddle)
			{
				m_rect.right = m_rect.left + nWidth;
				m_rect.bottom = m_rect.top + nHeight;
			}
			// allow caller to adjust the rectangle if necessary
			AdjustRect(nHandle, &m_rect);
			//m_pScreenCtrl->SendMessage(WM_MOUSEMOVE);
			//m_pScreenCtrl->m_bIsRefresh = TRUE;
			m_bIsRefresh = TRUE;
			if(m_nTrackerType == TRACKER_TYPE_SCREEN)
				m_pScreenCtrl->Invalidate();
			//OnChangedRect(/*rectOld*/m_rect);
			//ReleaseCapture();
			// only redraw and callback if the rect actually changed!
			m_bFinalErase = (msg.message == WM_LBUTTONUP);
			if (!rectOld.EqualRect(&m_rect) || m_bFinalErase)
			{
				if (bMoved)
				{
					m_bErase = TRUE;
					DrawTrackerRect(rectOld/*&m_rect*/, pWndClipTo, pDrawDC, pWnd);
					if(m_nTrackerType == TRACKER_TYPE_SCREEN)
						m_pScreenCtrl->Invalidate();
				}
				OnChangedRect(/*rectOld*/m_rect);
				if (msg.message != WM_LBUTTONUP)
				{
					bMoved = TRUE;
				}
			}
			if (m_bFinalErase)
			{
				//m_pScreenCtrl->m_bIsRefresh = FALSE;
				//m_bIsRefresh = FALSE;
				goto ExitLoop;
			}

			if (!rectOld.EqualRect(&m_rect))
			{
				m_bErase = FALSE;
				DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
			break;

			// handle cancel messages
		case WM_KEYDOWN:
			if (msg.wParam != VK_ESCAPE)
				break;
		case WM_RBUTTONDOWN:
			if (bMoved)
			{
				m_bErase = m_bFinalErase = TRUE;
				DrawTrackerRect(&m_rect, pWndClipTo, pDrawDC, pWnd);
			}
			m_rect = rectSave;
			goto ExitLoop;

			// just dispatch rest of the messages
		default:
			DispatchMessage(&msg);
			break;
		}
	}

ExitLoop:
	if (pWndClipTo != NULL)
		pWndClipTo->ReleaseDC(pDrawDC);
	else
		pWnd->ReleaseDC(pDrawDC);
	ReleaseCapture();

	AfxUnlockTempMaps(FALSE);

	// restore rect in case bMoved is still FALSE
	if (!bMoved)
		m_rect = rectSave;
	m_bFinalErase = FALSE;
	m_bErase = FALSE;

	// return TRUE only if rect has changed
	return !rectSave.EqualRect(&m_rect);
}

void CRectTrackerEx::DrawTrackerRect(
	LPCRECT lpRect, CWnd* pWndClipTo, CDC* pDC, CWnd* pWnd)
{
	// first, normalize the rectangle for drawing
	CRect rect = *lpRect;
	rect.NormalizeRect();

	// convert to client coordinates
	if (pWndClipTo != NULL)
	{
		pWnd->ClientToScreen(&rect);
		pWndClipTo->ScreenToClient(&rect);
	}

	CSize size(0, 0);
	if (!m_bFinalErase)
	{
		// otherwise, size depends on the style
		if (m_nStyle & hatchedBorder)
		{
			size.cx = size.cy = max(1, GetHandleSize(rect)-1);
			rect.InflateRect(size);
		}
		else
		{
			size.cx = AFX_CX_BORDER;
			size.cy = AFX_CY_BORDER;
		}
	}

	// and draw it
	if (m_bFinalErase || !m_bErase)
		//pDC->DrawDragRect(rect, size, m_rectLast, m_sizeLast);
	{
		//pDC->SelectStockObject(NULL_BRUSH);   
		//pDC->Rectangle(rect.left, rect.top, rect.right, rect.bottom);
	}
		//pDC->DrawDragRect()

	// remember last rectangles
	m_rectLast = rect;
	m_sizeLast = size;
}