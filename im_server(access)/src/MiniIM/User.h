#pragma once

#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "SortArray.h"
#include <ext/hash_map>
#include "RouterProtocol.h"
#include "RouterBusiness.h"
#ifdef IM_USE_IMAPI
#include "BusiCacheInterface.h"
#endif
using namespace __gnu_cxx;


typedef SafeMap<uint32,uint32>	FamilyList_Type;		//����ID����ϢID
typedef FamilyList_Type::value_type FamilyList_ValueType;

class CMiniAppNetwork;
class CGlobalBusi;
class CDataBaseMini;
//class ClientService;

struct FriendInfo
{
	uint32 id;
	uint8  status;
	uint8  GroupID;
};

#ifdef IM_USE_IMAPI
// ���ҿͷ��Ƿ����(�Ȳ鱾�����ٲ�memcache)
bool g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi);

// ���ҿͷ��Ƿ����ڱ���أ��������أ���������id(�Ȳ鱾�����ٲ�memcache)
bool g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi);

// ���ҿͷ����ڽӴ������(�Ȳ鱾�����ٲ�memcache)
bool g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi);

// ���ÿͷ����ڱ�˭���(ͬʱ�޸������ļ�memcache)
bool g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CRouterBusiness *glob_busi);

// ���ÿͷ�������ϵ�����(ͬʱ�޸������ļ�memcache)
bool g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CRouterBusiness *glob_busi);
// ��ȡ�ͷ�����
bool g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi);


#endif


class CUser
{
public:
	CUser(void);
	~CUser(void);

	void Reset();

	int NotifyFriendStatus(CGlobalBusi *pGlobalBusi);
	int NotifyServerClose(CGlobalBusi *pGlobalBusi);
	int NotifyFriendInfo(CGlobalBusi *pGlobalBusi);

	//static CUser* GetUserListHead() { return m_pUserListHead; }

	//void AddToOnlineList();

	//void DelFromOnlineList();
	
	void SendLogoutReq(CUser* pUser);
#if 0
	void AddToCSQueueList();

	void DelFromCSQueueList();
#endif
public:
	//��������
	uint32	m_nID;						//�û�ID
	// Normal= 0x00; Hide= 0x02; Offline= 0x03;  Leave= 0x01;
	uint8 	m_nflow;			//�û�������ʽ
	uint8   m_nStatus ;                  //�û�״̬
	uint16	m_ClientVersion;			//�û�ʹ�õĿͻ��˰汾
	uint8	m_nSexy;					//�û��Ա�0-Ů��1-�У�2-δ֪
	char	m_szHeadURL[MAX_HEADURL_LEN+1];		//�Զ���ͷ��URL
	char	m_szNickName[MAX_NICKNAME_LEN+1];	//�س�
	char	m_szUserName[MAX_USERNAME_LEN+1];	//�û���
	bool	m_bIsFullInfoValid;
	//��ϸ����
	char	m_szEmail[MAX_EMAIL_LEN+1];				//Email��ַ
	char	m_szBirthday[MAX_BIRTHDAY_LEN+1];		//����
	uint8	m_nCountry;								//����ID
	uint8	m_nProvince;							//ʡ��ID
	uint8	m_nCity;								//����ID
	uint8	m_nCareer;								//ְҵID
	uint8	m_AllowRule;							//�Ӻ�����֤��ʽ
	uint8	m_FamilyFlag;							//��Ϊ�����Ա��֤��ʽ
	uint32	m_nMoney;								//�û��������ֵ
//	uint8	m_nLevel;								//�û��ȼ�
	uint32	m_nOnlineTime;							//�û��ۼ�����ʱ��
	uint8	m_linkpop;								//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char	m_szAddress[MAX_ADDRESS_LEN+1];			//��ַ
	char	m_szPhone[MAX_PHONE_LEN+1];				//ͨѶ����
	char	m_szDescription[MAX_DESCRIPTION_LEN+1];	//����˵��
	char	m_szUserSign[MAX_USERSIGN_LEN + 1];		//����ǩ��	
	char	m_SessionKey[MAX_SESSIONKEY_LEN];		//Session Key
	char	m_P2PSessionKey[MAX_SESSIONKEY_LEN];	//P2P Session Key

	char	m_szAvatoruri[MAX_AVATOR_LEN + 1];		//�����ַ
	uint8   m_nMobileStatus;						//���ֻ�  0δ�󶨣�1�Ѱ� 2�Ѱ󶨲��Ҷ�����ҵ��
	uint32	m_nIntegral;							//�û��ܻ���
	char 	m_szGUID[MAX_GUID_LEN + 1];				//GUID
	
	//�������ʱ��

	uint32  m_nNewlyLiveTime;
	uint32  m_nLoginTime;
	uint32  m_nLoginType; //�ο��û���¼���� flag
	uint32	m_nIP;
	uint16	m_nPort;

