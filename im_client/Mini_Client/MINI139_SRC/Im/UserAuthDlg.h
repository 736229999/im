#pragma once

#include "FaceIconCtrl.h"

#include "XSkinDlg.h"
#include "afxwin.h"
#include "buttonext.h"

// CUserAuthReqDlg 对话框

class CUserAuthReqDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CUserAuthReqDlg)

public:
	virtual ~CUserAuthReqDlg();

	static void Show(FRIEND_INFO *pFriend,uint32 nGroupId);
	static void Close();
// 对话框数据
	enum { IDD = IDD_DIALOG_USERAUTHREQ };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedSend();
	afx_msg void OnStnClickedStaticMore();
	afx_msg void OnNcDestroy();
	afx_msg void OnBnClickedCancel();
	afx_msg void OnClose();

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnUserInfoGet(const char *data,WORD wRet);
	void ShowUserInfo();

private:
	CUserAuthReqDlg(FRIEND_INFO *pFriend,CWnd* pParent = NULL);   // 标准构造函数

	CBmpBtn		m_btnSend;
	CBmpBtn		m_btnCancel;
	CHyperBtn	m_btnDetail;
	CFlatEdit	m_edtAuth;

	CFaceIconCtrl m_wndFaceIcon;

	HBRUSH		m_hBkGround;

	FRIEND_INFO m_FriendInfo ;	
	uint32		m_nGroupId;

	static  vector<CUserAuthReqDlg *>  m_vecInstance;
	static  uint32  m_preReqId;
	static  CString m_preReqInfo;
	static  time_t  m_preReqTime;

	DWORD m_dwSelAuth;//身份验证框光标位置和长度
	int   m_iLenAuth;
public:
	CButton m_btnAllow;
	afx_msg void OnChangeEditAuth();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};
