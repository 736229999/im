#include "stdafx.h"
#include "ThreadManager.h"
#include "RouterBusiness.h"
#include "MiniAppNetwork.h"
#include "RouterProtocol.h"
#include "BusinessFriendManager.h"
#include "BusiLogin.h"
#include "BusinessServerMsg.h"
#include "BusinessOfflineMsg.h"
#include "BusiHeartTick.h"
#include "BusinessFamilyManager.h"
#include "BusinessFileTrans.h"
#include "BusinessRouterUserPacket.h"
#include "BusinessWebMsg.h"
#include "BusinessMobileMsg.h"


CThreadManager::CThreadManager(void)
{

}

CThreadManager::~CThreadManager(void)
{
	EndThread();
}

//启动各个工作线程
void CThreadManager::StartThread()
{
	CRouterProtocol::GetInstance()->SetPacketBufferArray(m_PacketBufferArray);

	//线程A
	pthread_t hThread; 
	int i = 0;
	for(i = 0 ; i < CAppManager::GetInstance().GetConfig().GetMaxTdGrpA() ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcA,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}

	for(i = 0 ; i < 1 ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcE,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}
	
	for(i = 0 ; i < 1 ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcF,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}

	for(i = 0 ; i < 1 ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcG,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}

	//线程B
	for( i = 0 ; i < CAppManager::GetInstance().GetConfig().GetMaxTdGrpB() ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcB,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}

	//线程C
	for( i = 0 ; i < CAppManager::GetInstance().GetConfig().GetMaxTdGrpC() ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcC,				// thread function 
			this,						// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}

	//统计线程
	#if 0
	hThread = CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		BusinessProcD,				// thread function 
		this,						// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier 
	m_listThreadHandleBuf.push_back(hThread);
	#endif
}

//结束工作线程
void CThreadManager::EndThread()
{
	for( int i=0 ;i <BUSINESS_PROC_TYPE_NUM; ++i )
	{
		m_ThreadExitFlag[i] = true;
	}
	
	if( 0 == m_listThreadHandleBuf.size())
	{
		return ;
	}

	pthread_t* pThreadHandle = new pthread_t[m_listThreadHandleBuf.size()];
	
	
	list <pthread_t>::iterator Handle_Iter;
	pthread_t hThread; 

	//等待所有线程结束
	int j = 0;
	for( Handle_Iter = m_listThreadHandleBuf.begin(); Handle_Iter != m_listThreadHandleBuf.end(); ++Handle_Iter)
	{
		hThread = *Handle_Iter;		
		pThreadHandle[j++] = hThread;
	}

	::WaitForMultipleObjects((DWORD)m_listThreadHandleBuf.size(),
		pThreadHandle,TRUE,2000);	//等待两秒退出

	//关闭所有线程句柄
	for( Handle_Iter = m_listThreadHandleBuf.begin(); Handle_Iter != m_listThreadHandleBuf.end(); ++Handle_Iter)
	{
		hThread = *Handle_Iter;
		::CloseHandle(hThread);
	}

	m_listThreadHandleBuf.clear();

	delete[] pThreadHandle;
}


void*  CThreadManager::BusinessProcA( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_A);

	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}
	CDataBaseFamily DatabaseFamily;
	DatabaseFamily.SetConnectDB(familyuid, familypwd, familysid);
	if (!DatabaseFamily.ConnectDB())
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseFamily.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	router.SetDataBase(&DatabaseMini,&DatabaseWeb,&DatabaseFamily);

	//初始化与全局服务器的连接
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//业务类注册处
	CBusiRegister				busRegister(&router);
	//CBusiLogin					busLogin(&router);
	CBusiLogout					busLogout(&router);
	CBusinessReloginNotify		busiReloginNotify(&router);
	CBusinessGetFriendBasicInfo busGetFrinedBasicInfo(&router);
	CBusinessGetFriendFullInfo	busGetFriendFullInfo(&router);
	CBusinessModFriendComment	busModFriendComment(&router);
	CBusinessServerMsg			busServerMsg(&router);
	CBusinessFriendModify		busFriendModify(&router);
	CBusinessVerifyFriend		busVerifyFriend(&router);
	CBusinessGetFriendList		busGetFriendList(&router);
	CBusinessGetGroupInfo		busGetGroupInfo(&router);
	CBusinessGroupInfoModify	busGroupInfoModify(&router);
	CBusinessModifyUserInfo		busModifyUserInfo(&router);
	CBusinessOfflineMsg			busOfflineMsg(&router);
	CBusinessSearchFriend		busSearchFriend(&router);
	CBusinessUserActionAudit	busUserActionAudit(&router);
	CBusinessCreateFamily		busCreateFamily(&router);
	CBusinessDismissFamily		busDismissFamily(&router);
	CBusinessFamilyApply		busFamilyApply(&router);
	CBusinessFamilyFind			busFamilyFind(&router);
	CBusinessFamilySendMsg		busFamilySendMsg(&router);
	CBusinessFamilyVerify		busFamilyVerify(&router);
	CBusinessGetFamilyInfo		busFamilyGetInfo(&router);
	CBusinessGetFamilyList		busFamilyList(&router);
	CBusinessGetFamilyMem		busFamilyMem(&router);
	CBusinessSetFamilyInfo		busSetFamilyInfo(&router);
	CBusinessSetFamilyMember	busSetFamilyMember(&router);
	CBusinessModifyUnderWrite	busModifyUnderWrite(&router);
	CBusinessModifyFriendFlag	busModifyFriendFlag(&router);
	CBusinessGetUserFamilyFlag	busGetUserFamilyFlag(&router);
	CBusinessFriendInvite		busSendFriendInvite(&router);
	CBusinessFamilyNotify		busFamilyNotify(&router);
	CBusinessFamiliyOfflineMsg	busFamilyOfflineMsg(&router);
	CBusinessWebMsgAck			busWebMsgAck(&router);
	CBusinessPayMoney			busPayMoney(&router);
	CBusinessSendMobileMessage  busSendMobileMsg(&router);
	CBusinessCustListMod        busCustListMod(&router);
	CBusinessCustListReq        busCustListReq(&router);
	CBusinessFamiliyGetProfile	busFamilyGetProfile(&router);
	CBusinessFamiliyGetProfileByID	busFamilyGetProfileByID(&router);
	CBusinessFamiliySetProfile	busFamilySetProfile(&router);
	CBusinessSendMessage		busSendMessage(&router);
