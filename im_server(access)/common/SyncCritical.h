#ifndef SYNC_CRITICAL_H
#define SYNC_CRITICAL_H

#include <pthread.h>

//ʹ���ٽ�����ʵ�ֶ��̻߳�����ʵ���

class CSyncCritical
{
public:
	CSyncCritical(void);
	~CSyncCritical(void);

	//��ȡ������ʣ�����б���߳��ڶ�������ᷢ������
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
