
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
	m_nID = 0;				//��ǰclientservice��ID��
	//m_userlist = 0;
	m_MaxRecConsult = 10;		//�����ѯ�Ӵ���		
	m_CurReception = 0;			//��ǰ�Ӵ���
	m_CurConsult = 0;			//��ǰ��ѯ��
	m_Customerlist.clear();	//��ͷ���ѯ���û�[�û�id-----�û���Ϣ]

	m_bMonitored = false;	// �ͷ��Ƿ񱻼��
	m_nChattingBuyerID = (uint32)-1;	// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
	m_nDirectorID = (uint32)-1;	// ����ͷ�����أ���¼�����������id
}


ClientService::~ClientService()
{
	map<uint32,CSortArray<customer_t> *>::iterator Iter;	
	for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++) 
	{ 
	  if(Iter->second)
	  { 		 
		 delete (Iter->second);
		 //Iter = m_Customerlist.begin(); //��erase�󣬾ɵ������ᱻ���������һ���µ�����
	  } 
	}		
	//m_Customerlist.clear();	//��ͷ���ѯ���û�[�û�id-----�û���Ϣ]
	
}

void ClientService::AddCustomer(uint32 teamid,uint32 customerId)		//������ѯ�û�
{
	CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"׼������ �� %u �û� %u", teamid,customerId);
	
	map<uint32 ,CSortArray<customer_t> *>::iterator l_it; 
   	l_it=m_Customerlist.find(teamid);
   	if(l_it==m_Customerlist.end())	//���������������
	{
		CSortArray<customer_t> *teamNode = new CSortArray<customer_t>(MAX_FRIENDS_NUM);		
		customer_t *customer = teamNode->Add(customerId);
		if(customer != NULL)
		{			
			m_CurConsult++; 	//��ǰ��ѯ��
			customer->ReceptionState = 0;
			customer->teamid = teamid;
			customer->userid = customerId;
			m_Customerlist.insert(make_pair(teamid,teamNode));
		}
		else
		{			
			CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"�� %u �Ѿ����ڣ��û� %u �쳣", teamid,customerId);
		}
	}
   	else	
	{		
		customer_t *customer = (l_it->second)->Add(customerId);
		if(customer != NULL)		//û������û��Ӵ�
		{			
			m_CurConsult++; 	//��ǰ��ѯ��
			customer->ReceptionState = 0;	//��ǰ��Ҵ���δ�Ӵ�״̬
			customer->teamid = teamid;
			customer->userid = customerId;
			m_Customerlist[teamid] = (l_it->second); 
		}
		else
		{
			CLog::Log("ClientService::AddCustomer", CLog::TYPE_IMPORTANT,"�� %u ���� %u �û�", teamid,customerId);
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
			CLog::Log("CustomerQueueDebug", CLog::TYPE_IMPORTANT,"��ǰ�� %u  ���û� %u �Ӵ�״̬ %d", 
				pFriendInfo->teamid,
				pFriendInfo->userid,
				pFriendInfo->ReceptionState);
		}

	}
	return;
}

void ClientService::RemoveCustomer(uint32 teamid,uint32 customerId)	//ɾ����ѯ�û�
{
	CLog::Log("CUser::RemoveCustomer", CLog::TYPE_IMPORTANT,"׼��ɾ�� �� %u �û� %u ", teamid,customerId);


	map<uint32 ,CSortArray<customer_t> *>::iterator Iter;
	Iter = m_Customerlist.find(teamid);
	if(Iter!=m_Customerlist.end())
	{
		CSortArray<customer_t> * ctx = (CSortArray<customer_t> *)Iter->second;
		if(ctx == NULL)
		{			
			CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"��ѯ�û�%u�������쳣,�޷��ı�״̬��",customerId);
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
					"�ͷ� %d ����%u���˳��Ӵ��û� %u,�Ӵ���Ϊ%d ��ѯ�� %d"
					,m_nID,teamid,customerId,m_CurReception,m_CurConsult);
			}
			else
			{				
				CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"1��ѯ�û�%u�������쳣,�޷��ı�״̬��",customerId);
			}
			
		}
	}
	else
	{		
		CLog::Log("RemoveCustomer", CLog::TYPE_IMPORTANT,"���û�%u��������ѯ����%u�Ŀͷ�%u��,�޷��ı�״̬��",customerId,teamid,m_nID);
	}


	CustomerQueueDebug();	
}

