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
#ifdef USE_CONNECT_SERVER
	LPSERVER_STATUSLIST_REQ lpStatusListReq = (LPSERVER_STATUSLIST_REQ)m_RecvPacket.GetData();

	// 打包服务器状态列表
	char plainData[MAX_RECV_BUFFERS] = {0};
	int plain_len = 0;
	int maxRetCount = (MAX_SEND_BUFFERS - sizeof(XT_HEAD) - sizeof(SERVER_STATUSLIST_REQ_ACK)) / sizeof(tagSrvStatus) + 1;

	LPSERVER_STATUSLIST_REQ_ACK lpAck = (LPSERVER_STATUSLIST_REQ_ACK)plainData;
	
	int pos = 0 , retCount = 0;
	SmartList<SRVSTATUSINFO>::iterator it;
	SmartList<SRVSTATUSINFO> & lstSrvStatusInfo = CAppManager::GetInstance().GetConnBusiControl()->GetSrvStatusInfoLst();

	lstSrvStatusInfo.Lock();
	for(it = lstSrvStatusInfo.begin(); it != lstSrvStatusInfo.end(); ++it)
	{
		if(pos++ < lpStatusListReq->startPos)
		{
			continue;
		}
		else
		{
			if(retCount <= maxRetCount)
			{
				if((*it)->SrvStatus.srvBaseInfo.srvType == lpStatusListReq->srvType)
				{
					memcpy(plainData + sizeof(SERVER_STATUSLIST_REQ_ACK) + (retCount - 1) * sizeof(tagSrvStatus),&(*it)->SrvStatus,sizeof(tagSrvStatus));
					retCount++;
				}
			}
			else
			{
				break;
			}
		}
	}
	if(it != lstSrvStatusInfo.end())
	{
		lpAck->nextPos = pos;
	}
	else
	{//
		lpAck->nextPos = 0xffff;
	}

	lstSrvStatusInfo.UnLock();

	lpAck->ret = 0;
	lpAck->retCount = retCount;

	plain_len = sizeof(SERVER_STATUSLIST_REQ_ACK) + (retCount - 1) * sizeof(tagSrvStatus);
	
	m_SendPacket.WriteData(plainData,plain_len);
#endif
	return 0;
}

