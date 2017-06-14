#include "zmqthread.h"
#include "zmq.h"
#include "Log.h"
#include "AppManager.h"
#include "PacketBuffer.h"
#include "ConfigInfo.h"
#include "SyncCritical.h"
#include "GlobalBusi.h"
#include "User.h"

using namespace std;

// 根据fd获取对端ip
string GetGuestIP(uint32_t fd)
{
	char guest_ip[20] = {0};
	struct sockaddr_in guest;
    socklen_t guest_len = sizeof(guest);
    getpeername(fd, (struct sockaddr *)&guest, &guest_len);
    inet_ntop(AF_INET, &guest.sin_addr, guest_ip, sizeof(guest_ip));

    return guest_ip;
}

// IP地址转换
string GetStrIP(uint32 ip)
{
	sockaddr_in p;
    p.sin_addr.s_addr=ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));
    return szClientIP;
}


CZmqServer *CZmqServer::g_pZmqServer = NULL;
#ifdef ACCESS_SERVER
map<uint32_t, string> CZmqServer::g_mapFD_ImIP;	// 连接到此接入服务器的ImServer 
#else
map<uint32_t, string> CZmqServer::g_mapFD_AccessIP;	// 连接到此ImServer的AccessServer 
#endif


CZmqServer* CZmqServer::GetInstance()
{
	if(g_pZmqServer == NULL)
	{
		CSyncCritical lock;
		lock.Lock();
    	if (g_pZmqServer == NULL)
    	{
       		g_pZmqServer = new CZmqServer();
    	}
    	lock.Unlock();
	}

	return g_pZmqServer;
}

CZmqServer::CZmqServer()
{
	m_pCTX = NULL;
#ifdef ACCESS_SERVER
	m_pSocketAccessServer = NULL;
	m_pSocketMsgNotify = NULL;
#else
	m_pSocketImServer = NULL;
#endif	
	m_pGlobalBusi = NULL;
}

CZmqServer::~CZmqServer()
{
#ifdef ACCESS_SERVER
	if(m_pSocketAccessServer != NULL)
	{
		zmq_close(m_pSocketAccessServer);
		m_pSocketAccessServer = NULL;
	}
	if(m_pSocketMsgNotify != NULL)
	{
		zmq_close(m_pSocketMsgNotify);
		m_pSocketMsgNotify = NULL;
	}
#else
	if(m_pSocketImServer != NULL)
	{
		zmq_close(m_pSocketImServer);
		m_pSocketImServer = NULL;
	}	
#endif

	if(m_pCTX != NULL)
	{
		zmq_ctx_term(m_pCTX);
		m_pCTX = NULL;
	}

	if(m_pGlobalBusi != NULL)
	{
		delete m_pGlobalBusi;
		m_pGlobalBusi = NULL;
	}
}

int CZmqServer::StartWork()
{
	if(!Init())
	{
		CLog::Log("CZmqServer::StartWork", CLog::TYPE_ERROR, "ZMQ线程初始化失败");
		return -1;
	}
	CLog::Log("CZmqServer::StartWork", CLog::TYPE_DEBUG, "ZMQ线程初始化成功");
	
    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        Proc,            // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    return 0;
}

