#include "stdafx.h"
#include "AppManager.h"
#include "PublicType.h"
#include "Log.h"
#include "GlobalBusiControl.h"
#include "ConnBusi.h"
#include "ConnBusiControl.h"
#include "BusinessExIMSrv.h"
#include "BusiHeartTick.h"
//#include "resource.h"
#include "DataBaseLib.h"
#include "BusinessWebMsg.h"
#include "BusinessWebIM.h"
#include "./zookeeper/ZookeeperClient.h"
#include <time.h>
#include "RouterProtocol.h"
#include "../DataBaseLib/MysqlLib.h"
#include "zmqthread.h"
#include "ThreadMsgNotify.h"
#include "imapi.h"

/* 版本说明
ver:0105 家族消息缓存
ver:0106 取消家族消息缓存

*/

#ifdef ACCESS_SERVER
// 收到网络数据
BOOL RecvNetData(void *pNet, const RecvDataStruct &recv)
{
	LPXT_HEAD lpSendHead = (LPXT_HEAD)recv.data;

	if(lpSendHead->cmd != CMD_LOGIN_REQ && lpSendHead->cmd != CMD_DIRECTORY_REQ && lpSendHead->cmd != CMD_LOGOUT)
	{
		CLog::Log("RecvNetData",CLog::TYPE_DEBUG,"收到数据包cmd[0x%X] flag[0x%X]", lpSendHead->cmd, lpSendHead->flag);
		
		// 接收到数据包，转给ImServer服务器
		CZmqServer::GetInstance()->SendPacket(recv);

		return FALSE;
	}
	return TRUE;
}
#else
void SendNetData(CGlobalBusi *pGlobalBusi, const SendDataStruct &send)
{
	CZmqServer::GetInstance()->SendPacket(pGlobalBusi, send);
}
#endif


CAppManager::CAppManager(void):
m_hBlock(false,false)
{ 
	srand(timeGetTime());
	m_SendPacketSeq = (rand()<<16)|rand(); 
	m_pGlobalBusiControl = NULL;
	m_pGlobalFamilyBusiControl = NULL;
#ifdef ACCESS_SERVER	
	m_pHeartTickCheck = NULL;
#ifdef XMPP_BUILD	
	m_nFD = -1;
#endif	
#endif	
//	m_WebMsgStatus = false;
//	m_pWebIMMsgCheck = NULL;
	m_hNameEvent = NULL;
	//m_pSSDBConnCheck = NULL;
	m_pMsgNotifyBuffer = NULL;
	m_pThreadMsgNotify = NULL;
}

CAppManager::~CAppManager(void)
{
	SAFE_DELETE(m_pGlobalBusiControl);
	SAFE_DELETE(m_pGlobalFamilyBusiControl);
	SAFE_DELETE(m_pConnBusiControl);
	SAFE_DELETE(m_pConnBusi);
#ifdef ACCESS_SERVER	
	SAFE_DELETE(m_pHeartTickCheck);
#endif	
//	SAFE_DELETE(m_pWebIMMsgCheck);
//	SAFE_DELETE(m_pTimeLenSave);
	SAFE_DELETE(m_pMsgNotifyBuffer);
	SAFE_DELETE(m_pThreadMsgNotify);
	//还有其他
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM; i++)
	{
		SAFE_DELETE(m_pChatMsgPacket[i]);
		SAFE_DELETE(m_pChatMsgSave[i]);
	}
}


#ifdef ACCESS_SERVER
bool CAppManager::Init()
{
	//记录程序启动时间
	time( &m_start );

	printf("CAppManager::Init \n");

	// 连接redis，传递给网络层
	/*char *memcached_addr = m_ConfigInfo.GetMemcachedAddr();
	int memcached_port = m_ConfigInfo.GetMemcachedPort();
	
	m_pRedis = new ImMemCache();
	if(m_pRedis->init_ImMemcached(memcached_addr,memcached_port) < 0)
	{	
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"init_ImMemcached fail ip[%s] port[%d]", memcached_addr, memcached_port);
		return false;
	}*/
	
	//配置网络层
	char* pAddr = m_ConfigInfo.GetListenAddr();
	m_AppNetwork.SetUserCallback(RecvNetData);
	if( 0 != m_AppNetwork.StartWork(pAddr,PORT_IM_BUSINESS) )
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Listen falied,ip:%s, port:%d",pAddr,PORT_IM_BUSINESS);
		return false;
	}

	UpdateVersion();

	printf("CBusinessExIMSrv::SetNetTrans\n");
	CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);
	CBusiness::SetServerID(m_ConfigInfo.GetServerNo());

	//启动mysql连接
	char* MySQLSid = m_ConfigInfo.GetMySQLSid();
	char* MySQLUser = m_ConfigInfo.GetMySQLUser();
	char* MySQLPwd = m_ConfigInfo.GetMySQLPwd();
	char* MySQLDb = m_ConfigInfo.GetMySQLDb();
	if (!MYSQL_LIB->connect(MySQLDb,MySQLSid,MySQLUser,MySQLPwd)) {
		return false;
	}
	
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceiveNULL);
	printf("m_AppNetwork.SetUserCallback\n");
		list < uint32 > m_ServerList;
		m_ConfigInfo.GetServerList(&m_ServerList);

	//配置与全局服务器的连接
	printf("CAppManager::Init 3.1\n");

