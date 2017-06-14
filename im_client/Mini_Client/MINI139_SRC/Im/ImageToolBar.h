#pragma once


// CFontToolBar
#include "impublic.h"

class CImageToolBar : public CToolBar
{
	DECLARE_DYNAMIC(CImageToolBar)

public:
	CImageToolBar();
	virtual ~CImageToolBar();

public:
	void InitToolBar(SIZE sz,UINT nIDNormal, UINT nIDHot);

protected:
	virtual void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnNcPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	LRESULT OnIdleUpdateCmdUI(WPARAM wParam, LPARAM);

	DECLARE_MESSAGE_MAP()

private:
	CImageList	    m_imgToolNormal;
	CImageList      m_imgToolHot;

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};


