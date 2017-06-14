#pragma once

#include "..\..\..\DuiLib\UIlib.h"
#include "im\impublic.h"
#include <vector>
#include "im\DuiP2PSendMsgDlg.h"
/*
class CCSPlatQueuDlg : public DuiLib::WindowImplBase
{
public:
	enum { 
		TIMER_SERCHEDIT_SETFOCUS=0x00,
	};

	CCSPlatQueuDlg();
	~CCSPlatQueuDlg();
	static std::tr1::shared_ptr<CCSPlatQueuDlg> Show(HWND hWnd, POINT pt, vector<int> vecWaitFriendID);
	static void CloseDlg();
	static std::tr1::shared_ptr<CCSPlatQueuDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CCSPlatQueuDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CSPlatQueu.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	void Notify(DuiLib::TNotifyUI& msg);
	// 根据好友ID刷新列表
	void UpdateList(std::vector<int> &vecQueu);
	// 添加等待队列中的好友
	static void AddWaitFriend(int nFriendID);
	// 删除等待队列中的好友
	static void DelWaitFriend(int nFriendID);

protected:
	// 添加单元
	void AddItem(int nFriendID);

	static std::tr1::shared_ptr<CCSPlatQueuDlg> pInstance;
	DuiLib::CListUI *m_pList;
};
*/

class CSearchListWnd;
class CDuiRecentList;
class CDuiFriendList;
class CDuiCSPlatDlg : public DuiLib::WindowImplBase
{
	DECLARE_XT_MESSAGE_MAP()

public:
	enum { 
		TIMER_SERCHEDIT_SETFOCUS=0x00,
	};

	CDuiCSPlatDlg();
	~CDuiCSPlatDlg();
	static std::tr1::shared_ptr<CDuiCSPlatDlg> Show();
	static void CloseDlg();
	static std::tr1::shared_ptr<CDuiCSPlatDlg> GetInstance()	{	return pInstance;	}
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CSPlatDlg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("CSPlatDlg.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	virtual LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	void Notify(DuiLib::TNotifyUI& msg);
	static void UpdateBk();
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//处理菜单点击消息
	void ShowCounselingDlg();
	void ShowAllCounselDlg();
	// 返回最近联系人列表
	CDuiRecentList* GetRecentList()	{	return m_pRecentList;	}
	// 返回好友列表
	CDuiFriendList* GetFriendList()		{	return m_pFriendList;	}

	//正在咨询
	void AddFriendListElement(FRIEND_INFO* pFriend, bool bIsMessage = false, bool bIsTransfer = false, bool bIsRepeat = false, bool bIsReply = false, bool bIsRemind = false, bool bBtnMarkeing = false, bool bBtnMarked = false);
	void AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);
	void SetFriendListElementTranPre(FRIEND_INFO* pFriend, bool bIsTranPre);
	void SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind, int nUnReadNum = 0);
	void MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//根据咨询框当前位置移动聊天框
	void ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead = 0);
	void DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//咨询框删除好友
	void MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent);
	void UpdateFriendListElement(FRIEND_INFO* pFriend);
	BOOL IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg);
	void OnFinalMessage(HWND hWnd);

	// 添加等待队列中的好友
	static void AddWaitFriend(int nFriendID);
	static void UpdateWaitFriend();
	// 删除等待队列中的好友
	static void DelWaitFriend(int nFriendID);
	// 清空等待队列，再客户端重新登录成功后调用
	static void ClearWaitFriend();
	//设置第一次tab显示我的团队
	void SetLayoutPageTeam();
	//tab页选择用户
	void SetLayoutPageCust();
/*	
	// 获取等待队列中最开始的好友
	static int GetFirstWaitBuyer();
*/
	void SetListCurselNull();//打开客服聊天窗口时客户列表设置为-1
	void SetCustUnMsgVisable(bool bIsVisable);//显示或隐藏客户标签的红点
	void SetTeamUnMsgVisable(bool bIsVisable);//显示或隐藏我的团队标签的红点
	void SetFriendUnMsg(FRIEND_INFO* pFriend, bool bIsUnMsg);//设置我的团队中好友的红点
	BOOL IsPopMsgWarn(int &nUnRead);///查看所有会话有无未读消息
	BOOL IsCurrentPopMsgWarn(int &nUnRead,int nFriendID);///查看当前会话有无未读消息
	BOOL CheckUnSavedServiceLog();
	BOOL SelectUnCompletedServiceLog(FRIEND_INFO *pFriend);
	BOOL CheckBuyerTalkingExist();//检测是否有买家和客服聊天的窗口
	int  CalcBuyerTalkingWnd();//计算买家和客服聊天的窗口总数
	// 获取等待队列长度
	int GetWaiterCount()	{	return g_vecWaitFriendID.size();	}
	
	//最大最小化
	CRect GetPreRc();//获取最大化之前的位置
	void SetPreRc(CDuiP2PSendMsgDlg* pDlg);//设置最大化之前的位置
	void SetAllWindowMax(BOOL bIsMax);//所有窗口都跟着最大或者最小化
	void UpdateFriendList(FRIEND_INFO *pFriend);
protected:
	// 通过关键字查找好友
	void SearchFriend(const char *pKey, vector<int> &vecMeetFriend);
	// 查找关键字改变
	void SearchTextChange(const char *pKey);
	void OnTimer(UINT nIDEvent);
	//更新显示好友列表
	void UpdateTeamList();
	void OnSendMsg();
	void OnRecentListRButtonDown();
	void OnFriendInfo();
	void OnModfCommName();
	//向组员发消息
	void OnSendGroupMessage();
	void OnModFCNameAck(const char * data,WORD wRet);
	void ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck);
	// 更新等待队列标签
	void UpdateWaitLabel();

	static std::tr1::shared_ptr<CDuiCSPlatDlg> pInstance;

	std::tr1::shared_ptr<CSearchListWnd> m_pSearchListWnd;

	CDuiRecentList *m_pRecentList;
	CDuiFriendList *m_pFriendList;
	DuiLib::CTabLayoutUI *m_pLayoutPage;
	DuiLib::CListUI* m_pCounselList;
	DuiLib::CButtonUI* m_pBtnCust;
	
	DuiLib::CLabelUI* m_labelUnMsgCust;//客户未读消息红点
	DuiLib::CLabelUI* m_labelUnMsgTeam;//我的团队未读消息红点

	CRect m_preRc;//最大化之前的位置
public:
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* m_pDlgSelectP2PSendMsgDlg;//当前选择的聊天框
	vector<FRIEND_INFO*> m_vecFriend;//所有正在咨询的好友
	vector<CDuiP2PSendMsgDlg*> m_vecP2PSendMsgDlg;//所有正在咨询的聊天框

	map<FRIEND_INFO*, int> m_mapFriendCount;//咨询的次数，用于判断重复咨询
	map<DuiLib::CDuiString,bool> m_MenuCheckInfo; //判断菜单当前选择的待处理还是全部
	BOOL m_bIsUpdate;

protected:
	static std::vector<int> g_vecWaitFriendID;		// 等待队列
};