#pragma once

#include "afxwin.h"

#include "im\tabpage1.h"//����
#include "im\tabpage2.h"//�ᣬ���˿ռ����
#include "im\tabpage3.h"//�������ʱ�
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
// �򿪶������ڵ���Ϣ��Ӧ
#define WM_OPEN_ORDER_MSG_DLG            WM_USER+2102

#define NEAR_SIZE			5		// �����Զ�ͣ����Ч����
#define LEAVE_SIZE			15		// ȡ���Զ�ͣ��ʱ��Ҫ����Ч����	
#define NEAR_SIDE			2		// �������غ�����Ļ�ϱ��������أ���ʹ�����Դ���
#define TIMER_HIDE			0x1000	// �������Ƿ��ڴ����ڵ��¼�ID
#define AUOT_CHECK_TIME		200		// ��ʱ����ʱ����

//�ؼ�λ��
#define TAP_LEFT 1
#define TAP_TOP 121
#define BOTTOM_HEIGHT 40

// �����б���ʹ�õĲ˵�
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
#define	ID_DELMFRIEND			0x0114	// ɾ���������
#define ID_MODF_COMMNAME		0x0115	// �޸ĺ��ѱ�ע����
#define ID_CREATE_TEMPCLUSTER	0x0116 
#define	ID_FRIENDARTICLE		0x0117
#define	ID_SEND_GROUP_MESSAGE	0x0118
#define	ID_ADD_CGROUP			0x0119
#define	ID_ADD_FRIEND			0x0120
#define	ID_RENAME_GROUP			0x0121
#define ID_REMOVE_BLACKLIST		0x0122
#define ID_ADD_BLACKLIST		0x0123

// Ⱥ�б���ʹ�õĲ˵�
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

// �����ϵ���б���ʹ�õĲ˵�
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

	bool	bTry;	// �Ƿ��Թ�����

};

class CIMServerArr:public std::vector<CIMServer>
{
public:
	//------------------------------------------------------
	// ���һ���������б��У���������򷵻�
	//------------------------------------------------------
	void		AddServer(const CIMServer & srv)
	{
		if(FindServer(srv.ip) == -1)
			push_back(srv);
	};
	//------------------------------------------------------
	// ���ݷ�����ip��ȡ�����б��е�����λ��
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
	// ��ȡһ��û�����������ӵķ�������
	// ����ֵ��ʧ��-1���ɹ���Ϊ���б��е�����
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
	// ���ط������б�
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
	// ��ȡ�������б� 
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
	// ��λ
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
		TIMER_BEAT =0x02,  //����
		TIMER_RELOGIN =0x03, //�Զ��������ӷ�����
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

	// ����ͣ����ʽ
	enum WND_ALIGN_TYPE
	{
		ALIGN_NONE,		// ��ͣ��
		ALIGN_TOP,		// ͣ���ϱ�
		ALIGN_LEFT,		// ͣ�����
		ALIGN_RIGHT		// ͣ���ұ�
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
	// ��Ϣ
	void Notify(DuiLib::TNotifyUI& msg);
	CDuiRecentList* GetRecentList()	{	return m_pRecentList;	}

private:
	void InitializeIm();
	LRESULT OnWebIconDownload(WPARAM w, LPARAM l);

public:
	//------------------------------------------------------
	// ��������ӵĺ����б�
	// oldFriends	�ɺ����б�
	// newFriends	�»�ȡ�ĺ����б�
	// addiFriends	�Ƚϵó������ӵĺ����б�
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
	//�����û�״̬.
	void ChangeUserStatus(uint8 status);

	void StartHeartBeart();

	void OnNetWorkError();

	//�ͻ�������
	void SetUserOffLineFlag();

	void StartWebInfoTimer();

	void FindItem(FRIEND_INFO *pUser, int & nGroup, int & nItem);
	void SelItem(FRIEND_INFO * pUser);

