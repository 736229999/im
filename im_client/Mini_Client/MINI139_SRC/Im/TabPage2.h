#pragma once

// CTabPage2 �Ի���
#include "..\controls\Floor_Wnd_Ctrl\xfloorwndctl.h"
#include "FloorPageWoku.h"

class CTabPage2 : public CDialog
{
	DECLARE_DYNAMIC(CTabPage2)

public:
	CTabPage2(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CTabPage2();

// �Ի�������
	enum { IDD = IDD_TAB_PAGE2 };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	virtual BOOL OnInitDialog();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()

protected:
//	void DrawTitleBar();

	//CXFloorWndCtrl     m_wndFloorCtrl;
	CFloorPageWoku     m_pageWoku;

};