#ifdef ACCESS_SERVER
// REP socket monitor thread
static void *rep_socket_monitor (void *ctx)
{
    static char addr[1025] ;
    int rc;
    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT, "accessserver starting monitor...");
    void *s = zmq_socket (ctx, ZMQ_PAIR);
    assert (s);
    rc = zmq_connect (s, "inproc://monitor_accessserver.rep");
    assert (rc == 0);
	if(rc != 0)
	{
	 	return NULL;
	}
	
    // Initialize poll set
    zmq_pollitem_t items [] =
    {
        { s, 0, ZMQ_POLLIN, 0 }
    };
    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 1, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
            //  First frame in message contains event number and value
            zmq_msg_t msg;
            zmq_msg_init (&msg);
            if (zmq_msg_recv (&msg, s, 0) == -1)
            {
                CLog::Log("rep_socket_monitor", CLog::TYPE_ERROR, "接收监控数据失败");
                return NULL;              //  Interruped, presumably
            }
            assert (zmq_msg_more (&msg));

            uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
            uint16_t event = *(uint16_t *) (data);
            uint32_t value = *(uint32_t *) (data + 2);

            //  Second frame in message contains event address
            zmq_msg_init (&msg);
            if (zmq_msg_recv (&msg, s, 0) == -1)
            {
                CLog::Log("rep_socket_monitor", CLog::TYPE_ERROR, "接收监控数据失败");
                return NULL;              //  Interruped, presumably
            }
            assert (!zmq_msg_more (&msg));

            //if (addr)
            {
            	memset(addr, 0, sizeof(addr));
                uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
                size_t size = zmq_msg_size (&msg);
                memcpy (addr, data, size);
                addr[size] = 0;
            }

            switch (event)
            {
                case ZMQ_EVENT_LISTENING:
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT, "istening socket descriptor %d", value);
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"listening socket address %s", addr);
                    break;
                case ZMQ_EVENT_ACCEPTED:
                {
                    string strGuestIP = GetGuestIP(value);
                    CZmqServer::g_mapFD_ImIP[value] = strGuestIP;
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer[%s]连接到AccessServer", strGuestIP.c_str());
                }
                break;
                case ZMQ_EVENT_CLOSE_FAILED:
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket close failure error code %d",  value);
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket address %s", addr);
                    break;
                case ZMQ_EVENT_CLOSED:
                {
                    auto map_it = CZmqServer::g_mapFD_ImIP.find(value);
                    if(map_it != CZmqServer::g_mapFD_ImIP.end())
                    {
                    	CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer[%s]断开连接", map_it->second.c_str());
                    	CZmqServer::g_mapFD_ImIP.erase(map_it);
                    }
                }
                break;
                case ZMQ_EVENT_DISCONNECTED:
                {
                    auto map_it = CZmqServer::g_mapFD_ImIP.find(value);
                    if(map_it != CZmqServer::g_mapFD_ImIP.end())
                    {
                    	CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer[%s]断开连接", map_it->second.c_str());
                    	CZmqServer::g_mapFD_ImIP.erase(map_it);
                    }
                }
                break;
            }
        }
    }

    zmq_close (s);
    return NULL;
}
#else
// REP socket monitor thread
static void *rep_socket_monitor (void *ctx)
{
    static char addr[1025] ;
    int rc;
    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT, "imserver starting monitor...");
    void *s = zmq_socket (ctx, ZMQ_PAIR);
    assert (s);
    rc = zmq_connect (s, "inproc://monitor_imserver.rep");
    assert (rc == 0);
	if(rc != 0)
	{
	 	return NULL;
	}
	
    // Initialize poll set
    zmq_pollitem_t items [] =
    {
        { s, 0, ZMQ_POLLIN, 0 }
    };
    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 1, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
            //  First frame in message contains event number and value
            zmq_msg_t msg;
            zmq_msg_init (&msg);
            if (zmq_msg_recv (&msg, s, 0) == -1)
            {
                CLog::Log("rep_socket_monitor", CLog::TYPE_ERROR, "接收监控数据失败");
                return NULL;              //  Interruped, presumably
            }
            assert (zmq_msg_more (&msg));

            uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
            uint16_t event = *(uint16_t *) (data);
            uint32_t value = *(uint32_t *) (data + 2);

            //  Second frame in message contains event address
            zmq_msg_init (&msg);
            if (zmq_msg_recv (&msg, s, 0) == -1)
            {
                CLog::Log("rep_socket_monitor", CLog::TYPE_ERROR, "接收监控数据失败");
                return NULL;              //  Interruped, presumably
            }
            assert (!zmq_msg_more (&msg));

            //if (addr)
            {
            	memset(addr, 0, sizeof(addr));
                uint8_t *data = (uint8_t *) zmq_msg_data (&msg);
                size_t size = zmq_msg_size (&msg);
                memcpy (addr, data, size);
                addr[size] = 0;
            }

            switch (event)
            {
                case ZMQ_EVENT_LISTENING:
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT, "listening socket descriptor %d", value);
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"listening socket address %s", addr);
                    break;
                case ZMQ_EVENT_CONNECTED:
                {
                    string strGuestIP = GetGuestIP(value);
                    CZmqServer::g_mapFD_AccessIP[value] = strGuestIP;
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer连接到AccessServer[%s]", strGuestIP.c_str());
                }
                break;
                case ZMQ_EVENT_CLOSE_FAILED:
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket close failure error code %d",  value);
                    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket address %s", addr);
                    break;
                case ZMQ_EVENT_CLOSED:
                {
                    auto map_it = CZmqServer::g_mapFD_AccessIP.find(value);
                    if(map_it != CZmqServer::g_mapFD_AccessIP.end())
                    {
                    	CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"AccessServer[%s]断开连接", map_it->second.c_str());
                    	CZmqServer::g_mapFD_AccessIP.erase(map_it);
                    }
                }
                break;
                case ZMQ_EVENT_DISCONNECTED:
                {
                    auto map_it = CZmqServer::g_mapFD_AccessIP.find(value);
                    if(map_it != CZmqServer::g_mapFD_AccessIP.end())
                    {
                    	CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"AccessServer[%s]断开连接", map_it->second.c_str());
                    	CZmqServer::g_mapFD_AccessIP.erase(map_it);
                    }
                }
                break;
            }
        }
    }

    zmq_close (s);
    return NULL;
}

