#pragma once

#include "faceiconctrl.h"

#include <vector>
using namespace std;
// CDailogClusterCheck 对话框


class CDailogClusterCheck : public CXSkinDlg
{
	DECLARE_DYNAMIC(CDailogClusterCheck)
	DECLARE_HMG_SCROLLBAR()

public:
	virtual ~CDailogClusterCheck();

	struct tagCF
	{
		uint32 cid;
		uint32 fid;
		CDailogClusterCheck *pDlg;
	};

// 对话框数据
	enum { IDD = IDC_DIALOG_CLUSETR_CHECK };

public:
	static CDailogClusterCheck * Show(XT_CLUSTER_AUTH_IND *ind);
	static void Close();
 
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	afx_msg void OnClose();
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedCancel();
	void		SetInfo();

	void OnUserInfoAck(const char *data,WORD wRet);
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:

	CDailogClusterCheck(XT_CLUSTER_AUTH_IND *ind, CWnd* pParent = NULL);   // 标准构造函数

private:
	XT_CLUSTER_AUTH_IND		m_clusterAuthInd;
	static   vector<tagCF>	m_vecCF;
	FRIEND_INFO				m_friendInfo;
	CString					m_sCluster;
	tagCF					m_cf;

	CFlatEdit				m_editAuth;
	CBmpBtn					m_btnOk, m_btnCancel;
	HBRUSH					m_hBkGround;
public:
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	CFaceIconCtrl m_faceIcon;
	CHyperBtn m_btnFullInfo;
	afx_msg void OnStnClickedLblMore();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
