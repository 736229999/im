// im\BmpBtn.cpp : 实现文件
//

#include "..\..\stdafx.h"
#include "button.h"


// CBmpBtn

#include "..\..\resource.h"
#include ".\button.h"

#pragma comment(lib,"Msimg32.lib")

// CBmpBtn
IMPLEMENT_DYNAMIC(CButtonBase, CButton)

CButtonBase::CButtonBase()
{
	m_ToolTip.m_hWnd = NULL;
	m_dwData         = 0;
}

CButtonBase::~CButtonBase()
{
}


BEGIN_MESSAGE_MAP(CButtonBase, CButton)
END_MESSAGE_MAP()

void CButtonBase::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		//m_ToolTip.Create(this);
		//m_ToolTip.Activate(FALSE);
		//m_ToolTip.SendMessage(TTM_SETMAXTIPWIDTH, 0, 400);
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;
		m_ToolTip.Activate( TRUE ) ;
	} 
}

void CButtonBase::SetToolTip(CString strTip)
{
	if (strTip == "") 
	{
		return;
	}

	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() == 0)
	{
		CRect rectBtn; 
		GetClientRect(rectBtn);
		m_ToolTip.AddTool(this, strTip, rectBtn, 1);
	} // if

	m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);
}

void CButtonBase::PreSubclassWindow()
{	
	CButton::PreSubclassWindow();
	this->EnableToolTips(TRUE);
	m_Font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");
	this->SetFont(&m_Font);
	InitToolTip();
}

BOOL CButtonBase::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (m_ToolTip.m_hWnd)
		m_ToolTip.RelayEvent(pMsg);

	return CButton::PreTranslateMessage(pMsg);
}


// CBmpBtn
IMPLEMENT_DYNAMIC(CBmpBtn, CButtonBase)

CBmpBtn::CBmpBtn()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent =0;

	SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
}

CBmpBtn::~CBmpBtn()
{
}


BEGIN_MESSAGE_MAP(CBmpBtn, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CBmpBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CFont* pOldFont;
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CResBitmap *pBmpButton=NULL;

	CPen pen;

	if ( m_bOver && !bIsPressed )
	{
		pBmpButton = &m_bmpHover;
		pen.CreatePen(PS_SOLID,1,RGB(0,122,255));
	}
	else
	if ( bIsDisabled )
	{
		pBmpButton = &m_bmpDisable;
		pen.CreatePen(PS_SOLID,1,RGB(122,122,122));
	}
	else
	if ( bIsPressed )
	{
		pBmpButton = &m_bmpDown;
		pen.CreatePen(PS_SOLID,1,RGB(122,122,122));
	}
	else
	{
		pBmpButton = &m_bmpNormal;
		pen.CreatePen(PS_SOLID,1,RGB(0,122,255));
	}

	if ( pBmpButton->GetSafeHandle() )
	{
		BITMAP bm;
		pBmpButton->GetBitmap(&bm);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldbmp = memDC.SelectObject(pBmpButton);

		TransparentBlt(pDC->m_hDC,0,0,bm.bmWidth,bm.bmHeight,
			memDC.m_hDC,0,0,bm.bmWidth,bm.bmHeight,RGB(0,255,0));

		CString s;
		GetWindowText(s);
		int offset= bIsPressed? 1 : 0;

		CRect rc;
		GetWindowRect(&rc);

		pDC->SetBkMode(TRANSPARENT);
		pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(RGB(39,135,161));

		CSize sz=pDC->GetTextExtent(s);
		// 提供特殊字符的显示
		rc.left = (rc.Width()-sz.cx) / 2 + offset;
		rc.top = (rc.Height()-sz.cy) / 2;
		pDC->DrawText(s,&rc,DT_LEFT);
		memDC.SelectObject(pOldbmp);
	}
	

	pDC->SelectObject(pOldFont);
	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);

}

void CBmpBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CBmpBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;

	Invalidate();

	return 1;
}

LRESULT CBmpBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		m_bOver = TRUE;
		Invalidate();
	}

	return 1;
}

void CBmpBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	SizeToContent();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);	
}

