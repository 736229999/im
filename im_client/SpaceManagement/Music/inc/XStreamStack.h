#pragma once
#include "XLock.h"
#include "XSyncCritical.h"
#include <assert.h>

//------------------------------------------------------
// 数据流堆栈(先进先出,多线程版)
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
	// 设置栈的大小(原有数据会丢失)
	//------------------------------------------------------
	void		SetStackLen(int nLen)
	{
		XLock myl(m_sc.Get());
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
	// 入栈，写数据(如果数据过长会被截取)
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
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
			long nLen1 = long((m_pBuf + m_nBufLen) - (m_pCusor + m_nDataLen)) ;
			assert(nLen1 >= 0);
			memcpy(m_pCusor + m_nDataLen,pData,nLen1);
			memcpy(m_pBuf,(const char*)pData + nLen1,nCopy - nLen1);
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
	int			Pop(void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		if(m_nDataLen <= 0)
			return 0;
		int nCopy = m_nDataLen > nLen ? nLen : m_nDataLen;
		// 判断是否分段拷贝数据
		if(m_pCusor + nCopy > m_pBuf + m_nBufLen)
		{
			//  分两段拷贝	
			long nLen2 = long((m_pCusor + nCopy) - (m_pBuf + m_nBufLen));
			long nLen1 = nCopy - nLen2;
			if(pData)
				memcpy(pData,m_pCusor,nLen1);
			if(pData)
				memcpy((char*)pData + nLen1,m_pBuf,nLen2);
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
		XLock myl(m_sc.Get());
		m_pCusor = m_pBuf;
		m_nDataLen = 0;
	}

	//------------------------------------------------------
	// 获取栈内数据大小
	//------------------------------------------------------
	int			GetDataSize()
	{
		XLock myl(m_sc.Get());
		return m_nDataLen;
	}

	//------------------------------------------------------
	// 获取栈的大小
	//------------------------------------------------------
	int			GetStackSize()
	{
		XLock myl(m_sc.Get());
		return m_nBufLen;
	}

	//------------------------------------------------------
	// 获取可用栈空间大小
	//------------------------------------------------------
	int			GetSpace()
	{
		XLock myl(m_sc.Get());
		return m_nBufLen - m_nDataLen;
	}

	//------------------------------------------------------
	// 判断栈是否为空
	//------------------------------------------------------
	bool		IsEmpty()
	{
		XLock myl(m_sc.Get());
		return m_nDataLen == 0;
	}

	//------------------------------------------------------
	// 判断栈是否已满
	//------------------------------------------------------
	bool		IsFull()
	{
		XLock myl(m_sc.Get());
		return m_nDataLen == m_nBufLen;
	}

protected:

	int							m_nDataLen;

	int							m_nBufLen;

	char *						m_pCusor;

	char *						m_pBuf;

	XSyncCritical				m_sc;		// 资源访问锁

};