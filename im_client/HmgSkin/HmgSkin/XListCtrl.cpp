#include "stdafx.h"
#include "resource.h"
#include "XListCtrl.h"
#include "HmgSkinPublic.h"

UINT NEAR WM_XLISTCTRL_COMBO_SELECTION  = ::RegisterWindowMessage(_T("WM_XLISTCTRL_COMBO_SELECTION"));
UINT NEAR WM_XLISTCTRL_CHECKBOX_CLICKED = ::RegisterWindowMessage(_T("WM_XLISTCTRL_CHECKBOX_CLICKED"));

/////////////////////////////////////////////////////////////////////////////
// CXListCtrl

BEGIN_MESSAGE_MAP(CXListCtrl, CListCtrl)
	//{{AFX_MSG_MAP(CXListCtrl)
	ON_WM_CREATE()
	ON_NOTIFY_REFLECT(NM_CUSTOMDRAW, OnCustomDraw)
	ON_WM_DESTROY()
	ON_WM_LBUTTONDOWN()
	ON_WM_PAINT()
	ON_WM_SYSCOLORCHANGE()
	//}}AFX_MSG_MAP
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTW, 0, 0xFFFF, OnToolTipText)
	ON_NOTIFY_EX_RANGE(TTN_NEEDTEXTA, 0, 0xFFFF, OnToolTipText)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


///////////////////////////////////////////////////////////////////////////////
// ctor
CXListCtrl::CXListCtrl()
{
	m_uID                   = 0;
	m_dwExtendedStyleX      = 0;
	m_bHeaderIsSubclassed   = FALSE;

	m_cr3DFace              = ::GetSysColor(COLOR_3DFACE);
	m_cr3DHighLight         = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow            = ::GetSysColor(COLOR_3DSHADOW);
	m_crBtnFace             = ::GetSysColor(COLOR_BTNFACE);
	m_crBtnShadow           = ::GetSysColor(COLOR_BTNSHADOW);
	m_crBtnText             = ::GetSysColor(COLOR_BTNTEXT);
	m_crGrayText            = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHighLight           = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText       = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindow              = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText          = ::GetSysColor(COLOR_WINDOWTEXT);

	m_hArrowDown            = HMG_ResLoadBitmap(IDB_ARROW_DOWN);////::LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_ARROW_DOWN), RT_BITMAP), MAKEINTRESOURCE(IDB_ARROW_DOWN));//LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ARROW_DOWN));
	m_hArrowUp              = HMG_ResLoadBitmap(IDB_ARROW_UP);//LoadBitmap(AfxFindResourceHandle(MAKEINTRESOURCE(IDB_ARROW_UP), RT_BITMAP), MAKEINTRESOURCE(IDB_ARROW_UP));// LoadBitmap(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDB_ARROW_UP));
}

///////////////////////////////////////////////////////////////////////////////
// dtor
CXListCtrl::~CXListCtrl()
{
	DeleteObject(m_hArrowDown);
	DeleteObject(m_hArrowUp);
}

///////////////////////////////////////////////////////////////////////////////
// PreSubclassWindow
void CXListCtrl::PreSubclassWindow()
{
	CListCtrl::PreSubclassWindow();

	// for Dialog based applications, this is a good place
	// to subclass the header control because the OnCreate()
	// function does not get called.

	SubclassHeaderControl();
}

///////////////////////////////////////////////////////////////////////////////
// OnCreate
int CXListCtrl::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CListCtrl::OnCreate(lpCreateStruct) == -1)
	{
		ASSERT(FALSE);
		return -1;
	}

	// When the CXListCtrl object is created via a call to Create(), instead
	// of via a dialog box template, we must subclass the header control
	// window here because it does not exist when the PreSubclassWindow()
	// function is called.

	SubclassHeaderControl();

	return 0;
}

