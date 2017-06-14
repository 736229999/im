#include "stdafx.h"
#include "OnlineManager.h"
#include "Log.h"
#include "SrvFrameProtocol.h"
#include "protocol.h"
#include <netinet/in.h>
#include <arpa/inet.h>

#pragma pack(push,1)

	//在线信息查询时的返回信息头
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


//添加/修改一个上线的用户
bool COnlineManager::AddModifyOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,
										 ReloginUser* pReloginUserList,uint8& nReloginCount)
{
	COnlineUser* pNewUser = NULL;
	nReloginCount = 0;

	//检查是什么请求类型
	uint8 action;
	uint8 nUserCount;
	RecvPacket>>action;
	RecvPacket>>nUserCount;

	SendPacket << (uint8)V_SUCCESS	//操作结果
			   << (uint8) 0	;		//返回个数，后面会再次修改该值

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
				CLog::Log("COnlineManager",CLog::TYPE_ERROR,"用户ID越界,可能包数据错误");
				return false;
			}


			//检查原来是否已经存在该用户
			pNewUser = m_pGlobalOnline[nClientID];
			if( pNewUser == NULL )
			{
				pNewUser = m_ObjectPool.AllocateObj();
				if( NULL == pNewUser )
				{
					CLog::Log("COnlineManager",CLog::TYPE_ERROR,"内存分配失败");
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
			//设置新用户的信息
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
				CLog::Log("AddModifyOnlineUser", CLog::TYPE_ERROR, "向全局注册了一个离线用户：%d", nClientID);

			//检查是否重登陆
			if( !bIsNewUser && action == GSRV_ADDMODIFY_FULLUSERINFO_QUERY::ACTION_ADD )
			{
				if (pNewUser->m_ServerIp != NewServerIP && pNewUser->m_ip == OldUserIp && pNewUser->m_port == OldUserPort) //need del by lwl ver:0101
					CLog::Log("COnlineManager::AddModifyOnlineUser", CLog::TYPE_ERROR, "使用了相同的IP、Port登录不同的服务器");
				if( pNewUser->m_ServerIp != NewServerIP || (pNewUser->m_ip != OldUserIp) || (pNewUser->m_LanIp != OldLanIp))
				{
					//添加一个已经存在的用户，说明有重登录情况发生，
					//这时要通知旧的IM服务器,所以这里记录下来
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
				//加入索引列表
				m_pGlobalOnline[pNewUser->m_UserID] = pNewUser;

				//加入在线列表
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
				//空间存储已满
				break;
			}
		}

		//返回设置成功的用户数
		GSRV_ADDMODIFY_FULLUSERINFO_ACK* pHead = (GSRV_ADDMODIFY_FULLUSERINFO_ACK*)SendPacket.GetData();
		pHead->count = count ;

	}
	else 
	{
		CLog::Log("COnlineManager::AddModifyOnlineUser",CLog::TYPE_ERROR,"收到一个未知action的协议包:%d",action);
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
//删除一个上线用户
bool COnlineManager::DelOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,uint32 fromip)
{
	COnlineUser* pUser = NULL;

	GSRV_DEL_ONLINE_USER_QUERY* pQueryHead = (GSRV_DEL_ONLINE_USER_QUERY*)RecvPacket.GetData();
	
	SendPacket << (uint8)V_SUCCESS	//操作结果
			   << (uint8) 0	;		//返回个数，后面会再次修改该值
	
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
// 				CLog::Log("COnlineManager::DelOnlineUser",CLog::TYPE_ERROR,"IM请求全局登出非直连用户:%d",pQueryHead->userid[i]);
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
		//若为0,表示不限制，则设一个足够大的值
		nReturnUserNum = 20;
	}
	
	m_CriticSearch.Lock();
	

	//初始信息
//	if(NULL == m_pCurrentSearch)
	{
		m_pCurrentSearch = m_pOnlineList;
		if( NULL == m_pCurrentSearch )
		{
			CLog::Log("SearchOnlineUser",CLog::TYPE_ERROR,"没有在线用户但收到用户请求");
			m_CriticSearch.Unlock();
			return false;
		}
	}

	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充
	SendPacket << m_nOnlineNum;		//在线用户总数

	int nOneUserInfoSize = sizeof(GSRV_SEARCH_ONLINE_USER_ACK_HEAD::tagUserInfo) ;

	//得到特定数量的在线用户信息
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
				//在线用户是空的
			ReturnNum = 0;
			CLog::Log("SearchOnlineUser",CLog::TYPE_ERROR,"Skip %d online users,no other users else",startpos);
			has_user_flag = false;
			break;						
		}
		
	}
	if(has_user_flag)	//没有更多用户存在
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

			//继续搜索下一个用户信息
			m_pCurrentSearch = m_pCurrentSearch->m_pNext;
			
			if( NULL == m_pCurrentSearch)
			{
				m_pCurrentSearch = m_pOnlineList;
				if( NULL == m_pOnlineList->m_pNext )
				{
					//在线用户是空的
					break;
				}			
			}
			if( m_pCurrentSearch == pListHead)
			{
				break;
			}

			
		}
	}
	//最后补上个数
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

	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket << nClientID 
				<< (uint32)0;	//离线
			
			//后面几个值都是没意义的,仅起填充作用
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

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//超过尺寸了,退出
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

	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket  << nClientID 
						<< (uint8)XTREAM_OFFLINE;	//离线

		}
		else
		{
			SendPacket  << nClientID 
						<< pUser->m_status;
		}

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//超过尺寸了,退出
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


	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE;	//离线
		}
		else
		{
			pUser->m_status = pQueryHead->user[i].status;
			SendPacket << nClientID 
				<< pUser->m_status;
		}

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//超过尺寸了,退出
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

	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket  << nClientID 
						<< (uint8)XTREAM_OFFLINE	//离线
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

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(nOneUserInfoSize) )
		{
			//超过尺寸了,退出
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


	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE	//离线
				<< (uint8)0;
		}
		else
		{
			pUser->m_flow = pQueryHead->user[i].flow;
			SendPacket << nClientID 
				<< pUser->m_status
				<< pUser->m_flow;
		}

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//超过尺寸了,退出
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

	SendPacket << (uint8)0;			//成功标志
	SendPacket << (uint8)0;			//返回个数,后面根据实际值重新填充

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
			//说明该用户不在线
			SendPacket << nClientID 
				<< (uint8)XTREAM_OFFLINE	//离线
				//下面是无意义的值,仅填充作用
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

		//检查下一个用户的信息是否还能装载得下
		if( !SendPacket.HasData(sizeof(pAckHead->user[0])) )
		{
			//超过尺寸了,退出
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
		
		if(pGetBestAccount->flow_type == 0)//在线分流
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
		else//分组分流
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