#ifdef IM_USE_CACHE
	list < uint32 > GlobalBusiServer;
	m_ConfigInfo.GetServerList(&GlobalBusiServer);
	m_pGlobalBusiControl = new GlobalBusiControl(GlobalBusiServer,PORT_MEMCACHED_ONLINE);
	//将服务器IP加入到网络层中
	int i = 0;
	for(list < uint32 >::iterator svrit = GlobalBusiServer.begin();
		svrit != GlobalBusiServer.end(); svrit++)
	{	
		m_AppNetwork.AddImServer(i,*svrit);
		i++;
	}
	printf("CAppManager::Init 3.2\n");
	pAddr = m_ConfigInfo.GetinListenAddr();
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr,PORT_MEMCACHED_ONLINE))
	{
		printf("CAppManager::Init 3.3\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,PORT_MEMCACHED_ONLINE);
		return false;
	}	
#else
	uint32 nListenAddr;
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineUserServerIP());		
	m_pGlobalBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_USER_MANAGER);
	printf("CAppManager::Init 3.2\n");
	pAddr = m_ConfigInfo.GetinListenAddr();
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 3.3\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineFamilyServerIP());
		
	printf("CAppManager::Init 2\n");

	m_pGlobalFamilyBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_CLUSTER_MANAGER);
	printf("CAppManager::Init 3\n");
	if( 0 != m_pGlobalFamilyBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 4\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online family service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}	
#endif

	m_pMsgNotifyBuffer = new CFIFOPacketBuffer(10240, MSGNOTIFY_BUF_LENGTH);	
	if(m_pMsgNotifyBuffer == NULL)
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"m_pMsgNotifyBuffer 申请空间失败");
	}
	
	//启动工作线程
	m_ThreadManager.StartThread();
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	//启动心跳管理	
	m_pHeartTickCheck = new CHeartTickCheck;
	m_pHeartTickCheck->StartWork();
	//启动WEBIM互通(离线)消息处理线程
	//m_pWebIMMsgCheck = new CWebIMMsgCheck;
	//m_pWebIMMsgCheck->StartWork();

	//启动httpserver线程
	//m_pHttpServer = new CHttpServer;
	//m_pHttpServer->StartWork();
	#ifdef IM_USE_ZOOKEEPER
	//启动zookeeper线程
	m_pZookeeper = new CZookeeper;
	m_pZookeeper->StartWork();
	#endif

	//初始化数据库

	//启动定时记录用户在线里长
// 	m_pTimeLenSave = new CTimeLenSave;
// 	m_pTimeLenSave->StartWork();
	CZmqServer::GetInstance()->StartWork();

	m_pThreadMsgNotify = new CThreadMsgNotify;
	m_pThreadMsgNotify->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");


	//显示控制台,并阻塞主线程,仅让工作线程处理
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL_PAL), NULL, MainDlgProc );
#ifdef XMPP_BUILD	
	m_pJabberdIm = new JabberdIm;
	m_pJabberdIm->SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);
	m_pJabberdIm->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");

	// 创建tcp socket，用于连接JabberdIm线程
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5222);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_nFD = socket(AF_INET, SOCK_STREAM, 0);

	unsigned long ul = 1;
	ioctl(m_nFD, FIONBIO, &ul); //设置为非阻塞模式

	// 尝试10次连接
	int nRlt = -1;
	for(int i = 0; i < 10; ++i)
	{
		sleep(1);
		nRlt = connect(m_nFD, (const sockaddr*)&addr, sizeof(addr));

		if(nRlt != -1)
		{
			break;
		}
	}

	if(nRlt == -1)
	{	// 连接失败
		m_nFD = -1;
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"error:%s", strerror(errno));
	}
	else
	{	// 连接成功
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"success: fd: %d", m_nFD);
	}
