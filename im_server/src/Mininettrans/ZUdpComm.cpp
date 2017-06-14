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
	pObj->recvTcpData();
	return 0;
}

//添加event
void CZUdpComm::addFd(int epollFd,int fd, bool one_shot)
{
	//
    epoll_event event;
    event.data.fd = fd;
    event.events = EPOLLIN | EPOLLET | EPOLLRDHUP | EPOLLERR;
    if(one_shot)
    {
        event.events |= EPOLLONESHOT;
    }
    epoll_ctl(epollFd, EPOLL_CTL_ADD, fd, &event);

	//设置为非阻塞
    fcntl(fd, F_SETFL, fcntl(fd, F_GETFL) | O_NONBLOCK);
}


void CZUdpComm::closeConn(int epollfd,int fd)
{
    epoll_ctl(epollfd, EPOLL_CTL_DEL, fd, 0);
    close(fd);
}

void CZUdpComm::eraseConn(int fd)
{
	t_sock_containor::iterator sock_iter;

	sock_iter = sock_conns.find(fd);
	if(sock_iter != sock_conns.end())
	{
		delete (*sock_iter).second;
		sock_conns.erase(sock_iter);
	}
}


/**&
	webim建立连接时，发送系统时间给webim

*/
void CZUdpComm::sendStytemTimeToNewConnect(int acptFd)
{
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
	uiSendLen = send(acptFd , szSendBuf, uiBufLen, 0);

	CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "svrConnSd:%u, sendLen:%d, serverTime:%d", 
			acptFd, uiSendLen, szSendBuf + sizeof(uint32));	

}


//


