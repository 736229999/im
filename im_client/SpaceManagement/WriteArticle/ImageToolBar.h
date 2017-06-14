#pragma once

class CImageToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CImageToolBar)

public:
	CImageToolBar();
	virtual ~CImageToolBar();

public:
	void InitToolBar(SIZE sz,UINT nIDNormal, UINT nIDHot);
	void AddToolTip(UINT nID,CString strTip);

protected:
	void InitToolTip();
	void ShowToolTip( CString strTip);
	virtual void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual INT_PTR OnToolHitTest(CPoint point, TOOLINFO* pTI) const;
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);

	DECLARE_MESSAGE_MAP()

protected:
	CImageList	    m_imgToolNormal;
	CImageList      m_imgToolHot;

	CToolTipCtrl      m_ToolTip;

	CString           m_strPreTip;
	map<UINT,CString> m_mapToolTipString;
	virtual void PreSubclassWindow();
};


