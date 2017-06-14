#pragma once

#include "afxwin.h"

#include "im\tabpage1.h"//交友
#include "im\tabpage2.h"//酷，个人空间管理，
#include "im\tabpage3.h"//娱乐新鲜报
#include "im\advertisepanel.h"
#include "im\maintoolbar.h"
#include "im\faceiconctrl.h"

#include "controls\labeledit.h"
#include "controls\tabwndctrl.h"
#include "controls\skinctrl.h"
#include "im\serverlistFile.h"

#include "im\webprocess.h"
#include "im\NamedPipe.h"
#include "controls\TabWndCtrl.h"
#include "TabWndSearch.h"
#include "im\ScreenCopyDlg.h"
#include "CExitWarning.h"
#include "RoundFaceCtrl.h"

#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


#define		WM_ADD_EMOTION		(WM_USER + 2100)
#define		WM_COPY_URL			(WM_USER + 2101)
// 打开订单窗口的消息响应
#define WM_OPEN_ORDER_MSG_DLG            WM_USER+2102

#define NEAR_SIZE			5		// 定义自动停靠有效距离
#define LEAVE_SIZE			15		// 取消自动停靠时需要的有效距离	
#define NEAR_SIDE			2		// 窗体隐藏后在屏幕上保留的像素，以使鼠标可以触及
#define TIMER_HIDE			0x1000	// 检测鼠标是否在窗体内的事件ID
#define AUOT_CHECK_TIME		200		// 定时检测的时间间隔

//控件位置
#define TAP_LEFT 1
#define TAP_TOP 121
#define BOTTOM_HEIGHT 40

// 好友列表中使用的菜单
#define	ID_P2PSENDMSG			0x0101
#define	ID_FRIENDINFO			0x0102
#define	ID_SEARCH				0x0103
#define	ID_DELFRIEND			0x0104
#define	ID_ADDGROUP				0x0105
#define	ID_DELGROUP				0x0106
#define	ID_MSGHISTORY			0x0107
#define	ID_RENAMEGROUP			0x0108
#define	ID_REFRESH_FRIENDLIST	0x0109
#define	ID_SHOWALL				0x0110
#define	ID_HIDEOFFLINE			0x0111
#define	ID_FRIENDALBUM			0x0112
#define	ID_FRIENDBLOG			0x0113
#define	ID_DELMFRIEND			0x0114	// 删除多个好友
#define ID_MODF_COMMNAME		0x0115	// 修改好友备注名称
#define ID_CREATE_TEMPCLUSTER	0x0116 
#define	ID_FRIENDARTICLE		0x0117
#define	ID_SEND_GROUP_MESSAGE	0x0118
#define	ID_ADD_CGROUP			0x0119
#define	ID_ADD_FRIEND			0x0120
#define	ID_RENAME_GROUP			0x0121
#define ID_REMOVE_BLACKLIST		0x0122
#define ID_ADD_BLACKLIST		0x0123

// 群列表中使用的菜单
#define ID_CLUSTER_MODIFY		0x0124
#define ID_CLUSTER_SENDMSG      0x0125
#define ID_CLUSTER_SEARCH		0x0126
#define ID_CLUSTER_CREATE		0x0127
#define ID_CLUSTER_DISMISS		0x0128
#define ID_CLUSTER_MEMBER		0x0129
#define ID_CLUSTER_MSGHISTORY	0x012A
#define ID_CLUSTER_FRIENDINFO	0x012B
#define ID_CLUSTER_QUIT         0x012D
#define ID_SEARCH_CLUSTER       0x0130
#define ID_CLUSTER_REFUSE       0x0135
#define ID_CINFO_REFRESH        0x0136
#define ID_CMEMBER_LOADING      0x0137
#define ID_CLIST_REFRESH        0x0138
#define ID_USER_ARTICLE         0x0139

// 最近联系人列表中使用的菜单
#define ID_SEND_P2PMSG   0x013A
#define ID_FRIEND_INFO   0x013B
#define ID_FRIEND_ALBUM  0x013C
#define ID_FRIEND_BLOG   0x013D
#define ID_FRIEND_MSG    0x013E
#define ID_FRIEND_ADD    0x0140
#define ID_CLUSTER_TALK  0x0141
#define ID_CLUSTER_INFO  0x0142
#define ID_CLUSTER_PAGE  0x0143
#define ID_CLUSTER_MSG   0x0144
#define ID_CLEAR_RECENT	 0x0145
#define ID_CLUSTER_FRIEND_REMOVE 0x0146


