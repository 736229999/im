#pragma once

#include <map>
using namespace std;

#include "SyncCritical.h"
#include "PacketBuffer.h"
#include "PublicType.h"

//用于调度一个特定协议属于哪个类型的队列，根据协议的不同放到不同
//的处理队列中．判定协议属于哪个队列的基本规则是看该协议处理是否
//有数据库操作或处理时间等


class CRouterProtocol
{
public:

	~CRouterProtocol(void);

	static CRouterProtocol* GetInstance();

	//注册一个协议属于哪类处理队列
	//nProtocolID--处理队列ID
	//nBusinessID--协议ID
	bool Register(int nProtocolID,uint16 nBusinessID);

	//路由一个包到合适的处理队列
	void RouterOnePacket(const RecvDataStruct* pRecvData);

	CFIFOPacketBuffer* GetPacketBufferArray()  { return m_pPacketBufferArray;}

	void SetPacketBufferArray(CFIFOPacketBuffer* pPacketBufferArray)
	{
		m_pPacketBufferArray = pPacketBufferArray;
	}

protected:
		CRouterProtocol(void);
private:
	static CRouterProtocol* m_pInstance;


	map<uint16,int>		m_mapProtocol;
	CSyncCritical		m_Critical;


	CFIFOPacketBuffer*	m_pPacketBufferArray;
	
};

