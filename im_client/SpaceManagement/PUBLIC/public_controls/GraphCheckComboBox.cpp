// GraphComboBox.cpp : implementation file
//

#include "stdafx.h"
#include "GraphCheckComboBox.h"
#include "..\..\Mini_Client\Mini139_res\resource.h"
#include ".\graphcheckcombobox.h"


static WNDPROC m_pWndProc = 0;
static CGraphCheckComboBox *m_pComboBox = 0;

//
// The subclassed COMBOLBOX message handler
//
extern "C" LRESULT FAR PASCAL ComboBoxListBoxProc(HWND hWnd, UINT nMsg, WPARAM wParam, LPARAM lParam)
{

	switch (nMsg) {

		case WM_RBUTTONDOWN: {
			// If you want to select all/unselect all using the
			// right button, remove this ifdef. Personally, I don't really like it
#if FALSE

			if (m_pComboBox != 0) {
				INT nCount = m_pComboBox->GetCount();
				INT nSelCount = 0;

				for (INT i = 0; i < nCount; i++) {
					if (m_pComboBox->GetCheck(i))
						nSelCount++;
				}


				m_pComboBox->SelectAll(nSelCount != nCount);

				// Make sure to invalidate this window as well
				InvalidateRect(hWnd, 0, FALSE);
				m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);

			}
#endif

			break;
							 }

							 // Make the combobox always return -1 as the current selection. This
							 // causes the lpDrawItemStruct->itemID in DrawItem() to be -1
							 // when the always-visible-portion of the combo is drawn
		case LB_GETCURSEL: {
			return -1;
						   }


		case WM_CHAR: {
			if (wParam == VK_SPACE) {
				// Get the current selection
				INT nIndex = CallWindowProcA(m_pWndProc, hWnd, LB_GETCURSEL, wParam, lParam);

				CRect rcItem;
				SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LONG)(VOID *)&rcItem);
				InvalidateRect(hWnd, rcItem, FALSE);

				// Invert the check mark
				m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));

				// Notify that selection has changed
				m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);
				return 0;
			}

			break;
					  }


		case WM_LBUTTONDOWN: {

			CRect rcClient;
			GetClientRect(hWnd, rcClient);

			CPoint pt;
			pt.x = LOWORD(lParam);
			pt.y = HIWORD(lParam);


			if (PtInRect(rcClient, pt)) {
				INT nItemHeight = SendMessage(hWnd, LB_GETITEMHEIGHT, 0, 0);
				INT nTopIndex   = SendMessage(hWnd, LB_GETTOPINDEX, 0, 0);

				// Compute which index to check/uncheck
				INT nIndex = nTopIndex + pt.y / nItemHeight;

				CRect rcItem;
				SendMessage(hWnd, LB_GETITEMRECT, nIndex, (LONG)(VOID *)&rcItem);

				if (PtInRect(rcItem, pt)) {
					// Invalidate this window
					InvalidateRect(hWnd, rcItem, FALSE);
					m_pComboBox->SetCheck(nIndex, !m_pComboBox->GetCheck(nIndex));

					// Notify that selection has changed
					m_pComboBox->GetParent()->SendMessage(WM_COMMAND, MAKELONG(GetWindowLong(m_pComboBox->m_hWnd, GWL_ID), CBN_SELCHANGE), (LPARAM)m_pComboBox->m_hWnd);


				}
			}

			// Do the default handling now (such as close the popup
			// window when clicked outside)
			break;
							 }

		case WM_LBUTTONUP: {
			// Don't do anything here. This causes the combobox popup
			// windows to remain open after a selection has been made
			return 0;
						   }
	}

	return CallWindowProc(m_pWndProc, hWnd, nMsg, wParam, lParam);
}



/////////////////////////////////////////////////////////////////////////////
// CGraphCheckComboBox
/////////////////////////////////////////////////////////////////////////////
CGraphCheckComboBox::CGraphCheckComboBox()
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

	m_hListBox       = 0;
	m_bTextUpdated   = FALSE;
	m_bItemHeightSet = FALSE;

	m_cxHThumb     = ::GetSystemMetrics(SM_CXHTHUMB);
}

