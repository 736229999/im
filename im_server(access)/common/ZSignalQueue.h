/************************************************************************ 
 �ļ�������ʵ�ֻ����ź��������Ķ��� 
 
 �����ˣ� Zouyf, 2006-6-8 
 
 �޸ļ�¼�� 
            
************************************************************************/

#pragma once
#include "Utility.h"
#include "SyncEvent.h"

#define CONSUME_THREAD_COUNT 2
#define MAX_SIGNAL_COUNT QUEUESIZE

typedef void (*pfnSQCallback)(void * pUserKey,const char * data,int size);

class CZSignalQueue
{
public:
	CZSignalQueue(int nConsumeThreadCount = 1,int size = 1000);
	~CZSignalQueue(void);
	int Start();
	void SetUserCallback(void * pUserKay,pfnSQCallback pfnCallback)
	{
		m_pUserKey = pUserKay;
		m_pfnCallback = pfnCallback;
	}
	int Push(const char * data,int size);
	void Stop();
	int GetSize() { return m_pPacketQueue->GetSize(); 	}
	int GetMaxSize() { return m_pPacketQueue->GetMaxSize(); }
private:
	static void* WrokThreadFun(LPVOID lpParam);
	void PostQueueData(); //���ϲ�Ͷ�ݶ�������
private:
	CSyncEvent m_hEventQuit;
	CSyncEvent m_hSemSignal;

	bool	   m_bQuit;

	int    m_nConsumeThreadCount;
	pthread_t * m_phWorkThread;
	
	void * m_pUserKey;
	pfnSQCallback m_pfnCallback;

	CPacketQueue * m_pPacketQueue;
};
