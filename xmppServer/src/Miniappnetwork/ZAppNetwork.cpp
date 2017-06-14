#include "stdafx.h"
#include "ZAppNetwork.h"
#include "../../common/Function.h"
#include "../../common/md5.h"
#include "../../include/protocol.h"
#include "../../common/ConfigInfo.h"
#include <netinet/in.h>    // for sockaddr_in
#include <sys/types.h>    // for socket
#include <sys/socket.h>    // for socket
#include "../../common/xtpacket.h"

#define ONLINEEXPIRETIME (1000 * 60 * 30) //在线定位信息过期时间 30 分钟

#define TIMERPERIOD (1000 * 60 * 1) //时钟周期 1分钟
#define DELAYROUTETIMEOUT (1000 * 60 * 2) //延时路由超时时间 2分钟

#define MAX_ONETIMECHECK 4000

void  ZAppNetworkMyTimeProc(void* dwUser)
{
	CZAppNetwork* pObj = (CZAppNetwork*)dwUser;
	pObj->HandleOnlineExpire();
}

void UdpSrvNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	CZAppNetwork * pObj = (CZAppNetwork *)pUserKey;
	pObj->HandleSrvRecvData(nEvent,pRecvData);
}

void UdpCntNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	if( NULL != pRecvData )
	{
		CZAppNetwork * pObj = (CZAppNetwork *)pUserKey;
		pObj->HandleCntRecvData(nEvent,pRecvData);
	}
}

void ImSvrP2PCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	if( NULL != pRecvData )
	{
		CZAppNetwork * pObj = (CZAppNetwork *)pUserKey;
		pObj->ImSvrP2PRecvData(nEvent,pRecvData);
	}
}

CZAppNetwork::CZAppNetwork(void):m_ServerList(200)
{
	m_listenIP = 0;
	m_listenPort = 0;

	m_globalUserIP = 0;
	m_globalUserPort = 0;
	m_globalFamilyIP = 0;
	m_globalFamilyPort = 0;

	memset(m_globalSessionKey,0,sizeof(m_globalSessionKey));  //全局服务器session key
	memset(m_mineSessionKey,0,sizeof(m_mineSessionKey));    //自己的 session key

	m_hMMT = NULL;

	m_pUdpSrvNetTrans = new CUdpNetTrans();

	m_nManSrvID = 0; 
	srand(timeGetTime());
	m_nCntREQSeq = (rand()<<16)|rand();

	InitializeCriticalSection(&m_seqLock);

	m_pUdpCntNetTrans = new CUdpNetTrans();

	ImSvrp2p = new CUdpNetTrans();

}

CZAppNetwork::~CZAppNetwork(void)
{
	StopWork();
	DeleteCriticalSection(&m_seqLock);
	delete m_pUdpSrvNetTrans; m_pUdpSrvNetTrans = NULL;
	delete m_pUdpCntNetTrans; m_pUdpCntNetTrans = NULL;
	delete ImSvrp2p; ImSvrp2p = NULL;
}

/*
功能描述: 处理在线内存信息过期，防止过期无效内存信息常驻内存，
监测过期的缓存包
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::HandleOnlineExpire()
{
	uint32 now = timeGetTime();
	//检查非直连用户
	LPONLINE_USER_INFO lpUser = NULL;
	m_lstIndirectUser.Lock();
	int nCount = (int)(m_lstIndirectUser.size() > MAX_ONETIMECHECK ? MAX_ONETIMECHECK : m_lstIndirectUser.size());
	int i = 0;
	for(i = 0; i < nCount; ++i)
	{//采用弹出再进入的方式，可以采用做标签循环检索方式
		lpUser = m_lstIndirectUser.front();
		if(lpUser->isDirect == 1)
		{
			m_lstIndirectUser.pop_front();
			CLog::Log("HandleOnlineExpire", CLog::TYPE_NORMAL, "非直连用户已直连:%d",lpUser->uid);
		}
		else
		{
			if((now - lpUser->joinedTime) > ONLINEEXPIRETIME )
			{//过期
				m_hashUser.DelHash(lpUser->uid);
				CLog::Log("HandleOnlineExpire", CLog::TYPE_NORMAL, "非直连用户信息过期，网络层登出用户:%d",lpUser->uid);
			}
			else
			{
				m_lstIndirectUser.push_back(lpUser);
			}
			m_lstIndirectUser.pop_front();
		}

	}
	m_lstIndirectUser.UnLock();

	//检查非直接登陆家族
	LPONLINE_FAMILY_INFO  lpFamily = NULL;
	m_lstIndirectFamily.Lock();
	nCount = (int)(m_lstIndirectFamily.size() > MAX_ONETIMECHECK ? MAX_ONETIMECHECK : m_lstIndirectFamily.size());
	for(i = 0; i < nCount; ++i)
	{//采用弹出再进入的方式，可以采用做标签循环检索方式
		lpFamily = m_lstIndirectFamily.front();
		if(lpFamily->isDirect == 1)
		{
			m_lstIndirectFamily.pop_front();
		}
		else
		{
			if(now - lpFamily->joinedTime > ONLINEEXPIRETIME)
			{//过期
				m_hashFamily.DelHash(lpFamily->fid);
			}
			else
			{
				m_lstIndirectFamily.push_back(lpFamily);
			}
			m_lstIndirectFamily.pop_front();
		}

	}
	m_lstIndirectFamily.UnLock();

	//监测过期的缓存包...
	//路由缓存包
	LPROUTEDATA lpRouteData = NULL;
	m_stLstRouteData.Lock();
	int count = (int)m_stLstRouteData.size() > MAX_ONETIMECHECK ? MAX_ONETIMECHECK : (int)m_stLstRouteData.size();
	for(int i = 0; i < count; ++i)
	{
		lpRouteData = m_stLstRouteData.front();
		if(now - lpRouteData->joinTime > DELAYROUTETIMEOUT)
		{//过期，清除缓存包
			m_stLstRouteData.DeleteObject(lpRouteData);
		}
		else
		{
			m_stLstRouteData.push_back(lpRouteData);
		}
		m_stLstRouteData.pop_front();
	}
	m_stLstRouteData.UnLock();


	//服务器将发送包缓存
	LPSRVSENDDATA lpSrvSendData = NULL;
	m_stLstSrvSendData.Lock();
	count = (int)m_stLstSrvSendData.size() > MAX_ONETIMECHECK ? MAX_ONETIMECHECK : (int)m_stLstSrvSendData.size();
	for(i = 0; i < count; ++i)
	{
		lpSrvSendData = m_stLstSrvSendData.front();
		if(now - lpSrvSendData->joinTime > DELAYROUTETIMEOUT)
		{//过期，清除缓存包
			m_stLstSrvSendData.DeleteObject(lpSrvSendData);
		}
		else
		{
			m_stLstSrvSendData.push_back(lpSrvSendData);
		}
		m_stLstSrvSendData.pop_front();
	}
	m_stLstSrvSendData.UnLock();
}

/*
功能描述: 直接投递接收数据到应用层
必要参数说明：pRecvData：收到的网络数据
lpSUser ： 数据包源用户信息
lpDUser ： 数据包目标用户信息
返 回 值：
*/ 
void CZAppNetwork::DirectPostData(RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpSUser/* = NULL*/)
{
	//printf("CZAppNetwork::DirectPostData\n");
	if( m_pUdpSrvNetTrans->FilterRecvData(pRecvData))
	{
		printf("CZAppNetwork::DirectPostData FilterRecvData return true\n");
		return;
	}

	//printf("CZAppNetwork::DirectPostData usercallback\n");
	m_pfnUserCallback(m_pUserKey,NE_RECVDATA,pRecvData);
}

