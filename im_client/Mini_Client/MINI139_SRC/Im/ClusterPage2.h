#pragma once

#include "afxcmn.h"
#include "WizardPage.h"

#include "..\controls\folderlist.h"


//#define MIN_CLUSTER_MEMBER	3	// 创建组必须满足的最少人数

class CClusterPage2 : public CWizardPage
{
	DECLARE_DYNAMIC(CClusterPage2)

public:
	CClusterPage2(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterPage2();

// 对话框数据
	enum { IDD = IDD_WIZARD_CLUSTER_PAGE2 };

public:
	bool GetClusterInfo(CLUSTER_INFO &info);

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnPaint();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedButtonAll();
	afx_msg void OnBnClickedButtonClear();
	//afx_msg void OnNMRclickTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMThemeChangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnTvnSelchangedTree1(NMHDR *pNMHDR, LRESULT *pResult);
	//afx_msg void OnNMClickTree1(NMHDR *pNMHDR, LRESULT *pResult);

	DECLARE_MESSAGE_MAP()

public:
	CXpButton m_btnAll;
	CXpButton m_btnClear;
	CFolderList m_lstFriend;
	//CTreeCtrl m_wndTree;
	afx_msg void OnTvnItemexpandedTree1(NMHDR *pNMHDR, LRESULT *pResult);
};
