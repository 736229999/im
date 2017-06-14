/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 8/23/98 19:32:46
  Comments: TLabelEdit.cpp : implementation file
 ************************************/

#include "stdafx.h"
#include "TLabelEdit.h"
#include "TLabelList.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

// Name of the window class
#define wndClassName _T("TLabelEdit")
CBrush CTLabelEdit::m_brBkGnd(GetSysColor(COLOR_APPWORKSPACE));

/////////////////////////////////////////////////////////////////////////////
// CTLabelEdit

// Function name	: WindowProcEdit
// Description	    : With this, edit control can beremoved from this
// Return type		: LRESULT CALLBACK 
// Argument         : HWND hwnd
// Argument         : UINT uMsg
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CALLBACK WindowProcEdit(HWND hwnd,UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	switch (uMsg)
	{
		case WM_KILLFOCUS:
			{
				if (CTLabelEdit* pLabel = (CTLabelEdit*)GetWindowLong(hwnd, GWL_USERDATA))
					pLabel->HideEdit();
			}
		return 0;
	}
	return  CallWindowProc(CTLabelEdit::m_wndEditDefaultWndProc, hwnd,uMsg,wParam,lParam);
}

WNDPROC CTLabelEdit::m_wndEditDefaultWndProc = NULL;
CWnd* CTLabelEdit::m_pWndEdit = NULL;

// Function name	: CTLabelEdit::CTLabelEdit
// Description	    : Default constructor
// Return type		: 
// Argument         : LPCTSTR pDefaultText
CTLabelEdit::CTLabelEdit(LPCTSTR pDefaultText):m_sDefaultString(pDefaultText)
{
}

// Function name	: CTLabelEdit::~CTLabelEdit
// Description	    : virtual destructor
// Return type		: 
CTLabelEdit::~CTLabelEdit()
{
	if (m_pWndEdit && IsEditDestroyable()) delete m_pWndEdit;
	m_pWndEdit = NULL;
}

BEGIN_MESSAGE_MAP(CTLabelEdit, CWnd)
	//{{AFX_MSG_MAP(CTLabelEdit)
	ON_WM_LBUTTONDOWN()
	ON_WM_SETFOCUS()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CTLabelEdit message handlers

// Function name	: CTLabelEdit::RegClassLabelEdit
// Description	    : Call this function to register a new class window
// Return type		: BOOL 
BOOL CTLabelEdit::RegClassLabelEdit()
{
	WNDCLASS wndClass;
		wndClass.style = CS_DBLCLKS;
		wndClass.lpfnWndProc = ::DefWindowProc;
		wndClass.cbClsExtra = NULL;
		wndClass.cbWndExtra = NULL;
		wndClass.hInstance = AfxGetInstanceHandle();
		wndClass.hIcon = NULL;
		wndClass.hCursor = NULL;
		wndClass.hbrBackground = (HBRUSH)m_brBkGnd;
		wndClass.lpszMenuName = NULL;
		wndClass.lpszClassName = wndClassName;
	return AfxRegisterClass(&wndClass);
}

// Function name	: CTLabelEdit::PreSubclassWindow
// Description	    : To work this control, must be subclassed
// Return type		: void 
void CTLabelEdit::PreSubclassWindow() 
{
	CWnd::PreSubclassWindow();
}

// Function name	: CTLabelEdit::OnLButtonDown
// Description	    : Because this control is in fact a stati control, this do not have a focus never. I forced this
// Return type		: void 
// Argument         : UINT nFlags
// Argument         : CPoint point
void CTLabelEdit::OnLButtonDown(UINT nFlags, CPoint point) 
{
	SetFocus();

	CWnd::OnLButtonDown(nFlags, point);
}

// Function name	: CTLabelEdit::OnSetFocus
// Description	    : When control is focussed then edit will take the control. No?:)
// Return type		: void 
// Argument         : CWnd* pOldWnd
void CTLabelEdit::OnSetFocus(CWnd* pOldWnd) 
{
	CWnd::OnSetFocus(pOldWnd);

	ShowEdit();

}

// Function name	: CTLabelEdit::SetText
// Description	    : Put the text
// Return type		: void 
// Argument         : LPCSTR lpszCaption
void CTLabelEdit::SetText(LPCSTR lpszCaption)
{
	CString sCaption(lpszCaption);
	SetWindowText(sCaption.IsEmpty() ? (LPCTSTR)m_sDefaultString : lpszCaption);
}

// Function name	: CTLabelEdit::GetText
// Description	    : Return the text from this
// Return type		: CString 
CString CTLabelEdit::GetText()
{
	CString sCaption;
	GetWindowText(sCaption);
	sCaption = sCaption == m_sDefaultString ? _T("") : sCaption;
	return sCaption;
}

// Function name	: CTLabelEdit::OnCtlColor
// Description	    : Put a decent color in edit mode!
// Return type		: HBRUSH 
// Argument         : CDC* pDC
// Argument         : CWnd* pWnd
// Argument         : UINT nCtlColor
HBRUSH CTLabelEdit::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor) 
{
	HBRUSH hbr = CWnd::OnCtlColor(pDC, pWnd, nCtlColor);

	pDC->SetTextColor(GetSysColor(COLOR_HIGHLIGHT));
	
	// TODO: Return a different brush if the default is not desired
	return hbr;
}

// Function name	: CTLabelEdit::IsEditDestroyable
// Description	    : This function retunr TRUE because in HideEdit the edit control have to be destroyed
// Return type		: BOOL 
BOOL CTLabelEdit::IsEditDestroyable()
{
	return TRUE;
}

