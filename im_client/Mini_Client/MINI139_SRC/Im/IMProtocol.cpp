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
/* 计算超时时间的python函数代码 
/*************************************************************
def CountTime(timespan,times):
	ran = range(1,times)
	newtime = timespan
	rst = 0
	for i in ran :
		newtime = newtime*1.4
		rst = rst + newtime
		print "重试次数",i+1,":\t新间隔时间 = ",newtime,";\t总耗时 = ",rst
	return rst

****************************************************************/



#define IDS_ERROR_DECODE			"解密错误"
// ack 超时重发次数
#define RETRY_COUNT     2  


//                            () ()
//                            (-o-) 

// 是否打印网络数据调试选项
#define _TRACE_NETWORK

CIMProtocol * CIMProtocol::pInstance = NULL;
CIMProtocol * g_pProtocol = NULL;
DWORD CIMProtocol::s_nHeartBeat = GetTickCount();
int CIMProtocol::s_nCurVer = 0;


// 获取随机端口，范围[1024,4999]
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
		(*this)[0x201] = "注册";
		(*this)[0x202] = "-注册应答-";
		(*this)[0x203] = "登出";
		(*this)[0x211] = "登录";
		(*this)[0x212] = "-登录应答-";
		(*this)[0x301] = "心跳";
		(*this)[0x302] = "-心跳应答-";
		(*this)[0x303] = "修改状态";
		(*this)[0x304] = "-修改状态应答-";
		(*this)[0x413] = "询问好友是否愿意加入群";
		(*this)[0x414] = "-询问好友是否愿意加入群应答-";
		(*this)[0x415] = "查询用户详细资料";
		(*this)[0x416] = "-用户详细资料应答-";
		(*this)[0x417] = "修改用户详细资料";
		(*this)[0x418] = "-修改详细资料应答-";
		(*this)[0x501] = "编辑用户组";
		(*this)[0x502] = "-编辑用户组应答-";
		(*this)[0x503] = "获取用户分组信息";
		(*this)[0x504] = "-用户分组信息应答-";
		(*this)[0x601] = "获取好友列表";
		(*this)[0x602] = "-获取好友列表应答-";
		(*this)[0x603] = "查找用户";
		(*this)[0x604] = "-查找用户应答-";
		(*this)[0x605] = "添加、更改好友到某组";
		(*this)[0x606] = "-添加、更改好友到某组应答-";
		(*this)[0x607] = "发送验证信息";
		(*this)[0x608] = "-发送验证信息应答-";
		(*this)[0x609] = "p2p连接测试";
		(*this)[0x60a] = "-p2p测试应答-";
		(*this)[0x611] = "获取基本资料";
		(*this)[0x612] = "-获取基本资料应答-";
		(*this)[0x60b] = "发送消息";
		(*this)[0x60c] = "-发送消息应答-";
		(*this)[0x60d] = "服务器转发消息";
		(*this)[0x60e] = "-服务器转发消息应答-";
		(*this)[0x701] = "用户状态改变通知";
		(*this)[0x703] = "请求离线消息";
		(*this)[0x704] = "-请求离线消息应答-";
		(*this)[0x705] = "获取好友状态";
		(*this)[0x706] = "-获取好友状态应答-";
		(*this)[0x707] = "修改好友备注";
		(*this)[0x708] = "-修改好友应答-";
		(*this)[0x801] = "发送方请求发送表情";
		(*this)[0x802] = "-接受方对发送表情的应答-";
		(*this)[0x901] = "创建新群";
		(*this)[0x902] = "-创建新群应答-";
		(*this)[0x903] = "编辑群成员";
		(*this)[0x904] = "-编辑群成员应答-";
		(*this)[0x905] = "更改群信息";
		(*this)[0x906] = "-更改群信息应答-";
		(*this)[0x907] = "获取群信息";
		(*this)[0x908] = "-获取群信息应答-";
		(*this)[0x909] = "查找群";
		(*this)[0x90a] = "-查找群应答-";
		(*this)[0x90b] = "加入退出群";
		(*this)[0x90c] = "-加入退出群应答-";
		(*this)[0x90d] = "群验证";
		(*this)[0x90e] = "-群验证应答-";
		(*this)[0x911] = "向群发送消息";
		(*this)[0x912] = "-向群发送消息应答-";
		(*this)[0x913] = "获取群成员列表";
		(*this)[0x914] = "-获取群成员列表应答-";
		(*this)[0x915] = "解散群";
		(*this)[0x916] = "-解散群应答-";
		(*this)[0x91d] = "获取群列表";
		(*this)[0x91e] = "-获取群列表应答-";
		(*this)[0x920] = "群成员在线离线通知";
		(*this)[0x931] = "上传群图片";
		(*this)[0x932] = "-上传群图片应答-";
		(*this)[0x933] = "下载群图片";
		(*this)[0x934] = "-下载群图片应答-";	
		(*this)[0xc01] = "请求发送文件";
		(*this)[0xc02] = "-请求发送文件的应答-";
		(*this)[0xc03] = "下载文件请求";
		(*this)[0xc04] = "-下载文件请求应答-";
		(*this)[0xc05] = "p2p包请求";
		(*this)[0xc06] = "-p2p包数据-";
		(*this)[0xc09] = "通知接收完毕";
		(*this)[0xc0a] = "-通知接收完毕应答-";
		(*this)[0xc0b] = "中止传输";
		(*this)[0xc0c] = "-中止传输应答-";
		(*this)[0x8009] = "服务器中转";
		(*this)[0x800a] = "-服务器中转应答-";
		(*this)[0x800b] = "加密服务器中转";
		(*this)[0x800c] = "-加密服务器中转应答-";
		(*this)[0x8101] = "发送者登录";
		(*this)[0x8102] = "-发送者登录应答-";
		(*this)[0x8107] = "接收者登录";
		(*this)[0x8108] = "-接收者登录应答-";
		(*this)[0x8111] = "发送者关闭会话";
		(*this)[0x8112] = "-发送者关闭会话应答-";
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
				TRACE("%s -> 收 心跳包\n",sTime);
			}
			else
				TRACE("%s -> 收 cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
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
				TRACE("%s <= 重发 心跳包\n",sTime);
			}
			else
			{
				TRACE("%s <= 重发 cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
#endif
			break;	
		}
	case NE_SENDFAILED:
		{
			// 超时
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
				// 发生网络错误
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
			// 设置数据长度
			pHead->len = pSendData->data_len;
#ifdef _TRACE_NETWORK
			in.s_addr = pSendData->dest_ip;
			port = pSendData->port;
			if(pHead->cmd == 0x301)
			{
				TRACE("%s !! 失败 心跳包\n",sTime);
			}
			else
			{
				TRACE("%s !! 失败 cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
#endif
			strcpy(pProtocol->m_szError,"请求超时");
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
				TRACE("%s <- 发 心跳包\n",sTime);
			}
			else
			{
				TRACE("%s <- 发 cmd=0x%x[%s],seq=%d,sid=%d,did=%d,len=%d,vlk=%d [ip]%s:%d\n",
					sTime,pHead->cmd,DIC_CMD[pHead->cmd],pHead->seq,pHead->sid,pHead->did,pSendData->data_len,pSendData->vlink
					,inet_ntoa(in),port);
			}
			break;
		}
	case 1001:
		TRACE("p2p 收到发送者的连接\n");
		break;
	case 1002:
		TRACE("p2p 收到发送者数据\n");
		break;
	case 2001:
		TRACE("p2p 收到接收者同意\n");
		break;
	case 2002:
		TRACE("p2p 收到接收者数据应答\n");
		break;
#endif
	}

	return 0;
}



