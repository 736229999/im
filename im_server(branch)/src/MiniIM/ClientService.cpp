
#include "ClientService.h"
#include "AppManager.h"
#include "stdafx.h"
#include "RouterProtocol.h"
#include "BusiCacheInterface.h"

CSyncCritical ClientService::m_CSListLock;
__gnu_cxx::hash_map<uint32, ClientService*> ClientService::m_cshash;




ClientService::ClientService()
{		
	Reset();
}

void ClientService::Reset()
{		
	m_nID = 0;				//当前clientservice的ID号
	//m_userlist = 0;
	m_MaxRecConsult = 10;		//最大咨询接待量		
	m_CurReception = 0;			//当前接待量
	m_CurConsult = 0;			//当前咨询量
	m_Customerlist.clear();	//向客服咨询的用户[用户id-----用户信息]

	m_bMonitored = false;	// 客服是否被监控
	m_nChattingBuyerID = (uint32)-1;	// 客服当前正在联系的买家（用于监控过程中，将正在联系的买家告知主管）
	m_nDirectorID = (uint32)-1;	// 如果客服被监控，记录监控他的主管id
}


ClientService::~ClientService()
{
	map<uint32,CSortArray<customer_t> *>::iterator Iter;	
	for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++) 
	{ 
	  if(Iter->second)
	  { 		 
		 delete (Iter->second);
		 //Iter = m_Customerlist.begin(); //当erase后，旧的容器会被重新整理成一个新的容器
	  } 
	}		
	//m_Customerlist.clear();	//向客服咨询的用户[用户id-----用户信息]
	
}

void ClientService::AddCustomer(uint32 teamid,uint32 customerId)		//新增咨询用户
{
	CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"准备加入 组 %u 用户 %u", teamid,customerId);
	
	map<uint32 ,CSortArray<customer_t> *>::iterator l_it; 
   	l_it=m_Customerlist.find(teamid);
   	if(l_it==m_Customerlist.end())	//不存在组的上下文
	{
		CSortArray<customer_t> *teamNode = new CSortArray<customer_t>(MAX_FRIENDS_NUM);		
		customer_t *customer = teamNode->Add(customerId);
		if(customer != NULL)
		{			
			m_CurConsult++; 	//当前咨询量
			customer->ReceptionState = 0;
			customer->teamid = teamid;
			customer->userid = customerId;
			m_Customerlist.insert(make_pair(teamid,teamNode));
		}
		else
		{			
			CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"组 %u 已经存在，用户 %u 异常", teamid,customerId);
		}
	}
   	else	
	{		
		customer_t *customer = (l_it->second)->Add(customerId);
		if(customer != NULL)		//没有这个用户接待
		{			
			m_CurConsult++; 	//当前咨询量
			customer->ReceptionState = 0;	//当前买家处于未接待状态
			customer->teamid = teamid;
			customer->userid = customerId;
			m_Customerlist[teamid] = (l_it->second); 
		}
		else
		{
			CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"组 %u 已有 %u 用户", teamid,customerId);
		}
	}	
	CustomerQueueDebug();

	
}

void ClientService::CustomerQueueDebug()
{
	map<uint32,CSortArray<customer_t> *>::iterator Iter;	
	for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++) 
	{ 	 
	 	for(int i = 0; i < (Iter->second)->GetSize(); ++i)
		{	
			customer_t *pFriendInfo = (Iter->second)->FindByPos(i);				
			CLog::Log("CustomerQueueDebug", CLog::TYPE_IMPORTANT,"当前组 %u  有用户 %u 接待状态 %d", 
				pFriendInfo->teamid,
				pFriendInfo->userid,
				pFriendInfo->ReceptionState);
		}

	}
	return;
}

