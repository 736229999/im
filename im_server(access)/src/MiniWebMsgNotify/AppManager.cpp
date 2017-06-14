#include "stdafx.h"
#include "AppManager.h"
#include "PublicType.h"
#include "Log.h"
#include "GlobalBusiControl.h"
//#include "BusinessExIMSrv.h"
//#include "resource.h"
#include "DataBaseLib.h"
#include "WebMsgCheck.h"
#include <time.h>
#include "RouterProtocol.h"
//#include "MobileMsgCheck.h"

//#include "Mmsystem.h"
//#pragma comment(lib,"Winmm.lib")


CAppManager::CAppManager(void):
m_hBlock(false,false)
{ 
	srand(timeGetTime());
	m_SendPacketSeq = (rand()<<16)|rand(); 
	m_pGlobalBusiControl = NULL;
	m_pGlobalFamilyBusiControl = NULL;
	m_SystemLogStatus = false;
	m_OperationStatus = false;
	m_MobileMsgStatus = false;
	m_hNameEvent = NULL;
}

CAppManager::~CAppManager(void)
{
	SAFE_DELETE(m_pGlobalBusiControl);
	SAFE_DELETE(m_pGlobalFamilyBusiControl);;
//	CloseHandle(m_hBlock);

}

bool CAppManager::Init()
{
	//��¼��������ʱ��
	time( &m_start );


	//���������
	char* pAddr = m_ConfigInfo.GetWebMsgNotifyServerIP();
	if( 0 != m_AppNetwork.StartWork(pAddr,PORT_NOTIFY_BUSINESS) )
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"����ʧ��, ip:%s, port:%d",pAddr,PORT_NOTIFY_BUSINESS);
		return false;
	}
	m_ConfigInfo.GetServerList(&m_ServerList);


	//CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);


	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceiveNULL);

	//������ȫ�ַ�����������




#ifdef IM_USE_CACHE


		list < uint32 > GlobalBusiServer;
		m_ConfigInfo.GetServerList(&GlobalBusiServer);

		
		m_pGlobalBusiControl = new GlobalBusiControl(GlobalBusiServer,PORT_MEMCACHED_ONLINE);	

		printf("CAppManager::Init 3.2\n");
#if 0

		pAddr = m_ConfigInfo.GetinListenAddr();
		if( 0 != m_pGlobalBusiControl->StartWork(pAddr,PORT_MEMCACHED_ONLINE))
		{
			printf("CAppManager::Init 3.3\n");
			CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,PORT_MEMCACHED_ONLINE);
			return false;
		}



	

		char *memcached_addr = m_ConfigInfo.GetMemcachedAddr();
		int memcached_port = m_ConfigInfo.GetMemcachedPort();
		
		ImMemCache& memcached = ImMemCache::GetInstance();
		if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
			return false;	
#endif


#else

	uint32 nListenAddr;
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineUserServerIP());
	m_pGlobalBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_USER_MANAGER);
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr))
	{
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"����ȫ�������û�������ʧ��, ip:%s, port:%d",pAddr,0);
		return false;
	}

	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineFamilyServerIP());
	m_pGlobalFamilyBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_CLUSTER_MANAGER);
	if( 0 != m_pGlobalFamilyBusiControl->StartWork(pAddr))
	{
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"����ȫ�����߼��������ʧ��, ip:%s, port:%d",pAddr,0);
		return false;
	}	
#endif
#if 0
		//����MEMCACHED���
#ifdef IM_USE_CACHE
		m_pMEMCConnCheck = new ImMemCacheConnCheck(memcached_addr,memcached_port);
		m_pMEMCConnCheck->StartWorkWebNotify();
#endif
#endif
	//���������߳�
	m_ThreadManager.StartThread();
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	StartGetOperation();
	StartGetSystemLog();
	StartGetMobileMsg();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");


	//��ʾ����̨,���������߳�,���ù����̴߳���
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL_PAL), NULL, MainDlgProc );

	if (::WaitForSingleObject(m_hBlock, INFINITE) == WAIT_OBJECT_0)
		return true;
	return true;
}

void CAppManager::StartGetSystemLog()
{
	//������վ��Ϣ����
//	if (!m_pSystemLogCheck)
//		m_pSystemLogCheck = new CSystemLogCheck;
//	m_pSystemLogCheck->StartWork();
	m_SystemLogStatus = true;
}

