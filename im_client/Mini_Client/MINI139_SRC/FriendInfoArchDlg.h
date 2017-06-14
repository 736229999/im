#pragma once
#include "im\impublic.h"

// CFriendInfoArchDlg 对话框

class CFriendInfoArchDlg : public CDialog
{
	DECLARE_DYNAMIC(CFriendInfoArchDlg)

public:
	CFriendInfoArchDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent);   // 标准构造函数
	virtual ~CFriendInfoArchDlg();
	BOOL Create();
	string GetRemarkName();
	int GetGroupIndex();

	// 更新用户信息
	void Refresh(FRIEND_INFO *pFriendInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	void OnOK()	{}
	void OnCancel() {}

	DECLARE_MESSAGE_MAP()

private:
	HBRUSH m_hBkGround;
	FRIEND_INFO *m_pFriendInfo;
	CFlatEdit m_editRemark;
	CGraphComboBox m_comboGroup;
};
