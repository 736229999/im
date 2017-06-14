#pragma once
#include <list>
#include "xstream.h"
#include "XMemStack.h"
#include "XLock.h"


//------------------------------------------------------
// 可读写的内存数据流（自动扩展缓冲大小）
//------------------------------------------------------

class XMemStream:public XStream
{
public:

	enum { BLOCK_SIZE = 64 * 1024 };

	typedef std::list<XMemStack*>	DataPool;

public:

	XMemStream()
	{
		m_bEOS = false;
		m_nDataSize = 0;
	}



	virtual ~XMemStream()
	{
		DataPool::iterator it,itEnd;
		itEnd = m_data.end();
		for(it = m_data.begin();it != itEnd;it++)
		{
			if(*it)
			{
				delete (*it);
			}
		}
		itEnd = m_dataUnused.end();
		for(it = m_dataUnused.begin();it != itEnd;it++)
		{
			if(*it)
			{
				delete (*it);
			}
		}

	}

public:
	
	// -1	EndOfStream
	int					Pop(void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		int nDataSize = GetSize();
		if(nLen <= 0)
			return 0;
		XMemStack * pMem;
		DataPool::iterator it,itEnd;
		int nRead = 0;
		int nRet = 0;	// 成功读取的长度

		if(nDataSize >= nLen)
		{
			it = m_data.begin();
			itEnd = m_data.end();
			char * p = (char*)pData;
			for(;it != itEnd;it++)
			{
				pMem = (*it);
				nRead = pMem->Pop(p,nLen);
				nRet += nRead;
				if(nRead >= nLen)
				{
					break;
				}
				else
				{
					nLen -= nRead;
					p += nRead;
				}
			}

			m_dataUnused.insert(m_dataUnused.end(),m_data.begin(),it);
			m_data.erase(m_data.begin(),it);
		}
		m_nDataSize -= nRet;
		if(nRet == 0)
		{
			if(m_bEOS)
				return EOS;
			else
				return nRet;
		}
		else
		{
			return nRet;
		}
	};

	int					Push(const void * pData,int  nLen)
	{
		XLock myl(m_sc.Get());
		XMemStack * pMem ;
		DataPool::iterator it;
		char * p = (char*)pData;
		int nWrite = 0;
		int nRet = 0;	// 成功写入的长度
		if(m_data.size() > 0)
		{
			it = m_data.end();
			it--;
			pMem = *it;
			if(pMem->GetSpace() > 0)
			{
				nWrite = pMem->Push(p,nLen);
				nLen -= nWrite;
				nRet += nWrite;
				p += nWrite;
			}
		}

		// 写入缓冲
		while(nLen > 0)
		{
			if(m_dataUnused.size() > 0)
			{
				pMem = *(m_dataUnused.begin());
				m_dataUnused.pop_front();
			}
			else
			{
				pMem = new XMemStack(BLOCK_SIZE);
			}
			nWrite = pMem->Push(p,nLen);
			nLen -= nWrite;
			nRet += nWrite;
			p += nWrite;
			m_data.push_back(pMem);
		}
		m_nDataSize += nRet;
		return nRet;
	};

	void					Clear()
	{
		XLock myl(m_sc.Get());
		DataPool::iterator it,itEnd;
		itEnd = m_data.end();
		for(it = m_data.begin();it != itEnd;it++)
		{
			if(*it)
			{
				delete (*it);
			}
		}
		m_data.clear();
	};

	virtual int				GetSize()
	{
		XLock myl(m_sc.Get());
		int nRet = 0;
		DataPool::iterator it,itEnd;
		itEnd = m_data.end();
		for(it = m_data.begin();it != itEnd;it++)
		{
			if(*it)
			{
				nRet += (*it)->DataSize();
			}
		}
		return nRet;
	}



	virtual int				GetTotalSize()
	{
		return m_nTotalSize;
	};

	virtual void			SetTotalSize(int nSize)
	{
		m_nTotalSize = nSize;
	}


	bool					IsOpen()
	{
		return true;
	}

protected:

	int						CountDataSize()
	{
		XLock myl(m_sc.Get());
		m_nDataSize = 0;
		DataPool::iterator it,itEnd;
		itEnd = m_data.end();
		for(it = m_data.begin();it != itEnd;it++)
		{
			if(*it)
			{
				m_nDataSize += (*it)->DataSize();
			}
		}
		return m_nDataSize;
	}

protected:

	bool					m_bEOS;

	DataPool				m_data;

	DataPool				m_dataUnused;

	int						m_nTotalSize;

	int						m_nDataSize;

	XSyncCritical			m_sc;			// 资源访问锁

};
