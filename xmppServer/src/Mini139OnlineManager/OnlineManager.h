#pragma once
/************************************************************************ 
�ļ�������ȫ�����߹�����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-9 
************************************************************************/

#include "OnlineUser.h"
#include "xtpacket.h"
#include "SyncCritical.h"
#include "Utility.h"
#include "ZObjectPool.h"
#include "ConfigInfo.h"


struct ReloginUser
{
	uint32 userid;
	uint32 serverip;
	uint32 userip;
	uint32 lanip;
};

typedef SafeMap<uint32, uint32> MerchantAccount_Map;
typedef SafeMap<uint32, uint32> GroupAccount_Map;

class COnlineManager
{
public:
	COnlineManager(void);
	~COnlineManager(void);

	//���/�޸�һ�����ߵ��û�
	bool AddModifyOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,
							ReloginUser* pReloginUserList,uint8& nReloginCount);
	bool DelUser(uint32 userid);

	//ɾ��һ�������û�
	bool DelOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket,uint32 fromip=0);

	//�õ��û���ϸ��Ϣ
	bool GetUserFullInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);
	
	//�õ��ļ�����������û���Ϣ
	bool GetTUserInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//�õ��û�״̬��Ϣ
	bool GetUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//�޸��û�״̬��Ϣ
	bool ModifyUserStatus(CRecvPacket& RecvPacket,CSendPacket& SendPacket);


	//��ȡ������Ϣ
	bool GetUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket);
	//�޸ķ�����Ϣ
	bool ModifyUserFlow(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//��ȡ������˺�
	bool GetBestAccount(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//�õ��û���·��Ϣ
	bool GetUserLinkInfo(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//���������û�
	bool SearchOnlineUser(CRecvPacket& RecvPacket,CSendPacket& SendPacket);

	//ĳ̨IM������ͣ����
	void IMServerShutdown(uint32 serverip);

	//֪ͨ�ļ�����������û���¼
	void NotifyTransferLogin(COnlineUser* pNewUser);

	uint32 GetOnlineUserNum()
	{
		return m_nOnlineNum;
	}
	
	COnlineUser* GetUser(uint32 id)
	{
		if( id<0 || id >= MAX_139_ACCOUNT_NUM )
		{
			return NULL;
		}
		return m_pGlobalOnline[id]; 
	}

private:

	COnlineUser**		m_pGlobalOnline;	//�����úŵ�����
	COnlineUser*		m_pOnlineList;		//�����û��б�

	COnlineUser*		m_pCurrentSearch;	//���������û��ĵ�ǰ����

	CZObjectPool<COnlineUser>	m_ObjectPool;
	
	uint32				m_nOnlineNum;		//�����û�����

	CSyncCritical		m_CriticOnlineList;
	CSyncCritical		m_CriticSearch;

	MerchantAccount_Map	Merchant_ids;
	GroupAccount_Map	Group_ids;

};
