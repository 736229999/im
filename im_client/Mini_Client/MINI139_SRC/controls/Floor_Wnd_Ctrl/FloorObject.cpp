/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 9/12/98 2:17:20 PM
  Comments: FloorObject.cpp: implementation of the CFloorObject class.
 ************************************/

#include "stdafx.h"
#include "FloorObject.h"
#include "XFloorWndCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

IMPLEMENT_DYNAMIC(CFloorObject,CObject)

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////
// Function name	: CFloorObject::CFloorObject
// Description	    : Default constructor
// Return type		: 
CFloorObject::CFloorObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszObjectName):m_sName(lpszObjectName),m_brBkGnd(defaultBkGnd)
{
	m_pParentWnd = pWnd;
	m_rect.SetRectEmpty(); // this coordonate is in client coordonate
	m_bPullUp = TRUE; // By default, object is putting up in floor window
	m_nHeight = defaultHeight; //Do not call SetHeight
	m_nFormatDrawText = DT_LEFT | DT_SINGLELINE | DT_VCENTER;
	m_lParam = NULL;
}

// Function name	: CFloorObject::~CFloorObject
// Description	    : virtual Destructor
// Return type		: 
CFloorObject::~CFloorObject()
{
}

// Function name	: CFloorObject::Invalidate
// Description	    : Call to invalidate the page
// Return type		: void 
// Argument         : BOOL bEraseBkGnd
void CFloorObject::Invalidate(BOOL bEraseBkGnd)
{
	if (m_pParentWnd)
		if (::IsWindow(m_pParentWnd->m_hWnd))
		{
			CDC* pDC = m_pParentWnd->GetDC();
			OnDraw(pDC, bEraseBkGnd);
			m_pParentWnd->ReleaseDC(pDC);
		}
}

// Function name	: CFloorObject::SetName
// Description	    : Set the new name of this object.
// Return type		: void 
// Argument         : LPCTSTR lpszObjectName
void CFloorObject::SetName(LPCTSTR lpszObjectName)
{
	m_sName = CString(lpszObjectName);
	Invalidate(TRUE);
}

// Function name	: CFloorObject::GetName
// Description	    : Return the name of page
// Return type		: const CString&
const CString& CFloorObject::GetName()
{
	return m_sName;
}

// Function name	: CFloorObject::IsPullUp
// Description	    : return true if this object is up.
// Return type		: const BOOL 
const BOOL CFloorObject::IsPullUp()
{
	return m_bPullUp;
}

// Function name	: CFloorObject::IsPullDown
// Description	    : return TRUE if the object is puting down
// Return type		: const BOOL 
const BOOL CFloorObject::IsPullDown()
{
	return !IsPullUp();
}

// Function name	: CFloorObject::SetHeight
// Description	    : Call this function to set the height of object
// Return type		: int 
// Argument         : int nHeight
int CFloorObject::SetHeight(int nHeight)
{
	int sNHeight = m_nHeight;
	m_nHeight = nHeight;
	m_pParentWnd->RecalcLayout();
	SetRectClient(m_pParentWnd->GetRectClient());
	return sNHeight;
}

// Function name	: CFloorObject::GetHeight
// Description	    : Return the height
// Return type		: const int 
const int CFloorObject::GetHeight()
{
	return m_nHeight;
}

// Function name	: CFloorObject::SetBkGnd
// Description	    : Set the new RGB form bkgnd of page
// Return type		: COLORREF 
// Argument         : COLORREF rgbBkGnd
COLORREF CFloorObject::SetBkGnd(COLORREF rgbBkGnd)
{
	LOGBRUSH logBrush; m_brBkGnd.GetLogBrush(&logBrush);
	m_brBkGnd.DeleteObject();
	m_brBkGnd.CreateSolidBrush(rgbBkGnd);
	Invalidate(TRUE);
	return logBrush.lbColor;
}

// Function name	: CFloorObject::IsPointInObject
// Description	    : Return TRUE if point is in m_rect
// Return type		: BOOL 
// Argument         : CPoint point
BOOL CFloorObject::PtInObject(CPoint point)
{
	return m_rect.PtInRect(point);
}

LPARAM CFloorObject::SetUserData(LPARAM lParam)
{
	LPARAM lResult = m_lParam;
	m_lParam = lParam;
	return lResult;
}

const LPARAM CFloorObject::GetUserData()
{
	return m_lParam;
}
