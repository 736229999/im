#include "stdafx.h"
#include "OnlineManager.h"
#include "Log.h"
#include "SrvFrameProtocol.h"
#include "protocol.h"
#include <netinet/in.h>
#include <arpa/inet.h>

#pragma pack(push,1)

	//������Ϣ��ѯʱ�ķ�����Ϣͷ
	struct OnlineInfoReturnHead
	{
		uint8 result;
		uint8 count;
	} ;
#pragma pack(pop)


COnlineManager::COnlineManager(void)
{
	m_pOnlineList = NULL;
	m_pCurrentSearch = NULL;

	m_pGlobalOnline = new COnlineUser*[MAX_139_ACCOUNT_NUM];
	memset(m_pGlobalOnline,0,sizeof(COnlineUser*)*MAX_139_ACCOUNT_NUM);

	m_nOnlineNum = 0;

}

COnlineManager::~COnlineManager(void)
{
	delete[] m_pGlobalOnline;
}


//���/�޸�һ�����ߵ��û�
bool COnlineManager::AddModifyOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,
										 ReloginUser* pReloginUserList,uint8& nReloginCount)
{
	COnlineUser* pNewUser = NULL;
	nReloginCount = 0;

	//�����ʲô��������
	uint8 action;
	uint8 nUserCount;
	RecvPacket>>action;
	RecvPacket>>nUserCount;

	SendPacket << (uint8)V_SUCCESS	//�������
			   << (uint8) 0	;		//���ظ�����������ٴ��޸ĸ�ֵ

	uint32 nClientID;
	bool	bIsNewUser = false;
	int count = 0 ;

	if(action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD ||
		action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_MODIFY  ) 
	{
		for(int i = 0 ; i < nUserCount; ++i)
		{

			bIsNewUser = false;
			RecvPacket>>nClientID;

			if( 0 > nClientID ||
				nClientID >= MAX_139_ACCOUNT_NUM)
			{
				CLog::Log("COnlineManager",CLog::TYPE_ERROR,"�û�IDԽ��,���ܰ����ݴ���");
				return false;
			}


			//���ԭ���Ƿ��Ѿ����ڸ��û�
			pNewUser = m_pGlobalOnline[nClientID];
			if( pNewUser == NULL )
			{
				pNewUser = m_ObjectPool.AllocateObj();
				if( NULL == pNewUser )
				{
					CLog::Log("COnlineManager",CLog::TYPE_ERROR,"�ڴ����ʧ��");
					continue;
				}	
				pNewUser->m_flow = 0;
				++m_nOnlineNum;
				bIsNewUser = true;
			}

			uint32 NewServerIP;
			uint32 OldUserIp;
			uint16 OldUserPort;
			uint32 OldLanIp;
			if (!bIsNewUser)
			{
				OldUserIp	= pNewUser->m_ip;
				OldUserPort	= pNewUser->m_port;
				OldLanIp	= pNewUser->m_LanIp;
			}
			//�������û�����Ϣ
			pNewUser->m_UserID = nClientID;
			RecvPacket>>pNewUser->m_status
					  >>pNewUser->m_rule
					  >>pNewUser->m_UserName
					  >>pNewUser->m_NickName
					  >>pNewUser->m_ip
					  >>pNewUser->m_port
					  >>pNewUser->m_sexy
					  >>NewServerIP
					  >>pNewUser->m_LanIp
					  >>pNewUser->flag;
				
			if (pNewUser->m_status == XTREAM_OFFLINE)
				CLog::Log("AddModifyOnlineUser", CLog::TYPE_ERROR, "��ȫ��ע����һ�������û���%d", nClientID);

			//����Ƿ��ص�½
			if( !bIsNewUser && action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD )
			{
				if (pNewUser->m_ServerIp != NewServerIP && pNewUser->m_ip == OldUserIp && pNewUser->m_port == OldUserPort) //need del by lwl ver:0101
					CLog::Log("COnlineManager::AddModifyOnlineUser", CLog::TYPE_ERROR, "ʹ������ͬ��IP��Port��¼��ͬ�ķ�����");
				if( pNewUser->m_ServerIp != NewServerIP || (pNewUser->m_ip != OldUserIp) || (pNewUser->m_LanIp != OldLanIp))
				{
					//���һ���Ѿ����ڵ��û���˵�����ص�¼���������
					//��ʱҪ֪ͨ�ɵ�IM������,���������¼����
					pReloginUserList[nReloginCount].userid = nClientID;
					pReloginUserList[nReloginCount].serverip = pNewUser->m_ServerIp;
					pReloginUserList[nReloginCount].userip	= pNewUser->m_ip;
					pReloginUserList[nReloginCount].lanip	= pNewUser->m_LanIp;

					++nReloginCount;
				}

			}
            
			pNewUser->m_ServerIp = NewServerIP;
			RecvPacket.ReadPacket((char*)pNewUser->m_sessionKey,sizeof(pNewUser->m_sessionKey));
			RecvPacket.ReadPacket((char*)pNewUser->m_p2pSessionKey,sizeof(pNewUser->m_p2pSessionKey));


			if( bIsNewUser )
			{
				//���������б�
				m_pGlobalOnline[pNewUser->m_UserID] = pNewUser;

				//���������б�
				m_CriticOnlineList.Lock();

				pNewUser->m_pNext = m_pOnlineList;
				pNewUser->m_pPre  = NULL;	

				if( m_pOnlineList )
				{
					m_pOnlineList->m_pPre = pNewUser;
				}
				m_pOnlineList = pNewUser;

				m_CriticOnlineList.Unlock();

			}
			

			SendPacket << pNewUser->m_UserID;
			++count;

			if( !SendPacket.HasData(sizeof(pNewUser->m_UserID)))
			{
				//�ռ�洢����
				break;
			}
		}

		//�������óɹ����û���
		GSRV_ADDMODIFY_FULLUSERINFO_ACK* pHead = (GSRV_ADDMODIFY_FULLUSERINFO_ACK*)SendPacket.GetData();
		pHead->count = count ;

	}
	else 
	{
		CLog::Log("COnlineManager::AddModifyOnlineUser",CLog::TYPE_ERROR,"�յ�һ��δ֪action��Э���:%d",action);
		return false;
	}

	return true;
}