///////////////////////////////////////////////////////////////////////////////
// SubclassHeaderControl
void CXListCtrl::SubclassHeaderControl()
{
	if (m_bHeaderIsSubclassed)
		return;

	// if the list control has a header control window, then
	// subclass it

	// Thanks to Alberto Gattegno and Alon Peleg  and their article
	// "A Multiline Header Control Inside a CListCtrl" for easy way
	// to determine if the header control exists.

	CHeaderCtrl* pHeader = GetHeaderCtrl();
	if (pHeader)
	{
		VERIFY(m_HeaderCtrl.SubclassWindow(pHeader->m_hWnd));
		m_bHeaderIsSubclassed = TRUE;

		//static CFont f;

		//f.CreatePointFont(100, "MS Sans Serif");

		m_HeaderCtrl.SetFont(GetFont());
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnCustomDraw
void CXListCtrl::OnCustomDraw(NMHDR* pNMHDR, LRESULT* pResult)
{
	NMLVCUSTOMDRAW* pLVCD = reinterpret_cast<NMLVCUSTOMDRAW*>(pNMHDR);

	// Take the default processing unless we set this to something else below.
	*pResult = CDRF_DODEFAULT;

	// First thing - check the draw stage. If it's the control's prepaint
	// stage, then tell Windows we want messages for every item.

	if (pLVCD->nmcd.dwDrawStage == CDDS_PREPAINT)
	{
		*pResult = CDRF_NOTIFYITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == CDDS_ITEMPREPAINT)
	{
		// This is the notification message for an item.  We'll request
		// notifications before each subitem's prepaint stage.

		*pResult = CDRF_NOTIFYSUBITEMDRAW;
	}
	else if (pLVCD->nmcd.dwDrawStage == (CDDS_ITEMPREPAINT | CDDS_SUBITEM))
	{
		// This is the prepaint stage for a subitem. Here's where we set the
		// item's text and background colors. Our return value will tell
		// Windows to draw the subitem itself, but it will use the new colors
		// we set here.

		int nItem = static_cast<int> (pLVCD->nmcd.dwItemSpec);
		int nSubItem = pLVCD->iSubItem;

		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) pLVCD->nmcd.lItemlParam;
		ASSERT(pXLCD);

		COLORREF crText  = m_crWindowText;
		COLORREF crBkgnd = m_crWindow;

		if (pXLCD)
		{
			crText  = pXLCD[nSubItem].crText;
			crBkgnd = pXLCD[nSubItem].crBackground;

			if (!pXLCD[0].bEnabled)
				crText = m_crGrayText;
		}

		// store the colors back in the NMLVCUSTOMDRAW struct
		pLVCD->clrText = crText;
		pLVCD->clrTextBk = crBkgnd;

		CDC* pDC = CDC::FromHandle(pLVCD->nmcd.hdc);

		CRect rect;
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);

		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		int nSavedMemDC = dcMem.SaveDC();

		CRect rectDraw;
		rectDraw.left   = rectDraw.top = 0;
		rectDraw.right  = rect.Width();
		rectDraw.bottom = rect.Height();

		CBitmap bmpMem;
		bmpMem.CreateCompatibleBitmap(pDC, rectDraw.right,rectDraw.bottom);

		dcMem.SelectObject(&bmpMem);
		dcMem.SelectObject(GetFont());

		if (pXLCD && (pXLCD[nSubItem].bShowProgress))
		{
			DrawProgress(nItem, nSubItem, &dcMem, crText, crBkgnd, rectDraw, pXLCD);

			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
		else if (pXLCD && (pXLCD[nSubItem].nCheckedState != -1))
		{
			DrawCheckbox(nItem, nSubItem, &dcMem, crText, crBkgnd, rectDraw, pXLCD);

			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}
		else
		{
			rectDraw.left += DrawImage(nItem, nSubItem, &dcMem, crText, crBkgnd, rectDraw, pXLCD);

			DrawText(nItem, nSubItem, &dcMem, crText, crBkgnd, rectDraw, pXLCD);

			*pResult = CDRF_SKIPDEFAULT;	// We've painted everything.
		}

		pDC->BitBlt(rect.left,rect.top,rect.Width(),rect.Height(),&dcMem,0,0,SRCCOPY);

		dcMem.RestoreDC(nSavedMemDC);
	}
}

///////////////////////////////////////////////////////////////////////////////
// DrawProgress
void CXListCtrl::DrawProgress(int nItem,
							  int nSubItem,
							  CDC *pDC,
							  COLORREF crText,
							  COLORREF crBkgnd,
							  CRect& rect,
							  XLISTCTRLDATA *pXLCD)
{
	UNUSED_ALWAYS(nItem);

	ASSERT(pDC);
	ASSERT(pXLCD);

	rect.bottom -= 1;
	rect.left += 1;		// leave margin in case row is highlighted
	rect.right -= 2;

	// draw border

	CPen graypen(PS_SOLID, 1, m_crBtnShadow);
	CPen *pOldPen = pDC->SelectObject(&graypen);

	pDC->MoveTo(rect.left, rect.bottom);
	pDC->LineTo(rect.right+1, rect.bottom);

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.right, rect.top);

	pDC->MoveTo(rect.left, rect.top);
	pDC->LineTo(rect.left, rect.bottom);

	pDC->MoveTo(rect.right, rect.top);
	pDC->LineTo(rect.right, rect.bottom);

	// fill interior with light gray
	CRect InteriorRect;
	InteriorRect = rect;
	InteriorRect.left += 1;
	InteriorRect.top += 1;
	pDC->FillSolidRect(InteriorRect, RGB(224,224,224));

	// finish drawing border
	CPen blackpen(PS_SOLID, 1, RGB(0,0,0));
	pDC->SelectObject(&blackpen);

	pDC->MoveTo(rect.left+1, rect.top+1);
	pDC->LineTo(rect.right, rect.top+1);

	pDC->MoveTo(rect.left+1, rect.top+1);
	pDC->LineTo(rect.left+1, rect.bottom);

	pDC->SelectObject(pOldPen);

	if (pXLCD[nSubItem].nProgressPercent > 0)
	{
		// draw progress bar and text

		CRect LeftRect, RightRect;
		LeftRect = rect;
		LeftRect.left += 2;
		LeftRect.top += 2;
		RightRect = LeftRect;
		int w = (LeftRect.Width() * pXLCD[nSubItem].nProgressPercent) / 100;
		LeftRect.right = LeftRect.left + w;
		RightRect.left = LeftRect.right + 1;
		pDC->FillSolidRect(LeftRect, m_crHighLight);

		if (pXLCD[nSubItem].bShowProgressMessage)
		{
			CString str, format;
			format = pXLCD[nSubItem].strProgressMessage;
			if (format.IsEmpty())
				str.Format(_T("%d%%"), pXLCD[nSubItem].nProgressPercent);
			else
				str.Format(format, pXLCD[nSubItem].nProgressPercent);

			pDC->SetBkMode(TRANSPARENT);

			CRect TextRect;
			TextRect = rect;
			TextRect.DeflateRect(1, 1);
			TextRect.top += 1;

			CRgn rgn;
			rgn.CreateRectRgn(LeftRect.left, LeftRect.top, LeftRect.right, LeftRect.bottom);
			pDC->SelectClipRgn(&rgn);
			pDC->SetTextColor(crBkgnd);
			pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);

			rgn.DeleteObject();
			rgn.CreateRectRgn(RightRect.left, RightRect.top, RightRect.right, RightRect.bottom);
			pDC->SelectClipRgn(&rgn);
			pDC->SetTextColor(crText);
			pDC->DrawText(str, &TextRect, DT_CENTER | DT_VCENTER | DT_SINGLELINE);
			rgn.DeleteObject();
			pDC->SelectClipRgn(NULL);
		}
	}
}

///////////////////////////////////////////////////////////////////////////////
// DrawCheckbox
void CXListCtrl::DrawCheckbox(int nItem,
							  int nSubItem,
							  CDC *pDC,
							  COLORREF crText,
							  COLORREF crBkgnd,
							  CRect& rect,
							  XLISTCTRLDATA *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);