void CZUdpComm::recvTcpData()
{
	//配置
	CConfigInfo configInfo;

	//创建socket
	int listenSocket = socket(AF_INET, SOCK_STREAM,0);
	if(listenSocket == -1)
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"createSocketError");
		exit(0);
	}

	//配置socket各属性
	uint32 reuse = 1;
	int keepAlive = 1, keepIdle = 5, keepInterval = 5, keepCount = 5;
	setsockopt(listenSocket, SOL_SOCKET, SO_REUSEADDR, (void *)&reuse, sizeof(reuse));
	setsockopt(listenSocket, SOL_SOCKET, SO_KEEPALIVE, (void*)&keepAlive, sizeof(keepAlive));
	setsockopt(listenSocket, SOL_TCP, TCP_KEEPIDLE, (void *)&keepIdle, sizeof(keepIdle));
	setsockopt(listenSocket, SOL_TCP,TCP_KEEPINTVL, (void *)&keepInterval, sizeof(keepInterval));
	setsockopt(listenSocket,SOL_TCP, TCP_KEEPCNT, (void *)&keepCount, sizeof(keepCount));

	//初始化监听ip和端口
	struct sockaddr_in servaddr;
	memset(&servaddr,0, sizeof(servaddr));
	servaddr.sin_family      = AF_INET;
	servaddr.sin_addr.s_addr = inet_addr(configInfo.GetTcpIP());
	servaddr.sin_port = htons(PORT_TCP_BUSINESS);

	//bind socket 和serveraddr
	if(bind(listenSocket,(struct sockaddr *)&servaddr, sizeof(servaddr)) == -1)
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"bindFailed");
		exit(0);
	}
	
	//监听
	if(listen(listenSocket, 1) == -1)
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"listenFailed");
		exit(0);
	}

	//epoll创建
	epoll_event events[MAX_EVENT_NUMBER];
    int epollFd = epoll_create(5);
    assert(epollFd != -1);
    addFd(epollFd, listenSocket, false);

	while(true)
	{
		//等待epoll  事件发生
		int waitNum = epoll_wait(epollFd, events, MAX_EVENT_NUMBER, -1);
		//CLog::Log(__FUNCTION__, CLog::TYPE_DEBUG, "epollWait:%d", waitNum);
        if ((waitNum < 0) && (errno != EINTR))
        {
            CLog::Log(__FUNCTION__,CLog::TYPE_ERROR, "epoll failure");
            break;
        }
		//处理所发生的所有事件
		for (int i = 0; i < waitNum; i++)
		{
			//如果新监测到一个SOCKET用户连接到了绑定的SOCKET端口，建立新的连接
            if( events[i].data.fd == listenSocket )
            {
				socklen_t acptaddrlen;
				struct sockaddr acptaddr;
				struct sockaddr_in *p_acptaddr = (sockaddr_in *)&acptaddr;
				memset(&acptaddr, 0, sizeof(acptaddr));
				acptaddrlen = sizeof(acptaddr);
				int connSocket = accept(listenSocket, &acptaddr, &acptaddrlen);
                if (connSocket < 0)
                {
                    CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"acceptFailed");
                    continue;
                } 	
				else
				{
					addFd(epollFd, connSocket, false);

					char str_ipaddr[32] = {0};
					memset(str_ipaddr, 0, 32);
					if(NULL == inet_ntop(AF_INET, &p_acptaddr->sin_addr, str_ipaddr, 32))
					{
						CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"inet_ntopError");
						closeConn(epollFd,connSocket);
						continue;
					}
					
					uint32 port = ntohs(p_acptaddr->sin_port);
					CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT,"connect, ip:%s, port:%d, fd:%d", str_ipaddr, port,connSocket);

					try
					{
						t_sock_info *p_tmp_sock_info = new t_sock_info(p_acptaddr->sin_addr.s_addr, port);
						sock_conns.insert(t_sock_containor::value_type(connSocket, p_tmp_sock_info));
					}
					catch(...)
					{
						CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "sock_connsInsertFailed!");
						closeConn(epollFd,connSocket);
						continue;
					}

					//webim建立连接时，发送系统时间给webim
					sendStytemTimeToNewConnect(connSocket);
		
					
				} 
            }
			//连接中断
			else if( events[i].events & (EPOLLRDHUP | EPOLLHUP | EPOLLERR) )
			{	
				t_sock_containor::iterator sock_iter = sock_conns.find(events[i].data.fd);
				if(sock_iter != sock_conns.end())
				{
					struct in_addr addr;
					addr.s_addr = sock_iter->second->addr;
					CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "disconnet, ip:%s, port:%d, fd %d", inet_ntoa(addr), sock_iter->second->port, events[i].data.fd);
				}
				else
				{
					
					CLog::Log(__FUNCTION__,CLog::TYPE_IMPORTANT, "disconnet, fd %d",  events[i].data.fd);
				}
				closeConn(epollFd,events[i].data.fd);
				eraseConn(events[i].data.fd);
			}
			//如果是已经连接的用户，并且收到数据，那么进行读入。
			else if( events[i].events & EPOLLIN )
			{
				if( recv_webmsg(events[i].data.fd) < 0)
				{
					CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"recvError");
					closeConn(epollFd,events[i].data.fd); 
					eraseConn(events[i].data.fd);
				}
			}
			else
			{
				CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"otherEvents");
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
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"wrongSockDescriptor");
		return -1;
	}

	buf_len = recv(fd, (char *)(sock_iter->second->recv_buf + sock_iter->second->cur_pos), MAX_RECV_BUF_SIZE, 0x0);
	sock_iter->second->cur_pos = 0;
	sock_iter->second->remain_buf_len += buf_len;

	if(buf_len > 0)
	{
		if(sock_iter->second->remain_buf_len >= 4)
		{
			msg_len = ntohl(*(int *)(sock_iter->second->recv_buf + sock_iter->second->cur_pos));
			//判断长度是否合法
			if(msg_len > MAX_RECV_BUF_SIZE || msg_len <= 0)
			{
				sock_iter->second->remain_buf_len = 0;
				memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE + MAX_RECV_BUF_SIZE);
				sock_iter->second->cur_pos = 0;
				CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"msgLenInvaild");
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
			webmsg_push((char *)(sock_iter->second->recv_buf + sock_iter->second->cur_pos), fd);
			sock_iter->second->cur_pos += msg_len;
			sock_iter->second->remain_buf_len -= msg_len;
			if(sock_iter->second->remain_buf_len >= 4)
			{
				msg_len = ntohl(*(int *)(sock_iter->second->recv_buf + sock_iter->second->cur_pos));
				//判断长度是否合法
				if(msg_len > MAX_RECV_BUF_SIZE || msg_len <= 0)
				{
					sock_iter->second->remain_buf_len = 0;
					memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE + MAX_RECV_BUF_SIZE);
					sock_iter->second->cur_pos = 0;
					CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"msgLenInvaild2");
					return -1;
				}
			}
		}  
		if(sock_iter->second->remain_buf_len != 0)
		{
			memcpy(sock_iter->second->recv_buf, sock_iter->second->recv_buf + sock_iter->second->cur_pos, sock_iter->second->remain_buf_len);
		}
		sock_iter->second->cur_pos = sock_iter->second->remain_buf_len;

		return 1;
	}
	else
	{
		sock_iter->second->remain_buf_len = 0;
		memset(sock_iter->second->recv_buf, 0, MAX_RECV_BUF_SIZE + MAX_RECV_BUF_SIZE);
		sock_iter->second->cur_pos = 0;
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"recvError:%d", buf_len);
		return -1;
	}
}