BOOL CBmpBtn::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CBmpBtn::SetBitmap(UINT nHover, UINT nNormal, UINT nDown, UINT nDisable)
{
	if ( m_bmpHover.GetSafeHandle() )
	{
		m_bmpHover.DeleteObject();
	}

	m_bmpHover.LoadBitmap(nHover);

	if ( m_bmpNormal.GetSafeHandle() )
	{
		m_bmpNormal.DeleteObject();
	}

	m_bmpNormal.LoadBitmap(nNormal);

	if ( m_bmpDisable.GetSafeHandle() )
	{
		m_bmpDisable.DeleteObject();
	}

	m_bmpDisable.LoadBitmap(nDisable);

	if ( m_bmpDown.GetSafeHandle() )
	{
		m_bmpDown.DeleteObject();
	}

	m_bmpDown.LoadBitmap(nDown);

	SizeToContent();
}

CSize CBmpBtn::SizeToContent()
{
	CSize sz(0,0);

	if ( m_bmpNormal.GetSafeHandle() && m_hWnd )
	{
		BITMAP bi;
		m_bmpNormal.GetBitmap(&bi);

		sz.cx = bi.bmWidth;
		sz.cy = bi.bmHeight;

		CRect rcWnd;
		::GetWindowRect(m_hWnd,&rcWnd);

		CWnd *pParent = GetParent();
		pParent->ScreenToClient(&rcWnd);

		::MoveWindow(m_hWnd,rcWnd.left,rcWnd.top,bi.bmWidth,bi.bmHeight,FALSE);
	}

	return sz;
}


// CBmpBtn
IMPLEMENT_DYNAMIC(CBmpBtn2, CButtonBase)

CBmpBtn2::CBmpBtn2()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_bCheck     = FALSE;
	m_bDown      = false;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent = 0;
}

CBmpBtn2::~CBmpBtn2()
{
}


BEGIN_MESSAGE_MAP(CBmpBtn2, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CBmpBtn2::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED) || m_bDown;
	BOOL bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CResBitmap *pBmpButton=NULL;

	CPen pen;

	if ( m_bCheck )
	{
		pBmpButton = &m_bmpSel;
		pen.CreatePen(PS_SOLID,1,RGB(122,122,122));
	}
	else
	if ( bIsPressed )
	{
		pBmpButton = &m_bmpDown;
		pen.CreatePen(PS_SOLID,1,RGB(122,122,122));
	}
	else
	{
		pBmpButton = &m_bmpNormal;
		pen.CreatePen(PS_SOLID,1,RGB(0,122,255));
	}

	if ( pBmpButton->GetSafeHandle() )
	{
		BITMAP bm;
		pBmpButton->GetBitmap(&bm);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);
		CBitmap *pOldbmp = memDC.SelectObject(pBmpButton);

		TransparentBlt(pDC->m_hDC,0,0,bm.bmWidth,bm.bmHeight,
			memDC.m_hDC,0,0,bm.bmWidth,bm.bmHeight,RGB(0,255,0));

		CString s;
		GetWindowText(s);
		int offset= bIsPressed? 1 : 0;

		CRect rc;
		GetClientRect(&rc);

		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(GetFont());
		pDC->SetTextColor(RGB(39,135,161));

		CSize sz=
		pDC->GetTextExtent(s);

		pDC->TextOut( (rc.Width()-sz.cx)/2+offset, (rc.Height()-sz.cy)/2, s);

		memDC.SelectObject(pOldbmp);
	}
	
	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);
}

void CBmpBtn2::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CBmpBtn2::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;

	Invalidate();

	return 1;
}

LRESULT CBmpBtn2::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		m_bOver = TRUE;
		Invalidate();
	}

	return 1;
}

void CBmpBtn2::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	SizeToContent();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}

BOOL CBmpBtn2::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

void CBmpBtn2::SetCheck(bool bCheck)
{
	m_bCheck = bCheck;
	Invalidate();
}

