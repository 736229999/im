#pragma  once

inline 	bool VER_PAIR::operator < (VER_PAIR v) const
{
	return (v1 < v.v1) || ((v1 == v.v1) && (v2 < v.v2));  
}

inline bool VER_PAIR::operator == (VER_PAIR v) const
{
	return (v1 == v.v1) && (v2 == v.v2);
}


inline CMiniData::CMiniData()
{
	nLen = 0;
	pData = NULL;
	nSpace = 0;
}

inline CMiniData::CMiniData(const CMiniData & v)
{
	nLen = v.nLen;
	nSpace = nLen;
	pData = new uint8[v.nLen + 1];
	memcpy(pData,v.pData,nLen);
	pData[nLen] = 0;
}

inline CMiniData::CMiniData(const char * sz)
{
	assert(sz);
	nLen = (uint32)strlen(sz);
	nSpace = nLen;
	pData = new uint8[nLen + 1];
	memcpy(pData,sz,nLen+1);
}

inline CMiniData::CMiniData(const uint8 * p,uint32 nLen)
{
	assert(p);
	nLen = nLen;
	nSpace = nLen;
	pData = new uint8[nLen + 1];
	memcpy(pData,p,nLen);
	pData[nLen] = 0;
}

inline void CMiniData::operator = (const CMiniData & v)
{
	if(nSpace < v.nLen)
	{
		if(pData)
			delete [] pData;
		pData = new uint8[v.nLen + 1];
		nSpace = v.nLen;
	}
	memcpy(pData,v.pData,v.nLen);
	if(pData)
		pData[v.nLen] = 0;
	nLen = v.nLen;
}

inline  CMiniData::~CMiniData()
{
	if(pData)
		delete [] pData;
}



inline void BUF_DATA::SetDefaultBufSize(size_t nSize)
{
	s_nBufLen = nSize;
}

inline void	BUF_DATA::Clear()
{
	BUF_DATA * pNext = this;
	m_p = m_pData;
	while(pNext->m_pNext)
	{
		pNext = pNext->m_pNext;
		pNext->m_p = pNext->m_pData;
	}
}



inline int	BUF_DATA::GetDataLen()
{
	// 当前缓冲中数据的长度
	int nLen = int(m_p - m_pData);
	assert(nLen >= 0 && nLen <= (int)m_nLen);
	if(nLen == m_nLen && m_pNext)
	{
		nLen += m_pNext->GetDataLen();
	}
	return nLen;
};

inline int	BUF_DATA::GetSpace()
{
	int n = (int)(m_pData + m_nLen - m_p);
	assert(n >= 0);
	if(m_pNext)
	{
		n += m_pNext->GetSpace();
	}
	return n;
};

inline void	BUF_DATA::IncBuf(int n)
{
	assert(n > 0);
	BUF_DATA * pEnd = this;
	while(pEnd->m_pNext != NULL)
	{
		pEnd = pEnd->m_pNext;
	}
	pEnd->m_pNext = new BUF_DATA(n);
}

inline BUF_DATA & BUF_DATA::operator <<(uint64 i)
{
	Write((const char *)&i,sizeof(i));
	return *this;
};

inline BUF_DATA & BUF_DATA::operator <<(uint32 i)
{
	Write((const char *)&i,sizeof(i));
	return *this;
};

inline BUF_DATA & BUF_DATA::operator <<(uint16 i)
{
	Write((const char *)&i,sizeof(i));
	return *this;
};

inline BUF_DATA & BUF_DATA::operator <<(uint8 i)
{
	Write((const char *)&i,sizeof(i));
	return *this;
};

inline BUF_DATA & BUF_DATA::operator <<(float32 f)
{
	Write((const char *)&f,sizeof(f));
	return *this;
}

inline BUF_DATA & BUF_DATA::operator <<(float64 f)
{
	Write((const char *)&f,sizeof(f));
	return *this;
}


inline BUF_DATA & BUF_DATA::operator <<(const char * sz)
{
	UINT n = (UINT)strlen(sz);
	Write((const char *)&n,4);
	Write(sz,n);
	return *this;
};

inline BUF_DATA & BUF_DATA::operator << (const MINI_DATA & v)
{
	Write((const char *)&v.nLen,4);
	Write((const char *)v.pData,v.nLen);
	return *this;
}

#ifdef _DEBUG
inline int		BUF_DATA::Buffers() const
{
	int n = 1;
	const BUF_DATA * pEnd = this;
	while(pEnd->m_pNext)
	{
		pEnd = pEnd->m_pNext;
		n++;
	}
	return n;
};
#endif

