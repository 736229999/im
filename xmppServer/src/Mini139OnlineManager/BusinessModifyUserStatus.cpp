#include "stdafx.h"
#include "BusinessModifyUserStatus.h"
#include "protocol.h"
#include "AppManager.h"

CBusinessModifyUserStatus::CBusinessModifyUserStatus(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_MODIFY_USERSTATUS_QUERY,CMDGSRV_MODIFY_USERSTATUS_ACK)
{
}

CBusinessModifyUserStatus::~CBusinessModifyUserStatus(void)
{
}

int CBusinessModifyUserStatus::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->ModifyUserStatus(m_RecvPacket,m_SendPacket);

	return 0;
}

