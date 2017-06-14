// ClusterOnlineManager.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"
#include "Log.h"
#include "AppManager.h"
//#include "FamilyOnlineService.h"

#define VERSION_ONLINE_CLUSTER_MANAGER	0x0100

int main(int argc, char* argv[])
{
	/*
  	CFamilyService MyService;
  	if (!MyService.ParseStandardArgs(argc, argv)) 
  	{
  
  		// Didn't find any standard args so start the service
  		// Uncomment the DebugBreak line below to enter the debugger
  		// when the service is started.
  		//DebugBreak();
  		MyService.StartService();
  	}
  
  	// When we get here, the service has been stopped
  	return MyService.m_Status.dwWin32ExitCode;

	*/
	CLog::SetLogPath(CAppManager::GetInstance().GetConfig().GetFamilyLogPath());
		
	CLog::SetLogClass(CAppManager::GetInstance().GetConfig().GetFamilyLogClass());
		
 	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����ȫ�ּ������߹���ϵͳ....\n�汾:%.4x\n",VERSION_ONLINE_CLUSTER_MANAGER);
 
 	CAppManager app;
 	app.GetInstance().Init();
 
 	return 0;
}

