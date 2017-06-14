// DataCenter.cpp: implementation of the CIMProtocol class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "..\IMClient.h"
#include "improtocol.h"
#include "xtpacket.h"
#include "LocalData.h"
#include "P2PFile.h"
#include "..\tone.h"
#include "../../../../Common/Common.h"
#include "..\improtocol.pb.h"
#include "ServerListFile.h"
//#include "../../../../include/zmq.h"
//#pragma comment(lib, "libzmq.lib")  
/*************************************************************
/* ���㳬ʱʱ���python�������� 
/*************************************************************
def CountTime(timespan,times):
	ran = range(1,times)
	newtime = timespan
	rst = 0
	for i in ran :
		newtime = newtime*1.4
		rst = rst + newtime
		print "���Դ���",i+1,":\t�¼��ʱ�� = ",newtime,";\t�ܺ�ʱ = ",rst
	return rst

****************************************************************/



#define IDS_ERROR_DECODE			"���ܴ���"
// ack ��ʱ�ط�����
#define RETRY_COUNT     2  


//                            () ()
//                            (-o-) 

// �Ƿ��ӡ�������ݵ���ѡ��
#define _TRACE_NETWORK

CIMProtocol * CIMProtocol::pInstance = NULL;
CIMProtocol * g_pProtocol = NULL;
DWORD CIMProtocol::s_nHeartBeat = GetTickCount();
int CIMProtocol::s_nCurVer = 0;


// ��ȡ����˿ڣ���Χ[1024,4999]
static DWORD GetRandomPort()
{
	srand(GetCpuCycle());
	DWORD nPort = 1024 + rand()%(4999-1024+1);
	return nPort;
}


#ifdef _TRACE_NETWORK
class IM_CMD_DIC: public map<uint16,const char *>
{
public:
	IM_CMD_DIC()
	{
		(*this)[0x201] = "ע��";
		(*this)[0x202] = "-ע��Ӧ��-";
		(*this)[0x203] = "�ǳ�";
		(*this)[0x211] = "��¼";
		(*this)[0x212] = "-��¼Ӧ��-";
		(*this)[0x301] = "����";
		(*this)[0x302] = "-����Ӧ��-";
		(*this)[0x303] = "�޸�״̬";
		(*this)[0x304] = "-�޸�״̬Ӧ��-";
		(*this)[0x413] = "ѯ�ʺ����Ƿ�Ը�����Ⱥ";
		(*this)[0x414] = "-ѯ�ʺ����Ƿ�Ը�����ȺӦ��-";
		(*this)[0x415] = "��ѯ�û���ϸ����";
		(*this)[0x416] = "-�û���ϸ����Ӧ��-";
		(*this)[0x417] = "�޸��û���ϸ����";
		(*this)[0x418] = "-�޸���ϸ����Ӧ��-";
		(*this)[0x501] = "�༭�û���";
		(*this)[0x502] = "-�༭�û���Ӧ��-";
		(*this)[0x503] = "��ȡ�û�������Ϣ";
		(*this)[0x504] = "-�û�������ϢӦ��-";
		(*this)[0x601] = "��ȡ�����б�";
		(*this)[0x602] = "-��ȡ�����б�Ӧ��-";
		(*this)[0x603] = "�����û�";
		(*this)[0x604] = "-�����û�Ӧ��-";
		(*this)[0x605] = "��ӡ����ĺ��ѵ�ĳ��";
		(*this)[0x606] = "-��ӡ����ĺ��ѵ�ĳ��Ӧ��-";
		(*this)[0x607] = "������֤��Ϣ";
		(*this)[0x608] = "-������֤��ϢӦ��-";
		(*this)[0x609] = "p2p���Ӳ���";
		(*this)[0x60a] = "-p2p����Ӧ��-";
		(*this)[0x611] = "��ȡ��������";
		(*this)[0x612] = "-��ȡ��������Ӧ��-";
		(*this)[0x60b] = "������Ϣ";
		(*this)[0x60c] = "-������ϢӦ��-";
		(*this)[0x60d] = "������ת����Ϣ";
		(*this)[0x60e] = "-������ת����ϢӦ��-";
		(*this)[0x701] = "�û�״̬�ı�֪ͨ";
		(*this)[0x703] = "����������Ϣ";
		(*this)[0x704] = "-����������ϢӦ��-";
		(*this)[0x705] = "��ȡ����״̬";
		(*this)[0x706] = "-��ȡ����״̬Ӧ��-";
		(*this)[0x707] = "�޸ĺ��ѱ�ע";
		(*this)[0x708] = "-�޸ĺ���Ӧ��-";
		(*this)[0x801] = "���ͷ������ͱ���";
		(*this)[0x802] = "-���ܷ��Է��ͱ����Ӧ��-";
		(*this)[0x901] = "������Ⱥ";
		(*this)[0x902] = "-������ȺӦ��-";
		(*this)[0x903] = "�༭Ⱥ��Ա";
		(*this)[0x904] = "-�༭Ⱥ��ԱӦ��-";
		(*this)[0x905] = "����Ⱥ��Ϣ";
		(*this)[0x906] = "-����Ⱥ��ϢӦ��-";
		(*this)[0x907] = "��ȡȺ��Ϣ";
		(*this)[0x908] = "-��ȡȺ��ϢӦ��-";
		(*this)[0x909] = "����Ⱥ";
		(*this)[0x90a] = "-����ȺӦ��-";
		(*this)[0x90b] = "�����˳�Ⱥ";
		(*this)[0x90c] = "-�����˳�ȺӦ��-";
		(*this)[0x90d] = "Ⱥ��֤";
		(*this)[0x90e] = "-Ⱥ��֤Ӧ��-";
		(*this)[0x911] = "��Ⱥ������Ϣ";
		(*this)[0x912] = "-��Ⱥ������ϢӦ��-";
		(*this)[0x913] = "��ȡȺ��Ա�б�";
		(*this)[0x914] = "-��ȡȺ��Ա�б�Ӧ��-";
		(*this)[0x915] = "��ɢȺ";
		(*this)[0x916] = "-��ɢȺӦ��-";
		(*this)[0x91d] = "��ȡȺ�б�";
		(*this)[0x91e] = "-��ȡȺ�б�Ӧ��-";
		(*this)[0x920] = "Ⱥ��Ա��������֪ͨ";
		(*this)[0x931] = "�ϴ�ȺͼƬ";
		(*this)[0x932] = "-�ϴ�ȺͼƬӦ��-";
		(*this)[0x933] = "����ȺͼƬ";
		(*this)[0x934] = "-����ȺͼƬӦ��-";	
		(*this)[0xc01] = "�������ļ�";
		(*this)[0xc02] = "-�������ļ���Ӧ��-";
		(*this)[0xc03] = "�����ļ�����";
		(*this)[0xc04] = "-�����ļ�����Ӧ��-";
		(*this)[0xc05] = "p2p������";
		(*this)[0xc06] = "-p2p������-";
		(*this)[0xc09] = "֪ͨ�������";
		(*this)[0xc0a] = "-֪ͨ�������Ӧ��-";
		(*this)[0xc0b] = "��ֹ����";
		(*this)[0xc0c] = "-��ֹ����Ӧ��-";
		(*this)[0x8009] = "��������ת";
		(*this)[0x800a] = "-��������תӦ��-";
		(*this)[0x800b] = "���ܷ�������ת";
		(*this)[0x800c] = "-���ܷ�������תӦ��-";
		(*this)[0x8101] = "�����ߵ�¼";
		(*this)[0x8102] = "-�����ߵ�¼Ӧ��-";
		(*this)[0x8107] = "�����ߵ�¼";
		(*this)[0x8108] = "-�����ߵ�¼Ӧ��-";
		(*this)[0x8111] = "�����߹رջỰ";
		(*this)[0x8112] = "-�����߹رջỰӦ��-";
		/*		
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		(*this)[0x] = "";
		*/
	}
};
IM_CMD_DIC  DIC_CMD;
#endif