/*
功能描述: 直接转发包到用户
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::DirectTrans2User(const RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpDUser)
{
	if(lpDUser == NULL || lpDUser->isDirect != 1)
		return ;
	SendDataStruct sendData;
	memcpy(sendData.data,pRecvData->data,pRecvData->data_len);
	sendData.data_len = pRecvData->data_len;

	XT_HEAD* lpHead  = (XT_HEAD*)pRecvData->data;
	if( lpHead->cmd == CMD_GET_CPIC_ACK ||
		lpHead->cmd == CMD_ROUTERUSERPACKET_REQ ||
		lpHead->cmd == CMD_ROUTERUSERPACKET_ACK
	  )
	{

	}
	else
	{
		//加密数据再放入发送队列 
		int tlen = 0;
		if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),lpDUser->sessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
		{

			CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"DirectTrans2User(%d) 加密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
				__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
			return ;
		}
		sendData.data_len = (uint16)tlen +  sizeof(XT_HEAD);
	}


	sendData.dest_ip = lpDUser->uip;
	sendData.port = lpDUser->uport;

	m_pUdpSrvNetTrans->SendDataTo(sendData);
}
/*
功能描述: 及时路由数据
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::InstRouteData(uint32 srvip,RecvDataStruct * pRecvData)
{	
	SendDataStruct sendData;
	memcpy(sendData.data,pRecvData->data,pRecvData->data_len);
	sendData.data_len = pRecvData->data_len;
	sendData.dest_ip = srvip;
	sendData.vlink = pRecvData->vlink;
	sendData.port = PORT_IM_SVRP2P;

	//二次路由不需要再加密
	ImSvrp2p->SendDataTo(sendData);

}
void CZAppNetwork::InstRouteData(uint32 srvip,SendDataStruct * pSendData)
{
	pSendData->dest_ip = srvip;
	pSendData->port = PORT_IM_SVRP2P;

	ImSvrp2p->SendDataTo(*pSendData);

}
/*
功能描述: 延时路由数据,先将数据包缓存，向全局查询目标定位新息，等待查询结果返回再路由包
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::DelayRouteData(RecvDataStruct * pRecvData)
{

	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

	//缓存需路由的包
	LPROUTEDATA pRouteData;
	pRouteData = m_stLstRouteData.NewObject();
	pRouteData->sendData.data_len = pRecvData->data_len;
	pRouteData->sendData.vlink = pRecvData->vlink;
	memcpy(&pRouteData->sendData.data,pRecvData->data,pRouteData->sendData.data_len);

	pRouteData->origin_sip = pRecvData->src_ip;
	pRouteData->origin_sport = pRecvData->port;
	pRouteData->emDestType = lpRecvHead->dtype;
	pRouteData->destID = lpRecvHead->did;
	pRouteData->queryDataSeq = AddRequestSeq(); 
	m_stLstRouteData.Lock();
	m_stLstRouteData.push_back(pRouteData);
	m_stLstRouteData.UnLock();

	//向全局查询目标定位新息
	LocateInfoQuery(lpRecvHead->dtype,lpRecvHead->did,pRouteData->queryDataSeq,QS_RECVQDEST);

}

void CZAppNetwork::QueryData(RecvDataStruct * pRecvData,uint8 flag)
{
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

	//缓存需路由的包
	LPROUTEDATA pRouteData;
	pRouteData = m_stLstRouteData.NewObject();
	pRouteData->sendData.data_len = pRecvData->data_len;
	pRouteData->sendData.vlink = pRecvData->vlink;
	memcpy(&pRouteData->sendData.data,pRecvData->data,pRouteData->sendData.data_len);

	pRouteData->origin_sip = pRecvData->src_ip;
	pRouteData->origin_sport = pRecvData->port;
	pRouteData->emDestType = lpRecvHead->dtype;
	pRouteData->destID = lpRecvHead->did;
	pRouteData->queryDataSeq = AddRequestSeq(); 
	m_stLstRouteData.Lock();
	m_stLstRouteData.push_back(pRouteData);
	m_stLstRouteData.UnLock();

	//向全局查询目标定位信息
	if(flag == QS_TRANSFERDST)
		LocateInfoQuery(lpRecvHead->dtype,lpRecvHead->did,pRouteData->queryDataSeq,flag);
	else
		LocateInfoQuery(lpRecvHead->dtype,lpRecvHead->sid,pRouteData->queryDataSeq,flag);
}

/*
功能描述: 向全局查询目标定位新息
必要参数说明：dtype：目标类型，用户或群
did： 目标ID
返 回 值：
*/ 
void CZAppNetwork::LocateInfoQuery(uint8 dtype,uint32 did,uint32 seq,uint8 vlink)
{
	//向全局查询目标定位新息
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	char plainData[MAX_NETWORK_PACKET];
	int plain_len = 0;
	if(vlink != QS_TRANSFERSRC && vlink != QS_TRANSFERDST)
	{
		if(dtype == XT_HEAD::DEST_TYPE_USER)
		{
			InitHead(lpSendHead,CMDGSRV_USERCONTROLINFO_QUERY,seq,sizeof(GSRV_USERCONTROLINFO_QUERY),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);

			LPGSRV_USERCONTROLINFO_QUERY lpUserInfoQuery = (LPGSRV_USERCONTROLINFO_QUERY)plainData;
			lpUserInfoQuery->count = 1;
			lpUserInfoQuery->uid[0] = did;
			plain_len = sizeof(GSRV_USERCONTROLINFO_QUERY) + (lpUserInfoQuery->count - 1) * sizeof(lpUserInfoQuery->uid);

			sendData.dest_ip = m_globalUserIP;
			sendData.port = m_globalUserPort;

		}
		else if(dtype == XT_HEAD::DEST_TYPE_FAMILY)
		{
			InitHead(lpSendHead,CMDGSRV_FAMILYINFO_QUERY,seq,sizeof(GSRV_FAMILYINFO_QUERY),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_CLUSTER);

			LPGSRV_FAMILYINFO_QUERY lpFamilyInfoQuery = (LPGSRV_FAMILYINFO_QUERY)plainData;

			lpFamilyInfoQuery->count = 1;
			lpFamilyInfoQuery->type = GetFamilyType(did);
			lpFamilyInfoQuery->fid[0] = did;

			plain_len = sizeof(GSRV_FAMILYINFO_QUERY) + (lpFamilyInfoQuery->count - 1) * sizeof(lpFamilyInfoQuery->fid);

			sendData.dest_ip = m_globalFamilyIP;
			sendData.port = m_globalFamilyPort;
		}
		else
		{
			return ;
		}
	}
	else
	{
		InitHead(lpSendHead,CMDGSRV_TUSERINFO_QUERY,seq,sizeof(GSRV_USERCONTROLINFO_QUERY),XT_HEAD::DEST_TYPE_SERVER,SERVERID_ONLINE_USER);
		LGSRV_TUSERINFO_QUERY lpInfoQuery = (LGSRV_TUSERINFO_QUERY)plainData;

		lpInfoQuery->id = did;
		plain_len = sizeof(GSRV_TUSERINFO_QUERY);
		
		sendData.dest_ip = m_globalUserIP;
		sendData.port = m_globalUserPort;		
	}
	int tlen = 0;
	if( !xTEAEncryptWithKey(plainData,plain_len,m_globalSessionKey,sendData.data + sizeof(XT_HEAD),&tlen) )
	{
		XT_HEAD* lpHead  = lpSendHead;
		CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"LocateInfoQuery(%d) 加密失败: sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
			__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did,lpHead->dtype);
		return ;
	}
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	sendData.vlink = vlink;
	sendData.retry_times = SEND_RETRY_TIME;

	m_pUdpCntNetTrans->SendDataTo(sendData);
}