void CZUdpComm::webmsg_push(char* recvBuf, int fd)
{
	//减去长度所占的四个字节
	int bufLen= ntohl(*(int *)recvBuf) - sizeof(uint32);

	char recvJson[MAX_RECV_BUF_SIZE] = {0};


	//格式: 长度(4字节)  +  buf 内容
	if(bufLen < MAX_RECV_BUF_SIZE && bufLen > 0)
	{	
		memcpy(recvJson,(char*)recvBuf + sizeof(uint32), bufLen);
		recvJson[bufLen] = 0;

		//json串转化为二进制
		RecvDataStruct recvDataStruct;
		JsonParse::GetInstance().jsonToBin(recvDataStruct, recvJson);

		//ip和port
		t_sock_containor::iterator sock_iter;
		sock_iter = sock_conns.find(fd);
		recvDataStruct.src_ip = sock_iter->second->addr;
		recvDataStruct.port = fd;
		CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG,"webim ip[%u] fd[%d]", recvDataStruct.src_ip, recvDataStruct.port);

		if(m_funRecvNetData != NULL)
		{
			if(!(*m_funRecvNetData)((void*)this, recvDataStruct))
			{
				return;
			}
		}
		else
		{
			CLog::Log("CZUdpComm::RecvNetData",CLog::TYPE_DEBUG,"未设置回调[PORT:%d]", GetPort());
		}

		//转化完后放入recv队列
		m_RecvQueue.Push((const char * )&recvDataStruct,sizeof(RecvDataStruct) - sizeof(recvDataStruct.data) + recvDataStruct.data_len);
	}
	else
	{
		CLog::Log(__FUNCTION__,CLog::TYPE_ERROR,"webMsgPushError:%d", bufLen);
	}
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

	m_funRecvNetData = NULL;
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
	char Sendbuf[MAX_JSON_BUFFER] = {0};