void CBmpBtn2::SetBitmap(UINT nNormal, UINT nDown, UINT nSel)
{
	if ( m_bmpNormal.GetSafeHandle() )
	{
		m_bmpNormal.DeleteObject();
	}

	m_bmpNormal.LoadBitmap(nNormal);

	if ( m_bmpDown.GetSafeHandle() )
	{
		m_bmpDown.DeleteObject();
	}

	m_bmpDown.LoadBitmap(nDown);

	if ( m_bmpSel.GetSafeHandle() )
	{
		m_bmpSel.DeleteObject();
	}

	m_bmpSel.LoadBitmap(nSel);

	SizeToContent();
}

CSize CBmpBtn2::SizeToContent()
{
	CRect rcWnd;
	::GetWindowRect(m_hWnd,&rcWnd);

	CWnd *pParent = GetParent();
	pParent->ScreenToClient(&rcWnd);

	CSize sz(0,0);

	if ( m_bmpNormal.GetSafeHandle() && m_hWnd )
	{
		BITMAP bi;
		m_bmpNormal.GetBitmap(&bi);

		sz.cx = bi.bmWidth;
		sz.cy = bi.bmHeight;

		::MoveWindow(m_hWnd,rcWnd.left,rcWnd.top,bi.bmWidth,bi.bmHeight,FALSE);
	}

	return sz;
}


#define XPBTN_BORDER_COLOR  RGB(134,178,227)

// CXpButton
IMPLEMENT_DYNAMIC(CXpButton, CButtonBase)

CXpButton::CXpButton()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent = 0;

	m_nImage = 0;
}

CXpButton::~CXpButton()
{
}


BEGIN_MESSAGE_MAP(CXpButton, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CXpButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CPen*  pOldPen		= pDC->GetCurrentPen();
	CFont* pOldFont     = pDC->SelectObject(&m_Font);

	BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED);
	BOOL bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CRect rc=lpDrawItemStruct->rcItem;

	int cx = rc.Width();
	int cy = rc.Height();

	int offset = 0 ;

	pDC->BeginPath();
	pDC->RoundRect(rc.left,rc.top,rc.right,rc.bottom,9,9);
	pDC->EndPath();

	CRgn rgnDraw;
	rgnDraw.CreateFromPath(pDC);
	pDC->SelectObject(&rgnDraw);
	
	int nXOriginDest, nYOriginDest;

	CResBitmap bmp;

	CPen pen;

	pen.CreatePen(PS_SOLID,1,XPBTN_BORDER_COLOR);

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

    if ( m_bOver )
	{
		bmp.LoadBitmap(IDB_IM_BTN_HOVER );
	}
	else
	if ( bIsDisabled )
	{
		bmp.LoadBitmap(IDB_IM_BTN_DISABLE);
	}
	else
	if ( bIsPressed )
	{
		bmp.LoadBitmap(IDB_IM_BTN_DOWN);
	}
	else
	{
		bmp.LoadBitmap(IDB_IM_BTN_NORMAL);
	}

	BITMAP bm;
	bmp.GetBitmap(&bm);

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldbmp = memDC.SelectObject(&bmp);

	pDC->StretchBlt(0,0,rc.Width(),rc.Height(),&memDC,0,0,1,20,SRCCOPY);

	pDC->SelectObject(&pen);


	CRgn rgnClient;
	rgnClient.CreateRectRgn(rc.left,rc.top,rc.right,rc.bottom);
	pDC->SelectObject(&rgnClient);
	pDC->RoundRect(rc.left,rc.top,rc.right,rc.bottom,9,9);

