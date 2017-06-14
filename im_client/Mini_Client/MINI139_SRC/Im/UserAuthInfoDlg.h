#pragma once

#include "FaceIconCtrl.h"
#include "afxwin.h"
#include "XSkinDlg.h"

#include "IMFriendListCtrl.h"
#include "buttonext.h"
// CUserAuthInfoDlg 对话框

#define WM_SELECT_GROUP		WM_USER+0x1000

class CUserAuthInfoDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CUserAuthInfoDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CUserAuthInfoDlg(CWnd* pFloorIMPage, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CUserAuthInfoDlg();

	void SetUserAuthInfo(FRIEND_INFO *pFriend, uint16 action/*0,请求1通过,2拒绝*/,CString strVerify,uint32 nGroupId);
	void SetUserAuthInfo(uint32 fid, uint16 action/*0,请求1通过,2拒绝*/,CString strVerify,uint32 nGroupId);
	static void Close();

public:
	CBmpBtn			m_btnOk;
	CBmpBtn			m_btnCancel;
	CButton			m_chkAdd;
// 对话框数据
	enum { IDD = IDD_DIALOG_USERAUTHINFO };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual void PostNcDestroy();
	afx_msg void OnBnClickedOk();
	afx_msg void OnClose();
	afx_msg void OnStnClickedStaticMore();
	afx_msg void OnBnClickedCancel();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedRadio1();
	afx_msg void OnBnClickedRadio2();	

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:
	void OnUserInfoAck(const char *data,WORD wRet);
	void ShowUserInfo();

private:
	CFlatEdit		m_edtAuth;
	CFaceIconCtrl	m_wndFaceIcon;
	CHyperBtn		m_btnMore;
	HBRUSH			m_hBkGround   ;

	CWnd*			m_pFloorIMPage;				// im窗体

	//CIMFriendListCtrl*	m_pFriendList;	// 好友树型列表
	//int					m_nGroupIndex;	// 同意加对方为好友的时候选择的分组号

	XT_USERAUTH_REQ m_userAuthReq;
	FRIEND_INFO	m_friendInfo;

	static vector<CUserAuthInfoDlg *> m_vecInstance;
	static vector<uint32>  vecRefuseUser;
	static bool			   s_bInitRefuseUser;

public:
	CButton m_chkRefuse;
};
