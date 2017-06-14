#include "StdAfx.h"
#include ".\utility.h"

CPacketQueue::CPacketQueue(int size, int bufsize)
{
	Init(size, bufsize);
}

void CPacketQueue::Init(int size, int bufsize)
{
	InitializeCriticalSection(&m_QueueLock);

	m_nQueueSize = size;
	m_nBufSize = bufsize;
	m_nHeadPos = 0;
	m_nTailPos = -1;
	m_nQueueCount = 0;

	m_pQueueBuf = new char*[m_nQueueSize] ;
	for(int i = 0; i < m_nQueueSize; i++)
	{
		m_pQueueBuf[i] = new char[m_nBufSize];
		memset(m_pQueueBuf[i],0,m_nBufSize);
	}
}


CPacketQueue::~CPacketQueue(void)
{
	for(int i = 0; i < m_nQueueSize; i++)
	{
		delete[] m_pQueueBuf[i];
	}

	delete[] m_pQueueBuf;
	
	DeleteCriticalSection(&m_QueueLock);
}

int CPacketQueue::Push(const char * data,int size)
{
	assert(size <= m_nBufSize - sizeof(int));
	if(size > m_nBufSize - sizeof(int))
		return -1;

	CMyLock MyLock(&m_QueueLock);
	if(m_nQueueCount == m_nQueueSize)
		return  -2;
	m_nTailPos++;
	m_nTailPos %= m_nQueueSize;

	m_nQueueCount++;

	memcpy(m_pQueueBuf[m_nTailPos],&size,sizeof(int));
	memcpy(m_pQueueBuf[m_nTailPos] + sizeof(int),data,size);
	return 0;
}

int CPacketQueue::Pop(char * buf,int len)
{
	assert(len >= m_nBufSize - sizeof(int));
	if(len < m_nBufSize - sizeof(int))
		return -1;
	CMyLock MyLock(&m_QueueLock);
	if(IsEmpty())
		return 0;
	int size = 0;
	memcpy(&size,m_pQueueBuf[m_nHeadPos],sizeof(int));
	memcpy(buf,m_pQueueBuf[m_nHeadPos] + sizeof(int),size);

	m_nHeadPos++;
	m_nHeadPos %= m_nQueueSize;		
	m_nQueueCount--;
	
	return size;
}