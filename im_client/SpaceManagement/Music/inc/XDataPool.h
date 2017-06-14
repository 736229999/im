#pragma once

#include "XLock.h"


//------------------------------------------------------
// FIFO�������(���̰߳�),����̶���С,�������,�����ݻ��ȡ��ʧ
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
	// ������	nPoolSize	������еĴ�С
	//			nItemLen	����������Ĵ�С
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
	// �������
	//------------------------------------------------------
	void			Clear()
	{
		m_nBeg = 0;
		m_nItems = 0;
	}

	//------------------------------------------------------
	// ��ʼ��������еĴ�С�����
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
	// ���ص�ǰ���ݳ������ݿ�ĸ��� 
	//------------------------------------------------------
	int			GetSize()
	{
		XLock myl(m_sc.Get());
		return m_nItems;
	};

	//------------------------------------------------------
	// �ж϶����Ƿ�����
	//------------------------------------------------------
	bool		IsFull()
	{
		XLock myl(m_sc.Get());
		return m_nItems == m_nPoolSize;
	}

	//------------------------------------------------------
	// �����ݳ�������ݣ�ʧ�ܷ���ֵС��0���ɹ����سɹ���ӵ�
	// ���ݳ���,������ݳ�����������С�򱻽�ȡ
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// �ж����ݳ��Ƿ�����
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
	// ��ȡ����ĳ������ݳ���
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
	// �����ݳػ�ȡ���ݣ�ʧ�ܷ���ֵС��0���ɹ����سɹ���ȡ�����ݳ���
	//------------------------------------------------------
	int			Pop(void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// �ж����ݳ��Ƿ�Ϊ��
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
	// �����ݳػ�ȡ����,�������������У�ʧ�ܷ���ֵС��0���ɹ����سɹ���ȡ�����ݳ���
	//------------------------------------------------------
	void		Pop(
		/*OUT*/void *	pData1,
		/*IN-OUT*/int & nLen1,
		/*OUT*/void *	pData2,
		/*IN-OUT*/int & nLen2)
	{
		XLock myl(m_sc.Get());
		// �ж����ݳ��Ƿ�Ϊ��
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

	int				m_nItemLen;			// ��������е���ĳ���
	int				m_nBeg;				// ����ͷ��λ��
	int				m_nItems;			// ��ǰ�����е���ĸ���
	int				m_nPoolSize;		// ������е��ܴ�С
	char **			m_pDataPool;		// ��������
	int *			m_pDataLen;			// ��¼ÿ�������������ݳ��ȵ�����
	XSyncCritical	m_sc;				// ������Դ������

};



//------------------------------------------------------
// ����push�����ⳤ�ȴ�С���ݿ�Ļ������,���ݲ��ᱻ��ȡ
//------------------------------------------------------

class XDataPoolEx: public XDataPool
{
public:

	XDataPoolEx()
	{
		m_pItemCap = NULL;
	}

	//------------------------------------------------------
	// ������	nPoolSize	������еĴ�С
	//			nItemLen	����������Ĵ�С
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
	// ��ʼ��������еĴ�С�����
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
	// �����ݳ�������ݣ�
	// ʧ��	����ֵС��0��
	// �ɹ�	������ӵ����ݳ���
	//------------------------------------------------------
	int			Push(const void * pData,int nLen)
	{
		XLock myl(m_sc.Get());
		// �ж����ݳ��Ƿ�����
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
	// �����ݳ�������ݣ����سɹ���ӵ����ݳ���
	//------------------------------------------------------
	void		Push(
		/*IN*/const void * pData,
		/*IN-OUT*/int & nLen,
		/*IN*/const void * pData2,
		/*IN-OUT*/int & nLen2)
	{
		XLock myl(m_sc.Get());
		// �ж����ݳ��Ƿ�����
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

	int *			m_pItemCap;			// ��¼������Ĵ洢��С������

};