/*
功能描述: 通知原路由者更新用户定位信息
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::UpdateInfoNotify(const RecvDataStruct * pRecvData)
{
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	InitHead(lpSendHead,CMDCS_UPDATELOCATION_NOTIFY,AddRequestSeq(),sizeof(CS_UPDATELOCATION_NOTIFY));

	char plainData[MAX_NETWORK_PACKET];
	int plain_len = 0;
	LPCS_UPDATELOCATION_NOTIFY lpNotify = (LPCS_UPDATELOCATION_NOTIFY)plainData;
	lpNotify->emDestType = lpRecvHead->dtype;
	lpNotify->destID = lpRecvHead->did;
	plain_len = sizeof(CS_UPDATELOCATION_NOTIFY);

	int tlen = 0;
	if(!xTEAEncryptWithKey(plainData,plain_len,m_mineSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
	{
		CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"UpdateInfoNotify(%d) 加密失败: sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
			__LINE__, lpRecvHead->sid,lpRecvHead->from,lpRecvHead->cmd,lpRecvHead->did, lpRecvHead->dtype);
		return ;
	}
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	sendData.dest_ip = pRecvData->src_ip;
	sendData.port = PORT_APPNET2GLOBAL;  //发送到与全局通讯的客户端端口,便于程序处理

	m_pUdpCntNetTrans->SendDataTo(sendData);
}

void CZAppNetwork::HandleSrvRecvData(int nEvent,const RecvDataStruct * pRecvData)
{
	//printf("CZAppNetwork::HandleSrvRecvData nEvent:%d\n",nEvent);
	LPXT_HEAD lpHead = (LPXT_HEAD)pRecvData->data;
	switch(nEvent)
	{
	case NE_RECVDATA:
		switch(lpHead->cmd)
		{
		case CMD_LOGIN_REQ:
		case CMD_REGIST_REQ:
			//对登陆包不作任何处理，直接交由应用层处理
			DirectPostData((RecvDataStruct *)pRecvData);
			break;
		case CMD_FILE_TRANS_REQ:
		case CMD_FILE_TRANS_IND:
		case CMD_FILE_TRANS_NOTIFY:
			HandleTransferData(pRecvData);
			break;
		default:
			HandleRecvRouteData(pRecvData);
			break;
		}
		break;
	case NE_SENDFAILED:
		{
/*			SendDataStruct* pSendData = (SendDataStruct*)pRecvData;
			XT_HEAD *lpHead = (LPXT_HEAD)pSendData->data;

			LPONLINE_USER_INFO lpSUser = NULL;
			
			CLog::Log("CZAppNetwork::HandleRecvRouteData()",CLog::TYPE_ERROR,"发送包失败id=%d,from=%d,cmd=0x%.4x,did=%d,dtype=%d",lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
			if (lpHead->dtype != XT_HEAD::DEST_TYPE_USER)
				break;
			lpSUser = m_hashUser.Find(lpHead->did);
			if (!lpSUser)
				break;

			int tlen;
			int ret = xTEADecryptWithKey(pSendData->data + sizeof(XT_HEAD),pSendData->data_len - sizeof(XT_HEAD),lpSUser->sessionKey,pSendData->data + sizeof(XT_HEAD),&tlen);
			if(!ret)
			{
				CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"HandleRecvRouteData(%d) 解密失败:sid=%d,from=%d,cmd=0x%.4x,did=%d,dtype=%d",
					__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
				return ;
			}


			pSendData->data_len = (uint16)(tlen  + sizeof(XT_HEAD));
			LPXT_SERVER_TRANSMIT pTransmit = (LPXT_SERVER_TRANSMIT)(pSendData->data + sizeof(XT_HEAD));
			*/

		}
		break;
	case NE_LISTENSTOP:
		break;
	}
}

void CZAppNetwork::HandleTransferData(const RecvDataStruct * pRecvData)
{
	LPXT_HEAD lpHead = (LPXT_HEAD)pRecvData->data;
	LPONLINE_USER_INFO lpDUser = NULL;
	LPONLINE_USER_INFO lpSUser = NULL;

	CLog::Log("HandleTransferData",CLog::TYPE_IMPORTANT,"sid:%d, cmd:0x%.4x",lpHead->sid,lpHead->cmd);

	if(lpHead->sid > 0)
	{
		lpSUser = m_hashUser.Find(lpHead->sid);
		if(lpSUser)
		{	
			if(lpHead->did > 0)
			{	
				RecvDataStruct* pRecvDataTemp = (RecvDataStruct*)pRecvData;
				int tlen = 0;
				int ret = xTEADecryptWithKey(pRecvDataTemp->data + sizeof(XT_HEAD),pRecvDataTemp->data_len - sizeof(XT_HEAD),lpSUser->sessionKey,pRecvDataTemp->data + sizeof(XT_HEAD),&tlen);
				if(!ret)
				{
					CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"HandleTransferData(%d) 解密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
						__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
					return ;
				}
				else
				{
					//获取客户端文件传输ip和端口
					if(lpHead->cmd == CMD_FILE_TRANS_NOTIFY)
					{
						lpSUser->uip = pRecvData->src_ip;
						lpSUser->uport = pRecvData->port;
					}
				}
				pRecvDataTemp->data_len = (uint16)tlen + sizeof(XT_HEAD);
			
				lpDUser = m_hashUser.Find(lpHead->did);
				if(lpDUser)
					DirectPostData((RecvDataStruct *)pRecvData);
				else
					QueryData((RecvDataStruct *)pRecvData,QS_TRANSFERDST);
			}
			else
				CLog::Log("HandleTransferData",CLog::TYPE_ERROR,"sid:%d, cmd:0x%.4x, did error",lpHead->sid,lpHead->cmd);
		}
		else
		{
			QueryData((RecvDataStruct *)pRecvData,QS_TRANSFERSRC);
		}
	}
}

