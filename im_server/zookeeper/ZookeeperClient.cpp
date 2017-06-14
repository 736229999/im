#ifdef IM_USE_ZOOKEEPER

#include "stdafx.h"
#include <stdlib.h>
#include "./src/MiniIM/AppManager.h"
#include "zookeeper.h"
#include "ZookeeperClient.h"
#include "./src/MiniIM/UserManager.h"

list<uint32> CZookeeper::m_downServer(0);
CMemCBusi CZookeeper::m_MemCBusi;

CZookeeper::CZookeeper()
{
	
}
CZookeeper::~CZookeeper()
{
	

}

int CZookeeper::StartWork()
{
	CreateThread( 
		NULL,						// default security attributes 
		0,							// use default stack size  
		ZookeeperStart,				// thread function 
		this,						// argument to thread function 
		0,							// use default creation flags 
		NULL);						// returns the thread identifier 

	return RESULT_SUCCESS;
}

//To judge whether this node is leader or not.
int CZookeeper::is_leader( zhandle_t* zkhandle, char *myid)
{
    int ret = 0;
    int flag = 1;

    struct String_vector strings;
    ret = zoo_get_children(zkhandle, "/bwwan", 0, &strings);
    if (ret) 
	{
		CLog::Log("is_leader",CLog::TYPE_ERROR,"Error %d for get_children",ret);
        return -1;
    }
    /* 计数 */
    for (int i = 0;  i < strings.count; i++) 
	{
        if (strcmp(myid, strings.data[i]) > 0) 
		{
            flag = 0;
            break;
        }
    }
    return flag;
}

void CZookeeper::get_node_name(const char *buf, char *node)
{
    const char *p = buf;
    int i;
    for (i = strlen(buf) -1; i >= 0; i--) 
	{
        if (*(p + i) == '/') 
		{
            break;
        }
    }
    strcpy(node, p + i + 1);
	CLog::Log("get_node_name",CLog::TYPE_DEBUG,"node:%s",node);
    return;
}

