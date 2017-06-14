#pragma once

#include "xtpacket.h"
#include "AutoID.h"
#include "HashTable.h"



struct ClusterInfo
{
	uint32 id;
	uint32 ServerIp;
	time_t tm;	//����ʱ��
	ClusterInfo* pre;
	ClusterInfo* next;
};

typedef CHashTable<ClusterInfo> ClusterInfoTable;

class CClusterManager
{
public:
	CClusterManager(void);
	~CClusterManager(void);

	//��ѯ���߼�����Ϣ
	bool GetClusterInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//������߼�����Ϣ
	bool AddCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//ɾ�����߼�����Ϣ
	bool DelCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//���ĳ̨IM�������ϵļ���
	bool ClearClusterOnImServer(uint32 serverip);

	void GetClusterFromID(uint32 nId,char* pInfo);


private:
	ClusterInfoTable	m_PermCluster;		//�̶�����
	ClusterInfoTable	m_TempCluster;		//��ʱ����

	ClusterInfo*		m_pClusterListHead;	//�������߼���Ķ���
	CSyncCritical		m_listLock;			//������

	CAutoID				m_AutoID;			//���ڷ�����ʱ����ID

	void AddToClustList(ClusterInfo* pClusterInfo);

};

