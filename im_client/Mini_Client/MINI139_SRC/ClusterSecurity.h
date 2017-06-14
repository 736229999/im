#pragma once
#include "..\resource.h"
#include  "..\controls\skinctrl.h"

// CClusterSecurity �Ի���

class CClusterSecurity : public CDialog
{
	DECLARE_DYNAMIC(CClusterSecurity)

public:
	CClusterSecurity(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClusterSecurity();

// �Ի�������
	enum { IDD = IDD_CLUSTER_SECURITY };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()

public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	int  GetClusterRadioBtnChecked();
	BOOL PreTranslateMessage(MSG* pMsg);
	CButton m_btnRadioAll;
	CButton m_btnRadioVerify;
	CButton m_btnRadioNone;
};
