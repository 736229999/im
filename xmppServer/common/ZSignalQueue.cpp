#include "stdafx.h"
#include "ZSignalQueue.h"

void* CZSignalQueue::WrokThreadFun(LPVOID lpParam)
{
	printf("CZSignalQueue::WrokThreadFun start run\n");
	CZSignalQueue* pObj = (CZSignalQueue*)lpParam;
	pObj->PostQueueData();
	return 0;
}

CZSignalQueue::CZSignalQueue(int nConsumeThreadCount /* = 1 */,int size ):
m_hEventQuit(false,false),m_hSemSignal(false,false)
{
	m_nConsumeThreadCount = nConsumeThreadCount;

	m_phWorkThread = new pthread_t[m_nConsumeThreadCount];
	memset(m_phWorkThread,0,sizeof(HANDLE) * m_nConsumeThreadCount);

	m_pUserKey = NULL;
	m_pfnCallback = NULL;
	m_bQuit = false;

	m_pPacketQueue = new CPacketQueue(size);


}

CZSignalQueue::~CZSignalQueue(void)
{
	delete m_pPacketQueue; m_pPacketQueue = NULL;
}

void CZSignalQueue::PostQueueData()
{
/*	
	DWORD dwRet = 0;
	CSyncEvent* arrHandle[2];
	arrHandle[0] = &m_hEventQuit;
	arrHandle[1] = &m_hSemSignal;
*/
	char szBuffer[PACKETSIZE] = {0};
	int size = 0;
	while(m_bQuit==false)
	{
/*		
		printf("CZSignalQueue::PostQueueData wait for data\n");
		dwRet = WaitForMultipleObjects(2,*arrHandle,false,INFINITE);
		if(dwRet == WAIT_FAILED)
		{
			printf("CZSignalQueue::PostQueueData WAIT_FAILED\n");
			//CLog::Log("CZSignalQueue",CLog::TYPE_ERROR,"WAIT_FAILED");
			break;
		}
		if(dwRet == WAIT_OBJECT_0 + 0)
		{
			printf("CZSignalQueue::PostQueueData WAIT_OBJECT_0 need quit\n");
			//CLog::Log("CZSignalQueue",CLog::TYPE_ERROR,"Quit Event Set");
			break; //Quit Event Set
		}

		printf("CZSignalQueue::PostQueueData get a data\n");
*/
		size = m_pPacketQueue->Pop(szBuffer,sizeof(szBuffer));
//		printf("CZSignalQueue::PostQueueData get a data with size=%d\n",size);
		if( size > 0)
		{
			m_pfnCallback(m_pUserKey,szBuffer,size);
		}
/*		
		} while (!m_pPacketQueue->IsEmpty());

		m_hSemSignal.ResetEvent();
*/
	}


}

int CZSignalQueue::Start()
{
	printf("CZSignalQueue::Start m_nConsumeThreadCount=%d\n",m_nConsumeThreadCount);
	m_bQuit = false;
	for(int i = 0; i < m_nConsumeThreadCount; i++)
		m_phWorkThread[i] = CreateThread(NULL,0,WrokThreadFun,this,0,NULL);
	return 0;
}

int CZSignalQueue::Push(const char * data,int size)
{

//	printf("CZSignalQueue::Push\n");
	int nRet = 0;
	nRet = m_pPacketQueue->Push(data,size);
/*
	if(nRet == 0)
	{
		// 增加当前可用资源计数
		printf("CZSignalQueue::Push begin setEvent\n");
		m_hSemSignal.SetEvent();
		printf("CZSignalQueue::Push end setEvent\n");
	}
	printf("CZSignalQueue::Push end with Ret:%d\n",nRet);
*/
	return nRet;
}

void CZSignalQueue::Stop()
{
	DWORD dwRet = 0;
	m_bQuit =true;
	m_hEventQuit.SetEvent();
	for(int i = 0; i < m_nConsumeThreadCount; i++)
	{
		if(m_phWorkThread[i] != 0)
		{
			dwRet = ::WaitForSingleObject(m_phWorkThread[i],1000);
			if(dwRet != WAIT_OBJECT_0)
				TerminateThread(m_phWorkThread[i],-1);
			CloseHandle(m_phWorkThread[i]);
			m_phWorkThread[i] = 0;
		}
	}


	m_pPacketQueue->Clear();
}