bool COnlineManager::DelUser(uint32 userid)
{
	COnlineUser* pUser = NULL;
	pUser = m_pGlobalOnline[userid];
	if( pUser)
	{
		m_ObjectPool.ReleaseObj(pUser);
		m_pGlobalOnline[userid] = 0;

		m_CriticOnlineList.Lock();
		if( m_pOnlineList == pUser )
		{
			m_pOnlineList = pUser->m_pNext;
		}

		if(pUser->m_pPre)
		{
			pUser->m_pPre->m_pNext = pUser->m_pNext;
		}
		if( pUser->m_pNext )
		{
			pUser->m_pNext->m_pPre = pUser->m_pPre;
		}
		pUser->m_pPre = NULL;
		pUser->m_pNext = NULL;
		--m_nOnlineNum;
		m_CriticOnlineList.Unlock();
	}

	return true;
}
//ɾ��һ�������û�
bool COnlineManager::DelOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,uint32 fromip)
{
	COnlineUser* pUser = NULL;

	GSRV_DEL_ONLINE_USER_QUERY* pQueryHead = (GSRV_DEL_ONLINE_USER_QUERY*)RecvPacket.GetData();
	
	SendPacket << (uint8)V_SUCCESS	//�������
			   << (uint8) 0	;		//���ظ�����������ٴ��޸ĸ�ֵ
	
	int i;
	for( i =0;i < pQueryHead->count; ++i)
	{
		if( 0 > pQueryHead->userid[i] ||
			pQueryHead->userid[i] >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("DelOnlineUser",CLog::TYPE_ERROR,"userId over %d ",pQueryHead->userid[i]);
			return false;
		}
		pUser = m_pGlobalOnline[pQueryHead->userid[i]];
		if( pUser)
		{
//  			if (fromip && pUser->m_ServerIp != fromip)
// 			{
// 				CLog::Log("COnlineManager::DelOnlineUser",CLog::TYPE_ERROR,"IM����ȫ�ֵǳ���ֱ���û�:%d",pQueryHead->userid[i]);
// 				continue;
// 			}
			m_ObjectPool.ReleaseObj(pUser);
			m_pGlobalOnline[pQueryHead->userid[i]] = 0;

			m_CriticOnlineList.Lock();
			if( m_pOnlineList == pUser )
			{
				m_pOnlineList = pUser->m_pNext;
			}

			if(pUser->m_pPre)
			{
				pUser->m_pPre->m_pNext = pUser->m_pNext;
			}
			if( pUser->m_pNext )
			{
				pUser->m_pNext->m_pPre = pUser->m_pPre;
			}
			pUser->m_pPre = NULL;
			pUser->m_pNext = NULL;
			--m_nOnlineNum;
			m_CriticOnlineList.Unlock();

		}
		SendPacket << pQueryHead->userid[i];
	}

	GSRV_DEL_ONLINE_USER_ACK* pAckHead = (GSRV_DEL_ONLINE_USER_ACK*)SendPacket.GetData();
	pAckHead->count = i;


	return true ;
}



