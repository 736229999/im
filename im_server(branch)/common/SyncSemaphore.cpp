#include "stdafx.h"
#include "SyncSemaphore.h"

CSyncSemaphore::CSyncSemaphore(int nInitValue,int nMaxCount)
{
	sem_init(&m_handle,0,nInitValue);
}

CSyncSemaphore::~CSyncSemaphore(void)
{
	sem_destroy(&m_handle);
}


void CSyncSemaphore::Lock()
{

	sem_wait(&m_handle);
}

void CSyncSemaphore::Unlock()
{
	sem_post(&m_handle);
}

