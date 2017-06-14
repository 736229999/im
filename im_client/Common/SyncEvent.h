#pragma once
/************************************************************************ 
文件描述：读写锁类的声明文件 

创建人：hechang
时间：	2006-6-9 
************************************************************************/



//使用事件实现多线程互斥访问的类

class CSyncEvent
{
public:
	CSyncEvent(bool bManualReset,bool bInitialState);
	~CSyncEvent(void);

	void SetEvent();
	void ResetEvent();

	void Wait();

private:
	HANDLE m_handle;

};
