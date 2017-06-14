// im\BmpBtn.cpp : 实现文件
//

#include "stdafx.h"
#include "buttonext.h"
#include "../../HmgSkin/HmgSkin/FlatPopupMenu.h"

// CBmpBtn
IMPLEMENT_DYNAMIC(CBmpBtn, CButtonBase)

CBmpBtn::CBmpBtn()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_bDown      = false;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent =0;

	m_clrText = RGB(255, 255, 255);

	SetBitmap(IDB_BTN1_HOT,IDB_BTN1_NORMAL,IDB_BTN1_DOWN,IDB_BTN1_DISABLE);
}

CBmpBtn::~CBmpBtn()
{
	m_imgNormal.Destroy();
}


BEGIN_MESSAGE_MAP(CBmpBtn, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CBmpBtn::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}
void CBmpBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// TODO: Add your code to draw the specified item
	CFont* pOldFont;
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed  = (lpDrawItemStruct->itemState & ODS_SELECTED) || m_bDown;
	BOOL bIsFocused  = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CBitmap *pBmpButton=NULL;

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

		if(m_imgNormal.IsNull())
		{

			TransparentBlt(pDC->m_hDC,0,0,bm.bmWidth,bm.bmHeight,
				memDC.m_hDC,0,0,bm.bmWidth,bm.bmHeight,RGB(0,255,0));
		}
		else
		{
			m_imgNormal.Draw(pDC->m_hDC, 0, 0,m_imgNormal.GetWidth(),m_imgNormal.GetHeight());
		}

		CString s;
		GetWindowText(s);
		int offset= bIsPressed? 1 : 0;

		CRect rc;
		GetWindowRect(&rc);

		pDC->SetBkMode(TRANSPARENT);
		pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(m_clrText);


		CSize sz=pDC->GetTextExtent(s);
		// 提供特殊字符的显示
		rc.left = (rc.Width()-sz.cx) / 2 + offset;
		rc.top = (rc.Height()-sz.cy) / 2;
		pDC->DrawText(s,&rc,DT_LEFT);
		memDC.SelectObject(pOldbmp);

		pDC->SelectObject(pOldFont);
	}
	
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


/////////////////////////////////////////////////////////////////////////
/// 从资源文件中加载图片
/// @param [in] pImage 图片指针
/// @param [in] nResID 资源号
/// @param [in] lpTyp 资源类型
//////////////////////////////////////////////////////////////////////////
bool LoadImageFromResource2(IN CImage* pImage,
                           IN UINT nResID, 
                           IN LPCSTR lpTyp)
{
    if ( pImage == NULL) return false;

    pImage->Destroy();

    // 查找资源
    HRSRC hRsrc = ::FindResource(AfxGetResourceHandle(), MAKEINTRESOURCE(nResID), lpTyp);
    if (hRsrc == NULL) return false;

    // 加载资源
    HGLOBAL hImgData = ::LoadResource(AfxGetResourceHandle(), hRsrc);
    if (hImgData == NULL)
    {
        ::FreeResource(hImgData);
        return false;
    }

    // 锁定内存中的指定资源
    LPVOID lpVoid    = ::LockResource(hImgData);

    LPSTREAM pStream = NULL;
    DWORD dwSize    = ::SizeofResource(AfxGetResourceHandle(), hRsrc);
    HGLOBAL hNew    = ::GlobalAlloc(GHND, dwSize);
    LPBYTE lpByte    = (LPBYTE)::GlobalLock(hNew);
    ::memcpy(lpByte, lpVoid, dwSize);

    // 解除内存中的指定资源
    ::GlobalUnlock(hNew);

    // 从指定内存创建流对象
    HRESULT ht = ::CreateStreamOnHGlobal(hNew, TRUE, &pStream);
    if ( ht != S_OK )
    {
        GlobalFree(hNew);
    }
    else
    {
        // 加载图片
        pImage->Load(pStream);
		
		if (pImage->GetBPP() == 32) //确认该图像包含Alpha通道
		{
			for (int i = 0; i < pImage->GetWidth(); i++)
			{
				for (int j = 0; j < pImage->GetHeight(); j++)
				{
					byte *pByte = (byte *)pImage->GetPixelAddress(i, j);
					pByte[0] = pByte[0] * pByte[3] / 255;
					pByte[1] = pByte[1] * pByte[3] / 255;
					pByte[2] = pByte[2] * pByte[3] / 255;
				}
			}
		}

        GlobalFree(hNew);
    }

    // 释放资源
    ::FreeResource(hImgData);

    return true;
}

