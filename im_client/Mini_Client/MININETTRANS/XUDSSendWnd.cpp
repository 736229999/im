#include "StdAfx.h"
#include "xudssendwnd.h"
#include "xudsokect.h"

#include "Utility.h"

const int MAX_DATA_LEN = BEST_SEG_SIZE - sizeof(XUDS_HEAD);

XUDSSendWnd::XUDSSendWnd(uint32 nSendWndSize,uint32 nSendBufSize):m_buf(nSendBufSize)
{
	m_pSendPool = new XUDS_SEND_SEG[nSendWndSize];
	m_nWndSize = nSendWndSize;
	m_nBeg = 0;
	m_nEnd = 0;
	m_nSending = 0;

	m_RTT_D = 50*1000;		// us
	m_RTT_A = 100*1000;		// us
	m_RTO = 300*1000;
	m_RTT_C = 100*1000;

	m_nAckNext = 0;

	m_nSendBytes = 0;
	m_nAckNext = 0;
	m_nAckTimes = 0;
	m_nSuccSend = 0;

#ifdef _MyDebug
	m_nResend = 0;
	m_nQResend = 0;
	m_nNeedSend = 0;

	m_nSendSeq = 0;
	m_nAI = m_nSendInfo = m_nAckInfo = 1024 * 100;
	m_pSendTick = new UINT[m_nSendInfo];
	m_pSendSeq = new UINT[m_nSendInfo];
	m_pAckInfo = new int[m_nAckInfo];
	m_pAckTick = new UINT[m_nAckInfo];
	m_pSendWait = new UINT[m_nSendInfo];
	m_pQuickResend = new UINT[m_nSendInfo];
	m_pAIInfo = new UINT[m_nAI];
	m_pCurSPTInfo = new UINT[m_nSendInfo];
	memset(m_pAckInfo,0,m_nAckInfo*sizeof(int));
	memset(m_pAckTick,0,m_nAckInfo*sizeof(int));
	memset(m_pSendTick,0,m_nSendInfo*sizeof(int));
	memset(m_pSendSeq,0,m_nSendInfo*sizeof(int));
	memset(m_pSendWait,0,m_nSendInfo*sizeof(UINT));
	memset(m_pAIInfo,0,m_nAI * sizeof(UINT));
	memset(m_pCurSPTInfo,0,m_nAI * sizeof(UINT));
	m_nAckSeq = 0;
	m_nAISeq = 0;
#endif
}

XUDSSendWnd::~XUDSSendWnd(void)
{
	if(m_pSendPool)
	{
		delete [] m_pSendPool;
	}
#ifdef _MyDebug
	if(m_pQuickResend)
	{
		delete [] m_pQuickResend;
	}
	if(m_pAckInfo)
	{
		delete [] m_pAckInfo;
	}
	if(m_pAckTick)
	{
		delete [] m_pAckTick;
	}
	if(m_pSendSeq)
	{
		delete [] m_pSendSeq;
	}
	if(m_pSendTick)
	{
		delete [] m_pSendTick;
	}
	if(m_pSendWait)
	{
		delete [] m_pSendWait;
	}
	if(m_pAIInfo)
	{
		delete [] m_pAIInfo;
	}
	if(m_pCurSPTInfo)
	{
		delete [] m_pCurSPTInfo;
	}
#endif
}

void XUDSSendWnd::Slow()
{
	m_RTO *= 1.2;
	m_RTT_A *= 1.2;
}

