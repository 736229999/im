// ----------------------------------------------------------------
// �ļ�:ServerListFile.h
// �汾: 
// ����: xy
// ����: 2006-7-18
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#ifndef __SERVERLISTFILE_H__
#define __SERVERLISTFILE_H__

#include <vector>

typedef struct _IM_SERVER{
	unsigned int	ip;
	unsigned short	port;
	unsigned short	maxConn;
	_IM_SERVER()
	{
		ip = 0;
		port = 0;
		maxConn = 0;
	}
	bool operator > (const _IM_SERVER & r) const
	{
		return maxConn > r.maxConn;
	}
} IM_SERVER,*LPIM_SERVER;




class IMSERVER_LIST:public std::vector<IM_SERVER>
{
public:
	bool	GetBestSrv(IM_SERVER & srv) const;
	bool	GetRandSrv(IM_SERVER & srv);
} ;

typedef const IMSERVER_LIST IMSERVER_LISTC;


//------------------------------------------------------
// �������б��ļ�
//------------------------------------------------------

class CServerListFile
{
public:
	
	CServerListFile();

	~CServerListFile();

public:

	//------------------------------------------------------
	// �򿪷������б��ļ�
	//------------------------------------------------------

	bool			Load(const char * szFile);

	//------------------------------------------------------
	// ����������б��ļ�
	//------------------------------------------------------

	bool			Save(const char * szFile);

	//------------------------------------------------------
	// ��ȡҵ��������б�
	//------------------------------------------------------

	IMSERVER_LISTC	GetBusiSrvList() const;	

	IMSERVER_LIST	GetBusiSrvList();	

	//------------------------------------------------------
	// ��ȡת���������б�
	//------------------------------------------------------

	IMSERVER_LISTC	GetTranSrvList() const;

	IMSERVER_LIST	GetTranSrvList();

	//------------------------------------------------------
	// ����ҵ��������б�
	//------------------------------------------------------

	void			SetBusiSrvList(IMSERVER_LISTC & arrBusiSrv);

	//------------------------------------------------------
	// ����ת���������б�
	//------------------------------------------------------

	void			SetTranSrvList(IMSERVER_LISTC & arrTranSrv);

#ifdef _DEBUG
public:
	static void		Print(IMSERVER_LISTC & svrList);
#endif
	
private:

	IMSERVER_LIST	m_arrBusiSrv;	// ҵ��������б�
	IMSERVER_LIST	m_arrTranSrv;	// ת���������б�

};

#include "ServerListFile.inl"

#endif
