#pragma once

#include "PublicType.h"
#include "Utility.h"
#include "SyncCritical.h"

//实现一个支持快速查找，占用固定大小内存（避免动态分配内存），
//可快速遍历，快速添加与删除的数据结构模板，基本技术是使用hash与list的
//结合，所以叫hashlist

typedef unsigned int     uint32;

//hashlist的基本组成元素的基类，为定义元素的具体成员数据，必须从该基类派生
class HashListItem
{
public:
	HashListItem();
	virtual ~HashListItem();

	virtual uint32 GetHashNum() = 0 ;
	virtual bool IsMatch(HashListItem& CompareItem)	= 0;
	virtual	void InitIndex(HashListItem& IndexItem) = 0;


	HashListItem*  m_hashPre;
	HashListItem*	m_hashNext;

	HashListItem*  m_ListPre;
	HashListItem*	m_ListNext;
};


template <class T>
class CHashList
{
public:
	CHashList(int size = 2000)
	{
		m_HashSize = size;
		m_itemCount = 0;
		m_pHashIndex = new T*[m_HashSize];
		memset(m_pHashIndex,0,m_HashSize*sizeof(T*));

		m_pHead = NULL;
		m_pTail = NULL;

	}

	~CHashList(void)
	{
		Clear();

		if(m_pHashIndex )
		{
			delete[] m_pHashIndex; 
		}

	}

	T* Find(HashListItem& item)
	{
		m_lock.Lock();

		uint32 hashnum = item.GetHashNum();
		hashnum %= m_HashSize;
		HashListItem* pListItem =  (HashListItem*)m_pHashIndex[hashnum];

		while(pListItem)
		{
			if( pListItem->IsMatch(item) )
			{
				break;
			}
			pListItem = pListItem->m_hashNext;
		}

		m_lock.Unlock();

		return (T*)pListItem;

	}

	T* Add(HashListItem& item)
	{
		m_lock.Lock();

		//先查找是否已经存在该项
		T* pOldItem = Find(item);
		if( pOldItem != NULL )
		{
			m_lock.Unlock();
			return NULL;
		}

		T* pNewItem = NULL;
		uint32 hashnum = item.GetHashNum();
		//开始添加
		if( m_itemCount >= m_HashSize )
		{
			//队列过大，把头给清掉	
			if (m_pHead)
				Del(*m_pHead);
			
		}

		pNewItem = m_ItemBuff.AllocateObj();
		HashListItem* pNewItemBase = (HashListItem*)pNewItem;
		*pNewItemBase = item;

		//添加到hash
		AddToHash(pNewItemBase,hashnum);

		//添加到队列尾部
		AddToListTail(pNewItemBase);

		if( NULL == m_pHead )
		{
			m_pHead = pNewItemBase;
		}
		++m_itemCount;

		pNewItem->InitIndex(item);

		m_lock.Unlock();

		return pNewItem;

	}

	bool Del(HashListItem& item)
	{
		m_lock.Lock();

		//先查找是否已经存在该项
		T* pDelItem = Find(item);
		if( pDelItem == NULL )
		{
			m_lock.Unlock();
			return false;
		}
		
		HashListItem* pDelItemBase = (HashListItem*)pDelItem;

		//从hash表中清除
		uint32 hashnum = item.GetHashNum();
		hashnum %= m_HashSize;
		if( pDelItemBase == m_pHashIndex[hashnum] )
		{
			m_pHashIndex[hashnum] = (T*)pDelItemBase->m_hashNext;
		}

		if( pDelItemBase->m_hashPre )
		{
			pDelItemBase->m_hashPre->m_hashNext = pDelItemBase->m_hashNext;
		}

		if( pDelItemBase->m_hashNext)
		{
			pDelItemBase->m_hashNext->m_hashPre = pDelItemBase->m_hashPre;
		}

		pDelItemBase->m_hashNext = NULL;
		pDelItemBase->m_hashPre	 = NULL;

		//从队列中清除
		RemoveFromList(pDelItemBase);

		m_ItemBuff.ReleaseObj((T*)pDelItem);

		--m_itemCount;

		m_lock.Unlock();

		return true;
	}

