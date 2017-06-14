#pragma once

// CRadioButton

class AFX_EXT_CLASS CRadioButton : public CButton
{
	DECLARE_DYNAMIC(CRadioButton)

public:
	CRadioButton();
	virtual ~CRadioButton();
	virtual void PreSubclassWindow();

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
};