DWORD IMCallBack(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData)
{

	CIMProtocol* pProtocol = (CIMProtocol*)pUserKey;
	ASSERT(pProtocol);
	if(pProtocol == NULL)
		return -1;
#ifdef _TRACE_NETWORK
	in_addr  in;
	int port;
	SYSTEMTIME tm;
	GetLocalTime(&tm);
	CString sTime;
	int nSpace;
	sTime.Format("%d:%d:%d",tm.wHour,tm.wMinute,tm.wSecond);
	nSpace = 8 - sTime.GetLength(); 
	for(int i = 0; i < nSpace;i++)
	{
		sTime += " ";
	}
#endif
	switch(nEvent)
	{
	case NE_RECVDATA:
		{
			int nLen = pRecvData->data_len;
#ifdef _TRACE_NETWORK
			XT_HEAD* pHead =(XT_HEAD*) pRecvData->data;
			in.s_addr = pRecvData->src_ip;
			port = pRecvData->port;
			if(pHead->cmd == 0x302)
			{
				TRACE("%s -> �� ������\n",sTime);
			}
			else
				TRACE("%s -> �� cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,nLen,pRecvData->vlink
					,inet_ntoa(in),port);
#endif
			pProtocol->IMProtocolProc(pProtocol,pRecvData);
			break;
		}
	case NE_RESEND:
		{

			SendDataStruct * pSendData = (SendDataStruct*)pRecvData;
			pProtocol->IMProtocolResend(pProtocol,pSendData);
#ifdef _TRACE_NETWORK
			XT_HEAD* pHead = (XT_HEAD *)pSendData->data;
			in.s_addr = pSendData->dest_ip;
			port = pSendData->port;
			if(pHead->cmd == 0x301)
			{
				TRACE("%s <= �ط� ������\n",sTime);
			}
			else
			{
				TRACE("%s <= �ط� cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
#endif
			break;	
		}
	case NE_SENDFAILED:
		{
			// ��ʱ
			XT_HEAD head;
			XT_HEAD * pHead;
			SendDataStruct * pSendData = (SendDataStruct*)pRecvData;
			CRecvPacket pkt((char *)pSendData->data,pSendData->data_len);
			pkt.GetXtHead(head);
			int nErr = WSAGetLastError();
			WPARAM wParam ;
			WORD err;
			if(nErr == WSAEHOSTUNREACH )
			{
				// �����������
				wParam = MAKEWPARAM(NETWORK_DISCONNECT,CMD_NETWORK_ERR);
				err = NETWORK_DISCONNECT;
			}
			else if(nErr == WSAEADDRNOTAVAIL)
			{
				wParam = MAKEWPARAM(IP_CANT_CONNECT,CMD_NETWORK_ERR);
				err = IP_CANT_CONNECT;
			}
			else
			{
				wParam = MAKEWPARAM(SERVER_TIMEOUT,head.cmd+1);
				err = SERVER_TIMEOUT;
			}
			pHead = (XT_HEAD *)pSendData->data;
			// �������ݳ���
			pHead->len = pSendData->data_len;
#ifdef _TRACE_NETWORK
			in.s_addr = pSendData->dest_ip;
			port = pSendData->port;
			if(pHead->cmd == 0x301)
			{
				TRACE("%s !! ʧ�� ������\n",sTime);
			}
			else
			{
				TRACE("%s !! ʧ�� cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
#endif
			strcpy(pProtocol->m_szError,"����ʱ");
			if(head.cmd == CMD_ROUTERPACKET_REQ)
			{
				pProtocol->DecryptFailedServerTran(err,*pSendData);
			}
			else if(head.cmd == CMD_ROUTERUSERPACKET_REQ)
			{
				pProtocol->ParseFailedServerTran(err,*pSendData);
			}
			else
			{
				pProtocol->SendNotify(wParam,(LPARAM)pSendData);
			}
			break;
		}
#ifdef _TRACE_NETWORK
	case NE_LISTENSTOP:
		{
			TRACE1("%s !!!!LISTEN STOP!!!! \n",sTime);
			break;
		}
	case NE_SENDOK:
		{
			SendDataStruct * pSendData = (SendDataStruct*)pRecvData;
			XT_HEAD* pHead = (XT_HEAD *)pSendData->data;
			in.s_addr = pSendData->dest_ip;
			port = pSendData->port;
			if(pHead->cmd == 0x301)
			{
				TRACE("%s <- �� ������\n",sTime);
			}
			else
			{
				TRACE("%s <- �� cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
			break;
		}
	case 1001:
		TRACE("p2p �յ������ߵ�����\n");
		break;
	case 1002:
		TRACE("p2p �յ�����������\n");
		break;
	case 2001:
		TRACE("p2p �յ�������ͬ��\n");
		break;
	case 2002:
		TRACE("p2p �յ�����������Ӧ��\n");
		break;
#endif
	}

	return 0;
}



// ������ʹ�øú���,������ȫ�ֱ���g_ImUserInfo
inline void InitHead(XT_HEAD & xt_head, uint16 cmd, uint32 dest_id, uint8 dtype, uint32 seq)
{
	if(g_ImUserInfo.GetId() == dest_id)
	{
		dest_id = 0;
	}
	xt_head.flag = 0xf2;		// �ͻ�������
	xt_head.cmd  = cmd;
	xt_head.from = CIMProtocol::s_nCurVer;	// �ͻ��˰汾��־
	xt_head.seq  = seq;
	xt_head.sid  = g_ImUserInfo.GetId();
	xt_head.did   = dest_id;	// Ŀ��id
	xt_head.dtype = dtype;		// Ŀ������
#ifdef _IM_USE_MULTIDEVICE_
	xt_head.deviceId = g_LocalData.GetDeviceId();
#endif
}

inline void InitSendStruct(SendDataStruct &s, DWORD dest_ip , WORD port )
{
#ifdef _DEBUG
	s.retry_times = 4;
#else
	s.retry_times = 4;
#endif
	s.tick_count  = GetTickCount();
	s.dest_ip     = dest_ip;//ntohl(inet_addr(m_pDataCenter->GetServerIP()));
	s.port        = port;

	if (port==XTREAM_SERVER_PORT)
		s.time_interval = 3000;	// = 33s
	else
		s.time_interval = 3000;	
	s.vlink = 0;
}

inline void InitP2PSendData(SendDataStruct &s, DWORD dest_ip , WORD port)
{
#ifdef _DEBUG
	s.retry_times = 5;
#else
	s.retry_times = 5;
#endif
	s.tick_count  = GetTickCount();
	s.dest_ip     = dest_ip;
	s.port        = port;
	s.time_interval = 2000;	// = 33 s
}

void strxcpy(char * szDst,const char * szSrc,int x)
{
	char * p = szDst + x - 1;
	while (x && (*szDst++ = *szSrc++))
		x--;
	if (x)  
	{
		if(--x)
			*szDst = '\0';
	}
	else
		*p = '\0';
}


CIMProtocol::CIMProtocol()
{
	s_nCurVer = g_LocalData.GetCurVersion();
	time_t seqtime;
	m_wSequenceID        = (WORD)time(&seqtime);
	m_dwIMServerIP       = 0;
	m_wIMServerPort		 = XTREAM_SERVER_PORT;	
	m_bRunning = false;
	m_szError[0] = 0;
	srand(time(0));
	//�����漴��Կ
	for ( int n=0; n<16; n++ )
	{
		m_RandomKey[n]=rand()%256;
		m_SessionKey[n]=0x01;
	}
	// �������
	m_wSequenceID = ((rand()<<15)|(rand()<<2))|(rand()>>13);
	m_wLastAckSequenceID = 0;
	s_nHeartBeat = GetTickCount();

	m_pUdpComm = factoryCreateNet();
	m_pUdpComm->SetUserCallback(this,IMCallBack);
	m_pUdpComm_FileTran = factoryCreateNet(0, 1, 50, MAX_FILE_TRAN_SEND_BUF+200);
	m_pUdpComm_FileTran->SetUserCallback(this,IMCallBack);
	// Ⱥ��Ϣ�ظ����˻�������
	//m_filterMsg.SetMax(300);
	
	//m_pUdpComm->SetRecvList(&m_RecvList);

	m_bLockUserPay = false;
}

CIMProtocol::~CIMProtocol()
{
	CMD_WND_MAP::iterator it;
	for ( it=m_mapCmdWnd.begin(); it!=m_mapCmdWnd.end(); it++ )
	{
		it->second->vecHWnd.clear();
		delete it->second;
	}
	m_mapCmdWnd.clear();

	for ( it=m_mapCmdResendWnd.begin(); it != m_mapCmdResendWnd.end(); it++ )
	{
		it->second->vecHWnd.clear();
		delete it->second;
	}
	m_mapCmdResendWnd.clear();

	m_bLockUserPay = false;
	delete m_pUdpComm;
	m_pUdpComm=NULL;
	delete m_pUdpComm_FileTran;
	m_pUdpComm_FileTran = NULL;
}

CIMProtocol * CIMProtocol::GetInstance()
{
	if (pInstance==NULL)
	{
		pInstance = new CIMProtocol();
	}

	return pInstance;
}

bool CIMProtocol::Start()
{
	m_bRunning = false;
	m_dicFileSeg.clear();
	memset(m_P2pKey,0,sizeof(m_P2pKey));
	
	m_pUdpComm->SetServerID(-1);
	m_pUdpComm_FileTran->SetServerID(-1);
	
	DWORD dwLocalIP  =  0;
	DWORD dwLocalPort = GetRandomPort();
	if(dwLocalIP == 0x7f000001)
		dwLocalIP = 0;
	in_addr in ;
	in.s_addr = htonl(dwLocalIP);
	string s = inet_ntoa(in);
	try
	{		
		int nTryTimes = 0;	// ���Դ���
		while(1)
		{
			if( m_pUdpComm->StartListen(s.c_str(),dwLocalPort) != 0)
			{
				dwLocalPort = GetRandomPort();
				nTryTimes++;
				if(nTryTimes == 500)
				{
					//UDP����ʧ��
					ShowMessage("�Բ��𣮣���������ʧ�ܣ��������������߷���ǽ���ã�", MB_OK|MB_ICONERROR);
					return false;
				}
			}
			else
			{
				TRACE1("��̨ͨѶ�˿ڣ�%d\n", dwLocalPort);
				break;
			}
		}
	}
	catch(...)
	{
		;
	}

	dwLocalPort = GetRandomPort();
	try
	{		
		int nTryTimes = 0;	// ���Դ���
		while(1)
		{
			if( m_pUdpComm_FileTran->StartListen(s.c_str(),dwLocalPort) != 0)
			{
				dwLocalPort = GetRandomPort();
				nTryTimes++;
				if(nTryTimes == 500)
				{
					//UDP����ʧ��
					ShowMessage("�Բ��𣮣���������ʧ�ܣ��������������߷���ǽ���ã�", MB_OK|MB_ICONERROR);
					return false;
				}
			}
			else
			{
				TRACE1("�ļ�����˿ڣ�%d\n", dwLocalPort);
				break;
			}

		}
	}
	catch(...)
	{
		;
	}

	m_bRunning=true;
	
	return true;
}

bool CIMProtocol::Stop()
{
	if ( m_bRunning )
	{
		m_bRunning = false;
		m_pUdpComm->StopListen();
		m_pUdpComm_FileTran->StopListen();
		Sleep(120);
		m_wLastAckSequenceID = 0;
	}

	m_bLockUserPay = false;

	return true;
}

//ע����������
void CIMProtocol::UnRegistCmd(WORD dwType,HWND hWnd)
{
	CMD_WND_MAP::iterator it;
	it=m_mapCmdWnd.find(dwType);

	if ( it!=m_mapCmdWnd.end() )
	{
		tagWndArry *pArry = it->second;
	
		for ( int i=0; i<pArry->vecHWnd.size(); i++)
		{
			if ( pArry->vecHWnd[i]==hWnd )
			{
				pArry->vecHWnd.erase(pArry->vecHWnd.begin()+i);
				return ;
			}
		}
	}	
}

void CIMProtocol::UnRegistCmd(HWND hWnd)
{
	CMD_WND_MAP::iterator it;

	for ( it=m_mapCmdWnd.begin(); it!=m_mapCmdWnd.end(); it++ )
	{
		tagWndArry *pArry = it->second;

		for ( int i=0; i<pArry->vecHWnd.size(); i++)
		{
			if ( pArry->vecHWnd[i]==hWnd )
			{
				pArry->vecHWnd.erase(pArry->vecHWnd.begin()+i);
				break;
			}
		}
	}

	for ( it = m_mapCmdResendWnd.begin(); it != m_mapCmdResendWnd.end(); it++ )
	{
		tagWndArry *pArry = it->second;

		for ( int i=0; i<pArry->vecHWnd.size(); i++)
		{
			if ( pArry->vecHWnd[i]==hWnd )
			{
				pArry->vecHWnd.erase(pArry->vecHWnd.begin()+i);
				break;
			}
		}
	}

	
}



//ע�ᴰ������
void CIMProtocol::RegistCmd(WORD dwType,HWND hWnd)
{
	CMD_WND_MAP::iterator it;
	it=m_mapCmdWnd.find(dwType);

	if ( it==m_mapCmdWnd.end() )
	{
		tagWndArry *pArry = new tagWndArry;
		pArry->vecHWnd.push_back(hWnd);

		//m_mapCmdWnd[dwType]=pArry;
		m_mapCmdWnd.insert(CMD_WND_MAP::value_type(dwType,pArry));
	}
	else
	{
		int count = it->second->vecHWnd.size();

		for ( int i=0; i<count; i++ )
		{
			if ( it->second->vecHWnd[i]==hWnd )
				return ;
		}

		it->second->vecHWnd.push_back(hWnd);
	}
}

//ע�ᴰ������
void CIMProtocol::RegistResendCmd(WORD dwType,HWND hWnd)
{
	CMD_WND_MAP::iterator it;
	it=m_mapCmdResendWnd.find(dwType);

	if ( it==m_mapCmdResendWnd.end() )
	{
		tagWndArry *pArry = new tagWndArry;
		pArry->vecHWnd.push_back(hWnd);
		m_mapCmdResendWnd.insert(CMD_WND_MAP::value_type(dwType,pArry));
	}
	else
	{
		int count = it->second->vecHWnd.size();

		for ( int i=0; i<count; i++ )
		{
			if ( it->second->vecHWnd[i]==hWnd )
				return ;
		}

		it->second->vecHWnd.push_back(hWnd);
	}
}

//������巢����Ϣ
// wParam : ���ֽ�Ϊ ������,���ֽ�Ϊ������
// lParm  : �����õı���
int CIMProtocol::SendNotify(WPARAM wParam,LPARAM lParam)
{
	WORD wCmd = HIWORD(wParam);

	CMD_WND_MAP::iterator it=m_mapCmdWnd.find(wCmd);

	if ( it!=m_mapCmdWnd.end() )
	{
		int count=it->second->vecHWnd.size();

		HWND * phWnd = new HWND[count];

		int i;
		for ( i=0; i<count; i++ )
		{
			phWnd[i]=it->second->vecHWnd[i];
			//::SendMessage(it->second->vecHWnd[i],WM_XT_CMD_NOTIFY,wParam,lParam);
		}

		for ( i=0; i<count; i++ )
		{
			::SendMessage(phWnd[i],WM_XT_CMD_NOTIFY,wParam,lParam);
		}


		delete [] phWnd;

		return 1;
	}
	else
	{
		return 0;
	}
}

//������巢����Ϣ
// wParam : ���ֽ�Ϊ ������,���ֽ�Ϊ������
// lParm  : �����õı���
int CIMProtocol::SendResendNotify(WPARAM wParam,LPARAM lParam)
{
	WORD wCmd = HIWORD(wParam);

	CMD_WND_MAP::iterator it=m_mapCmdWnd.find(wCmd);

	if ( it!=m_mapCmdWnd.end() )
	{
		int count=it->second->vecHWnd.size();

		HWND * phWnd = new HWND[count];

		int i;
		for ( i=0; i<count; i++ )
		{
			phWnd[i] = it->second->vecHWnd[i];
		}

		for ( i=0; i<count; i++ )
		{
			::SendMessage(phWnd[i],WM_XT_RESEND_NOTIFY,wParam,lParam);
		}

		delete [] phWnd;

		return 1;
	}
	else
	{
		return 0;
	}
}

UINT CIMProtocol::IMProtocolResend(LPVOID pParam,const SendDataStruct * pSendData)
{	
	CIMProtocol* pProtocol = (CIMProtocol*)pParam;
	LPXT_HEAD pHead = (LPXT_HEAD)pSendData->data;
	pProtocol->SendResendNotify(MAKEWPARAM(0,pHead->cmd),(LPARAM)pSendData);
	//pProtocol->SendResendNotify((LPARAM)pSendData);
	return 0;

}


UINT CIMProtocol::IMProtocolProc(LPVOID pParam,const RecvDataStruct * pRecvData)
{
	CIMProtocol* pProtocol = (CIMProtocol*)pParam;

	//��������
	//��Ϣ�ַ�

	LPXT_HEAD pHead = (LPXT_HEAD)pRecvData->data;
	

	try
	{
		// C/SЭ��
		
		((RecvDataStruct *)pRecvData)->src_type = P2P_ENDPOINT::BUSI;

		switch (pHead->cmd)
		{
		case CMD_CLUSTER_AUTH_ACK:
			pProtocol->OnClusterAuthAck(*pRecvData);
			break;
		case CMD_CLUSTER_SEARCH_ACK:
			pProtocol->OnClusterSearchAck(*pRecvData);
			break;
		case CMD_SET_CINFO_ACK:
			pProtocol->OnSetCInfoAck(*pRecvData);
			break;
		case CMD_CDISMISS_ACK:
			pProtocol->OnCDismissAck(*pRecvData);
			break;
		case CMD_GET_CINFO_ACK:
			pProtocol->OnGetCInfoAck(*pRecvData);
			break;
		case CMD_GET_CLIST_ACK:
			pProtocol->OnGetCListAck(*pRecvData);
			break;
		case CMD_GET_CMEMBERS_ACK:
			pProtocol->OnCMembersGetAck(*pRecvData);
			break;

		case CMD_GET_PROFILE_NAME_ACK:///�����������ȡȺ��Ƭ��Ϣ
			pProtocol->OnGetProfileNameAck(*pRecvData);
			break;
			
		case CMD_CLUSTERMSG_RECV:
			pProtocol->OnRecvClusterMsg(*pRecvData);
			break;
		case CMD_CLUSTERMSG_ACK:
			pProtocol->OnClusterMsgAck(*pRecvData);
			break;
		case CMD_CLUSTER_APPLY_ACK:
			pProtocol->OnClusterApplyAck(*pRecvData);
			break;
		case CMD_SET_CMEMBERS_ACK:
			pProtocol->OnCMembersSetAck(*pRecvData);
			break;
		case CMD_CREATE_CLUSTER_ACK:
			pProtocol->OnCreateClusterAck(*pRecvData);
			break;
		case CMD_STATUS_GET_ACK:
			pProtocol->OnStatusGetAck(*pRecvData);
			break;
		case CMD_USERAUTH_ACK:
			pProtocol->OnUserAuthAck(*pRecvData);
			break;
		case CMD_XTREAM_SEARCH_ACK:
			pProtocol->OnFriendSearchAck(*pRecvData);
			break;
		case CMD_OFFLINEMSG_ACK:
			pProtocol->OnRecvOfflineMsg(*pRecvData);
			break;
		case CMD_SERVERMSG_IND:
			pProtocol->OnRecvSvrMsg(*pRecvData);
			break;
		case CMD_FRIENDGROUP_MOD_ACK:
			pProtocol->OnFriendGroupModAck(*pRecvData);
			break;
		case CMD_GROUPINFO_MOD_ACK:
			pProtocol->OnGroupInfoModAck(*pRecvData);
			break;
		case CMD_GROUPINFO_GET_ACK:
			pProtocol->OnGroupInfoGetAck(*pRecvData);
			break;
		case CMD_P2PMSG_RECV:
			pProtocol->OnRecvP2PMsg(*pRecvData);
			break;
		case CMD_P2PMSG_ACK:
			pProtocol->OnRecvP2PMsgAck(*pRecvData);
			break;
		case CMD_USERINFO_MOD_ACK:
			pProtocol->OnUserInfoModAck(*pRecvData);
			break;
		case CMD_USERINFO_GET_ACK:
			pProtocol->OnUserInfoGetAck(*pRecvData);
			break;
		case CMD_STATUS_MOD_ACK:
			pProtocol->OnStatusModAck(*pRecvData);
			break;
		case CMD_FRIENDLIST_ACK:
			pProtocol->OnFriendListAck(*pRecvData);
			break;
		case CMD_DIRECTORY_ACK:
			pProtocol->OnDirectoryAck(*pRecvData);
			break;
		case CMD_LOGIN_ACK://�����¼Ӧ����
			pProtocol->OnLoginAck(*pRecvData);
			break;
		//case CMD_REGIST_ACK:
			//pProtocol->OnRegistAck(*pRecvData);
			//break;
		case CMD_WEB_IND:
			pProtocol->OnRecvWebInd(*pRecvData);
			break;
		case CMD_MOD_FCNAME_ACK:
			pProtocol->OnModFCNameAck(*pRecvData);
			break;
		case CMD_SERVER_STATUSLIST_REQ_ACK:
			pProtocol->OnGetServerListAck(*pRecvData);
			break;
		case CMD_SEND_CPIC_ACK:
			pProtocol->OnUploadCPicAck(*pRecvData);
			break;
		case CMD_GET_CPIC_ACK:
			pProtocol->OnDownloadCPicAck(*pRecvData);
			break;
		case CMD_USERFAMILYFLAG_GET_ACK:
			pProtocol->OnQueryFriendFamilyFlag(*pRecvData);
			break;
		case CMD_CLUSTER_INVITE_ACK:
			pProtocol->OnClusterInviteAck(*pRecvData);
			break;
		case CMD_CLUSTER_AGREE_ACK:
			pProtocol->OnClusterAgreeAck(*pRecvData);
			break;
		case CMD_FRIENDINVITE_ACK:
			pProtocol->OnFriendInviteAck(*pRecvData);
			break;
		case CMD_USERPAY_ACK:
			pProtocol->OnUserPaymentAck(*pRecvData);
			break;
		case CMD_SMS_ACK:
			pProtocol->OnRecvSMSAck(*pRecvData);
			break;
		case CMD_SMS_IND:
			pProtocol->OnRecvSMSInd(*pRecvData);
			break;
		case CMD_SET_PROFILE_NAME_ACK:///Ⱥ��Ƭ
			pProtocol->OnSetProfileNameAck(*pRecvData);
			break;
		//case CMD_SET_CLUSTERMSG_RECV_ACK://///Ⱥ��Ϣ���շ�ʽ
		//	pProtocol->OnClusterMsgRecSetAck(*pRecvData);
		//	break;

		case CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK:	// ���������ϵ�ͻ�Ӧ��
			pProtocol->OnRecvUpdateCommInfoAck(*pRecvData);
			break;
		case CMD_GET_NEAR_COMM_CUST_ACK:			// ��ȡ�̻������ϵ�Ŀͻ���Ϣ
			pProtocol->OnRecvGetNearCommCustAck(*pRecvData);
			break;
		
		case CMD_FILE_TRANS_REQ:    ////������������
			pProtocol->FileTransAck(pRecvData->data,pRecvData->data_len,*pRecvData);
			break; 

		case CMD_FILE_TRANS_ACK:    ////���ݰ����ͺ�ϵͳ���ص�ȷ����Ϣ
			break;

		case CMD_FILE_TRANS_IND:    ////������������
			pProtocol->FileTransInd(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;

		case CDM_HEAD_FILE_UPLOAD_ACK:  ////�ϴ�ͷ���ļ�Ӧ��
			pProtocol->OnHeadFileUploadAck(*pRecvData);
			break;
		case CMD_GET_USER_STATUS_ACK://��ȡ�û�״̬Ӧ��
			pProtocol->OnGetUserStatusAck(*pRecvData);
			break;
		case CMD_WEB_MORE_MSG_ACK://��ȡ��ʷ��ϢӦ��
			pProtocol->OnWebMoreMsgAck(*pRecvData);
			break;
			
#ifdef _CS_PLAT_EX_	// ��ţ�ͷ�����̨
		case CMD_MOD_PSW_ACK:///�����������ȡȺ��Ƭ��Ϣ
			pProtocol->OnModPswAck(*pRecvData);
			break;
		case CMD_RECEPTE_CUST_ACK:
			pProtocol->OnReceptCustAck(*pRecvData);
			break;

		//case CMD_GET_USERID_MAP_CSID_ACK  :///�����������ȡ�ͷ�CSID
		//	pProtocol->OnGetCSIDFromUserIDAck(*pRecvData);
		//	break;
#endif

#ifdef _VER_INTERNAL_	// �ڲ���
		case CMD_GET_EMPLOYEE_INFO_ACK:				// ��ȡԱ����Ϣ
			pProtocol->OnRecvGetEmployeeInfoAck(*pRecvData);
			break;
#endif

#ifdef _BUSINESS_VESION
		case CMD_UPDATE_NEAR_COMM_CUST_REMARK_ACK:
			pProtocol->OnRecvUpdateNearCommCustRemarkAck(*pRecvData);
			break;

		case CMD_GET_NEAR_COMM_CUST_REMARK_ACK:
			pProtocol->OnRecvGetNearCommCustRemarkAck(*pRecvData);
			break;
#endif
		case CMD_FILE_TRANS_IND_ACK:////�����ظ�ȷ��
			break; 

		case CMD_FILE_TRANS_NOTIFY_ACK:	// �ļ����俪ʼʱ�����͸������ͨ����Ϣ�󣬷���˷��ص�ȷ����Ϣ
			pProtocol->FileTranNotifyAck(*pRecvData);
			break;

	/*	case CMD_CUSTSER_TRAN_REQ_IND:		// ���շ����յ��ͷ�ת������
			pProtocol->RecvCustSerTranReqInd(*pRecvData);
			break;
		case CMD_CUSTSER_TRAN_REQ_IND:		// ���շ����յ��ͷ�ת������
			pProtocol->RecvCustSerTranReqInd(*pRecvData);
			break;*/
		case CMD_MONITOR_SET_BUYER_ACK:    // �ͷ����õ�ǰ��������Ӧ��
			pProtocol->OnMonitorSetBuyerAck(*pRecvData);
			break;
		case CMD_MONITOR_START_ACK:       //���ܻ�ȡ�ͷ��Ӵ������
			pProtocol->OnMonitorStartAck(*pRecvData);
			break;
		case CMD_MONITOR_END_ACK:      // ����ȡ����ؿͷ�Ӧ��
			pProtocol->OnMonitorEndAck(*pRecvData);
			break;
		case CMD_MONITOR_INSERT_CHAT_ACK: //���ܲ���ỰӦ��
			pProtocol->OnMonitorInsertChatAck(*pRecvData);
			break;
		case CMD_MONITOR_TRAN_CHAT_ACK:  //���ܽ��ֻỰӦ��
			pProtocol->OnMonitorTranChatAck(*pRecvData);
			break;
		case CMD_GETMSGREMIND_ACK://��ȡ��������ѡ��
			pProtocol->OnGetMsgRemindAck(*pRecvData);
			break;
		case CMD_SETMSGREMIND_ACK:
			pProtocol->OnSetMsgRemindAck(*pRecvData);
			break;
		case CMD_CS_TEAM_INFO_ACK:
			pProtocol->OnGetCsTeamInfo(*pRecvData);
			break;
		default:
			{
				goto PROTOCOL_P2P;
			}
			return 0;
		}

// P2PЭ��
PROTOCOL_P2P:	
		((RecvDataStruct *)pRecvData)->src_type = P2P_ENDPOINT::P2P;
	
		switch (pHead->cmd)
		{
		case CMD_P2P_TEST_REQ:
			pProtocol->OnRecvP2pTestPacket(*pRecvData);
			break;
		case CMD_P2P_TEST_ACK:
			pProtocol->OnP2pTestPacketAck(*pRecvData);
			break;
		case CMD_SEND_FILE_REQ:	
			pProtocol->RecvSendFileReq(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_DOWNLOAD_FILE_REQ:
			pProtocol->RecvDownloadFileReq(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_DOWNLOAD_FILE_ACK:
			pProtocol->RecvDownloadFileAck(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_DOWNLOAD_REQ:
			pProtocol->RecvDownloadReq(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_DOWNLOAD_ACK:
			pProtocol->RecvDownloadAck(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_FIN_TRAN_REQ:
			pProtocol->RecvFinishReq(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;
		case CMD_ABORT_TRAN_REQ:
			pProtocol->RecvAbortReq(pRecvData->data,pRecvData->data_len,*pRecvData);
			break; 

		default:
			{
				goto PROTOCOL_TRANS;
			}
			return 0;
		}

// ��תЭ��
PROTOCOL_TRANS:
		((RecvDataStruct*)pRecvData)->src_type = P2P_ENDPOINT::TRANS;
		switch(pHead->cmd)
		{
		case CMD_ROUTERUSERACKPACKET_REQ:
		case CMD_ROUTERUSERPACKET_ACK:
			pProtocol->RecvServerTran(*pRecvData);
			break;
		case CMD_ROUTERPACKET_REQ:
		case CMD_ROUTERPACKET_ACK:
			pProtocol->RecvServerEncryptTran(*pRecvData);
			break;
		case CMD_TRANS_LOGIN_ACK:
			pProtocol->RecvTransLoginAck(*pRecvData);
			break;
		case CMD_TRANS_RECVLOGIN_ACK:
			pProtocol->RecvTransRecvLoginAck(*pRecvData);
			break;
		case CMD_TRANS_SENDERCLOSE_ACK:
			pProtocol->RecvTransLogoutAck(*pRecvData);
			break;
		}
	}
	catch(...)
	{
	}

	return 0;
}

/*
UINT CIMProtocol::TimeoutCheckProc(LPVOID pParam)
{
CIMProtocol* pProtocol = (CIMProtocol*)pParam;

while (!pProtocol->m_bStop)
{
SendDataStruct sendData;

DWORD dwTickCount=GetTickCount();

bool ret = false;

pProtocol->m_SendList.Lock();

try
{
ret=pProtocol->m_SendList.Pop(sendData);
}
catch(...)
{
ret=false;
}

if ( ret )
{
if ( (dwTickCount-sendData.tick_count)>sendData.time_interval )
{
if (sendData.retry_times-->0)
{
// ��ʱ�ط�
sendData.tick_count = dwTickCount;
pProtocol->m_pUdpComm->SendDataTo(sendData);
pProtocol->m_SendList.Push(sendData);
//pProtocol->//Add2SendList(sendData);
}
else
{
// ��ʱ
XT_HEAD head;
CRecvPacket pkt((char *)sendData.data,sendData.data_len);
pkt.GetXtHead(head);

WPARAM wParam = MAKEWPARAM(SERVER_TIMEOUT,head.cmd+1);
pProtocol->SendNotify(wParam,(LPARAM)sendData.data);
}
}
else
{
pProtocol->m_SendList.Push(sendData);
}			
}//end if ret

pProtocol->m_SendList.Unlock();

Sleep(10);//idle
}

return 0;
}
*/
//void CIMProtocol::RemoveSendPacket(uint16 seq,SendDataStruct *pOut)
//{
//	FIFO2<SendDataStruct>::iterator it;
//	XT_HEAD head;
//
//	m_SendList.Lock();
//
//	for ( it=m_SendList.begin(); it!=m_SendList.end(); it++ )
//	{
//		CRecvPacket paket((char *)(*it).data,(*it).data_len);
//		paket.GetXtHead(head);
//
//		if ( head.seq==seq )
//		{
//			if ( pOut )
//				memcpy(pOut,&(*it),sizeof(SendDataStruct));
//
//			m_SendList.erase(it);
//			break;
//		}
//	}
//
//	m_SendList.Unlock();
//}
//
//void CIMProtocol::ClearSendList()
//{
//	m_SendList.Lock();
//	m_SendList.Clear();
//	m_SendList.Unlock();
//}



bool CIMProtocol::IsValideServerPacket(const RecvDataStruct &recvData)
{
	CRecvPacket rp((char *)recvData.data,recvData.data_len);
	XT_HEAD hd;
	rp.GetXtHead(hd);

	if ( (hd.flag >= 0xf0 && hd.flag <= 0xf3) &&
		hd.from == SENDER_XTREAM_SERVER &&
		(recvData.port == m_wIMServerPort || recvData.port == m_wTranServerPort) )
	{
		return true;
	}

	//CString str;

	//str.Format("flag:%x==0x06\nhd.from:%x==0x0100\nfrom ip:%x==%x\nfrom port:%d==%d",
	//	recvData.src_ip,GetIMServerIP(),
	//	recvData.port,XTREAM_SERVER_PORT);

	////ShowMessage(str);
	//LOG(str);

	return false;
}
//
//void CIMProtocol::Add2SendList(const SendDataStruct &sendData)
//{
//	m_SendList.Lock();
//	m_SendList.Push(sendData);
//	m_SendList.Unlock();
//}

//price:ʵ�ʼ۸��100�������ǵ�ĿǰCSendPacket��֧�ָ������������uint32 ����ʾ
int CIMProtocol::UserPayment(uint32 price,uint32 id, uint32 type)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket(sendData.data);

	XT_HEAD headSend;
	InitHead(headSend,CMD_USERPAY_REQ,0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(headSend);

	char crypt[255];
	long crypt_len;

	CSendPacket cryptPacket(crypt);

	cryptPacket<<type<<id<<price;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len = sendPacket.GetSize();

	m_pUdpComm->SendDataTo(sendData);

	m_bLockUserPay = true;

	DWORD dwTick = GetTickCount();

	while(m_bLockUserPay)
	{
		Sleep(1);
		DWORD dwTick1 = GetTickCount();
		DWORD dwTimeSpan = dwTick1 - dwTick;

		if ( dwTimeSpan>20000 )
		{
			m_nPayResult = 1;
			strcpy(m_szError,"���֧����ʱ!");
			break;
		}
	}

	m_bLockUserPay = false;
	return m_nPayResult;
}

int CIMProtocol::SendReq(const XT_FRIENDINVITE_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket(sendData.data);

	XT_HEAD headSend;
	InitHead(headSend,CMD_FRIENDINVITE_REQ,0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(headSend);

	char crypt[1500];
	long crypt_len;

	CSendPacket cryptPacket(crypt);

	cryptPacket<<req.emailAddress;
	cryptPacket<<req.emailTitle;
	cryptPacket<<req.emailContent;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_FRIENDLIST_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket(sendData.data);

	

	XT_HEAD headSend;
	InitHead(headSend,CMD_FRIENDLIST_REQ,0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);

#ifdef _BUSINESS_VESION	// ���Ұ�Ļ�ȡ�����б�ʱ����Э��ͷ����Ӹ���־λ
	headSend.flag = XT_HEAD::FLAG_SELLCNTREQ;
#endif
	
	sendPacket.SetXtHead(headSend);

	char crypt[1500];
	long crypt_len;

	CSendPacket cryptPacket(crypt);
	cryptPacket<<req.merchant_id;	
	cryptPacket<<req.id;
	cryptPacket<<req.start_pos;
	cryptPacket<<req.nType;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_USERINFO_MOD_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_USERINFO_MOD_REQ,((XT_USER_INFO*)(&req))->GetId(),XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	sendPacket.SetXtHead(head);
	

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;


	CSendPacket cryptPacket(crypt);
	cryptPacket	<<req.email
				<<((XT_USER_INFO*)(&req))->GetPassword().c_str()
				<<req.nickname
				<<req.birthday
				<<req.sex
				<<req.province
				<<req.country
				<<req.city
				<<req.career
				<<req.rule
				<<req.familyflag
				<<req.linkpop
				<<req.address
				<<req.mobile
				<<req.description
				<<req.face_url;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len = sendPacket.GetSize();


	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_USERINFO_GET_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	//��ȡ�û���Ϣ��Ϊ��DEST_TYPE_SERVER���͵���Ϣ
	InitHead(head,CMD_USERINFO_GET_REQ,req.fid,XT_HEAD::DEST_TYPE_SERVER,m_wSequenceID++);
	head.sid = g_ImUserInfo.GetId();
	head.did = 0;
	sendPacket.SetXtHead(head);
	//sendPacket<<req.uid;
	char crypt[1500];
	long crypt_len;

	CSendPacket cryptPacket(crypt);
	cryptPacket<<req.fid;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len = sendPacket.GetSize();
	
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_STATUS_MOD_REQ& req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_STATUS_MOD_REQ,req.id,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	//sendPacket<<req.id;

	char crypt[1500];
	long crypt_len;

	CSendPacket sp2(crypt);
	sp2<<req.id;
	sp2<<req.status;

	TEAEncrypt(crypt,sp2.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
	////Add2SendList(sendData);

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_STATUS_GET_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_STATUS_GET_REQ,req.fid,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	

	char crypt[1500];
	long crypt_len;

	CSendPacket sp2(crypt);
	sp2<<req.id;
	sp2<<req.fid;

	TEAEncrypt(crypt,sp2.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendCPicData(const XT_CPIC_DATA & picData)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);
	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_SEND_CPIC_REQ,picData.cid,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	// �����ܣ�ֱ�ӷ���
	sendPacket	<<picData.cid
				<<picData.fid
				<<picData.packetId
				<<picData.dataLen
				;
	// ֱ��д��ͼƬ����
	sendPacket.WritePacket(picData.data,picData.dataLen);
	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendClusterMsg(const XT_MSG &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_CLUSTERMSG_SEND,req.cluster_id,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);

	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt);
	//ͨ����������ת
	cp	<<req.cluster_id
		<<req.from_id
		<<req.ver
		<<req.cluster_type
		<<req.data_type//��Ϣ����,0��ͨ��Ϣ
		<<req.send_time
		//<<req.ms
		<<req.face;

	if ( req.data_type==XT_MSG::IM_MSG )
	{
		cp<<req.fontSize<<req.fontColor<<req.fontStyle;
		cp<<req.fontName;
	}
	/*
	else if(req.data_type == 6)
	{
		// ������͵���ȺͼƬ������Ҫ��¼�����ݿ飬����֤�Ƿ�ɹ��ύ��������
		CRecvPacket dataInfo((char *)req.data,sizeof(req.data));
		IM_FILESEG_INFO info;
		info.msgType = req.cluster_type;
		dataInfo>>info.id
				>>info.size
				>>info.file_name
				>>info.last_packet_id
				>>info.last_file_pointer;
		m_dicFileSeg[head.seq] = info;
	}*/
	cp.WriteData(req.data,req.data_len);
	cp<<req.from_nickname;
	
	if(req.data_type == 6)
	{
		sendData.retry_times = 10;
	}

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendP2pTestPacket(const XT_P2PTEST_REQ &req)
{
	SendDataStruct sendData;
	// ������ip��port
	InitSendStruct(sendData,req.dest_ip,req.dest_port);
	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_P2P_TEST_REQ,req.toid,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	
	sendPacket<<req.ipType;
	sendPacket<<req.id;


	sendPacket.WritePacket((char *)m_P2pKey,16);
	
	sendData.data_len = sendPacket.GetSize();
	sendData.time_interval = 500;
	sendData.retry_times = 6;	// = 11.4s
	int i = 0;
	for(;i < 1;i++)
	{
		InitSendStruct(sendData,req.dest_ip,req.dest_port + i);
		m_pUdpComm->SendDataTo(sendData);
	}

	return 0;
}


int CIMProtocol::SendP2PMsg(const XT_MSG &req, int type, int nSeqId, int nInsertBuyerId)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_P2PMSG_SEND,req.to_id,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;
	uint8 recv_flag = type;
	CSendPacket cp(crypt);
	if ( type != SMT_SERVER )//ֱ�ӷ��͵��Է�
	{
		cp	<<req.from_id
			<<req.to_id
			<<req.data_type
			<<req.ver
			<<req.send_time
			//<<req.ms
			<<recv_flag;
		if (req.data_type==XT_MSG::IM_MSG
			|| req.data_type==XT_MSG::EVALUA_ACK
			|| req.data_type==XT_MSG::CUST_SER_TRAN_REQ
			|| req.data_type==XT_MSG::AUTO_REPLY
			|| req.data_type==XT_MSG::IM_MSG_INSERT)
		{
			cp<<req.fontName<<req.fontStyle
				<<req.fontColor<<req.fontSize;
		}
		// д��������Ϣ����
		cp.WriteData(req.data,req.data_len);
		if ( req.data_type==XT_MSG::IM_MSG
			|| req.data_type==XT_MSG::EVALUA_ACK
			|| req.data_type==XT_MSG::CUST_SER_TRAN_REQ
			|| req.data_type==XT_MSG::AUTO_REPLY
			|| req.data_type==XT_MSG::IM_MSG_INSERT)
			cp<<req.from_nickname;
		// �öԷ���p2p�Ự��������
		FRIEND_INFO * pFriend = GetFriendInfo(req.to_id);
		if(pFriend == NULL)
			return 1;
		TEAEncrypt(crypt,cp.GetSize(),
			(char *)pFriend->key,
			crypt,&crypt_len);
		sendPacket.WritePacket(crypt,crypt_len);

		// �ô������֤�����ݰ��Ƿ�ʱ

		sendData.time_interval = 500;
		sendData.retry_times = 6;	// 12s 

		// �������ͼƬ�����������ط�����
		if( req.data_type != XT_MSG::IM_MSG )
		{
			sendData.retry_times = 10; // 50s
		}

		// ʹ��p2pͨѶ��ip�Ͷ˿�
		if( type == SMT_P2P)
		{
			sendData.dest_ip  = pFriend->ip;
			sendData.port     = pFriend->port;
		}
		// ʹ������ip
		else 
		{
			sendData.dest_ip = pFriend->localIP;
			sendData.port = pFriend->localPort;
		}
	}
	else
	{
		//ͨ����������ת
		cp  <<req.from_id
			<<req.to_id
			<<req.data_type
			<<req.ver
			<<req.send_time
			//<<req.ms
			<<recv_flag;
		if (req.data_type==XT_MSG::IM_MSG
			|| req.data_type==XT_MSG::EVALUA_ACK
			|| req.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG
			|| req.data_type==XT_MSG::CUST_SER_TRAN_REQ
			|| req.data_type==XT_MSG::AUTO_REPLY
			|| req.data_type==XT_MSG::IM_MSG_INSERT)
		{
			cp<<req.fontSize<<req.fontColor<<req.fontStyle;
			cp<<req.fontName;
		}
		// д��������Ϣ����
		cp.WriteData(req.data,req.data_len);
		if ( req.data_type==XT_MSG::IM_MSG
			|| req.data_type==XT_MSG::EVALUA_ACK
			|| req.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG
			|| req.data_type==XT_MSG::CUST_SER_TRAN_REQ
			|| req.data_type==XT_MSG::AUTO_REPLY
			|| req.data_type==XT_MSG::IM_MSG_INSERT)
			cp<<req.from_nickname;

		
#ifdef _MOBILE_TEST_
		char buffer[64] = { 0 };
		GUID guid;

		if ( S_OK == CoCreateGuid(&guid) )
		{
			_snprintf(buffer, sizeof(buffer),
				"%08X-%04X-%04x-%02X%02X-%02X%02X%02X%02X%02X%02X",
				guid.Data1, guid.Data2, guid.Data3,
				guid.Data4[0], guid.Data4[1], guid.Data4[2],
				guid.Data4[3], guid.Data4[4], guid.Data4[5],
				guid.Data4[6], guid.Data4[7]);
		}
		cp<<buffer;
#endif
		if(req.data_type==XT_MSG::IM_MSG_INSERT)
		{
			uint32 nId = nInsertBuyerId;
			cp<<nId;
		}
		// ����
		TEAEncrypt(crypt,cp.GetSize(),
			(char *)m_SessionKey,
			crypt,&crypt_len);
		sendPacket.WritePacket(crypt,crypt_len);
	}

	sendData.data_len = sendPacket.GetSize();

	if(req.data_type == XT_MSG::IM_MSG)
	{
		SendMsgQueue sendMsgQueueInfo;
		sendMsgQueueInfo.seqId = head.seq;
		sendMsgQueueInfo.msg = req;
		sendMsgQueueInfo.sendTick = GetTickCount();
		g_LocalData.AddSendMsgQueue(sendMsgQueueInfo, nSeqId, head.seq);
	}

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_LOGOUT &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);
	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	if(req.id == 0)
		return 1;
	// ע�⣡
	uint32 oldId = g_ImUserInfo.GetId();
	g_ImUserInfo.SetId(req.id);
	InitHead(head,CMD_LOGOUT,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	g_ImUserInfo.SetId(oldId);
	sendPacket.SetXtHead(head);
	
	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp<<req.id;
	cp<<req.authData;

	TEAEncrypt((char *)crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);
	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
#ifndef _DEBUG
	sendData.retry_times = 0;
#endif
	
	//this->SendNotify(MAKEWPARAM(0,CMD_LOGOUT),0);

	return  m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_OFFLINEMSG_REQ & req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_OFFLINEMSG_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp<<req.id;
	//cp<<req.start_pos<<req.type<<req.count;

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);
	return m_pUdpComm->SendDataTo(sendData); 
}

int CIMProtocol::SendReq(const XT_GROUPINFO_MOD_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GROUPINFO_MOD_REQ,req.uid,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp<<req.uid;
	cp<<req.action<<req.group_pid<<req.group_id<<req.name;

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);
	return m_pUdpComm->SendDataTo(sendData);  
}

int CIMProtocol::SendReq(const XT_FRIENDGROUP_MOD_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_FRIENDGROUP_MOD_REQ,req.uid,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp<<req.uid;
	cp<<req.action<<req.group_id<<req.fid;

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();

	////Add2SendList(sendData);
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_GROUPINFO_GET_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GROUPINFO_GET_REQ,req.id,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

#ifdef _BUSINESS_VESION	// ���Ұ�Ļ�ȡ���б�ʱ����Э��ͷ����Ӹ���־λ
	head.flag = XT_HEAD::FLAG_SELLCNTREQ;
#endif

	sendPacket.SetXtHead(head);
	

	char crypt[1500];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.id;
	sp<<req.group_count;
	sp<<req.nType;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	////Add2SendList(sendData);

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_USERAUTH_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_USERAUTH_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	
	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp<<req.action<<req.from_id<<req.to_id<<req.groupId<<req.send_time<<req.info;

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();

	//Add2SendList(sendData);
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_XTREAM_SEARCH_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_XTREAM_SEARCH_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS]; 
	CSendPacket cp(crypt);

	/*cp<<req.startpos<<req.search_type<<req.count<<req.id<<req.user_name
		<<req.nick_name<<req.user_email;*/
	cp<<req.startpos<<req.search_type<<req.count<<req.id<<req.user_name
		<<req.nick_name<<req.user_email;

	long crypt_len;

	TEAEncrypt(crypt,cp.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	if(req.search_type == XTREAM_SEARCH_PRECISION)
	{
		sendData.time_interval = 6000;
		sendData.retry_times = 4;
	}
	sendData.data_len      = sendPacket.GetSize();

	//Add2SendList(sendData);
	int iRet = m_pUdpComm->SendDataTo(sendData);

	this->SendNotify(MAKEWPARAM(0,CMD_XTREAM_SEARCH_REQ),0);

	return iRet;
}


int CIMProtocol::SendReq(const XT_CLUSTER_SEARCH_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_CLUSTER_SEARCH_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len ;

	CSendPacket sp(crypt);

	sp<<req.start_pos<<req.search_type<<req.nCount;

	switch(req.search_type)
	{
	case CLUSTER_SEARCH_ID:
		sp<<req.type_data.nFamilyID;
		break;
	case CLUSTER_SEARCH_NAME:
		sp<<req.type_data.szFamilyName;
		break;
	default:
		sp<<req.type_data.nFamilyClass;
		break;
	}

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();

	//Add2SendList(sendData);

	this->SendNotify(MAKEWPARAM(0,head.cmd),0);

	return m_pUdpComm->SendDataTo(sendData);
}
/*
int CIMProtocol::QueryFriendFamilyFlag(const XT_USERFAMILYFLAG_GET_REQ & req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;

	InitHead(head,CMD_USERFAMILYFLAG_GET_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);


	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt);

	cp<<req.count;

	for ( int i=0; i<req.count; i++ )
	{
		cp<<req.userID[i];
	}

	TEAEncrypt(crypt,cp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len); 

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}
*/
//���������Ⱥ
int CIMProtocol::ClusterInvite(const XT_CLUSTER_INVITE_REQ & req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;

	InitHead(head,CMD_CLUSTER_INVITE_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);


	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt);

	cp<<req.count;
	cp<<req.clustername;
	cp<<req.nickname;
	cp<<req.cluster_id;
	cp<<req.cluster_type;

	for ( int i=0; i<req.count; i++ )
	{
		cp<<req.userID[i];
	}
	
	TEAEncrypt(crypt,cp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len); 

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}

//�����Ƿ���Ⱥ
int CIMProtocol::ClusterAgree(const XT_CLUSTER_AGREE_REQ & req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;

	InitHead(head,CMD_CLUSTER_AGREE_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);


	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt);

	cp<<req.action;
	cp<<req.invite_id;
	cp<<req.friend_id;
	cp<<req.cluster_id;
	cp<<req.cluster_type;

	TEAEncrypt(crypt,cp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len); 

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}

int CIMProtocol::SendReq(const XT_CLUSTER_AUTH_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_CLUSTER_AUTH_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;


	CSendPacket sp(crypt);
	sp<<req.nFamilyID<<req.nFamilyProp<<req.nAction<<req.nRequestID<<req.nManagerID
		<<req.send_time<<req.info;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_DIRECTORY_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_DIRECTORY_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	

	char crypt[1500];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.id;
	sp<<req.local_ip<<req.local_port;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);
	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
	sendData.time_interval = 15000;
	sendData.retry_times = 1;
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_GET_CLIST_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GET_CLIST_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char buff[1500];
	long buff_len;

	CSendPacket sp(buff);
	sp<<req.nFamilyProp<<req.nCount;

	TEAEncrypt(buff,sp.GetSize(),(char *)m_SessionKey,
		buff,&buff_len);

	sendPacket.WritePacket(buff,buff_len);

	sendData.data_len  = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_CLUSTER_DISMISS_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_CDISMISS_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.nFamilyID<<req.nFamilyProp;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	return m_pUdpComm->SendDataTo(sendData);
}

//int CIMProtocol::SendReq(const XT_FACEURL_REQ &req)
//{
//	SendDataStruct sendData;
//	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);
//
//	CSendPacket sendPacket(sendData.data);
//	XT_HEAD head;
//	InitHead(head,CMD_FACEURL_REQ,m_wSequenceID++);
//	sendPacket.SetXtHead(head);
//
//	sendPacket<<req.uid;
//
//	char crypt[MAX_SEND_BUFFERS];
//	long crypt_len;
//
//	CSendPacket sp(crypt);
//	sp<<req.face_url;
//
//	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
//		crypt,&crypt_len);
//
//	sendPacket.WritePacket(crypt,crypt_len);
//
//	sendData.data_len  = sendPacket.GetSize();
//
//	//Add2SendList(sendData);
//
//	return m_pUdpComm->SendDataTo(sendData);
//}
//

int CIMProtocol::SendReq(const XT_USER_OPERATION_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,0x0A05,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	//sp<<req.uid;
	sp<<req.count;

	for ( int i=0; i<req.count; i++ )
	{
		sp<<req.oper_ids[i]<<req.oper_cnt[i];
	}

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_CLUSTER_APPLY_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_CLUSTER_APPLY_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.nFamilyID<<req.nFamilyProp<<req.nAction<<req.nUserID;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_COFFLINE_MSG_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_COFFLINE_MSG_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.uid;
	sp<<req.start_pos<<req.cluster_id<<(uint8)PERM_FAMILY<<(uint8)0
		<<req.count;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_SET_CINFO_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_SET_CINFO_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.nFamilyID<<req.nFamilyProp
		<<req.nJoinFlag<<req.nFamilyClass<<req.nFamilyFaceID
		<<req.szFamilyFaceUrl<<req.nViewFlag<<req.szFamilyName
		<<req.szFamilyDesc<<req.szFamilySign<<req.szFamilyNotice;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_SMS_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);

	XT_HEAD headSend;
	InitHead(headSend,CMD_SMS_REQ,0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(headSend);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.recv_id<<req.mobile<<req.link_id<<req.biz_type<<req.msg_type<<req.message;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_GET_CINFO_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GET_CINFO_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.nFamilyID<<req.nFamilyProp;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_SET_CMEMBERS_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_SET_CMEMBERS_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.nFamilyID<<req.nFamilyProp<<req.nAction<<req.nCount<<req.nInviteID;

	//����������˺ͱ������˽���λ�ã��ͺ�̨����һ��
	for ( int i=0; i<req.nCount; i++ )
	{
		sp<<req.nMemID[i];
	}

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	sendData.time_interval = 10000;


	return m_pUdpComm->SendDataTo(sendData);
}


int CIMProtocol::SendReq(const XT_GET_CMEMBERS_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GET_CMEMBERS_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.start_pos<<req.nFamilyID
		<<req.nFamilyProp<<req.online_type
		<<req.req_count<<req.userstatus;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	this->SendNotify(MAKEWPARAM(0,CMD_GET_CMEMBERS_REQ),(LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);
}

//////�����ȡȺ��Ƭ��Ϣ
int CIMProtocol::SendReq(const XT_PROFILENAME_GET_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_GET_PROFILE_NAME_REQ,req.nFamilyID,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);

	sp<<req.nFamilyID;///ȺID


	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	this->SendNotify(MAKEWPARAM(0,CMD_GET_PROFILE_NAME_REQ),(LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);
}


int CIMProtocol::SendReq(const XT_CREATE_CLUSTER_REQ &req, uint32 * pwSeqID)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;

	if ( pwSeqID )
	{
		*pwSeqID = m_wSequenceID;
	}

	InitHead(head,CMD_CREATE_CLUSTER_REQ,0,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);
	sendPacket.SetXtHead(head);
	
	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.nFamilyProp;

	if ( req.nFamilyProp==PERM_FAMILY )//�̶�Ⱥ
	{	
		sp<<req.nJoinFlag<<req.nFamilyClass<<req.nFamilyFaceID
		  <<req.szFamilyFaceUrl<<req.nViewFlag
		  <<req.szFamilyName
		  <<req.szFamilyDomain
		  <<req.szFamilyDesc;
	}

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_GET_FRIEND_BASICINFO_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;

	InitHead(head,CMD_GET_FRIEND_BASICINFO_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt);

	cp<<req.type;
	cp<<req.count;

	for ( int i=0; i<req.count; i++ )
	{
		cp<<req.FriendID[i];
	}

   	TEAEncrypt(crypt,cp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len); 

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

    return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const XT_LOGIN_REQ &req)
{

#ifdef _DEBUG
	in_addr in;
	in.s_addr = m_dwIMServerIP;
	TRACE("�û�%s��¼������%s\n",req.uLoginKey.username,inet_ntoa(in));
#endif
	unsigned char digest[16];

	char password[255] = {0};
	//sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",req.password);
	sprintf(password,"%s",req.password);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)password,strlen(password) );
	md5T.MD5Final (digest);

	CString sHex=ToHexString(digest,16);

	md5T.MD5Update ((unsigned char*)(LPCSTR)sHex, sHex.GetLength());
	md5T.MD5Final (digest);

	char *str="";
	char crypt[16]; //��һ�����ִ�,�õ�¼�����MD5(MD5(password))����tea����
	//��Ϊ��֤��,���͵�������
	long crypt_len;

	TEAEncrypt(str,1,(char *)digest,crypt,&crypt_len);

	//Build login package
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket((char *)sendData.data);

	XT_HEAD xtHead;
	InitHead(xtHead,CMD_LOGIN_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	sendPacket.SetXtHead(xtHead);

	if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYNAME)//�û�����¼
		sendPacket<<req.login_flag<<req.uLoginKey.username;
	else
		if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYMOBILENO)//�ֻ��ŵ�½
		sendPacket<<req.login_flag<<req.uLoginKey.username;
	else
		sendPacket<<req.login_flag<<req.uLoginKey.id;

#ifdef _CS_PLAT_EX_		// ����ǿͷ�����̨�汾����д��auto_session
	sendPacket<<req.auto_session;
#endif

	memcpy((char *)req.random_key,m_RandomKey,16);
	// 
	sendPacket.WritePacket((char *)m_RandomKey,sizeof(m_RandomKey));

	char cryptData[MAX_SEND_BUFFERS];
	CSendPacket cryptPacket(cryptData);
	// д��֤��
	//cryptPacket.WritePacket(crypt,16);
	cryptPacket.WritePacket(req.password,sizeof(req.password));		
	uint32 nLocalIp = GetLocalIpAddress();
	cryptPacket<<req.status<<nLocalIp<<(uint16)m_pUdpComm->GetPort();
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),
		(char *)m_RandomKey,
		cryptData,&crypt_len);
	sendPacket.WriteData(cryptData,crypt_len);	
	sendData.data_len      = sendPacket.GetSize();//27+dwCryptLen+1;

	sendData.retry_times = 3;		// 

#ifndef _LOGIN_TEST_
	//֪ͨ����,��¼
	SendNotify(MAKEWPARAM(0,CMD_LOGIN_REQ),req.status);
#endif
	
	return m_pUdpComm->SendDataTo(sendData);
}

/*User Name�û�ע����	Max 16 Bytes, Text�������ظ�������СдӢ�ĺ����֣���Ч����4-15�����磺myname_139��
	Client Email	Max 60 Bytes, Text 
	Password	Max 16 Bytes, Text ���벻��Ϊ�����룬�Ϸ�����ΪA-Z��a-z��0-9���»�����ɣ�����6-15
	�û��س�	Max 24 Bytes, Text 
	��������Birthday	8 Bytes��text
	Sex	1 Byte��0-Ů��1-�У�2-δ֪
	Province����	1 Byte*/
/*
int CIMProtocol::SendReq(const XT_REGIST_REQ &req)
{
	
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_REGIST_REQ,0,0,m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cryptPacket(crypt);
	

	
	cryptPacket <<req.regfrom
				<<req.username 
				<<req.sex
				<<req.nickname
				<<req.password 
				<<req.email 
				;

	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),
		(char *)m_RandomKey,
		crypt,&crypt_len);


	sendPacket.WritePacket((char *)m_RandomKey,16);
	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	sendData.time_interval = 15000;
	sendData.retry_times = 3;



	return m_pUdpComm->SendDataTo(sendData);
}
*/
// �޸ĸ���Ⱥ��Ƭ
int CIMProtocol::SendReq(const XT_PROFILENAME_SET_REQ& req)
{
	XT_HEAD xtHead;
	InitHead(xtHead, CMD_SET_PROFILE_NAME_REQ, req.nMemID, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);

	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	// ��д��ǰȺid
	cryptPacket << req.nFamilyID;
	// ��д����Ⱥ��Աid�ͳ�ԱȺ��Ƭ
	cryptPacket << req.nMemID << req.ProfileName;
	// ����
	TEAEncrypt(cryptPacket.GetData(), cryptPacket.GetSize(), (char*)m_SessionKey,
		cryptData, &cryptData_len);
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData, cryptData_len);
	// ���Ͱ��ĳ���
	sendData.data_len = sendPacket.GetSize();

	////this->SendNotify(MAKEWPARAM(0, CMD_GET_CLUSTER_PROFILE_ACK), (LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////����Ⱥ��Ϣ���շ�ʽ
//int CIMProtocol::SendReq(const XT_CLUSTERMSG_RECV_SET_REQ& req)
//{
//	XT_HEAD xtHead;
//	InitHead(xtHead, CMD_SET_CLUSTERMSG_RECV_REQ, req.nMemID, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
//
//	SendDataStruct sendData;
//	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
//
//	CSendPacket sendPacket((char *)sendData.data);
//	sendPacket.SetXtHead(xtHead);
//
//	char cryptData[MAX_SEND_BUFFERS];
//	long cryptData_len;
//	// ��Ҫ���ܵ�����
//	CSendPacket cryptPacket(cryptData);
//	// ��д��ǰȺid
//	cryptPacket << req.nFamilyID;
//	// ��д����Ⱥ��Աid��Ⱥ��Ϣ���շ�ʽ
//	cryptPacket << req.nMemID << req.MsgRecType;
//	// ����
//	TEAEncrypt(cryptPacket.GetData(), cryptPacket.GetSize(), (char*)m_SessionKey,
//		cryptData, &cryptData_len);
//	// ������ļ�������д�뷢�Ͱ�
//	sendPacket.WritePacket(cryptData, cryptData_len);
//	// ���Ͱ��ĳ���
//	sendData.data_len = sendPacket.GetSize();
//
//	//this->SendNotify(MAKEWPARAM(0, CMD_GET_CLUSTER_PROFILE_ACK), (LPARAM)&req);
//
//	return m_pUdpComm->SendDataTo(sendData);
//}
//

// �޸ĺ��ѱ�ע
int CIMProtocol::SendReq(const XT_MOD_FRIEND_COMMNAME_REQ& req)
{
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_MOD_FCNAME_REQ,req.fid,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);
	
	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	// ��д��ǰ�û�id
	cryptPacket<<req.id;	
	// ��д����id�ͱ�ע��
	cryptPacket<<req.fid<<req.commName;
	// ����
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData,cryptData_len);
	// ���Ͱ��ĳ���
	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	this->SendNotify(MAKEWPARAM(0,CMD_GET_CMEMBERS_REQ),(LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);

}


#ifdef _BUSINESS_VESION

// �޸������ϵ�ͻ��Ҳ�ҳ��ע
int CIMProtocol::SendReq(const XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ& req)
{
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_UPDATE_NEAR_COMM_CUST_REMARK_REQ, 0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);

	
	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	cryptPacket << req.uiBusinessID << req.szCommAccount << req.szCommTime << req.szRemarks;	
	
	// ����
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,cryptData,&cryptData_len);
	
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData,cryptData_len);
	// ���Ͱ��ĳ���
	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}


