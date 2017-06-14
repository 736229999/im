#include "stdafx.h"
#include "ZUdpComm.h"
#include <netinet/in.h>
#include <arpa/inet.h>
#include "../../common/JsonParse.h"
#include "ConfigInfo.h"
#include <sys/epoll.h>
#include <fcntl.h>
#include <sys/socket.h>
#include <netinet/tcp.h>

#ifdef _DEBUG
#include <assert.h>
#endif

#define ACKTIMEOUT 50 * 1000 //ack应答超时时间 ms



void RecvQueueCallback(void * pUserKey,const char * data,int size)
{
//	printf("RecvQueueCallback with CZUdpComm\n");
	CZUdpComm* pObj = (CZUdpComm*)pUserKey;
	pObj->PostQueueData(data,size);
}

void SendQueueCallback(void * pUserKey,const char * data,int size)
{
	CZUdpComm* pObj = (CZUdpComm*)pUserKey;
	pObj->SendQueueData(data,size);
}

void* CZUdpComm::WrokNetRecvThreadFun(LPVOID lpParam)
{
	CZUdpComm* pObj = (CZUdpComm*)lpParam;
	pObj->RecvNetData();
	return 0;
}

void* CZUdpComm::WrokNetTcpThreadFun(LPVOID lpParam)
{
	CZUdpComm* pObj = (CZUdpComm*)lpParam;
	pObj->RecvTcpData();
	return 0;
}

void CZUdpComm::addfd(int epollfd,int fd,bool one_shot)
{
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR;
    if(one_shot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollfd, EPOLL_CTL_ADD, fd, &event);
    setnonblocking(fd);
}

int CZUdpComm::setnonblocking(int fd)
{
    int old_option = fcntl(fd, F_GETFL);
    int new_option = old_option | O_NONBLOCK;
    fcntl(fd, F_SETFL, new_option);
    return old_option;
}