#endif

#ifdef ACCESS_SERVER
void* CZmqServer::Proc( LPVOID lpParam )
{
	CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ZMQ线程开启");
	
    CZmqServer *pThis = (CZmqServer *)lpParam;

    // Initialize poll set
    zmq_pollitem_t items [] =
    {
        { pThis->m_pSocketMsgNotify, 0, ZMQ_POLLIN | ZMQ_POLLERR, 0 },
        { pThis->m_pSocketAccessServer, 0, ZMQ_POLLIN | ZMQ_POLLERR, 0 }
    };

    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
            // 收到MsgNotify的消息
            char szData[10240] = {0};
            int nLength = zmq_recv (pThis->m_pSocketMsgNotify, (void*)szData, 10240, 0);
            if(nLength < 0)
            {
                CLog::Log("CZmqServer MsgNotify", CLog::TYPE_ERROR, "接收消息失败");
            }
            else
            {
                CLog::Log("CZmqServer MsgNotify", CLog::TYPE_DEBUG, "接收消息成功.消息长度:%d", nLength);

                // 解包
                DataPacket dpRecvice;
                if(dpRecvice.ParseFromArray(szData, nLength))
                {
                    // 解包成功
                    CLog::Log("CZmqServer MsgNotify", CLog::TYPE_IMPORTANT, "解包成功");
                    pThis->OnRecvMsgNotifyServerPacket(dpRecvice);
                }
                else
                {
                    // 解包失败
                    CLog::Log("CZmqServer MsgNotify", CLog::TYPE_ERROR, "解包失败");
                }
            }
        }
        if (items [1].revents & ZMQ_POLLIN)
        {
            // 收到imserver发过来的数据包
			// 第一包为接收方唯一标识，过滤掉
        	int id = 0;
            int nLength = zmq_recv (pThis->m_pSocketAccessServer, (void*)&id, sizeof(int), ZMQ_SNDMORE);

            // 第二包为发过来的数据
            // 收到接入服务器发过来的数据
            CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "AccessServer 接收到 ImServer 传过来的数据包");
            static char szData[10240] = {0};
            memset(szData, 0, sizeof(szData));
            nLength = zmq_recv (pThis->m_pSocketAccessServer, (void*)szData, 10240, 0);
            if(nLength < 0)
            {
                CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "接收ImServer消息失败");
            }
            else
            {
                CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "接收ImServer消息成功.消息长度:%d", nLength);
                SendDataStruct *pSendDataStruct = (SendDataStruct *)szData;
                LPXT_HEAD pHead = (LPXT_HEAD)pSendDataStruct->data;
                CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "数据包发给客户端did[%u] cmd[0x%x] sid[%u] flag[0x%x] from[%u] dtype[0x%x] destip[%u] destport[%d]", pHead->did, pHead->cmd, pHead->sid, pHead->flag, pHead->from, pHead->dtype, pSendDataStruct->dest_ip, pSendDataStruct->port);
                CAppManager::GetInstance().GetAppNetWork().SendDataTo(pThis->m_pGlobalBusi, *pSendDataStruct);
            }
        }

        if (items [1].revents & ZMQ_POLLERR)
        {
            CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "##############接收ImServer错误包###############");
        }
    }

    CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ线程结束");
    return 0 ;
}
#else
void* CZmqServer::Proc( LPVOID lpParam )
{
	CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ZMQ线程开启");
	
    CZmqServer *pThis = (CZmqServer *)lpParam;

    // Initialize poll set

	zmq_pollitem_t items [] =
    {
        { pThis->m_pSocketImServer, 0, ZMQ_POLLIN, 0 }
    };

    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 1, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
        	// 第一包为接收方唯一标识，过滤掉
        	int id = 0;
            int nLength = zmq_recv (pThis->m_pSocketImServer, (void*)&id, sizeof(int), ZMQ_SNDMORE);

            // 第二包为发过来的数据
            // 收到接入服务器发过来的数据
            CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ImServer 接收到 AccessServer 传过来的数据包");
            static char szData[10240] = {0};
            memset(szData, 0, sizeof(szData));
            nLength = zmq_recv (pThis->m_pSocketImServer, (void*)szData, 10240, 0);
            if(nLength < 0)
            {
                CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "接收AccessServer消息失败");
            }
            else
            {
                CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "接收AccessServer消息成功.消息长度:%d", nLength);
                RecvDataStruct *pRecvDataStruct = (RecvDataStruct *)szData;
                CAppManager::GetInstance().GetAppNetWork().ImSvrP2PRecvData(NE_RECVDATA, pRecvDataStruct);
            }
        }
    }

    CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ线程结束");
    return 0 ;
}