#endif
	if (::WaitForSingleObject(m_hBlock, INFINITE) == WAIT_OBJECT_0)
		return true;
	return true;
}
#else
bool CAppManager::Init()
{
	//记录程序启动时间
	time( &m_start );

	/*
	//为守护进程的信号通讯作准备
	m_hNameEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"ImServer");
	if( m_hNameEvent == NULL )
	{
		CLog::Log("CAppManager","can't open named event ,maybe the protect process is not running");
	}
*/
	//CLog::Log("CAppManager","Version 1.0.0.2");
	printf("CAppManager::Init \n");

	// 连接redis，传递给网络层
	/*char *memcached_addr = m_ConfigInfo.GetMemcachedAddr();
	int memcached_port = m_ConfigInfo.GetMemcachedPort();
	
	m_pRedis = new ImMemCache();
	if(m_pRedis->init_ImMemcached(memcached_addr,memcached_port) < 0)
	{	
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"init_ImMemcached fail ip[%s] port[%d]", memcached_addr, memcached_port);
		return false;
	}*/
	
	//配置网络层
	char* pAddr = m_ConfigInfo.GetListenAddr();
	m_AppNetwork.SetCallback_SendNetData(SendNetData);
	if( 0 != m_AppNetwork.StartWork(pAddr,/*PORT_IM_BUSINESS*/-1) )	// 监听端口为-1表示不开启监听，ImServer只从AccessServer中接收数据包
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Listen falied,ip:%s, port:%d",pAddr,/*PORT_IM_BUSINESS*/-1);
		return false;
	}

	UpdateVersion();

	printf("CBusinessExIMSrv::SetNetTrans\n");
	CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);
	CBusiness::SetServerID(m_ConfigInfo.GetServerNo());

	//启动mysql连接
	char* MySQLSid = m_ConfigInfo.GetMySQLSid();
	char* MySQLUser = m_ConfigInfo.GetMySQLUser();
	char* MySQLPwd = m_ConfigInfo.GetMySQLPwd();
	char* MySQLDb = m_ConfigInfo.GetMySQLDb();
	if (!MYSQL_LIB->connect(MySQLDb,MySQLSid,MySQLUser,MySQLPwd)) {
		return false;
	}
	
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceiveNULL);
	printf("m_AppNetwork.SetUserCallback\n");
		list < uint32 > m_ServerList;
		m_ConfigInfo.GetServerList(&m_ServerList);

	//配置与全局服务器的连接
	printf("CAppManager::Init 3.1\n");
	
#ifdef IM_USE_CACHE
	list < uint32 > GlobalBusiServer;
	m_ConfigInfo.GetServerList(&GlobalBusiServer);
	m_pGlobalBusiControl = new GlobalBusiControl(GlobalBusiServer,PORT_MEMCACHED_ONLINE+10);
	//将服务器IP加入到网络层中
	int i = 0;
	for(list < uint32 >::iterator svrit = GlobalBusiServer.begin();
		svrit != GlobalBusiServer.end(); svrit++)
	{	
		m_AppNetwork.AddImServer(i,*svrit);
		i++;
	}
	printf("CAppManager::Init 3.2\n");
	pAddr = m_ConfigInfo.GetinListenAddr();
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr,PORT_MEMCACHED_ONLINE+10))
	{
		printf("CAppManager::Init 3.3\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,PORT_MEMCACHED_ONLINE);
		return false;
	}	
#else
	uint32 nListenAddr;
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineUserServerIP());		
	m_pGlobalBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_USER_MANAGER+10);
	printf("CAppManager::Init 3.2\n");
	pAddr = m_ConfigInfo.GetinListenAddr();
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 3.3\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineFamilyServerIP());
		
	printf("CAppManager::Init 2\n");

	m_pGlobalFamilyBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_CLUSTER_MANAGER+10);
	printf("CAppManager::Init 3\n");
	if( 0 != m_pGlobalFamilyBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 4\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online family service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}	
#endif

	m_pMsgNotifyBuffer = new CFIFOPacketBuffer(1024, MSGNOTIFY_BUF_LENGTH);	
	if(m_pMsgNotifyBuffer == NULL)
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"m_pMsgNotifyBuffer 申请空间失败");
	}

	//启动工作线程
	m_ThreadManager.StartThread();
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

