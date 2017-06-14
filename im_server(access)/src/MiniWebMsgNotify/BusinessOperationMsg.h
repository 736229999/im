#pragma once
#include "Business.h"
#include "DataBaseLib.h"

class CMiniAppNetwork;
//发送未在线用户的更新给好友
class CBusinessOperationMsg : public CBusiness
{
public:
	CBusinessOperationMsg(CRouterBusiness* pRouter);
	~CBusinessOperationMsg(void);
	virtual int DoWork();
	virtual int SendReply();
private:
	CMiniAppNetwork *	m_pAppNetwork;	//网络发送层对象指针

	CDataBaseWeb * m_pDataBaseWeb;

};

class CBusinessSystemLog : public CBusiness
{
public:
	CBusinessSystemLog(CRouterBusiness* pRouter);
	~CBusinessSystemLog(void);
	virtual int DoWork();
	virtual int SendReply();
};
/*
class CBusinessMobileMsg : public CBusiness
{
public:
	CBusinessMobileMsg(CRouterBusiness* pRouter);
	~CBusinessMobileMsg(void);
	virtual int DoWork();
	virtual int SendReply();
};
*/