/*
功能描述: 处理客户端的业务数据和集群服务器间的路由数据
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::HandleRecvRouteData(const RecvDataStruct * pRecvData)
{
	//GetCurrentThreadId  // map
	LPXT_HEAD lpHead = (LPXT_HEAD)pRecvData->data;
	LPONLINE_USER_INFO lpDUser = NULL;
	LPONLINE_FAMILY_INFO lpDFamily = NULL;
	LPONLINE_USER_INFO lpSUser = NULL;
	if( lpHead->from != XT_HEAD::FROM_SERVER)
	{
		lpSUser = m_hashUser.Find(lpHead->sid);
	}
	EM_METHOD emMethod = MD_POSTAPP;
	uint32 srvip = 0; //路由目标服务器地址
	if(lpHead->did > 0)
	{//有目标
		if(lpHead->dtype == XT_HEAD::DEST_TYPE_USER)
		{//用户
			lpDUser = m_hashUser.Find(lpHead->did);
			if(lpDUser)
			{//内存信息在本机
				if(lpDUser->isDirect == 1)
				{//目标与本机直连，则直接投递到应用层 ,若为服务器应答包则直接发送到用户
					if(lpHead->flag == XT_HEAD::FLAG_SRVACK || lpHead->flag == XT_HEAD::FLAG_MOBSRVACK)
					{
						emMethod = MD_DIRECT2USER; //服务器应答包
					}
					else
					{
						emMethod = MD_POSTAPP;
					}
				}
				else
				{//目标不与本机直连，路由此包到相应服务器
					emMethod = MD_INSTROUTE;
					srvip = lpDUser->srvip;
				}
			}
			else
			{//本机无目标用户信息，路由此包，先缓存包，向全局查询目标信息
				emMethod = MD_DELAYROUTE;
			}
		}
		else if(lpHead->dtype == XT_HEAD::DEST_TYPE_FAMILY)
		{//家族
			lpDFamily = m_hashFamily.Find(lpHead->did);
			if(lpDFamily)
			{//内存信息在本机
				if(lpDFamily->isDirect == 1)
				{
					//如果来源是服务器,不需要解密直接投递到应用层
					if( lpHead->from == XT_HEAD::FROM_SERVER)
					{
						DirectPostData((RecvDataStruct *)pRecvData);
						return;
					}

					//目标与本机直连，则直接投递到应用层
					emMethod = MD_POSTAPP;
				}
				else
				{//目标不与本机直连，路由此包到相应服务器
					emMethod = MD_INSTROUTE;
					srvip = lpDFamily->srvip;
				}
			}
			else
			{//本机无目标群信息，路由此包，先缓存包，向全局查询目标信息
				emMethod = MD_DELAYROUTE;
			}
		}
		else if( lpHead->dtype == XT_HEAD::DEST_TYPE_SERVER )
		{
			if( lpHead->from == XT_HEAD::FROM_SERVER )
			{
				//对服务器的通知，直接投递到应用层
				emMethod = MD_POSTAPP;
				DirectPostData((RecvDataStruct *)pRecvData);
				return;
			}
			else
			{ 
				if( lpHead->did == m_nManSrvID )
				{
					emMethod = MD_POSTAPP;
				}
				else
				{
					ServerInfo* pInfo = m_ServerList.Find(lpHead->did);
					if(pInfo)
					{
						srvip = pInfo->ip;
						emMethod = MD_INSTROUTE;
					}
					else
					{
						//没有目的id信息,无法路由
						CLog::Log("HandleRecvRouteData",CLog::TYPE_ERROR,"没有目的id:%d服务器信息,无法路由",lpHead->did);
						return;
					}
					
				}
			}
		}


		if(lpSUser && lpSUser->isDirect)
		{
			//源信息在本机内存 且 直连客户端发来的包

			if(emMethod != MD_DIRECT2USER )
			{

				if( (lpHead->cmd == CMD_SEND_CPIC_REQ)	||
					(lpHead->cmd == CMD_GET_CPIC_ACK)	|| 
					(lpHead->cmd == CMD_ROUTERUSERPACKET_REQ) ||
					(lpHead->cmd == CMD_ROUTERUSERPACKET_ACK)
				  )
				{
					//图片数据不需要解密
				}
				else
				{
					//直连用户发来的包，则用源key解密，投递到应用层
					if(lpHead->flag != XT_HEAD::FLAG_MOBREQ)
					{
						RecvDataStruct* pRecvDataTemp =  (RecvDataStruct*)pRecvData;
						int tlen = 0;
						int ret = xTEADecryptWithKey(pRecvDataTemp->data + sizeof(XT_HEAD),pRecvDataTemp->data_len - sizeof(XT_HEAD),lpSUser->sessionKey,pRecvDataTemp->data + sizeof(XT_HEAD),&tlen);
						if(!ret)
						{
							CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"HandleRecvRouteData(%d) 解密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
								__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
							return ;
						}
						pRecvDataTemp->data_len = (uint16)tlen + sizeof(XT_HEAD);
					}
				}
			}

			switch(emMethod)
			{
			case MD_DIRECT2USER:
				{
					DirectTrans2User((RecvDataStruct *)pRecvData,lpDUser);
					break;
				}
			case MD_POSTAPP:    //直接投递到应用层
				{
					DirectPostData((RecvDataStruct *)pRecvData);
					break;
				}
			case MD_INSTROUTE:  //一次路由
				{
					InstRouteData(srvip,(RecvDataStruct *)pRecvData);
					break;
				}
			case MD_DELAYROUTE: //延时一次路由
				{
					DelayRouteData((RecvDataStruct *)pRecvData);
					break;
				}
			}
		}
		else
		{//源信息不在本机内存 或 包源不与本机直连，则一定是路由过来的包
			switch(emMethod)
			{
			case MD_DIRECT2USER:
				{
					DirectTrans2User((RecvDataStruct *)pRecvData,lpDUser);
					break;
				}
			case MD_POSTAPP:   //正确的路由包
				{
					DirectPostData((RecvDataStruct *)pRecvData,lpSUser);
					break;
				}
			case MD_INSTROUTE: //二次路由，需通知原路由者重定位目标信息
				{
					InstRouteData(srvip,(RecvDataStruct *)pRecvData);

					//通知原路由者重定位目标信息
					UpdateInfoNotify(pRecvData);
					break;

				}
			case MD_DELAYROUTE://延时二次路由，需通知原路由者重定位目标信息
				{
					DelayRouteData((RecvDataStruct *)pRecvData);

					//通知原路由者重定位目标信息
					UpdateInfoNotify(pRecvData);
					break;
				}
			}
		}

	}
	else
	{
		//无目标，则直接投递到应用层,无目标则不会产生路由，所以一定是直连客户端发来的包

		//解密数据
		if(lpSUser&&lpSUser->isDirect )
		{
			if( lpHead->cmd == CMD_SEND_CPIC_REQ )
			{

			}
			else
			{
				if(lpHead->flag != XT_HEAD::FLAG_MOBREQ)
				{
					RecvDataStruct* pRecvDataTemp =  (RecvDataStruct*)pRecvData;
					int tlen = 0;
					int ret = xTEADecryptWithKey(pRecvDataTemp->data + sizeof(XT_HEAD),pRecvDataTemp->data_len - sizeof(XT_HEAD),lpSUser->sessionKey,pRecvDataTemp->data + sizeof(XT_HEAD),&tlen);
					if(!ret)
					{
						CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"HandleRecvRouteData(%d) 解密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
							__LINE__,lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
						return ;
					}
					pRecvDataTemp->data_len = (uint16)tlen + sizeof(XT_HEAD);
				}
			}


			DirectPostData((RecvDataStruct *)pRecvData,lpSUser);
		}
		else
		{
			CLog::Log("CZAppNetwork::HandleRecvRouteData",CLog::TYPE_NET,"not direct user: sid:%d, cmd:0x%.4x ",
				lpHead->sid,lpHead->cmd);
		}
	}
}

/*
功能描述: 发送缓存的路由包(在收到查询应答后),为防止缓存的路由包未收到相应的查询应答而常驻内存
(采用一定时间过期机制,因为此队列理论比较短，且查询并不频繁，所以采用包触发机制处理过期包)
新的方案由应用网络层全自动维护非直连的用户信息，所以查询变得频繁，因此采用时钟机制检测过期
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::SendDelayRouteData(uint32 ackSeq,uint32 srvip)
{
	m_stLstRouteData.Lock();
	SmartList<ROUTEDATA>::iterator it = m_stLstRouteData.begin();
	while(it != m_stLstRouteData.end())
	{
		if((*it)->queryDataSeq == ackSeq)
		{
			InstRouteData(srvip,&((*it)->sendData));
			m_stLstRouteData.DeleteObject(*it);
			it = m_stLstRouteData.erase(it); 
			break;
		}
		++it;
	}
	m_stLstRouteData.UnLock();
}

/*
功能描述: 直接将需延时路有的包投递到应用层，多为直连用户发来的包，而目的不在线，
（如给好友留言， 用户登录一个未加载的群）
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::DirectPostDelayRouteData(uint32 ackSeq)
{
	m_stLstRouteData.Lock();
	SmartList<ROUTEDATA>::iterator it = m_stLstRouteData.begin();
	while(it != m_stLstRouteData.end())
	{
		if((*it)->queryDataSeq == ackSeq)
		{
			LPXT_HEAD lpHead = (LPXT_HEAD)((*it)->sendData.data);
			LPONLINE_USER_INFO lpSUser = m_hashUser.Find(lpHead->sid);
			m_recvDataDirectPostDelayRoute.src_ip = (*it)->origin_sip;
			m_recvDataDirectPostDelayRoute.port = (*it)->origin_sport;
			m_recvDataDirectPostDelayRoute.data_len = (*it)->sendData.data_len;
			memcpy(m_recvDataDirectPostDelayRoute.data,&((*it)->sendData.data),m_recvDataDirectPostDelayRoute.data_len);

			DirectPostData(&m_recvDataDirectPostDelayRoute,lpSUser);
			m_stLstRouteData.DeleteObject(*it);
			it = m_stLstRouteData.erase(it); 
			break;
		}
		++it;
	}
	m_stLstRouteData.UnLock();	
}

void CZAppNetwork::ReHandleDelayRouteData(uint32 ackSeq)
{
	m_stLstRouteData.Lock();
	SmartList<ROUTEDATA>::iterator it = m_stLstRouteData.begin();
	while(it != m_stLstRouteData.end())
	{
		if((*it)->queryDataSeq == ackSeq)
		{
			m_recvDataDirectPostDelayRoute.src_ip = (*it)->origin_sip;
			m_recvDataDirectPostDelayRoute.port = (*it)->origin_sport;
			m_recvDataDirectPostDelayRoute.data_len = (*it)->sendData.data_len;
			memcpy(m_recvDataDirectPostDelayRoute.data,&((*it)->sendData.data),m_recvDataDirectPostDelayRoute.data_len);

			HandleTransferData(&m_recvDataDirectPostDelayRoute);
			m_stLstRouteData.DeleteObject(*it);
			it = m_stLstRouteData.erase(it); 
			break;
		}
		++it;
	}
	m_stLstRouteData.UnLock();	
}

//删除延时路由包
void CZAppNetwork::DelDelayRouteData(uint32 ackSeq)
{
	m_stLstRouteData.Lock();
	SmartList<ROUTEDATA>::iterator it = m_stLstRouteData.begin();
	while(it != m_stLstRouteData.end())
	{
		if((*it)->queryDataSeq == ackSeq)
		{
			m_stLstRouteData.DeleteObject(*it);
			it = m_stLstRouteData.erase(it); 
			break;
		}
		++it;
	}
	m_stLstRouteData.UnLock();	
}

/*
功能描述: “发送查询目标”返回结果后，发送原缓存数据到目标所在服务器, 目标用户不存在，丢弃 或 重新投递到应用层
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::SendBufferSrvSendData(uint32 ackSeq,uint32 srvip)
{
	m_stLstSrvSendData.Lock();
	LPSRVSENDDATA lpSrvSendData = NULL;
	SmartList<SRVSENDDATA>::iterator it = m_stLstSrvSendData.begin();
	for(; it != m_stLstSrvSendData.end(); ++it)
	{
		if((*it)->queryDataSeq == ackSeq)
		{
			lpSrvSendData = *it;
			break;
		}
	}

	if(it == m_stLstSrvSendData.end())
	{
		m_stLstSrvSendData.UnLock();
		return ;
	}
	m_stLstSrvSendData.erase(it);
	m_stLstSrvSendData.UnLock();

	SendDataStruct & sendData = lpSrvSendData->sendData;
	LPXT_HEAD lpSHead = (LPXT_HEAD)sendData.data;
	if(srvip > 0)
	{
		sendData.dest_ip = srvip;
		sendData.port = PORT_IM_SVRP2P;
		//用服务器session key 加密 ???
		ImSvrp2p->SendDataTo(sendData);
	}
	else
	{
		if(lpSHead->flag == XT_HEAD::FLAG_SRVACTION || lpSHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
		{//重新投递到应用层
			RecvDataStruct recvData;
			recvData.data_len = sendData.data_len;
			memcpy(recvData.data,sendData.data,recvData.data_len);
			m_pfnUserCallback(m_pUserKey,NE_RECVDATA,&recvData);	
		}
	}

	m_stLstSrvSendData.DeleteObject(lpSrvSendData);
}

/*
功能描述: 处理全局服务器应答的数据，收到正确的应答后，需要将相应缓存的路由包路由出去
必要参数说明：
返 回 值：
*/ 
void CZAppNetwork::HandleCntRecvData(int nEvent,const RecvDataStruct * pRecvData)
{
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

	switch(nEvent)
	{
	case NE_RECVDATA:
		{

			int plain_len = 0;

			if( !xTEADecryptWithKey(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD),m_mineSessionKey,plainData,&plain_len))
			{
				XT_HEAD* lpHead = (XT_HEAD*)pRecvData->data;
				CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"HandleCntRecvData(%d) 解密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
					__LINE__, lpHead->sid,lpHead->from,lpHead->cmd,lpHead->did, lpHead->dtype);
				return;
			}

			uint32 now = timeGetTime();

			switch(lpRecvHead->cmd)
			{
			case CMDGSRV_USERCONTROLINFO_QUERY_ACK:
				{
					LPGSRV_USERCONTROLINFO_QUERY_ACK lpUserInfoAck = (LPGSRV_USERCONTROLINFO_QUERY_ACK)plainData;
					LPONLINE_USER_INFO lpUser = NULL;
					if(lpUserInfoAck->ret == 0 && lpUserInfoAck->userControlInfo[0].srvip > 0)
					{
						//建立用户内存信息
						lpUser = m_hashUser.AddHash(lpUserInfoAck->userControlInfo[0].uid);
						lpUser->uid		 = lpUserInfoAck->userControlInfo[0].uid;
						lpUser->isDirect = 0;
						lpUser->joinedTime = now;
						lpUser->srvip = lpUserInfoAck->userControlInfo[0].srvip;
						if(lpUser->srvip == m_listenIP)
							lpUser->isDirect = 1;
						memcpy(lpUser->p2pSessionKey,lpUserInfoAck->userControlInfo[0].p2pSessionKey,sizeof(lpUser->p2pSessionKey));
						memcpy(lpUser->sessionKey,lpUserInfoAck->userControlInfo[0].sessionKey,sizeof(lpUser->sessionKey));
						lpUser->flag = lpUserInfoAck->userControlInfo[0].flag;

						if( lpUser->isDirect == 0 )
						{
							m_lstIndirectUser.Lock();
							m_lstIndirectUser.push_back(lpUser); //非直连用户
							m_lstIndirectUser.UnLock();
						}
					}
					else if( lpUserInfoAck->ret == 0 && (lpUserInfoAck->userControlInfo[0].srvip == 0) )
					{
						m_hashUser.DelHash(lpUserInfoAck->userControlInfo[0].uid);
						CLog::Log("CZAppNetwork::HandleCntRecvData", CLog::TYPE_NET, "Not in globalserver，netlayer logout id:%u",lpUserInfoAck->userControlInfo[0].uid);
					}

					switch(pRecvData->vlink)
					{
					case QS_RECVQDEST: //接收查询目标
						{
							if(lpUserInfoAck->ret == 0 && lpUserInfoAck->userControlInfo[0].srvip > 0)
							{
								//发送缓存的路由包
								SendDelayRouteData(lpRecvHead->seq,lpUser->srvip);
							}
							else
							{//目标用户不再全局，用户不在线，直接投递到应用层
								DirectPostDelayRouteData(lpRecvHead->seq);
							}
							break;
						}
					case QS_SENDQDEST://发送查询目标
						{
							if(lpUserInfoAck->ret == 0 && lpUserInfoAck->userControlInfo[0].srvip > 0)
							{
								//(用服务器间key加密缓存数据 ???)，转发到目标所在的服务器
								SendBufferSrvSendData(lpRecvHead->seq,lpUser->srvip);
							}
							else
							{//目标用户不再全局，用户不在线，丢弃原缓存包  或 重新投递到应用层
								SendBufferSrvSendData(lpRecvHead->seq);
							}
							break;
						}
					}
					break;
				}
			case CMDGSRV_FAMILYINFO_QUERY_ACK:
				{
					LPGSRV_FAMILYINFO_QUERY_ACK lpFamilyInfoAck = (LPGSRV_FAMILYINFO_QUERY_ACK)plainData;
					LPONLINE_FAMILY_INFO lpFamily = NULL;
					if(lpFamilyInfoAck->ret == 0 && lpFamilyInfoAck->FamilyInfo[0].srvip > 0)
					{
						//建立家族内存信息
						lpFamily = m_hashFamily.AddHash(lpFamilyInfoAck->FamilyInfo[0].fid);
						m_lstIndirectFamily.Lock();
						m_lstIndirectFamily.push_back(lpFamily); //非直接登陆群
						m_lstIndirectFamily.UnLock();

						lpFamily->srvip = lpFamilyInfoAck->FamilyInfo[0].srvip;
						if(lpFamily->srvip == m_listenIP)
							lpFamily->isDirect = 1;
						else
							lpFamily->isDirect = 0;
					}
					switch(pRecvData->vlink)
					{
					case QS_RECVQDEST: //接收查询目标
						{
							if(lpFamilyInfoAck->ret == 0 && lpFamilyInfoAck->FamilyInfo[0].srvip > 0)
							{
								//发送缓存的路由包
								SendDelayRouteData(lpRecvHead->seq,lpFamily->srvip);
							}
							else
							{//目标群未加载，直接投递到应用层
								DirectPostDelayRouteData(lpRecvHead->seq);
							}
							break;
						}
					case QS_SENDQDEST://发送查询目标
						{
							if(lpFamilyInfoAck->ret == 0 && lpFamilyInfoAck->FamilyInfo[0].srvip > 0)
							{//目标是群，不用加密缓存数据，直接转发到目标所在的服务器
								SendBufferSrvSendData(lpRecvHead->seq,lpFamily->srvip);
							}
							else
							{//目标用户不再全局，用户不在线，丢弃原缓存包  或 重新投递到应用层
								SendBufferSrvSendData(lpRecvHead->seq);
							}
							break;
						}
					}
					break;
				}
			case CMDCS_UPDATELOCATION_NOTIFY: //更新目标定位信息通知
				{
					//向全局查询目标信息
					LPCS_UPDATELOCATION_NOTIFY lpNotify = (LPCS_UPDATELOCATION_NOTIFY)plainData;
					LocateInfoQuery(lpNotify->emDestType,lpNotify->destID,AddRequestSeq());
					break;
				}
			case CMDGSRV_USERRELOGIN_NOTIFY:	//有用户重登陆
				{

					RecvDataStruct tempRecv;
					memcpy((void*)&tempRecv,pRecvData,sizeof(RecvDataStruct) - MAX_NETWORK_PACKET + sizeof(XT_HEAD)  );
					memcpy((void*)(tempRecv.data + sizeof(XT_HEAD)) ,plainData,plain_len);
					tempRecv.data_len = (uint16)plain_len + sizeof(XT_HEAD);
					m_pfnUserCallback(m_pUserKey,NE_RECVDATA,&tempRecv);
				}
				break;
			case CMDGSRV_NOTIFY_TRANSFER_LOGIN://用户登录通知
				{
					LPONLINE_USER_INFO lpUser = NULL;
					CRecvPacket RecvPacket;
					GSRV_ADDMODIFY_FULLUSERINFO_QUERY Info;
					uint8 flag;

					RecvPacket.SetData(plainData,plain_len);
					RecvPacket>>Info.action
							  >>Info.count
							  >>Info.user[0].id
							  >>Info.user[0].status
							  >>Info.user[0].rule
							  >>Info.user[0].username
							  >>Info.user[0].nickname
							  >>Info.user[0].clientip
							  >>Info.user[0].clientport
							  >>Info.user[0].sexy
							  >>Info.user[0].serverip
							  >>Info.user[0].lanip
							  >>flag;
							  
					lpUser = m_hashUser.Find(Info.user[0].id);

					if(lpUser == NULL)
					{
						lpUser = m_hashUser.AddHash(Info.user[0].id);
						if(lpUser == NULL)
						{
							CLog::Log("CMDGSRV_NOTIFY_TRANSFER_LOGIN", CLog::TYPE_NORMAL, "New user:%d failed",Info.user[0].id);
							break;
						}
					}

					lpUser->uid = Info.user[0].id;
					lpUser->uip = Info.user[0].clientip;
					lpUser->uport = Info.user[0].clientport;
					lpUser->isDirect = 1;

					RecvPacket.ReadPacket(lpUser->sessionKey,sizeof(lpUser->sessionKey));
				}
				break;
			case CMDGSRV_NOTIFY_TRANSFER_LOGOUT://用户登出通知
				{	
					LPONLINE_USER_INFO lpUser = NULL;
					uint32 user_id = *(uint32*)(plainData + sizeof(uint8));

					lpUser = m_hashUser.Find(user_id);

					if(lpUser == NULL)
					{
						CLog::Log("CMDGSRV_NOTIFY_TRANSFER_LOGOUT", CLog::TYPE_NORMAL, "User %d not exist",user_id);
						break;
					}
					m_hashUser.DelHash(user_id);
					
				}
				break;
			case CMDGSRV_TUSERINFO_ACK://文件传输用户信息获取应答
				{
					switch(pRecvData->vlink)
					{
						case QS_TRANSFERDST:
						{
							LPONLINE_USER_INFO lpUser = NULL;
							GSRV_TUSERINFO_ACK Info;
							CRecvPacket RecvPacket;

							RecvPacket.SetData(plainData,plain_len);
							RecvPacket>>Info.ret
									  >>Info.id
									  >>Info.ip
									  >>Info.port;

							if(Info.ret == RESULT_SUCCESS)
							{
								lpUser = m_hashUser.Find(Info.id);

								if(lpUser == NULL)
								{
									lpUser = m_hashUser.AddHash(Info.id);
									if(lpUser == NULL)
									{
										CLog::Log("CMDGSRV_TUSERINFO_ACK", CLog::TYPE_ERROR, "New user:%d failed",Info.id);
										break;
									}
								}

								lpUser->uid = Info.id;
								lpUser->uip = Info.ip;
								lpUser->uport = Info.port;
								lpUser->isDirect = 1;
								
								RecvPacket.ReadPacket(lpUser->sessionKey,sizeof(lpUser->sessionKey));	

								DirectPostDelayRouteData(lpRecvHead->seq);
							}
							else
							{
								CLog::Log("CMDGSRV_TUSERINFO_ACK", CLog::TYPE_ERROR, "%d not found",Info.id);
								DelDelayRouteData(lpRecvHead->seq);
							}
						}
						break;
						case QS_TRANSFERSRC:
						{
							LPONLINE_USER_INFO lpUser = NULL;
							GSRV_TUSERINFO_ACK Info;
							CRecvPacket RecvPacket;
							
							RecvPacket.SetData(plainData,plain_len);
							RecvPacket>>Info.ret
									  >>Info.id
									  >>Info.ip
									  >>Info.port;

							if(Info.ret == RESULT_SUCCESS)
							{
								lpUser = m_hashUser.Find(Info.id);

								if(lpUser == NULL)
								{
									lpUser = m_hashUser.AddHash(Info.id);
									if(lpUser == NULL)
									{
										CLog::Log("CMDGSRV_TUSERINFO_ACK", CLog::TYPE_ERROR, "New user:%d failed",Info.id);
										break;
									}
								}

								lpUser->uid = Info.id;
								lpUser->uip = Info.ip;
								lpUser->uport = Info.port;
								lpUser->isDirect = 1;
								
								RecvPacket.ReadPacket(lpUser->sessionKey,sizeof(lpUser->sessionKey));

								ReHandleDelayRouteData(lpRecvHead->seq);
							}
							else
							{
								CLog::Log("CMDGSRV_TUSERINFO_ACK", CLog::TYPE_ERROR, "%d not found",Info.id);
								DelDelayRouteData(lpRecvHead->seq);
							}
						}
						break;
					}
				}
				break;
			}
		}
		break;
	case NE_SENDFAILED:
		break;
	case NE_LISTENSTOP:
		break;
	}
}

