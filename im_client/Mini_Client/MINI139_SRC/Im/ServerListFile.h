// ----------------------------------------------------------------
// 文件:ServerListFile.h
// 版本: 
// 作者: xy
// 日期: 2006-7-18
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
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
// 服务器列表文件
//------------------------------------------------------

class CServerListFile
{
public:
	
	CServerListFile();

	~CServerListFile();

public:

	//------------------------------------------------------
	// 打开服务器列表文件
	//------------------------------------------------------

	bool			Load(const char * szFile);

	//------------------------------------------------------
	// 保存服务器列表到文件
	//------------------------------------------------------

	bool			Save(const char * szFile);

	//------------------------------------------------------
	// 获取业务服务器列表
	//------------------------------------------------------

	IMSERVER_LISTC	GetBusiSrvList() const;	

	IMSERVER_LIST	GetBusiSrvList();	

	//------------------------------------------------------
	// 获取转发服务器列表
	//------------------------------------------------------

	IMSERVER_LISTC	GetTranSrvList() const;

	IMSERVER_LIST	GetTranSrvList();

	//------------------------------------------------------
	// 设置业务服务器列表
	//------------------------------------------------------

	void			SetBusiSrvList(IMSERVER_LISTC & arrBusiSrv);

	//------------------------------------------------------
	// 设置转发服务器列表
	//------------------------------------------------------

	void			SetTranSrvList(IMSERVER_LISTC & arrTranSrv);

#ifdef _DEBUG
public:
	static void		Print(IMSERVER_LISTC & svrList);
#endif
	
private:

	IMSERVER_LIST	m_arrBusiSrv;	// 业务服务器列表
	IMSERVER_LIST	m_arrTranSrv;	// 转发服务器列表

};

#include "ServerListFile.inl"

#endif
