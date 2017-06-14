#pragma once
#include "..\..\HmgSkin\HmgSkin\MenuCtrl.h"

class CMsgManage_Friend_Dlg;
class CMsgManageDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CMsgManageDlg)

public:
	CMsgManageDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgManageDlg();
	static CMsgManageDlg * Show();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void OnCancel();
	afx_msg void OnNcDestroy();
	// �˵���ѡ�Ÿı��¼�
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void  InitTabCtrl();

private:
	static CMsgManageDlg * m_stapInstance;
	CMenuCtrl m_MainTab;
	CFont m_font;

	// �Ӵ���
	CMsgManage_Friend_Dlg *m_pDlgFriendMsg;
};
