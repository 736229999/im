#pragma once
#include "..\resource.h"
#include  "..\controls\skinctrl.h"
#include "afxwin.h"
#include  "..\Im\impublic.h"
// CClusterInfo �Ի���

class CClusterInfo : public CDialog
{
	DECLARE_DYNAMIC(CClusterInfo)

public:
	CClusterInfo(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CClusterInfo();

// �Ի�������
	enum { IDD = IDD_CLUSTER_INFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
public:
	CFlatEdit m_ctlClusterName;
	CFlatEdit m_ctlClusterSign;
	CFlatEdit m_ctlClusterInfo;
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void  UpdateComboBox(CLUSTER_INFO *pCluster);
	CGraphComboBox m_ctlComboKinds;
	CStatic m_ctlClusterId;
	CStatic m_ctlCluserOwner;
	
	afx_msg void OnEnChangeEditClustersign();
	afx_msg void OnEnChangeEditClusterinfo();
	afx_msg void OnEnChangeEditClustername();
	afx_msg void OnCbnSelchangeComboKinds();

	BOOL PreTranslateMessage(MSG* pMsg);
	DWORD m_dwSelName;///Ⱥ��
	int   m_iLenName;

	DWORD m_dwSelSign;//Ⱥǩ��
	int   m_iLenSign;

	DWORD m_dwSelInfo;//Ⱥ����
	int   m_iLenInfo;
};

