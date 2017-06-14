#pragma once
#include "ZUdpComm.h"
#include "SyncEvent.h"

#include "HashList.h"

class CZUdpCommExt : public CZUdpComm
{
public:
	CZUdpCommExt(int nConsumeThreadCount = 1, int nQuereCount = 1000, int nBufferSize = 1600);
	virtual ~CZUdpCommExt(void);
	virtual int StartListen(const char * addr,int port = 0);
	virtual void StopListen();
	int GetReSendListSize() { return (int)m_hlReSend.Size();	}


	void SetServerID(uint32 id) { m_nServerId = id ; }

	//�����ظ����󣬷�������Ӧ��
	//����ֵtrue--���˳ɹ�������Ҫ��������
	//		false-���ܹ��ˣ���Ҫ��������
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
	virtual void PostQueueData(const char * data,int size);  //����Ͷ�ݽ��ն�������
	virtual void SendQueueData(const char * data,int size);  //���緢�� ���Ͷ�������
private:
	friend void  WINAPI MyTimeProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2);
	void HandleReSendData(); //�����ش�����


	void RemoveReSendData(const RecvDataStruct * pRecvData,SendDataStruct * pSendData = NULL);

	TranSendPacketItem * IsServerReplyExist(TranPacketItem& ReplyIndex);

	// �ж��Ƿ����ظ���ack
	bool IsRepeatAck(TranPacketItem& AckIndex);
	// �ж��Ƿ����ظ���req
	bool IsRepeatReq(TranPacketItem& ReqIndex);
private:
	int								m_nServerId;
	//CSyncEvent						m_timerLock;
	//�ش�ʱ��
	MMRESULT						m_hMMT;


	
	CHashList<TranSendPacketItem>	m_hlReSend;	//�ش�����
	CHashList<TranSendPacketItem>	m_hlReply;	//��ʷӦ�����
	CHashList<TranRecvPacketItem>	m_hlAck;	//��ʷ��Ӧ����
	CHashList<TranRecvPacketItem>	m_hlReq;	//��ʷ�������

	

	int			m_nSendPacketNum;				//�����ã�����������������
	int			m_nSendFailedNum;				//�����ã�����������ʧ����

	int			m_nLastResendTime;				//���ڼ���ط����Ƿ񵽴��ط���ʱ��
	int			m_nCanSend;
};
