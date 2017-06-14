#include "stdafx.h"
#include "BusinessSearchOnlineUser.h"
#include "protocol.h"
#include "AppManager.h"

CBusinessSearchOnlineUser::CBusinessSearchOnlineUser(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_SEARCH_USER_QUERY,CMDGSRV_SEARCH_USER_ACK)
{
}

CBusinessSearchOnlineUser::~CBusinessSearchOnlineUser(void)
{
}

int CBusinessSearchOnlineUser::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->SearchOnlineUser(m_RecvPacket,m_SendPacket);

	return 0;
}

