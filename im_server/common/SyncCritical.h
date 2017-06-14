#ifndef SYNC_CRITICAL_H
#define SYNC_CRITICAL_H

#include <pthread.h>

//使用临界区的实现多线程互斥访问的类

class CSyncCritical
{
public:
	CSyncCritical(void);
	~CSyncCritical(void);

	//获取独享访问，如果有别的线程在独享，这里会发生阻塞
	void Lock();
	void Unlock();

private:
	pthread_mutexattr_t _attr;
	pthread_mutex_t  m_lock;
	pthread_t		m_threadId;
	int				m_nSameThreadLockTime;
};


class CAutoLock
{
public:
	CAutoLock(CSyncCritical& lock):m_lock(lock)
	{
		m_lock.Lock();
	}

	CAutoLock(CSyncCritical* lock):m_lock(*lock)
	{
		m_lock.Lock();
	}

	~CAutoLock()
	{
		m_lock.Unlock();
	}
private:
	CSyncCritical&	m_lock;


};

#endif 	//SYNC_CRITICAL_H
