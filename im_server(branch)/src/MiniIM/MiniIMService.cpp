#include "stdafx.h"
#include "MiniIMService.h"
#include "PacketBuffer.h"
#include "Log.h"
#include "PublicType.h"
#include "AppManager.h"

#define VERSION_IM_SERVER 0x0100

CMiniIMService::CMiniIMService(void)
//:CNTService(MINIIMSERVICE,MINIIMSERVICEDESC)
{
}

CMiniIMService::~CMiniIMService(void)
{
}

BOOL CMiniIMService::OnInit()
{

	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����IM��̨....\n�汾:%.4x\n",VERSION_IM_SERVER);
	
	
	return true;
}

void CMiniIMService::Run()
{
	if( CAppManager::GetInstance().Init() )
	{
		//CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ�����!");
	}
	else
	{
		CLog::Log("main",CLog::TYPE_ERROR,"��ʼ��ʧ��!");
		getchar();
	}

	CAppManager::GetInstance().Exit();
}

// Process user control requests
BOOL CMiniIMService::OnUserControl(DWORD dwOpcode)
{/*
	char szInfo[1024];
	switch (dwOpcode) 
	{
	case SERVICE_IM_UPDATEVERSION:
		{
			CAppManager::GetInstance().UpdateVersion();
			sprintf_s(szInfo, "���õ�ַ:\t %s\r\n��Ͱ汾:\t 0x0%x\r\n���°汾:\t 0x0%x\r\n������ʽ:\t %s\r\n������Ϣ:\t%s\r\n",				
				CAppManager::GetInstance().GetUpdateConfig(),
				CAppManager::GetInstance().GetCanUserver(),
				CAppManager::GetInstance().GetAgreever(),
				CAppManager::GetInstance().GetUpdateOption(),
				CAppManager::GetInstance().GetUpdateInfo());
			CLog::Log("CMiniIMService", CLog::TYPE_IMPORTANT, szInfo);
			//LogEvent(EVENTLOG_INFORMATION_TYPE, EVMSG_IMPORTANT,szInfo);
		}
		break;
	default:
		break;
	}*/
	return TRUE; // say not handled
}
void CMiniIMService::OnStop()
{
	CAppManager::GetInstance().Stop();
	CAppManager::GetInstance().Exit();
}

