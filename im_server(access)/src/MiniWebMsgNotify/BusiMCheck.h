/************************************************************************ 
 �ļ���������������ҵ�񣬿ͻ����������
 
 �����ˣ� Du-Yong 20150827
 
 �޸ļ�¼�� 
            
************************************************************************/

#pragma once


#ifdef IM_USE_CACHE

#include "GlobalBusi.h"

class ImMemCacheConnCheck
{
public:
	ImMemCacheConnCheck(char* addr,int port);
	~ImMemCacheConnCheck();	
	int StartWorkWebNotify();
	void StopWork();
	
private:
	friend void  ConnProc(void* dwUser);

	void StartTimer();
	void StopTimer();
	
	friend void ConnProcWebNotifyNew(union sigval v);
	friend void ConnProcWebNotify(void* dwUser);
	void StartTimerWebNotify();
	void HandleTimerWebNotify();
	
	char* memcAddr;
	int  memcPort;
	bool brokenFlag; //�ж������Ƿ������
private:
	PMMRESULT m_hMMT;	
	timer_t m_timer;
	ImMemCache Memcache;
};

#endif

