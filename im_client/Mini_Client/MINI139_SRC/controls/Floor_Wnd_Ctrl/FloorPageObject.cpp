/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 9/12/98 2:22:50 PM
  Comments: FloorPageObject.cpp: implementation of the CFloorPageObject class.
 ************************************/

#include "stdafx.h"
#include "FloorPageObject.h"
#include "XFloorWndCtl.h"
#include "..\..\resource.h"
#include "resgdiobj.h"
//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFloorPageObject, CFloorObject)

// Function name	: CFloorPageObject::CFloorPageObject
// Description	    : default Contructor
// Return type		: 
CFloorPageObject::CFloorPageObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszPageName):CFloorObject(pWnd, lpszPageName)
{
	m_pWndClient = NULL;
	m_pOldParent = NULL;
}

BOOL CFloorPageObject::m_bAutoDetach = TRUE;
// Function name	: CFloorPageObject::~CFloorPageObject
// Description	    : virtual destructor
// Return type		: 
CFloorPageObject::~CFloorPageObject()
{
	if (m_bAutoDetach)
		Detach();
}

// Function name	: CFloorPageObject::OnDraw
// Description	    : Call this function by m_pParentWnd->OnDraw function to draw itself
// Return type		: void 
// Argument         : CDC* pDC
// Argument         : BOOL bEraseBkGnd
void CFloorPageObject::OnDraw(CDC* pDC, BOOL bEraseBkGnd)
{
	if (GetHeight())
	{
		CPen    penBorder(PS_SOLID,1,RGB(169,204,240));

		CPen   *pOldpen   = pDC->SelectObject(&penBorder);
		CBrush *pBrush    = CBrush::FromHandle((HBRUSH)GetStockObject(NULL_BRUSH));//
		CBrush *pOldbrush = pDC->SelectObject(pBrush);
	    CFont  *pOldfont  = pDC->SelectObject(&g_fontText);

		int nOldmode      = pDC->SetBkMode(TRANSPARENT);

		bool bActive = (m_pParentWnd->GetActivePage() == this);

		CRect rDraw(m_rect);

		CRgn  rgnDraw;
		rgnDraw.CreateRoundRectRgn(rDraw.left,rDraw.top,rDraw.right,rDraw.bottom,3,3);

		pDC->SelectObject(&rgnDraw);

		CResBitmap bmpBar;

		if ( bActive )
			bmpBar.LoadBitmap(IDB_FLOOR_HOT);
		else
			bmpBar.LoadBitmap(IDB_FLOOR_NORMAL);

		CDC dcTemp;

		dcTemp.CreateCompatibleDC(pDC);
		CBitmap *pOldbmp = dcTemp.SelectObject(&bmpBar);

		BITMAP bi;
		bmpBar.GetBitmap(&bi);

		pDC->StretchBlt(rDraw.left,rDraw.top,rDraw.Width(),rDraw.Height(),
			&dcTemp,0,0,bi.bmWidth,bi.bmHeight,SRCCOPY);
	
		pDC->RoundRect(rDraw.left,rDraw.top,rDraw.right-1,rDraw.bottom-1,3,3);

		pDC->SetTextColor(RGB(0,84,166));

		rDraw.left+=4;
		pDC->DrawText(m_sName, rDraw, m_nFormatDrawText);

		pDC->SetBkMode(nOldmode);
		pDC->SelectObject(pOldfont);
		pDC->SelectObject(pOldbrush);
		pDC->SelectObject(pOldpen);

		dcTemp.SelectObject(pOldbmp);
	}
}

// Function name	: CFloorPageObject::OnFocus
// Description	    : Called to draw a arrow in this 
// Return type		: void 
void CFloorPageObject::OnFocus()
{
	if (m_pParentWnd)
		if (::IsWindow(m_pParentWnd->m_hWnd))
			if (m_pParentWnd->m_bSign)
			{
				CDC* pDC = m_pParentWnd->GetDC();
				pDC->DrawIcon(m_rect.right - 32, (m_rect.top + m_rect.bottom) / 2 - 16, ResLoadIcon(IsPullUp() ? IDI_ICON_DOWN : IDI_ICON_UP));//AfxGetApp()->LoadIcon(IsPullUp() ? IDI_ICON_DOWN : IDI_ICON_UP));
				m_pParentWnd->ReleaseDC(pDC);
			}
}

void CFloorPageObject::OnLButtonDown()
{
}

// Function name	: CFloorPageObject::SetRectClient
// Description	    : 
// Return type		: void 
// Argument         : CRect rect
void CFloorPageObject::SetRectClient(CRect rect)
{
	if (m_pWndClient)
		m_pWndClient->MoveWindow(rect.left, rect.top, rect.Width(), rect.Height());
}

// Function name	: CFloorPageObject::OnDeactivateObject
// Description	    : Called before deactivate the object
// Return type		: void 
void CFloorPageObject::OnDeactivateObject()
{
	if (m_pWndClient)
		m_pWndClient->ShowWindow(SW_HIDE);
}

// Function name	: CFloorPageObject::OnActivateObject
// Description	    : Called after that object was activated
// Return type		: void 
void CFloorPageObject::OnActivateObject()
{
	if (m_pWndClient)
		m_pWndClient->ShowWindow(SW_SHOW);
}

// Function name	: CFloorPageObject::Attach
// Description	    : Attach a window to this page
// Return type		: void 
// Argument         : CWnd * pWnd
void CFloorPageObject::Attach(CWnd * pWnd)
{
	ASSERT (m_pParentWnd && ::IsWindow(m_pParentWnd->m_hWnd));
	ASSERT (pWnd && ::IsWindow(pWnd->m_hWnd));
	m_pOldParent = pWnd->GetParent();
	m_pParentWnd->ModifyStyle(0, WS_CLIPCHILDREN);
	m_pWndClient = pWnd;
	
	m_pWndClient->SetParent(m_pParentWnd);
	OnDeactivateObject();
	CFloorPageObject* pPage = m_pParentWnd->GetActivePage();
	if (pPage)
	{
		pPage->SetRectClient(m_pParentWnd->GetRectClient());
		pPage->OnActivateObject();
	}
}

// Function name	: CFloorPageObject::Detach
// Description	    : Dettach the window.
// Return type		: void 
void CFloorPageObject::Detach()
{
	if (m_pOldParent)
		if (::IsWindow(m_pOldParent->m_hWnd))
			m_pWndClient->SetParent(m_pOldParent);
	m_pOldParent = NULL;
	m_pWndClient = NULL;
	if (m_pParentWnd)
		if (::IsWindow(m_pParentWnd->m_hWnd))
			m_pParentWnd->Invalidate();
}

// Function name	: CFloorPageObject::GetWindow
// Description	    : 
// Return type		: CWnd* 
CWnd* CFloorPageObject::GetWindow()
{
	return m_pWndClient;
}

void CFloorPageObject::PrepareToScroll()
{
}

void CFloorPageObject::EndScroll()
{
}
