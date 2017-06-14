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

	m_globalSrvIP =  globalSrvIP;
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

void GlobalBusiControl::HandleRecvData(const RecvDataStruct * pRecvData)
{
	map<uint32,CGlobalBusi *>::iterator it;
	it = m_mapGlobalBusi.find(pRecvData->vlink);
	if(it == m_mapGlobalBusi.end())
		return ;

	RecvDataStruct * pRecvDataTmp = (RecvDataStruct * )pRecvData;
	int tlen = 0;
    if(!xTEADecryptWithKey(pRecvData->data + sizeof(XT_HEAD),pRecvData->data_len - sizeof(XT_HEAD),m_mineSessionKey,(char *)(pRecvData->data + sizeof(XT_HEAD)),&tlen))
		return ;
	pRecvDataTmp->data_len = (uint16)tlen + sizeof(XT_HEAD);

	CGlobalBusi * pGlobalBusi = it->second;
	pGlobalBusi->SetEvent(pRecvData);
}

void GlobalBusiControl::SendRequestData(SendDataStruct & sendData)
{	
	sendData.dest_ip = m_globalSrvIP;
	sendData.port = m_globalSrvPort;
	sendData.retry_times = SEND_RETRY_TIME;
	//º”√‹
	int tlen = 0;
	if(!xTEAEncryptWithKey(sendData.data + sizeof(XT_HEAD),sendData.data_len - sizeof(XT_HEAD),m_globalSessionKey,sendData.data + sizeof(XT_HEAD),&tlen))
		return ;
	sendData.data_len = (uint16)tlen + sizeof(XT_HEAD);

	SendDataTo(sendData);
}

