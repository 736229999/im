/************************************************************************ 
 文件描述：通用工具类，包括安全锁实现与基本的多线程安全队列
 
 创建人： Zouyf, 2006-6-8 
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "ZObjectPool.h"
#include <list>
#include <set>
#include <map>

using namespace std;

#include "WinCommonForLinux.h"
#include "SyncCritical.h"
#include "SyncSemaphore.h"

class CSyncSemaphore;

template <class T>
class SafeList : public list<T>
{
public:
	SafeList()
	{
	}
	~SafeList()
	{
	}
	void Lock()
	{
		m_lstLock.Lock();
	}
	void UnLock()
	{
		m_lstLock.Unlock();
	}
 
	CSyncCritical m_lstLock;
};

template <class K, class T>
class SafeMap : public map<K,T>
{
public:
	SafeMap()
	{
	}
	~SafeMap()
	{
	}
	void Lock()
	{
		m_mapLock.Lock();
	}
	void UnLock()
	{
		m_mapLock.Unlock();
	}
	CSyncCritical m_mapLock;
};


template <class T>
class SafeSet : public set<T>
{
public:
	SafeSet()
	{
	}
	~SafeSet()
	{
	}
	void Lock()
	{
		m_setLock.Lock();
	}
	void UnLock()
	{
		m_setLock.Unlock();
	}
	CSyncCritical m_setLock;
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


#define QUEUESIZE 1000
#ifndef IM_TRANSFER
#define PACKETSIZE 1600 //1500(网络实际发送数据最大长度) + 100(预留的应用数据头)
#else 
#define PACKETSIZE 1600*50
#endif

//data format : len(int) + data
class CPacketQueue
{
public:
	CPacketQueue(void);
	CPacketQueue(int size);
	~CPacketQueue(void);
	int Push(const char * data,unsigned int size);
	int Pop(char * buf,unsigned int len);
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

	int GetSize() { return m_nQueueCount;	}

	int	 GetMaxSize() { return m_nQueueSize;	}
protected:
	void Init(int size);

private:
	char ** m_pQueueBuf;
	int m_nHeadPos;  //指队列头元素
	int m_nTailPos;  //指队列尾元素
		
	int m_nQueueSize;

	int m_nQueueCount; //队列元素个数
	CSyncSemaphore*     m_pDataLockFull;    //数据的同步互斥锁,用于控制是否缓冲满了
	CSyncSemaphore*     m_pDataLockEmpty;   //数据的同步互斥锁,用于控制是否缓冲为空
	CSyncCritical m_QueueLock;
};