//	CBusinessWebMoreMsg			busWebMoreMsg(&router);
	CBusinessWebBusiList		busWebBusiList(&router);
	CBusinessXmppBusiList		busXmppBusiList(&router);
//	CBusinessGetSubAccount		busGetSubAccount(&router);
	CBusinessCheckSubOnline		busCheckSubOnline(&router);
	CBusinessGetOnlineStatus	busGetOnlineStatus(&router);
	CBusinessFamiliyInviteReq   busFamilyInviteReq(&router);
	CBusinessFamiliyAgreeReq	busFamilyAgreeReq(&router);
	CBusinessModHangUp			busModHangUp(&router);
	CBusinessSaveHeadFile		busSaveHeadFile(&router);
	CBusinessModifyCustRemark   busModifyCustRemark(&router);
	CBusinessGetCustRemark      busGetCustRemark(&router);
	CBusinessReqCusterTran      busReqCusterTran(&router);
	CBusinessAckCusterTran      busAckCusterTran(&router);
	CBusinessHistoryMsg			busHistoryMsg(&router);

	
	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcA Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_A])
	{
		router.RoutOne();
	}

	//DatabaseWeb.DisConnect();

	return RESULT_SUCCESS;
}



void*  CThreadManager::BusinessProcB( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_B);


	//初始化与全局服务器的连接
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//业务类注册处
	CBusiHeartTick				busiHeartTick(&router);
	//CBusinessServerMsgAck		busServerMsgAck(&router);
	CBusinessGetLinkStatus		busGetLinkStatus(&router);
	//CBusinessFamilyNotifyAck	busFamilyNotifyAck(&router);
	CBusinessRouterUserPacket	busRouterUserPacket(&router);
	CBusinessRouterUserAckPacket busRouterUserAckPacket(&router);
	CBusinessRouterPacket		busRouterPacket(&router);
	CBusinessRouterAckPacket	busRouterAckPacket(&router);
	//modify by lvwl ver:0106 家族消息缓存版将访问数据库，在家族状态通知离线时可能释放家族成员。
	CBusinessModifyStatus		busModifyStatus(&router);
	CBusinessWebMsg				busWebMsg(&router);
	CBusinessSystemLog			busSystemLog(&router);
	CBusinessMobileMsg			busMobilyMsg(&router);
	CBusinessMobileMsgAck		busMobileMsgAck(&router);
	CBusinessTransfer			busTransfer(&router);
	CBusinessTransferInd		busTransferInd(&router);
	//CBusinessShakeFriendWin     busShakeFriendWin(&router);


	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcB Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_B])
	{
		router.RoutOne();
	}

	return RESULT_SUCCESS;
}

void*  CThreadManager::BusinessProcC( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_C);

	CBusinessFileUpload		busFileUpload(&router);
	CBusinessFileDownload	busFileDownload(&router);


	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcC Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_C])
	{
		router.RoutOne();
	}

	return RESULT_SUCCESS;
}