void CBmpBtn::SetPng( UINT nNormal)
{
	LoadImageFromResource2(&m_imgNormal, nNormal, _T("PNG"));
	Invalidate();
	//SizeToContent();
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

	CBitmap *pBmpButton=NULL;

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



IMPLEMENT_DYNAMIC(CFlatButton, CButtonBase)
CFlatButton::CFlatButton()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_bMouseDown = FALSE;
	m_clrTransparent =RGB(0,255,0);
	m_clrText = 0;
	m_bDown      = FALSE;
	m_btnFs      = FS_NORMAL;
	m_btnAs		 = AS_LEFT;
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

	CFont* pOldFont =pDC->SelectObject(GetFont());

	CRect rc=lpDrawItemStruct->rcItem;

	int cx = rc.Width();
	int cy = rc.Height();
	int offset=0;

	int nOffSet = ( this->m_btnFs==FS_NORMAL? 0 : 1);

	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);
/*
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
*/
	int nTextLeft = 0, nTextTop = 0;

	CString s;
	GetWindowText(s);

	SIZE szText = pDC->GetTextExtent(s);
	SIZE szImage={0};

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
		
		int nBmpLeft,nBmpTop;

		if ( m_btnAs==AS_LEFT )
		{
			nTextLeft = offset+szImage.cx+(cx-szText.cx-szImage.cx)*2/3;//offset + ;
			nTextTop  = offset +(cy-szText.cy)/2;

			nBmpLeft  = offset + (cx-bi.bmWidth-szText.cx)/3;
			nBmpTop   = offset + (cy-bi.bmHeight)/2;
		}
		else
		if ( m_btnAs==AS_BOTTOM )
		{
			nTextLeft = offset + ( cx-szText.cx )/2;
			nTextTop  = offset + szImage.cy + ( cy- szText.cy-szImage.cy)*2/3;

			nBmpLeft  = offset + (cx-bi.bmWidth)/2;
			nBmpTop   = offset + (cy-bi.bmHeight-szText.cy)/3;
		}


		TransparentBlt( pDC->m_hDC,
			nBmpLeft,   // x-coord of destination upper-left corner
			nBmpTop,   // y-coord of destination upper-left corner
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

		//if ( szImage.cx>0 )
		//	nXOriginDest = offset + szImage.cx+(cx-szText.cx-szImage.cx)*2/3;
		//else
		//	nXOriginDest = offset + (cx-szText.cx)/2;

		//nYOriginDest = offset + (cy-szText.cy)/2;


	pDC->SetBkMode(TRANSPARENT);

	CSize sz=pDC->GetTextExtent(s);
	// 提供特殊字符的显示
	if ( bIsDisabled )
	{
		pDC->DrawState(CPoint(nTextLeft,nTextTop),szText,(LPCTSTR)s,DSS_DISABLED,1,0,(CBrush *)NULL);
	}
	else
	{
		pDC->SetTextColor(m_clrText);
		rc.left=nTextLeft;
		rc.top=nTextTop;
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
/*
CCheckBox::CCheckBox()
{

}

CCheckBox::~CCheckBox()
{
}

void CCheckBox::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	return;
}
*/

// CPngBtn
IMPLEMENT_DYNAMIC(CPngBtn, CButtonBase)

CPngBtn::CPngBtn()
{
	m_bTracking = FALSE;
	m_bOver = FALSE;
	m_bDown = false;
	m_ToolTip.m_hWnd = NULL;

	m_clrText = RGB(255, 255, 255);
	m_bSaveBackground = FALSE;
}

CPngBtn::~CPngBtn()
{
	m_imgNormal.Destroy();
	m_imgHover.Destroy();
	m_imgDown.Destroy();
	m_imgDisable.Destroy();
	m_bitmap.Detach();
}


BEGIN_MESSAGE_MAP(CPngBtn, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_LBUTTONDOWN()
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

void CPngBtn::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}
void CPngBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct)
{
	// TODO: Add your code to draw the specified item
	CFont* pOldFont;
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed = (lpDrawItemStruct->itemState & ODS_SELECTED) || m_bDown;
	BOOL bIsFocused = (lpDrawItemStruct->itemState & ODS_FOCUS);
	BOOL bIsDisabled = (lpDrawItemStruct->itemState & ODS_DISABLED);

	CImage *pPngImg = NULL;

	if (m_bOver && !bIsPressed)
	{
		pPngImg = &m_imgHover;
	}
	else
	{
		if (bIsDisabled)
		{
			pPngImg = &m_imgDisable;
		}
		else
		{
			if (bIsPressed)
			{
				pPngImg = &m_imgDown;
			}
			else
			{
				pPngImg = &m_imgNormal;
			}
		}
	}
		

	if (!pPngImg->IsNull())
	{
		//HGDIOBJ OldObj = pDC->SelectObject(pPngImg);
		pPngImg->Draw(pDC->m_hDC, 0, 0, pPngImg->GetWidth(), pPngImg->GetHeight());

		CString s;
		GetWindowText(s);
		int offset = bIsPressed ? 1 : 0;

		CRect rc;
		GetWindowRect(&rc);

		pDC->SetBkMode(TRANSPARENT);
		pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(m_clrText);

		CSize sz = pDC->GetTextExtent(s);
		// 提供特殊字符的显示
		rc.left = (rc.Width() - sz.cx) / 2 + offset;
		rc.top = (rc.Height() - sz.cy) / 2;
		pDC->DrawText(s, &rc, DT_LEFT);
		//pDC->SelectObject(OldObj);

		pDC->SelectObject(pOldFont);
	}

	pDC->SetBkMode(TRANSPARENT);

	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);
}