void CZookeeper::election_children_watcher(zhandle_t* zh, int type, int state,const char* path, void* watcherCtx)
{
    int ret = 0;
	CConfigInfo m_ConfigInfo;

    struct watch_func_para_t* para= (struct watch_func_para_t*)watcherCtx;
    struct String_vector strings;
    struct Stat stat;

    /* 重新监听 */
    ret = zoo_wget_children2(para->zkhandle, "/bwwan", election_children_watcher,
                             watcherCtx, &strings, &stat);
    if (ret) 
	{
		CLog::Log("election_children_watcher",CLog::TYPE_ERROR,"child: zoo_wget_children2 error:%d",ret);
        return;
    }
	CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"Imserver's state is changed!");

	//遍历serverlist,得到downserver
	list < uint32 > pZooList;
	m_ConfigInfo.GetServerList(&pZooList);
	list<uint32>::iterator it = pZooList.begin();
	for(;it != pZooList.end(); it++)
	{
		int flag = 0;
		for(int i = 0;  i < strings.count; i++)
		{
			CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"count:%d, data:%s", strings.count,strings.data[i]);
			struct in_addr addr;		
			char buf[64]= {0}; 
			char bufall[64]= {0}; 
			strcpy(buf,strings.data[i]);
			char buffer2[64]={0};
			int bufferlen2=sizeof(buffer2);
			sprintf(bufall,"/bwwan/%s",buf);
			int flag2=zoo_get(para->zkhandle,bufall,0,buffer2,&bufferlen2,NULL);  
			char intchar[64]={0};
			addr.s_addr = *it;
			strcpy(intchar,inet_ntoa(addr));
			CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"the server:%u,intchar:%s,bufall:%s,buffer2:%s",*it,intchar,bufall,buffer2);
			if (flag2 == ZOK && strcmp(intchar,buffer2)== 0)  
			{  
				flag = 1;
				CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"the node's value:%s\n",buffer2);  
				break;
			} 
		}
		if(flag == 0)
		{
			CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"The DownServer:%u",*it);
			m_downServer.push_back(*it);
		}
	}
	//向memcache 查询downserver的在线用户
	tagGSRV_GET_ONLINE_FULLINFO_ACK::UserInfo DownSerUserInfo[20];
	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY::UserInfo NewSerUserInfo[20];
	int nResult;
	uint8 m_nReturnCount = 20;
	uint32 nTotalOnlineNum = 0;
	uint32 number=0;
	uint32 startpos =0;
	CUserManager *pUserManager = CAppManager::GetInstance().GetUserManager();
	list<uint32>::iterator ip = m_downServer.begin();
	 /* 判断主从 */
	 ret = is_leader(para->zkhandle, para->node);
    if (ret == 1)
    {
		CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"This is [%s], i am a leader",para->node);
		for(;ip !=	m_downServer.end(); ip++)
		{
			number = 0;
			do
			{
				nResult = m_MemCBusi.SearchDownSerUserinfo(DownSerUserInfo,m_nReturnCount,nTotalOnlineNum,startpos,*ip);
				
				CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,
					"SearchDownSerUserin number:%d, m_nReturnCount:%d, nTotalOnlineNum:%d, startpos:%d, *ip:%u, nResult:%d", 
					number,m_nReturnCount,nTotalOnlineNum,startpos,*ip,nResult); 
				if(RESULT_SUCCESS == nResult)
				{
					for(uint32 num = 0;num < m_nReturnCount; ++num)
					{
						CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"m_nReturnCount:%d",m_nReturnCount);
						int m_uid;
						m_uid = DownSerUserInfo[num].id;
						
						CUser * pUser = pUserManager->Find(m_uid);
						CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"Find:%d",m_uid);
						
						//将downserver的用户信息写本地
						if (!pUser)
						{
							CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"pUser == NULL");
							pUser = pUserManager->AddHash(m_uid);
							pUser->Reset();
							pUser->m_nID = m_uid;
							pUser->AddToOnlineList();
							SysStat::GetInstance().addOnlineNum();
							pUser->m_nID = DownSerUserInfo[num].id;
							pUser->m_nStatus= DownSerUserInfo[num].status;
							pUser->m_AllowRule= DownSerUserInfo[num].rule;
							pUser->m_ClientVersion = DownSerUserInfo[num].version;
							strcpy(pUser->m_szUserName,DownSerUserInfo[num].username);
							strcpy(pUser->m_szNickName,DownSerUserInfo[num].nickname);
							pUser->m_nIP = DownSerUserInfo[num].clientip;
							pUser->m_nPort = DownSerUserInfo[num].clientport;
							pUser->m_nSexy= DownSerUserInfo[num].sexy;
							pUser->m_nflow = DownSerUserInfo[num].flow;
							pUser->m_nServerIp = m_ConfigInfo.GetDeviceIp();
							pUser->m_nLanIP = DownSerUserInfo[num].lanip;
							strcpy(pUser->m_SessionKey,DownSerUserInfo[num].sessionkey);
							strcpy(pUser->m_P2PSessionKey,DownSerUserInfo[num].p2psessionkey);
							pUser->flag = DownSerUserInfo[num].flag;
							CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"has infomation");
							CUser * pUser = pUserManager->Find(m_uid);
							if(pUser)
							{
								CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"write %d user in local server success",m_uid);
								//删除memcache中旧server的用户信息
								int	result= m_MemCBusi.DelUserInfo(m_uid,*ip);
								if(result)
								{
									CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"delete %d user downserver:%u memcache success",m_uid,*ip);

								}
								else
								{
									CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"delete %d user downserver:%u memcache fail",m_uid,*ip);
								}
								//将用户信息加新server写入memcache
								NewSerUserInfo[num].id = DownSerUserInfo[num].id;
								NewSerUserInfo[num].status = DownSerUserInfo[num].status;
								NewSerUserInfo[num].rule= DownSerUserInfo[num].rule;
								strcpy(NewSerUserInfo[num].username,DownSerUserInfo[num].username);
								strcpy(NewSerUserInfo[num].nickname,DownSerUserInfo[num].nickname);
								NewSerUserInfo[num].clientip = DownSerUserInfo[num].clientip;
								NewSerUserInfo[num].clientport = DownSerUserInfo[num].clientport;
								NewSerUserInfo[num].sexy= DownSerUserInfo[num].sexy;
								NewSerUserInfo[num].flow= DownSerUserInfo[num].flow;
								NewSerUserInfo[num].serverip = m_ConfigInfo.GetDeviceIp();
								NewSerUserInfo[num].lanip = DownSerUserInfo[num].lanip;
								strcpy(NewSerUserInfo[num].sessionkey,DownSerUserInfo[num].sessionkey);
								strcpy(NewSerUserInfo[num].p2psessionkey,DownSerUserInfo[num].p2psessionkey);
								NewSerUserInfo[num].flag = DownSerUserInfo[num].flag;
								
								
								result= m_MemCBusi.AddModiUserInfo(NewSerUserInfo[num]);
								if(result)
								{
									CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"add %d usermemcache success",m_uid);

								}
								else
								{
									CLog::Log("election_children_watcher",CLog::TYPE_ERROR,"add %d usermemcache fail",m_uid);
								}
								
							}
							else
							{
								CLog::Log("election_children_watcher",CLog::TYPE_ERROR,"write %d user local fail",m_uid);
							}	
						}
						else
						{
							CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"Local server arleady has userinfo:%d",m_uid);
						}

					}
					startpos += m_nReturnCount;

					CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"count = 0");
				}
				else
				{
					CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"selfserver has userinfo");
					break;
				}
				number += m_nReturnCount;
			}while( number < nTotalOnlineNum && m_nReturnCount != 0);
		}
    }
    else if(ret == 0)
	{
		CLog::Log("election_children_watcher",CLog::TYPE_DEBUG,"This is [%s], i am a follower",para->node);
    }
	else
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"Get node Error!");
		return;		
	}
    return;
}

