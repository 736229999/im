#pragma once

#include "BusinessExGSrv.h"

class CBusinessSearchOnlineUser : public CBusinessExGSrv
{
public:
	CBusinessSearchOnlineUser(CRouterBusiness* pRouter);
	~CBusinessSearchOnlineUser(void);

	virtual int DoWork();
};

