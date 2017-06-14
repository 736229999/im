#pragma once

#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "DataBaseLib.h"

class CGlobalBusi;

typedef struct FAMILYMEM_DATA
{
	uint8		nProp;								//家族成员属性	FAMILY_CREATER|FAMILY_MANAGER|FAMILY_NORMAL	
	uint8		nStat;								//家族成员状态  XTREAM_ONLINE|XTREAM_OFFLINE|XTREAM_LEAVE|XTREAM_HIDE					0x02  //断线
	char		nProfile[MAX_CLUSTER_MEMBER_PROFILE + 1];	// 群名片
	
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
	uint32		m_FamilyID;									//家族ID
	static uint8 m_FamilyProp;								//家族属性。FIRM_CLUSTER 固定群
	uint8		m_JoinFlag;									//申请加入规则
	uint16		m_FamilyClass;								//家族分类	明星、影视等
	uint8		m_FamilyFace;								//族徽代码，0xff表示自定义
	uint8		m_ViewFlag;									//家族页面浏览权限
	char        m_FamilyName[MAX_FAMILYNAME_LEN + 1];		//家族名
	char		m_FamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];	//家族域名
	char        m_FamilyDesc[MAX_FAMILYDESC_LEN + 1];		//家族描述
	uint16      m_FamilyLimit;								//家族人数上限
	char        m_FamilySign[MAX_FAMILYNAME_LEN + 1];//群签名
	char        m_FamilyNotice[MAX_FAMILYNAME_LEN + 1];//群公告 

	uint8		m_FamilyType;								//家族类型 FAMILY_COMMON 普通家族，FAMILY_VIP VIP家族
	char		m_FamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	//自定义族徽连接地址
															//1－家族成员，2－注册用户，3－所有，4－密码访问

	uint32		m_FamilyMaxMsgID;							//家族当前最大消息ID
	uint32		m_FamilyMinMsgID;							//数据库中保存最小消息ID
	uint32		m_FamilyBufMinMsgID;						//缓存中的最小消息ID
	uint32		m_CreatorID;								//创建者ID
	time_t		tm;											//加载时间
	uint32		m_FamilySavedMsgID;							//数据库中已存储的最大消息ID
	uint32 		m_FamilyServerIp;							//所在服务器IP地址

	
	FamilyMember_Map m_FamilyMembers;						//家族所有成员集合
	FamilyMember_List m_FamilyMemberList;					//家族所有成员列表	用户遍历
	FamilyMsg_Map	m_FamilyMsgMap;							//家族消息缓存
	
	bool IsManager(uint32 UserID);
	bool IsCreater(uint32 UserID);
	bool IsMember(uint32 UserID);
	bool AddMember(uint32 UserID,uint8 UserProp = FAMILY_NORMAL, uint8 UserStat = XTREAM_OFFLINE);	
	bool AddMember(char *Profile,uint32 UserID, uint8 UserProp, uint8 UserStat) ;
	bool DelMember(uint32 UserID);
	bool SetManager(uint32 UserID);
	bool CancelManager(uint32 UserID);
	void SetMemStat(uint32 UserID, uint8 UserStat)		//设置成员状态
	{
		CAutoLock mylock(&m_FamilyMembers.m_mapLock);
		FamilyMember_Map::iterator it = m_FamilyMembers.find(UserID);
		if ( it != m_FamilyMembers.end())
			m_FamilyMembers[UserID].nStat = UserStat;
	}
	uint16 GetMaxMemberNum();								//获取该家族最大成员容量
	uint16 GetCurMemberNum();								//获取家族当前所有成员数，包括Creater、Manager
	uint16 GetMaxManagerNum();								//获取该家族最大管理员容量
	uint16 GetCurManagerNum();								//获取家族当前管理员数
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
	static uint8	m_FamilyProp;												//临时家族	TEMP_CLUSTER
	SafeList<uint32>	m_FamilyMemberList;
	uint32 m_FamilyServerIp;													//所在服务器IP地址
	uint16 GetMaxMemberNum(){return MAX_TEMPFAMILYMEM_NUM;};						//获取该家族最大成员容量
	uint16 GetCurMemberNum(){return static_cast<uint16>(m_FamilyMemberList.size());};					//获取家族当前所有成员数，包括Creater、Manager

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

