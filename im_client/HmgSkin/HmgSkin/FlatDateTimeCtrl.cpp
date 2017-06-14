// FlatDateTimeCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include "FlatDateTimeCtrl.h"
#include ".\flatdatetimectrl.h"
#include "skinbitmap.h"

// CFlatDateTimeCtrl
IMPLEMENT_DYNAMIC(CFlatDateTimeCtrl, CDateTimeCtrl)
CFlatDateTimeCtrl::CFlatDateTimeCtrl()
{
	m_bDisableAC   = FALSE;
	m_bFlatLook    = TRUE;
	m_bLBtnDown    = FALSE;
	m_bPainted     = FALSE;
	m_bHasFocus    = FALSE;
	m_bAutoComp    = FALSE;
	m_clrBorder    = RGB(107,133,206);
	m_clrBack	   = RGB(255,255,255);
	m_clrText      = RGB(0,0,0);
	m_hBack        = CreateSolidBrush(m_clrBack);
	m_cxHThumb     = ::GetSystemMetrics(SM_CXHTHUMB);

}

CFlatDateTimeCtrl::~CFlatDateTimeCtrl()
{
	if ( m_hBack )
		DeleteObject(m_hBack);
	m_hBack=NULL;
}


BEGIN_MESSAGE_MAP(CFlatDateTimeCtrl, CDateTimeCtrl)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_WM_CTLCOLOR()
	ON_WM_DRAWITEM()
	ON_WM_PAINT()
	ON_WM_NCPAINT()
	ON_WM_LBUTTONUP()
	ON_WM_SETCURSOR()
	ON_WM_TIMER()
	ON_WM_LBUTTONDOWN()
	ON_WM_KILLFOCUS()
	ON_NOTIFY_REFLECT(DTN_DROPDOWN, OnDtnDropdown)
END_MESSAGE_MAP()



// CFlatDateTimeCtrl 消息处理程序


BOOL CFlatDateTimeCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//CRect rcClient; 
	//GetClientRect(rcClient); 
	//HBRUSH hbr = ::CreateSolidBrush(RGB(210, 240, 0)); 
	//::FillRect(pDC->m_hDC, &rcClient, hbr); 
	//::DeleteObject(hbr); 
	//return TRUE; 

	return CDateTimeCtrl::OnEraseBkgnd(pDC);
}

HBRUSH CFlatDateTimeCtrl::CtlColor(CDC* /*pDC*/, UINT /*nCtlColor*/)
{
	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果不应调用父级的处理程序，则返回非空画笔
	return NULL;
}

HBRUSH CFlatDateTimeCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDateTimeCtrl::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何属性

	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}

void CFlatDateTimeCtrl::OnNoFlickerPaint(CDC* pDC)
{
	ASSERT(pDC != NULL);

	// Get the client area to paint.
	CRect rcClient;
	GetClientRect(&rcClient);

	CDC memDC; // memory device context.

	// Create the memory DC, set Map Mode
	memDC.CreateCompatibleDC(pDC);
	memDC.SetMapMode(pDC->GetMapMode());

	// Create a bitmap big enough to hold the window's image
	CBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

	// Select the bitmap into the memory DC
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// Repaint the background, this takes the place of WM_ERASEBKGND.
	memDC.FillSolidRect(rcClient, RGB(255,255,255));

	// Let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );


	if (PointInRect() || m_bHasFocus) {
		DrawCombo(&memDC, RAISED);
	}
	else {
		DrawCombo(&memDC, NORMAL);
	}


	// Blt it
	pDC->BitBlt(rcClient.left, rcClient.top, rcClient.Width(), rcClient.Height(),
		&memDC, rcClient.left, rcClient.top, SRCCOPY);            

	// Select the original bitmap back in
	memDC.SelectObject(pOldBitmap);

	// Clean up
	bitmap.DeleteObject();
	memDC.DeleteDC();
}

