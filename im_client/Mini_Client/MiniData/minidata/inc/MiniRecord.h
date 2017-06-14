// ----------------------------------------------------------------
// 文件:MiniRecord.h
// 版本: 
// 作者: xy
// 日期: 2006-11-24
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
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
// 变体变量，记录中各字段数据的表现形式
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

	MINI_VAR_TYPE	m_type;		// 变量类型
	MINI_RESTRICT	m_restrict;	// 约束条件
	MINI_VAR		m_var;		// 变量内容
};


//------------------------------------------------------
// 记录属性结构
//------------------------------------------------------

struct FIELD_INFO
{
	
	const static uint32	MAX_ATTRS = 3000;// 规定的最多属性个数

	CString			sName;		// 属性名称 长度为4+nB
	MINI_VAR_TYPE	type;		// 长度为4B
	MINI_RESTRICT	restrict;	// 长度为12B

	uint32			GetLen() const;

	STATUS			Read(CMiniReader & r);

	STATUS			Write(CMiniWriter & w) const;
};



typedef std::vector<int>	FIELD_MAP;

class CMiniRecord;

//------------------------------------------------------
// 记录数据的字段属性信息，描述数据字段在文件中的存储形式
//------------------------------------------------------

class MINI_API CMiniRecordInfo
{
protected:

	friend class CMiniDataFile;

	CMiniRecordInfo();

public:

	//------------------------------------------------------
	// 构造函数,初始化属性定义
	//------------------------------------------------------
	CMiniRecordInfo(const FIELD_INFO * pAttrs,int nAttrs,int nVer);

	//------------------------------------------------------
	// 重载比较操作符
	//------------------------------------------------------
	bool operator == (const CMiniRecordInfo & ver) const;

	//------------------------------------------------------
	// 获取从另一版本的记录读取的字段映射规则
	//------------------------------------------------------
	void				GetFieldMap(const CMiniRecordInfo & ver,FIELD_MAP & fm) const;
	
	//------------------------------------------------------
	// 根据当前记录版本信息,创建一条新记录 
	//------------------------------------------------------
	CMiniRecord *		NewRecord() const;

	//------------------------------------------------------
	// 获取当前版本
	//------------------------------------------------------
	uint32				GetVer() const;

	//------------------------------------------------------
	// 串行化数据
	//------------------------------------------------------
	STATUS				Save(CMiniWriter & w) const;

	//------------------------------------------------------
	// 串行化数据 
	//------------------------------------------------------
	STATUS				Read(CMiniReader & r);

	//------------------------------------------------------
	// 获取下条记录结构信息的文件位置
	//------------------------------------------------------
	uint32				GetNext() const;

#ifdef _TEST
	void				Print() const;
#endif

protected:

	//------------------------------------------------------
	// 计算数据长度
	//------------------------------------------------------
	uint32				GetDataLen() const;

protected:

	std::vector<FIELD_INFO>	m_attrs;	// 字段属性定义信息
	uint32					m_nLen;		// 长度
	uint32					m_nVer;		// 版本号
	uint32					m_nNext;	// 下条记录在文件中的位置
	uint32					m_nLen2;	// 长度2
	uint32					m_nPos;		// 在文件中的位置

public:

	const static uint32			ID;
	const static uint32			FIXED_SIZE = 24;

};



//------------------------------------------------------
// 数据记录，由指定格式的字段组成
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
	std::vector<CMiniVar>	m_fields;			// 记录字段

protected:

	uint32					m_nVer;				// 版本号
	uint32					m_nLen;				// 数据长度(不确定,故保护成员)
	uint32					m_nLen2;			// 数据长度(不确定,故保护成员)

};

//#ifdef _DLL_EXPORT
#include "MiniVar.inl"
//#endif

_MINIDATA_END