XUDSSendWnd::ACK_STATE XUDSSendWnd::ConfirmAck(XUDSocket * pS, uint32 seq,ULONGLONG tickCount)
{
	CMyLock lock(m_sc.Get());
	ACK_STATE rt = ACK_OK;
#ifdef _MyDebug
	// 记录下收到的ACK及其返回时间
	if(m_nAckSeq < m_nAckInfo)
	{
		m_pAckInfo[m_nAckSeq] = seq;
		m_pAckTick[m_nAckSeq] = tickCount;
		m_nAckSeq++;
	}
	
#endif
	if(seq < m_nBeg)
	{
		TRACE("收到重复应答%d,beg=%d\n",seq,m_nBeg);
		return ACK_ERR;
	}
	else if(seq > m_nEnd)
	{
		TRACE("收到无效应答%d,end=%d\n",seq,m_nEnd);
		return ACK_ERR;
	}
	else
	{
		TRACE("Recv Ack %d\n",seq);
	}
	int idx = seq % m_nWndSize;
	SEND_PACK_STATE st = m_pSendPool[idx].GetState();
	// 判断发送窗体中的对应块是否已经确认
	if(st != SPS_CONFIRMED)
	{
		// 更新RTT
		LONGLONG nInterTime = m_pSendPool[idx].GetInterTime(tickCount);
		if(nInterTime < 0)
		{
			nInterTime = 0;
		}
		m_RTT_C = nInterTime;

		INT nErr = nInterTime - m_RTT_A;
		// 计算均值A
		m_RTT_A += nErr / 8;					// >>3等同与除8
		if(m_RTT_A <= 0)
		{
			//cout<<nInterTime<<","<<nErr<<",rtt"<<m_RTT_A<<endl;
			m_RTT_A = 1;
		}
		// 计算均值偏差D
		if(nErr >= 0)
			m_RTT_D += (nErr - m_RTT_D) / 4;	// >>2等同与除4
		else
			m_RTT_D += (-nErr - m_RTT_D) / 4;	// >>2等同与除4
		if(m_RTT_D < 0)
		{
			m_RTT_D = 0;
		}
		// 计算RTO
		m_RTO = 4 * (m_RTT_A + 4 * m_RTT_D);

		m_nSendBytes += m_pSendPool[idx].GetDataLen();
		m_nSuccSend += m_pSendPool[idx].GetDataLen();
		m_pSendPool[idx].SetState(SPS_CONFIRMED);
		m_nSending--;
	}
	else
	{
		TRACE("收到重复应答 %d\n",seq);
		return ACK_REPEAT;
	}
	
	// 检测并滑动窗体
	uint32 i;
	for(i = m_nBeg;i < m_nEnd;i++)
	{
		idx = i % m_nWndSize;
		if(m_pSendPool[idx].GetState() == SPS_CONFIRMED)
		{
			m_nBeg++;
		}
		else
		{
			break;
		}
	}
	if(seq == m_nBeg)
	{
		// 不需要快速重传
	}
	else
	{
		if(CheckQuickResend(pS,seq,st))
			rt = ACK_QUICK_RESEND;
	}
	m_eventAck.Set();
	return rt;
	}

bool XUDSSendWnd::CheckQuickResend(XUDSocket * pS,int nSeq,SEND_PACK_STATE st)
{
	int i;
	int idx;
	bool bQR = false;

	if(st == SPS_SENDING)
	{
		for(i = nSeq;i >= m_nBeg;i--)
		{
			idx = i % m_nWndSize;
			if(m_pSendPool[idx].GetState() ==  SPS_SENDING)
			{
#ifdef _MyDebug
				m_nQResend++;
#endif
				// 需要快速重传
				pS->AddQuickResend(i);
				//TRACE("quick resend %d\n",i);
				bQR = true;
			}
		}
	}
	return bQR;
}


//void XUDSSendWnd::AddTask(int nSeq,const BYTE * pData,int nLen,ULONGLONG tickCount)
//{
//	CMyLock lock(m_sc.Get());
//	assert((m_nEnd - m_nBeg) < m_nWndSize);
//	if(m_nEnd - m_nBeg > m_nWndSize )
//	{
//		TRACE("发送窗口已满,无法发送数据\n");
//		return;
//	}
//	int idx;
//	idx = nSeq  % m_nWndSize;
//	m_pSendPool[idx].Fill(pData,nLen,tickCount);
//	assert(m_nEnd == nSeq);
//	if(m_nEnd != nSeq)
//	{
//		TRACE("error");
//		
//	}
//	m_nEnd++;
//	m_nSending++;
//
//}

int XUDSSendWnd::PushData(const BYTE * pData,int nLen)
{
	return m_buf.Push((const char *)pData,nLen);
}