extern bool			 g_bFirstLogin;

class CIMServer:public IM_SERVER
{
public:

	CIMServer()
	{ bTry = false; };

	CIMServer(const IM_SERVER & srv)
	{
		*((IM_SERVER*)this) = srv;
		bTry = false;
	}

	void operator = (const IM_SERVER & srv)
	{
		*((IM_SERVER*)this) = srv;
		bTry = false;
	}

public:

	bool	bTry;	// 是否尝试过连接

};

class CIMServerArr:public std::vector<CIMServer>
{
public:
	//------------------------------------------------------
	// 添加一服务器到列表中，如果存在则返回
	//------------------------------------------------------
	void		AddServer(const CIMServer & srv)
	{
		if(FindServer(srv.ip) == -1)
			push_back(srv);
	};
	//------------------------------------------------------
	// 根据服务器ip获取它在列表中的索引位置
	//------------------------------------------------------
	int			FindServer(uint32 ip)
	{
		size_t i,c;
		c = size();
		for(i = 0; i < c; i++)
		{
			if(ip == (*this)[i].ip)
				return i;
		}
		return -1;
	}
	//------------------------------------------------------
	// 获取一个没有做尝试连接的服务器。
	// 返回值：失败-1，成功则为在列表中的索引
	//------------------------------------------------------
	int			GetServer()
	{
		size_t i,c;
		c = size();
		for(i = 0; i < c; i++)
		{
			if((*this)[i].bTry == false)
				return i;
		}
		return -1;
	}
	//------------------------------------------------------
	// 加载服务器列表
	//------------------------------------------------------
	void		Load(const IMSERVER_LIST & srvList)
	{
		size_t i,c;
		c = srvList.size();
		clear();
		for(i = 0; i < c; i++)
		{
			push_back(srvList[i]);
		}
	}
	//------------------------------------------------------
	// 获取服务器列表 
	//------------------------------------------------------
	void		Get(IMSERVER_LIST & srvList)
	{
		srvList.clear();
		size_t i,c;
		c = size();
		for(i = 0; i < c; i++)
		{
			srvList.push_back(operator [](i));
		}
	}
	//------------------------------------------------------
	// 置位
	//------------------------------------------------------
	void		Reset()
	{
		size_t i,c;
		c = size();
		for(i = 0; i < c; i++)
		{
			(*this)[i].bTry = false;
		}
	}
};


class CDulFriendList;
class CDuiClusterList;
class CSearchListUI;
class CDuiGuidePage;
class CSpecifySendDlg;
class CSelfInfoDlg;
class CDuiSelfInfoDlg;
class CDuiRecentList;
class CScreenCtrl;
class CMainFrame : public DuiLib::WindowImplBase
{
	friend class CAddBlackDlg;

	DECLARE_XT_MESSAGE_MAP()

public:
	enum { 
		TIMER_BEAT =0x02,  //心跳
		TIMER_RELOGIN =0x03, //自动重新连接服务器
		TIMER_INVALIDATELIST=0x05,
		TIMER_SORTLIST=0x06,
		TIMER_CHECK_NETWORD=0x07,
		TIMER_USER_CARD=0x08,
		TIMER_EDITGOUP=0x09,
		TIMER_SORTGROUP=0x10,
		TIMER_SERCHEDIT_SETFOCUS=0x1A,
		TIMER_LOGIN=0x1B,
		TIMER_LBUTTONUP=0x1C,
		TIMER_DELAY=0x1D,
		TIMER_SHOW_PCAST=0x20,
		TIMER_DISCONNECT=0x21,
		TIMER_WAKEUP=0x22,
		TIMER_MOD_HANGUP_STATE=0x23,
		TIMER_CLOSE=0x24,
		TIMER_SEND_MSG_QUEUE=0x25,
	};

