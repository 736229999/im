reverse_iterator
// im\MessageHandleWnd.cpp : 实现文件
//
#include "stdafx.h"
#include "..\IMClient.h"
#include "MessageHandleWnd.h"
#include ".\messagehandlewnd.h"
#include "sysmsgdlg.h"
#include "saconfig.h"
#include "localdata.h"
#include "139MsgDlg.h"
#include "139NoteDlg.h"
#include "..\MainFrame.h"
#include "smsdlg.h"
#include "DuiCounsel.h"
#include "ScreenCtrl.h"
#ifdef _CS_PLAT_EX_
#include "..\CSPlatDlg.h"
#endif
//#include "../CExitMsgWarning.h"
static const int TIMER_MSG  = 0x02;
static const int TIMER_SMS  = 0x03;
extern map<FRIEND_INFO *,UINT>  g_mapMsgUnRead;////保存未读消息的好友ID和未读条数

// CMessageHandleWnd
CMessageHandleWnd * CMessageHandleWnd::pInstance = NULL;

CMessageHandleWnd * g_pMessageHandleWnd = NULL;
CMessageHandleWnd::CMessageHandleWnd()
{
	m_pNifData			 = NULL;
	m_uiLastClusterMsg   = (uint32)-1;
	m_uiLastInd			 = (uint32)-1;
	m_uiLastP2pMsg       = (uint32)-1;
	m_bSaveAppTrayIcon   = false;

	m_hTrayIcon			 = NULL;
	m_bHeadPhotoPay      = false;
	m_pScreenCtrl = NULL;
	m_pDlgSelectP2PSendMsgDlg = NULL;
}

CMessageHandleWnd::~CMessageHandleWnd()
{
	if ( m_hTrayIcon )
		DestroyIcon(m_hTrayIcon);
	if(m_pNifData != NULL)
		Shell_NotifyIcon(NIM_DELETE,m_pNifData);
	m_hTrayIcon = NULL;

	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator itP2p;

	for ( itP2p=m_mapP2PSendMsgDlg.begin(); itP2p!=m_mapP2PSendMsgDlg.end(); itP2p++)
	{
		try
		{
			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = itP2p->second;

			if ( pDlg->m_hWnd )
				pDlg->DestroyWindow();

			delete pDlg;
		}
		catch(...)
		{
		}
	}

	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator itGroup;

	for ( itGroup=m_mapTempClusterMsgDlg.begin(); itGroup!=m_mapTempClusterMsgDlg.end(); itGroup++)
	{
		try
		{
			/*CP2PSendMsgDlg*/CP2PSendMsgDlg *pDlg = itGroup->second;
			pDlg->DestroyWindow();
			delete pDlg;
		}
		catch(...)
		{
		}
	}

	map<uint32,CClusterSendMsgDlg *>::iterator itCluster;

	for ( itCluster=m_mapClusterSendMsgDlg.begin(); itCluster!=m_mapClusterSendMsgDlg.end(); itCluster++)
	{
		try
		{
			CClusterSendMsgDlg *pDlg = itCluster->second;
			if( pDlg->GetSafeHwnd())
			{
				pDlg->DestroyWindow();	
			}
			delete pDlg;
		}
		catch(...)
		{
		}
	}
}


BEGIN_MESSAGE_MAP(CMessageHandleWnd, CWnd)
	ON_MESSAGE(WM_P2PSENDMSGDLG_NOTIFY,OnP2PSendMsgDlgNotify)
	ON_MESSAGE(WM_CLUSTERSENDMSGDLG_NOTIFY,OnClusterSendMsgDlgNotify)
	ON_MESSAGE(WM_P2P_TO_GROUP, OnP2pToGroupDlg)
	ON_MESSAGE(WM_SHOWMESSAGE, OnShowMessage)
	ON_MESSAGE(WM_TRAYICON, OnTrayIconClick )
	ON_MESSAGE(WM_MODULE_NOTIFY,OnModuleNotify)
	ON_WM_TIMER()
	ON_XT_MESSAGE()
	ON_WM_CREATE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CMessageHandleWnd)
	ON_XT_COMMAND(CMD_LOGOUT,OnLogOut)
	ON_XT_COMMAND(CMD_SERVERMSG_IND,OnRecvSvrMsg)
	ON_XT_COMMAND(CMD_SMS_IND,OnRecvSMS)
END_XT_MESSAGE_MAP()

CMessageHandleWnd * CMessageHandleWnd::GetInstance()
{
	if ( pInstance==NULL )
	{
		pInstance = new CMessageHandleWnd();

		LPCTSTR szClassName = AfxRegisterWndClass(0);
		pInstance->CreateEx(0,szClassName, "MessageHandleWnd",0 ,0 ,0 ,0 ,0 ,HWND_MESSAGE,0); 
		g_hMessageWnd = pInstance->m_hWnd;
	}

	return pInstance;
}

CClusterSendMsgDlg* CMessageHandleWnd::FindClusterSendMsgDlg(uint32 cid)
{
	map<uint32,CClusterSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapClusterSendMsgDlg.find(cid);

	if ( mapIt==m_mapClusterSendMsgDlg.end() )
		return NULL;
	else
		return mapIt->second;
}

bool CMessageHandleWnd::FindGroupTalkDlg(uint32 cid)
{
	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapTempClusterMsgDlg.find(cid);

	if ( mapIt==m_mapTempClusterMsgDlg.end() )
		return false;
	else
		return true;
}

bool CMessageHandleWnd::FindP2PSendMsgDlg(uint32 fid)
{
	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapP2PSendMsgDlg.find(fid);

	if ( mapIt==m_mapP2PSendMsgDlg.end() )
		return false;
	else
		return true;
}

/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg* CMessageHandleWnd::FindP2PSendMsgDlgById(uint32 fid)
{
	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapP2PSendMsgDlg.find(fid);

	if ( mapIt==m_mapP2PSendMsgDlg.end() )
		return NULL;
	else
		return mapIt->second;
}

