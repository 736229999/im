#pragma once

#include "..\..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"
class CDuiRecentList : public DuiLib::CListUI, public DuiLib::IDialogBuilderCallback
{
public:
	CDuiRecentList(HWND hParent, DuiLib::CPaintManagerUI& paint_manager);
	virtual ~CDuiRecentList();
	//DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)	{	return NULL;	}
	void DoEvent(DuiLib::TEventUI& event);

public:
	void Load();
	void Save();

	void AddUser(FRIEND_INFO *pUser);
	void UpdateUser(FRIEND_INFO *pUser);
	void DeleteUser(int nId);

	void AddCluster(CLUSTER_INFO *pCluster);
	void UpdateCluster(CLUSTER_INFO *pCluster);

	// 获取最近联系人
	const FRIEND_LIST* GetRecentFriend();

	static void GetRealValue(DWORD dwData, FRIEND_INFO *&pFriend, CLUSTER_INFO *&pCluster);

	afx_msg void OnClusterTalk();
	afx_msg void OnClusterInfo();
	afx_msg void OnClusterMsg();
	afx_msg	void OnClearRecent();

	void OnFriendInfo()		;
	void OnFriendMsg()      ;
	void OnFriendAdd()      ;
	void OnP2PTalk()		;

	void OnUserInfoGet(const char *data, WORD wRet);
	void OnUserStatusInd(const char *data, WORD wRet);
	// 获取好友状态
	void OnStatusGetAck(const char *data,WORD wRet);

	// 界面添加节点（好友或者群）
	void AddItem_Interface(DWORD dwData);

protected:
	virtual void DblClick(int nSel);
	void ClearRecentList();
	bool FindUser(uint32 fid,FRIEND_INFO **ppFriend, int * pnItem, DuiLib::CListContainerElementUI **ppNode);
	bool FindCluster(uint32 cid,FRIEND_INFO **ppFriend, int *pnItem, DuiLib::CListContainerElementUI **ppNode);

private:
	struct tagCluster
	{
		uint32 cid;
		char   name[128];
		char   domain[24];
	};

	static void GetCluster(FRIEND_INFO *pFriend,tagCluster &);
	static void SetCluster(FRIEND_INFO *pFriend, const tagCluster & c);
	void UpdateItem_Interface(DuiLib::CListContainerElementUI *pNode, DWORD dwData);
	
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass)
	{
		if (_tcsicmp(pstrClass, _T("ButtonGif")) == 0)
		{
			return	new DuiLib::CButtonGifUI();
		}
		return NULL;
	}
	FRIEND_LIST		m_flRecent;
	int				m_nMaxCount;	// 设定最近联系人列表中的最大个数

public:
	afx_msg void OnLbnDblclk();

private:
	DuiLib::CPaintManagerUI& m_PaintManager;
	HWND m_hParent;
};