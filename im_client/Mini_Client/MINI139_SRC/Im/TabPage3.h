#pragma once

// CTabPage3 对话框
#include "..\controls\Floor_Wnd_Ctrl\xfloorwndctl.h"
#include "FloorWebPage.h"

class CTabPage3 : public CDialog
{
	DECLARE_DYNAMIC(CTabPage3)

public:
	CTabPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTabPage3();

// 对话框数据
	enum { IDD = IDD_TAB_PAGE3 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	DECLARE_MESSAGE_MAP()

protected:
	CXFloorWndCtrl     m_wndFloorCtrl;
	CFloorWebPage      m_pageWeb;

public:
	virtual BOOL OnInitDialog();
	afx_msg void OnTimer(UINT nIDEvent);
};
