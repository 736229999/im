#include "stdafx.h"
#include "..\IMClient.h"
#include ".\skinctrl.h"

BEGIN_MESSAGE_MAP(CStaticBase, CStatic)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

BOOL CStaticBase::OnEraseBkgnd(CDC *pDC)
{
	return FALSE;
}

BEGIN_MESSAGE_MAP(CTransparentLabel, CStaticBase)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CTransparentLabel::OnPaint()
{
	CPaintDC dc(this);

	int nSave=dc.SaveDC();

	dc.SetBkMode(TRANSPARENT);
	dc.SelectObject(GetFont());
	dc.SetTextColor(m_clrText);

	CSize size1 = dc.GetTextExtent(m_strStatus);
	CSize size2 = dc.GetTextExtent(m_strName);


	CRect rc;
	GetClientRect(rc);

	rc.right-=(size1.cx);

	if ( size2.cx>rc.Width() )
	{
		dc.TextOut(rc.right,rc.top,m_strStatus);
		dc.DrawText(m_strName,&rc,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);
	}
	else
	{
		dc.TextOut(0,rc.top,m_strName);
		dc.TextOut(size2.cx,rc.top,m_strStatus);
	}

	dc.RestoreDC(nSave);

	//CSize sz = dc.GetTextExtent(m_strText);

	//if ( rc.Width()>sz.cx )
	//{
	//	ClientToScreen(&rc);
	//	GetParent()->ScreenToClient(&rc);
	//	rc.right=rc.left+sz.cx+1;
	//	MoveWindow(&rc,false);
	//}
}


void CTransparentLabel::PreSubclassWindow()
{
	// TODO: 在此添加专用代码和/或调用基类

	CStaticBase::PreSubclassWindow();
	GetWindowText(m_strName);
}


BEGIN_MESSAGE_MAP(CImageLogo, CStaticBase)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CImageLogo::SetLogoText(CString strLogoText)
{
	m_strLogo=strLogoText;
	Invalidate();
}

void CImageLogo::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmpMem;
	bmpMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	CBitmap *pOldmem1 = dcMem.SelectObject(&bmpMem);

	//dcMem.StretchBlt(0,0,rc.Width(),rc.Height(),&dcTemp,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);
	//HorizStretchBlt(&dcMem,0,0,rc.Width(),&m_bitmap);
	dcMem.FillSolidRect(&rc,BACKGND_COLOR);

	CFont *pOldfont = dcMem.SelectObject(&g_font14B);

	CSize szText = dcMem.GetTextExtent(m_strLogo);

	BITMAP bi;
	m_bitmapLogo.GetBitmap(&bi);

	int iLogoX = (rc.Width()-bi.bmWidth)/2;
	int iLogoY = (rc.Height()-bi.bmHeight)/2-szText.cy-3;

	//DrawBitmap(&dcMem,iLogoX,0,&m_bitmapLogo,RGB(0,255,0));
	g_ImgFaceFrame.Draw(dcMem, iLogoX, 0,100,100);
	g_ImgIcon.Draw(dcMem, iLogoX, 0,100,100);

	dcMem.SetBkMode(TRANSPARENT);
	dcMem.SetTextColor(0xF8F8F8);
	dcMem.TextOut((rc.Width()-szText.cx)/2,bi.bmHeight+10,m_strLogo);
	
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);

    dcMem.SelectObject(pOldfont);
	dcMem.SelectObject(pOldmem1);
}


BEGIN_MESSAGE_MAP(CImageBack, CStaticBase)
	ON_WM_PAINT()
END_MESSAGE_MAP()


void CImageBack::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	HorizStretchBlt(&dc,0,0,rc.Width(),&m_bitmap);
	//dc.StretchBlt(0,0,rc.Width(),rc.Height(),&dcTemp,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);
}


BEGIN_MESSAGE_MAP(CImageMenuBtn, CStaticBase)
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

void CImageMenuBtn::OnPaint()
{
	CPaintDC dc(this);
	CRect rc;
	GetClientRect(&rc);

	DrawMenuBtn(&dc,rc,m_bMouseOver);
}

