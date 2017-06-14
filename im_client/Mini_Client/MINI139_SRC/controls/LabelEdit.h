#pragma once

#include "flatedit.h"
// CLabelEdit

class CLabelEdit : public CFlatEdit
{
	DECLARE_DYNAMIC(CLabelEdit)

public:
	CLabelEdit();
	virtual ~CLabelEdit();

public:
	bool IsFocused() { return m_bFocused; }
	void SetMaxWidth(int n) ;
	void SetWindowText(LPCSTR lpszWindowText);
	void SetToolTip(CString strTip);


protected:
	virtual void PreSubclassWindow();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg void OnSetFocus(CWnd* pOldWnd);
	afx_msg void OnEnChange();
	DECLARE_MESSAGE_MAP()

	void TestWindowText();
	void SizeToContent();

	void InitToolTip();

protected:
	int     m_nMaxWidth  ;
	bool	m_bMouseOver ;
	bool    m_bFocused   ;
	int		m_nIMEChars  ;

protected:
	CToolTipCtrl m_ToolTip;

};


