// InPlaceTextTip.cpp : implementation file
//

#include "..\stdafx.h"
#include "InPlaceTextTip.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CInPlaceTextTip

CInPlaceTextTip::CInPlaceTextTip()
{
	WNDCLASS wndcls;
	HINSTANCE hInst = AfxGetInstanceHandle();
	if(!(::GetClassInfo(hInst, TEXTTIP_CLASSNAME, &wndcls)))
	{
		wndcls.style = CS_SAVEBITS ;
		wndcls.lpfnWndProc = ::DefWindowProc;
		wndcls.cbClsExtra = wndcls.cbWndExtra = 0;
		wndcls.hInstance = hInst;
		wndcls.hIcon = NULL;
		wndcls.hCursor = LoadCursor(hInst, IDC_ARROW);
		wndcls.hbrBackground = (HBRUSH)(COLOR_INFOBK + 1); 
		wndcls.lpszMenuName = NULL;
		wndcls.lpszClassName = TEXTTIP_CLASSNAME;
		if (!AfxRegisterClass(&wndcls)) AfxThrowResourceException();
	}

}

CInPlaceTextTip::~CInPlaceTextTip()
{
}


BEGIN_MESSAGE_MAP(CInPlaceTextTip, CWnd)
	//{{AFX_MSG_MAP(CInPlaceTextTip)
	ON_WM_MOUSEMOVE()
	//}}AFX_MSG_MAP
END_MESSAGE_MAP()


/////////////////////////////////////////////////////////////////////////////
// CInPlaceTextTip message handlers

void CInPlaceTextTip::ShowTip(CRect &rectTitle, LPCTSTR lpszTitleText, int xOffset)
{
	if (!::IsWindow( m_hWnd )) return	;
	if (rectTitle.IsRectEmpty()) return	;

	ASSERT(!rectTitle.IsRectEmpty());

	if (lpszTitleText == NULL || *lpszTitleText == 0) return;

	if (IsWindowVisible()) return	;
	if (GetFocus() == NULL) return	;

	m_rcText.top = -1;
	m_rcText.left = -xOffset-1;
	m_rcText.right = rectTitle.Width()-xOffset;
	m_rcText.bottom = rectTitle.Height()+1;
	m_pParentWnd->ClientToScreen( rectTitle );

	CClientDC dc(this);
	CString strTitle(lpszTitleText);
	CFont *pFont = m_pParentWnd->GetFont(); 	// use same font as ctrl
	CFont *pFontDC = dc.SelectObject( pFont );

	CRect rectDisplay = rectTitle;
	CSize size = dc.GetTextExtent( strTitle );

	CRect rc;
	dc.DrawText(lpszTitleText, &rc, DT_CALCRECT);

	rectDisplay.left += xOffset;
	rectDisplay.top += ((rectTitle.Width() / 2) - 10);
	rectDisplay.right = rectDisplay.left + rc.Width() + 5 + 3;
	rectDisplay.bottom = rectDisplay.top + rc.Height() + 5;

	if (rectDisplay.right <= rectTitle.right-xOffset) return;
	
	SetWindowPos(&wndTop, rectDisplay.left, rectDisplay.top, rectDisplay.Width(), rectDisplay.Height(), SWP_SHOWWINDOW|SWP_NOACTIVATE);
	
	dc.SetBkMode(TRANSPARENT);	//	dc.SetTextAlign(TA_CENTER);
	dc.TextOut(3, 0, strTitle);
	dc.SelectObject(pFontDC);

	SetCapture();
}

BOOL CInPlaceTextTip::Create(CWnd *pParent)
{
	ASSERT_VALID(pParent);

	DWORD dwStyle	= WS_BORDER | WS_POPUP; 
	DWORD dwExStyle = WS_EX_TOOLWINDOW | WS_EX_TOPMOST;

	m_pParentWnd = pParent;
	return CreateEx(dwExStyle, TEXTTIP_CLASSNAME, NULL, dwStyle, 0, 0, 0, 0, pParent ->GetSafeHwnd(), NULL, NULL);
}

