// GraphComboBox.cpp : implementation file
//


#include "..\..\stdafx.h"
#include "GraphComboBox.h"
#include "..\..\resource.h"
#include ".\graphcombobox.h"

#define CResBitmap CBitmap

// XTFlatComboBox.cpp : implementation of the CGraphComboBox class.
//
// This file is a part of the Xtreme Toolkit for MFC.
// ?998-2001 Codejock Software, All Rights Reserved.
//
// This source code can only be used under the terms and conditions 
// outlined in the accompanying license agreement.
//
// support@codejock.com
// http://www.codejock.com
//
//////////////////////////////////////////////////////////////////////

/////////////////////////////////////////////////////////////////////////////
// CGraphComboBox
/////////////////////////////////////////////////////////////////////////////
CGraphComboBox::CGraphComboBox()
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
	m_mask         = NONE;
	m_cxHThumb     = ::GetSystemMetrics(SM_CXHTHUMB);

//	m_bEnableEditUpdate = TRUE;
}

CGraphComboBox::~CGraphComboBox()
{
	if ( m_hBack )
		DeleteObject(m_hBack);
	m_hBack=NULL;
}

IMPLEMENT_DYNAMIC(CGraphComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CGraphComboBox, CComboBox)
	//{{AFX_MSG_MAP(CGraphComboBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	//}}AFX_MSG_MAP
	ON_WM_NCPAINT()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CGraphComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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

	return CComboBox::OnSetCursor(pWnd, nHitTest, message);
}

void CGraphComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bFlatLook) {
		m_bLBtnDown = TRUE;
	}

	CComboBox::OnLButtonDown(nFlags, point);
}

void CGraphComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bFlatLook)
	{
		m_bLBtnDown = FALSE;
		Invalidate();
	}

	CComboBox::OnLButtonUp(nFlags, point);
}

void CGraphComboBox::OnTimer(UINT nIDEvent) 
{
	UNREFERENCED_PARAMETER(nIDEvent);

	if (m_bFlatLook)
	{
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
	}
	
}

BOOL CGraphComboBox::OnEraseBkgnd(CDC* pDC) 
{
	//UNREFERENCED_PARAMETER(pDC);
	//return TRUE;
	return CComboBox::OnEraseBkgnd(pDC);
}

void CGraphComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnNoFlickerPaint(&dc);
}

void CGraphComboBox::OnNoFlickerPaint(CDC* pDC)
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
	CResBitmap bitmap;
	bitmap.CreateCompatibleBitmap(pDC, rcClient.Width(), rcClient.Height());

	// Select the bitmap into the memory DC
	CBitmap* pOldBitmap = memDC.SelectObject(&bitmap);

	// Repaint the background, this takes the place of WM_ERASEBKGND.
	memDC.FillSolidRect(rcClient, RGB(255,255,255));

	// Let the window do its default painting...
	CWnd::DefWindowProc( WM_PAINT, (WPARAM)memDC.m_hDC, 0 );

	if (m_bFlatLook)
	{
		if (PointInRect() || m_bHasFocus) {
			DrawCombo(&memDC, RAISED);
		}
		else {
			DrawCombo(&memDC, NORMAL);
		}
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

void CGraphComboBox::DrawCombo(CDC* pDC, STATE eState)
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

	CResBitmap bmp;
	bmp.LoadBitmap(IDB_IM_COMBO_BUTTON);

	CRect rcBox;
	rcBox.SetRect(rcItem.right-m_cxHThumb-1,rcItem.top,rcItem.right,rcItem.bottom);
	pDC->FillSolidRect(&rcBox,RGB(255,255,255));
    
	BITMAP bi;
	bmp.GetBitmap(&bi);
	rcBox.top =  rcBox.top + ( rcBox.Height()-bi.bmHeight )/2;
	DramBmp(pDC, rcBox, &bmp);

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldBrush);
}


