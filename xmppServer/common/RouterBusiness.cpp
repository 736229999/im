#include "stdafx.h"
#include "PublicType.h"
#include "Log.h"
#include "RouterBusiness.h"
#include "RouterProtocol.h"
#include <typeinfo> 
#include "Function.h"
CRouterBusiness::CRouterBusiness(int nType)
{
	m_pPacketBuf = NULL;
	m_pDataBaseMini = NULL;
	m_pDataBaseWeb	= NULL;
	m_pGlobalBusi	= NULL;
	m_pGlobalFamilyBusi = NULL;
	m_nType = nType;
	CFIFOPacketBuffer* pPacketBufferArray = CRouterProtocol::GetInstance()->GetPacketBufferArray();
	SetPacketBuf(&(pPacketBufferArray[m_nType]));
}

CRouterBusiness::~CRouterBusiness(void)
{
}

bool CRouterBusiness::Register( CBusiness* pBusiness, int nBusinessID)
{
	assert(pBusiness);

	typedef pair <int, CBusiness*> Int_Pair;
	m_Critical.Lock();
	m_mapBusiness.insert( Int_Pair( nBusinessID, pBusiness ) );
	m_Critical.Unlock();
	CRouterProtocol::GetInstance()->Register(m_nType,nBusinessID);

	return true;
}


void CRouterBusiness::RoutOne()
{
	int nDataLength = 0;
	XT_HEAD* pHead = NULL;
	map <int, CBusiness*> :: const_iterator BusiIter;
	CBusiness*	pBusiness = NULL;

	m_pPacketBuf->Pop(m_PacketBuf,nDataLength);

	if( (unsigned int)nDataLength < sizeof(XT_HEAD))
	{
		CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_ERROR, "recv data size %d less than head size, throw away", nDataLength);
		return;
	}

	//查找能处理该包的业务类
	pHead = (XT_HEAD*)(((RecvDataStruct*)m_PacketBuf)->data) ;
	BusiIter = m_mapBusiness.find(pHead->cmd);
	if ( BusiIter == m_mapBusiness.end() )
	{
		CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_ERROR, "No busi to deal,cmd:%d, sid:%d, did:%d", pHead->cmd,pHead->sid,pHead->did);
		return;
	}
	pBusiness = BusiIter->second;

	//开始业务处理

	if (pHead->cmd != CMD_DIRECTORY_REQ && pHead->cmd !=CMD_SERVERMSG_IND && pHead->cmd != CMD_P2PMSG_SEND)
	{
#ifdef NET_DETAIL
		CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_NET, "sid:%u, did:%u, cmd:0x%.4x, data:%s", 
			pHead->sid, pHead->did, pHead->cmd, 
			ToHexString((BYTE*)m_PacketBuf + sizeof(XT_HEAD), nDataLength - sizeof(XT_HEAD)).c_str());
#else
		CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_NET, "sid:%u, did:%u, cmd:0x%.4x", 
			pHead->sid, pHead->did, pHead->cmd);
#endif

	}
	try
	{
	/*
		CLog::Log("RoutOne",CLog::TYPE_DEBUG,"sid:%u, did:%u, cmd:0x%.4x, flag:%u",
			pHead->sid,pHead->did,pHead->cmd,pHead->flag);
	*/
		unsigned long nTime = 0;
		if (pHead->cmd != CMD_DIRECTORY_REQ && pHead->cmd !=CMD_SERVERMSG_IND && pHead->cmd != CMD_P2PMSG_SEND)
		{
			nTime = timeGetTimeUsec();
			CLog::Log(typeid(*pBusiness).name(),CLog::TYPE_IMPORTANT,"PROCESS IN: sid: %u , did: %u , cmd: 0x%.4x , flag: %u , seq: %u , time: %lu",
				pHead->sid,pHead->did,pHead->cmd,pHead->flag,pHead->seq,nTime);	
		}
	
		pBusiness->SetRowPacket(m_PacketBuf,nDataLength);
		pBusiness->DoWork();
		if( pBusiness->IsNeedReply())
		{
			pBusiness->SendReply();
		}
//#ifdef TIME_STATISITC
		if (pHead->cmd != CMD_DIRECTORY_REQ && pHead->cmd !=CMD_SERVERMSG_IND && pHead->cmd != CMD_P2PMSG_SEND)
		{
			CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_IMPORTANT, "PROCESS OUT: sid: %u , did: %u , cmd: 0x%.4x , flag: %u , seq: %u , time: %lu",
				pHead->sid, pHead->did, pHead->cmd,pHead->flag, pHead->seq,timeGetTimeUsec() - nTime);
		}
//#endif		
		pBusiness->Reset();	//重置以便处理下一个业务
	}
	catch(...)
	{
		CLog::Log(typeid(*pBusiness).name(), CLog::TYPE_ERROR,"catch error");
	}


}