/*CP2PSendMsgDlg*/CP2PSendMsgDlg * CMessageHandleWnd::GetTempGroupSendMsgDlg(int nGroupIndex)
{
	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator it;
	for(it=m_mapTempClusterMsgDlg.begin();it!=m_mapTempClusterMsgDlg.end();it++)
	{
		if(it->second->GetTempGroupTalkIndex()==nGroupIndex)
			return it->second;	
	}
	return NULL;
}

/*CP2PSendMsgDlg*/CP2PSendMsgDlg * CMessageHandleWnd::GetTempClusterSendMsgDlg2(uint32 gid)
{
	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator it;
	it = m_mapTempClusterMsgDlg.find(gid);

	/*CP2PSendMsgDlg*/CP2PSendMsgDlg *pDlg = NULL;

	if ( it==m_mapTempClusterMsgDlg.end() )
	{
		pDlg = new CP2PSendMsgDlg(NULL,pmMultiTalk,GetDesktopWindow());

		pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
		//pDlg = /*CP2PSendMsgDlg*/CP2PSendMsgDlg::NewDlg(NULL,pmMultiTalk,GetDesktopWindow());

		////pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
		// 先隐藏
		pDlg->ShowWindow(SW_HIDE);
		pDlg->UpdateCluserInfo(gid);
	}
	else
	{
		pDlg = it->second;
	}

	return pDlg;
}

/*CP2PSendMsgDlg*/CP2PSendMsgDlg * CMessageHandleWnd::GetTempClusterSendMsgDlg(uint32 gid)
{
	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator it;
	it = m_mapTempClusterMsgDlg.find(gid);

	/*CP2PSendMsgDlg*/CP2PSendMsgDlg *pDlg = NULL;

	if ( it==m_mapTempClusterMsgDlg.end() )
	{
		pDlg = new CP2PSendMsgDlg(NULL,pmMultiTalk,GetDesktopWindow());

		pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
		//pDlg = /*CP2PSendMsgDlg*/CP2PSendMsgDlg::NewDlg(NULL,pmMultiTalk,GetDesktopWindow());

		////pDlg->Create(CP2PSendMsgDlg::IDD,GetDesktopWindow());
		// 先隐藏
		pDlg->ShowWindow(SW_HIDE);
		pDlg->UpdateCluserInfo(gid);

 		m_mapTempClusterMsgDlg.insert(
 			map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::value_type(gid,pDlg)
 			);
	}
	else
	{
		pDlg = it->second;
	}

	return pDlg;
}

/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * CMessageHandleWnd::GetP2PSendMsgDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo,int nUnReadNum)
{
#ifdef _CS_PLAT_EX_
	////是否窗口已打开
	uint32 fid = pFriend->GetId();

	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
	it = m_mapP2PSendMsgDlg.find(fid);

	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = NULL;
	
	if ( it==m_mapP2PSendMsgDlg.end() )
	{
		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow(), struExtInfo,nUnReadNum);
		m_mapP2PSendMsgDlg.insert(
			map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::value_type(fid,pDlg)
			);
#ifdef _BUSINESS_VESION //卖家版才有正在咨询窗口
		if(CMainFrame::IsBuyer(pFriend->GetId()))//只显示卖家咨询框
		{
			if(CDuiCSPlatDlg::GetInstance() == NULL || !IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()))
			{
				CDuiCSPlatDlg::Show();
				CDuiCSPlatDlg::GetInstance()->AddFriendListElement(pFriend);
				CDuiCSPlatDlg::GetInstance()->AddP2PSendMsgDlg(pDlg);
				CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);///false

				pDlg->CenterWindow();
			}
			else/////已打开了咨询窗口
			{
				CDuiCSPlatDlg::GetInstance()->AddFriendListElement(pFriend);
				CDuiCSPlatDlg::GetInstance()->AddP2PSendMsgDlg(pDlg);
				CDuiCSPlatDlg::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);
			}
		}
		else
		{
			//第一次打开左侧页如果是商家则tab页显示我的团队
			CDuiCSPlatDlg::GetInstance()->SetLayoutPageTeam();
		}
		CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);
#endif
	}
	else
	{
		pDlg = it->second;
		pDlg->RefreshFriendStatus();
		if(CDuiCSPlatDlg::GetInstance() != NULL)
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
				CDuiCSPlatDlg::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
	}
	return pDlg;

#elif defined _WORK_EX_
	uint32 fid = pFriend->GetId();

	map<uint32,CDuiP2PSendMsgDlg *>::iterator it;
	it = m_mapP2PSendMsgDlg.find(fid);

	CDuiP2PSendMsgDlg *pDlg = NULL;

	if ( it==m_mapP2PSendMsgDlg.end() )
	{
		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow(), struExtInfo,nUnReadNum);
		m_mapP2PSendMsgDlg.insert(
			map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::value_type(fid,pDlg)
			);
		if(CDuiCounsel::GetInstance() == NULL || !IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
		{
			CDuiCounsel::Show();
			CDuiCounsel::GetInstance()->AddFriendListElement(pFriend);
			CDuiCounsel::GetInstance()->AddP2PSendMsgDlg(pDlg);
			CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);///false

			pDlg->CenterWindow();
		}
		else/////已打开了咨询窗口
		{
			CDuiCounsel::GetInstance()->AddFriendListElement(pFriend);
			CDuiCounsel::GetInstance()->AddP2PSendMsgDlg(pDlg);
			CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);
		}
	}
	else
	{
		pDlg = it->second;
		if(CDuiCounsel::GetInstance() != NULL)
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
				CDuiCounsel::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
	}
	return pDlg;

#else
	////是否窗口已打开
	uint32 fid = pFriend->GetId();

	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
	it = m_mapP2PSendMsgDlg.find(fid);

	/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *pDlg = NULL;

	if ( it==m_mapP2PSendMsgDlg.end() )
	{
		//pDlg = new /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg(pFriend,pmSingleTalk, GetDesktopWindow());
		pDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow(), struExtInfo,nUnReadNum);
		m_mapP2PSendMsgDlg.insert(
			map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::value_type(fid,pDlg)
			);
