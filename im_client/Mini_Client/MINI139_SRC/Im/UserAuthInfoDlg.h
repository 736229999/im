#pragma once

#include "FaceIconCtrl.h"
#include "afxwin.h"
#include "XSkinDlg.h"

#include "IMFriendListCtrl.h"
#include "buttonext.h"
// CUserAuthInfoDlg �Ի���

#define WM_SELECT_GROUP		WM_USER+0x1000

class CUserAuthInfoDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CUserAuthInfoDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CUserAuthInfoDlg(CWnd* pFloorIMPage, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CUserAuthInfoDlg();

	void SetUserAuthInfo(FRIEND_INFO *pFriend, uint16 action/*0,����1ͨ��,2�ܾ�*/,CString strVerify,uint32 nGroupId);
	void SetUserAuthInfo(uint32 fid, uint16 action/*0,����1ͨ��,2�ܾ�*/,CString strVerify,uint32 nGroupId);
	static void Close();

public:
	CBmpBtn			m_btnOk;
	CBmpBtn			m_btnCancel;
	CButton			m_chkAdd;
// �Ի�������
	enum { IDD = IDD_DIALOG_USERAUTHINFO };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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

	CWnd*			m_pFloorIMPage;				// im����

	//CIMFriendListCtrl*	m_pFriendList;	// ���������б�
	//int					m_nGroupIndex;	// ͬ��ӶԷ�Ϊ���ѵ�ʱ��ѡ��ķ����

	XT_USERAUTH_REQ m_userAuthReq;
	FRIEND_INFO	m_friendInfo;

	static vector<CUserAuthInfoDlg *> m_vecInstance;
	static vector<uint32>  vecRefuseUser;
	static bool			   s_bInitRefuseUser;

public:
	CButton m_chkRefuse;
};
