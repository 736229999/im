#include "zmqthread.h"
#include "zmq.h"
#include "Log.h"
#include "AppManager.h"
#include "PacketBuffer.h"
#include "ConfigInfo.h"
#include "SyncCritical.h"
#include "GlobalBusi.h"
#include "User.h"
#include "PacketBuffer.h"
#include <sys/timerfd.h>
#include "SyncCritical.h"

// ����̲߳���
struct STRU_THREAD_PARAM
{
	void *pCtx;
	void *pSocket;
};

using namespace std;

// ����fd��ȡ�Զ�ip
string GetGuestIP(uint32_t fd)
{
    char guest_ip[20] = {0};
    struct sockaddr_in guest;
    socklen_t guest_len = sizeof(guest);
    getpeername(fd, (struct sockaddr *)&guest, &guest_len);
    inet_ntop(AF_INET, &guest.sin_addr, guest_ip, sizeof(guest_ip));

    return guest_ip;
}

// IP��ַת��
string GetStrIP(uint32 ip)
{
    sockaddr_in p;
    p.sin_addr.s_addr=ip;
    char szClientIP[64] = {0};
    strcpy( szClientIP, (char*)inet_ntoa(p.sin_addr));
    return szClientIP;
}


static int read_msg(void* s, uint16_t &event, uint32_t &value, char* ep)
{
    int rc ;
    zmq_msg_t msg1;  // binary part
    zmq_msg_init (&msg1);
    zmq_msg_t msg2;  //  address part
    zmq_msg_init (&msg2);
    rc = zmq_msg_recv (&msg1, s, 0);
    if (rc == -1 && zmq_errno() == ETERM)
        return 1 ;
    assert (rc != -1);
    assert (zmq_msg_more(&msg1) != 0);
    rc = zmq_msg_recv (&msg2, s, 0);
    if (rc == -1 && zmq_errno() == ETERM)
        return 1;
    assert (rc != -1);
    assert (zmq_msg_more(&msg2) == 0);
    // copy binary data to event struct
    const char* data = (char*)zmq_msg_data(&msg1);
    event = *(uint16_t *) (data);
    value = *(uint32_t *) (data + 2);
    // copy address part
    const size_t len = zmq_msg_size(&msg2) ;
    memcpy(ep, zmq_msg_data(&msg2), len);
    *(ep + len) = 0 ;
    return 0 ;
}


CZmqServer *CZmqServer::g_pZmqServer = NULL;
#ifdef ACCESS_SERVER
map<uint32_t, string> CZmqServer::g_mapFD_ImIP; // ���ӵ��˽����������ImServer
#else
map<uint32_t, string> CZmqServer::g_mapFD_AccessIP; // ���ӵ���ImServer��AccessServer
#endif

static CSyncCritical g_InstanceLock;

CZmqServer* CZmqServer::GetInstance()
{
    if(g_pZmqServer == NULL)
    {
        g_InstanceLock.Lock();
        if (g_pZmqServer == NULL)
        {
            g_pZmqServer = new CZmqServer();
        }
        g_InstanceLock.Unlock();
    }

    return g_pZmqServer;
}

CZmqServer::CZmqServer()
{
    m_pCTX = NULL;
#ifdef ACCESS_SERVER
    m_pSocketAccessServer = NULL;
    m_pSocketMsgNotify = NULL;
    m_pFifoMsg = new CFIFOPacketBuffer(10000, sizeof(RecvDataStruct));
#else
    m_pSocketImServer = NULL;
    m_pFifoMsg = new CFIFOPacketBuffer(10000, sizeof(SendDataStruct));
#endif
	m_pSocketPush = NULL;
	m_pSocketPull = NULL;
}

CZmqServer::~CZmqServer()
{
	if(m_pSocketPush != NULL)
    {
        zmq_close(m_pSocketPush);
        m_pSocketPush = NULL;
    }

    if(m_pSocketPull != NULL)
    {
        zmq_close(m_pSocketPull);
        m_pSocketPull = NULL;
    }

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

    delete m_pFifoMsg;
    m_pFifoMsg = NULL;
}

