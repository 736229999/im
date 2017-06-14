// ----------------------------------------------------------------
// 文件:MiniReader.h
// 版本: 
// 作者: xy
// 日期: 2006-11-28
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "MiniData.h"


#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}


_MINIDATA_BEG


const uint32 DEFAULT_BUF_SIZE = 1024*64;

class CMiniVar;

//------------------------------------------------------
// 带缓冲区的文件读取器(高效)
//------------------------------------------------------

class MINI_API CMiniReader
{
public:

	CMiniReader(CFile * f = NULL,uint32 nBufSize = DEFAULT_BUF_SIZE);

	~CMiniReader(void);

public:

	void		Seek(LONGLONG lOff,UINT nFrom);

	void			Clear()
	{ m_p = m_pData + m_nBufSize; m_nSize = m_nBufSize; m_pF = NULL; };
	
	void			SetFile(CFile * pF)
	{ Clear(); m_pF = pF;};

	bool			Read(uint8 * p,int nLen);

	bool Read(uint8 & v)
	{ return Read(&v,1); }

	CMiniReader & operator >> (uint8 & v)
	{ 
		Read(&v,1);
		return *this;
	};

	bool Read(int8 & v)
	{ return Read((uint8*)&v,1); };

	CMiniReader & operator >> (int8 & v)
	{
		Read((uint8*)&v,1);
		return *this;
	}

	bool Read(uint16 & v)
	{ return Read((uint8*)&v,2); };

	CMiniReader & operator >> (uint16 & v)
	{
		Read((uint8*)&v,2);
		return *this;
	}

	bool Read(uint32 & v)
	{ return Read((uint8*)&v,4); };

	CMiniReader & operator >> (uint32 & v)
	{
		Read((uint8*)&v,4);
		return *this;
	}

	bool Read(uint64 & v)
	{ return Read((uint8*)&v,8); };

	CMiniReader & operator >> (uint64 & v)
	{
		Read((uint8*)&v,8);
		return *this;
	}

	bool Read(float32 & v)
	{ return Read((uint8*)&v,4); };

	CMiniReader & operator >> (float32 & v)
	{
		Read((uint8*)&v,4);
		return *this;
	}

	bool Read(float64 & v)
	{ return Read((uint8*)&v,8); };

	CMiniReader & operator >> (float64 &v)
	{
		Read((uint8*)&v,8);
		return *this;
	}

	//------------------------------------------------------
	// 读取字符串数据(不推荐)
	//------------------------------------------------------
	bool Read(char * szBuf)
	{
		uint32 nLen = 0;
		if(Read((uint8*)&nLen,4)==false)
			return false;
		return Read((uint8*)szBuf,nLen);
	};

	//------------------------------------------------------
	// 读取字符串数据(不推荐)
	//------------------------------------------------------
	CMiniReader & operator >> (char * szBuf)
	{
		uint32 nLen = 0;
		Read((uint8*)&nLen,4);
		Read((uint8*)szBuf,nLen);
		return *this;
	}

	//------------------------------------------------------
	// 读取字符串数据
	//------------------------------------------------------
	bool	Read(CString & s);

	//------------------------------------------------------
	// 读mini数据
	//------------------------------------------------------
	bool Read(MINI_DATA & d);

	//------------------------------------------------------
	// 读mini数据
	//------------------------------------------------------
	CMiniReader & operator >> (MINI_DATA & d)
	{
		Read(d);
		return *this;
	}

	//------------------------------------------------------
	// 读取字符串数据 
	//------------------------------------------------------
	CMiniReader & operator >> (CString & s);

	//------------------------------------------------------
	// 获取缓冲区中剩余数据长度
	//------------------------------------------------------
	int32			GetSpace() const
	{ return int32(m_pData + m_nSize - m_p); };
	
protected:

	//------------------------------------------------------
	// 读取一个mini变量(没有考虑约束条件)
	//------------------------------------------------------
	CMiniReader & operator >> (CMiniVar & v);

protected:

	uint32			m_nBufSize;	// 数据缓冲大小
	uint32			m_nSize;	// 数据大小
	uint8 *			m_pData;	// 数据
	uint8 *			m_p;		// 数据读取游标
	CFile *			m_pF;		// 文件

};



_MINIDATA_END