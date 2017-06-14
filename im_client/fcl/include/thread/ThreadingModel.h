/******************************************************************
** 文件名:	ThreadingModel.h
** 版  权:	
** 创建人:	
** 日  期:	
** 描  述: 	线程模型定义,可以把各种线程模型做为参数传给模版库

**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
*******************************************************************/

#ifndef __THREADING_MODEL_H__
#define __THREADING_MODEL_H__
#include "Config.h"

// 预定义
template<class mutex>	class CScopeLock;
template<class Type,class Mutex> class CVolatileValue;
class CNoneMutex;
class CMutexWrapper;

//////////////////////////////////////////////////////////////////////////////////////

/*
 *	线程模型: singleThread , multiThread
 *  功能:	  封装与多线程相关的一些操作和方案,做为模版参数传递给模版类,这样可以自由选择是否要实现多线程版本
 */

 template <class Mutex>
 class CThreadingModel
 {
 public:
	 // 互斥体
	 typedef Mutex mutex;

	 // 区域锁
	 //typedef typename CScopeLock<mutex> scope_lock;
 	 typedef CScopeLock<mutex> scope_lock;


	 // 多线程访问值
	 template<class Type>
	 class volatile_value : public CVolatileValue<Type,mutex>
	 {
	 };
 };

 class CSingleThread : public CThreadingModel<CNoneMutex>
{
 public:
	// 原子操作
	typedef uint32 IntType; 

	static IntType AtomicIncrement(volatile IntType& lval)
	{ return ++lval; }

	static IntType AtomicDecrement(volatile IntType& lval)
	{ return --lval; }

	static void AtomicAssign(volatile IntType & lval, IntType val)
	{ lval = val; }

	static void AtomicAssign(IntType & lval, volatile IntType & val)
	{ lval = val; }
};

class CMultiThread : public CThreadingModel<CMutexWrapper>
{
 public:
	static uint32 AtomicIncrement(volatile uint32& lval);

	static uint32 AtomicDecrement(volatile uint32& lval);

	static void AtomicAssign(volatile uint32 & lval, uint32 val);

	static void AtomicAssign(uint32 & lval, volatile uint32 & val);
};

//////////////////////////////////////////////////////////////////////////////////////

/*
 *  区域锁用法: 构造时加锁,释构时解锁
 *		void sampleFunction()
 *		{
 *			threadingModel::scope_lock guard(m_mutex);	// 这样sampleFunction函数就被锁住了
 *			(void)gurad;								// 这一句只是为了去掉编译器无意义的警告
 *			... ...;
 *		}
 *
 *  其他可能还有对象锁,类锁等
 */
template<class mutex>
class CScopeLock
{
public:
	explicit CScopeLock(mutex &_m) : m_mutex(_m)
	{
		m_mutex.do_lock();
	}
	~CScopeLock()
	{
		m_mutex.do_unlock();
	}

	void lock()
	{
		m_mutex.do_lock();
	}

	void unlock()
	{
		m_mutex.do_unlock();
	}

protected:
	mutex &	m_mutex;
};

//////////////////////////////////////////////////////////////////////////////////////

/*
 *	多线程安全值对象: 如果一个值需要在多个线程中引用可以直接把他定义成VolatileValue
 *	例子:
 *		threadingModel::volatile_value< int > m_nValue;
 *		int nTempValue = m_nValue.value() + 1;
 *		m_nValue.value() ++; .
 */

template<class Type,class Mutex>
class CVolatileValue
{
private:
	volatile Type m_value;
	Mutex	 m_mutex;

public:
	volatile Type & value()
	{
		CScopeLock<Mutex> guard(m_mutex);
		(void)guard;

		return m_value;
	}
};

//////////////////////////////////////////////////////////////////////////////////////

/*
 *	下面是互斥体,事件,线程等在不同平台的实现;.
 */

class FCL_CLASS CMutexWrapper
{
public:
	CMutexWrapper();
	virtual ~CMutexWrapper();
	void do_lock();
	void do_unlock();
private:
	///防止拷贝和赋值构造函数
	void operator = (const CMutexWrapper&);
	CMutexWrapper(const CMutexWrapper&);
	void * m_handle;
};

class FCL_CLASS CNoneMutex
{
public:
	void do_lock()
	{
	};

	void do_unlock()
	{
	};
};

class FCL_CLASS CEventWrapper
{
public:
	CEventWrapper();
	virtual ~CEventWrapper();
	void Active();
	void* m_handle;
};


//////////////////////////////////////////////////////////////////////////////////////

/*
 *	你可以不关心具体平台直接用下面的定义
 */

typedef CSingleThread		singleThread;
typedef CMultiThread		multiThread;

//typedef CEventWrapper		EVENT;	
//typedef CEventWrapper	*	EVENT_HANDLE;
//
//typedef CThreadWrapper		THREAD;
//typedef CThreadWrapper	*	THREAD_HANDLE;



#endif//__THREADING_MODEL_H__