// 不建议使用该函数,引用了全局变量g_ImUserInfo
inline void InitHead(XT_HEAD & xt_head, uint16 cmd, uint32 dest_id, uint8 dtype, uint32 seq)
{
	if(g_ImUserInfo.GetId() == dest_id)
	{
		dest_id = 0;
	}
	xt_head.flag = 0xf2;		// 客户端请求
	xt_head.cmd  = cmd;
	xt_head.from = CIMProtocol::s_nCurVer;	// 客户端版本标志
	xt_head.seq  = seq;
	xt_head.sid  = g_ImUserInfo.GetId();
	xt_head.did   = dest_id;	// 目标id
	xt_head.dtype = dtype;		// 目标类型
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
	//生成随即密钥
	for ( int n=0; n<16; n++ )
	{
		m_RandomKey[n]=rand()%256;
		m_SessionKey[n]=0x01;
	}
	// 随机序列
	m_wSequenceID = ((rand()<<15)|(rand()<<2))|(rand()>>13);
	m_wLastAckSequenceID = 0;
	s_nHeartBeat = GetTickCount();

	m_pUdpComm = factoryCreateNet();
	m_pUdpComm->SetUserCallback(this,IMCallBack);
	m_pUdpComm_FileTran = factoryCreateNet(0, 1, 50, MAX_FILE_TRAN_SEND_BUF+200);
	m_pUdpComm_FileTran->SetUserCallback(this,IMCallBack);
	// 群消息重复过滤缓存条数
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
		int nTryTimes = 0;	// 尝试次数
		while(1)
		{
			if( m_pUdpComm->StartListen(s.c_str(),dwLocalPort) != 0)
			{
				dwLocalPort = GetRandomPort();
				nTryTimes++;
				if(nTryTimes == 500)
				{
					//UDP启动失败
					ShowMessage("对不起．．网络连接失败！请检查你的网络或者防火墙设置！", MB_OK|MB_ICONERROR);
					return false;
				}
			}
			else
			{
				TRACE1("后台通讯端口：%d\n", dwLocalPort);
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
		int nTryTimes = 0;	// 尝试次数
		while(1)
		{
			if( m_pUdpComm_FileTran->StartListen(s.c_str(),dwLocalPort) != 0)
			{
				dwLocalPort = GetRandomPort();
				nTryTimes++;
				if(nTryTimes == 500)
				{
					//UDP启动失败
					ShowMessage("对不起．．网络连接失败！请检查你的网络或者防火墙设置！", MB_OK|MB_ICONERROR);
					return false;
				}
			}
			else
			{
				TRACE1("文件传输端口：%d\n", dwLocalPort);
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

//注销窗口命令
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



//注册窗口命令
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

//注册窗口命令
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

//向各窗体发送消息
// wParam : 高字节为 命令字,低字节为返回码
// lParm  : 解析好的报文
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

//向各窗体发送消息
// wParam : 高字节为 命令字,低字节为返回码
// lParm  : 解析好的报文
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

	//解析报文
	//消息分发

	LPXT_HEAD pHead = (LPXT_HEAD)pRecvData->data;
	

	try
	{
		// C/S协议
		
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

		case CMD_GET_PROFILE_NAME_ACK:///请求服务器获取群名片信息
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
		case CMD_LOGIN_ACK://处理登录应答报文
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
		case CMD_SET_PROFILE_NAME_ACK:///群名片
			pProtocol->OnSetProfileNameAck(*pRecvData);
			break;
		//case CMD_SET_CLUSTERMSG_RECV_ACK://///群消息接收方式
		//	pProtocol->OnClusterMsgRecSetAck(*pRecvData);
		//	break;

		case CMD_UPDATE_NEAR_COMM_CUST_INFO_ACK:	// 更新最近联系客户应答
			pProtocol->OnRecvUpdateCommInfoAck(*pRecvData);
			break;
		case CMD_GET_NEAR_COMM_CUST_ACK:			// 获取商户最近联系的客户信息
			pProtocol->OnRecvGetNearCommCustAck(*pRecvData);
			break;
		
		case CMD_FILE_TRANS_REQ:    ////新增传输请求
			pProtocol->FileTransAck(pRecvData->data,pRecvData->data_len,*pRecvData);
			break; 

		case CMD_FILE_TRANS_ACK:    ////数据包发送后系统返回的确认消息
			break;

		case CMD_FILE_TRANS_IND:    ////新增传输请求
			pProtocol->FileTransInd(pRecvData->data,pRecvData->data_len,*pRecvData);
			break;

		case CDM_HEAD_FILE_UPLOAD_ACK:  ////上传头像文件应答
			pProtocol->OnHeadFileUploadAck(*pRecvData);
			break;
		case CMD_GET_USER_STATUS_ACK://获取用户状态应答
			pProtocol->OnGetUserStatusAck(*pRecvData);
			break;
		case CMD_WEB_MORE_MSG_ACK://获取历史消息应答
			pProtocol->OnWebMoreMsgAck(*pRecvData);
			break;
			
#ifdef _CS_PLAT_EX_	// 飞牛客服工作台
		case CMD_MOD_PSW_ACK:///请求服务器获取群名片信息
			pProtocol->OnModPswAck(*pRecvData);
			break;
		case CMD_RECEPTE_CUST_ACK:
			pProtocol->OnReceptCustAck(*pRecvData);
			break;

		//case CMD_GET_USERID_MAP_CSID_ACK  :///请求服务器获取客服CSID
		//	pProtocol->OnGetCSIDFromUserIDAck(*pRecvData);
		//	break;
#endif

#ifdef _VER_INTERNAL_	// 内部版
		case CMD_GET_EMPLOYEE_INFO_ACK:				// 获取员工信息
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
		case CMD_FILE_TRANS_IND_ACK:////新增回复确认
			break; 

		case CMD_FILE_TRANS_NOTIFY_ACK:	// 文件传输开始时，发送给服务端通告消息后，服务端返回的确认消息
			pProtocol->FileTranNotifyAck(*pRecvData);
			break;

	/*	case CMD_CUSTSER_TRAN_REQ_IND:		// 接收方接收到客服转接请求
			pProtocol->RecvCustSerTranReqInd(*pRecvData);
			break;
		case CMD_CUSTSER_TRAN_REQ_IND:		// 接收方接收到客服转接请求
			pProtocol->RecvCustSerTranReqInd(*pRecvData);
			break;*/
		case CMD_MONITOR_SET_BUYER_ACK:    // 客服设置当前聊天的买家应答
			pProtocol->OnMonitorSetBuyerAck(*pRecvData);
			break;
		case CMD_MONITOR_START_ACK:       //主管获取客服接待的买家
			pProtocol->OnMonitorStartAck(*pRecvData);
			break;
		case CMD_MONITOR_END_ACK:      // 主管取消监控客服应答
			pProtocol->OnMonitorEndAck(*pRecvData);
			break;
		case CMD_MONITOR_INSERT_CHAT_ACK: //主管插入会话应答
			pProtocol->OnMonitorInsertChatAck(*pRecvData);
			break;
		case CMD_MONITOR_TRAN_CHAT_ACK:  //主管接手会话应答
			pProtocol->OnMonitorTranChatAck(*pRecvData);
			break;
		case CMD_GETMSGREMIND_ACK://获取订单订阅选项
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

// P2P协议
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

// 中转协议
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
// 超时重发
sendData.tick_count = dwTickCount;
pProtocol->m_pUdpComm->SendDataTo(sendData);
pProtocol->m_SendList.Push(sendData);
//pProtocol->//Add2SendList(sendData);
}
else
{
// 超时
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

//price:实际价格的100倍，考虑到目前CSendPacket不支持浮点数打包，用uint32 来表示
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
			strcpy(m_szError,"金币支付超时!");
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

#ifdef _BUSINESS_VESION	// 卖家版的获取好友列表时，在协议头中添加个标志位
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
	//获取用户信息改为发DEST_TYPE_SERVER类型的消息
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
	// 不加密，直接发送
	sendPacket	<<picData.cid
				<<picData.fid
				<<picData.packetId
				<<picData.dataLen
				;
	// 直接写入图片数据
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
	//通过服务器中转
	cp	<<req.cluster_id
		<<req.from_id
		<<req.ver
		<<req.cluster_type
		<<req.data_type//消息类型,0普通消息
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
		// 如果发送的是群图片，则需要记录该数据块，以验证是否成功提交到服务器
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
	// 接受者ip和port
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
	if ( type != SMT_SERVER )//直接发送到对方
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
		// 写入聊天信息内容
		cp.WriteData(req.data,req.data_len);
		if ( req.data_type==XT_MSG::IM_MSG
			|| req.data_type==XT_MSG::EVALUA_ACK
			|| req.data_type==XT_MSG::CUST_SER_TRAN_REQ
			|| req.data_type==XT_MSG::AUTO_REPLY
			|| req.data_type==XT_MSG::IM_MSG_INSERT)
			cp<<req.from_nickname;
		// 用对方的p2p会话键来加密
		FRIEND_INFO * pFriend = GetFriendInfo(req.to_id);
		if(pFriend == NULL)
			return 1;
		TEAEncrypt(crypt,cp.GetSize(),
			(char *)pFriend->key,
			crypt,&crypt_len);
		sendPacket.WritePacket(crypt,crypt_len);

		// 让传输层验证该数据包是否超时

		sendData.time_interval = 500;
		sendData.retry_times = 6;	// 12s 

		// 如果发送图片数据则增加重发次数
		if( req.data_type != XT_MSG::IM_MSG )
		{
			sendData.retry_times = 10; // 50s
		}

		// 使用p2p通讯的ip和端口
		if( type == SMT_P2P)
		{
			sendData.dest_ip  = pFriend->ip;
			sendData.port     = pFriend->port;
		}
		// 使用内网ip
		else 
		{
			sendData.dest_ip = pFriend->localIP;
			sendData.port = pFriend->localPort;
		}
	}
	else
	{
		//通过服务器中转
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
		// 写入聊天信息内容
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
		// 加密
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
	// 注意！
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

#ifdef _BUSINESS_VESION	// 卖家版的获取组列表时，在协议头中添加个标志位
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
//邀请好友入群
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

//好友是否入群
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

	//这里把邀请人和被邀请人交换位置，和后台保持一致
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

//////请求获取群名片信息
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

	sp<<req.nFamilyID;///群ID


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

	if ( req.nFamilyProp==PERM_FAMILY )//固定群
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
	TRACE("用户%s登录服务器%s\n",req.uLoginKey.username,inet_ntoa(in));
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
	char crypt[16]; //对一个空字串,用登录密码的MD5(MD5(password))进行tea加密
	//作为验证码,发送到服务器
	long crypt_len;

	TEAEncrypt(str,1,(char *)digest,crypt,&crypt_len);

	//Build login package
	SendDataStruct sendData;
	InitSendStruct(sendData,m_dwIMServerIP,m_wIMServerPort);

	CSendPacket    sendPacket((char *)sendData.data);

	XT_HEAD xtHead;
	InitHead(xtHead,CMD_LOGIN_REQ,0,XT_HEAD::DEST_TYPE_USER,m_wSequenceID++);

	sendPacket.SetXtHead(xtHead);

	if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYNAME)//用户名登录
		sendPacket<<req.login_flag<<req.uLoginKey.username;
	else
		if ((req.login_flag & 0x3) == XT_LOGIN_REQ::LOGINBYMOBILENO)//手机号登陆
		sendPacket<<req.login_flag<<req.uLoginKey.username;
	else
		sendPacket<<req.login_flag<<req.uLoginKey.id;

#ifdef _CS_PLAT_EX_		// 如果是客服工作台版本，才写入auto_session
	sendPacket<<req.auto_session;
#endif

	memcpy((char *)req.random_key,m_RandomKey,16);
	// 
	sendPacket.WritePacket((char *)m_RandomKey,sizeof(m_RandomKey));

	char cryptData[MAX_SEND_BUFFERS];
	CSendPacket cryptPacket(cryptData);
	// 写验证码
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
	//通知窗体,登录
	SendNotify(MAKEWPARAM(0,CMD_LOGIN_REQ),req.status);
#endif
	
	return m_pUdpComm->SendDataTo(sendData);
}

/*User Name用户注册名	Max 16 Bytes, Text，不可重复。请用小写英文和数字，有效长度4-15，例如：myname_139。
	Client Email	Max 60 Bytes, Text 
	Password	Max 16 Bytes, Text 密码不能为空密码，合法密码为A-Z、a-z、0-9及下划线组成，长度6-15
	用户呢称	Max 24 Bytes, Text 
	出生日期Birthday	8 Bytes，text
	Sex	1 Byte，0-女，1-男，2-未知
	Province代码	1 Byte*/
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
// 修改个人群名片
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
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	// 填写当前群id
	cryptPacket << req.nFamilyID;
	// 填写个人群成员id和成员群名片
	cryptPacket << req.nMemID << req.ProfileName;
	// 加密
	TEAEncrypt(cryptPacket.GetData(), cryptPacket.GetSize(), (char*)m_SessionKey,
		cryptData, &cryptData_len);
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData, cryptData_len);
	// 发送包的长度
	sendData.data_len = sendPacket.GetSize();

	////this->SendNotify(MAKEWPARAM(0, CMD_GET_CLUSTER_PROFILE_ACK), (LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);
}

//////////////////////////////////////////////////////////////////////////////////////////////////////
///////设置群信息接收方式
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
//	// 需要加密的数据
//	CSendPacket cryptPacket(cryptData);
//	// 填写当前群id
//	cryptPacket << req.nFamilyID;
//	// 填写个人群成员id和群消息接收方式
//	cryptPacket << req.nMemID << req.MsgRecType;
//	// 加密
//	TEAEncrypt(cryptPacket.GetData(), cryptPacket.GetSize(), (char*)m_SessionKey,
//		cryptData, &cryptData_len);
//	// 将输出的加密数据写入发送包
//	sendPacket.WritePacket(cryptData, cryptData_len);
//	// 发送包的长度
//	sendData.data_len = sendPacket.GetSize();
//
//	//this->SendNotify(MAKEWPARAM(0, CMD_GET_CLUSTER_PROFILE_ACK), (LPARAM)&req);
//
//	return m_pUdpComm->SendDataTo(sendData);
//}
//

// 修改好友备注
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
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	// 填写当前用户id
	cryptPacket<<req.id;	
	// 填写好友id和备注名
	cryptPacket<<req.fid<<req.commName;
	// 加密
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData,cryptData_len);
	// 发送包的长度
	sendData.data_len  = sendPacket.GetSize();

	//Add2SendList(sendData);

	this->SendNotify(MAKEWPARAM(0,CMD_GET_CMEMBERS_REQ),(LPARAM)&req);

	return m_pUdpComm->SendDataTo(sendData);

}