void CPngBtn::OnMouseMove(UINT nFlags, CPoint point)
{
	TRACKMOUSEEVENT tme;
	tme.cbSize = sizeof(tme);
	tme.hwndTrack = m_hWnd;
	tme.dwFlags = TME_LEAVE | TME_HOVER;
	tme.dwHoverTime = 1;
	m_bTracking = _TrackMouseEvent(&tme);

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CPngBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver = FALSE;
	m_bTracking = FALSE;

	Invalidate();

	return 1;
}

LRESULT CPngBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	if (m_bOver == false)
	{
		m_bOver = TRUE;
		Invalidate();
	}

	return 1;
}

void CPngBtn::PreSubclassWindow()
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	SizeToContent();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);
}

BOOL CPngBtn::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(rc);
	CWnd* pParent = GetParent(); 
	pParent->ScreenToClient(rc);

	if(!m_bSaveBackground)
	{
		// 截取子窗口所掩盖的父窗口区域，刷新背景
		CPaintDC dc(pParent);

		m_bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		CBitmap *oldmemBitmap = memDC.SelectObject(&m_bitmap);
		memDC.BitBlt(0, 0, rc.Width(),rc.Height(), &dc, rc.left, rc.top, SRCCOPY);  
		memDC.SelectObject(oldmemBitmap);

		m_bSaveBackground = TRUE;
		memDC.DeleteDC();
	}
	
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = memDC.SelectObject(&m_bitmap);
	pDC->BitBlt(0, 0, rc.Width(),rc.Height(), &memDC, 0, 0, SRCCOPY);  
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();
	return FALSE;
}