void CZAppNetwork::ImSvrP2PRecvData(int nEvent,const RecvDataStruct * pRecvData)
{
	CZSignalQueue* m_RecvQueue;
	CZUdpCommExt* m_UdpComm;
	m_UdpComm = (CZUdpCommExt*)(m_pUdpSrvNetTrans->GetUdpComm());
	m_RecvQueue = m_UdpComm->GetRecvQueue();
	switch(nEvent)
	{
		case NE_RECVDATA:
				m_RecvQueue->Push((const char * )pRecvData,sizeof(RecvDataStruct) - sizeof(pRecvData->data) + pRecvData->data_len);
				printf("ImSvrP2PRecvData push data\n");
			break;
		case NE_SENDFAILED:
			break;
		case NE_LISTENSTOP:
			break; 
	}
}

LPONLINE_USER_INFO CZAppNetwork::FindOnlineUser(uint32 uid)
{
	return m_hashUser.Find(uid);
}

/*
功能描述: 用户登陆
必要参数说明：uid：用户ID
返 回 值： 0 成功
*/ 
int CZAppNetwork::UserLogin(uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey/* = NULL*/,const char * pP2PSessionKey/* = NULL*/,uint32 lanip /* = 0*/,uint16 lanport /* = 0*/,uint8 flag)
{
	int nRet = 0;
	LPONLINE_USER_INFO lpUser = NULL;
	lpUser = m_hashUser.AddHash(uid);

	lpUser->isDirect = 1;
	lpUser->joinedTime = timeGetTime();
	if(pP2PSessionKey)
		memcpy(lpUser->p2pSessionKey,pP2PSessionKey,sizeof(lpUser->p2pSessionKey));
	if(pSessionKey)
		memcpy(lpUser->sessionKey,pSessionKey,sizeof(lpUser->sessionKey));
	lpUser->srvip = m_listenIP;
	lpUser->uid = uid;
	lpUser->uip = uip;
	lpUser->uport = uport;
	lpUser->flag = flag;

	return nRet;
}

