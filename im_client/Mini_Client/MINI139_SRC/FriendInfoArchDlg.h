#pragma once
#include "im\impublic.h"

// CFriendInfoArchDlg �Ի���

class CFriendInfoArchDlg : public CDialog
{
	DECLARE_DYNAMIC(CFriendInfoArchDlg)

public:
	CFriendInfoArchDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent);   // ��׼���캯��
	virtual ~CFriendInfoArchDlg();
	BOOL Create();
	string GetRemarkName();
	int GetGroupIndex();

	// �����û���Ϣ
	void Refresh(FRIEND_INFO *pFriendInfo);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
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