void CImageMenuBtn::DrawMenuBtn(CDC *pDC, const CRect &rcMenu, bool bBold)
{
	CDC		dcMem;
	CBitmap bmMem;

	int nCaptionHeight = 22;

	dcMem.CreateCompatibleDC(pDC);
	bmMem.CreateCompatibleBitmap(pDC,rcMenu.Width(),nCaptionHeight);
    
	int	nSaveMemDC = dcMem.SaveDC();

	dcMem.SelectObject(&bmMem);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	int nSavedTempDC = dcTemp.SaveDC();

	dcTemp.SelectObject(&m_bitmapL);
	dcMem.BitBlt(0,0,4,22,&dcTemp,0,0,SRCCOPY);

	dcTemp.SelectObject(&m_bitmapR);
	dcMem.BitBlt(rcMenu.Width()-4,0,4,22,&dcTemp,0,0,SRCCOPY);

	HorizStretchBlt(&dcMem,4,0,rcMenu.Width()-8,&m_bitmapM);

	dcTemp.SelectObject(&m_bitmapI);

	dcMem.TransparentBlt(4,4,14,14,&dcTemp,0,0,14,14,RGB(0,255,0));
	
	dcTemp.RestoreDC(nSavedTempDC);

	dcMem.SetTextAlign(TA_CENTER);
	dcMem.SetBkMode(TRANSPARENT);

	CFont font;

	font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		bBold? FW_BOLD: FW_NORMAL, // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");
	
	dcMem.SelectObject(&font);

	CString sText = "菜单";
	dcMem.TextOut( (rcMenu.Width()-20)/2+18,5,sText);

	pDC->BitBlt(rcMenu.left,rcMenu.top,rcMenu.Width(),rcMenu.Height(),
		&dcMem,0,0,SRCCOPY);

	dcMem.RestoreDC(nSaveMemDC);

}

BEGIN_MESSAGE_MAP(CImageTitle, CStaticBase)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
END_MESSAGE_MAP()

void CImageTitle::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	HorizStretchBlt(&dc,0,0,rc.Width(),&m_bitmap);
//	dc.StretchBlt(0,0,rc.Width(),rc.Height(),&dcTemp,0,0,1,70,SRCCOPY);

	//CPen     pen(PS_SOLID,1,RGB(74,123,211));

	//CPen* pOldPen = dc.SelectObject(&pen);

	//dc.MoveTo(40,52);
	//dc.LineTo(40,65);

	//dc.MoveTo(83,52);
	//dc.LineTo(83,65);

	//dc.SelectObject(pOldPen);
}

BEGIN_MESSAGE_MAP(CImageToolBtn, CStaticBase)
	ON_WM_PAINT()
END_MESSAGE_MAP()

void CImageToolBtn::OnPaint()
{
	CPaintDC dc(this);

	CRect rc;
	GetClientRect(&rc);

	CResBitmap bmpTemp;
	bmpTemp.LoadBitmap(IDB_MAIN_TOOLBTNBK_MID);
	HorizStretchBlt(&dc,0,0,rc.Width(),&bmpTemp);
}

BEGIN_MESSAGE_MAP(CSAFrame, CStatic)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
END_MESSAGE_MAP()

BOOL CSAFrame::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);
	pDC->FillSolidRect(&rc,RGB(255,255,255));
	return FALSE;
}

void CSAFrame::SetWindowText(LPCSTR lpszString)
{
	m_strText = lpszString;
}

void CSAFrame::Invalidate(CWnd *pWnd)
{
	CRect rc;
	GetClientRect(&rc);

	CRgn rgn1;
	rgn1.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);

	CRect rc2 ;
	pWnd->GetWindowRect(rc2);
	ScreenToClient(&rc2);

	CRgn rgn2;
	rgn2.CreateRectRgn(rc2.left,rc2.top,rc2.right,rc2.bottom);

	CRgn rgn3;
	rgn3.CreateRectRgn(0,0,0,0);

	rgn3.CombineRgn(&rgn1,&rgn2,RGN_DIFF);

	this->InvalidateRgn(&rgn3);
}

void CSAFrame::OnPaint()
{
	CPaintDC dc(this);

    CRect rc;
	GetClientRect(&rc);

	try
	{
		int nSavedDC = dc.SaveDC();

		CFont *pFont = GetFont();

		if ( pFont )
		{
			dc.SelectObject( pFont );

			dc.SetBkMode(TRANSPARENT);

			dc.SetTextColor(RGB(0,84,166));	
			dc.TextOut(5,4,m_strText);

			CPen pen1(PS_DOT,1,RGB(194,194,194));
			CPen* pOldPen = dc.SelectObject(&pen1);

			dc.MoveTo(rc.left,19);
			dc.LineTo(rc.right,19);

			dc.Draw3dRect(rc,IM_LINE_COLOR,IM_LINE_COLOR);

			dc.SelectObject(pOldPen);
		}

		dc.RestoreDC(nSavedDC);
	}
	catch(...)
	{
	}
}