int CZmqServer::StartWork()
{
    if(!Init())
    {
        CLog::Log("CZmqServer::StartWork", CLog::TYPE_ERROR, "ZMQ�̳߳�ʼ��ʧ��");
        return -1;
    }
    CLog::Log("CZmqServer::StartWork", CLog::TYPE_DEBUG, "ZMQ�̳߳�ʼ���ɹ�");

	 CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        Proc,                       // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier

    CreateThread(
        NULL,                       // default security attributes
        0,                          // use default stack size
        ProcZmqPush,                       // thread function
        this,                       // argument to thread function
        0,                          // use default creation flags
        NULL);                      // returns the thread identifier
    /*
        CreateThread(
            NULL,                       // default security attributes
            0,                          // use default stack size
            Proc_ZmqSendMsg,            // thread function
            this,                       // argument to thread function
            0,                          // use default creation flags
            NULL);                      // returns the thread identifier
    */
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

    uint16_t event = 0;
    uint32_t value = 0;
    while (!read_msg(s, event, value, addr))
    {
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
                CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer[%s]���ӵ�AccessServer", strGuestIP.c_str());
            }
            break;
            case ZMQ_EVENT_CLOSE_FAILED:
                CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket close failure error code %d",value);
                CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"socket address %s", addr);
                break;
            case ZMQ_EVENT_CLOSED:
            {
                auto map_it = CZmqServer::g_mapFD_ImIP.find(value);
                if(map_it != CZmqServer::g_mapFD_ImIP.end())
                {
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ZMQ_EVENT_CLOSED ImServer[%s]�Ͽ�����", map_it->second.c_str());
                    CZmqServer::g_mapFD_ImIP.erase(map_it);
                }
            }
            break;
            case ZMQ_EVENT_DISCONNECTED:
            {
                auto map_it = CZmqServer::g_mapFD_ImIP.find(value);
                if(map_it != CZmqServer::g_mapFD_ImIP.end())
                {
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ZMQ_EVENT_DISCONNECTED ImServer[%s]�Ͽ�����", map_it->second.c_str());
                    CZmqServer::g_mapFD_ImIP.erase(map_it);
                }
            }
            break;
        }
    }

    zmq_close (s);
    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"accessserver stop monitor!!!");
    return NULL;
}
#else
// REP socket monitor thread
static void *rep_socket_monitor (void *ctx)
{
	STRU_THREAD_PARAM *pThreadParam = (STRU_THREAD_PARAM *)ctx;
			
    static char addr[1025] ;
    int rc;
    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT, "imserver starting monitor...");
    void *s = zmq_socket (pThreadParam->pCtx, ZMQ_PAIR);
    assert (s);
    rc = zmq_connect (s, "inproc://monitor_imserver.rep");
    assert (rc == 0);
    if(rc != 0)
    {
    	delete pThreadParam;
        return NULL;
    }

    uint16_t event = 0;
    uint32_t value = 0;
    while (!read_msg(s, event, value, addr))
    {
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
                CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ImServer���ӵ�AccessServer[%s]", strGuestIP.c_str());
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
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ZMQ_EVENT_CLOSED AccessServer[%s]�Ͽ�����", map_it->second.c_str());
                    CZmqServer::g_mapFD_AccessIP.erase(map_it);
                }
            }
            break;
            case ZMQ_EVENT_DISCONNECTED:
            {
                auto map_it = CZmqServer::g_mapFD_AccessIP.find(value);
                if(map_it != CZmqServer::g_mapFD_AccessIP.end())
                {
                    CLog::Log("rep_socket_monitor",CLog::TYPE_IMPORTANT,"ZMQ_EVENT_DISCONNECTED AccessServer[%s]�Ͽ�����", map_it->second.c_str());
                    CZmqServer::g_mapFD_AccessIP.erase(map_it);

                    Sleep(200);

                    // ��������
					char addr[128] = {0};
					int nPort = CAppManager::GetInstance().GetConfig().GetAccessServerPort();
					sprintf_s(addr, "tcp://%s:%d", map_it->second.c_str(), nPort);
					zmq_disconnect(pThreadParam->pSocket, addr);	// �ȶϿ�����
					Sleep(200);
					rc = zmq_connect(pThreadParam->pSocket, addr);
					if(rc != 0)
					{
						CLog::Log("rep_socket_monitor", CLog::TYPE_ERROR, "zmq imserver ���� AccessServer ʧ��[%s]", addr);
					}
					CLog::Log("rep_socket_monitor", CLog::TYPE_DEBUG, "zmq imserver ���� AccessServer �ɹ�[%s]", addr);
                }
            }
            break;
        }
    }

    zmq_close (s);
    CLog::Log("rep_socket_monitor", CLog::TYPE_IMPORTANT,"imserver stop monitor!!!");
    delete pThreadParam;
    return NULL;
}

#endif