#ifdef _BUSINESS_VESION

// 修改最近联系客户右侧页备注
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
	
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	cryptPacket << req.uiBusinessID << req.szCommAccount << req.szCommTime << req.szRemarks;	
	
	// 加密
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,cryptData,&cryptData_len);
	
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData,cryptData_len);
	// 发送包的长度
	sendData.data_len  = sendPacket.GetSize();

	return m_pUdpComm->SendDataTo(sendData);

}


// 获取最近联系客户右侧页备注
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
	
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	cryptPacket << req.uiBusinessID << req.szCommAccount;	
	
	// 加密
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData,cryptData_len);
	
	// 发送包的长度
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

	if ( ack.ret!=0 )//固定群
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
	// 好友列表
	static FRIENDLIST_SEG arrFriend;
	FRIENDITEM	fr;
	uint8 nC;
	uint8 i;
	arrFriend.clear();

	// 解密数据
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
		strcpy(m_szError,"获取好友列表信息失败");
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
			rp>>ack->FamilySearchData[i].nMemCount;///群成员个数
		}
	}
	else
	{
		strcpy(m_szError,"查询群信息失败");
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
		//服务器填写的错误信息
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
		//解密成功！获取用户信息
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket>>ack.ret;

		if ( ack.ret )//修改错误，获取错误信息
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
		strcpy(m_szError,"用户信息解密失败!");
	}
	else
	{
		//解密成功！获取用户信息
		CRecvPacket plainPacket(plain,plain_len);
		plainPacket>>ack.ret;

		static CString strDescription;

		if ( ack.ret==0 )//查询成功
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
			//获取了guid之后更新内存中好友的guid
			FRIEND_INFO *pRealFriend = FindFriend(ack.GetId());
			if(pRealFriend != NULL)
			{
				pRealFriend->SetGuid(ack.GetGuid().c_str());
			}
		}
		else
		{	//查询失败
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
		//sprintf(ack.err,"解密失败!");
	}
	else
	{
		// 解包数据
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

////修改密码后服务端返回确认
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
		//sprintf(ack.err,"解密失败!");
	}
	else
	{
		// 解包数据
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

// 子协议
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
		strcpy(m_szError,"更新状态时出现错误");
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);
}