//	GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	pDC->FillSolidRect(&rect, crBkgnd);

	CRect chkboxrect;
	chkboxrect = rect;
	chkboxrect.bottom -= 1;
	chkboxrect.left += 9;		// line up checkbox with header checkbox
	chkboxrect.right = chkboxrect.left + chkboxrect.Height();	// width = height

	CString str;
	str = GetItemText(nItem, nSubItem);

	if (str.IsEmpty())
	{
		// center the checkbox

		chkboxrect.left = rect.left + rect.Width()/2 - chkboxrect.Height()/2 - 1;
		chkboxrect.right = chkboxrect.left + chkboxrect.Height();
	}

	// fill rect around checkbox with white
	pDC->FillSolidRect(&chkboxrect, m_crWindow);

	chkboxrect.left += 1;

	// draw border
	pDC->DrawEdge(&chkboxrect, EDGE_SUNKEN, BF_RECT);

	if (pXLCD[nSubItem].nCheckedState == 1)
	{
		CPen *pOldPen = NULL;

		CPen graypen(PS_SOLID, 1, m_crGrayText);
		CPen blackpen(PS_SOLID, 1, RGB(0,0,0));

		if (pXLCD[0].bEnabled)
			pOldPen = pDC->SelectObject(&blackpen);
		else
			pOldPen = pDC->SelectObject(&graypen);

		// draw the checkmark
		int x = chkboxrect.left + 9;
		ASSERT(x < chkboxrect.right);
		int y = chkboxrect.top + 3;
		int i;
		for (i = 0; i < 4; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y++;
		}
		for (i = 0; i < 3; i++)
		{
			pDC->MoveTo(x, y);
			pDC->LineTo(x, y+3);
			x--;
			y--;
		}

		if (pOldPen)
			pDC->SelectObject(pOldPen);
	}

	if (!str.IsEmpty())
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		CRect textrect;
		textrect = rect;
		textrect.left = chkboxrect.right + 4;

		pDC->DrawText(str, &textrect, DT_LEFT | DT_VCENTER | DT_SINGLELINE);
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetDrawColors
//void CXListCtrl::GetDrawColors(int nItem,
//							   int nSubItem,
//							   COLORREF& colorText,
//							   COLORREF& colorBkgnd)
//{
//	DWORD dwStyle    = GetStyle();
//	DWORD dwExStyle  = GetExtendedStyle();
//
//	COLORREF crText  = colorText;
//	COLORREF crBkgnd = colorBkgnd;
//
//	if (GetItemState(nItem, LVIS_SELECTED))
//	{
//		if (dwExStyle & LVS_EX_FULLROWSELECT)
//		{
//			// selected?  if so, draw highlight background
//			crText  = m_crHighLightText;
//			crBkgnd = m_crHighLight;
//
//			// has focus?  if not, draw gray background
//			if (m_hWnd != ::GetFocus())
//			{
//				if (dwStyle & LVS_SHOWSELALWAYS)
//				{
//					crText  = m_crWindowText;
//					crBkgnd = m_crBtnFace;
//				}
//				else
//				{
//					crText  = colorText;
//					crBkgnd = colorBkgnd;
//				}
//			}
//
//			//if ( nSubItem!=0 )
//			//{
//			//	crText  = m_crHighLightText;
//			//	crBkgnd = m_crHighLight;
//			//}
//			//else
//			//{
//			//}
//		}
//		else	// not full row select
//		{
//			if (nSubItem == 0)
//			{
//				// selected?  if so, draw highlight background
//				crText  = m_crHighLightText;
//				crBkgnd = m_crHighLight;
//
//				// has focus?  if not, draw gray background
//				if (m_hWnd != ::GetFocus())
//				{
//					if (dwStyle & LVS_SHOWSELALWAYS)
//					{
//						crText  = m_crWindowText;
//						crBkgnd = m_crBtnFace;
//					}
//					else
//					{
//						crText  = colorText;
//						crBkgnd = colorBkgnd;
//					}
//				}
//			}
//		}
//	}
//
//	colorText = crText;
//	colorBkgnd = crBkgnd;
//}

///////////////////////////////////////////////////////////////////////////////
// DrawImage
int CXListCtrl::DrawImage(int nItem,
						  int nSubItem,
						  CDC* pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect rect,
  						  XLISTCTRLDATA *pXLCD)
{
	//GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	pDC->FillSolidRect(&rect, crBkgnd);

	int nWidth = 0;
	rect.left += m_HeaderCtrl.GetSpacing();

	CImageList* pImageList = GetImageList(LVSIL_SMALL);
	if (pImageList)
	{
		SIZE sizeImage;
		sizeImage.cx = sizeImage.cy = 0;
		IMAGEINFO info;

		int nImage = -1;
		if (pXLCD)
			nImage = pXLCD[nSubItem].nImage;

		if (nImage == -1)
			return 0;

		if (pImageList->GetImageInfo(nImage, &info))
		{
			sizeImage.cx = info.rcImage.right - info.rcImage.left;
			sizeImage.cy = info.rcImage.bottom - info.rcImage.top;
		}

		if (nImage >= 0)
		{
			if (rect.Width() > 0)
			{
				POINT point;

				point.y = rect.CenterPoint().y - (sizeImage.cy >> 1);
				point.x = rect.left;

				SIZE size;
				size.cx = rect.Width() < sizeImage.cx ? rect.Width() : sizeImage.cx;
				size.cy = rect.Height() < sizeImage.cy ? rect.Height() : sizeImage.cy;

				// save image list background color
				COLORREF rgb = pImageList->GetBkColor();

				// set image list background color
				pImageList->SetBkColor(crBkgnd);
				pImageList->DrawIndirect(pDC, nImage, point, size, CPoint(0, 0));
				pImageList->SetBkColor(rgb);

				nWidth = sizeImage.cx + m_HeaderCtrl.GetSpacing();
			}
		}
	}

	return nWidth;
}

///////////////////////////////////////////////////////////////////////////////
// DrawText
void CXListCtrl::DrawText(int nItem,
						  int nSubItem,
						  CDC *pDC,
						  COLORREF crText,
						  COLORREF crBkgnd,
						  CRect& rect,
						  XLISTCTRLDATA *pXLCD)
{
	ASSERT(pDC);
	ASSERT(pXLCD);

	//GetDrawColors(nItem, nSubItem, crText, crBkgnd);

	if (GetItemState(nItem, LVIS_SELECTED))
	{
		crBkgnd = m_crHighLight;
		crText  = 0xFFFFFF;
	}

	pDC->FillSolidRect(&rect, crBkgnd);

	CString str;
	str = GetItemText(nItem, nSubItem);

	if (!str.IsEmpty())
	{
		UINT nFormat = DT_LEFT | DT_SINGLELINE |DT_VCENTER;

		CFont *pOldFont = NULL;
		CFont boldfont;

		// check if bold specified for subitem
		if (pXLCD && pXLCD[nSubItem].bBold)
		{
			CFont *font = pDC->GetCurrentFont();
			if (font)
			{
				LOGFONT lf;
				font->GetLogFont(&lf);
				lf.lfWeight = FW_BOLD;
				boldfont.CreateFontIndirect(&lf);
				pOldFont = pDC->SelectObject(&boldfont);
			}
		}

		pDC->SetBkMode(TRANSPARENT);
		pDC->SetTextColor(crText);
		pDC->SetBkColor(crBkgnd);
		rect.left+=2;
		rect.right-=2;
		pDC->DrawText(str, &rect, nFormat);
		if (pOldFont)
			pDC->SelectObject(pOldFont);
	}
}

