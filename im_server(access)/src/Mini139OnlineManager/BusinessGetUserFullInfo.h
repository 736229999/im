#pragma once
#include "BusinessExGSrv.h"

class CBusinessGetUserFullInfo :
	public CBusinessExGSrv
{
public:
	CBusinessGetUserFullInfo(CRouterBusiness* pRouter);
	~CBusinessGetUserFullInfo(void);

	virtual int DoWork();
};

class CBusinessTUserInfoQuery : public CBusinessExGSrv
{
public:
	CBusinessTUserInfoQuery(CRouterBusiness* pRouter);
	~CBusinessTUserInfoQuery();

	virtual int DoWork();
};
	