BOOL CGraphComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the edit control.
	if (pMsg->message == WM_KEYDOWN || pMsg->message==WM_KEYUP)
	{
		if ( m_bAutoComp )
		{
			if (pMsg->message == WM_KEYDOWN)
			{
				if (pMsg->wParam == VK_DELETE || pMsg->wParam == VK_BACK)
				{
					m_bDisableAC = TRUE;
				}
			}

			if (pMsg->message == WM_KEYUP)
			{
				if (pMsg->wParam == VK_DELETE ||
					pMsg->wParam == VK_BACK)
				{
					m_bDisableAC = FALSE;
				}
			}

			::TranslateMessage(pMsg);
			::DispatchMessage(pMsg);

			return TRUE;
		}
		else
		{
		//	if(!m_bEnableEditUpdate)		return TRUE;
		}
		
/*
		if ( m_mask==NUMBER )
		{
			if ( (pMsg->wParam!=VK_TAB) &&
				(pMsg->wParam!=VK_BACK) && 
				(pMsg->wParam!=VK_DELETE) &&
				(pMsg->wParam<'0' || pMsg->wParam>'9') )
				return TRUE;
		}
		else
		if ( m_mask==IP )
		{
			if ( (pMsg->wParam!=0xBE ) && //VK '.'
				(pMsg->wParam!=VK_TAB) &&
				(pMsg->wParam!=VK_BACK) && 
				(pMsg->wParam!=VK_DELETE) &&
				(pMsg->wParam<'0' || pMsg->wParam>'9') )
				return TRUE;
		}	*/	
	}

	return CComboBox::PreTranslateMessage(pMsg);
}

void CGraphComboBox::SetMask(MASK mask)
{
	m_mask=mask;
}

void CGraphComboBox::OnEditUpdate() 
{
	// if we are not to auto update the text, get outta here
	if (m_bAutoComp)
	{
		if (m_bDisableAC)
		{
			Default();
		}
		else
		{
			// Get the text in the edit box
			CString strItemTyped;
			GetWindowText(strItemTyped);
			int nLength = strItemTyped.GetLength();

			if (nLength >= 1)
			{
				// Currently selected range
				DWORD dwCurSel = GetEditSel();
				WORD dStart = LOWORD(dwCurSel);
				WORD dEnd   = HIWORD(dwCurSel);

				// Search for, and select in, and string in the combo box that is prefixed
				// by the text in the edit box
				if (SelectString(-1, strItemTyped) == CB_ERR)
				{
					SetWindowText(strItemTyped);     // No text selected, so restore what was there before
					if (dwCurSel != CB_ERR) {
						SetEditSel(dStart, dEnd);   //restore cursor postion
					}
				}

				// Set the text selection as the additional text that we have added
				if (dEnd < nLength && dwCurSel != CB_ERR) {
					SetEditSel(dStart, dEnd);
				}
				else {
					SetEditSel(nLength, -1);
				}
			}
		}
	}
}

void CGraphComboBox::DramBmp(CDC *pDC, RECT &rect, CBitmap *pBitmap)
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

void CGraphComboBox::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CComboBox::OnNcPaint()
}

HBRUSH CGraphComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);
	return m_hBack;
}


HBRUSH CGraphComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);
	return m_hBack;
}

void CGraphComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
//	if(!m_bEnableEditUpdate)		return ;

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CGraphComboBox::OnSetFocus(CWnd* pOldWnd)
{
	CComboBox::OnSetFocus(pOldWnd);

	if (m_bFlatLook)
	{
		m_bHasFocus = TRUE;

		if ( m_hWnd )
		{
			CDC *pDC = GetDC();

			if ( pDC )
			{
				DrawCombo(pDC, RAISED);
				ReleaseDC(pDC);
			}
		}
	}

}

void CGraphComboBox::OnKillFocus(CWnd* pNewWnd)
{
	CComboBox::OnKillFocus(pNewWnd);

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

