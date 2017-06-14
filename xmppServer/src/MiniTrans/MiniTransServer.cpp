// Mini139Server.cpp : �������̨Ӧ�ó������ڵ㡣
//

#include "stdafx.h"

#include "Log.h"
#include "AppManager.h"

/************************************************************************ 
�ļ�����������������main����

�����ˣ�hechang
ʱ�䣺	2006-6-10 
************************************************************************/

int main(int argc, char* argv[])
{
	
	CLog::SetLogPath(CAppManager::GetInstance().GetConfig().GetUserLogPath());
	CLog::SetLogClass(CAppManager::GetInstance().GetConfig().GetTransLogClass());

  	CLog::Log("main",CLog::TYPE_IMPORTANT,"��ʼ����IM�����̨....\n");
  	
  	if( CAppManager::GetInstance().Init() )
  	{
  		CLog::Log("main",CLog::TYPE_IMPORTANT,"ImTransfer ��ʼ�����!");
  	}
  	else
  	{
  		CLog::Log("main",CLog::TYPE_ERROR,"��ʼ��ʧ��!");
  		getchar();
  	}
  
	return 0;

}