void ClientService::ModifyCustomerState(uint32 teamid,uint32 customerId)	//�����û�Ϊ�ѽӴ�
{
	map<uint32,CSortArray<customer_t> *>::iterator Iter;
	Iter = m_Customerlist.find(teamid);
	if(Iter!=m_Customerlist.end())
	{
		customer_t *customer = (Iter->second)->Find(customerId);
		if(customer->ReceptionState == 0)
		{	
			customer->ReceptionState = 1;									//����ֻ��Ӵ�״̬
			m_CurReception++;
		}
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,
			"�ͷ� %d ����%u�п�ʼ�Ӵ��û� %u,�Ӵ���Ϊ%u ��ѯ�� %u"
			,m_nID,teamid,customerId,m_CurReception,m_CurConsult);
	}
	else
	{		
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"���û���������ѯ������:%d���޷��ı�״̬��",m_nID);
	}
}


void ClientService::AddCustomer_x(uint32 teamid,uint32 customerId)		//������ѯ�û�
{
	CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_IMPORTANT,"׼������ͷ� %u �� %u �û� %u",m_nID,teamid,customerId);
	
	m_CSListLock.Lock();
	map<uint32 ,customer_t>::iterator l_it; 
   	l_it=m_CSCustomerlist.find(customerId);
   	if(l_it==m_CSCustomerlist.end())	//���������������
	{
		customer_t customer;
		m_CurConsult++; 	//��ǰ��ѯ��
		customer.ReceptionState = 0;
		customer.teamid = teamid;
		customer.userid = customerId;
		m_CSCustomerlist.insert(make_pair(customerId,customer));
	}
   	else	
	{		
		customer_t customer = (l_it->second);
		if(customer.userid != customerId || customer.teamid != teamid )		//�����쳣���޸�
		{			
			CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_ERROR,
				"�Ѵ������Ϊ%u �û� %u ���� �� %u ���� %u �û�",customer.teamid,customer.userid,teamid,customerId);
			//m_CurConsult++; 	//��ǰ��ѯ��
			customer.ReceptionState = 0;	//��ǰ��Ҵ���δ�Ӵ�״̬
			customer.teamid = teamid;
			customer.userid = customerId;
			m_CSCustomerlist[customerId] = customer; 
		}
		else
		{
			CLog::Log("ClientService::AddCustomer_x", CLog::TYPE_IMPORTANT,"�� %u ���� %u �û�", teamid,customerId);
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
		CLog::Log("CustomerQueueDebug_x", CLog::TYPE_IMPORTANT,"��ǰ�� %u  ���û� %u �Ӵ�״̬ %d", 
			pFriendInfo.teamid,
			pFriendInfo.userid,
			pFriendInfo.ReceptionState);
	}
	return;
}

void ClientService::RemoveCustomer_x(uint32 customerId)	//ɾ����ѯ�û�
{
	CLog::Log("CUser::RemoveCustomer_x", CLog::TYPE_IMPORTANT,"׼��ɾ�� �û� %u ",customerId);

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
			"�ͷ� %d ����%u���˳��Ӵ��û� %u,�Ӵ���Ϊ%d ��ѯ�� %d"
			,m_nID,ctx.teamid,customerId,m_CurReception,m_CurConsult);
			
	}
	else
	{		
		CLog::Log("RemoveCustomer_x", CLog::TYPE_IMPORTANT,"���û�%u��������ѯ���еĿͷ�%u��,�޷��ı�״̬��",customerId,m_nID);
	}

	m_CSListLock.Unlock();

	CustomerQueueDebug_x();	
}