void CZAppNetwork::UserLogout(uint32 uid)
{
	m_hashUser.DelHash(uid);
}

void CZAppNetwork::FamilyLogin(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	LPONLINE_FAMILY_INFO lpFamily = m_hashFamily.AddHash(fid);
	lpFamily->fid = fid;
	lpFamily->ftype = GetFamilyType(fid);
	lpFamily->isDirect = 1;
	lpFamily->joinedTime = timeGetTime();
	lpFamily->srvip = m_listenIP;
}

void CZAppNetwork::FamilyLogout(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	m_hashFamily.DelHash(fid);
}

int CZAppNetwork::StartWork(const char * addr,uint16 port/* = 0*/)
{
	CConfigInfo	configInfo;			//配置信息
	//从配置文件读取服务器编号
	m_globalUserIP = inet_addr(configInfo.GetOnlineUserServerIP());
	m_globalUserPort = PORT_ONLINE_USER_MANAGER;
	m_globalFamilyIP = inet_addr(configInfo.GetOnlineFamilyServerIP());
	m_globalFamilyPort = PORT_ONLINE_CLUSTER_MANAGER;
	m_nManSrvID = 0;configInfo.GetServerNo();



	//内存信息过期时钟
	m_listenIP = inet_addr(addr);
	m_listenPort = port;
	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,ZAppNetworkMyTimeProc,(void*)this,TIME_PERIODIC);

	m_pUdpSrvNetTrans->SetUserCallback(this,UdpSrvNetTransCallback);
	int nRet = m_pUdpSrvNetTrans->StartListen(addr,m_listenPort);
	if(nRet < 0)
		return -1;

	m_listenIP = inet_addr(configInfo.GetinListenAddr());
