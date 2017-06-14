#pragma once

#include <semaphore.h>

/************************************************************************ 
文件描述：读写锁类的声明文件 

创建人：hechang
时间：	2006-6-9 
************************************************************************/

#include <vector>
using namespace std;

#include "WinCommonForLinux.h"

#define INFINITE 0xFFFFFFFF 
#define WAIT_FAILED ((DWORD)0xFFFFFFFF)
#define WAIT_OBJECT_0 0


//使用事件实现多线程互斥访问的类

class CSyncEvent
{
public:
	CSyncEvent(bool bManualReset,bool bInitialState);
	~CSyncEvent(void);

	void SetEvent();
	void ResetEvent();

	void Wait();

	int GetValue();

private:
	sem_t m_handle;

};

DWORD WaitForMultipleObjects( DWORD nCount, CSyncEvent *lpHandles, BOOL bWaitAll, DWORD dwMilliseconds );
DWORD WaitForSingleObject(CSyncEvent& hHandle, DWORD dwMilliseconds );

inline void ResetEvent(CSyncEvent& hHandle)
{
	hHandle.ResetEvent();
}