void ClientService::ModifyCustomerState_x(uint32 customerId)	//�����û�Ϊ�ѽӴ�
{
	m_CSListLock.Lock();

	map<uint32,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		customer_t customer = (Iter->second);
		if(customer.ReceptionState == 0)
		{	
			customer.ReceptionState = 1;									//����ֻ��Ӵ�״̬
			m_CurReception++;
		}		
		m_CSCustomerlist[customerId] = customer; 
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,
			"�ͷ� %d ����%u�п�ʼ�Ӵ��û� %u,�Ӵ���Ϊ%u ��ѯ�� %u"
			,m_nID,customer.teamid,customerId,m_CurReception,m_CurConsult);
	}
	else
	{		
		CLog::Log("ModifyCustomerState", CLog::TYPE_IMPORTANT,"���û���������ѯ������:%d���޷��ı�״̬��",m_nID);
	}
	m_CSListLock.Unlock();
}


bool ClientService::getCustomerInfo(uint32 customerId,customer_t &info)	//��ȡ�Ӵ��û���Ϣ
{
	map<uint32,customer_t>::iterator Iter;
	Iter = m_CSCustomerlist.find(customerId);
	if(Iter!=m_CSCustomerlist.end())
	{
		info = (Iter->second);		
		CLog::Log("getCustomerInfo", CLog::TYPE_IMPORTANT,
			"�ͷ� %d ����%u�п�ʼ�Ӵ��û� %u,�Ӵ���Ϊ%u ��ѯ�� %u"
			,m_nID,info.teamid,customerId,m_CurReception,m_CurConsult);
		return true;
	}
	else
	{		
		CLog::Log("getCustomerInfo", CLog::TYPE_IMPORTANT,"���û���������ѯ������:%d",customerId);
		return false;
	}

}

bool ClientService::getCustomerTeamId(uint32 customerId,uint32 &teamid)//��ȡ�Ӵ��û���������Ϣ
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

bool ClientService::hasCustomer(uint32 customerId)//�ж��Ƿ���ѯ��
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

		

void ClientService::AddToOnlineList()	//����ǰ���Ҽ�������������
{
	m_CSListLock.Lock();
 	if (m_cshash.find(m_nID) != m_cshash.end())
	{
		CLog::Log("ClientService::AddToOnlineList()", CLog::TYPE_IMPORTANT,"�б����Ѿ�����Ҫ��ӵ��û�:%d��", m_nID);
	}
	else
	{
		m_cshash.insert(make_pair(m_nID, this));
	}
	m_CSListLock.Unlock();
}

void ClientService::DelFromOnlineList()		//����ǰ���Ҵ�����������ɾ��
{
	m_CSListLock.Lock();
	if (m_cshash.find(m_nID) == m_cshash.end())
	{
		CLog::Log("CUser::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "�б��в�����Ҫɾ�����û�:%d��", m_nID);
	}
	else
	{
		m_cshash.erase(m_nID);
	}
	m_CSListLock.Unlock();
}

//����һ���˳�ҵ���
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

int ClientService::PushQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 dst_id)	//���͸�ָ���ͷ��������ӵ��ŶӶ�����
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

		//����ú�����ֱ������������,��ֱ���޸�������б�״̬

		CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //���ҷ����ŶӶ���
		if(!csQueue)
		{				
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"��ѯ�Ŷ�����Ϣʧ�� :%d��", QueueChange->teamId);
		}
		if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
		{					
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"�����û����ŶӶ����� :TEAM %d UID:%u",
					QueueChange->teamId,QueueChange->idBuyer);			
			csQueue->AddToCSQueueList(QueueChange->idBuyer);
		}
		else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
		{			
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"���Ŷ��û��Ӷ������Ƴ� (����ѯ) :TEAM %d UID:%u",
					QueueChange->teamId,QueueChange->idBuyer);
			csQueue->DelFromCSQueueList(QueueChange->idBuyer);				
		}
#endif
	return RESULT_SUCCESS;

}


int ClientService::NotifyTeamQueue(CGlobalBusi *pGlobalBusi, XT_WAITQUEU_CHANGE* QueueChange,uint32 type)	//֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
{
	csTeam_t* csTeam;
	for(unsigned int i = 0 ;i < m_teamMates.size(); ++i )
	{
		csTeam = &(m_teamMates[i]);	//������idȥ��(����Ψһ��)
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"�ڲ�����:TEAM %d ��ѯ����: TEAM %d ,���͸� %u",
					csTeam->teamid,QueueChange->teamId,csTeam->userid);	
		if(csTeam->teamid != QueueChange->teamId)	//��������ͬһ����û�
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
			CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"׼������"); 

			CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi,
				sendData.data,sendData.data_len,SEND_RETRY_TIME);

		}
	}

	//��memcachedֱ��д��ȫ�ַ������У����ز��������Ϣ
