#include "stdafx.h"
#include "Business.h"
#include "MiniNetTrans.h"
#include "RouterBusiness.h"

int CBusiness::m_ServerID  = 0 ;

CBusiness::CBusiness(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck)
{
	m_pRouter = NULL;

	m_nRowPacketLength = 0;
	m_nBusinessID	 = nBusinessID;
	m_nBusinessIDAck = nBusinessIDAck;	

	m_pSendData = new SendDataStruct;

	m_bNeedSendReply = true;
	if(pRouter)
	{
		m_pRouter = pRouter;
		pRouter->Register(this,nBusinessID);
	}
}

CBusiness::~CBusiness(void)
{
	SAFE_DELETE(m_pSendData);
}


void CBusiness::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	assert(pRowPacket);
	assert(nPacketLength>0 && (unsigned int)nPacketLength<=sizeof(RecvDataStruct));
	
	m_RowPacketBuf = pRowPacket;
	m_nRowPacketLength = nPacketLength;
	
}

int CBusiness::SendReply()
{
	return 0;
}

void CBusiness::Reset()
{
	m_bNeedSendReply = true;
}