void*  CThreadManager::BusinessProcD( LPVOID lpParam )
{
	//CThreadManager* pManager = (CThreadManager*)lpParam;

	time_t timer; /* 当前的系统时间 */
	CheckResult result;
	char* Svrip = NULL;
	unsigned int length;

	Svrip = CAppManager::GetInstance().GetConfig().GetListenAddr();
	length = sizeof(result.SvrIp);
	strncpy(result.SvrIp, Svrip, length-1);
	
	result.SvrIp[length - 1] = 0;
	result.Svrport = PORT_IM_BUSINESS;

	while(1)

	{
		sleep(10);

		time(&timer);
		ctime_r(&timer, result.CurTime); 
		
		if(SysStat::GetInstance().getCpuUseRatio())
			result.CpuUsed = SysStat::GetInstance().getCpu().cpu_use;
		
		if(SysStat::GetInstance().getMemInfo())
		{
			result.MemAll = SysStat::GetInstance().getMem().nTotal;
			result.MemUsed = (SysStat::GetInstance().getMem().nTotal - SysStat::GetInstance().getMem().nFree);
		}	

		result.RcvPacket = SysStat::GetInstance().getRevNum();
		result.SendPacket = SysStat::GetInstance().getSndNum();
		result.onlinenum = SysStat::GetInstance().getOnlineNum();
	
//		CLog::Log("-----------------------------------------------------------------------\n");	
//		CLog::Log("CThreadManager::BusinessProcD",CLog::TYPE_IMPORTANT,"Current Time",result.CurTime);	
		CLog::Log("CThreadManager::BusinessProcD",CLog::TYPE_IMPORTANT,"Server Version: %s","Version 1.0.0");
//		printf("Server Ip: %s, Port: %d\n",result.SvrIp, result.Svrport);
//		printf("CPU USED RATIO: %f,MEMERY TOTAL: %d, MEMERY USED: %d\n",result.CpuUsed,result.MemAll,result.MemUsed);
//		printf("ONLINE PEOPEL: %d,RECEIVE PACKET: %ld,SEND PACKET: %ld\n",result.onlinenum,result.RcvPacket,result.SendPacket);
//		CLog::Log("-----------------------------------------------------------------------\n");		
	}


	return RESULT_SUCCESS;
}


void*  CThreadManager::BusinessProcE( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_E);

	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcE",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcE",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}
	CDataBaseFamily DatabaseFamily;
	DatabaseFamily.SetConnectDB(familyuid, familypwd, familysid);
	if (!DatabaseFamily.ConnectDB())
	{
		CLog::Log("CThreadManager::BusinessProcE",CLog::TYPE_ERROR,"DatabaseFamily.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	router.SetDataBase(&DatabaseMini,&DatabaseWeb,&DatabaseFamily);

	//初始化与全局服务器的连接
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//业务类注册处
	CBusiLogin					busLogin(&router);
	
	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcE Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_E])
	{
		router.RoutOne();
	}

	//DatabaseWeb.DisConnect();

	return RESULT_SUCCESS;
}

void*  CThreadManager::BusinessProcF( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_F);

	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcF",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcF",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}
	CDataBaseFamily DatabaseFamily;
	DatabaseFamily.SetConnectDB(familyuid, familypwd, familysid);
	if (!DatabaseFamily.ConnectDB())
	{
		CLog::Log("CThreadManager::BusinessProcF",CLog::TYPE_ERROR,"DatabaseFamily.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	router.SetDataBase(&DatabaseMini,&DatabaseWeb,&DatabaseFamily);

	//初始化与全局服务器的连接
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//业务类注册处
	CBusinessWebMoreMsg 		busWebMoreMsg(&router);
	
	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcF Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_F])
	{
		router.RoutOne();
	}

	//DatabaseWeb.DisConnect();

	return RESULT_SUCCESS;
}

void*  CThreadManager::BusinessProcG( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//初始化包源队列
	CRouterBusiness router(PROCESS_LIST_TYPE_G);

	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcG",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcG",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}
	CDataBaseFamily DatabaseFamily;
	DatabaseFamily.SetConnectDB(familyuid, familypwd, familysid);
	if (!DatabaseFamily.ConnectDB())
	{
		CLog::Log("CThreadManager::BusinessProcG",CLog::TYPE_ERROR,"DatabaseFamily.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	router.SetDataBase(&DatabaseMini,&DatabaseWeb,&DatabaseFamily);

	//初始化与全局服务器的连接
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//业务类注册处
	CBusinessGetSubAccount		busGetSubAccount(&router);
	
	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"BusinessProcG Register OK!");

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_G])
	{
		router.RoutOne();
	}

	//DatabaseWeb.DisConnect();

	return RESULT_SUCCESS;
}


//回调函数，接收到网络层的数据时的通知
void CThreadManager::OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	//printf("CThreadManager::OnReceive nEvent:%d\n",nEvent);

	//CThreadManager* pThreadManager = (CThreadManager*)pUserKey;
	switch(nEvent)
	{
	case NE_RECVDATA:	//收到一个数据包
		{
			//路由到合适的处理队列
			CRouterProtocol::GetInstance()->RouterOnePacket(pRecvData);
		}
		break;
	case NE_SENDFAILED: //发送失败
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"发送失败");
		}
		break;
	case NE_LISTENSTOP:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"监听服务停止");
		}
		break;
	case NE_ACKTIMEOUT:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"应答超时");
		}
		break;
	case NE_SENDOK:
	case NE_RESEND:
		break;
	default:
		CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"错误通知类型");
	}

}

