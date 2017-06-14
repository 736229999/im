#include "StdAfx.h"

#include "../inc/MiniRecord.h"
#include "../inc/minireader.h"


_MINIDATA_BEG

CMiniReader::CMiniReader(CFile * pF /* = NULL */,uint32 nBufSize /* = DEFAULT_BUF_SIZE */)
{
	m_pF = pF;
	m_nBufSize = nBufSize;
	//assert(m_nBufSize >= 128);
	if(m_nBufSize == 0)
		m_nBufSize = DEFAULT_BUF_SIZE;
	m_pData = new uint8[m_nBufSize];
	memset(m_pData,0,m_nBufSize);
	m_p = m_pData + m_nBufSize;
	m_nSize = m_nBufSize;
}

CMiniReader::~CMiniReader(void)
{
	if(m_pData)
	{
		delete [] m_pData;
	}
}

CMiniReader & CMiniReader::operator >> (CString & s)
{
	uint32 nLen = 0;
	Read((uint8*)&nLen ,4);
	char * sz = new char[nLen + 1];
	Read((uint8*)sz,nLen);
	sz[nLen] = 0;
	s = sz;
	delete [] sz;
	return *this;
}


bool CMiniReader::Read(MINI_DATA & d)
{
	uint32 nLen = 0;
	if(Read((uint8*)&nLen ,4) == false)
	{
		d.nLen = 0;
		if(d.pData)
			delete [] d.pData;
		return false;
	}
	if(nLen > d.nSpace)
	{
		if(d.pData)
			delete [] d.pData;
		d.pData = new uint8[nLen + 1];
		d.nSpace = nLen;
	}
	d.nLen = nLen;
	if(Read((uint8*)d.pData,nLen) == false)
		return false;
	if(nLen != 0)
		d.pData[nLen] = 0;
	else if(d.nSpace)
		d.pData[0] = 0;
	return true;
}

bool CMiniReader::Read(CString & s)
{
	uint32 nLen = 0;
	if(Read((uint8*)&nLen ,4) == false)
		return false;
	char * sz = new char[nLen + 1];
	if(Read((uint8*)sz,nLen) == false)
		return false;
	sz[nLen] = 0;
	s = sz;
	delete [] sz;
	return true;
}

bool CMiniReader::Read(uint8 * p,int nLen)
{
	assert(m_pF);
	assert(nLen >= 0);
	int32 n = GetSpace();
	if(n >= nLen)
	{
		// 缓冲足够
		memcpy(p,m_p,nLen);
		m_p += nLen;
		return true;
	}
	else
	{
		if(m_nSize == 0)
			return false;
		memcpy(p,m_p,n);
		p += n;
		nLen -=n;
		// 重新读取缓冲
		m_nSize = m_pF->Read(m_pData,m_nBufSize);
		m_p = m_pData;
		return Read(p,nLen);
	}
};

void CMiniReader::Seek(LONGLONG lOff,UINT nFrom)
{
	if(nFrom == CFile::current)
	{
		uint32 nLeft = GetSpace();
		if(lOff > 0)
		{
			if(lOff > nLeft)
			{
				// 移动位置超过了缓冲位置
				lOff -= nLeft;
				m_p += nLeft;
			}
			else
			{
				// 移动位置还在缓冲位置内
				m_p += lOff;
				return ;
			}
		}
		else
		{
			lOff += m_p - m_pData;
			if(lOff < 0)
			{
				// 移动位置超过了缓冲位置
				m_p = m_pData + m_nBufSize;
				m_nSize = m_nBufSize;
				lOff -= m_nSize;
			}
			else
			{
				// 移动位置还在缓冲位置内
				m_p = m_pData + lOff;
				return;	
			}
		}

	}
	else
	{
		m_p = m_pData + m_nBufSize; 
		m_nSize = m_nBufSize;
	}
	m_pF->Seek(lOff,nFrom);
};

CMiniReader & CMiniReader::operator >>(CMiniVar & v)
{
	v.m_var.i64 = 0;
	switch(v.m_type)
	{
	case MV_INT32:
		Read((uint8*)&v.m_var.i32,4);
		break;
	case MV_DATA:
		{
			uint32 iLen = 0; 
			if(Read((uint8*)&iLen,4) == false)
				break ;
			if(v.m_var.data.nSpace < iLen)
			{
				if(v.m_var.data.pData)
					delete [] v.m_var.data.pData;
				v.m_var.data.pData = new uint8[iLen + 1];
				v.m_var.data.nSpace = iLen;
			}
			v.m_var.data.nLen = iLen;
			if(iLen != 0)
				memset(v.m_var.data.pData,0,iLen + 1);
			else if(v.m_var.data.nSpace)
				v.m_var.data.pData[0] = 0;
			Read(v.m_var.data.pData,iLen);
			break;
		};
	case MV_INT8:
		Read((uint8*)&v.m_var.i8,1);
		break;
	case MV_INT16:
		Read((uint8*)&v.m_var.i16,2);
		break;
	case MV_INT64:
		Read((uint8*)&v.m_var.i64,8);
		break;
	case MV_FLOAT32:
		Read((uint8*)&v.m_var.f32,4);
		break;
	case MV_FLOAT64:
		Read((uint8*)&v.m_var.f64,8);
		break;
	};
	return *this;
}



_MINIDATA_END