bool COnlineManager::SearchOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint8 nReturnUserNum = 0;
	uint16 startpos = 0;
	RecvPacket>>nReturnUserNum;
	RecvPacket>>startpos;
	
	if( nReturnUserNum == 0 )
	{
		//��Ϊ0,��ʾ�����ƣ�����һ���㹻���ֵ
		nReturnUserNum = 20;
	}
	
	m_CriticSearch.Lock();
	

	//��ʼ��Ϣ
//	if(NULL == m_pCurrentSearch)
	{
		m_pCurrentSearch = m_pOnlineList;
		if( NULL == m_pCurrentSearch )
		{
			CLog::Log("SearchOnlineUser",CLog::TYPE_ERROR,"û�������û����յ��û�����");
			m_CriticSearch.Unlock();
			return false;
		}
	}

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������
	SendPacket << m_nOnlineNum;		//�����û�����

	int nOneUserInfoSize = sizeof(GSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo) ;

	//�õ��ض������������û���Ϣ
	bool has_user_flag = true;
	int ReturnNum = 0;
	COnlineUser* pListHead = m_pCurrentSearch;
	unsigned int count = 0;
	for(int i = 0; i < startpos && count < m_nOnlineNum ;count++)
	{
		
		if( m_pCurrentSearch->m_status != XTREAM_HIDE && m_pCurrentSearch->m_status != XTREAM_OFFLINE)
		{			
			i++;
		}
		m_pCurrentSearch = m_pCurrentSearch->m_pNext;
		if( NULL == m_pCurrentSearch)
		{
				//�����û��ǿյ�
			ReturnNum = 0;
			CLog::Log("SearchOnlineUser",CLog::TYPE_ERROR,"Skip %d online users,no other users else",startpos);
			has_user_flag = false;
			break;						
		}
		
	}
	if(has_user_flag)	//û�и����û�����
	{
		for(int i=0;i<nReturnUserNum;++i)
		{
			if( m_pCurrentSearch->m_status != XTREAM_HIDE && m_pCurrentSearch->m_status != XTREAM_OFFLINE)
			{
				SendPacket << m_pCurrentSearch->m_UserID
					<< m_pCurrentSearch->m_sexy
					<< m_pCurrentSearch->m_status
					<< m_pCurrentSearch->m_rule
					<< m_pCurrentSearch->m_UserName
					<< m_pCurrentSearch->m_NickName  ;

				++ReturnNum;

				if( !SendPacket.HasData(nOneUserInfoSize) )
				{
					break;
				}
				
			}

			//����������һ���û���Ϣ
			m_pCurrentSearch = m_pCurrentSearch->m_pNext;
			
			if( NULL == m_pCurrentSearch)
			{
				m_pCurrentSearch = m_pOnlineList;
				if( NULL == m_pOnlineList->m_pNext )
				{
					//�����û��ǿյ�
					break;
				}			
			}
			if( m_pCurrentSearch == pListHead)
			{
				break;
			}

			
		}
	}
	//����ϸ���
	OnlineInfoReturnHead* pHead = (OnlineInfoReturnHead*)SendPacket.GetData();
	pHead->count = ReturnNum;

	m_CriticSearch.Unlock();

	return true;
}

