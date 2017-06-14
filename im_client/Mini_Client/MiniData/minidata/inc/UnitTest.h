#pragma once

#include "MiniDataFile.h"
using namespace mini_data;

#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

_MINIDATA_BEG

#ifdef _TEST

//------------------------------------------------------
// 记录结构定义,注意列名区分大小写
//------------------------------------------------------

const FIELD_INFO attrs[] = {
	{"id",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"name",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	20	} },
	{"city",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	100	} },
	{"c2",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	100	} },
	{"c3",		MV_DATA,	{MINI_RESTRICT::DATA_LEN,	200	} },
	{"short",	MV_INT16,	{MINI_RESTRICT::VAL_SIGNED,	1000} },
	{"char",	MV_INT8,	{MINI_RESTRICT::VAL_SIGNED,	100	} },
	{"float32",	MV_FLOAT32,	{MINI_RESTRICT::EMPTY,		100	} },
	{"int64",	MV_INT64,	{MINI_RESTRICT::EMPTY,		100	} },
	{"float64",	MV_FLOAT64,	{MINI_RESTRICT::EMPTY,		100	} },
};

const FIELD_INFO attrs2[] = {	
	{"name",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	100	} },
	{"id",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"age",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
};

const FIELD_INFO attrs3[] = {
	{"id",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"name",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	100	} },
	{"address",	MV_DATA,	{MINI_RESTRICT::DATA_LEN,	200	} },
	{"age",		MV_INT32,	{MINI_RESTRICT::EMPTY,		0	} },
	{"hight",	MV_INT16,	{MINI_RESTRICT::EMPTY,		0	} },
	{"short",	MV_INT16,	{MINI_RESTRICT::VAL_SIGNED,	1000} },
	{"char",	MV_INT8,	{MINI_RESTRICT::VAL_SIGNED,	100	} },
	{"float32",	MV_FLOAT32,	{MINI_RESTRICT::EMPTY,		100	} },
	{"int64",	MV_INT64,	{MINI_RESTRICT::EMPTY,		100	} },
	{"float64",	MV_FLOAT64,	{MINI_RESTRICT::EMPTY,		100	} },
};

class MINI_API CUnitTest
{
public:

	//------------------------------------------------------
	// 写入几条不同版本格式的纪录
	//------------------------------------------------------
	static void TestWriteRecord(const CString & sFileName);

	//------------------------------------------------------
	// 用不同版本格式的方式读取记录
	//------------------------------------------------------
	static void TestReadRecord(const CString & sFileName);

	//------------------------------------------------------
	// 测试记录数据读取能力
	//------------------------------------------------------
	static void TestReadCapability(const CString & sFileName,int nOption = 0);

	//------------------------------------------------------
	// 测试记录数据写入能力
	//------------------------------------------------------
	static void TestWriteCapability(const CString & sFileName);

	//------------------------------------------------------
	// 临时测试
	//------------------------------------------------------
	static void TestTemp(const CString &sFileName);

	//------------------------------------------------------
	// 内存分配时间测试
	//------------------------------------------------------
	static void TestAllocateRecordTime(int nTimes);

};


#endif

_MINIDATA_END