#pragma once

#include "WizardPage.h"
// CClusterPage3 �Ի���

class CClusterPage3 : public CWizardPage
{
	DECLARE_DYNAMIC(CClusterPage3)

public:
	CClusterPage3(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClusterPage3();

// �Ի�������
	enum { IDD = IDD_WIZARD_CLUSTER_PAGE3 };

public:
	bool GetClusterInfo(CLUSTER_INFO &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL OnInitDialog();
};
