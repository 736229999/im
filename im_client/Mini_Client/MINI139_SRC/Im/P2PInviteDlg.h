#pragma once
#include "afxcmn.h"
#include "..\controls\FolderList.h"
#include "afxwin.h"
#include "..\controls\SkinCtrl.h"

class CClusterMemberSelectDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CClusterMemberSelectDlg)
//	DECLARE_XTREAM_SCROLLBAR()

public:
	CClusterMemberSelectDlg(CLUSTER_INFO *pInfo,CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CClusterMemberSelectDlg();

	// 对话框数据
	enum { IDD = IDD_DIALOG_INVITE };

//	void SetMemberNumLimit(int nNum) { m_nMemberNumLimit = nNum;	}
	void GetClusterInfo(CLUSTER_INFO & l);

public:

	void SetP2pInviteFriend(FRIEND_INFO *pFriend) { m_pP2pInviteFriend=pFriend;}

protected:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	virtual void OnOK();
	virtual void OnHelpClick();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnClose();
	afx_msg void OnBnClickedButtonAll();
	afx_msg void OnBnClickedButtonNone();
	///virtual BOOL PreTranslateMessage(MSG* pMsg);
	void   OnSearch();
	void   InitPos();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnCreateClusterAck(const char *data, WORD wRet);
	void OnCMembersSetAck(const char *data, WORD wRet);
	void OnUserFamilyFlagAck(const char *data, WORD wRet);

private:

	void SetClusterMember(uint32 clusterId);

private:

	CFolderList		m_wndListFriend;
	CListBox        m_ctlListAdd;
	FRIEND_INFO  *	m_pP2pInviteFriend;
	int				m_nClusterType;
	uint32			m_nClusterId;

	CLUSTER_INFO    m_clusterInfo;

	vector<FRIEND_INFO *> m_vecFriends;
	vector<FRIEND_INFO *> m_vecAddFriends;
	//CXpButton			m_btnAll,m_btnNone;
	CBmpBtn				m_btnOk, m_btnCancel;
	int m_nPos;
	map<int,int>    m_mapGroup;
	DWORD           m_nItemIndex;
//	int	m_nMemberNumLimit;
public:
	afx_msg void OnPaint();
	afx_msg void OnBnClickedOk();
	afx_msg void OnBnClickedCancel();
	CBmpBtn m_btnAdd;
	CBmpBtn m_btnDel;

	afx_msg void OnBnClickedBtnAdd();
	
	afx_msg void OnBnClickedBtnDel();
	afx_msg void OnLbnSelchangeListAdd();
	afx_msg void OnBnClickedBtnResume();
	afx_msg void OnBnClickedBtnSearch();
	afx_msg void OnBnClickedBtnAll();
	CFlatButton m_btnSearch;
	CFlatButton m_btnResume;
	CFlatEdit m_edtSearch;
	afx_msg void OnEnSetfocusEditContent();
	afx_msg void OnEnKillfocusEditContent();
	afx_msg void OnStnClickedStaticInfo();
	CStatic m_frame;
	bool m_Pos;
	CFlatButton m_btnRight;
	afx_msg void OnBnClickedBtnRight();
	afx_msg LRESULT OnAddFriendToCluster(WPARAM wParam, LPARAM lParam);

private:
    map<DWORD,FRIEND_INFO>  m_friend_id_map;

public:
	CTransparentLabel m_btnNote;
};
