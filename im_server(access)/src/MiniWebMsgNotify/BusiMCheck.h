/************************************************************************ 
 文件描述：处理心跳业务，客户端心跳检测
 
 创建人： Du-Yong 20150827
 
 修改记录： 
            
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
	bool brokenFlag; //判断曾经是否断链过
private:
	PMMRESULT m_hMMT;	
	timer_t m_timer;
	ImMemCache Memcache;
};

#endif

