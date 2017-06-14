#pragma once
#include "BusinessExGSrv.h"

class CBusinessGetUserStatus :
	public CBusinessExGSrv
{
public:
	CBusinessGetUserStatus(CRouterBusiness* pRouter);
	virtual ~CBusinessGetUserStatus(void);

	virtual int DoWork();
};



class CBusinessGetUserFlow :
	public CBusinessExGSrv
{
public:
	CBusinessGetUserFlow(CRouterBusiness* pRouter);
	virtual ~CBusinessGetUserFlow(void);

	virtual int DoWork();
};


class CBusinessModifyUserFlow :
	public CBusinessExGSrv
{
public:
	CBusinessModifyUserFlow(CRouterBusiness* pRouter);
	~CBusinessModifyUserFlow(void);

	virtual int DoWork();
};
	
class CBusinessGetBestAccount : public CBusinessExGSrv
{
public:
	CBusinessGetBestAccount(CRouterBusiness* pRouter);
	~CBusinessGetBestAccount(void);

	virtual int DoWork();
};
	