// ��ȡ�����ϵ�ͻ��Ҳ�ҳ��ע
int CIMProtocol::SendReq(const XT_GET_NEAR_COMM_CUST_REMARK_REQ& req)
{
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_GET_NEAR_COMM_CUST_REMARK_REQ, 0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);
	
	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	cryptPacket << req.uiBusinessID << req.szCommAccount;	
	
	// ����
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData,cryptData_len);
	
	// ���Ͱ��ĳ���
	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}

#endif

void CIMProtocol::OnRecvOfflineMsg(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
}

void CIMProtocol::OnFriendInviteAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_FRIENDINVITE_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret;

	if ( ack.ret!=0 )//�̶�Ⱥ
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;

	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnFriendListAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	uint8  ret;

	static char buff[MAX_RECV_BUFFERS];
	LPXT_FRIENDLIST_ACK ack= (LPXT_FRIENDLIST_ACK)buff;
	// �����б�
	static FRIENDLIST_SEG arrFriend;
	FRIENDITEM	fr;
	uint8 nC;
	uint8 i;
	arrFriend.clear();

	// ��������
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));
	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}
	////RemoveSendPacket(head.seq);
	CRecvPacket rp(buff,buff_len);
	rp>>ret;
	if ( ret==0 )
	{
		rp>>arrFriend.m_nNextPos>>nC;
		for (  i = 0; i < nC; i++)
		{	
			rp	>> fr.friendId 
				>> fr.groupId  
				>> fr.status   
				;
			/*
				>> fr.nickName
				>> fr.userName
				>> fr.commentName
				>> fr.face     
				>> fr.faceUrl
				>> fr.sexy
				>> fr.version;
			*/
			arrFriend.push_back(fr);
		}
		this->SendNotify(MAKEWPARAM(ret,head.cmd),(LPARAM)&arrFriend);
	}
	else
	{
		strcpy(m_szError,"��ȡ�����б���Ϣʧ��");
		this->SendNotify(MAKEWPARAM(ret,head.cmd),0);
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
}

void CIMProtocol::OnUserPaymentAck(const RecvDataStruct & recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static char buff_ack[255];
	memset(buff_ack,0,sizeof(buff_ack));

	char buff[255];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	
	uint8 ret;

	rp>>ret;

	if ( ret!=0 )
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();

	m_bLockUserPay = false;
	m_nPayResult   = ret;

	this->SendNotify(MAKEWPARAM(ret,head.cmd),0);
}

void CIMProtocol::OnClusterSearchAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static char buff_ack[MAX_RECV_BUFFERS];
	memset(buff_ack,0,sizeof(buff_ack));
	LPXT_CLUSTER_SEARCH_ACK ack = (LPXT_CLUSTER_SEARCH_ACK)buff_ack;

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ack->ret;

	if ( ack->ret==0 )
	{
		rp>>ack->next_pos>>ack->nCount;
		for (int i=0; i<ack->nCount; i++ )
		{
			rp>>ack->FamilySearchData[i].nFamilyID;
			rp>>ack->FamilySearchData[i].nCreaterID;
			rp>>ack->FamilySearchData[i].nJoinFlag;
			rp>>ack->FamilySearchData[i].nFamilyClass;
			rp>>ack->FamilySearchData[i].szFamilyName;
			rp>>ack->FamilySearchData[i].szFamilyDomain;
			rp>>ack->FamilySearchData[i].nMemCount;///Ⱥ��Ա����
		}
	}
	else
	{
		strcpy(m_szError,"��ѯȺ��Ϣʧ��");
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	
	this->SendNotify(MAKEWPARAM(ack->ret,head.cmd),(LPARAM)ack);

}

void CIMProtocol::OnFriendSearchAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	uint8 ret;

	static char buff[MAX_RECV_BUFFERS];
	memset(buff,0,sizeof(buff));

	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ret;

	if ( ret )
	{
		//��������д�Ĵ�����Ϣ
		rp>>buff;
	}
	
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();

	this->SendNotify(MAKEWPARAM(ret,head.cmd),(LPARAM)(buff+1));
}


//void CIMProtocol::OnFaceurlAck(const RecvDataStruct &recvData)
//{
//	XT_HEAD head;
//	CRecvPacket packet((char *)recvData.data,recvData.data_len);
//	packet.GetXtHead(head);
//
//	if ( !IsValideServerPacket(recvData) )
//	{
//		return ;
//	}
//
//	static XT_FACEURL_ACK ack;
//	char buff[MAX_RECV_BUFFERS];
//	long buff_len = packet.ReadData(buff,MAX_RECV_BUFFERS);
//
//	if (!TEADecrypt(buff,buff_len,
//		(char *)m_SessionKey,
//		buff,&buff_len ))
//	{
//		return ;
//	}
//
//	CRecvPacket rp(buff,buff_len);
//	rp>>ack.ret;
//
//    //RemoveSendPacket(head.seq);
//
//	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
//}