///////////////////////////////////////////////////////////////////////////////
// GetSubItemRect
BOOL CXListCtrl::GetSubItemRect(int nItem,
								int nSubItem,
								int nArea,
								CRect& rect)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL bRC = CListCtrl::GetSubItemRect(nItem, nSubItem, nArea, rect);

	// if nSubItem == 0, the rect returned by CListCtrl::GetSubItemRect
	// is the entire row, so use left edge of second subitem

	if (nSubItem == 0)
	{
		if (GetColumns() > 1)
		{
			CRect rect1;
			bRC = GetSubItemRect(nItem, 1, LVIR_BOUNDS, rect1);
			rect.right = rect1.left;
		}
	}

	return bRC;
}

///////////////////////////////////////////////////////////////////////////////
// OnLButtonDown
void CXListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	TRACE(_T("in CXListCtrl::OnLButtonDown\n"));

	int nItem = -1;
	CRect rect;

	int i;
	for (i = 0; i < GetItemCount(); i++)
	{
		if (CListCtrl::GetItemRect(i, &rect, LVIR_BOUNDS))
		{
			if (rect.PtInRect(point))
			{
				nItem = i;
				break;
			}
		}
	}

	if (nItem == -1)
	{
	}
	else
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (!pXLCD)
		{
			return;
		}

		if (!pXLCD[0].bEnabled)
			return;

		CRect rect;

		int nSubItem = -1;

		// check if a subitem checkbox was clicked

		for (i = 0; i < GetColumns(); i++)
		{
			GetSubItemRect(nItem, i, LVIR_BOUNDS, rect);
			if (rect.PtInRect(point))
			{
				nSubItem = i;
				break;
			}
		}

		if (nSubItem == -1)
		{
			// -1 = no checkbox for this subitem

		}
		else
		{
			if (pXLCD[nSubItem].nCheckedState >= 0)
			{
				int nChecked = pXLCD[nSubItem].nCheckedState;

				nChecked = (nChecked == 0) ? 1 : 0;

				pXLCD[nSubItem].nCheckedState = nChecked;

				UpdateSubItem(nItem, nSubItem);

				CWnd *pWnd = GetParent();
				if (!pWnd)
					pWnd = GetOwner();
				if (pWnd && ::IsWindow(pWnd->m_hWnd))
					pWnd->SendMessage(WM_XLISTCTRL_CHECKBOX_CLICKED, 
								nItem, nSubItem);

				// now update checkbox in header

				// -1 = no checkbox in column header
				if (GetHeaderCheckedState(nSubItem) != XHEADERCTRL_NO_IMAGE)
				{
					int nCheckedCount = CountCheckedItems(nSubItem);

					if (nCheckedCount == GetItemCount())
						SetHeaderCheckedState(nSubItem, XHEADERCTRL_CHECKED_IMAGE);
					else
						SetHeaderCheckedState(nSubItem, XHEADERCTRL_UNCHECKED_IMAGE);
				}
			}
		}
	}

	CListCtrl::OnLButtonDown(nFlags, point);
}

///////////////////////////////////////////////////////////////////////////////
// OnPaint
void CXListCtrl::OnPaint()
{
    Default();

	if (GetItemCount() <= 0)
	{
		CDC* pDC = GetDC();
		int nSavedDC = pDC->SaveDC();

		CRect rc;
		GetWindowRect(&rc);
		ScreenToClient(&rc);
		CHeaderCtrl* pHC = GetHeaderCtrl();
		if (pHC != NULL)
		{
			CRect rcH;
			pHC->GetItemRect(0, &rcH);
			rc.top = rcH.bottom;
		}
	//	rc.top += 1;

		COLORREF crBkgnd = m_crWindow;

		CBrush brush(crBkgnd);
		pDC->FillRect(rc, &brush);

		pDC->RestoreDC(nSavedDC);
		ReleaseDC(pDC);
	}
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(const LVITEM* pItem)
{
	ASSERT(pItem->iItem >= 0);
	if (pItem->iItem < 0)
		return -1;

	int index = CListCtrl::InsertItem(pItem);

	if (index < 0)
		return index;

	XLISTCTRLDATA *pXLCD = new XLISTCTRLDATA [GetColumns()];
	ASSERT(pXLCD);
	if (!pXLCD)
		return -1;

	pXLCD[0].crText       = m_crWindowText;
	pXLCD[0].crBackground = m_crWindow;
	pXLCD[0].nImage       = pItem->iImage;

	CListCtrl::SetItemData(index, (DWORD) pXLCD);

	return index;
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(int nItem, LPCTSTR lpszItem)
{
	ASSERT(nItem >= 0);
	if (nItem < 0)
		return -1;

	return InsertItem(nItem,
					  lpszItem,
					  m_crWindowText,
					  m_crWindow);
}

///////////////////////////////////////////////////////////////////////////////
// InsertItem
int CXListCtrl::InsertItem(int nItem,
						   LPCTSTR lpszItem,
						   COLORREF crText,
						   COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	if (nItem < 0)
		return -1;

	int index = CListCtrl::InsertItem(nItem, lpszItem);

	if (index < 0)
		return index;

	XLISTCTRLDATA *pXLCD = new XLISTCTRLDATA [GetColumns()];
	ASSERT(pXLCD);
	if (!pXLCD)
		return -1;

	pXLCD[0].crText       = crText;
	pXLCD[0].crBackground = crBackground;
	pXLCD[0].nImage       = -1;

	CListCtrl::SetItemData(index, (DWORD) pXLCD);

	return index;
}

///////////////////////////////////////////////////////////////////////////////
// SetItem
int CXListCtrl::SetItem(const LVITEM* pItem)
{
	ASSERT(pItem->iItem >= 0);
	if (pItem->iItem < 0)
		return -1;

	BOOL rc = CListCtrl::SetItem(pItem);

	if (!rc)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(pItem->iItem);
	if (pXLCD)
	{
		pXLCD[pItem->iSubItem].nImage = pItem->iImage;
		UpdateSubItem(pItem->iItem, pItem->iSubItem);
		rc = TRUE;
	}
	else
	{
		rc = FALSE;
	}

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemImage
BOOL CXListCtrl::SetItemImage(int nItem, int nSubItem, int nImage)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		pXLCD[nSubItem].nImage = nImage;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemText
BOOL CXListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = CListCtrl::SetItemText(nItem, nSubItem, lpszText);

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

BOOL CXListCtrl::SetItemBackColor(int nItem, int nSubItem, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
	//	pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
		pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

BOOL CXListCtrl::SetItemTextColor(int nItem, int nSubItem, COLORREF crText)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
	//	pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}


///////////////////////////////////////////////////////////////////////////////
// SetItemText
//
// This function will set the text and colors for a subitem.  If lpszText
// is NULL, only the colors will be set.  If a color value is -1, the display
// color will be set to the default Windows color.
//
BOOL CXListCtrl::SetItemText(int nItem, int nSubItem, LPCTSTR lpszText,
					COLORREF crText, COLORREF crBackground)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	BOOL rc = TRUE;

	if (nItem < 0)
		return FALSE;

	if (lpszText)
		rc = CListCtrl::SetItemText(nItem, nSubItem, lpszText);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		pXLCD[nSubItem].crText       = (crText == -1) ? m_crWindowText : crText;
		pXLCD[nSubItem].crBackground = (crBackground == -1) ? m_crWindow : crBackground;
	}

	UpdateSubItem(nItem, nSubItem);

	return rc;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteItem
BOOL CXListCtrl::DeleteItem(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
		delete [] pXLCD;
	CListCtrl::SetItemData(nItem, 0);
	return CListCtrl::DeleteItem(nItem);
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllItems
BOOL CXListCtrl::DeleteAllItems()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete [] pXLCD;
		CListCtrl::SetItemData(i, 0);
	}

	return CListCtrl::DeleteAllItems();
}

///////////////////////////////////////////////////////////////////////////////
// OnDestroy
void CXListCtrl::OnDestroy()
{
	int n = GetItemCount();
	for (int i = 0; i < n; i++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(i);
		if (pXLCD)
			delete [] pXLCD;
		CListCtrl::SetItemData(i, 0);
	}

	CListCtrl::OnDestroy();
}

///////////////////////////////////////////////////////////////////////////////
// SetProgress
//
// This function creates a progress bar in the specified subitem.  The
// UpdateProgress function may then be called to update the progress
// percent.  If bShowProgressText is TRUE, either the default text
// of "n%" or the custom percent text (lpszProgressText) will be
// displayed.  If bShowProgressText is FALSE, only the progress bar
// will be displayed, with no text.
//
// Note that the lpszProgressText string should include the format
// specifier "%d":  e.g., "Pct %d%%"
//
BOOL CXListCtrl::SetProgress(int nItem,
							 int nSubItem,
							 BOOL bShowProgressText /*= TRUE*/,
							 LPCTSTR lpszProgressText /*= NULL*/)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[nSubItem].bShowProgress        = TRUE;
	pXLCD[nSubItem].nProgressPercent     = 0;
	pXLCD[nSubItem].bShowProgressMessage = bShowProgressText;
	pXLCD[nSubItem].strProgressMessage   = lpszProgressText;

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteProgress
void CXListCtrl::DeleteProgress(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return;
	}

	pXLCD[nSubItem].bShowProgress = FALSE;
	pXLCD[nSubItem].nProgressPercent = 0;

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// UpdateProgress
void CXListCtrl::UpdateProgress(int nItem, int nSubItem, int nPercent)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return;

	ASSERT(nPercent >= 0 && nPercent <= 100);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return;
	}

	pXLCD[nSubItem].nProgressPercent = nPercent;

	UpdateSubItem(nItem, nSubItem);
}

