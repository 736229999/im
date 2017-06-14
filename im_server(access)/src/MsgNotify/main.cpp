#include "AppManager.h"


/************************************************************************
文件描述：程序的主入口main函数

创建人：sunding
时间：  2015-12-8
************************************************************************/

//CThreadPool g_PostThreadPool;


int main(int argc, char* argv[])
{
	CAppManager::GetInstance().Init();
    return 0;
}
