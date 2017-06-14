#pragma once

#include "MiniDataFile.h"
using namespace mini_data;

#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

_MINIDATA_BEG

#ifdef _TEST

//------------------------------------------------------
// ��¼�ṹ����,ע���������ִ�Сд
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
	// д�뼸����ͬ�汾��ʽ�ļ�¼
	//------------------------------------------------------
	static void TestWriteRecord(const CString & sFileName);

	//------------------------------------------------------
	// �ò�ͬ�汾��ʽ�ķ�ʽ��ȡ��¼
	//------------------------------------------------------
	static void TestReadRecord(const CString & sFileName);

	//------------------------------------------------------
	// ���Լ�¼���ݶ�ȡ����
	//------------------------------------------------------
	static void TestReadCapability(const CString & sFileName,int nOption = 0);

	//------------------------------------------------------
	// ���Լ�¼����д������
	//------------------------------------------------------
	static void TestWriteCapability(const CString & sFileName);

	//------------------------------------------------------
	// ��ʱ����
	//------------------------------------------------------
	static void TestTemp(const CString &sFileName);

	//------------------------------------------------------
	// �ڴ����ʱ�����
	//------------------------------------------------------
	static void TestAllocateRecordTime(int nTimes);

};


#endif

_MINIDATA_END