#endif


// 收到消息推送服务器传过来的数据包
void CZmqServer::OnRecvMsgNotifyServerPacket(const DataPacket &pack)
{
    //int rc = 0;
    const XT_HEAD_PB& head = pack.head();

    switch(head.cmd())
    {
        case SS_CID_MSG_PUSH:
        {
            CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_DEBUG, "SS_CID_MSG_PUSH 收到消息推送包");
            if(CAppManager::GetInstance().GetFifoMsgNotify() == NULL)
            {
            	CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_ERROR, "消息推送buf管道为空");
            }
            else
            {
            	if(pack.data().length() < MSGNOTIFY_BUF_LENGTH)
            		CAppManager::GetInstance().GetFifoMsgNotify()->Push((BYTE*)pack.data().c_str(),pack.data().length());
            	else
            		CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_IMPORTANT, "SS_CID_MSG_PUSH 收到消息推送包[包数据太长:%d]", pack.data().length());
            }
        }
        break;
        default:
            break;
    }
}

// 收到客户端传过来的数据包
void CZmqServer::OnRecvClientPacket(const DataPacket &pack)
{

}

// 初始化
bool CZmqServer::Init()
{
	pthread_t thread ;

    m_pCTX = zmq_ctx_new ();
    if(m_pCTX == NULL)
    {
    	CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq上下文申请失败");
    	return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq上下文申请成功");

    CConfigInfo		dbConfig;
	char *memcached_addr = dbConfig.GetMemcachedAddr();
	int memcached_port = dbConfig.GetMemcachedPort();
	
	ImMemCache memcached = ImMemCache();
	if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
	{	
		CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
		return false;	
	}
	m_pGlobalBusi = new CGlobalBusi(CAppManager::GetInstance().GetGlobalBusiControl(),&memcached);
	
#ifdef ACCESS_SERVER
	// 获取本机ip,绑定端口
	char ip[32] = {0};
	CAppManager::GetInstance().GetConfig().GetDeviceIp(ip);
	int nPort = CAppManager::GetInstance().GetConfig().GetAccessServerPort();
	m_pSocketAccessServer = zmq_socket (m_pCTX, ZMQ_ROUTER);
	if(m_pSocketAccessServer == NULL)
	{
		CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq接入服务器socket申请失败");
		return false;
	}
	CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq接入服务器socket申请成功");
	// 设置socketid为ip
	int nServerNo = CAppManager::GetInstance().GetConfig().GetServerNo();
	zmq_setsockopt(m_pSocketAccessServer, ZMQ_IDENTITY,(void*)&nServerNo,sizeof(nServerNo));
	
	// 开启连接事件监听
	int rc = zmq_socket_monitor (m_pSocketAccessServer, "inproc://monitor_accessserver.rep", ZMQ_EVENT_ALL);
    rc = pthread_create (&thread, NULL, rep_socket_monitor, m_pCTX);
    
	char addr[128] = {0};
	sprintf_s(addr, "tcp://%s:%d", ip, nPort);
    rc = zmq_bind (m_pSocketAccessServer, addr);
    if(rc != 0)
    {
    	CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq接入服务器绑定地址失败[%s]", addr);
    	return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq接入服务器绑定地址成功[%s]", addr);
    

    // 5561供MsgNotify连接
    vector<string> vecMsgNotifyIP;
    CAppManager::GetInstance().GetConfig().GetMsgNotifyIP(vecMsgNotifyIP);
    if(vecMsgNotifyIP.size() == 0)
    {
    	CLog::Log("CZmqServer", CLog::TYPE_ERROR, "未设置消息推送服务器地址");
    	//return NULL;
    }
    m_pSocketMsgNotify = zmq_socket (m_pCTX, ZMQ_SUB);
    assert (m_pSocketMsgNotify);
    char szConn[128] = {0};
	for(size_t i = 0; i < vecMsgNotifyIP.size(); ++i)
	{
		memset(szConn, 0, sizeof(szConn));
    	sprintf_s(szConn, "tcp://%s:5561", vecMsgNotifyIP[i].c_str());
   	 	rc = zmq_connect(m_pSocketMsgNotify, szConn);
    	assert (rc == 0);
    	if(rc != 0)
    	{
    		CLog::Log("CZmqServer", CLog::TYPE_ERROR, "ZMQ连接消息推送服务器[%s] 失败", szConn);
    		//return NULL;
    	}
    	else
    	{
    		CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ连接消息推送服务器[%s] 成功", szConn);
    	}
	}
	
    // 订阅所有消息
	const char *filter = "";
	zmq_setsockopt(m_pSocketMsgNotify, ZMQ_SUBSCRIBE, filter, strlen (filter)); 
#else
	// imserver连接到接入服务器
	// 获取接入服务器的地址
	vector<string> vecIPs;
	CAppManager::GetInstance().GetConfig().GetAccessServerIPs(vecIPs);
	if(vecIPs.size() == 0)
	{
		CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq 未配置AccessServer ip");
		return false;
	}
	m_pSocketImServer = zmq_socket (m_pCTX, ZMQ_ROUTER);
	if(m_pSocketImServer == NULL)
	{
		CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq imserver 连接 AccessServer socket申请失败");
		return false;
	}
	CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq imserver 连接 AccessServer socket申请成功");

	// 开启连接事件监听
	int rc = zmq_socket_monitor (m_pSocketImServer, "inproc://monitor_imserver.rep", ZMQ_EVENT_ALL);
    rc = pthread_create (&thread, NULL, rep_socket_monitor, m_pCTX);
	
	// 设置socketid为ip
	int nServerNo = CAppManager::GetInstance().GetConfig().GetServerNo();
	zmq_setsockopt(m_pSocketImServer, ZMQ_IDENTITY,(void*)&nServerNo,sizeof(nServerNo));
	char addr[128] = {0};
	int nPort = CAppManager::GetInstance().GetConfig().GetAccessServerPort();
	for(size_t i = 0; i < vecIPs.size(); ++i)
	{
		memset(addr, 0, sizeof(addr));
		sprintf_s(addr, "tcp://%s:%d", vecIPs[i].c_str(), nPort);
    	rc = zmq_connect(m_pSocketImServer, addr);
    	if(rc != 0)
    	{
    		CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq imserver 连接 AccessServer 失败[%s]", addr);
    		return false;
    	}
    	CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq imserver 连接 AccessServer 成功[%s]", addr);
	}
#endif

	return true;
}

#if 0
// MsgNotify推送过来的"消息推送"数据包
void CZmqServer::OnSSCmdMsgPush(const MsgPushInd &ind)
{
	int rc = 0;
    if(ind.mptype() == MP_NEWORDER)
    {
        // 新订单
        CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_IMPORTANT, "收到新订单");
        NewOrder newOrder;
        if(newOrder.ParseFromArray(ind.mpdata().c_str(), ind.mpdata().length()))
        {
            CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_IMPORTANT, "新订单包解包成功");
        }
        else
        {
            CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_ERROR, "新订单包解包失败");
        }
    }

    // 把包传给客户端
    DataPacket dpSend;
    XT_HEAD_PB *pHead = dpSend.mutable_head();
    pHead->set_cmd(CS_CID_MSGPUSH_IND);

	uint32_t msg_size = ind.ByteSize();
    char* szData = new char[msg_size];
    ind.SerializeToArray(szData, msg_size);
    dpSend.set_data(szData, msg_size);
    delete[] szData;
    szData = NULL;

    msg_size = dpSend.ByteSize();
    szData = new char[msg_size];
    dpSend.SerializeToArray(szData, msg_size);

    char szID[8] = {0};
    sprintf_s(szID, "12345");
    CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_IMPORTANT, "ZMQ指定客户端(%s)发送消息", szID);
    rc = zmq_send(sb, szID, 5, ZMQ_SNDMORE);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_ERROR, "发送消息失败:%s", zmq_strerror(zmq_errno()));
    }

    rc = zmq_send(sb, szData, msg_size, 0);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_ERROR, "发送消息失败:%s", zmq_strerror(zmq_errno()));
    }

    CLog::Log("CZmqServer::OnSSCmdMsgPush", CLog::TYPE_IMPORTANT, "ZMQ指定客户端发送成功");

	delete[] szData;
	szData = NULL;
}
#endif

