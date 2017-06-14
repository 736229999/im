#pragma once

#include "customlistctrl.h"
// CFloorPageRecent 对话框

class CFloorPageRecent : public CDialog
{
	DECLARE_DYNAMIC(CFloorPageRecent)

public:
	CFloorPageRecent(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorPageRecent();

// 对话框数据
	enum { IDD = IDD_FLOOR_RECENT };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnBnClickedButtonClear();
	DECLARE_MESSAGE_MAP()

protected:
//	CFlatButton  m_btnClear;
	CRecentListCtrl    m_wndRecentList;

};
