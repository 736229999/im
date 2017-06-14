// XSliderCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "resource.h"
#include ".\xsliderctrl.h"

#define THUM_WIDTH 35
#define THUM_HEIGHT 13
// CXSliderCtrl
static CXSliderCtrl * pCtrl=NULL;
static HHOOK hHook=NULL;

LRESULT	CALLBACK MouseHookProc(int nCode,WPARAM wParam,LPARAM lParam)
{
	if ( pCtrl )
	{
		CPoint pt;
		CRect  rc;
		GetCursorPos(&pt);
		pCtrl->ScreenToClient(&pt);
		pCtrl->GetClientRect(&rc);

		LPARAM lCursorPos = MAKELPARAM(pt.x,pt.y);

		if ( rc.PtInRect(pt)==FALSE )
		{
			if ( wParam == WM_MOUSEMOVE )
			{
				pCtrl->PostMessage(wParam,0,lCursorPos);
			}
			else
			if ( wParam == WM_LBUTTONUP )
			{
				pCtrl->PostMessage(wParam,0,lCursorPos);
			}
		}
	}

	return CallNextHookEx(hHook,nCode,wParam,lParam); 
}

IMPLEMENT_DYNAMIC(CXSliderCtrl, CStatic)
CXSliderCtrl::CXSliderCtrl()
{
	m_iMax = 100;
	m_iMin = 0;
	m_iPos = 0;

	m_bMouseDown = false;
	m_bMouseOver = false;

	m_bmpThumNormal.LoadBitmap(IDB_SLIDER_THUM_NORMAL);
	m_bmpThumOver.LoadBitmap(IDB_SLIDER_THUM_HOT);
	m_bmpThumDown.LoadBitmap(IDB_SLIDER_THUM_DOWN);
}

CXSliderCtrl::~CXSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CXSliderCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CXSliderCtrl 消息处理程序


BOOL CXSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CXSliderCtrl::OnPaint()
{
	CPaintDC dc(this);

	Draw(&dc);
}

void CXSliderCtrl::ReDraw()
{
	CDC *pDC = GetDC();
	if ( pDC )
	{
		Draw(pDC);
		ReleaseDC(pDC);
	}
}

void CXSliderCtrl::Draw(CDC *pDC)
{	
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(pDC,cx,cy);
	int nSavedMemDC = dcMem.SaveDC();
	dcMem.SelectObject(&bmMem);
	dcMem.FillSolidRect(0,0,cx,cy,m_clrBackground);

	CRect rcLine;
	rcLine.left   = 0;
	rcLine.right  = cx;
	rcLine.top    = (cy-4)/2;
	rcLine.bottom = rcLine.top+4;
	dcMem.FillSolidRect(&rcLine,RGB(185,213,237));
	dcMem.Draw3dRect(&rcLine,RGB(113,174,221),RGB(217,234,244));

	//Draw thum
	CBitmap * pBitmap = &m_bmpThumNormal;

	if ( m_bMouseOver )
		pBitmap = &m_bmpThumOver;

	if ( m_bMouseDown )
		pBitmap = &m_bmpThumDown;

	int  nThumWidth  = THUM_WIDTH;
	int  nThumHeight = THUM_HEIGHT;

	int iPos = m_iPos;// m_iMax - m_iPos;

	
	float fPos = (float)iPos/(m_iMax-m_iMin);

	m_rcThum.left  = (LONG)((cx-nThumWidth)*fPos);
	m_rcThum.top   = (cy-nThumHeight)/2;
	m_rcThum.right  = m_rcThum.left+THUM_WIDTH;
	m_rcThum.bottom = m_rcThum.top+THUM_HEIGHT;

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);
	CBitmap *pOldbmp = dcTemp.SelectObject(pBitmap);
	dcMem.BitBlt(m_rcThum.left,m_rcThum.top,nThumWidth,nThumHeight,&dcTemp,0,0,SRCCOPY);

	dcTemp.SelectObject(pOldbmp);

	pDC->BitBlt(0,0,cx,cy,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedMemDC);	
}

void CXSliderCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_NOTIFY, TRUE);
	
	CRect rcWnd;
	::GetWindowRect(m_hWnd,&rcWnd);

	CWnd *pParent = GetParent();
	pParent->ScreenToClient(&rcWnd);

	if ( m_hWnd )
	{
		::MoveWindow(m_hWnd,rcWnd.left,rcWnd.top,rcWnd.Width(),15,FALSE);
	}
}

void CXSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	//if ( hHook==NULL )
	//{
	//	hHook = SetWindowsHookEx(WH_MOUSE_LL, (HOOKPROC)MouseHookProc,
	//				       AfxGetInstanceHandle(), 
	//					   0);

	//	pCtrl = this;
	//}

	SetCapture();

	if ( m_rcThum.PtInRect(point) )
	{
		m_bMouseDown = true;
		SetTimer(0x02,10,NULL);
	}
	else
	{
		int iPos = CalculatePosition(point);
		if ( iPos!=m_iPos )
		{
			m_iPos = iPos;
			GetParent()->SendMessage(WM_HSCROLL,0,(LPARAM)this->m_hWnd);
			ReDraw();
		}
	}

	CStatic::OnLButtonDown(nFlags, point);
}

void CXSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();

	if ( m_bMouseDown )
	{
		m_bMouseDown=false;
		ReDraw();
	}

	if (hHook)
	{
		UnhookWindowsHookEx(hHook);

		hHook = NULL;
		pCtrl = NULL;
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CXSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_bMouseDown==false )
	{
		if ( m_bMouseOver==false )
		{
			if ( m_rcThum.PtInRect(point) )
			{
				m_bMouseOver = true;
				Invalidate();
				SetTimer(0x01,10,NULL);
			}
		}
		else
		{
			if ( m_rcThum.PtInRect(point)==FALSE )
			{
				m_bMouseOver = false;
				Invalidate();
				KillTimer(0x01);
			}
		}
	}
	else
	{
		int iPos = CalculatePosition(point);

        if ( iPos!=m_iPos )
		{
			m_iPos = iPos;
			GetParent()->SendMessage(WM_HSCROLL,0,(LPARAM)this->m_hWnd);
			ReDraw();
		}
	}

	CStatic::OnMouseMove(nFlags, point);
}


int CXSliderCtrl::CalculatePosition(const CPoint &point)
{
	CRect rc;
	GetClientRect(&rc);

	int iPos = 0;

	int n = (THUM_WIDTH)/2;
	int left = rc.left + n;
	int right = rc.right - n;
    
	if ( point.x<left ) 
		iPos=0;
	else
	if ( point.y>right ) 
		iPos=m_iMax-1;
	else
	{
		int pos = point.x-n;
		float f = (float)pos/(right-left);

		iPos = (LONG)(f*(m_iMax-m_iMin));	
	}

	if ( iPos>m_iMax )
		iPos = m_iMax;

	if ( iPos<m_iMin )
		iPos = m_iMin;

	//iPos = m_iMax - iPos;

	return iPos;
}

void CXSliderCtrl::SetPos(int iPos)
{
	m_iPos = iPos;

	if ( m_iPos<m_iMin )
		m_iPos = m_iMin;

	if ( m_iPos>=m_iMax )
		m_iPos=m_iMax-1;

	if ( m_hWnd )
		Invalidate();
}

void CXSliderCtrl::SetRange(int iMin, int iMax)
{
	m_iMin = min(iMin,iMax);
	m_iMax = max(iMin,iMax);

  	if ( m_iPos<m_iMin )
		m_iPos = m_iMin;

	if ( m_iPos>=m_iMax )
		m_iPos=m_iMax-1;

	if ( m_hWnd )
		Invalidate();  	
}

void CXSliderCtrl::OnTimer(UINT nIDEvent)
{	
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);

	if ( nIDEvent==0x01 )
	{
		if ( m_rcThum.PtInRect(point)==FALSE )
		{
			m_bMouseOver=false;
			KillTimer(0x01);
			Invalidate();
		}
	}
	else
	if ( nIDEvent==0x02 )
	{
		if ( m_rcThum.PtInRect(point)==FALSE )
		{
		//	m_bMouseDown=false;
			KillTimer(0x02);
		//	Invalidate();
		}
	}

	CStatic::OnTimer(nIDEvent);
}

BOOL CXSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CStatic::PreTranslateMessage(pMsg);
}