bool XUDSSendWnd::SendBuf(XUDSocket * pS)
{
	CMyLock lock(m_sc.Get());

	if(m_nEnd - m_nBeg >= m_nWndSize )
		return false;
	if(m_buf.GetDataSize() <= 0)
		return false;


	int nSend = MAX_DATA_LEN;
	int idx;
	ULONGLONG tick;
	idx = m_nEnd  % m_nWndSize;
	
	XUDS_DATA *pData = (XUDS_DATA*)m_pSendPool[idx].GetData();
	pData->flag = UDS_FLAG;
	pData->cmd = CMD_P2P_DATA;
	pData->did = pS->m_nDestId;
	pData->socketId = pS->m_xsocket;
	pData->seq = m_nEnd;
	// 从缓冲读取数据,准备发送
	nSend = m_buf.Pop((char*)pData->udsData,nSend);
	if(nSend <= 0)
		return false;
	pData->len = nSend;
	tick = getTickCount();
	// 设置发包状态
	m_pSendPool[idx].SetSending(tick,sizeof(XUDS_HEAD) + nSend);

	//
	// 开始发数据包
	//
#ifdef _MyDebug
	// 记录发包时间
	m_pSendSeq[m_nSendSeq] = m_nEnd;
	m_pSendTick[m_nSendSeq] = tick;
	m_pCurSPTInfo[m_nSendSeq] = pS->m_SPT;
	m_nSendSeq++;
	m_nNeedSend++;
#endif
	pS->SendPacket(pData);
	m_nEnd++;
	m_nSending++;
	return true;
}

//bool XUDSSendWnd::CanSend()
//{
//	CMyLock lock(m_sc.Get());
//	if(m_nEnd - m_nBeg > m_nWndSize )
//		return false;
//	else
//		return true;
//	//return m_buf.GetDataSize() > 0;
//}


bool XUDSSendWnd::ReSend(int nPack,XUDSocket * pS)
{
	CMyLock lock(m_sc.Get());
	if(nPack < m_nBeg)
	{
		// 该包已经重传成功
		//assert(0 && "you are great!!!");
		TRACE("%d已确认,取消重发!\n",nPack);
		return false;	
	}
	int idx = nPack % m_nWndSize;
	// 如果已经确认或正在重发,则不需要重传了
	SEND_PACK_STATE st = m_pSendPool[idx].GetState() ;
	if(	st == SPS_CONFIRMED ||
		st == SPS_RESENDING
		)
	{
		return false;
	}
#ifdef _MyDebug
	// 记录下重发包时间
	m_pSendSeq[m_nSendSeq] = nPack;
	m_pSendTick[m_nSendSeq] = getTickCount();
	m_nSendSeq++;

#endif
	m_pSendPool[idx].SetResending();
	pS->SendPacket((XUDS_DATA*) m_pSendPool[idx].GetData());
	return true;
}

bool XUDSSendWnd::CheckResend(XUDSocket * pS)
{
	uint32 i = 0;
	uint32 idx = 0;
	LONGLONG interTime = 0;
	ULONGLONG tickCount;
	int nFlag;
	CMyLock lock(m_sc.Get());
	SEND_PACK_STATE st;
	bool bResend = false;
	for(i = m_nBeg;i < m_nEnd;i++)
	{
		idx = i % m_nWndSize;
		st = m_pSendPool[idx].GetState();
		// 判断当前正在发送的数据包是否超时
		if(st ==  SPS_SENDING || st == SPS_RESENDING)
		{
			tickCount = getTickCount();
			//interTime = m_pSendPool[idx].GetInterTime(tickCount);;
			nFlag = m_pSendPool[idx].TestTimeOut(tickCount,m_RTO);
			if(nFlag == XUDS_SEND_SEG::NEED_RESEND)
			{
				m_pSendPool[idx].SetResend();

				TRACE("resend %d,inter=%d,rto=%d \n",i,(int)interTime,m_RTO);
				// 需要重传
				//pS->ResendData((XUDS_DATA*) m_pSendPool[idx].GetData());
				pS->AddResend(i);
				bResend = true;
#ifdef _MyDebug
				//cout<<"resend"<<i<<endl;
				//printf("resend %d,inter=%d,rto=%d \n",i,(int)interTime,m_RTO);
				m_nResend++;
#endif
			}
		};
	}

	return bResend;
}

#ifdef _MyDebug


