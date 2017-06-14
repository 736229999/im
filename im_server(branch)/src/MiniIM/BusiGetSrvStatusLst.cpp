#include "stdafx.h"
#include "BusiGetSrvStatusLst.h"
#include "ConnBusiControl.h"

CBusiGetSrvStatusLst::CBusiGetSrvStatusLst(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SERVER_STATUSLIST_REQ,CMD_SERVER_STATUSLIST_REQ_ACK)
{
}

CBusiGetSrvStatusLst::~CBusiGetSrvStatusLst(void)
{
}

int CBusiGetSrvStatusLst::DoWork()
{
	return 0;
}

