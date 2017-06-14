#pragma once


/************************************************************************ 
�ļ�������ҵ���߳���������ļ�

�����ˣ�hechang
ʱ�䣺	2006-6-14 
************************************************************************/

#include "PacketBuffer.h"
#include "Business.h"
//ҵ���߳��࣬������̴߳���ҵ��

#define BUSINESS_PROC_TYPE_NUM 1

class CThreadManager
{
public:
	CThreadManager(void);
	~CThreadManager(void);

	
	//�������������߳�
	void StartThread();

	//���������߳�
	void EndThread();

	static void OnReceive(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);

	//ȫ�����߹���Ĵ����̺߳���
	static void* WINAPI BusinessProcA( LPVOID lpParam );


private:
	list<pthread_t>			m_listThreadHandleBuf;

	bool					m_ThreadExitFlag[BUSINESS_PROC_TYPE_NUM];

	CFIFOPacketBuffer		m_PacketBufferArray[BUSINESS_PROC_TYPE_NUM];
};