void XUDSSendWnd::WriteInfo(ofstream & ofs)
{
	ofs<<"Send信息"<<endl;
	ofs<<"block size = "<<BEST_SEG_SIZE<<endl;
	ofs<<"total send times = "<<m_nSendSeq<<endl;
	ofs<<"total ack times = "<<m_nAckSeq<<endl;
	ofs<<"total task = "<<m_nNeedSend<<endl;
	ofs<<"quick resend = "<<m_nQResend<<endl;
	ofs<<"resend = "<<m_nResend<<endl;
	
	// calc speed
	if(m_nAckSeq > 0 && m_nSendSeq > 0)
	{
		float fTime = m_pAckTick[m_nAckSeq - 1] - m_pSendTick[0];
		fTime /= 1000;
		ofs<<"speed = "<<(m_nAckSeq * BEST_SEG_SIZE)/fTime/1.024<<"KB/s"<<endl;
	}
	
	
	int i,iS = 0,iR = 0;
	int nInfoC = m_nSendSeq + m_nAckSeq;
	UINT nLastTick = m_pSendTick[0];
	UINT nLastSendTick = m_pSendTick[0];
	UINT nLastAckTick = m_pAckTick[0];
	ofs<<"-----------------------------------------------------------------"<<endl;
	ofs<<"操作 |	序号	|	时间	| 	间隔	|s间隔	|r间隔"<<endl;
	ofs<<"-----------------------------------------------------------------"<<endl;
	for (i = 0;i < nInfoC;i++)
	{
		if(m_pSendTick[iS] != 0 && m_pSendTick[iS] < m_pAckTick[iR])
		{
			ofs	<<"发   |	"<<m_pSendSeq[iS]<<"	|"<<m_pSendTick[iS]<<"	|	"<<(m_pSendTick[iS] - nLastTick)<<"	|"
				<< m_pSendTick[iS] - nLastSendTick<<"	|	"
				<<endl;
			nLastTick = m_pSendTick[iS];
			nLastSendTick = nLastTick;
			iS++;
			
		}
		else
		{
			ofs	<<"收   |	"<<m_pAckInfo[iR]<<"	|"<<m_pAckTick[iR]<<"	|	"<<(m_pAckTick[iR] - nLastTick) <<"	|"
				<<"	|"<<m_pAckTick[iR] - nLastAckTick<<"	|"
				<<endl;
			nLastTick = m_pAckTick[iR];
			nLastAckTick = nLastTick;
			iR++;
		}

	}

	ofs<<endl;
	ofs<<"收包 "<<"ack seq\t"<<"收包时间\t"<<"ai"<<endl;
	for(i = 0;i < m_nAckSeq;i++)
	{
		ofs<<i<<"\t"<<m_pAckInfo[i]<<"\t"<<m_pAckTick[i]<<"\t"
			<<m_pAIInfo[i]
		<<endl;
	}

}

