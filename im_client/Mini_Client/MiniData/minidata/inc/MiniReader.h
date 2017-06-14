// ----------------------------------------------------------------
// �ļ�:MiniReader.h
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


const uint32 DEFAULT_BUF_SIZE = 1024*64;

class CMiniVar;

//------------------------------------------------------
// �����������ļ���ȡ��(��Ч)
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
	// ��ȡ�ַ�������(���Ƽ�)
	//------------------------------------------------------
	bool Read(char * szBuf)
	{
		uint32 nLen = 0;
		if(Read((uint8*)&nLen,4)==false)
			return false;
		return Read((uint8*)szBuf,nLen);
	};

	//------------------------------------------------------
	// ��ȡ�ַ�������(���Ƽ�)
	//------------------------------------------------------
	CMiniReader & operator >> (char * szBuf)
	{
		uint32 nLen = 0;
		Read((uint8*)&nLen,4);
		Read((uint8*)szBuf,nLen);
		return *this;
	}

	//------------------------------------------------------
	// ��ȡ�ַ�������
	//------------------------------------------------------
	bool	Read(CString & s);

	//------------------------------------------------------
	// ��mini����
	//------------------------------------------------------
	bool Read(MINI_DATA & d);

	//------------------------------------------------------
	// ��mini����
	//------------------------------------------------------
	CMiniReader & operator >> (MINI_DATA & d)
	{
		Read(d);
		return *this;
	}

	//------------------------------------------------------
	// ��ȡ�ַ������� 
	//------------------------------------------------------
	CMiniReader & operator >> (CString & s);

	//------------------------------------------------------
	// ��ȡ��������ʣ�����ݳ���
	//------------------------------------------------------
	int32			GetSpace() const
	{ return int32(m_pData + m_nSize - m_p); };
	
protected:

	//------------------------------------------------------
	// ��ȡһ��mini����(û�п���Լ������)
	//------------------------------------------------------
	CMiniReader & operator >> (CMiniVar & v);

protected:

	uint32			m_nBufSize;	// ���ݻ����С
	uint32			m_nSize;	// ���ݴ�С
	uint8 *			m_pData;	// ����
	uint8 *			m_p;		// ���ݶ�ȡ�α�
	CFile *			m_pF;		// �ļ�

};



_MINIDATA_END