#ifdef _BUSINESS_VESION //卖家版才有正在咨询窗口
 		if((CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))//只显示卖家咨询框
 		{
 			if(CDuiCounsel::GetInstance() == NULL || !IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
 			{
				CDuiCounsel::Show();
				CDuiCounsel::GetInstance()->AddFriendListElement(pFriend);
				CDuiCounsel::GetInstance()->AddP2PSendMsgDlg(pDlg);
				CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);///false

				pDlg->CenterWindow();
 			}
			else/////已打开了咨询窗口
			{
				CDuiCounsel::GetInstance()->AddFriendListElement(pFriend);
				CDuiCounsel::GetInstance()->AddP2PSendMsgDlg(pDlg);
				CDuiCounsel::GetInstance()->SetFriendListElement(pFriend, false, false, false, false, false,nUnReadNum);
			}
 		}

#endif
	}
	else
	{
		pDlg = it->second;
		if(CDuiCounsel::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
		{
			if(g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg != pDlg)
				CDuiCounsel::GetInstance()->MoveP2PSendMsgDlg(pDlg);
		}
	}

// 	map<uint32,CDuiP2PSendMsgDlg *>::iterator itDui;
// 	itDui = m_mapDuiP2PSendMsgDlg.find(fid);
	//CDuiP2PSendMsgDlg* pDuiDlg = CDuiP2PSendMsgDlg::NewDlg(pFriend,pmSingleTalk, GetDesktopWindow());
	//pDuiDlg->ShowWindow(true);
	return pDlg;
#endif
}

CClusterSendMsgDlg * CMessageHandleWnd::GetClusterSendMsgDlg(CLUSTER_INFO *pClusterInfo)
{
	uint32 cid = pClusterInfo->nFamilyID;

	map<uint32,CClusterSendMsgDlg *>::iterator it;
	it = m_mapClusterSendMsgDlg.find(cid);

	CClusterSendMsgDlg *pDlg = NULL;

	if ( it==m_mapClusterSendMsgDlg.end() )
	{
		pDlg = new CClusterSendMsgDlg(pClusterInfo,GetDesktopWindow());
		// 获取群成员列表
		if( !pClusterInfo->bBasicInfo )
		{
			XT_GET_CMEMBERS_REQ req;
			req.nFamilyID   = pClusterInfo->nFamilyID;
			req.nFamilyProp = PERM_FAMILY;
			req.req_count = 0;
			req.online_type  = 0;
			req.userstatus   = g_ImUserInfo.status;
			g_pProtocol->SendReq(req);
		}
		m_mapClusterSendMsgDlg.insert(
			map<uint32,CClusterSendMsgDlg *>::value_type(cid,pDlg)
			);
	}
	else
	{
		pDlg = it->second;
	}

	return pDlg;
}

LRESULT CMessageHandleWnd::OnP2pToGroupDlg(WPARAM w,LPARAM l)
{
//	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
//
//	if ( w==0 )
//	{
//		FRIEND_INFO *pFriend = (FRIEND_INFO *)l;
//
//		it = m_mapP2PSendMsgDlg.find(pFriend->GetId());
//
//		if ( it!=m_mapP2PSendMsgDlg.end() )
//		{
//			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * pDlg = it->second;
//
//			//if(CDuiCounsel::GetInstance() != NULL && pFriend->GetId() >= 5000000)
//			//{
//			//	CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg(pDlg);
//			//}
//
//			CLUSTER_INFO *pCluster = pDlg->GetClusterInfo();
//
//			m_mapTempClusterMsgDlg.insert(
//				map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::value_type(pCluster->nFamilyID,pDlg));
//		
//			m_mapP2PSendMsgDlg.erase(it);
//
//#ifdef _BUSINESS_VESION		// 销毁聊天窗口后，更新是否挂起状态
//			if(pFriend->group_index == 1)
//			{
//				CMainFrame::GetInstance()->Hangup();
//			}			
//#endif
//		}
//	}
//	else
//	{
//		uint32 gid = l;
//
//		it = m_mapTempClusterMsgDlg.find(gid);
//
//		if ( it!=m_mapTempClusterMsgDlg.end() )
//		{
//			/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * pDlg = it->second;
//			FRIEND_INFO *pFriend = pDlg->GetFriendInfo();
//
//			m_mapP2PSendMsgDlg.insert(
//				map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::value_type(pFriend->GetId(),pDlg));
//		
//			m_mapTempClusterMsgDlg.erase(it);
//		}
//
//
//	}

	return 1;
}

void CMessageHandleWnd::P2PSendMsgDlgNotify(TP2PMode w,FRIEND_INFO* l)
{
	

	if ( w==pmMultiTalk )
	{
		map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator it;
		uint32 cid=(uint32)l;

		it = m_mapTempClusterMsgDlg.find(cid);

		if ( it!=m_mapTempClusterMsgDlg.end() )
		{
			delete (/*CP2PSendMsgDlg*/CP2PSendMsgDlg *)it->second;
			m_mapTempClusterMsgDlg.erase(it);
		}
	}
	else
	{
		map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
		FRIEND_INFO *pFriend = l;

		it = m_mapP2PSendMsgDlg.find(pFriend->GetId());

		if ( it!=m_mapP2PSendMsgDlg.end() )
		{
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(pFriend->GetId()))
			{
				CDuiCSPlatDlg::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
			}
#elif defined _WORK_EX_
			if(CDuiCounsel::GetInstance() != NULL)
			{
				CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
			}
#else
			if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())))
			{
				CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
			}
#endif

			//int nGroupIndex = pFriend->group_index;
			delete (/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second;
			m_mapP2PSendMsgDlg.erase(it);

#ifdef _BUSINESS_VESION		// 销毁聊天窗口后，更新是否挂起状态
			CMainFrame::GetInstance()->Hangup();
#endif
		}		
	}
}

