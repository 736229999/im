/************************************************************************ 
 文件描述：hash表模板类，内部实现对象池管理，由内部分配对象
 
 创建人： Zouyf, 2006-6-9 
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "Utility.h"
#include "ZObjectPool.h"
#include "SyncCritical.h"


#define   HASHELEM  20000 

template <class T>
class CHashTable
{
public:
	struct tagLink
	{
		tagLink * next;
		uint32 id;
		T * pObj;
		tagLink()
		{
			next = NULL;
			id = 0;pObj = NULL;
		}
	};

	CHashTable(int hashElem = HASHELEM)
	{
		m_hashElem = HASHELEM;
		m_tableLock = new CSyncCritical[m_hashElem];
		m_hash = new tagLink * [m_hashElem];

		m_nElemCount = 0;
		memset(m_hash,0,m_hashElem * sizeof(tagLink *));
	}
	~CHashTable(void)
	{
		if( m_tableLock )
		{
			delete[] m_tableLock;
		}
		

		if( m_hash )
		{
			tagLink * pNode, * pDelNode;
			for(int i = 0; i < m_hashElem; ++i)
			{
				pNode = m_hash[i];
				while(pNode)
				{
					pDelNode = pNode;
					pNode = pNode->next;
					delete pDelNode;
				}
				m_hash[i] = NULL;
			}
			delete[] m_hash;
		}


	}
	T * AddHash(uint32 id)
	{
		CAutoLock MyLock(m_tableLock[id % m_hashElem]);

		T * pObj = NULL;
		pObj = Find(id);
		if(pObj)
			return pObj;
		
		tagLink *  pNode = new tagLink();
		pNode->id = id;
		pNode->pObj = m_ObjPool.AllocateObj();

		uint32 index = id % m_hashElem;
		if(m_hash[index])
		{
			pNode->next = m_hash[index];
			m_hash[index] = pNode;
		}
		else
		{
			m_hash[index] = pNode;
		}
		++m_nElemCount;
		return pNode->pObj;
	}
	bool DelHash(uint32 id)
	{
		tagLink * pNode, * pNodePre;
		uint32 index = id % m_hashElem;

		CAutoLock MyLock(m_tableLock[id % m_hashElem]);

		pNodePre = NULL;
		pNode = m_hash[index];
		while(pNode)
		{
			if(id == pNode->id)
			{
				if(pNodePre == NULL)
				{
					m_hash[index] = pNode->next;
				}
				else
				{
					pNodePre->next = pNode->next;
				}
				m_ObjPool.ReleaseObj(pNode->pObj);
				delete pNode;pNode = NULL;
				--m_nElemCount;
				break;
			}
			pNodePre = pNode;
			pNode = pNode->next;
		}
		return true;
	}
	T * Find(uint32 id)
	{
		tagLink * pNode;
		uint32 index = id % m_hashElem;	

		CAutoLock MyLock(m_tableLock[id % m_hashElem]);

		pNode = m_hash[index];
		while(pNode)
		{
			if(id == pNode->id)
			{
				return pNode->pObj;
			}
			pNode = pNode->next;
		}
		return NULL;
	}
	uint32 GetElemCount()
	{
		return m_nElemCount;
	}
public:
	tagLink ** m_hash;
private:
	CZObjectPool<T> m_ObjPool;
	//tagLink * m_hash;
	CSyncCritical * m_tableLock;
	uint32 m_nElemCount;
	int m_hashElem;
};
