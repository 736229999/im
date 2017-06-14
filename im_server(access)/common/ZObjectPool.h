/************************************************************************ 
 �ļ�������ʵ�ֶ����ģ���� 
 
 �����ˣ� Zouyf, 2006-6-8 
 
 �޸ļ�¼�� 
            
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
		// ��¼��Դ��������λ
		m_mapPt[pObj] = 0;
#endif
		return pObj;
	}
	void ReleaseObj(T * pObj)
	{
		CAutoLock MyLock(&m_poolLock);

#ifndef QUICK_DATA_POOL 
		// Ӧ��֤��ָ��ĺϷ��ԣ����ظ���Ұָ�룿��
		map<T*,int>::iterator it;
		it = m_mapPt.find(pObj);
		// ���ж��Ƿ���Ұָ��
		if(it == m_mapPt.end())
			return;
		// �ж�ָ���Ƿ��ظ��ͷ�
		if(it->second == 0)
			it->second = 1;
		else
			return;
#endif
		m_lstRecycle.push_back(pObj);
	}
private:
	CSyncCritical		m_poolLock;
	list<T *>			m_lstReclaim;	// �ڴ��������
	list<T *>			m_lstRecycle;	// ѭ����������
#ifndef QUICK_DATA_POOL 
	map<T*,int>			m_mapPt;		// �ڴ�ָ����Դ�ֵ�
#endif
};
