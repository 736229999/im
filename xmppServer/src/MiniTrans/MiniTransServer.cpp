// Mini139Server.cpp : 定义控制台应用程序的入口点。
//

#include "stdafx.h"

#include "Log.h"
#include "AppManager.h"

/************************************************************************ 
文件描述：程序的主入口main函数

创建人：hechang
时间：	2006-6-10 
************************************************************************/

int main(int argc, char* argv[])
{
	
	CLog::SetLogPath(CAppManager::GetInstance().GetConfig().GetUserLogPath());
	CLog::SetLogClass(CAppManager::GetInstance().GetConfig().GetTransLogClass());

  	CLog::Log("main",CLog::TYPE_IMPORTANT,"开始启动IM传输后台....\n");
  	
  	if( CAppManager::GetInstance().Init() )
  	{
  		CLog::Log("main",CLog::TYPE_IMPORTANT,"ImTransfer 初始化完成!");
  	}
  	else
  	{
  		CLog::Log("main",CLog::TYPE_ERROR,"初始化失败!");
  		getchar();
  	}
  
	return 0;

}