LRESULT CMessageHandleWnd::OnP2PSendMsgDlgNotify(WPARAM w,LPARAM l)
{
	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;

	if ( (TP2PMode)w==pmMultiTalk )
	{
		map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator it;
		uint32 cid=(uint32)l;

		it = m_mapTempClusterMsgDlg.find(cid);

		if ( it!=m_mapTempClusterMsgDlg.end() )
		{
			delete (/*CP2PSendMsgDlg*/CP2PSendMsgDlg *)it->second;
			m_mapTempClusterMsgDlg.erase(it);
		}
	}
	else
	{
		map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
		FRIEND_INFO *pFriend = (FRIEND_INFO *)l;

		it = m_mapP2PSendMsgDlg.find(pFriend->GetId());

		if ( it!=m_mapP2PSendMsgDlg.end() )
		{
#ifdef _CS_PLAT_EX_
			if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()) && CMainFrame::IsBuyer(pFriend->GetId()))
			{
				CDuiCSPlatDlg::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
				if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)
				{
					if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL)
						CDuiCSPlatDlg::GetInstance()->Close();
				}
			}
#elif defined _WORK_EX_
			if(CDuiCounsel::GetInstance() != NULL && ::IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
			{
				CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
				if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 0)
				{
					CDuiCounsel::GetInstance()->Close();
				}
			}
#else
			if(CDuiCounsel::GetInstance() != NULL && ::IsWindow(CDuiCounsel::GetInstance()->GetHWND()) && (CMainFrame::IsBuyer(pFriend->GetId()) || CMainFrame::IsWork(pFriend->GetId())) )
			{
				CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)it->second);
				if(CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg.size() == 0)
				{
					CDuiCounsel::GetInstance()->Close();
				}
			}
#endif

			vector<CDuiP2PSendMsgDlg*>::iterator vecit = std::find(m_vecAllP2PSendMsgDlg.begin(), m_vecAllP2PSendMsgDlg.end(), (CDuiP2PSendMsgDlg *)it->second);
			if(vecit != m_vecAllP2PSendMsgDlg.end())
			{
				Sleep(100);
				m_vecAllP2PSendMsgDlg.erase(vecit);
				CDuiP2PSendMsgDlg *pDlg = (CDuiP2PSendMsgDlg *)it->second;
				delete pDlg;
				//pDlg = NULL;
				
			}

			//int nGroupIndex = pFriend->group_index;
			m_mapP2PSendMsgDlg.erase(it);

			if(g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size() == 0)
			{
#ifdef _CS_PLAT_EX_
				if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL)
				{
					CDuiCSPlatDlg::GetInstance()->Close();
					//设置当前聊天的买家
					XT_MONITOR_SET_BUYER_REQ req;
					req.idSeller = g_ImUserInfo.GetId();
					req.idBuyer = 0;
					g_pProtocol->SendReq(req);
				}
#else
				if(CDuiCounsel::GetInstance() != NULL && CDuiCounsel::GetInstance()->GetHWND() != NULL)
					CDuiCounsel::GetInstance()->Close();
#endif
			}

#ifdef _BUSINESS_VESION		// 销毁聊天窗口后，更新是否挂起状态
			CMainFrame::GetInstance()->Hangup();
#endif
		}		
	}

	return 1;
}

LRESULT CMessageHandleWnd::OnClusterSendMsgDlgNotify(WPARAM wParam,LPARAM lParam)
{
	uint32 cid = lParam;

	if ( wParam==0 )//close
	{
		map<uint32,CClusterSendMsgDlg *>::iterator it;
		it = m_mapClusterSendMsgDlg.find(cid);

		if ( it!=m_mapClusterSendMsgDlg.end() )
		{
			delete (CClusterSendMsgDlg *)it->second;
			m_mapClusterSendMsgDlg.erase(it);
		}
	}
	else//show
	if ( wParam==1 )
	{
		map<uint32,CString>::iterator it;

		bool bFlag = (cid==m_uiLastClusterMsg);

		it = m_mapRecvClusterMsg.find(cid);

		if ( it!=m_mapRecvClusterMsg.end() )
			m_mapRecvClusterMsg.erase(it);


		it = m_mapRecvClusterMsg.begin();

		if ( it==m_mapRecvClusterMsg.end() )
		{
			m_uiLastClusterMsg = (uint32)-1;
		}
		else
		{
			m_uiLastClusterMsg = it->first;
		}

		if ( m_uiLastClusterMsg!=(uint32)-1 )
		{
			if ( bFlag )
			{
				m_pNifData->uFlags = NIF_TIP;
				sprintf(m_pNifData->szTip,"收到群 \"%s\" 的消息",it->second);
				Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
			}
		}
		else
		{
			RestoreAppTrayIcon();
		}		
	}

	return 1;
}