bool COnlineManager::GetUserLinkInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint8 nReturnUserNum;
	RecvPacket>>nReturnUserNum;

	uint32	nClientID;

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	int nOneUserInfoSize = sizeof(GSRV_USERCONTROLINFO_QUERY_ACK::tagUserControlInfo);
	
	int i;
	for( i = 0 ; i < nReturnUserNum; ++i )
	{
		RecvPacket>>nClientID;
		
		if( 0 > nClientID ||
			nClientID >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:GetUserLinkInfo",CLog::TYPE_ERROR,"id_over:%u", nClientID);
			return false;
		}

		COnlineUser* pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket << nClientID 
				<< (uint32)0;	//����
			
			//���漸��ֵ����û�����,�����������
			SendPacket.WritePacket((const char*)m_pGlobalOnline,sizeof(pUser->m_sessionKey));
			SendPacket.WritePacket((const char*)m_pGlobalOnline,sizeof(pUser->m_sessionKey));
		}
		else
		{
			SendPacket << nClientID 
					   << pUser->m_ServerIp;

			SendPacket.WritePacket((const char*)pUser->m_sessionKey,sizeof(pUser->m_sessionKey));
			SendPacket.WritePacket((const char*)pUser->m_p2pSessionKey,sizeof(pUser->m_p2pSessionKey));
			SendPacket << pUser->flag;

		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}

	OnlineInfoReturnHead* pHead = (OnlineInfoReturnHead*)SendPacket.GetData();
	pHead->count = i;

	return true;
}

bool COnlineManager::GetUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint8 nReturnUserNum;
	RecvPacket>>nReturnUserNum;


	uint32	nClientID;
	COnlineUser* pUser = NULL;

	int nOneUserInfoSize = sizeof( nClientID ) + sizeof( pUser->m_status );

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	int i;
	for( i = 0 ; i < nReturnUserNum; ++i )
	{
		RecvPacket>>nClientID;
		if( nClientID < 0 || nClientID >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:GetUserStatus",CLog::TYPE_ERROR,"id_over:%u", nClientID);
			return false;
		}
		pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket  << nClientID 
						<< (uint8)XTREAM_OFFLINE;	//����

		}
		else
		{
			SendPacket  << nClientID 
						<< pUser->m_status;
		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}

	GSRV_MODIFY_ONLINE_STATUS_ACK* pHead = (GSRV_MODIFY_ONLINE_STATUS_ACK*)SendPacket.GetData();
	pHead->count = i;

	return true;
}

bool COnlineManager::ModifyUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{

	GSRV_MODIFY_ONLINE_STATUS_QUERY* pQueryHead = (GSRV_MODIFY_ONLINE_STATUS_QUERY*)RecvPacket.GetData();
	GSRV_MODIFY_ONLINE_STATUS_ACK*	 pAckHead   = (GSRV_MODIFY_ONLINE_STATUS_ACK*)SendPacket.GetData();


	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	COnlineUser* pUser = NULL;
	uint32 nClientID;
	int i;
	for(i = 0 ; i < pQueryHead->count; ++i)
	{
		nClientID = pQueryHead->user[i].id;
		if( 0 > nClientID ||
			nClientID >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:ModifyUserStatus",CLog::TYPE_ERROR,"id_over:%u", nClientID);

			return false;
		}
		pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE;	//����
		}
		else
		{
			pUser->m_status = pQueryHead->user[i].status;
			SendPacket << nClientID 
				<< pUser->m_status;
		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}
	
	pAckHead->count = i ;
	
	return true;
}



bool COnlineManager::GetUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint8 nReturnUserNum;
	RecvPacket>>nReturnUserNum;


	uint32	nClientID;
	COnlineUser* pUser = NULL;

	int nOneUserInfoSize = sizeof( nClientID ) + sizeof( pUser->m_status );

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	int i;
	for( i = 0 ; i < nReturnUserNum; ++i )
	{
		RecvPacket>>nClientID;
		if( nClientID < 0 || nClientID >= MAX_139_ACCOUNT_NUM )
		{
			CLog::Log("COnlineManager:GetUserFlow",CLog::TYPE_ERROR,"id_over:%u", nClientID);
			return false;
		}
		pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket  << nClientID 
						<< (uint8)XTREAM_OFFLINE	//����
						<< (uint8)0;
			CLog::Log("COnlineManager:GetUserFlow",CLog::TYPE_ERROR,"NO USER SO id :%d m_status:XTREAM_OFFLINE m_flow: 0",nClientID);

		}
		else
		{
			SendPacket  << nClientID 
						<< pUser->m_status
						<< pUser->m_flow;
			
			CLog::Log("COnlineManager:GetUserFlow",CLog::TYPE_ERROR,"id :%d m_status: %d m_flow: %d", nClientID,pUser->m_status,pUser->m_flow);
		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}

	GSRV_GET_USER_FLOW_ACK* pHead = (GSRV_GET_USER_FLOW_ACK*)SendPacket.GetData();
	pHead->count = i;

	return true;
}


