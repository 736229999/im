#include "stdafx.h"
#include "CheckButton.h"
#include "GdiPlus.h"
#include <string>
#include <atlconv.h>

using namespace Gdiplus;
using namespace std;

IMPLEMENT_DYNAMIC(CCheckButton, CButtonBase)

BOOL ImageFromIDResource(HINSTANCE hInst, UINT nID, LPCTSTR sTR, Image * & pImg)  
{  
	//HINSTANCE hInst = AfxGetResourceHandle();  
	HRSRC hRsrc = ::FindResource (hInst,MAKEINTRESOURCE(nID),sTR); // type  
	if (!hRsrc)  
		return FALSE;  
	// load resource into memory  
	DWORD len = SizeofResource(hInst, hRsrc);  
	BYTE* lpRsrc = (BYTE*)LoadResource(hInst, hRsrc);  
	if (!lpRsrc)  
		return FALSE;  
	// Allocate global memory on which to create stream  
	HGLOBAL m_hMem = GlobalAlloc(GMEM_FIXED, len);  
	BYTE* pmem = (BYTE*)GlobalLock(m_hMem);  
	memcpy(pmem,lpRsrc,len);  
	IStream* pstm;  
	CreateStreamOnHGlobal(m_hMem,FALSE,&pstm);  
	// load from stream  
	pImg=Gdiplus::Image::FromStream(pstm);  
	// free/release stuff  
	GlobalUnlock(m_hMem);  
	pstm->Release();  
	FreeResource(lpRsrc);  
	return TRUE;  
}  

CCheckButton::CCheckButton()
{
	m_hPicInstance = GetModuleHandle("HmgSkinRes.dll");

	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&gdiplusStartupInput,NULL);
	m_bSaveBackground = FALSE;
	m_bTracking = FALSE;
	m_bHover = FALSE;

	SetPic(IDB_PNG_CHECKED_BOX_HOVER, IDB_PNG_CHECK_BOX_HOVER, IDB_PNG_CHECK_BOX_PRESSED, IDB_PNG_CHECK_BOX_NORMAL);
}

CCheckButton::~CCheckButton()
{
	GdiplusShutdown(m_pGdiToken);
	m_bitmap.Detach();
}

BEGIN_MESSAGE_MAP(CCheckButton, CButtonBase)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

BOOL CCheckButton::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(rc);
	CWnd* pParent = GetParent(); 
	pParent->ScreenToClient(rc);

	if(!m_bSaveBackground)
	{
		// 截取子窗口所掩盖的父窗口区域，刷新背景
		CPaintDC dc(pParent);

		m_bitmap.DeleteObject();
		m_bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		CBitmap *oldmemBitmap = memDC.SelectObject(&m_bitmap);
		memDC.BitBlt(0, 0, rc.Width(),rc.Height(), &dc, rc.left, rc.top, SRCCOPY);  
		memDC.SelectObject(oldmemBitmap);

		m_bSaveBackground = TRUE;
		memDC.DeleteDC();
	}

	return TRUE;
}

HBRUSH CCheckButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CButtonBase::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CCheckButton::OnPaint()
{
	CPaintDC dc(this);

	// 先绘制在背景图上
	Bitmap *pBmp = Bitmap::FromHBITMAP((HBITMAP)m_bitmap.m_hObject, NULL);
	if(pBmp == NULL)
		return;
	Graphics bmpGraphics(pBmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CRect rc;
	GetClientRect(&rc);

	// 绘制图片
	HMODULE hModule = GetModuleHandle("HmgSkinRes.dll"); 
	Image *pImg;
	if(m_bHover)
	{
		if(GetCheck())
		{
			ImageFromIDResource(m_hPicInstance, m_idCheckedHover, _T("png"), pImg);
		}
		else
		{
			ImageFromIDResource(m_hPicInstance, m_idNotCheckHover, _T("png"), pImg);
		}
	}
	else
	{
		if(GetCheck())
		{
			ImageFromIDResource(m_hPicInstance, m_idChecked, _T("png"), pImg);
		}
		else
		{
			ImageFromIDResource(m_hPicInstance, m_idNotCheck, _T("png"), pImg);
		}
	}

	RectF srcRect;
	Unit units;
	pImg->GetBounds(&srcRect, &units);

	RectF destRect((REAL)rc.left, (REAL)(rc.top + (rc.Height()-srcRect.Height)/2), srcRect.Width, srcRect.Height);
	bmpGraphics.DrawImage(pImg, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);

	// 绘制文本
	USES_CONVERSION;
	CString strText;
	GetWindowText(strText);
	wstring mTextValue = A2W((LPSTR)(LPCTSTR)strText);
	LOGFONT LogFont;
	GetFont()->GetLogFont(&LogFont);
	Font nFont(dc.m_hDC, &LogFont);
	StringFormat format;
	format.SetAlignment(StringAlignmentNear);
	format.SetLineAlignment(StringAlignmentCenter);
	RectF textRect(srcRect.GetRight()+3, (REAL)rc.top, (REAL)rc.Width()-srcRect.Width-3, (REAL)rc.Height());
	Color color;
	if(m_bHover)
	{
		color = Color(0x4d, 0x95, 0xc2);
	}
	else
	{
		color.SetFromCOLORREF(GetTextColor(dc.m_hDC));
	}
	SolidBrush brush(color); 
	bmpGraphics.DrawString(mTextValue.c_str(), mTextValue.length(), &nFont, textRect, &format, &brush);

	/*Drawing on DC*/
	Graphics graphics(dc.m_hDC);
	/*Important! Create a CacheBitmap object for quick drawing*/
	CachedBitmap cachedBmp(pBmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,0,0);
}

void CCheckButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CButtonBase::OnMouseMove(nFlags, point);
	Invalidate();

	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	}
}

LRESULT CCheckButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	Invalidate();

	return 0;
}

void CCheckButton::PreSubclassWindow() 
{
	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	m_bTracking = TrackMouseEvent(&m_TMS); 

	CButtonBase::PreSubclassWindow();
}

void CCheckButton::OnMouseHover(UINT nFlags, CPoint point)
{
	CButtonBase::OnMouseHover(nFlags, point);
	m_bHover = TRUE;
	Invalidate();
}

void CCheckButton::SetPic(UINT idCheckedHover, UINT idNotCheckHover, UINT idChecked, UINT idNotCheck, HINSTANCE hPicInstance /*= NULL*/)
{
	if(hPicInstance != NULL)
	{
		m_hPicInstance = hPicInstance;
	}
	m_idCheckedHover = idCheckedHover;
	m_idNotCheckHover = idNotCheckHover;
	m_idChecked = idChecked;
	m_idNotCheck = idNotCheck;
}