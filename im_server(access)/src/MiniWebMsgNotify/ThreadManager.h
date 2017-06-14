#pragma once


#include "PacketBuffer.h"
#include "Business.h"
#include "Utility.h"
//业务线程类，管理多线程处理业务

//业务的类型数
#define BUSINESS_PROC_TYPE_NUM 1

#define PROCESS_LIST_TYPE_A 0

class CThreadManager
{
public:
	CThreadManager(void);
	~CThreadManager(void);

	//启动各个工作线程
	void StartThread();

	//结束工作线程
	void EndThread();

	static void OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
	static void OnReceiveNULL(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData){};

	static void * WINAPI BusinessProcA( LPVOID lpParam );


	CFIFOPacketBuffer* GetBusProcBuf(int nType) { return m_PacketBufferArray + nType;	}

private:
	list<pthread_t>			m_listThreadHandleBuf;

	bool					m_ThreadExitFlag[BUSINESS_PROC_TYPE_NUM];

	CFIFOPacketBuffer		m_PacketBufferArray[BUSINESS_PROC_TYPE_NUM];



	SmartList<tagClientReply> m_stLstRecvReq;	// 缓存最近已收到的ack 包序号，过滤掉重复收到的req包

};