/*	if ( bIsFocused && (!m_bOver) )
	{
		RECT rcFocus=rc;
		rcFocus.bottom-=2;
		rcFocus.top+=2;
		rcFocus.left+=2;
		rcFocus.right-=2;

		pen.DeleteObject();
		pen.CreatePen(PS_SOLID,1,RGB(255,200,122));
		pDC->SelectObject(&pen);

		pt.x--;
		pt.y--;

		pDC->RoundRect(&rcFocus,pt);
	}*/

	memDC.SelectObject(pOldbmp);

	pDC->SetBkMode(TRANSPARENT);

	CString s;
	GetWindowText(s);

	SIZE szText = pDC->GetTextExtent(s);
	SIZE szImage = {0};

	if ( bIsPressed )
	{
		offset++;
	}

	int nCaps = 0;

	if ( m_nImage!=0 )//draw button image
	{
		CResBitmap bmpImage ;
		bmpImage.LoadBitmap(m_nImage);

		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldboj=dcMem.SelectObject(&bmpImage);

		BITMAP bi;
		bmpImage.GetBitmap(&bi);

		szImage.cx = bi.bmWidth;
		szImage.cy = bi.bmHeight;

		nCaps = (cx-bi.bmWidth-szText.cx)/3;

		pDC->TransparentBlt(nCaps+2+offset,(cy-bi.bmHeight)/2+offset,bi.bmWidth,bi.bmHeight,&dcMem,0,0,
			bi.bmWidth,bi.bmHeight,RGB(0,255,0));

		dcMem.SelectObject(pOldboj);
	}
	
	nXOriginDest = offset + szImage.cx+nCaps+(cx-szText.cx-szImage.cx-nCaps)/2;
	nYOriginDest = offset + (cy-szText.cy)/2;

	nXOriginDest+=3;


	CRect rcText(lpDrawItemStruct->rcItem);

	UINT nFormat = DT_SINGLELINE | DT_VCENTER | DT_CENTER;

#if 0
	//rcText.left = nXOriginDest+1;
	//rcText.top  = nYOriginDest;
	//rcText.right = rcText.left+szText.cx;
	//rcText.bottom = rcText.top+ szText.cy+3;
	pDC->SetTextColor(RGB(255,255,255));



	pDC->DrawText(s,rcText,nFormat);
	rcText.left=nXOriginDest+1;
	pDC->DrawText(s,rcText,nFormat);
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,nFormat);

	rcText.left=nXOriginDest;
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,nFormat);
	rcText.top=nYOriginDest+1;
	pDC->DrawText(s,rcText,nFormat);

	rcText.left=nXOriginDest-1;
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,nFormat);
	rcText.top=nYOriginDest+1;
	pDC->DrawText(s,rcText,nFormat);
	rcText.top=nYOriginDest;
	pDC->DrawText(s,rcText,nFormat);

	//pDC->TextOut(nXOriginDest+1,nYOriginDest,s);
	//pDC->TextOut(nXOriginDest+1,nYOriginDest+1,s);
	//pDC->TextOut(nXOriginDest+1,nYOriginDest-1,s);

	//pDC->TextOut(nXOriginDest,nYOriginDest-1,s);
	//pDC->TextOut(nXOriginDest,nYOriginDest+1,s);

	//pDC->TextOut(nXOriginDest-1,nYOriginDest-1,s);
	//pDC->TextOut(nXOriginDest-1,nYOriginDest+1,s);
	//pDC->TextOut(nXOriginDest-1,nYOriginDest,s);
#endif
	pDC->SetTextColor(RGB(0,84,166));

	if ( bIsDisabled )
		pDC->SetTextColor(RGB(122,122,122));

	pDC->DrawText(s,rcText, nFormat);
	

	pDC->SelectObject(pOldFont);
	pDC->SelectObject(pOldPen);
	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);
}

void CXpButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CXpButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;

	Invalidate();

	return 1;
}

LRESULT CXpButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		m_bOver = TRUE;
		Invalidate();
	}

	return 1;
}

void CXpButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}

void CXpButton::SetImageIcon(UINT nImage)
{
	m_nImage = nImage;
}

BOOL CXpButton::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// im\FlatButton.cpp : 实现文件
//

IMPLEMENT_DYNAMIC(CFlatButton, CButtonBase)
CFlatButton::CFlatButton()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_bMouseDown = FALSE;
	m_clrTransparent =RGB(0,255,0);
	m_clrText    = 0;
	m_bDown      = FALSE;
	m_btnFs      = FS_NORMAL;
	m_bLockOver  = FALSE;
}

CFlatButton::~CFlatButton()
{
}


