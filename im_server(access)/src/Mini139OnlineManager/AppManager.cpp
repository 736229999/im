#include "stdafx.h"
#include "AppManager.h"
#include "Log.h"
#include "RouterBusiness.h"
#include "BusinessAddModifyOnlineUser.h"
#include "Function.h"
#include "BusinessExGSrv.h"
#include "resource.h"
#include <time.h>


#include "backctrl.h"


#define TIMERPERIOD 60000
void  WINAPI MyOnlineProc(UINT uTimerID, UINT uMsg, LPVOID dwData, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CAppManager * pManger = (CAppManager*)dwData;
	pManger->m_DatabaseMini.Save_OnlineUser_Num(pManger->GetOnlineManager()->GetOnlineUserNum());
}


CAppManager*  CAppManager::m_pInstance = NULL;
//CTcpNetTrans CAppManager::m_BackNet;

CAppManager::CAppManager(void):m_hBlock(FALSE,FALSE)
{
	srand(timeGetTime());
	m_SendPacketSeq = (rand()<<16)|rand(); 
}

CAppManager::~CAppManager(void)
{
	m_Network.StopListen();
//	m_BackNet.StopListen();
}

CAppManager& CAppManager::GetInstance()
{
	static CAppManager SingleInstance;
	return SingleInstance;
}


bool CAppManager::Init()
{

	time( &m_start );


	char* pAddr = m_ConfigInfo.GetOnlineUserServerIP(); 
	int	  nPort = PORT_ONLINE_USER_MANAGER;
	if( 0 != m_Network.StartListen(pAddr,nPort) )
	{
		CLog::Log("CAppManager::init",CLog::TYPE_ERROR,"Listen failed,ip:%s, port:%d",pAddr,nPort);
		return false;
	}

	CBusinessExGSrv::SetServerID(SERVERID_ONLINE_USER);

	m_Network.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	CBusinessExGSrv::SetNetTrans(&m_Network);

	m_ThreadManager.StartThread();


	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	m_ConfigInfo.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);

	m_DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
