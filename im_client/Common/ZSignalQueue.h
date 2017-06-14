/************************************************************************ 
 �ļ�������ʵ�ֻ����ź��������Ķ��� 
 
 �����ˣ� Zouyf, 2006-6-8 
 
 �޸ļ�¼�� 
            
************************************************************************/

#pragma once
#include "Utility.h"

#define CONSUME_THREAD_COUNT 2
#define MAX_SIGNAL_COUNT QUEUESIZE

//
// ���ȼ�(��):		60%��������
// ���ȼ�(����):	24%��������
// ���ȼ�(��):		16%��������
//
const int	PROBABILITY_HIGH = 60;		 
const int	PROBABILITY_COMMON = 60;	
const int	PROBABILITY_LOW = 100;

const int	PRIOR_QUEUE_COUNT = 3;

typedef void (*pfnSQCallback)(void * pUserKey,const char * data,int size);

class CZSignalQueue
{
public:

	CZSignalQueue(int nConsumeThreadCount = 1,int size = 1000, int buffersize = 1600);

	~CZSignalQueue(void);

	int Start();

	void SetUserCallback(void * pUserKay,pfnSQCallback pfnCallback)
	{
		m_pUserKey = pUserKay;
		m_pfnCallback = pfnCallback;
	}

	int Push(const char * data,int size,int nPrior = 0);

	void Stop();

	long GetSize() { return m_PacketQueue[0]->GetSize(); 	}

private:
	static DWORD WINAPI WrokThreadFun(LPVOID lpParam);
	void PostQueueData(); //���ϲ�Ͷ�ݶ�������
private:
	HANDLE m_hSemSignal;
	HANDLE m_hEventQuit;

	int    m_nConsumeThreadCount;
	HANDLE * m_phWorkThread;
	
	void * m_pUserKey;
	pfnSQCallback m_pfnCallback;

	CPacketQueue *	m_PacketQueue[PRIOR_QUEUE_COUNT];
	int				m_probability[PRIOR_QUEUE_COUNT];
};