///////////////////////////////////////////////////////////////////////////////
// SetCheckbox
BOOL CXListCtrl::SetCheckbox(int nItem, int nSubItem, int nCheckedState)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	ASSERT(nCheckedState == 0 || nCheckedState == 1 || nCheckedState == -1);

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update checkbox in subitem

	pXLCD[nSubItem].nCheckedState = nCheckedState;

	UpdateSubItem(nItem, nSubItem);

	// now update checkbox in column header

	// -1 = no checkbox in column header
	if (GetHeaderCheckedState(nSubItem) != XHEADERCTRL_NO_IMAGE)
	{
		int nCheckedCount = CountCheckedItems(nSubItem);

		if (nCheckedCount == GetItemCount())
			SetHeaderCheckedState(nSubItem, XHEADERCTRL_CHECKED_IMAGE);
		else
			SetHeaderCheckedState(nSubItem, XHEADERCTRL_UNCHECKED_IMAGE);
	}

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetCheckbox
int CXListCtrl::GetCheckbox(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return -1;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return -1;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return -1;
	}

	return pXLCD[nSubItem].nCheckedState;
}

///////////////////////////////////////////////////////////////////////////////
// GetEnabled
//
// Note that GetEnabled and SetEnabled only Get/Set the enabled flag from
// subitem 0, since this is a per-row flag.
//
BOOL CXListCtrl::GetEnabled(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	return pXLCD[0].bEnabled;
}

///////////////////////////////////////////////////////////////////////////////
// SetEnabled
BOOL CXListCtrl::SetEnabled(int nItem, BOOL bEnable)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[0].bEnabled = bEnable;

	CRect rect;
	GetItemRect(nItem, &rect, LVIR_BOUNDS);
	InvalidateRect(&rect);
	UpdateWindow();

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// SetBold
BOOL CXListCtrl::SetBold(int nItem, int nSubItem, BOOL bBold)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update bold flag
	pXLCD[nSubItem].bBold = bBold;

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetBold
BOOL CXListCtrl::GetBold(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update bold flag
	return pXLCD[nSubItem].bBold;
}

///////////////////////////////////////////////////////////////////////////////
// SetComboBox
//
// Note:  SetItemText may also be used to set the initial combo selection.
//
BOOL CXListCtrl::SetComboBox(int nItem,
							 int nSubItem,
							 BOOL bEnableCombo,
							 CStringArray *psa,
							 int nComboListHeight,
							 int nInitialComboSel)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	ASSERT(psa);
	if (!psa)
		return FALSE;
	ASSERT(nComboListHeight > 0);
	ASSERT(nInitialComboSel >= 0 && nInitialComboSel < psa->GetSize());
	if ((nInitialComboSel < 0) || (nInitialComboSel >= psa->GetSize()))
		nInitialComboSel = 0;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	// update flag
	pXLCD[nSubItem].bCombo = bEnableCombo;

	if (bEnableCombo)
	{
		pXLCD[nSubItem].psa = psa;
		pXLCD[nSubItem].nComboListHeight = nComboListHeight;
		pXLCD[nSubItem].nInitialComboSel = nInitialComboSel;

		if (pXLCD[nSubItem].psa)
		{
			int index = 0;
			if ((pXLCD[nSubItem].nInitialComboSel >= 0) &&
				(pXLCD[nSubItem].psa->GetSize() > pXLCD[nSubItem].nInitialComboSel))
			{
				index = pXLCD[nSubItem].nInitialComboSel;
				CString str;
				str = pXLCD[nSubItem].psa->GetAt(index);
				SetItemText(nItem, nSubItem, str);
			}
		}
	}

	UpdateSubItem(nItem, nSubItem);

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetComboText
//
// Actually this does nothing more than GetItemText()
//
CString	CXListCtrl::GetComboText(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return _T("");
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return _T("");

	CString str;
	str = _T("");

	str = GetItemText(nItem, nSubItem);

	return str;
}