	void ShowGuiPage();//��ʾ����������ҳ

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
	//�����б�˵�������Ӧ����
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
	//�������
	void OnAddSubGroup();
	void OnRemoveBlackList();
	void OnAddBlackList();
	// ���ط����ߺ���
	void OnHideOffline();
	// ��ʾ���к���
	void OnShowAll();
	// �޸ĺ��ѱ�ע����
	void OnModfCommName();
	// ������ʱȺ��
	void OnCreateTempCluster();

protected:
	//IMЭ���֪ͨ
	//��Ӻ������󡢾ܾ���ͨ�� ֪ͨ
	void OnStatusGetAck(const char *data,WORD wRet);
	void OnUserAuthInd(const char *data,WORD wRet);
	void OnUserAuthAck(const char *data,WORD wRet);
	void OnUserInfoGet(const char *data,WORD wRet);
	void OnFriendGroupModAck(const char *data,WORD wRet);
	void OnGroupModAck(const char *data,WORD wRet);
	void OnGroupInfoGet(const char *data,WORD wRet);
	void OnHeartBeat(const char * data,WORD wRet);
	void OnFriendStatusChange(const char *data,WORD wRet);//����״̬����֪ͨ
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

	// �޸ĺ������ƣ����棩
	void SetCommName(FRIEND_INFO* pFriend, const CString& sCommName);
	// ���ú�������
	void ResetName(uint32 fid);
	void ModifyName(XT_MOD_FRIEND_COMMNAME_ACK *pAck);///�޸ı�ע

protected:

	void		ImGetFriendList(int start_pos=0, int type=FLS_TOTAL );
	//����������������
	void		ImHeartBeat();
	void		ImSaveUserOperations();
	void		ImGetGroupInfo();
	//���һ���û���
	void		ImAddGroup(int nParentGroupId, int nGroupId,CString strGroupName);
	void		ImRenameGroup(int nGroupId, CString strGroupName);
	void		ImDelGroup(int nGroupId);
	//���ú���������
	void		ImSetUserGroup(uint32 fid, int nGroupId,uint8 action=GROUP_MODIFY);
	//��ȡ��������״̬
	void		ImGetUserStatus(uint32 uid);
	//------------------------------------------------------
	// ��ȡ���ѻ�������
	//------------------------------------------------------
	void		ImGetFriendBasicInfo() const;

	void		IMSetFoucs(uint32 fid,uint8 focus);
protected:

	//�����б���
	void UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort=TRUE);	//���º���״̬ͼ��
	//������������
	void UpdateFriendCnt();
	void UpdateFriendList();
	void SortGroupList();
	void SortGroupList(int nGroup);
	void QuickSortGroupList(int nGroup, int iLo, int iHi);
	void DelFriend(FRIEND_INFO *pFriend);
	void DelFriend(uint32 nFriendId);
public:
	bool AddFriend(FRIEND_INFO *pFriend,bool bSel);
	void DeleteStranger(int nAddId);//�̼Ұ����İ�����б�
protected:
	// ����������,���յ�������Ϣ��ʱ��,��������
	// ��һ����ʱ�������
	void RemoveHideTick(uint32 fid);
	void AddHideTick(uint32 fid);
	void CheckHideTick();

	void CreateGroup(int nParentGroupId, int& nGroupId, CString& strGroupName);

protected:

	// �ӱ��ؼ��غ����б�
	void	LoadLocalFriends();
	// �������б��浽����
	void	SaveLocalFriends();
	// 
	void	ShowUserAuthInfo(uint32 fid,uint16 action/*0���󣬣�ͨ�������ܾ�*/,CString strVerify,uint32 nGroupId);
	// ��ĳ�����ߺ��ѽ�����ʱȺ��
	void	MakeTempCluster(FL_GROUP_INFO* pGroup);
	// �����б�ؼ��ڶ�ѡʱ��������Ҽ�
	void	OnNCFlRbum();

#ifdef _BUSINESS_VESION
	// ����ҵ��Ŷӷ��飬����Ѿ����ھͲ����
	BOOL AddGroup_MyTeam();
#endif

	// �Զ��ظ��������߻���æµʱ��
	static unsigned __stdcall AutoReply(void * pParam);

	// ���º�����Ϣ
	static unsigned __stdcall UpdateFriendInfo(void * pParam);

protected:
	LRESULT  OnThreadNotify(WPARAM,LPARAM);

