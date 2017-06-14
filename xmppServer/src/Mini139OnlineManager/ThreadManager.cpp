#include "stdafx.h"
#include "ThreadManager.h"
#include "RouterBusiness.h"
#include "BusinessSearchOnlineUser.h"
#include "BusinessAddModifyOnlineUser.h"
#include "BusinessDelOnlineUser.h"
#include "BusinessGetUserFullInfo.h"
#include "BusinessGetUserLinkInfo.h"
#include "BusinessGetUserStatus.h"
#include "BusinessModifyUserStatus.h"
#include "BusinessImServerShutdown.h"
#include "MiniAppNetwork.h"
#include "RouterProtocol.h"


#define PROCESS_LIST_TYPE_A 0

CThreadManager::CThreadManager(void)
{
}

CThreadManager::~CThreadManager(void)
{
	EndThread();
}



//启动各个工作线程
void CThreadManager::StartThread()
{
	CRouterProtocol::GetInstance()->SetPacketBufferArray(m_PacketBufferArray);

	pthread_t hThread; 
	for(int i = 0 ; i < 1 ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcA,				// thread function 
			this,		// argument to thread function 
			0,                          // use default creation flags 
			NULL);						// returns the thread identifier 
		m_listThreadHandleBuf.push_back(hThread);
	}
}

//结束工作线程
void CThreadManager::EndThread()
{
	for( int i=0 ;i <BUSINESS_PROC_TYPE_NUM; ++i )
	{
		m_ThreadExitFlag[i] = true;
	}

	if( 0 == m_listThreadHandleBuf.size())
	{
		return ;
	}

	pthread_t* pThreadHandle = new pthread_t[m_listThreadHandleBuf.size()];


	list <pthread_t>::iterator Handle_Iter;
	pthread_t hThread; 

	//等待所有线程结束
	int j = 0;
	for( Handle_Iter = m_listThreadHandleBuf.begin(); Handle_Iter != m_listThreadHandleBuf.end(); ++Handle_Iter)
	{
		hThread = *Handle_Iter;		
		pThreadHandle[j++] = hThread;
	}

	::WaitForMultipleObjects((DWORD)m_listThreadHandleBuf.size(),
		pThreadHandle,TRUE,2000);	//等待两秒退出

	//关闭所有线程句柄
	for( Handle_Iter = m_listThreadHandleBuf.begin(); Handle_Iter != m_listThreadHandleBuf.end(); ++Handle_Iter)
	{
		::CloseHandle(hThread);
	}

	m_listThreadHandleBuf.clear();

	delete[] pThreadHandle;
}

void* CThreadManager::BusinessProcA( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	CRouterBusiness router(PROCESS_LIST_TYPE_A);

	//CBusiness* pBusiness = NULL;


	//业务类注册处
	CBusinessSearchOnlineUser		busSearchOnlineUser(&router);
	CBusinessAddModifyOnlineUser	busAddModifyOnlineUser(&router);
	CBusinessDelOnlineUser			busDelOnlineUser(&router);
	CBusinessGetUserFullInfo		busGetUserFullInfo(&router);
	CBusinessGetUserLinkInfo		busGetUserLinkInfo(&router);
	CBusinessGetUserStatus			busGetUserStatus(&router);
	CBusinessModifyUserStatus		busModifyUserStatus(&router);
	CBusinessImServerShutdown		busImServerShutdown(&router);
	CBusinessGetUserFlow		   	busGetUserFlow(&router);
	CBusinessModifyUserFlow		   	busModifyUserFlow(&router);
	CBusinessTUserInfoQuery			busTUserInfoQuery(&router);
	CBusinessGetBestAccount			busGetBestAccount(&router);

	//开始分发业务包
	while(!pManager->m_ThreadExitFlag[0])
	{
		router.RoutOne();
	}

	return (void*)RESULT_SUCCESS;
}


//回调函数，接收到网络层的数据时的通知
void CThreadManager::OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	//CThreadManager* pThreadManager = (CThreadManager*)pUserKey;
	switch(nEvent)
	{
	case NE_RECVDATA:	//收到一个数据包
		{
			//路由到合适的处理队列
			CRouterProtocol::GetInstance()->RouterOnePacket(pRecvData);
		}
		break;
	case NE_SENDFAILED: //发送失败
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"发送失败");
		}
		break;
	case NE_LISTENSTOP:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"监听服务停止");
		}
		break;
	case NE_ACKTIMEOUT:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"应答超时");
		}
		break;
	case NE_SENDOK:
	case NE_RESEND:
			break;
	default:
		CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"错误通知类型");
	}

}

