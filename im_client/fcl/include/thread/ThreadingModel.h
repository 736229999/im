/******************************************************************
** �ļ���:	ThreadingModel.h
** ��  Ȩ:	
** ������:	
** ��  ��:	
** ��  ��: 	�߳�ģ�Ͷ���,���԰Ѹ����߳�ģ����Ϊ��������ģ���

**************************** �޸ļ�¼ ******************************
** �޸���: 
** ��  ��: 
** ��  ��: 
*******************************************************************/

#ifndef __THREADING_MODEL_H__
#define __THREADING_MODEL_H__
#include "Config.h"

// Ԥ����
template<class mutex>	class CScopeLock;
template<class Type,class Mutex> class CVolatileValue;
class CNoneMutex;
class CMutexWrapper;

//////////////////////////////////////////////////////////////////////////////////////

/*
 *	�߳�ģ��: singleThread , multiThread
 *  ����:	  ��װ����߳���ص�һЩ�����ͷ���,��Ϊģ��������ݸ�ģ����,������������ѡ���Ƿ�Ҫʵ�ֶ��̰߳汾
 */

 template <class Mutex>
 class CThreadingModel
 {
 public:
	 // ������
	 typedef Mutex mutex;

	 // ������
	 //typedef typename CScopeLock<mutex> scope_lock;
 	 typedef CScopeLock<mutex> scope_lock;


	 // ���̷߳���ֵ
	 template<class Type>
	 class volatile_value : public CVolatileValue<Type,mutex>
	 {
	 };
 };

 class CSingleThread : public CThreadingModel<CNoneMutex>
{
 public:
	// ԭ�Ӳ���
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
 *  �������÷�: ����ʱ����,�͹�ʱ����
 *		void sampleFunction()
 *		{
 *			threadingModel::scope_lock guard(m_mutex);	// ����sampleFunction�����ͱ���ס��
 *			(void)gurad;								// ��һ��ֻ��Ϊ��ȥ��������������ľ���
 *			... ...;
 *		}
 *
 *  �������ܻ��ж�����,������
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
 *	���̰߳�ȫֵ����: ���һ��ֵ��Ҫ�ڶ���߳������ÿ���ֱ�Ӱ��������VolatileValue
 *	����:
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
 *	�����ǻ�����,�¼�,�̵߳��ڲ�ͬƽ̨��ʵ��;.
 */

class FCL_CLASS CMutexWrapper
{
public:
	CMutexWrapper();
	virtual ~CMutexWrapper();
	void do_lock();
	void do_unlock();
private:
	///��ֹ�����͸�ֵ���캯��
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
 *	����Բ����ľ���ƽֱ̨��������Ķ���
 */

typedef CSingleThread		singleThread;
typedef CMultiThread		multiThread;

//typedef CEventWrapper		EVENT;	
//typedef CEventWrapper	*	EVENT_HANDLE;
//
//typedef CThreadWrapper		THREAD;
//typedef CThreadWrapper	*	THREAD_HANDLE;



#endif//__THREADING_MODEL_H__
