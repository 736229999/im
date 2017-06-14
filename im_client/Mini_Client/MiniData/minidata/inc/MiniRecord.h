// ----------------------------------------------------------------
// �ļ�:MiniRecord.h
// �汾: 
// ����: xy
// ����: 2006-11-24
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "MiniData.h"
#include "MiniReader.h"
#include "MiniWriter.h"
#include <map>
#include <vector>

#ifdef _TEST
#include <iostream>
#endif

#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

_MINIDATA_BEG



class CMiniDataFile;
class CMiniReader;




const MINI_RESTRICT NULL_RESTIRCT ={MINI_RESTRICT::EMPTY,0};

void strxcpy(char * szDst,const char * szSrc,int x);

//------------------------------------------------------
// �����������¼�и��ֶ����ݵı�����ʽ
//------------------------------------------------------

class MINI_API CMiniVar
{
public:

	friend class CMiniReader;

	~CMiniVar();

	explicit CMiniVar(MINI_VAR_TYPE t);

	CMiniVar(MINI_VAR_TYPE t,MINI_RESTRICT r);

	CMiniVar();

	CMiniVar(uint8 v);

	CMiniVar(uint16 v);

	CMiniVar(uint32 v);

	CMiniVar(uint64 v);

	CMiniVar(float32 v);

	CMiniVar(float64 v);

	CMiniVar(const CMiniVar &v);

	CMiniVar(const char * sz);

	void	operator = (int8 v);

	void	operator = (int16 v);

	void	operator = (int32 v);

	void	operator = (uint8 v);

	void	operator = (uint16 v);

	void	operator = (uint32 v);

	void	operator = (uint64 v);

	void	operator = (float32 v);

	void	operator = (float64 v);

	void	operator = (const char * sz);

	void	SetData(const uint8 * pData,size_t nLen);;

	const uint32	GetLen() const;

	const uint8 *	GetData(size_t & nLen) const;

	void operator = (const CMiniVar & v);

	STATUS	Write(CMiniWriter & w) const;

	STATUS	Read(CMiniReader & r);

	operator uint8 () const;

	operator int8 () const;

	operator uint16 () const;

	operator int16 () const;

	operator int32() const;

	operator uint32() const;

	operator uint64() const;

	operator float32() const;

	operator const char * () const;

#ifdef _TEST

	friend std::ostream & operator << (std::ostream & os,const CMiniVar & v);

#endif

protected:

	MINI_VAR_TYPE	m_type;		// ��������
	MINI_RESTRICT	m_restrict;	// Լ������
	MINI_VAR		m_var;		// ��������
};


//------------------------------------------------------
// ��¼���Խṹ
//------------------------------------------------------

struct FIELD_INFO
{
	
	const static uint32	MAX_ATTRS = 3000;// �涨��������Ը���

	CString			sName;		// �������� ����Ϊ4+nB
	MINI_VAR_TYPE	type;		// ����Ϊ4B
	MINI_RESTRICT	restrict;	// ����Ϊ12B

	uint32			GetLen() const;

	STATUS			Read(CMiniReader & r);

	STATUS			Write(CMiniWriter & w) const;
};



typedef std::vector<int>	FIELD_MAP;

class CMiniRecord;

//------------------------------------------------------
// ��¼���ݵ��ֶ�������Ϣ�����������ֶ����ļ��еĴ洢��ʽ
//------------------------------------------------------

class MINI_API CMiniRecordInfo
{
protected:

	friend class CMiniDataFile;

	CMiniRecordInfo();

public:

	//------------------------------------------------------
	// ���캯��,��ʼ�����Զ���
	//------------------------------------------------------
	CMiniRecordInfo(const FIELD_INFO * pAttrs,int nAttrs,int nVer);

	//------------------------------------------------------
	// ���رȽϲ�����
	//------------------------------------------------------
	bool operator == (const CMiniRecordInfo & ver) const;

	//------------------------------------------------------
	// ��ȡ����һ�汾�ļ�¼��ȡ���ֶ�ӳ�����
	//------------------------------------------------------
	void				GetFieldMap(const CMiniRecordInfo & ver,FIELD_MAP & fm) const;
	
	//------------------------------------------------------
	// ���ݵ�ǰ��¼�汾��Ϣ,����һ���¼�¼ 
	//------------------------------------------------------
	CMiniRecord *		NewRecord() const;

	//------------------------------------------------------
	// ��ȡ��ǰ�汾
	//------------------------------------------------------
	uint32				GetVer() const;

	//------------------------------------------------------
	// ���л�����
	//------------------------------------------------------
	STATUS				Save(CMiniWriter & w) const;

	//------------------------------------------------------
	// ���л����� 
	//------------------------------------------------------
	STATUS				Read(CMiniReader & r);

	//------------------------------------------------------
	// ��ȡ������¼�ṹ��Ϣ���ļ�λ��
	//------------------------------------------------------
	uint32				GetNext() const;

#ifdef _TEST
	void				Print() const;
#endif

protected:

	//------------------------------------------------------
	// �������ݳ���
	//------------------------------------------------------
	uint32				GetDataLen() const;

protected:

	std::vector<FIELD_INFO>	m_attrs;	// �ֶ����Զ�����Ϣ
	uint32					m_nLen;		// ����
	uint32					m_nVer;		// �汾��
	uint32					m_nNext;	// ������¼���ļ��е�λ��
	uint32					m_nLen2;	// ����2
	uint32					m_nPos;		// ���ļ��е�λ��

public:

	const static uint32			ID;
	const static uint32			FIXED_SIZE = 24;

};



//------------------------------------------------------
// ���ݼ�¼����ָ����ʽ���ֶ����
//------------------------------------------------------

class MINI_API CMiniRecord
{

protected:
	
	CMiniRecord();

	friend class	CMiniRecordInfo;

public:	
	
	virtual			~CMiniRecord(void);

public:

	uint32			GetDataLen() const;

	uint32			GetVer() const;

	STATUS			Read(CMiniReader & r);

	STATUS			Save(CMiniWriter & w);

#ifdef _TEST
	void			Print();
#endif

public:

	const static uint32		ID ;
	const static uint32		FIXED_SIZE = 16;
	std::vector<CMiniVar>	m_fields;			// ��¼�ֶ�

protected:

	uint32					m_nVer;				// �汾��
	uint32					m_nLen;				// ���ݳ���(��ȷ��,�ʱ�����Ա)
	uint32					m_nLen2;			// ���ݳ���(��ȷ��,�ʱ�����Ա)

};

//#ifdef _DLL_EXPORT
#include "MiniVar.inl"
//#endif

_MINIDATA_END