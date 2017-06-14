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

		void AddToCSQueueList(uint32 buyerId); //����ǰ�ͷ������Ŷ�������
		void DelFromCSQueueList(uint32 buyerId);//����ǰ�ͷ�������������ɾ��
		int CheckCSQueueSize();
		int isQueue(){return m_Customerlist.size();}	//ȷ����ǰ���Ƿ��Ŷ�
		void DebugCSQueue();
		uint32 getCSID(){return this->m_nMasterID;}
		uint32 setCSID(uint32 id){this->m_nMasterID = id; return this->m_nMasterID;}
		int Findposition(uint32 buyerId);	//��ѯ�ڶ����е�λ��		
		uint32 PopFromCSQueueList();	//ȡ����ǰ����Ŷӿͷ�		
		bool isFirst(uint32 buyerId);	//ȡ����һ���Ŷӿͷ�		
		bool isInList(uint32 buyerId);	//��ѯ�Ƿ��ڶ�����
		static CSyncCritical m_CSQueueLock;
		
	private:
		uint32 m_nMasterID;				//��ǰ�ͷ����ID��
		list<uint32> m_Customerlist;	//��ǰ�Ŷӵ����

//		static __gnu_cxx::hash_map<uint32, ClientService*> m_cshash;

};

	