void CZookeeper::def_election_watcher(zhandle_t* zh, int type, int state,const char* path, void* watcherCtx)
{
	CLog::Log("def_election_watcher",CLog::TYPE_DEBUG,"Something happened! type:%d, state:%d, path:%s, watcherCtx:%s",
		type, state, path, (char *)watcherCtx);
}

void* CZookeeper::ZookeeperStart(LPVOID lpParam)
{
	CConfigInfo	m_ConfigInfo;
	char host[48]={0};
	char* zookeeperserver_addr = m_ConfigInfo.GetZookeeperAddr();
	int zookeeperserver_port = m_ConfigInfo.GetZookeeperPort();
	sprintf(host,"%s:%d",zookeeperserver_addr,zookeeperserver_port);
    zhandle_t* zkhandle;
    int timeout = 5000;
	char ParentsValue[32] = {0};
	char ChildrenValue[32] = {0};
    char node[32] = {0};
	char title[32] = "hello!Zookeeper!";
	char deviceip[20] = {0};
	m_ConfigInfo.GetDeviceIp(deviceip);
    zoo_set_debug_level(ZOO_LOG_LEVEL_DEBUG);
	CLog::Log("ZookeeperStart",CLog::TYPE_DEBUG,"deviceip:%s",deviceip);
	//初始化zookeeper
    zkhandle = zookeeper_init(host, def_election_watcher, timeout, 0, title, 0);
    if (zkhandle == NULL) 
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"Connecting to zookeeper servers error…");
		return NULL;
    }
	//创建父节点
	int ret = zoo_create(zkhandle,
								"/bwwan",
								"partents",
								8,
								&ZOO_OPEN_ACL_UNSAFE,
								0,
								ParentsValue,
								sizeof(ParentsValue)-1);
	
	if((ret == ZNODEEXISTS) || (ret == ZOK))
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_DEBUG,"zoo_create parents success:%d",ret);	
	}
	else
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"zoo_create parents error:%d",ret);
		return NULL;
	}	
	char nodeName[30]={0};
	
	sprintf(nodeName,"/bwwan/%s",deviceip);
    /* 在父节点下创建member节点 */
    ret = zoo_create(zkhandle,
                     nodeName,
                     deviceip,
                     30,
                     &ZOO_OPEN_ACL_UNSAFE,  /* a completely open ACL */
                     /*ZOO_SEQUENCE|*/ZOO_EPHEMERAL,
                     ChildrenValue,
                     sizeof(ChildrenValue)-1);
    if (ret) 
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"zoo_create error:%d",ret);
		return NULL;
    }

    get_node_name(ChildrenValue, node);
    /* 判断当前是否是Leader节点 */
	ret = is_leader(zkhandle, node);
    if (ret == 1) 
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_DEBUG,"This is:%s, ChildrenValue:%s, i am a leader,first",node, ChildrenValue);
		
    }
	else if(ret == 0)
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_DEBUG,"This is:%s, ChildrenValue:%s, i am a follower,first",node, ChildrenValue);
    }
	else
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"Get node Error!");
		return NULL;
	}

    struct Stat stat;
    struct String_vector strings;
    struct watch_func_para_t para;
    memset(&para, 0, sizeof(para));
    para.zkhandle = zkhandle;
    strcpy(para.node, node);

    /* 监视/bwwan的所有子节点事件 */
    ret = zoo_wget_children2(zkhandle, "/bwwan", election_children_watcher, &para, &strings, &stat);
    if (ret) 
	{
		CLog::Log("ZookeeperStart",CLog::TYPE_ERROR,"zoo_wget_children2 error:%d",ret);
        return NULL;
    }

    /* just wait for experiments*/
    sleep(10000);

    zookeeper_close(zkhandle);
	return NULL;
}

#endif