BEGIN_MESSAGE_MAP(CFlatButton, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


void CFlatButton::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = GetDC();
	int nSavedDC = pDC->SaveDC();

	CFont* pOldFont =pDC->SelectObject(&m_Font);

	CRect rc=lpDrawItemStruct->rcItem;

	int cx = rc.Width();
	int cy = rc.Height();
	int offset=0;

	int nOffSet = ( this->m_btnFs==FS_NORMAL? 0 : 1);

	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	if ( m_bDown )
	{
		if ( m_bMouseDown )
		{
			offset = nOffSet;
		}

		if ( m_btnFs==FS_NORMAL )
		{
			pDC->Draw3dRect(&rc,GetSysColor(COLOR_3DDKSHADOW),RGB(255,255,255));
		}
        else
		{
			pDC->FillSolidRect(&rc,RGB(178,214,250));
			pDC->Draw3dRect(&rc,RGB(70,136,201),RGB(70,136,201));
		}
	}
	else
	{
		if ( m_bOver )
		{
			if ( m_btnFs==FS_NORMAL )
			{
				pDC->Draw3dRect(&rc,RGB(255,255,255),GetSysColor(COLOR_3DDKSHADOW));
			}
			else
			{
				pDC->FillSolidRect(&rc,RGB(178,214,250));
				pDC->Draw3dRect(&rc,RGB(70,136,201),RGB(70,136,201));
			}
		}

		if ( m_bMouseDown )
		{
			offset = nOffSet;
			
			if ( m_btnFs==FS_NORMAL )
				pDC->Draw3dRect(&rc,GetSysColor(COLOR_3DDKSHADOW),RGB(255,255,255));
			else
			{
				pDC->FillSolidRect(&rc,RGB(178,214,250));
				pDC->Draw3dRect(&rc,RGB(70,136,201),RGB(70,136,201));
			}
		}

		if ( bIsDisabled )
		{
			pDC->Draw3dRect(&rc,RGB(155,155,155),RGB(155,155,155));
		}
	}
	    
	int nXOriginDest, nYOriginDest;
	
	CString s;
	GetWindowText(s);

	SIZE szText = pDC->GetTextExtent(s);
	SIZE szImage={0};

	nXOriginDest = offset + (cx-szText.cx)/2;

	CBitmap *pBitmap = NULL;

	if ( bIsDisabled )
		pBitmap = &m_bmpDisable;
	else
		pBitmap = &m_bmpNormal;

	if ( pBitmap->GetSafeHandle() )
	{
		BITMAP bi;
		pBitmap->GetBitmap(&bi);

		szImage.cx = bi.bmWidth;
		szImage.cy = bi.bmHeight;

		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOld=dcMem.SelectObject(pBitmap);

		nXOriginDest = offset + (cx-bi.bmWidth-szText.cx)/3;
		nYOriginDest = offset + (cy-bi.bmHeight)/2;
		
		TransparentBlt( pDC->m_hDC,
				nXOriginDest+offset,   // x-coord of destination upper-left corner
				nYOriginDest+offset,   // y-coord of destination upper-left corner
				bi.bmWidth,     // width of destination rectangle
				bi.bmHeight,    // height of destination rectangle
				dcMem.m_hDC,         // handle to source DC
				0,    // x-coord of source upper-left corner
				0,    // y-coord of source upper-left corner
				bi.bmWidth,      // width of source rectangle
				bi.bmHeight,     // height of source rectangle
				m_clrTransparent  // color to make transparent
				);

		dcMem.SelectObject(pOld);
	}

	if ( szImage.cx>0 )
		nXOriginDest = offset + szImage.cx+(cx-szText.cx-szImage.cx)*2/3;
	else
		nXOriginDest = offset + (cx-szText.cx)/2;

	nYOriginDest = offset + (cy-szText.cy)/2;

	pDC->SetBkMode(TRANSPARENT);

	CSize sz=pDC->GetTextExtent(s);
	// 提供特殊字符的显示
	if ( bIsDisabled )
	{
		pDC->DrawState(CPoint(nXOriginDest,nYOriginDest),szText,(char *)(LPCSTR)s,DSS_DISABLED,1,0,(CBrush *)NULL);
	}
	else
	{
		pDC->SetTextColor(m_clrText);
		rc.left=nXOriginDest;
		rc.top=nYOriginDest;
		rc.right=rc.left+sz.cx;
		rc.bottom=rc.top+sz.cy;
        //pDC->TextOut(nXOriginDest,nYOriginDest,s);
		pDC->DrawText(s,&rc,DT_NOCLIP  );
	}

	pDC->SelectObject(pOldFont);
	pDC->RestoreDC(nSavedDC);
	//pDC->Detach();
	ReleaseDC(pDC);
}

