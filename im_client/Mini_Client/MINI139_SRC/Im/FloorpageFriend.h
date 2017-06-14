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
			TIMER_BEAT =0x02,  //����
			TIMER_RELOGIN =0x03, //�Զ��������ӷ�����
			TIMER_INVALIDATELIST=0x05,
			TIMER_SORTLIST=0x06,
			TIMER_CHECK_NETWORD=0x07,
			//TIMER_USER_CARD=0x07
	};
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

	void InitializeIm();

	void OnLogOut();

	void OnNetWorkError();

	//�ͻ�������
	void SetUserOffLineFlag();

	void StartWebInfoTimer();

	void FindItem(FRIEND_INFO *pUser, int & nGroup, int & nItem);
	void SelItem(FRIEND_INFO * pUser);

public:
	CIMFriendListCtrl  m_wndGroupList;


protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ��
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
	void		ImGetFriendBasicInfo(const vector<uint32> & arrFriend) const;

	void		IMSetFoucs(uint32 fid,uint8 focus);
protected:

	//�����б���
	void UpdateFriend(FRIEND_INFO *pFriend,BOOL bSort=TRUE);	//���º���״̬ͼ��
	//������������
	void UpdateFriendCnt();
	void UpdateFriendList();
	void SortGroupList(int nGroup);
	void QuickSortGroupList(int nGroup, int iLo, int iHi);
	void DelFriend(FRIEND_INFO *pFriend);
	void DelFriend(uint32 nFriendId);
public:
	bool AddFriend(FRIEND_INFO *pFriend,bool bSel);
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
	vector<uint16>				m_vecPos;		// ��ֹ�յ��ظ��Ļ�ȡ�����б�Ӧ���
	map<uint32,FRIEND_INFO*>	m_mapNewFriend;
	map<uint32,uint32>          m_mapWebInfo;// map<friend id, last get web info tick>

	BOOL				m_bShowAllFriend;
	vector<FRIENDITEM>	m_arrFriend;			// �ӷ��������صĺ����б�	
	DWORD				m_nLastHeartSeq;			// �ϴ�����Ӧ������к�


	bool				m_bDisconnectFlag;
	uint8				m_nPreOnlineStatus;
	bool                m_bIsHideTick;

protected:
	LRESULT  OnThreadNotify(WPARAM,LPARAM);

	// ��ȡ�µ���ID���Զ������ʱ�õ���
	int GetNewGroupID();


public:
	afx_msg void OnBnClickedBtnCluster();
	CString m_sFriendCntInfo;
};


extern CFloorPageFriend *g_FloorPageFriend;