#if 0
	//启动MEMCACHED检测
#ifdef IM_USE_CACHE
	m_pMEMCConnCheck = new ImMemCacheConnCheck(memcached_addr,memcached_port);
	m_pMEMCConnCheck->StartWork();
#endif
#endif

#ifdef SAVE_OFFLINE
	//启动WEBIM互通(离线)消息处理线程
	//m_pWebIMMsgCheck = new CWebIMMsgCheck;
	//m_pWebIMMsgCheck->StartWork();
#endif
	//启动httpserver线程
	m_pHttpServer = new CHttpServer;
	m_pHttpServer->StartWork();
	#ifdef IM_USE_ZOOKEEPER
	//启动zookeeper线程
	m_pZookeeper = new CZookeeper;
	m_pZookeeper->StartWork();
	#endif
	#if 0//bwwan
	//启动向运营后台获取组信息线程
	//m_pGetGroup = new CGetBackendGroup;
	//m_pGetGroup->StartWork();
	#endif

#if 0

	m_test = new Testpacket;
	//m_test->SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	m_test->StartWork();
#endif

	//	11 : 初始化redis排队	
	imapi csInitReq;
	string info;
	csInitReq.imapiReq(imapi::CSINITSERVER,info);

	//初始化数据库


	
	//启动写消息记录线程
//	m_pChatMsgPacket = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, sizeof(XT_TALK));
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM ; i++)
	{
		m_pChatMsgPacket[i] = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, sizeof(XT_TALK));
		
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init m_pChatMsgPacket %p",m_pChatMsgPacket[i]);
	}
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM ; i++)
	{
		m_pChatMsgSave[i] = new CChatMsgSave;
		m_pChatMsgSave[i]->StartWork();
	}

	m_pKafKaPacket = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, 8192);
	m_pKafKaProducer = new KafKa_Producer;
	m_pKafKaProducer->StartWork();


	//启动定时记录用户在线里长
// 	m_pTimeLenSave = new CTimeLenSave;
// 	m_pTimeLenSave->StartWork();
	CZmqServer::GetInstance()->StartWork();

	m_pThreadMsgNotify = new CThreadMsgNotify;
	m_pThreadMsgNotify->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");


	//显示控制台,并阻塞主线程,仅让工作线程处理
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL_PAL), NULL, MainDlgProc );

	if (::WaitForSingleObject(m_hBlock, INFINITE) == WAIT_OBJECT_0)
		return true;
	return true;
}

#endif

//配置与接入服务器的连接
bool CAppManager::IniConnectServer()
{
	printf("CAppManager::IniConnectServer\n");
	char* pAddr = m_ConfigInfo.GetinListenAddr();
	int nListenAddr = inet_addr(m_ConfigInfo.GetConnectServerIP());
	m_pConnBusiControl = new CConnBusiControl(nListenAddr,PORT_CONN_SERVER);
	m_pConnBusi = new CConnBusi(m_pConnBusiControl);
	m_pConnBusiControl->SetConnBusi(m_pConnBusi);
	if( 0 != m_pConnBusiControl->StartWork(pAddr))
	{
		CLog::Log("CAppManager CConnBusiControl",CLog::TYPE_ERROR,"监听接入服务器失败, ip:%s, port:%d",pAddr,0);
		return false;
	}


	//向接入注册
	SERVER_REGISTER srvRegister;
	srvRegister.dnsIP = inet_addr(pAddr);
	srvRegister.flags = 0x00;
	srvRegister.maxCount = GetConfig().GetMaxUserCount();
	srvRegister.srvType = 0x01;
	srvRegister.udpPort = PORT_IM_SVRP2P;

	m_pConnBusi->BusiRequest(&srvRegister); 

	return true;
}

bool CAppManager::UnregistConnectServer()
{
	SERVER_UNREGISTER srvUnregister;
	srvUnregister.authData = 0x00;
	m_pConnBusi->BusiRequest(&srvUnregister); 
	return true;
}

 
CAppManager& CAppManager::GetInstance()
{
	static CAppManager	SingleInstance;
	return SingleInstance;
}

