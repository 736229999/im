#pragma once

#include "BusinessExGSrv.h"

class CBusinessGetUserLinkInfo : public CBusinessExGSrv
{
public:
	CBusinessGetUserLinkInfo(CRouterBusiness* pRouter);
	~CBusinessGetUserLinkInfo(void);

	virtual int DoWork();
};
