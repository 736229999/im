
#include "Utility.h"

CPacketQueue::CPacketQueue(void)
{
	Init(QUEUESIZE);
}

CPacketQueue::CPacketQueue(int size)
{
	Init(size);
}

void CPacketQueue::Init(int size)
{
	m_nQueueSize = size;
	m_nHeadPos = 0;
	m_nTailPos = -1;
	m_nQueueCount = 0;

	m_pQueueBuf = new char*[m_nQueueSize] ;
	memset(m_pQueueBuf, 0, m_nQueueSize*sizeof(char*));
	for(int i = 0; i < m_nQueueSize; i++)
	{
		m_pQueueBuf[i] = new char[PACKETSIZE];
		memset(m_pQueueBuf[i],0,PACKETSIZE);
	}
	
	m_pDataLockFull = new CSyncSemaphore(size,size);
	m_pDataLockEmpty = new CSyncSemaphore(0,size);
}


CPacketQueue::~CPacketQueue(void)
{
	for(int i = 0; i < m_nQueueSize; i++)
	{
		delete[] m_pQueueBuf[i];
	}

	delete[] m_pQueueBuf;
	
}

int CPacketQueue::Push(const char * data,unsigned int size)
{
	assert(size <= PACKETSIZE - sizeof(int));
	if(size > PACKETSIZE - sizeof(int))
		return -1;

	//CAutoLock MyLock(m_QueueLock);
	
	m_pDataLockFull->Lock();
	m_QueueLock.Lock();

	if(m_nQueueCount >= m_nQueueSize)
	{
		m_QueueLock.Unlock();
		return  -2;
	}
	
	m_nTailPos++;
	m_nTailPos %= m_nQueueSize;

	m_nQueueCount++;

	memcpy(m_pQueueBuf[m_nTailPos],&size,sizeof(int));
	if(size > 0)
		memcpy(m_pQueueBuf[m_nTailPos] + sizeof(int),data,size);

	
	m_pDataLockEmpty->Unlock();
	m_QueueLock.Unlock();
    	
	return 0;
}

int CPacketQueue::Pop(char * buf,unsigned int len)
{
	assert(len >= PACKETSIZE - sizeof(int));
	if(len < PACKETSIZE - sizeof(int))
		return -1;

	//CAutoLock MyLock(&m_QueueLock);

	m_pDataLockEmpty->Lock();
	m_QueueLock.Lock();

	if(IsEmpty())
	{
		m_QueueLock.Unlock();
		return 0;
	}
	
	int size = 0;
	memcpy(&size,m_pQueueBuf[m_nHeadPos],sizeof(int));
	if(size > 0)
		memcpy(buf,m_pQueueBuf[m_nHeadPos] + sizeof(int),size);

	m_nHeadPos++;
	m_nHeadPos %= m_nQueueSize;		
	m_nQueueCount--;

	
	m_pDataLockFull->Unlock();	
	m_QueueLock.Unlock();
    	
	return size;
}