///////////////////////////////////////////////////////////////////////////////
// SetCurSel
BOOL CXListCtrl::SetCurSel(int nItem)
{

	int n = GetCurSel();
	if ( n!=-1 )
		 SetItemState(n, 0, LVIS_FOCUSED | LVIS_SELECTED);

	BOOL bRet = SetItemState(nItem, LVIS_FOCUSED | LVIS_SELECTED,
		LVIS_FOCUSED | LVIS_SELECTED);

	CRect rcItem;
	CRect rcClient;

	if ( bRet )
	{
		//GetClientRect(&rcClient);
		//GetItemRect(nItem, &rcItem, LVIR_BOUNDS);

		//CRect rc1;
		//rc1.IntersectRect(&rcItem,&rcClient);

		//if ( rc1.IsRectNull() )
		//{
		//	int nItemHeight = rcItem.Height();

		//	int nTop1 = rcClient.top+(rcClient.Height()-nItemHeight)/2;
		//	int nTop2 = nTop1+nItemHeight;

		//	while ( rcItem.top>nTop2 || rcItem.bottom<nTop1 )
		//	{
		//		this->Scroll(
		//		GetItemRect(nItem, &rcItem, LVIR_BOUNDS);
		//	}
		//}
		this->EnsureVisible(nItem,FALSE);
	}

	return bRet;
}

///////////////////////////////////////////////////////////////////////////////
// GetCurSel - returns selected item number, or -1 if no item selected
//
// Note:  for single-selection lists only
//
int CXListCtrl::GetCurSel()
{
	POSITION pos = GetFirstSelectedItemPosition();
	int nSelectedItem = -1;
	if (pos != NULL)
		nSelectedItem = GetNextSelectedItem(pos);
	return nSelectedItem;
}

///////////////////////////////////////////////////////////////////////////////
// UpdateSubItem
void CXListCtrl::UpdateSubItem(int nItem, int nSubItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	//if ((nItem < 0) || nItem >= GetItemCount())
	//	return;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	//if ((nSubItem < 0) || nSubItem >= GetColumns())
	//	return;

	CRect rect;
	if (nSubItem == -1)
	{
		GetItemRect(nItem, &rect, LVIR_BOUNDS);
	}
	else
	{
		GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	}

	InvalidateRect(&rect);
	UpdateWindow();
}

///////////////////////////////////////////////////////////////////////////////
// GetColumns
int CXListCtrl::GetColumns()
{
	return GetHeaderCtrl()->GetItemCount();
}

///////////////////////////////////////////////////////////////////////////////
// GetItemData
//
// The GetItemData and SetItemData functions allow for app-specific data
// to be stored, by using an extra field in the XLISTCTRLDATA struct.
//
DWORD CXListCtrl::GetItemData(int nItem)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return 0;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return 0;
	}

	return pXLCD->dwItemData;
}

///////////////////////////////////////////////////////////////////////////////
// SetItemData
BOOL CXListCtrl::SetItemData(int nItem, DWORD dwData)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD->dwItemData = dwData;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetHeaderCheckedState
//
// The GetHeaderCheckedState and SetHeaderCheckedState may be used to toggle
// the checkbox in a column header.
//     0 = no checkbox
//     1 = unchecked
//     2 = checked
//
int CXListCtrl::GetHeaderCheckedState(int nSubItem)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return -1;

	HDITEM hditem;

	// use the image index (0 or 1) to indicate the checked status
	hditem.mask = HDI_IMAGE;
	m_HeaderCtrl.GetItem(nSubItem, &hditem);
	return hditem.iImage;
}

///////////////////////////////////////////////////////////////////////////////
// SetHeaderCheckedState
BOOL CXListCtrl::SetHeaderCheckedState(int nSubItem, int nCheckedState)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;
	ASSERT(nCheckedState == 0 || nCheckedState == 1 || nCheckedState == 2);

	HDITEM hditem;

	hditem.mask = HDI_IMAGE;
	hditem.iImage = nCheckedState;
	m_HeaderCtrl.SetItem(nSubItem, &hditem);

	return TRUE;
}

/////////////////////////////////////////////////////////////////////////////////
// SetSubItemButton
void CXListCtrl::CreateItemButton( int nItem, int nSubItem, HWND hMain )
{
	CRect rect;
	int offset =0;

	// Make sure that the item is visible
	if( !EnsureVisible(nItem, TRUE)) 
		return ;

	GetSubItemRect(nItem, nSubItem, LVIR_BOUNDS, rect);
	offset = rect.Width()-30;
	// Now scroll if we need to expose the column
	CRect rcClient;
	GetClientRect(rcClient);
	if( offset + rect.left < 0 || offset + rect.left > rcClient.right )
	{
		CSize size;
		size.cx = offset + rect.left;
		size.cy = 0;
		Scroll(size);
		rect.left -= size.cx;
	}

	rect.left += offset;	
	rect.right = rect.left + 30;/*GetColumnWidth(nSubItem)*/
	if(rect.right > rcClient.right) 
		rect.right = rcClient.right;
	//basic code end

	rect.bottom = rect.top + rect.Height();

	int iPageCout = GetCountPerPage();
	if ( nItem >= iPageCout )
	{
		rect.top += rect.Height();
		rect.bottom += rect.Height();
	}

	DWORD dwStyle =  WS_CHILD | WS_VISIBLE;
	CButtonEx *pButton = new CButtonEx(nItem,nSubItem,rect,hMain);
	m_uID++;
	pButton->Create(_T("+"),dwStyle, rect, this, m_uID);
	//CBitmap bitmap;
	//bitmap.LoadBitmap(IDB_HELP_NORMAL);
	//pButton->SetBitmap(bitmap);
	m_mButton.insert( make_pair( nItem, pButton ) );

	int iTopIndex = GetTopIndex();
	if ( iTopIndex > 0 )
	{
		updateListCtrlButtonPos();
	}
	delete pButton;
	return;
}

void CXListCtrl::release()
{
	button_map::iterator iter;
	iter = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		delete iter->second;
		iter->second = NULL;
		iter++;
	}
}


