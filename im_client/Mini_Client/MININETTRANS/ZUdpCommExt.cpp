#include <afx.h>

#include "StdAfx.h"
#include ".\zudpcommext.h"
#include "PublicType.h"
#include <protocol.h>



#define TIMERPERIOD 300 //ʱ������ ms
#define ACKTIMEOUT 5000 //ackӦ��ʱʱ�� ms
void  WINAPI MyTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CZUdpCommExt* pObj = (CZUdpCommExt*)dwUser;
	if(pObj->m_hMMT == uTimerID)
		pObj->HandleReSendData();
}


void  WINAPI MyTimeProcTest(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	

}

CZUdpCommExt::CZUdpCommExt(int nConsumeThreadCount /*= 1*/, int nQuereCount /*= 1000*/, int nBufferSize /*= 1600*/) 
	: CZUdpComm(nConsumeThreadCount, nQuereCount, nBufferSize)//,m_timerLock(true,true)
{
	m_nServerId = -1;
	m_hMMT = NULL;

	m_nSendPacketNum = 0 ;
	m_nSendFailedNum = 0 ;

	m_nLastResendTime = ::timeGetTime();
}

CZUdpCommExt::~CZUdpCommExt(void)
{

}

int CZUdpCommExt::StartListen(const char * addr,int port)
{
	m_hlReSend.Clear();
	m_hlReq.Clear();
	m_hlAck.Clear();
	m_hlReply.Clear();

	if(m_hMMT == NULL)
	{
		//�ش�ʱ��
		m_hMMT = timeSetEvent(TIMERPERIOD,0,(LPTIMECALLBACK)MyTimeProc,(DWORD_PTR)this,TIME_PERIODIC);	

	}
	return CZUdpComm::StartListen(addr,port);
}

void CZUdpCommExt::StopListen()
{
	timeKillEvent(m_hMMT);
	m_hMMT = NULL;
	
	CZUdpComm::StopListen();
}

bool CZUdpCommExt::FilterRecvData(RecvDataStruct* pRecvData)
{
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;

	//�Է���������Ҫ���˷��������������������������,��Щ����ֱ�ӷ��͸��ͻ���
	//�ͻ��˱��������ظ�������.
	if( (m_nServerId > 0) && 
		(lpRecvHead->flag == XT_HEAD::FLAG_SRVACTION) )
	{
		return false;
	}	
	//�����������̫��Ϊ��Ч��ԭ�򲻽��й��˴���
	/*if( lpRecvHead->cmd == CMD_DIRECTORY_REQ || lpRecvHead->cmd == CMD_DOWNLOAD_REQ || lpRecvHead->cmd == CMD_ROUTERUSERPACKET_REQ)
	{
		return false;
	}*/
	//modify by Tony
	if( lpRecvHead->cmd == CMD_DIRECTORY_REQ ||
		lpRecvHead->cmd == CMD_ROUTERUSERPACKET_REQ ||
		lpRecvHead->cmd == CMD_ROUTERUSERPACKET_ACK ||
		lpRecvHead->cmd == CMD_ROUTERPACKET_REQ ||
		lpRecvHead->cmd == CMD_ROUTERPACKET_ACK
		)
	{
		return false;
	}

	
	//�����Ƿ��Ѵ����
	TranPacketItem PacketIndex;
	PacketIndex.m_id	= lpRecvHead->sid; 
	PacketIndex.m_seq	= lpRecvHead->seq;
	PacketIndex.m_vlink = pRecvData->vlink;

	if(lpRecvHead->flag == XT_HEAD::FLAG_SRVACTION || 
		lpRecvHead->flag == XT_HEAD::FLAG_CNTREQUEST)
	{
		// �жϸ������Ƿ��д���ö�Ӧ��Ӧ��
		TranSendPacketItem * pReply = IsServerReplyExist(PacketIndex);
		if(pReply)
		{
			SendDataTo(pReply->packet);
			return true;
		}
		// ���������û��Ӧ��������ظ�������
		if(IsRepeatReq(PacketIndex))
			return true;

		m_hlReq.Add(PacketIndex);	

	}
	else if(lpRecvHead->flag == XT_HEAD::FLAG_SRVACK || lpRecvHead->flag == XT_HEAD::FLAG_CNTACK)
	{
		if(IsRepeatAck(PacketIndex))
		{
			RemoveReSendData(pRecvData);
			return true; //�ظ���ack����ֱ�Ӷ���
		}

		m_hlAck.Add(PacketIndex);		
	}

	return false;
}


