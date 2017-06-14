#include "stdafx.h"
#include "ClusterManager.h"
#include "SrvFrameProtocol.h"
#include "protocol.h"
#include <time.h>

CClusterManager::CClusterManager(void)
{
	m_pClusterListHead = NULL;
}

CClusterManager::~CClusterManager(void)
{
}


//���ؼ�����Ϣ
bool CClusterManager::GetClusterInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	GSRV_FAMILYINFO_QUERY* pHead = (GSRV_FAMILYINFO_QUERY*)RecvPacket.GetData();
	GSRV_FAMILYINFO_QUERY_ACK* pAckHead = (GSRV_FAMILYINFO_QUERY_ACK*)SendPacket.GetData();

	ClusterInfoTable* pClusterInfoTable;

	if( pHead->type == FAMILYINFO_QUERY_TYPE_PERM )	//�̶�����
	{
		pClusterInfoTable = &m_PermCluster;
	}
	else if( pHead->type == FAMILYINFO_QUERY_TYPE_TEMP ) //��ʱ����
	{
		pClusterInfoTable = &m_TempCluster;
	}
	else
	{
		CLog::Log("CClusterManager::GetClusterInfo",CLog::TYPE_ERROR,"��⵽�Ƿ���������:%d!",pHead->type);
		return false;
	}
	
	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	int nReturnNum = pHead->count;
	ClusterInfo* pClusterInfo = NULL;
	uint32 nClusterId = 0;
	int i = 0;
	for( i = 0 ; i < nReturnNum ; ++i )
	{
		nClusterId = pHead->fid[i];
		SendPacket << nClusterId;
		pClusterInfo = pClusterInfoTable->Find(nClusterId);
		if( NULL == pClusterInfo)
		{
			SendPacket << (uint32)0;
		}
		else
		{
			SendPacket << pClusterInfo->ServerIp;
		}

		if( !SendPacket.HasData(sizeof(pAckHead->FamilyInfo) ) )
		{
			break;
		}
	}

	//����ϸ���
	pAckHead->count = i;


	return true;
}


//������߼�����Ϣ
bool CClusterManager::AddCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket )
{

	GSRV_FAMILYINFO_ADD_QUERY* pHead = (GSRV_FAMILYINFO_ADD_QUERY*)RecvPacket.GetData();
	GSRV_FAMILYINFO_ADD_ACK*   pAckHead = (GSRV_FAMILYINFO_ADD_ACK*)SendPacket.GetData();

	ClusterInfo* pClusterInfo = NULL;
	if( pHead->type == FAMILYINFO_QUERY_TYPE_PERM )	//�̶�����
	{

		SendPacket << (uint8)0;			//�ɹ���־
		SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

		int nReturnNum = pHead->count;
		uint32 nClusterId = 0;
		int i = 0;			
		for(  i = 0 ; i < nReturnNum ; ++i )
		{
			nClusterId = pHead->FamilyInfo[i].FamilyID;
			
			pClusterInfo = m_PermCluster.Find(nClusterId);
			if( NULL == pClusterInfo )
			{
				pClusterInfo = m_PermCluster.AddHash(nClusterId);
				pClusterInfo->id = nClusterId;
				pClusterInfo->ServerIp = pHead->FamilyInfo[i].ServerIp;
				time(&pClusterInfo->tm);
				AddToClustList(pClusterInfo);
			}
			else
			{
				if (pClusterInfo->ServerIp != pHead->FamilyInfo[i].ServerIp)
					pAckHead->result = RESULT_FAILED;
			}

			SendPacket << nClusterId;
		
			if( !SendPacket.HasData(sizeof(pAckHead->FamilyID) ) )
			{
				break;
			}
		}

		//����ϸ���
		pAckHead->count = i;

	}
	else if( pHead->type == FAMILYINFO_QUERY_TYPE_TEMP ) //��ʱ����
	{
		
		int nNewClusterID = m_AutoID.GetNewID();
		nNewClusterID = nNewClusterID | 0x80000000 ;
		pClusterInfo = m_TempCluster.AddHash(nNewClusterID);
		pClusterInfo->id = nNewClusterID;
		pClusterInfo->ServerIp = pHead->FamilyInfo[0].ServerIp;

		pAckHead->result = 0;
		pAckHead->count	 = 1;
		pAckHead->FamilyID[0] = nNewClusterID;

		SendPacket.SetCursor(sizeof(GSRV_FAMILYINFO_ADD_ACK));

		AddToClustList(pClusterInfo);
	}
	else
	{
		CLog::Log("CClusterManager::GetClusterInfo",CLog::TYPE_ERROR,"��⵽�Ƿ���������:%d!",pHead->type);
		return false;
	}


	return true ;
}

void CClusterManager::AddToClustList(ClusterInfo* pClusterInfo)
{
	if( pClusterInfo )
	{
		m_listLock.Lock();
		if( NULL == m_pClusterListHead )
		{
			m_pClusterListHead = pClusterInfo;
			pClusterInfo->pre = NULL;
			pClusterInfo->next= NULL;
		}
		else
		{
			pClusterInfo->pre = NULL;
			pClusterInfo->next = m_pClusterListHead;
			m_pClusterListHead->pre = pClusterInfo;
			m_pClusterListHead = pClusterInfo;
		}
		m_listLock.Unlock();
	}
}


