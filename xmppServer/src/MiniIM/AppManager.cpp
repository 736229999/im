#include "stdafx.h"
#include "AppManager.h"
#include "PublicType.h"
#include "Log.h"
#include "GlobalBusiControl.h"
#include "ConnBusi.h"
#include "ConnBusiControl.h"
#include "BusinessExIMSrv.h"
#include "BusiHeartTick.h"
//#include "resource.h"
#include "DataBaseLib.h"
#include "BusinessWebMsg.h"
#include "BusinessWebIM.h"
#include <time.h>
#include "RouterProtocol.h"
#include "../DataBaseLib/MysqlLib.h"
#ifdef IM_USE_SSDB
#include "../../ssdb/SSDBConn.h"
#endif



/* �汾˵��
ver:0105 ������Ϣ����
ver:0106 ȡ��������Ϣ����

*/

CAppManager::CAppManager(void):
m_hBlock(false,false)
{ 
	srand(timeGetTime());
	m_SendPacketSeq = (rand()<<16)|rand(); 
	m_pGlobalBusiControl = NULL;
	m_pGlobalFamilyBusiControl = NULL;
	m_pHeartTickCheck = NULL;
//	m_WebMsgStatus = false;
	m_pWebIMMsgCheck = NULL;
	m_hNameEvent = NULL;
	m_pSSDBConnCheck = NULL;

	m_nFD = -1;
}

CAppManager::~CAppManager(void)
{
	SAFE_DELETE(m_pGlobalBusiControl);
	SAFE_DELETE(m_pGlobalFamilyBusiControl);
	SAFE_DELETE(m_pConnBusiControl);
	SAFE_DELETE(m_pConnBusi);
	SAFE_DELETE(m_pHeartTickCheck);
	SAFE_DELETE(m_pWebIMMsgCheck);
	SAFE_DELETE(m_pTimeLenSave);
	//��������
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM; i++)
	{
		SAFE_DELETE(m_pChatMsgPacket[i]);
		SAFE_DELETE(m_pChatMsgSave[i]);
	}

}

bool CAppManager::Init()
{
	//��¼��������ʱ��
	time( &m_start );

	/*
	//Ϊ�ػ����̵��ź�ͨѶ��׼��
	m_hNameEvent = OpenEvent(EVENT_ALL_ACCESS,FALSE,"ImServer");
	if( m_hNameEvent == NULL )
	{
		CLog::Log("CAppManager","can't open named event ,maybe the protect process is not running");
	}
*/
	//CLog::Log("CAppManager","Version 1.0.0.2");
	printf("CAppManager::Init \n");
	//���������
	char* pAddr = m_ConfigInfo.GetListenAddr();
	if( 0 != m_AppNetwork.StartWork(pAddr,PORT_IM_BUSINESS) )
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Listen falied,ip:%s, port:%d",pAddr,PORT_IM_BUSINESS);
		return false;
	}

	UpdateVersion();

	printf("CBusinessExIMSrv::SetNetTrans\n");
	CBusinessExIMSrv::SetNetTrans(&m_AppNetwork);
	CBusiness::SetServerID(m_ConfigInfo.GetServerNo());

	//����mysql����
	char* MySQLSid = m_ConfigInfo.GetMySQLSid();
	char* MySQLUser = m_ConfigInfo.GetMySQLUser();
	char* MySQLPwd = m_ConfigInfo.GetMySQLPwd();
	char* MySQLDb = m_ConfigInfo.GetMySQLDb();
	if (!MYSQL_LIB->connect(MySQLDb,MySQLSid,MySQLUser,MySQLPwd)) {
		return false;
	}
	
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceiveNULL);
	printf("m_AppNetwork.SetUserCallback\n");

	//������ȫ�ַ�����������
	uint32 nListenAddr;
	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineUserServerIP());
		printf("CAppManager::Init 3.1\n");
	m_pGlobalBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_USER_MANAGER);
	printf("CAppManager::Init 3.2\n");
	pAddr = m_ConfigInfo.GetinListenAddr();
	if( 0 != m_pGlobalBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 3.3\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online user service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}

	printf("after m_pGlobalBusiControl->StartWork\n");

	nListenAddr = inet_addr(m_ConfigInfo.GetOnlineFamilyServerIP());
		
	printf("CAppManager::Init 2\n");

	m_pGlobalFamilyBusiControl = new GlobalBusiControl(nListenAddr,PORT_ONLINE_CLUSTER_MANAGER);
	printf("CAppManager::Init 3\n");
	if( 0 != m_pGlobalFamilyBusiControl->StartWork(pAddr))
	{
		printf("CAppManager::Init 4\n");
		CLog::Log("CAppManager GlobalBusiControl",CLog::TYPE_ERROR,"Global online family service listen failed,ip:%s, port:%d",pAddr,0);
		return false;
	}	

	printf("after m_pGlobalFamilyBusiControl->StartWork\n");

	//SSDB��ʼ��
