#pragma once
#include "BusinessExGSrv.h"


class CBusinessImServerShutdown:
	public CBusinessExGSrv
{
public:
	CBusinessImServerShutdown(CRouterBusiness* pRouter);
	~CBusinessImServerShutdown(void);

	virtual int DoWork();
};