CGraphCheckComboBox::~CGraphCheckComboBox()
{
	if ( m_hBack )
		DeleteObject(m_hBack);
	m_hBack=NULL;
}

IMPLEMENT_DYNAMIC(CGraphCheckComboBox, CComboBox)

BEGIN_MESSAGE_MAP(CGraphCheckComboBox, CComboBox)
	//{{AFX_MSG_MAP(CGraphCheckComboBox)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_PAINT()
	ON_CONTROL_REFLECT(CBN_EDITUPDATE, OnEditUpdate)
	ON_WM_SETCURSOR()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR_REFLECT()
	ON_MESSAGE(WM_CTLCOLORLISTBOX, OnCtlColorListBox)
	ON_MESSAGE(WM_GETTEXT, OnGetText)
	ON_MESSAGE(WM_GETTEXTLENGTH, OnGetTextLength)
	ON_CONTROL_REFLECT(CBN_DROPDOWN, OnDropDown)
	//}}AFX_MSG_MAP
	ON_WM_NCPAINT()
	ON_WM_CHAR()
	ON_WM_SETFOCUS()
	ON_WM_KILLFOCUS()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()

BOOL CGraphCheckComboBox::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
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

void CGraphCheckComboBox::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if (m_bFlatLook) {
		m_bLBtnDown = TRUE;
	}

	CComboBox::OnLButtonDown(nFlags, point);
}

void CGraphCheckComboBox::OnLButtonUp(UINT nFlags, CPoint point) 
{
	if (m_bFlatLook)
	{
		m_bLBtnDown = FALSE;
		Invalidate();
	}

	CComboBox::OnLButtonUp(nFlags, point);
}

void CGraphCheckComboBox::OnTimer(UINT nIDEvent) 
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

BOOL CGraphCheckComboBox::OnEraseBkgnd(CDC* pDC) 
{
	//UNREFERENCED_PARAMETER(pDC);
	//return TRUE;
	return CComboBox::OnEraseBkgnd(pDC);
}

void CGraphCheckComboBox::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	OnNoFlickerPaint(&dc);
}

void CGraphCheckComboBox::OnNoFlickerPaint(CDC* pDC)
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

void CGraphCheckComboBox::DrawCombo(CDC* pDC, STATE eState)
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

