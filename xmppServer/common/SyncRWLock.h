#pragma once

/************************************************************************ 
文件描述：读写锁类的声明文件 

创建人：hechang
时间：	2006-6-9 
************************************************************************/


#include "SyncCritical.h"
#include "SyncEvent.h"


//用于实现读写锁的类，规则为多个读操作可以同时进行．写操作一个时间只能
//有一个在进行．当有写操作时，读操作不能进行．当读操作在进行时，写操作
//也不能进行，但这时写操作将阻止后面的读操作，当前的读操作完后，才开始写操作
class CSyncRWLock
{
public:
	CSyncRWLock(void);
	~CSyncRWLock(void);

	void WriteLock();
	void WriteUnlock();

	void ReadLock();
	void ReadUnlock();


private:
	CSyncCritical		m_WriteLock;
	CSyncCritical		m_ReadLock;

	CSyncEvent			m_EnableWrite;
	int					m_nReaderNum;

};