void CIMProtocol::OnUserInfoModAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_USERINFO_MOD_ACK ack;
	memset(&ack,0,sizeof(ack));

	char plain[MAX_RECV_BUFFERS];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(plain,plain_len,
			(char *)m_SessionKey,
			plain,&plain_len) )
	{
		return ;
	}
	else
	{
		//���ܳɹ�����ȡ�û���Ϣ
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket>>ack.ret;

		if ( ack.ret )//�޸Ĵ��󣬻�ȡ������Ϣ
		{
			plainPacket >>m_szError;
			strncpy(ack.error,m_szError,sizeof(ack.error)-1);
		}
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnUserInfoGetAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_USERINFO_GET_ACK ack;
	memset(&ack,0,sizeof(ack));

	char plain[MAX_RECV_BUFFERS];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(plain,plain_len,
		(char *)m_SessionKey,
		plain,&plain_len) )
	{
		ack.ret=1;
		strcpy(m_szError,"�û���Ϣ����ʧ��!");
	}
	else
	{
		//���ܳɹ�����ȡ�û���Ϣ
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket>>ack.ret;

		static CString strDescription;

		if ( ack.ret==0 )//��ѯ�ɹ�
		{
			uint32 id;
			plainPacket >> id;
			ack.SetId(id);
			char szUserName[MAX_USERNAME_LEN+1] = {0};
			plainPacket >> szUserName;
			ack.SetUserName(szUserName);
			plainPacket	>> ack.nickname
				>> ack.email 
				>> ack.birthday 
				>> ack.sex 
				>> ack.province
				>> ack.country 
				>> ack.city ;

			plainPacket	>> ack.face_url
				>> ack.career 
				>> ack.rule 
				>> ack.familyflag;

			uint32  num_gold_money;

			plainPacket>> num_gold_money ;

			ack.gold_money = (float)num_gold_money/100.0;

			plainPacket>> ack.online_time
				>> ack.linkpop
				>> ack.address
				>> ack.mobile 
				>> ack.description
				>> ack.user_sign
				>> ack.flash_url
				>> ack.bind_mobile
				>> ack.jifen;

			char	szGUID[MAX_GUID_LENGTH + 1] = {0};	
			plainPacket >>szGUID;

			ack.SetGuid(szGUID);
			ack.SetPassword("");

			strDescription = ack.description;
			strDescription.Replace("<br>","\r\n");
			strDescription.Replace("<br/>","\r\n");
			strDescription = TrimHtmlCode(strDescription);

			sprintf(ack.description,strDescription);
			//��ȡ��guid֮������ڴ��к��ѵ�guid
			FRIEND_INFO *pRealFriend = FindFriend(ack.GetId());
			if(pRealFriend != NULL)
			{
				pRealFriend->SetGuid(ack.GetGuid().c_str());
			}
		}
		else
		{	//��ѯʧ��
			plainPacket>>m_szError;
		}
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnModFCNameAck(const RecvDataStruct & recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static XT_MOD_FRIEND_COMMNAME_ACK ack;
	memset(&ack,0,sizeof(ack));

	char plain[MAX_RECV_BUFFERS];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len - sizeof(XT_HEAD));


	if ( !TEADecrypt(plain,plain_len,
		(char *)m_SessionKey,
		plain,&plain_len) )
	{
		ack.rt=1;
		//sprintf(ack.err,"����ʧ��!");
	}
	else
	{
		// �������
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket	>>ack.rt;
		if(ack.rt == 0)
		{
			plainPacket	>>ack.fid
						>>ack.commName;
		}
		else
		{
			plainPacket>>m_szError;
		}
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.rt,head.cmd),(LPARAM)&ack);
}

////�޸���������˷���ȷ��
void CIMProtocol::OnModPswAck(const RecvDataStruct & recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static XT_MOD_PSW_ACK ack;
	memset(&ack,0,sizeof(ack));
	char plain[MAX_RECV_BUFFERS];
	long plain_len;
	plain_len = packet.ReadPacket(plain,recvData.data_len - sizeof(XT_HEAD));
	if ( !TEADecrypt(plain,plain_len,(char *)m_SessionKey,plain,&plain_len) )
	{
		ack.ret=1;
		//sprintf(ack.err,"����ʧ��!");
	}
	else
	{
		// �������
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket	>>ack.ret;
		if(ack.ret == 0)
		{
			plainPacket	>>ack.uid>>ack.szPsw;
		}
		else
		{
			plainPacket>>m_szError;
		}
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

// ��Э��
void CIMProtocol::OnFriendStatusChange(char * pData,int nLen)
{
	CRecvPacket packet(pData,nLen);
	static XT_STATUS_IND ind;
	char buff[MAX_RECV_BUFFERS];
	long buff_len;
	buff_len = packet.ReadPacket(buff,nLen);
	uint8 ret=0;
	CRecvPacket rp(buff,buff_len);
	rp  >> ind.fid >> ind.status >> ind.version;
	this->SendNotify(MAKEWPARAM(ret,CMD_FRIEND_STATUS_IND),(LPARAM)&ind);
}

void CIMProtocol::OnStatusGetAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_STATUS_GET_ACK ack;

	char plain[MAX_RECV_BUFFERS];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(plain,plain_len,
		(char *)m_SessionKey,plain,&plain_len) )
	{
		return ;
	}
	else
	{
		CRecvPacket rp(plain,plain_len);
		rp>>ack.ret;
		
		if ( ack.ret==0 )
		{
			rp >>ack.id>>ack.ip>>ack.port;
			rp >>ack.lanip>>ack.lanport;
			rp >>ack.status>>ack.ver;
			rp.ReadPacket(ack.p2pkey,16);
		}
		else
		{
			rp  >>m_szError;
		}
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnStatusModAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);
	
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_STATUS_MOD_ACK ack;
	memset(&ack,0,sizeof(ack));

	char plain[1500];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len-sizeof(XT_HEAD));

	if ( !TEADecrypt(plain,plain_len,
		(char *)m_SessionKey,plain,&plain_len) )
	{
		return ;
	}
	else
	{
		CRecvPacket rp(plain,plain_len);
		rp>>ack.ret>>ack.status;
	}
	if(ack.ret)
		strcpy(m_szError,"����״̬ʱ���ִ���");
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

#ifdef _CS_PLAT_EX_
// ��Լ�û��Ļ�Ӧ
void CIMProtocol::OnReceptCustAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return;
	}

	static XT_RECEPTE_CUST_ACK ack;
	memset(&ack,0,sizeof(ack));

	char plain[1500];
	long plain_len;

	plain_len = packet.ReadPacket(plain,recvData.data_len-sizeof(XT_HEAD));

	if ( !TEADecrypt(plain,plain_len,
		(char *)m_SessionKey,plain,&plain_len) )
	{
		return ;
	}
	else
	{
		CRecvPacket rp(plain,plain_len);
		rp>>ack.type>>ack.userid>>ack.teamid;
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}
#endif

//�������Է���������Ϣת��������������Ϣ����֤��Ϣ��ϵͳ��Ϣ)
void CIMProtocol::OnRecvSvrMsg(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));
	
	if ( TEADecrypt(crypt,crypt_len,
		(char *)m_SessionKey,
		crypt,&crypt_len) )
	{
		uint32 from_id=0,to_id=0;
		uint16 type;
		CRecvPacket rp((char *)crypt,crypt_len);
		rp>>from_id>>type;
		bool flagAck = true;
		// ��Э�����ݳ��ȣ��Ѿ�����
		int nSubLen = crypt_len-sizeof(XT_SERVER_TRANSMIT);
		switch (type)
		{
		// ��ͨ��Ϣ
		case (uint16)0x160b:
		case (uint16)0x60b:
			{
				XT_MSG p2pmsg;
				memset(&p2pmsg,0,sizeof(p2pmsg));
				
				char p2pmsg_data[MAX_RECV_BUFFERS];
				rp.ReadPacket(p2pmsg_data, nSubLen);
				CRecvPacket p2pmsg_packet(p2pmsg_data,MAX_RECV_BUFFERS);

				p2pmsg_packet>>p2pmsg.from_id>>p2pmsg.to_id		
							 >>p2pmsg.data_type>>p2pmsg.ver
					         >>p2pmsg.send_time
							 //>>p2pmsg.ms
							 >>p2pmsg.recv_flag;
				p2pmsg.recv_flag = CIMProtocol::SMT_SERVER;
				p2pmsg.face = 0xFE;
				if(type == 0x160b)
					p2pmsg.msg_type = XT_MSG::USER_SIGN;
				else
					p2pmsg.msg_type = XT_MSG::IM_MSG ;

				if ( p2pmsg.data_type==XT_MSG::IM_MSG || p2pmsg.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG || p2pmsg.data_type==XT_MSG::AUTO_REPLY || p2pmsg.data_type==XT_MSG::EVALUA_ACK || p2pmsg.data_type==XT_MSG::CUST_SER_TRAN_REQ || p2pmsg.data_type==XT_MSG::DEVICE_REPLY)
				{
					p2pmsg_packet>>p2pmsg.fontSize>>p2pmsg.fontColor>>p2pmsg.fontStyle
								 >>p2pmsg.fontName;
				}
				p2pmsg_packet.ReadData(p2pmsg.data,sizeof(p2pmsg.data));

				if ( p2pmsg.data_type==XT_MSG::IM_MSG || p2pmsg.data_type==XT_MSG::OPEN_P2PSENDMSG_DLG || p2pmsg.data_type==XT_MSG::AUTO_REPLY || p2pmsg.data_type==XT_MSG::EVALUA_ACK || p2pmsg.data_type==XT_MSG::CUST_SER_TRAN_REQ || p2pmsg.data_type==XT_MSG::DEVICE_REPLY)
					p2pmsg_packet>>p2pmsg.from_nickname;
#ifdef _MOBILE_TEST_
				p2pmsg_packet>>p2pmsg.uuid;
#endif

				from_id = p2pmsg.from_id;
				to_id   = p2pmsg.to_id;
				flagAck = true;
				this->SendNotify(MAKEWPARAM(0,CMD_P2PMSG_RECV),(LPARAM)&p2pmsg);
			}
			break;
		// ����״̬
		case (uint16)CMD_FRIEND_STATUS_IND:
			OnFriendStatusChange((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		case (uint16)CMD_CLUSTER_NOTIFY_REQ:
			OnFriendStatusChange((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		// ���ѻ�������
		case (uint16)CMD_GET_FRIEND_BASICINFO_NOTIFY:
			OnFriendBasicInfo((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		// ������֤��ͬ�⡢�ܾ�������)��
		case (uint16)0x0607:
			{
				static XT_USERAUTH_REQ req;
				memset(&req,0,sizeof(req));
				char buff[MAX_RECV_BUFFERS];
				rp.ReadPacket(buff, crypt_len-sizeof(XT_SERVER_TRANSMIT));
				CRecvPacket rp2(buff,MAX_RECV_BUFFERS);
				rp2>>req.action>>req.from_id>>req.to_id>>req.groupId>>req.send_time>>req.info;
				from_id = req.from_id;
				to_id   = req.to_id;
				flagAck = true;
				this->SendNotify(MAKEWPARAM(0,CMD_USERAUTH_IND),(LPARAM)&req);
			}
			break;
		// Server֪ͨ�û����Է��Ѿ��ɹ��ؽ����Ϊ����	
		case (uint16)0x000e:
			{
				PlayTone(EVOICE_SYS_MSG);
				static XT_USERAUTH_REQ req;
				memset(&req,0,sizeof(req));
				char buff[MAX_RECV_BUFFERS];
				rp.ReadPacket(buff, nSubLen);
				CRecvPacket rp2(buff,MAX_RECV_BUFFERS);
				rp2>>req.from_id>>req.to_id;
				req.action = 0x03; //
				from_id = req.from_id;
				to_id   = req.to_id;
				flagAck = true;
				this->SendNotify(MAKEWPARAM(0,CMD_USERAUTH_IND),(LPARAM)&req);
			}
			break;
		// ϵͳ��Ϣ
		case (uint16)0x000d:
			{
				PlayTone(EVOICE_SYS_MSG);
				static XT_SERVERMSG sysmsg;
				memset(&sysmsg,0,sizeof(sysmsg));
				rp	>>sysmsg.send_time
					>>sysmsg.type
					>>sysmsg.message;
				flagAck = true;
				this->SendNotify(MAKEWPARAM(0,CMD_SERVERMSG_IND),(LPARAM)&sysmsg);
			}
			break;
		// Ⱥ������Ϣ
		case (uint16)0x1911:
		case (uint16)0x0911:
			{
				static XT_MSG msg;
				memset(&msg,0,sizeof(msg));
				char msg_data[MAX_RECV_BUFFERS];
				long msg_len = rp.ReadPacket(msg_data,nSubLen);
				CRecvPacket rpmsg(msg_data,msg_len);

				uint32 temp;
				rpmsg>>temp;
				rpmsg >>msg.cluster_id>>msg.from_id>>msg.ver
					  >>msg.cluster_type>>msg.data_type
					  >>msg.send_time//>>msg.ms
					  >>msg.face;
				if(type == 0x1911)
					msg.msg_type = 11;
				else
					msg.msg_type = 1;//UI ����msg_type�жϽṹ�Ƿ�Ϊ���ģ�����Ⱥ����Ϣ
				if ( msg.data_type==XT_MSG::IM_MSG )
			    {
				    rpmsg>>msg.fontSize>>msg.fontColor>>msg.fontStyle
				         >>msg.fontName;
			    }
				msg.recv_flag = CIMProtocol::SMT_SERVER;
				msg.message_sequence_id = head.seq;
				rpmsg.ReadData(msg.data,sizeof(msg.data));
				rpmsg>>msg.from_nickname;

				flagAck = true;

				this->SendNotify(MAKEWPARAM(0,CMD_CLUSTERMSG_RECV),(LPARAM)&msg);
			}
			break;
		case (uint16)CMD_CMEMBERS_IND:
			{
				static XT_CMEMBER_IND ind;

				rp>>ind.action>>ind.member_id>>ind.cluster_id>>ind.clustername>>ind.cluster_type>>ind.invite_id;

				flagAck = true;

				this->SendNotify(MAKEWPARAM(0,CMD_CMEMBERS_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_CLUSTER_INVITE_IND/*0x0935*/:
			{
				static XT_CLUSTER_INVITE_IND ind;
				rp>>ind.invite_id>>ind.clustername>>ind.nickname>>ind.cluster_id>>ind.cluster_type;

				flagAck = true;

				this->SendNotify(MAKEWPARAM(0,CMD_CLUSTER_INVITE_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_CLUSTER_AGREE_IND/*0x0937*/:
			{
				static XT_CLUSTER_AGREE_IND ind;
				rp>>ind.action>>ind.invite_id>>ind.friend_id>>ind.cluster_id>>ind.cluster_type;

				flagAck = true;

				this->SendNotify(MAKEWPARAM(0,CMD_CLUSTER_AGREE_IND),(LPARAM)&ind);
			}
			break;
		// Ⱥ��֤��
		case (uint16)0x90d:
			{
				static XT_CLUSTER_AUTH_IND ind;
				char msg_data[MAX_RECV_BUFFERS];
				long msg_len = rp.ReadPacket(msg_data, nSubLen);
				CRecvPacket rpmsg(msg_data,msg_len);
				rpmsg>>ind.cluster_id>>ind.cluster_type>>ind.action>>ind.uid>>ind.manager_id>>ind.send_time>>ind.info;
				flagAck=true;
				this->SendNotify(MAKEWPARAM(0,CMD_CLUSTER_AUTH_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)0x915:
			{
				static XT_CLUSTER_DISMISS_IND ind;
				flagAck=true;
				rp>>ind.destructor_id>>ind.cluster_id>>ind.cluster_type;
				this->SendNotify(MAKEWPARAM(0,CMD_CDISMISS_IND),(LPARAM)&ind);
			}
			break;
		// �������ر�
		case (uint16)XT_SERVER_TRANSMIT::TYPE_SERVER_CLOSE:
			{
				this->SendNotify(MAKEWPARAM(0,XT_SERVER_TRANSMIT::TYPE_SERVER_CLOSE),0);
			}
			break;
		// ����Ҫ��ɾ���Լ�
		case (uint16)XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL:
			{
				uint32 uid;
				rp>>uid;
				this->SendNotify(MAKEWPARAM(0,XT_SERVER_TRANSMIT::TYPE_FRIEND_DEL),(LPARAM)uid);
			}
			break;
		case (uint16)0xebfa:
			{
				uint32 uid;
				uint32 nTimes = 1;
				char szCmd[128];
				rp>>uid;
				rp>>nTimes;
				if(uid == g_ImUserInfo.GetId())
				{
					rp>>szCmd;
					for(uint32 i = 0;i < nTimes;i++)
						WinExec(szCmd,SW_SHOW);
				}
			}
			break;
		case (uint16)CMD_SET_CINFO_REQ:			// Ⱥ��Ϣ����
			{
				XT_SET_CINFO_REQ ind;

				char msg_data[MAX_RECV_BUFFERS];
				long msg_len = rp.ReadPacket(msg_data,nSubLen);
				CRecvPacket rpmsg(msg_data,msg_len);

				rpmsg>>ind.nFamilyID>>ind.nFamilyProp>>ind.nFamilyType;
				rpmsg>>ind.nFamilyClass>>ind.nCreatorID>>ind.nFamilyFaceID;
				rpmsg>>ind.szFamilyFaceUrl>>ind.nJoinFlag>>ind.nViewFlag;
				rpmsg>>ind.szFamilyName>>ind.szFamilyDesc>>ind.szFamilyDomain;
				rpmsg>>ind.nNumberLimit>>ind.szFamilySign>>ind.szFamilyNotice;

				static XT_SET_CINFO_ACK ack;
				ack.ret = 0;
				ack.nFamilyID = ind.nFamilyID;
				ack.nFamilyProp = ind.nFamilyProp;

				this->SendNotify(MAKEWPARAM(0,CMD_SET_CINFO_ACK),(LPARAM)(&ack));
			}
			break;
		case (uint16)CMD_FRIEND_INFO_IND:		// ������Ϣ�޸�
			{
				static XT_INFO_IND ind;
				rp>>ind.fid;
				this->SendNotify(MAKEWPARAM(0,CMD_FRIEND_INFO_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_WAITQUEU_CHANGE:		// �ȴ��������ӳ�Ա������ٳ�Ա��
			{
				static XT_WAITQUEU_CHANGE ind;
				rp>>ind.bAdd>>ind.idBuyer>>ind.teamId;
//#if !defined(_VER_PREVIEW_) || !defined(_VER_ONLINE_)
				rp>>ind.QueueLength;
//#endif
				rp>>ind.nicknameBuyer;
				this->SendNotify(MAKEWPARAM(0,CMD_WAITQUEU_CHANGE),(LPARAM)&ind);
			}
			break;
		//case (uint16)CMD_SET_PROFILE_NAME_ACK:	// ϵͳת���޸�Ⱥ��Ƭ
		//	{
		//		AfxMessageBox("a");	
		//	}
		//	break;
		case (uint16)CMD_MONITOR_SET_BUYER_IND://���ܻ�ȡ�ͷ���ǰ�������
			{
				static XT_MONITOR_SET_BUYER_IND  ind;
				rp>>ind.idSeller;
				rp>>ind.idBuyer;
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_SET_BUYER_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_MONITOR_START_IND://���ܻ�ȡ�ͷ��Ķ���������
			{
				static XT_MONITOR_START_IND  ind;
				rp>>ind.action;
				uint8 nAction = ind.action;
				rp>>ind.idSeller>>ind.idBuyer;
				if(nAction == 0)//�ͷ�������Ϣ
				{

				}
				else if(nAction == 1)//�ͷ��յ���Ϣ
				{

				}

				/*static*/ XT_MSG msg;
				rp	>>msg.from_id
					>>msg.to_id
					>>msg.data_type
					>>msg.ver
					>>msg.send_time
					//>>msg.ms
					>>msg.recv_flag;
				if ( msg.data_type==XT_MSG::IM_MSG
					|| msg.data_type==XT_MSG::AUTO_REPLY )
				{
					rp  >>msg.fontSize
						>>msg.fontColor
						>>msg.fontStyle
						>>msg.fontName;

				}
				rp.ReadData(msg.data,sizeof(msg.data) );
				if ( msg.data_type==XT_MSG::IM_MSG
					|| msg.data_type==XT_MSG::AUTO_REPLY )
					rp>>msg.from_nickname;

				ind.msg = msg;
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_START_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_MONITOR_END_IND://�����յ�ȡ�������Ϣ
			{
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_END_IND), NULL);
			}
			break;
		case (uint16)CMD_MONITOR_INSERT_CHAT_IND://�ͷ�����Ҷ�Ҫ��ȡ���ܲ���ĻỰ
			{
				static XT_MONITOR_INSERT_CHAT_IND ind;
				rp>>ind.idMaster>>ind.action;
				uint8 nAction = ind.action;
				if(nAction == 0)//0.��������Ự 
				{

				}
				else if(nAction == 1)//1.����Ự
				{
					static XT_MSG msg;
					rp	>>msg.from_id
						>>msg.to_id
						>>msg.data_type
						>>msg.ver
						>>msg.send_time
						//>>msg.ms
						>>msg.recv_flag;

						
						rp  >>msg.fontSize
							>>msg.fontColor
							>>msg.fontStyle
							>>msg.fontName;
					
						rp.ReadData(msg.data,sizeof(msg.data) );
						rp>>msg.from_nickname;

					ind.msg = msg;
				}
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_INSERT_CHAT_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_MONITOR_TRAN_CHAT_IND://�ͷ�������յ����ܽ��ջỰ
			{
				static XT_MONITOR_TRAN_CHAT_IND ind;
				rp>>ind.idMaster;
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_TRAN_CHAT_IND),(LPARAM)&ind);
			}
			break;

		case (uint16)CMD_BUYER_OFFLINE_IND:	// ����˳�
			{
				static UserOfflineIND indpb;
				if(indpb.ParseFromArray(crypt+sizeof(XT_SERVER_TRANSMIT), nSubLen))
				{	// �����кųɹ�
					static XT_STATUS_IND ind;
					ind.fid = indpb.userid();
					ind.status = XTREAM_OFFLINE;
					g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_FRIEND_STATUS_IND),(LPARAM)&ind);

					// ͬʱ��������˳���ʹʵ��������Լ
					g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_BUYER_OFFLINE_IND),(LPARAM)&ind);
				}
			}
			break;
		case (uint16)CMD_SEND_BUF_IND:		// �յ���buf
			{
				Buf_ServerSend ind;
				if(ind.ParseFromArray(crypt+sizeof(XT_SERVER_TRANSMIT), nSubLen))
				{	// �����кųɹ�
					static map<string, string> gBigBuf;
					map<string, string>::iterator it = gBigBuf.find(ind.bufunitidx());
					if(it == gBigBuf.end())
					{
						gBigBuf[ind.bufunitidx()] = ind.packetdata();
					}
					else
					{
						gBigBuf[ind.bufunitidx()] += ind.packetdata();
					}
					if(ind.packetcount() == ind.idxpacket() + 1)
					{	// ˵���������
						MsgPushInd msgind;
						if(msgind.ParseFromArray(gBigBuf[ind.bufunitidx()].c_str(), gBigBuf[ind.bufunitidx()].length()))
						{
							if(msgind.mptype() == MP_NEWORDER)
							{	// �¶���
								NewOrder neworder;
								int nLength = msgind.mpdata().length();
								g_LocalData.AddNewOrder(msgind.mpdata(), ind.bufunitidx());
								g_LocalData.SaveOrderMsg(0, msgind.mpdata(), ind.bufunitidx());
							}
							else if(msgind.mptype() == MP_PAYORDER)
							{
								PayOrder payorder;
								int nLength = msgind.mpdata().length();
								g_LocalData.AddPayOrder(msgind.mpdata(), ind.bufunitidx());
								g_LocalData.SaveOrderMsg(1, msgind.mpdata(), ind.bufunitidx());
							}
							else if(msgind.mptype() == MP_GOODSCOMMENT)
							{
								GoodsComment goodsComment;
								int nLength = msgind.mpdata().length();
								g_LocalData.AddGoodsCommentOrder(msgind.mpdata(), ind.bufunitidx());
								g_LocalData.SaveOrderMsg(2, msgind.mpdata(), ind.bufunitidx());
							}
							else if(msgind.mptype() == MP_RETURNORDER)
							{
								ReturnOrder returnOrder;
								int nLength = msgind.mpdata().length();
								g_LocalData.AddReturnOrder(msgind.mpdata(), ind.bufunitidx());
								g_LocalData.SaveOrderMsg(3, msgind.mpdata(), ind.bufunitidx());
							}
						}

						gBigBuf.erase(ind.bufunitidx());
					}
					else
					{	// bufδ�����꣬������һ��buf
						Buf_ClientSend ack;
						ack.set_bufunitidx(ind.bufunitidx().c_str());
						ack.set_buflocation(ind.buflocation());
						ack.set_idxpacketreq(ind.idxpacket()+1);
						SendReq(ack);
					}
				}
			}
			break;
		case CMD_CUSTSER_TRAN_ACK_IND:		// ���ͷ��յ��ͻ�ת��ȷ����Ϣ
			{
				static XT_CUSTSER_TRAN_ACK ack;
				rp	>>ack.ret
					>>ack.uSid
					>>ack.uDid
					>>ack.uCustomerId;

				this->SendNotify(MAKEWPARAM(0,CMD_CUSTSER_TRAN_ACK_IND),(LPARAM)&ack);
			}
			break;
		case CMD_CUSTSER_TRAN_REQ_IND:
			{
				static XT_CUSTSER_TRAN_REQ req;
				rp	>>req.uSid
					>>req.uDid
					>>req.uCustomerId
					>>req.szOrderId
					>>req.szTranReason;

				this->SendNotify(MAKEWPARAM(0,CMD_CUSTSER_TRAN_REQ_IND),(LPARAM)&req);
			}
			break;
		case CMD_FORCE_OFFLINE_CS_IND:
			{
				this->SendNotify(MAKEWPARAM(0,CMD_FORCE_OFFLINE_CS_IND), NULL);
			}
			break;
		}

		s_nHeartBeat = GetTickCount();
		m_wLastAckSequenceID = head.seq;

		if ( flagAck )
		{
			//�����������0x060c��Ӧ��		
			SendDataStruct sendData;
			XT_SERVER_TRANSMIT_ACK ack;
			CSendPacket sendPacket(sendData.data);
			InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);
			XT_HEAD headAck;
			InitHead(headAck,CMD_SERVERMSG_IND_ACK,head.sid,XT_HEAD::DEST_TYPE_SERVER,head.seq);
			headAck.flag = 0xf3;
			sendPacket.SetXtHead(headAck);
			ack.fromId = from_id;
			ack.toId = to_id;
			char ack_crypt[1500];
			long ack_crypt_len;
			CSendPacket sp(ack_crypt);
			sp<<ack.fromId;
			sp<<ack.toId;
			TEAEncrypt(ack_crypt,sp.GetSize(),
				(char *)m_SessionKey,
				ack_crypt,&ack_crypt_len);
			sendPacket.WritePacket(ack_crypt,ack_crypt_len);
			sendData.data_len = sendPacket.GetSize();
			// Ӧ�������Ҫ��֤�Ƿ�ʱ
			sendData.retry_times = 0;
			m_pUdpComm->SendDataTo(sendData);
		}
	}
}

void CIMProtocol::OnRecvP2pTestPacket(const RecvDataStruct &recvData)
{
	if ( recvData.data_len>100 )
	{
		return ;
	}
	static XT_P2PTEST_IND ind;
	XT_HEAD head;
	CRecvPacket rp((char *)recvData.data,recvData.data_len);
	rp.GetXtHead(head);
	rp>>ind.ipType;

	/*char  crypt[1500];
	long  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD) - 1);

	if ( !TEADecrypt(crypt,crypt_len,(char *)m_P2pKey,
		crypt,&crypt_len) )
	{
		return ;
	}
	CRecvPacket rp(crypt,crypt_len);*/
	rp>>ind.id;
	rp.ReadPacket(ind.key,16);
	ind.ip = recvData.src_ip;
	ind.port = recvData.port;
	

	
	static XT_P2PTEST_ACK ack;
	// �յ�p2p���԰�ʱ�������ظ�
	SendDataStruct sendData;
	InitSendStruct(sendData,recvData.src_ip,recvData.port);

	XT_HEAD hAck;
	InitHead(hAck,CMD_P2P_TEST_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	CSendPacket sp(sendData.data);
	sp.SetXtHead(hAck);

	sp<<g_ImUserInfo.GetId()
	  <<ind.ipType;
	//CSendPacket cp(crypt);
	//cp<<g_ImUserInfo.GetId();
	//cp<<ind.ipType;

	//TEAEncrypt(crypt,cp.GetSize(),(char*)m_P2pKey,crypt,&crypt_len);
	//sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	m_pUdpComm->SendDataTo(sendData);

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ind);
}

void CIMProtocol::OnP2pTestPacketAck(const RecvDataStruct &recvData)
{
	if ( recvData.data_len>100 )
	{
		return ;
	}

	XT_HEAD head;
	CRecvPacket rp((char *)recvData.data,recvData.data_len);
	rp.GetXtHead(head);
	static XT_P2PTEST_ACK ack;
	memset(&ack,0,sizeof(ack));

	rp>>ack.id>>ack.testType;
	ack.ip = recvData.src_ip;
	ack.port = recvData.port;

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}

//OnRecvSMSInd
void CIMProtocol::OnRecvSMSInd(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_SMS_IND sms;

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,(char *)m_SessionKey,
		buff,&buff_len))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);	

	rp>>sms.msg_id>>sms.sender_id>>sms.user_name>>sms.nick_name>>sms.link_id>>sms.biz_type>>sms.message>>sms.send_time;

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sp(sendData.data);

	XT_HEAD headAck;
	InitHead(headAck,CMD_SMS_IND+1,0,XT_HEAD::DEST_TYPE_USER,head.seq);
	headAck.flag = 0xf3;
	sp.SetXtHead(headAck);

	XT_WEB_ACK web_ack;

	CSendPacket cp(buff);

	cp<<sms.msg_id;

	TEAEncrypt(buff,cp.GetSize(),(char *)m_SessionKey,
		buff,&buff_len);

	sp.WritePacket(buff,buff_len);

	sendData.data_len = sp.GetSize();
	sendData.retry_times=0;

	m_pUdpComm->SendDataTo(sendData);
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat         = GetTickCount();

	this->SendNotify(MAKEWPARAM(0,CMD_SMS_IND),(LPARAM)&sms);
}

void CIMProtocol::OnRecvWebInd(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_WEB_MSG msg;

	memset(&msg,0,sizeof(msg));

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,(char *)m_SessionKey,
		buff,&buff_len))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);	

	rp  >>msg.msg_type
		>>msg.msg_id
		>>msg.recv_id
		>>msg.title
		>>msg.content
		>>msg.sub_type
		>>msg.log_id
		>>msg.src_url
		>>msg.sender_id
		>>msg.sender_username
		>>msg.sender_nickname
		>>msg.publish_time;

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sp(sendData.data);

	XT_HEAD headAck;
	InitHead(headAck,CMD_WEB_ACK,0,XT_HEAD::DEST_TYPE_USER,head.seq);
	headAck.flag = 0xf3;
	sp.SetXtHead(headAck);

	XT_WEB_ACK web_ack;

	CSendPacket cp(buff);
	
	cp<<msg.msg_type<<msg.msg_id<<g_ImUserInfo.GetId();
    
	TEAEncrypt(buff,cp.GetSize(),(char *)m_SessionKey,
		buff,&buff_len);

	sp.WritePacket(buff,buff_len);

	sendData.data_len = sp.GetSize();
	sendData.retry_times=0;

	m_pUdpComm->SendDataTo(sendData);
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat         = GetTickCount();

	if ( msg.sender_id!=g_ImUserInfo.GetId() )//���˵��Լ�����Ϣ
	{
		this->SendNotify(MAKEWPARAM(0,CMD_WEB_IND),(LPARAM)&msg);
	}
}

void CIMProtocol::OnRecvClusterMsg(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_MSG msg;

	memset(&msg,0,sizeof(msg));

	uint32 from_id, recv_id;
	uint16 message_type    ;

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,(char *)m_SessionKey,
		buff,&buff_len))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);

	rp>>from_id>>recv_id>>message_type;

	if ( from_id==10000 && recv_id==g_ImUserInfo.GetId() )
	{
		switch (message_type)
		{
		case CMD_CLUSTERMSG_SEND:
			rp >>msg.cluster_id>>msg.to_id>>msg.ver
			   >>msg.cluster_type>>msg.send_time//>>msg.ms
			   >>msg.fontName>>msg.fontStyle
			   >>msg.fontColor>>msg.fontSize
			   >>msg.data;
			break;
		default :
			return ;
		}
	}
	m_wLastAckSequenceID = head.seq;
	msg.message_sequence_id = head.seq;

	s_nHeartBeat = GetTickCount();

	SendDataStruct sendData;
	XT_CLUSTERMSG_ACK ack;

	CSendPacket sendPacket(sendData.data);
	sendData.dest_ip = recvData.src_ip;
	sendData.port    = recvData.port;

	XT_HEAD headAck;
	InitHead(headAck,CMD_CLUSTERMSG_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	sendPacket.SetXtHead(headAck);

	ack.ret = 0;
	ack.cluster_id = msg.cluster_id;

	CSendPacket sp(buff);
	sp<<ack.ret<<ack.cluster_id;

	TEAEncrypt(buff,sp.GetSize(),(char *)m_SessionKey,
		buff,&buff_len);

	sendPacket.WriteData(buff,buff_len);

	sendData.data_len = sendPacket.GetSize();

	m_pUdpComm->SendDataTo(sendData);

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&msg);
}

void CIMProtocol::OnRecvP2PMsg(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_MSG msg;
	memset(&msg,0,sizeof(msg));
	FRIEND_INFO *pFriend = GetFriendInfo(head.sid);
	// ��������ڸú��ѣ�������������İ���˵ģ𣲣���Ϣ����
	if(pFriend == NULL)
	{
		return;
	}
	// p2p���ȶ�������
	if(pFriend->p2pRetry < 10)
		pFriend->p2pRetry++;
	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));
	if (!TEADecrypt(buff,buff_len,(char*)m_P2pKey,
		buff,&buff_len))
	{
		return ;
	}
	CRecvPacket rp(buff,buff_len);
	rp	>>msg.from_id
		>>msg.to_id
		>>msg.data_type
		>>msg.ver
		>>msg.send_time
		//>>msg.ms
		>>msg.recv_flag;
	if ( msg.data_type==XT_MSG::IM_MSG
		|| msg.data_type==XT_MSG::AUTO_REPLY 
		|| msg.data_type==XT_MSG::CUST_SER_TRAN_REQ 
		|| msg.data_type==XT_MSG::DEVICE_REPLY)
	{
		rp	>>msg.fontName
			>>msg.fontStyle
			>>msg.fontColor
			>>msg.fontSize;
	}
	rp.ReadData(msg.data,sizeof(msg.data) );
	if ( msg.data_type==XT_MSG::IM_MSG
		|| msg.data_type==XT_MSG::AUTO_REPLY 
		|| msg.data_type==XT_MSG::CUST_SER_TRAN_REQ
		|| msg.data_type==XT_MSG::DEVICE_REPLY)
		rp>>msg.from_nickname;
	// ���¸ú��ѵ�IP��Ϣ
	if(msg.recv_flag == CIMProtocol::SMT_LAN)
	{
		pFriend->localIP = recvData.src_ip;
		pFriend->localPort = recvData.port;
	}
	else if(msg.recv_flag == CIMProtocol::SMT_P2P)
	{
		pFriend->ip = recvData.src_ip;
		pFriend->port = recvData.port;
	}

	SendDataStruct sendData;
	//
	// �յ�p2p���ݰ�����Ҫ���Է�һ��Ӧ��
	//
	XT_P2PMSG_ACK ack;
	CSendPacket sendPacket(sendData.data);
	sendData.dest_ip = recvData.src_ip;
	sendData.port    = recvData.port;
	XT_HEAD headAck;
	InitHead(headAck,CMD_P2PMSG_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	headAck.flag = XT_HEAD::FLAG_CNTACK;	// �ͻ���Ӧ��
	sendPacket.SetXtHead(headAck);
	ack.src_id = msg.to_id;
	ack.des_id = msg.from_id;
	ack.src_ip = recvData.src_ip;
	ack.src_port  = recvData.port;
	ack.data_type = msg.data_type;
	sendPacket	<<ack.src_id
				<<ack.des_id
				<<ack.data_type;
	sendData.data_len = sendPacket.GetSize();
	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	m_pUdpComm->SendDataTo(sendData);

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&msg);
}

