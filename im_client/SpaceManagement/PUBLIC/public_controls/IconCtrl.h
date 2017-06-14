#pragma once

// CIconCtrl

class CIconCtrl : public CStatic
{
	DECLARE_DYNAMIC(CIconCtrl)

public:
	CIconCtrl();
	virtual ~CIconCtrl();

	void SetFaceFile(CString strFile);
	void SetCursor(HCURSOR hCursor) { m_hCursor = hCursor ; }//


	void SetToolTip(const CString& sTip)
	{ m_sTip = sTip; };

protected:
	void ShowToolTip(const CString& strTip);
	virtual void PreSubclassWindow();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnPaint();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);

	DECLARE_MESSAGE_MAP()

private:
	CBitmap * m_bitmap;


	bool m_bMouseOver ;
	bool m_bMouseDown ;

	HCURSOR  m_hCursor ;

	CToolTipCtrl	m_toolTip;		// 提示控件	
	CString			m_sTip;			// 提示信息
	CString			m_sLastTip;		// 上次的提示信息

};


