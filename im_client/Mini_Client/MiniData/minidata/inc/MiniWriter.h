// ----------------------------------------------------------------
// 文件:MiniWriter.h
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

//------------------------------------------------------
// 写数据类
//------------------------------------------------------

class MINI_API CMiniWriter:public BUF_DATA
{
public:

	CMiniWriter(CFile * pF = NULL,uint32 nBufSize = BUF_DATA_DEFAULT_SIZE);

	~CMiniWriter();

public:

	void		SetFile(CFile * pF)
	{ BUF_DATA::Clear(); m_pFile = pF; };

	void		Save();

	void		Clear()
	{ BUF_DATA::Clear(); m_pFile = NULL; };

	void		ClearBuf()
	{ BUF_DATA::Clear(); };

protected:

	CFile *		m_pFile;

};



_MINIDATA_END