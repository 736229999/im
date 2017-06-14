#pragma once

#include "CustomListCtrl.h"
#include "afxwin.h"

#include "formprogress.h"
#include "..\..\..\SpaceManagement\public\public_controls\ResGdiObj.h"

class CClusterSettingDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CClusterSettingDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	virtual ~CClusterSettingDlg();

public: 
	static void Show(CLUSTER_INFO *pCluster);

public:
	static CClusterSettingDlg * pInstance ;
	XT_SET_CINFO_REQ    m_cInfoReq;

// 对话框数据
	enum { IDD = IDD_DIALOG_CSETTING };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual void OnHelpClick();
	afx_msg void OnBnClickedOk();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedClose();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedSetmembers();
	afx_msg void OnBnClickedBtnAddfriend();
	afx_msg void OnBnClickedBtnDelmember();
	afx_msg void OnBnClickedBtnDelmanage();
	afx_msg void OnBnClickedBtnSetmanage();
	afx_msg void OnBnClickedUpdate();
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedBtnSite();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnBnClickedClusterOpr();
	afx_msg void OnStnClickedLinkFamilyhome();
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	LRESULT OnCancelRefresh(WPARAM, LPARAM);
	LRESULT OnFamilyIconNotify(WPARAM wParam, LPARAM lParam);

public:
	CFlatEdit m_edtID;
	CFlatEdit m_edtName;
	CFlatEdit m_edtCreator;
	CFlatEdit m_edtIntro;
	//CFlatEdit m_edtBulitin;
	CClusterListCtrl m_lstCluster;

protected:
	CClusterSettingDlg(CLUSTER_INFO *pInfo, CWnd* pParent = NULL);   // 标准构造函数

	void Real_UpdateClusterInfo();
	void UpdateClusterInfo();
	void OnGetClusterInfoAck(const char *data,WORD wRet);
    void OnSetClusterInfoAck(const char *data,WORD wRet);
	void OnClusterMembersAck(const char *data,WORD wRet);
	void OnCMemberInd(const char *data, WORD wRet);
	void OnSetCMemberAck(const char *data,WORD wRet);
	void OnLoginOut(const char *data,WORD wRet);

	void SetClusterAuthRadioBtn(int nAuth);
	int GetClusterAuth();
	
	void ShowProgress(CString strTip);
	void CloseProgress();

	void InvalidateFamilyHead();

protected:
	CLUSTER_INFO		m_clusterInfo;
	
	CFormProgress3   *  m_pProgress;
	
	HBRUSH				m_hBrushControl;

	CXpButton m_btnSetmembers;
	CXpButton m_btnAddfriend;
	CXpButton m_btnDelmember;
	CXpButton m_btnSetmanage;
	CXpButton m_btnDelmanage;
	CXpButton m_btnDetail;
	CBmpBtn		m_btnOk,m_btnCancel;
	CXpButton m_btnUpdate;
	CXpButton m_btnAdvancedSetting;
	CXpButton m_btnFamilyHeadModify;
	CButton   m_btnRadio1, m_btnRadio2, m_btnRadio3;
	CGraphComboBox	m_combType;
	CButton			m_btnCheckMagic;
	CButton			m_btnCheckRefuse;

	CXpButton		m_btnOpr;
	CHyperBtn		m_btnLinkHome;

	CBitmap	 *		m_pBmpFamilyHead;
	CResBitmap		m_bmpDefaultFamilyHead;

public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedAdvancedSetting();
	afx_msg void OnBnClickedBtnHeadModify();
};