	// 窗体停靠方式
	enum WND_ALIGN_TYPE
	{
		ALIGN_NONE,		// 不停靠
		ALIGN_TOP,		// 停靠上边
		ALIGN_LEFT,		// 停靠左边
		ALIGN_RIGHT		// 停靠右边
	};

public:
	CMainFrame();
	~CMainFrame();
	static std::tr1::shared_ptr<CMainFrame> Show();
	static std::tr1::shared_ptr<CMainFrame> GetInstance()	{	return pInstance;	}
	static void CloseDlg();
	static void UpdateBk();

public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("MainFrame");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("mainframe.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	virtual void OnFinalMessage(HWND hWnd);
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	LRESULT HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled);
	DuiLib::CControlUI* CreateControl(LPCTSTR pstrClass);
	// 消息
	void Notify(DuiLib::TNotifyUI& msg);
	CDuiRecentList* GetRecentList()	{	return m_pRecentList;	}

private:
	void InitializeIm();
	LRESULT OnWebIconDownload(WPARAM w, LPARAM l);

public:
	//------------------------------------------------------
	// 获得新增加的好友列表
	// oldFriends	旧好友列表
	// newFriends	新获取的好友列表
	// addiFriends	比较得出新增加的好友列表
	//------------------------------------------------------
	static void GetDiffFriends(
		/*in*/const FRIEND_LIST &			oldFriends,
		/*in*/const vector<FRIENDITEM> &	newFriends,
		/*out*/vector<uint32> &				addiFriends
		) ;

	static void FindOldFriends(
		const FRIEND_LIST &			oldFriends,
		const vector<FRIENDITEM> &	newFriends,
		vector<uint32> &			rst
		);
#if 0
	BOOL Create(CWnd *pParentWnd, CRect rect);
#endif
	//更改用户状态.
	void ChangeUserStatus(uint8 status);

	void StartHeartBeart();

	void OnNetWorkError();

	//客户端离线
	void SetUserOffLineFlag();

	void StartWebInfoTimer();

	void FindItem(FRIEND_INFO *pUser, int & nGroup, int & nItem);
	void SelItem(FRIEND_INFO * pUser);

	void ShowGuiPage();//显示新特性引导页

	void SetSendMsgQueue();

	void StartSendMsgQueueThread();
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnFriendListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg	void OnGroupChoose(UINT nID);
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnBnClickedBtnSysSet();
	afx_msg void OnBnClickedBtnSysMsg();
	afx_msg void OnBnClickedBtnSysAdd();
	afx_msg void OnBnClickedBtnSysMenu();
	afx_msg void OnMenuModPSW();
	afx_msg void OnMenuChangeUser();
	afx_msg	void OnMenuExit();
	afx_msg void OnSoftUpdate();

	LRESULT OnHintDlgClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebIconNotify(WPARAM wParam, LPARAM lParam);
	LRESULT OnChangeFriendGroup(WPARAM wParam, LPARAM lParam);
	LRESULT OnWebDataNotify(WPARAM wParam, LPARAM lParam);

protected:
	//好友列表菜单命令响应函数
	void OnSendMsg();
	void OnFriendInfo();
	void OnFriendAlbum();
	void OnFriendArticle();
	void OnFriendBlog();
	void OnDelFriend();
	void OnDelMFriend();
	void OnMsgHistory();
	void OnAddGroup();
	void OnRenameGroup();
	void OnDelGroup();
	void OnSearchXtream();
	void OnRefreshFriendList();
	void OnSendGroupMessage();
	//添加子组
	void OnAddSubGroup();
	void OnRemoveBlackList();
	void OnAddBlackList();
	// 隐藏非在线好友
	void OnHideOffline();
	// 显示所有好友
	void OnShowAll();
	// 修改好友备注名称
	void OnModfCommName();
	// 创建临时群聊
	void OnCreateTempCluster();

protected:
	//IM协议层通知
	//添加好友请求、拒绝、通过 通知
	void OnStatusGetAck(const char *data,WORD wRet);
	void OnUserAuthInd(const char *data,WORD wRet);
	void OnUserAuthAck(const char *data,WORD wRet);
	void OnUserInfoGet(const char *data,WORD wRet);
	void OnFriendGroupModAck(const char *data,WORD wRet);
	void OnGroupModAck(const char *data,WORD wRet);
	void OnGroupInfoGet(const char *data,WORD wRet);
	void OnHeartBeat(const char * data,WORD wRet);
	void OnFriendStatusChange(const char *data,WORD wRet);//好友状态更改通知
	void OnStatusModAck(const char *data,WORD wRet);
	void OnRecvP2PMsg(const char *data,WORD wRet);
	void OnRecvClusterMsg(const char *data,WORD wRet);
	void OnP2PMsgAck(const char *data,WORD wRet);
	void OnModFCNameAck(const char * data,WORD wRet);
	void OnBasicInfoAck(const char * data,WORD wRet);
	void OnRecvP2pTestAck(const char * data, WORD wRet);
	void OnRecvP2pTest(const char * data,WORD wRet);
	void OnDelFriendReq(const char * data,WORD wRet);
	void OnRecvSendFileReq(const char *data,WORD wRet);
	void OnWebInd(const char *data,WORD wRet);
	void OnFriendInfoInd(const char *data,WORD wRet);
	void OnFriendListAck(const char *data,WORD wRet);
	void OnGetUserStatusAck(const char *data,WORD wRet);
	void RecentList_UserInfoGet(const char *data,WORD wRet);
	void RecentList_UserStatusInd(const char *data,WORD wRet);
	void RecentList_StatusGetAck(const char *data,WORD wRet);

public:

	// 修改好友名称（界面）
	void SetCommName(FRIEND_INFO* pFriend, const CString& sCommName);
	// 重置好友名称
	void ResetName(uint32 fid);
	void ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck);///修改备注

protected:

	void		ImGetFriendList(int start_pos=0, int type=FLS_TOTAL );
	//发送心跳到服务器
	void		ImHeartBeat();
	void		ImSaveUserOperations();
	void		ImGetGroupInfo();
	//添加一个用户组
	void		ImAddGroup(int nParentGroupId, int nGroupId,CString strGroupName);
	void		ImRenameGroup(int nGroupId, CString strGroupName);
	void		ImDelGroup(int nGroupId);
	//设置好友所在组
	void		ImSetUserGroup(uint32 fid, int nGroupId,uint8 action=GROUP_MODIFY);
	//获取好友在线状态
	void		ImGetUserStatus(uint32 uid);
	//------------------------------------------------------
	// 获取好友基本资料
	//------------------------------------------------------
	void		ImGetFriendBasicInfo() const;

	void		IMSetFoucs(uint32 fid,uint8 focus);
protected:

	//好友列表处理
	void UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort=TRUE);	//更新好友状态图标
	//好友数量更新
	void UpdateFriendCnt();
	void UpdateFriendList();
	void SortGroupList();
	void SortGroupList(int nGroup);
	void QuickSortGroupList(int nGroup, int iLo, int iHi);
	void DelFriend(FRIEND_INFO *pFriend);
	void DelFriend(uint32 nFriendId);
public:
	bool AddFriend(FRIEND_INFO *pFriend,bool bSel);
	void DeleteStranger(int nAddId);//商家版清空陌生人列表
protected:
	// 好友隐藏了,当收到聊天消息的时候,则亮起来
	// 用一个定时器来检测
	void RemoveHideTick(uint32 fid);
	void AddHideTick(uint32 fid);
	void CheckHideTick();

	void CreateGroup(int nParentGroupId, int& nGroupId, CString& strGroupName);

protected:

	// 从本地加载好友列表
	void	LoadLocalFriends();
	// 将好友列表保存到本地
	void	SaveLocalFriends();
	// 
	void	ShowUserAuthInfo(uint32 fid,uint16 action/*0请求，１通过，２拒绝*/,CString strVerify,uint32 nGroupId);
	// 将某组在线好友建立临时群聊
	void	MakeTempCluster(FL_GROUP_INFO* pGroup);
	// 好友列表控件在多选时弹起鼠标右键
	void	OnNCFlRbum();

#ifdef _BUSINESS_VESION
	// 添加我的团队分组，如果已经存在就不添加
	BOOL AddGroup_MyTeam();
#endif

	// 自动回复（当离线或者忙碌时）
	static unsigned __stdcall AutoReply(void * pParam);

	// 更新好友信息
	static unsigned __stdcall UpdateFriendInfo(void * pParam);

protected:
	LRESULT  OnThreadNotify(WPARAM,LPARAM);

	// 获取新的组ID（自定义分组时用到）
	int GetNewGroupID();


public:
	afx_msg void OnBnClickedBtnCluster();

public:
	void LoadLoacalClusterList();
	void BuildCluster(uint32 cid);
	void SetClusterOfflineFlag(); 
	void BuildClusterList();
	static void PopUnReadMsgDlg(FRIEND_INFO* pShowFriend = NULL);////弹出未读消息对话框
	// 打开聊天窗口，供外部调用，减少重复代码
	static void OpenP2PMsgDlg(FRIEND_INFO *pFriend);