void CMessageHandleWnd::SetHeartTimeOut()
{
	map<uint32,CClusterSendMsgDlg *>::iterator itCluster;

	for ( itCluster=this->m_mapClusterSendMsgDlg.begin();
		itCluster!=this->m_mapClusterSendMsgDlg.end();
		itCluster++ )
		itCluster->second->PostMessage(WM_HEART_TIMEOUT);

	map<uint32, /*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator itP2P2;
	
	for ( itP2P2=this->m_mapTempClusterMsgDlg.begin(); 
		  itP2P2!=this->m_mapTempClusterMsgDlg.end();
		  itP2P2++ )
		  itP2P2->second->PostMessage(WM_HEART_TIMEOUT);

	map<uint32, /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator itP2P;
	for ( itP2P=this->m_mapP2PSendMsgDlg.begin(); 
		  itP2P!=this->m_mapP2PSendMsgDlg.end();
		  itP2P++ )
		  itP2P->second->PostMessage(WM_HEART_TIMEOUT);

}

void CMessageHandleWnd::CloseClusterSendMsgDlg(uint32 cid)
{
	map<uint32,CClusterSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapClusterSendMsgDlg.find(cid);

	if ( mapIt!=m_mapClusterSendMsgDlg.end() )
	{
		CClusterSendMsgDlg * pDlg = mapIt->second;
		m_mapClusterSendMsgDlg.erase(mapIt);
		if( pDlg->GetSafeHwnd())
		{
			pDlg->DestroyWindow();
		}
		
	}
}

void CMessageHandleWnd::CloseP2PSendMsgDlg(uint32 fid)
{
	map<uint32,/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator mapIt;
	mapIt = m_mapP2PSendMsgDlg.find(fid);

	if ( mapIt!=m_mapP2PSendMsgDlg.end() )
	{
#ifdef _CS_PLAT_EX_
		if(CDuiCSPlatDlg::GetInstance() != NULL && CMainFrame::IsBuyer(fid))
		{
			CDuiCSPlatDlg::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)mapIt->second);
		}
#elif defined _WORK_EX_
		if(CDuiCounsel::GetInstance() != NULL)
		{
			CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)mapIt->second);
		}
#else
		if(CDuiCounsel::GetInstance() != NULL && (CMainFrame::IsBuyer(fid) || CMainFrame::IsWork(fid)) )
		{
			CDuiCounsel::GetInstance()->DeleteP2PSendMsgDlg((/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)mapIt->second);
		}
#endif
		delete (/*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *)mapIt->second;
		m_mapP2PSendMsgDlg.erase(mapIt);
#ifdef _BUSINESS_VESION		// 销毁聊天窗口后，更新是否挂起状态
		FRIEND_INFO *pFriend = ::FindFriend(fid);
		if(pFriend != NULL)
		{
			CMainFrame::GetInstance()->Hangup();
		}			
#endif
	}

	map<uint32,/*CP2PSendMsgDlg*/CP2PSendMsgDlg *>::iterator mapIt2;
	mapIt2 = m_mapTempClusterMsgDlg.find(fid);

	if ( mapIt2!=m_mapTempClusterMsgDlg.end() )
	{
		delete (/*CP2PSendMsgDlg*/CP2PSendMsgDlg *)mapIt2->second;
		m_mapTempClusterMsgDlg.erase(mapIt2);
	}
}

LRESULT CMessageHandleWnd::OnShowMessage(WPARAM w,LPARAM l)
{
	char *msg  = (char *)w;
	UINT nType = l;

	CMessageBox::Model(GetDesktopWindow()->m_hWnd,
		msg,
		IM_CAPTION,
		nType|MB_SYSTEMMODAL);

	delete [] msg;

	return 0;
}

void CMessageHandleWnd::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent== TIMER_SMS )
	{
		tagSMSItem si = m_vecRecvSMS[0];
		m_vecRecvSMS.erase(m_vecRecvSMS.begin());

		FRIEND_INFO *pFriend = FindFriend(si.dwFriendId);
		if ( pFriend )
		{
			CSMSDlg *pDlg = CSMSDlg::Show(pFriend);
            pDlg->ShowRecvSMS(si);
		}

		KillTimer(TIMER_SMS);

		if ( m_vecRecvSMS.size()==0 )
			RestoreAppTrayIcon();
	}

	if ( nIDEvent== TIMER_MSG )
	{
		static bool bFlag = true;
		if(m_pNifData == NULL)
			return;
		m_pNifData->uFlags = NIF_ICON;

		//初始化系统托盘的图标
		CImageList *pimgAppIcon ;
/*
		if ( ::g_bOs2K )
			pimgAppIcon = &g_imgAppIcon2k;
		else
*/
		pimgAppIcon = &g_imgAppIconXp;

		if ( m_hTrayIcon )
			DestroyIcon(m_hTrayIcon);

		m_hTrayIcon = NULL;

		if ( m_uiLastInd!=(uint32)-1 )
		{
			if ( bFlag )
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_NORM));
			 
			else
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_DIS));
		}
		else
		if ( m_uiLastClusterMsg!=(uint32)-1 )
		{
			if ( bFlag )
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_GROUP));
			else
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_DIS));
		}
		else
		if ( m_vecRecvSMS.size()>0 )
		{
			if ( bFlag )
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_NORM));
			else
				m_hTrayIcon = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDI_DIS));
		}

		m_pNifData->hIcon = m_hTrayIcon;

		Shell_NotifyIcon(NIM_MODIFY,m_pNifData);

		bFlag = !bFlag;
	}

	CWnd::OnTimer(nIDEvent);
}

int CMessageHandleWnd::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CWnd::OnCreate(lpCreateStruct) == -1)
		return -1;

	g_pProtocol->RegistCmd(CMD_LOGOUT,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SMS_IND,m_hWnd);
	g_pProtocol->RegistCmd(CMD_SERVERMSG_IND,m_hWnd);

	return 0;
}

