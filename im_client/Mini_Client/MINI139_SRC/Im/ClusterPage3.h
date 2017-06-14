#pragma once

#include "WizardPage.h"
// CClusterPage3 对话框

class CClusterPage3 : public CWizardPage
{
	DECLARE_DYNAMIC(CClusterPage3)

public:
	CClusterPage3(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterPage3();

// 对话框数据
	enum { IDD = IDD_WIZARD_CLUSTER_PAGE3 };

public:
	bool GetClusterInfo(CLUSTER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
