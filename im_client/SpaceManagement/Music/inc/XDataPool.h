#pragma once

#include "XLock.h"


//------------------------------------------------------
// FIFO缓冲队列(多线程版),缓冲固定大小,如果超出,则数据会截取丢失
//------------------------------------------------------

class XDataPool
{
public:

	XDataPool()
	{
		m_pDataPool = NULL;
		m_pDataLen = NULL;
		m_nPoolSize = 0;
		m_nItemLen = 0;
		m_nItems = 0;
		m_nBeg = 0;
	}

	//------------------------------------------------------
	// 参数：	nPoolSize	缓冲队列的大小
	//			nItemLen	单个缓冲项的大小
	//------------------------------------------------------
	XDataPool(int nPoolSize, int nItemLen)
	{
		assert(nPoolSize > 0);
		assert(nItemLen > 0);
		char * pItem = NULL;
		int i;
		m_pDataPool = new char * [nPoolSize];
		m_pDataLen = new int[nPoolSize];
		memset(m_pDataLen,0,sizeof(int)*nPoolSize);
		for(i = 0;i < nPoolSize; i++)
		{
			pItem = new char[nItemLen];
			m_pDataPool[i] = pItem;
		}
		m_nPoolSize = nPoolSize;
		m_nItemLen = nItemLen;
		m_nItems = 0;
		m_nBeg = 0;
	};

	virtual ~XDataPool(void)
	{
		int i;
		for(i = 0;i < m_nPoolSize; i++)
		{
			delete [] m_pDataPool[i];
		}
		delete [] m_pDataPool;
		if(m_pDataLen)
			delete [] m_pDataLen;
	};

	//------------------------------------------------------
	// 清除数据
	//------------------------------------------------------
	void			Clear()
	{
		m_nBeg = 0;
		m_nItems = 0;
	}

	//------------------------------------------------------
	// 初始化缓冲队列的大小和项长度
	//------------------------------------------------------
	void		Init(int nPoolSize, int nItemLen)
	{
		assert(nPoolSize > 0);
		assert(nItemLen > 0);
		char * pItem = NULL;
		int i;

		if(m_pDataPool)
		{
			for(i = 0;i < m_nPoolSize; i++)
			{
				delete [] m_pDataPool[i];
			}
			delete [] m_pDataPool;
			m_pDataPool = NULL;
		}

		if(m_pDataLen)
		{
			delete [] m_pDataLen;
			m_pDataLen = NULL;
		}
		m_pDataPool = new char * [nPoolSize];
		m_pDataLen = new int[nPoolSize];
		memset(m_pDataLen,0,sizeof(int)*nPoolSize);
		for(i = 0;i < nPoolSize; i++)
		{
			pItem = new char[nItemLen];
			m_pDataPool[i] = pItem;
		}
		m_nPoolSize = nPoolSize;
		m_nItemLen = nItemLen;
		m_nItems = 0;
		m_nBeg = 0;
	}

	//------------------------------------------------------
	// 返回当前数据池中数据块的个数 
	//------------------------------------------------------
	int			GetSize()
	{
		XLock myl(m_sc.Get());
		return m_nItems;
	};

	//------------------------------------------------------
	// 判断队列是否已满
	//------------------------------------------------------
	bool		IsFull()
	{
		XLock myl(m_sc.Get());
		return m_nItems == m_nPoolSize;
	}

	//------------------------------------------------------
	// 向数据池添加数据，失败返回值小于0，成功返回成功添加的
	// 数据长度,如果数据超过缓冲区大小则被截取
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// 判断数据池是否已满
		if(m_nItems >= m_nPoolSize)
			return -2;
		int nIdx = (m_nBeg + m_nItems) % m_nPoolSize;
		int nCopy = nLen > m_nItemLen ? m_nItemLen : nLen;
		memcpy(m_pDataPool[nIdx],pData,nCopy);
		m_pDataLen[nIdx] = nCopy;
		m_nItems++;
		return nCopy;
	};


	//------------------------------------------------------
	// 获取队列某项的数据长度
	//------------------------------------------------------
	int			GetItemLen(int nIdx)
	{
		XLock myl(m_sc.Get());
		assert(nIdx >= 0);
		if(m_nItems == 0 || nIdx >= m_nItems || nIdx < 0)
			return 0;
		else
			return m_pDataLen[(m_nBeg + nIdx) % m_nPoolSize];
	}

	//------------------------------------------------------
	// 向数据池获取数据，失败返回值小于0，成功返回成功获取的数据长度
	//------------------------------------------------------
	int			Pop(void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// 判断数据池是否为空
		if(m_nItems == 0)
			return -2;
		if(pData == NULL || nLen == 0)
		{
			m_nBeg++;
			m_nBeg %= m_nPoolSize;
			m_nItems--;
			return 0;
		}
		int nIdx = m_nBeg % m_nPoolSize;
		int nCopy = m_pDataLen[nIdx];
		nCopy = nCopy > nLen ? nLen:nCopy;
		memcpy(pData,m_pDataPool[nIdx],nCopy);
		m_nBeg++;
		m_nBeg %= m_nPoolSize;
		m_nItems--;
		return nCopy;
	};

	//------------------------------------------------------
	// 向数据池获取数据,放入两个缓冲中，失败返回值小于0，成功返回成功获取的数据长度
	//------------------------------------------------------
	void		Pop(
		/*OUT*/void *	pData1,
		/*IN-OUT*/int & nLen1,
		/*OUT*/void *	pData2,
		/*IN-OUT*/int & nLen2)
	{
		XLock myl(m_sc.Get());
		// 判断数据池是否为空
		if(m_nItems == 0 || pData1 == NULL)
		{
			nLen1 = 0;
			nLen2 = 0;
			return;
		}
		int nIdx = m_nBeg % m_nPoolSize;
		int nCopy = m_pDataLen[nIdx];
		if(nLen1 >= nCopy)
		{
			memcpy(pData1,m_pDataPool[nIdx],nCopy);
			nLen1 = nCopy;
			nLen2 = 0;
		}
		else
		{
			memcpy(pData1,m_pDataPool[nIdx],nLen1);
			nCopy -= nLen1;
			if(nLen2 > nCopy)
			{
				memcpy(pData2,m_pDataPool[nIdx] + nLen1,nCopy);
				nLen2 = nCopy;
			}
			else
			{
				memcpy(pData2,m_pDataPool[nIdx] + nLen1,nLen2);
			}
		}

		m_nBeg++;
		m_nBeg %= m_nPoolSize;
		m_nItems--;
	}


