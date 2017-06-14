// ----------------------------------------------------------------
// 文件:MiniDataFile.h
// 版本: 
// 作者: xy
// 日期: 2006-11-23
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "minidata.h"
#include "MiniRecord.h"
#include <map>



#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

//------------------------------------------------------
// Mini139使用的数据读写类
//------------------------------------------------------

_MINIDATA_BEG


const uint32 CUR_MINIDB_VERSION = 0x00000001;


class CMiniDataFile;

//------------------------------------------------------
// 数据文件头
//------------------------------------------------------


class CMiniDBHeader
{
protected:

	friend class CMiniDataFile;

	CMiniDBHeader();

public:

	
	STATUS				Read(CMiniReader & r);

	STATUS				Save(CMiniWriter & w);

	uint32				GetLen() const;

public:

	const static uint32	MIN_SIZE = 28;
	const static uint32 FIXED_SIZE = 28;
	const static uint32	ID;

	uint32				m_nVer;
	uint32				m_nLen;
	uint32				m_nReserve1;
	uint32				m_nReserve2;
	CMiniData			m_vReserveData;
	uint32				m_nLen2;
};


//------------------------------------------------------
// 数据库操作类
//------------------------------------------------------

class MINI_API CMiniDataFile
{
public:

	CMiniDataFile(void);

	~CMiniDataFile(void);

	enum SEEK_POS{
		CUR = CFile::current,	// 当前位置
		BEG = CFile::begin,		// 起始位置
		END	= CFile::end		// 最后位置
	};

	

public:

	//------------------------------------------------------
	// 打开记录文件,返回ERRORS信息
	//------------------------------------------------------
	STATUS			Open(const char * szFile,bool bCreate = true);

	//------------------------------------------------------
	// 关闭记录文件
	//------------------------------------------------------
	void			Close();

	//------------------------------------------------------
	// 查找指定版本的记录结构在数据库中是否存在
	//------------------------------------------------------
	bool			FindVersion(const CMiniRecordInfo &	info);

	//------------------------------------------------------
	// 保存版本记录,如果发出错误则返回ERRORS信息
	//------------------------------------------------------
	STATUS			SaveVersion(CMiniRecordInfo & inf);

	//------------------------------------------------------
	// 保存版本记录,如果发出错误则返回ERRORS信息
	//------------------------------------------------------
	STATUS			SaveVersion(const FIELD_INFO * pAttrs,int nAttrs,int nVer);

	//------------------------------------------------------
	// 寻找记录,如果发出错误则返回ERRORS信息
	//------------------------------------------------------
	//STATUS		Seek(int n,SEEK_POS pos = CUR);
	
	//------------------------------------------------------
	// 获取记录数据,输出记录指针,该指针在下次调用本函数时失效
	// 如果发出错误则返回ERRORS信息
	//------------------------------------------------------
	STATUS			GetRecord(CMiniRecord * pRecord);	

	//------------------------------------------------------
	// 保存记录数据到文件中
	//------------------------------------------------------
	STATUS			SaveRecord(CMiniRecord * pRecord);

	//------------------------------------------------------
	// 搜索第一条记录
	//------------------------------------------------------
	void			SeekToBeg();

	//------------------------------------------------------
	// 从记录末尾向前搜索n条记录
	//------------------------------------------------------
	STATUS			SeekFromEnd(uint32 n);

	//------------------------------------------------------
	// 获取文件名
	//------------------------------------------------------
	const CString &	GetFileName() const;

protected:

	//------------------------------------------------------
	// 不同版本记录之间的转化
	//------------------------------------------------------
	void			Convert(const CMiniRecord * pSrc,CMiniRecord * pDst);

	//------------------------------------------------------
	// 跳过记录结构信息记录块
	//------------------------------------------------------
	STATUS			SkipRecordInfo();

protected:

	typedef std::map<uint32,CMiniRecord*>::iterator RecordIt;
	typedef std::map<VER_PAIR,FIELD_MAP>::iterator	VerPairIt;
	
	bool							m_bOpen;	// 文件是否打开
	uint32							m_npRecord;	// 记录游标 
	uint32							m_npBeg;	// 记录开始位置

	CString							m_sFile;	// 文件名
	CMiniDBHeader					m_header;	// 文件头
	std::vector<CMiniRecordInfo>	m_infs;		// 记录结构描述信息
	std::map<uint32,CMiniRecord*>	m_recordTemplate;	// 记录模板
	std::map<VER_PAIR,FIELD_MAP>	m_mapVer;	// 版本映射

	CFile							m_file;		// 数据库文件对象
	CMiniReader						m_reader;	// 读数据
	CMiniWriter						m_writer;	// 写数据

};

#ifdef _DLL_EXPORT
#include "MiniDataFile.inl"
#endif

_MINIDATA_END