void CXListCtrl::updateListCtrlButtonPos()
{
	int iTopIndex = GetTopIndex();
	int nItem = iTopIndex;
	button_map::iterator iter;
	button_map::iterator iterUp;
	int iLine = 0;
#ifdef USE_TOPINDEX_BUTTON
	iter = m_mButton.find( iTopIndex );
	iterUp = m_mButton.begin();
	while ( iter != m_mButton.end() )
	{
		iterUp->second->EnableWindow( iter->second->bEnable );
		iter ++;
		iterUp++;
	}
#else
	for ( ; nItem < GetItemCount(); nItem++ )
	{
		iter = m_mButton.find(nItem);
		if( iter!= m_mButton.end() )
		{
			CRect rect;
			iterUp = m_mButton.find(iLine);
			rect = iterUp->second->m_rect;
			iter->second->MoveWindow( &rect );
			iter->second->ShowWindow( SW_SHOW );
			if( iLine < iTopIndex )
			{
				iterUp->second->ShowWindow( SW_HIDE );
			}
			iLine++;
		}
	}
#endif

}
/////////////////////////////////////////////////////////////////////////////////
//// OnColumnClick
//BOOL CXListCtrl::OnColumnClick(NMHDR* pNMHDR, LRESULT* pResult)
//{
//	NMLISTVIEW* pnmlv = (NMLISTVIEW*)pNMHDR;
//
//	int nSubItem = pnmlv->iSubItem;
//
//	int nCheckedState = GetHeaderCheckedState(nSubItem);
//
//	// 0 = no checkbox
//	if (nCheckedState != XHEADERCTRL_NO_IMAGE)
//	{
//		nCheckedState = (nCheckedState == 1) ? 2 : 1;
//		SetHeaderCheckedState(nSubItem, nCheckedState);
//
//		m_HeaderCtrl.UpdateWindow();
//
//		for (int nItem = 0; nItem < GetItemCount(); nItem++)
//		{
//			XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
//			if (!pXLCD)
//			{
//				continue;
//			}
//
//			if (pXLCD[nSubItem].nCheckedState != -1)
//			{
//				pXLCD[nSubItem].nCheckedState = nCheckedState - 1;
//				UpdateSubItem(nItem, nSubItem);
//			}
//		}
//	}
//
//	*pResult = 0;
//	return FALSE;		// return FALSE to send message to parent also -
//						// NOTE:  MSDN documentation is incorrect
//}

///////////////////////////////////////////////////////////////////////////////
// CountCheckedItems
int CXListCtrl::CountCheckedItems(int nSubItem)
{
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return 0;

	int nCount = 0;

	for (int nItem = 0; nItem < GetItemCount(); nItem++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (!pXLCD)
		{
			continue;
		}

		if (pXLCD[nSubItem].nCheckedState == 1)
			nCount++;
	}

	return nCount;
}

///////////////////////////////////////////////////////////////////////////////
// OnSysColorChange
void CXListCtrl::OnSysColorChange()
{
	TRACE(_T("in CXListCtrl::OnSysColorChange\n"));

	CListCtrl::OnSysColorChange();

	m_cr3DFace        = ::GetSysColor(COLOR_3DFACE);
	m_cr3DHighLight   = ::GetSysColor(COLOR_3DHIGHLIGHT);
	m_cr3DShadow      = ::GetSysColor(COLOR_3DSHADOW);
	m_crBtnFace       = ::GetSysColor(COLOR_BTNFACE);
	m_crBtnShadow     = ::GetSysColor(COLOR_BTNSHADOW);
	m_crBtnText       = ::GetSysColor(COLOR_BTNTEXT);
	m_crGrayText      = ::GetSysColor(COLOR_GRAYTEXT);
	m_crHighLight     = ::GetSysColor(COLOR_HIGHLIGHT);
	m_crHighLightText = ::GetSysColor(COLOR_HIGHLIGHTTEXT);
	m_crWindow        = ::GetSysColor(COLOR_WINDOW);
	m_crWindowText    = ::GetSysColor(COLOR_WINDOWTEXT);
}

///////////////////////////////////////////////////////////////////////////////
// OnToolHitTest
int CXListCtrl::OnToolHitTest(CPoint point, TOOLINFO * pTI) const
{
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt = point;
	
	int nItem = ListView_SubItemHitTest(this->m_hWnd, &lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
	TRACE(_T("in CToolTipListCtrl::OnToolHitTest: %d,%d\n"), nItem, nSubItem);

	UINT nFlags = lvhitTestInfo.flags;

	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL)
	{
		// If it did fall on a list item,
		// and it was also hit one of the
		// item specific subitems we wish to show tool tips for
		
		// get the client (area occupied by this control
		RECT rcClient;
		GetClientRect(&rcClient);
		
		// fill in the TOOLINFO structure
		pTI->hwnd = m_hWnd;
		pTI->uId = (UINT) (nItem * 1000 + nSubItem + 1);
		pTI->lpszText = LPSTR_TEXTCALLBACK;
		pTI->rect = rcClient;
		
		return pTI->uId;	// By returning a unique value per listItem,
							// we ensure that when the mouse moves over another
							// list item, the tooltip will change
	}
	else
	{
		//Otherwise, we aren't interested, so let the message propagate
		return -1;
	}
}

///////////////////////////////////////////////////////////////////////////////
// OnToolTipText
BOOL CXListCtrl::OnToolTipText(UINT /*id*/, NMHDR * pNMHDR, LRESULT * pResult)
{
	UINT nID = pNMHDR->idFrom;
	TRACE(_T("in CXListCtrl::OnToolTipText: id=%d\n"), nID);
	
	// check if this is the automatic tooltip of the control
	if (nID == 0) 
		return TRUE;	// do not allow display of automatic tooltip,
						// or our tooltip will disappear
	
	// handle both ANSI and UNICODE versions of the message
	TOOLTIPTEXTA* pTTTA = (TOOLTIPTEXTA*)pNMHDR;
	TOOLTIPTEXTW* pTTTW = (TOOLTIPTEXTW*)pNMHDR;
	
	*pResult = 0;
	
	// get the mouse position
	const MSG* pMessage;
	pMessage = GetCurrentMessage();
	ASSERT(pMessage);
	CPoint pt;
	pt = pMessage->pt;		// get the point from the message
	ScreenToClient(&pt);	// convert the point's coords to be relative to this control
	
	// see if the point falls onto a list item
	
	LVHITTESTINFO lvhitTestInfo;
	
	lvhitTestInfo.pt = pt;
	
	int nItem = SubItemHitTest(&lvhitTestInfo);
	int nSubItem = lvhitTestInfo.iSubItem;
	
	UINT nFlags = lvhitTestInfo.flags;
	
	// nFlags is 0 if the SubItemHitTest fails
	// Therefore, 0 & <anything> will equal false
	if (nFlags & LVHT_ONITEMLABEL)
	{
		// If it did fall on a list item,
		// and it was also hit one of the
		// item specific subitems we wish to show tooltips for
		
		CString strToolTip;
		strToolTip = _T("");

		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (pXLCD)
		{
			strToolTip = pXLCD[nSubItem].strToolTip;
		}

		if (!strToolTip.IsEmpty())
		{
			// If there was a CString associated with the list item,
			// copy it's text (up to 80 characters worth, limitation 
			// of the TOOLTIPTEXT structure) into the TOOLTIPTEXT 
			// structure's szText member
			
#ifndef _UNICODE
			if (pNMHDR->code == TTN_NEEDTEXTA)
				lstrcpyn(pTTTA->szText, strToolTip, 80);
			else
				_mbstowcsz(pTTTW->szText, strToolTip, 80);
#else
			if (pNMHDR->code == TTN_NEEDTEXTA)
				_wcstombsz(pTTTA->szText, strToolTip, 80);
			else
				lstrcpyn(pTTTW->szText, strToolTip, 80);
#endif
			return FALSE;	 // we found a tool tip,
		}
	}
	
	return FALSE;	// we didn't handle the message, let the 
					// framework continue propagating the message
}

