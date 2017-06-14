#pragma once
#include "afxwin.h"
#include "CustomListCtrl.h"

class CP2PUserList : public CDialog
{
	DECLARE_DYNAMIC(CP2PUserList)
	DECLARE_HMG_SCROLLBAR()

public:
	CP2PUserList(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CP2PUserList();

	void SetClusterInfo(CLUSTER_INFO * pCluster);
	void UpdateClusterList();

// �Ի�������
	enum { IDD = IDD_P2PUSER_LIST };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL OnInitDialog();
	afx_msg void OnLbnSelchangeListCluster();
	afx_msg void OnLbnDblclkListCluster();

	DECLARE_MESSAGE_MAP()

public:
	CClusterListCtrl m_wndList;
	CLUSTER_INFO  *  m_pClusterInfo;

	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