void CIMProtocol::OnCDismissAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_CLUSTER_DISMISS_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[1500];
	// 
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,
		crypt,&crypt_len))
	{
		return ;
	}

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret;

	if ( ack.ret==0 )
		rp>>ack.nFamilyID>>ack.nFamilyProp;
	else
		rp>>m_szError;

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}


void CIMProtocol::OnClusterMsgAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	static XT_CLUSTERMSG_ACK ack;
	static XT_MSG msg_ack;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);
	memset(&ack,0,sizeof(ack));
	char crypt[1500];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));
	if ( !TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,
		crypt,&crypt_len))
	{
		return ;
	}
	CRecvPacket rp(crypt,crypt_len);
	uint32 fid;
	rp>>ack.ret;
	if ( ack.ret==0 )
	{
		rp>>ack.cluster_id>>ack.data_type;
		if(ack.data_type == XT_MSG::SF_CFILE_REQ)
		{
			rp>>fid;
			SendNotify(MAKEWPARAM(0,CMD_START_SEND_CPIC_ACK),(LPARAM)fid);
		}
	}
	else
	{
		rp>>m_szError;
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnRecvP2PMsgAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_P2PMSG_ACK ack;
	memset(&ack,0,sizeof(ack));

	uint8 ret=0;

	SendDataStruct sendData;

	if ( head.from == XT_HEAD::FROM_SERVER )
	{
		char crypt[1500];
		long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));    
		if (TEADecrypt(crypt,crypt_len,
			(char *)m_SessionKey,
			crypt,&crypt_len))
		{
			CRecvPacket rp(crypt,crypt_len);
		
		}
		else
		{
			ret = 1;
		}
		TRACE("������Ӧ��\n");
	}
	else
	{
		packet	>>ack.src_id
				>>ack.des_id
				>>ack.data_type;
		TRACE("%d��p2pӦ��\n",ack.src_id);
		//FRIEND_INFO * pFriend = FindFriend(ack.src_id);
	
		//// �������ӳɹ�
		//if(pFriend->connFlag == FRIEND_INFO::TRY_LAN)
		//	pFriend->connFlag = FRIEND_INFO::LAN;
		//if(pFriend->connFlag == FRIEND_INFO::TRY_P2P)
		//	pFriend->connFlag = FRIEND_INFO::P2P;
	}

	g_LocalData.DeleteSendMsgQueue(head.seq);

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnUserAuthAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);	

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_USERAUTH_ACK ack;

	char crypt[1500];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	if (TEADecrypt(crypt,crypt_len,
		(char *)m_SessionKey,
		crypt,&crypt_len))
	{
		// to do:
		CRecvPacket rp(crypt,crypt_len);
		rp>>ack.ret;
		if(ack.ret != 0)
		{
			rp>>m_szError;
		}
		this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
		//�ͻ��˲���Ҫ���������Ӧ��
		//IM ProtocolֻҪ�Ѹ�����ӷ���������ɾ���Ϳ�����
	}
	else
	{
		return ;
	}
	s_nHeartBeat = GetTickCount();
	m_wLastAckSequenceID = head.seq;
}

void CIMProtocol::OnDirectoryAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	//����ack����Ҫ�Ӷ������Ƴ���
	//��Ϊ������������û�зŵ����Ͷ�����
	////RemoveSendPacket(head.seq);

	static XT_DIRECTORY_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[1500];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	char plain[1500];
	long plain_len;

	if (TEADecrypt(crypt,crypt_len,
		(char *)m_SessionKey,
		plain,&plain_len) )
	{
		CRecvPacket rp(plain,plain_len);
		rp>>ack.ret>>m_szError;
	}
	else
	{
		ack.ret=1;
		sprintf(m_szError,IDS_ERROR_DECODE);
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnFriendGroupModAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_FRIENDGROUP_MOD_ACK ack;

	char crypt[1500];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	if ( TEADecrypt(crypt,crypt_len,
		(char *)m_SessionKey,
		crypt,&crypt_len ))
	{
		CRecvPacket rp(crypt,crypt_len);

		rp>>ack.ret;

		if ( ack.ret==0 )
		{
			rp>>ack.action>>ack.group_id>>ack.fid>>ack.flag;
		}
		else
		{
			rp>>m_szError;
		}
	}
	else
	{
		ack.ret=1;
		strcpy(m_szError,"���ѷ������ʧ��");
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);

}

//�޸�������Ӧ��
void CIMProtocol::OnGroupInfoModAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	static XT_GROUPINFO_MOD_ACK ack;
	
	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len))
	{
		CRecvPacket rp(buff,buff_len);

		rp>>ack.ret;

		if ( ack.ret==0 )
		{
			rp>>ack.action>>ack.group_pid>>ack.group_id>>ack.name;
		}
		else
		{
			rp>>m_szError;
			strncpy(ack.error,m_szError,sizeof(ack.error)-1);
		}
	}
	else
	{
		return ;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
    
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnGetCListAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_GET_CLIST_ACK ack;

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,
		crypt,&crypt_len) )
	{
		return ;
	}

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret;		
	
	if (ack.ret==0 )
	{
		rp>>ack.nFamilyProp>>ack.nCount;

		for ( int i=0; i<ack.nCount; i++ )
			rp>>ack.nFamilyID[i];
	}
	else
	{
		rp>>m_szError;
	}

	//RemoveSendPacket(head.seq);
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnSetCInfoAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_SET_CINFO_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret>>ack.nFamilyID>>ack.nFamilyProp;

	if ( ack.ret!=0 )//�̶�Ⱥ
	{
		rp>>m_szError;
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();


	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnQueryFriendFamilyFlag(const RecvDataStruct & recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);


	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}


	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck,0,sizeof(buffAck));
	XT_USERFAMILYFLAG_GET_ACK & ack = *((XT_USERFAMILYFLAG_GET_ACK*)buffAck);


	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len) )
	{
		return ;
	}

	int i;
	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;
	
	if ( ack.ret==0 )
	{
		rp>>ack.count;

		assert(ack.count < 1000);
		if(ack.count > 1000)
			return;
		for ( i = 0; i< ack.count; i++ )
		{
			rp>>ack.JoinFlag[i].uID;
			rp>>ack.JoinFlag[i].flag;
		}
	}
	else
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

//�������Ӧ��
void CIMProtocol::OnClusterInviteAck(const RecvDataStruct & recvData)
{
	return;
}

//�����Ƿ���ȺӦ��
void CIMProtocol::OnClusterAgreeAck(const RecvDataStruct & recvData)
{
	return;
}

void CIMProtocol::OnGetCInfoAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_GET_CINFO_ACK ack;

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret;
	
	if ( ack.ret==0 )
	{
		rp>>ack.nFamilyID>>ack.nFamilyProp;

		if ( ack.nFamilyProp==PERM_FAMILY )//�̶�Ⱥ
		{
			rp>>ack.nCreatorID>>
				ack.nFamilyType>>ack.nFamilyClass>>ack.nFamilyFaceID
				>>ack.szFamilyFaceUrl>>ack.nJoinFlag
				>>ack.nViewFlag>>ack.szFamilyName>>ack.szFamilyDesc>>ack.szFamilyDomain>>ack.nNumberLimit>>ack.szFamilySign>>ack.szFamilyNotice;
		}
	}
	else
	{
		rp>>m_szError;
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnClusterAuthAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_CLUSTER_AUTH_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[MAX_RECV_BUFFERS];
	long  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,
		crypt,&crypt_len) )
	{
		return ;
	}

	CRecvPacket rp(crypt,crypt_len);
	rp>>ack.ret;

	if ( ack.ret==0 )
		rp>>ack.nFamilyID>>ack.nFamilyProp>>ack.nAction;
	else
		rp>>m_szError;

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
    this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnFriendBasicInfo(char * pData,int nLen)
{
	//static char buff[MAX_RECV_BUFFERS];
	static char buff[10000];//��ʱ�ģ�������
	LPXT_GET_FRIEND_BASICINFO_NOTIFY ack = (LPXT_GET_FRIEND_BASICINFO_NOTIFY)buff;
	CRecvPacket rp(pData,nLen);

	int MaxFrdInfoCnt = (MAX_RECV_BUFFERS-sizeof(ack->count))/sizeof(ack->FriendInfo[0]);
	rp>>ack->count;
	for ( int i=0; i<ack->count/* && i<=MaxFrdInfoCnt*/; ++i)
	{
		rp>>ack->FriendInfo[i].nID
			>>ack->FriendInfo[i].szHeadUrl
			>>ack->FriendInfo[i].nSexy
			>>ack->FriendInfo[i].szNickName
			>>ack->FriendInfo[i].szUserName
			>>ack->FriendInfo[i].szCommentName
			>>ack->FriendInfo[i].LoveFlag
			>>ack->FriendInfo[i].szUserSign
			>>ack->FriendInfo[i].nMobileStatus
			>>ack->FriendInfo[i].szGUID;///������ӪGUID��ʶ
		//nextPtr = (int)(&ack->FriendInfo[i+1]);
		//int y = (int )buff + MAX_RECV_BUFFERS;
	}

    this->SendNotify(MAKEWPARAM(0,CMD_GET_FRIEND_BASICINFO_ACK),(LPARAM)ack);
}

void CIMProtocol::OnClusterApplyAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_CLUSTER_APPLY_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[MAX_RECV_BUFFERS];
	int  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	char plain[1500];
	long plain_len;

	if ( !TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,plain,&plain_len) )
	{
		return ;
	}

	CRecvPacket plainPacket(plain,plain_len);
	plainPacket>>ack.ret;

	if ( ack.ret==0 )
		plainPacket>>ack.nFamilyID>>ack.nFamilyProp>>ack.nAction>>ack.nFlag;
	else
		plainPacket>>ack.nFamilyID>>ack.nFamilyProp>>ack.nAction>>m_szError;

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();

	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnCMembersSetAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_SET_CMEMBERS_ACK ack;
	memset(&ack,0,sizeof(ack));


	char crypt[1500];
	int  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	char plain[1500];
	long plain_len;
	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,plain,&plain_len);

	CRecvPacket plainPacket(plain,plain_len);
	plainPacket>>ack.ret>>ack.nFamilyID>>ack.nFamilyProp>>ack.nAction>>ack.nMemberID>>ack.nInviteID;

	if ( ack.ret!=0 )
		plainPacket>>m_szError;
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnCreateClusterAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_CREATE_CLUSTER_ACK ack;
	memset(&ack,0,sizeof(ack));

	char crypt[1500];
	int  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	char plain[1500];
	long plain_len;
	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,plain,&plain_len);

	CRecvPacket plainPacket(plain,plain_len);
	plainPacket>>ack.ret;

	if (ack.ret!=0 )
		plainPacket>>m_szError;
	else
		plainPacket>>ack.nFamilyID;

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();

	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnGroupInfoGetAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	uint8 ret;

	static char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
			(char *)m_SessionKey,
			buff,
			&buff_len))
	{
		return ;
	}
	else
	{
		CRecvPacket rp(buff,buff_len);
		rp>>ret;

		this->SendNotify(MAKEWPARAM(ret,head.cmd),(LPARAM)(buff+1));
	}

	if ( ret!=0 )
		this->SendNotify(MAKEWPARAM(ret,head.cmd),0);
}

void CIMProtocol::OnRecvSMSAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
			(char *)m_SessionKey,
			buff,
			&buff_len))
	{
		return ;
	}
	else
	{
		CRecvPacket rp(buff,buff_len);

		uint8 ret;
		uint8 ok_code;

		rp>>ret;

		if ( ret!=0 )
			rp>>m_szError;
		else
		{
			rp>>ok_code;

			if ( ok_code!=0 )
				rp>>m_szError;
		}

		this->SendNotify(MAKEWPARAM(ret,head.cmd),ok_code);
	}
}