CSize CPngBtn::SizeToContent()
{
	CSize sz(0, 0);

	if (!m_imgNormal.IsNull() && m_hWnd)
	{
		sz.cx = m_imgNormal.GetWidth();
		sz.cy = m_imgNormal.GetHeight();

		CRect rcWnd;
		::GetWindowRect(m_hWnd, &rcWnd);

		CWnd *pParent = GetParent();
		pParent->ScreenToClient(&rcWnd);

		::MoveWindow(m_hWnd, rcWnd.left, rcWnd.top, m_imgNormal.GetWidth(), m_imgNormal.GetHeight(), FALSE);
	}

	return sz;
}

void CPngBtn::SetPng(UINT nHover, UINT nNormal, UINT nDown, UINT nDisable)
{
	LoadImageFromResource2(&m_imgHover, nHover, _T("PNG"));
	LoadImageFromResource2(&m_imgNormal, nNormal, _T("PNG"));
	LoadImageFromResource2(&m_imgDown, nDown, _T("PNG"));
	LoadImageFromResource2(&m_imgDisable, nDisable, _T("PNG"));
	Invalidate();

	SizeToContent();
}

void CPngBtn::OnLButtonDown(UINT nFlags, CPoint point)
{
	CButtonBase::OnLButtonDown(nFlags, point);
	Invalidate();
}



// CBmpBtn
IMPLEMENT_DYNAMIC(CSplitBtn, CButtonBase)

	CSplitBtn::CSplitBtn()
{
	m_bTracking  = FALSE;
	m_bOver      = FALSE;
	m_bSplitTracking  = FALSE;
	m_bSplitOver      = FALSE;
	m_ToolTip.m_hWnd = NULL;
	m_clrTransparent =0;

	m_clrText = RGB(255, 255, 255);

	SetBitmap();
	SetBitmapSplit();
	m_bPressedText = TRUE;
	m_bTogether = FALSE;
}

CSplitBtn::~CSplitBtn()
{
	if(!m_bmpHover.IsNull())
		m_bmpHover.Destroy();

	if(!m_bmpDown.IsNull())
		m_bmpDown.Destroy();

	if(!m_bmpNormal.IsNull())
		m_bmpNormal.Destroy();

	if(!m_bmpSplitHover.IsNull())
		m_bmpSplitHover.Destroy();

	if(!m_bmpSplitDown.IsNull())
		m_bmpSplitDown.Destroy();

	if(!m_bmpSplitNormal.IsNull())
		m_bmpSplitNormal.Destroy();
}


BEGIN_MESSAGE_MAP(CSplitBtn, CButtonBase)
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE, OnMouseLeave)
	ON_MESSAGE(WM_MOUSEHOVER, OnMouseHover)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()

// 判断鼠标处于文本按钮上还是下拉按钮上
BOOL CSplitBtn::PtInText(CPoint pt)
{
	BOOL bPtInText = TRUE;
	if(pt.x > m_bmpNormal.GetWidth())
	{
		bPtInText = FALSE;
	}

	return bPtInText;
}


void CSplitBtn::SetTogether(BOOL bTogether)
{
	m_bTogether = bTogether;
}