LRESULT CMessageHandleWnd::OnModuleNotify(WPARAM wParam, LPARAM lParam)
{
	switch ( wParam )
	{
	case MODULE_MiniPictureBroadCast:
		{
			TWebPictureInfo *pInfo = (TWebPictureInfo *)lParam;
			FRIEND_INFO *pFriend = FindFriend(pInfo->uID);
			
			if ( pFriend )
			{
				CString strUrl;
				strUrl.Format("http://%s.feiniu.com/photo/%u_%u.html",
					pFriend->GetUserName().c_str(),
					pInfo->nAlbumId,
					pInfo->dwPictureID);
				MiniOpenUrl(strUrl);
			}
		}
		break;
	case MODULE_MyArticles:
		CSAConfig::Show(3);
		break;
	case MODULE_MyPictures:
		{
			switch(lParam)
			{
				case 0:
				CSAConfig::Show(3);
				break;
				case 1:
				ShowUploadPicDlg((char*)g_ImUserInfo.GetUserName().c_str(),g_ImUserInfo.nickname,(char*)g_ImUserInfo.GetPassword().c_str(),g_ImUserInfo.GetId());
				break;
			}
		}
		break;
	case MODULE_HeadPhoto:
		{
			if ( m_bHeadPhotoPay )
			{
				struct tagPrice
				{
					float fPrice;
					long  nProductId;
					long  nType;
				} tP;

				tP = *(tagPrice *)lParam;
				static char szError[255];

				m_bHeadPhotoPay = false;

				if ( g_pProtocol->UserPayment(tP.fPrice*100,tP.nProductId,tP.nType)==0 )
				{
					return 0;//success
				}
				else
				{
					strcpy(szError,g_pProtocol->GetLastError());
					return (LPARAM)(LPCSTR)szError;//failed
				}
			}
			else
			{	
				switch(lParam)
				{
				case 0://
					{
						strncpy(g_ImUserInfo.face_url,(LPCSTR)((IUserInfoX *)g_pHeadPhoto->GetUserInfo())->GetFaceUrl(),sizeof(g_ImUserInfo.face_url)-1);
						theApp.m_pMainWnd->PostMessage(WM_139WEB_NOTIFY,1,0);
					}
					break;
				case 1:
					{
						/*CMainFrame *pFrame = (CMainFrame *)theApp.m_pMainWnd;
						g_ImUserInfo.gold_money = ((IUserInfoX *)g_pHeadPhoto->GetUserInfo())->GetGoldMoney();
						pFrame->ShowGoldMoney(g_ImUserInfo.gold_money);*/
					}
					break;
				case 2:
					{
						m_bHeadPhotoPay = true;
					}
					break;
				}
			}
	
		}
		break;
	}
	return 0;
}

LRESULT CMessageHandleWnd::OnTrayIconClick(WPARAM wParam, LPARAM lParam)
{
	LRESULT lRet = 0;

	switch(lParam)
	{
	case WM_LBUTTONUP:
		if ( m_uiLastClusterMsg!=(uint32)-1 || m_uiLastInd!=(uint32)-1 || m_uiLastP2pMsg!=(uint32)-1 || m_vecRecvSMS.size()>0 )
		{
			return 1;
		}
		break;
	case WM_LBUTTONDBLCLK:
		{
			map<uint32,CString>::iterator it ;

			if ( m_uiLastInd!=-1 )
			{
				it = m_mapWebInd.find(m_uiLastInd);
				CSysMsgDlg::ShowSysMsg(it->first,it->second);

				m_mapWebInd.erase(it);

				it = m_mapWebInd.begin();

				if ( it==m_mapWebInd.end() )
				{
					m_uiLastInd = (uint32)-1;
				}
				else
				{
					m_uiLastInd = it->first;
				}

				if ( m_uiLastInd!=(uint32)-1 )
				{
					sprintf(m_pNifData->szTip,"系统消息");
					m_pNifData->uFlags = NIF_TIP;
					Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
				}
				else
				{
					if ( m_uiLastClusterMsg!=-1 )
					{
						CLUSTER_INFO *pCluster=FindClusterInfo(this->m_uiLastClusterMsg);

						if ( pCluster )
						{
							m_pNifData->uFlags = NIF_TIP;
							sprintf(m_pNifData->szTip,"收到群 \"%s\" 的消息",pCluster->szFamilyName);
							Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
						}
					}
				}

				lRet=1;
			}
			else
			if ( m_uiLastClusterMsg!=-1 )
			{
				CLUSTER_INFO *pCluster=FindClusterInfo(this->m_uiLastClusterMsg);
	
				if ( pCluster )
				{
					CClusterSendMsgDlg *pDlg = g_pMessageHandleWnd->GetClusterSendMsgDlg(pCluster);
	                
					if ( pDlg->m_hWnd==NULL )
					{
						pDlg->Create(CClusterSendMsgDlg::IDD,GetDesktopWindow());
					}

					if ( pDlg->IsWindowVisible()==FALSE )
					{
						pDlg->ShowWindow(SW_MINIMIZE);
						pDlg->ShowWindow(SW_SHOW);
					}

					if ( pDlg->IsIconic() )
					{
						pDlg->ShowWindow(SW_RESTORE);
					}

					pDlg->BringWindowToTop();
					pDlg->SetForegroundWindow();
					
					it = m_mapRecvClusterMsg.find(pCluster->nFamilyID);

					if ( it!=m_mapRecvClusterMsg.end() )
						m_mapRecvClusterMsg.erase(it);
				}
				else
				{
					it = m_mapRecvClusterMsg.find(m_uiLastClusterMsg);

					if ( it!=m_mapRecvClusterMsg.end() )
						m_mapRecvClusterMsg.erase(it);
				}

				it = m_mapRecvClusterMsg.begin();

				if ( it==m_mapRecvClusterMsg.end() )
				{
					m_uiLastClusterMsg = (uint32)-1;
				}
				else
				{
					this->m_uiLastClusterMsg = it->first;
				}

				if ( m_uiLastClusterMsg!=(uint32)-1 )
				{
					m_pNifData->uFlags = NIF_TIP;
					sprintf(m_pNifData->szTip,"收到群 \"%s\" 的消息",it->second);
					Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
				}
				else
				{
					if ( m_vecRecvSMS.size()>0 )
					{	
						m_pNifData->uFlags = NIF_TIP;
						sprintf(m_pNifData->szTip,"哞哞 - 你有手机短信");
						Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
					}
				}

				lRet = 1;
			}		
			else
			if ( m_vecRecvSMS.size()!=0 )
			{
				SetTimer(TIMER_SMS,300,NULL);
			}			
		}
	default:
		break;
	}

	this->RestoreAppTrayIcon();

	return lRet;
}

void CMessageHandleWnd::SaveAppTrayIcon()
{
	if ( m_bSaveAppTrayIcon==false && m_uiLastClusterMsg==(uint32)-1 && m_uiLastInd==(uint32)-1 && m_uiLastP2pMsg==(uint32)-1 && m_vecRecvSMS.size()==0 )
	{
		m_bSaveAppTrayIcon = true;
		memcpy(&this->m_nifSaveData,m_pNifData,sizeof(m_nifSaveData));
	}
}

