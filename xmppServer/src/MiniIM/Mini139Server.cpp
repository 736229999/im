// Mini139Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "MiniIMService.h"
#include "AppManager.h"

/************************************************************************ 
文件描述：程序的主入口main函数

创建人：hechang
时间：	2006-6-10 
************************************************************************/

int main(int argc, char* argv[])
{
	if(argc==2)
	{
		if(strcmp(argv[1],"-quitall")==0)
		{
			CConfigInfo  dbConfig;
			char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
			dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
			CDataBaseMini DatabaseMini;
			DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
			if( !DatabaseMini.ConnectDB() )
			{
				cout <<"Connect DB failed!" << endl;
			}
			if(0 == DatabaseMini.Update_All_Logout(dbConfig.GetServerNo()))
				cout << "Quit all from DB ok!" << endl;
			else
				cout << "Quit all from DB failed!" << endl;
			DatabaseMini.DisConnect();
		}
		return 0;
	}
	CLog::SetLogPath(CAppManager::GetInstance().GetConfig().GetIMLogPath());

	CLog::SetLogClass(CAppManager::GetInstance().GetConfig().GetIMLogClass());

  	CLog::Log("main",CLog::TYPE_IMPORTANT,"Starting im Server");
  	
  	if( CAppManager::GetInstance().Init() )
  	{
  		//CLog::Log("main",CLog::TYPE_IMPORTANT,"初始化完成!");
  	}
  	else
  	{
  		CLog::Log("main",CLog::TYPE_ERROR,"init error");
  		getchar();
  	}
  
  	CAppManager::GetInstance().Exit();



//   	CMiniIMService MyService;
//   	if (!MyService.ParseStandardArgs(argc, argv)) {
//   
//   		// Didn't find any standard args so start the service
//   		// Uncomment the DebugBreak line below to enter the debugger
//   		// when the service is started.
//   		//DebugBreak();
//   		MyService.StartService();
//   	}
//   
//   	// When we get here, the service has been stopped
//  	return MyService.m_Status.dwWin32ExitCode;

	return 0;

}


