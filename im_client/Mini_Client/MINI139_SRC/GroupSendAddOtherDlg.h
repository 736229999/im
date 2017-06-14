#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"

// 点击添加按钮发送的消息
#define WM_OK (WM_USER+0x01)

class CDuiSelFriendList : public DuiLib::CTreeViewUI, public DuiLib::INotifyUI
{
public:
	CDuiSelFriendList(DuiLib::CPaintManagerUI& paint_manager);	
	// 界面上添加组
	BOOL AddGroup_Interface(int nGroupId, CString strTitle);
	// 好友列表添加好友
	BOOL AddItem_Interface(FRIEND_INFO *pFriend);
	// 获取现在的好友
	void GetSelFriend(vector<int> &vecFriendID);
	// 获取所有的好友
	void GetAllFriend(vector<int> &vecFriendID);
	// 判断好友列表中的item是否为分组
	BOOL IsGroupItem(DuiLib::CTreeNodeUI *pItem);
	// 选中好友
	void SelFriend(int nFriendID);
	// 勾选好友
	void CheckFriend(int nFriendID, BOOL bCheck);

private:
	// 查找组节点
	DuiLib::CTreeNodeUI* FindGroupNode(int nGroup);
	// 查找好友node
	DuiLib::CTreeNodeUI* FindItem_Interface(int nGroupId, DWORD dwData);
	// 更新好友节点内容
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
	// 通过关键字查找好友
	void SearchFriend(const char *pKey, vector<int> &vecMeetFriend);
	// 勾选好友
	void CheckFriend(int nFriendID, BOOL bCheck);

	static vector<int> g_SelFriendID;

protected:
	// 查找关键字改变
	void SearchTextChange(const char *pKey);
	void OnTimer(UINT nIDEvent);

	static std::tr1::shared_ptr<CGroupSendAddOtherDlg> pInstance;
	CDuiSelFriendList *m_pSelFriendList;
	HWND m_hParent;

	std::tr1::shared_ptr<CSearchListWnd> m_pSearchListWnd;
};
