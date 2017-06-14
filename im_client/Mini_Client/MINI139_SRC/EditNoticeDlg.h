#pragma once
#include "im\CustomToolBar.h"

#define EDIT_NOTICE_SELF_MSG (WM_USER + 766)		// 自定义消息

// CEditNoticeDlg 对话框

class CEditNoticeDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CEditNoticeDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CEditNoticeDlg(CWnd* pParent, string strOldNotice, const CPoint &ptPos);   // 标准构造函数
	virtual ~CEditNoticeDlg();
	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	BOOL Create();
	string GetText()	{	return m_strNotice;	}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnSetfocusEditNotice();
	DECLARE_MESSAGE_MAP()

protected:
	virtual void DrawCaptionBar();

private:
	XT_FONT m_font;
	CRichEditCtrlEx m_editNotice;
	CP2PTalkToolBar m_wndTalkToolBar;
	CPoint m_ptPos;

	CBmpBtn m_btnOK;
	CBmpBtn m_btnCancel;

	CBmpBtn m_btnFace;
	CBmpBtn m_btnPic;

	COLORREF m_clrBorder;
	string m_strNotice;
};
