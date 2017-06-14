#include "stdafx.h"
#include "BusinessExGSrv.h"
#include "MiniNetTrans.h"
#include "Function.h"
#include <typeinfo>


CBusinessExGSrv::CBusinessExGSrv(CRouterBusiness* pRouter,uint16 nBusinessID,uint16 nBusinessIDAck):
CBusiness(pRouter,nBusinessID,nBusinessIDAck)
{
	memset(m_globalSessionKey,0,sizeof(m_globalSessionKey));
}


CUdpNetTrans*	CBusinessExGSrv::m_pNetTrans = NULL;		//���緢�Ͳ����ָ��

//����Ӧ�������
//����ֵ:0�ɹ�������ֵʧ��
int CBusinessExGSrv::SendReply()
{
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	SendDataStruct* pSendData = m_pSendData;
	
	if( m_SendPacket.GetSize() == 0 )
	{
		//û���ݣ�����ҪӦ��
		return 0;
	}

	//������������Ϣ
	pSendData->dest_ip = pRecvData->src_ip;
	pSendData->port	   = pRecvData->port;
	pSendData->vlink   = pRecvData->vlink;
	pSendData->retry_times = 0;
	pSendData->data_len  = (uint16)m_SendPacket.GetSize() + sizeof(XT_HEAD);
	

	//����Э��ͷ����Ϣ
	XT_HEAD* pRecvHead = (XT_HEAD*)pRecvData->data;
	XT_HEAD* pSendHead = (XT_HEAD*)pSendData->data;

	pSendHead->flag = XT_HEAD::FLAG_SRVACK;
	pSendHead->from = XT_HEAD::FROM_SERVER;
	pSendHead->cmd  = m_nBusinessIDAck;
	pSendHead->seq  = pRecvHead->seq;
	pSendHead->sid  = m_ServerID;
	pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
	pSendHead->did  = pRecvHead->sid;
	pSendHead->len  = m_SendPacket.GetSize();
 
	//��������
	int nDataLength = pSendData->data_len;
	::xTEAEncryptWithKey(pSendData->data + sizeof(XT_HEAD) ,pSendData->data_len - sizeof(XT_HEAD) ,
						m_globalSessionKey,
						pSendData->data + sizeof(XT_HEAD) ,&nDataLength ) ;
	pSendData->data_len = (uint16)nDataLength;

	pSendData->data_len += sizeof(XT_HEAD);

	//��������
	assert(m_pNetTrans);
	m_pSendData->retry_times = 0;			
	m_pNetTrans->SendDataTo(*m_pSendData);

	

	return 0;
}


void CBusinessExGSrv::SetRowPacket(BYTE* pRowPacket,int nPacketLength)
{
	CBusiness::SetRowPacket(pRowPacket,nPacketLength);

	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	
	char* pProtocolData    = pRecData->data + sizeof(XT_HEAD);
	int	  nProtocolDataLen = pRecData->data_len - sizeof(XT_HEAD);

	//��������
	::xTEADecryptWithKey(pProtocolData,nProtocolDataLen,
						m_globalSessionKey,
						pProtocolData,&nProtocolDataLen);

	m_RecvPacket.SetData(pProtocolData ,nProtocolDataLen);
	m_SendPacket.SetData(m_pSendData->data+ sizeof(XT_HEAD),
						 MAX_SEND_BUFFERS - sizeof(XT_HEAD) );
}