BEGIN_MESSAGE_MAP(CEmotionSelList, CStatic)
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_WM_TIMER()
END_MESSAGE_MAP()

#define EM_CX 30
#define EM_CY 22 

void CEmotionSelList::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bMouseDown = true;
	m_nSel = HitTest(point);

	CStatic::OnLButtonDown(nFlags, point);
}

void CEmotionSelList::OnLButtonUp(UINT nFlags, CPoint point)
{
	m_bMouseDown = false;
	CStatic::OnLButtonUp(nFlags, point);
}

void CEmotionSelList::OnMouseMove(UINT nFlags, CPoint point)
{
	int iHot = HitTest(point);

	if ( m_bOver==false )
	{
		m_bOver=true;
		SetTimer(0x01,200,NULL);
	}

	if ( iHot!=-1 && iHot!=m_nPreHot )
	{
		m_nPreHot = iHot;	
		Invalidate();
	}

	CStatic::OnMouseMove(nFlags, point);
}

int CEmotionSelList::HitTest(CPoint point)
{
	CRect rc;
	GetClientRect(rc);

	int nMaxCol = rc.Width()/EM_CX;

	int row = (point.y-1)/EM_CY;
	int col = (point.x-1)/EM_CX;

	if ( col>=nMaxCol ) 
		col=(nMaxCol-1);

	int iHot = row*nMaxCol+col;

	if ( iHot<0 || iHot>=m_pImageList->GetImageCount() ) 
		iHot=-1;

	return iHot;
}

BOOL CEmotionSelList::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CEmotionSelList::OnPaint()
{
	CPaintDC dc(this); CRect rc;
	GetClientRect(rc);

	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	CBitmap *pOld = dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(rc,0xFFFFFF);

	int nMaxCol = rc.Width()/EM_CX;

	int row = 0;
	int col = 0;

	int iCount = m_pImageList->GetImageCount();

	for ( int i=0; i<iCount; i++ )
	{
		if ( i>0 && (i%nMaxCol)==0 )
		{
			row++;
		}

		col = i%nMaxCol;

		CRect rcDraw;
		rcDraw.left = col*EM_CX;
		rcDraw.top  = row*EM_CY;
		rcDraw.right  = rcDraw.left+EM_CX;
		rcDraw.bottom = rcDraw.top+EM_CY;

		if ( i==m_nPreHot )
		{
			dcMem.FillSolidRect(rcDraw,RGB(233,244,255));
			dcMem.Draw3dRect(rcDraw,RGB(0,84,166),RGB(0,84,166));
		}

		POINT pt;
		pt.x = rcDraw.left+(EM_CX-20)/2;
		pt.y = rcDraw.top+(EM_CY-20)/2;

		m_pImageList->Draw(&dcMem,i,pt,ILD_NORMAL);
	}

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOld);
}

void CEmotionSelList::OnTimer(UINT nIDEvent)
{
	
	if ( m_bOver )
	{
		CPoint pt;
		GetCursorPos(&pt);

		CRect rc;
		GetWindowRect(rc);

		if ( rc.PtInRect(pt)==FALSE )
		{
			m_bOver   = false;
			m_nPreHot = -1;

			Invalidate();

			KillTimer(0x01);
		}
	}

	CStatic::OnTimer(nIDEvent);
}

void CImageTitle::OnLButtonDown(UINT nFlags, CPoint point)
{
//	ClientToScreen(&point);
//	GetParent()->ScreenToClient(&point);
	theApp.m_pMainWnd->PostMessage (WM_NCLBUTTONDOWN , HTCAPTION , MAKELPARAM (point.x , point. y ) );
}

void CImageMenuBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_bMouseOver==false )
	{
		m_bMouseOver=true;
		Invalidate();
		SetTimer(0x01,10,NULL);
	}

	CStaticBase::OnMouseMove(nFlags, point);
}

void CImageMenuBtn::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		CRect rc;
		GetWindowRect(&rc);

		CPoint pt;
		GetCursorPos(&pt);
		if ( rc.PtInRect(pt)==FALSE )
		{
			m_bMouseOver=false;
			Invalidate();
			KillTimer(0x01);
		}
	}

	CStaticBase::OnTimer(nIDEvent);
}
