#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"

// �����Ӱ�ť���͵���Ϣ
#define WM_OK (WM_USER+0x01)

class CDuiSelFriendList : public DuiLib::CTreeViewUI, public DuiLib::INotifyUI
{
public:
	CDuiSelFriendList(DuiLib::CPaintManagerUI& paint_manager);	
	// �����������
	BOOL AddGroup_Interface(int nGroupId, CString strTitle);
	// �����б���Ӻ���
	BOOL AddItem_Interface(FRIEND_INFO *pFriend);
	// ��ȡ���ڵĺ���
	void GetSelFriend(vector<int> &vecFriendID);
	// ��ȡ���еĺ���
	void GetAllFriend(vector<int> &vecFriendID);
	// �жϺ����б��е�item�Ƿ�Ϊ����
	BOOL IsGroupItem(DuiLib::CTreeNodeUI *pItem);
	// ѡ�к���
	void SelFriend(int nFriendID);
	// ��ѡ����
	void CheckFriend(int nFriendID, BOOL bCheck);

private:
	// ������ڵ�
	DuiLib::CTreeNodeUI* FindGroupNode(int nGroup);
	// ���Һ���node
	DuiLib::CTreeNodeUI* FindItem_Interface(int nGroupId, DWORD dwData);
	// ���º��ѽڵ�����
	BOOL UpdateItem(DuiLib::CTreeNodeUI *pNode, FRIEND_INFO *pFriend);
	void Notify(DuiLib::TNotifyUI& msg);
	bool OnCheckBoxChanged( void* param );
	void IsAllChildChecked(DuiLib::CTreeNodeUI *pNode);
	bool SetItemCheckBox( bool _Selected, DuiLib::CTreeNodeUI* _TreeNode = NULL );

private:
	DuiLib::CPaintManagerUI& paint_manager_;
};

class CSearchListWnd;
class CGroupSendAddOtherDlg : public DuiLib::WindowImplBase
{
public:
	enum { 
		TIMER_SERCHEDIT_SETFOCUS=0x00,
	};

	CGroupSendAddOtherDlg();
	~CGroupSendAddOtherDlg();
	static std::tr1::shared_ptr<CGroupSendAddOtherDlg> Show(HWND hParend, vector<int> &vecSelFriendID, CPoint pt);
	static void CloseDlg();
	static std::tr1::shared_ptr<CGroupSendAddOtherDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("GroupSendAddOtherDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("GroupSendAddOtherDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	void CheckFriend(vector<int> &vecFriendID);
	// ͨ���ؼ��ֲ��Һ���
	void SearchFriend(const char *pKey, vector<int> &vecMeetFriend);
	// ��ѡ����
	void CheckFriend(int nFriendID, BOOL bCheck);

	static vector<int> g_SelFriendID;

protected:
	// ���ҹؼ��ָı�
	void SearchTextChange(const char *pKey);
	void OnTimer(UINT nIDEvent);

	static std::tr1::shared_ptr<CGroupSendAddOtherDlg> pInstance;
	CDuiSelFriendList *m_pSelFriendList;
	HWND m_hParent;

	std::tr1::shared_ptr<CSearchListWnd> m_pSearchListWnd;
};