#ifdef	ACCESS_SERVER
// 发送数据包给ImServer服务器
void CZmqServer::SendPacket(const RecvDataStruct &send)
{
	if(m_pSocketAccessServer == NULL)
		return;

	int nServerNo = GetRouterImServerIP(0);
	if(nServerNo == 0)
	{
		CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "获取ImServer IP失败");
		return;
	}
    CLog::Log("CZmqServer::SendPacket", CLog::TYPE_IMPORTANT, "AccessServer 发送给 ImServer[%s]", GetStrIP((uint32)nServerNo).c_str());
    int rc = zmq_send(m_pSocketAccessServer, (void*)&nServerNo, sizeof(nServerNo), ZMQ_SNDMORE);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "发送数据包失败:%s", zmq_strerror(zmq_errno()));
        return;
    }
	// 需要发送一个空的消息帧
	//zmq_send(m_pSocketAccessServer, "", 0, ZMQ_SNDMORE);

    rc = zmq_send(m_pSocketAccessServer, (void*)&send, sizeof(send), 0);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "发送数据包失败:%s", zmq_strerror(zmq_errno()));
        return;
    }

    CLog::Log("CZmqServer::SendPacket", CLog::TYPE_DEBUG, "发送数据包成功[datalen:%d]", sizeof(send));
}

