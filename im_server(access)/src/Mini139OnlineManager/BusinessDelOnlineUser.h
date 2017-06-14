#pragma once
#include "BusinessExGSrv.h"
#include "ConfigInfo.h"

class CBusinessDelOnlineUser :
	public CBusinessExGSrv
{
public:
	CBusinessDelOnlineUser(CRouterBusiness* pRouter);
	~CBusinessDelOnlineUser(void);

	virtual int DoWork();

	void NotifyTransferLogout(CRecvPacket& RecvPacket);

	uint32 TransferIP;

	CConfigInfo	m_ConfigInfo;
};