#ifdef ACCESS_SERVER
void* CZmqServer::Proc( LPVOID lpParam )
{
    CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ZMQ�߳̿���");

    CConfigInfo     dbConfig;
    char *memcached_addr = dbConfig.GetMemcachedAddr();
    int memcached_port = dbConfig.GetMemcachedPort();

    ImMemCache memcached = ImMemCache();
    if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
    {
        CLog::Log("CZmqServer::Proc",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
        return NULL;
    }
    CGlobalBusi globalbusi(CAppManager::GetInstance().GetGlobalBusiControl(),&memcached);

    CZmqServer *pThis = (CZmqServer *)lpParam;

    // Initialize poll set
    zmq_pollitem_t items [] =
    {
        { pThis->m_pSocketMsgNotify, 0, ZMQ_POLLIN, 0 },
        { pThis->m_pSocketAccessServer, 0, ZMQ_POLLIN, 0 },
        { pThis->m_pSocketPull, 0, ZMQ_POLLIN, 0 }
    };

    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 3, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
            // �յ�MsgNotify����Ϣ
            char szData[10240] = {0};
            int nLength = zmq_recv (pThis->m_pSocketMsgNotify, (void*)szData, 10240, 0);
            if(nLength < 0)
            {
                CLog::Log("CZmqServer MsgNotify", CLog::TYPE_ERROR, "������Ϣʧ��");
            }
            else
            {
                CLog::Log("CZmqServer MsgNotify", CLog::TYPE_DEBUG, "������Ϣ�ɹ�.��Ϣ����:%d", nLength);

                // ���
                DataPacket dpRecvice;
                if(dpRecvice.ParseFromArray(szData, nLength))
                {
                    // ����ɹ�
                    CLog::Log("CZmqServer MsgNotify", CLog::TYPE_IMPORTANT, "����ɹ�");
                    pThis->OnRecvMsgNotifyServerPacket(dpRecvice);
                }
                else
                {
                    // ���ʧ��
                    CLog::Log("CZmqServer MsgNotify", CLog::TYPE_ERROR, "���ʧ��");
                }
            }
        }
        if (items [1].revents & ZMQ_POLLIN)
        {
            // �յ�imserver�����������ݰ�
            // ��һ��Ϊ���շ�Ψһ��ʶ�����˵�
			static char szData[2048] = {0};
			size_t nDataLen = 0;
			int64_t more;
			size_t more_size = sizeof more;
			do {
    			/* Create an empty ?MQ message to hold the message part */
    			zmq_msg_t part;
    			zmq_msg_init (&part);
    			/* Block until a message is available to be received from socket */
    			zmq_msg_recv (&part, items [1].socket, 0);
    			memset(szData, 0, sizeof(szData));
    			nDataLen = zmq_msg_size(&part);
    			memcpy(szData, zmq_msg_data(&part), nDataLen);
    			if(nDataLen != sizeof(SendDataStruct)  )
            	{
            		if(nDataLen!= 4)
                		CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "aa:����ImServer��Ϣʧ��.��Ϣ����:%d",nDataLen);
            	}
            	else
            	{
            		SendDataStruct *pSendDataStruct = (SendDataStruct *)szData;
                	LPXT_HEAD pHead = (LPXT_HEAD)pSendDataStruct->data;
                	CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "aa:���ݰ������ͻ���did[%u] cmd[0x%x] sid[%u] flag[0x%x] from[%u] dtype[0x%x] destip[%u] destport[%d]", pHead->did, pHead->cmd, pHead->sid, pHead->flag, pHead->from, pHead->dtype, pSendDataStruct->dest_ip, pSendDataStruct->port);
#ifdef XMPP_BUILD
					if(CAppManager::GetInstance().GetXmppSession()->IsExist(pHead->did,0))
            		{
                		CLog::Log("SendToJabberdIM",CLog::TYPE_IMPORTANT,"send to xmpp, length:%d", pSendDataStruct->data_len);
                		CAppManager::GetInstance().SendToJabberdIM((BYTE*)pSendDataStruct->data, pSendDataStruct->data_len);
            		}	
                	else
#endif					
                	{
                		CAppManager::GetInstance().GetAppNetWork().SendDataTo(&globalbusi, *pSendDataStruct);
                	}	
            	}
    			/* Determine if more message parts are to follow */
    			zmq_getsockopt (items [1].socket, ZMQ_RCVMORE, &more, &more_size);
    			zmq_msg_close (&part); } while (more);