#ifdef IM_USE_SSDB
	int num = m_ConfigInfo.GetMaxTdGrpA();
	char* ssdbAddr = m_ConfigInfo.GetSSDBAddr();
	SSDBLinks& ssdbLinks = SSDBLinks::GetInstance();
	if(ssdbLinks.init_links(num,ssdbAddr,PORT_SSDB) < 0)
	{
		CLog::Log("CAppManager",CLog::TYPE_ERROR,"Ssdb links connect fail");
		return false;
	}	
	else
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"ssdb links connect success");
#endif
#ifdef USE_CONNECT_SERVER
	//��������������������
	if( !IniConnectServer())
	{
		return false;
	}
	printf("after IniConnectServer\n");
#endif
	//���������߳�
	m_ThreadManager.StartThread();
	m_AppNetwork.SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);

	//������������
	m_pHeartTickCheck = new CHeartTickCheck;
	m_pHeartTickCheck->StartWork();

	//����SSDB���
	#ifdef IM_USE_SSDB
	m_pSSDBConnCheck = new SSDBConnCheck(ssdbAddr);
	m_pSSDBConnCheck->StartWork();
	#endif

	//����WEBIM��ͨ(����)��Ϣ�����߳�
	m_pWebIMMsgCheck = new CWebIMMsgCheck;
	m_pWebIMMsgCheck->StartWork();

	//����httpserver�߳�
	m_pHttpServer = new CHttpServer;
	m_pHttpServer->StartWork();


#ifdef IMG_SAVE

	//����д��Ϣ��¼�߳�
//	m_pChatMsgPacket = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, sizeof(XT_TALK));
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM ; i++)
	{
		m_pChatMsgPacket[i] = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, sizeof(XT_TALK));
		
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init m_pChatMsgPacket %p",m_pChatMsgPacket[i]);
	}
	for(int i = 0 ; i < MAX_MSG_PTHREAD_NUM ; i++)
	{
		m_pChatMsgSave[i] = new CChatMsgSave;
		m_pChatMsgSave[i]->StartWork();
	}

#endif 	
	//������ʱ��¼�û������ﳤ
// 	m_pTimeLenSave = new CTimeLenSave;
// 	m_pTimeLenSave->StartWork();

	

	m_pJabberdIm = new JabberdIm;
	m_pJabberdIm->SetUserCallback(&m_ThreadManager,CThreadManager::OnReceive);
	m_pJabberdIm->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");

	// ����tcp socket����������JabberdIm�߳�
	struct sockaddr_in addr;
	bzero(&addr, sizeof(addr));
	addr.sin_family = AF_INET;
	addr.sin_port = htons(5222);
	addr.sin_addr.s_addr = inet_addr("127.0.0.1");
	m_nFD = socket(AF_INET, SOCK_STREAM, 0);

	unsigned long ul = 1;
	ioctl(m_nFD, FIONBIO, &ul); //����Ϊ������ģʽ

	// ����10������
	int nRlt = -1;
	for(int i = 0; i < 10; ++i)
	{
		sleep(1);
		nRlt = connect(m_nFD, (const sockaddr*)&addr, sizeof(addr));

		if(nRlt != -1)
		{
			break;
		}
	}

	if(nRlt == -1)
	{	// ����ʧ��
		m_nFD = -1;
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"error:%s", strerror(errno));
	}
	else
	{	// ���ӳɹ�
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"success: fd: %d", m_nFD);
	}


	//��ʾ����̨,���������߳�,���ù����̴߳���
	//DialogBox( GetModuleHandle(NULL), MAKEINTRESOURCE(IDD_DIALOG_CONTROL_PAL), NULL, MainDlgProc );

	if (::WaitForSingleObject(m_hBlock, INFINITE) == WAIT_OBJECT_0)
		return true;
	return true;
}

