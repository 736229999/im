#pragma once
#include "StdPublic.h"
#include "PublicType.h"
#include "improtocol.pb.h"
#include <string>
#include <map>


// ImServer开启一个线程，使用zmq接收tcp数据
class CGlobalBusi;
class CFIFOPacketBuffer;
class CZmqServer
{
public:
	static CZmqServer* GetInstance();
    int StartWork();
#ifdef	ACCESS_SERVER
	// 发送数据包给ImServer服务器
	void SendPacket(const RecvDataStruct &send);
	static std::map<uint32_t, std::string> g_mapFD_ImIP;	// 连接到此接入服务器的ImServer 
#else
	// 发送数据包给AccessServer
	void SendPacket(CGlobalBusi *pGlobalBusi, const SendDataStruct &send);
	static std::map<uint32_t, std::string> g_mapFD_AccessIP;	// 连接到此ImServer的AccessServer 
#endif
/*
	// 设置时钟速率
	void SetTimerSpeed(double dSpeed);
*/
private:
	CZmqServer();
    ~CZmqServer();
	
	static void* Proc( LPVOID lpParam );
	static void* ProcZmqPush( LPVOID lpParam );

	// 初始化
	bool Init();

	// 收到消息推送服务器传过来的数据包
	void OnRecvMsgNotifyServerPacket(const DataPacket &pack);

	// 收到客户端传过来的数据包
	void OnRecvClientPacket(const DataPacket &pack);


#ifdef ACCESS_SERVER
	// 获取路由的ImServer IP
	int GetRouterImServerIP(uint32 userid);
#else
	// 获取路由的AccessServer IP
	int GetRouterAccessServerIP(CGlobalBusi *pGlobalBusi, uint32 userid,uint32 deviceip = 0);
#endif

private:
	static CZmqServer *g_pZmqServer;
	void *m_pCTX;				// zmq上下文，一个进程只需要创建一个
#ifdef ACCESS_SERVER
	void *m_pSocketAccessServer;// 供ImServer连接的套接字
	void *m_pSocketMsgNotify;	// 与消息推送服务器连接的套接字	
#else
	void *m_pSocketImServer;	// ImServer连接到接入服务器的套接字
#endif
	CGlobalBusi *m_pGlobalBusi;
	CFIFOPacketBuffer *m_pFifoMsg;			// 待发送的消息
	//public:
	//static bool bTimeStarting = false;

	// 申请一个zmq管道	
	void *m_pSocketPush;
	void *m_pSocketPull;
};
