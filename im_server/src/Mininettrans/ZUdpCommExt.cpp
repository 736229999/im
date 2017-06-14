#include "stdafx.h"
#include "ZUdpCommExt.h"
#include "PublicType.h"
#include "protocol.h"




#define TIMERPERIOD 300 //ʱ������ ms
#define ACKTIMEOUT 5000 //ackӦ��ʱʱ�� ms
void  ZudpMyTimeProc(void* dwUser)
{
	CZUdpCommExt* pObj = (CZUdpCommExt*)dwUser;
	pObj->HandleReSendData();
}

void ZudpMyTimeProcNew( union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:11, thread%d", (uint32)pthread_self());
	ZudpMyTimeProc(v.sival_ptr);
}

CZUdpCommExt::CZUdpCommExt(int nConsumeThreadCount) : CZUdpComm(nConsumeThreadCount)//,m_timerLock(true,true)
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

	//���̫�̣��ɴ���һ���̶����߳���������飬zhangmin	
	if(m_hMMT == NULL)
	{
		//�ش�ʱ��
		m_hMMT = timeSetEvent(TIMERPERIOD,0,ZudpMyTimeProc,(void*)this,TIME_PERIODIC);	
	}
	
/*
	if(!timeSetEventNew(m_timer, m_nTimerDelay, m_nTimerResolution, ZudpMyTimeProcNew, (void*)this))
	{
		
	}
*/

	return CZUdpComm::StartListen(addr,port);
}

void CZUdpCommExt::StartTcpWork()
{
	CZUdpComm::StartTcpWork();
}

void CZUdpCommExt::StopListen()
{
	
	timeKillEvent(m_hMMT);
	m_hMMT = NULL;
	
	//timer_delete(m_timer);
	
	CZUdpComm::StopListen();
}

bool CZUdpCommExt::FilterRecvData(RecvDataStruct* pRecvData)
{
	LPXT_HEAD lpRecvHead = (LPXT_HEAD)pRecvData->data;
	
	//�Է���������Ҫ���˷��������������������������,��Щ����ֱ�ӷ��͸��ͻ���
	//�ͻ��˱��������ظ�������.
	//m_nServerId not possible eqal 0
	if( (m_nServerId != 0) && 
		(lpRecvHead->flag == XT_HEAD::FLAG_SRVACTION) )
	{
		return false;
	}	

	//�����������̫��Ϊ��Ч��ԭ�򲻽��й��˴���
	if( lpRecvHead->cmd == CMD_DIRECTORY_REQ ||
		lpRecvHead->cmd == CMD_ROUTERUSERPACKET_REQ ||
		lpRecvHead->cmd == CMD_ROUTERUSERPACKET_ACK ||
		lpRecvHead->cmd == CMD_ROUTERPACKET_REQ ||
		lpRecvHead->cmd == CMD_ROUTERPACKET_ACK ||
		lpRecvHead->cmd == CMD_FILE_TRANS_REQ ||
		lpRecvHead->cmd == CMD_FILE_TRANS_ACK ||
		lpRecvHead->cmd == CMD_FILE_TRANS_IND ||
		lpRecvHead->cmd == CMD_FILE_TRANS_IND_ACK
		)
	{
		return false;
	}
//	�����������İ����޷����ˡ�
// 	if (lpRecvHead->cmd == CMD_FAMILYOFFLINEMSG_REQ)
// 	{
// 		printf("Sid:%s Seq:%s, Vlink:%s", lpRecvHead->sid, lpRecvHead->seq, pRecvData->vlink);
// 	}
	
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
			CLog::Log("CZAppNetwork",CLog::TYPE_DEBUG,"1Sid:%u, Seq:%u, Vlink:%u, Cmd %#x",
				lpRecvHead->sid, lpRecvHead->seq, pRecvData->vlink,lpRecvHead->cmd);
			SendDataTo(pReply->packet);
			return true;
		}
		
		// ���������û��Ӧ��������ظ�������
		
		if(IsRepeatReq(PacketIndex))
		{			
			CLog::Log("CZAppNetwork",CLog::TYPE_DEBUG,"2Sid:%u Seq:%u, Vlink:%u, Cmd %#x", 
				lpRecvHead->sid, lpRecvHead->seq, pRecvData->vlink,lpRecvHead->cmd);
			CLog::Log("CZAppNetwork",CLog::TYPE_DEBUG,"2Sid:%u, Seq:%u, Vlink:%u, Cmd %#x",
				lpRecvHead->sid, lpRecvHead->seq, pRecvData->vlink,lpRecvHead->cmd);
			return true;
		}		
		m_hlReq.Add(PacketIndex);	
	}
	else if(lpRecvHead->flag == XT_HEAD::FLAG_SRVACK || lpRecvHead->flag == XT_HEAD::FLAG_CNTACK)
	{
		if(IsRepeatAck(PacketIndex))
		{			
			CLog::Log("CZAppNetwork",CLog::TYPE_DEBUG,"3Sid:%u, Seq:%u, Vlink:%u, Cmd %#x",
				lpRecvHead->sid, lpRecvHead->seq, pRecvData->vlink,lpRecvHead->cmd);
			RemoveReSendData(pRecvData);
			return true; //�ظ���ack����ֱ�Ӷ���
		}
		m_hlAck.Add(PacketIndex);		
	}

	return false;
}


