#include "stdafx.h"
#include "ThreadManager.h"
#include "RouterBusiness.h"
#include "MiniAppNetwork.h"
#include "RouterProtocol.h"
#include "ConfigInfo.h"
#include "AppManager.h"
#include "BusinessOperationMsg.h"



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

	//�߳�A
	pthread_t hThread; 
	for(int i = 0 ; i < 1 ; ++i )
	{
		hThread = CreateThread( 
			NULL,                       // default security attributes 
			0,                          // use default stack size  
			BusinessProcA,				// thread function 
			this,						// argument to thread function 
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
		hThread = *Handle_Iter;
		::CloseHandle(hThread);
	}

	m_listThreadHandleBuf.clear();

	delete[] pThreadHandle;
}


void * CThreadManager::BusinessProcA( LPVOID lpParam )
{
	CThreadManager* pManager = (CThreadManager*)lpParam;

	//��ʼ����Դ����
	CRouterBusiness router(PROCESS_LIST_TYPE_A);

	//��ʼ�����ݿ�
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}
	CDataBaseFamily DatabaseFamily;
	DatabaseFamily.SetConnectDB(familyuid, familypwd, familysid);
	if (!DatabaseFamily.ConnectDB())
	{
		CLog::Log("CThreadManager::BusinessProcA",CLog::TYPE_ERROR,"DatabaseFamily.ConnectDB() failed!");
		return (void*)RESULT_FAILED;
	}


	router.SetDataBase(&DatabaseMini,&DatabaseWeb,&DatabaseFamily);

	//��ʼ����ȫ�ַ�����������
	CGlobalBusi GlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl());
	CGlobalBusi GlobalFamilyBusi(CAppManager::GetInstance().GetGlobalFamilyBusiControl());
	router.SetGlobalBusi(&GlobalBusi,&GlobalFamilyBusi);

	//ҵ����ע�ᴦ
	//CBusiRegister				busRegister(&router);

	 CBusinessOperationMsg      busOperationMsg(&router);
	 CBusinessSystemLog			busSystemLogMsg(&router);
	 CBusinessMobileMsg			busMobileMsg(&router);


	CLog::Log("CThreadManager",CLog::TYPE_IMPORTANT,"�߳�Aҵ��ע�����");

	//��ʼ�ַ�ҵ���
	while(!pManager->m_ThreadExitFlag[PROCESS_LIST_TYPE_A])
	{
		router.RoutOne();
	}

	//DatabaseWeb.DisConnect();

	return RESULT_SUCCESS;
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