void CZUdpComm::close_conn(int epollfd,int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

void CZUdpComm::erase_conn(int fd)
{
	t_sock_containor::iterator sock_iter;

	sock_iter = sock_conns.find(fd);
	if(sock_iter != sock_conns.end())
	{
		delete (*sock_iter).second;
		sock_conns.erase(sock_iter);
	}
}

#define STR_IPADDR_LEN 32
#define MAX_EVENT_NUMBER 1024
#define WEB_SERVER_NUM 2
char webim_online_ip[WEB_SERVER_NUM][32] = {"10.212.128.161","10.212.128.162"};
void CZUdpComm::RecvTcpData()
{
	CConfigInfo	configInfo;	
	
	int listen_sd = 0 ,svr_conn_sd = 0;
	uint32 reuse;
	uint32 port;
	//fd_set fdread;
	socklen_t acptaddrlen;

	struct sockaddr_in servaddr;
	struct sockaddr acptaddr;
	struct sockaddr_in *p_acptaddr = (sockaddr_in *)&acptaddr;
	char str_ipaddr[STR_IPADDR_LEN] = {0};

	//webim online ip transfer
	struct sockaddr_in webim_addr;
	vector<uint32> v_webimips; 
	for(int i = 0; i < WEB_SERVER_NUM; i ++)
	{
		inet_aton(webim_online_ip[i],&webim_addr.sin_addr);
		CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_DEBUG,"web server ip %s,sin_addr %d",webim_online_ip[i],webim_addr.sin_addr.s_addr);
		v_webimips.push_back(webim_addr.sin_addr.s_addr);
	}

	listen_sd = socket(AF_INET, SOCK_STREAM,0);
	if(listen_sd == -1)
	{
		CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"Create socket failed!\n");
		exit(0);
	}

	reuse = 1;
	setsockopt(listen_sd, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse));

	int keepAlive = 1;
	int keepIdle = 5;
	int keepInterval = 5;
	int keepCount = 5;
	setsockopt(listen_sd, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
	setsockopt(listen_sd, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
	setsockopt(listen_sd, SOL_TCP,TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
	setsockopt(listen_sd,SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));

	memset(&servaddr,0, sizeof(servaddr));

	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(configInfo.GetTcpIP());
	servaddr.sin_port = htons(PORT_TCP_BUSINESS);
	
	if(bind(listen_sd,(struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"Bind failed!\n");
		exit(0);
	}

	if(listen(listen_sd, 1) == -1)
	{
		CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"Listen failed!\n");
		exit(0);
	}

	epoll_event events[MAX_EVENT_NUMBER];
	
    int epollfd = epoll_create(5);
    assert(epollfd != -1);
    addfd(epollfd, listen_sd, false);

	while(1)
	{
		//等待epoll  事件发生
		int number = epoll_wait(epollfd, events, MAX_EVENT_NUMBER, -1);
        if ((number < 0) && (errno != EINTR))
        {
            CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"epoll failure");
            break;
        }
		//处理所发生的所有事件
		for (int i = 0; i < number; i++)
		{
			int sockfd = events[i].data.fd;
			//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
            if( sockfd == listen_sd )
            {
				memset(&acptaddr, 0, sizeof(acptaddr));
				acptaddrlen = sizeof(acptaddr);
				svr_conn_sd = accept(listen_sd, &acptaddr, &acptaddrlen);
                if (svr_conn_sd < 0)
                {
                    CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"Accept failed!");
                    continue;
                } 	
				else
				{
					addfd(epollfd, svr_conn_sd, false);
					
					memset(str_ipaddr, 0, STR_IPADDR_LEN);
					if(NULL==inet_ntop(AF_INET, &p_acptaddr->sin_addr, str_ipaddr, STR_IPADDR_LEN))
					{
						CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"inet_ntop error");
						close_conn(epollfd,svr_conn_sd);
						continue;
					}
					port = ntohs(p_acptaddr->sin_port);
					CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_DEBUG,"client ip %s, port %d accepted!", str_ipaddr, port);
					try
					{
						t_sock_info *p_tmp_sock_info = new t_sock_info(p_acptaddr->sin_addr.s_addr,port);
						sock_conns.insert(t_sock_containor::value_type(svr_conn_sd, p_tmp_sock_info));
					}
					catch(...)
					{
						CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"sock_conns.insert failed!");
						close_conn(epollfd,svr_conn_sd);
						continue;
					}

					// 给web发送系统时间{"serverTime":time}
					time_t timeNow;
					time(&timeNow);
					char szSendBuf[MAX_JSON_BUFFER];
					uint32_t uiSendLen = 0, uiBufLen = 0;

					//组织系统时间json串
					sprintf(szSendBuf + sizeof(uint32), "{\"serverTime\":%u}", (uint32)timeNow);
					uiBufLen = strlen(szSendBuf + sizeof(uint32)) + sizeof(uint32); 
					*(uint32*)szSendBuf = htonl(uiBufLen);

					//socket send
					uiSendLen = send(svr_conn_sd , szSendBuf, uiBufLen, 0);
					if( uiSendLen > 0)
					{
						CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "svrConnSd:%u, sendLen:%d, ip:%s, len:%d, buf:%s", 
							svr_conn_sd, uiSendLen, inet_ntoa(p_acptaddr->sin_addr), uiBufLen, szSendBuf + sizeof(uint32));
					}
					else
					{
						CLog::Log(__FUNCTION__,CLog::TYPE_ERROR, "svrConnSd:%u, sendLen:%d, ip:%s, len:%d, buf:%s", 
							svr_conn_sd, uiSendLen, inet_ntoa(p_acptaddr->sin_addr), uiBufLen, szSendBuf + sizeof(uint32));
					}
		
					
				} 
            }
			else if( events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) )
			{				
				int w = 0;
				struct in_addr webserver_addr;
				t_sock_containor::iterator sock_iter = sock_conns.find(sockfd);
				if(sock_iter != sock_conns.end())
				{
					for(w = 0; w < WEB_SERVER_NUM; w ++)
					{
						if(sock_iter->second->addr == v_webimips[w])
							break;
					}
					if(w < WEB_SERVER_NUM)
					{
						webserver_addr.s_addr = v_webimips[w];
						CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"ip %s disconnect,fd %d,error occur!",inet_ntoa(webserver_addr),sockfd);
					}
				}
				
				CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_DEBUG,"fd %d,error occur!",sockfd);		 
				close_conn(epollfd,sockfd);
				erase_conn(sockfd);
			}
			//如果是已经连接的用户，并且收到数据，那么进行读入。
			else if( events[i].events & EPOLLIN )
			{
				if( recv_webmsg(sockfd) < 0)
				{
					CLog::Log("CZUdpComm::WrokNetTcpThreadFun",CLog::TYPE_ERROR,"interrupt on receiving socket message!");		 
					close_conn(epollfd,sockfd); 
					erase_conn(sockfd);
				}
			}
		}
	}
}


