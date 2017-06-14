#pragma once

#include <semaphore.h>
//使用信号量的实现多线程互斥访问的类

#include "WinCommonForLinux.h"

class CSyncSemaphore
{
public:
	CSyncSemaphore(int nInitValue,int nMaxCount);
	~CSyncSemaphore(void);

	//获取信号量访问，这里可能会发生阻塞如果没有空闲信号量的时候
	void Lock();
	void Unlock();

private:
	sem_t m_handle;
};

