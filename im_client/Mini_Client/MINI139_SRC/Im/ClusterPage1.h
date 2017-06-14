#pragma once

#include "WizardPage.h"

#include "..\controls\skinctrl.h"
// CClusterPage1 对话框

class CClusterPage1 : public CWizardPage
{
	DECLARE_DYNAMIC(CClusterPage1)

public:
	CClusterPage1(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterPage1();

// 对话框数据
	enum { IDD = IDD_WIZARD_CLUSTER_PAGE1 };

public:
	bool GetClusterInfo(CLUSTER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();

	DECLARE_MESSAGE_MAP()

public:
	CFlatEdit m_edtName;
	CFlatEdit m_edtIntro;
	//CFlatEdit m_edtDomain;
	//CStatic   m_btnDomain;
	CButton m_btnRadioAll;
	CButton m_btnRadioVerify;
	CButton m_btnRadioNone;
	CGraphComboBox m_comboType;
public:
	//afx_msg void OnEnChangeEditDomain();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	int  GetClusterRadioBtnChecked();
	afx_msg void OnChangeEditName();
	afx_msg void OnChangeEditIntro();
};
