#pragma once
#include "..\..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "./im/impublic.h"
#include "./im/RichEditCtrlEx.h"
#include <vector>
#include "./im/LocalData.h"

// ���Լ��ӳ�Ա��Ϣ
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
	CMsgManage_Friend_Dlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMsgManage_Friend_Dlg();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	void OnCancel() {}
	void OnOK()	{}
	// �˵���ѡ�Ÿı��¼�
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()

private:
	// ��ʼ����--��Ա��Ϣ
	void InitGF();

private:
	CColumnCtrl m_colFriendGroup;
	CColumnCtrl m_colFriend;
	CFont m_font;

	vector<STRU_GROUP_FRIEND> m_vecGF;
	CRichEditCtrlEx m_edtReHistory;

	MSG_LIST m_mlRecord;
};