//	m_listenPort = m_pUdpSrvNetTrans->GetPort();

	m_pUdpCntNetTrans->SetUserCallback(this,UdpCntNetTransCallback);

	if (port == PORT_IM_BUSINESS || port == PORT_TRANSPORT_SERVER) //做为IM服务器需要通过PORT_APPNET2GLOBAL监听全局通知
	{
		m_nManSrvID = configInfo.GetServerNo();
		addr=configInfo.GetinListenAddr();
		if(port == PORT_IM_BUSINESS)
			nRet = m_pUdpCntNetTrans->StartListen(addr,PORT_APPNET2GLOBAL);
		else
			nRet = m_pUdpCntNetTrans->StartListen(addr,PORT_TRANS2GLOBAL);
		if(nRet < 0)
			return -1;
	}
	else if (port == PORT_NOTIFY_BUSINESS) //做为通知服务器需要通过PORT_NOTIFY2GLOBAL监听全局通知
	{
			m_nManSrvID = configInfo.GetServerNo();
			addr=configInfo.GetinListenAddr();
			nRet = m_pUdpCntNetTrans->StartListen(addr,PORT_NOTIFY2GLOBAL);
			if(nRet < 0)
				return -1;
	}

	m_pUdpSrvNetTrans->SetServerID(m_nManSrvID);

	//业务服务器之间通讯
	if(port == PORT_IM_BUSINESS)
	{
		ImSvrp2p->SetUserCallback(this,ImSvrP2PCallback);
		m_nManSrvID = configInfo.GetServerNo();
		addr=configInfo.GetinListenAddr();
		nRet = ImSvrp2p->StartListen(addr,PORT_IM_SVRP2P);
		if(nRet < 0)
			return -1;
#ifdef WEBIM_TCP
		ImSvrp2p->StartTcpWork();
#endif
	}
