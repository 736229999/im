#pragma once
#include "..\resource.h"
#include  "..\controls\skinctrl.h"
#include "afxwin.h"
#include "afxcmn.h"
#include "..\Im\impublic.h"
#include "..\Im\formprogress.h"
#include "MainFrame.h"
#include "..\..\HmgSkin\XListCtrl.h"
// CClusterMember 对话框

class CClusterMember : public CDialog
{
	DECLARE_DYNAMIC(CClusterMember)
	DECLARE_HMG_SCROLLBAR()

public:
	CClusterMember(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterMember();

// 对话框数据
	enum { IDD = IDD_CLUSTER_MEMBER };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()
public:
	CFlatEdit m_edit;
	/*CListCtrl*/ CXListCtrl m_list;
	int m_row;
	int m_column;
	BOOL m_bChooseMemberFlag;
	BOOL m_bChooseFlag;
	CImageList m_imagelist;
public:

	CFlatButton m_btnSetAdmin;
	CFlatButton m_btnCancelAdmin;
	CFlatButton m_btnDelMember;
	CFlatButton m_btnModifyInfo;
	CFlatButton m_btnAddMember;
	CLUSTER_INFO  m_clusterInfo;
	CFormProgress3   *  m_pProgress;

	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnEnKillfocusEditListctl();
	afx_msg void OnBnClickedBtnSetAdmin();
	afx_msg void OnBnClickedBtnCancelAdmin();
	afx_msg void OnBnClickedBtnDelMember();
	afx_msg void OnBnClickedBtnModifyInfo();
	afx_msg void OnBnClickedBtnAddMember();
	afx_msg void OnTimer(UINT nIDEvent);

	void  CreateListInfo(CLUSTER_INFO  *pClusterInfo);
	void  UpdateClusterInfo();
	void  Real_UpdateClusterInfo();
	void  ModifyProfileName();
	afx_msg void OnNMClickList1(NMHDR *pNMHDR, LRESULT *pResult);
	void ShowProgress(CString strTip);
	void CloseProgress();
	void OnSetCMemberAck(const char *data,WORD wRet);
	void OnGetCMemberProAck(const char *data, WORD wRet);
	BOOL PreTranslateMessage(MSG* pMsg);
};
