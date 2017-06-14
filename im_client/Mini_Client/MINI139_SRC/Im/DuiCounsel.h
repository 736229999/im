/*====================================================================  
=  文件名:CDuiCounsel类                                    
=  修改日期:2015-6-10                                            
=  作者:wyx                  
=  作用:“正在咨询”信息展示 
====================================================================*/ 
#pragma once
#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\DuiLib\Core\UIManager.h"
#include "LocalData.h"
#include "P2PSendMsgDlg.h"
#include "DuiP2PSendMsgDlg.h"
class CDuiCounsel :
	public DuiLib::WindowImplBase
{
public:
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* m_pDlgSelectP2PSendMsgDlg;//当前选择的聊天框
	vector<FRIEND_INFO*> m_vecFriend;//所有正在咨询的好友
	vector</*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg*> m_vecP2PSendMsgDlg;//所有正在咨询的聊天框
	map<FRIEND_INFO*, int> m_mapFriendCount;//咨询的次数，用于判断重复咨询
	//vector<DuiLib::CListContainerElementUI*> m_vecListElement;//所有单元，用于查看待处理还是全部的咨询
	int m_nRemindCount;//未读消息数
	map<DuiLib::CDuiString,bool> m_MenuCheckInfo; //判断菜单当前选择的待处理还是全部
	BOOL m_bIsUpdate;
	CRect m_preRc;
public:
	virtual LPCTSTR    GetWindowClassName() const   {   return _T("DUIMainFrame");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("DuiCounsel.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const;
	LPCTSTR GetResourceID() const;
	virtual void InitWindow();//初始化
	void Notify(DuiLib::TNotifyUI& msg);//消息响应

	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);//处理菜单点击消息
	static void UpdateBk();
	static std::tr1::shared_ptr<CDuiCounsel> Show();
	static std::tr1::shared_ptr<CDuiCounsel> GetInstance();
	DuiLib::CButtonUI* pBtnRemind;////未读提醒

private:
	static std::tr1::shared_ptr<CDuiCounsel> pInstance;
	DuiLib::CListUI* m_pList;
public:
	//添加一个咨询的好友
	void AddFriendListElement(FRIEND_INFO* pFriend, bool bIsMessage = false, bool bIsTransfer = false, bool bIsRepeat = false, bool bIsReply = false, bool bIsRemind = false, bool bBtnMarkeing = false, bool bBtnMarked = false);
	//设置咨询好友的标记
	void SetFriendListElement(FRIEND_INFO* pFriend, bool bIsReply, bool bIsTransfer, bool bIsRepeat, bool bIsMessage, bool bIsRemind,int nUnReadNum = 0);	
	void AddP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//添加聊天框，判断为重复则不添加
	void DeleteP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//咨询框删除好友
	void ShowSelectP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgSelect,int nCntUnRead = 0);//显示当前聊天框
	void MoveP2PSendMsgDlg(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlg);//根据咨询框当前位置移动聊天框
	void ShowAllCounselDlg();//显示所有咨询好友
	void ShowCounselingDlg();//显示待处理的咨询好友
	void MoveAllWindow(/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* pDlgCurrent);//一个聊天框改变大小和位置所有聊天框都随之改变
	void SetAllWindowMax(BOOL bIsMax);//所有窗口都最大化
	BOOL IsPopMsgWarn(int &nUnRead);
	BOOL IsCurrentShowDlg(CDuiP2PSendMsgDlg* pDlg);
	void UpdateFriendListElement(FRIEND_INFO* pFriend);
	CRect GetPreRc();//获取最大化之前的位置
	void SetPreRc(CDuiP2PSendMsgDlg* pDlg);//设置最大化之前的位置
};

