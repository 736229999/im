
#include "CsQueue.h"
#include "AppManager.h"
#include "stdafx.h"


CSyncCritical CsQueue::m_CSQueueLock;

CsQueue::CsQueue()
{
	m_nMasterID = 0;				//��ǰ�ͷ����ID��
	m_Customerlist.clear();	//��ǰ�Ŷӵ����
}

CsQueue::~CsQueue()
{
	m_nMasterID = 0;				//��ǰ�ͷ����ID��
	m_Customerlist.clear();	//��ǰ�Ŷӵ����
}


void CsQueue::AddToCSQueueList(uint32 buyerId)
{
	m_CSQueueLock.Lock();
	if(buyerId != 0)
	{
		m_Customerlist.push_back(buyerId);
	}
	
	CLog::Log("CsQueue::AddToOnlineList()", CLog::TYPE_IMPORTANT,"����Ŷӿͷ�:%u��", buyerId);
	m_CSQueueLock.Unlock();

	DebugCSQueue();
}

int CsQueue::CheckCSQueueSize()
{
	return m_Customerlist.size();	
}

void CsQueue::DebugCSQueue()
{
	CLog::Log("DebugCSQueue",CLog::TYPE_IMPORTANT, "�˶�������Ϊ %u ", m_Customerlist.size());
	list<uint32>::iterator Iter;
	for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++)
	{		
		CLog::Log("DebugCSQueue",CLog::TYPE_IMPORTANT, "�ŶӶ��г�Ա %u ", *Iter);
	}
	return;
}


void CsQueue::DelFromCSQueueList(uint32 buyerId)
{
	m_CSQueueLock.Lock();	

	if (buyerId == 0)
	{
		CLog::Log("CsQueue::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "�����ŶӶ��еĿͷ�id �쳣:%u��", buyerId);
	}
	else
	{
		list<uint32>::iterator Iter;	
		for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++) 
		{ 
		  if((*Iter) == buyerId)
		  { 
			 m_Customerlist.erase(Iter);
			 Iter = m_Customerlist.begin(); //��erase�󣬾ɵ������ᱻ���������һ���µ�����
		  } 
		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();
}

uint32 CsQueue::PopFromCSQueueList()	//ȡ����ǰ����Ŷӿͷ�
{
	m_CSQueueLock.Lock();	
	uint32 popClient = 0;

	if (m_Customerlist.size() == 0)
	{
		CLog::Log("CsQueue::PopFromCSQueueList()",CLog::TYPE_IMPORTANT, "û�г����Ŷӵ��û�");
	}
	else
	{
		popClient = m_Customerlist.front();	
		m_Customerlist.pop_front();
		CLog::Log("CsQueue::PopFromCSQueueList()",CLog::TYPE_IMPORTANT, "�Ӷ����е����Ŷ��û� %u",popClient);
	}
	
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return popClient;
}

bool CsQueue::isFirst(uint32 buyerId)	//ȡ����һ���Ŷӿͷ�
{
	m_CSQueueLock.Lock();	
	bool flag = false;
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�������Ϊ0");
 	}
	else
	{
		uint32 popClient = m_Customerlist.front();
		if(buyerId == popClient)
		{
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�û� %u �ڶ���",popClient);
			flag = true;
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�����û� %u ��ѯ�û� %u",buyerId,popClient);
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return flag;
}

bool CsQueue::isInList(uint32 buyerId)	//��ѯ�Ƿ��ڶ�����
{
	m_CSQueueLock.Lock();	
	bool flag = false;
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isInList()",CLog::TYPE_IMPORTANT, "�������Ϊ0");
 	}
	else
	{
		list<uint32>::iterator Iter = find(m_Customerlist.begin(),m_Customerlist.end(),buyerId);	
		if(Iter != m_Customerlist.end())
		{
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�û� %u �ڶ���",buyerId);
			flag = true;
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�û� %u ���ڶ�����",buyerId);
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return flag;
}

int CsQueue::Findposition(uint32 buyerId)	//��ѯ�Ƿ��ڶ�����
{
	m_CSQueueLock.Lock();	
	int pos = 1;		//���д�1 ��ʼ����  0��ʾ���Է�����
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isInList()",CLog::TYPE_IMPORTANT, "�������Ϊ0");
 	}
	else
	{	list<uint32>::iterator Iter;
		for(Iter = 	m_Customerlist.begin();
			Iter != m_Customerlist.end();	Iter++,pos++)
		{			
			if(*Iter == buyerId)
				break;
		}
		if(Iter != m_Customerlist.end())
		{
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�û� %u �ڶ���,λ��Ϊ %d",buyerId,pos);			
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "�û� %u ���ڶ�����",buyerId);
			pos = -1;
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return pos;
}


