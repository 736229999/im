/************************************************************************ 
 文件描述：应用网络层实现
     对业务服务器的集群实现请求包的透明路由，管理维护服务器的各通讯链路
 
 创建人： zouyf, 2006-6-15
 
 修改记录： 
************************************************************************/
#pragma once
#include "../../common/Utility.h"
#include "../../include/PublicType.h"
#include "../../common/HashTable.h"
#include "../../include/MiniNetTrans.h"
#include "../../include/SrvFrameProtocol.h"
#include "../../include/MiniAppNetwork.h"
#include "../Mininettrans/ZUdpCommExt.h"
#include <vector>

class CGlobalBusi;

//缓存需要向全局查询目标位置的请求包，等待全局应答数据时再路由  “接收查询目标”
typedef struct tagROUTEDATA
{
	SendDataStruct sendData;
	uint32  origin_sip;     //路有包原始来源ip
	uint16  origin_sport;   //路有包原始来源port
	uint8 emDestType;
	uint32      destID;
	uint32      queryDataSeq; //查询包seq，避免重复查询
	uint32 joinTime; //加入的时刻
	tagROUTEDATA()
	{
		memset(this,0,sizeof(tagROUTEDATA));
		joinTime = timeGetTime();
	}
}ROUTEDATA, * LPROUTEDATA;

//缓存将投递到应用层的接收包   “接收查询源”
typedef struct tagRECVPOSTDATA
{
	RecvDataStruct recvData;
	uint32         queryDataSeq;
	uint32 joinTime; //加入的时刻
}RECVPOSTDATA, * LPRECVPOSTDATA;

//缓存服务器将发送的包          “发送查询目标”
typedef struct tagSRVSENDDATA
{
	SendDataStruct sendData;
	uint32         did;
	uint8          dtype;
	uint32         queryDataSeq;
	uint32 joinTime; //加入的时刻
}SRVSENDDATA, * LPSRVSENDDATA;

class CZAppNetwork
{
public:
	CZAppNetwork(void);
	~CZAppNetwork(void);
	void SetUserCallback(void * pUserKey,pfnAppNetCallback pfnCallback)
	{
		m_pUserKey = pUserKey;
		m_pfnUserCallback = pfnCallback;
	}
	void SetUserCallback(FunRecvNetData funRecvNetData)
	{
		if(m_pUdpSrvNetTrans != NULL)
		{
			m_pUdpSrvNetTrans->SetUserCallback(funRecvNetData);
		}
//#ifdef ACCESS_SERVER		
//		if(ImSvrp2p != NULL)
//		{	
//			ImSvrp2p->SetUserCallback(funRecvNetData);
//		}
//#endif		
	}

	void SetCallback_SendNetData(FunSendNetData pFunSendNetData)
	{
		m_pFunSendNetData = pFunSendNetData;
	}

	int StartWork(const char * addr,uint16 port = 0);

	void StartTcpWork();

//#ifdef ACCESS_SERVER
//	shared_ptr<ONLINE_USER_INFO> FindOnlineUser(CGlobalBusi *pGlobalBusi, uint32 uid);
//	// 用户遍历hash列表的接口
//	// 获取在线用户hash列表的hash分支数
//	int GetUserHash_Elems();
//	// 获取在线用户hash列表的hash分支下面的索引数(这里以用户id为索引)
//	void GetUserHash_ElemIDs(int nElemIdex, std::vector<uint32> &vecID);
//#else
	BOOL FindOnlineUser(CGlobalBusi *pGlobalBusi, uint32 uid);
//#endif
#ifdef ACCESS_SERVER	
	int UserLogin(CGlobalBusi *pGlobalBusi, uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey = NULL,const char * pP2PSessionKey = NULL,uint32 lanip = 0,uint16 lanport = 0,uint8 flag = XT_HEAD::FLAG_CNTREQUEST);
	void UserLogout(uint32 uid);
#endif
	void FamilyLogin(uint32 fid,uint8 ftype = PERM_FAMILY);
	void FamilyLogout(uint32 fid,uint8 ftype = PERM_FAMILY);