//��������������������
bool CAppManager::IniConnectServer()
{
	printf("CAppManager::IniConnectServer\n");
	char* pAddr = m_ConfigInfo.GetinListenAddr();
	int nListenAddr = inet_addr(m_ConfigInfo.GetConnectServerIP());
	m_pConnBusiControl = new CConnBusiControl(nListenAddr,PORT_CONN_SERVER);
	m_pConnBusi = new CConnBusi(m_pConnBusiControl);
	m_pConnBusiControl->SetConnBusi(m_pConnBusi);
	if( 0 != m_pConnBusiControl->StartWork(pAddr))
	{
		CLog::Log("CAppManager CConnBusiControl",CLog::TYPE_ERROR,"�������������ʧ��, ip:%s, port:%d",pAddr,0);
		return false;
	}


	//�����ע��
	SERVER_REGISTER srvRegister;
	srvRegister.dnsIP = inet_addr(pAddr);
	srvRegister.flags = 0x00;
	srvRegister.maxCount = GetConfig().GetMaxUserCount();
	srvRegister.srvType = 0x01;
	srvRegister.udpPort = PORT_IM_SVRP2P;

	m_pConnBusi->BusiRequest(&srvRegister); 

	return true;
}

bool CAppManager::UnregistConnectServer()
{
	SERVER_UNREGISTER srvUnregister;
	srvUnregister.authData = 0x00;
	m_pConnBusi->BusiRequest(&srvUnregister); 
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
	char szInfo[1024];

	switch (LOWORD(wParam))
	{
	case IDOK:
		if( IDOK == ::MessageBoxA(hDlg,"�����ȷ��Ҫ�˳���������?","IM��̨",MB_OKCANCEL))
		{
			PostQuitMessage(0);
		}
		break;
	case IDC_BUTTON_USER_NUM:
		{
			_itoa_s(CAppManager::GetInstance().GetUserManager().GetElemCount(),szInfo,10);
			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_SHOW_USERINFO_FROM_ID:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));
			int nUserId = atoi(szInfo);
			if( nUserId > 0)
			{
				CUser* pUser = CAppManager::GetInstance().GetUserManager().Find(nUserId);
				if( pUser )
				{
					in_addr addr, lanaddr;
					addr.S_un.S_addr = pUser->m_nIP;
					lanaddr.S_un.S_addr = htonl(pUser->m_nLanIP);
					char ip[32];
					char lanip[32];
					strncpy_s(ip,inet_ntoa(addr),sizeof(ip));
					strncpy_s(lanip,inet_ntoa(lanaddr),sizeof(lanip));

					string strStatus;
					switch(pUser->m_nStatus) 
					{
					case XTREAM_ONLINE:
						strStatus = "����";
						break;
					case XTREAM_LEAVE:
						strStatus = "�뿪";
						break;
					case XTREAM_HIDE:
						strStatus = "����";
						break;
					case XTREAM_OFFLINE:
						strStatus = "����";
						break;
					default:
						strStatus = "��Ч״̬";
					}

					sprintf_s(szInfo,"ID:%d  UserName:%s  NickName:%s \r\nIP:%s Port:%d \r\nLanIP:%s LanPort:%d \r\n Status = %d(%s)",
						pUser->m_nID,pUser->m_szUserName,pUser->m_szNickName,
						ip, pUser->m_nPort, lanip, pUser->m_nLanPort, pUser->m_nStatus,strStatus.c_str());
					::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo); 
					
				}
				else
				{
					::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"���û������߻�û�е�¼����������");
				}
			}
			else
			{
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"��������");
			}
		}
		break;
	case IDC_BUTTON_PROECSSLISTNUM:
		{
			CAppManager& AppManager = CAppManager::GetInstance();
			sprintf_s(szInfo,"IMӦ��ҵ�����\r\n\tҵ��A����:%d\tҵ��B����:%d\tҵ��C����:%d\tҵ��D����:%d\r\n��������\r\n\t�������������� ��:%d,��:%d,�ش�:%d\tȫ�ֲ�ѯ���� ��:%d,��:%d\r\nȫ�ַ������\r\n\tȫ���û�������С���:%d,��:%d\tȫ�ּ��������С���:%d,��:%d",
				AppManager.m_ThreadManager.GetBusProcBuf(PROCESS_LIST_TYPE_A)->GetSize(),
				AppManager.m_ThreadManager.GetBusProcBuf(PROCESS_LIST_TYPE_B)->GetSize(),
				AppManager.m_ThreadManager.GetBusProcBuf(PROCESS_LIST_TYPE_C)->GetSize(),
				AppManager.m_ThreadManager.GetBusProcBuf(PROCESS_LIST_TYPE_D)->GetSize(),
				
				AppManager.m_AppNetwork.GetSrvNetTrans()->GetSendQueueSize(),
				AppManager.m_AppNetwork.GetSrvNetTrans()->GetRecvQueuesize(),
				AppManager.m_AppNetwork.GetSrvNetTrans()->GetReSendListSize(),
				AppManager.m_AppNetwork.GetCntNetTrans()->GetSendQueueSize(),
				AppManager.m_AppNetwork.GetCntNetTrans()->GetRecvQueuesize(),
				
				AppManager.m_pGlobalBusiControl->GetNetTrans()->GetSendQueueSize(),
				AppManager.m_pGlobalBusiControl->GetNetTrans()->GetRecvQueuesize(),
				AppManager.m_pGlobalFamilyBusiControl->GetNetTrans()->GetSendQueueSize(),
				AppManager.m_pGlobalFamilyBusiControl->GetNetTrans()->GetRecvQueuesize()  );

			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_FAMILY_NUM:
		{
			sprintf_s(szInfo,"�̶�����:%d  ��ʱ����:%d ",
				CAppManager::GetInstance().GetPermFamilyManager().GetElemCount(),
				CAppManager::GetInstance().GetTempFamilyManager().GetElemCount() ) ;
			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_SHOW_FAMILY_INFO:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));
			int nFamilyId = atoi(szInfo);
			if( nFamilyId > 0)
			{
				if( 0x80000000 & nFamilyId )
				{
					CTempFamily* pTempFamily = CAppManager::GetInstance().GetTempFamilyManager().Find(nFamilyId);
					if( pTempFamily )
					{
						sprintf_s(szInfo,"��ʱ���� id:%d  ",
							pTempFamily->m_FamilyID);
						::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
					}
					else
					{
						::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"�ü��岻���߻�û�е�¼����������");
					}
				}
				else
				{
					CPermFamily* pPermFamily = CAppManager::GetInstance().GetPermFamilyManager().Find(nFamilyId);
					if( pPermFamily )
					{
						char strtime[32];
						ctime_s(strtime,32,&pPermFamily->tm);
						sprintf_s(szInfo,"�̶����塡id:%d  familyname:%s  ����ʱ��:%s  ",
							pPermFamily->m_FamilyID,pPermFamily->m_FamilyName, strtime);
						::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
					}
					else
					{
						::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"�ü��岻���߻�û�е�¼����������");
					}
				}
			}
			else
			{
				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"��������");
			}
		}
		break;
	case IDC_BUTTON_SHOW_ONLINE_USER:
		{
// 			CUser * pUserLinkHead = CUser::GetUserListHead();
// 
// 			if(!pUserLinkHead)
// 			{
// 				::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),"û�������û�");
// 				break;
// 			}
// 
// 			CUser *pUserCurPiont = pUserLinkHead;
// 			string strInfo;
// 
// 			CUser * ptmpUser = NULL;
// 			
// 			for(int i=0;(i<100 && pUserCurPiont); ++i)
// 			{
// 				sprintf(szInfo,"id:%d\t username:%s\t nickname:%s\r\n",
// 									pUserCurPiont->m_nID,
// 									pUserCurPiont->m_szUserName,
// 									pUserCurPiont->m_szNickName);
// 				strInfo+=szInfo;
// 				pUserCurPiont = pUserCurPiont->m_next;
// 			}
// 			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),strInfo.c_str());

		}
		break;
	case IDC_UPDATEVERSION_BUTTON:
		{
			CAppManager::GetInstance().UpdateVersion();
			sprintf_s(szInfo, "���õ�ַ:\t %s\r\n��Ͱ汾:\t 0x0%x\r\n���°汾:\t 0x0%x\r\n������ʽ:\t %s\r\n������Ϣ:\t%s\r\n",
				CAppManager::GetInstance().GetUpdateConfig(),
				CAppManager::GetInstance().GetCanUserver(),
				CAppManager::GetInstance().GetAgreever(),
				CAppManager::GetInstance().GetUpdateOption(),
				CAppManager::GetInstance().GetUpdateInfo());
			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_SEND_PUB:
		{
			if( IDOK != ::MessageBoxA(hDlg,"ȷ��Ҫ����ϵͳ����������û���","IM��̨",MB_OKCANCEL))
			{
				break;
			}
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));

			
			time_t tm;
			time(&tm);

			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

			LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= XT_SERVER_TRANSMIT::TYPE_SYS_MESSAGE;


			char* pMsgData = sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT);
			CSendPacket SendPacket;
			SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN - sizeof(XT_SERVER_TRANSMIT) );


			SendPacket		<< (uint32)tm
							<< (uint8)0			
							<< szInfo;

			sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize());