// 获取路由的ImServer IP
int CZmqServer::GetRouterImServerIP(uint32 userid)
{
	// 当前连接的ImServer地址
	if(g_mapFD_ImIP.size() == 0)
	{	// 所有的ImServer都挂了
		CLog::Log("CZmqServer::GetRouterImServerIP", CLog::TYPE_ERROR, "所有ImServer都挂了!!!");
		return 0;
	}
	
	vector<uint32> vecImServerIP;
	auto map_it = g_mapFD_ImIP.begin();
	for(; map_it != g_mapFD_ImIP.end(); ++map_it)
	{
		vecImServerIP.push_back(inet_addr(map_it->second.c_str()));
	}

	int nCount = (int)vecImServerIP.size();
	static int index = 0;
	if(nCount > 0)
	{
		int i = index % nCount;
		index++;
		uint32 nListenAddr = vecImServerIP[i];		
		return (int)nListenAddr;
	}

	return 0;
}
#else
// 发送数据包给AccessServer
void CZmqServer::SendPacket(CGlobalBusi *pGlobalBusi, const SendDataStruct &send)
{
	if(m_pSocketImServer == NULL)
		return;

	LPXT_HEAD lpSendHead = (LPXT_HEAD)send.data;
	int nServerNo = GetRouterAccessServerIP(pGlobalBusi, lpSendHead->did);
	if(nServerNo == 0)
	{
		CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "获取AccessServer IP失败");
		return;
	}
    CLog::Log("CZmqServer::SendPacket", CLog::TYPE_IMPORTANT, "ImServer 发送给 AccessServer[%s]", GetStrIP((uint32)nServerNo).c_str());
    int rc = zmq_send(m_pSocketImServer, (void*)&nServerNo, sizeof(nServerNo), ZMQ_SNDMORE);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "发送数据包失败:%s", zmq_strerror(zmq_errno()));
        return;
    }
	// 需要发送一个空的消息帧
	//zmq_send(m_pSocketAccessServer, "", 0, ZMQ_SNDMORE);

    rc = zmq_send(m_pSocketImServer, (void*)&send, sizeof(send), 0);
    if(rc == -1)
    {
        CLog::Log("CZmqServer::SendPacket", CLog::TYPE_ERROR, "发送数据包失败:%s", zmq_strerror(zmq_errno()));
        return;
    }

    CLog::Log("CZmqServer::SendPacket", CLog::TYPE_DEBUG, "发送数据包成功[datalen:%d]", sizeof(send));
}