/*


            

            static char szData[2048] = {0};
            memset(szData, 0, sizeof(szData));
            //int id = 0;
            int nLength = zmq_recv (pThis->m_pSocketAccessServer, (void*)szData, 2048, ZMQ_SNDMORE);

            if(nLength != sizeof(SendDataStruct)  )
            {
            	if(nLength!= 4)
                	CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "aa:����ImServer��Ϣʧ��.��Ϣ����:%d",nLength);
            }
            else if(nLength != sizeof(int))
            {
                //CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "1:����ImServer��Ϣ�ɹ�.��Ϣ����:%d", nLength);
                SendDataStruct *pSendDataStruct = (SendDataStruct *)szData;
                LPXT_HEAD pHead = (LPXT_HEAD)pSendDataStruct->data;
                CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "aa:���ݰ������ͻ���did[%u] cmd[0x%x] sid[%u] flag[0x%x] from[%u] dtype[0x%x] destip[%u] destport[%d]", pHead->did, pHead->cmd, pHead->sid, pHead->flag, pHead->from, pHead->dtype, pSendDataStruct->dest_ip, pSendDataStruct->port);
                CAppManager::GetInstance().GetAppNetWork().SendDataTo(&globalbusi, *pSendDataStruct);
            }

            // �ڶ���Ϊ������������
            // �յ����������������������
           // CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "AccessServer ���յ� ImServer �����������ݰ�");
            memset(szData, 0, sizeof(szData));

            nLength = zmq_recv (pThis->m_pSocketAccessServer, (void*)szData, 2048, 0);
            if(nLength != sizeof(SendDataStruct) )
            {
            	if(nLength!= 4)
                	CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "bb:����ImServer��Ϣʧ��.��Ϣ����:%d",nLength);
            }
            else
            {
                SendDataStruct *pSendDataStruct = (SendDataStruct *)szData;
                LPXT_HEAD pHead = (LPXT_HEAD)pSendDataStruct->data;
                CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "bb:���ݰ������ͻ���did[%u] cmd[0x%x] sid[%u] flag[0x%x] from[%u] dtype[0x%x] destip[%u] destport[%d]", pHead->did, pHead->cmd, pHead->sid, pHead->flag, pHead->from, pHead->dtype, pSendDataStruct->dest_ip, pSendDataStruct->port);
                CAppManager::GetInstance().GetAppNetWork().SendDataTo(&globalbusi, *pSendDataStruct);
            }
*/            
        }
        if (items [2].revents & ZMQ_POLLIN)
        {
        	CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "access recv");
        	int nLen = 0;
        	RecvDataStruct send;
			nLen = zmq_recv(pThis->m_pSocketPull, (void*)&send, sizeof(send), 0);
			if(nLen != sizeof(RecvDataStruct))
            {
                continue;
            }

            if(pThis->m_pSocketAccessServer == NULL)
            {
            	continue;
            }

			LPXT_HEAD pHead = (LPXT_HEAD)send.data;
                    int nServerNo = pThis->GetRouterImServerIP(pHead->sid);
                    if(nServerNo == 0)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "��ȡImServer IPʧ��");
                        continue;
                    }
                    CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_IMPORTANT, "AccessServer ���͸� ImServer[%s]", GetStrIP((uint32)nServerNo).c_str());
                    int rc = zmq_send(pThis->m_pSocketAccessServer, (void*)&nServerNo, sizeof(nServerNo), ZMQ_SNDMORE);
                    if(rc == -1)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "�������ݰ�ʧ��:%s", zmq_strerror(zmq_errno()));
                        continue;
                    }
                    // ��Ҫ����һ���յ���Ϣ֡
                    //zmq_send(m_pSocketAccessServer, "", 0, ZMQ_SNDMORE);

                    rc = zmq_send(pThis->m_pSocketAccessServer, (void*)&send, sizeof(send), 0);
                    if(rc == -1)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "�������ݰ�ʧ��:%s", zmq_strerror(zmq_errno()));
                        continue;
                    }

                    CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_DEBUG, "�������ݰ��ɹ�[datalen:%d]", rc);					
        }
    }

    CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ�߳̽���");
    return 0 ;
}
#else
void* CZmqServer::Proc( LPVOID lpParam )
{
    CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ZMQ�߳̿���");

    CZmqServer *pThis = (CZmqServer *)lpParam;

    CConfigInfo     dbConfig;
    char *memcached_addr = dbConfig.GetMemcachedAddr();
    int memcached_port = dbConfig.GetMemcachedPort();

    ImMemCache memcached = ImMemCache();
    if(memcached.init_ImMemcached(memcached_addr,memcached_port) < 0)
    {
        CLog::Log("CZmqServer::Proc",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
        return NULL;
    }
    CGlobalBusi globalbusi(CAppManager::GetInstance().GetGlobalBusiControl(),&memcached);

    // Initialize poll set

    zmq_pollitem_t items [] =
    {
        { pThis->m_pSocketImServer, 0, ZMQ_POLLIN, 0 },
        { pThis->m_pSocketPull, 0, ZMQ_POLLIN, 0 }
    };

    // Process messages from both sockets
    while (1)
    {
        zmq_poll (items, 2, -1);
        if (items [0].revents & ZMQ_POLLIN)
        {
			static char szData[2048] = {0};
			size_t nDataLen = 0;
			int64_t more;
			size_t more_size = sizeof more;
			do {
    			/* Create an empty ?MQ message to hold the message part */
    			zmq_msg_t part;
    			zmq_msg_init (&part);
    			/* Block until a message is available to be received from socket */
    			zmq_msg_recv (&part, items [0].socket, 0);
    			memset(szData, 0, sizeof(szData));
    			nDataLen = zmq_msg_size(&part);
    			memcpy(szData, zmq_msg_data(&part), nDataLen);
    			if(nDataLen != sizeof(RecvDataStruct))
            	{
            		if(nDataLen!=4)
                		CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "aa:����AccessServer��Ϣʧ��.��Ϣ����:%d", nDataLen);
            	}
            	else
            	{
            		RecvDataStruct *pRecvDataStruct = (RecvDataStruct *)szData;
               	 	CAppManager::GetInstance().GetAppNetWork().ImSvrP2PRecvData(NE_RECVDATA, pRecvDataStruct);
            	}
    			/* Determine if more message parts are to follow */
    			zmq_getsockopt (items [0].socket, ZMQ_RCVMORE, &more, &more_size);
    			zmq_msg_close (&part); } while (more);
/*
        
            // ��һ��Ϊ���շ�Ψһ��ʶ�����˵�
            static char szData[2048] = {0};
            memset(szData, 0, sizeof(szData));
            int nLength = zmq_recv (pThis->m_pSocketImServer, (void*)szData, 2048, ZMQ_SNDMORE);

            if(nLength != sizeof(RecvDataStruct))
            {
            	if(nLength!=4)
                	CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "aa:����AccessServer��Ϣʧ��.��Ϣ����:%d", nLength);
            }
            else
            {
                RecvDataStruct *pRecvDataStruct = (RecvDataStruct *)szData;
                CAppManager::GetInstance().GetAppNetWork().ImSvrP2PRecvData(NE_RECVDATA, pRecvDataStruct);
            }

            // �ڶ���Ϊ������������
            // �յ����������������������
            //CLog::Log("CZmqServer::Proc", CLog::TYPE_IMPORTANT, "ImServer ���յ� AccessServer �����������ݰ�");
            //static char szData[10240] = {0};
            memset(szData, 0, sizeof(szData));
            nLength = zmq_recv (pThis->m_pSocketImServer, (void*)szData, 2048, 0);
            if(nLength != sizeof(RecvDataStruct))
            {
            	if(nLength!=4)
                	CLog::Log("CZmqServer::Proc", CLog::TYPE_ERROR, "bb:����AccessServer��Ϣʧ��.��Ϣ����:%d",nLength);
            }
            else
            {
                RecvDataStruct *pRecvDataStruct = (RecvDataStruct *)szData;
                CAppManager::GetInstance().GetAppNetWork().ImSvrP2PRecvData(NE_RECVDATA, pRecvDataStruct);
            }
           */ 
        }
        if (items [1].revents & ZMQ_POLLIN)
        {
        	CLog::Log("CZmqServer::Proc", CLog::TYPE_DEBUG, "im recv");
			int nLen = 0;
        	SendDataStruct send;
			nLen = zmq_recv(pThis->m_pSocketPull, (void*)&send, sizeof(send), 0);
			if(nLen != sizeof(SendDataStruct))
            {
                continue;
            }

            if(pThis->m_pSocketImServer == NULL)
            {
            	continue;
            }
			
			LPXT_HEAD lpSendHead = (LPXT_HEAD)send.data;
#ifdef IM_USE_MULTIDEVICE
				int nServerNo = pThis->GetRouterAccessServerIP(&globalbusi, lpSendHead->did,lpSendHead->deviceId);
#else
				int nServerNo = pThis->GetRouterAccessServerIP(&globalbusi, lpSendHead->did);
#endif
                    if(nServerNo == 0)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "��ȡAccessServer IPʧ��");
                        continue;
                    }
                    CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_IMPORTANT, "ImServer ���͸� AccessServer[%s]", GetStrIP((uint32)nServerNo).c_str());
                    int rc = zmq_send(pThis->m_pSocketImServer, (void*)&nServerNo, sizeof(nServerNo), ZMQ_SNDMORE);
                    if(rc == -1)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "�������ݰ�ʧ��:%s", zmq_strerror(zmq_errno()));
                        continue;
                    }
                    // ��Ҫ����һ���յ���Ϣ֡
                    //zmq_send(m_pSocketAccessServer, "", 0, ZMQ_SNDMORE);

                    rc = zmq_send(pThis->m_pSocketImServer, (void*)&send, sizeof(send), 0);
                    if(rc == -1)
                    {
                        CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_ERROR, "�������ݰ�ʧ��:%s", zmq_strerror(zmq_errno()));
                        continue;
                    }

                    CLog::Log("Proc_ZmqSendMsg", CLog::TYPE_DEBUG, "�������ݰ��ɹ�[datalen:%d]", sizeof(send));
        }
    }

    CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ�߳̽���");
    return 0 ;
}