void ClientService::RemoveCustomer(uint32 teamid,uint32 customerId)	//删除咨询用户
{
	CLog::Log("CUser::RemoveCustomer", CLog::TYPE_IMPORTANT,"准备删除 组 %u 用户 %u ", teamid,customerId);


	map<uint32 ,CSortArray<customer_t> *>::iterator Iter;
	Iter = m_Customerlist.find(teamid);
	if(Iter!=m_Customerlist.end())
	{
		CSortArray<customer_t> * ctx = (CSortArray<customer_t> *)Iter->second;
		if(ctx == NULL)
		{			
			CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"咨询用户%u上下文异常,无法改变状态！",customerId);
		}
		else
		{
			customer_t *cusInfo = ctx->Find(customerId);
			if(cusInfo)
			{
				if(cusInfo->ReceptionState == 1)
				{
					m_CurReception--;
				}
				m_CurConsult--; 
				ctx->Del(customerId);				
				
				CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,
					"客服 %d 在组%u中退出接待用户 %u,接待量为%d 咨询量 %d"
					,m_nID,teamid,customerId,m_CurReception,m_CurConsult);
			}
			else
			{				
				CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"1咨询用户%u上下文异常,无法改变状态！",customerId);
			}
			
		}
	}
	else
	{		
		CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"该用户%u并不在咨询队列%u的客服%u中,无法改变状态！",customerId,teamid,m_nID);
	}


	CustomerQueueDebug();	
}

void ClientService::ModifyCustomerState(uint32 teamid,uint32 customerId)	//设置用户为已接待
{
	map<uint32,CSortArray<customer_t> *>::iterator Iter;
	Iter = m_Customerlist.find(teamid);
	if(Iter!=m_Customerlist.end())
	{
		customer_t *customer = (Iter->second)->Find(customerId);
		if(customer->ReceptionState == 0)
		{	
			customer->ReceptionState = 1;									//现在只存接待状态
			m_CurReception++;
		}
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,
			"客服 %d 在组%u中开始接待用户 %u,接待量为%u 咨询量 %u"
			,m_nID,teamid,customerId,m_CurReception,m_CurConsult);
	}
	else
	{		
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"该用户并不在咨询队列中:%d，无法改变状态！",m_nID);
	}
}


void ClientService::AddCustomer_x(uint32 teamid,uint32 customerId)		//新增咨询用户
{
	CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_IMPORTANT,"准备加入客服 %u 组 %u 用户 %u",m_nID,teamid,customerId);
	
	m_CSListLock.Lock();
	map<uint32 ,customer_t>::iterator l_it; 
   	l_it=m_CSCustomerlist.find(customerId);
   	if(l_it==m_CSCustomerlist.end())	//不存在组的上下文
	{
		customer_t customer;
		m_CurConsult++; 	//当前咨询量
		customer.ReceptionState = 0;
		customer.teamid = teamid;
		customer.userid = customerId;
		m_CSCustomerlist.insert(make_pair(customerId,customer));
	}
   	else	
	{		
		customer_t customer = (l_it->second);
		if(customer.userid != customerId || customer.teamid != teamid )		//数据异常，修复
		{			
			CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_ERROR,
				"已存入的组为%u 用户 %u 存入 组 %u 已有 %u 用户",customer.teamid,customer.userid,teamid,customerId);
			//m_CurConsult++; 	//当前咨询量
			customer.ReceptionState = 0;	//当前买家处于未接待状态
			customer.teamid = teamid;
			customer.userid = customerId;
			m_CSCustomerlist[customerId] = customer; 
		}
		else
		{
			CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_IMPORTANT,"组 %u 已有 %u 用户", teamid,customerId);
		}
	}		
	m_CSListLock.Unlock();
	CustomerQueueDebug_x();
	return;
	
}

void ClientService::CustomerQueueDebug_x()
{
	map<uint32,customer_t>::iterator Iter;	
	for(Iter = m_CSCustomerlist.begin(); Iter != m_CSCustomerlist.end(); Iter++) 
	{ 	 
		customer_t pFriendInfo = (Iter->second);				
		CLog::Log("CustomerQueueDebug_x", CLog::TYPE_IMPORTANT,"当前组 %u  有用户 %u 接待状态 %d", 
			pFriendInfo.teamid,
			pFriendInfo.userid,
			pFriendInfo.ReceptionState);
	}
	return;
}

