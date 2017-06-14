#pragma once

#include "imclusterlistctrl.h"

#include "FormProgress.h"

#include <map>
using namespace std;
#include "FloorPage.h"

// 外部发送解散族的消息
class CFloorPageCluster : public CFloorPage
{
	DECLARE_DYNAMIC(CFloorPageCluster)
	DECLARE_HMG_SCROLLBAR()

public:
	CFloorPageCluster(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CFloorPageCluster();

// 对话框数据
	enum { IDD = IDD_FLOOR_CLUSTER };

public:
	void LoadLoacalClusterList();
	void BuildClusterList();
	void BuildCluster(uint32 cid);
	void SetClusterOfflineFlag(); 
	void OnLogOut();
	void SetClusterFlashFlag(uint32 cid, bool bFlash );
	void SelectUserOnList(uint32 cid, uint32 uid);
	void InitializeList();

public:
	CIMClusterListCtrl  m_wndClusterList	;
	CFlatButton         m_btnCreate, m_btnSearch;

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnDestroy();
	afx_msg LRESULT OnGroupListNotify(WPARAM wParam, LPARAM lParam);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnInitMenuPopup(CMenu* pPopupMenu, UINT nIndex, BOOL bSysMenu);
	afx_msg void OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct);
	
	afx_msg LRESULT OnDismissCluster(WPARAM wParam, LPARAM lParam);
	afx_msg LRESULT OnImOperation(WPARAM wParam,LPARAM lParam);
	afx_msg LRESULT OnHintDlgClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnFamilyIconNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

	void OnSendClusterMsg();
	void OnSendP2PMsg();
	void OnClusterCreate();
	void OnFriendInfo();
	void OnClusterInfo();
	void OnClusterDismiss(); //解散群
	void OnClusterAlbum();
	void OnClusterHome();
	void OnClusterForum();
	void OnQuitCluster();
	void OnMemberDel();
	void OnAdd2Friend();
	void OnSearchCluster();
	void OnClusterMsgHistory();
	void OnFriendMsgHistory();
	void OnClusterRefuse();
	void OnUserBlog();
	void OnUserAlbum();
	void OnCInfoRefresh();
	void OnCListRefresh();
	void OnCMemberLoad();

public:
	void ImGetClusterMember(uint32 cid);
	void ImGetClusterInfo(uint32 cid);
	void ImGetClusterList();
//	void ImGetCofflineMsg(uint32 cid);
	void ImGetOfflineMsg();
	static void ImGetFriendBasicInfo(const vector<uint32> & arrFriend) ;

private:
	void OnCMemberStatusChange(const char *data,WORD wRet);//群成员状态更改通知
	void OnClusterListAck(const char *data,WORD wRet);
	void OnClusterMembersAck(const char *data,WORD wRet);
	void OnClusterInfoAck(const char *data,WORD wRet);
	void OnRecvClusterMsg(const char *data,WORD wRet);
	void OnSetCMemberAck(const char *data,WORD wRet);
	void OnClusterApplyAck(const char *data,WORD wRet);
	void OnRecvCofflineMsg(const char *data,WORD wRet);
	void OnClusterAuthInd(const char *data,WORD wRet);
	void OnCMemberInd(const char *data,WORD wRet);
	void OnClusterInviteInd(const char *data, WORD wRet);
	void OnClusterAgreeInd(const char *data, WORD wRet);
	void OnGetCMemberReq(const char *data,WORD wRet);
	void OnClusterDismissInd(const char *data,WORD wRet);
	void OnUserInfoGetAck(const char *data,WORD wRet);
	void OnStatusModAck(const char *data,WORD wRet);
	void OnStatusGetAck(const char *data,WORD wRet);
	void OnSetClusterInfoAck(const char *data,WORD wRet);
	void OnGetFriendBasicInfoAck(const char *data,WORD wRet);

private:
	void SortGroupList();
	void SortClusterList(uint32 cid);
	void SortClusterListReal(uint32 cid);
	void QuickSort(int nRoot, int nGroup, int iLo, int iHi);
	void DelCluster(uint32 cid);
	void DelClusterMember(uint32 cid, uint32 mid);
	void AddClusterMemberLoadFlag(uint32 cid);
	int  FindClusterMember(CL_GROUP_POS posGroup, uint32 mid);
	void ClearMembersPos();
	void ClearMembersPos(uint32 cid);
	//------------------------------------------------------
	// 找出过时的群成员，用于垃圾数据删除
	//------------------------------------------------------
	static void FindOldMembers(
		const vector<FRIEND_INFO> & oldList,
		const vector<uint32> &		newList,
		vector<uint32> &			oldMembers
	);
private:

	struct tagPos
	{
		vector<uint16> vecPos;
	};

	map<uint32,tagPos *>	m_mapPos;
	vector<uint32>			m_arrNeedGetBasicInfo;	// 需要获取基本资料的好友
	vector<uint32>			m_memberList;			// 返回的群成员列表

	CFormProgress2 *		m_pProgressLoad ;
	bool					m_bNeedSort;
public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnPaint();
	afx_msg void OnShowWindow(BOOL bShow, UINT nStatus);
	afx_msg void OnBnClickedButtonCreate();

	void OnGetCMProfileAck(const char *data, WORD wRet);
};


