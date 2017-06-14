#include "StdAfx.h"
#include ".\routerprotocol.h"
#include "Log.h"



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
		CLog::Log("CRouterProtocol","数据包没有一个包头大小，丢弃．Size:%d",pRecvData->data_len);
		return;
	}

	XT_HEAD* pHead = (XT_HEAD*)pRecvData->data;
	
	map <uint16,int> :: const_iterator BusiIter;
	BusiIter = m_mapProtocol.find(pHead->cmd);
	if( BusiIter == m_mapProtocol.end() )
	{
		if( (pHead->flag != XT_HEAD::FLAG_CNTACK) &&
			(pHead->flag != XT_HEAD::FLAG_SRVACK ) )
		{
			CLog::Log("CRouterProtocol",CLog::TYPE_ERROR,"收到一个未注册的协议包:0x%.4x,丢弃",pHead->cmd);
		}
		return;
	}

    m_pPacketBufferArray[BusiIter->second].Push((BYTE*)pRecvData,
												sizeof(RecvDataStruct)-MAX_SEND_BUFFERS + pRecvData->data_len);
}