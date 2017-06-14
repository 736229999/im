#pragma once

#include "protocol.h"
#include "PublicType.h"
#include "Utility.h"
#include "SortArray.h"
#include <ext/hash_map>
using namespace __gnu_cxx;


typedef SafeMap<uint32,uint32>	FamilyList_Type;		//家族ID，消息ID
typedef FamilyList_Type::value_type FamilyList_ValueType;

class CMiniAppNetwork;
class CGlobalBusi;
class CDataBaseMini;

struct FriendInfo
{
	uint32 id;
	uint8  status;
	uint8  GroupID;
};


class CUser
{
public:
	CUser(void);
	~CUser(void);

	void Reset();

	int NotifyFriendStatus();
	int NotifyServerClose();
	int NotifyFriendInfo();

	//static CUser* GetUserListHead() { return m_pUserListHead; }

	void AddToOnlineList();

	void DelFromOnlineList();

public:
	//基本资料
	uint32	m_nID;						//用户ID
	// Normal= 0x00; Hide= 0x02; Offline= 0x03;  Leave= 0x01;
	uint8   m_nStatus ;                  //用户状态
	uint16	m_ClientVersion;			//用户使用的客户端版本
	uint8	m_nSexy;					//用户性别0-女，1-男，2-未知
	char	m_szHeadURL[MAX_HEADURL_LEN+1];		//自定义头像URL
	char	m_szNickName[MAX_NICKNAME_LEN+1];	//呢称
	char	m_szUserName[MAX_USERNAME_LEN+1];	//用户名

	bool	m_bIsFullInfoValid;
	//详细资料
	char	m_szEmail[MAX_EMAIL_LEN+1];				//Email地址
	char	m_szBirthday[MAX_BIRTHDAY_LEN+1];		//生日
	uint8	m_nCountry;								//国家ID
	uint8	m_nProvince;							//省份ID
	uint8	m_nCity;								//城市ID
	uint8	m_nCareer;								//职业ID
	uint8	m_AllowRule;							//加好友认证方式
	uint8	m_FamilyFlag;							//列为家族成员认证方式
	uint32	m_nMoney;								//用户虚拟货币值
//	uint8	m_nLevel;								//用户等级
	uint32	m_nOnlineTime;							//用户累计在线时长
	uint8	m_linkpop;								//联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char	m_szAddress[MAX_ADDRESS_LEN+1];			//地址
	char	m_szPhone[MAX_PHONE_LEN+1];				//通讯号码
	char	m_szDescription[MAX_DESCRIPTION_LEN+1];	//个人说明
	char	m_szUserSign[MAX_USERSIGN_LEN + 1];		//个人签名	
	char	m_SessionKey[MAX_SESSIONKEY_LEN];		//Session Key
	char	m_P2PSessionKey[MAX_SESSIONKEY_LEN];	//P2P Session Key

	char	m_szAvatoruri[MAX_AVATOR_LEN + 1];		//闪秀地址
	uint8   m_nMobileStatus;						//绑定手机  0未绑定，1已绑定 2已绑定并且订制了业务
	uint32	m_nIntegral;							//用户总积分

	
	//最后心跳时间
	uint32  m_nNewlyLiveTime;
	uint32  m_nLoginTime;
	uint32  m_nLoginType; //参考用户登录请求 flag
	uint32	m_nIP;
	uint16	m_nPort;

	uint32	m_nLanIP;
	uint16	m_nLanPort;

	uint32  m_merchantid;
	uint32  m_nXmppID;		//xmpp���ӵ�id

	//用户加入的家族信息
	FamilyList_Type	m_PermFamilyMap;						//用户加入的固定家族列表 key:familyid, value:curmsgid
	bool Del_Perm_Family(uint32 nFamilyID);					//从用户的m_PermFamilyMap中删除一个家族
	bool Add_Perm_Family(uint32 nFamilyID, uint32 nMsgID = 0);	//在用户的m_PermFamilyMap中添加一个家族,nMsgID当前家族的消息ID
	void Set_Family_MsgID(uint32 nFamilyID, uint32 nMsgID);	//设置固定家族消息ID
	SafeList<uint32>	m_TempFamilyList;					//用户加入的临时家族列表	
	bool Add_Temp_Family(uint32 nFamilyID);
	bool Del_Temp_Family(uint32 nFamilyID);

	void NotifyFamilyStatus(CMiniAppNetwork * pAppNetwork,CGlobalBusi* pGlobalFamilyBusi,CDataBaseMini* pDataBaseMini);	//modify by lwl ver:0105							//向家族发送状态通知
	void LogoutTempFamily(CMiniAppNetwork * pAppNetwork ,CGlobalBusi* pGlobalFamilyBusi);

	CSortArray<FriendInfo>		m_FriendList;			//好友列表

	uint32 m_nOperationID,m_nUserMsgID,m_nSysMsgID,m_nMobileMsgID,m_nUsercommentID,m_nLogcommentID;
	//CUser * m_next;
	//CUser * m_pre;
	static __gnu_cxx::hash_map<uint32, CUser*> m_userhash;
	//static CUser* m_pUserListHead;
	static CSyncCritical m_UserListLock;
};