#endif


// �յ���Ϣ���ͷ����������������ݰ�
void CZmqServer::OnRecvMsgNotifyServerPacket(const DataPacket &pack)
{
    //int rc = 0;
    const XT_HEAD_PB& head = pack.head();

    switch(head.cmd())
    {
        case SS_CID_MSG_PUSH:
        {
            CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_DEBUG, "SS_CID_MSG_PUSH �յ���Ϣ���Ͱ�");
            if(CAppManager::GetInstance().GetFifoMsgNotify() == NULL)
            {
                CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_ERROR, "��Ϣ����buf�ܵ�Ϊ��");
            }
            else
            {
                if(pack.data().length() < MSGNOTIFY_BUF_LENGTH)
                    CAppManager::GetInstance().GetFifoMsgNotify()->Push((BYTE*)pack.data().c_str(),pack.data().length());
                else
                    CLog::Log("CZmqServer::OnRecvMsgNotifyServerPacket", CLog::TYPE_IMPORTANT, "SS_CID_MSG_PUSH �յ���Ϣ���Ͱ�[������̫��:%d]", pack.data().length());
            }
        }
        break;
        default:
            break;
    }
}

// �յ��ͻ��˴����������ݰ�
void CZmqServer::OnRecvClientPacket(const DataPacket &pack)
{

}