void CFlatDateTimeCtrl::DrawCombo(CDC* pDC, STATE eState)
{
	CFont* pOldFont = pDC->GetCurrentFont();
	CBrush* pOldBrush = pDC->GetCurrentBrush();
	CRect rcItem;
	GetClientRect(&rcItem);
	if (IsWindowEnabled())
	{
		pDC->Draw3dRect(&rcItem,m_clrBorder,m_clrBorder);
	}
	else
	{
		pDC->Draw3dRect(&rcItem,RGB(110,110,110),RGB(110,110,110));
	}

	rcItem.InflateRect(-1,-1);
	pDC->Draw3dRect(&rcItem,RGB(255,255,255),RGB(255,255,255));

	CSkinBitmap bmp;
	if(bmp.LoadBitmap(IDB_IM_COMBO_BUTTON))
	{
		CRect rcBox;
		rcBox.SetRect(rcItem.right-m_cxHThumb-1,rcItem.top,rcItem.right,rcItem.bottom);
		pDC->FillSolidRect(&rcBox,RGB(255,255,255));

		BITMAP bi;
		bmp.GetBitmap(&bi);
		rcBox.top =  rcBox.top + ( rcBox.Height()-bi.bmHeight )/2;
		DramBmp(pDC, rcBox, &bmp);
	}	

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);
}

void CFlatDateTimeCtrl::DramBmp(CDC *pDC, RECT &rect, CBitmap *pBitmap)
{
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap* pOld = memDC.SelectObject(pBitmap);
	CFont* pOldFont = memDC.GetCurrentFont();

	CRect rectD;
	rectD.CopyRect(&rect);
	BITMAP bmpInfo;
	pBitmap->GetBitmap(&bmpInfo);

	pDC->BitBlt(rectD.left,rectD.top,bmpInfo.bmWidth,bmpInfo.bmHeight,&memDC,0,0,SRCCOPY);

	memDC.SelectObject(pOld);
	memDC.SelectObject(pOldFont);
}


void CFlatDateTimeCtrl::OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDC *pDC = GetDC();
	OnNoFlickerPaint(pDC);
	ReleaseDC(pDC);
//	CDateTimeCtrl::OnDrawItem(nIDCtl, lpDrawItemStruct);
}

void CFlatDateTimeCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
//	CDateTimeCtrl::OnPaint();
	OnNoFlickerPaint(&dc);
}

void CFlatDateTimeCtrl::OnNcPaint()
{
	//CDateTimeCtrl::OnNcPaint();
}

void CFlatDateTimeCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	Invalidate();
	CDateTimeCtrl::OnLButtonUp(nFlags, point);
}

BOOL CFlatDateTimeCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if (message == WM_MOUSEMOVE)
	{
		CRect rcWindow;

		GetWindowRect (&rcWindow);

		if (rcWindow.PtInRect(GetCurrentMessage()->pt))
		{
			if (m_bFlatLook)
			{
				SetTimer(1,10,NULL);
				OnTimer(1);
			}
		}
	}
	return CDateTimeCtrl::OnSetCursor(pWnd, nHitTest, message);
}

void CFlatDateTimeCtrl::OnTimer(UINT nIDEvent)
{
	UNREFERENCED_PARAMETER(nIDEvent);


		CClientDC dc(this);
		CFont* pOldFont = dc.GetCurrentFont();

		// OnLButtonDown, show PRESSED.
		if (m_bLBtnDown==TRUE)
		{
			KillTimer (1);

			if (m_bPainted == TRUE)
			{
				DrawCombo(&dc, PRESSED);
				m_bPainted = FALSE;
			}
		}

		// If mouse leaves, show flat.
		else if (!PointInRect() && !m_bHasFocus)
		{
			KillTimer (1);

			if (m_bPainted == TRUE)
			{
				DrawCombo(&dc, NORMAL);
				m_bPainted = FALSE;
			}

		}

		// On mouse over, show RAISED.
		else
		{
			if (m_bPainted == TRUE)
			{
				dc.SelectObject( pOldFont);
				return;
			}
			else
			{
				m_bPainted = TRUE;
				DrawCombo(&dc, RAISED);
			}
		}

		dc.SelectObject( pOldFont);


	CDateTimeCtrl::OnTimer(nIDEvent);
}

void CFlatDateTimeCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	SetTimer(1,10,NULL);
	CDateTimeCtrl::OnLButtonDown(nFlags, point);
}

void CFlatDateTimeCtrl::OnKillFocus(CWnd* pNewWnd)
{
	CDateTimeCtrl::OnKillFocus(pNewWnd);

	if (m_bFlatLook)
	{
		m_bHasFocus = FALSE;

		if ( m_hWnd )
		{
			CClientDC dc(this);
			DrawCombo(&dc, NORMAL);    

		}
	}
}

void CFlatDateTimeCtrl::OnDtnDropdown(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	Invalidate();
	*pResult = 0;
}
