#pragma once

#include <semaphore.h>
//ʹ���ź�����ʵ�ֶ��̻߳�����ʵ���

#include "WinCommonForLinux.h"

class CSyncSemaphore
{
public:
	CSyncSemaphore(int nInitValue,int nMaxCount);
	~CSyncSemaphore(void);

	//��ȡ�ź������ʣ�������ܻᷢ���������û�п����ź�����ʱ��
	void Lock();
	void Unlock();

private:
	sem_t m_handle;
};

