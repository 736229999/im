#if !defined(_InPlaceTextTip_h_)
#define _InPlaceTextTip_h_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// InPlaceTextTip.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CInPlaceTextTip window

#define TEXTTIP_CLASSNAME _T("xf-Yang Text Tips")

class CInPlaceTextTip : public CWnd
{
// Construction
public:
	CInPlaceTextTip();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CInPlaceTextTip)
	public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	//}}AFX_VIRTUAL

// Implementation
public:
	BOOL Create(CWnd *pParent);
	virtual ~CInPlaceTextTip();

	// Generated message map functions
protected:
	//{{AFX_MSG(CInPlaceTextTip)
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CRect m_rcText;
	CWnd * m_pParentWnd;
public:
	void ShowTip( CRect & rectTitle, LPCTSTR lpszTitleText, int xOffset = 0);
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(_InPlaceTextTip_h_)