// ��ʼ��
bool CZmqServer::Init()
{
    pthread_t thread ;

    m_pCTX = zmq_ctx_new ();
    if(m_pCTX == NULL)
    {
        CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq����������ʧ��");
        return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq����������ɹ�");

    

#ifdef ACCESS_SERVER
		m_pSocketPull = zmq_socket (m_pCTX, ZMQ_PULL);
		zmq_connect(m_pSocketPull, "inproc://accessserver.rep");

    //zmq_ctx_set(m_pCTX,ZMQ_IO_THREADS,3);
    // ��ȡ����ip,�󶨶˿�
    char ip[32] = {0};
    CAppManager::GetInstance().GetConfig().GetDeviceIp(ip);
    int nPort = CAppManager::GetInstance().GetConfig().GetAccessServerPort();
    m_pSocketAccessServer = zmq_socket (m_pCTX, ZMQ_ROUTER);
    if(m_pSocketAccessServer == NULL)
    {
        CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq���������socket����ʧ��");
        return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq���������socket����ɹ�");
    // ����socketidΪip
    int nServerNo = CAppManager::GetInstance().GetConfig().GetServerNo();
    zmq_setsockopt(m_pSocketAccessServer, ZMQ_IDENTITY,(void*)&nServerNo,sizeof(nServerNo));
    int64_t nMaxMsgCount = 50000;
    zmq_setsockopt(m_pSocketAccessServer, ZMQ_SNDHWM,(void*)&nMaxMsgCount,sizeof(nMaxMsgCount));
    zmq_setsockopt(m_pSocketAccessServer, ZMQ_RCVHWM,(void*)&nMaxMsgCount,sizeof(nMaxMsgCount));

    // ���������¼�����
    int rc = zmq_socket_monitor (m_pSocketAccessServer, "inproc://monitor_accessserver.rep", ZMQ_EVENT_ALL);
    rc = pthread_create (&thread, NULL, rep_socket_monitor, m_pCTX);

    char addr[128] = {0};
    sprintf_s(addr, "tcp://%s:%d", ip, nPort);
    rc = zmq_bind (m_pSocketAccessServer, addr);
    if(rc != 0)
    {
        CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq����������󶨵�ַʧ��[%s]", addr);
        return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq����������󶨵�ַ�ɹ�[%s]", addr);


    // 5561��MsgNotify����
    vector<string> vecMsgNotifyIP;
    CAppManager::GetInstance().GetConfig().GetMsgNotifyIP(vecMsgNotifyIP);
    if(vecMsgNotifyIP.size() == 0)
    {
        CLog::Log("CZmqServer", CLog::TYPE_ERROR, "δ������Ϣ���ͷ�������ַ");
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
            CLog::Log("CZmqServer", CLog::TYPE_ERROR, "ZMQ������Ϣ���ͷ�����[%s] ʧ��", szConn);
            //return NULL;
        }
        else
        {
            CLog::Log("CZmqServer", CLog::TYPE_IMPORTANT, "ZMQ������Ϣ���ͷ�����[%s] �ɹ�", szConn);
        }
    }

    // ����������Ϣ
    const char *filter = "";
    zmq_setsockopt(m_pSocketMsgNotify, ZMQ_SUBSCRIBE, filter, strlen (filter));
#else
	m_pSocketPull = zmq_socket (m_pCTX, ZMQ_PULL);
    zmq_connect(m_pSocketPull, "inproc://imserver.rep");

    //zmq_ctx_set(m_pCTX,ZMQ_IO_THREADS,2);
    // imserver���ӵ����������
    // ��ȡ����������ĵ�ַ
    vector<string> vecIPs;
    CAppManager::GetInstance().GetConfig().GetAccessServerIPs(vecIPs);
    if(vecIPs.size() == 0)
    {
        CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq δ����AccessServer ip");
        return false;
    }
    m_pSocketImServer = zmq_socket (m_pCTX, ZMQ_ROUTER);
    if(m_pSocketImServer == NULL)
    {
        CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq imserver ���� AccessServer socket����ʧ��");
        return false;
    }
    CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq imserver ���� AccessServer socket����ɹ�");

    // ���������¼�����
    int rc = zmq_socket_monitor (m_pSocketImServer, "inproc://monitor_imserver.rep", ZMQ_EVENT_ALL);
    STRU_THREAD_PARAM *pThreadParam = new STRU_THREAD_PARAM();
    pThreadParam->pCtx = m_pCTX;
    pThreadParam->pSocket = m_pSocketImServer;
    rc = pthread_create (&thread, NULL, rep_socket_monitor, pThreadParam);

    // ����socketidΪip
    int nServerNo = CAppManager::GetInstance().GetConfig().GetServerNo();
    zmq_setsockopt(m_pSocketImServer, ZMQ_IDENTITY,(void*)&nServerNo,sizeof(nServerNo));
    int64_t nMaxMsgCount = 50000;
    zmq_setsockopt(m_pSocketImServer, ZMQ_SNDHWM,(void*)&nMaxMsgCount,sizeof(nMaxMsgCount));
    zmq_setsockopt(m_pSocketImServer, ZMQ_RCVHWM,(void*)&nMaxMsgCount,sizeof(nMaxMsgCount));
    char addr[128] = {0};
    int nPort = CAppManager::GetInstance().GetConfig().GetAccessServerPort();
	
    for(size_t i = 0; i < vecIPs.size(); ++i)
    {
        memset(addr, 0, sizeof(addr));
        sprintf_s(addr, "tcp://%s:%d", vecIPs[i].c_str(), nPort);
        rc = zmq_connect(m_pSocketImServer, addr);
        if(rc != 0)
        {
            CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq imserver ���� AccessServer ʧ��[%s]", addr);
            return false;
        }
        CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq imserver ���� AccessServer �ɹ�[%s]", addr);
    }

    /*
	//��ֻ�����Լ���imserver
	char ip[32] = {0};
    CAppManager::GetInstance().GetConfig().GetDeviceIp(ip);
	memset(addr, 0, sizeof(addr));
	sprintf_s(addr, "tcp://%s:%d", ip, nPort);
	rc = zmq_connect(m_pSocketImServer, addr);
	
	if(rc != 0)
	{
		CLog::Log("CZmqServer::Init", CLog::TYPE_ERROR, "zmq imserver ���� AccessServer ʧ��[%s]", addr);
		return false;
	}
	CLog::Log("CZmqServer::Init", CLog::TYPE_DEBUG, "zmq imserver ���� AccessServer �ɹ�[%s]", addr);
	*/
#endif

    //m_nTimerFd = InitTimerfd();
    //CLog::Log("CZmqServer::Init", CLog::TYPE_IMPORTANT, "timerfd [%d]", m_nTimerFd);

    return true;
}

#ifdef  ACCESS_SERVER
// �������ݰ���ImServer������
void CZmqServer::SendPacket(const RecvDataStruct &send)
{
	m_pFifoMsg->Push((BYTE*)&send,(int)sizeof(send));
}

// ��ȡ·�ɵ�ImServer IP
int CZmqServer::GetRouterImServerIP(uint32 userid)
{
/*
	//��ʹ�ñ���ip
	char ip[32] = {0};
	CAppManager::GetInstance().GetConfig().GetDeviceIp(ip);
	return inet_addr(ip);
*/	


#if 1
    // ��ǰ���ӵ�ImServer��ַ
    if(g_mapFD_ImIP.size() == 0)
    {
        // ���е�ImServer������
        CLog::Log("CZmqServer::GetRouterImServerIP", CLog::TYPE_ERROR, "����ImServer������!!!");
        return 0;
    }

    vector<uint32> vecImServerIP;
    auto map_it = g_mapFD_ImIP.begin();
    for(; map_it != g_mapFD_ImIP.end(); ++map_it)
    {
        vecImServerIP.push_back(inet_addr(map_it->second.c_str()));
    }
#else
    vector<uint32> vecImServerIP;
    vector<string> vecIPs;
    CAppManager::GetInstance().GetConfig().GetAccessServerIPs(vecIPs);
    for(int i = 0; i < (int)vecIPs.size(); ++i)
    {
        vecImServerIP.push_back(inet_addr(vecIPs[i].c_str()));
    }
#endif

    int nCount = (int)vecImServerIP.size();
    int index = userid % nCount;
    uint32 nListenAddr = vecImServerIP[index];
    return (int)nListenAddr;
}
#else
// �������ݰ���AccessServer
void CZmqServer::SendPacket(CGlobalBusi *pGlobalBusi, const SendDataStruct &send)
{
	m_pFifoMsg->Push((BYTE*)&send,(int)sizeof(send));
}

// ��ȡ·�ɵ�AccessServer IP
int CZmqServer::GetRouterAccessServerIP(CGlobalBusi *pGlobalBusi, uint32 userid,uint32 deviceId)
{
/*
	//��ʹ�ñ���ip
	char ip[32] = {0};
	CAppManager::GetInstance().GetConfig().GetDeviceIp(ip);
	return inet_addr(ip);
*/	

#if 1
    // ��ǰ���ӵ�AccessServer��ַ
    if(g_mapFD_AccessIP.size() == 0)
    {
        // ���е�ImServer������
        CLog::Log("CZmqServer::GetRouterAccessServerIP", CLog::TYPE_ERROR, "����AccessServer������!!!");
        return 0;
    }

    vector<uint32> vecAccessServerIP;
    auto map_it = g_mapFD_AccessIP.begin();
    for(; map_it != g_mapFD_AccessIP.end(); ++map_it)
    {
        vecAccessServerIP.push_back(inet_addr(map_it->second.c_str()));
    }
#else
    vector<uint32> vecAccessServerIP;
    vector<string> vecIPs;
    CAppManager::GetInstance().GetConfig().GetAccessServerIPs(vecIPs);
    for(int i = 0; i < (int)vecIPs.size(); ++i)
    {
        vecAccessServerIP.push_back(inet_addr(vecIPs[i].c_str()));
    }
#endif
#ifdef IM_USE_MULTIDEVICE
		multiDevice_t tUserInfo;
			
		if(MEMCACHED_GET_INFO_SUCCESS != pGlobalBusi->QueryUserDeviceInfo(userid,deviceId,tUserInfo))
		{
			CLog::Log("CZmqServer::GetRouterImServerIP", CLog::TYPE_ERROR, "��ȡ%u������Ϣʧ��", userid);
			return 0;
		}
#else	
		// ������Ϣ�в��ҵ�AccessServer��IP
		tagGSRV_TUSERINFO_ACK tUserInfo;
		if(MEMCACHED_GET_INFO_SUCCESS != pGlobalBusi->GetTUserInfo(userid,tUserInfo))
		{
			CLog::Log("CZmqServer::GetRouterImServerIP", CLog::TYPE_ERROR, "��ȡ%u������Ϣʧ��", userid);
			return 0;
		}

			
#endif



    // �ж�ԭ����������Ƿ�Ͽ�

    if(std::find(vecAccessServerIP.begin(),vecAccessServerIP.end(),tUserInfo.serverip) != vecAccessServerIP.end())
    {
        return tUserInfo.serverip;
    }
    else
    {
        // ԭ������������ˣ����ȡһ���������������
        srand((unsigned)time(NULL));
        return vecAccessServerIP[rand()%vecAccessServerIP.size()];
    }
}
#endif

void* CZmqServer::ProcZmqPush( LPVOID lpParam )
{
    CZmqServer *pThis = (CZmqServer *)lpParam;
   	
#ifdef ACCESS_SERVER
	pThis->m_pSocketPush = zmq_socket (pThis->m_pCTX, ZMQ_PUSH);
	zmq_bind(pThis->m_pSocketPush, "inproc://accessserver.rep");	
#else
	pThis->m_pSocketPush = zmq_socket (pThis->m_pCTX, ZMQ_PUSH);
    zmq_bind(pThis->m_pSocketPush, "inproc://imserver.rep");
#endif

    while(true)
    {
        char buffer[2048] = {0};
        int nLen;
        pThis->m_pFifoMsg->Pop((BYTE*)buffer, nLen);
        zmq_send(pThis->m_pSocketPush,(void*)buffer,nLen,0);
    }

    return NULL;
}