/*	if( !m_DatabaseMini.ConnectDB() )
	{
		CLog::Log("CAppManager::LogoutAllUser","DatabaseMini.ConnectDB() failed!");
	}

	m_hMMT = timeSetEvent(TIMERPERIOD,TIMERPERIOD,(LPTIMECALLBACK)MyOnlineProc,(DWORD_PTR)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"在线记数定时器设置失败");
	}
*/

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");
//	m_BackNet.SetUserCallback(CAppManager::OnReceive);
//	m_BackNet.StartListen("",10010);

	//显示控制台,并阻塞主线程,仅让工作线程处理
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL), NULL, MainDlgProc );

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
	char szInfo[1024];

	switch (LOWORD(wParam))
	{
	case IDOK:
		if( IDOK == ::MessageBoxA(hDlg,"你真的确定要退出服务器吗?","IM后台",MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
		break;
	case IDC_BUTTON_ONLINE_NUM:
		{
			itoa(CAppManager::GetInstance().GetOnlineManager()->GetOnlineUserNum(),szInfo,10);
			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_SHOW_USERINFO:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));
			int nUserId = atoi(szInfo);
			if( nUserId > 0)
			{
				COnlineUser* pUser = CAppManager::GetInstance().GetOnlineManager()->GetUser(nUserId);
				if( pUser )
				{
					in_addr addr, Serveraddr;
					addr.S_un.S_addr = pUser->m_ip;
					Serveraddr.S_un.S_addr = htonl(pUser->m_ServerIp);
					char ip[32];
					char Serip[32];

					strncpy(ip,inet_ntoa(addr),sizeof(ip));
					strncpy(Serip,inet_ntoa(Serveraddr),sizeof(Serip));

					string strStatus;
					switch(pUser->m_status) 
					{
					case XTREAM_ONLINE:
						strStatus = "在线";
						break;
					case XTREAM_LEAVE:
						strStatus = "离开";
						break;
					case XTREAM_HIDE:
						strStatus = "隐身";
						break;
					case XTREAM_OFFLINE:
						strStatus = "离线";
						break;
					default:
						strStatus = "无效状态";
					}

					sprintf(szInfo,"ID:%d  UserName:%s  NickName:%s \r\nIP:%s Port:%d \r\nServerIP:%s \r\nStatus:%d(%s) ",
						pUser->m_UserID , pUser->m_UserName,pUser->m_NickName,
						ip, pUser->m_port, Serip, pUser->m_status,strStatus.c_str());
					::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo); 

				}
				else
				{
					::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"该用户不在线");
				}
			}
			else
			{
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"输入有误");
			}
		}
		break;
	case IDC_BUTTON_RUN_TIME:
		{
			time_t   finish;
			int   elapsed_time;
			time( &finish );
			elapsed_time = (int)difftime( finish, CAppManager::GetInstance().m_start );

			int nDay,nHours,nMinute ,nSecond;
			nDay = elapsed_time/(3600*24);
			elapsed_time -= nDay*3600*24;
			nHours = elapsed_time/3600;
			elapsed_time -= nHours*3600;
			nMinute = elapsed_time/60;
			elapsed_time -= nMinute*60;
			nSecond = elapsed_time;

			struct tm *ptmStart;
			ptmStart = localtime( &CAppManager::GetInstance().m_start );


			sprintf(szInfo,"启动时间:%d年%d月%d日 %d:%d:%d\r\n到现在程序已运行了%d天%d小时%d分%d秒",
				ptmStart->tm_year+1900,ptmStart->tm_mon,ptmStart->tm_mday,ptmStart->tm_hour,ptmStart->tm_min,ptmStart->tm_sec,
				nDay,nHours,nMinute,nSecond);

			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_TICKOUT:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));
			int nUserId = atoi(szInfo);
			int ret =  ::MessageBoxA(hDlg,"是否要登出用户并修改其密码","IM后台",MB_YESNOCANCEL);
			if (ret == IDYES)
			{
				ret = 1;
			}
			else if(ret == IDNO)
			{
				ret = 0;				
			}
			else if (ret == IDCANCEL)
			{
				break;
			}
			if( nUserId > 0)
			{
				COnlineUser* pUser = CAppManager::GetInstance().GetOnlineManager()->GetUser(nUserId);
				if( pUser )
				{
// 					CRouterBusiness router(0);
// 					CBusinessAddModifyOnlineUser	busAddModifyOnlineUser(&router);
// 					busAddModifyOnlineUser.NotifyRelogin(pUser->m_ServerIp,pUser->m_UserID);
					SendDataStruct SendData;
					XT_HEAD* pSendHead = (XT_HEAD*)SendData.data;
					//RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;

					CSendPacket SendPacket;
					SendPacket.SetData(SendData.data+ sizeof(XT_HEAD),
						MAX_SEND_BUFFERS - sizeof(XT_HEAD) );
					SendPacket << pUser->m_UserID
								<<(uint8)ret
								<<(uint32)0
								<<(uint32)0;


					//构建网络层的信息
					SendData.dest_ip = pUser->m_ServerIp;
					SendData.port	   = PORT_APPNET2GLOBAL;
					SendData.vlink   = 0;
					SendData.retry_times = SEND_RETRY_TIME;
					SendData.data_len  = (uint16)SendPacket.GetSize() + sizeof(XT_HEAD);


					//构建协议头的信息
					//XT_HEAD* pRecvHead = (XT_HEAD*)pRecData->data;

					pSendHead->flag = XT_HEAD::FLAG_SRVACTION;
					pSendHead->from = XT_HEAD::FROM_SERVER;
					pSendHead->cmd  = CMDGSRV_USERRELOGIN_NOTIFY;
					pSendHead->seq  = CAppManager::GetInstance().GetSendSeq();
					pSendHead->sid  = XT_HEAD::SID_SERVER;
					pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
					pSendHead->did  = XT_HEAD::SID_SERVER;
					pSendHead->len  = SendPacket.GetSize();


					//加密数据
					char m_globalSessionKey[16];
					memset(m_globalSessionKey,0,sizeof(m_globalSessionKey));					
					int nDataLength = SendData.data_len;
					::xTEAEncryptWithKey(SendData.data + sizeof(XT_HEAD) ,SendData.data_len - sizeof(XT_HEAD) ,
						m_globalSessionKey,
						SendData.data + sizeof(XT_HEAD) ,&nDataLength ) ;
					SendData.data_len = (uint16)nDataLength;

					SendData.data_len += sizeof(XT_HEAD);

					//发送数据
					CAppManager::GetInstance().GetAppNetWork().SendDataTo(SendData);
					COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();
					pOnlineManager->DelUser(pUser->m_UserID);	
					//::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"成功登出用户");
					
				}
				else
				{
					::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"该用户不在线");
				}
			}
			else
			{
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"输入有误");
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



void CAppManager::OnReceive(void *pUserKey,int nEvent,char * pRecvData,int nlen)
{
	uint8 result = RESULT_SUCCESS;
	char szInfo[1024];
// 	char key[16] = {0};
// 	strncpy(key,"abcdefghigkl1234",16);
	char szMsg[256] = {0};

	int tlen = 0;
	if(!xTEADecryptWithKey(pRecvData,nlen,g_backkey,(char *)pRecvData,&tlen))
		return;
	CRecvPacket recvPacket;
	recvPacket.SetData((char*)pRecvData,tlen);
	uint32 cmd;
	recvPacket>>cmd;
	switch(cmd)
	{
	case CMD_BACK_LOGIN:
		{
			char username[MAX_USERNAME_LEN+1] = {0},password[32] = {0};
			recvPacket >> username
					   >> password; 
			if (strcmp(username,"admin") == 0 && strcpy(password,"123456") == 0)
			{
				result = RESULT_FAILED;
				strcpy(szMsg,"用户名或密码错误");
			}
			else
				strcpy(szMsg,"登录成功");
		}
		break;
	
	case CMD_BACK_LOGIN3:
	    break;
	default:
	    break;
	}


	CSendPacket sendPacket;
	memset(szInfo,0,sizeof(szInfo));
	sendPacket.SetData(szInfo,1024);
	sendPacket	<< uint32(CMD_BACK_LOGINACK)
		<< result
		<< szMsg;
	if(!xTEAEncryptWithKey(szInfo,sendPacket.GetSize(),g_backkey,szInfo,&tlen))
		return;


//	m_BackNet.SendData(pUserKey,szInfo,tlen);
}