void ClientService::RemoveCustomer_x(uint32 customerId)	//删除咨询用户
{
	CLog::Log("CUser::RemoveCustomer_x", CLog::TYPE_IMPORTANT,"准备删除 用户 %u ",customerId);

	m_CSListLock.Lock();

	map<uint32 ,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		customer_t ctx = Iter->second;		

		if(ctx.ReceptionState == 1)
		{
			m_CurReception--;
		}
			m_CurConsult--;
			
		if(m_CurConsult < 0)
		{
			m_CurConsult = 0;
		}
 		m_CSCustomerlist.erase(Iter);				
		
		CLog::Log("RemoveCustomer_x", CLog::TYPE_IMPORTANT,
			"客服 %d 在组%u中退出接待用户 %u,接待量为%d 咨询量 %d"
			,m_nID,ctx.teamid,customerId,m_CurReception,m_CurConsult);
			
	}
	else
	{		
		CLog::Log("RemoveCustomer_x", CLog::TYPE_IMPORTANT,"该用户%u并不在咨询队列的客服%u中,无法改变状态！",customerId,m_nID);
	}

	m_CSListLock.Unlock();

	CustomerQueueDebug_x();	
}

void ClientService::ModifyCustomerState_x(uint32 customerId)	//设置用户为已接待
{
	m_CSListLock.Lock();

	map<uint32,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		customer_t customer = (Iter->second);
		if(customer.ReceptionState == 0)
		{	
			customer.ReceptionState = 1;									//现在只存接待状态
			m_CurReception++;
		}		
		m_CSCustomerlist[customerId] = customer; 
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,
			"客服 %d 在组%u中开始接待用户 %u,接待量为%u 咨询量 %u"
			,m_nID,customer.teamid,customerId,m_CurReception,m_CurConsult);
	}
	else
	{		
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"该用户并不在咨询队列中:%d，无法改变状态！",m_nID);
	}
	m_CSListLock.Unlock();
}


bool ClientService::getCustomerInfo(uint32 customerId,customer_t &info)	//获取接待用户信息
{
	map<uint32,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		info = (Iter->second);		
		CLog::Log("getCustomerInfo", CLog::TYPE_IMPORTANT,
			"客服 %d 在组%u中开始接待用户 %u,接待量为%u 咨询量 %u"
			,m_nID,info.teamid,customerId,m_CurReception,m_CurConsult);
		return true;
	}
	else
	{		
		CLog::Log("getCustomerInfo", CLog::TYPE_IMPORTANT,"该用户并不在咨询队列中:%d",customerId);
		return false;
	}

}

bool ClientService::getCustomerTeamId(uint32 customerId,uint32 &teamid)//获取接待用户所在组信息
{
	customer_t info;
	if(getCustomerInfo(customerId,info))
	{
		teamid = info.teamid;
		return true;
	}
	else
	{		
		return false;
	}
}

bool ClientService::hasCustomer(uint32 customerId)//判断是否咨询中
{
	map<uint32,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		return true;
	}
	else
	{
		return false;
	}
}

		

void ClientService::AddToOnlineList()	//将当前卖家加入在线链表中
{
	m_CSListLock.Lock();
 	if (m_cshash.find(m_nID) != m_cshash.end())
	{
		CLog::Log("ClientService::AddToOnlineList()", CLog::TYPE_IMPORTANT,"列表中已经存在要添加的用户:%d！", m_nID);
	}
	else
	{
		m_cshash.insert(make_pair(m_nID, this));
	}
	m_CSListLock.Unlock();
}

void ClientService::DelFromOnlineList()		//将当前卖家从在线链表中删除
{
	m_CSListLock.Lock();
	if (m_cshash.find(m_nID) == m_cshash.end())
	{
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "列表中不存在要删除的用户:%d！", m_nID);
	}
	else
	{
		m_cshash.erase(m_nID);
	}
	m_CSListLock.Unlock();
}

//发送一个退出业务包
void ClientService::SendLogoutReq(CUser* pUser)
{

	RecvDataStruct RecvData;
	RecvData.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
	XT_HEAD* pHead	= (XT_HEAD*)RecvData.data;
	pHead->cmd		= CMD_LOGOUT;
	pHead->sid		= pUser->m_nID;

	XT_LOGOUT* pLogOut = (XT_LOGOUT*)(RecvData.data + sizeof(XT_HEAD));
	pLogOut->id = pUser->m_nID;
	pLogOut->authData = 0x00;

	CRouterProtocol::GetInstance()->RouterOnePacket(&RecvData);
}

