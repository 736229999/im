/************************************************************************ 
 文件描述：实现对象池模板类 
 
 创建人： Zouyf, 2006-6-8 
 
 修改记录： 
            
************************************************************************/
#pragma once


#define NDEBUG

#include "SyncCritical.h"

#include <list>
#include <map>
#include <assert.h>
using namespace std;




#define QUICK_DATA_POOL

template <class T>
class CZObjectPool
{
public:
	CZObjectPool(void)
	{
	}
	~CZObjectPool(void)
	{
		Clear();
	}

	void Clear()
	{
		CAutoLock MyLock(&m_poolLock);
		T * pObj = NULL;
		typename list<T *>::iterator it;
		for( it = m_lstReclaim.begin();it != m_lstReclaim.end();it++)
		{
			pObj = (*it);
			delete pObj; pObj = NULL;
		}
		m_lstReclaim.clear();
		m_lstRecycle.clear();
#ifndef QUICK_DATA_POOL 
		m_mapPt.clear();
#endif
	}

	T * AllocateObj()
	{
		T * pObj = NULL;
		CAutoLock MyLock(&m_poolLock);
		if(m_lstRecycle.empty())
		{
			pObj = new T();
			assert(pObj);
			m_lstReclaim.push_back(pObj);
		}
		else
		{
			pObj = m_lstRecycle.front();
#ifdef _DEBUG
			//memset(pObj,0,sizeof(T));
#endif
			m_lstRecycle.pop_front();
		}
#ifndef QUICK_DATA_POOL 
		// 记录资源或重新置位
		m_mapPt[pObj] = 0;
#endif
		return pObj;
	}
	void ReleaseObj(T * pObj)
	{
		CAutoLock MyLock(&m_poolLock);

#ifndef QUICK_DATA_POOL 
		// 应验证该指针的合法性！（重复，野指针？）
		map<T*,int>::iterator it;
		it = m_mapPt.find(pObj);
		// 先判断是否是野指针
		if(it == m_mapPt.end())
			return;
		// 判断指针是否重复释放
		if(it->second == 0)
			it->second = 1;
		else
			return;
#endif
		m_lstRecycle.push_back(pObj);
	}
private:
	CSyncCritical		m_poolLock;
	list<T *>			m_lstReclaim;	// 内存回收连表
	list<T *>			m_lstRecycle;	// 循环利用连表
#ifndef QUICK_DATA_POOL 
	map<T*,int>			m_mapPt;		// 内存指针资源字典
#endif
};