// 获取路由的AccessServer IP
int CZmqServer::GetRouterAccessServerIP(CGlobalBusi *pGlobalBusi, uint32 userid)
{
	// 当前连接的AccessServer地址
	if(g_mapFD_AccessIP.size() == 0)
	{	// 所有的ImServer都挂了
		CLog::Log("CZmqServer::GetRouterAccessServerIP", CLog::TYPE_ERROR, "所有AccessServer都挂了!!!");
		return 0;
	}

	vector<uint32> vecAccessServerIP;
	auto map_it = g_mapFD_AccessIP.begin();
	for(; map_it != g_mapFD_AccessIP.end(); ++map_it)
	{
		vecAccessServerIP.push_back(inet_addr(map_it->second.c_str()));
	}

	// 控制信息中查找到AccessServer的IP
	CUser user;
	if(MEMCACHED_GET_INFO_SUCCESS != pGlobalBusi->GetFUserInfo(userid,user))
	{
		CLog::Log("CZmqServer::GetRouterImServerIP", CLog::TYPE_ERROR, "获取%u全量信息失败", userid);
		return 0;
	}

	// 判断原接入服务器是否断开
#if 1
	if(std::find(vecAccessServerIP.begin(),vecAccessServerIP.end(),user.m_nServerIp) != vecAccessServerIP.end())
	{
		return user.m_nServerIp;
	}
	else
#endif	
	{	// 原接入服务器挂了，随机取一个接入服务器返回
		srand((unsigned)time(NULL));
		return vecAccessServerIP[rand()%vecAccessServerIP.size()];
	}
}

#endif

