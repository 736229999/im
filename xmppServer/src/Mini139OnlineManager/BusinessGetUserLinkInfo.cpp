#include "stdafx.h"
#include "BusinessGetUserLinkInfo.h"
#include "protocol.h"
#include "AppManager.h"

CBusinessGetUserLinkInfo::CBusinessGetUserLinkInfo(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_USERCONTROLINFO_QUERY,CMDGSRV_USERCONTROLINFO_QUERY_ACK)
{
}

CBusinessGetUserLinkInfo::~CBusinessGetUserLinkInfo(void)
{
}


int CBusinessGetUserLinkInfo::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();

	if( false == pOnlineManager->GetUserLinkInfo(m_RecvPacket,m_SendPacket) )
	{
		CancelReply();
	}

	return 0;
}

