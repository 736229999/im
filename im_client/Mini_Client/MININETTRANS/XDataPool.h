// ----------------------------------------------------------------
// 文件:XDataPool.h
// 版本: 
// 作者: xy
// 日期: 2007-5-22
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once
#include <assert.h>
#include <list>


namespace Common{

class CSyncCritical
{
public:

	CSyncCritical(void)
	{
		::InitializeCriticalSection(&m_sect);
	}

	~CSyncCritical(void)
	{
		::DeleteCriticalSection(&m_sect);
	}


	void Lock()
	{
		::EnterCriticalSection(&m_sect);
	}

	void Unlock()
	{
		::LeaveCriticalSection(&m_sect);	
	}

	CRITICAL_SECTION * Get()
	{ return &m_sect;};

private:

	CRITICAL_SECTION	m_sect;

};

class CMyLock
{
public:
	CMyLock(CRITICAL_SECTION *pSec)
	{
		m_pSec = pSec;
		::EnterCriticalSection(pSec);
	}

	~CMyLock()
	{
		::LeaveCriticalSection(m_pSec);
	}

private:
	CRITICAL_SECTION *m_pSec;
};

class CXEvent
{
protected:

	CXEvent(const CXEvent & o);

	void operator = (const CXEvent & o);

public:

	operator HANDLE () const
	{
		return m_hEvent;
	}
	
	explicit CXEvent(BOOL bInitState = FALSE)
	{
		m_hEvent = ::CreateEvent(NULL,FALSE,bInitState,0);
	};

	void	ReCreate()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
		m_hEvent = ::CreateEvent(NULL,FALSE,FALSE,0);
	}

	~CXEvent()
	{
		if(m_hEvent)
		{
			::CloseHandle(m_hEvent);
		}
	}

	void	Set()
	{
		if(m_hEvent)
		{
			::SetEvent(m_hEvent);
		}
	}

public:

	HANDLE	m_hEvent;

};


};



//------------------------------------------------------
// 数据流堆栈
//------------------------------------------------------

class XStreamStack
{
public:


	XStreamStack()
	{
		m_pBuf = NULL;
		m_pCusor = NULL;
		m_nDataLen = 0;
		m_nBufLen = 0;
	}

	XStreamStack(int nLen)
	{
		assert(nLen > 0);
		m_pBuf = new char[nLen];
		m_pCusor = m_pBuf;
		m_nDataLen = 0;
		m_nBufLen = nLen;
	};

	~XStreamStack()
	{
		if(m_pBuf)
		{
			delete [] m_pBuf;
		}
	};

	//------------------------------------------------------
	// 设置栈的大小
	//------------------------------------------------------
	void		SetStackLen(int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
		assert(nLen > 0);
		if(m_pBuf)
		{
			delete [] m_pBuf;
			m_pBuf = NULL;
		}
		m_nDataLen = 0;
		m_pBuf = new char[nLen];
		m_nBufLen = nLen;
		m_pCusor = m_pBuf;
	}


	//------------------------------------------------------
	// 入栈，写数据
	//------------------------------------------------------
	int			Push(const char * pData,int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
		int nLeft = m_nBufLen - m_nDataLen;
		if(nLeft <= 0)
			return 0;
		assert(nLeft >= 0);
		assert(pData);
		// 计算可以拷贝的长度
		int nCopy = nLeft > nLen ? nLen : nLeft;

		// 判断是否需要截成两份拷贝
		if(m_pCusor + m_nDataLen >= m_pBuf + m_nBufLen)
		{
			//  m_pBuf                m_pCusor
			//  |                     | 
			//	[ 1 1 1 0 0 0 0 0 0 0 1 1 1 1 1]
			//          |<-nCopy->|
			//          pCopy

			char * pCopy = m_pBuf + ((m_pCusor + m_nDataLen) - (m_pBuf + m_nBufLen));
			memcpy(pCopy,pData,nCopy);
		}
		else if( nCopy > ((m_pBuf + m_nBufLen) - (m_pCusor + m_nDataLen)) )
		{
			//  m_pBuf      m_pCusor
			//  |           |           
			//	[ 0 0 0 0 0 1 1 1 1 1 1 0 0 0 0]
			//                          |<-------nCopy------>| 
			//                          pCopy
			// 需要截成两份拷贝
			int nLen1 = (m_pBuf + m_nBufLen) - (m_pCusor + m_nDataLen) ;
			assert(nLen1 >= 0);
			memcpy(m_pCusor + m_nDataLen,pData,nLen1);
			memcpy(m_pBuf,pData + nLen1,nCopy - nLen1);
		}
		else
		{
			//  m_pBuf  m_pCusor
			//  |       |           
			//	[ 0 0 0 1 1 1 0 0 0 0 0 0 0 0 0]
			//                |<----nCopy-->| 
			//                pCopy
			// 直接拷贝
			memcpy(m_pCusor + m_nDataLen,pData,nCopy);
		}
		m_nDataLen += nCopy;
		return nCopy;
	};