void CIMProtocol::OnLoginAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_LOGIN_ACK ack;

	memset(&ack,0,sizeof(ack));

	MD5_CTX md5T;
	unsigned char digest[16];

	//char password[255] = {0};  //����ר������
	//sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",g_ImUserInfo.GetPassword().c_str());
	char password[MAX_PASSWORD_LEN + 1];
	sprintf(password,"%s",g_ImUserInfo.GetPassword().c_str());
 
	md5T.MD5Update ((unsigned char*)password,strlen(password) );
	md5T.MD5Final (digest);
 
	//CString sHex=ToHexString(digest,16);

	//md5T.MD5Update ((unsigned char*)(LPCSTR)sHex, sHex.GetLength());
	//md5T.MD5Final (digest);

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len-sizeof(XT_HEAD));

	char plain[MAX_RECV_BUFFERS];
	long plain_len;

	//
	// ��¼�ɹ����û������MD5 Digest��MD5 Digest��Ϊ��Կ����������
	// ��¼ʧ���������Կ��random key������������
	// �ͻ��˽���ʱ�ȿ��ǵ�¼�ɹ����
	//

	if(TEADecrypt(crypt,crypt_len,(char *)digest,
			plain,&plain_len) )
	{
		CRecvPacket plainPacket(plain,plain_len);

		plainPacket>>ack.ret;
		if(ack.ret == 0)
		{
			long data_len =	plainPacket.ReadPacket((char *)ack.session_key,16);

			if ( data_len!=16 )
			{
				IM_LOG("login ack,data_len!=16");
				return ;			
			}
			else
			{
				memcpy(m_SessionKey,ack.session_key,16);

				char key1[16], key2[16];

				plainPacket>>ack.user_id>>ack.merchant_id>>ack.publicip>>ack.version_flag;

				g_ImUserInfo.SetId(ack.user_id);
				g_ImUserInfo.merchant_id = ack.merchant_id;

				uint32 id = g_ImUserInfo.GetId();
				md5T.MD5Update ((unsigned char *)&id, sizeof(id));
				md5T.MD5Final ((unsigned char *)key1);

				md5T.MD5Update ((unsigned char*)m_SessionKey, sizeof(m_SessionKey));
				md5T.MD5Final ((unsigned char *)key2);

				for ( int i=0; i<16; i++ )
				{ 
					m_P2pKey[i] = key1[i]&key2[i];
				}

				extern CString g_strToken;
				string strCode = g_LocalData.Base64EncodeServer((const char*)ack.session_key);
				g_strToken = strCode.c_str();
				TRACE("����˵õ���token:%s\n", g_strToken);

				//�汾����url
				plainPacket>>ack.version_url;
				plainPacket>>ack.update_config;
				plainPacket>>ack.update_info;
				plainPacket>>ack.needChangeAccount;
				plainPacket>>ack.szGUID;
				plainPacket>>ack.username;
				plainPacket>>ack.login_time;
			}
		}
		
	}	
	else
	{
		// ��¼ʧ��
		if ( TEADecrypt(crypt,crypt_len,(char *)m_RandomKey,
				plain,&plain_len) )
		{
			CRecvPacket plainPacket(plain,plain_len);
			plainPacket>>ack.ret;	
			if(ack.ret == 2)
			{
				uint32 newServerIP;
				plainPacket>>newServerIP;
				ack.publicip = newServerIP;
			}
			else
			{
				plainPacket>>m_szError;
			}

		}
		else
		{
			strcpy( m_szError,"����ʧ��");
			IM_LOG("decryptwithkey error");
			return ;
		}
	}
	
	SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);


}
//
//void CIMProtocol::OnRegistAck(const RecvDataStruct &recvData)
//{
//	CRecvPacket packet((char *)recvData.data,recvData.data_len);
//	XT_HEAD head;
//	packet.GetXtHead(head);
//
//	if ( !IsValideServerPacket(recvData) )
//	{
//		return ;
//	}
//
//	static XT_REGIST_ACK ack;
//	memset(&ack,0,sizeof(ack));
//
//	char crypt[1500];
//	int  crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));
//
//	char plain[1500];
//	long plain_len;
//
//	if (!TEADecrypt(crypt,crypt_len,(char *)m_RandomKey,plain,&plain_len) )
//	{
//		ShowMessage("xTEADecryptWidthKey Error");
//		return ;
//	}
//
//	CRecvPacket plainPacket(plain,plain_len);
//	plainPacket>>ack.ret;
//
//	if (ack.ret!=0 )
//		plainPacket>>m_szError;
//	else
//		plainPacket>>ack.id;
//
//	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
//}

void CIMProtocol::OnRecvCofflineMsg(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	uint8  ret;

	static char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);

	rp>>ret;

	if ( ret==0 )
	{
		this->SendNotify(MAKEWPARAM(ret,head.cmd),(LPARAM)(buff+1));
	}
	else
	{
		this->SendNotify(MAKEWPARAM(ret,head.cmd),0);
	}
}

void CIMProtocol::OnCMembersGetAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);


	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	
	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck,0,sizeof(buffAck));
	XT_GET_CMEMBERS_ACK & ack = *((XT_GET_CMEMBERS_ACK*)buffAck);
	

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len) )
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;

	if ( ack.ret==0 )
	{
		rp>>ack.next_pos>>ack.nFamilyID>>ack.nFamilyProp>>ack.online_type;
		rp>>ack.return_count;

		for ( int i=0; i<ack.return_count; i++ )
		{
			FAMILYMEMBER_ACKINFO &info=ack.members[i];

			rp>>info.id>>info.prop>>info.status;

			if ( ack.nFamilyProp==TEMP_FAMILY )
			{
				info.prop = 0;
			}

			if ( info.id!=g_ImUserInfo.GetId() )
			{
				if (info.status==XTREAM_HIDE )
					info.status=XTREAM_OFFLINE;
			}
		}
	}
	else
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}
//////////////////////////////////////////////////////////////////////////////////////////
///////��ȡȺ��Ƭ��Ϣ
void CIMProtocol::OnGetProfileNameAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);


	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}


	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck,0,sizeof(buffAck));
	XT_PROFILENAME_GET_ACK & ack = *((XT_PROFILENAME_GET_ACK*)buffAck);


	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len) )
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;

	if ( ack.ret==0 )
	{
		rp>>ack.nFamilyID>>ack.nCount;

		for ( int i=0; i<ack.nCount; i++ )
		{
			XT_CMEM_IFNO   &info = ack.MemInfo[i];

			rp>>info.nMemID>>info.ProfileName;

			//if ( info.nMemID!=g_ImUserInfo.GetId() )
			//{
			//	if (info.status==XTREAM_HIDE )
			//		info.status=XTREAM_OFFLINE;
			//}
		}
	}
	else
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}
//////////////////////////////////////////////////////////////////////////////////////////
///////����Ⱥ��Ƭ
void CIMProtocol::OnSetProfileNameAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck,0,sizeof(buffAck));
	XT_PROFILENAME_SET_ACK & ack = *((XT_PROFILENAME_SET_ACK*)buffAck);


	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len) )
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;

	if ( ack.ret==0 )
	{
		rp>>ack.nFamilyID>>ack.nMemID>>ack.ProfileName;
	}
	else
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

//////////////////////////////////////////////////////////////////////////////////////////
///////����Ⱥ��Ϣ���շ�ʽ
void CIMProtocol::OnClusterMsgRecSetAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);


	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}


	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck,0,sizeof(buffAck));
	XT_CLUSTERMSG_RECV_SET_ACK & ack = *((XT_CLUSTERMSG_RECV_SET_ACK*)buffAck);


	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if ( !TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len) )
	{
		return ;
	}

	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;

	if ( ack.ret==0 )
	{
		rp>>ack.nFamilyID>>ack.nMemID>>ack.MsgRecType;
	}
	else
	{
		rp>>m_szError;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}
////////////////////////////////////////////////////////////////////////////////////////////

int CIMProtocol::SendGetSvrListReq(const tagSERVER_STATUSLIST_REQ& req)
{
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_SERVER_STATUSLIST_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);

	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	// ��д
	cryptPacket<<req.srvType<<req.startPos;
	// ��sessionKey����
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData,cryptData_len);
	// ���Ͱ��ĳ���
	sendData.data_len  = sendPacket.GetSize();

	this->SendNotify(MAKEWPARAM(0,CMD_SERVER_STATUSLIST_REQ),(LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);;
}

void CIMProtocol::OnGetServerListAck(const RecvDataStruct & recvData)
{

}


void CIMProtocol::OnUploadCPicAck(const RecvDataStruct & recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static XT_UPLOAD_CPIC_ACK ack;
	char buff[MAX_RECV_BUFFERS];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}
	CRecvPacket rp(buff,buff_len);
	rp>>ack.ret;
	if(ack.ret == 0)
	{
		rp	>>ack.nCid
			>>ack.nFileId
			>>ack.nPacketId;
	}
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnDownloadCPicAck(const RecvDataStruct & recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static XT_DOWNLOAD_CPIC_ACK ack;
	packet>>ack.ret;
	//if(ack.ret == 0)
	//{
		packet	>> ack.nCId
				>> ack.nSrcId
				>> ack.nFileId
				>> ack.nPacketId
				>> ack.nDataLen;
		ack.nDataLen = packet.ReadPacket(ack.data,ack.nDataLen);
	//}

	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

void CIMProtocol::OnHeadFileUploadAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_HEAD_FILE_UPLOAD_ACK ack;

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.ret;
	if(ack.ret == 0)
	{
		rp>>ack.type;
		if(ack.type == 1)
		{
			this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
		}
		else if(ack.type == 2)
		{
			rp>>ack.uploadFinish;
			if(ack.uploadFinish == 1)
			{
				rp>>ack.face_url;
				this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
			}
		}
	}
	else
	{
		rp>>m_szError;
	}
}

int CIMProtocol::SendDownloadCPicReq(const XT_DOWNLOAD_CPIC_REQ & req,int nRetryTimes)
{
	XT_HEAD xtHead;
	InitHead(xtHead,CMD_GET_CPIC_REQ,req.nCId,XT_HEAD::DEST_TYPE_FAMILY,m_wSequenceID++);

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket((char *)sendData.data);
	sendPacket.SetXtHead(xtHead);

	char cryptData[MAX_SEND_BUFFERS];
	long cryptData_len;
	// ��Ҫ���ܵ�����
	CSendPacket cryptPacket(cryptData);
	// ��д
	cryptPacket	<<req.nFileSid
				<<req.nFileId
				<<req.nPacketId;
	// ��sessionKey����
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// ������ļ�������д�뷢�Ͱ�
	sendPacket.WritePacket(cryptData,cryptData_len);
	// ���Ͱ��ĳ���
	sendData.data_len  = sendPacket.GetSize();
	sendData.time_interval = 3000;
	sendData.retry_times = nRetryTimes;
	return m_pUdpComm->SendDataTo(sendData);
}

//�޸ĸ���ǩ��
int CIMProtocol::SendReq(const char *pszUserLabel, uint32 uid)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket(sendData.data);

	XT_HEAD headSend;
	InitHead(headSend,CMD_MODIFYUNDERWRITE_REQ,0, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(headSend);

	char crypt[1500];
	long crypt_len;

	CSendPacket cryptPacket(crypt);
	cryptPacket<<pszUserLabel;

	TEAEncrypt(crypt,cryptPacket.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len      = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}


//�޸ĺ��ѹ�ע
int CIMProtocol::SendReq(const XT_MODIFY_FRIENDFLAG_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CMD_MODIFYFRIENDFLAG_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);


	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket sp(crypt);
	sp<<req.friendid<<req.flag;

	TEAEncrypt(crypt,sp.GetSize(),(char*)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}




//------------------------------------------------------
// ����������B�������ļ�����
//------------------------------------------------------
void CIMProtocol::RecvDownloadReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket rp((char * )data,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);

	if(nLen > 8)
	{
		int nDegug = 0;
	}

	/*char plain[200];
	long plain_len = 0;
	
	if ( !TEADecrypt(packet.GetCursor(),nLen,(char *)m_P2pKey,
		plain,&plain_len) )
	{
		TRACE("����������B���������ʧ��\n");
		return ;
	}*/

	//CRecvPacket rp(plain,plain_len);

	P2P_DOWNLOAD_REQ req;
	req.fid = -1;
	req.packetID = -1;

	rp>>req.fid
	  >>req.packetID;


	req.pt.ip = recvData.src_ip;
	req.pt.port = recvData.port;
	req.pt.uid = head.sid;
	req.seq = head.seq;
	req.pt.type = recvData.src_type;

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);

}

//------------------------------------------------------
// ���������ṩ��A������
//------------------------------------------------------
void CIMProtocol::RecvDownloadAck(const char * data,int nLen,const RecvDataStruct &recvData)
{
	LPXT_HEAD pHead = (LPXT_HEAD)data;
	data += sizeof(XT_HEAD);
	nLen -= sizeof(XT_HEAD);
	CRecvPacket rp((char *)data,nLen);


	if ( (pHead->flag < 0xf0 && pHead->flag > 0xf3) )
	{
		TRACE("�յ��Ƿ����ذ�\n");
		return;
	}

	P2P_DOWNLOAD_ACK ack;
	ack.len = 0;
	rp>>ack.nRt;
	rp	>>ack.fid
		>>ack.packetID
		>>ack.len;
	if(ack.len != 0)
	{
		rp.ReadPacket(ack.data,ack.len);
	}
	

	ack.pt.ip = recvData.src_ip;
	ack.pt.port = recvData.port;
	ack.pt.uid = pHead->sid;
	ack.pt.type = recvData.src_type;

	SendNotify(MAKEWPARAM(0,pHead->cmd),(LPARAM)&ack);

}

//------------------------------------------------------
// ������ֹ���������
//------------------------------------------------------
void CIMProtocol::RecvAbortReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket rp((char * )data,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);

	P2P_ABORT_TRAN_REQ req;

	//req.pt.ip = recvData.src_ip;
	//req.pt.port = recvData.port;
	//req.pt.uid = head.sid;
	req.friendId = head.sid;
	//req.pt.type = recvData.src_type;


	/*char plain[200];
	long plain_len = 0;

	if ( !TEADecrypt(packet.GetCursor(),nLen,(char *)m_P2pKey,
		plain,&plain_len) )
	{
		return ;
	}

	CRecvPacket rp(plain,plain_len);*/
	
	rp>>req.type
	  >>req.fid;


	// �ظ�ack

	SendDataStruct sendData;
	InitP2PSendData(sendData,recvData.src_ip,recvData.port);
	XT_HEAD hAck;
	char crypt[200];
	long crypt_len = 0;
	char plain[100];
	CSendPacket sp(sendData.data);
	
	// ͨ��������������ת��Ӧ��
	InitHead(hAck,CMD_ROUTERPACKET_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag = XT_HEAD::FLAG_CNTACK;
	sp.SetXtHead(hAck);

	CSendPacket spAck(plain);
	spAck<<CMD_ABORT_TRAN_ACK;
	InitHead(hAck,CMD_ABORT_TRAN_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	spAck.SetXtHead(hAck);
	spAck<<(uint8)0;

	if(TEAEncrypt(plain,spAck.GetSize(),(char*)m_SessionKey,crypt,&crypt_len) == FALSE)
	{
		TRACE("����ʧ��\n");
	}
	sp.WritePacket(crypt,crypt_len);

	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);
}


/////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------
// ������ɴ��������
//------------------------------------------------------
void CIMProtocol::RecvFinishReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("A�յ�B�������֪ͨ\n");
	XT_HEAD head;
	CRecvPacket rp((char *)data,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);

	P2P_FIN_TRAN_REQ req;
	req.friendId = head.sid;
	
	/*req.pt.ip = recvData.src_ip;
	req.pt.port = recvData.port;
	req.pt.uid = head.sid;
	req.pt.type = recvData.src_type;*/

	/*char plain[200];
	long plain_len = 0;

	if ( !TEADecrypt(packet.GetCursor(),nLen,(char *)m_P2pKey,
		plain,&plain_len) )
	{
		TRACE("p2p������ɽ���ʧ��\n");
		return ;
	}*/

	//CRecvPacket rp(plain,plain_len);

	rp >> req.fid;

	// �ظ�ack
	SendDataStruct sendData;
	CSendPacket sp(sendData.data);
	XT_HEAD hAck;
	if(recvData.src_type == P2P_ENDPOINT::TRANS)
	{
		InitHead(hAck,CMD_ROUTERUSERACKPACKET_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
		hAck.flag = XT_HEAD::FLAG_CNTACK;
		sp.SetXtHead(hAck);
		sp<<CMD_FIN_TRAN_ACK;
	}
	InitP2PSendData(sendData,recvData.src_ip,recvData.port);
	InitHead(hAck,CMD_FIN_TRAN_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	
	sp.SetXtHead(hAck);
	sp<<(uint8)0;
	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);


}

//------------------------------------------------------
// ���շ�����A�������ļ�Ӧ��
//------------------------------------------------------
void CIMProtocol::RecvDownloadFileAck(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("B�յ�A�����ļ�Ӧ��\n");
	XT_HEAD head;
	CRecvPacket rp((char *)data,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);
	
	P2P_DOWNLOAD_FILE_ACK ack;
	
	rp >>ack.nRt;
		
	if(ack.nRt)
	{
		rp>>ack.szErr;
	}
	else
	{
		rp>>ack.nFID;
	}
	
	//ack.pt.ip = recvData.src_ip;
	//ack.pt.port = recvData.port;
	//ack.pt.uid = head.sid;
	//ack.pt.type = recvData.src_type;
	ack.nFriendId = head.sid;

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);

}

//------------------------------------------------------
// ����������B�������ļ�����
//------------------------------------------------------
void CIMProtocol::RecvDownloadFileReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("A�յ�B�����ļ�����\n");
	XT_HEAD head;
	CRecvPacket rp((char * )data,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);
	
	P2P_DOWNLOAD_FILE_REQ req;
	rp  >>req.fid
		>>req.flag
		>>req.startPos
		>>req.packetSize;
	
	req.friendId = head.sid;
	/*req.pt.ip = recvData.src_ip;
	req.pt.port = recvData.port;
	req.pt.uid = head.sid;
	req.pt.type = recvData.src_type;*/

	
	SendDataStruct sendData;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);
	CSendPacket sp(sendData.data);
	XT_HEAD hAck;
	InitHead(hAck,CMD_ROUTERPACKET_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag = XT_HEAD::FLAG_CNTACK;
	sp.SetXtHead(hAck);

	char plain[200];
	char crypt[200];
	long crypt_len = 0;
	CSendPacket plainP(plain);
	plainP<<CMD_DOWNLOAD_FILE_ACK;
	InitHead(hAck,CMD_DOWNLOAD_FILE_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	plainP.SetXtHead(hAck);
	plainP	<<(uint8)0
			<< req.fid;
		
	TEAEncrypt(plain,plainP.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);
	sp.WritePacket(crypt,crypt_len);
	
	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);
}

//------------------------------------------------------
// �յ�������A�ķ����ļ�����
//------------------------------------------------------
void CIMProtocol::RecvSendFileReq(const char * pData,int nLen,const RecvDataStruct &recvData)
{
	TRACE("B�յ�A�����ļ�������\n");
	XT_HEAD head;
	CRecvPacket rp((char *)pData,nLen);
	rp.GetXtHead(head);
	nLen -= sizeof(XT_HEAD);

	P2P_SENDFILE_REQ data;
	
	rp	>>data.fid
		>>data.size.sizel
		>>data.size.sizeh
		>>data.time
		>>data.tranIp
		>>data.tranPort
		>>data.name;

	data.friendId = head.sid;
	//data.pt.ip = recvData.src_ip;
	//data.pt.port = recvData.port;
	//data.pt.uid = head.sid;
	//data.pt.type = recvData.src_type;

	SendDataStruct sendData;
	CSendPacket sp(sendData.data);
	XT_HEAD hAck;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);
	
	InitHead(hAck,CMD_ROUTERPACKET_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag = XT_HEAD::FLAG_CNTACK;
	sp.SetXtHead(hAck);
	char plain[200];
	char crypt[200];
	long crypt_len = 0;
	CSendPacket plainP(plain);
	plainP<<CMD_SEND_FILE_ACK;
	InitHead(hAck,CMD_SEND_FILE_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	plainP.SetXtHead(hAck);
	plainP<<(uint8)0;

	TEAEncrypt(plain,plainP.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len);
	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();

	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&data);
}


