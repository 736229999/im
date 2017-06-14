// im\FaceIconCtrl.cpp : 实现文件
//

#include "stdafx.h"
#include "IconCtrl.h"

// CIconCtrl

IMPLEMENT_DYNAMIC(CIconCtrl, CStatic)
CIconCtrl::CIconCtrl()
{
	m_bMouseOver = false;
	m_bMouseDown = false;

	m_bitmap     = NULL;
	m_hCursor    = ::LoadCursor(NULL,MAKEINTRESOURCE(32649));//MAKEINTRESOURCE(IDC_HAND));
}

CIconCtrl::~CIconCtrl()
{
	if ( m_bitmap )
		delete m_bitmap;


	m_bitmap= NULL;
}


BEGIN_MESSAGE_MAP(CIconCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()



// CIconCtrl 消息处理程序


void CIconCtrl::OnPaint()
{
	CPaintDC dc(this); 

	int nSavedDC = dc.SaveDC();
	
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = rcClient.Width(), cy = rcClient.Height();

	dc.SelectObject(CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH)));
	
	CPen pen(PS_SOLID,1,RGB(0,122,255));
	CPen* pOldPen = dc.SelectObject(&pen);
	
	POINT pt;
	pt.x=(rcClient.Width()-40)/2;
	pt.y=(rcClient.Height()-40)/2;

	CRgn rgn1;
	rgn1.CreateRoundRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,3,3);
	dc.SelectObject(&rgn1);

	if ( m_bitmap )
	{
		CDC dcTemp;
		dcTemp.CreateCompatibleDC(&dc);

		CBitmap * pOld=dcTemp.SelectObject(m_bitmap);			
		dc.BitBlt(0,0,cx,cy,&dcTemp,0,0,SRCCOPY);
		dcTemp.SelectObject(pOld);
	}


	CRgn rgn2;
	rgn2.CreateRectRgn(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom);
	dc.SelectObject(rgn2);
	dc.RoundRect(rcClient.left,rcClient.top,rcClient.right,rcClient.bottom,3,3);

	dc.SelectObject(pOldPen);
	dc.RestoreDC(nSavedDC);
}

void CIconCtrl::SetFaceFile(CString strFile)
{
	CRect rc;
	GetClientRect(rc);
	
	CBitmap * pBitmap = ::CBitmapFromFile(strFile,rc.Width(),rc.Height());

	if ( pBitmap )
	{	
		if ( m_bitmap )
			delete m_bitmap;

		m_bitmap=pBitmap ;

		Invalidate();
	}
}

void CIconCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_NOTIFY, TRUE);
	//SetClassLong(m_hWnd,    // window handle 
	//	GCL_HCURSOR,		// change cursor 
	//	(LONG)m_hCursor);	// new cursor 
	//SetCursor((HCURSOR)m_hCursor);
}

void CIconCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseDown=true;
	CStatic::OnLButtonDown(nFlags, point);
}

void CIconCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	m_bMouseDown=false;
//	Invalidate();
	CStatic::OnLButtonUp(nFlags, point);
}

void CIconCtrl::OnTimer(UINT nIDEvent)
{	
	if ( nIDEvent==0x01 )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CRect rc;
		GetWindowRect(&rc);

		if ( !rc.PtInRect(pt) )
		{
			m_bMouseOver=false;
			m_bMouseDown=false;
			Invalidate();
			KillTimer(0x01);
		}
	}

	CStatic::OnTimer(nIDEvent);
}

void CIconCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	/*
	if ( m_bMouseOver==false )
	{
		m_bMouseOver=true;
		Invalidate();
		SetTimer(0x01,10,NULL);
	}
	*/
	//::SetCursor(m_hCursor);
	ShowToolTip(m_sTip);
    CStatic::OnMouseMove(nFlags, point);
}

BOOL CIconCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);

	pDC->FillSolidRect(&rcClient,RGB(213,233,255));

	return FALSE;
}

void CIconCtrl::ShowToolTip(const CString& strTip)
{
	if(m_sLastTip==strTip)
		return;
	m_sLastTip=strTip;
	m_sTip=strTip;
	if (m_toolTip.m_hWnd == NULL)
	{
		m_toolTip.Create(this,TTS_ALWAYSTIP);
		// 设置出现提示需要的时间长度
		m_toolTip.SetDelayTime(TTDT_INITIAL,100);
		// 设置提示持续时间
		m_toolTip.SetDelayTime(TTDT_AUTOPOP,10000);
	}
	if (m_toolTip.GetToolCount() == 0)
	{
		CRect rc;
		GetClientRect(&rc);
		m_toolTip.AddTool(this, strTip, rc, 1001);
	}
	m_toolTip.UpdateTipText(strTip ,this,1001);

}
BOOL CIconCtrl::PreTranslateMessage(MSG* pMsg)
{
	if( pMsg->message== WM_LBUTTONDOWN ||
		pMsg->message== WM_LBUTTONUP ||
		pMsg->message== WM_MOUSEMOVE
		)
	{
		if(m_toolTip.m_hWnd!=NULL)
			m_toolTip.RelayEvent(pMsg);
	}
	return CStatic::PreTranslateMessage(pMsg);
}


BOOL CIconCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(m_hCursor);
	return FALSE;
	//return CStatic::OnSetCursor(pWnd, nHitTest, message);
}
