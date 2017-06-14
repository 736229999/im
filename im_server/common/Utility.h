/************************************************************************ 
 �ļ�������ͨ�ù����࣬������ȫ��ʵ��������Ķ��̰߳�ȫ����
 
 �����ˣ� Zouyf, 2006-6-8 
 
 �޸ļ�¼�� 
            
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
#define PACKETSIZE 1600 //1500(����ʵ�ʷ���������󳤶�) + 100(Ԥ����Ӧ������ͷ)
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
	int m_nHeadPos;  //ָ����ͷԪ��
	int m_nTailPos;  //ָ����βԪ��
		
	int m_nQueueSize;

	int m_nQueueCount; //����Ԫ�ظ���
	CSyncSemaphore*     m_pDataLockFull;    //���ݵ�ͬ��������,���ڿ����Ƿ񻺳�����
	CSyncSemaphore*     m_pDataLockEmpty;   //���ݵ�ͬ��������,���ڿ����Ƿ񻺳�Ϊ��
	CSyncCritical m_QueueLock;
};