protected:
	afx_msg LRESULT OnGroupListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDismissCluster(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImOperation(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHintClick_ClusterPage(WPARAM wParam, LPARAM lParam);

	void OnSendClusterMsg();
	void OnClusterCreate();
	void OnClusterInfo();
	void OnClusterDismiss(); //解散群
	void OnQuitCluster();
	void OnSearchCluster();
	void OnClusterMsgHistory();
	void OnCInfoRefresh();
	void OnCListRefresh();

public:
	void ImGetClusterMember(uint32 cid);
	void ImGetClusterInfo(uint32 cid);
	void ImGetClusterList();
	void ImGetOfflineMsg();

private:
	void OnClusterListAck(const char *data,WORD wRet);
	void OnClusterInfoAck(const char *data,WORD wRet);
	void OnSetCMemberAck(const char *data,WORD wRet);
	void OnClusterApplyAck(const char *data,WORD wRet);
	void OnClusterAuthInd(const char *data,WORD wRet);
	void OnCMemberInd(const char *data,WORD wRet);
	void OnClusterInviteInd(const char *data, WORD wRet);
	void OnClusterAgreeInd(const char *data, WORD wRet);
	void OnClusterDismissInd(const char *data,WORD wRet);
	void OnSetClusterInfoAck(const char *data,WORD wRet);
	// 处理获得族成员列表数据
	void OnClusterMembersAck(const char *data,WORD wRet);
#ifdef _CS_PLAT_EX_
	// 系统消息：排队队列增加成员或减少成员
	void OnWaitQueuChange(const char *data,WORD wRet);
	void OnRecepteCustAck(const char* data,WORD wRet);
	// 买家退出，主动邀约
	void OnBuyOfflineInd(const char* data,WORD wRet);
#endif
#ifdef _BUSINESS_VESION
	//设置
	void OnGetOrderSubscibe(const char *data,WORD wRet);
#endif
	
	// 阻止群消息或解除阻止
	void OnClusterRefuse();

private:
	void DelCluster(uint32 cid);
	void DelClusterMember(uint32 cid, uint32 mid);
	//------------------------------------------------------
	// 找出过时的群成员，用于垃圾数据删除
	//------------------------------------------------------
	static void FindOldMembers(
		const vector<FRIEND_INFO> & oldList,
		const vector<uint32> &		newList,
		vector<uint32> &			oldMembers
		);

	void SearchListAddGroup(LPCTSTR szText, int nHeight = 32);
	void SearchListAddFriend(LPCTSTR szImgPath, LPCTSTR szText, LPCTSTR szRemarks, DWORD tag, BOOL bUser = TRUE, int nHeight = 32);
	void SearchListRemoveAll();
	int  SearchListGetCount();
	void SearchListSetCurSel(int nIndex);
	void SearchListSelPage(int nIndex);
	void SetTempSessionBtnText(LPCTSTR pText);
	// 根据选择项，打开聊天窗口
	void OpenCharWnd();
	// 是否显示临时会话按钮
	void ShowTempSessionBtn(BOOL bShow);

public:
	afx_msg void OnBnClickedButtonCreate();

	void OnGetCMProfileAck(const char *data, WORD wRet);

	// 查找用户
	void OnSearchUser(const char *data, int nRetCode);

public:
	// 返回列表项数目
	void ShowSearchList(CString &str);
	void UpdateSearchList(CLUSTER_INFO * pCluster,CString strItem);

public:
	BOOL  IsKeyMatch(CString strKey, FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	BOOL  ShowSearchRes(CString strKey,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	BOOL  KeySearch(CString strKey,CString strName,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);

	map<CLUSTER_INFO*,CString > m_mapGroup;    ////群名称
	map<FRIEND_INFO* ,CString > m_mapCluster;  ////群中成员
	map<FRIEND_INFO* ,CString > m_mapTeamFriend;   ////好友组成员
	map<FRIEND_INFO* ,CString > m_mapFriend;   ////好友组成员

public:
	bool AddTaskIcon();
	void RemoveTaskIcon();
	void ShowTaskIcon(int sex, int status); 
	void SetTaskIconTip(CString strTip);
	static void SaveLoginAccount();
	// 获取“会话挂起”功能中菜单栏的选项
	int GetHangupMenuIndex()
	{
		return m_nHangupIndex;
	}

	// 设置“会话挂起”功能中菜单栏的选项
	void SetHangupMenuIndex(int nHangupIndex)
	{
		m_nHangupIndex = nHangupIndex;
	}

	// 获取变为挂起状态时需要的会话数
	int GetHangupStatusNeededSessions()
	{
		return m_nNumThenHangup;
	}

	// 设置变为挂起状态时需要的会话数
	void SetHangupStatusNeededSessions(int nNumThenHangup)
	{
		m_nNumThenHangup = nNumThenHangup;
	}

	// 设置是否为主动挂起
	void SetHangupSelect(bool bIsHangupSelect)
	{
		m_bIsHangupSelect = bIsHangupSelect;
	}

	bool GetHangupSelect()
	{
		return m_bIsHangupSelect;
	}

#ifdef _BUSINESS_VESION
	// 子账号挂起功能
	void Hangup();
#endif

protected:
	afx_msg void OnMenuConfig();
	afx_msg void OnMenuRegisterP2p();
	afx_msg void OnMenuOnline();
	afx_msg void OnMenuAbout();
	afx_msg void OnMenuLeave();
	afx_msg void OnMenuHide();
	afx_msg void OnMenuBusy();
	afx_msg void OnMenuOffline();
	virtual void OnGetMinMaxInfo(MINMAXINFO* lpMMI);
	afx_msg void OnBnClickedBtnLogin();
	afx_msg void OnBnClickedButtonStatus();
	afx_msg void OnStnClickedStaticStatus();
	afx_msg void OnBnClickedBtnHome();
	afx_msg void OnBnClickedBtnGold();
	afx_msg void OnDestroy();
	afx_msg void OnMoving(UINT fwSide, LPRECT pRect);
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnSearchFriend();
	afx_msg void OnCreateFamily();
	afx_msg void OnSearchFamily();
	afx_msg void OnUploadPic();
	afx_msg BOOL OnCopyData(CWnd* pWnd, COPYDATASTRUCT* pCopyDataStruct);

	LRESULT OnLoginFailed(WPARAM wParam, LPARAM lParam);
 
protected:
	LRESULT OnCopyUrl(WPARAM w,LPARAM l);
	LRESULT OnAddEmotion(WPARAM w,LPARAM l);
	LRESULT OnHotKey(WPARAM w,LPARAM l);
	LRESULT OnTaskIconClick(WPARAM w,LPARAM l);
	LRESULT OnCheckVersion(WPARAM w,LPARAM l);
	LRESULT OnNetWorkError(WPARAM w,LPARAM l);
	LRESULT OnImOk(WPARAM w,LPARAM l);
	LRESULT OnHeartTimeOut(WPARAM w,LPARAM l);
	LRESULT OnTrayIconUpdate(WPARAM w,LPARAM l);
	LRESULT OnCallMe(WPARAM w,LPARAM l);
	LRESULT OnTaskbarCreated(WPARAM w, LPARAM l);
	LRESULT OnWebPictureNotify(WPARAM w,LPARAM l);
	LRESULT OnIMESetContext(WPARAM w,LPARAM l);
	LRESULT OnExitMainFrm(WPARAM wParam,LPARAM lParam);
	LRESULT OpenSellerMsgWin(WPARAM wParam,LPARAM lParam);
	LRESULT OpenWorkMsgWin(WPARAM wParam,LPARAM lParam);
	LRESULT OpenCsInsertMsgWin(WPARAM wParam,LPARAM lParam);

	void OnLoginReq(const char *data,WORD wRet);
	void OnLoginAck(const char *data,WORD wRet);
	void OnUserInfoModified(const char *data,WORD wRet);
	void OnLogOut(const char *data,WORD wRet);
	void OnRecvSvrMsg(const char *data,WORD wRet);
	void OnNetWorkErr(const char * data,WORD wRet);
	void OnForceOfflineCsInd(const char * data, WORD wRet);
#ifdef _BUSINESS_VESION
	// 更新最近联系客户应答
	void OnUpdateNearCommCustInfoAck(const char * data, WORD wRet);
	// 收到客服转接请求
	void OnRecvCustSerTranReqInd(const char * data, WORD wRet);
	// 客服转接功能，打开客户聊天窗口
	static unsigned __stdcall ThreadCustSerTran(void * pParam);
	// 打开客户聊天窗口
	LRESULT OnOpenCustMsgDlg(WPARAM w,LPARAM l);
	// 打开订单消息窗口
	LRESULT OnOpenOrderMsgDlg(WPARAM w,LPARAM l);
#endif

protected:
	void ShowUserInfo();
	void ShowMiniPictureBroadCast();

	void UpdateUserSign();//更新个性签名
	void SafeTerminateThreads();
	void ImNotifyMySign();
	void StartMiniWebSession();
//
//#ifdef _BUSINESS_VESION
//	// 统计需要更新到后台的最近联系客户信息
//	void StatNeedUpdateCustInfo();
//	// 更新最近联系客户信息到后台
//	void UpdateCustInfoToServer();
//#endif

	// 关闭
	void CLOSE();
	void OnMove(int x, int y);

	// 开启线程检测网络连接
	static unsigned __stdcall ThreadCheckNetwork(void * pParam);
	static BOOL g_bNetworkOK;

	// 开启线程获取所有好友状态
	static unsigned __stdcall ThreadGetAllFriendStatus(void * pParam);

public:	
	WORD m_nWakeupVK;
	WORD m_nStatus;
	static int			s_nLastHeight;	// 上次窗体高度

protected:
	NOTIFYICONDATA		m_nifData		;
	bool                m_bLogined		;
	bool				m_bHeartTimeOut ;
	int					m_nUserStatus	;	
	int                 m_nLoginStatus	; //0:not logined, 1 logining.., 2:logined
	bool                m_bImOk;
	// 业务服务器列表
	CIMServerArr		m_arrBusiSrv;
	// 记录服务器列表位置
	vector<uint16>		m_arrSrvPos;

	//引导页
	CDuiGuidePage* m_pDuiGuidePage;

protected:
	HANDLE              m_hMini139UpdateThread  ;	// 后台自动更新线程
	HANDLE				m_hMiniWebSession       ;   // Mini与web维持在线线程句柄
	HANDLE				m_hNotifyWebUserSign	;
	static  UINT		MiniWebSessionProc(LPVOID param);

protected:
	FIFO<TWebPictureInfo *> m_fifoPics;
	//
	// 自动停靠功能
	//
protected:
	void				HideSide(BOOL hide,const CWnd*pAfter);
public:
	void				NearSide();		// 确定窗体的停靠方式

#ifdef _BUSINESS_VESION
	// 统计需要更新到后台的最近联系客户信息
	void StatNeedUpdateCustInfo(FRIEND_INFO *pUser);
	// 更新最近联系客户信息到后台
	void UpdateCustInfoToServer();
#endif

protected:
	BOOL				m_bForceShow;	// 是否需要强制显示
	int					m_nScreenW;		// 屏幕宽度
	int					m_nScreenH;		// 屏幕高度
	BOOL				m_bHide;
	int					m_alignType;	// 用于记录窗体停靠状态
	CString				m_sLastUser;	// 上一个用户名

	HACCEL				m_hAccel;
	DWORD				m_nHotkeyID;
	DWORD				m_nSnapShotkeyID;

protected:
	CPoint				m_ptLeftButtonDown;

protected:
	bool				m_bLoadFriendPic;//是否加在关注图片
	bool				m_bInitFriendPic;//当m_bLoadFriendPic=false的时候，用来判断是否初始化下载关注图片
	bool				m_bLoadFriendArticle;//是否加载关注wenzhang
	char				m_szExternPic[256];	// 外部程序传入的数据
	const CWnd*			m_pLastAfter;

protected:
	CNamedPipe			m_pipeCallMe;

public:
	afx_msg void OnMenuMedia();
	afx_msg void OnBnClickedBtnNearCommCust();
	// 相应系统设置窗口的自定义消息
	afx_msg LRESULT OnSelfMsgSystemSettingDlgMsg(WPARAM, LPARAM);

	// 指定发送
	afx_msg void OnSpecifySend();

	// 处理鼠标移动消息，当鼠标移动到头像区域时，弹出我的资料窗口
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// “指定发送”窗口返回的消息
	afx_msg LRESULT OnSpecSendDlgMsg(WPARAM wp, LPARAM lp);

	//改备注
	afx_msg LRESULT OnModBackName(WPARAM wp, LPARAM lp);

	// 当弹出用户信息卡片时，头像区域被点击的消息相应
	afx_msg LRESULT OnUserCardDlgFaceClicked(WPARAM wp, LPARAM lp);

	// 点击头像
	afx_msg void OnBnClickedFaceIcon();

	// 最近联系人Page页点击右键
	void OnRecentListRButtonDown();

	//注册热键
	void SetWndHotKey();

	// 关闭附加动作
	void LastOpera();

	// 返回好友列表
	CDuiFriendList* GetFriendList()		{	return m_pFriendList;	}

private:
	int m_nHangupIndex;				// 自动挂起菜单栏中的默认选项，默认为“不自动挂起”
	int m_nNumThenHangup;			// 会话数达到多少时自动挂起，默认为-1，表示不自动挂起
	bool m_bIsHangupSelect;         // 是否手动挂起
	CSpecifySendDlg *m_pSecifySendDlg;
	CSelfInfoDlg *m_pSelfInfoDlg;
	CString		m_strVersionUrl;
	bool		m_bIsNewVersion;
public:
	CPngBtn m_btnMerchatnEntrance;
	afx_msg void OnBnClickedBtnMerchantEntrance();
	void ExitApp();
	void LockDlg();
	void ReleaseDlg();
	void SaveUnReadMsgLog(FRIEND_INFO* pFriend ,int nUnReadCnt,int nIndex =0 ,BOOL bWrite = FALSE);////bWrite = FALSE时,不写.INI
	// 判断id是否为买家
	static BOOL IsBuyer(uint32 nID);
	// 判断id是否为商家
	static BOOL IsSeller(uint32 nID);
	// 判断id是否为客服工作人员
	static BOOL IsCSer(uint32 nID);
	// 判断id是否为工作版人员
	static BOOL IsWork(uint32 nID);
	//cookie中设置Token
	void SetCookieToken();

	//设置订单订阅选项
	void GetOrderSubscibe();
	//获取离线订单
	void GetOrderMsgOffline();
	//设置主界面是否显示未读消息红点
	void SetLabelOrderMsgUnRead(bool bIsUnRead);
	//是否显示红点
	bool GetLabelOrderMsgUnRead();

	//分组信息
	void GetCsTeamInfo();
protected:
	struct 
	{
		FL_ITEM_POS pos;
		CRect       rect;

	}  m_hotItem;
	list<tagHideTick>	m_lsHideTick;
	bool				m_bInvalidateLock;	
	DWORD				m_dwPrevBeatTick;
	CFont				m_fontBold;
	map<uint32,FRIEND_INFO*>	m_mapNewFriend;
	//map<uint32,uint32>          m_mapWebInfo;// map<friend id, last get web info tick>

	DWORD				m_nLastHeartSeq;			// 上次心跳应答的序列号

	bool				m_bDisconnectFlag;
	uint8				m_nPreOnlineStatus;

private:
	static std::tr1::shared_ptr<CMainFrame> pInstance;
	DuiLib::CLabelUI *m_pLabelUmMsg;
	CDuiFriendList *m_pFriendList;
	DuiLib::CLabelUI *m_pTitle;
	DuiLib::COptionUI *m_pFriendPage;
	DuiLib::CLabelUI *m_plbUserName;
	DuiLib::CButtonUI *m_pbtnStatus;
	DuiLib::CEditUI *m_pEditSign;
	DuiLib::CVerticalLayoutUI *m_pUserImg;
	DuiLib::CButtonGifUI *m_pHead;

	DuiLib::CTabLayoutUI *m_pLayoutSearch;
	DuiLib::CEditUI *m_pEditSearchView;
	DuiLib::CButtonUI *m_pBtnClearSearch;
	DuiLib::CEditUI *m_pEditSearchValue;

	DuiLib::CTabLayoutUI *m_pLayoutPage;
	DuiLib::CTabLayoutUI *m_pLayoutSearcePage;
	DuiLib::CButtonGifUI *m_pBtnModSelfInfo;

	CDuiClusterList *m_pClusterList;
	CDuiRecentList *m_pRecentList;
	DuiLib::CButtonUI *m_pbtnAddCluster;
	DuiLib::CButtonUI *m_pbtnSellerHome;
	DuiLib::CButtonUI *m_pbtnNearCust;
	vector<uint16> m_vecPos;					// 防止收到重复的获取好友列表应答包
	vector<FRIENDITEM> m_arrFriend;			// 从服务器返回的好友列表	

#ifdef _BUSINESS_VESION
	bool				m_bHasUpdateNearCommCust;	// 是否已经更新了后台“最近联系客户”	
	std::vector<XT_COMMINFO_UPDATE> m_vecNeedUpdateCommInfo;	// 需要更新到后台的客户信息	
	BOOL m_bTranComplete;			// 是否更新完成
#endif
	vector<uint32>			m_arrNeedGetBasicInfo;	// 需要获取基本资料的好友
	vector<uint32>			m_memberList;			// 返回的群成员列表
	bool					m_bNeedSort;
	CString m_sFriendCntInfo;

public:
	int m_nLastReqSeq;
    int m_nUnReadCnt;
private:
	CSearchListUI *m_pSearchList;
	DuiLib::CTabLayoutUI *m_pTab;
	DuiLib::CButtonUI *m_pBtnTempSession;

	BOOL m_bExit;
	CScreenCtrl* m_pScreenCtrl;
	BOOL m_bFriendWnd;////当前选择了好友页面

	vector<uint32> m_arrFriendsID_Basic;	// 需要获取基本信息的好友id
};
