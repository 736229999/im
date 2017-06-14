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
	// ��ǰ���������ݵĳ���
	int nLen = int(m_p - m_pData);
	assert(nLen >= 0 && nLen <= (int)m_nLen);

	// �����е������㹻����
	if((int)nBuf <= nLen)
	{
		memcpy(pData,m_pData,nBuf);
	}
	// ��ǰ�������ݲ�������
	else
	{
		if((int)m_nLen > nLen || m_pNext == NULL)
		{
			// �������е����ݲ�������
			memcpy(pData,m_pData,nLen);
			nBuf = nLen;
		}
		else if(m_pNext)
		{
			// ��ǰ��������ݲ�������,������ȡ��һ������
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
	// ���㵱ǰ����������Ŀ��ÿռ�
	int nSpace = int(m_pData + m_nLen - m_p);
	assert(nSpace >= 0);
	// �����㹻��
	if(nSpace >= (int)nLen)
	{
		// д����
		memcpy(m_p,pData,nLen);
		m_p += nLen;
	}
	// ���岻��
	else 
	{
		if(nSpace != 0)
		{
			memcpy(m_p,pData,nSpace);
			m_p += nSpace;
			nLen -= nSpace;
			pData += nSpace;
		}	
		// ��ǰ�����Ѿ����꣬�ж��Ƿ���Ҫ�¿��ٻ���
		if(m_pNext)
		{
			m_pNext->Write(pData,nLen);
		}
		else
		{
			// �½�������n���Ŀռ�
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