int ClientService::PushQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 dst_id)	//推送给指定客服，并增加到排队队列中
{
		SendDataStruct sendData;
		LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
		lpSendHead->cmd		 = CMD_SERVERMSG_IND;
		lpSendHead->did		 = dst_id;
		lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
		lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
		lpSendHead->from	 = XT_HEAD::FROM_SERVER;
		CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
		lpSendHead->sid		 = CAppManager::GetInstance().GetConfig().GetServerNo();	

		LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
		lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
		lpSrvTransmit->msgtype				= CMD_WAITQUEU_CHANGE;


		XT_WAITQUEU_CHANGE* pTeamMatesNotify = (XT_WAITQUEU_CHANGE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
		pTeamMatesNotify->bAdd    = QueueChange->bAdd;
		pTeamMatesNotify->idBuyer = QueueChange->idBuyer;
		strcpy(pTeamMatesNotify->nicknameBuyer,QueueChange->nicknameBuyer);
		pTeamMatesNotify->teamId = QueueChange->teamId;
		//pFriendStatusNotify->status = m_nStatus;

		sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);

		CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
			sendData.data,sendData.data_len,SEND_RETRY_TIME);

#if 0

		//如果该好友是直连到本服务器,则直接修改其好友列表状态

		CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //查找分组排队队列
		if(!csQueue)
		{				
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"查询排队组信息失败 :%d！", QueueChange->teamId);
		}
		if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
		{					
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"增加用户到排队队列中 :TEAM %d UID:%u",
					QueueChange->teamId,QueueChange->idBuyer);			
			csQueue->AddToCSQueueList(QueueChange->idBuyer);
		}
		else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
		{			
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"将排队用户从队列中移除 (被咨询) :TEAM %d UID:%u",
					QueueChange->teamId,QueueChange->idBuyer);
			csQueue->DelFromCSQueueList(QueueChange->idBuyer);				
		}
#endif
	return RESULT_SUCCESS;

}


int ClientService::NotifyTeamQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 type)	//通知组内其他成员排队状况 0 包括自己 1 不包括自己
{
	csTeam_t* csTeam;
	for(unsigned int i = 0 ;i < m_teamMates.size(); ++i )
	{
		csTeam = &(m_teamMates[i]);	//按照组id去查(组是唯一的)
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"内部队列:TEAM %d 查询队列: TEAM %d ,发送给 %u",
					csTeam->teamid,QueueChange->teamId,csTeam->userid);	
		if(csTeam->teamid != QueueChange->teamId)	//跳过不是同一组的用户
		{
			continue;
		}
		if(csTeam && ((type == 0) || (csTeam->userid != m_nID )))
		{
			SendDataStruct sendData;
			LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
			lpSendHead->cmd		 = CMD_SERVERMSG_IND;
			lpSendHead->did		 = csTeam->userid;
			lpSendHead->dtype	 = XT_HEAD::DEST_TYPE_USER;
			lpSendHead->flag	 = XT_HEAD::FLAG_SRVACTION;
			lpSendHead->from	 = XT_HEAD::FROM_SERVER;
			CAppManager::GetInstance().GetSendSeq(lpSendHead->seq);
			lpSendHead->sid		 = CAppManager::GetInstance().GetConfig().GetServerNo();	

			LPXT_SERVER_TRANSMIT lpSrvTransmit  = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
			lpSrvTransmit->origin_id			= XT_SERVER_TRANSMIT::ORIGIN_ID_SERVER;
			lpSrvTransmit->msgtype				= CMD_WAITQUEU_CHANGE;


			XT_WAITQUEU_CHANGE* pTeamMatesNotify = (XT_WAITQUEU_CHANGE*)(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) );
			pTeamMatesNotify->bAdd    = QueueChange->bAdd;
			pTeamMatesNotify->idBuyer = QueueChange->idBuyer;			
			pTeamMatesNotify->teamId = QueueChange->teamId;
			strcpy(pTeamMatesNotify->nicknameBuyer,QueueChange->nicknameBuyer);
			
			//pFriendStatusNotify->status = m_nStatus;

			sendData.data_len = sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT) + sizeof(XT_WAITQUEU_CHANGE);
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"准备发送"); 

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
				sendData.data,sendData.data_len,SEND_RETRY_TIME);

		}
	}

	//用memcached直接写入全局服务器中，本地不存队列信息