void CInPlaceTextTip::OnMouseMove(UINT nFlags, CPoint point) 
{
	 if (!m_rcText.PtInRect(point)) 
	 {
		  ReleaseCapture();
		  ShowWindow( SW_HIDE );

		  ClientToScreen(&point);
		  CWnd *pWnd = WindowFromPoint(point);
		  if (pWnd == this) pWnd = m_pParentWnd;
		  int hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0,MAKELONG(point.x,point.y));
		  if (hittest == HTCLIENT) 
		  {
			   pWnd->ScreenToClient( &point );
			   pWnd->PostMessage(WM_MOUSEMOVE, nFlags, MAKELONG(point.x,point.y));
		  } 
		  else 
		  {
			  pWnd->PostMessage(WM_NCMOUSEMOVE, hittest, MAKELONG(point.x,point.y));
		  }
	 }
}

BOOL CInPlaceTextTip::PreTranslateMessage(MSG* pMsg) 
{
	CWnd *pWnd	= NULL;
	int hittest	= 0;

	switch(pMsg->message)
	{
	case WM_LBUTTONDOWN:
	case WM_RBUTTONDOWN:
	case WM_MBUTTONDOWN:
		{
			POINTS pts = MAKEPOINTS(pMsg->lParam);
			POINT  point;
			point.x = pts.x;
			point.y = pts.y;
			ClientToScreen(&point);
			pWnd = WindowFromPoint(point);
			if( pWnd == this ) pWnd = m_pParentWnd;

			ReleaseCapture();
			ShowWindow(SW_HIDE);

			hittest = (int)pWnd->SendMessage(WM_NCHITTEST, 0,MAKELONG(point.x,point.y));
			if (hittest == HTCLIENT) 
			{
				pWnd->ScreenToClient(&point);
				pMsg->lParam = MAKELONG(point.x,point.y);
			} 
			else 
			{
				switch (pMsg->message) 
				{
				case WM_LBUTTONDOWN: 
					pMsg->message	= WM_NCLBUTTONDOWN;
					pMsg ->message	= WM_LBUTTONDBLCLK;
					break;
				case WM_RBUTTONDOWN: 
					pMsg->message = WM_NCRBUTTONDOWN;
					break;
				case WM_MBUTTONDOWN: 
					pMsg->message = WM_NCMBUTTONDOWN;
					break;
				}
				pMsg->wParam = hittest;
				pMsg->lParam = MAKELONG(point.x,point.y);
			}
			
			if(pMsg ->message != WM_LBUTTONDOWN)	pMsg ->message = WM_LBUTTONDOWN;
		//	else pMsg ->message = WM_RBUTTONDOWN;	
			pWnd->PostMessage(pMsg ->message,pMsg->wParam,pMsg->lParam);

			m_pParentWnd ->SendMessage(WM_LISTCTRL_TEXTTIP_LBUTTONDOWN, 0, 0);
#if 0
			{
			/*	LV_DISPINFO dispinfo;

				dispinfo.hdr.hwndFrom = GetParent()->m_hWnd;
				dispinfo.hdr.idFrom = GetDlgCtrlID();
				dispinfo.hdr.code =	NM_CLICK;*/

			//	dispinfo.item.mask = LVIF_TEXT;
			//	dispinfo.item.iItem = m_nListCtrlItem;
			//	dispinfo.item.iSubItem = m_nListCtrlISubtem;
			//	dispinfo.item.pszText = m_bESC ? LPTSTR((LPCTSTR)m_strInitText) : LPTSTR((LPCTSTR)strText);
			//	dispinfo.item.cchTextMax = m_bESC ? m_strInitText.GetLength() : strText.GetLength();

				// 是否已改变了InitText
			//	dispinfo.item.iImage = (m_strInitText.Compare(strText) != 0 ? 1 : 0);
			//	dispinfo.item.lParam =  (LPARAM)((LPTSTR)((LPCTSTR) m_strInitText));

			//	m_pParentWnd->SendMessage(WM_NOTIFY, GetParent()->GetDlgCtrlID(), (LPARAM)&dispinfo); 
				
			}
#endif
		}
		return TRUE;	
	case WM_LBUTTONDBLCLK:
		ReleaseCapture();
		ShowWindow(SW_HIDE);
		pWnd ->PostMessage(pMsg ->message, pMsg ->wParam, pMsg ->lParam);
		return TRUE;
	case WM_KEYDOWN:
	case WM_SYSKEYDOWN:
		ReleaseCapture();
		ShowWindow(SW_HIDE);
		m_pParentWnd->PostMessage(pMsg->message, pMsg->wParam, pMsg->lParam);
		return TRUE;
	}

	if (GetFocus() == NULL)
	{
		ReleaseCapture();
		ShowWindow(SW_HIDE);		return TRUE;
	}

	
	return CWnd::PreTranslateMessage(pMsg);
}
