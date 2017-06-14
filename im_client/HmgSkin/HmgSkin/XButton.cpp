// im\BmpBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "xbutton.h"
#include "skinbitmap.h"
#include "resource.h"
#include "skinbitmap.h"

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
		_T("宋体"));
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



#define XPBTN_BORDER_COLOR  RGB(134,178,227)

// CXpButton
IMPLEMENT_DYNAMIC(CXpButton, CButtonBase)

CXpButton::CXpButton()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent = 0;
	m_bmpImage.m_hObject = NULL; 
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
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CPen*  pOldPen = pDC->GetCurrentPen();
	CFont* pOldFont = pDC->SelectObject(&m_Font);

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

	CSkinBitmap bmp;

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


	if ( bmp.GetSafeHandle()==NULL )
	{
		return ;
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

	if ( m_bmpImage.GetSafeHandle() )//draw button image
	{
		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);
		CBitmap *pOldboj=dcMem.SelectObject(&m_bmpImage);

		BITMAP bi;
		m_bmpImage.GetBitmap(&bi);

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


	CRect rcText;
	rcText.left = nXOriginDest+1;
	rcText.top  = nYOriginDest;
	rcText.right = rcText.left+szText.cx;
	rcText.bottom = rcText.top+ szText.cy+3;
	pDC->SetTextColor(RGB(255,255,255));

	pDC->DrawText(s,rcText,DT_SINGLELINE);
	rcText.left=nXOriginDest+1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);

	rcText.left=nXOriginDest;
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);
	rcText.top=nYOriginDest+1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);

	rcText.left=nXOriginDest-1;
	rcText.top=nYOriginDest-1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);
	rcText.top=nYOriginDest+1;
	pDC->DrawText(s,rcText,DT_SINGLELINE);
	rcText.top=nYOriginDest;
	pDC->DrawText(s,rcText,DT_SINGLELINE);

	//pDC->TextOut(nXOriginDest+1,nYOriginDest,s);
	//pDC->TextOut(nXOriginDest+1,nYOriginDest+1,s);
	//pDC->TextOut(nXOriginDest+1,nYOriginDest-1,s);

	//pDC->TextOut(nXOriginDest,nYOriginDest-1,s);
	//pDC->TextOut(nXOriginDest,nYOriginDest+1,s);

	//pDC->TextOut(nXOriginDest-1,nYOriginDest-1,s);
	//pDC->TextOut(nXOriginDest-1,nYOriginDest+1,s);
	//pDC->TextOut(nXOriginDest-1,nYOriginDest,s);

	pDC->SetTextColor(RGB(0,84,166));

	if ( bIsDisabled )
		pDC->SetTextColor(RGB(122,122,122));

	pDC->DrawText(s,rcText,DT_SINGLELINE);
	

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
	m_bTracking = (bool)_TrackMouseEvent(&tme);

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

void CXpButton::SetButtonImage(HINSTANCE hInst,UINT nImage)
{
	if ( m_bmpImage.GetSafeHandle() )
	{
		m_bmpImage.DeleteObject();
		m_bmpImage.m_hObject = NULL;
	}

	HBITMAP hBitmap = LoadBitmap(hInst,MAKEINTRESOURCE(nImage));
	if ( hBitmap )
		m_bmpImage.Attach(hBitmap);
}

BOOL CXpButton::OnEraseBkgnd(CDC* pDC)
{
	return FALSE;
}

// im\FlatButton.cpp : 实现文件
//
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

	CBitmap *pBmpTop=NULL;
	CBitmap *pBmpMid=NULL;
	CBitmap *pBmpBottom=NULL;

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
	if (bIsDisabled)
	{
		pBmpTop = &m_bmpTopDisable;
		pBmpMid = &m_bmpMidDisable;
		pBmpBottom = &m_bmpBottomDisable;
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

	CSkinBitmap bmpArrow;

	if ( m_iStyle==0 )
		bmpArrow.LoadBitmap(IDB_COLUMN_BTN_ARROW_LEFT);
	else
		bmpArrow.LoadBitmap(IDB_COLUMN_BTN_ARROW_RIGHT);

	if (bmpArrow.GetSafeHandle())
	{
		memDC.SelectObject(&bmpArrow);
		TransparentBlt(pDC->m_hDC,
			5,rc.Height()/2-3,3,6,
			memDC.m_hDC,
			0,0,3,6,
			RGB(0,255,0));
	}


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
