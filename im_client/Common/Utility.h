/************************************************************************ 
 �ļ�������ͨ�ù����࣬������ȫ��ʵ��������Ķ��̰߳�ȫ����
 
 �����ˣ� Zouyf, 2006-6-8 
 
 �޸ļ�¼�� 
            
************************************************************************/
#pragma once
#include "./ZObjectPool.h"
#include <list>
#include <set>
#include <map>

using namespace std;

template <class T>
class SafeList : public list<T>
{
public:
	SafeList()
	{
		InitializeCriticalSection(&m_lstLock);
	}
	~SafeList()
	{
		DeleteCriticalSection(&m_lstLock);
	}
	void Lock()
	{
		EnterCriticalSection(&m_lstLock);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_lstLock);
	}

	CRITICAL_SECTION m_lstLock;
};

template <class K, class T>
class SafeMap : public map<K,T>
{
public:
	SafeMap()
	{
		InitializeCriticalSection(&m_mapLock);
	}
	~SafeMap()
	{
		DeleteCriticalSection(&m_mapLock);
	}
	void Lock()
	{
		EnterCriticalSection(&m_mapLock);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_mapLock);
	}
	CRITICAL_SECTION m_mapLock;
};


template <class T>
class SafeSet : public set<T>
{
public:
	SafeSet()
	{
		InitializeCriticalSection(&m_setLock);
	}
	~SafeSet()
	{
		DeleteCriticalSection(&m_setLock);
	}
	void Lock()
	{
		EnterCriticalSection(&m_setLock);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_setLock);
	}
	CRITICAL_SECTION m_setLock;
};

template <class T>
class SmartList : public SafeList<T *>
{
public:
	T * NewObject()
	{
		return m_pool.AllocateObj();
	}
	void DeleteObject(T * pObj)
	{
		m_pool.ReleaseObj(pObj);
	}
private:
	CZObjectPool<T> m_pool;
};

class CMyLock
{
public:
	CMyLock(CRITICAL_SECTION *pSec)
	{
		m_pSec = pSec;
		::EnterCriticalSection(pSec);
	}

	~CMyLock()
	{
		::LeaveCriticalSection(m_pSec);
	}

private:
	CRITICAL_SECTION *m_pSec;
};


//#define QUEUESIZE 1000
#define QUEUESIZE 1000
//#define PACKETSIZE 1600 //1500(����ʵ�ʷ���������󳤶�) + 100(Ԥ����Ӧ������ͷ)
//#define PACKETSIZE_FILETRAN 1600*40 // �ļ�����ʱʹ��

//data format : len(int) + data
class CPacketQueue
{
public:
	CPacketQueue(int size, int buffersize);
	~CPacketQueue(void);
	int Push(const char * data,int size);
	int Pop(char * buf,int len);
	bool IsEmpty()
	{
		return m_nQueueCount == 0;
	}
	void Clear()
	{
		m_nHeadPos = 0;
		m_nTailPos = -1;
		m_nQueueCount = 0;
	}

	long GetSize() { 
		CMyLock MyLock(&m_QueueLock);
		return m_nQueueCount;
	}

	int	 GetMaxSize() { return m_nQueueSize;	}

	int  GetBufferSize()	{	return 	m_nBufSize;	}
protected:
	void Init(int size, int bufsize);

private:
	char ** m_pQueueBuf;
	int m_nHeadPos;  //ָ����ͷԪ��
	int m_nTailPos;  //ָ����βԪ��
		
	int m_nQueueSize;
	int m_nBufSize;

	long m_nQueueCount; //����Ԫ�ظ���
	CRITICAL_SECTION m_QueueLock;
};