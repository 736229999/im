#include "stdafx.h"
#include "RadioButton.h"
#include "GdiPlus.h"
#include <string>
#include <atlconv.h>
using namespace Gdiplus;
using namespace std;

extern BOOL ImageFromIDResource(HINSTANCE hInst, UINT nID, LPCTSTR sTR, Image * & pImg);

IMPLEMENT_DYNAMIC(CRadioButton, CButton)

CRadioButton::CRadioButton()
{
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&gdiplusStartupInput,NULL);
	m_bSaveBackground = FALSE;
	m_bTracking = FALSE;
	m_bHover = FALSE;
}

CRadioButton::~CRadioButton()
{
	GdiplusShutdown(m_pGdiToken);
	m_bitmap.Detach();
}

BEGIN_MESSAGE_MAP(CRadioButton, CButton)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_MOUSEHOVER()
END_MESSAGE_MAP()

BOOL CRadioButton::OnEraseBkgnd(CDC* pDC)
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

HBRUSH CRadioButton::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	return CButton::OnCtlColor(pDC, pWnd, nCtlColor);
}

void CRadioButton::OnPaint()
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
			ImageFromIDResource(hModule, IDB_PNG_RADIOED_BOX_HOVER, _T("png"), pImg);
		}
		else
		{
			ImageFromIDResource(hModule, IDB_PNG_RADIO_BOX_HOVER, _T("png"), pImg);
		}
	}
	else
	{
		if(GetCheck())
		{
			ImageFromIDResource(hModule, IDB_PNG_RADIO_BOX_PRESSED, _T("png"), pImg);
		}
		else
		{
			ImageFromIDResource(hModule, IDB_PNG_RADIO_BOX_NORMAL, _T("png"), pImg);
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

void CRadioButton::OnMouseMove(UINT nFlags, CPoint point)
{
	CButton::OnMouseMove(nFlags, point);
	Invalidate();

	if(!m_bTracking)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	}
}

LRESULT CRadioButton::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bHover = FALSE;
	Invalidate();

	return 0;
}

void CRadioButton::PreSubclassWindow() 
{
	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	m_bTracking = TrackMouseEvent(&m_TMS); 

	CButton::PreSubclassWindow();
}

void CRadioButton::OnMouseHover(UINT nFlags, CPoint point)
{
	CButton::OnMouseHover(nFlags, point);
	m_bHover = TRUE;
	Invalidate();
}