bool COnlineManager::ModifyUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{

	tagGSRV_MODIFY_USER_FLOW_QUERY* pQueryHead = (tagGSRV_MODIFY_USER_FLOW_QUERY*)RecvPacket.GetData();
	tagGSRV_MODIFY_USER_FLOW_ACK*	 pAckHead   = (tagGSRV_MODIFY_USER_FLOW_ACK*)SendPacket.GetData();


	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	COnlineUser* pUser = NULL;
	uint32 nClientID;
	int i;
	for(i = 0 ; i < pQueryHead->count; ++i)
	{
		nClientID = pQueryHead->user[i].id;
		if( 0 > nClientID ||
			nClientID >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:ModifyUserFlow",CLog::TYPE_ERROR,"id_over:%u", nClientID);
			return false;
		}
		pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE	//����
				<< (uint8)0;
		}
		else
		{
			pUser->m_flow = pQueryHead->user[i].flow;
			SendPacket << nClientID 
				<< pUser->m_status
				<< pUser->m_flow;
		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}
	
	pAckHead->count = i ;

	return true;
}

bool COnlineManager::GetTUserInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint32 id;
	COnlineUser* pUser = NULL;

	RecvPacket>>id;

	if(id < 0 || id >= MAX_139_ACCOUNT_NUM)
	{
		CLog::Log("COnlineManager:GetTUserInfo",CLog::TYPE_ERROR,"id_over:%u", id);
		SendPacket<<(uint8)RESULT_FAILED
				  <<id;
		return false;
	}

	pUser = m_pGlobalOnline[id];
	if(pUser == NULL)
	{
		SendPacket<<(uint8)RESULT_FAILED
				  <<id;
	}
	else
	{
		SendPacket<<(uint8)RESULT_SUCCESS
				  <<id
				  <<pUser->m_ip
				  <<pUser->m_port;

		SendPacket.WritePacket((const char*)pUser->m_sessionKey,sizeof(pUser->m_sessionKey));
	}
	return true;
}

bool COnlineManager::GetUserFullInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	GSRV_GET_ONLINE_FULLINFO_QUERY* pQueryHead = (GSRV_GET_ONLINE_FULLINFO_QUERY*)RecvPacket.GetData();
	GSRV_GET_ONLINE_FULLINFO_ACK*	 pAckHead   = (GSRV_GET_ONLINE_FULLINFO_ACK*)SendPacket.GetData();

	SendPacket << (uint8)0;			//�ɹ���־
	SendPacket << (uint8)0;			//���ظ���,�������ʵ��ֵ�������

	COnlineUser* pUser = NULL;
	uint32 nClientID;
	int i;
	for(i = 0 ; i < pQueryHead->count; ++i)
	{
		nClientID = pQueryHead->userid[i];
		if( 0 > nClientID ||
			nClientID >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:GetUserFullInfo",CLog::TYPE_ERROR,"id_over:%u", nClientID);
			return false;
		}
		pUser = m_pGlobalOnline[nClientID];
		if( NULL == pUser)
		{
			//˵�����û�������
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE	//����
				//�������������ֵ,���������
				<< (uint16)0				//version
				<< "username"				
				<< "nickname"
				<< (uint32)0				//ip
				<< (uint16)0				//port
				<< (uint8)0					//sexy
				<< (uint32)0;				//serverip
			
			SendPacket.WritePacket((const char*)m_pGlobalOnline,sizeof(pAckHead->user[0].sessionkey));
			SendPacket.WritePacket((const char*)m_pGlobalOnline,sizeof(pAckHead->user[0].p2psessionkey));
		}
		else
		{
			SendPacket << nClientID 
				<< pUser->m_status
				<< pUser->m_version
				<< pUser->m_UserName
				<< pUser->m_NickName
				<< pUser->m_ip
				<< pUser->m_port
				<< pUser->m_sexy
				<< pUser->m_ServerIp;

			SendPacket.WritePacket((const char*)pUser->m_sessionKey,sizeof(pAckHead->user[0].sessionkey));
			SendPacket.WritePacket((const char*)pUser->m_p2pSessionKey,sizeof(pAckHead->user[0].p2psessionkey));
		}

		//�����һ���û�����Ϣ�Ƿ���װ�ص���
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//�����ߴ���,�˳�
			break;
		}

	}

	pAckHead->count = i ;	
	return true ;
}


