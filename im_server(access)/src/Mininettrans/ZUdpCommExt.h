#pragma once
#include "ZUdpComm.h"
#include "../../common/SyncEvent.h"

#include "HashList.h"

class CZUdpCommExt : public CZUdpComm
{
public:
	CZUdpCommExt(int nConsumeThreadCount = 1);
	virtual ~CZUdpCommExt(void);
	virtual int StartListen(const char * addr,int port = 0);
	virtual void StopListen();
	int GetReSendListSize() { return (int)m_hlReSend.Size();	}
	void StartTcpWork();

	void SetServerID(int id) { m_nServerId = id ; }

	//过滤重复请求，返回已有应答
	//返回值true--过滤成功，不需要继续处理
	//		false-不能过滤，需要继续处理
	bool FilterRecvData(RecvDataStruct* pRecvData);
	void AddServerReply(SendDataStruct & sendData);

	void GetSendPacketNum(int& nSend,int& nFailed,int& nRecvPacket)
	{
		nSend   = m_nSendPacketNum;
		nFailed = m_nSendFailedNum;
		nRecvPacket = m_nRecvPacketNum;
	}

	void ResetSendPacketNum()
	{
		m_nSendPacketNum = 0 ;
		m_nSendFailedNum = 0 ;
		m_nRecvPacketNum = 0 ;
	}

protected:
	virtual void PostQueueData(const char * data,int size);  //向上投递接收队列数据
	virtual void SendQueueData(const char * data,int size);  //网络发送 发送队列数据
private:
	friend void  ZudpMyTimeProc(void* dwUser);
	friend void  ZudpMyTimeProcNew(union sigval v);
	void HandleReSendData(); //处理重传数据


	void RemoveReSendData(const RecvDataStruct * pRecvData,SendDataStruct * pSendData = NULL);

	TranSendPacketItem * IsServerReplyExist(TranPacketItem& ReplyIndex);

	// 判断是否是重复的ack
	bool IsRepeatAck(TranPacketItem& AckIndex);
	// 判断是否是重复的req
	bool IsRepeatReq(TranPacketItem& ReqIndex);
private:
	int							m_nServerId;
	//CSyncEvent						m_timerLock;
	//重传时钟
	PMMRESULT						m_hMMT;
	timer_t m_timer;

	
	CHashList<TranSendPacketItem>	m_hlReSend;	//重传队列
	CHashList<TranSendPacketItem>	m_hlReply;	//历史应答队列
	CHashList<TranRecvPacketItem>	m_hlAck;	//历史回应队列
	CHashList<TranRecvPacketItem>	m_hlReq;	//历史请求队列



	int			m_nSendPacketNum;				//测试用，服务器主动发包数
	int			m_nSendFailedNum;				//测试用，服务器发送失败数

	DWORD		m_nLastResendTime;				//用于检查重发包是否到达重发的时间
	const unsigned int m_nTimerDelay = 300;      		//Delay : 300ms
	const unsigned int m_nTimerResolution = 300; 					//Resolution : 0s

	
};
