/************************************************************************ 
 文件描述：通用工具类，包括安全锁实现与基本的多线程安全队列
 
 创建人： Zouyf, 2006-6-8 
 
 修改记录： 
            
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
//#define PACKETSIZE 1600 //1500(网络实际发送数据最大长度) + 100(预留的应用数据头)
//#define PACKETSIZE_FILETRAN 1600*40 // 文件传输时使用

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
	int m_nHeadPos;  //指队列头元素
	int m_nTailPos;  //指队列尾元素
		
	int m_nQueueSize;
	int m_nBufSize;

	long m_nQueueCount; //队列元素个数
	CRITICAL_SECTION m_QueueLock;
};