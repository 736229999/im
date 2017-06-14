/************************************************************************ 
 文件描述：实现基于信号量驱动的队列 
 
 创建人： Zouyf, 2006-6-8 
 
 修改记录： 
            
************************************************************************/

#pragma once
#include "Utility.h"

#define CONSUME_THREAD_COUNT 2
#define MAX_SIGNAL_COUNT QUEUESIZE

//
// 优先级(高):		60%的命中率
// 优先级(正常):	24%的命中率
// 优先级(低):		16%的命中率
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
	void PostQueueData(); //向上层投递队列数据
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
