#include "stdafx.h"
#include "BusinessGetUserFullInfo.h"
#include "protocol.h"
#include "AppManager.h"

CBusinessGetUserFullInfo::CBusinessGetUserFullInfo(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_GET_USER_FULLINFO_QUERY,CMDGSRV_GET_USER_FULLINFO_ACK)
{
}

CBusinessGetUserFullInfo::~CBusinessGetUserFullInfo(void)
{
}
int CBusinessGetUserFullInfo::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->GetUserFullInfo(m_RecvPacket,m_SendPacket);

	return 0;
}

CBusinessTUserInfoQuery::CBusinessTUserInfoQuery(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_TUSERINFO_QUERY,CMDGSRV_TUSERINFO_ACK)
{
}

CBusinessTUserInfoQuery::~CBusinessTUserInfoQuery(void)
{
}

int CBusinessTUserInfoQuery::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->GetTUserInfo(m_RecvPacket,m_SendPacket);

	return 0;
}

