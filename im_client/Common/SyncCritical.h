#pragma once


//使用临界区的实现多线程互斥访问的类

class CSyncCritical
{
public:
	CSyncCritical(void);
	~CSyncCritical(void);

	//获取独享访问，如果有别的线程在独享，这里会发生阻塞
	void Lock();
	void Unlock();

	CRITICAL_SECTION * Get()
	{ return &m_sect;};

private:
	CRITICAL_SECTION	m_sect;
};

