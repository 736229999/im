#pragma once

#include "afxwin.h"
#include "pubheader.h"
#include "..\resource.h"

#include "xskindlg.h"

#include "controls\button.h"

// CPreviewDlg 对话框

#if 1
	#define _USE_XSKING_DLG
#endif

#ifndef _USE_XSKING_DLG
#define CXSkinDlg CDialog
#endif

class CPreviewDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CPreviewDlg)

public:
	static void Show(const CString & strFile);
	static void UnShow();

	virtual ~CPreviewDlg();

	// 对话框数据
	enum { IDD = IDD_PREVIEW_DIALOG };


protected:
	CPreviewDlg(CWnd* pParent = NULL);   // 标准构造函数

	virtual void OnCancel();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	//{{AFX_MSG(CPreviewDlg)
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDrawItem(int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	//	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	//}}AFX_MSG
	DECLARE_MESSAGE_MAP()

	CXpButton   m_btnCancel;

private:// // CThumBnaiListCtrl 消息处理程序

	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedCancel();
	
	static CPreviewDlg * pInstance;

	void SetImageFile(const CString & strFile);

	CBitmap    * m_pBitmap ;
	CRect GetPicRect();
	void DrawPic(CDC *pDC);
	void GetRepaintRgn(CRgn & rgn);
public:
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg	void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
};