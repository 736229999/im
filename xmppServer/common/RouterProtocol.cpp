#include "stdafx.h"
#include "RouterProtocol.h"
#include "Log.h"
#include "protocol.h"


CRouterProtocol* CRouterProtocol::m_pInstance = NULL;

CRouterProtocol::CRouterProtocol(void)
{
	m_pPacketBufferArray = NULL;

}

CRouterProtocol::~CRouterProtocol(void)
{
}

CRouterProtocol* CRouterProtocol::GetInstance()
{
	static CRouterProtocol	SingleInstance;
	return &SingleInstance;
}
bool CRouterProtocol::Register(int nProtocolID,uint16 nBusinessID)
{
	m_Critical.Lock();

	typedef pair <uint16, int> Int_Pair;
	m_mapProtocol.insert( Int_Pair( nBusinessID, nProtocolID ) );

	m_Critical.Unlock();

	return true;
}


void CRouterProtocol::RouterOnePacket(const RecvDataStruct * pRecvData)
{
	if( pRecvData->data_len < sizeof(XT_HEAD))
	{
		if(pRecvData->data_len != 15)
		CLog::Log("CRouterProtocol",CLog::TYPE_ERROR,"数据包没有一个包头大小，丢弃．Size:%d",pRecvData->data_len);
		return;
	}

	XT_HEAD* pHead = (XT_HEAD*)pRecvData->data;
	
	map <uint16,int> :: const_iterator BusiIter;
	BusiIter = m_mapProtocol.find(pHead->cmd);
	if( BusiIter == m_mapProtocol.end() )
	{
		if( (pHead->flag != XT_HEAD::FLAG_CNTACK) &&
			(pHead->flag != XT_HEAD::FLAG_SRVACK ) &&
			(pHead->flag != XT_HEAD::FLAG_MOBSRVACK))
		{
			CLog::Log("CRouterProtocol",CLog::TYPE_ERROR,"收到一个未注册的协议包:0x%.4x,丢弃,sid:%u, did:%u",pHead->cmd,pHead->sid,pHead->did);
		}
		return;
	}
/*
	CLog::Log("RouterOnePacket",CLog::TYPE_DEBUG,"sid:%u, did:%u, cmd:0x%.4x, flag:%u",
			pHead->sid,pHead->did,pHead->cmd,pHead->flag);
*/
	
	
	unsigned long nTime = timeGetTimeUsec();
	CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"sid: %u , did: %u , cmd: 0x%.4x , flag: %u , seq: %u , time: %lu",
			pHead->sid,pHead->did,pHead->cmd,pHead->flag,pHead->seq,nTime);

    m_pPacketBufferArray[BusiIter->second].Push((BYTE*)pRecvData,
												sizeof(RecvDataStruct)-MAX_NETWORK_PACKET + pRecvData->data_len);

	map <uint16,int> :: iterator AuditIter;
	AuditIter = m_mapCmdAudit.find(pHead->cmd);
	if( AuditIter != m_mapCmdAudit.end())
	{
		(AuditIter->second)++;
	}
	else
	{
		m_mapCmdAudit[pHead->cmd] = 1;
	}
}