void CAppManager::StartGetOperation()
{
//	if (!m_pOperationCheck)
//		m_pOperationCheck = new COperationCheck;
//	m_pOperationCheck->StartWork();
	m_OperationStatus = true;
}

void CAppManager::StopGetSystemLog()
{
//	if (m_pSystemLogCheck)
//	{
//		m_pSystemLogCheck->StopWork();
//	}
	m_SystemLogStatus = false;
}
void CAppManager::StopGetOperation()
{
//	if (m_pOperationCheck)
//	{
//		m_pOperationCheck->StopWork();
//	}
	m_OperationStatus = false;
}

bool CAppManager::GetWebMsgStatus()
{
	return m_SystemLogStatus;
}
bool CAppManager::GetOperationStatus()
{
	return m_OperationStatus;
}


/*
INT_PTR CALLBACK CAppManager::MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam )
{
	switch (msg) 
	{   
	case WM_INITDIALOG:
		//OnInitDialog( hDlg );
		return FALSE;
	case WM_CLOSE:
		if( IDOK == ::MessageBoxA(hDlg,"�����ȷ��Ҫ�˳���������?","IM��̨",MB_OKCANCEL|MB_ICONQUESTION))
		{
			PostQuitMessage(0);
		}
		return FALSE;
		break;     
	case WM_COMMAND:
		return MainDlgOnCommand(hDlg,msg,wParam,lParam);
		break;
	default:
		return FALSE;
	}
	return TRUE;
}

BOOL CAppManager::MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam)
{
	//char szInfo[1024];

	switch (LOWORD(wParam))
	{
	case IDOK:
		if( IDOK == ::MessageBoxA(hDlg,"�����ȷ��Ҫ�˳���������?","IM��̨",MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
		break;	
	case 2: //IDC_WEBMSG_BUTTON
		{
// 			char stat[128];
// 			if (CAppManager::GetInstance().GetWebMsgStatus())
// 				strcpy(stat,"ȷ��Ҫ\"ֹͣ\"վ��֪ͨ��Ϣ��(ֻ����һ̨IM������������!)��");
// 			else
// 				strcpy(stat,"ȷ��Ҫ\"����\"վ��֪ͨ��Ϣ��(ֻ����һ̨IM������������!)��");
// 
// 
// 			if( IDOK != ::MessageBoxA(hDlg,stat,"IM��̨",MB_OKCANCEL))
// 			{
// 				break;
// 			}
// 			if (CAppManager::GetInstance().GetWebMsgStatus())
// 			{
// 				CAppManager::GetInstance().StopGetWebMsg();
// 				::SetWindowText(::GetDlgItem(hDlg,IDC_WEBMSG_BUTTON),"����վ��֪ͨ��Ϣ");
// 				::UpdateWindow(::GetDlgItem(hDlg,IDC_WEBMSG_BUTTON));
// 			}
// 			else
// 			{
// 				CAppManager::GetInstance().StartGetWebMsg();
// 				::SetWindowText(::GetDlgItem(hDlg,IDC_WEBMSG_BUTTON),"ֹͣվ��֪ͨ��Ϣ");
// 				::UpdateWindow(::GetDlgItem(hDlg,IDC_WEBMSG_BUTTON));
// 			}

		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
*/

CAppManager& CAppManager::GetInstance()
{
	static CAppManager	SingleInstance;
	return SingleInstance;
}

void CAppManager::Exit()
{

	m_ThreadManager.EndThread();

	m_AppNetwork.StopWork();

	if( m_pGlobalBusiControl )
	{
		m_pGlobalBusiControl->StopWork();
	}
	if( m_pGlobalFamilyBusiControl )
	{
		m_pGlobalFamilyBusiControl->StopWork();
	}
	StopGetSystemLog();
	StopGetOperation();
	StopGetMobileMsg();


}

void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}

list <uint32> *CAppManager::GetServerList()
{
	return &m_ServerList;
}


void CAppManager::StartGetMobileMsg()
{
	//�����ֻ������·���Ϣ
	//if (!m_pMobileMsgCheck)
	//	m_pMobileMsgCheck = new CMobileMsgCheck;
	//m_pMobileMsgCheck->StartWork();
	m_MobileMsgStatus = true;
}

void CAppManager::StopGetMobileMsg()
{
	//if (m_pMobileMsgCheck)
	//{
	//	m_pMobileMsgCheck->StopWork();
	//}
	m_MobileMsgStatus = false;
}

bool CAppManager::GetMobileMsgStatus()
{
	return m_MobileMsgStatus;
}
