#include "stdafx.h"
#include "AppManager.h"
#include "PublicType.h"
#include "Log.h"
#include "GlobalBusiControl.h"
#include "BusinessExIMSrv.h"
#include <time.h>
#include "RouterProtocol.h"

/* 版本说明
ver:0105 家族消息缓存
ver:0106 取消家族消息缓存

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
	//记录程序启动时间
	time( &m_start );

	//配置网络层
	char* pAddr = m_ConfigInfo.GetTransferServerIP();
	if( 0 != m_AppNetwork.StartWork(pAddr,PORT_TRANSPORT_SERVER) )
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Listen failed,ip:%s, port:%d",pAddr,PORT_TRANSPORT_SERVER);
		return false;
	}

	printf("CBusinessExIMSrv::SetNetTrans\n");
	CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);
	CBusiness::SetServerID(m_ConfigInfo.GetServerNo());

	//启动工作线程
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

