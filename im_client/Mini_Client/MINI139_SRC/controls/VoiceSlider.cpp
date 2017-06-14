// controls\VoiceSlider.cpp : 实现文件
//

#include "stdafx.h"
#include "..\imclient.h"
#include "VoiceSlider.h"


IMPLEMENT_DYNAMIC(CVoiceSliderCtrl, CStatic)
CVoiceSliderCtrl::CVoiceSliderCtrl()
{
	m_iMax = 100;
	m_iMin = 0;
	m_iPos = 70;

	m_bMouseDown = false;
	m_bMouseOver = false;

	m_bmpThum.LoadBitmap(IDB_VOICE_THUM);

	BITMAP bi;
	m_bmpThum.GetBitmap(&bi);

	m_nThumWidth  = bi.bmWidth;
	m_nThumHeight = bi.bmHeight;

	m_clrBackground = 0xFFFFFF;
}

CVoiceSliderCtrl::~CVoiceSliderCtrl()
{
}


BEGIN_MESSAGE_MAP(CVoiceSliderCtrl, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()



// CVoiceSliderCtrl 消息处理程序


BOOL CVoiceSliderCtrl::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CVoiceSliderCtrl::OnPaint()
{
	CPaintDC dc(this);

	Draw(&dc);
}

void CVoiceSliderCtrl::ReDraw()
{
	CDC *pDC = GetDC();
	if ( pDC )
	{
		Draw(pDC);
		ReleaseDC(pDC);
	}
}

void CVoiceSliderCtrl::Draw(CDC *pDC)
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

	int nTriangleBottom;

	nTriangleBottom = (cy-m_nThumHeight)/2+m_nThumHeight-2;

	int nTriangleX1 = 0;
	int nTriangleY1 = nTriangleBottom;

	int nTriangleX2 = cx-1;
	int nTriangleY2 = nTriangleBottom-m_nThumHeight+3;

	int nTriangleX3 = nTriangleX2;
	int nTriangleY3 = nTriangleBottom;

	CPen ps(PS_SOLID,1,RGB(91,118,152));
	dcMem.SelectObject(&ps);
	
	dcMem.MoveTo(nTriangleX1,nTriangleY1);
	dcMem.LineTo(nTriangleX2,nTriangleY2);
	dcMem.LineTo(nTriangleX3,nTriangleY3);
	dcMem.LineTo(nTriangleX1,nTriangleY1);

	//Draw thum
	CBitmap * pBitmap = &m_bmpThum;

	int iPos = m_iPos;// m_iMax - m_iPos;
	
	float fPos = (float)iPos/(m_iMax-m_iMin);

	m_rcThum.left  = (cx-m_nThumWidth)*fPos;;
	m_rcThum.top   = (cy-m_nThumHeight)/2;
	m_rcThum.right  = m_rcThum.left+m_nThumWidth;
	m_rcThum.bottom = m_rcThum.top+m_nThumHeight;

	CBrush brushLeftFill, brushRightFill;
	COLORREF clrLeftFill, clrRightFill;

	clrLeftFill  = RGB(141,220,59);
	clrRightFill = RGB(161,190,221);

	brushLeftFill.CreateSolidBrush(clrLeftFill);
	brushRightFill.CreateSolidBrush(clrRightFill);

	dcMem.SelectObject(&brushLeftFill);
	dcMem.ExtFloodFill(nTriangleX3-1,nTriangleY3-1,m_clrBackground,FLOODFILLSURFACE);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);
	CBitmap *pOldbmp = dcTemp.SelectObject(pBitmap);
	dcMem.BitBlt(m_rcThum.left,m_rcThum.top,m_nThumWidth,m_nThumHeight,&dcTemp,0,0,SRCCOPY);

	if ( m_rcThum.right<cx-1 )
	{
		dcMem.SelectObject(&brushRightFill);
		dcMem.ExtFloodFill(nTriangleX3-1,nTriangleY3-1,clrLeftFill,FLOODFILLSURFACE);
	}

	dcTemp.SelectObject(pOldbmp);

	pDC->BitBlt(0,0,cx,cy,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedMemDC);	
}

void CVoiceSliderCtrl::PreSubclassWindow()
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

void CVoiceSliderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
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

void CVoiceSliderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	::ReleaseCapture();

	if ( m_bMouseDown )
	{
		m_bMouseDown=false;
		ReDraw();
	}

	CStatic::OnLButtonUp(nFlags, point);
}

void CVoiceSliderCtrl::OnMouseMove(UINT nFlags, CPoint point)
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


int CVoiceSliderCtrl::CalculatePosition(const CPoint &point)
{
	CRect rc;
	GetClientRect(&rc);

	int iPos = 0;

	int n = (m_nThumWidth)/2;
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

		iPos = f*(m_iMax-m_iMin);	
	}

	if ( iPos>m_iMax )
		iPos = m_iMax;

	if ( iPos<m_iMin )
		iPos = m_iMin;

	//iPos = m_iMax - iPos;

	return iPos;
}

void CVoiceSliderCtrl::SetPos(int iPos)
{
	m_iPos = iPos;

	if ( m_iPos<m_iMin )
		m_iPos = m_iMin;

	if ( m_iPos>=m_iMax )
		m_iPos=m_iMax-1;

	if ( m_hWnd )
		Invalidate();
}

void CVoiceSliderCtrl::SetRange(int iMin, int iMax)
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

void CVoiceSliderCtrl::OnTimer(UINT nIDEvent)
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

BOOL CVoiceSliderCtrl::PreTranslateMessage(MSG* pMsg)
{
	return CStatic::PreTranslateMessage(pMsg);
}
