#include "StdAfx.h"
#include ".\OnlineService.h"
#include "Log.h"
#include "PublicType.h"

#include "ConfigInfo.h"


COnlineService::COnlineService(void)
:CNTService(MINIONLINESERVICE,MINIONLINESERVICEDESC)
{
}

COnlineService::~COnlineService(void)
{
}

BOOL COnlineService::OnInit()
{

	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����ȫ�����߹���ϵͳ..");

	return true;
}

void COnlineService::Run()
{
	if( !CAppManager::GetInstance().Init() )
	{
		CLog::Log("Main",CLog::TYPE_ERROR,"�����ʼ��ʧ��");
	}

	// 	return 0;

}

// Process user control requests
BOOL COnlineService::OnUserControl(DWORD dwOpcode)
{
	//char szInfo[1024];
	switch (dwOpcode) 
	{
	case 0:

		break;
	default:
		break;
	}
	return TRUE; // say not handled
}
void COnlineService::OnStop()
{
	CAppManager::GetInstance().Stop();
}