	int SendDataToA(CGlobalBusi *pGlobalBusi, const char * data,int size,int retrys = 0,uint16 interval = 0,uint32 ip = 0,uint16 port = 0,uint32 vlink = 0);
	int SendDataToA(CGlobalBusi *pGlobalBusi, SendDataStruct & sendData);
	void StopWork();
	uint16 GetPort() {return m_listenPort;}
	uint32 GetLocalIP() {return m_listenIP;}

	CUdpNetTrans* GetSrvNetTrans() { return m_pUdpSrvNetTrans; }
/*	
	CUdpNetTrans* GetCntNetTrans() { return m_pUdpCntNetTrans; }
*/

	void AddImServer(uint32 srvid,uint32 srvip);
	void DelImServer(uint32 srvid);

	//检查用户类型
	int UserType(CGlobalBusi *pGlobalBusi, uint32 id);

	void ImSvrP2PRecvData(int nEvent,const RecvDataStruct * pRecvData);

private:
	friend void  ZAppNetworkMyTimeProc(void* dwUser);
	friend void  ZAppNetworkMyTimeProcNew(union sigval v);
	friend void UdpSrvNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
/*	
	friend void UdpCntNetTransCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
*/	
	friend void ImSvrP2PCallback(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);

	void HandleSrvRecvData(int nEvent,const RecvDataStruct * pRecvData);
	void HandleRecvRouteData(const RecvDataStruct * pRecvData);
/*	
	void HandleCntRecvData(int nEvent,const RecvDataStruct * pRecvData);
*/	
	void HandleOnlineExpire();
/*	文件传输功能暂时屏蔽	孙丁	
	void HandleTransferData(const RecvDataStruct * pRecvData);
*/
	void DirectTrans2User(const RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpDUser);

	//m_stLstRouteData
/*	
	void InstRouteData(uint32 srvip,RecvDataStruct * pRecvData);
	void InstRouteData(uint32 srvip,SendDataStruct * pSendData);
*/
/*
	void DelayRouteData(RecvDataStruct * pRecvData);
*/	
	void DirectPostData(RecvDataStruct * pRecvData,const LPONLINE_USER_INFO lpSUser = NULL);
/*	
	void SendDelayRouteData(uint32 ackSeq,uint32 srvip);
*/	
	void DirectPostDelayRouteData(uint32 ackSeq);
/*	
	void QueryData(RecvDataStruct * pRecvData,uint8 flag);
*/	
/*
	void DelDelayRouteData(uint32 ackSeq);
	void ReHandleDelayRouteData(uint32 ackSeq);
*/
	//m_stLstRecvPostData
	void PostBufferRecvData(uint32 ackSeq,const LPONLINE_USER_INFO lpSUser = NULL);
/*
	//m_stLstSrvSendData
	void SendBufferSrvSendData(uint32 ackSeq,uint32 srvip = 0);
*/
	void UpdateInfoNotify(const RecvDataStruct * pRecvData);
	//void LocateInfoQuery(uint8 dtype,uint32 did,uint32 seq,uint8 vlink = 0);

	inline void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = 0xF2;
		lpHead->from = 0x0200;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = m_nManSrvID;
	}
	
	inline void InitServerHead(LPXT_HEAD lpHead,uint16 cmd,uint32 seq,uint16 len,uint8 dtype = XT_HEAD::DEST_TYPE_SERVER,uint32 did = 0)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = 0xF2;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = seq;
		lpHead->sid = m_nManSrvID;
	}

	uint32 AddRequestSeq()
	{
		CMyLock MyLock(&m_seqLock);
		return ++m_nCntREQSeq;
	}

	uint8 GetFamilyType(uint32 fid)
	{
		if(fid & 0x80000000)
			return TEMP_FAMILY;
		else
			return PERM_FAMILY;
	}