void CMessageHandleWnd::RestoreAppTrayIcon()
{
	if ( m_bSaveAppTrayIcon && m_uiLastInd==(uint32)-1 && m_uiLastClusterMsg==(uint32)-1 && m_uiLastP2pMsg==(uint32)-1 && m_vecRecvSMS.size()==0 )
	{
		KillTimer(TIMER_MSG);

		m_pNifData->hIcon  = this->m_nifSaveData.hIcon;
		sprintf(m_pNifData->szTip,this->m_nifSaveData.szTip);
		m_pNifData->uFlags = NIF_ICON|NIF_TIP;

		Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
		
		if (theApp.m_pMainWnd )
			theApp.m_pMainWnd->PostMessage(WM_TRAYICON_UPDATE,0,0);

		m_bSaveAppTrayIcon = false;
	}
}

void CMessageHandleWnd::OnLogOut(const char *data,WORD wRet)
{
	if ( m_uiLastClusterMsg!=(uint32)-1 || m_uiLastInd!=(uint32)-1 || 
		 m_uiLastP2pMsg!=(uint32)-1 )
	{
		m_mapRecvClusterMsg.clear();
		m_mapRecvP2pMsg.clear();
		m_mapWebInd.clear();

		m_uiLastInd			= (uint32)-1;
		m_uiLastClusterMsg  = (uint32)-1;
		m_uiLastP2pMsg      = (uint32)-1;

		this->RestoreAppTrayIcon();
	}
}

void CMessageHandleWnd::OnRecvClusterMsg(CLUSTER_INFO *pCluster)
{
	SaveAppTrayIcon();

	KillTimer(TIMER_MSG);
	SetTimer(TIMER_MSG,350,NULL);

	m_pNifData->uFlags = NIF_TIP;
	sprintf(m_pNifData->szTip,"收到群 \"%s\" 的消息",pCluster->szFamilyName);
	Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
	
	m_uiLastClusterMsg = pCluster->nFamilyID;
	map<uint32,CString>::iterator it=this->m_mapRecvClusterMsg.find(pCluster->nFamilyID);

	if ( it==m_mapRecvClusterMsg.end() )
	{
		m_mapRecvClusterMsg.insert(map<uint32,CString>::value_type(pCluster->nFamilyID,pCluster->szFamilyName));
	}
}

void CMessageHandleWnd::OnRecvSMS(const char *data, WORD wRet)
{
	LPXT_SMS_IND sms = (LPXT_SMS_IND)data;

	if ( wRet==0 )
	{
		SaveAppTrayIcon();

		KillTimer(TIMER_MSG);
		SetTimer(TIMER_MSG,350,NULL);

		if ( m_vecRecvSMS.size()==0 )
		{
			if ( m_uiLastClusterMsg==(uint32)-1 && m_uiLastInd==(uint32)-1 )
			{
				m_pNifData->uFlags = NIF_TIP;
				strcpy(m_pNifData->szTip,"哞哞 - 收到手机短信");
				Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
			}
		}

		tagSMSItem si;

		strcpy(si.szTime , sms->send_time);
		strcpy(si.message, sms->message);
		si.nType      = 1;//recv
		si.dwFriendId = sms->sender_id;
		strcpy(si.szNickName,sms->nick_name);
		strcpy(si.szUserName,sms->user_name);

		g_LocalData.AddMobileSMS(si.dwFriendId,si);

		m_vecRecvSMS.push_back(si);
	}
}

void CMessageHandleWnd::OnRecvSvrMsg(const char *data, WORD wRet)
{
	LPXT_SERVERMSG sysmsg = (LPXT_SERVERMSG)data;
	
	if ( wRet==0 )
	{	
		if ( sysmsg->type == 2 )//加金币提示
		{
			AddSystemMessage(sysmsg->send_time,sysmsg->message);
		}
		else if(sysmsg->type == 0)
		{
			AddSystemMessage(sysmsg->send_time,sysmsg->message);
		}
		else if(sysmsg->type==3)//web 公告
		{
			C139MsgDlg::Show(sysmsg->message);
		}
	}
}

//void CMessageHandleWnd::Add139Notify()
//{
//	SaveAppTrayIcon();
//
//	KillTimer(TIMER_MSG);
//	SetTimer(TIMER_MSG,350,NULL);
//
//	if ( m_b139Notify==false )
//	{
//		if ( m_uiLastClusterMsg==(uint32)-1 && m_uiLastInd==(uint32)-1 )
//		{
//			m_pNifData->uFlags = NIF_TIP;
//			strcpy(m_pNifData->szTip,"Mini - 你收到的消息");
//			Shell_NotifyIcon(NIM_MODIFY,m_pNifData);
//		}
//
//		m_b139Notify = true;
//	}
//}
//
//void CMessageHandleWnd::Remove139Notify()
//{
//	m_b139Notify=false;
//	RestoreAppTrayIcon();
//}

void CMessageHandleWnd::AddSystemMessage(uint32 time_recv, CString strMessage)
{
	SaveAppTrayIcon();
		
	KillTimer(TIMER_MSG);
	SetTimer(TIMER_MSG,350,NULL);

	if ( m_uiLastInd==(uint32)-1 )
	{
		m_mapWebInd.insert(map<uint32,CString>::value_type(time_recv,strMessage));

		m_pNifData->uFlags = NIF_TIP;
		strcpy(m_pNifData->szTip,"系统消息");
		Shell_NotifyIcon(NIM_MODIFY,m_pNifData);

		m_uiLastInd = time_recv;
	}
	else
	{
		map<uint32,CString>::iterator it = m_mapWebInd.find(time_recv);

		if ( it==m_mapWebInd.end() )
		{
			m_mapWebInd.insert(map<uint32,CString>::value_type(time_recv, strMessage));
		}
		else
		{
			it->second = strMessage;
		}
	}

	SYSTEM_MSG msg;

	msg.dwTime = time_recv;
	msg.iType  = 0;
	sprintf(msg.szMessage,strMessage);

	g_LocalData.AddSystemMsg(g_ImUserInfo.GetId(),msg);

	m_uiLastInd = time_recv;
}

