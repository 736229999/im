#pragma once
#include "StdPublic.h"
#include "PublicType.h"
#include "improtocol.pb.h"
#include <string>
#include <map>


// ImServer����һ���̣߳�ʹ��zmq����tcp����
class CGlobalBusi;
class CFIFOPacketBuffer;
class CZmqServer
{
public:
	static CZmqServer* GetInstance();
    int StartWork();
#ifdef	ACCESS_SERVER
	// �������ݰ���ImServer������
	void SendPacket(const RecvDataStruct &send);
	static std::map<uint32_t, std::string> g_mapFD_ImIP;	// ���ӵ��˽����������ImServer 
#else
	// �������ݰ���AccessServer
	void SendPacket(CGlobalBusi *pGlobalBusi, const SendDataStruct &send);
	static std::map<uint32_t, std::string> g_mapFD_AccessIP;	// ���ӵ���ImServer��AccessServer 
#endif
/*
	// ����ʱ������
	void SetTimerSpeed(double dSpeed);
*/
private:
	CZmqServer();
    ~CZmqServer();
	
	static void* Proc( LPVOID lpParam );
	static void* ProcZmqPush( LPVOID lpParam );

	// ��ʼ��
	bool Init();

	// �յ���Ϣ���ͷ����������������ݰ�
	void OnRecvMsgNotifyServerPacket(const DataPacket &pack);

	// �յ��ͻ��˴����������ݰ�
	void OnRecvClientPacket(const DataPacket &pack);


#ifdef ACCESS_SERVER
	// ��ȡ·�ɵ�ImServer IP
	int GetRouterImServerIP(uint32 userid);
#else
	// ��ȡ·�ɵ�AccessServer IP
	int GetRouterAccessServerIP(CGlobalBusi *pGlobalBusi, uint32 userid,uint32 deviceip = 0);
#endif

private:
	static CZmqServer *g_pZmqServer;
	void *m_pCTX;				// zmq�����ģ�һ������ֻ��Ҫ����һ��
#ifdef ACCESS_SERVER
	void *m_pSocketAccessServer;// ��ImServer���ӵ��׽���
	void *m_pSocketMsgNotify;	// ����Ϣ���ͷ��������ӵ��׽���	
#else
	void *m_pSocketImServer;	// ImServer���ӵ�������������׽���
#endif
	CGlobalBusi *m_pGlobalBusi;
	CFIFOPacketBuffer *m_pFifoMsg;			// �����͵���Ϣ
	//public:
	//static bool bTimeStarting = false;

	// ����һ��zmq�ܵ�	
	void *m_pSocketPush;
	void *m_pSocketPull;
};