void COnlineManager::IMServerShutdown(uint32 serverip)
{
	m_CriticOnlineList.Lock();

	COnlineUser* pCurrentUser = m_pOnlineList;

	while( pCurrentUser )
	{
		if( pCurrentUser->m_ServerIp == serverip )
		{
			m_pGlobalOnline[pCurrentUser->m_UserID] = 0;

			if( m_pOnlineList == pCurrentUser )
			{
				m_pOnlineList = pCurrentUser->m_pNext;
			}

			if(pCurrentUser->m_pPre)
			{
				pCurrentUser->m_pPre->m_pNext = pCurrentUser->m_pNext;
			}
			if( pCurrentUser->m_pNext )
			{
				pCurrentUser->m_pNext->m_pPre = pCurrentUser->m_pPre;
			}

			m_ObjectPool.ReleaseObj(pCurrentUser);
			--m_nOnlineNum;	
		}

		pCurrentUser = pCurrentUser->m_pNext;
	}


	m_CriticOnlineList.Unlock();
	
}


bool COnlineManager::GetBestAccount(CRecvPacket& RecvPacket,CSendPacket& SendPacket)
{
	uint32 account = 0;
	uint32 tmp_account = 0;
	uint32 best_id = 0;
//	uint32 flow_id = 0;
	std::vector<uint32> accounts;
	COnlineUser* pUser = NULL;
	GSRV_GET_BEST_ACCOUNT_QUERY* pGetBestAccount = (GSRV_GET_BEST_ACCOUNT_QUERY*)RecvPacket.GetData();

	SendPacket<<(uint8)0;

	for(int i = 0; i < pGetBestAccount->count; i ++)
	{
		account = pGetBestAccount->id[i];
		if(0 > account || account >= MAX_139_ACCOUNT_NUM)
		{
			CLog::Log("COnlineManager:GetBestAccount",CLog::TYPE_ERROR,"id_over:%u", account);
			return false;
		}

		pUser = m_pGlobalOnline[account];

		if(pUser == NULL)
		{
			CLog::Log("COnlineManager::GetBestAccount",CLog::TYPE_ERROR,"not find %u",account);
		}
		else
		{			
			if(pUser->m_status != XTREAM_OFFLINE && pUser->m_status != XTREAM_LEAVE
					 && pUser->m_flow == XT_MOD_HANGUP_STATE_REQ::RECEPTION)
			{
				accounts.push_back(account);
			}
		}
	}

	if(! accounts.empty())
	{
		std::sort(accounts.begin(), accounts.end());
		
		if(pGetBestAccount->flow_type == 0)//���߷���
		{
			CAutoLock mylock(&Merchant_ids.m_mapLock);
			MerchantAccount_Map::iterator MerchantIter;
			MerchantIter = Merchant_ids.find(pGetBestAccount->merchant_id);

			if(MerchantIter != Merchant_ids.end())
			{
				tmp_account = MerchantIter->second;
			}

			for(size_t i = 0; i < accounts.size(); i ++)
			{
				if(accounts[i] > tmp_account)
				{
					best_id = accounts[i];
					break;
				}	
			}

			if(best_id == 0)
			{
				best_id = accounts[0];
			}

			Merchant_ids[pGetBestAccount->merchant_id] = best_id;
		}
		else//�������
		{
			CAutoLock mylock(&Group_ids.m_mapLock);
			GroupAccount_Map::iterator GroupIter;
			GroupIter = Group_ids.find(pGetBestAccount->group_id);

			if(GroupIter != Group_ids.end())
			{
				tmp_account = GroupIter->second;
			}

			for(size_t i = 0; i < accounts.size(); i ++)
			{
				if(accounts[i] > tmp_account)
				{
					best_id = accounts[i];
					break;
				}	
			}

			if(best_id == 0)
			{
				best_id = accounts[0];
			}

			Group_ids[pGetBestAccount->group_id] = best_id;
		}
	}

	SendPacket<<best_id;

	return true;	
}