protected:

	int				m_nItemLen;			// 缓冲队列中的项的长度
	int				m_nBeg;				// 队列头的位置
	int				m_nItems;			// 当前队列中的项的个数
	int				m_nPoolSize;		// 缓冲队列的总大小
	char **			m_pDataPool;		// 缓冲数据
	int *			m_pDataLen;			// 记录每个缓冲区内数据长度的数组
	XSyncCritical	m_sc;				// 队列资源访问锁

};



//------------------------------------------------------
// 允许push入任意长度大小数据块的缓冲队列,数据不会被截取
//------------------------------------------------------

class XDataPoolEx: public XDataPool
{
public:

	XDataPoolEx()
	{
		m_pItemCap = NULL;
	}

	//------------------------------------------------------
	// 参数：	nPoolSize	缓冲队列的大小
	//			nItemLen	单个缓冲项的大小
	//------------------------------------------------------
	XDataPoolEx(int nPoolSize, int nItemLen):XDataPool(nPoolSize,nItemLen)
	{
		m_pItemCap = new int[nPoolSize];
		int i;
		for(i = 0; i < nPoolSize; i++)
		{
			m_pItemCap[i] = nItemLen;
		}
	}

	virtual ~XDataPoolEx()
	{
		if(m_pItemCap)
		{
			delete [] m_pItemCap;
			m_pItemCap = NULL;
		}
	}


public:

	//------------------------------------------------------
	// 初始化缓冲队列的大小和项长度
	//------------------------------------------------------
	void		Init(int nPoolSize, int nItemLen)
	{
		XDataPool::Init(nPoolSize,nItemLen);
		if(m_pItemCap)
			delete [] m_pItemCap;
		m_pItemCap = new int[nPoolSize];
		int i;
		for(i = 0; i < nPoolSize; i++)
		{
			m_pItemCap[i] = nItemLen;
		}
	}



	//------------------------------------------------------
	// 向数据池添加数据，
	// 失败	返回值小于0，
	// 成功	返回添加的数据长度
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// 判断数据池是否已满
		if(m_nItems >= m_nPoolSize)
			return -2;
		int nIdx = (m_nBeg + m_nItems) % m_nPoolSize;
		if(nLen > m_pItemCap[nIdx])
		{
			delete [] m_pDataPool[nIdx];
			m_pDataPool[nIdx] = new char[nLen];
			m_pItemCap[nIdx] = nLen;
		}
		memcpy(m_pDataPool[nIdx],pData,nLen);
		m_pDataLen[nIdx] = nLen;
		m_nItems++;
		return nLen;
	};


	//------------------------------------------------------
	// 向数据池添加数据，返回成功添加的数据长度
	//------------------------------------------------------
	void		Push(
		/*IN*/const void * pData,
		/*IN-OUT*/int & nLen,
		/*IN*/const void * pData2,
		/*IN-OUT*/int & nLen2)
	{
		XLock myl(m_sc.Get());
		// 判断数据池是否已满
		if(m_nItems >= m_nPoolSize)
		{
			nLen = 0;
			nLen2 = 0;
			return;
		}
		int nIdx = (m_nBeg + m_nItems) % m_nPoolSize;
		int nTotal = nLen + nLen2;
		if(nTotal > m_pItemCap[nIdx])
		{
			delete [] m_pDataPool[nIdx];
			m_pDataPool[nIdx] = new char[nTotal];
			m_pItemCap[nIdx] = nTotal;
		}

		memcpy(m_pDataPool[nIdx],pData,nLen);
		memcpy(m_pDataPool[nIdx] + nLen,pData2,nLen2);

		m_pDataLen[nIdx] = nTotal;
		m_nItems++;
	};



protected:

	int *			m_pItemCap;			// 记录缓冲项的存储大小的数组

};