#if !defined(AFX_STATICCTRLEX_H__962358EF_3700_4198_AA3A_6C3D3D449BE7__INCLUDED_)
#define AFX_STATICCTRLEX_H__962358EF_3700_4198_AA3A_6C3D3D449BE7__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
// StaticCtrlEx.h : header file
//

/////////////////////////////////////////////////////////////////////////////
// CStaticCtrlEx window

enum FlashType {_SCX_FTNone, _SCX_FTText, _SCX_FTBackground };

class CStaticCtrlEx : public CStatic
{
// Construction
public:
	CStaticCtrlEx();

// Attributes
public:

// Operations
public:

// Overrides
	// ClassWizard generated virtual function overrides
	//{{AFX_VIRTUAL(CStaticCtrlEx)
	//}}AFX_VIRTUAL

// Implementation
public:
	virtual ~CStaticCtrlEx();

	// Generated message map functions
protected:
	//{{AFX_MSG(CStaticCtrlEx)
		afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
		afx_msg void OnTimer(UINT nIDEvent);
		afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
		afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	//}}AFX_MSG

	DECLARE_MESSAGE_MAP()
protected:
	COLORREF	m_crText;
	HBRUSH		m_hBrush;
	HBRUSH		m_hwndBrush;
	LOGFONT		m_lf;
	CFont		m_font;
	CString		m_strText;
	BOOL		m_bState;
	BOOL		m_bTimer;
	BOOL		m_bLink;
	FlashType	m_Type;
	HCURSOR		m_hCursor;
public:
	CStaticCtrlEx& SetBkColor(COLORREF crBkgnd);
	CStaticCtrlEx& SetTextColor(COLORREF crText);
	CStaticCtrlEx& SetText(const CString& strText);
	CStaticCtrlEx& SetFontBold(BOOL bBold);
	CStaticCtrlEx& SetFontName(const CString& strFont);
	CStaticCtrlEx& SetFontUnderline(BOOL bSet);
	CStaticCtrlEx& SetFontItalic(BOOL bSet);
	CStaticCtrlEx& SetFontSize(int nSize);
	CStaticCtrlEx& SetSunken(BOOL bSet);
	CStaticCtrlEx& SetBorder(BOOL bSet);
	CStaticCtrlEx& FlashText(BOOL bActivate);
	CStaticCtrlEx& FlashBackground(BOOL bActivate);
	CStaticCtrlEx& SetLinkAddr(const CString strLinkAddr, const BOOL bLink = TRUE);
	CStaticCtrlEx& SetLinkCursor(HCURSOR hCursor);
protected:
	void ReconstructFont();
private:
	CString m_strLinkAddr;
};


/////////////////////////////////////////////////////////////////////////////

//{{AFX_INSERT_LOCATION}}
// Microsoft Visual C++ will insert additional declarations immediately before the previous line.

#endif // !defined(AFX_STATICCTRLEX_H__962358EF_3700_4198_AA3A_6C3D3D449BE7__INCLUDED_)
