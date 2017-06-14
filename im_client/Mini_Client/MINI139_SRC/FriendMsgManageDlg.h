#pragma once
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "./im/impublic.h"
#include "./im/RichEditCtrlEx.h"
#include <vector>
#include "./im/LocalData.h"

// 组以及子成员信息
struct STRU_GROUP_FRIEND
{
	GROUP_ITEM *pGroup;
	std::vector<FRIEND_INFO*> vecpFriend;
};

BOOL operator==(STRU_GROUP_FRIEND &gf, int groupIndex);

class CMsgManage_Friend_Dlg : public CDialog
{
	DECLARE_DYNAMIC(CMsgManage_Friend_Dlg)

public:
	CMsgManage_Friend_Dlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMsgManage_Friend_Dlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	void OnCancel() {}
	void OnOK()	{}
	// 菜单栏选着改变事件
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	// 初始化组--成员信息
	void InitGF();

private:
	CColumnCtrl m_colFriendGroup;
	CColumnCtrl m_colFriend;
	CFont m_font;

	vector<STRU_GROUP_FRIEND> m_vecGF;
	CRichEditCtrlEx m_edtReHistory;

	MSG_LIST m_mlRecord;
};