//	else if(port == PORT_NOTIFY_BUSINESS)
//	{
//		ImSvrp2p->SetUserCallback(this,ImSvrP2PCallback);
//		m_nManSrvID = configInfo.GetServerNo();
//		addr=configInfo.GetinListenAddr();
//		nRet = ImSvrp2p->StartListen(addr,PORT_WEB_NOTIFY);
//		if(nRet < 0)
//			return -1;
//	}

	return 0;
}

//服务器通过应用网络层主动发送数据，自动加密 , 向群主动发送的数据 (群成员登陆通知群)
int CZAppNetwork::SendDataToA(const char * data,int size,int retrys/* = 0*/,uint16 interval /* = 0*/ ,uint32 ip ,uint16 port,uint32 vlink)
{
	//由应用层主动发起的包，此一定为应答包或通知包
	SendDataStruct sendData;
	memcpy(sendData.data,data,size);
	sendData.data_len = size;
	sendData.retry_times = retrys;
	sendData.tick_count = 0;
	sendData.time_interval = interval;
	sendData.vlink = vlink;
	XT_HEAD* pHead = (XT_HEAD*)sendData.data;
	
	bool bDestExist = true;
	if(pHead->dtype == XT_HEAD::DEST_TYPE_FAMILY)
	{
		LPONLINE_FAMILY_INFO lpFamily = m_hashFamily.Find(pHead->did);
		if( lpFamily )
		{
			sendData.dest_ip = lpFamily->srvip;
			sendData.port = PORT_IM_SVRP2P;

			//用服务器session key 加密 ???
			return ImSvrp2p->SendDataTo(sendData);
		}
		else
		{
			if( ip != 0 )
			{
				sendData.dest_ip = ip;
				sendData.port = port;
				return ImSvrp2p->SendDataTo(sendData);
			}

			bDestExist = false;
		}
	}
	else if(pHead->dtype == XT_HEAD::DEST_TYPE_USER )
	{

		LPONLINE_USER_INFO lpUser = m_hashUser.Find(pHead->did);
		if(lpUser)
		{
			if(lpUser->isDirect == 1 && lpUser->uip>0 )
			{//直连用户，则直接发给客户端
				sendData.dest_ip = lpUser->uip;
				sendData.port = lpUser->uport;

				if( pHead->cmd == CMD_GET_CPIC_ACK ||
					pHead->cmd == CMD_ROUTERUSERPACKET_REQ ||
					pHead->cmd == CMD_ROUTERUSERPACKET_ACK
				  )
				{
					//图片数据不需要加密
				}
				else
				{
					if(lpUser->flag != XT_HEAD::FLAG_MOBREQ)
					{
						if(pHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
							pHead->flag = XT_HEAD::FLAG_SRVACTION;
						else if(pHead->flag == XT_HEAD::FLAG_MOBSRVACK)
							pHead->flag = XT_HEAD::FLAG_SRVACK;
						//加密数据再放入发送队列 
						int tlen = 0;
						if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),lpUser->sessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
						{
							CLog::Log("CZAppNetwork",CLog::TYPE_ERROR,"SendDataToA(%d) 加密失败:sid:%d, from:%d, cmd:0x%.4x, did:%d, dtype:%d",
								__LINE__, pHead->sid,pHead->from,pHead->cmd,pHead->did, pHead->dtype);
							return -1;
						}
						sendData.data_len = (uint16)tlen +  sizeof(XT_HEAD);
					}
					else
					{					
						if(pHead->flag == XT_HEAD::FLAG_SRVACTION)
							pHead->flag = XT_HEAD::FLAG_MOBSRVACTION;
						else if(pHead->flag == XT_HEAD::FLAG_SRVACK)
							pHead->flag = XT_HEAD::FLAG_MOBSRVACK;
					}
				}
			}
			else
			{//非直连用户，发到用户所在的业务服务器
				sendData.dest_ip = lpUser->srvip;
				sendData.port = PORT_IM_SVRP2P;
				
				return ImSvrp2p->SendDataTo(sendData);
				//用服务器session key 加密 ???
			}

			return m_pUdpSrvNetTrans->SendDataTo(sendData);		 
		}
		else
		{
			//有ip则直接发送
			if( ip != 0 )
			{
				sendData.dest_ip = ip;
				sendData.port = port;
				return m_pUdpSrvNetTrans->SendDataTo(sendData);
			}

			//目标用户信息不存在,需要去查全局,多为通知包，如加好友结果通知
			if(pHead->cmd != CMD_FILE_TRANS_ACK || pHead->cmd != CMD_FILE_TRANS_IND_ACK)
				bDestExist = false;
		}
	}
	else if( pHead->dtype == XT_HEAD::DEST_TYPE_SERVER)
	{
		//有ip则直接发送,否则失败
		if( ip != 0 )
		{
			sendData.dest_ip = ip;
			sendData.port = port;

			return m_pUdpSrvNetTrans->SendDataTo(sendData);
		}

		//m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(Recev)&sendData);
		return RESULT_FAILED;
	}

	if(false == bDestExist)
	{
		LPSRVSENDDATA lpSrvSendData = m_stLstSrvSendData.NewObject();
		memcpy(&lpSrvSendData->sendData,&sendData,sizeof(lpSrvSendData->sendData) - MAX_NETWORK_PACKET + sendData.data_len);
		lpSrvSendData->joinTime = timeGetTime();
		lpSrvSendData->queryDataSeq = AddRequestSeq();
		lpSrvSendData->did = pHead->did;
		lpSrvSendData->dtype = pHead->dtype;
		m_stLstSrvSendData.Lock();
		m_stLstSrvSendData.push_back(lpSrvSendData);
		m_stLstSrvSendData.UnLock();

		//向全局查询目标定位新息
		LocateInfoQuery(lpSrvSendData->dtype,lpSrvSendData->did,lpSrvSendData->queryDataSeq,QS_SENDQDEST);
	}

	return 0;
}

//直接通过传输层发送数据
int CZAppNetwork::SendDataToA(SendDataStruct & sendData)
{
	//直接通过传输层发送数据，加解密由应用层自行处理
	return m_pUdpSrvNetTrans->SendDataTo(sendData);
}
void CZAppNetwork::StopWork()
{
	if(m_hMMT)
	{
		timeKillEvent(m_hMMT); m_hMMT = NULL;
		m_pUdpSrvNetTrans->StopListen();
	}
}


void CZAppNetwork::AddImServer(uint32 srvid,uint32 srvip)
{
	ServerInfo* pInfo = m_ServerList.AddHash(srvid);
	pInfo->id = srvid;
	pInfo->ip = srvip;
}
void CZAppNetwork::DelImServer(uint32 srvid)
{
	m_ServerList.DelHash(srvid);
}

int CZAppNetwork::UserType(uint32 id)
{	
	//int nRet = 0;
	LPONLINE_USER_INFO lpUser = NULL;
	lpUser = m_hashUser.Find(id);

	if(lpUser == NULL)
		return TYPE_NULL;
	else
	{
		if(lpUser->flag == XT_HEAD::FLAG_MOBREQ)
			return TYPE_WEB;
		else
			return TYPE_CLIENT;
	}
}
