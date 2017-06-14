#include <stdio.h>
#include "SyncCritical.h"



CSyncCritical::CSyncCritical(void)
{
	::pthread_mutexattr_init(&_attr);
	::pthread_mutexattr_settype(&_attr,PTHREAD_MUTEX_RECURSIVE);
	::pthread_mutex_init(&m_lock,&_attr);

	m_threadId = 0 ;

	m_nSameThreadLockTime = 0 ;
}

CSyncCritical::~CSyncCritical(void)
{
	::pthread_mutex_destroy(&m_lock);
}


void CSyncCritical::Lock()
{
//	pthread_t pid = pthread_self();

//	if( m_threadId != pid)
//	{
		::pthread_mutex_lock(&m_lock);
//		m_threadId = pid;
//		m_nSameThreadLockTime = 0 ;
//	}
//	else
//	{
//		++m_nSameThreadLockTime;
//	}

}

void CSyncCritical::Unlock()
{
//	pthread_t pid = pthread_self();
//	if( m_threadId == pid )
//	{
//		if( m_nSameThreadLockTime > 0 )
//		{
//			--m_nSameThreadLockTime;
//		}
//		else
//		{
			::pthread_mutex_unlock(&m_lock);
//		}
//	}	 
}

