#include <afx.h>

#include "StdAfx.h"
#include ".\zudpcommext.h"
#include "PublicType.h"
#include <protocol.h>



#define TIMERPERIOD 300 //时钟周期 ms
#define ACKTIMEOUT 5000 //ack应答超时时间 ms
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
		//重传时钟
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

	//对服务器不需要过滤服务器本身或其它服务器的请求,这些请求直接发送给客户端
	//客户端本身会过滤重复的请求.
	if( (m_nServerId > 0) && 
		(lpRecvHead->flag == XT_HEAD::FLAG_SRVACTION) )
	{
		return false;
	}	
	//心跳请求包量太大，为了效率原因不进行过滤处理
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

	
	//查找是否已处理过
	TranPacketItem PacketIndex;
	PacketIndex.m_id	= lpRecvHead->sid; 
	PacketIndex.m_seq	= lpRecvHead->seq;
	PacketIndex.m_vlink = pRecvData->vlink;

	if(lpRecvHead->flag == XT_HEAD::FLAG_SRVACTION || 
		lpRecvHead->flag == XT_HEAD::FLAG_CNTREQUEST)
	{
		// 判断该请求是否有处理好对应的应答
		TranSendPacketItem * pReply = IsServerReplyExist(PacketIndex);
		if(pReply)
		{
			SendDataTo(pReply->packet);
			return true;
		}
		// 如果该请求还没有应答则过滤重复的请求
		if(IsRepeatReq(PacketIndex))
			return true;

		m_hlReq.Add(PacketIndex);	

	}
	else if(lpRecvHead->flag == XT_HEAD::FLAG_SRVACK || lpRecvHead->flag == XT_HEAD::FLAG_CNTACK)
	{
		if(IsRepeatAck(PacketIndex))
		{
			RemoveReSendData(pRecvData);
			return true; //重复的ack包，直接丢弃
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
		// 过滤重复包
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
	//加入回复缓存
	AddServerReply(*pSendDataST);

	//判断是否需要放入重传队列
	SendDataStruct * pSTTmp = NULL;
	if(pSendDataST->retry_times > 0)
	{
		++m_nSendPacketNum;
		if(pSendDataST->time_interval <= 0)
			pSendDataST->time_interval = ACKTIMEOUT; //默认的超时间隔
		pSendDataST->tick_count = m_nLastResendTime; //不使用timeGetTime()是为了速度更快点，虽然这里面有点误差
		
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
 功能描述: 检测重传队列，一次最多检测100个,避免长时间占用重传队列锁
 必要参数说明：
 返 回 值：
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
		
		// 离上次发出的时间间隔是否超出
		if( (nCurrentTime - pSendDataST->tick_count) >= pSendDataST->time_interval)
		{
			pSendDataST->tick_count = nCurrentTime ;
			// 还可以重试
			if(pSendDataST->retry_times > 0)
			{
				pSendDataST->time_interval = (uint16)(pSendDataST->time_interval * 1.4);
				--pSendDataST->retry_times;
				/*wsaBuf.buf = (char *)&pSendDataST->vlink;
				wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;*/
				int len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;
				// 通知开始重传
				if(m_pfnUserCallback(m_pUserKey,NE_RESEND,(RecvDataStruct *)pSendDataST) == 0)
				{
					// 重发
					if(SendUdpData((char*)&pSendDataST->vlink,len,pSendDataST->dest_ip,pSendDataST->port) == -1)
					{
						m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST); 
					}
					
				}
				// 放到重传队列尾部
				m_hlReSend.MoveToTail(pTranItem);
			}
			else
			{
				// 重试次数已经用完，发送失败
				++m_nSendFailedNum;
				m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST);
				m_hlReSend.Del(*pTranItem);
			}
		}
		else
		{  
			// 没有超时则放到重传队列尾部
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
		
		//心跳业务,透明中转量太大，为了效率原因不进行应答缓存处理
		if( lpSendHead->cmd == CMD_DIRECTORY_ACK || lpSendHead->cmd == CMD_DOWNLOAD_ACK || lpSendHead->cmd == CMD_ROUTERUSERACKPACKET_ACK)
		{
			return ;
		}

		TranPacketItem ReplyIndex;
		ReplyIndex.m_id		= lpSendHead->did; //对目标的应答，即原请求的发起者
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



