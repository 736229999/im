#pragma once
#include <string.h>
#include <assert.h>

//------------------------------------------------------
// 固定缓冲大小的内存栈(单线程版)
//------------------------------------------------------

class XMemStack
{
public:

	XMemStack(int nBuf)
	{
		m_pBuf = new char[nBuf];
		m_pData = m_pBuf;
		m_nData = 0;
		m_nBuf = nBuf;
	}

	~XMemStack()
	{
		if(m_pBuf)
		{
			delete [] m_pBuf;
		}
	}

	void	Clear()
	{
		m_nData = 0;
		m_pData = m_pBuf;
	}

	int		DataSize()
	{
		return m_nData;
	};

	//
	// pData	out	可以为NULL
	// nLen		in	获取的数据长度
	int		Pop(void * pData,int nLen)
	{
		//
		//   |-- data2 --|--- space ---|-- data1 --|
		//   [ 2 2 2 2 2 2 0 0 0 0 0 0 1 1 1 1 1 1 ]
		//

		int nData2 = int((m_pData + m_nData) - (m_pBuf + m_nBuf));
		if(nData2 < 0)
			nData2 = 0;
		assert(nData2 <= m_nData);
		int nData1 = m_nData - nData2;
		assert(nData1 >= 0);
		int nCopy1 = nLen > nData1 ? nData1 : nLen;
		if(pData && nCopy1 > 0)
		{
			memcpy(pData,m_pData,nCopy1);
		}
		m_pData += nCopy1;
		m_nData -= nCopy1;
		nLen -= nCopy1;
		if(pData)
		{
			pData = (char*)pData + nCopy1;
		}
		if(m_nData == 0 )
		{
			m_pData = m_pBuf;
			return nCopy1;
		}
		if(nLen <= 0)
		{
			return nCopy1;
		}
		int nCopy2 = nLen > nData2 ? nData2 : nLen;
		if(pData && nCopy2 > 0)
		{
			memcpy(pData,m_pBuf,nCopy2);
		}
		m_nData -= nCopy2;
		if(m_nData == 0)
		{
			m_pData = m_pBuf;
		}
		else
		{
			m_pData = m_pBuf + nCopy2;
		}
		return nCopy1 + nCopy2;
	}

	int		GetSpace() const
	{
		return m_nBuf - m_nData;
	}

	int		Push(const void * pData,int nLen)
	{
		//
		//   |-- space1 --|-- data ---|-- space2 --|
		//   [ 0 0 0 0 0 0 1 1 1 1 1 1 0 0 0 0 0 0 ]
		//

		int nSpace = m_nBuf - m_nData;
		if(nSpace <= 0)
			return 0;
		int nSpace2 = int((m_pBuf + m_nBuf) - (m_pData + m_nData));
		if(nSpace2 < 0)
			nSpace2 = 0;
		int nCopy2 = nLen > nSpace2 ? nSpace2 : nLen;
		memcpy(m_pData + m_nData,pData,nCopy2);
		m_nData += nCopy2;
		pData = (char *)pData + nCopy2;
		nLen -= nCopy2;
		if(nLen <= 0 || nSpace2 == nSpace)
			return nCopy2;
		int nCopy1;
		int nSpace1 = nSpace - nSpace2;
		nCopy1 = nLen > nSpace1 ? nSpace1 : nLen;
		if(nSpace2 == 0)
		{
			//   |-- data1 -|-- space1 --|-- data ---|-- space2 --|
			//   [ 1 1 1 1 1 0 0 0 0 0 0 1 1 1 1 1 1]
			int nData1 = int((m_pData + m_nData) - (m_pBuf + m_nBuf));
			if(nData1 < 0)
				nData1 = 0;
			memcpy(m_pBuf + nData1,pData,nCopy1);
		}
		else
			memcpy(m_pBuf,pData,nCopy1);
		m_nData += nCopy1;
		assert(m_nData <= m_nBuf);
		return nCopy1 + nCopy2;
	}

	int		GetStackSize()
	{
		return m_nBuf;
	}

protected:

	char *	m_pBuf;
	int		m_nBuf;
	char *	m_pData;
	int		m_nData;

};

