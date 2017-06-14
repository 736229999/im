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



//�������������߳�
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

//���������߳�
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

	//�ȴ������߳̽���
	int j = 0;
	for( Handle_Iter = m_listThreadHandleBuf.begin(); Handle_Iter != m_listThreadHandleBuf.end(); ++Handle_Iter)
	{
		hThread = *Handle_Iter;		
		pThreadHandle[j++] = hThread;
	}

	::WaitForMultipleObjects((DWORD)m_listThreadHandleBuf.size(),
		pThreadHandle,TRUE,2000);	//�ȴ������˳�

	//�ر������߳̾��
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


	//ҵ����ע�ᴦ
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

	//��ʼ�ַ�ҵ���
	while(!pManager->m_ThreadExitFlag[0])
	{
		router.RoutOne();
	}

	return (void*)RESULT_SUCCESS;
}


//�ص����������յ�����������ʱ��֪ͨ
void CThreadManager::OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{
	//CThreadManager* pThreadManager = (CThreadManager*)pUserKey;
	switch(nEvent)
	{
	case NE_RECVDATA:	//�յ�һ�����ݰ�
		{
			//·�ɵ����ʵĴ������
			CRouterProtocol::GetInstance()->RouterOnePacket(pRecvData);
		}
		break;
	case NE_SENDFAILED: //����ʧ��
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"����ʧ��");
		}
		break;
	case NE_LISTENSTOP:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"��������ֹͣ");
		}
		break;
	case NE_ACKTIMEOUT:
		{
			CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"Ӧ��ʱ");
		}
		break;
	case NE_SENDOK:
	case NE_RESEND:
			break;
	default:
		CLog::Log("CThreadManager",CLog::TYPE_DEBUG,"����֪ͨ����");
	}

}