void CSplitBtn::SetTextColor(COLORREF clrText)
{
	m_clrText = clrText;
}
void CSplitBtn::DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct) 
{
	// 获取鼠标位置
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	// 判断鼠标处于文本按钮上还是下拉按钮上
	m_bPressedText = PtInText(point);

	CFont* pOldFont;
	CDC* pDC = GetDC();

	int nSavedDC = pDC->SaveDC();

	CBrush *pBrush=CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
	pDC->SelectObject(pBrush);

	BOOL bIsPressed  = ((lpDrawItemStruct->itemState & ODS_SELECTED)) && m_bPressedText;
	BOOL bIsFocused  = ((lpDrawItemStruct->itemState & ODS_FOCUS)) && m_bPressedText;
	//BOOL bIsDisabled = ((lpDrawItemStruct->itemState & ODS_DISABLED)) && bPtInText;

	BOOL bIsSplitPressed  = ((lpDrawItemStruct->itemState & ODS_SELECTED)) && !m_bPressedText;
	BOOL bIsSplitFocused  = ((lpDrawItemStruct->itemState & ODS_FOCUS)) && !m_bPressedText;
	//BOOL bIsSplitDisabled = ((lpDrawItemStruct->itemState & ODS_DISABLED)) && !bPtInText;

	CImage *pBmpButton=NULL;
	CImage *pBmpSplitButton=NULL;

	CPen pen;

	if(m_bTogether)
	{
		m_bOver = m_bOver | m_bSplitOver;
		m_bSplitOver = m_bOver;

	}

	if ( m_bOver && !bIsPressed )
	{
		pBmpButton = &m_bmpHover;
		pen.CreatePen(PS_SOLID,1,RGB(0,122,255));
	}
	else
	{
		/*if ( bIsDisabled )
		{
			pBmpButton = &m_bmpDisable;
			pen.CreatePen(PS_SOLID,1,RGB(122,122,122));
		}
		else
		{*/
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
		//}
	}

	if ( m_bSplitOver && !bIsSplitPressed )
	{
		pBmpSplitButton = &m_bmpSplitHover;
	}
	else
	{
		/*if ( bIsSplitDisabled )
		{
			pBmpSplitButton = &m_bmpSplitDisable;
		}
		else
		{*/
			if ( bIsSplitPressed )
			{
				pBmpSplitButton = &m_bmpSplitDown;
			}
			else
			{
				pBmpSplitButton = &m_bmpSplitNormal;
			}
		//}
	}

	if ( !pBmpButton->IsNull() && !pBmpSplitButton->IsNull())
	{
		pBmpButton->Draw(pDC->m_hDC, 0, 0, pBmpButton->GetWidth(), pBmpButton->GetHeight()); 
		pBmpSplitButton->Draw(pDC->m_hDC, pBmpButton->GetWidth(), 0, pBmpButton->GetHeight(), pBmpButton->GetHeight()); 

		CString s;
		GetWindowText(s);
		int offset= bIsPressed? 1 : 0;

		pDC->SetBkMode(TRANSPARENT);
		pOldFont = pDC->SelectObject(GetFont());
		pDC->SetTextColor(m_clrText);

		CSize sz=pDC->GetTextExtent(s);
		// 提供特殊字符的显示
		CRect rc;
		rc.left = 0 + offset;
		rc.right = pBmpButton->GetWidth();
		rc.top = (pBmpButton->GetHeight()-sz.cy) / 2 + offset;
		rc.bottom = rc.top + sz.cy;;
		pDC->DrawText(s,&rc,DT_CENTER);

		pDC->SelectObject(pOldFont);
	}

	pDC->RestoreDC(nSavedDC);

	ReleaseDC(pDC);
}

void CSplitBtn::OnMouseMove(UINT nFlags, CPoint point) 
{
	// 判断鼠标处于文本按钮上还是下拉按钮上
	BOOL bPtInText = PtInText(point);
	
	if(bPtInText)
	{
		m_bSplitOver      = FALSE;
		m_bSplitTracking  = FALSE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bTracking = _TrackMouseEvent(&tme);
	}
	else
	{
		m_bOver      = FALSE;
		m_bTracking  = FALSE;

		TRACKMOUSEEVENT tme;
		tme.cbSize = sizeof(tme);
		tme.hwndTrack = m_hWnd;
		tme.dwFlags = TME_LEAVE | TME_HOVER;
		tme.dwHoverTime = 1;
		m_bSplitTracking = _TrackMouseEvent(&tme);
	}	

	CButtonBase::OnMouseMove(nFlags, point);
}

LRESULT CSplitBtn::OnMouseLeave(WPARAM wParam, LPARAM lParam)
{
	m_bOver      = FALSE;
	m_bTracking  = FALSE;

	m_bSplitOver      = FALSE;
	m_bSplitTracking  = FALSE;

	Invalidate();

	return 1;
}

