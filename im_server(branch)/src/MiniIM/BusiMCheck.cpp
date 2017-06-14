#include "stdafx.h"
#include "BusiMCheck.h"
#include "AppManager.h"
#include "RouterProtocol.h"
#include <time.h>


#ifdef IM_USE_CACHE



ImMemCacheConnCheck::ImMemCacheConnCheck(char* addr,int port)
{
	m_hMMT = NULL;
	brokenFlag = false;  //δ�Ͽ���
	memcAddr = addr;
	memcPort = port;
	imCheckFlag = false;
	
}

ImMemCacheConnCheck::~ImMemCacheConnCheck(void)
{
	StopWork();
}

int ImMemCacheConnCheck::StartWork()
{
	StartTimer();
	return 0;
}

void ImMemCacheConnCheck::StopWork()
{
	StopTimer();
}

void ConnProc(void* dwUser)
{
	ImMemCacheConnCheck* pObj = (ImMemCacheConnCheck*)dwUser;
	pObj->HandleTimer();
}

void ConnProcNew(union sigval v)
{
	//CLog::Log(__FUNCTION__,CLog::TYPE_DEBUG, "timer:1");
	ConnProc(v.sival_ptr);
}



#define MEMCPERIOD (1000 * 120)//120��
void ImMemCacheConnCheck::StartTimer()
{
	/*
	m_hMMT = timeSetEvent(MEMCPERIOD,MEMCPERIOD,ConnProc,(void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("ImMemCacheConnCheck",CLog::TYPE_ERROR,"������ʱ������ʧ��");
	}
	*/
	if(!timeSetEventNew(m_timer, MEMCPERIOD, MEMCPERIOD, ConnProcNew, (void*)this ) )
	{
		CLog::Log("ImMemCacheConnCheck", CLog::TYPE_ERROR, "������ʱ������ʧ��");
	}
}


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
				CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"init_ImMemcached reconnect success");
				Memcache.releaseLink(c_memc);	//prevent memory leak	
				brokenFlag= false;
			}			
		}
		CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"OK");
	}	
	return;
	
}

void ImMemCacheConnCheck::StopTimer()
{
	/*
	if( m_hMMT )
	{
		::timeKillEvent(m_hMMT);
		m_hMMT = NULL;
	}
	*/
	timer_delete(m_timer);
}

void ImMemCacheConnCheck::HandleTimer()
{
	if(imCheckFlag == false)
	{
		Memcache = ImMemCache();
		if(Memcache.init_ImMemcached(memcAddr,memcPort) < 0)
		{			
			CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"init_ImMemcached fail");
		}
		imCheckFlag = true;
	}
	
	global_Cache *c_memc = Memcache.GetLinks();

	if(Memcache.CheckSuccess() < 0) //�������ԣ����Զ�����
	{
		brokenFlag = true;//ֻ�����ö�����־		
	}
	else	//�ָ����ӻ��������ӵ����
	{		
		if(brokenFlag == true)//��������
		{
			if(Memcache.init_ImMemcached(memcAddr,memcPort) < 0) //���Դ��������ӣ���update֮ǰ������
			{				
				CLog::Log("ImMemCacheConnCheck",CLog::TYPE_ERROR,"init_ImMemcached reconnect failure");
				return;
			}
			else
			{			
				CLog::Log("ImMemCacheConnCheck",CLog::TYPE_ERROR,"init_ImMemcached reconnect success");
				Memcache.releaseLink(c_memc);	//prevent memory leak	
				brokenFlag= false;				
/*	������redis�����ָ�������������û��ڴ治��imserverά��			
				//�ָ��󣬽�����������ͬ����memcached��ȥ
				CUser *pUser = NULL;
				int nCheckUserNum = 0;
				for(__gnu_cxx::hash_map<uint32,CUser*>::iterator it =  CUser::m_userhash.begin();it != CUser::m_userhash.end(); ++ it)
				{
					pUser = it->second;
					if (!pUser)
						continue;
					if ( nCheckUserNum > 30000)
					{
						break;
					}
					++nCheckUserNum;
					Memcache.RecoverUsers(pUser);
					CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"update user %u",pUser->m_nID);				
				}
*/				
			}
		}
		
		CLog::Log("ImMemCacheConnCheck",CLog::TYPE_IMPORTANT,"OK");
	}		
	
}


#endif


