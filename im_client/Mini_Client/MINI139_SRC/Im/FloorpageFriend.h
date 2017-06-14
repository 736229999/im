#pragma once

#include "improtocol.h"
#include "LoginDlg.h"
#include "impublic.h"

#include "imfriendlistctrl.h"


#include "FloorPage.h"
#include "webprocess.h"
#include "buttonext.h"

#define	 ID_GROUP_CHOOSE_BEG	0x7001
#define	 ID_GROUP_CHOOSE_END	0x7020

struct tagHideTick
{
	uint32 fid;
	DWORD dwTick;
};

/////////////////////////////////////////////////////////////////////////////
// CFloorPageFriend window
class CFloorPageFriend : public CFloorPage//CDialog
{
	DECLARE_DYNAMIC(CFloorPageFriend)
	DECLARE_HMG_SCROLLBAR()

public:
	CFloorPageFriend(CWnd* pParent = NULL);
	virtual ~CFloorPageFriend();
	enum { IDD = IDD_FLOOR_FRIEND };
	enum { 
			TIMER_BEAT =0x02,  //心跳
			TIMER_RELOGIN =0x03, //自动重新连接服务器
			TIMER_INVALIDATELIST=0x05,
			TIMER_SORTLIST=0x06,
			TIMER_CHECK_NETWORD=0x07,
			//TIMER_USER_CARD=0x07
	};
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

	void InitializeIm();

	void OnLogOut();

	void OnNetWorkError();

	//客户端离线
	void SetUserOffLineFlag();

	void StartWebInfoTimer();

	void FindItem(FRIEND_INFO *pUser, int & nGroup, int & nItem);
	void SelItem(FRIEND_INFO * pUser);

public:
	CIMFriendListCtrl  m_wndGroupList;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 持
	virtual BOOL OnInitDialog();
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg LRESULT OnGroupListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	afx_msg	void OnGroupChoose(UINT nID);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedButtonAdd();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

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
	void OnFriendListAck(const char *data,WORD wRet);
	void OnRecvSvrMsg(const char *data,WORD wRet);
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
	void		ImGetFriendBasicInfo(const vector<uint32> & arrFriend) const;

	void		IMSetFoucs(uint32 fid,uint8 focus);
protected:

	//好友列表处理
	void UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort=TRUE);	//更新好友状态图标
	//好友数量更新
	void UpdateFriendCnt();
	void UpdateFriendList();
	void SortGroupList(int nGroup);
	void QuickSortGroupList(int nGroup, int iLo, int iHi);
	void DelFriend(FRIEND_INFO *pFriend);
	void DelFriend(uint32 nFriendId);
public:
	bool AddFriend(FRIEND_INFO *pFriend,bool bSel);
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
	static unsigned __stdcall UpdateFriend(void * pParam);

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
	vector<uint16>				m_vecPos;		// 防止收到重复的获取好友列表应答包
	map<uint32,FRIEND_INFO*>	m_mapNewFriend;
	map<uint32,uint32>          m_mapWebInfo;// map<friend id, last get web info tick>

	BOOL				m_bShowAllFriend;
	vector<FRIENDITEM>	m_arrFriend;			// 从服务器返回的好友列表	
	DWORD				m_nLastHeartSeq;			// 上次心跳应答的序列号


	bool				m_bDisconnectFlag;
	uint8				m_nPreOnlineStatus;
	bool                m_bIsHideTick;

protected:
	LRESULT  OnThreadNotify(WPARAM,LPARAM);

	// 获取新的组ID（自定义分组时用到）
	int GetNewGroupID();


public:
	afx_msg void OnBnClickedBtnCluster();
	CString m_sFriendCntInfo;
};


extern CFloorPageFriend *g_FloorPageFriend;

