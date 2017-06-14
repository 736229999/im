#include "stdafx.h"
//#include "PublicType.h"
#include "BusinessExIMSrv.h"
#include "MiniAppNetwork.h"
#include <typeinfo> 


CMiniAppNetwork*	CBusinessExIMSrv::m_pAppNetwork = NULL;		//网络发送层对象指针

CBusinessExIMSrv::CBusinessExIMSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck):
CBusiness(pRouter,nBusinessID,nBusinessIDAck)
{
}

void CBusinessExIMSrv::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	CBusiness::SetRowPacket(pRowPacket,nPacketLength);

	//初始化基本数据
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	m_RecvPacket.SetData(pRecData->data + sizeof(XT_HEAD) ,pRecData->data_len - sizeof(XT_HEAD));
	m_SendPacket.SetData(m_pSendData->data  + sizeof(XT_HEAD),
						 MAX_SEND_BUFFERS - sizeof(XT_HEAD) );

	m_pRecvHead = (XT_HEAD*)pRecData->data;
	m_pSendHead = (XT_HEAD*)m_pSendData->data;

	if(m_pRecvHead->flag != XT_HEAD::FLAG_MOBREQ)
		m_pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	else
		m_pSendHead->flag = XT_HEAD::FLAG_MOBSRVACK;
	m_pSendHead->from = XT_HEAD::FROM_SERVER;
	m_pSendHead->seq  = m_pRecvHead->seq;	
	m_pSendHead->cmd  = m_nBusinessIDAck;
	m_pSendHead->sid  = m_pRecvHead->did;
	m_pSendHead->dtype = XT_HEAD::DEST_TYPE_USER;
	m_pSendHead->did	= m_pRecvHead->sid;

	m_pSendData->retry_times = 0;	
	m_pSendData->time_interval = 0;

}

//发送应答包处理
//返回值:0成功，其它值失败
int CBusinessExIMSrv::SendReply()
{
//	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;

	if( m_SendPacket.GetSize() == 0 )
	{
		//没数据，不需要应答
		return 0;
	}
	
	//构建网络层的信息
	pSendData->data_len  = (uint16)(m_SendPacket.GetSize() + sizeof(XT_HEAD));
	

	//构建协议头的信息
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;
	pSendHead->len  = m_SendPacket.GetSize();

	//发送数据
	assert(m_pAppNetwork);
	m_pAppNetwork->SendDataTo(	pSendData->data,
								pSendData->data_len);


	return 0;
}

void  CBusinessExIMSrv::SetErrorReturn(const char* pError)
{
	m_SendPacket.Reset();

	m_SendPacket << (uint8)RESULT_FAILED
				 <<  pError ;
}