BOOL CGraphCheckComboBox::PreTranslateMessage(MSG* pMsg) 
{
	// Make sure that the keystrokes continue to the edit control.

	if (pMsg->message == WM_KEYDOWN || pMsg->message==WM_KEYUP)
	{
		if ( m_bAutoComp )
		{
			if (pMsg->message == WM_KEYDOWN)
			{
				if (pMsg->wParam == VK_DELETE ||
					pMsg->wParam == VK_BACK)
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

void CGraphCheckComboBox::SetMask(MASK mask)
{
	m_mask=mask;
}

void CGraphCheckComboBox::OnEditUpdate() 
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

void CGraphCheckComboBox::DramBmp(CDC *pDC, RECT &rect, CBitmap *pBitmap)
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

void CGraphCheckComboBox::OnNcPaint()
{
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CComboBox::OnNcPaint()
	CClientDC dc(this);
	OnNoFlickerPaint(&dc);
}

HBRUSH CGraphCheckComboBox::CtlColor(CDC* pDC, UINT nCtlColor)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);
	return m_hBack;
}


HBRUSH CGraphCheckComboBox::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	pDC->SetTextColor(m_clrText);
	pDC->SetBkColor(m_clrBack);
	return m_hBack;
}

void CGraphCheckComboBox::OnChar(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	CComboBox::OnChar(nChar, nRepCnt, nFlags);
}

void CGraphCheckComboBox::OnSetFocus(CWnd* pOldWnd)
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

void CGraphCheckComboBox::OnKillFocus(CWnd* pNewWnd)
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



BOOL CGraphCheckComboBox::Create(DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, UINT nID)
{

	return CComboBox::Create(dwStyle, rect, pParentWnd, nID);
}


LRESULT CGraphCheckComboBox::OnCtlColorListBox(WPARAM wParam, LPARAM lParam) 
{
	// If the listbox hasn't been subclassed yet, do so...
	if (m_hListBox == 0) {
		HWND hWnd = (HWND)lParam;

		if (hWnd != 0 && hWnd != m_hWnd) {
			// Save the listbox handle
			m_hListBox = hWnd;

			// Do the subclassing
			m_pWndProc = (WNDPROC)GetWindowLong(m_hListBox, GWL_WNDPROC);
			SetWindowLong(m_hListBox, GWL_WNDPROC, (LONG)ComboBoxListBoxProc);
		}
	}


	return DefWindowProc(WM_CTLCOLORLISTBOX, wParam, lParam);
}


void CGraphCheckComboBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	HDC dc = lpDrawItemStruct->hDC;

	CRect rcBitmap = lpDrawItemStruct->rcItem;
	CRect rcText   = lpDrawItemStruct->rcItem;

	CString strText;

	// 0 - No check, 1 - Empty check, 2 - Checked
	INT nCheck = 0;

	// Check if we are drawing the static portion of the combobox
	if ((LONG)lpDrawItemStruct->itemID < 0) {
		// Make sure the m_strText member is updated
		RecalcText();

		// Get the text
		strText = m_strText;

		// Don't draw any boxes on this item
		nCheck = 0;
	}

	// Otherwise it is one of the items
	else {
		GetLBText(lpDrawItemStruct->itemID, strText);
		nCheck = 1 + (GetCheck(lpDrawItemStruct->itemID) != 0);

		TEXTMETRIC metrics;
		GetTextMetrics(dc, &metrics);

		rcBitmap.left    = 0;
		rcBitmap.right   = rcBitmap.left + metrics.tmHeight + metrics.tmExternalLeading + 6;
		rcBitmap.top    += 1;
		rcBitmap.bottom -= 1;

		rcText.left = rcBitmap.right;
	}



	if (nCheck > 0) {
		::SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		::SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));

		UINT nState = DFCS_BUTTONCHECK;

		if (nCheck > 1)
			nState |= DFCS_CHECKED;

		// Draw the checkmark using DrawFrameControl
		DrawFrameControl(dc, rcBitmap, DFC_BUTTON, nState);
	}

	if (lpDrawItemStruct->itemState & ODS_SELECTED) {
		::SetBkColor(dc, GetSysColor(COLOR_HIGHLIGHT));
		::SetTextColor(dc, GetSysColor(COLOR_HIGHLIGHTTEXT));
	}
	else {
		::SetBkColor(dc, GetSysColor(COLOR_WINDOW));
		::SetTextColor(dc, GetSysColor(COLOR_WINDOWTEXT));
	}

	// Erase and draw
	ExtTextOut(dc, 0, 0, ETO_OPAQUE, &rcText, 0, 0, 0);
	DrawText(dc, ' ' + strText, strText.GetLength() + 1, &rcText, DT_SINGLELINE|DT_VCENTER|DT_END_ELLIPSIS);

	if ((lpDrawItemStruct->itemState & (ODS_FOCUS|ODS_SELECTED)) == (ODS_FOCUS|ODS_SELECTED))
		DrawFocusRect(dc, &rcText);

}


void CGraphCheckComboBox::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct) 
{
	CClientDC dc(this);
	CFont *pFont = dc.SelectObject(GetFont());

	if (pFont != 0) {

		TEXTMETRIC metrics;
		dc.GetTextMetrics(&metrics);

		lpMeasureItemStruct->itemHeight = metrics.tmHeight + metrics.tmExternalLeading;

		// An extra height of 2 looks good I think. 
		// Otherwise the list looks a bit crowded...
		lpMeasureItemStruct->itemHeight += 2;


		// This is needed since the WM_MEASUREITEM message is sent before
		// MFC hooks everything up if used in i dialog. So adjust the
		// static portion of the combo box now
		if (!m_bItemHeightSet) {
			m_bItemHeightSet = TRUE;
			SetItemHeight(-1, lpMeasureItemStruct->itemHeight);
		}

		dc.SelectObject(pFont);
	}
}


