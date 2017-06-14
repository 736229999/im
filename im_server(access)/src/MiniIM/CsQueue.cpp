
#include "CsQueue.h"
#include "AppManager.h"
#include "stdafx.h"


CSyncCritical CsQueue::m_CSQueueLock;

CsQueue::CsQueue()
{
	m_nMasterID = 0;				//当前客服组的ID号
	m_Customerlist.clear();	//当前排队的买家
}

CsQueue::~CsQueue()
{
	m_nMasterID = 0;				//当前客服组的ID号
	m_Customerlist.clear();	//当前排队的买家
}


void CsQueue::AddToCSQueueList(uint32 buyerId)
{
	m_CSQueueLock.Lock();
	if(buyerId != 0)
	{
		m_Customerlist.push_back(buyerId);
	}
	
	CLog::Log("CsQueue::AddToOnlineList()", CLog::TYPE_IMPORTANT,"添加排队客服:%u！", buyerId);
	m_CSQueueLock.Unlock();

	DebugCSQueue();
}

int CsQueue::CheckCSQueueSize()
{
	return m_Customerlist.size();	
}

void CsQueue::DebugCSQueue()
{
	CLog::Log("DebugCSQueue",CLog::TYPE_IMPORTANT, "此队列总数为 %u ", m_Customerlist.size());
	list<uint32>::iterator Iter;
	for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++)
	{		
		CLog::Log("DebugCSQueue",CLog::TYPE_IMPORTANT, "排队队列成员 %u ", *Iter);
	}
	return;
}


void CsQueue::DelFromCSQueueList(uint32 buyerId)
{
	m_CSQueueLock.Lock();	

	if (buyerId == 0)
	{
		CLog::Log("CsQueue::DelFromOnlineList()",CLog::TYPE_IMPORTANT, "传入排队队列的客服id 异常:%u！", buyerId);
	}
	else
	{
		list<uint32>::iterator Iter;	
		for(Iter = m_Customerlist.begin(); Iter != m_Customerlist.end(); Iter++) 
		{ 
		  if((*Iter) == buyerId)
		  { 
			 m_Customerlist.erase(Iter);
			 Iter = m_Customerlist.begin(); //当erase后，旧的容器会被重新整理成一个新的容器
		  } 
		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();
}

uint32 CsQueue::PopFromCSQueueList()	//取出最前面的排队客服
{
	m_CSQueueLock.Lock();	
	uint32 popClient = 0;

	if (m_Customerlist.size() == 0)
	{
		CLog::Log("CsQueue::PopFromCSQueueList()",CLog::TYPE_IMPORTANT, "没有出现排队的用户");
	}
	else
	{
		popClient = m_Customerlist.front();	
		m_Customerlist.pop_front();
		CLog::Log("CsQueue::PopFromCSQueueList()",CLog::TYPE_IMPORTANT, "从队列中弹出排队用户 %u",popClient);
	}
	
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return popClient;
}

bool CsQueue::isFirst(uint32 buyerId)	//取出第一个排队客服
{
	m_CSQueueLock.Lock();	
	bool flag = false;
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "传入参数为0");
 	}
	else
	{
		uint32 popClient = m_Customerlist.front();
		if(buyerId == popClient)
		{
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "用户 %u 在队首",popClient);
			flag = true;
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "队首用户 %u 查询用户 %u",buyerId,popClient);
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return flag;
}

bool CsQueue::isInList(uint32 buyerId)	//查询是否在队列中
{
	m_CSQueueLock.Lock();	
	bool flag = false;
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isInList()",CLog::TYPE_IMPORTANT, "传入参数为0");
 	}
	else
	{
		list<uint32>::iterator Iter = find(m_Customerlist.begin(),m_Customerlist.end(),buyerId);	
		if(Iter != m_Customerlist.end())
		{
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "用户 %u 在队中",buyerId);
			flag = true;
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "用户 %u 不在队列中",buyerId);
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return flag;
}

int CsQueue::Findposition(uint32 buyerId)	//查询是否在队列中
{
	m_CSQueueLock.Lock();	
	int pos = 1;		//队列从1 开始计数  0表示可以分流了
	if (buyerId == 0)
	{
		CLog::Log("CsQueue::isInList()",CLog::TYPE_IMPORTANT, "传入参数为0");
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
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "用户 %u 在队中,位置为 %d",buyerId,pos);			
		}
		else
		{			
			CLog::Log("CsQueue::isFirst()",CLog::TYPE_IMPORTANT, "用户 %u 不在队列中",buyerId);
			pos = -1;
 		}		
	}
	m_CSQueueLock.Unlock();
	DebugCSQueue();	
	return pos;
}


