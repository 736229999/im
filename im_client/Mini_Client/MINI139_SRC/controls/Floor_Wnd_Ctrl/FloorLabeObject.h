/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 11/15/98 9:42:14 PM
  Comments: FloorLabeObject.h: interface for the CFloorLabeObject class.
 ************************************/

#if !defined(AFX_FLOORLABEOBJECT_H__8EB6C475_7CB5_11D2_8702_0040055C08D9__INCLUDED_)
#define AFX_FLOORLABEOBJECT_H__8EB6C475_7CB5_11D2_8702_0040055C08D9__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include "FloorPageObject.h"
#include "TLabelComboBox.h"

class CFloorLabelObject : public CFloorPageObject  
{
	DECLARE_DYNCREATE(CFloorLabelObject)
public:
	virtual CTLabelComboBox* GetLabelControl();
	virtual CComboBox* GetComboBox();
	//Constructor & destructor
	CFloorLabelObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszPageName);
	virtual ~CFloorLabelObject();
	// Overidable functions ...
	virtual void OnDraw(CDC* pDC, BOOL bEraseBkGnd = FALSE);
	virtual void SetName(LPCTSTR lpszObjectName);
	virtual void EndScroll();
	virtual void PrepareToScroll();
	virtual void OnActivateObject();
protected:
	virtual void NewPage(LPCTSTR lpszPageName);
};

#endif // !defined(AFX_FLOORLABEOBJECT_H__8EB6C475_7CB5_11D2_8702_0040055C08D9__INCLUDED_)
