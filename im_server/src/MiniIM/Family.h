#pragma once

#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "DataBaseLib.h"

class CGlobalBusi;

typedef struct FAMILYMEM_DATA
{
	uint8		nProp;								//�����Ա����	FAMILY_CREATER|FAMILY_MANAGER|FAMILY_NORMAL	
	uint8		nStat;								//�����Ա״̬  XTREAM_ONLINE|XTREAM_OFFLINE|XTREAM_LEAVE|XTREAM_HIDE					0x02  //����
	char		nProfile[MAX_CLUSTER_MEMBER_PROFILE + 1];	// Ⱥ��Ƭ
	
	FAMILYMEM_DATA()
	{
		memset(this,0,sizeof(FAMILYMEM_DATA));
	}
	FAMILYMEM_DATA(uint8 prop, uint8 stat)
	{
		nProp = prop;
		nStat = stat;
		nProfile[0] = '\0'; 
	}
}FAMILYMEM_DATA,* LPFAMILYMEM_DATA;

typedef struct FAMILYMSG_DATA
{
	uint32 recvID;
	uint32 familyMsgID;
	uint32 familyID;
	uint32 senderID;
	int tm;
	int msgType;
	int msgLen;
	char msg[MAX_MSG_LEN+1];
}FAMILYMSG_DATA, *LPFAMILYMSG_DATA;

typedef SafeList<uint32> FamilyMember_List;
typedef SafeMap<uint32, FAMILYMEM_DATA>	FamilyMember_Map;
typedef SafeMap<uint32, FAMILYMSG_DATA*>	FamilyMsg_Map;

class CPermFamily
{
public:
	CPermFamily(void);
	~CPermFamily(void);
public:
	uint32		m_FamilyID;									//����ID
	static uint8 m_FamilyProp;								//�������ԡ�FIRM_CLUSTER �̶�Ⱥ
	uint8		m_JoinFlag;									//����������
	uint16		m_FamilyClass;								//�������	���ǡ�Ӱ�ӵ�
	uint8		m_FamilyFace;								//��մ��룬0xff��ʾ�Զ���
	uint8		m_ViewFlag;									//����ҳ�����Ȩ��
	char        m_FamilyName[MAX_FAMILYNAME_LEN + 1];		//������
	char		m_FamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];	//��������
	char        m_FamilyDesc[MAX_FAMILYDESC_LEN + 1];		//��������
	uint16      m_FamilyLimit;								//������������
	char        m_FamilySign[MAX_FAMILYNAME_LEN + 1];//Ⱥǩ��
	char        m_FamilyNotice[MAX_FAMILYNAME_LEN + 1];//Ⱥ���� 

	uint8		m_FamilyType;								//�������� FAMILY_COMMON ��ͨ���壬FAMILY_VIP VIP����
	char		m_FamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	//�Զ���������ӵ�ַ
															//1�������Ա��2��ע���û���3�����У�4���������

	uint32		m_FamilyMaxMsgID;							//���嵱ǰ�����ϢID
	uint32		m_FamilyMinMsgID;							//���ݿ��б�����С��ϢID
	uint32		m_FamilyBufMinMsgID;						//�����е���С��ϢID
	uint32		m_CreatorID;								//������ID
	time_t		tm;											//����ʱ��
	uint32		m_FamilySavedMsgID;							//���ݿ����Ѵ洢�������ϢID
	uint32 		m_FamilyServerIp;							//���ڷ�����IP��ַ

	
	FamilyMember_Map m_FamilyMembers;						//�������г�Ա����
	FamilyMember_List m_FamilyMemberList;					//�������г�Ա�б�	�û�����
	FamilyMsg_Map	m_FamilyMsgMap;							//������Ϣ����
	
	bool IsManager(uint32 UserID);
	bool IsCreater(uint32 UserID);
	bool IsMember(uint32 UserID);
	bool AddMember(uint32 UserID,uint8 UserProp = FAMILY_NORMAL, uint8 UserStat = XTREAM_OFFLINE);	
	bool AddMember(char *Profile,uint32 UserID, uint8 UserProp, uint8 UserStat) ;
	bool DelMember(uint32 UserID);
	bool SetManager(uint32 UserID);
	bool CancelManager(uint32 UserID);
	void SetMemStat(uint32 UserID, uint8 UserStat)		//���ó�Ա״̬
	{
		CAutoLock mylock(&m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = m_FamilyMembers.find(UserID);
		if ( it != m_FamilyMembers.end())
			m_FamilyMembers[UserID].nStat = UserStat;
	}
	uint16 GetMaxMemberNum();								//��ȡ�ü�������Ա����
	uint16 GetCurMemberNum();								//��ȡ���嵱ǰ���г�Ա��������Creater��Manager
	uint16 GetMaxManagerNum();								//��ȡ�ü���������Ա����
	uint16 GetCurManagerNum();								//��ȡ���嵱ǰ����Ա��
	uint8 SaveFamilyMsg(uint32 familyMsgID, uint32 senderID, int tm, int msgType,int msgLen,const char *msg);
	uint8 GetFamilyMsg(uint32 &familyMsgID,uint32 userID, ST_OFFLINE_MSG* lpMsg, uint8 &nRetCount, CDataBaseMini *pDataBaseMini);
	int GetOfflineFamilyMsg(uint32 minMsgID,CDataBaseMini *pDataBaseMini);
	void SendFamilyMsg(CGlobalBusi *pGlobalBusi, ST_OFFLINE_MSG* pOfflineMsg);
	void Reset();
};


class CTempFamily
{
public:
	CTempFamily();
	~CTempFamily();
	uint32			m_FamilyID;
	static uint8	m_FamilyProp;												//��ʱ����	TEMP_CLUSTER
	SafeList<uint32>	m_FamilyMemberList;
	uint32 m_FamilyServerIp;													//���ڷ�����IP��ַ
	uint16 GetMaxMemberNum(){return MAX_TEMPFAMILYMEM_NUM;};						//��ȡ�ü�������Ա����
	uint16 GetCurMemberNum(){return static_cast<uint16>(m_FamilyMemberList.size());};					//��ȡ���嵱ǰ���г�Ա��������Creater��Manager

	bool AddMember(uint32 UserID)
	{
		CAutoLock mylock(&m_FamilyMemberList.m_lstLock);
		if (GetCurMemberNum() >= GetMaxMemberNum())
			return RESULT_FAILED;
		if (find(m_FamilyMemberList.begin(), m_FamilyMemberList.end(), UserID) == m_FamilyMemberList.end())
			m_FamilyMemberList.push_back(UserID);
		return RESULT_SUCCESS;
	};
	bool DelMember(CGlobalBusi *pGlobalBusi, uint32 UserID, CGlobalBusi* pGlobalFamilyBusi);


	void Reset();

};