	uint32	m_nLanIP;
	uint16	m_nLanPort;
	uint32  m_nServerIp; 			//���ڷ�����IP��ַ
	uint8 	flag;					 //��¼��ʽ
	uint8   m_nLevel;				//�û��ȼ�
	uint8	m_nUsertype;				//���   ����  ���ǿͷ�
	uint8	m_nQueueflag;				//�Ƿ��Ŷ� 0:���Ŷ� 1:�Ŷ���
	uint32	m_nCSuserid;				//��Ϊ���ʱ�Ӵ��ͷ���id
	uint32	m_nCSTeamid;				//��Ϊ���ʱ�Ӵ��ͷ�����
	uint32  m_merchantid;
	uint32  m_nCSRecptionLimit;			//�ͷ��Ӵ�����
	char	m_nWorkGroup[MAX_WORKGROUP_LEN +1];			//��������û�����Ϣ

	bool m_bMonitored;	// �ͷ��Ƿ񱻼��
	uint32 m_nChattingBuyerID;		// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
	uint32 m_nDirectorID;	// ����ͷ�����أ���¼�����������id
#ifdef IM_USE_IMAPI

	// ���ÿͷ����ڽӴ������
	void SetChattingBuyerID(uint32 nBuyerID)
	{
		m_nChattingBuyerID = nBuyerID;
	};
	// ��ȡ�ͷ����ڽӴ�����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
	uint32 GetChattingBuyerID()
	{
		return m_nChattingBuyerID;
	};

	// ��ȡ�ͷ����ڱ��ĸ����ܼ��
	uint32 GetDirectorID();

	// ���ÿͷ����ڱ��ĸ����ܼ��
	void SetDirectorID(uint32 nDirectorID);

	// �жϿͷ��Ƿ����ڱ����
	bool IsMonitored()		{	return 	m_bMonitored;		}
	
	// ���ҿͷ��Ƿ����(�Ȳ鱾�����ٲ�memcache)
	bool g_cs_exist(uint32 cs_id, CGlobalBusi *glob_busi);
	// ���ҿͷ��Ƿ����ڱ���أ��������أ���������id(�Ȳ鱾�����ٲ�memcache)
	bool g_cs_ismonitored(uint32 cs_id, bool &monitored, uint32 &cs_master_id, CGlobalBusi *glob_busi);
	// ���ҿͷ����ڽӴ������(�Ȳ鱾�����ٲ�memcache)
	bool g_cs_get_chatting_buyer(uint32 cs_id, uint32 &chatting_buyer_id, CGlobalBusi *glob_busi);
	// ���ÿͷ����ڱ�˭���(ͬʱ�޸������ļ�memcache)
	bool g_cs_set_director(uint32 cs_id, uint32 cs_master_id, CRouterBusiness *glob_busi);
	// ���ÿͷ�������ϵ�����(ͬʱ�޸������ļ�memcache)
	bool g_cs_set_chatting_buyer(uint32 cs_id, uint32 chattring_buyer_id, CRouterBusiness *glob_busi);
	// ��ȡ�ͷ�����
	bool g_cs_get_name(uint32 cs_id, char name[], CGlobalBusi *glob_busi);

	
#endif
#ifdef IM_USE_MULTIDEVICE
	vector<multiDevice_t> m_multiDeviceinfo;
#endif
	volatile uint8 m_isLogout;	//�жϸ��û��Ƿ����ڵǳ�	1���ڵǳ� 0 δ�ǳ�
	//�û�����ļ�����Ϣ
	FamilyList_Type	m_PermFamilyMap;						//�û�����Ĺ̶������б� key:familyid, value:curmsgid
	bool Del_Perm_Family(uint32 nFamilyID);					//���û���m_PermFamilyMap��ɾ��һ������
	bool Add_Perm_Family(uint32 nFamilyID, uint32 nMsgID = 0);	//���û���m_PermFamilyMap�����һ������,nMsgID��ǰ�������ϢID
	void Set_Family_MsgID(uint32 nFamilyID, uint32 nMsgID);	//���ù̶�������ϢID
	SafeList<uint32>	m_TempFamilyList;					//�û��������ʱ�����б�	
	bool Add_Temp_Family(uint32 nFamilyID);
	bool Del_Temp_Family(uint32 nFamilyID);
	//��ȡ���ڽӴ��Ŀͷ�ID
	int getCsuserId();
	void setCsuserId(uint32 CSid);
	//��ȡ���ڽӴ��Ŀͷ�����
	int getCsTeamId();
	void setCsTeamId(uint32 CSTeamid);
	void ClearWebClient();	//��յ�ǰweb�û��ڿͷ��ϵ�������
	
	void NotifyFamilyStatus(CMiniAppNetwork * pAppNetwork,CGlobalBusi* pGlobalFamilyBusi,CDataBaseMini* pDataBaseMini);	//modify by lwl ver:0105							//向家族发送状态通知
	void LogoutTempFamily(CMiniAppNetwork * pAppNetwork ,CGlobalBusi* pGlobalFamilyBusi, CGlobalBusi *pGlobalBusi);

	CSortArray<FriendInfo>		m_FriendList;			//�����б�

	uint32 m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nMobileMsgID,m_nUsercommentID,m_nLogcommentID;
	//CUser * m_next;
	//CUser * m_pre;
	//static __gnu_cxx::hash_map<uint32, CUser*> m_userhash;
//	static __gnu_cxx::hash_map<uint32, vector<CUser*>> m_userQueue;		//�����Ŷ���ҵĶ���
	//static CUser* m_pUserListHead;
	//static CSyncCritical m_UserListLock;	
};

