#pragma once
#include "XLock.h"
#include "XSyncCritical.h"
#include <assert.h>

//------------------------------------------------------
// ��������ջ(�Ƚ��ȳ�,���̰߳�)
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
	// ����ջ�Ĵ�С(ԭ�����ݻᶪʧ)
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
	// ��ջ��д����(������ݹ����ᱻ��ȡ)
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		int nLeft = m_nBufLen - m_nDataLen;
		if(nLeft <= 0)
			return 0;
		assert(nLeft >= 0);
		assert(pData);
		// ������Կ����ĳ���
		int nCopy = nLeft > nLen ? nLen : nLeft;

		// �ж��Ƿ���Ҫ�س����ݿ���
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
			// ��Ҫ�س����ݿ���
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
			// ֱ�ӿ���
			memcpy(m_pCusor + m_nDataLen,pData,nCopy);
		}
		m_nDataLen += nCopy;
		return nCopy;
	};

	//------------------------------------------------------
	// ��ջ��ȡ����
	// ����ֵ	���سɹ���ȡ���ֽ���	
	// ������
	// pData	ȡ���ݻ��壬����Ϊ��
	// nLen		����ȡ���ݵĳ���
	//------------------------------------------------------
	int			Pop(void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		if(m_nDataLen <= 0)
			return 0;
		int nCopy = m_nDataLen > nLen ? nLen : m_nDataLen;
		// �ж��Ƿ�ֶο�������
		if(m_pCusor + nCopy > m_pBuf + m_nBufLen)
		{
			//  �����ο���	
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
			// ֱ��ȡ����
			if(pData)
				memcpy(pData,m_pCusor,nCopy);
			m_pCusor += nCopy;
		}
		m_nDataLen -= nCopy;
		return nCopy;
	};

	//------------------------------------------------------
	// �������
	//------------------------------------------------------
	void		Clear()
	{
		XLock myl(m_sc.Get());
		m_pCusor = m_pBuf;
		m_nDataLen = 0;
	}

	//------------------------------------------------------
	// ��ȡջ�����ݴ�С
	//------------------------------------------------------
	int			GetDataSize()
	{
		XLock myl(m_sc.Get());
		return m_nDataLen;
	}

	//------------------------------------------------------
	// ��ȡջ�Ĵ�С
	//------------------------------------------------------
	int			GetStackSize()
	{
		XLock myl(m_sc.Get());
		return m_nBufLen;
	}

	//------------------------------------------------------
	// ��ȡ����ջ�ռ��С
	//------------------------------------------------------
	int			GetSpace()
	{
		XLock myl(m_sc.Get());
		return m_nBufLen - m_nDataLen;
	}

	//------------------------------------------------------
	// �ж�ջ�Ƿ�Ϊ��
	//------------------------------------------------------
	bool		IsEmpty()
	{
		XLock myl(m_sc.Get());
		return m_nDataLen == 0;
	}

	//------------------------------------------------------
	// �ж�ջ�Ƿ�����
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

	XSyncCritical				m_sc;		// ��Դ������

};