/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 8/23/98 19:33:54
  Comments: TLabelComboBox.cpp: implementation of the CTLabelComboBox class.
 ************************************/

#include "stdafx.h"
#include "TLabelComboBox.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

// Function name	: CTLabelComboBox::CTLabelComboBox
// Description	    : Default constructor
// Return type		: 
CTLabelComboBox::CTLabelComboBox()
{

}

// Function name	: CTLabelComboBox::~CTLabelComboBox
// Description	    : virtual destructor
// Return type		: 
CTLabelComboBox::~CTLabelComboBox()
{

}

// Function name	: CTLabelComboBox::GetEditControl
// Description	    : Get edit from combo
// Return type		: CWnd* 
CWnd* CTLabelComboBox::GetEditControl()
{
	CWnd* pWnd = GetWindowControl();
	switch (pWnd->GetStyle() & 0x3)
	{
		case CBS_SIMPLE:
		case CBS_DROPDOWN:
			return pWnd->GetDlgItem(1001);
			break;
		case CBS_DROPDOWNLIST:
			return pWnd;
			break;
	}
	return NULL;
}
