#pragma once
#include "Im\IMProtocol.h"

// CInternalFriendInfoDlg 对话框
struct FRIEND_INFO;
class CRountFaceCtrl;

class CInternalFriendInfoDlg : public CDialog
{
	DECLARE_DYNAMIC(CInternalFriendInfoDlg)

public:
	CInternalFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CInternalFriendInfoDlg();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
// 对话框数据
	enum { IDD = IDD_DLG_INTERNAL_FRIENDINFO };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void OnUserInfoAck(const char *data, WORD wRet);
	// 更新界面用户信息
	void UpdateUserInfo();

private:
	HBRUSH m_hBkGround;
	FRIEND_INFO *m_pFriendInfo;
	CRountFaceCtrl *m_pFacePic;
};
