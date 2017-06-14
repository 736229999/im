#pragma once
#include "BusinessExGSrv.h"

class CBusinessModifyUserStatus :
	public CBusinessExGSrv
{
public:
	CBusinessModifyUserStatus(CRouterBusiness* pRouter);
	~CBusinessModifyUserStatus(void);

	virtual int DoWork();
};


