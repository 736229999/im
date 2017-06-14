#include "StdAfx.h"
#include "../inc/unittest.h"

#ifdef _TEST
#include <iostream>
using namespace std;

#define _MINIDATA_BEG	namespace mini_data{
#define _MINIDATA_END	}

_MINIDATA_BEG

//------------------------------------------------------
// д�뼸����ͬ�汾��ʽ�ļ�¼
//------------------------------------------------------
void CUnitTest::TestWriteRecord(const CString & sFileName)
{
	CMiniDataFile db;
	STATUS s;

	CMiniRecordInfo inf1(attrs,sizeof(attrs)/sizeof(FIELD_INFO),1);
	CMiniRecordInfo inf2(attrs2,sizeof(attrs2)/sizeof(FIELD_INFO),2);
	CMiniRecordInfo inf3(attrs3,sizeof(attrs3)/sizeof(FIELD_INFO),3);
	CMiniRecord * pRecord1 = inf1.NewRecord();
	CMiniRecord * pRecord2 = inf2.NewRecord();
	CMiniRecord * pRecord3 = inf3.NewRecord();

	s = db.Open(sFileName,true);



	pRecord1->m_fields[0] = (uint32)124;
	pRecord1->m_fields[1] = "Yao ming";
	pRecord1->m_fields[2] = "�Ϻ�";
	pRecord1->m_fields[3] = "this is c2";
	pRecord1->m_fields[4] = "this is c3";
	pRecord1->m_fields[5] = (uint16)939;
	pRecord1->m_fields[6] = (uint8)12;
	pRecord1->m_fields[7] = (float32)34.534;
	pRecord1->m_fields[8] = (uint64)0x1020304050607080;
	pRecord1->m_fields[9] = (float64)1.234567890123456;

	pRecord2->m_fields[0] = "Ke bi";
	pRecord2->m_fields[1] = (uint32)125;
	pRecord2->m_fields[2] = (uint32)28;

	pRecord3->m_fields[0] = (uint32)126;
	pRecord3->m_fields[1] = "O'lir";
	pRecord3->m_fields[2] = "����";
	pRecord3->m_fields[3] = (uint32)39;

	
	s = db.SaveVersion(inf1);
	s = db.SaveVersion(inf2);
	s = db.SaveRecord(pRecord1);
	s = db.SaveRecord(pRecord2);
	s = db.SaveVersion(inf3);
	s = db.SaveRecord(pRecord3);

}

//------------------------------------------------------
// �ò�ͬ�汾��ʽ�ķ�ʽ��ȡ��¼
//------------------------------------------------------
void	CUnitTest::TestReadRecord(const CString & sFileName)
{
	CMiniDataFile db;
	STATUS s;

	CMiniRecordInfo inf1(attrs,sizeof(attrs)/sizeof(FIELD_INFO),1);
	CMiniRecordInfo inf2(attrs2,sizeof(attrs2)/sizeof(FIELD_INFO),2);
	CMiniRecordInfo inf3(attrs3,sizeof(attrs3)/sizeof(FIELD_INFO),3);
	CMiniRecord * pRecord1 = inf1.NewRecord();
	CMiniRecord * pRecord2 = inf2.NewRecord();
	CMiniRecord * pRecord3 = inf3.NewRecord();


	s = db.Open(sFileName,false);

	s = OK;
	cout<<"* ʹ�ð汾1��ȡ����"<<endl;
	while(s == OK)
	{
		s = db.GetRecord(pRecord1);
		if(s != OK)
			break;
		pRecord1->Print();
	}

	db.SeekToBeg();
	s = OK;
	cout<<"* ʹ�ð汾2��ȡ����"<<endl;
	while(s == OK)
	{
		s = db.GetRecord(pRecord2);
		if(s != OK)
			break;
		pRecord2->Print();
	}
	db.SeekToBeg();
	s = OK;
	cout<<"* ʹ�ð汾3��ȡ����"<<endl;
	while(s == OK)
	{
		s = db.GetRecord(pRecord3);
		if(s != OK)
			break;
		pRecord3->Print();
	}
}

