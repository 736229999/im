#include "stdafx.h"
#include "SyncRWLock.h"

CSyncRWLock::CSyncRWLock(void):m_EnableWrite(true,true),m_nReaderNum(0)
{
}

CSyncRWLock::~CSyncRWLock(void)
{
}

//д���߳��ڲ���ǰ��Ҫ��������ĺ���
void CSyncRWLock::WriteLock()
{
	m_WriteLock.Lock();
	m_EnableWrite.Wait();
}

//д���߳��ڲ�������ʱ��Ҫ��������ĺ���
void CSyncRWLock::WriteUnlock()
{
	m_WriteLock.Unlock();
}

//�����߳��ڲ���ǰ��Ҫ��������ĺ���
void CSyncRWLock::ReadLock()
{
	m_WriteLock.Lock();
	m_ReadLock.Lock();

	if( m_nReaderNum++ == 0 )
	{
		//�ж����̴߳��ڣ���ʱ��Ҫ��ֹд������
		m_EnableWrite.ResetEvent();
	}

	m_ReadLock.Unlock();
	m_WriteLock.Unlock();
}

//�����߳��ڲ�������ʱ��Ҫ��������ĺ���
void CSyncRWLock::ReadUnlock()
{
	m_ReadLock.Lock();

	if( m_nReaderNum-- == 1 )
	{
		//�����߳��Ѿ�ȫ���˳��ˣ���ʱ������д����
		m_EnableWrite.SetEvent();
	}

	m_ReadLock.Unlock();

}