void CFlatButton::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CFlatButton::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;
	m_bMouseDown = FALSE;
	m_bLockOver  = FALSE;
	
	Invalidate();

	return 1;
}

LRESULT CFlatButton::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		if ( m_bLockOver==FALSE )
			m_bOver = TRUE;

		if ( m_btnFs==FS_NORMAL)
			Invalidate();
		else
		if ( m_btnFs==FS_OFFICE )
			Invalidate();
	}

	return 1;
}

void CFlatButton::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	LOGFONT lf;
	GetFont()->GetLogFont(&lf);
	m_Font.CreateFontIndirect(&lf);

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}

void CFlatButton::SetBitmap(UINT nIDNormal, UINT nIDDisable, COLORREF clrTransparent)
{
	if ( m_bmpNormal.GetSafeHandle() )
		m_bmpNormal.DeleteObject();

	m_bmpNormal.LoadBitmap(nIDNormal);

	if ( m_bmpDisable.GetSafeHandle() )
		m_bmpDisable.DeleteObject();

	m_bmpDisable.LoadBitmap(nIDDisable);

	m_clrTransparent = clrTransparent;

	Invalidate();
}

void CFlatButton::Invalidate()
{	
	CWnd *pParent = GetParent();
	
	if ( pParent && pParent->m_hWnd )
	{
		CRect rc;
		GetClientRect(&rc);
		ClientToScreen(&rc);

		pParent->ScreenToClient(&rc);
		pParent->InvalidateRect(&rc);
	}
}

void CFlatButton::OnLButtonDown(UINT nFlags, CPoint point)
{	
    m_bMouseDown = TRUE;

	Invalidate();

	CButtonBase::OnLButtonDown(nFlags, point);
}

BOOL CFlatButton::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
	//return CButtonBase::OnEraseBkgnd(pDC);
}

void CFlatButton::OnLButtonUp(UINT nFlags, CPoint point)
{
	BOOL bPreDown=m_bMouseDown;

	m_bMouseDown = FALSE;

	if ( bPreDown )
		m_bLockOver=true;

//	m_bOver=FALSE;
	Invalidate();	

	CButtonBase::OnLButtonUp(nFlags, point);
}

void CFlatButton::SetDown(bool bDown)
{
	m_bDown = bDown;
	Invalidate();
}


//CColumBtn
#define COLUMN_BTN_WIDTH 13

IMPLEMENT_DYNAMIC(CColumnBtn, CButtonBase)

CColumnBtn::CColumnBtn()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_ToolTip.m_hWnd = NULL;

	m_bmpTopNormal.LoadBitmap(IDB_COLUMN_BTN_TOP_NORMAL);
	m_bmpTopOver.LoadBitmap(IDB_COLUMN_BTN_TOP_HOT);
	m_bmpTopDown.LoadBitmap(IDB_COLUMN_BTN_TOP_DOWN);
	m_bmpTopDisable.LoadBitmap(IDB_COLUMN_BTN_TOP_DISABLE);

	m_bmpMidNormal.LoadBitmap(IDB_COLUMN_BTN_MID_NORMAL);
	m_bmpMidOver.LoadBitmap(IDB_COLUMN_BTN_MID_HOT);
	m_bmpMidDown.LoadBitmap(IDB_COLUMN_BTN_MID_DOWN);
	m_bmpMidDisable.LoadBitmap(IDB_COLUMN_BTN_MID_DISABLE);

	m_bmpBottomNormal.LoadBitmap(IDB_COLUMN_BTN_BOTTOM_NORMAL);
	m_bmpBottomOver.LoadBitmap(IDB_COLUMN_BTN_BOTTOM_HOT);
	m_bmpBottomDown.LoadBitmap(IDB_COLUMN_BTN_BOTTOM_DOWN);
	m_bmpBottomDisable.LoadBitmap(IDB_COLUMN_BTN_BOTTOM_DISABLE);

	m_iStyle = 0;
}