void CUnitTest::TestReadCapability(const CString & sFileName,int nOption)
{
	CMiniDataFile db;
	STATUS s;
	DWORD nTime1;
	DWORD nTime2;

	CMiniRecordInfo inf1(attrs,sizeof(attrs)/sizeof(FIELD_INFO),1);
	CMiniRecordInfo inf2(attrs2,sizeof(attrs2)/sizeof(FIELD_INFO),2);
	CMiniRecordInfo inf3(attrs3,sizeof(attrs3)/sizeof(FIELD_INFO),3);
	CMiniRecord * pRecord1 = inf1.NewRecord();
	CMiniRecord * pRecord2 = inf2.NewRecord();
	CMiniRecord * pRecord3 = inf3.NewRecord();

	s = db.Open(sFileName,false);
	db.SeekToBeg();

	cout<<"==��ȡ���ݲ���=="<<endl;

	if(nOption == 0)
	{
		cout<<"1.����ͬ�汾��ʽ��ȡ����"<<endl;
		s = OK;
		nTime1 = GetTickCount();
		int nT = 0;
		while(s == OK)
		{
			s = db.GetRecord(pRecord1);	
			/*pRecord1->Print();
			nT++;
			if(nT>50)
			break;*/
		}
	}
	else if(nOption == 1)
	{
		cout<<"2.�ò�ͬ�汾��ʽ(2)��ȡ����"<<endl;
		s = OK;
		nTime1 = GetTickCount();
		int nT = 0;
		while(s == OK)
		{

			s = db.GetRecord(pRecord2);	
			/*nT++;
			if(nT > 10)
			{
			int nt = 0;
			break;
			}
			pRecord2->Print();*/
		}

	}
	else if(nOption == 2)
	{
		cout<<"3.�ò�ͬ�汾��ʽ(3)��ȡ����"<<endl;
		s = OK;
		nTime1 = GetTickCount();
		int nT = 0;
		while(s == OK)
		{
			s = db.GetRecord(pRecord3);	
			/*nT++;
			if(nT > 10)
			{
			int nt = 0;
			break;
			}
			pRecord3->Print();*/
		}

	}

	if(s != OK)
	{
		if(s != FILE_EOF)
			cout<<"��������"<<endl;
	}
	nTime2 = GetTickCount();
	cout<<"*��ʱ"<<(nTime2 - nTime1)/1000.0<<"��"<<endl;
}