// 			CUser *pUser = CUser::m_pUserListHead;
// 			while(pUser)
// 			{
// 				CAppManager::GetInstance().InitHead(
// 					lpSendHead,
// 					CMD_SERVERMSG_IND,
// 					(uint16)(sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize()),
// 					pUser->m_nID);
// 
// 				CAppManager::GetInstance().GetAppNetWork().SendDataTo(
// 					sendData.data,sendData.data_len,SEND_RETRY_TIME);
// 
// 				pUser = pUser->m_next;
// 			}
		}
		break;
	case IDC_WEBMSG_BUTTON:
		{
// 			char stat[128];
// 			if (CAppManager::GetInstance().GetWebMsgStatus())
// 				strcpy_s(stat,"ȷ��Ҫ\"ֹͣ\"վ��֪ͨ��Ϣ��(ֻ����һ̨IM������������!)��");
// 			else
// 				strcpy_s(stat,"ȷ��Ҫ\"����\"վ��֪ͨ��Ϣ��(ֻ����һ̨IM������������!)��");
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

			struct tm ptmStart;
			localtime_s(&ptmStart,&CAppManager::GetInstance().m_start );


			sprintf_s(szInfo,"����ʱ��:%d��%d��%d�� %d:%d:%d\r\n�����ڳ�����������%d��%dСʱ%d��%d��",
				ptmStart.tm_year+1900,ptmStart.tm_mon+1,ptmStart.tm_mday,ptmStart.tm_hour,ptmStart.tm_min,ptmStart.tm_sec,
				nDay,nHours,nMinute,nSecond);

			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_AUDIT_BUS:
		{
			map <uint16,int> mapAudit = CRouterProtocol::GetInstance()->GetAuditMap();
			map <uint16,int>::iterator it;
			string strInfo;
			for(it = mapAudit.begin();it!=mapAudit.end();it++)
			{
				sprintf_s(szInfo,"Э��:0x%.4x -- %d\r\n",it->first,it->second);
				strInfo +=szInfo;
			}
			if( strInfo.length() == 0)
			{
				strInfo = "��û�д����κ�ҵ��";
			}
			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),strInfo.c_str());	
		}
		break;
	case IDC_BUTTON_RESET_PACKETNUM:
		{
			CAppManager& AppManager = CAppManager::GetInstance();
			AppManager.m_AppNetwork.GetSrvNetTrans()->ResetPacketNum();
		}
		//����û��break,�����
	case IDC_BUTTON_SEND_STATUS:
		{
			int nSendNum,nFailedNum,nRecvNum;
			float fFailedRate;
			CAppManager& AppManager = CAppManager::GetInstance();
			AppManager.m_AppNetwork.GetSrvNetTrans()->GetSendPacketNum(nSendNum,nFailedNum,nRecvNum);
			if( nSendNum == 0 )
			{
				fFailedRate = 0 ;
			}
			else
			{
				fFailedRate = (float)nFailedNum/(float)nSendNum*(float)100.0;
			}
			sprintf_s(szInfo,"���հ�:%d\r\n�������Ͱ���%d,��������ʧ�ܰ�:%d,������:%.4f%%",
						nRecvNum,nSendNum,nFailedNum, fFailedRate );

			::SetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo);
		}
		break;
	case IDC_BUTTON_COMMOND:
		{
			::GetWindowText(::GetDlgItem(hDlg,IDC_EDIT_OUTPUT),szInfo,sizeof(szInfo));
			char strID[32] = {0}, strCmd[128] = {0};
			char strTime[32] = {0};
			int len = (int)strlen(szInfo);
			char* p = szInfo;
			const char* pbegin = szInfo;
			while(*p)
			{
				if (*p == ' ')
				{	
					*p = 0;
					strcpy_s(strID, pbegin);
					p++;
					pbegin = p;
					
					break;
				}	
				p++;
			}

			while(*p)
			{
				if (*p == ' ')
				{
					*p = 0;
					strcpy_s(strTime, pbegin);
					p++;
					pbegin = p;
					break;					
				}
				p++;
			}

			strcpy_s(strCmd, pbegin);
		
// 			strncpy_s(strID, szInfo,strlen(szInfo) - strlen(strstr(szInfo," ")) + 1);
// 			strcpy_s(strCmd,strstr(szInfo, " ") + 1);
// 			strcpy_s(strTime, strCmd, strlen(strCmd) - strlen(strstr(strCmd," ")) + 1);

			sprintf_s(szInfo,"ȷ��Ҫ��������:%s %s�Σ����û�:%s��",strCmd, strTime,strID );
			if( IDOK != ::MessageBoxA(hDlg,szInfo,"IM��̨",MB_OKCANCEL))
			{
				break;
			}

			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;

			LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= 0xebfa;


			char* pMsgData = sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT);
			CSendPacket SendPacket;
			SendPacket.SetData(pMsgData,MAX_PROTOLPACKET_LEN - sizeof(XT_SERVER_TRANSMIT) );


			SendPacket		<< (uint32)atoi(strID)
							<< (uint32)atoi(strTime)
							<< strCmd;

			sendData.data_len = uint16(sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize());

			CUser *pUser = CAppManager::GetInstance().GetUserManager().Find(atoi(strID));
			if(pUser)
			{
				CAppManager::GetInstance().InitHead(
					lpSendHead,
					CMD_SERVERMSG_IND,
					(uint16)(sizeof(XT_SERVER_TRANSMIT) + SendPacket.GetSize()),
					pUser->m_nID);

				CAppManager::GetInstance().GetAppNetWork().SendDataTo(
					sendData.data,sendData.data_len,SEND_RETRY_TIME);
				
			}
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

