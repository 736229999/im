// XHeaderCtrl.cpp
//
// Author:  Hans Dietrich
//          hdietrich2@hotmail.com
//
// This code is based on "Outlook 98-Style FlatHeader Control" 
// by Maarten Hoeben.
//
// See http://www.codeguru.com/listview/FlatHeader.shtml
//
// This software is released into the public domain.
// You are free to use it in any way you like.
//
// This software is provided "as is" with no expressed
// or implied warranty.  I accept no liability for any
// damage or loss of business that this software may cause.
//
///////////////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "XHeaderCtrl.h"
#include ".\xheaderctrl.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#undef THIS_FILE
static char THIS_FILE[] = __FILE__;
#endif

/////////////////////////////////////////////////////////////////////////////
// CXHeaderCtrl

IMPLEMENT_DYNCREATE(CXHeaderCtrl, CHeaderCtrl)

BEGIN_MESSAGE_MAP(CXHeaderCtrl, CHeaderCtrl)
	//{{AFX_MSG_MAP(CXHeaderCtrl)
	ON_MESSAGE(HDM_INSERTITEMA, OnInsertItem)
	ON_MESSAGE(HDM_INSERTITEMW, OnInsertItem)
	ON_MESSAGE(HDM_DELETEITEM, OnDeleteItem)
	ON_MESSAGE(HDM_SETIMAGELIST, OnSetImageList)
	ON_MESSAGE(HDM_LAYOUT, OnLayout)
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	ON_WM_ERASEBKGND()
	//}}AFX_MSG_MAP
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()

///////////////////////////////////////////////////////////////////////////////
// ctor
CXHeaderCtrl::CXHeaderCtrl()
{
	m_bDoubleBuffer = TRUE;
	m_iSpacing = 6;
	m_sizeArrow.cx = 8;
	m_sizeArrow.cy = 8;
	m_sizeImage.cx = 0;
	m_sizeImage.cy = 0;
	m_bStaticBorder = FALSE;
	m_nDontDropCursor = 0;
	m_bResizing = FALSE;
	m_nClickFlags = 0;

	m_crLine        = RGB(255,255,255);
	m_crBkGround    = RGB(128,168,194);
	m_crBtnText     = RGB(255,255,255);

	m_iHotItem      = -1;

	m_Font.CreateFont(12,0,0,90,FW_NORMAL,0,0,0,DEFAULT_CHARSET,OUT_TT_ONLY_PRECIS,
		CLIP_DEFAULT_PRECIS,DRAFT_QUALITY,(DEFAULT_PITCH|FF_MODERN),_T("宋体"));
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXHeaderCtrl::~CXHeaderCtrl()
{
}

///////////////////////////////////////////////////////////////////////////////
// ModifyProperty
BOOL CXHeaderCtrl::ModifyProperty(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
		case FH_PROPERTY_SPACING:
			m_iSpacing = (int)lParam;
			break;

		case FH_PROPERTY_ARROW:
			m_sizeArrow.cx = LOWORD(lParam);
			m_sizeArrow.cy = HIWORD(lParam);
			break;

		case FH_PROPERTY_STATICBORDER:
			m_bStaticBorder = (BOOL)lParam;
			break;

		case FH_PROPERTY_DONTDROPCURSOR:
			m_nDontDropCursor = (UINT)lParam;
			break;

		default:
			return FALSE;
	}

	Invalidate();
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DrawCtrl
void CXHeaderCtrl::DrawCtrl(CDC* pDC)
{
	CRect rectClip;
	if (pDC->GetClipBox(&rectClip) == ERROR)
		return;

	CRect rectClient, rectItem;
	GetClientRect(&rectClient);

	int nRight = rectClient.right;

    pDC->FillSolidRect(rectClip,m_crBkGround);

	int iItems = GetItemCount();
	ASSERT(iItems >= 0);

	CPen penLine(PS_SOLID, 1, m_crLine);
	CPen* pPen = pDC->GetCurrentPen();

	CFont* pFont = pDC->SelectObject(&m_Font);

	pDC->SetBkColor(m_crBkGround);
	pDC->SetTextColor(m_crBtnText);

	int iWidth = 0;

	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);

		TCHAR szText[FLATHEADER_TEXT_MAX];

		HDITEM hditem;
		hditem.mask = HDI_WIDTH|HDI_FORMAT|HDI_TEXT|HDI_IMAGE|HDI_BITMAP;
		hditem.pszText = szText;
		hditem.cchTextMax = sizeof(szText);
		VERIFY(GetItem(iItem, &hditem));

		VERIFY(GetItemRect(iItem, rectItem));

		if (rectItem.right >= rectClip.left || rectItem.left <= rectClip.right)
		{
			if (hditem.fmt & HDF_OWNERDRAW)
			{
				DRAWITEMSTRUCT disItem;
				disItem.CtlType = ODT_BUTTON;
				disItem.CtlID = GetDlgCtrlID();
				disItem.itemID = iItem;
				disItem.itemAction = ODA_DRAWENTIRE;
				disItem.itemState = 0;
				disItem.hwndItem = m_hWnd;
				disItem.hDC = pDC->m_hDC;
				disItem.rcItem = rectItem;
				disItem.itemData = 0;

				DrawItem(&disItem);
			}
			else
			{
				if ( iItem==m_iHotItem )
				{
					pDC->FillSolidRect(rectItem,0xFF0000);
				}

				rectItem.DeflateRect(m_iSpacing, 0);
		
				DrawItem(pDC, rectItem, &hditem);
				rectItem.InflateRect(m_iSpacing, 0);
				//if (m_nClickFlags & MK_LBUTTON && m_iHotIndex == iItem && m_hdhtiHotItem.flags & HHT_ONHEADER)
				//	pDC->InvertRect(rectItem);
			}

			if (i < iItems )
			{
				//draw seperator line
				pDC->SelectObject(&penLine);
				pDC->MoveTo(rectItem.right-1, rectItem.top);
				pDC->LineTo(rectItem.right-1, rectItem.bottom);

			//	pDC->MoveTo(rectItem.right, rectItem.top+2);
			//	pDC->LineTo(rectItem.right, rectItem.bottom-2);
			}
		}

		iWidth += hditem.cxy;
	}

	if (iWidth > 0)
	{
		rectClient.right =  nRight;//rectClient.left + iWidth;
		rectClient.bottom+=1;
		pDC->Draw3dRect(rectClient, m_crLine, m_crLine);
	}

	pDC->SelectObject(pFont);
	pDC->SelectObject(pPen);

}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(LPDRAWITEMSTRUCT)
{
	ASSERT(FALSE);  // must override for self draw header controls
}

