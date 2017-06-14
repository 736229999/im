/************************************
  REVISION LOG ENTRY
  Revision By: Mihai Filimon
  Revised on 8/23/98 19:32:57
  Comments: TLabelEdit.h : header file, CTLabelEdit window
 ************************************/

#if !defined(AFX_TLABELEDIT_H__75B3F830_ECCB_11D1_8540_9B1C6856D236__INCLUDED_)
#define AFX_TLABELEDIT_H__75B3F830_ECCB_11D1_8540_9B1C6856D236__INCLUDED_

#if _MSC_VER >= 1000
#pragma once
#endif // _MSC_VER >= 1000
/////////////////////////////////////////////////////////////////////////////

class CTLabelEdit : public CWnd
{
// Construction
public:
	static WNDPROC m_wndEditDefaultWndProc;
	static CWnd* FAR  m_pWndEdit;
	static CBrush m_brBkGnd;
	CTLabelEdit(LPCTSTR pDefaultText = _T("<...>"));

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CTLabelEdit)
	protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual void Init();
	virtual BOOL IsEditDestroyable();
	void HideEdit();
	virtual void ShowEdit();
	virtual CString GetText();
	virtual void SetText(LPCSTR lpszCaption);
	static BOOL RegClassLabelEdit();
	virtual ~CTLabelEdit();

	// Generated message map functions
protected:
	CFont m_font;
	//{{AFX_MSG(CTLabelEdit)
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()
private:
	CString m_sDefaultString;
};

/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Developer Studio will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_TLABELEDIT_H__75B3F830_ECCB_11D1_8540_9B1C6856D236__INCLUDED_)