void CAppManager::LogoutAllUser()
{
	//��ʼ�����ݿ�
	CConfigInfo		dbConfig;
	char miniuid[32],minipwd[32],minisid[32],webuid[32],webpwd[32],websid[32],familyuid[32],familypwd[32],familysid[32];
	dbConfig.GetDataBaseConfig(miniuid, minipwd, minisid,webuid,webpwd,websid,familyuid,familypwd,familysid);
	CDataBaseWeb	DatabaseWeb;
	DatabaseWeb.SetConnectDB(webuid,webpwd,websid);
	if( !DatabaseWeb.ConnectDB() )
	{
		CLog::Log("CAppManager::LogoutAllUser",CLog::TYPE_ERROR,"DatabaseWeb.ConnectDB() failed!");
		return;
	}

	CDataBaseMini DatabaseMini;
	DatabaseMini.SetConnectDB(miniuid, minipwd, minisid);
	if( !DatabaseMini.ConnectDB() )
	{
		CLog::Log("CAppManager::LogoutAllUser",CLog::TYPE_ERROR,"DatabaseMini.ConnectDB() failed!");
		return;
	}

	CUser *pUser = NULL;
	CUser::m_UserListLock.Lock();;
	for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it = CUser::m_userhash.begin();it != CUser::m_userhash.end();++it)
	{
		pUser = it->second;//CUser::m_pUserListHead;
		if(!pUser)
			continue;
		pUser->NotifyServerClose();
		FamilyList_Type::iterator itf = pUser->m_PermFamilyMap.begin();
		for(;itf != pUser->m_PermFamilyMap.end(); ++itf)
		{
			//����̶�Ⱥ��Ϣid
			DatabaseMini.Save_User_RecvFamilyMsgID(pUser->m_nID, (*itf).first, (*itf).second);
		}
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USEROPERATION,pUser->m_nOperationID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERRECVMSG,pUser->m_nUserMsgID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::SYSTEMPUB,pUser->m_nSysMsgID);
		//DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERPUB,pUser->m_nBroadcastID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::USERCOMMENT,pUser->m_nUsercommentID);
		DatabaseMini.SetWokuMsgRecvID(pUser->m_nID,ST_WOKU_MSG::LOGCOMMENT,pUser->m_nLogcommentID);

		//�������ݿ����߱�״̬
		DatabaseMini.Save_User_Logout(pUser->m_nID);
		
		//�����û�����ʱ��
		int money;
		time_t tm;
		time(&tm);
		DatabaseMini. Save_User_OnlineTime(pUser->m_nID, pUser->m_nLoginTime, (int)tm, money);
		if (money > 0  && pUser->m_nID != 0)
		{
			if (money > 5)
				money = 5;
			DatabaseWeb.Update_User_Money(pUser->m_nID, money);
// 			SendDataStruct sendData;
// 
// 			char packet[MAX_NETWORK_PACKET];
// 			char content[255];
// 			sprintf(content,"Ϊ�˹�����ʹ��Mini���Ѿ�����������%d���֣�",money*5);
// 			CLog::Log("Money", CLog::TYPE_IMPORTANT, "�û�:%d���ͻ���:%d", pUser->m_nID, money*5);
// 
// 			CSendPacket pSendPacket(packet);
// 			pSendPacket <<(uint32)0
// 				<<(uint16)XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE
// 				<<(uint32)tm
// 				<<(uint8)2
// 				<<content;
// 
// 			DatabaseMini.Save_User_Msg(pUser->m_nID, 0,0,0,(uint32)tm,XT_SERVER_TRANSMIT::SRVMSGTYPE::TYPE_SYS_MESSAGE,pSendPacket.GetData(),(uint16)pSendPacket.GetSize());
		}
		
		//pUser = it->second;//pUser->m_next;
	}
	CUser::m_UserListLock.Unlock();


	return;
}
void CAppManager::Exit()
{

//	UnregistConnectServer();
	LogoutAllUser();
	m_pHeartTickCheck->StopWork();
	m_pWebIMMsgCheck->StopWork();
	
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
	
	if( m_pConnBusiControl )
	{
		m_pConnBusiControl->StopWork();
	}

	SAFE_DELETE(m_pConnBusi);


#ifndef _DEBUG
/*
	if( m_hNameEvent )
	{
		CloseHandle(m_hNameEvent);
	}
	*/
#endif

}

