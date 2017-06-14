// MiniAppNetwork.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "./ZAppNetwork.h"
#include "../../include/MiniAppNetwork.h"
#include "GlobalBusi.h"
#include <vector>

class CGlobalBusi;

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}

// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 MiniAppNetwork.h
CMiniAppNetwork::CMiniAppNetwork()
{ 
	m_pAppNetwork = new CZAppNetwork();
}

CMiniAppNetwork::~CMiniAppNetwork()
{
	delete (CZAppNetwork*)m_pAppNetwork; m_pAppNetwork = NULL;
}

/*
功能描述: 设置用户回调参数和回调函数
必要参数说明：pUserKey：用户回调参数
				pfnCallback：用户回调函数
返 回 值：
*/ 
void CMiniAppNetwork::SetUserCallback(void * pUserKey,pfnAppNetCallback pfnCallback)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->SetUserCallback(pUserKey,pfnCallback);
}

void CMiniAppNetwork::SetUserCallback(FunRecvNetData funRecvNetData)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->SetUserCallback(funRecvNetData);	
}

void CMiniAppNetwork::SetCallback_SendNetData(FunSendNetData pFunSendNetData)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->SetCallback_SendNetData(pFunSendNetData);	
}

/*
功能描述: 启动应用网络层服务
必要参数说明：addr：IP 地址
				prot：udp监听端口
返 回 值：0 成功，< 0 失败
*/ 
int CMiniAppNetwork::StartWork(const char * addr,uint16 port)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->StartWork(addr,port);
}

#ifdef ACCESS_SERVER
LPONLINE_USER_INFO CMiniAppNetwork::FindOnlineUser(uint32 uid)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->FindOnlineUser(uid);
}

// 获取在线用户hash列表的hash分支数
int CMiniAppNetwork::GetUserHash_Elems()
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->GetUserHash_Elems();
}

// 获取在线用户hash列表的hash分支下面的索引数(这里以用户id为索引)
void CMiniAppNetwork::GetUserHash_ElemIDs(int nElemIdex, std::vector<uint32> &vecID)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->GetUserHash_ElemIDs(nElemIdex, vecID);
}
#else
BOOL CMiniAppNetwork::FindOnlineUser(CGlobalBusi *pGlobalBusi, uint32 uid)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->FindOnlineUser(pGlobalBusi, uid);
}
#endif

/*
功能描述: 用户登陆
必要参数说明：uid：用户ID
返 回 值：
*/ 
#ifdef ACCESS_SERVER
int CMiniAppNetwork::UserLogin(CGlobalBusi *pGlobalBusi, uint32 uid,uint32 uip,uint16 uport,const char * pSessionKey ,const char * pP2PSessionKey,uint32 lanip,uint16 lanport, uint8 flag)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->UserLogin(pGlobalBusi, uid,uip,uport,pSessionKey,pP2PSessionKey,lanip,lanport,flag);
}

void CMiniAppNetwork::UserLogout(uint32 uid)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->UserLogout(uid);
}
#endif
/*
功能描述: 家族登陆
必要参数说明：fid：家族ID
返 回 值：
*/ 
void CMiniAppNetwork::FamilyLogin(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->FamilyLogin(fid);
}
void CMiniAppNetwork::FamilyLogout(uint32 fid,uint8 ftype/* = PERM_FAMILY*/)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->FamilyLogout(fid);
}


/*
功能描述: 发送数据
必要参数说明：uid ：目标用户
				data：发送的数据
				size：数据长度
				retrys：重发次数
				interval：重发间隔
返 回 值：0 成功，< 0 失败
*/ 
int CMiniAppNetwork::SendDataTo(CGlobalBusi *pGlobalBusi, const char * data,int size,int retrys ,uint16 interval,uint32 ip ,uint16 port,uint32 vlink)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->SendDataToA(pGlobalBusi, data,size,retrys,interval,ip,port,vlink);
}

//直接通过传输层发送数据
int CMiniAppNetwork::SendDataTo(CGlobalBusi *pGlobalBusi, SendDataStruct & sendData)
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->SendDataToA(pGlobalBusi, sendData);	
}

void CMiniAppNetwork::StopWork()
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	pAppNetwork->StopWork();
}

uint16 CMiniAppNetwork::GetPort() 
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->GetPort();
}
uint32 CMiniAppNetwork::GetLocalIP()
{
	CZAppNetwork * pAppNetwork = (CZAppNetwork *)m_pAppNetwork;
	return pAppNetwork->GetLocalIP();
}

CUdpNetTrans*  CMiniAppNetwork::GetSrvNetTrans()
{
	return ((CZAppNetwork *)m_pAppNetwork)->GetSrvNetTrans();
}
/*
CUdpNetTrans*  CMiniAppNetwork::GetCntNetTrans()
{
	return ((CZAppNetwork *)m_pAppNetwork)->GetCntNetTrans();
}
*/
void CMiniAppNetwork::AddImServer(uint32 srvid,uint32 srvip)
{
	((CZAppNetwork *)m_pAppNetwork)->AddImServer(srvid,srvip);
}

void CMiniAppNetwork::DelImServer(uint32 srvid)
{
	((CZAppNetwork *)m_pAppNetwork)->DelImServer(srvid);
}

int CMiniAppNetwork::UserType(CGlobalBusi *pGlobalBusi, uint32 id)
{
	return ((CZAppNetwork *)m_pAppNetwork)->UserType(pGlobalBusi,id);
}

void CMiniAppNetwork::ImSvrP2PRecvData(int nEvent,const RecvDataStruct * pRecvData)
{
	((CZAppNetwork *)m_pAppNetwork)->ImSvrP2PRecvData(nEvent, pRecvData);
}