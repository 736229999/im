#pragma once
#include "explorer1.h"

#include "xskindlg.h"
// CSelWebPicDlg 对话框

class CTestWebPicDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CTestWebPicDlg)

public:
	CTestWebPicDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CTestWebPicDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_TEST_WEBPIC };

public:
	CString GetPictureUrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnBnClickedTest();
	DECLARE_MESSAGE_MAP()

protected:
	CExplorer1	m_explore1;

	CFlatEdit   m_edtUrl;
	CXpButton   m_btn1,m_btn2,m_btn3;
	CString		m_strUrl;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
};
