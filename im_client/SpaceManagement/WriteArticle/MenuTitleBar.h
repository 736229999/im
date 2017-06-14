#pragma once

#include <vector>
#include "dhtmledit_blog.h"
using namespace std;

// CMenuTitleBar

class CMenuTitleBar : public CWnd
{
	DECLARE_DYNAMIC(CMenuTitleBar)

public:
	CMenuTitleBar();
	virtual ~CMenuTitleBar();

public:
	BOOL Create(CWnd *pParent, CRect rc, DWORD dwStyle, UINT nID);
	int  AddTitle(UINT nMenu, CString strTitle);
	int  DeleteTitle(int nItem);
	int  GetCount();
	void SetDHtmlCtrl(CDHtmlEditCtrl * pCtrl) { m_pEditCtrl = pCtrl;
	}

protected:
	int  HitTest(CPoint point);

protected:
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	LRESULT OnMenuItemClick(WPARAM wParam, LPARAM lParam);

	struct tagItemInfo
	{
		UINT	nMenu;
		CRect   rcItem;
		CString strTitle;
	};

	vector<tagItemInfo> m_vecItems;
	CFont   m_Font;

	int     m_nHotItem;
	int     m_nSelItem;
	bool    m_bMouseOver;
	BOOL    m_bIsPopup;

	CDHtmlEditCtrl * m_pEditCtrl;
	CMenuEx * m_pPopMenu;

public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
};