#ifdef _CS_PLAT_EX_
// 邀约用户的回应
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

//处理来自服务器的消息转发（包括聊天消息，验证消息，系统消息)
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
		// 子协议数据长度，已经解密
		int nSubLen = crypt_len-sizeof(XT_SERVER_TRANSMIT);
		switch (type)
		{
		// 普通消息
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
		// 好友状态
		case (uint16)CMD_FRIEND_STATUS_IND:
			OnFriendStatusChange((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		case (uint16)CMD_CLUSTER_NOTIFY_REQ:
			OnFriendStatusChange((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		// 好友基本资料
		case (uint16)CMD_GET_FRIEND_BASICINFO_NOTIFY:
			OnFriendBasicInfo((char *)rp.GetCursor(),nSubLen);
			flagAck = true;
			break;
		// 好友验证（同意、拒绝、请求)包
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
		// Server通知用户，对方已经成功地将你加为好友	
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
		// 系统消息
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
		// 群聊天消息
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
					msg.msg_type = 1;//UI 根据msg_type判断结构是否为单聊，或者群聊消息
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
		// 群验证包
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
		// 服务器关闭
		case (uint16)XT_SERVER_TRANSMIT::TYPE_SERVER_CLOSE:
			{
				this->SendNotify(MAKEWPARAM(0,XT_SERVER_TRANSMIT::TYPE_SERVER_CLOSE),0);
			}
			break;
		// 好友要求删除自己
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
		case (uint16)CMD_SET_CINFO_REQ:			// 群信息更改
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
		case (uint16)CMD_FRIEND_INFO_IND:		// 好友信息修改
			{
				static XT_INFO_IND ind;
				rp>>ind.fid;
				this->SendNotify(MAKEWPARAM(0,CMD_FRIEND_INFO_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_WAITQUEU_CHANGE:		// 等待队列增加成员（或减少成员）
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
		//case (uint16)CMD_SET_PROFILE_NAME_ACK:	// 系统转发修改群名片
		//	{
		//		AfxMessageBox("a");	
		//	}
		//	break;
		case (uint16)CMD_MONITOR_SET_BUYER_IND://主管获取客服当前聊天买家
			{
				static XT_MONITOR_SET_BUYER_IND  ind;
				rp>>ind.idSeller;
				rp>>ind.idBuyer;
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_SET_BUYER_IND),(LPARAM)&ind);
			}
			break;
		case (uint16)CMD_MONITOR_START_IND://主管获取客服的动作，聊天
			{
				static XT_MONITOR_START_IND  ind;
				rp>>ind.action;
				uint8 nAction = ind.action;
				rp>>ind.idSeller>>ind.idBuyer;
				if(nAction == 0)//客服发送消息
				{

				}
				else if(nAction == 1)//客服收到消息
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
		case (uint16)CMD_MONITOR_END_IND://主管收到取消监控消息
			{
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_END_IND), NULL);
			}
			break;
		case (uint16)CMD_MONITOR_INSERT_CHAT_IND://客服和买家都要获取主管插入的会话
			{
				static XT_MONITOR_INSERT_CHAT_IND ind;
				rp>>ind.idMaster>>ind.action;
				uint8 nAction = ind.action;
				if(nAction == 0)//0.开启插入会话 
				{

				}
				else if(nAction == 1)//1.插入会话
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
		case (uint16)CMD_MONITOR_TRAN_CHAT_IND://客服和买家收到主管接收会话
			{
				static XT_MONITOR_TRAN_CHAT_IND ind;
				rp>>ind.idMaster;
				this->SendNotify(MAKEWPARAM(0,CMD_MONITOR_TRAN_CHAT_IND),(LPARAM)&ind);
			}
			break;

		case (uint16)CMD_BUYER_OFFLINE_IND:	// 买家退出
			{
				static UserOfflineIND indpb;
				if(indpb.ParseFromArray(crypt+sizeof(XT_SERVER_TRANSMIT), nSubLen))
				{	// 反序列号成功
					static XT_STATUS_IND ind;
					ind.fid = indpb.userid();
					ind.status = XTREAM_OFFLINE;
					g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_FRIEND_STATUS_IND),(LPARAM)&ind);

					// 同时发送买家退出，使实现主动邀约
					g_pProtocol->SendNotify(MAKEWPARAM(0,CMD_BUYER_OFFLINE_IND),(LPARAM)&ind);
				}
			}
			break;
		case (uint16)CMD_SEND_BUF_IND:		// 收到大buf
			{
				Buf_ServerSend ind;
				if(ind.ParseFromArray(crypt+sizeof(XT_SERVER_TRANSMIT), nSubLen))
				{	// 反序列号成功
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
					{	// 说明传输完成
						MsgPushInd msgind;
						if(msgind.ParseFromArray(gBigBuf[ind.bufunitidx()].c_str(), gBigBuf[ind.bufunitidx()].length()))
						{
							if(msgind.mptype() == MP_NEWORDER)
							{	// 新订单
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
					{	// buf未接受完，请求下一个buf
						Buf_ClientSend ack;
						ack.set_bufunitidx(ind.bufunitidx().c_str());
						ack.set_buflocation(ind.buflocation());
						ack.set_idxpacketreq(ind.idxpacket()+1);
						SendReq(ack);
					}
				}
			}
			break;
		case CMD_CUSTSER_TRAN_ACK_IND:		// 发送方收到客户转接确认消息
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
			//向服务器返回0x060c的应答		
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
			// 应答包不需要验证是否超时
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
	// 收到p2p测试包时，给出回复
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
	// 应答包不需要验证是否超时
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

	if ( msg.sender_id!=g_ImUserInfo.GetId() )//过滤掉自己的消息
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
	// 如果不存在该好友，丢弃（不接受陌生人的ｐ２ｐ消息？）
	if(pFriend == NULL)
	{
		return;
	}
	// p2p的稳定性增加
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
	// 更新该好友的IP信息
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
	// 收到p2p数据包后需要给对方一个应答
	//
	XT_P2PMSG_ACK ack;
	CSendPacket sendPacket(sendData.data);
	sendData.dest_ip = recvData.src_ip;
	sendData.port    = recvData.port;
	XT_HEAD headAck;
	InitHead(headAck,CMD_P2PMSG_ACK,head.sid,XT_HEAD::DEST_TYPE_USER,head.seq);
	headAck.flag = XT_HEAD::FLAG_CNTACK;	// 客户端应答
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
	// 应答包不需要验证是否超时
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
		TRACE("服务器应答\n");
	}
	else
	{
		packet	>>ack.src_id
				>>ack.des_id
				>>ack.data_type;
		TRACE("%d的p2p应答\n",ack.src_id);
		//FRIEND_INFO * pFriend = FindFriend(ack.src_id);
	
		//// 内网连接成功
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
		//客户端不需要处理这个响应。
		//IM Protocol只要把该请求从发送链表中删除就可以了
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

	//心跳ack不需要从队列中移除。
	//因为，心跳请求报文没有放到发送队列中
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
		strcpy(m_szError,"好友分组解密失败");
	}
	m_wLastAckSequenceID = head.seq;
	s_nHeartBeat = GetTickCount();
	this->SendNotify(MAKEWPARAM(ack.ret,head.cmd),(LPARAM)&ack);

}

//修改组资料应答
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

	if ( ack.ret!=0 )//固定群
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

//邀请好友应答
void CIMProtocol::OnClusterInviteAck(const RecvDataStruct & recvData)
{
	return;
}

//好友是否入群应答
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

		if ( ack.nFamilyProp==PERM_FAMILY )//固定群
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
	static char buff[10000];//临时的，后面会改
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
			>>ack->FriendInfo[i].szGUID;///新增自营GUID标识
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

	//char password[255] = {0};  //交行专用屏蔽
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
	// 登录成功以用户密码的MD5 Digest的MD5 Digest做为密钥加密数据体
	// 登录失败用随机密钥（random key）加密数据体
	// 客户端解密时先考虑登录成功情况
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
				TRACE("服务端得到的token:%s\n", g_strToken);

				//版本更新url
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
		// 登录失败
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
			strcpy( m_szError,"解密失败");
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
///////获取群名片信息
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
///////设置群名片
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
///////设置群信息接收方式
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
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	// 填写
	cryptPacket<<req.srvType<<req.startPos;
	// 用sessionKey加密
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData,cryptData_len);
	// 发送包的长度
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
	// 需要加密的数据
	CSendPacket cryptPacket(cryptData);
	// 填写
	cryptPacket	<<req.nFileSid
				<<req.nFileId
				<<req.nPacketId;
	// 用sessionKey加密
	TEAEncrypt(cryptPacket.GetData(),cryptPacket.GetSize(),(char*)m_SessionKey,
		cryptData,&cryptData_len);
	// 将输出的加密数据写入发送包
	sendPacket.WritePacket(cryptData,cryptData_len);
	// 发送包的长度
	sendData.data_len  = sendPacket.GetSize();
	sendData.time_interval = 3000;
	sendData.retry_times = nRetryTimes;
	return m_pUdpComm->SendDataTo(sendData);
}

//修改个性签名
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


//修改好友关注
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
// 接收下载者B的下载文件请求
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
		TRACE("接收下载者B的请求解密失败\n");
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
// 接收数据提供者A的数据
//------------------------------------------------------
void CIMProtocol::RecvDownloadAck(const char * data,int nLen,const RecvDataStruct &recvData)
{
	LPXT_HEAD pHead = (LPXT_HEAD)data;
	data += sizeof(XT_HEAD);
	nLen -= sizeof(XT_HEAD);
	CRecvPacket rp((char *)data,nLen);


	if ( (pHead->flag < 0xf0 && pHead->flag > 0xf3) )
	{
		TRACE("收到非法下载包\n");
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
// 接收中止传输的请求
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


	// 回复ack

	SendDataStruct sendData;
	InitP2PSendData(sendData,recvData.src_ip,recvData.port);
	XT_HEAD hAck;
	char crypt[200];
	long crypt_len = 0;
	char plain[100];
	CSendPacket sp(sendData.data);
	
	// 通过服务器加密中转回应答
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
		TRACE("加密失败\n");
	}
	sp.WritePacket(crypt,crypt_len);

	// 应答包不需要验证是否超时
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);
}


/////////////////////////////////////////////////////////////////////////////////////////////
//------------------------------------------------------
// 接收完成传输的请求
//------------------------------------------------------
void CIMProtocol::RecvFinishReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("A收到B下载完成通知\n");
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
		TRACE("p2p传输完成解密失败\n");
		return ;
	}*/

	//CRecvPacket rp(plain,plain_len);

	rp >> req.fid;

	// 回复ack
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
	// 应答包不需要验证是否超时
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);


}