	// ��ȡ�µ���ID���Զ������ʱ�õ���
	int GetNewGroupID();


public:
	afx_msg void OnBnClickedBtnCluster();

public:
	void LoadLoacalClusterList();
	void BuildCluster(uint32 cid);
	void SetClusterOfflineFlag(); 
	void BuildClusterList();
	static void PopUnReadMsgDlg(FRIEND_INFO* pShowFriend = NULL);////����δ����Ϣ�Ի���
	// �����촰�ڣ����ⲿ���ã������ظ�����
	static void OpenP2PMsgDlg(FRIEND_INFO *pFriend);

protected:
	afx_msg LRESULT OnGroupListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnDismissCluster(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImOperation(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHintClick_ClusterPage(WPARAM wParam, LPARAM lParam);

	void OnSendClusterMsg();
	void OnClusterCreate();
	void OnClusterInfo();
	void OnClusterDismiss(); //��ɢȺ
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
	// ���������Ա�б�����
	void OnClusterMembersAck(const char *data,WORD wRet);
#ifdef _CS_PLAT_EX_
	// ϵͳ��Ϣ���ŶӶ������ӳ�Ա����ٳ�Ա
	void OnWaitQueuChange(const char *data,WORD wRet);
	void OnRecepteCustAck(const char* data,WORD wRet);
	// ����˳���������Լ
	void OnBuyOfflineInd(const char* data,WORD wRet);
#endif
#ifdef _BUSINESS_VESION
	//����
	void OnGetOrderSubscibe(const char *data,WORD wRet);
#endif
	
	// ��ֹȺ��Ϣ������ֹ
	void OnClusterRefuse();

private:
	void DelCluster(uint32 cid);
	void DelClusterMember(uint32 cid, uint32 mid);
	//------------------------------------------------------
	// �ҳ���ʱ��Ⱥ��Ա��������������ɾ��
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
	// ����ѡ��������촰��
	void OpenCharWnd();
	// �Ƿ���ʾ��ʱ�Ự��ť
	void ShowTempSessionBtn(BOOL bShow);

public:
	afx_msg void OnBnClickedButtonCreate();

	void OnGetCMProfileAck(const char *data, WORD wRet);

	// �����û�
	void OnSearchUser(const char *data, int nRetCode);

public:
	// �����б�����Ŀ
	void ShowSearchList(CString &str);
	void UpdateSearchList(CLUSTER_INFO * pCluster,CString strItem);

public:
	BOOL  IsKeyMatch(CString strKey, FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	BOOL  ShowSearchRes(CString strKey,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);
	BOOL  KeySearch(CString strKey,CString strName,FRIEND_INFO *pFriend,CLUSTER_INFO *pCluster);

	map<CLUSTER_INFO*,CString > m_mapGroup;    ////Ⱥ����
	map<FRIEND_INFO* ,CString > m_mapCluster;  ////Ⱥ�г�Ա
	map<FRIEND_INFO* ,CString > m_mapTeamFriend;   ////�������Ա
	map<FRIEND_INFO* ,CString > m_mapFriend;   ////�������Ա

public:
	bool AddTaskIcon();
	void RemoveTaskIcon();
	void ShowTaskIcon(int sex, int status); 
	void SetTaskIconTip(CString strTip);
	static void SaveLoginAccount();
	// ��ȡ���Ự���𡱹����в˵�����ѡ��
	int GetHangupMenuIndex()
	{
		return m_nHangupIndex;
	}

	// ���á��Ự���𡱹����в˵�����ѡ��
	void SetHangupMenuIndex(int nHangupIndex)
	{
		m_nHangupIndex = nHangupIndex;
	}

	// ��ȡ��Ϊ����״̬ʱ��Ҫ�ĻỰ��
	int GetHangupStatusNeededSessions()
	{
		return m_nNumThenHangup;
	}

	// ���ñ�Ϊ����״̬ʱ��Ҫ�ĻỰ��
	void SetHangupStatusNeededSessions(int nNumThenHangup)
	{
		m_nNumThenHangup = nNumThenHangup;
	}

	// �����Ƿ�Ϊ��������
	void SetHangupSelect(bool bIsHangupSelect)
	{
		m_bIsHangupSelect = bIsHangupSelect;
	}

	bool GetHangupSelect()
	{
		return m_bIsHangupSelect;
	}

#ifdef _BUSINESS_VESION
	// ���˺Ź�����
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
	// ���������ϵ�ͻ�Ӧ��
	void OnUpdateNearCommCustInfoAck(const char * data, WORD wRet);
	// �յ��ͷ�ת������
	void OnRecvCustSerTranReqInd(const char * data, WORD wRet);
	// �ͷ�ת�ӹ��ܣ��򿪿ͻ����촰��
	static unsigned __stdcall ThreadCustSerTran(void * pParam);
	// �򿪿ͻ����촰��
	LRESULT OnOpenCustMsgDlg(WPARAM w,LPARAM l);
	// �򿪶�����Ϣ����
	LRESULT OnOpenOrderMsgDlg(WPARAM w,LPARAM l);
#endif

protected:
	void ShowUserInfo();
	void ShowMiniPictureBroadCast();

	void UpdateUserSign();//���¸���ǩ��
	void SafeTerminateThreads();
	void ImNotifyMySign();
	void StartMiniWebSession();
//
//#ifdef _BUSINESS_VESION
//	// ͳ����Ҫ���µ���̨�������ϵ�ͻ���Ϣ
//	void StatNeedUpdateCustInfo();
//	// ���������ϵ�ͻ���Ϣ����̨
//	void UpdateCustInfoToServer();
//#endif

	// �ر�
	void CLOSE();
	void OnMove(int x, int y);

	// �����̼߳����������
	static unsigned __stdcall ThreadCheckNetwork(void * pParam);
	static BOOL g_bNetworkOK;

	// �����̻߳�ȡ���к���״̬
	static unsigned __stdcall ThreadGetAllFriendStatus(void * pParam);

public:	
	WORD m_nWakeupVK;
	WORD m_nStatus;
	static int			s_nLastHeight;	// �ϴδ���߶�

protected:
	NOTIFYICONDATA		m_nifData		;
	bool                m_bLogined		;
	bool				m_bHeartTimeOut ;
	int					m_nUserStatus	;	
	int                 m_nLoginStatus	; //0:not logined, 1 logining.., 2:logined
	bool                m_bImOk;
	// ҵ��������б�
	CIMServerArr		m_arrBusiSrv;
	// ��¼�������б�λ��
	vector<uint16>		m_arrSrvPos;

	//����ҳ
	CDuiGuidePage* m_pDuiGuidePage;

protected:
	HANDLE              m_hMini139UpdateThread  ;	// ��̨�Զ������߳�
	HANDLE				m_hMiniWebSession       ;   // Mini��webά�������߳̾��
	HANDLE				m_hNotifyWebUserSign	;
	static  UINT		MiniWebSessionProc(LPVOID param);

protected:
	FIFO<TWebPictureInfo *> m_fifoPics;
	//
	// �Զ�ͣ������
	//
protected:
	void				HideSide(BOOL hide,const CWnd*pAfter);
public:
	void				NearSide();		// ȷ�������ͣ����ʽ

#ifdef _BUSINESS_VESION
	// ͳ����Ҫ���µ���̨�������ϵ�ͻ���Ϣ
	void StatNeedUpdateCustInfo(FRIEND_INFO *pUser);
	// ���������ϵ�ͻ���Ϣ����̨
	void UpdateCustInfoToServer();
#endif

protected:
	BOOL				m_bForceShow;	// �Ƿ���Ҫǿ����ʾ
	int					m_nScreenW;		// ��Ļ���
	int					m_nScreenH;		// ��Ļ�߶�
	BOOL				m_bHide;
	int					m_alignType;	// ���ڼ�¼����ͣ��״̬
	CString				m_sLastUser;	// ��һ���û���

	HACCEL				m_hAccel;
	DWORD				m_nHotkeyID;
	DWORD				m_nSnapShotkeyID;

protected:
	CPoint				m_ptLeftButtonDown;

protected:
	bool				m_bLoadFriendPic;//�Ƿ���ڹ�עͼƬ
	bool				m_bInitFriendPic;//��m_bLoadFriendPic=false��ʱ�������ж��Ƿ��ʼ�����ع�עͼƬ
	bool				m_bLoadFriendArticle;//�Ƿ���ع�עwenzhang
	char				m_szExternPic[256];	// �ⲿ�����������
	const CWnd*			m_pLastAfter;

protected:
	CNamedPipe			m_pipeCallMe;

public:
	afx_msg void OnMenuMedia();
	afx_msg void OnBnClickedBtnNearCommCust();
	// ��Ӧϵͳ���ô��ڵ��Զ�����Ϣ
	afx_msg LRESULT OnSelfMsgSystemSettingDlgMsg(WPARAM, LPARAM);

	// ָ������
	afx_msg void OnSpecifySend();

	// ��������ƶ���Ϣ��������ƶ���ͷ������ʱ�������ҵ����ϴ���
	//afx_msg void OnMouseMove(UINT nFlags, CPoint point);

	// ��ָ�����͡����ڷ��ص���Ϣ
	afx_msg LRESULT OnSpecSendDlgMsg(WPARAM wp, LPARAM lp);

	//�ı�ע
	afx_msg LRESULT OnModBackName(WPARAM wp, LPARAM lp);

	// �������û���Ϣ��Ƭʱ��ͷ�����򱻵������Ϣ��Ӧ
	afx_msg LRESULT OnUserCardDlgFaceClicked(WPARAM wp, LPARAM lp);

	// ���ͷ��
	afx_msg void OnBnClickedFaceIcon();

	// �����ϵ��Pageҳ����Ҽ�
	void OnRecentListRButtonDown();

	//ע���ȼ�
	void SetWndHotKey();

	// �رո��Ӷ���
	void LastOpera();

	// ���غ����б�
	CDuiFriendList* GetFriendList()		{	return m_pFriendList;	}

private:
	int m_nHangupIndex;				// �Զ�����˵����е�Ĭ��ѡ�Ĭ��Ϊ�����Զ�����
	int m_nNumThenHangup;			// �Ự���ﵽ����ʱ�Զ�����Ĭ��Ϊ-1����ʾ���Զ�����
	bool m_bIsHangupSelect;         // �Ƿ��ֶ�����
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
	void SaveUnReadMsgLog(FRIEND_INFO* pFriend ,int nUnReadCnt,int nIndex =0 ,BOOL bWrite = FALSE);////bWrite = FALSEʱ,��д.INI
	// �ж�id�Ƿ�Ϊ���
	static BOOL IsBuyer(uint32 nID);
	// �ж�id�Ƿ�Ϊ�̼�
	static BOOL IsSeller(uint32 nID);
	// �ж�id�Ƿ�Ϊ�ͷ�������Ա
	static BOOL IsCSer(uint32 nID);
	// �ж�id�Ƿ�Ϊ��������Ա
	static BOOL IsWork(uint32 nID);
	//cookie������Token
	void SetCookieToken();

	//���ö�������ѡ��
	void GetOrderSubscibe();
	//��ȡ���߶���
	void GetOrderMsgOffline();
	//�����������Ƿ���ʾδ����Ϣ���
	void SetLabelOrderMsgUnRead(bool bIsUnRead);
	//�Ƿ���ʾ���
	bool GetLabelOrderMsgUnRead();

	//������Ϣ
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

	DWORD				m_nLastHeartSeq;			// �ϴ�����Ӧ������к�

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
	vector<uint16> m_vecPos;					// ��ֹ�յ��ظ��Ļ�ȡ�����б�Ӧ���
	vector<FRIENDITEM> m_arrFriend;			// �ӷ��������صĺ����б�	

#ifdef _BUSINESS_VESION
	bool				m_bHasUpdateNearCommCust;	// �Ƿ��Ѿ������˺�̨�������ϵ�ͻ���	
	std::vector<XT_COMMINFO_UPDATE> m_vecNeedUpdateCommInfo;	// ��Ҫ���µ���̨�Ŀͻ���Ϣ	
	BOOL m_bTranComplete;			// �Ƿ�������
#endif
	vector<uint32>			m_arrNeedGetBasicInfo;	// ��Ҫ��ȡ�������ϵĺ���
	vector<uint32>			m_memberList;			// ���ص�Ⱥ��Ա�б�
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
	BOOL m_bFriendWnd;////��ǰѡ���˺���ҳ��

	vector<uint32> m_arrFriendsID_Basic;	// ��Ҫ��ȡ������Ϣ�ĺ���id
};