int CZUdpComm::recv_webmsg(int fd)
{
	int buf_len = 0;
	int msg_len = 0;
	t_sock_containor::iterator sock_iter;

	sock_iter = sock_conns.find(fd);
	if(sock_iter == sock_conns.end())
	{
		CLog::Log("CZUdpComm::recv_webmsg",CLog::TYPE_ERROR,"receive_socket_msg() with wrong sock_descriptor!\n");
		return -1;
	}

	buf_len = recv(fd,
		(char *)(sock_iter->second->recv_buf+sock_iter->second->cur_pos),
		MAX_RECV_BUF_SIZE,
		0x0);
	sock_iter->second->cur_pos = 0;
	sock_iter->second->remain_buf_len += buf_len;

	if(buf_len > 0)
	{
		if(sock_iter->second->remain_buf_len >= 4)
		{
			msg_len = ntohl(*(int *)(sock_iter->second->recv_buf+sock_iter->second->cur_pos));
			if(msg_len > MAX_RECV_BUF_SIZE || msg_len <= 0)
			{
				sock_iter->second->remain_buf_len = 0;
				memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
				sock_iter->second->cur_pos = 0;
				CLog::Log("CZUdpComm::recv_webmsg",CLog::TYPE_ERROR,"[SOCKET]: Get wrong message1!\n");
				return -1;
			}
			if(sock_iter->second->remain_buf_len < msg_len)
			{
				sock_iter->second->cur_pos = sock_iter->second->remain_buf_len;
				return 1;
			}
		}
		else
		{
			sock_iter->second->cur_pos = sock_iter->second->remain_buf_len;
			return 1;
		}

		while(sock_iter->second->remain_buf_len >= msg_len)
		{
			webmsg_push(sock_iter->second->recv_buf + sock_iter->second->cur_pos, fd);
			sock_iter->second->cur_pos += msg_len;
			sock_iter->second->remain_buf_len -= msg_len;
			if(sock_iter->second->remain_buf_len >= 4)
			{
				msg_len = ntohl(*(int *)(sock_iter->second->recv_buf + sock_iter->second->cur_pos));
				if(msg_len > MAX_RECV_BUF_SIZE || msg_len <= 0)
				{
					sock_iter->second->remain_buf_len = 0;
					memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
					sock_iter->second->cur_pos = 0;
					CLog::Log("CZUdpComm::recv_webmsg",CLog::TYPE_ERROR,"[SOCKET]: Get wrong message2!\n");
					return -1;
				}
			}
		}  
		if(sock_iter->second->remain_buf_len != 0)
		{
			memcpy(sock_iter->second->recv_buf, sock_iter->second->recv_buf+sock_iter->second->cur_pos, sock_iter->second->remain_buf_len);
		}
		sock_iter->second->cur_pos = sock_iter->second->remain_buf_len;

		return 1;
	}
	else
	{
		sock_iter->second->remain_buf_len = 0;
		memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE+MAX_RECV_BUF_SIZE);
		sock_iter->second->cur_pos = 0;
		CLog::Log("CZUdpComm::recv_webmsg",CLog::TYPE_ERROR,"[SOCKET]: Receive msg with invalid buf_len = %d\n", buf_len);
		return -1;
	}
}

