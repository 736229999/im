/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 11/15/98 9:42:21 PM
  Comments: FloorLabeObject.cpp: implementation of the CFloorLabeObject class.
 ************************************/

#include "stdafx.h"
#include "FloorLabeObject.h"
#include "XFloorWndCtl.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

IMPLEMENT_DYNAMIC(CFloorLabelObject, CFloorPageObject)

// Function name	: CFloorLabelObject::CFloorLabelObject
// Description	    : constuctor
// Return type		: 
// Argument         : CXFloorWndCtrl* pWnd
// Argument         : LPCTSTR lpszPageName
CFloorLabelObject::CFloorLabelObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszPageName):
	CFloorPageObject(pWnd, lpszPageName)
{
	NewPage(lpszPageName);
}

// Function name	: CFloorLabelObject::NewPage
// Description	    : Add a new page into combo
// Return type		: void 
// Argument         : LPCTSTR lpszPageName
void CFloorLabelObject::NewPage(LPCTSTR lpszPageName)
{
	int nItem = GetComboBox()->AddString(lpszPageName);
	GetComboBox()->SetItemData(nItem, (DWORD)this);
}


// Function name	: CFloorLabelObject::~CFloorLabelObject
// Description	    : virtual destructor
// Return type		: 
CFloorLabelObject::~CFloorLabelObject()
{
}

// Function name	: CFloorLabelObject::OnDraw
// Description	    : 
// Return type		: void 
// Argument         : CDC* pDC
// Argument         : BOOL bEraseBkGnd = FALSE
void CFloorLabelObject::OnDraw(CDC* pDC, BOOL bEraseBkGnd)
{
	// Do not implement this
}

// Function name	: CFloorLabelObject::SetName
// Description	    : Rename the page
// Return type		: void 
// Argument         : LPCTSTR lpszObjectName
void CFloorLabelObject::SetName(LPCTSTR lpszObjectName)
{
	ASSERT (m_pParentWnd);
	int iItem = m_pParentWnd->Lookup(this);
	if (iItem >= 0)
	{
		// The combobox need a rename functions
		GetComboBox()->DeleteString(iItem);
		NewPage(lpszObjectName);
		if (m_pParentWnd->GetActivePage() == this)
			GetLabelControl()->SetWindowText(lpszObjectName);
		CFloorPageObject::SetName(lpszObjectName);
	}
}

// Function name	: CFloorLabelObject::OnActivateObject
// Description	    : When you activate this select into combo the new page
// Return type		: void 
void CFloorLabelObject::OnActivateObject()
{
	CFloorPageObject::OnActivateObject();
	ASSERT (m_pParentWnd);
	int iItem = m_pParentWnd->Lookup(this);
	if (iItem >= 0)
	{
		GetComboBox()->SetCurSel(iItem);
		CString sItem; GetComboBox()->GetLBText(iItem, sItem);
		GetLabelControl()->SetWindowText(sItem);
	}
}

// Function name	: CFloorLabelObject::GetComboBox
// Description	    : return the combobox
// Return type		: CComboBox* 
CComboBox* CFloorLabelObject::GetComboBox()
{
	ASSERT (m_pParentWnd);
	return m_pParentWnd->GetComboBox();
}

// Function name	: CFloorLabelObject::GetLabelControl
// Description	    : return the label control...
// Return type		: const CTLabelComboBox* 
CTLabelComboBox* CFloorLabelObject::GetLabelControl()
{
	ASSERT (m_pParentWnd);
	return m_pParentWnd->GetLabelControl();
}

// Function name	: CFloorLabelObject::EndScroll
// Description	    : End scroll, refresh control
// Return type		: void 
void CFloorLabelObject::EndScroll()
{
	if (GetComboBox())
	{
		GetComboBox()->ModifyStyleEx(WS_EX_TRANSPARENT, 0);
		GetLabelControl()->Invalidate();
	}
}

// Function name	: CFloorLabelObject::PrepareToScroll
// Description	    : begin to scroll
// Return type		: void 
void CFloorLabelObject::PrepareToScroll()
{
	if (GetComboBox())
		GetComboBox()->ModifyStyleEx(0, WS_EX_TRANSPARENT);
}