private:
//#ifdef ACCESS_SERVER	// 只有接入服务器中记录在线用户信息	
//	CHashTable<ONLINE_USER_INFO> m_hashUser;     //在线用户定位信息Hash
//#endif	
	CHashTable<ONLINE_FAMILY_INFO> m_hashFamily; //登陆群定位信息Hash
//#ifdef ACCESS_SERVER
//	SafeList<LPONLINE_USER_INFO> m_lstIndirectUser;     //自动维护的非直连用户列表，因为好友、家族成员等非直连用户是由应用层维护的
//#endif	
	SafeList<LPONLINE_FAMILY_INFO> m_lstIndirectFamily; //自动维护的非直接登陆群列表

	PMMRESULT m_hMMT;
	timer_t m_timer;

	uint32 m_listenIP;
	uint16 m_listenPort;
	//不能合并为一个端口，因为“服务器udp监听”被是被应用层控制的，应用层也要用来访问全局
	//接管的服务器udp，只做透明路由，不自己组包，所有不用知道seq
	CUdpNetTrans * m_pUdpSrvNetTrans;  //服务器udp监听
	
	//与全局交互 
	uint32         m_globalUserIP;
	uint16         m_globalUserPort;
	uint32         m_globalFamilyIP;
	uint16         m_globalFamilyPort;

	char           m_globalSessionKey[16];  //全局服务器session key
	char           m_mineSessionKey[16];    //自己的 session key
	unsigned	int         m_nManSrvID;       //自己的服务器ID，本机手工分配的服务器编号
	
	volatile uint32 m_nCntREQSeq;       //向全局请求包的seq
	CRITICAL_SECTION m_seqLock;         //seq锁
	//CUdpNetTrans * m_pUdpCntNetTrans;  //客户端udp，只与全局交互，为应用网络层私有
	
	//应用层回调函数
	void * m_pUserKey;
	pfnAppNetCallback m_pfnUserCallback;

	//缓存需要向全局查询目标位置的请求包，等待全局应答数据时再路由,用时钟来监测包是否应答过期
	//查询变得频繁，因此对速度要求提高了，采用类似网络传输层超时重发机制，一次监测100缓存包
	SmartList<ROUTEDATA>    m_stLstRouteData;
	SmartList<SRVSENDDATA>  m_stLstSrvSendData;   //缓存服务器将发送的包          “发送查询目标”

	//对网络接收包的四种处理方式
	enum EM_METHOD
	{
		MD_POSTAPP,    //投递到应用层
		MD_INSTROUTE,  //及时路由
		MD_DELAYROUTE, //延时路由，先缓存，待查询定位后再路由 或 投递到应用层处理
		MD_DIRECT2USER //直接转发到用户
	};

	
	//向全局查询用户或群控制信息的四种状态
	enum EM_QUERYSTATUS
	{
		QS_RECVQDEST = 1,  //接收查询目标
		QS_RECVQSOURCE = 2,//接收查询源
		QS_SENDQDEST = 3,  //发送查询目标
		QS_SENDQSOURCE  = 4,//发送查询源
		QS_TRANSFERSRC = 5,//文件传输源查询
		QS_TRANSFERDST = 6//文件传输目的查询
	};


	RecvDataStruct m_recvDataDirectPostDelayRoute;

	char plainData[MAX_NETWORK_PACKET];

	struct ServerInfo
	{
		uint32 id;
		uint32 ip;
	};

	CHashTable<ServerInfo>		m_ServerList;		//其他IM服务器的信息
//#ifdef ACCESS_SERVER	// 只有接入服务器中用到，沿用其是为了接收webim发过来的数据 孙丁
//	CUdpNetTrans* ImSvrp2p;//业务服务器之间通信
//#endif
	const unsigned int m_nTimerDelay = 1000 * 60 * 1;      		//Delay : 1分钟
	const unsigned int m_nTimerResolution = 1000 * 60 * 1; 		//Resolution : 1分钟

	FunSendNetData	m_pFunSendNetData;
};
