#include "stdafx.h"
#include "MiniNotifyService.h"
#include "PacketBuffer.h"
#include "Log.h"
#include "PublicType.h"
#include "AppManager.h"

#define VERSION_IM_SERVER 0x0100

CMiniNotifyService::CMiniNotifyService(void)
//:CNTService(MININOTIFYSERVICE,MININOTIFYSERVICEDESC)
{
}

CMiniNotifyService::~CMiniNotifyService(void)
{
}

BOOL CMiniNotifyService::OnInit()
{

	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����վ����Ϣ���ͳ���....");


	return true;
}

void CMiniNotifyService::Run()
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
BOOL CMiniNotifyService::OnUserControl(DWORD dwOpcode)
{
//	char szInfo[1024];
	switch (dwOpcode) 
	{
	case 1:
		break;
	default:
		break;
	}
	return TRUE; // say not handled
}
void CMiniNotifyService::OnStop()
{
	CAppManager::GetInstance().Stop();
	CAppManager::GetInstance().Exit();
}
