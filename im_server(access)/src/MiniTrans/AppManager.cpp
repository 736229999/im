#include "stdafx.h"
#include "AppManager.h"
#include "PublicType.h"
#include "Log.h"
#include "GlobalBusiControl.h"
#include "BusinessExIMSrv.h"
#include <time.h>
#include "RouterProtocol.h"

/* �汾˵��
ver:0105 ������Ϣ����
ver:0106 ȡ��������Ϣ����

*/

CAppManager::CAppManager(void):
m_hBlock(false,false)
{ 
	srand(timeGetTime());
	m_SendPacketSeq = (rand()<<16)|rand(); 
}

CAppManager::~CAppManager(void)
{

}

CAppManager& CAppManager::GetInstance()
{
	static CAppManager SingleInstance;
	return SingleInstance;
}

bool CAppManager::Init()
{
	//��¼��������ʱ��
	time( &m_start );

	//���������
	char* pAddr = m_ConfigInfo.GetTransferServerIP();
	if( 0 != m_AppNetwork.StartWork(pAddr,PORT_TRANSPORT_SERVER) )
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Listen failed,ip:%s, port:%d",pAddr,PORT_TRANSPORT_SERVER);
		return false;
	}

	printf("CBusinessExIMSrv::SetNetTrans\n");
	CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);
	CBusiness::SetServerID(m_ConfigInfo.GetServerNo());

	//���������߳�
	m_ThreadManager.StartThread();
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");

	m_hBlock.Wait();
	
	return true;
}

void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}