	//------------------------------------------------------
	// 出栈，取数据
	// 返回值	返回成功获取的字节数	
	// 参数：
	// pData	取数据缓冲，可以为空
	// nLen		待获取数据的长度
	//------------------------------------------------------
	int			Pop(char * pData,int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
		if(m_nDataLen <= 0)
			return 0;
		int nCopy = m_nDataLen > nLen ? nLen : m_nDataLen;
		// 判断是否分段拷贝数据
		if(m_pCusor + nCopy > m_pBuf + m_nBufLen)
		{
			//  分两段拷贝	
			int nLen2 = (m_pCusor + nCopy) - (m_pBuf + m_nBufLen);
			int nLen1 = nCopy - nLen2;
			if(pData)
				memcpy(pData,m_pCusor,nLen1);
			if(pData)
				memcpy(pData + nLen1,m_pBuf,nLen2);
			m_pCusor = m_pBuf + nLen2;
		}
		else
		{
			// 直接取数据
			if(pData)
				memcpy(pData,m_pCusor,nCopy);
			m_pCusor += nCopy;
		}
		m_nDataLen -= nCopy;
		return nCopy;
	};

	//------------------------------------------------------
	// 清空数据
	//------------------------------------------------------
	void		Clear()
	{
		Common::CMyLock myl(m_sc.Get());
		m_pCusor = m_pBuf;
		m_nDataLen = 0;
	}

	//------------------------------------------------------
	// 获取栈内数据大小
	//------------------------------------------------------
	int			GetDataSize()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nDataLen;
	}

	//------------------------------------------------------
	// 获取栈的大小
	//------------------------------------------------------
	int			GetStackSize()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nBufLen;
	}

	//------------------------------------------------------
	// 获取可用栈空间大小
	//------------------------------------------------------
	int			GetSpace()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nBufLen - m_nDataLen;
	}

	//------------------------------------------------------
	// 判断栈是否为空
	//------------------------------------------------------
	bool		IsEmpty()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nDataLen == 0;
	}

	//------------------------------------------------------
	// 判断栈是否已满
	//------------------------------------------------------
	bool		IsFull()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nDataLen == m_nBufLen;
	}

protected:

	int							m_nDataLen;

	int							m_nBufLen;

	char *						m_pCusor;

	char *						m_pBuf;

	Common::CSyncCritical		m_sc;		// 资源访问锁

};


