/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 9/12/98 2:17:04 PM
  Comments: FloorObject.h: interface for the CFloorObject class.
 ************************************/

#if !defined(AFX_FLOOROBJECT_H__FF4DABE4_4A17_11D2_868E_0040055C08D9__INCLUDED_)
#define AFX_FLOOROBJECT_H__FF4DABE4_4A17_11D2_868E_0040055C08D9__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000

class CFloorWnd;

#define defaultHeight 20
#define defaultBkGnd RGB(192,192,192)

// Abstract class
class CFloorObject : public CObject
{
	DECLARE_DYNAMIC(CFloorObject)
public:
	virtual const LPARAM GetUserData();
	virtual LPARAM SetUserData(LPARAM lParam);
	virtual BOOL PtInObject(CPoint point);
	virtual COLORREF SetBkGnd(COLORREF rgbBkGnd = defaultBkGnd);
	friend class CFloorWnd;
	friend class CXFloorWndCtrl;
	virtual const int GetHeight();
	virtual int SetHeight(int nHeight = defaultHeight);
	virtual const BOOL IsPullDown();
	virtual const BOOL IsPullUp();
	BOOL m_bPullUp;
	virtual const CString& GetName();
	virtual void SetName(LPCTSTR lpszObjectName);
	// Overidable functions
	virtual void OnDeactivateObject() = 0;	// Called when object is about to be deactivated
	virtual void OnActivateObject() = 0;	// Called after object was activated
	virtual void OnDraw(CDC* pDC, BOOL bEraseBkGnd = FALSE) = 0;	// Called when object must redraw
	virtual void SetRectClient(CRect rect) = 0;	// Set the client rect of window
	virtual void OnFocus() = 0;			// Called when mouse is over the object
	// Constructors and Destructors
	CFloorObject(CXFloorWndCtrl* pWnd, LPCTSTR lpszObjectName);
	virtual ~CFloorObject();
protected:
	LPARAM m_lParam;
	UINT m_nFormatDrawText;
	CBrush m_brBkGnd;
	int m_nHeight;
	CRect m_rect;
	virtual void Invalidate(BOOL bEraseBkGnd = FALSE);
	CString m_sName;
	CXFloorWndCtrl* m_pParentWnd;
};

#endif // !defined(AFX_FLOOROBJECT_H__FF4DABE4_4A17_11D2_868E_0040055C08D9__INCLUDED_)
