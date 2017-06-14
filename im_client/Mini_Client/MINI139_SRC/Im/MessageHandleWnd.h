#pragma once

#include <map>
using namespace std;

#include "p2pSendmsgDlg.h"
#include "clustersendmsgdlg.h"
#include "improtocol.h"
#include "LocalData.h"
#include "DuiP2PSendMsgDlg.h"
#include "..\FriendInfoDlg.h"


class CMessageHandleWnd : public CWnd
{
public:
	virtual ~CMessageHandleWnd();

public:

	static CMessageHandleWnd * GetInstance() ;
	void UpdateBk();
	void SetNotifyIconData ( NOTIFYICONDATA *pData )
	{
		m_pNifData = pData;
	}
	void OnRecvClusterMsg(CLUSTER_INFO *pCluster);
	void OnRecvP2pMsg(FRIEND_INFO *pFrien);

	void OnHearTimeOut();

	void CloseP2PSendMsgDlg(uint32 fid);
	void CloseAllP2PSendMsgDlg();
	void CloseClusterSendMsgDlg(uint32 cid);//cluster id
	bool FindP2PSendMsgDlg(uint32 fid);
	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* FindP2PSendMsgDlgById(uint32 fid);
	CClusterSendMsgDlg * FindClusterSendMsgDlg(uint32 cid);
	bool FindGroupTalkDlg(uint32 cid);

	bool HasP2PFile();

	void AddSystemMessage(uint32 time_recv, CString strMessage);
	void AddSMS();

	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * GetP2PSendMsgDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo = StruExtraInfo(),int nUnReadNum = 0);
	CClusterSendMsgDlg * GetClusterSendMsgDlg(CLUSTER_INFO *pClusterInfo);
	/*CP2PSendMsgDlg*/CP2PSendMsgDlg * GetTempClusterSendMsgDlg(uint32 gid);
	/*CP2PSendMsgDlg*/CP2PSendMsgDlg * GetTempClusterSendMsgDlg2(uint32 gid);
	// 查询由组在线成员组成的临时群聊窗体是否存在，不存在则返回NULL
	/*CP2PSendMsgDlg*/CP2PSendMsgDlg * GetTempGroupSendMsgDlg(int nGroupIndex);

	void AddP2PSendMsgDlg(CDuiP2PSendMsgDlg* pDlg);
	void DeleteAllP2PSendMsgDlg();
	void UpdateAllFastWord();
	CScreenCtrl* GetScreenCtrl();
	void NewScreenCtrl();
	void DeleteScreenCtrl();
	// 获取含有未读消息的对话框数量
	int GetStrangerMsgDlgCount();
	void P2PSendMsgDlgNotify(TP2PMode w,FRIEND_INFO* l);

#ifdef _CS_PLAT_EX_
	void CloseAllMonitor();
#endif
protected:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();

protected:
	void SetHeartTimeOut();
	LRESULT OnP2PSendMsgDlgNotify(WPARAM,LPARAM);
	LRESULT OnClusterSendMsgDlgNotify(WPARAM,LPARAM);
	LRESULT OnP2pToGroupDlg(WPARAM,LPARAM);
	LRESULT OnShowMessage(WPARAM,LPARAM);
	LRESULT OnTrayIconClick(WPARAM wParam, LPARAM lParam);
	LRESULT OnModuleNotify(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()

private:
	CMessageHandleWnd();

	static CMessageHandleWnd *pInstance;
	CScreenCtrl* m_pScreenCtrl;
public:
	//map<uint32, CDuiP2PSendMsgDlg *> m_mapDuiP2PSendMsgDlg;
	map<uint32, /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *> m_mapP2PSendMsgDlg;
	vector<CDuiP2PSendMsgDlg*> m_vecAllP2PSendMsgDlg;
	map<uint32, /*CP2PSendMsgDlg*/CP2PSendMsgDlg *> m_mapTempClusterMsgDlg;
	map<uint32,  CClusterSendMsgDlg *> m_mapClusterSendMsgDlg;
	CDuiP2PSendMsgDlg* m_pDlgSelectP2PSendMsgDlg;//当前显示的聊天窗口

private:
	void SaveAppTrayIcon();
	void RestoreAppTrayIcon();

private:
	void OnLogOut(const char *data, WORD wRet);
	void OnRecvSvrMsg(const char *data, WORD wRet);
	void OnRecvSMS(const char *data,WORD wRet);

	DECLARE_XT_MESSAGE_MAP()

private:
	map<uint32,CString>   m_mapRecvClusterMsg     ;//map<cluster_id,cluster_name>
	map<uint32,CString>   m_mapWebInd			  ;//系统消息
	map<uint32,CString>   m_mapRecvP2pMsg         ;//map<user_id, nickname>
	vector<tagSMSItem>    m_vecRecvSMS            ;//手机短信

	uint32                m_uiLastP2pMsg          ;
	uint32				  m_uiLastInd			  ;
	uint32                m_uiLastClusterMsg      ;

	NOTIFYICONDATA        m_nifSaveData    ;
	NOTIFYICONDATA		* m_pNifData       ;
	
	HICON                 m_hTrayIcon      ;
	bool				  m_bSaveAppTrayIcon   ;	
	bool				  m_bHeadPhotoPay      ;
};

extern CMessageHandleWnd *g_pMessageHandleWnd;


