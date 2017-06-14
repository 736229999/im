#include "stdafx.h"
#include "BusinessImServerShutdown.h"
#include "protocol.h"
#include "AppManager.h"

CBusinessImServerShutdown::CBusinessImServerShutdown(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDCSRV_IMSERVER_SHUTDOWN_NOTIFY,CMDCSRV_IMSERVER_SHUTDOWN_ACK)
{

}

int CBusinessImServerShutdown::DoWork()
{
	SERVER_IMSERVER_POWEROFF_NOTIFY* pNotify = (SERVER_IMSERVER_POWEROFF_NOTIFY*)m_RecvPacket.GetData();

	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();
	pOnlineManager->IMServerShutdown(pNotify->severIp );

	m_SendPacket << pNotify->severIp;
	
	return RESULT_SUCCESS;
}


CBusinessImServerShutdown::~CBusinessImServerShutdown(void)
{
	
}


