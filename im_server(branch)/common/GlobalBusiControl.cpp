#include "stdafx.h"
#include "Function.h"
#include "GlobalBusiControl.h"
#include "GlobalBusi.h"
#include "ConfigInfo.h"


GlobalBusiControl::GlobalBusiControl(uint32 globalSrvIP,uint16 globalSrvPort)
{
	printf("GlobalBusiControl::GlobalBusiControl\n");
	m_vLink = 0;
	srand(timeGetTime());
	m_seq = (rand()<<16)|rand();
	

	CConfigInfo Config;
	m_nManSrvID = Config.GetServerNo();

	m_globalSrvIP = globalSrvIP;
	m_globalSrvPort = globalSrvPort;
	memset(m_globalSessionKey,0,sizeof(m_globalSessionKey));
	memset(m_mineSessionKey,0,sizeof(m_mineSessionKey));
}


GlobalBusiControl::GlobalBusiControl(list<uint32> globalSrvIPs,uint16 globalSrvPort)
{
	printf("GlobalBusiControl::GlobalBusiControl\n");
	m_vLink = 0;
	srand(timeGetTime());
	m_seq = (rand()<<16)|rand();
	

	CConfigInfo Config;
	m_nManSrvID = Config.GetServerNo();

	m_globalSrvIPs =  globalSrvIPs;
	m_globalSrvPort = globalSrvPort;
	memset(m_globalSessionKey,0,sizeof(m_globalSessionKey));
	memset(m_mineSessionKey,0,sizeof(m_mineSessionKey));
}
GlobalBusiControl::~GlobalBusiControl(void)
{
	m_mapGlobalBusi.clear();
}

void GlobalBusiControl::RegisterLink(uint32 vlink,CGlobalBusi * pGlobalBusi)
{
	map<uint32,CGlobalBusi *>::iterator it;
	it = m_mapGlobalBusi.find(vlink);
	if(it != m_mapGlobalBusi.end())
		return ;

	m_mapGlobalBusi.insert(pair<uint32,CGlobalBusi *>(vlink,pGlobalBusi));
}




void GlobalBusiControl::SendAckData(SendDataStruct & sendData,uint32 dstIp)
{
	sendData.dest_ip = dstIp;
	sendData.port = m_globalSrvPort;
	sendData.retry_times = SEND_RETRY_TIME;
	//加密
	int tlen = 0;
	if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),m_globalSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
		return ;
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	SendDataTo(sendData);

}


void GlobalBusiControl::SendMultiRequestData(SendDataStruct & sendData)
{
	sendData.port = PORT_IM_SVRP2P;
	sendData.retry_times = 0;// SEND_RETRY_TIME;

	
	CLog::Log("CGlobalBusi::SendMultiRequestData",CLog::TYPE_IMPORTANT,"vlink %d",sendData.vlink);
	
	//加密
//	int tlen = 0;
//	if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),m_globalSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
//		return ;
//	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);	 
	for(list<uint32>::iterator ServerIplist_cur = m_globalSrvIPs.begin();ServerIplist_cur != m_globalSrvIPs.end();ServerIplist_cur++)
	{		
		if(*ServerIplist_cur == m_nManSrvID)
		{			
			CLog::Log("CGlobalBusi::SendMultiRequestData",CLog::TYPE_DEBUG,"skip my self %u",m_nManSrvID);
			continue;
		}		
		sendData.dest_ip = *ServerIplist_cur;
		CLog::Log("CGlobalBusi::SendMultiRequestData",CLog::TYPE_IMPORTANT,"sendto %u",sendData.dest_ip);
		SendDataTo(sendData);
	}
}

void GlobalBusiControl::SendRequestData(SendDataStruct & sendData)
{	
	sendData.dest_ip = m_globalSrvIP;
	sendData.port = m_globalSrvPort;
	sendData.retry_times = SEND_RETRY_TIME;
	//加密
	int tlen = 0;
	if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),m_globalSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
		return ;
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	SendDataTo(sendData);
}

void GlobalBusiControl::HandleRecvData(const RecvDataStruct * pRecvData)
{
	
	CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"HandleRecvData1   vlink %d",pRecvData->vlink);
	CGlobalBusi * pGlobalBusi;
	map<uint32,CGlobalBusi *>::iterator it;
	it = m_mapGlobalBusi.find(pRecvData->vlink);
	if(it == m_mapGlobalBusi.end())
		return ;

	RecvDataStruct * pRecvDataTmp = (RecvDataStruct * )pRecvData;
		LPXT_HEAD lpRecvHead  = (LPXT_HEAD)(pRecvData->data);
	switch(lpRecvHead->cmd)
	{
		case CMDGSRV_GET_USER_OTHER_ACK:
		case CMDGSRV_USERSTATUS_OTHER_ACK:		
			
			CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"cmd:0x%.4x",lpRecvHead->cmd);
			pGlobalBusi = it->second;			
			pGlobalBusi->SetMultiMessange(pRecvData);			
			CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"server size %d , messages received %d",m_globalSrvIPs.size() , pGlobalBusi->m_MultiRecvData.size());
			if(m_globalSrvIPs.size()-1 == pGlobalBusi->m_MultiRecvData.size()) //收到所有回复，打开信号量的锁
			{
				pGlobalBusi->SetMultiEvent(pRecvData);				
				CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"call up the threads");
			}			
			break;
			
		default:				
			CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"default process cmd:0x%.4x",lpRecvHead->cmd);
			int tlen = 0;
			if(!xTEADecryptWithKey(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD),m_mineSessionKey,(char *)(pRecvData->data + sizeof(XT_HEAD)),&tlen))
			{
			
				return ;
			}
			pRecvDataTmp->data_len = (uint16)tlen + sizeof(XT_HEAD);			
			pGlobalBusi = it->second;
			pGlobalBusi->SetEvent(pRecvData);
			
	}

}