//------------------------------------------------------
// 接收发送者A的下载文件应答
//------------------------------------------------------
void CIMProtocol::RecvDownloadFileAck(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("B收到A下载文件应答\n");
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
// 处理请求者B的下载文件请求
//------------------------------------------------------
void CIMProtocol::RecvDownloadFileReq(const char * data,int nLen,const RecvDataStruct &recvData)
{
	TRACE("A收到B下载文件请求\n");
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
	
	// 应答包不需要验证是否超时
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&req);
}

//------------------------------------------------------
// 收到发送者A的发送文件请求
//------------------------------------------------------
void CIMProtocol::RecvSendFileReq(const char * pData,int nLen,const RecvDataStruct &recvData)
{
	TRACE("B收到A发送文件的请求\n");
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

	// 应答包不需要验证是否超时
	sendData.retry_times = 0;
	sendData.data_len = sp.GetSize();
	sendData.vlink = recvData.vlink;
	m_pUdpComm->SendDataTo(sendData);

	SendNotify(MAKEWPARAM(0,head.cmd),(LPARAM)&data);
}


//------------------------------------------------------
// 发送者A发送文件请求
//------------------------------------------------------
int CIMProtocol::SendSendFileReq(const P2P_SENDFILE_REQ & req)
{
	TRACE("A向B发送文件请求%d\n",req.fid);
	// 发送文件请求

	SendDataStruct sendData;
	XT_HEAD head;
	uint32 seq = m_wSequenceID++;

	CSendPacket sendPacket(sendData.data);
	
	
	// 业务服务器加密中转请求
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
		TRACE("没有找到接收文件的好友\n");
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

	// 加密
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
// 请求者B发送文件下载请求
//------------------------------------------------------
int CIMProtocol::SendDownloadFileReq(const P2P_DOWNLOAD_FILE_REQ & req)
{
	TRACE("B请求A下载某文件%d\n",req.fid);
	SendDataStruct sendData;
	XT_HEAD head;
	CSendPacket sendPacket(sendData.data);
	int seq = m_wSequenceID++;
	InitP2PSendData(sendData,m_dwIMServerIP,m_wIMServerPort);

	
	// 业务服务器加密中转
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
//// 发送者A提供下载数据
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
//// 请求者B发送数据下载请求
////------------------------------------------------------
//int CIMProtocol::SendDownloadReq(const P2P_DOWNLOAD_REQ & req,CP2PRecvWnd * pWnd)
//{
//	TRACE("B下载文件%d,%d\n",req.fid,req.packetID);
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
//	//// 用对方的p2p会话键来加密
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
//	//	::MessageBox(NULL,"下载请求超长","",0);
//	//}
//	sendData.vlink = req.fid;
//	
//
//	return m_pUdpComm->SendDataTo(sendData);
//}
//
////------------------------------------------------------
//// 请求者B快速重传数据下载请求
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
//	//// 用对方的p2p会话键来加密
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
// 双方发出终止传输请求
//------------------------------------------------------
int CIMProtocol::SendAbortTranReq(const P2P_ABORT_TRAN_REQ & req)
{
	TRACE("发出终止传输通知\n");
	SendDataStruct sendData;
	// 通过服务器加密中转
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
// 请求者B发送完成传输请求
//------------------------------------------------------
int CIMProtocol::SendFinishTranReq(const P2P_FIN_TRAN_REQ & req)
{
	TRACE("B发送下载完成通知\n");
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


	// 用对方的p2p会话键来加密
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
// 收到经过服务器加密中转的数据
//------------------------------------------------------
void CIMProtocol::RecvServerEncryptTran(const RecvDataStruct & recvData)
{

	// 得到加密数据
	const char * pCryptData = recvData.data + sizeof(XT_HEAD);
	long crypt_len = recvData.data_len - sizeof(XT_HEAD);

	char plain[1500];
	long plain_len = 0;

	if ( !TEADecrypt(pCryptData,crypt_len,(char *)m_SessionKey,
		plain,&plain_len) )
	{
		TRACE("服务器转发加密数据包解密失败\n");
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
// 收到经过服务器透明中转的数据
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
// A向中转服务器申请会话
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

	// 用p2p会话键来加密

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
// A向中转服务器发出关闭会话
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

	// 用p2p会话键来加密

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
// B登录中转服务器
//------------------------------------------------------
int CIMProtocol::C2TSendRecvLogin(const XT_TRANS_RECVLOGIN_REQ & req)
{
	TRACE("B登录中转服务器\n");
	SendDataStruct sendData;
	InitP2PSendData(sendData,req.ip,req.port);
	CSendPacket sp(sendData.data);
	XT_HEAD head;
	int seq = m_wSequenceID++;
	InitHead(head,CMD_TRANS_RECVLOGIN_REQ,req.uid,XT_HEAD::DEST_TYPE_TRANSMIT,seq);
	sp.SetXtHead(head);

	// 用会话键来加密

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
// 通知B登录中转服务器
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
// 发送者A登录中转服务器应答
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
		TRACE("err,发送者A登录中转服务器应答解密失败\n");
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
// 发送者A收到登出中转服务器的应答
//------------------------------------------------------
void CIMProtocol::RecvTransLogoutAck(const RecvDataStruct & recvData)
{
	// 无响应
}

//------------------------------------------------------
// 收到接收者登录/注销中转服务器的应答
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
		TRACE("接收者登录/注销中转服务器的应答解密失败\n");
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
// 收到发送者A请求登录中转服务器的请求
//------------------------------------------------------
void CIMProtocol::RecvNotifyLoginReq(const char * data,int nDataLen,const RecvDataStruct & recvData)
{
	TRACE("B收到A通知登录中转服务器\n");
	LPXT_HEAD pHead = (LPXT_HEAD)data;
	int nLen = nDataLen - sizeof(XT_HEAD);
	CRecvPacket rp((char *)data + sizeof(XT_HEAD),nLen);

	//
	// 回复发送者A应答
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
		TRACE("通知登录中转服务器的应答加密失败\n");
		return;
	}
	sp.WritePacket(crypt,crypt_len);

	// 应答包不需要验证是否超时
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
// 收到接收者B的通知登录应答
//------------------------------------------------------
void CIMProtocol::RecvNotifyLoginAck(const char * data,int nLen,const RecvDataStruct & recvData)
{
	// 无响应
}


//------------------------------------------------------
// 解密发送失败的服务器加密中转数据
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
		TRACE("加密服务器中转数据解密失败\n");
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
// 解释发送失败的服务器透明中转数据
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

// 更新后台”最近联系客户”信息，发送消息
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

// 更新后台”最近联系客户”信息，接收消息
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

// 获取商户最近联系的客户信息(发送消息)
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

// 获取商户最近联系的客户信息(接收消息)
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
	// 错误编号
	uint32 ret = 0;
	rp >> ret;
	if (ret != 0)
	{	// 失败
		XT_GET_NEAR_COMM_CUST_ACK ack;
		ack.ret = ret;

		m_wLastAckSequenceID = head.seq;
		s_nHeartBeat = GetTickCount();
		this->SendNotify(MAKEWPARAM(ret, head.cmd), (LPARAM)&ack);
	}
	else
	{
		uint32  uiBusinessID;				// 商户ID
		uint8   bMask;
		uint32  actualCount;				// 实际发送客户数量
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


// 更新商户最近联系的客户右侧页备注(接收消息)
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


// 获取商户最近联系的客户右侧备注(接收消息)
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
// 修改挂起状态
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

#ifdef _VER_INTERNAL_	// 内部版
// 获取员工信息
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

// 获取员工信息协议返回值
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
	// 错误编号
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
////A向B传输文件，经服务器中转
int CIMProtocol::SendReq(const SEND_FILE_PACK_REQ &req)
{
	TRACE("A向B:%d发送文件请求\n",req.fid);
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
	cp << req.send_id;   ////发送ID
	cp << req.fid;       ////接收ID
	cp << req.flag;      ////是否结束
	cp << req.file_id;   ////文件ID
	cp << req.pack_id;   ////分组序号 
	cp << req.size;      ////分组长度 
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
////B端和服务端通信
int CIMProtocol::SendReq(const SEND_FILE_PACK_IND &req)
{
	TRACE("B向A:%d发送确认消息\n",req.fid);
	// 发送文件请求

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
	cp << req.send_id;    ///发送ID
	cp << req.fid;        ///接收ID
	cp << req.flag;       ////标志
	cp << req.file_id;    ////文件ID
	cp << req.pack_id;    ////组序号
	cp << req.size;       ////大小
	TEAEncrypt(crypt, cp.GetSize(),
		(char *)m_SessionKey,
		crypt, &crypt_len);

	sendPacket.WritePacket(crypt, crypt_len);

	sendData.data_len = sendPacket.GetSize();

	return m_pUdpComm_FileTran->SendDataTo(sendData);
}

//上传头像文件请求
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

//上传头像文件包请求
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
	sp.WriteData(buffer,/*req.nPacketCount*/datalen);//每次写的文件包

	TEAEncrypt(crypt,sp.GetSize(),(char *)m_SessionKey,
		crypt,&crypt_len);

	sendPacket.WritePacket(crypt,crypt_len);

	sendData.data_len = sendPacket.GetSize();
	return m_pUdpComm->SendDataTo(sendData);
}

//////////////////////////////////////////////////////////////////////////////////////////
////A开始传输文件的请求
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
////服务器转发0x8012请求
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

// 文件传输开始时，发送的消息，便于服务端记录客户端的ip和端口
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

// 文件传输开始时，发送给服务端通告消息后，服务端返回的确认消息
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

// 发起客服转接
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

// 接收方发送转接确认消息
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
// 接收方接收到客服转接请求
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
// 发送方收到客户转接确认消息
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
////用户修改登录密码
int CIMProtocol::SendReq(const XT_MOD_PSW_REQ &req)
{
	////密码MD5加密
	unsigned char digest[16];
	char password[255] = {0};
	sprintf(password,"%s",req.szPsw);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)password,strlen(password) );
	md5T.MD5Final (digest);
	CString sHex=ToHexString(digest,16);
	int nLen = 0;
	nLen = strlen(sHex);

	////密码加密后发送
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

// 发送已接待客户请求
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

//客服设置当前聊天的买家
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

//客服设置当前聊天的买家应答
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
		//客服没有正在咨询买家
	}
	else
	{
		this->SendNotify(MAKEWPARAM(ack.ret, head.cmd), (LPARAM)&ack);
	}
}

//主管监控客服
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

// 主管获取客服接待的买家
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

//主管取消监控客服
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

//主管取消监控客服应答
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

//主管插入会话
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

//主管插入会话应答
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

//主管接手会话
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

//主管接手会话应答
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
//获取订单订阅选项
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

//获取订单订阅选项应答
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
//设置订单订阅选项
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

//设置订单订阅选项应答
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

//订单消息
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

	////创建context，zmq的socket 需要在context上进行创建 
	//if((pCtx = zmq_ctx_new()) == NULL)
	//{
	//	return;
	//}
	////创建zmq socket ，socket目前有6中属性 ，这里使用dealer方式
	////具体使用方式请参考zmq官方文档（zmq手册） 
	//if((pSock = zmq_socket(pCtx, ZMQ_DEALER)) == NULL)
	//{
	//	zmq_ctx_destroy(pCtx);
	//	return ;
	//}
	//int iRcvTimeout = 5000;// millsecond
	////设置zmq的接收超时时间为5秒 
	//if(zmq_setsockopt(pSock, ZMQ_RCVTIMEO, &iRcvTimeout, sizeof(iRcvTimeout)) < 0)
	//{
	//	zmq_close(pSock);
	//	zmq_ctx_destroy(pCtx);
	//	return ;
	//}
	//CString strId;
	//strId.Format("%s", /*g_ImUserInfo.GetId()*/"12345");
	//zmq_setsockopt(pSock, ZMQ_IDENTITY, strId, 5);
	////绑定地址 tcp://*:*
	////也就是使用tcp协议进行通信，使用网络端口 5560
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
	//			//CLog::Log("CZmqServer", CLog::TYPE_ERROR, "接收消息失败");
	//		}
	//		else
	//		{
	//			// 解包
	//			DataPacket dpRecvice;
	//			if(dpRecvice.ParseFromArray(szData, nLength))
	//			{
	//				// 解包成功
	//				//CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "解包成功");
	//				const XT_HEAD_PB& head = dpRecvice.head();
	//				if(head.cmd() == CS_CID_MSGPUSH_IND)
	//				{
	//					// 消息推送
	//					//CLog::Log("CZmqServer", CLog::TYPE_DEBUG, "SS_CID_MSG_PUSH 收到消息推送包");
	//					MsgPushInd ind;
	//					if(ind.ParseFromArray(dpRecvice.data().c_str(), dpRecvice.data().length()))
	//					{
	//						//CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "消息推送包解包成功");
	//					}
	//					else
	//					{
	//						//CLog::Log("CZmqServer", CLog::TYPE_ERROR, "消息推送包解包失败");
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

////查看用户在线状态
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

////获取历史消息
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
	//strLog.Format("\r\n转接的历史消息的长度：%d\r\n", recvData.data_len);
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

////请求离线消息
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