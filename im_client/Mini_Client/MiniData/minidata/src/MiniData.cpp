#include "stdafx.h"
#include "../inc/minidata.h"


_MINIDATA_BEG

size_t BUF_DATA::s_nBufLen = BUF_DATA_DEFAULT_SIZE;


BUF_DATA::BUF_DATA(size_t nBufLen)
{
	if(nBufLen == 0)
		nBufLen = s_nBufLen;
	m_pNext = NULL;
	m_pData = new char[nBufLen];
	assert(m_pData);
	if(m_pData)
	{
		m_p = m_pData;
		m_nLen = nBufLen;
	}
	else
	{
		m_p = NULL;
		m_nLen = 0;
	}
};

BUF_DATA::~BUF_DATA()
{
	if(m_pData)
	{
		delete [] m_pData;
	}
	if(m_pNext)
	{
		delete  m_pNext;
	}
}



void	BUF_DATA::GetData(/*OUT*/char * pData,/*IN-OUT*/size_t & nBuf)
{
	// 当前缓冲中数据的长度
	int nLen = int(m_p - m_pData);
	assert(nLen >= 0 && nLen <= (int)m_nLen);

	// 缓冲中的数据足够需求
	if((int)nBuf <= nLen)
	{
		memcpy(pData,m_pData,nBuf);
	}
	// 当前缓冲数据不够需求
	else
	{
		if((int)m_nLen > nLen || m_pNext == NULL)
		{
			// 缓冲区中的数据不够需求
			memcpy(pData,m_pData,nLen);
			nBuf = nLen;
		}
		else if(m_pNext)
		{
			// 当前缓存的数据不够需求,继续获取下一个缓冲
			size_t n = nBuf - nLen;
			assert(nLen == m_nLen);
			memcpy(pData,m_pData,nLen);
			pData += nLen;
			nBuf = nLen;
			m_pNext->GetData(pData,n);
			nBuf += n;
		}
	}
}

bool	BUF_DATA::Write(const char * pData,size_t nLen)
{
	// 计算当前这个缓冲区的可用空间
	int nSpace = int(m_pData + m_nLen - m_p);
	assert(nSpace >= 0);
	// 缓冲足够大
	if(nSpace >= (int)nLen)
	{
		// 写数据
		memcpy(m_p,pData,nLen);
		m_p += nLen;
	}
	// 缓冲不足
	else 
	{
		if(nSpace != 0)
		{
			memcpy(m_p,pData,nSpace);
			m_p += nSpace;
			nLen -= nSpace;
			pData += nSpace;
		}	
		// 当前缓冲已经用完，判断是否需要新开辟缓冲
		if(m_pNext)
		{
			m_pNext->Write(pData,nLen);
		}
		else
		{
			// 新建缓冲区n倍的空间
			nSpace = (int)(((nLen / s_nBufLen) + 1) * s_nBufLen);
			m_pNext = new BUF_DATA(nSpace);
			if(m_pNext)
				m_pNext->Write(pData,nLen);
			else
				return false;
		}
	}
	return true;
}





_MINIDATA_END