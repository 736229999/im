#include "SSDBConnCheck.h"
#include "SSDBConn.h"
#include "../common/Log.h"

SSDBConnCheck::SSDBConnCheck(char* addr)
{
	m_hMMT = NULL;
	ssdbAddr = addr;
}

SSDBConnCheck::~SSDBConnCheck(void)
{
	StopWork();
}

int SSDBConnCheck::StartWork()
{
	StartTimer();
	return 0;
}

void SSDBConnCheck::StopWork()
{
	StopTimer();
}

void ConnProc(void* dwUser)
{
	SSDBConnCheck* pObj = (SSDBConnCheck*)dwUser;
	pObj->HandleTimer();
}

#define SSDBPERIOD (1000 * 10)//10Ãë
void SSDBConnCheck::StartTimer()
{
	m_hMMT = timeSetEvent(SSDBPERIOD,SSDBPERIOD,ConnProc,(void*)this,TIME_PERIODIC);
	if( NULL == m_hMMT )
	{
		CLog::Log("CHeartTickCheck",CLog::TYPE_ERROR,"ĞÄÌø¶¨Ê±Æ÷ÉèÖÃÊ§°Ü");
	}
}

void SSDBConnCheck::StopTimer()
{
	if( m_hMMT )
	{
		::timeKillEvent(m_hMMT);
		m_hMMT = NULL;
	}
}

void SSDBConnCheck::HandleTimer()
{
	SSDBLinks& ssdbConn = SSDBLinks::GetInstance();

	ssdbLink_Map& Links = ssdbConn.GetLinks();

	ssdbLink_Map::iterator iter;
	CAutoLock mylock(&Links.m_mapLock);

	for(iter = Links.begin(); iter != Links.end(); ++ iter)
	{
		if(iter->second->l == NULL)
		{
			CLog::Log("SSDBConnCheck",CLog::TYPE_IMPORTANT,"SSDB[%d] reconnecting...",iter->first);
			
			ssdb::Client* link = NULL;
			link = ssdb::Client::connect(ssdbAddr, PORT_SSDB);
			if(link == NULL)
			{
				CLog::Log("SSDBConnCheck",CLog::TYPE_ERROR,"SSDB[%d] reconnect fail",iter->first);
			}	
			else
			{
				iter->second->l = link;
				CLog::Log("SSDBConnCheck",CLog::TYPE_IMPORTANT,"SSDB[%d] reconnect OK",iter->first);
			}	
		}					
	}
}