int CZUdpComm::webmsg_push(uint8* data,int fd)
{
	unsigned long length = 0;
	char recvbuf[MAX_RECV_BUF_SIZE];
	char* jsonStr = NULL;
	RecvDataStruct recvData;
	t_sock_containor::iterator sock_iter;

	length = ntohl(*(int *)data);
	memcpy(recvbuf,(char*)data,length);
	jsonStr = recvbuf + 4;

	JsonParse::GetInstance().parseFile((char*)&recvData,jsonStr);
#ifdef TIME_STATISITC
{
		RecvDataStruct* data = &recvData;
		XT_HEAD* head = (XT_HEAD*)data->data;			
		unsigned long nBeginTime = timeGetTimeUsec();

		CLog::Log("WEB_STATISTICS",CLog::TYPE_STATISTIC,"RECV - id:%u , did: %u , cmd:0x%.4x , flag: %u , time: %lu , buffersize %lu ",
			head->sid,head->did,head->cmd,head->flag,nBeginTime,length);
		CLog::Log("WEB_QUEUE_BUFFER",CLog::TYPE_STATISTIC,"RECV - Queue : %d , QueueMax %d ",
			m_RecvQueue.GetSize(),m_RecvQueue.GetMaxSize());

		
}
#endif
	sock_iter = sock_conns.find(fd);
	recvData.src_ip = sock_iter->second->addr;
	recvData.port = fd;

	//XT_HEAD* phead = (XT_HEAD*)recvData.data;

	m_RecvQueue.Push((const char * )&recvData,sizeof(RecvDataStruct) - sizeof(recvData.data) + recvData.data_len);

	return 1;
	
}

CZUdpComm::CZUdpComm(int nConsumeThreadCount /* = 2 */) : 
		   m_RecvQueue(nConsumeThreadCount,6000),m_SendQueue(nConsumeThreadCount,6000)
{
	m_hNetRecvThread = 0;   //处理接收网络数据线程
	m_hNetTcpThread = 0;

	m_pUserKey = NULL;
	m_pfnUserCallback = NULL;

	m_bWork = false;


	m_nRecvPacketNum = 0 ;

}

CZUdpComm::~CZUdpComm(void)
{
	
}

//接收队列回调处理函数
void CZUdpComm::PostQueueData(const char * data,int size)
{
//	printf("CZUdpComm::PostQueueData\n");
	RecvDataStruct * pRecvDataST = (RecvDataStruct * )data;

	m_pfnUserCallback(m_pUserKey,NE_RECVDATA,pRecvDataST);
}

int CZUdpComm::SendUdpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port)
{
	
	DWORD dwFlags = 0;
	int nRet = 0;

	SOCKADDR_IN addrto = {0};
	addrto.sin_family = AF_INET;
	addrto.sin_addr.s_addr = destip;
	addrto.sin_port = htons(port);

	struct timeval timeout;
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	setsockopt(m_sockListen,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
	setsockopt(m_sockListen,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));
    	
	nRet = sendto(m_sockListen,lpWsaBuf->buf,lpWsaBuf->len,dwFlags,
		(struct sockaddr *)&addrto,sizeof(addrto));

	if( nRet == SOCKET_ERROR )
	{
		return -1;
	}

	SysStat::GetInstance().setSndNum();
	return 0;
}

int CZUdpComm::SendTcpData(LPWSABUF lpWsaBuf,uint32 destip,uint16 port,uint16 cmd)
{
	struct in_addr addr;
	t_sock_containor::iterator sock_iter;
	int len = 0;
	int fd = -1;
	uint32 tmpLen = 0;

	fd = port;
	if(fd != -1)
	{
		while(tmpLen < lpWsaBuf->len)
		{
			len = send(fd,lpWsaBuf->buf,lpWsaBuf->len,0);
			
			if(len <= 0)
			{
				addr.s_addr = destip;
				CLog::Log("CZUdpComm::SendTcpData",CLog::TYPE_ERROR,"send to web client:%s failed,fd:%d,cmd:0x%.4x,buf_len:%d,err:%d",inet_ntoa(addr),fd,cmd,lpWsaBuf->len,errno);
				return -1;
			}
			else
				tmpLen += len;
		}		
	}

	return 0;
}

