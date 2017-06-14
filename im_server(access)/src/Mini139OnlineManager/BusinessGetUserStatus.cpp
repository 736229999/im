#include "stdafx.h"
#include "BusinessGetUserStatus.h"
#include "AppManager.h"
#include "protocol.h"


/*查询用户状态*/
CBusinessGetUserStatus::CBusinessGetUserStatus(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_GET_USERSTATUS_QUERY,CMDGSRV_GET_USERSTATUS_ACK)
{

}

CBusinessGetUserStatus::~CBusinessGetUserStatus(void)
{
}


int CBusinessGetUserStatus::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->GetUserStatus(m_RecvPacket,m_SendPacket);

	return 0;
}


/*查询分流*/
CBusinessGetUserFlow::CBusinessGetUserFlow(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_GET_USERFLOW_QUERY, CMDGSRV_GET_USERFLOW_ACK)
{

}

CBusinessGetUserFlow::~CBusinessGetUserFlow(void)
{
}


int CBusinessGetUserFlow::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->GetUserFlow(m_RecvPacket,m_SendPacket);

	return 0;
}




/*修改分流*/
CBusinessModifyUserFlow::CBusinessModifyUserFlow(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_MODIFY_USERFLOW_QUERY,CMDGSRV_MODIFY_USERFLOW_ACK)
{
}

CBusinessModifyUserFlow::~CBusinessModifyUserFlow(void)
{
}

int CBusinessModifyUserFlow::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->ModifyUserFlow(m_RecvPacket,m_SendPacket);

	return 0;
}

CBusinessGetBestAccount::CBusinessGetBestAccount(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_GET_BEST_ACCOUNT_QUERY,CMDGSRV_GET_BEST_ACCOUNT_ACK)
{
}

CBusinessGetBestAccount::~CBusinessGetBestAccount(void)
{
}

int CBusinessGetBestAccount::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	pOnlineManager->GetBestAccount(m_RecvPacket,m_SendPacket);

	return 0;
}