void XUDSSendWnd::WriteSendInfo(ofstream & ofs,int nRecords /* = -1 */)
{
	int i;
	int nC;
	if(nRecords  == -1 )
		nC = m_nSendSeq;
	else if(nRecords > m_nSendSeq)
		nC = m_nSendSeq;
	else
		nC = nRecords;
	
	if(nC < 1)
		return;

	float fMaxSpeed = 0;
	float time;
	float lost = m_nAckSeq  / (float)(m_pAckInfo[m_nAckSeq - 1] + 1);
	

	ofs<<"Send信息"<<endl;
	ofs<<"block size = "<<BEST_SEG_SIZE<<endl;
	ofs<<"成功率"<< lost * 100.0<<"%"<<endl;
	ofs<<"successed "<<m_nAckSeq<<endl;
	

	time =  (m_pAckTick[m_nAckSeq - 1] - m_pSendTick[0]) / 1000000.0;
	fMaxSpeed  = ((m_nAckSeq + 1) * BEST_SEG_SIZE)  / time;
	ofs<<"best spi can be="<<(BEST_SEG_SIZE * 1000 / fMaxSpeed)<<"ms"<<endl;
	ofs<<"Max speed = "<<fMaxSpeed/1024.0<<"kb/s"<<endl;
	ofs<<"No.\tAck\tSend ->\t\tRecv <-\t\t间隔(ms) RTT(ms) 接收间隔 spi"<<endl;
	
	UINT nLastTick;

	if(nRecords  == -1 )
		nC = m_nAckSeq;
	else if(nRecords > m_nAckSeq)
		nC = m_nAckSeq;
	else
		nC = nRecords;
	nLastTick = m_pAckTick[0];
	int nAI = 0;
	int nARTT = 0;
	int nSendSeq = 0;
	int nTemp;
	int j;
	int nLastAck = -1;
	for(i = 0;i < nC;i++,nSendSeq++)
	{
		//time = m_pAckTick[i] - nLastTick;// 两次包之间的间隔
		//fMaxSpeed  = ((i + 1) * BEST_SEG_SIZE)  / time;
		ofs <<nSendSeq<<"\t";
		nTemp = m_pAckInfo[i];
		for(j = nLastAck + 1;j < nTemp;j++)
		{
			ofs<<"x\t"<<m_pSendTick[j];
			nSendSeq++;
			ofs<<endl;
			ofs <<nSendSeq<<"\t";
			
		}
		nLastAck = nTemp;
		ofs<<i
		// <<m_pAckInfo[i]
			<<"\t"<<m_pSendTick[m_pAckInfo[i]]
			<<"\t"<<m_pAckTick[i]
			<<"\t"<<float(m_pAckTick[i] - nLastTick)/1000.
			<<"\t"<<(float)(m_pAckTick[i] - m_pSendTick[m_pAckInfo[i]])/1000.
			<<"\t"<<(float)m_pAIInfo[i]/1000.
			//<<"\t"<<(float)m_pCurAIInfo[i]/1000.
			<<endl;
		nLastTick = m_pAckTick[i];
		nAI += m_pAIInfo[i];
		nARTT += (m_pAckTick[i] - m_pSendTick[m_pAckInfo[i]]);
	}
	for(i = nLastAck + 1;i < m_nSendSeq;i++)
	{
		ofs<<i<<"\t"<<"x\t"<<m_pSendTick[i]<<endl;
	}

	ofs<<"avg AI="<<(float)nAI/(nC - 1)/1000.<<endl;
	ofs<<"avg RTT = "<<(float)nARTT/(nC)/1000.<<endl;
	ofs<<"seq\ttime\t发送间隔时间(ms)"<<endl;
	nLastTick = m_pSendTick[0];
	for(i = 0;i < m_nSendSeq;i++)
	{
		ofs<<i<<"\t"<<m_pSendTick[i]<<"\t\t"<<(m_pSendTick[i] - nLastTick)/1000.;
		/*if(m_pSendWait[i])
		ofs<<"\t="<<m_pSendWait[i];*/
		ofs<<endl;
		nLastTick = m_pSendTick[i];
	}


	/*ofs<<"Quick Resend info"<<endl;

	for(i = 0;i < m_nQuickResend;i++)
	{
	ofs<<i<<"\t"<<m_pQuickResend[i]<<endl;
	}*/

}
void XUDSSendWnd::WriteDebugInfo(ofstream & ofs,int nRecords /* = -1 */)
{
	int i;
	int nC;
	if(nRecords  == -1 )
		nC = m_nSendSeq;
	else if(nRecords > m_nSendSeq)
		nC = m_nSendSeq;
	else
		nC = nRecords;
	ofs<<"Send信息"<<endl;
	UINT nLastTick = m_pSendTick[0];
	for(i = 0;i < nC;i++)
	{
		ofs<<i<<"\t"<<m_pSendTick[i]<<"\t\t"<<m_pSendTick[i] - nLastTick;
		if(m_pSendWait[i])
			ofs<<"\t="<<m_pSendWait[i];
		ofs<<endl;
		nLastTick = m_pSendTick[i];
	}

	ofs<<"Ack信息\tack\ttick\t\t间隔\tRTT"<<endl;
	if(nRecords  == -1 )
		nC = m_nAckSeq;
	else if(nRecords > m_nAckSeq)
		nC = m_nAckSeq;
	else
		nC = nRecords;
	nLastTick = m_pAckTick[0];
	for(i = 0;i < nC;i++)
	{
		ofs <<i<<"\t";
		if(m_pAckInfo[i] != i)
			ofs<<"*";
		ofs	<<m_pAckInfo[i]<<"\t"<<m_pAckTick[i]<<"\t"<<m_pAckTick[i] - nLastTick
			<<"\t"<<m_pAckTick[i] - m_pSendTick[m_pAckInfo[i]]
			<<endl;
		nLastTick = m_pAckTick[i];
	}

	/*ofs<<"Quick Resend info"<<endl;

	for(i = 0;i < m_nQuickResend;i++)
	{
		ofs<<i<<"\t"<<m_pQuickResend[i]<<endl;
	}*/

}
#endif