void CAppManager::LogoutAllUser()
{
/*
	//初始化数据库
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CAppManager::LogoutAllUser",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CAppManager::LogoutAllUser",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return;
	}

	CUser *pUser = NULL;
	CUser::m_UserListLock.Lock();;
	for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin();it != CUser::m_userhash.end();++it)
	{
		pUser = it->second;//CUser::m_pUserListHead;
		if(!pUser)
			continue;
		pUser->NotifyServerClose();
		FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
		for(;itf != pUser->m_PermFamilyMap.end(); ++itf)
		{
			//保存固定群消息id
			DatabaseMini.Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
		}
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
		//DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nBroadcastID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

		//更新数据库在线表状态
		DatabaseMini.Save_User_Logout(pUser->m_nID);
		
		//保存用户在线时长
		int money;
		time_t tm;
		time(&tm);
		DatabaseMini. Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
		if (money > 0  && pUser->m_nID != 0)
		{
			if (money > 5)
				money = 5;
			DatabaseWeb.Update_User_Money(pUser->m_nID, money);
// 			SendDataStruct sendData;
// 
// 			char packet[MAX_NETWORK_PACKET];
// 			char content[255];
// 			sprintf(content,"为了鼓励您使用Mini，已经给您增加了%d积分！",money*5);
// 			CLog::Log("Money", CLog::TYPE_IMPORTANT, "用户:%d赠送积分:%d", pUser->m_nID, money*5);
// 
// 			CSendPacket pSendPacket(packet);
// 			pSendPacket <<(uint32)0
// 				<<(uint16)XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE
// 				<<(uint32)tm
// 				<<(uint8)2
// 				<<content;
// 
// 			DatabaseMini.Save_User_Msg(pUser->m_nID, 0,0,0,(uint32)tm,XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE,pSendPacket.GetData(),(uint16)pSendPacket.GetSize());
		}
		
		//pUser = it->second;//pUser->m_next;
	}
	CUser::m_UserListLock.Unlock();

*/
	return;
}
void CAppManager::Exit()
{

//	UnregistConnectServer();
	LogoutAllUser();
#ifdef ACCESS_SERVER	
	if(m_pHeartTickCheck)
	{
		m_pHeartTickCheck->StopWork();
	}
#endif	
//	if(m_pWebIMMsgCheck)
//	{
//		m_pWebIMMsgCheck->StopWork();
//	}
	
	m_ThreadManager.EndThread();

	m_AppNetwork.StopWork();

	if( m_pGlobalBusiControl )
	{
		m_pGlobalBusiControl->StopWork();
	}
	if( m_pGlobalFamilyBusiControl )
	{
		m_pGlobalFamilyBusiControl->StopWork();
	}
	
	if( m_pConnBusiControl )
	{
		m_pConnBusiControl->StopWork();
	}

	SAFE_DELETE(m_pConnBusi);


#ifndef _DEBUG
/*
	if( m_hNameEvent )
	{
		CloseHandle(m_hNameEvent);
	}
	*/
#endif

}

void CAppManager::ResetProtectEvent()
{
	static DWORD nLastTime = 0;

	//设置守护进程信号状态，表明当前后台服务是活动的,
	//如果该信号状态关闭，守护进程会重起后台服务
	DWORD nCurrentTime = timeGetTime();
	if( (nCurrentTime - nLastTime) > 30*1000)	//不用每个包都设置信号，超过一定时间才设置
	{
		nLastTime = nCurrentTime;
		/*
		if( m_hNameEvent )
		{
			::SetEvent(m_hNameEvent);
		}
		*/
	}
}
void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}

#ifdef ACCESS_SERVER
#ifdef XMPP_BUILD
// 向jabberdIM线程发送数据
bool CAppManager::SendToJabberdIM(BYTE data[], int nLength, uint32 fdCl /*=0*/)
{
	if(m_nFD == -1)
	{
		return false;
	}

	XT_HEAD *pHead = (XT_HEAD*)data;

	CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"send。length:%d did[%d] cmd[0x%x]", nLength+8, pHead->did, pHead->cmd);

	// 加头部标示
	BYTE *pData = new BYTE[nLength+8];
	memcpy(pData, "XMPP", 4);
	memcpy(pData+4, (BYTE*)&fdCl, 4);
	memcpy(pData+8, data, nLength);
	int length = 0;
	length += send(m_nFD, pData, nLength+8, 0);
	while(length < nLength+8 || length == 0)
	{
		length += send(m_nFD, pData, nLength+8, 0);
		
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"more send %d length:%d",m_nFD,length);
	}
	CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"send %d length:%d",m_nFD,length);	

	return true;
}
#endif
#endif