void CZUdpCommExt::PostQueueData(const char * data,int size)
{
//	printf("CZUdpCommExt::PostQueueData \n");
	RecvDataStruct * pRecvDataST = (RecvDataStruct * )data;

	// not possible equal 0
	if( m_nServerId == 0)
	{
		FilterRecvData(pRecvDataST);
	}
	RemoveReSendData(pRecvDataST);

	CZUdpComm::PostQueueData(data,size);
}
void CZUdpCommExt::SendQueueData(const char * data,int size)
{
	SendDataStruct * pSendDataST = (SendDataStruct *)data;
	XT_HEAD* pHead = (XT_HEAD*)(pSendDataST->data);

#ifdef WEBIM_TCP
	if(pHead->flag != XT_HEAD::FLAG_MOBSRVACK && pHead->flag != XT_HEAD::FLAG_MOBSRVACTION)
	{
#endif
		//����ظ�����
		AddServerReply(*pSendDataST);

		//�ж��Ƿ���Ҫ�����ش�����
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
				return;
			}

			memcpy(&pSendPacket->packet,pSendDataST,sizeof(SendDataStruct));
		}
		
#ifdef WEBIM_TCP
	}
#endif

	CZUdpComm::SendQueueData(data,size);
}

void CZUdpCommExt::RemoveReSendData(const RecvDataStruct * pRecvData,SendDataStruct * pSendData/* = NULL*/)
{
	LPXT_HEAD lpRecvHead  = (LPXT_HEAD)pRecvData->data;
	
	if(lpRecvHead->flag != XT_HEAD::FLAG_SRVACK && lpRecvHead->flag != XT_HEAD::FLAG_CNTACK && lpRecvHead->flag != XT_HEAD::FLAG_MOBSRVACK)
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

	DWORD nCurrentTime = ::timeGetTime();

	m_nLastResendTime = nCurrentTime;

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
	
		if((nCurrentTime - pSendDataST->tick_count) >= pSendDataST->time_interval)
		{
			if(pSendDataST->retry_times > 0 )
			{
				pSendDataST->tick_count = nCurrentTime ;
				--pSendDataST->retry_times;
#ifdef _DEBUG			
				m_pfnUserCallback(m_pUserKey,NE_RESEND,(RecvDataStruct *)pSendDataST);
#endif
				
				// �ط�
				CZUdpComm::SendQueueData((const char*)pSendDataST,pSendDataST->data_len);
				m_hlReSend.MoveToTail(pTranItem);

			}
			else
			{  
				++m_nSendFailedNum;
				m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST);


				m_hlReSend.Del(*pTranItem);
			}

			//m_hlReSend.MoveToTail(pTranItem);
		}
		else
		{
			m_hlReSend.MoveToTail(pTranItem);
		}

		m_hlReSend.GetLock()->Unlock();
	}

	m_nLastResendTime = ::timeGetTime();
}

void CZUdpCommExt::AddServerReply(SendDataStruct & sendData)
{
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	if(lpSendHead->flag == XT_HEAD::FLAG_SRVACK || lpSendHead->flag == XT_HEAD::FLAG_CNTACK || lpSendHead->flag == XT_HEAD::FLAG_MOBSRVACK)
	{
		
		//����ҵ����̫��Ϊ��Ч��ԭ�򲻽���Ӧ�𻺴洦��
		if( lpSendHead->cmd == CMD_DIRECTORY_ACK)
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