//发送队列回调处理函数
void CZUdpComm::SendQueueData(const char * data,int size)
{
	SendDataStruct * pSendDataST = (SendDataStruct *)data;
	XT_HEAD* pHead = (XT_HEAD*)(pSendDataST->data);
	WSABUF wsaBuf;
	int nRet; 
	char Sendbuf[MAX_JSON_BUFFER];

#ifdef WEBIM_TCP
	if(pHead->flag == XT_HEAD::FLAG_MOBSRVACK || pHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
	{
		wsaBuf.buf = Sendbuf + sizeof(uint32);
		wsaBuf.len = JsonParse::GetInstance().WriteFile(data,wsaBuf.buf);	
		if(wsaBuf.len > 0)
		{
			wsaBuf.buf = Sendbuf;
			*(uint32*)Sendbuf = htonl(wsaBuf.len + sizeof(uint32));
			wsaBuf.len = wsaBuf.len + sizeof(uint32);
			nRet = SendTcpData(&wsaBuf,pSendDataST->dest_ip,pSendDataST->port,pHead->cmd);	

#ifdef TIME_STATISITC
			{
			unsigned long nBeginTime = timeGetTimeUsec();
			CLog::Log("WEB_STATISTICS",CLog::TYPE_STATISTIC,"SEND - id:%u , did: %u , cmd:0x%.4x , flag: %u , time: %lu , buffersize %lu ",
				pHead->sid,pHead->did,pHead->cmd,pHead->flag,nBeginTime,wsaBuf.len);
			}
#endif
		}	
	}	
	else
	{
		wsaBuf.buf = (char *)&pSendDataST->vlink;
		wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;		
		if(wsaBuf.len > 0)
			nRet = SendUdpData(&wsaBuf,pSendDataST->dest_ip,pSendDataST->port);
	}
#else
	wsaBuf.buf = Sendbuf;
	if(pHead->flag == XT_HEAD::FLAG_MOBSRVACK || pHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
		wsaBuf.len = JsonParse::GetInstance().WriteFile(data,wsaBuf.buf);
	else
	{
		wsaBuf.buf = (char *)&pSendDataST->vlink;
		wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;
	}
	if(wsaBuf.len > 0)
		nRet = SendUdpData(&wsaBuf,pSendDataST->dest_ip,pSendDataST->port);
#endif
	if(nRet < 0)
	{
		//error occur
		m_pfnUserCallback(m_pUserKey,NE_SENDFAILED,(RecvDataStruct *)pSendDataST); 
		return ;
	}
	else
	{
		m_pfnUserCallback(m_pUserKey,NE_SENDOK,(RecvDataStruct *)pSendDataST);
	}
}

//线程回调函数
void CZUdpComm::HandleSendIO() 
{
}

void CZUdpComm::RecvOverlapped()
{
}


//线程回调函数
void CZUdpComm::RecvNetData()
{
	int nRet = 0;
	
	socklen_t nAddrLen = sizeof(SOCKADDR_IN);
	
	DWORD dwFlags = 0;

	char recvbuf[MAX_REVBUF_LENGTH];
	RecvDataStruct recvData;	

	for(;;)
	{
		nRet = recvfrom( m_sockListen,
			recvbuf,
			MAX_REVBUF_LENGTH,
			dwFlags,
			(struct sockaddr *)&m_peerAddr,
			&nAddrLen
		);
		if( nRet == SOCKET_ERROR )
		{
			if(WSAGetLastError() == WSAESHUTDOWN )

			{
				//error occur
				CLog::Log("CZUdpComm::RecvNetData",CLog::TYPE_ERROR,"recvfrom data error: %d ",WSAESHUTDOWN);
				m_pfnUserCallback(m_pUserKey,NE_LISTENSTOP,NULL); 
				break;
			}
		}
		else
		{
			if(nRet <= 0)
				continue;

			recvData.data_len = (uint16)(nRet - sizeof(recvData.vlink));
			recvData.src_ip = m_peerAddr.sin_addr.s_addr;
			recvData.port = ntohs( m_peerAddr.sin_port);
/* modify by zhangmin 20150730
			if(recvbuf[0] == '{')
			{
				recvbuf[nRet] = 0;
				JsonParse::GetInstance().parseFile((char*)&recvData,recvbuf);
			}	
			else
*/			{
				memcpy((char*)&recvData.vlink, recvbuf, nRet);
			}	
				
			//XT_HEAD* phead = (XT_HEAD*)recvData.data;
			
			nRet = m_RecvQueue.Push((const char * )&recvData,sizeof(RecvDataStruct) - sizeof(recvData.data) + recvData.data_len);
			
			++m_nRecvPacketNum;
			SysStat::GetInstance().setRevNum();
		}
	}
	CLog::Log("CZUdpComm::RecvNetData",CLog::TYPE_ERROR,"recvfrom data error break loop");
}

int CZUdpComm::StartListen(const char * addr,int port)
{
	//初始化、监听socket
	m_bWork = false;
	/*
	int	iResult; 
	WSADATA ws; 
	iResult = WSAStartup(0x0202,&ws); 
	if(iResult)
		return -1;
*/
	m_listenIP = inet_addr(addr);
	SOCKADDR_IN addrBnd;
	addrBnd.sin_family = AF_INET;
	addrBnd.sin_addr.s_addr = m_listenIP;// m_listenIPINADDR_ANY;
	addrBnd.sin_port = htons(port);

	//m_sockListen = WSASocket(AF_INET,SOCK_DGRAM,0,NULL,0,WSA_FLAG_OVERLAPPED);
	m_sockListen = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
	if(m_sockListen == INVALID_SOCKET)
		return -1;

	struct timeval timeout;
	timeout.tv_sec=5;
	timeout.tv_usec=0;
	setsockopt(m_sockListen,SOL_SOCKET,SO_RCVTIMEO,&timeout,sizeof(timeout));
	setsockopt(m_sockListen,SOL_SOCKET,SO_SNDTIMEO,&timeout,sizeof(timeout));

	int nRet = bind(m_sockListen,(struct sockaddr *)&addrBnd,sizeof(addrBnd));
	if(nRet != 0)
	{
		//trace("socket err:%d",WSAGetLastError());
		return -1;
	}
	
	socklen_t namelen = sizeof(addrBnd);
	getsockname(m_sockListen,(struct sockaddr *)&addrBnd,&namelen);
	m_listenPort = ntohs(addrBnd.sin_port);

	//启动发送和接收信号队列
	m_SendQueue.SetUserCallback(this,SendQueueCallback);
	m_SendQueue.Start();

	m_RecvQueue.SetUserCallback(this,RecvQueueCallback);
	m_RecvQueue.Start();

	m_hNetRecvThread = CreateThread(NULL,0,WrokNetRecvThreadFun,this,0,NULL);
	
	m_bWork = true;
	return 0;
}

void CZUdpComm::StartTcpWork()
{
	m_hNetTcpThread = CreateThread(NULL,0,WrokNetTcpThreadFun,this,0,NULL);
}

int CZUdpComm::SendDataTo(const SendDataStruct &sendData)
{

	if(m_bWork == false)
		return -1;
#ifdef TIME_STATISITC
	CLog::Log("WEB_QUEUE_BUFFER",CLog::TYPE_STATISTIC,"SEND - Queue : %d , QueueMax %d ",
		m_SendQueue.GetSize(),m_SendQueue.GetMaxSize());
#endif
	return m_SendQueue.Push((const char *)&sendData,sizeof(SendDataStruct) - sizeof(sendData.data) + sendData.data_len);
}

void CZUdpComm::StopListen()
{
	m_bWork = false;
// 	m_RecvIO.Reset();
// 	m_SendIO.Reset();

	DWORD dwRet = 0;

	dwRet = WaitForSingleObject(m_hNetRecvThread,10);
	if(dwRet != WAIT_OBJECT_0)
		TerminateThread(m_hNetRecvThread,-1);
	CloseHandle(m_hNetRecvThread);
	m_hNetRecvThread = 0;

#ifdef WEBIM_TCP	
	dwRet = WaitForSingleObject(m_hNetTcpThread,10);
	if(dwRet != WAIT_OBJECT_0)
		TerminateThread(m_hNetTcpThread,-1);
	CloseHandle(m_hNetTcpThread);
	m_hNetTcpThread = 0;
#endif

	//停止队列服务
	m_SendQueue.Stop();
	m_RecvQueue.Stop();

	//关闭内核对象

	closesocket(m_sockListen);
	m_sockListen = 0;

	WSACleanup();
}