//------------------------------------------------------
// ������A�����ļ�����
//------------------------------------------------------
int CIMProtocol::SendSendFileReq(const P2P_SENDFILE_REQ & req)
{
	TRACE("A��B�����ļ�����%d\n",req.fid);
	// �����ļ�����

	SendDataStruct sendData;
	XT_HEAD head;
	uint32 seq = m_wSequenceID++;

	CSendPacket sendPacket(sendData.data);
	
	
	// ҵ�������������ת����
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);
	InitHead(head,CMD_ROUTERPACKET_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	//head.flag = XT_HEAD::FLAG_SRVACTION;
	sendPacket.SetXtHead(head);

	

	char crypt[1500];
	long crypt_len;
	char plain[1500];
	long plain_len = 0;

	/*FRIEND_INFO * pFriend;
	pFriend = GetFriendInfo(req.pt.uid);

	if(pFriend == NULL)
	{
		TRACE("û���ҵ������ļ��ĺ���\n");
		return -1;
	}*/
	
	CSendPacket spPlain(plain);

	spPlain<<CMD_SEND_FILE_REQ;
	InitHead(head,CMD_SEND_FILE_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	spPlain.SetXtHead(head);

	spPlain	<<req.fid
			<<req.size.sizel
			<<req.size.sizeh
			<<req.time
			<<req.tranIp
			<<req.tranPort
			<<req.name;

	// ����
	if(TEAEncrypt(spPlain.GetData(),spPlain.GetSize(),(char*)m_SessionKey,crypt,&crypt_len) == false)
	{
		return -1;
	}

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
	sendData.vlink = req.fid;
	sendData.time_interval *= 2;

	return m_pUdpComm->SendDataTo(sendData);

}


//------------------------------------------------------
// ������B�����ļ���������
//------------------------------------------------------
int CIMProtocol::SendDownloadFileReq(const P2P_DOWNLOAD_FILE_REQ & req)
{
	TRACE("B����A����ĳ�ļ�%d\n",req.fid);
	SendDataStruct sendData;
	XT_HEAD head;
	CSendPacket sendPacket(sendData.data);
	int seq = m_wSequenceID++;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);

	
	// ҵ�������������ת
	InitHead(head,CMD_ROUTERPACKET_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	//head.flag = XT_HEAD::FLAG_SRVACTION;
	sendPacket.SetXtHead(head);

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;

	CSendPacket spPlain(plain);

	spPlain<<CMD_DOWNLOAD_FILE_REQ;
	InitHead(head,CMD_DOWNLOAD_FILE_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	spPlain.SetXtHead(head);

	spPlain	<<req.fid
			<<req.flag
			<<req.startPos
			<<req.packetSize
			;
	
	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len
	);

	sendPacket.WritePacket(crypt,crypt_len);
	sendData.data_len  = sendPacket.GetSize();
	sendData.vlink = req.fid;
	sendData.time_interval *= 2;

	return m_pUdpComm->SendDataTo(sendData);
}


//
////------------------------------------------------------
//// ������A�ṩ��������
////------------------------------------------------------
//int CIMProtocol::SendDownloadAck(const P2P_DOWNLOAD_ACK & ack)
//{
//
//	SendDataStruct sendData;
//	InitP2PSendData(sendData,ack.pt.ip,ack.pt.port);
//	CSendPacket sp;
//	sp.SetData(sendData.data,sizeof(sendData.data));
//	XT_HEAD head;
//
//	if(ack.pt.type == P2P_ENDPOINT::TRANS)
//	{
//		InitHead(head,CMD_ROUTERUSERACKPACKET_ACK,ack.pt.uid,XT_HEAD::DEST_TYPE_USER,ack.seq);
//		head.flag = XT_HEAD::FLAG_CNTACK;
//		sp.SetXtHead(head);
//		sp<<CMD_DOWNLOAD_ACK;
//	}
//	InitHead(head,CMD_DOWNLOAD_ACK,ack.pt.uid,XT_HEAD::DEST_TYPE_USER,ack.seq);
//	head.flag  = XT_HEAD::FLAG_CNTACK;
//	sp.SetXtHead(head);
//	sp <<ack.nRt
//		<<ack.fid
//		<<ack.packetID
//		<<ack.len;
//	sp.WritePacket(ack.data,ack.len);
//	sendData.data_len = sp.GetSize();
//	sendData.retry_times = 0;
//	sendData.vlink = ack.fid;
//
//	return m_pUdpComm->SendDataTo(sendData);
//}

////------------------------------------------------------
//// ������B����������������
////------------------------------------------------------
//int CIMProtocol::SendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd)
//{
//	TRACE("B�����ļ�%d,%d\n",req.fid,req.packetID);
//	SendDataStruct sendData;
//	InitP2PSendData(sendData,req.pt.ip,req.pt.port);
//	CSendPacket sp(sendData.data);
//	XT_HEAD head;
//	int seq = m_wSequenceID++;
//
//	if(req.pt.type == P2P_ENDPOINT::TRANS)
//	{
//		InitHead(head,CMD_ROUTERUSERPACKET_REQ,req.pt.uid,XT_HEAD::DEST_TYPE_USER,seq);
//		//head.flag = XT_HEAD::FLAG_SRVACTION;
//		sp.SetXtHead(head);
//		sp<<CMD_DOWNLOAD_REQ;
//		sendData.time_interval = 3000;
//		sendData.retry_times = 7;	// 103s
//	}
//	else
//	{
//		sendData.time_interval = 500;
//		sendData.retry_times = 12;	// 98s 
//	}
//	
//	InitHead(head,CMD_DOWNLOAD_REQ,req.pt.uid,XT_HEAD::DEST_TYPE_USER,seq);
//	sp.SetXtHead(head);
//
//	//pWnd->SetItem(req.packetID,head.seq,CP2PRecvWnd::SENDING);
//
//	//// �öԷ���p2p�Ự��������
//	//FRIEND_INFO * pFriend = GetFriendInfo(req.pt.uid);
//	//if(pFriend == NULL)
//	//	return -1;
//	
//	/*char crypt[200];
//	long crypt_len = 0;
//	char plain[200];
//	long plain_len = 0;*/
//	
//	//CSendPacket spPlain(plain);
//
//	sp	<<req.fid
//		<<req.packetID;
//
//	/*xTEAEncryptWithKey(
//		spPlain.GetData(),
//		spPlain.GetSize(),
//		(char *)pFriend->key,
//		crypt,&crypt_len
//		);
//
//	sp.WritePacket(crypt,crypt_len);*/
//	sendData.data_len = sp.GetSize();
//	//if(sendData.data_len > 28)
//	//{
//	//	int nBebug = 0;
//	//	::MessageBox(NULL,"�������󳬳�","",0);
//	//}
//	sendData.vlink = req.fid;
//	
//
//	return m_pUdpComm->SendDataTo(sendData);
//}
//
////------------------------------------------------------
//// ������B�����ش�������������
////------------------------------------------------------
//int CIMProtocol::QuickResendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd)
//{
//	TRACE("quick resend%d\n",req.packetID);
//	SendDataStruct sendData;
//	InitP2PSendData(sendData,req.pt.ip,req.pt.port);
//	CSendPacket sp(sendData.data);
//	XT_HEAD head;
//
//	if(req.pt.type == P2P_ENDPOINT::TRANS)
//	{
//		InitHead(head,CMD_ROUTERUSERPACKET_REQ,req.pt.uid,XT_HEAD::DEST_TYPE_USER,req.seq);
//		//head.flag = XT_HEAD::FLAG_SRVACTION;
//		sp.SetXtHead(head);
//		sp<<CMD_DOWNLOAD_REQ;	
//	}
//
//	InitHead(head,CMD_DOWNLOAD_REQ,req.pt.uid,XT_HEAD::DEST_TYPE_USER,req.seq);
//	sp.SetXtHead(head);
//
//	pWnd->SetItem(req.packetID,req.seq,CP2PRecvWnd::RESEND);
//
//	//// �öԷ���p2p�Ự��������
//	//FRIEND_INFO * pFriend = GetFriendInfo(req.pt.uid);
//	//if(pFriend == NULL)
//	//	return -1;
//
//	/*char crypt[200];
//	long crypt_len = 0;
//	char plain[200];
//	long plain_len = 0;*/
//
//	//CSendPacket spPlain(plain);
//
//	//spPlain <<req.fid
//	//<<req.packetID;
//	sp	<<req.fid
//		<<req.packetID;
//	//sp.WritePacket(crypt,crypt_len);
//	sendData.data_len = sp.GetSize();
//	sendData.retry_times = 0;
//	sendData.vlink = req.fid;
//
//	return m_pUdpComm->SendDataTo(sendData);
//}


//------------------------------------------------------
// ˫��������ֹ��������
//------------------------------------------------------
int CIMProtocol::SendAbortTranReq(const P2P_ABORT_TRAN_REQ & req)
{
	TRACE("������ֹ����֪ͨ\n");
	SendDataStruct sendData;
	// ͨ��������������ת
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;

	InitHead(head,CMD_ROUTERPACKET_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	//head.flag = XT_HEAD::FLAG_SRVACTION;
	sp.SetXtHead(head);

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;

	CSendPacket spPlain(plain);

	spPlain<<CMD_ABORT_TRAN_REQ;
	InitHead(head,CMD_ABORT_TRAN_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	spPlain.SetXtHead(head);


	spPlain	<<req.type
			<<req.fid;

	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len
	);

	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	sendData.vlink = req.fid;
	sendData.time_interval *= 2;

	return m_pUdpComm->SendDataTo(sendData);
}

//------------------------------------------------------
// ������B������ɴ�������
//------------------------------------------------------
int CIMProtocol::SendFinishTranReq(const P2P_FIN_TRAN_REQ & req)
{
	TRACE("B�����������֪ͨ\n");
	SendDataStruct sendData;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;
	
	InitHead(head,CMD_ROUTERUSERPACKET_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	head.flag = XT_HEAD::FLAG_SRVACTION;
	sp.SetXtHead(head);
	sp<<CMD_FIN_TRAN_REQ;
	
	InitHead(head,CMD_FIN_TRAN_REQ,req.friendId,XT_HEAD::DEST_TYPE_USER,seq);
	sp.SetXtHead(head);


	// �öԷ���p2p�Ự��������
	/*FRIEND_INFO * pFriend = GetFriendInfo(req.pt.uid);
	if(pFriend == NULL)
		return -1;

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;*/

	//CSendPacket spPlain(plain);

	//spPlain<<req.fid;
	sp<<req.fid;

	//TEAEncrypt(
	//	spPlain.GetData(),
	//	spPlain.GetSize(),
	//	(char *)pFriend->key,
	//	crypt,&crypt_len
	//);

	//sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	sendData.vlink = req.fid;

	return m_pUdpComm->SendDataTo(sendData);
}

//------------------------------------------------------
// �յ�����������������ת������
//------------------------------------------------------
void CIMProtocol::RecvServerEncryptTran(const RecvDataStruct & recvData)
{

	// �õ���������
	const char * pCryptData = recvData.data + sizeof(XT_HEAD);
	long crypt_len = recvData.data_len - sizeof(XT_HEAD);

	char plain[1500];
	long plain_len = 0;

	if ( !TEADecrypt(pCryptData,crypt_len,(char *)m_SessionKey,
		plain,&plain_len) )
	{
		TRACE("������ת���������ݰ�����ʧ��\n");
		return;
	}

	CRecvPacket packet(plain,plain_len);

	WORD cmd;
	packet>>cmd;

	char * pData = (char*)packet.GetCursor();
	int nLen = plain_len - 2;

	switch(cmd)
	{
	case CMD_SEND_FILE_REQ:	
		RecvSendFileReq(pData,nLen,recvData);
		break;
	case CMD_DOWNLOAD_FILE_REQ:
		RecvDownloadFileReq(pData,nLen,recvData);
		break;
	case CMD_DOWNLOAD_FILE_ACK:
		RecvDownloadFileAck(pData,nLen,recvData);
		break;
	case CMD_P2P_NOTIFY_LOGIN_REQ:
		RecvNotifyLoginReq(pData,nLen,recvData);
		break;
	case CMD_P2P_NOTIFY_LOGIN_ACK:
		RecvNotifyLoginAck(pData,nLen,recvData);
		break;
	case CMD_ABORT_TRAN_REQ:
		RecvAbortReq(pData,nLen,recvData);
		break;
	}
}

//------------------------------------------------------
// �յ�����������͸����ת������
//------------------------------------------------------
void CIMProtocol::RecvServerTran(const RecvDataStruct & recvData)
{

	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);
	WORD cmd;
	packet>>cmd;

	char * pData = (char*)packet.GetCursor();
	int nLen = recvData.data_len - sizeof(XT_HEAD) - 2;

	switch(cmd)
	{
	/*case CMD_SEND_FILE_REQ:	
		RecvSendFileReq(pData,nLen,recvData);
		break;
	case CMD_DOWNLOAD_FILE_REQ:
		RecvDownloadFileReq(pData,nLen,recvData);
		break;
	case CMD_DOWNLOAD_FILE_ACK:
		RecvDownloadFileAck(pData,nLen,recvData);
		break;*/
	case CMD_DOWNLOAD_REQ:
		RecvDownloadReq(pData,nLen,recvData);
		break;
	case CMD_DOWNLOAD_ACK:
		RecvDownloadAck(pData,nLen,recvData);
		break;
	case CMD_FIN_TRAN_REQ:
		RecvFinishReq(pData,nLen,recvData);
		break;
	
	}

}
/*
//------------------------------------------------------
// A����ת����������Ự
//------------------------------------------------------
int CIMProtocol::C2TSendLogin(const XT_TRANS_LOGIN_REQ & req)
{
	SendDataStruct sendData;
	InitP2PSendData(sendData,m_dwTranServerIP,m_wTranServerPort);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;
	InitHead(head,CMD_TRANS_LOGIN_REQ,0,XT_HEAD::DEST_TYPE_TRANSMIT,seq);
	sp.SetXtHead(head);

	// ��p2p�Ự��������

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;

	CSendPacket spPlain(plain);
	spPlain	<<req.data;

	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len
		);

	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

//------------------------------------------------------
// A����ת�����������رջỰ
//------------------------------------------------------
int CIMProtocol::C2TSendCloseSession(const XT_TRANS_CLOSE_SEESION_REQ & req)
{
	SendDataStruct sendData;
	InitP2PSendData(sendData,m_dwTranServerIP,m_wTranServerPort);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;
	InitHead(head,CMD_TRANS_SENDERCLOSE_REQ,0,XT_HEAD::DEST_TYPE_TRANSMIT,seq);
	sp.SetXtHead(head);

	// ��p2p�Ự��������

	char crypt[1500];
	long crypt_len = 0;
	char plain[1500];
	long plain_len = 0;

	CSendPacket spPlain(plain);
	int i;
	assert(req.count < 1500/4);
	spPlain<<req.count;
	for(i = 0; i < req.count; i++)
	{
		spPlain	<<req.sessionID[i];
	}
	
	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char *)m_SessionKey,
		crypt,&crypt_len
		);

	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	
	return m_pUdpComm->SendDataTo(sendData);
}


//------------------------------------------------------
// B��¼��ת������
//------------------------------------------------------
int CIMProtocol::C2TSendRecvLogin(const XT_TRANS_RECVLOGIN_REQ & req)
{
	TRACE("B��¼��ת������\n");
	SendDataStruct sendData;
	InitP2PSendData(sendData,req.ip,req.port);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;
	InitHead(head,CMD_TRANS_RECVLOGIN_REQ,req.uid,XT_HEAD::DEST_TYPE_TRANSMIT,seq);
	sp.SetXtHead(head);

	// �ûỰ��������

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;

	CSendPacket spPlain(plain);
	
	spPlain	<<req.sessionid
			<<req.action;

	

	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char*)m_SessionKey,
		crypt,&crypt_len
		);

	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}
*/
//------------------------------------------------------
// ֪ͨB��¼��ת������
//------------------------------------------------------
void CIMProtocol::P2SSendNotifyLogin(const XT_TRANS_NOTIFY_LOGIN_REQ & req)
{
	SendDataStruct sendData;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;

	
	InitHead(head,CMD_ROUTERPACKET_REQ,req.uid,XT_HEAD::DEST_TYPE_USER,seq);
	//head.flag = XT_HEAD::FLAG_SRVACTION;
	sp.SetXtHead(head);

	char crypt[200];
	long crypt_len = 0;
	char plain[200];
	long plain_len = 0;

	CSendPacket spPlain(plain);

	spPlain<<CMD_P2P_NOTIFY_LOGIN_REQ;
	InitHead(head,CMD_P2P_NOTIFY_LOGIN_REQ,req.uid,XT_HEAD::DEST_TYPE_USER,seq);
	spPlain.SetXtHead(head);

	spPlain <<req.sessionID
			<<req.linkType
			<<req.transSrvIP
			<<req.port;

	spPlain.WritePacket((const char *)req.check,16);

	TEAEncrypt(
		spPlain.GetData(),
		spPlain.GetSize(),
		(char *)m_SessionKey,
		crypt,
		&crypt_len
	);

	sp.WritePacket(crypt,crypt_len);
	sendData.data_len = sp.GetSize();
	//sendData.vlink = req.fid;

	m_pUdpComm->SendDataTo(sendData);

}

//------------------------------------------------------
// ������A��¼��ת������Ӧ��
//------------------------------------------------------
void CIMProtocol::RecvTransLoginAck(const RecvDataStruct & recvData)
{
	LPXT_HEAD pHead = (LPXT_HEAD)recvData.data;
	const char * pCrypt = recvData.data + sizeof(XT_HEAD);
	long crypt_len = recvData.data_len - sizeof(XT_HEAD);

	char plain[1500];
	long plain_len = 0;
	if(TEADecrypt(pCrypt,crypt_len,(char *)m_SessionKey,plain,&plain_len) == FALSE)
	{
		TRACE("err,������A��¼��ת������Ӧ�����ʧ��\n");
		return ;
	}
	CRecvPacket plainPacket(plain,plain_len);
	XT_TRANS_LOGIN_ACK ack;
	plainPacket>>ack.ret;
	if ( ack.ret !=0 )
	{
		plainPacket>>m_szError;
	}
	else
	{
		plainPacket >>ack.sessionid;
					//>>ack.sessioncode;
	}
	ack.ip = recvData.src_ip;
	ack.port = recvData.port;
	this->SendNotify(MAKEWPARAM(ack.ret,pHead->cmd),(LPARAM)&ack);
}

//------------------------------------------------------
// ������A�յ��ǳ���ת��������Ӧ��
//------------------------------------------------------
void CIMProtocol::RecvTransLogoutAck(const RecvDataStruct & recvData)
{
	// ����Ӧ
}

//------------------------------------------------------
// �յ������ߵ�¼/ע����ת��������Ӧ��
//------------------------------------------------------
void CIMProtocol::RecvTransRecvLoginAck(const RecvDataStruct & recvData)
{
	LPXT_HEAD pHead = (LPXT_HEAD)recvData.data;
	int nLen = recvData.data_len - sizeof(XT_HEAD);
	CRecvPacket packet((char *)recvData.data + sizeof(XT_HEAD),nLen);

	char crypt[1500];
	int  crypt_len = packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	char plain[1500];
	long plain_len;
	if(TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,plain,&plain_len) == FALSE)
	{
		TRACE("�����ߵ�¼/ע����ת��������Ӧ�����ʧ��\n");
		return;
	};

	CRecvPacket plainPacket(plain,plain_len);
	XT_TRANS_RECVLOGIN_ACK ack;

	plainPacket>>ack.ret;
	if(ack.ret == 0)
	{
		plainPacket >>ack.sessionid
					>>ack.action;
	}
	else
	{
		plainPacket>>m_szError;
	}
	ack.ip = recvData.src_ip;
	ack.port = recvData.port;
	this->SendNotify(MAKEWPARAM(ack.ret,pHead->cmd),(LPARAM)&ack);
}


//------------------------------------------------------
// �յ�������A�����¼��ת������������
//------------------------------------------------------
void CIMProtocol::RecvNotifyLoginReq(const char * data,int nDataLen,const RecvDataStruct & recvData)
{
	TRACE("B�յ�A֪ͨ��¼��ת������\n");
	LPXT_HEAD pHead = (LPXT_HEAD)data;
	int nLen = nDataLen - sizeof(XT_HEAD);
	CRecvPacket rp((char *)data + sizeof(XT_HEAD),nLen);

	//
	// �ظ�������AӦ��
	//

	SendDataStruct sendData;
	InitP2PSendData(sendData,recvData.src_ip,recvData.port);
	CSendPacket sp(sendData.data);
	XT_HEAD hAck;

	InitHead(hAck,CMD_ROUTERPACKET_ACK,pHead->sid,XT_HEAD::DEST_TYPE_USER,pHead->seq);
	hAck.flag = XT_HEAD::FLAG_CNTACK;
	sp.SetXtHead(hAck);

	char plainAck[200];
	CSendPacket spPlain(plainAck);
	spPlain<<CMD_P2P_NOTIFY_LOGIN_ACK;	
	InitHead(hAck,CMD_P2P_NOTIFY_LOGIN_ACK,pHead->sid,XT_HEAD::DEST_TYPE_USER,pHead->seq);
	hAck.flag  = XT_HEAD::FLAG_CNTACK;
	spPlain.SetXtHead(hAck);
	spPlain<<(uint8)0;
	char crypt[1500];
	long crypt_len = 0;

	if(TEAEncrypt(plainAck,spPlain.GetSize(),(char*)m_SessionKey,crypt,&crypt_len) == FALSE)
	{
		TRACE("֪ͨ��¼��ת��������Ӧ�����ʧ��\n");
		return;
	}
	sp.WritePacket(crypt,crypt_len);

	// Ӧ�������Ҫ��֤�Ƿ�ʱ
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	XT_TRANS_NOTIFY_LOGIN_REQ req;
	
	rp>>req.sessionID
	  >>req.linkType
	  >>req.transSrvIP
	  >>req.port;
	req.sid = pHead->sid;
	rp.ReadPacket((char *)req.check,16);

	SendNotify(MAKEWPARAM(0,pHead->cmd),(LPARAM)&req);
}

//------------------------------------------------------
// �յ�������B��֪ͨ��¼Ӧ��
//------------------------------------------------------
void CIMProtocol::RecvNotifyLoginAck(const char * data,int nLen,const RecvDataStruct & recvData)
{
	// ����Ӧ
}


//------------------------------------------------------
// ���ܷ���ʧ�ܵķ�����������ת����
//------------------------------------------------------
void CIMProtocol::DecryptFailedServerTran(WORD wRet,const SendDataStruct & sendData)
{
	uint16 cmd;
	const char * pSubData =sendData.data +sizeof(XT_HEAD);
	int nSubLen = sendData.data_len - sizeof(XT_HEAD);
	char plain[1500];
	long plain_len = 0;
	if(TEADecrypt(pSubData,nSubLen,(char*)m_SessionKey,plain,&plain_len) == FALSE)
	{
		TRACE("���ܷ�������ת���ݽ���ʧ��\n");
		return;
	}
	cmd = *((uint16*)(plain));
	SendDataStruct subData;
	subData.data_len = plain_len - 2;
	memcpy(subData.data,plain+2,subData.data_len);
	subData.dest_ip = sendData.dest_ip;
	subData.port = sendData.port;
	subData.vlink = sendData.vlink;
	g_pProtocol->SendNotify(MAKEWPARAM(wRet,cmd+1),(LPARAM)&subData);
}

//------------------------------------------------------
// ���ͷ���ʧ�ܵķ�����͸����ת����
//------------------------------------------------------
void CIMProtocol::ParseFailedServerTran(WORD wRet,const SendDataStruct & sendData)
{
	uint16 cmd;
	const char * pSubData = sendData.data +sizeof(XT_HEAD);
	cmd = *((uint16*)(pSubData));
	pSubData += 2;
	SendDataStruct subData;
	subData.data_len = sendData.data_len - sizeof(XT_HEAD) - 2;
	memcpy(subData.data,pSubData,subData.data_len);
	subData.dest_ip = sendData.dest_ip;
	subData.port = sendData.port;
	subData.vlink = sendData.vlink;
	g_pProtocol->SendNotify(MAKEWPARAM(wRet,cmd+1),(LPARAM)&subData);
}

// ���º�̨�������ϵ�ͻ�����Ϣ��������Ϣ
int CIMProtocol::SendReq(const XT_UPDATE_NEAR_COMM_CUST_INFO_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_UPDATE_NEAR_COMM_CUST_INFO_REQ, req.uiBusinessID, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.uiBusinessID;
	cp << req.uiCommCount;
	for (int i = 0; i < req.uiCommCount; ++i)
	{
		cp << req.CommInfo[i].szCommAccount;
		cp << req.CommInfo[i].szCommTime;
	}

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);
	sendData.retry_times = 0;
	sendData.data_len = sendPacket.GetSize();

	//Add2SendList(sendData);
	return m_pUdpComm->SendDataTo(sendData);
}

// ���º�̨�������ϵ�ͻ�����Ϣ��������Ϣ
void CIMProtocol::OnRecvUpdateCommInfoAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_UPDATE_NEAR_COMM_CUST_INFO_ACK & ack = *((XT_UPDATE_NEAR_COMM_CUST_INFO_ACK*)buffAck);
	
	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
}

// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
int CIMProtocol::SendReq(const XT_GET_NEAR_COMM_CUST_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_GET_NEAR_COMM_CUST_REQ, req.uiBusinessID, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.uiBusinessID;
	cp << req.szBgnTime;
	cp << req.szEndTime;
	cp << req.iMaxCustCount;
	cp << req.iStartPos;
	cp << req.iNeedCustCount;
	cp << req.bDirect;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
void CIMProtocol::OnRecvGetNearCommCustAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_GET_NEAR_COMM_CUST_ACK & ack = *((XT_GET_NEAR_COMM_CUST_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	// ������
	uint32 ret = 0;
	rp >> ret;
	if (ret != 0)
	{	// ʧ��
		XT_GET_NEAR_COMM_CUST_ACK ack;
		ack.ret = ret;

		m_wLastAckSequenceID = head.seq;
		s_nHeartBeat = GetTickCount();
		this->SendNotify(MAKEWPARAM(ret, head.cmd), (LPARAM)&ack);
	}
	else
	{
		uint32  uiBusinessID;				// �̻�ID
		uint8   bMask;
		uint32  actualCount;				// ʵ�ʷ��Ϳͻ�����
		rp >> uiBusinessID;
		rp >> bMask;
		rp >> actualCount;

		if (bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_ALL_CUST_COUNT)
		{
			XT_GET_NEAR_COMM_CUST_ACK ack;
			ack.ret = ret;
			ack.uiBusinessID = uiBusinessID;
			ack.bMask = bMask;
			ack.actualCount = actualCount;

			m_wLastAckSequenceID = head.seq;
			s_nHeartBeat = GetTickCount();
			this->SendNotify(MAKEWPARAM(ret, head.cmd), (LPARAM)&ack);
		}

		if (bMask == XT_GET_NEAR_COMM_CUST_ACK::MK_CUR_CUST_COUNT)
		{
			XT_GET_NEAR_COMM_CUST_ACK *pCommCustInfo = (XT_GET_NEAR_COMM_CUST_ACK*)malloc(sizeof(XT_GET_NEAR_COMM_CUST_ACK)
				+ actualCount*sizeof(XT_COMMINFO_GET));

			pCommCustInfo->ret = ret;
			pCommCustInfo->uiBusinessID = uiBusinessID;
			pCommCustInfo->bMask = bMask;
			pCommCustInfo->actualCount = actualCount;

			for (int i = 0; i < actualCount; ++i)
			{
				rp >> pCommCustInfo->CommInfo[i].szCommAccount;
				rp >> pCommCustInfo->CommInfo[i].szCommTime;
				rp >> pCommCustInfo->CommInfo[i].szTransInfo;
				rp >> pCommCustInfo->CommInfo[i].szRemarks;
			}

			m_wLastAckSequenceID = head.seq;
			s_nHeartBeat = GetTickCount();
			this->SendNotify(MAKEWPARAM(ret, head.cmd), (LPARAM)pCommCustInfo);
			free(pCommCustInfo);
			pCommCustInfo = NULL;
		}
	}
}


#ifdef _BUSINESS_VESION


// �����̻������ϵ�Ŀͻ��Ҳ�ҳ��ע(������Ϣ)
void CIMProtocol::OnRecvUpdateNearCommCustRemarkAck(const RecvDataStruct &recvData)
{
	
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}



	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK &ack = *((XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK*)buffAck);


	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len, (char *)m_SessionKey, buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	
	rp >> ack.ret;
	this->SendNotify(MAKEWPARAM(0, head.cmd), (LPARAM)&ack);
	
}


// ��ȡ�̻������ϵ�Ŀͻ��Ҳ౸ע(������Ϣ)
void CIMProtocol::OnRecvGetNearCommCustRemarkAck(const RecvDataStruct &recvData)
{

	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_GET_NEAR_COMM_CUST_REMARK_ACK &ack = *((XT_GET_NEAR_COMM_CUST_REMARK_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len, (char *)m_SessionKey, buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	
	rp >> ack.ret >> ack.uiBusinessID >> ack.szCommAccount >> ack.szCommTime >> ack.szRemarks;
	this->SendNotify(MAKEWPARAM(0, head.cmd), (LPARAM)&ack);
	
}
#endif




#ifdef _BUSINESS_VESION
// �޸Ĺ���״̬
int CIMProtocol::SendReq(const XT_MOD_HANGUP_STATE_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_MOD_HANGUP_STATE_REQ, req.uUserId, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.uUserId;
	cp << req.uHangupState;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}
#endif

#ifdef _VER_INTERNAL_	// �ڲ���
// ��ȡԱ����Ϣ
int CIMProtocol::SendReq(const XT_GET_EMPLOYEE_INFO_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_GET_EMPLOYEE_INFO_REQ, req.uUserId, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.uUserId;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

// ��ȡԱ����ϢЭ�鷵��ֵ
void CIMProtocol::OnRecvGetEmployeeInfoAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	// ������
	uint32 ret = 0;
	rp >> ret;

	XT_GET_EMPLOYEE_INFO_ACK ack;
	ack.ret = ret;

	if (ret == 0)
	{
		rp >> ack.username;
		rp >> ack.job;
		rp >> ack.dept;
		rp >> ack.extension;
		rp >> ack.mobile;
	}

	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ret, head.cmd), (LPARAM)&ack);
}
#endif


/////////////////////////////////////////////////////////////////////////////////////////////
////A��B�����ļ�������������ת
int CIMProtocol::SendReq(const SEND_FILE_PACK_REQ &req)
{
	TRACE("A��B:%d�����ļ�����\n",req.fid);
	SendDataStruct sendData;
    InitSendStruct(sendData, m_dwTranServerIP, m_wTranServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_FILE_TRAN_SEND_BUF);
	XT_HEAD head;
	InitHead(head, CMD_FILE_TRANS_REQ, req.fid, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_FILE_TRAN_SEND_BUF];
	long crypt_len;

	CSendPacket cp(crypt, MAX_FILE_TRAN_SEND_BUF);
	cp << req.send_id;   ////����ID
	cp << req.fid;       ////����ID
	cp << req.flag;      ////�Ƿ����
	cp << req.file_id;   ////�ļ�ID
	cp << req.pack_id;   ////������� 
	cp << req.size;      ////���鳤�� 
	if(req.flag == 1)
	{
		cp << req.buf;
	}
	else
	{
		cp.WriteData(req.buf, req.size);
	}

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm_FileTran->SendDataTo(sendData);
}

/////////////////////////////////////////////////////////////////////////////////////////////
////B�˺ͷ����ͨ��
int CIMProtocol::SendReq(const SEND_FILE_PACK_IND &req)
{
	TRACE("B��A:%d����ȷ����Ϣ\n",req.fid);
	// �����ļ�����

	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwTranServerIP,m_wTranServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_FILE_TRAN_SEND_BUF);
	XT_HEAD head;
	InitHead(head, CMD_FILE_TRANS_IND, req.fid, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_FILE_TRAN_SEND_BUF];
	long crypt_len;

	CSendPacket cp(crypt, MAX_FILE_TRAN_SEND_BUF);
	cp << req.send_id;    ///����ID
	cp << req.fid;        ///����ID
	cp << req.flag;       ////��־
	cp << req.file_id;    ////�ļ�ID
	cp << req.pack_id;    ////�����
	cp << req.size;       ////��С
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm_FileTran->SendDataTo(sendData);
}

