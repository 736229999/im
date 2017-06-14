// MiniWebMsgNotify.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"
#include "AppManager.h"
#include "MiniNotifyService.h"

int main(int argc, char* argv[])
{
		
		CLog::SetLogPath(CAppManager::GetInstance().GetConfig().GetNotifyLogPath());
		CLog::SetLogClass(CAppManager::GetInstance().GetConfig().GetNotifyLogClass());
				
    	CLog::Log("main",CLog::TYPE_IMPORTANT,"开始启动站内消息发送程序....");
    	
    	if( CAppManager::GetInstance().Init() )
    	{
    		CLog::Log("main",CLog::TYPE_IMPORTANT,"初始化完成!");
    	}
    	else
    	{
    		CLog::Log("main",CLog::TYPE_ERROR,"初始化失败!");
    		getchar();
    	}
    
    	CAppManager::GetInstance().Exit();


//  	CMiniNotifyService MyService;
//  	if (!MyService.ParseStandardArgs(argc, argv)) {
//  
//  		// Didn't find any standard args so start the service
//  		// Uncomment the DebugBreak line below to enter the debugger
//  		// when the service is started.
//  		//DebugBreak();
//  		MyService.StartService();
//  	}
  
//  	// When we get here, the service has been stopped
//  	return MyService.m_Status.dwWin32ExitCode;

}

