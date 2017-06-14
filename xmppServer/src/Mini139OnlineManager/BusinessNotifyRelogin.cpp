#include "stdafx.h"
#include "BusinessNotifyRelogin.h"

CBusinessNotifyRelogin::CBusinessNotifyRelogin(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_USERRELOGIN_NOTIFY,CMDGSRV_USERRELOGIN_NOTIFY)
{
	m_NotifyServerIP = 0 ;
	m_ReloginUser	 = 0 ;
}

CBusinessNotifyRelogin::~CBusinessNotifyRelogin(void)
{
}

int CBusinessNotifyRelogin::DoWork()
{
	//���½��հ�Դ��ַ,���͵�ʱ���Զ�����Դ���ݼ���Ŀ�ĵ�ַ
	RecvDataStruct* pRecvData = (RecvDataStruct*)m_RowPacketBuf;
	pRecvData->src_ip		  = m_NotifyServerIP;
	pRecvData->port			  = PORT_APPNET2GLOBAL;

	m_SendPacket << m_ReloginUser;

	return 0; 
}