	void Clear()
	{
		m_lock.Lock();

		memset(m_pHashIndex,0,m_HashSize*sizeof(T*));
		HashListItem* pCurrentItem = m_pHead;
		while(pCurrentItem)
		{
			HashListItem* pClearItem = pCurrentItem;
			pCurrentItem = pCurrentItem->m_ListNext;
			pClearItem->m_hashNext = NULL;
			pClearItem->m_hashPre  = NULL;
			pClearItem->m_ListNext = NULL;
			pClearItem->m_ListPre  = NULL;

			m_ItemBuff.ReleaseObj((T*)pClearItem);
		}
		m_pHead = NULL;
		m_pTail = NULL;
		m_itemCount = 0;

		m_lock.Unlock();
	}

	uint32 Size()
	{ 
		return  m_itemCount;	
	}

	T* Head()
	{ 
		return (T*)m_pHead;
	}


	CSyncCritical* GetLock()
	{ 
		return &m_lock;
	}

	void MoveToTail(T* pItem)
	{
		if(m_itemCount<2 )
		{
			return;
		}

		m_lock.Lock();

		HashListItem* pItemBase = (HashListItem*)pItem;
		RemoveFromList(pItemBase);
		AddToListTail(pItemBase);
		
		m_lock.Unlock();
	}

protected:

	void RemoveFromList(HashListItem* pDelItemBase)
	{
		if(m_pHead == pDelItemBase )
		{
			m_pHead = pDelItemBase->m_ListNext;
		}

		if( m_pTail == pDelItemBase )
		{
			m_pTail = pDelItemBase->m_ListPre;
		}

		if( pDelItemBase->m_ListNext )
		{
			pDelItemBase->m_ListNext->m_ListPre = pDelItemBase->m_ListPre;
		}

		if( pDelItemBase->m_ListPre )
		{
			pDelItemBase->m_ListPre->m_ListNext = pDelItemBase->m_ListNext;
		}

		pDelItemBase->m_ListPre = NULL;
		pDelItemBase->m_ListNext= NULL;

	}

	void AddToHash(HashListItem* pNewItem,uint32 hashnum)
	{
		hashnum %= m_HashSize;

		HashListItem* pListItem =  (HashListItem*)m_pHashIndex[hashnum];
		pNewItem->m_hashPre = NULL;
		pNewItem->m_hashNext = pListItem;
		if( pListItem )
		{
			pListItem->m_hashPre = pNewItem;
		}
		m_pHashIndex[hashnum] = (T*)pNewItem;
	}

	void AddToListTail(HashListItem* pNewItem)
	{
		if( m_pTail )
		{
			m_pTail->m_ListNext = pNewItem;
		}
		pNewItem->m_ListPre = m_pTail;
		pNewItem->m_ListNext= NULL;
		m_pTail = pNewItem;
		if (!m_pHead)
			m_pHead = pNewItem;

	}

protected:
	int	m_HashSize ;
	int	m_itemCount;

	HashListItem*	m_pHead;
	HashListItem*	m_pTail;
	
	T** m_pHashIndex;
	CZObjectPool<T>	m_ItemBuff;

	CSyncCritical	m_lock;

};


//传输层需要的一些结构类
class TranPacketItem :public HashListItem
{
public:
	TranPacketItem();

	virtual uint32 GetHashNum() ;
	virtual bool IsMatch(HashListItem& CompareItem);
	virtual	void InitIndex(HashListItem& IndexItem);

public:
	uint32 m_id;
	uint32 m_seq;
	uint32 m_vlink;
};


class TranSendPacketItem : public TranPacketItem
{
public:
	SendDataStruct  packet; 
};

class TranRecvPacketItem : public TranPacketItem
{
public:
	RecvDataStruct  packet;
};