//------------------------------------------------------
// FIFO缓冲队列，多线程版本
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
		Common::CMyLock myl(m_sc.Get());
		return m_nItems;
	};

	//------------------------------------------------------
	// 判断队列是否已满
	//------------------------------------------------------
	bool		IsFull()
	{
		Common::CMyLock myl(m_sc.Get());
		return m_nItems == m_nPoolSize;
	}

	//------------------------------------------------------
	// 向数据池添加数据，失败返回值小于0，成功返回成功添加的数据长度
	//------------------------------------------------------
	int			Push(const char * pData,int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
		// 判断数据池是否已满
		if(m_nItems >= m_nPoolSize)
			return -2;
		int nIdx = (m_nBeg + m_nItems) % m_nPoolSize;
		int nCopy = nLen > m_nItemLen ? m_nItemLen:nLen;
		memcpy(m_pDataPool[nIdx],pData,nCopy);
		m_pDataLen[nIdx] = nCopy;
		m_nItems++;
		return nCopy;
	};

	//------------------------------------------------------
	// 获取队列第一项的数据长度
	//------------------------------------------------------
	int			GetFrontLen()
	{
		Common::CMyLock myl(m_sc.Get());
		if(m_nItems == 0)
			return 0;
		else
			return m_pDataLen[m_nBeg % m_nPoolSize];
	}

	//------------------------------------------------------
	// 向数据池获取数据，失败返回值小于0，成功返回成功获取的数据长度
	//------------------------------------------------------
	int			Pop(char * pData,int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
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
		/*OUT*/char *	pData1,
		/*IN-OUT*/int & nLen1,
		/*OUT*/char *	pData2,
		/*IN-OUT*/int & nLen2)
	{
		Common::CMyLock myl(m_sc.Get());
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
	Common::CSyncCritical				// 队列资源访问锁
					m_sc;

};


//------------------------------------------------------
// 允许push入任意长度大小数据块的缓冲队列
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
	// 向数据池添加数据，失败返回值小于0，返回成功添加的数据长度
	//------------------------------------------------------
	int			Push(const char * pData,int nLen)
	{
		Common::CMyLock myl(m_sc.Get());
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
		/*IN*/const char * pData,
		/*IN-OUT*/int & nLen,
		/*IN*/const char * pData2,
		/*IN-OUT*/int & nLen2)
	{
		Common::CMyLock myl(m_sc.Get());
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


//------------------------------------------------------
// 
//------------------------------------------------------

class XMemBlock
{
public:
	
	XMemBlock()
	{ _pData = NULL; _nLen = 0; };
	
	explicit XMemBlock(int nSize)
	{
		assert(nSize > 0);
		_pData = new BYTE[nSize];
		_nLen = nSize;
	}

	~XMemBlock()
	{
		if(_pData)
		{
			delete [] _pData;
		}
	};

public:

	operator BYTE * ()
	{ return _pData; };

	operator char * ()
	{ return (char*)_pData; };

	BYTE *	 GetMem()
	{ return _pData; };

	void	Resize(int nNewSize)
	{ 
		if(nNewSize > _nLen)
		{
			if(_pData)
			{
				delete [] _pData;
				_pData = NULL;
			}
			_pData = new BYTE[nNewSize];
			_nLen = nNewSize;
		}
	};

	int		GetSize() const
	{ return _nLen; };

protected:

	void operator = (const XMemBlock & m);

	XMemBlock(const XMemBlock & m);

protected:
	
	BYTE *		_pData;

	int			_nLen;

};

//------------------------------------------------------
// 内存分配块
//------------------------------------------------------
template<class T>
class XObjAlloc
{
public:

	XObjAlloc(void)
	{
		InitializeCriticalSection(&m_poolLock);
	}

	~XObjAlloc(void)
	{
		Clear();
		DeleteCriticalSection(&m_poolLock);
	}

	void Clear()
	{
		Common::CMyLock MyLock(&m_poolLock);
		T * pObj = NULL;
		std::list<T *>::iterator it;
		for(it = m_lstReclaim.begin();
			it != m_lstReclaim.end();
			it++)
		{
			pObj = (*it);
			delete  pObj; 
			pObj = NULL;
		}
		m_lstReclaim.clear();
		m_lstRecycle.clear();
	}

	T * AllocData()
	{
		T * pObj = NULL;
		Common::CMyLock MyLock(&m_poolLock);
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

		return pObj;
	}
	void ReleaseObj(T * pObj)
	{
		Common::CMyLock MyLock(&m_poolLock);
		m_lstRecycle.push_back(pObj);
	}
private:

	CRITICAL_SECTION		m_poolLock;

	std::list<T*>			m_lstReclaim;	// 内存块

	std::list<T*>			m_lstRecycle;	// 循环利用

};

typedef XObjAlloc<XMemBlock>	XMemAlloc;
