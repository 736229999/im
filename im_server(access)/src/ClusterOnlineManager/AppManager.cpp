#include "stdafx.h"
#include "AppManager.h"
#include "Log.h"
#include "BusinessExGSrv.h"
#include "resource.h"
  
CAppManager*  CAppManager::m_pInstance = NULL;

CAppManager::CAppManager(void):m_hBlock(FALSE,FALSE)
{
	
}

CAppManager::~CAppManager(void)
{
	m_Network.StopListen();
}

CAppManager& CAppManager::GetInstance()
{
	static CAppManager SingleInstance;
	return SingleInstance;
}


bool CAppManager::Init()
{
	char* pAddr = m_ConfigInfo.GetOnlineFamilyServerIP(); 
	int	  nPort = PORT_ONLINE_CLUSTER_MANAGER;
	if( 0 != m_Network.StartListen(pAddr,nPort) )
	{
		CLog::Log("CAppManager::init",CLog::TYPE_ERROR,"Listen failed,ip:%s, port:%d",pAddr,nPort);
		return false;
	}

	m_Network.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);
	CBusinessExGSrv::SetNetTrans(&m_Network);
	CBusinessExGSrv::SetServerID(SERVERID_ONLINE_CLUSTER);


	m_ThreadManager.StartThread();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");

	//显示控制台,并阻塞主线程,仅让工作线程处理
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL_PAL), NULL, MainDlgProc );

	m_hBlock.Wait();

	return true;
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
		if( IDOK == ::MessageBoxA(hDlg,"你真的确定要退出服务器吗?","IM后台",MB_OKCANCEL|MB_ICONQUESTION))
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
	char szInfo[256];

	switch (LOWORD(wParam))
	{
	case IDOK:
		if( IDOK == ::MessageBoxA(hDlg,"你真的确定要退出服务器吗?","IM后台",MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
		break;
	case IDC_BUTTON_SHOW_FAMILY_INFO:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_INFO),szInfo,sizeof(szInfo));
			int nFamilyId = atoi(szInfo);
			if( nFamilyId > 0)
			{
				CAppManager::GetInstance().m_ClusterManager.GetClusterFromID(nFamilyId,szInfo);
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_INFO),szInfo);
			}
			else
			{
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_INFO),"请输入正确的家族ID");
			}
		}
		break;
	default:
		return FALSE;
	}
	return TRUE;
}
*/

void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}