#if 0

	//����ú�����ֱ������������,��ֱ���޸�������б�״̬

	CsQueue * csQueue =  CAppManager::GetInstance().GetCSQueueManager().Find(QueueChange->teamId); //���ҷ����ŶӶ���
	if(!csQueue)
	{				
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"��ѯ�Ŷ�����Ϣʧ�� :%d��", QueueChange->teamId);
	}
	if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_ADD)
	{					
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"�����û����ŶӶ����� :TEAM %d UID:%u",
			QueueChange->teamId,QueueChange->idBuyer);			
		csQueue->AddToCSQueueList(QueueChange->idBuyer);
	}
	else if(QueueChange->bAdd == XT_WAITQUEU_CHANGE::TYPE_DELETE)
	{			
		CLog::Log("NotifyTeamQueue", CLog::TYPE_IMPORTANT,"���Ŷ��û��Ӷ������Ƴ� (����ѯ) :TEAM %d UID:%u",
			QueueChange->teamId,QueueChange->idBuyer);
		csQueue->DelFromCSQueueList(QueueChange->idBuyer);				
	}
#endif
	

	return RESULT_SUCCESS;

}

// ��ȡ�ͷ����ڽӴ�����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
uint32 ClientService::GetChattingBuyerID()
{
	return m_nChattingBuyerID;
}
void ClientService::AddTeamMatesList(uint32 teamId,uint32 friendId)	//�Ӻ���
{
	if (friendId == 0 || teamId == 0)
	{
		CLog::Log("ClientService::AddTeamMatesList()",CLog::TYPE_IMPORTANT, "����ͷ��ĺ��� %u ���߶��� %u�쳣",friendId,teamId);
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

// ���ÿͷ����ڽӴ������
void ClientService::SetChattingBuyerID(uint32 nBuyerID)
{
	m_nChattingBuyerID = nBuyerID;
}

// ��ȡ�ͷ����ڱ��ĸ����ܼ��
uint32 ClientService::GetDirectorID()
{
	return m_nDirectorID;
}

// ���ÿͷ����ڱ��ĸ����ܼ��
void ClientService::SetDirectorID(uint32 nDirectorID)
{
	m_nDirectorID = nDirectorID;
	m_bMonitored = (m_nDirectorID != (uint32)-1 && m_nDirectorID != 0);		// �����õļ������id��Ϊ-1ʱ����ʾ�ͷ����ڱ����
}

void ClientService::DelTeamMatesList(uint32 teamId,uint32 friendId)	//ɾ������
{
	if (friendId == 0 || teamId == 0)
	{
		CLog::Log("ClientService::DelTeamMatesList()",CLog::TYPE_IMPORTANT, "����ͷ��ĺ��� %u ���߶��� %u�쳣",friendId,teamId);
	}
	else
	{
		vector<csTeam_t>::iterator Iter;	
		for(Iter = m_teamMates.begin(); Iter != m_teamMates.end(); Iter++) 
		{ 
		  if(Iter->teamid == teamId && Iter->userid == friendId)
		  { 
			 m_teamMates.erase(Iter);
			 Iter = m_teamMates.begin(); //��erase�󣬾ɵ������ᱻ���������һ���µ�����
		  } 
		}		
	}
	DebugTeamMatesList();


}

void ClientService::DebugTeamMatesList()
{
	
	CLog::Log("ClientService:DebugTeamMatesList",CLog::TYPE_IMPORTANT, "��������Ϊ%u (�п��ܻ��ظ�)",m_teamMates.size());
#if 0
	vector<csTeam_t>::iterator Iter;	
	for(Iter = m_teamMates.begin(); Iter != m_teamMates.end(); Iter++) 
	{
		CLog::Log("ClientService::DebugTeamMatesList()",CLog::TYPE_IMPORTANT, 
			"�����б��еĺ����� :    Team %u  Id %u ",Iter->teamid,Iter->userid);	   
	}	
#endif
	return;

}


