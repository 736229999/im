#pragma once


/************************************************************************ 
�ļ�������ҵ���߳���������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-10 
************************************************************************/

#include "PacketBuffer.h"
#include "Business.h"
#include "Utility.h"
#include "SysStatic.h"
//ҵ���߳��࣬������̴߳���ҵ��

//ҵ���������
#define BUSINESS_PROC_TYPE_NUM 7

#define PROCESS_LIST_TYPE_A 0
#define PROCESS_LIST_TYPE_B 1
#define PROCESS_LIST_TYPE_C 2
#define PROCESS_LIST_TYPE_D 3
#define PROCESS_LIST_TYPE_E 4
#define PROCESS_LIST_TYPE_F 5
#define PROCESS_LIST_TYPE_G 6



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
	static void OnReceiveNULL(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData){ printf("OnReceiveNULL\n"); };

	//����������Ҫ�����ݿ�򽻵���ҵ��
	static void* WINAPI BusinessProcA( LPVOID lpParam );

	//�������в���Ҫ�����ݿ�򽻵���ҵ��
	static void*  WINAPI BusinessProcB( LPVOID lpParam );

	//����������Ҫ�ļ�����򽻵���ҵ��
	static void*  WINAPI BusinessProcC( LPVOID lpParam );

	//ͳ���߳�
	static void*  WINAPI BusinessProcD( LPVOID lpParam );
	
	static void*  WINAPI BusinessProcE( LPVOID lpParam );

	static void*  WINAPI BusinessProcF( LPVOID lpParam );

	static void*  WINAPI BusinessProcG( LPVOID lpParam );

	CFIFOPacketBuffer* GetBusProcBuf(int nType) { return m_PacketBufferArray + nType;	}

private:
	list<pthread_t>			m_listThreadHandleBuf;

	bool					m_ThreadExitFlag[BUSINESS_PROC_TYPE_NUM];

	CFIFOPacketBuffer		m_PacketBufferArray[BUSINESS_PROC_TYPE_NUM];

	SmartList<tagClientReply> m_stLstRecvReq;	// ����������յ���ack ����ţ����˵��ظ��յ���req��
	
};
