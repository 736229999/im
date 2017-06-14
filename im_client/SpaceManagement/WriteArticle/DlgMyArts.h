#pragma once
#include "resource.h"
#include "hyperbtn.h"

// CDlgMyArts 对话框

class CDlgMyArts : public CDialog
{
	DECLARE_DYNAMIC(CDlgMyArts)

public:
	CDlgMyArts(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CDlgMyArts();

// 对话框数据
	enum { IDD = IDD_FORMVIEW };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CHyperBtn m_staticArts;
	CHyperBtn m_staticWebArts;
	virtual BOOL OnInitDialog();
protected:
//	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
public:
//	afx_msg void OnStnClickedStaticArts();
//	afx_msg void OnStnClickedStaticWebarts();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
