#pragma once

#include "BusinessExGSrv.h"

class CBusinessNotifyRelogin:
		public CBusinessExGSrv
{
public:
	CBusinessNotifyRelogin(CRouterBusiness* pRouter);
	~CBusinessNotifyRelogin(void);

	virtual int DoWork();

	void SetNotifyInfo(uint32 ServerIP,uint32 user)
	{
		m_NotifyServerIP = ServerIP;
		m_ReloginUser	 = user;
	}

private:
	uint32			m_NotifyServerIP;
	uint32			m_ReloginUser;
};


