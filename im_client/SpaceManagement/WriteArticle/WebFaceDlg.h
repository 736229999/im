#pragma once

#include "explorer1.h"

class CWebFaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CWebFaceDlg)

public:
	virtual ~CWebFaceDlg();

public:
	static void Show(int x, int y, HWND hWndSel);
	static void Destory();
	// 对话框数据
	enum { IDD = IDD_DIALOG_WEBFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	DECLARE_MESSAGE_MAP()

protected:
	void BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	DECLARE_EVENTSINK_MAP()

private:
	CWebFaceDlg(CWnd* pParent = NULL);   // 标准构造函数

	CExplorer1	m_explorer		;

	uint32		m_iUserId       ;
	HWND		m_hFaceSelWnd	;

	static CWebFaceDlg *pInstance ;

public:
	afx_msg void OnTimer(UINT nIDEvent);
};
