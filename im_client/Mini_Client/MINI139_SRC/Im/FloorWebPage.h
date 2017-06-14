#pragma once

#include "explorer1.h"

// CFloorWebPage 对话框

class CFloorWebPage : public CDialog
{
	DECLARE_DYNAMIC(CFloorWebPage)

public:
	CFloorWebPage(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorWebPage();

// 对话框数据
	enum { IDD = IDD_FLOOR_WEBPAGE };
public:
	void InitWebPage();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	DECLARE_MESSAGE_MAP()
	DECLARE_EVENTSINK_MAP()
	void NavigateComplete2Explorer1(LPDISPATCH pDisp, VARIANT* URL);

protected:
	CExplorer1 m_wndExplore;
	bool       m_bNavigated;
	CPictureEx m_wndProgress;

	
};