///////////////////////////////////////////////////////////////////////////////
// SetItemToolTipText
BOOL CXListCtrl::SetItemToolTipText(int nItem, int nSubItem, LPCTSTR lpszToolTipText)
{
	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return FALSE;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return FALSE;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (!pXLCD)
	{
		return FALSE;
	}

	pXLCD[nSubItem].strToolTip = lpszToolTipText;

	return TRUE;
}

///////////////////////////////////////////////////////////////////////////////
// GetItemToolTipText
CString CXListCtrl::GetItemToolTipText(int nItem, int nSubItem)
{
	CString strToolTip;
	strToolTip = _T("");

	ASSERT(nItem >= 0);
	ASSERT(nItem < GetItemCount());
	if ((nItem < 0) || nItem >= GetItemCount())
		return strToolTip;
	ASSERT(nSubItem >= 0);
	ASSERT(nSubItem < GetColumns());
	if ((nSubItem < 0) || nSubItem >= GetColumns())
		return strToolTip;

	XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
	if (pXLCD)
	{
		strToolTip = pXLCD[nSubItem].strToolTip;
	}

	return strToolTip;
}

///////////////////////////////////////////////////////////////////////////////
// DeleteAllToolTips
void CXListCtrl::DeleteAllToolTips()
{
	int nRow = GetItemCount();
	int nCol = GetColumns();

	for (int nItem = 0; nItem < nRow; nItem++)
	{
		XLISTCTRLDATA *pXLCD = (XLISTCTRLDATA *) CListCtrl::GetItemData(nItem);
		if (pXLCD)
			for (int nSubItem = 0; nSubItem < nCol; nSubItem++)
				pXLCD[nSubItem].strToolTip = _T("");
	}
}

BOOL CXListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rect;
	GetClientRect(&rect);
	int nItems = this->GetItemCount();

	static COLORREF clrBack = RGB(255,249,242);

	Default();

	if ( nItems>0 )
	{
		CRect rectView,rectItem;
		this->GetItemRect(0,&rectItem,LVIR_BOUNDS);
		rectView.top = rectItem.top+2;
		rectView.left = rectItem.left;
		rectView.bottom = rectItem.top+rectItem.Height()*nItems;
		
		GetItemRect(GetColumns()-1,&rectItem,LVIR_BOUNDS);
		rectView.right = rectItem.right;
	
		CRgn rgn1,rgn2,rgn3;
		rgn1.CreateRectRgn(rect.left,rect.top,rect.right,rect.bottom);
		rgn2.CreateRectRgn(rectView.left,rectView.top,rectView.right,rectView.bottom);
		rgn3.CreateRectRgn(0,0,0,0);
		rgn3.CombineRgn(&rgn1,&rgn2,RGN_XOR);
		pDC->SelectObject(&rgn3);
		pDC->FillSolidRect(&rect,clrBack);
		pDC->SelectObject(&rgn1);		
	}
	else
	{
		pDC->FillSolidRect(&rect,clrBack);
	}

	return TRUE;
}

void CXListCtrl::SetColumnStatus(int nCol, XColumnStatus status)
{
	HDITEM hditem;
	hditem.mask = HDI_FORMAT;
	m_HeaderCtrl.GetItem( nCol, &hditem );

	if ( status==xcsArrowDown || status==xcsArrowUp )
	{	
		hditem.mask = HDI_BITMAP | HDI_FORMAT;
		hditem.fmt |= HDF_BITMAP | HDF_BITMAP_ON_RIGHT;

		if ( status==xcsArrowUp )
			hditem.hbm = m_hArrowUp;
		else
			hditem.hbm = m_hArrowDown;
	}
	else
	{
		hditem.mask = HDI_FORMAT;
		hditem.fmt &= ~HDF_BITMAP;
	}

	m_HeaderCtrl.SetItem( nCol, &hditem );
}


XColumnStatus CXListCtrl::GetColumnStatus(int nCol)
{
	HDITEM hditem;
	hditem.mask = HDI_FORMAT|HDI_BITMAP;
	m_HeaderCtrl.GetItem( nCol, &hditem );

	if ( hditem.fmt & HDF_BITMAP )
	{
		if ( hditem.hbm==m_hArrowDown )
			return xcsArrowDown;
		else
		if ( hditem.hbm==m_hArrowUp)
			return xcsArrowUp;
		else
			return xcsNormal;
	}
	else
	{
		return xcsNormal;
	}
}

void CXListCtrl::SwapLine(int nLine1, int nLine2, int nColumns)
{
	if ( nLine1!=nLine2 )
	{
		CString  s;

		XLISTCTRLDATA *pXLCD1 = (XLISTCTRLDATA *) CListCtrl::GetItemData(nLine1);
		XLISTCTRLDATA *pXLCD2 = (XLISTCTRLDATA *) CListCtrl::GetItemData(nLine2);

		COLORREF c1 = pXLCD1->crBackground;
		COLORREF c2 = pXLCD2->crBackground;
	
		DWORD dwData  = CListCtrl::GetItemData(nLine1);

		CListCtrl::SetItemData( nLine1, CListCtrl::GetItemData(nLine2) );
		CListCtrl::SetItemData( nLine2, dwData );

		for ( int i=0; i<nColumns; i++ )
		{
			s = GetItemText(nLine1,i );
			CListCtrl::SetItemText(nLine1, i, GetItemText(nLine2, i ));
			CListCtrl::SetItemText(nLine2, i, s);
			this->SetItemBackColor(nLine1,i,c1);
			this->SetItemBackColor(nLine2,i,c2);
		}

	}
}