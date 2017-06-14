#include "stdafx.h"
#include "BusiMCheck.h"
#include "AppManager.h"
#include "RouterProtocol.h"
#include <time.h>


#ifdef IM_USE_CACHE



ImMemCacheConnCheck::ImMemCacheConnCheck(char* addr,int port)
{
	m_hMMT = NULL;
	brokenFlag = false; //δ�Ͽ���
	memcAddr = addr;
	memcPort = port;
	Memcache = ImMemCache();
	if(Memcache.init_ImMemcached(memcAddr,memcPort) < 0)
	{			
		CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
	}	
}

ImMemCacheConnCheck::~ImMemCacheConnCheck(void)
{
	StopWork();
}

void ImMemCacheConnCheck::StopTimer()
{
	/*if( m_hMMT )
	{
		::timeKillEvent(m_hMMT);
		m_hMMT = NULL;
	}
	*/
	timer_delete(m_timer);
}

void ImMemCacheConnCheck::StopWork()
{
	StopTimer();
}



#define MEMCPERIOD (1000 * 120)//120��



int ImMemCacheConnCheck::StartWorkWebNotify()
{
	StartTimerWebNotify();
	return 0;
}


void ConnProcWebNotify(void* dwUser)
{
	ImMemCacheConnCheck* pObj = (ImMemCacheConnCheck*)dwUser;
	pObj->HandleTimerWebNotify();
}

void ConnProcWebNotifyNew(union sigval v)
{
	ConnProcWebNotify(v.sival_ptr);
}


void ImMemCacheConnCheck::StartTimerWebNotify()
{
/*
	m_hMMT = timeSetEvent(MEMCPERIOD,MEMCPERIOD,ConnProcWebNotify,(void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"������ʱ������ʧ��");
	}
*/
	if(!timeSetEventNew(m_timer, MEMCPERIOD, MEMCPERIOD, ConnProcWebNotifyNew, (void*)this ) )
	{
		CLog::Log("ImMemCacheConnCheck", CLog::TYPE_ERROR, "������ʱ������ʧ��");
	}

}



void ImMemCacheConnCheck::HandleTimerWebNotify()
{
	//ImMemCache &Memcache =  ImMemCache::GetInstance();
	global_Cache *c_memc = Memcache.GetLinks();
	
	if(Memcache.CheckSuccess() < 0) //�������ԣ����Զ�����
	{
		brokenFlag = true;//ֻ�����ö�����־		
	}
	else	//�ָ����ӻ��������ӵ����
	{		
		if(brokenFlag == true)//��������
		{
			if(Memcache.init_ImMemcached_WebNotify(memcAddr,memcPort) < 0) //���Դ��������ӣ���update֮ǰ������
			{				
				CLog::Log("ImMemCacheConnCheck",CLog::TYPE_ERROR,"init_ImMemcached reconnect failure");
			}
			else
			{			
				CLog::Log("ImMemCacheConnCheck",CLog::TYPE_ERROR,"init_ImMemcached reconnect success");
				Memcache.releaseLink(c_memc);	//prevent memory leak	
				brokenFlag= false;
			}
		}
		CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"OK");
	}		
	
}



#endif


