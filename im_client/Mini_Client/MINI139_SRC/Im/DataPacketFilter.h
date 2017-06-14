// ----------------------------------------------------------------
// 文件:DataPacketFilter.h
// 版本: 
// 作者: xy
// 日期: 2006-10-20
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

//
// 单元测试开关
//
//#define __UNIT_TEST

#define XY_BEG namespace xy{
#define XY_END }

#include <list>
#include <assert.h>
#include <Windows.h>

#ifdef __UNIT_TEST
#include <iostream>
using namespace std;
#endif

XY_BEG




template <class T>
class XPool
{
public:
	XPool(void)
	{
		InitializeCriticalSection(&m_poolLock);
	}
	~XPool(void)
	{
		Clear();
		DeleteCriticalSection(&m_poolLock);
	}

	void Clear()
	{
		EnterCriticalSection(&m_poolLock);
		T * pObj = NULL;
		list<T *>::iterator it;
		for(it = m_lstReclaim.begin();
			it != m_lstReclaim.end();
			it++)
		{
			pObj = (*it);
			delete pObj; 
			pObj = NULL;
		}
		m_lstReclaim.clear();
		m_lstRecycle.clear();
		LeaveCriticalSection(&m_poolLock);
	}

	T * NewObj()
	{
		EnterCriticalSection(&m_poolLock);
		T * pObj = NULL;
		if(m_lstRecycle.empty())
		{
			pObj = new T();
			assert(pObj);
			m_lstReclaim.push_back(pObj);
		}
		else
		{
			pObj = m_lstRecycle.front();
			m_lstRecycle.pop_front();
		}
		LeaveCriticalSection(&m_poolLock);
		return pObj;
	}
	void DeleteObj(T * pObj)
	{
		EnterCriticalSection(&m_poolLock);
		m_lstRecycle.push_back(pObj);
		LeaveCriticalSection(&m_poolLock);
	}
private:
	CRITICAL_SECTION		m_poolLock;
	std::list<T *>			m_lstReclaim;	// 内存回收连表
	std::list<T *>			m_lstRecycle;	// 循环利用连表
};

template <class T>
class XList : public std::list<T>
{
public:
	XList()
	{
		InitializeCriticalSection(&m_lstLock);
	}
	~XList()
	{
		DeleteCriticalSection(&m_lstLock);
	}
	void Lock()
	{
		EnterCriticalSection(&m_lstLock);
	}
	void UnLock()
	{
		LeaveCriticalSection(&m_lstLock);
	}
protected:

	CRITICAL_SECTION m_lstLock;
};



template <class T>
class XFilter
{
public:
	// 相同返回0,大于返回1,小于返回-1
	typedef int (*FncComp)(const T & v1,const T & v2);
	//------------------------------------------------------
	// 构造函数:	nMaxCache为保存缓存数据包最大个数
	//------------------------------------------------------
	XFilter(int nMaxCache = 100)
	{ assert(nMaxCache > 0); m_nMaxCache = nMaxCache; };
	//------------------------------------------------------
	// 析构函数
	//------------------------------------------------------
	~XFilter(void)
	{ };

	size_t Size()
	{
		size_t s;
		m_lstCache.Lock();
		s = m_lstCache.size();
		m_lstCache.UnLock();
		return s; 
	}
	//------------------------------------------------------
	// 判断一个数据包是否在缓存中已经存在
	//------------------------------------------------------
	bool	IsRepeat(const T & dataPacket,FncComp cmp)
	{
		XList<T*>::iterator it,itE;
		m_lstCache.Lock();
		it = m_lstCache.begin();
		itE = m_lstCache.end();
		for(;it != itE;it++)
		{
			if(cmp(*(*it),dataPacket) == 0)
				return true;
		}
		m_lstCache.UnLock();
		return false;
	};
	//------------------------------------------------------
	// 判断一个数据包是否在缓存中已经存在
	//------------------------------------------------------
	bool	IsRepeat(const T & dataPacket)
	{
		XList<T*>::iterator it,itE;
		m_lstCache.Lock();
		it = m_lstCache.begin();
		itE = m_lstCache.end();
		for(;it != itE;it++)
		{
			if(*(*it) == dataPacket)
				return true;
		}
		m_lstCache.UnLock();
		return false;
	}
	//------------------------------------------------------
	// 添加一个数据包到缓存中
	//------------------------------------------------------
	void	Add(const T & dataPacket)
	{
		T* p = m_data.NewObj();
		assert(p);
		if(p != NULL)
		{
			*p = dataPacket;
			m_lstCache.Lock();
			if( (int)m_lstCache.size() > m_nMaxCache)
			{
				m_data.DeleteObj(*(--m_lstCache.end()));
				// 如果缓冲满，则删除链表最后一个对象
				m_lstCache.pop_back();
			}
			// 添加到链表首
			m_lstCache.push_front(p);
			m_lstCache.UnLock();
		}
	};
	//------------------------------------------------------
	// 设置缓冲最大个数
	//------------------------------------------------------
	void	SetMax(int nMax)
	{	assert(nMax > 0); 
		if(nMax < (int)Size())
		{
			// 新设置的缓冲比之前小,则缩小缓冲区
			int i;
			m_lstCache.Lock();
			for(i = m_nMaxCache;i > nMax;i--)
			{
				m_data.DeleteObj(*(--m_lstCache.end()));
				m_lstCache.pop_back();
			}
			m_lstCache.UnLock();
		}
		m_nMaxCache = nMax;
	};
	//-------------------------------------------------------
	// 重置缓存的数据 
	//-------------------------------------------------------
	void	Reset()
	{
		XList<T*>::iterator it,itE;
		m_lstCache.Lock();
		it = m_lstCache.begin();
		itE = m_lstCache.end();
		for(;it != itE;it++)
		{
			m_data.DeleteObj(*it);
		}
		m_lstCache.clear();
		m_lstCache.UnLock();
	}

protected:

	XList<T*>		m_lstCache;		// 缓冲的数据包
	int				m_nMaxCache;	// 最大缓冲个数

private:
	
	XPool<T>		m_data;			// 数据池
};

#ifdef __UNIT_TEST
struct TEST_DATA
{
	int n;
	bool operator  == (const TEST_DATA & d)
	{
		return n == d.n;
	}
	static int Cmp(const TEST_DATA& d1,const TEST_DATA& d2)
	{
		if(d1.n == d2.n)
			return 0;
		return 1;
	}
	static void Print()
	{
		int n = 50;
		int i;
		TEST_DATA d;
		XFilter<TEST_DATA> dic(n);
		for(i = 0;i < n;i++)
		{
			d.n = i;
			dic.Add(d);
		}

		dic.SetMax(40);

		for(i = 0;i< 100;i++)
		{
			d.n = i;
			if(dic.IsRepeat(d))
			{
				cout<<i<<"repeat"<<endl;
			}
			else
			{
				cout<<i<<"not r"<<endl;
			}
		}
		cout<<"-----------------"<<endl;
		cout<<"size = "<<dic.Size()<<endl;
		//dic.Reset();
		cout<<"size = "<<dic.Size()<<endl;
		cout<<"-----------------"<<endl;
		for(i = 0;i< 100;i++)
		{
			d.n = i;
			if(dic.IsRepeat(d,Cmp))
			{
				cout<<i<<"repeat"<<endl;
			}
			else
			{
				cout<<i<<"not r"<<endl;
			}
		}
	};
};

#endif

XY_END