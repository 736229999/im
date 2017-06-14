////////////////////////////////////////////////////////////////
// PixieLib(TM) Copyright 1997-2000 Paul DiLascia
// If this code works, it was written by Paul DiLascia.
// If not, I don't know who wrote it.
//
#ifndef _STATLINK_H
#define _STATLINK_H

#include "HyprLink.h"

//
class CStaticLink : public CStatic 
{
public:
	DECLARE_DYNAMIC(CStaticLink)
	CStaticLink(LPCTSTR lpText = NULL, BOOL bDeleteOnDestroy=FALSE);

	~CStaticLink() { }

	// Hyperlink contains URL/filename. If NULL, I will use the window text.
	// (GetWindowText) to get the target.
	CHyperLinkString	m_link;
	COLORREF		m_color;

	static COLORREF g_colorUnvisited;
	static COLORREF g_colorVisited;

	// Cursor used when mouse is on a link--you can set, or
	// it will default to the standard hand with pointing finger.
	// This is global, so it's the same for all links.
	static HCURSOR	 g_hCursorLink;

protected:
	CFont		m_font;			// underline font for text control
	BOOL		m_bDeleteOnDestroy;	// delete object when window destroyed?

	virtual void PostNcDestroy();
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	// message handlers
	DECLARE_MESSAGE_MAP()

#if _MSC_VER > 1310
	afx_msg	LRESULT OnNcHitTest(CPoint point);
#else
	afx_msg	UINT OnNcHitTest(CPoint point);
#endif

	afx_msg HBRUSH CtlColor(CDC* pDC, UINT nCtlColor);
	afx_msg void	OnLButtonDown(UINT nFlags, CPoint point);
private:
	CString m_strHyperLink;
public:
	inline	void SetHyperLinkeString(const CString & strLink)
	{
		m_strHyperLink = strLink;
	}


	CBrush m_br;
};

#endif _STATLINK_H
