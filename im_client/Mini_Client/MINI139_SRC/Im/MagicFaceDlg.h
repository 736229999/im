#pragma once
#include "explorer1.h"


// CMagicFaceDlg 对话框
class CMagicFaceDlg : public CDialog
{
	DECLARE_DYNAMIC(CMagicFaceDlg)

public:
	virtual ~CMagicFaceDlg();

public:
	static void Show(int x, int y, uint32 iUserId, HWND hWndSel);
	static void Destory();
// 对话框数据
	enum { IDD = IDD_DIALOG_MFACE };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnKillFocus(CWnd* pNewWnd);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);

	DECLARE_MESSAGE_MAP()

protected:
	void BeforeNavigate2Explorer1(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	DECLARE_EVENTSINK_MAP()

private:
	CMagicFaceDlg(CWnd* pParent = NULL);   // 标准构造函数
	
    CExplorer1	m_explorer		;

	uint32		m_iUserId       ;
	HWND		m_hFaceSelWnd	;

	static CMagicFaceDlg *pInstance ;

public:
	afx_msg void OnNcPaint();
	afx_msg void OnSize(UINT nType, int cx, int cy);
};
