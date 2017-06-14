#pragma once
#include "..\..\HmgSkin\HmgSkin\MenuCtrl.h"

class CMsgManage_Friend_Dlg;
class CMsgManageDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CMsgManageDlg)

public:
	CMsgManageDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgManageDlg();
	static CMsgManageDlg * Show();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnCancel();
	afx_msg void OnNcDestroy();
	// 菜单栏选着改变事件
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	DECLARE_MESSAGE_MAP()
	void  InitTabCtrl();

private:
	static CMsgManageDlg * m_stapInstance;
	CMenuCtrl m_MainTab;
	CFont m_font;

	// 子窗口
	CMsgManage_Friend_Dlg *m_pDlgFriendMsg;
};