LRESULT CSplitBtn::OnMouseHover(WPARAM wParam, LPARAM lParam)
{
	// 获取鼠标位置
	CPoint point;
	GetCursorPos(&point);
	ScreenToClient(&point);
	// 判断鼠标处于文本按钮上还是下拉按钮上
	BOOL bPtInText = PtInText(point);

	if(bPtInText)
	{
		m_bSplitOver      = FALSE;
		m_bSplitTracking  = FALSE;

		if ( m_bOver==false )
		{
			m_bOver = TRUE;
			Invalidate();
		}
	}
	else
	{
		m_bOver      = FALSE;
		m_bTracking  = FALSE;

		if ( m_bSplitOver==false )
		{
			m_bSplitOver = TRUE;
			Invalidate();
		}
	}

	return 1;
}

void CSplitBtn::PreSubclassWindow() 
{
	// TODO: Add your specialized code here and/or call the base class
	CButtonBase::PreSubclassWindow();

	SizeToContent();

	this->EnableToolTips(TRUE);
	ModifyStyle(0, BS_OWNERDRAW);	
}

BOOL CSplitBtn::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc,0xFFFFFF);
	return FALSE;

	//return CButtonBase::OnEraseBkgnd(pDC);
	//return FALSE;
}

void CSplitBtn::SetBitmap()
{
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 

	LoadImageFromResource(&m_bmpNormal, hModule, IDB_PNG_SENDBTN_LEFT_NORMAL, _T("PNG"));
	LoadImageFromResource(&m_bmpHover, hModule, IDB_PNG_SENDBTN_LEFT_HOVER, _T("PNG"));
	LoadImageFromResource(&m_bmpDown, hModule, IDB_PNG_SENDBTN_LEFT_PRESSED, _T("PNG"));

	SizeToContent();
}


void CSplitBtn::SetBitmap(int nNormal, int nDown, int nSel)
{
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 

	LoadImageFromResource(&m_bmpNormal, hModule, nNormal, _T("PNG"));
	LoadImageFromResource(&m_bmpHover, hModule, nSel, _T("PNG"));
	LoadImageFromResource(&m_bmpDown, hModule, nDown, _T("PNG"));

	SizeToContent();
}

void CSplitBtn::SetBitmapSplit()
{
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 

	LoadImageFromResource(&m_bmpSplitNormal, hModule, IDB_PNG_SENDBTN_RIGHT_NORMAL, _T("PNG"));
	LoadImageFromResource(&m_bmpSplitHover, hModule, IDB_PNG_SENDBTN_RIGHT_HOVER, _T("PNG"));
	LoadImageFromResource(&m_bmpSplitDown, hModule, IDB_PNG_SENDBTN_RIGHT_PRESSED, _T("PNG"));
}


void CSplitBtn::SetBitmapSplit(int nNormal, int nDown, int nSel )
{
	HMODULE hModule = GetModuleHandle("Mini139_res.dll"); 

	LoadImageFromResource(&m_bmpSplitNormal, hModule, nNormal, _T("PNG"));
	LoadImageFromResource(&m_bmpSplitHover, hModule, nSel, _T("PNG"));
	LoadImageFromResource(&m_bmpSplitDown, hModule, nDown, _T("PNG"));
}

CSize CSplitBtn::SizeToContent()
{
	CSize sz(0,0);

	if ( !m_bmpNormal.IsNull() && !m_bmpSplitNormal.IsNull() && m_hWnd )
	{
		sz.cx = m_bmpNormal.GetWidth() + m_bmpNormal.GetHeight();
		sz.cy = m_bmpNormal.GetHeight();

		CRect rcWnd;
		::GetWindowRect(m_hWnd,&rcWnd);

		CWnd *pParent = GetParent();
		pParent->ScreenToClient(&rcWnd);

		::MoveWindow(m_hWnd,rcWnd.left,rcWnd.top,sz.cx,sz.cy,FALSE);
	}

	return sz;
}