#ifdef WEBIM_TCP
	if(pHead->flag == XT_HEAD::FLAG_MOBSRVACK || pHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
	{
		wsaBuf.buf = Sendbuf + sizeof(uint32);
		wsaBuf.len = JsonParse::GetInstance().binToJson(data,wsaBuf.buf);	
		if(wsaBuf.len > 0)
		{
			wsaBuf.buf = Sendbuf;
			*(uint32*)Sendbuf = htonl(wsaBuf.len + sizeof(uint32));
			wsaBuf.len = wsaBuf.len + sizeof(uint32);
			nRet = SendTcpData(&wsaBuf,pSendDataST->dest_ip,pSendDataST->port,pHead->cmd);	

		}	
	}	
	else
	{
		wsaBuf.buf = Sendbuf;
		//wsaBuf.buf = (char *)&pSendDataST->vlink;
		memcpy(wsaBuf.buf, (char *)&pSendDataST->vlink, sizeof(pSendDataST->vlink));
		memcpy(wsaBuf.buf + sizeof(pSendDataST->vlink), pSendDataST->data, pSendDataST->data_len);
		wsaBuf.len = sizeof(pSendDataST->vlink) + pSendDataST->data_len;		
		if(wsaBuf.len > 0)
			nRet = SendUdpData(&wsaBuf,pSendDataST->dest_ip,pSendDataST->port);
	}
#else
	wsaBuf.buf = Sendbuf;
	if(pHead->flag == XT_HEAD::FLAG_MOBSRVACK || pHead->flag == XT_HEAD::FLAG_MOBSRVACTION)
		wsaBuf.len = JsonParse::GetInstance().binToJson(data,wsaBuf.buf);
	else
	{
		//wsaBuf.buf = (char *)&pSendDataST->vlink;
		memcpy(wsaBuf.buf, (char *)&pSendDataST->vlink, sizeof(pSendDataST->vlink));
		memcpy(wsaBuf.buf + sizeof(pSendDataST->vlink), pSendDataST->data, pSendDataST->data_len);
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

void CZUdpComm::SetUserCallback(FunRecvNetData funRecvNetData)
{
	m_funRecvNetData = funRecvNetData;
	CLog::Log("CZUdpComm::SetUserCallback",CLog::TYPE_DEBUG,"设置回调[0x%X]", (long)this);
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
			if(nRet < (int)sizeof(XT_HEAD))
				continue;	// 过滤掉很小的包

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

			
			if(m_funRecvNetData != NULL)
			{
				if(!(*m_funRecvNetData)((void*)this, recvData))
				{
					continue;
				}
			}
			else
			{
				CLog::Log("CZUdpComm::RecvNetData",CLog::TYPE_DEBUG,"未设置回调[PORT:%d]", GetPort());
			}

			//XT_HEAD* phead = (XT_HEAD*)recvData.data;
			//CLog::Log("RecvNetData",CLog::TYPE_DEBUG,"sid:%u, did:%u, cmd:0x%.4x, flag:%u",phead->sid,phead->did,phead->cmd,phead->flag);
			nRet = m_RecvQueue.Push((const char * )&recvData,sizeof(RecvDataStruct) - sizeof(recvData.data) + recvData.data_len);
			
			++m_nRecvPacketNum;
			SysStat::GetInstance().setRevNum();
		}
	}
	CLog::Log("CZUdpComm::RecvNetData",CLog::TYPE_ERROR,"recvfrom data error break loop");
}

int CZUdpComm::StartListen(const char * addr,int port)
{
	if(port == 10002)
	{
		if(m_funRecvNetData == NULL)
		{
			CLog::Log("CZUdpComm::StartListen", CLog::TYPE_DEBUG, "未设置回调[0x%X]", (long)this);
		}
		else
		{
			CLog::Log("CZUdpComm::StartListen", CLog::TYPE_DEBUG, "已设置回调");
		}
	}

	//初始化、监听socket
	m_bWork = false;
	/*
	int	iResult; 
	WSADATA ws; 
	iResult = WSAStartup(0x0202,&ws); 
	if(iResult)
		return -1;
*/
	if(port != -1)
	{
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
	}

	//启动发送和接收信号队列
	m_SendQueue.SetUserCallback(this,SendQueueCallback);
	m_SendQueue.Start();

	m_RecvQueue.SetUserCallback(this,RecvQueueCallback);
	m_RecvQueue.Start();
	
	if(port != -1)
	{
		m_hNetRecvThread = CreateThread(NULL,0,WrokNetRecvThreadFun,this,0,NULL);
	}
	
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
	CLog::Log("WEB_QUEUE_BUFFER",CLog::TYPE_STATISTIC,"SEND - Queue : %d , QueueMax %d self %u ",
		m_SendQueue.GetSize(),m_SendQueue.GetMaxSize(),pthread_self());
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