// Function name	: CTLabelEdit::ShowEdit
// Description	    : Create the edit control to allow editing in label control/
// Return type		: void 
void CTLabelEdit::ShowEdit()
{
	CRect rect; GetClientRect(rect);
	if (m_pWndEdit) HideEdit();
	m_pWndEdit = new CEdit();
	m_pWndEdit->CreateEx(WS_EX_CLIENTEDGE, _T("Edit"),NULL, WS_CHILD | ES_NOHIDESEL | ES_LEFT | ES_AUTOHSCROLL , rect.left, rect.top, rect.Width(), rect.Height(), m_hWnd, NULL);
	ASSERT (m_pWndEdit && ::IsWindow(m_pWndEdit->m_hWnd));
	m_pWndEdit->SetFont(GetParent()->GetFont());
	CString sCaption;
	GetWindowText(sCaption);
	m_pWndEdit->SetWindowText(sCaption);
	m_pWndEdit->ShowWindow(SW_SHOW);
	((CEdit*)m_pWndEdit)->SetSel(MAKELONG(0,-1));
	m_wndEditDefaultWndProc = (WNDPROC)SetWindowLong(m_pWndEdit->m_hWnd, GWL_WNDPROC, (long)WindowProcEdit);
	CWnd* pWnd = (CWnd*)m_pWndEdit;
	SetWindowLong(pWnd->m_hWnd, GWL_USERDATA, (long)this);
	pWnd->SetWindowPos(0,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_NOZORDER | SWP_DRAWFRAME | SWP_FRAMECHANGED | SWP_SHOWWINDOW);
	pWnd->SetFocus();
}

// Function name	: CTLabelEdit::HideEdit
// Description	    : Hide or destroy the edit control. If edit is destroyable...
// Return type		: void 
void CTLabelEdit::HideEdit()
{
	if (::IsWindow(m_hWnd))
		if (m_pWndEdit && m_wndEditDefaultWndProc)
			if (::IsWindow(m_pWndEdit->m_hWnd))
			{
				(WNDPROC)SetWindowLong(m_pWndEdit->m_hWnd, GWL_WNDPROC, (long)m_wndEditDefaultWndProc);
				m_wndEditDefaultWndProc = NULL;
				CString sCaption;
				m_pWndEdit->GetWindowText(sCaption);
				SetText(sCaption);
				if (IsEditDestroyable())
				{
					// Do not call DestroyWindow because, you have created m_pWndEdit with CreateE API function. OK?!
					::DestroyWindow(m_pWndEdit->m_hWnd);
					delete m_pWndEdit;
				}
				else
				{
					CTLabelList* pList = (CTLabelList*)this;
					pList->ShowWindow(SW_SHOW);
					pList->GetWindowControl()->ShowWindow(SW_HIDE);
				}
				m_pWndEdit = NULL;
			}
}

// Function name	: CTLabelList::ShowEdit
// Description	    : Show the list control in case CLabelList
// Return type		: void 
void CTLabelList::ShowEdit()
{
	CRect rect; GetClientRect(rect);
	CWnd* pWnd = GetWindowControl();
	if (m_pWndEdit)
		HideEdit();
	pWnd->ShowWindow(SW_SHOW);
	// For draw
	pWnd->SetWindowPos(&CWnd::wndBottom,0,0,0,0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
	pWnd->SetFocus();
	m_pWndEdit = GetEditControl();
	ASSERT (m_pWndEdit != NULL);
	SetWindowLong(m_pWndEdit->m_hWnd, GWL_USERDATA, (long)this);
	m_wndEditDefaultWndProc = (WNDPROC)SetWindowLong(m_pWndEdit->m_hWnd, GWL_WNDPROC, (long)WindowProcEdit);
	ShowWindow(SW_HIDE); // for relay LButtonDown to this control...see wndBottom
}

// Function name	: CTLabelEdit::Init
// Description	    : 
// Return type		: void 
void CTLabelEdit::Init()
{
	ASSERT (GetParent() != NULL);
	CRect rect; GetClientRect(rect);
	CString sCaption;
	GetWindowText(sCaption);
	SetText(sCaption);
}

// Function name	: CTLabelEdit::OnPaint
// Description	    : OnDraw function...
// Return type		: void 
void CTLabelEdit::OnPaint() 
{
	CPaintDC dc(this); // device context for painting
	
	CRect rect; GetClientRect(rect);
	dc.DrawEdge(rect, EDGE_RAISED, BF_RECT);
	CString sCaption; GetWindowText(sCaption);
	dc.SetBkMode(TRANSPARENT);
	CFont* pFont = dc.SelectObject(&m_font);
		dc.DrawText(sCaption, rect, DT_CENTER | DT_VCENTER| DT_SINGLELINE);
	dc.SelectObject(pFont);
}

// Function name	: CTLabelEdit::WindowProc
// Description	    : default window procedure
// Return type		: LRESULT 
// Argument         : UINT message
// Argument         : WPARAM wParam
// Argument         : LPARAM lParam
LRESULT CTLabelEdit::WindowProc(UINT message, WPARAM wParam, LPARAM lParam) 
{
	
	switch (message)
	{
		case WM_SETFONT:
			if (CFont* pFont = CFont::FromHandle((HFONT)wParam))
			{
				LOGFONT logFont;
				pFont->GetLogFont(&logFont);
				m_font.DeleteObject();
				m_font.CreateFontIndirect((const LOGFONT*)&logFont);
				Invalidate();
				break;
			}
		case WM_SETTEXT:
			{
				Invalidate();
				break;
			}
	}
	
	return CWnd::WindowProc(message, wParam, lParam);
}