#if 0

	//如果该好友是直连到本服务器,则直接修改其好友列表状态

	CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //查找分组排队队列
	if(!csQueue)
	{				
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"查询排队组信息失败 :%d！", QueueChange->teamId);
	}
	if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
	{					
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"增加用户到排队队列中 :TEAM %d UID:%u",
			QueueChange->teamId,QueueChange->idBuyer);			
		csQueue->AddToCSQueueList(QueueChange->idBuyer);
	}
	else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
	{			
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"将排队用户从队列中移除 (被咨询) :TEAM %d UID:%u",
			QueueChange->teamId,QueueChange->idBuyer);
		csQueue->DelFromCSQueueList(QueueChange->idBuyer);				
	}
#endif
	

	return RESULT_SUCCESS;

}

// 获取客服正在接待的买家（用于监控过程中，将正在联系的买家告知主管）
uint32 ClientService::GetChattingBuyerID()
{
	return m_nChattingBuyerID;
}
void ClientService::AddTeamMatesList(uint32 teamId,uint32 friendId)	//加好友
{
	if (friendId == 0 || teamId == 0)
	{
		CLog::Log("ClientService::AddTeamMatesList()",CLog::TYPE_IMPORTANT, "传入客服的好友 %u 或者队列 %u异常",friendId,teamId);
	}
	else
	{
		csTeam_t TeamMates;
		TeamMates.teamid = teamId;
		TeamMates.userid = friendId;
		m_teamMates.push_back(TeamMates);
	}
	DebugTeamMatesList();
	return;

}

// 设置客服正在接待的买家
void ClientService::SetChattingBuyerID(uint32 nBuyerID)
{
	m_nChattingBuyerID = nBuyerID;
}

// 获取客服正在被哪个主管监控
uint32 ClientService::GetDirectorID()
{
	return m_nDirectorID;
}

// 设置客服正在被哪个主管监控
void ClientService::SetDirectorID(uint32 nDirectorID)
{
	m_nDirectorID = nDirectorID;
	m_bMonitored = (m_nDirectorID != (uint32)-1 && m_nDirectorID != 0);		// 当设置的监控主管id不为-1时，表示客服正在被监控
}

void ClientService::DelTeamMatesList(uint32 teamId,uint32 friendId)	//删除好友
{
	if (friendId == 0 || teamId == 0)
	{
		CLog::Log("ClientService::DelTeamMatesList()",CLog::TYPE_IMPORTANT, "传入客服的好友 %u 或者队列 %u异常",friendId,teamId);
	}
	else
	{
		vector<csTeam_t>::iterator Iter;	
		for(Iter = m_teamMates.begin(); Iter != m_teamMates.end(); Iter++) 
		{ 
		  if(Iter->teamid == teamId && Iter->userid == friendId)
		  { 
			 m_teamMates.erase(Iter);
			 Iter = m_teamMates.begin(); //当erase后，旧的容器会被重新整理成一个新的容器
		  } 
		}		
	}
	DebugTeamMatesList();


}

void ClientService::DebugTeamMatesList()
{
	
	CLog::Log("ClientService:DebugTeamMatesList",CLog::TYPE_IMPORTANT, "好友总数为%u (有可能会重复)",m_teamMates.size());
#if 0
	vector<csTeam_t>::iterator Iter;	
	for(Iter = m_teamMates.begin(); Iter != m_teamMates.end(); Iter++) 
	{
		CLog::Log("ClientService::DebugTeamMatesList()",CLog::TYPE_IMPORTANT, 
			"好友列表中的好友有 :    Team %u  Id %u ",Iter->teamid,Iter->userid);	   
	}	
#endif
	return;

}


