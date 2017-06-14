#pragma once

#include "xtpacket.h"
#include "AutoID.h"
#include "HashTable.h"



struct ClusterInfo
{
	uint32 id;
	uint32 ServerIp;
	time_t tm;	//加载时间
	ClusterInfo* pre;
	ClusterInfo* next;
};

typedef CHashTable<ClusterInfo> ClusterInfoTable;

class CClusterManager
{
public:
	CClusterManager(void);
	~CClusterManager(void);

	//查询在线家族信息
	bool GetClusterInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//添加在线家族信息
	bool AddCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//删除在线家族信息
	bool DelCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//清除某台IM服务器上的家族
	bool ClearClusterOnImServer(uint32 serverip);

	void GetClusterFromID(uint32 nId,char* pInfo);


private:
	ClusterInfoTable	m_PermCluster;		//固定家族
	ClusterInfoTable	m_TempCluster;		//临时家族

	ClusterInfo*		m_pClusterListHead;	//所有在线家族的队列
	CSyncCritical		m_listLock;			//队列锁

	CAutoID				m_AutoID;			//用于分配临时家族ID

	void AddToClustList(ClusterInfo* pClusterInfo);

};