void CUnitTest::TestWriteCapability(const CString & sFileName)
{
	CMiniDataFile db;
	STATUS s;
	DWORD nTime1;
	DWORD nTime2;

	CMiniRecordInfo inf1(attrs,sizeof(attrs)/sizeof(FIELD_INFO),1);
	CMiniRecordInfo inf2(attrs2,sizeof(attrs2)/sizeof(FIELD_INFO),2);
	CMiniRecordInfo inf3(attrs3,sizeof(attrs3)/sizeof(FIELD_INFO),3);
	CMiniRecord * pRecord1 = inf1.NewRecord();
	CMiniRecord * pRecord2 = inf2.NewRecord();
	CMiniRecord * pRecord3 = inf3.NewRecord();

	pRecord1->m_fields[0] = (uint32)124;
	pRecord1->m_fields[1] = "Yao ming!";
	pRecord1->m_fields[2] = "this is c1";
	pRecord1->m_fields[3] = "this is c2";
	pRecord1->m_fields[4] = "this is c3";
	pRecord1->m_fields[5] = (uint16)939;
	pRecord1->m_fields[6] = (uint8)12;
	pRecord1->m_fields[7] = (float32)34.534;
	pRecord1->m_fields[8] = (uint64)0x1020304050607080;
	pRecord1->m_fields[9] = (float64)1.234567890123456;

	/*
	{"id",MV_INT32,{MINI_RESTRICT::EMPTY,0}},
	{"name",MV_DATA,{MINI_RESTRICT::DATA_LEN,100}},
	{"c1",MV_DATA,{MINI_RESTRICT::DATA_LEN,100}},
	{"c2",MV_DATA,{MINI_RESTRICT::DATA_LEN,100}},
	{"c3",MV_DATA,{MINI_RESTRICT::DATA_LEN,200}},
	{"short",MV_INT16,{MINI_RESTRICT::VAL_SIGNED,1000}},
	{"char",MV_INT8,{MINI_RESTRICT::VAL_SIGNED,100}},
	{"float32",MV_FLOAT32,{MINI_RESTRICT::EMPTY,100}},
	{"int64",MV_INT64,{MINI_RESTRICT::EMPTY,100}},
	{"float64",MV_FLOAT64,{MINI_RESTRICT::EMPTY,100}},
	*/

	pRecord2->m_fields[0] = (uint32)125;
	pRecord2->m_fields[1] = "Ke bi";
	pRecord2->m_fields[2] = (uint32)20;

	pRecord3->m_fields[0] = (uint32)126;
	pRecord3->m_fields[1] = "O'lir";
	pRecord3->m_fields[2] = (uint32)33;
	pRecord3->m_fields[3] = (uint32)211;

	s = db.Open(sFileName,true);
	// д��汾1�Ľṹ
	s = db.SaveVersion(inf1);
	// д��汾2�Ľṹ
	s = db.SaveVersion(inf2);
	// д��汾3�Ľṹ
	s = db.SaveVersion(inf3);

	cout<<"==д���¼���ܲ���=="<<endl;
	cout<<"1.д��10������¼����,��¼����Ϊ"<<pRecord1->GetDataLen()<<endl;
	s = OK;
	int i;
	const int c = 100000;
	nTime1 = GetTickCount();
	for(i = 0;i < c;i++)
	{
		s = db.SaveRecord(pRecord1);
	}
	if(s != OK)
	{
		if(s != FILE_EOF)
			cout<<"��������"<<endl;
	}
	nTime2 = GetTickCount();
	cout<<"*��ʱ"<<(nTime2 - nTime1)/1000.0<<"��"<<endl;
}

void CUnitTest::TestTemp(const CString &sFileName)
{
	CMiniDataFile db;
	STATUS s;
	DWORD nTime1 = 0;
	DWORD nTime2 = 0;

	CMiniRecordInfo inf1(attrs,sizeof(attrs)/sizeof(FIELD_INFO),1);
	CMiniRecordInfo inf2(attrs2,sizeof(attrs2)/sizeof(FIELD_INFO),2);
	CMiniRecordInfo inf3(attrs3,sizeof(attrs3)/sizeof(FIELD_INFO),3);
	CMiniRecord * pRecord1 = inf1.NewRecord();
	CMiniRecord * pRecord2 = inf2.NewRecord();
	CMiniRecord * pRecord3 = inf3.NewRecord();

	//pRecord1->m_fields[0] = (uint32)124;
	//pRecord1->m_fields[1] = "Yao ming!";
	//pRecord1->m_fields[2] = "this is c1";
	//pRecord1->m_fields[3] = "this is c2";
	//pRecord1->m_fields[4] = "this is c3";
	//pRecord1->m_fields[5] = (uint16)939;
	//pRecord1->m_fields[6] = (uint8)12;
	//pRecord1->m_fields[7] = (float32)34.534;
	//pRecord1->m_fields[8] = (uint64)0x1020304050607080;
	//pRecord1->m_fields[9] = (float64)1.234567890123456;

	
	s = db.Open(sFileName,true);
	s = db.SeekFromEnd(3);
	while(s == OK)
	{
		s = db.GetRecord(pRecord1);
		if(s == OK)
			pRecord1->Print();
	}
}

//------------------------------------------------------
// �ڴ����ʱ�����
//------------------------------------------------------

void CUnitTest::TestAllocateRecordTime(int nTimes)
{
	DWORD nTime1;
	DWORD nTime2;
	int i;

	cout<<nTimes<<"���ڴ����ʱ����� "<<endl;
	nTime1 = GetTickCount();
	for(i = 0;i < nTimes;i++)
	{
		uint8 * pData = new uint8[100];
	}


	nTime2 = GetTickCount();
	cout<<"*��ʱ"<<(nTime2 - nTime1)/1000.0<<"��"<<endl;
}

_MINIDATA_END

#endif