///////////////////////////////////////////////////////////////////////////////
// DrawItem
void CXHeaderCtrl::DrawItem(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	ASSERT(lphdi->mask & HDI_FORMAT);

	int iWidth = 0;

	CBitmap* pBitmap = NULL;
	BITMAP BitmapInfo;

	if (lphdi->fmt & HDF_BITMAP)
	{
		ASSERT(lphdi->mask & HDI_BITMAP);
		ASSERT(lphdi->hbm);

		pBitmap = CBitmap::FromHandle(lphdi->hbm);
		if (pBitmap)
			VERIFY(pBitmap->GetObject(sizeof(BITMAP), &BitmapInfo));
	}

	rect.left += ((iWidth = DrawImage(pDC, rect, lphdi, FALSE)) != 0) ? iWidth + m_iSpacing : 0;
	rect.right -= ((iWidth = DrawBitmap(pDC, rect, lphdi, pBitmap, &BitmapInfo, TRUE)) != 0) ? 
		iWidth + m_iSpacing : 0;

	pDC->SetBkMode(TRANSPARENT);
	DrawText(pDC, rect, lphdi);
}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXHeaderCtrl::DrawImage(CDC* pDC, CRect rect, LPHDITEM lphdi, BOOL bRight)
{
	CImageList* pImageList = GetImageList();
	int iWidth = 0;

	if (lphdi->iImage != XHEADERCTRL_NO_IMAGE)
	{
		if (pImageList)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (m_sizeImage.cy >> 1);

				if (bRight)
					point.x = rect.right - m_sizeImage.cx;
				else
					point.x = rect.left;

				SIZE size;
				size.cx = rect.Width()<m_sizeImage.cx ? rect.Width():m_sizeImage.cx;
				size.cy = m_sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color to same as header control
				pImageList->SetBkColor(pDC->GetBkColor());
				pImageList->DrawIndirect(pDC, lphdi->iImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				iWidth = m_sizeImage.cx;
			}
		}
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawBitmap
int CXHeaderCtrl::DrawBitmap(CDC* pDC, 
								CRect rect, 
								LPHDITEM lphdi, 
								CBitmap* pBitmap, 
								BITMAP* pBitmapInfo, 
								BOOL bRight)
{
	UNUSED_ALWAYS(lphdi);

	int iWidth = 0;

	if (pBitmap)
	{
		iWidth = pBitmapInfo->bmWidth;
		if (iWidth <= rect.Width() && rect.Width() > 0)
		{
			POINT point;

			point.y = rect.CenterPoint().y - (pBitmapInfo->bmHeight >> 1);

			if (bRight)
				point.x = rect.right - iWidth;
			else
				point.x = rect.left;

			CDC dc;
			if (dc.CreateCompatibleDC(pDC) == TRUE) 
			{
				VERIFY(dc.SelectObject(pBitmap));
				//iWidth = pDC->BitBlt(
				//	point.x, point.y, 
				//	pBitmapInfo->bmWidth, pBitmapInfo->bmHeight, 
				//	&dc, 
				//	0, 0, 
				//	SRCCOPY
				//) ? iWidth:0;
				pDC->TransparentBlt(
					point.x,point.y,
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight,
					&dc,
					0, 0,
					pBitmapInfo->bmWidth, pBitmapInfo->bmHeight,
					RGB(0,255,0));
			}
			else 
				iWidth = 0;
		}
		else
			iWidth = 0;
	}

	return iWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
int CXHeaderCtrl::DrawText(CDC* pDC, CRect rect, LPHDITEM lphdi)
{
	CSize size;

	pDC->SetTextColor(m_crBtnText);

	if (rect.Width() > 0 && lphdi->mask & HDI_TEXT && lphdi->fmt & HDF_STRING)
	{
		size = pDC->GetTextExtent(lphdi->pszText);

		// always center column headers
		pDC->DrawText(lphdi->pszText, -1, rect, 
			DT_LEFT|DT_END_ELLIPSIS|DT_SINGLELINE|DT_VCENTER);
	}

	size.cx = rect.Width()>size.cx ? size.cx:rect.Width();
	return size.cx>0 ? size.cx:0;
}

///////////////////////////////////////////////////////////////////////////////
// OnInsertItem
LRESULT CXHeaderCtrl::OnInsertItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnDeleteItem
LRESULT CXHeaderCtrl::OnDeleteItem(WPARAM, LPARAM)
{
	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnSetImageList
LRESULT CXHeaderCtrl::OnSetImageList(WPARAM, LPARAM lParam)
{
	CImageList* pImageList;
	pImageList = CImageList::FromHandle((HIMAGELIST)lParam);

	IMAGEINFO info;
	if (pImageList->GetImageInfo(0, &info))
	{
		m_sizeImage.cx = info.rcImage.right - info.rcImage.left;
		m_sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
	}

	return Default();
}

///////////////////////////////////////////////////////////////////////////////
// OnLayout
LRESULT CXHeaderCtrl::OnLayout(WPARAM, LPARAM lParam)
{
	LPHDLAYOUT lphdlayout = (LPHDLAYOUT)lParam;

	if (m_bStaticBorder)
		lphdlayout->prc->right += GetSystemMetrics(SM_CXBORDER)*2;

	return CHeaderCtrl::DefWindowProc(HDM_LAYOUT, 0, lParam);
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXHeaderCtrl::OnSysColorChange() 
{
	TRACE(_T("in CXHeaderCtrl::OnSysColorChange\n"));

	CHeaderCtrl::OnSysColorChange();

	m_crLine        = ::GetSysColor(COLOR_3DSHADOW);
	m_crBkGround    = 
	m_crBtnText     = ::GetSysColor(COLOR_BTNTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// OnEraseBkgnd
BOOL CXHeaderCtrl::OnEraseBkgnd(CDC* pDC) 
{
	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXHeaderCtrl::OnPaint() 
{
    CPaintDC dc(this);

    //if (m_bDoubleBuffer)
    //{
    //    CMemDC MemDC(&dc);
    //    DrawCtrl(&MemDC);
    //}
    //else
    //    DrawCtrl(&dc);
	DrawCtrl(&dc);
}

int CXHeaderCtrl::HitTest(CPoint point)
{
	int iItems = GetItemCount();
	CRect rectItem;
	for (int i = 0; i < iItems; i++)
	{
		int iItem = OrderToIndex(i);
		GetItemRect(iItem,&rectItem);

		rectItem.InflateRect(-7,-1);
		if ( rectItem.PtInRect(point) )
		{
			return i;
		}
	}
		
	return -1;
}

void CXHeaderCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	CHeaderCtrl::OnMouseMove(nFlags, point);

	int iHotItem = HitTest(point);

	if ( iHotItem!=m_iHotItem )
	{
		m_iHotItem = iHotItem;
		SetTimer(0x01,100,NULL);
		Invalidate();
	}
}

void CXHeaderCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CHeaderCtrl::OnLButtonDown(nFlags, point);	
			SetTimer(0x01,100,NULL);
}

void CXHeaderCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CHeaderCtrl::OnLButtonUp(nFlags, point);
	SetTimer(0x01,100,NULL);

}

void CXHeaderCtrl::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if ( nIDEvent==0x01 )
	{
		CRect rect;
		GetClientRect(&rect);
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		if ( !rect.PtInRect(point) )
		{
			m_iHotItem = -1;
			Invalidate();
			KillTimer(0x01);
		}
		
	}

	CHeaderCtrl::OnTimer(nIDEvent);
}
