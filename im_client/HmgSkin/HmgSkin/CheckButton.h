#pragma once

#ifndef CHECKBUTTON_H
#define CHECKBUTTON_H

#include "XButton.h"
class AFX_EXT_CLASS CCheckButton : public CButtonBase
{
	DECLARE_DYNAMIC(CCheckButton)

public:
	CCheckButton();
	virtual ~CCheckButton();
	virtual void PreSubclassWindow();
	void SetPic(UINT idCheckedHover, UINT idNotCheckHover, UINT idChecked, UINT idNotCheck, HINSTANCE hPicInstance = NULL);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnMouseLeave(WPARAM wParam,LPARAM lParam);
	afx_msg void OnMouseHover(UINT nFlags, CPoint point);
	DECLARE_MESSAGE_MAP()

private:
	unsigned long m_pGdiToken;
	CBitmap m_bitmap;
	BOOL m_bSaveBackground;
	BOOL m_bTracking;
	TRACKMOUSEEVENT m_TMS; 
	BOOL m_bHover;
	UINT m_idCheckedHover;
	UINT m_idNotCheckHover;
	UINT m_idChecked;
	UINT m_idNotCheck;
	HINSTANCE m_hPicInstance;
};

#endif