//ɾ�����߼�����Ϣ
bool CClusterManager::DelCluster(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	GSRV_FAMILYINFO_DEL_QUERY* pQueryHead = (GSRV_FAMILYINFO_DEL_QUERY*)RecvPacket.GetData();
	GSRV_FAMILYINFO_DEL_ACK*   pAckHead	  = (GSRV_FAMILYINFO_DEL_ACK*)SendPacket.GetData();

	ClusterInfoTable* pClusterInfoTable;
	if( pQueryHead->type == FAMILYINFO_QUERY_TYPE_PERM )	//�̶�����
	{
		pClusterInfoTable = &m_PermCluster;
	}
	else if( pQueryHead->type == FAMILYINFO_QUERY_TYPE_TEMP ) //��ʱ����
	{
		pClusterInfoTable = &m_TempCluster;
	}
	else
	{
		CLog::Log("CClusterManager::GetClusterInfo",CLog::TYPE_ERROR,"��⵽�Ƿ���������!type:%d",pQueryHead->type);
		return false;
	}

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	int nReturnNum = pQueryHead->count;
	ClusterInfo* pClusterInfo = NULL;
	//ClusterInfo* pTempClusterInfo = NULL;
	uint32 nClusterId = 0;
	int nDelNum =0;
	for( int i = 0 ; i < nReturnNum ; ++i )
	{
		nClusterId = pQueryHead->FamilyID[i];

		if( pQueryHead->type == FAMILYINFO_QUERY_TYPE_TEMP)
		{
			m_AutoID.DeleteID(nClusterId);
		}

		pClusterInfo = pClusterInfoTable->Find(nClusterId);
		if( pClusterInfo )
		{
			m_listLock.Lock();
			if( pClusterInfo->pre )
			{
				pClusterInfo->pre->next = pClusterInfo->next;
			}
			if( pClusterInfo->next)
			{
				pClusterInfo->next->pre = pClusterInfo->pre;
			}

			if( m_pClusterListHead == pClusterInfo)
			{
				m_pClusterListHead = pClusterInfo->next;
			}

			pClusterInfo->next = NULL;
			pClusterInfo->pre  = NULL;

			m_listLock.Unlock();
		}

		if(pClusterInfoTable->DelHash(nClusterId) )
		{
			SendPacket << nClusterId;
			++nDelNum;
		}

		if( !SendPacket.HasData(sizeof(pAckHead->FamilyID) ) )
		{
			break;
		}
	}

	//����ϸ���
	pAckHead->count = nDelNum;

	return true;
}


bool CClusterManager::ClearClusterOnImServer(uint32 serverip)
{
	m_listLock.Lock();

	ClusterInfo* pCurrentInfo = m_pClusterListHead;
	ClusterInfo* pTeampClusterInfo;

	while( pCurrentInfo)
	{
		if( pCurrentInfo->ServerIp == serverip )
		{
			if( pCurrentInfo->pre )
			{
				pCurrentInfo->pre->next = pCurrentInfo->next;
			}
			if( pCurrentInfo->next)
			{
				pCurrentInfo->next->pre = pCurrentInfo->pre;
			}

			if( m_pClusterListHead == pCurrentInfo)
			{
				m_pClusterListHead = pCurrentInfo->next;
			}
		}
		
		pTeampClusterInfo = pCurrentInfo;
		pCurrentInfo  = pCurrentInfo->next;
		pTeampClusterInfo->next = NULL;
		pTeampClusterInfo->pre  = NULL;
	}


	m_listLock.Unlock();

	return true;
}

void CClusterManager::GetClusterFromID(uint32 nId,char* pInfo)
{
	if( 0x80000000 & nId )
	{
		ClusterInfo* pCluster = m_TempCluster.Find(nId);
		if( pCluster)
		{
			sprintf(pInfo,"��ʱȺ%d����ע�ᵽIM������%d.%d.%d.%d",
						nId, 
						pCluster->ServerIp&0xff,
						(pCluster->ServerIp&0xff00) >> 8 ,
						(pCluster->ServerIp&0xff0000) >> 16 ,
						(pCluster->ServerIp&0xff000000) >> 24 );
		}
		else
		{
			sprintf(pInfo,"��ʱȺ%d������",nId);
		}
	}
	else
	{
		ClusterInfo* pCluster = m_PermCluster.Find(nId);
		if( pCluster)
		{
			sprintf(pInfo,"�̶�Ⱥ%d����ע�ᵽIM������%d.%d.%d.%d ����ʱ��:%s",
				nId, 
				pCluster->ServerIp&0xff,
				(pCluster->ServerIp&0xff00) >> 8 ,
				(pCluster->ServerIp&0xff0000) >> 16 ,
				(pCluster->ServerIp&0xff000000) >> 24,
				ctime(&pCluster->tm));
		}
		else
		{
			sprintf(pInfo,"�̶�Ⱥ%d������",nId);
		}
	}
}
