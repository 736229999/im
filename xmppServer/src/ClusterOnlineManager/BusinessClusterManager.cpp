#include "stdafx.h"
#include "BusinessClusterManager.h"
#include "AppManager.h"

//-------------------------------------------------------------------------
//��ѯ���߼���ȫ������
CBusinessGetClusterInfo::CBusinessGetClusterInfo(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_FAMILYINFO_QUERY,CMDGSRV_FAMILYINFO_QUERY_ACK)
{
}

int CBusinessGetClusterInfo::DoWork()
{
	CClusterManager* pClusterManager = CAppManager::GetInstance().GetClusterManager();

	pClusterManager->GetClusterInfo(m_RecvPacket,m_SendPacket);

	return 0;
}

//-------------------------------------------------------------------------
//���ȫ�ּ�������

CBusinessAddCluster::CBusinessAddCluster(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_FAMILY_ADD_QUERY,CMDGSRV_FAMILY_ADD_ACK)
{
}

int CBusinessAddCluster::DoWork()
{
	CClusterManager* pClusterManager = CAppManager::GetInstance().GetClusterManager();

	pClusterManager->AddCluster(m_RecvPacket,m_SendPacket);

	return 0;
}


//-------------------------------------------------------------------------
//ɾ��ȫ�ּ�������
CBusinessDelCluster::CBusinessDelCluster(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_FAMILY_DEL_QUERY,CMDGSRV_FAMILY_DEL_ACK)
{
}

int CBusinessDelCluster::DoWork()
{
	CClusterManager* pClusterManager = CAppManager::GetInstance().GetClusterManager();

	pClusterManager->DelCluster(m_RecvPacket,m_SendPacket);

	return 0;
}


//-------------------------------------------------------------------------
//ĳ̨IM������ͣ��֪ͨ,��ʱ����Ҫ�����IM����������Ϣ
CBusinessImServerShutdown::CBusinessImServerShutdown(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDCSRV_IMSERVER_SHUTDOWN_NOTIFY,CMDCSRV_IMSERVER_SHUTDOWN_ACK)
{
}

int CBusinessImServerShutdown::DoWork()
{
	SERVER_IMSERVER_POWEROFF_NOTIFY* pNotify = (SERVER_IMSERVER_POWEROFF_NOTIFY*)m_RecvPacket.GetData();

	CClusterManager* pClusterManager = CAppManager::GetInstance().GetClusterManager();

	pClusterManager->ClearClusterOnImServer(pNotify->severIp);

	return 0;
}

