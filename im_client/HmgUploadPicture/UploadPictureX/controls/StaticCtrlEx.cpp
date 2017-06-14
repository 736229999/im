// StaticCtrlEx.cpp : implementation file
//

#include "..\..\stdafx.h"
#include "StaticCtrlEx.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CStaticCtrlEx

CStaticCtrlEx::CStaticCtrlEx()
{
	m_crText = GetSysColor(COLOR_WINDOWTEXT);
	m_hBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));

	::GetObject((HFONT)GetStockObject(DEFAULT_GUI_FONT),sizeof(m_lf),&m_lf);

	m_font.CreateFontIndirect(&m_lf);
	m_bTimer = FALSE;
	m_bState = FALSE;
	m_bLink = TRUE;
	m_hCursor = NULL;
	m_Type = _SCX_FTNone;

	m_hwndBrush = ::CreateSolidBrush(GetSysColor(COLOR_3DFACE));
}

CStaticCtrlEx::~CStaticCtrlEx()
{
	m_font.DeleteObject();
	::DeleteObject(m_hBrush);
}


CStaticCtrlEx& CStaticCtrlEx::SetText(const CString& strText)
{
	SetWindowText(strText);
	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::SetTextColor(COLORREF crText)
{
	m_crText = crText;
	RedrawWindow();
	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::SetFontBold(BOOL bBold)
{	
	m_lf.lfWeight = bBold ? FW_BOLD : FW_NORMAL;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::SetFontUnderline(BOOL bSet)
{	
	m_lf.lfUnderline = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::SetFontItalic(BOOL bSet)
{
	m_lf.lfItalic = bSet;
	ReconstructFont();
	RedrawWindow();
	return *this;	
}

CStaticCtrlEx& CStaticCtrlEx::SetSunken(BOOL bSet)
{
	if (!bSet)
		ModifyStyleEx(WS_EX_STATICEDGE,0,SWP_DRAWFRAME);
	else
		ModifyStyleEx(0,WS_EX_STATICEDGE,SWP_DRAWFRAME);
		
	return *this;	
}

CStaticCtrlEx& CStaticCtrlEx::SetBorder(BOOL bSet)
{  
	if (!bSet)
		ModifyStyle(WS_BORDER,0,SWP_DRAWFRAME);
	else
		ModifyStyle(0,WS_BORDER,SWP_DRAWFRAME);
		
	return *this;	
}

CStaticCtrlEx& CStaticCtrlEx::SetFontSize(int nSize)
{
	nSize*=-1;
	m_lf.lfHeight = nSize;
	ReconstructFont();
	RedrawWindow();
	return *this;
}


CStaticCtrlEx& CStaticCtrlEx::SetBkColor(COLORREF crBkgnd)
{
	if (m_hBrush)
		::DeleteObject(m_hBrush);
	
	m_hBrush = ::CreateSolidBrush(crBkgnd);
	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::SetFontName(const CString& strFont)
{	
	strcpy(m_lf.lfFaceName,strFont);
	ReconstructFont();
	RedrawWindow();
	return *this;
}


BEGIN_MESSAGE_MAP(CStaticCtrlEx, CStatic)
	//{{AFX_MSG_MAP(CStaticCtrlEx)
		ON_WM_CTLCOLOR_REFLECT()
		ON_WM_TIMER()
		ON_WM_LBUTTONDOWN()
		ON_WM_SETCURSOR()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()

/////////////////////////////////////////////////////////////////////////////
// CStaticCtrlEx message handlers


HBRUSH CStaticCtrlEx::CtlColor(CDC* pDC, UINT nCtlColor) 
{
	// TODO: Change any attributes of the DC here
	
	// TODO: Return a non-NULL brush if the parent's handler should not be called

	pDC->SetBkMode(TRANSPARENT);

	if (CTLCOLOR_STATIC == nCtlColor)
	{
		pDC->SelectObject(&m_font);
		pDC->SetTextColor(m_crText);
		pDC->SetBkMode(TRANSPARENT);
	}


	if (m_Type == _SCX_FTBackground)
	{
		if (!m_bState)
			return m_hwndBrush;
	}

	return m_hBrush;
}


void CStaticCtrlEx::ReconstructFont()
{
	m_font.DeleteObject();
	BOOL bCreated = m_font.CreateFontIndirect(&m_lf);

	ASSERT(bCreated);
}


CStaticCtrlEx& CStaticCtrlEx::FlashText(BOOL bActivate)
{
	if (m_bTimer)
	{
		SetWindowText(m_strText);
		KillTimer(1);
	}

	if (bActivate)
	{
		GetWindowText(m_strText);
		m_bState = FALSE;
		
		m_bTimer = TRUE;
		SetTimer(1,500,NULL);
		m_Type = _SCX_FTText;
	}

	return *this;
}

CStaticCtrlEx& CStaticCtrlEx::FlashBackground(BOOL bActivate)
{

	if (m_bTimer)
		KillTimer(1);

	if (bActivate)
	{
		m_bState = FALSE;

		m_bTimer = TRUE;
		SetTimer(1,500,NULL);

		m_Type = _SCX_FTBackground;
	}

	return *this;
}


void CStaticCtrlEx::OnTimer(UINT nIDEvent) 
{
	m_bState = !m_bState;

	switch (m_Type)
	{
		case _SCX_FTText:
			if (m_bState)
				SetWindowText("");
			else
				SetWindowText(m_strText);
		break;

		case _SCX_FTBackground:
			InvalidateRect(NULL,FALSE);
			UpdateWindow();
		break;
	}
	
	CStatic::OnTimer(nIDEvent);
}

CStaticCtrlEx& CStaticCtrlEx::SetLinkAddr(const CString strLinkAddr, const BOOL bLink)
{
	m_bLink = bLink;
	m_strLinkAddr = strLinkAddr;

	if (bLink)
		ModifyStyle(0,SS_NOTIFY);
	else
		ModifyStyle(SS_NOTIFY,0);
	
	if(m_hCursor != NULL)	DestroyCursor(m_hCursor);
	m_hCursor = NULL;	// AfxGetApp() ->LoadCursor(IDC_CURSOR_POINTER);
	
	return *this;
}

void CStaticCtrlEx::OnLButtonDown(UINT nFlags, CPoint point) 
{
	if(m_strLinkAddr.GetLength() > 0)
	{
	//	HINSTANCE hInst = ShellExecute(AfxGetMainWnd() ->m_hWnd, _T("open"), (LPCTSTR) m_strLinkAddr, NULL, NULL, SW_SHOWNORMAL);
	}
	
//	GetParent() ->PostMessage(WM_COMMAND, GetDlgCtrlID());
	CStatic::OnLButtonDown(nFlags, point);
}

BOOL CStaticCtrlEx::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message) 
{
	if (m_hCursor != NULL)
	{
		::SetCursor(m_hCursor);		return TRUE;
	}

	return CStatic::OnSetCursor(pWnd, nHitTest, message);
}

CStaticCtrlEx& CStaticCtrlEx::SetLinkCursor(HCURSOR hCursor)
{
	m_hCursor = hCursor;
	return *this;
}