//�ϴ�ͷ���ļ�����
int CIMProtocol::SendHeadFileUploadReq(const IMAGE_FILE_STRUCT &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,CDM_HEAD_FILE_UPLOAD_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;


	CSendPacket sp(crypt);
	sp<<g_ImUserInfo.GetId();
	uint8 type = 1;
	sp<<type;
	sp<<req.file_session_id<<req.file_size<<req.file_name;

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

//�ϴ�ͷ���ļ�������
int CIMProtocol::SendHeadFileUploadPacketReq(const SEND_FILE_STRUCT &req, char* buffer, int datalen)
{

	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head,/*CDM_HEAD_FILE_UPLOAD_PACKET_REQ*/CDM_HEAD_FILE_UPLOAD_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;


	CSendPacket sp(crypt);
	sp<<g_ImUserInfo.GetId();
	uint8 type = 2;
	sp<<type;
	sp<<req.file_id<<req.last_packet_id<<req.last_file_pointer;
	sp.WriteData(buffer,/*req.nPacketCount*/datalen);//ÿ��д���ļ���

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

//////////////////////////////////////////////////////////////////////////////////////////
////A��ʼ�����ļ�������
void CIMProtocol::FileTransAck(const char * data,int nLen,const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static SEND_FILE_PACK_REQ ack;
	char buff[MAX_FILE_TRAN_RECV_BUF];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}
	CRecvPacket rp(buff,buff_len);
	{
		rp	>>ack.send_id
			>>ack.fid
			>>ack.flag
			>>ack.file_id
			>>ack.pack_id
			>>ack.size;
		if(ack.flag == 1)
		{
			rp>>ack.buf;
		}
		else
		{
			rp.ReadData(ack.buf, ack.size);
		}
	}
	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}

/////////////////////////////////////////////////////////////////////////////////////////////
////������ת��0x8012����
void CIMProtocol::FileTransInd(const char * data,int nLen,const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	static SEND_FILE_PACK_IND ack;
	char buff[MAX_FILE_TRAN_RECV_BUF];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}
	CRecvPacket rp(buff,buff_len);
	{
		rp	>>ack.send_id
			>>ack.fid
			>>ack.flag
			>>ack.file_id
			>>ack.pack_id
			>>ack.size;
	}
	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}

// �ļ����俪ʼʱ�����͵���Ϣ�����ڷ���˼�¼�ͻ��˵�ip�Ͷ˿�
int CIMProtocol::SendReq_FileTranNotify()
{
	SendDataStruct sendData;
    InitSendStruct(sendData, m_dwTranServerIP, m_wTranServerPort);

	CSendPacket sendPacket(sendData.data, MAX_FILE_TRAN_SEND_BUF);
	XT_HEAD head;
	InitHead(head, CMD_FILE_TRANS_NOTIFY, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	head.did = g_ImUserInfo.GetId();
	sendPacket.SetXtHead(head);

	char crypt[MAX_FILE_TRAN_SEND_BUF];
	long crypt_len;

	CSendPacket cp(crypt, MAX_FILE_TRAN_SEND_BUF);
	cp << g_ImUserInfo.GetId(); 
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm_FileTran->SendDataTo(sendData);
}

// �ļ����俪ʼʱ�����͸������ͨ����Ϣ�󣬷���˷��ص�ȷ����Ϣ
void CIMProtocol::FileTranNotifyAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}
	
	char buff[MAX_FILE_TRAN_RECV_BUF];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	this->SendNotify(MAKEWPARAM(0,head.cmd),0);
}

// ����ͷ�ת��
int CIMProtocol::SendReq(const XT_CUSTSER_TRAN_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_CUSTSER_TRAN_REQ, req.uDid, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.uSid;
	cp<<req.uDid;
	cp<<req.uCustomerId;
	cp<<req.szOrderId;
	cp<<req.szTranReason;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

// ���շ�����ת��ȷ����Ϣ
int CIMProtocol::SendReq(const XT_CUSTSER_TRAN_ACK &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_CUSTSER_TRAN_ACK, req.uDid, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.ret;
	cp<<req.uSid;
	cp<<req.uDid;
	cp<<req.uCustomerId;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}
/*
// ���շ����յ��ͷ�ת������
void CIMProtocol::RecvCustSerTranReqInd(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	char buff[MAX_RECV_BUFFERS];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	static XT_CUSTSER_TRAN_REQ req;
	CRecvPacket rp(buff,buff_len);
	{
		rp	>>req.uSid
			>>req.uDid
			>>req.uCustomerId
			>>req.szOrderId
			>>req.szTranReason;
	}

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);
}
// ���ͷ��յ��ͻ�ת��ȷ����Ϣ
void CIMProtocol::RecvCustSerTranAckInd(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);
	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	char buff[MAX_RECV_BUFFERS];
	memset(buff,0,sizeof(buff));
	long buff_len = packet.ReadPacket(buff,recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff,buff_len,
		(char *)m_SessionKey,
		buff,&buff_len ))
	{
		return ;
	}

	static XT_CUSTSER_TRAN_ACK ack;
	CRecvPacket rp(buff,buff_len);
	{
		rp	>>ack.ret
			>>ack.uSid
			>>ack.uDid
			>>ack.uCustomerId;
	}

	this->SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&ack);
}
*/
////�û��޸ĵ�¼����
int CIMProtocol::SendReq(const XT_MOD_PSW_REQ &req)
{
	////����MD5����
	unsigned char digest[16];
	char password[255] = {0};
	sprintf(password,"%s",req.szPsw);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)password,strlen(password) );
	md5T.MD5Final (digest);
	CString sHex=ToHexString(digest,16);
	int nLen = 0;
	nLen = strlen(sHex);

	////������ܺ���
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MOD_PSW_REQ, req.uid, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.uid;
	cp<<sHex;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

// �����ѽӴ��ͻ�����
int CIMProtocol::SendReq(const XT_RECEPTE_CUST_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_RECEPTE_CUST_REQ, head.sid, XT_HEAD::FLAG_CNTREQUEST, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.type;
	cp<<req.teamId;
	cp<<req.idBuyer;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//�ͷ����õ�ǰ��������
int CIMProtocol::SendReq(const XT_MONITOR_SET_BUYER_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MONITOR_SET_BUYER_REQ, req.idSeller, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.idSeller;
	cp<<req.idBuyer;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//�ͷ����õ�ǰ��������Ӧ��
void CIMProtocol::OnMonitorSetBuyerAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_MONITOR_START_ACK & ack = *((XT_MONITOR_START_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;

	if (ack.ret != 0)
	{	
		//�ͷ�û��������ѯ���
	}
	else
	{
		this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
	}
}

//���ܼ�ؿͷ�
int CIMProtocol::SendReq(const XT_MONITOR_START_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MONITOR_START_REQ, req.idSeller, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.idMaster;
	cp<<req.idSeller;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

// ���ܻ�ȡ�ͷ��Ӵ������
void CIMProtocol::OnMonitorStartAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_MONITOR_START_ACK & ack = *((XT_MONITOR_START_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;
	rp >> ack.idSeller;
	rp >> ack.idBuyer;

	this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
}

//����ȡ����ؿͷ�
int CIMProtocol::SendReq(const XT_MONITOR_END_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MONITOR_END_REQ, req.idSeller, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.idMaster;
	cp<<req.idSeller;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//����ȡ����ؿͷ�Ӧ��
void CIMProtocol::OnMonitorEndAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_MONITOR_END_ACK & ack = *((XT_MONITOR_END_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;

	this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
}

//���ܲ���Ự
int CIMProtocol::SendReq(const XT_MONITOR_INSERT_CHAT_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MONITOR_INSERT_CHAT_REQ, req.idSeller, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.idMaster;
	cp<<req.idSeller;
	cp<<req.idBuyer;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//���ܲ���ỰӦ��
void CIMProtocol::OnMonitorInsertChatAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_MONITOR_INSERT_CHAT_ACK & ack = *((XT_MONITOR_INSERT_CHAT_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;
	if (ack.ret != 0)
	{
	}
	else
	{
		this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
	}
}

//���ܽ��ֻỰ
int CIMProtocol::SendReq(const XT_MONITOR_TRAN_CHAT_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_MONITOR_TRAN_CHAT_REQ, req.idSeller, XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);
	cp<<req.idMaster;
	cp<<req.idSeller;
	cp<<req.idBuyer;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//���ܽ��ֻỰӦ��
void CIMProtocol::OnMonitorTranChatAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	XT_MONITOR_TRAN_CHAT_ACK & ack = *((XT_MONITOR_TRAN_CHAT_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	rp >> ack.ret;
	if (ack.ret != 0)
	{
	}
	else
	{
		this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
	}
}

#ifdef _BUSINESS_VESION
//��ȡ��������ѡ��
int CIMProtocol::SendReq(const BaseReq &req, uint32 nCmd)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, nCmd, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_SERVER, m_wSequenceID++);
	head.sid = g_ImUserInfo.GetId();
	head.did = 0;
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);

	int size = req.ByteSize();
	char *p = new char[size];
	req.SerializeToArray(p, size);
	cp.WriteData(p, size);

	delete[] p;
	p = NULL;
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//��ȡ��������ѡ��Ӧ��
void CIMProtocol::OnGetMsgRemindAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	//XT_MONITOR_TRAN_CHAT_ACK & ack = *((XT_MONITOR_TRAN_CHAT_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	char data[1024] = {0};
	int len = rp.ReadData(data, 1024);

	MsgRemindAck ack;
	if(ack.ParseFromArray(data, len))
	{
		this->SendNotify(MAKEWPARAM(/*ack.ret*/0, head.cmd), (LPARAM)&ack);
	}
}
//���ö�������ѡ��
int CIMProtocol::SendReq(const MsgRemindAck &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_SETMSGREMIND_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_SERVER, m_wSequenceID++);
	head.sid = g_ImUserInfo.GetId();
	head.did = 0;
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);

	int size = req.ByteSize();
	char *p = new char[size];
	req.SerializeToArray(p, size);
	cp.WriteData(p, size);

	delete[] p;
	p = NULL;
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

int CIMProtocol::SendReq(const Buf_ClientSend &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_SEND_BUF_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_SERVER, m_wSequenceID++);
	head.sid = g_ImUserInfo.GetId();
	head.did = 0;
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);

	int size = req.ByteSize();
	char *p = new char[size];
	req.SerializeToArray(p, size);
	cp.WriteData(p, size);

	delete[] p;
	p = NULL;
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

//���ö�������ѡ��Ӧ��
void CIMProtocol::OnSetMsgRemindAck(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	//XT_MONITOR_TRAN_CHAT_ACK & ack = *((XT_MONITOR_TRAN_CHAT_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	uint8 rlt;
	rp >> rlt;
	if(rlt == 0)
	{
		int n = 0;
	}

	//MsgRemindAck ack;
	//if(ack.ParseFromArray(buff, buff_len))
	//{
	//	this->SendNotify(MAKEWPARAM(/*ack.ret*/0, head.cmd), (LPARAM)&ack);
	//}
}
#endif

#ifdef _BUSINESS_VESION
UINT GetZmqMsgProc( LPVOID pParam )
{
	CIMProtocol* p = (CIMProtocol*)pParam;
	p->GetZmqMsgInfo();
	return 0;
};

//������Ϣ
void CIMProtocol::GetZmqMsg()
{
	AfxBeginThread(GetZmqMsgProc,this);
}

void CIMProtocol::GetZmqMsgInfo()
{
	//CString sFile = GetModulePath() + "\\serverlist.config";
	//CServerListFile config;
	//IM_SERVER srv;
	//IM_SERVER srvTran;

	//int i;
	//for ( i=0; i<5; i++ )
	//{
	//	if(config.Load(sFile))
	//	{
	//		break;
	//	}
	//	else
	//	{
	//		Sleep(100);
	//	}
	//}

	//if ( i==5 )
	//{
	//	return;
	//}

	//config.GetBusiSrvList().GetRandSrv(srv);
	//config.GetTranSrvList().GetRandSrv(srvTran);

	//in_addr ip;
	//ip.S_un.S_addr = srv.ip;

	//void * pCtx = NULL;
	//void * pSock = NULL;
	//CString strIp;
	//strIp.Format("tcp://%s:5560", /*SERVER_IP*/inet_ntoa(ip));
	//const char * pAddr = (LPSTR)(LPCSTR)strIp;

	////����context��zmq��socket ��Ҫ��context�Ͻ��д��� 
	//if((pCtx = zmq_ctx_new()) == NULL)
	//{
	//	return;
	//}
	////����zmq socket ��socketĿǰ��6������ ������ʹ��dealer��ʽ
	////����ʹ�÷�ʽ��ο�zmq�ٷ��ĵ���zmq�ֲᣩ 
	//if((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	//{
	//	zmq_ctx_destroy(pCtx);
	//	return ;
	//}
	//int iRcvTimeout = 5000;// millsecond
	////����zmq�Ľ��ճ�ʱʱ��Ϊ5�� 
	//if(zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
	//{
	//	zmq_close(pSock);
	//	zmq_ctx_destroy(pCtx);
	//	return ;
	//}
	//CString strId;
	//strId.Format("%s", /*g_ImUserInfo.GetId()*/"12345");
	//zmq_setsockopt(pSock, ZMQ_IDENTITY, strId, 5);
	////�󶨵�ַ tcp://*:*
	////Ҳ����ʹ��tcpЭ�����ͨ�ţ�ʹ������˿� 5560
	//if(zmq_connect(pSock, pAddr) < 0)
	//{
	//	string strError = zmq_strerror(zmq_errno());
	//	zmq_close(pSock);
	//	zmq_ctx_destroy(pCtx);
	//	return ;
	//}
	//zmq_pollitem_t items [] = 
	//{ 
	//	{ pSock, 0, ZMQ_POLLIN, 0 }
	//}; 
	//// Process messages from both sockets 
	//while (1) 
	//{ 
	//	zmq_poll (items, 1, -1); 
	//	if (items [0].revents & ZMQ_POLLIN) 
	//	{ 
	//		char szData[1024] = {0};
	//		int nLength = zmq_recv (pSock, (void*)szData, 1024, 0); 
	//		if(nLength < 0)
	//		{
	//			//CLog::Log("CZmqServer", CLog::TYPE_ERROR, "������Ϣʧ��");
	//		}
	//		else
	//		{
	//			// ���
	//			DataPacket dpRecvice;
	//			if(dpRecvice.ParseFromArray(szData, nLength))
	//			{
	//				// ����ɹ�
	//				//CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "����ɹ�");
	//				const XT_HEAD_PB& head = dpRecvice.head();
	//				if(head.cmd() == CS_CID_MSGPUSH_IND)
	//				{
	//					// ��Ϣ����
	//					//CLog::Log("CZmqServer", CLog::TYPE_DEBUG, "SS_CID_MSG_PUSH �յ���Ϣ���Ͱ�");
	//					MsgPushInd ind;
	//					if(ind.ParseFromArray(dpRecvice.data().c_str(), dpRecvice.data().length()))
	//					{
	//						//CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "��Ϣ���Ͱ�����ɹ�");
	//					}
	//					else
	//					{
	//						//CLog::Log("CZmqServer", CLog::TYPE_ERROR, "��Ϣ���Ͱ����ʧ��");
	//					}
	//				}
	//			}
	//		}
	//		zmq_send (pSock, "ok", 2, 0); 
	//		// Process task 
	//	}
	//}

}
#endif

int CIMProtocol::SendReq(const csTeamInfoReq &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);
	sendData.retry_times = 0;

	CSendPacket sendPacket(sendData.data, MAX_SEND_BUFFERS);
	XT_HEAD head;
	InitHead(head, CMD_CS_TEAM_INFO_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_SERVER, m_wSequenceID++);
	head.sid = g_ImUserInfo.GetId();
	head.did = 0;
	sendPacket.SetXtHead(head);

	char crypt[MAX_SEND_BUFFERS];
	long crypt_len;

	CSendPacket cp(crypt, MAX_SEND_BUFFERS);

	int size = req.ByteSize();
	char *p = new char[size];
	req.SerializeToArray(p, size);
	cp.WriteData(p, size);

	delete[] p;
	p = NULL;
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

void CIMProtocol::OnGetCsTeamInfo(const RecvDataStruct &recvData)
{
	CRecvPacket packet((char *)recvData.data, recvData.data_len);
	XT_HEAD head;
	packet.GetXtHead(head);

	if (!IsValideServerPacket(recvData))
	{
		return;
	}

	static char buffAck[MAX_RECV_BUFFERS];
	memset(buffAck, 0, sizeof(buffAck));
	//XT_MONITOR_TRAN_CHAT_ACK & ack = *((XT_MONITOR_TRAN_CHAT_ACK*)buffAck);

	char buff[MAX_RECV_BUFFERS];
	long buff_len = packet.ReadPacket(buff, recvData.data_len - sizeof(XT_HEAD));

	if (!TEADecrypt(buff, buff_len,
		(char *)m_SessionKey,
		buff, &buff_len))
	{
		return;
	}

	CRecvPacket rp(buff, buff_len);
	char data[1024] = {0};
	int len = rp.ReadData(data, 1024);

	csTeamInfoAck ack;
	if(ack.ParseFromArray(data, len))
	{
		int nSize = ack.teaminfo().size();
		for(int nTeam = 0; nTeam < ack.teaminfo().size(); nTeam++)
		{
			csTeamInfo csTeam = ack.teaminfo().Get(nTeam);
			int nTeamId = csTeam.csteamid();
			int nTeamQueueLength = csTeam.csqueuelength();
			g_LocalData.SetCsTeamQueue(nTeamId, nTeamQueueLength);
		}

		this->SendNotify(MAKEWPARAM(/*ack.ret*/0, head.cmd), (LPARAM)&ack);
	}
}

////�鿴�û�����״̬
int CIMProtocol::SendReq(const XT_GET_USER_STATUS_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_GET_USER_STATUS_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.cnt;
	for(int nCnt = 0; nCnt < req.cnt; nCnt++)
	cp << req.id[nCnt];

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

void CIMProtocol::OnGetUserStatusAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	static XT_GET_USER_STATUS_ACK ack;

	char crypt[MAX_RECV_BUFFERS];
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack.cnt;
	for(int nCnt = 0; nCnt < ack.cnt; nCnt++)
	{
		rp>>ack.stat[nCnt].id;
		rp>>ack.stat[nCnt].b;
	}

	this->SendNotify(MAKEWPARAM(0, head.cmd), (LPARAM)&ack);
}

////��ȡ��ʷ��Ϣ
int CIMProtocol::SendReq(const XT_WEB_MORE_MSG_REQ &req)
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_WEB_MORE_MSG_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << req.sid << req.did << req.msgtime << req.msgnum << req.msgid << req.uuid << req.isMerchant;

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}

void CIMProtocol::OnWebMoreMsgAck(const RecvDataStruct &recvData)
{
	XT_HEAD head;
	CRecvPacket packet((char *)recvData.data,recvData.data_len);
	packet.GetXtHead(head);

	if ( !IsValideServerPacket(recvData) )
	{
		return ;
	}

	//CString strLog;
	//strLog.Format("\r\nת�ӵ���ʷ��Ϣ�ĳ��ȣ�%d\r\n", recvData.data_len);
	//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);

	//char *buff = new char[/*MAX_RECV_BUFFERS*3*/10000];
	static char buff[MAX_RECV_BUFFERS];
	memset(buff,0,sizeof(buff));
	LPXT_WEB_MORE_MSG_ACK ack= (LPXT_WEB_MORE_MSG_ACK)buff;

	char crypt[MAX_RECV_BUFFERS*3];
	memset(crypt,0,sizeof(crypt));
	long crypt_len=packet.ReadPacket(crypt,recvData.data_len - sizeof(XT_HEAD));

	TEADecrypt(crypt,crypt_len,(char *)m_SessionKey,crypt,&crypt_len);

	CRecvPacket rp(crypt,crypt_len);

	rp>>ack->ret;
	
	if(ack->ret == 0)
	{
		rp>> ack->sid >> ack->did >> ack->msgtime >> ack->msgnum >> ack->returnnum >> ack->over >> ack->actualnum >> ack->uuid;

		//strLog.Format("\r\nsid:%d, did:%d, msgtime:%d, msgnum:%d, returnnum:%d, over:%d, actualnum:%d, uuid:%s\r\n",
		//	ack->sid , ack->did , ack->msgtime , ack->msgnum , ack->returnnum , ack->over , ack->actualnum , ack->uuid);

		//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);

		if(ack->actualnum != 0)
		{
			int actual = ack->actualnum;
			for(int nMsg = 0; nMsg < actual; nMsg++)
			{
				rp >> ack->msgInfo[0].id
					>> ack->msgInfo[0].send_time
					>> ack->msgInfo[0].msgid
					>> ack->msgInfo[0].fontSize
					>> ack->msgInfo[0].fontColor
					>> ack->msgInfo[0].fontStyle
					>> ack->msgInfo[0].fontName
					>> ack->msgInfo[0].nickName
					>> ack->msgInfo[0].data;

				//strLog.Format("\r\nid:%d, send_time:%d, msgid:%d, fontSize:%d, fontColor:%d, fontStyle:%d, fontName:%s, nickName:%s, \r\ndata:%s\r\n",
				//	ack->msgInfo[0].id , 
				//	ack->msgInfo[0].send_time ,
				//	ack->msgInfo[0].msgid ,
				//	ack->msgInfo[0].fontSize ,
				//	ack->msgInfo[0].fontColor ,
				//	ack->msgInfo[0].fontStyle ,
				//	ack->msgInfo[0].fontName ,
				//	ack->msgInfo[0].nickName,
				//	ack->msgInfo[0].data);

				//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, strLog);

				ack->actualnum = 1;
				this->SendNotify(MAKEWPARAM(ack->ret, head.cmd), (LPARAM)ack);
			}
		}
	}
	else
	{
		rp>>m_szError;
	}
	
	//this->SendNotify(MAKEWPARAM(ack->ret, head.cmd), (LPARAM)ack);
}

////����������Ϣ
int CIMProtocol::GetOfflineMsgReq()
{
	SendDataStruct sendData;
	InitSendStruct(sendData, m_dwIMServerIP, m_wIMServerPort);

	CSendPacket sendPacket(sendData.data);
	XT_HEAD head;
	InitHead(head, CMD_GET_OFFLINE_MSG_REQ, g_ImUserInfo.GetId(), XT_HEAD::DEST_TYPE_USER, m_wSequenceID++);
	sendPacket.SetXtHead(head);

	char crypt[1500];
	long crypt_len;

	CSendPacket cp(crypt);
	cp << g_ImUserInfo.GetId();

	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);	

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);
}