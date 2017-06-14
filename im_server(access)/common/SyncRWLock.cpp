#include "stdafx.h"
#include "SyncRWLock.h"

CSyncRWLock::CSyncRWLock(void):m_EnableWrite(true,true),m_nReaderNum(0)
{
}

CSyncRWLock::~CSyncRWLock(void)
{
}

//写的线程在操作前需要调用下面的函数
void CSyncRWLock::WriteLock()
{
	m_WriteLock.Lock();
	m_EnableWrite.Wait();
}

//写的线程在操作结束时需要调用下面的函数
void CSyncRWLock::WriteUnlock()
{
	m_WriteLock.Unlock();
}

//读的线程在操作前需要调用下面的函数
void CSyncRWLock::ReadLock()
{
	m_WriteLock.Lock();
	m_ReadLock.Lock();

	if( m_nReaderNum++ == 0 )
	{
		//有读的线程存在，这时候要禁止写操作了
		m_EnableWrite.ResetEvent();
	}

	m_ReadLock.Unlock();
	m_WriteLock.Unlock();
}

//读的线程在操作结束时需要调用下面的函数
void CSyncRWLock::ReadUnlock()
{
	m_ReadLock.Lock();

	if( m_nReaderNum-- == 1 )
	{
		//读的线程已经全部退出了，这时候允许写操作
		m_EnableWrite.SetEvent();
	}

	m_ReadLock.Unlock();

}