void CZUdpCommExt::PostQueueData(const char * data,int size)
{
	RecvDataStruct * pRecvDataST = new RecvDataStruct();
	memcpy(pRecvDataST, data, size);
	LPXT_HEAD pHead = (LPXT_HEAD)pRecvDataST->data;
	/*if(pHead->flag == 0xf4 || pHead->flag == 0xf5)
	{
		ProcRecvStreamData(pRecvDataST);
		return;
	}*/
	if( (int)m_nServerId < 0)
	{
		// �����ظ���
		if(FilterRecvData(pRecvDataST))
		{
			delete pRecvDataST;
			pRecvDataST = NULL;
			return;
		}
	}
	RemoveReSendData(pRecvDataST);

	CZUdpComm::PostQueueData(data,size);
	delete pRecvDataST;
	pRecvDataST = NULL;
}
void CZUdpCommExt::SendQueueData(const char * data,int size)
{
	SendDataStruct * pSendDataST = new SendDataStruct();
	memcpy(pSendDataST, data, size);
	/*P2P_HEAD * pHead = (P2P_HEAD * )pSendDataST->data;
	if(pHead->flag == 0xf4 || pHead->flag == 0xf5)
	{
		CZUdpComm::SendQueueData(data,size);
		return;
	}*/
	//����ظ�����
	AddServerReply(*pSendDataST);

	//�ж��Ƿ���Ҫ�����ش�����
	SendDataStruct * pSTTmp = NULL;
	if(pSendDataST->retry_times > 0)
	{
		++m_nSendPacketNum;
		if(pSendDataST->time_interval <= 0)
			pSendDataST->time_interval = ACKTIMEOUT; //Ĭ�ϵĳ�ʱ���
		pSendDataST->tick_count = m_nLastResendTime; //��ʹ��timeGetTime()��Ϊ���ٶȸ���㣬��Ȼ�������е����
		
		XT_HEAD* pHead = (XT_HEAD*)pSendDataST->data;
		TranPacketItem index;
		index.m_seq = pHead->seq;
		index.m_id	= pHead->did;
		index.m_vlink=pSendDataST->vlink;
		
		TranSendPacketItem* pSendPacket = m_hlReSend.Add(index);
		if( NULL == pSendPacket)
		{
			delete pSendDataST;
			pSendDataST = NULL;
			return;
		}
		memcpy(&pSendPacket->packet,pSendDataST,sizeof(SendDataStruct));
	}

	CZUdpComm::SendQueueData(data,size);
	delete pSendDataST;
	pSendDataST = NULL;
}

void CZUdpCommExt::RemoveReSendData(const RecvDataStruct * pRecvData,SendDataStruct * pSendData/* = NULL*/)
{
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)pRecvData->data;
	
	if(lpRecvHead->flag != XT_HEAD::FLAG_SRVACK && lpRecvHead->flag != XT_HEAD::FLAG_CNTACK)
		return ;

	if( 0 == m_hlReSend.Size())
	{
		return;
	}

	TranPacketItem index;
	index.m_seq = lpRecvHead->seq;
	index.m_id	= lpRecvHead->sid;
	index.m_vlink=pRecvData->vlink;

	m_hlReSend.Del(index);

}

