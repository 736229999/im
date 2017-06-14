/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 9/13/98 12:24:50
  Comments: FloorPageObject.h: interface for the CFloorPageObject class.
 ************************************/

#if !defined(AFX_FLOORPAGEOBJECT_H__FF4DABE5_4A17_11D2_868E_0040055C08D9__INCLUDED_)
#define AFX_FLOORPAGEOBJECT_H__FF4DABE5_4A17_11D2_868E_0040055C08D9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

#include "FloorObject.h"

class CFloorPageObject : public CFloorObject  
{
	friend class CXFloorWndCtrl;
	DECLARE_DYNAMIC(CFloorPageObject)
public:
	virtual void EndScroll();
	virtual void PrepareToScroll();
	virtual CWnd* GetWindow();
	virtual void Detach();
	virtual void Attach(CWnd* pWnd);
	// Overidable functions
	virtual void OnDraw(CDC* pDC, BOOL bEraseBkGnd = FALSE);
	virtual void OnFocus();
	virtual void OnLButtonDown();
	virtual void SetRectClient(CRect rect);
	virtual void OnDeactivateObject();	
	virtual void OnActivateObject();
	// Constructors and Destructors;
	CFloorPageObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszPageName);
	virtual ~CFloorPageObject();

protected:
	CWnd* m_pOldParent;
	CWnd* m_pWndClient;
	static BOOL m_bAutoDetach;
};

#endif // !defined(AFX_FLOORPAGEOBJECT_H__FF4DABE5_4A17_11D2_868E_0040055C08D9__INCLUDED_)
