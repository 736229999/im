#pragma once
	
#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "SortArray.h"
#include <ext/hash_map>
using namespace __gnu_cxx;

class CsQueue
{
	public:
		CsQueue();
		~CsQueue();		

		void AddToCSQueueList(uint32 buyerId); //将当前客服加入排队链表中
		void DelFromCSQueueList(uint32 buyerId);//将当前客服从在线链表中删除
		int CheckCSQueueSize();
		int isQueue(){return m_Customerlist.size();}	//确定当前组是否排队
		void DebugCSQueue();
		uint32 getCSID(){return this->m_nMasterID;}
		uint32 setCSID(uint32 id){this->m_nMasterID = id; return this->m_nMasterID;}
		int Findposition(uint32 buyerId);	//查询在队列中的位置		
		uint32 PopFromCSQueueList();	//取出最前面的排队客服		
		bool isFirst(uint32 buyerId);	//取出第一个排队客服		
		bool isInList(uint32 buyerId);	//查询是否在队列中
		static CSyncCritical m_CSQueueLock;
		
	private:
		uint32 m_nMasterID;				//当前客服组的ID号
		list<uint32> m_Customerlist;	//当前排队的买家

//		static __gnu_cxx::hash_map<uint32, ClientService*> m_cshash;

};

	

