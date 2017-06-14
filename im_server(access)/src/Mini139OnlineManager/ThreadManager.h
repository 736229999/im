#pragma once


/************************************************************************ 
文件描述：业务线程类的声明文件

创建人：hechang
时间：	2006-6-14 
************************************************************************/

#include "PacketBuffer.h"
#include "Business.h"
//业务线程类，管理多线程处理业务

#define BUSINESS_PROC_TYPE_NUM 1

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

	//全局在线管理的处理线程函数
	static void* WINAPI BusinessProcA( LPVOID lpParam );


private:
	list<pthread_t>			m_listThreadHandleBuf;

	bool					m_ThreadExitFlag[BUSINESS_PROC_TYPE_NUM];

	CFIFOPacketBuffer		m_PacketBufferArray[BUSINESS_PROC_TYPE_NUM];
};
