#include "StdAfx.h"
#include ".\FamilyOnlineService.h"
#include "Log.h"
#include "PublicType.h"

#include "ConfigInfo.h"


CFamilyService::CFamilyService(void)
:CNTService(MINIFAMILYSERVICE,MINIFAMILYSERVICEDESC)
{
}

CFamilyService::~CFamilyService(void)
{
}

BOOL CFamilyService::OnInit()
{

	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����ȫ�ּ������߹���ϵͳ..");
	return true;
}

void CFamilyService::Run()
{
	CAppManager::GetInstance().Init();
}

// Process user control requests
BOOL CFamilyService::OnUserControl(DWORD dwOpcode)
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
void CFamilyService::OnStop()
{
	CAppManager::GetInstance().Stop();
}
