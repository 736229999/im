#include "StdAfx.h"



#include ".\zsignalqueue.h"



DWORD WINAPI CZSignalQueue::WrokThreadFun(LPVOID lpParam)
{
	CZSignalQueue* pObj = (CZSignalQueue*)lpParam;
	pObj->PostQueueData();
	return 0;
}

CZSignalQueue::CZSignalQueue(int nConsumeThreadCount /* = 1 */,int size /*= 1000*/, int buffersize /*= 1600*/)
{
	m_nConsumeThreadCount = nConsumeThreadCount;
	m_hSemSignal = NULL;
	m_hEventQuit = NULL;

	m_phWorkThread = new HANDLE[m_nConsumeThreadCount];
	memset(m_phWorkThread,0,sizeof(HANDLE) * m_nConsumeThreadCount);

	m_pUserKey = NULL;
	m_pfnCallback = NULL;
	int i;
	for(i = 0; i < PRIOR_QUEUE_COUNT; i++)
	{
		m_PacketQueue[i] = new CPacketQueue(size, buffersize);
	}
	m_probability[0] = PROBABILITY_HIGH;
	m_probability[1] = PROBABILITY_COMMON;
	m_probability[2] = PROBABILITY_LOW;
	srand(::GetTickCount());


}

CZSignalQueue::~CZSignalQueue(void)
{
	int i;
	for(i = 0; i < PRIOR_QUEUE_COUNT; i++)
	{
		if(m_PacketQueue[i])
		{
			delete m_PacketQueue[i];
			m_PacketQueue[i] = NULL;
		}
	}
	if(m_phWorkThread)
	{
		delete [] m_phWorkThread;
		m_phWorkThread = NULL;
	}
}

void CZSignalQueue::PostQueueData()
{
	DWORD dwRet = 0;
	HANDLE arrHandle[2];
	arrHandle[1] = m_hEventQuit;
	arrHandle[0] = m_hSemSignal;
	int size = 0;
	CPacketQueue * pQueue = NULL;
	int i;
	while(1)
	{
		dwRet = WaitForMultipleObjects(2,arrHandle,false,INFINITE);
		if(dwRet == WAIT_FAILED)
		{
			//CLog::Log("CZSignalQueue",CLog::TYPE_ERROR,"WAIT_FAILED");
			break;
		}
		if(dwRet == WAIT_OBJECT_0 + 1)
		{
			//CLog::Log("CZSignalQueue",CLog::TYPE_ERROR,"Quit Event Set");
			break; //Quit Event Set
		}
		pQueue = NULL;

		/*pQueue = m_PacketQueue[0];
		size = pQueue->Pop(szBuffer,sizeof(szBuffer));
		if( size > 0)
		{
			m_pfnCallback(m_pUserKey,szBuffer,size);
		}*/

		for(i = 0;i < PRIOR_QUEUE_COUNT; i++)
		{
			// 根据优先级别获取任务
			if(m_PacketQueue[i]->GetSize() != 0)
			{
				if( (rand() % 100) < m_probability[i])
				{
					// 命中
					char *pBuffer = new char[m_PacketQueue[i]->GetBufferSize()];
					size = m_PacketQueue[i]->Pop(pBuffer,m_PacketQueue[i]->GetBufferSize());
					if( size > 0)
					{
						m_pfnCallback(m_pUserKey,pBuffer,size);
					}
					delete[] pBuffer;
					break;
				}
				if(pQueue == NULL)
				{
					pQueue = m_PacketQueue[i];
				}
			}
		}
		if(i == PRIOR_QUEUE_COUNT)
		{
			if(pQueue)
			{
				char *pBuffer = new char[pQueue->GetBufferSize()];
				size = pQueue->Pop(pBuffer,pQueue->GetBufferSize());
				if( size > 0)
				{
					m_pfnCallback(m_pUserKey,pBuffer,size);
				}
				delete[] pBuffer;
			}
		}
	}


}

int CZSignalQueue::Start()
{
	m_hSemSignal = CreateSemaphore(NULL,0,m_PacketQueue[0]->GetMaxSize() * PRIOR_QUEUE_COUNT ,NULL);
	m_hEventQuit = CreateEvent(NULL,TRUE,FALSE,NULL);
	for(int i = 0; i < m_nConsumeThreadCount; i++)
		m_phWorkThread[i] = CreateThread(NULL,0,WrokThreadFun,this,0,NULL);
	return 0;
}

int CZSignalQueue::Push(const char * data,int size,int nPrior /* = 0 */)
{
	long nPreSemapCount = 0;
	int nRet = 0;
	nRet = m_PacketQueue[nPrior]->Push(data,size);
	if(nRet == 0)
	{
		// 增加当前可用资源计数
		ReleaseSemaphore(m_hSemSignal,1,&nPreSemapCount);
#ifdef _DEBUG
		//printf("send pool packet= %d\n",nPreSemapCount);
#endif
	}
	return nRet;
}

void CZSignalQueue::Stop()
{
	int i;
	DWORD dwRet = 0;
	if(m_hEventQuit)
	{
		SetEvent(m_hEventQuit);
		for(i = 0; i < m_nConsumeThreadCount; i++)
		{
			if(m_phWorkThread[i] != NULL)
			{
				dwRet = ::WaitForSingleObject(m_phWorkThread[i],1000);
				if(dwRet != WAIT_OBJECT_0)
					TerminateThread(m_phWorkThread[i],-1);
				CloseHandle(m_phWorkThread[i]);
				m_phWorkThread[i] = NULL;
			}
		}

		CloseHandle(m_hEventQuit);
		m_hEventQuit = NULL;
	}

	if(m_hSemSignal)
	{
		CloseHandle(m_hSemSignal);
		m_hSemSignal = NULL;
	}

	for(i = 0; i < PRIOR_QUEUE_COUNT; i++)
	{
		m_PacketQueue[i]->Clear();
	}
	
}
