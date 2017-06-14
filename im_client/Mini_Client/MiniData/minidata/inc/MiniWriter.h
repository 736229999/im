// ----------------------------------------------------------------
// �ļ�:MiniWriter.h
// �汾: 
// ����: xy
// ����: 2006-11-28
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "MiniData.h"


#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}


_MINIDATA_BEG

//------------------------------------------------------
// д������
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