CColumnBtn::~CColumnBtn()
{
}


BEGIN_MESSAGE_MAP(CColumnBtn, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


void CColumnBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED) ;
	BOOL bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CResBitmap *pBmpTop=NULL;
	CResBitmap *pBmpMid=NULL;
	CResBitmap *pBmpBottom=NULL;

	if ( bIsPressed )
	{
		pBmpTop = &m_bmpTopDown;
		pBmpMid = &m_bmpMidDown;
		pBmpBottom = &m_bmpBottomDown;
	}
	else
	if (m_bOver)
	{
		pBmpTop = &m_bmpTopOver;
		pBmpMid = &m_bmpMidOver;
		pBmpBottom = &m_bmpBottomOver;
	}
	else
	{
		pBmpTop = &m_bmpTopNormal;
		pBmpMid = &m_bmpMidNormal;
		pBmpBottom = &m_bmpBottomNormal;
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = memDC.SelectObject(pBmpTop);

	CRect rc;
	GetClientRect(&rc);

	TransparentBlt(pDC->m_hDC,
		0,0,13,7,
		memDC.m_hDC,
		0,0,13,7,
		RGB(0,255,0));

	int n = rc.Height()-14;

	memDC.SelectObject(pBmpMid);
	pDC->StretchBlt(0,7,13,n,&memDC,0,0,13,1,SRCCOPY);


	memDC.SelectObject(pBmpBottom);
	TransparentBlt(pDC->m_hDC,
		0,n+7,13,7,
		memDC.m_hDC,
		0,0,13,7,
		RGB(0,255,0));

	CResBitmap bmpArrow;

	if ( m_iStyle==0 )
		bmpArrow.LoadBitmap(IDB_COLUMN_BTN_ARROW_LEFT);
	else
		bmpArrow.LoadBitmap(IDB_COLUMN_BTN_ARROW_RIGHT);

	memDC.SelectObject(&bmpArrow);
	TransparentBlt(pDC->m_hDC,
		5,rc.Height()/2-3,3,6,
		memDC.m_hDC,
		0,0,3,6,
		RGB(0,255,0));


	memDC.SelectObject(pOldbmp);

	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);
}

void CColumnBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CColumnBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;

	Invalidate();

	return 1;
}

LRESULT CColumnBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if ( m_bOver==false )
	{
		m_bOver = TRUE;
		Invalidate();
	}

	return 1;
}

CSize CColumnBtn::SizeToContent()
{
	CRect rcWnd;
	::GetWindowRect(m_hWnd,&rcWnd);

	CWnd *pParent = GetParent();
	pParent->ScreenToClient(&rcWnd);

	CSize sz(0,0);

	if ( m_hWnd )
	{
		::MoveWindow(m_hWnd,rcWnd.left,rcWnd.top,COLUMN_BTN_WIDTH,rcWnd.Height(),FALSE);
	}

	return sz;
}

void CColumnBtn::PreSubclassWindow() 
{
	CButtonBase::PreSubclassWindow();

	SizeToContent();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}

BOOL CColumnBtn::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}


BOOL CColumnBtn::PreTranslateMessage(MSG* pMsg)
{
	if ( pMsg->message==WM_LBUTTONDOWN || pMsg->message==WM_LBUTTONUP )
	{
		CWnd *pParent = GetParent();
		CPoint pt;
		GetCursorPos(&pt);
        pParent->ScreenToClient(&pt);
		pParent->PostMessage(pMsg->message,pMsg->wParam,MAKELPARAM(pt.x,pt.y));
	}

	return CButtonBase::PreTranslateMessage(pMsg);
}