bool CMessageHandleWnd::HasP2PFile()
{
	map<uint32, /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg *>::iterator it;
	for ( it=m_mapP2PSendMsgDlg.begin(); it!=m_mapP2PSendMsgDlg.end(); it++ )
	{
		if ( it->second->m_poolRecv.size()>0 || it->second->m_poolSend.size()>0 )
		{
			return true;
		}
	}

	return false;
}

void CMessageHandleWnd::OnHearTimeOut()
{
	m_uiLastClusterMsg = (uint32)-1;
	m_uiLastInd = (uint32)-1;
	m_uiLastP2pMsg = (uint32)-1;
	m_mapWebInd.clear();
	m_mapRecvClusterMsg.clear();
	m_mapRecvP2pMsg.clear();

	RestoreAppTrayIcon();
}

void CMessageHandleWnd::OnDestroy()
{
	g_pProtocol->UnRegistCmd(m_hWnd);
	CWnd::OnDestroy();
}

// 获取含有未读消息的对话框数量
int CMessageHandleWnd::GetStrangerMsgDlgCount()
{
	int nCount = 0;

	std::tr1::shared_ptr<CDuiCounsel> pIns = CDuiCounsel::GetInstance();
	if(pIns == NULL)
		return 0;

	map<FRIEND_INFO*, int>::iterator it = pIns->m_mapFriendCount.begin();
	for(; it != pIns->m_mapFriendCount.end(); ++it)
	{
		if(it->second > 1)
		{
			nCount++;
		}
	}

	return nCount;
}

void CMessageHandleWnd::UpdateBk()
{
	map<uint32, CDuiP2PSendMsgDlg *>::iterator itr = m_mapP2PSendMsgDlg.begin();
	for(; itr != m_mapP2PSendMsgDlg.end(); itr++)
	{
		itr->second->UpdateBk();
	}
}

void CMessageHandleWnd::CloseAllP2PSendMsgDlg()
{
#ifdef _CS_PLAT_EX_
	if(CDuiCSPlatDlg::GetInstance() != NULL && ::IsWindow(CDuiCSPlatDlg::GetInstance()->GetHWND()))
	{   
		//vector<CDuiP2PSendMsgDlg*> vecDlg = /*CDuiCSPlatDlg::GetInstance()->m_vecP2PSendMsgDlg*/;
		for(int i = 0; i < g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg.size(); ++i)
		{
			g_pMessageHandleWnd->m_vecAllP2PSendMsgDlg[i]->SendMsgDlgNotify();
		}
		if(CDuiCSPlatDlg::GetInstance() != NULL && CDuiCSPlatDlg::GetInstance()->GetHWND() != NULL)
			CDuiCSPlatDlg::GetInstance()->Close();
		g_pMessageHandleWnd->m_pDlgSelectP2PSendMsgDlg = NULL;
	}
#else
	if(CDuiCounsel::GetInstance() != NULL && ::IsWindow(CDuiCounsel::GetInstance()->GetHWND()))
	{   
		vector<CDuiP2PSendMsgDlg*> vecDlg = CDuiCounsel::GetInstance()->m_vecP2PSendMsgDlg;
		for(int i = 0; i < (int)vecDlg.size(); ++i)
		{
			vecDlg[i]->SendMsgDlgNotify();
		}
		CDuiCounsel::GetInstance()->Close();
	}
#endif
}

void CMessageHandleWnd::AddP2PSendMsgDlg(CDuiP2PSendMsgDlg* pDlg)
{
	m_vecAllP2PSendMsgDlg.push_back(pDlg);
}

void CMessageHandleWnd::DeleteAllP2PSendMsgDlg()
{
	for (int nDlg = 0; nDlg < m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		if(m_vecAllP2PSendMsgDlg[nDlg]->m_hWnd != NULL)
			m_vecAllP2PSendMsgDlg[nDlg]->SendMsgDlgNotify();
		/*else
		{
		delete m_vecAllP2PSendMsgDlg[nDlg];
		m_vecAllP2PSendMsgDlg[nDlg] = NULL;
		}*/
	}
	//m_vecAllP2PSendMsgDlg.clear();
}

void CMessageHandleWnd::UpdateAllFastWord()
{
	for (int nDlg = 0; nDlg < m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		if(m_vecAllP2PSendMsgDlg[nDlg] != NULL && m_vecAllP2PSendMsgDlg[nDlg]->m_hWnd != NULL)
			m_vecAllP2PSendMsgDlg[nDlg]->UpdateFastWordInfo();
	}
}

CScreenCtrl* CMessageHandleWnd::GetScreenCtrl()
{
	return m_pScreenCtrl;
}

void CMessageHandleWnd::NewScreenCtrl()
{
	m_pScreenCtrl = new CScreenCtrl(GetDesktopWindow());
	m_pScreenCtrl->m_strImageFile = g_LocalData.GetImageFileCachePath();
}

void CMessageHandleWnd::DeleteScreenCtrl()
{
	delete m_pScreenCtrl;
	m_pScreenCtrl = NULL;
}


#ifdef _CS_PLAT_EX_
//结束所有的监控
void CMessageHandleWnd::CloseAllMonitor()
{
	for (int nDlg = 0; nDlg < m_vecAllP2PSendMsgDlg.size(); nDlg++)
	{
		if(m_vecAllP2PSendMsgDlg[nDlg] != NULL && m_vecAllP2PSendMsgDlg[nDlg]->m_hWnd != NULL)
			m_vecAllP2PSendMsgDlg[nDlg]->CloseMonitor();
	}
}
#endif