//
// Make sure the combobox window handle is updated since
// there may be many CCheckComboBox windows active
//
void CGraphCheckComboBox::OnDropDown() 
{
	m_pComboBox = this;
	m_pComboBox->Invalidate();

}


//
// Selects/unselects all items in the list
//
void CGraphCheckComboBox::SelectAll(BOOL bCheck)
{
	INT nCount = GetCount();

	for (INT i = 0; i < nCount; i++)
		SetCheck(i, bCheck);

}


//
// By adding this message handler, we may use CWnd::GetText()
//
LRESULT CGraphCheckComboBox::OnGetText(WPARAM wParam, LPARAM lParam)
{
	// Make sure the text is updated
	RecalcText();

	if (lParam == 0)
		return 0;

	// Copy the 'fake' window text
	lstrcpyn((LPSTR)lParam, m_strText, (INT)wParam);
	return m_strText.GetLength();
}


//
// By adding this message handler, we may use CWnd::GetTextLength()
//
LRESULT CGraphCheckComboBox::OnGetTextLength(WPARAM, LPARAM)
{
	// Make sure the text is updated
	RecalcText();
	return m_strText.GetLength();
}


//
// This routine steps thru all the items and builds
// a string containing the checked items
//
void CGraphCheckComboBox::RecalcText()
{
	if (!m_bTextUpdated) {
		CString strText;

		// Get the list count
		INT nCount    = GetCount();

		// Get the list separator
		TCHAR szBuffer[10] = {0};
		GetLocaleInfo(LOCALE_USER_DEFAULT, LOCALE_SLIST, szBuffer, sizeof(szBuffer));

		CString strSeparator = szBuffer;

		// If none found, the the ';'
		if (strSeparator.GetLength() == 0)
			strSeparator = ';';

		// Trim extra spaces
		strSeparator.TrimRight();

		// And one...
		strSeparator += ' ';

		for (INT i = 0; i < nCount; i++) {

			if (GetCheck(i)) {
				CString strItem;
				GetLBText(i, strItem);

				if (!strText.IsEmpty())
					strText += strSeparator;

				strText += strItem;
			}
		}

		// Set the text
		m_strText = strText;

		m_bTextUpdated = TRUE;
	}
}

void CGraphCheckComboBox::SetCheck(INT nIndex, BOOL bFlag)
{
	//INT nResult = SetItemData(nIndex, bFlag);

	//if (nResult < 0)
	//	return nResult;
	map<int,DWORD>::iterator it=m_mapCheck.find(nIndex);
	if ( it!=m_mapCheck.end() )
		it->second=bFlag;
	else
		m_mapCheck.insert(map<int,DWORD>::value_type(nIndex,bFlag));

	// Signal that the text need updating
	m_bTextUpdated = FALSE;

	// Redraw the window
	Invalidate(FALSE);
}

BOOL CGraphCheckComboBox::GetCheck(INT nIndex)
{
	map<int,DWORD>::iterator it=m_mapCheck.find(nIndex);
	if ( it!=m_mapCheck.end() )
		return it->second;
	else
		return FALSE;	
}


void CGraphCheckComboBox::PreSubclassWindow()
{

	//// Remove the CBS_SIMPLE and CBS_DROPDOWN styles and add the one I'm designed for
	//dwStyle &= ~0xF;
	//dwStyle |= CBS_DROPDOWNLIST;

	//// Make sure to use the CBS_OWNERDRAWVARIABLE style
	//dwStyle |= CBS_OWNERDRAWVARIABLE;

	//// Use default strings. We need the itemdata to store checkmarks
	//dwStyle |= CBS_HASSTRINGS;
	CComboBox::PreSubclassWindow();
}
