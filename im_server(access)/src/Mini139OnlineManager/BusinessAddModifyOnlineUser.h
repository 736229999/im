#pragma once

#include "BusinessExGSrv.h"
#include "BusinessNotifyRelogin.h"
#include "ConfigInfo.h"

class CBusinessAddModifyOnlineUser :
	public CBusinessExGSrv
{
public:
	CBusinessAddModifyOnlineUser(CRouterBusiness* pRouter);
	~CBusinessAddModifyOnlineUser(void);

	virtual int DoWork();

	void NotifyRelogin(uint32 ImIp,uint32 userid,uint32 uip, uint32 lanip);

	void NotifyTransferLogin(CRecvPacket& RecvPacket);

	uint32 TransferIP;

	CConfigInfo	m_ConfigInfo;
};

