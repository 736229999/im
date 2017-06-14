#pragma once


// CToolBarEx

class CToolBarEx : public CToolBar
{
	DECLARE_DYNAMIC(CToolBarEx)

public:
	CToolBarEx();
	virtual ~CToolBarEx();

protected:
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual void PreSubclassWindow();
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};


