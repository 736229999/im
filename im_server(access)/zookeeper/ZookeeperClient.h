#ifdef IM_USE_ZOOKEEPER
#pragma once

#include "zookeeper.h"
#include "GlobalBusi.h"
#include "SyncCritical.h"

#include "DataBaseLib.h"
#include "../include/ClientProtocol.h"

class CZookeeper
{
public:
	CZookeeper();
	virtual ~CZookeeper(void);

	int StartWork();
	static int is_leader(zhandle_t* zkhandle, char *myid);

	static void get_node_name(const char *buf, char *node);
	static void election_children_watcher(zhandle_t* zh, int type, int state,const char* path, void* watcherCtx);
	static void def_election_watcher(zhandle_t* zh, int type, int state,const char* path, void* watcherCtx);
	static void* ZookeeperStart(LPVOID lpParam);
	
	
private:
	CGlobalBusi *m_pGlobalBusi;
	static CMemCBusi m_MemCBusi;
	//CConfigInfo m_ConfigInfo;
	struct watch_func_para_t 
	{
    	zhandle_t *zkhandle;
   		char node[64];
	};
	static list<uint32>			m_downServer;
	
};
#endif