/*
 ��������: ����ش����У�һ�������100��,���ⳤʱ��ռ���ش�������
 ��Ҫ����˵����
 �� �� ֵ��
*/ 
void CZUdpCommExt::HandleReSendData()
{
	SendDataStruct * pSendDataST = NULL;

	if(m_bWork == false)
		return;

	int nCurrentTime = ::timeGetTime();
	m_nLastResendTime = nCurrentTime;

	WSABUF wsaBuf;

	int nCount = (int)(m_hlReSend.Size() > 100 ? 100 : m_hlReSend.Size());
	for(int i = 0; i < nCount; ++i)
	{
		m_hlReSend.GetLock()->Lock();

		TranSendPacketItem* pTranItem = m_hlReSend.Head();

		if( NULL == pTranItem)
		{
			m_hlReSend.GetLock()->Unlock();
			continue;
		}

		pSendDataST = &pTranItem->packet;
		
		// ���ϴη�����ʱ�����Ƿ񳬳�
		if( (nCurrentTime - pSendDataST->tick_count) >= pSendDataST->time_interval)
		{
			pSendDataST->tick_count = nCurrentTime ;
			// ����������
			if(pSendDataST->retry_times > 0)
			{
				pSendDataST->time_interval = (uint16)(pSendDataST->time_interval * 1.4);
				--pSendDataST->retry_times;
				/*wsaBuf.buf = (char *)&pSendDataST->vlink;
				wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;*/
				int len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;
				// ֪ͨ��ʼ�ش�
				if(m_pfnUserCallback(m_pUserKey,NE_RESEND,(RecvDataStruct *)pSendDataST) == 0)
				{
					// �ط�
					if(SendUdpData((char*)&pSendDataST->vlink,len,pSendDataST->dest_ip,pSendDataST->port) == -1)
					{
						m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST); 
					}
					
				}
				// �ŵ��ش�����β��
				m_hlReSend.MoveToTail(pTranItem);
			}
			else
			{
				// ���Դ����Ѿ����꣬����ʧ��
				++m_nSendFailedNum;
				m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST);
				m_hlReSend.Del(*pTranItem);
			}
		}
		else
		{  
			// û�г�ʱ��ŵ��ش�����β��
			m_hlReSend.MoveToTail(pTranItem);
		}
		m_hlReSend.GetLock()->Unlock();
	}

	m_nLastResendTime = ::timeGetTime();
}

void CZUdpCommExt::AddServerReply(SendDataStruct & sendData)
{
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	if(lpSendHead->flag == XT_HEAD::FLAG_SRVACK || lpSendHead->flag == XT_HEAD::FLAG_CNTACK)
	{
		
		//����ҵ��,͸����ת��̫��Ϊ��Ч��ԭ�򲻽���Ӧ�𻺴洦��
		if( lpSendHead->cmd == CMD_DIRECTORY_ACK || lpSendHead->cmd == CMD_DOWNLOAD_ACK || lpSendHead->cmd == CMD_ROUTERUSERACKPACKET_ACK)
		{
			return ;
		}

		TranPacketItem ReplyIndex;
		ReplyIndex.m_id		= lpSendHead->did; //��Ŀ���Ӧ�𣬼�ԭ����ķ�����
		ReplyIndex.m_seq	= lpSendHead->seq;
		ReplyIndex.m_vlink	= sendData.vlink;
		TranSendPacketItem* pReply = m_hlReply.Add(ReplyIndex);
		if( NULL == pReply)
		{
			return;
		}
		pReply->packet = sendData;

	}
}

TranSendPacketItem* CZUdpCommExt::IsServerReplyExist(TranPacketItem& ReplyIndex)
{
	return m_hlReply.Find(ReplyIndex);
}

bool CZUdpCommExt::IsRepeatAck(TranPacketItem& AckIndex)
{
	if( NULL == m_hlAck.Find(AckIndex) )
	{
		return false;
	}

	return true;
}

bool CZUdpCommExt::IsRepeatReq(TranPacketItem& ReqIndex)
{
	if( NULL == m_hlReq.Find(ReqIndex) )
	{
		return false;
	}

	return true;
}