void CAppManager::ResetProtectEvent()
{
	static DWORD nLastTime = 0;

	//�����ػ������ź�״̬��������ǰ��̨�����ǻ��,
	//������ź�״̬�رգ��ػ����̻������̨����
	DWORD nCurrentTime = timeGetTime();
	if( (nCurrentTime - nLastTime) > 30*1000)	//����ÿ�����������źţ�����һ��ʱ�������
	{
		nLastTime = nCurrentTime;
		/*
		if( m_hNameEvent )
		{
			::SetEvent(m_hNameEvent);
		}
		*/
	}
}
void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}

// ��jabberdIM�̷߳�������
bool CAppManager::SendToJabberdIM(BYTE data[], int nLength, uint32 fdCl /*=0*/)
{
	if(m_nFD == -1)
	{
		return false;
	}

	CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"send��length:%d", nLength+8);

	// ��ͷ����ʾ
	BYTE *pData = new BYTE[nLength+8];
	memcpy(pData, "XMPP", 4);
	memcpy(pData+4, (BYTE*)&fdCl, 4);
	memcpy(pData+8, data, nLength);
	int length = 0;
	length += send(m_nFD, pData, nLength+8, 0);
	while(length < nLength+8 || length == 0)
	{
		length += send(m_nFD, pData, nLength+8, 0);
		
		CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"more send %d length:%d",m_nFD,length);
	}
	CLog::Log("CAppManager::SendToJabberdIM",CLog::TYPE_IMPORTANT,"send %d length:%d",m_nFD,length);	

	return true;
}

