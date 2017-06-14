/************************************************************************ 
 �ļ����������������Э�鶨�壬���������֮��Ľ���Э��
		   
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/

#ifndef __SRVFRAMEPROTOCOL_H_
#define __SRVFRAMEPROTOCOL_H_
#include "protocol.h"
#include "ClientProtocol.h"
#include <vector>
using std::vector;

//#pragma pack(push,1)

//Э�鶨�� ...

/***
ȫ�ַ�����ҵ��Э��
***/
//�ļ������������ѯ�û���Ϣ
typedef struct tagGSRV_TUSERINFO_QUERY
{
	uint32 id;
}GSRV_TUSERINFO_QUERY,*LGSRV_TUSERINFO_QUERY;

typedef struct tagGSRV_TUSERINFO_ACK
{
	uint8 ret;
	uint32 id;
	uint32 ip;
	uint16 port;
	char sessionkey[MAX_SESSIONKEY_LEN];
	uint8 flag;
	uint32 newlylivetime;	// ��½ʱ��
	uint32 merchantID;		// merchantID
	uint32 serverip;		// ������ip	
}GSRV_TUSERINFO_ACK,*LGSRV_TUSERINFO_ACK;

typedef struct tagGSRV_TUSERINFO_ACK_EX
{
	uint8 ret;
	uint32 id;
	uint32 ip;
	uint16 port;
	char sessionkey[MAX_SESSIONKEY_LEN];
	uint8 flag;
	uint32 newlylivetime;	// ��½ʱ��
	uint32 merchantID;		// merchantID
	uint32 serverip;		// ������ip	
	uint8 status;			// ״̬
	uint8 flow;				// ����״̬	
}GSRV_TUSERINFO_ACK_EX,*LGSRV_TUSERINFO_ACK_EX;

//��ѯ�����û�������Ϣ
typedef struct tagGSRV_USERCONTROLINFO_QUERY
{
	uint8 count;
	uint32 uid[1];
}GSRV_USERCONTROLINFO_QUERY, * LPGSRV_USERCONTROLINFO_QUERY;

//��ѯ�����û�������ϢӦ��
typedef struct tagGSRV_USERCONTROLINFO_QUERY_ACK
{
	uint8 ret;
	uint8 count;
	struct tagUserControlInfo
	{
		uint32 uid;
		uint32 srvip;
		char sessionKey[MAX_SESSIONKEY_LEN];
		char p2pSessionKey[MAX_SESSIONKEY_LEN];
		uint8 teamID;	
		uint8 flag;
	}userControlInfo[1];
}GSRV_USERCONTROLINFO_QUERY_ACK, * LPGSRV_USERCONTROLINFO_QUERY_ACK;

//���������û��ķ���Ӧ��
typedef struct tagGSRV_SEARCH_ONLINE_USER_ACK_HEAD
{
	uint8 result;
	uint8  count;
	uint32 onlineNum;
	struct tagUserInfo
	{
		uint32 clientid;
		uint8	sexy;
		uint8	status;
		uint8	rule;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
	}UserInfo[1];
} GSRV_SEARCH_ONLINE_USER_ACK_HEAD,*LPGSRV_SEARCH_ONLINE_USER_ACK_HEAD;


//��ȡ���ѣ��Լ����������ϵ�������ṹ
// typedef struct tagIMSRV_GET_FRIEND_BASICINFO_REQ
// {
// 	uint8  count;
// 	uint32 FriendID[1];
// 
// }IMSRV_GET_FRIEND_BASICINFO_REQ,*LPIMSRV_GET_FRIEND_BASICINFO_REQ;
//��ȡ���ѣ��Լ����������ϵ�Ӧ����ṹ

// typedef struct tagIMSRV_GET_FRIEND_BASICINFO_ACK
// {
// 	uint8  result;
// 	uint8  count;
// 
// 	struct tagFriendInfo
// 	{
// 		uint32  nID;
// 		char	szHeadUrl[MAX_HEADURL_LEN+1];
// 		uint8	nSexy;
// 		char	szNickName[MAX_NICKNAME_LEN+1];
// 		char	szUserName[MAX_USERNAME_LEN+1];
// 		uint16	nVersion;
// 
// 	} FriendInfo[1];
// 
// }IMSRV_GET_FRIEND_BASICINFO_ACK,*LPIMSRV_GET_FRIEND_BASICINFO_ACK;


//��ѯ�����û�״̬����
typedef struct tagGSRV_GET_USER_STATUS_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_STATUS_QUERY,*LPGSRV_GET_USER_STATUS_QUERY;

//��ѯ�����û�״̬Ӧ��
typedef struct tagGSRV_GET_USER_STATUS_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserStatus
	{
		uint32 id;
		uint8  status;	
	} UserStatus[1];
} GSRV_GET_USER_STATUS_ACK ,*LPGSRV_GET_USER_STATUS_ACK;

//�޸������û�״̬����
typedef struct tagGSRV_MODIFY_ONLINE_STATUS_QUERY
{
	uint8 count;
	struct tagUser
	{
		uint32 id;
		uint8  status;
	} user[1];
} GSRV_MODIFY_ONLINE_STATUS_QUERY,*LPGSRV_MODIFY_ONLINE_STATUS_QUERY;

//�޸������û�״̬Ӧ��
typedef struct tagGSRV_MODIFY_ONLINE_STATUS_ACK
{
	uint8 result;
	uint8 count;
	struct tagUser
	{
		uint32 id;
		uint8  status;
	} user[1];
} GSRV_MODIFY_ONLINE_STATUS_ACK,*LPGSRV_MODIFY_ONLINE_STATUS_ACK;

//��ȡ������˺�����
typedef struct tagGSRV_GET_BEST_ACCOUNT_QUERY
{
	uint32 merchant_id;
	uint32 group_id;
	uint32 flow_type;
	uint8 count;
	uint32 id[1];
}GSRV_GET_BEST_ACCOUNT_QUERY,*LPGSRV_GET_BEST_ACCOUNT_QUERY;

//��ȡ������˺�Ӧ��
typedef struct tagGSRV_GET_BEST_ACCOUNT_ACK
{
	uint8 result;
	uint32 account;
}GSRV_GET_BEST_ACCOUNT_ACK,*LPGSRV_GET_BEST_ACCOUNT_ACK;
/*
	------------------��������------------------
*/

//��ѯ������������
typedef struct tagGSRV_GET_USER_FLOW_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_FLOW_QUERY,*LPGSRV_GET_USER_FLOW_QUERY;

//��ѯ��������Ӧ��
typedef struct tagGSRV_GET_USER_FLOW_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserFlow
	{
		uint32 id;
		uint8  status;
		uint8  flow;
	} UserFlow[1];
} GSRV_GET_USER_FLOW_ACK ,*LPGSRV_GET_USER_FLOW_ACK;


//�޸ķ�����������
typedef struct tagGSRV_MODIFY_USER_FLOW_QUERY
{
	uint8 count;
	struct tagFlow
	{
		uint32 id;
		uint8  flow;
	} user[1];
} GSRV_MODIFY_USER_FLOW_QUERY,*LPGSRV_MODIFY_USER_FLOW_QUERY;

//�޸ķ�������Ӧ��
typedef struct tagGSRV_MODIFY_USER_FLOW_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserFlow
	{
		uint32 id;
		uint8  status;
		uint8  flow;
	} user[1];
} GSRV_MODIFY_USER_FLOW_ACK,*LPGSRV_MODIFY_USER_FLOW_ACK;


//��ȡ�����û�ȫ����ϸ��Ϣ����
typedef struct tagGSRV_GET_ONLINE_FULLINFO_QUERY
{
	uint8 count;
	uint32 userid[1];
} GSRV_GET_ONLINE_FULLINFO_QUERY,*LPGSRV_GET_ONLINE_FULLINFO_QUERY;

//��ȡ�����û�ȫ����ϸ��ϢӦ��
typedef struct tagGSRV_GET_ONLINE_FULLINFO_ACK
{
	uint8 result;
	uint8 count;
	struct UserInfo
	{ 
		uint32	id;
		uint8	status;
		uint8 	rule;
		uint16	version;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
		uint32	clientip;
		uint16	clientport;
		uint8	sexy;
		uint8 	flow;
		uint32	serverip;
		uint32  lanip;
		uint32  logintime;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
#ifdef IM_USE_CACHE
		uint8 	flag;					 //��¼��ʽ
		uint32  teamID;				 //�ͷ�ģʽ���û��Ӵ�����teamID
		
#endif
	} user[1];
 
} GSRV_GET_ONLINE_FULLINFO_ACK,*LPGSRV_GET_ONLINE_FULLINFO_ACK;



//��ȡ�����û�ȫ�ּ����Ϣ
typedef struct tagGSRV_GET_ONLINE_MONITOR_ACK
{
	uint8 result;
	uint8 count;
	struct UserInfo
	{ 
		uint32	id;
		uint8	status;
		char	username[MAX_USERNAME_LEN+1];				
		bool    monitored;	// �ͷ��Ƿ񱻼��
		uint32  chattingBuyerID;		// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
		uint32  directorID;	// ����ͷ�����أ���¼�����������id
	} user[1];
 
} GSRV_GET_ONLINE_MONITOR_ACK,*LPGSRV_GET_ONLINE_MONITOR_ACK;


//���/�޸�ȫ���û�����
typedef struct tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 };
	uint8	action;
	uint8	count;
	struct UserInfo
	{ 
		uint32 id;
		uint8  status;
		uint8	rule;
		char	username[MAX_USERNAME_LEN+1];
		char	nickname[MAX_NICKNAME_LEN+1];
		uint32	clientip;
		uint16	clientport;
		uint8	sexy;
		uint8 	flow;
		uint32	serverip;
		uint32	lanip;	//add by lwl ver:0101
		uint32  logintime;
//		uint16	lanport;
		char	sessionkey[MAX_SESSIONKEY_LEN];
		char	p2psessionkey[MAX_SESSIONKEY_LEN];
#ifdef IM_USE_CACHE		
		uint8 	flag;					 //��¼��ʽ
		uint32  teamID;				 //�ͷ�ģʽ���û��Ӵ�����teamID
#endif
	} user[1];
	tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_FULLUSERINFO_QUERY));
	}
}GSRV_ADDMODIFY_FULLUSERINFO_QUERY,*LPGSRV_ADDMODIFY_FULLUSERINFO_QUERY;

//���/�޸�ȫ���û�����Ӧ��
typedef struct tagGSRV_ADDMODIFY_FULLUSERINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_FULLUSERINFO_ACK,*LPGSRV_ADDMODIFY_FULLUSERINFO_ACK;

//ɾ���û�ȫ����������
typedef struct tagGSRV_DEL_ONLINE_USER_QUERY
{
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_USER_QUERY,*LPGSRV_DEL_ONLINE_USER_QUERY;

//ɾ���û�ȫ������Ӧ��
typedef struct tagGSRV_DEL_ONLINE_USER_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_USER_ACK,*LPGSRV_DEL_ONLINE_USER_ACK;



//��ȡ���߿ͷ�ȫ����ϸ��Ϣ����
typedef struct tagGSRV_GET_ONLINE_CSINFO_QUERY
{
	uint8 count;
	uint32 userid[1];
} GSRV_GET_ONLINE_CSINFO_QUERY,*LPGSRV_GET_ONLINE_CSINFO_QUERY;


typedef struct tagCSinfo
{
	uint32 m_nID;				//��ǰclientservice��ID��
	char m_Csname[MAX_USERNAME_LEN]; 	
	int m_State;
	int m_MaxRecConsult;		//�����ѯ�Ӵ���		
	int m_CurReception; //��ǰ�Ӵ���
	int m_CurConsult;		//��ǰ��ѯ��	
	int m_Permission;	//��ǰ�û�Ȩ��
	uint32 m_nChattingBuyerID;		// �ͷ���ǰ������ϵ����ң����ڼ�ع����У���������ϵ����Ҹ�֪���ܣ�
	uint32 m_nDirectorID;	// ����ͷ�����أ���¼�����������id,���û�б���أ�д��-1 
	vector<uint32> m_Customerid;//��ͷ���ѯ���û�[�û�id-----�û���Ϣ]
}CSinfo_t;


//��ȡ���߿ͷ�ȫ����ϸ��ϢӦ��
typedef struct tagGSRV_GET_ONLINE_CSINFO_ACK
{
	uint8 result;
	uint8 count;
	CSinfo_t CSinfo[1];
 
} GSRV_GET_ONLINE_CSINFO_ACK,*LPGSRV_GET_ONLINE_CSINFO_ACK;

//���/�޸�ȫ�ֿͷ�����
typedef struct tagGSRV_ADDMODIFY_CSINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 };
	uint8	action;
	uint8	count;
	CSinfo_t 	csinfo[1];
	
	tagGSRV_ADDMODIFY_CSINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_CSINFO_QUERY));
	}
}GSRV_ADDMODIFY_CSINFO_QUERY,*LPGSRV_ADDMODIFY_CSINFO_QUERY;

//���/�޸�ȫ���û�����Ӧ��
typedef struct tagGSRV_ADDMODIFY_CSINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_CSINFO_ACK,*LPGSRV_ADDMODIFY_CSINFO_ACK;

//ɾ���û�ȫ����������
typedef struct tagGSRV_DEL_ONLINE_CSINFO_QUERY
{
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSINFO_QUERY,*LPGSRV_DEL_ONLINE_CSINFO_QUERY;

//ɾ���û�ȫ������Ӧ��
typedef struct tagGSRV_DEL_ONLINE_CSINFO_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSINFO_ACK,*LPGSRV_DEL_ONLINE_CSINFO_ACK;



//��ȡ���߿ͷ��Ŷ�ȫ����ϸ��Ϣ����
typedef struct tagGSRV_GET_ONLINE_CSQINFO_QUERY
{
	uint8 count;
	uint32 teamID;
} GSRV_GET_ONLINE_CSQINFO_QUERY,*LPGSRV_GET_ONLINE_CSQINFO_QUERY;

typedef struct CSQList
{
	uint32 userid;		//�û�ID
	uint32 requestTime;	//����ʱ��
	uint32 Level;		//�û��ȼ�
	uint32 teamID;		//�û�������
}CSQList_t;


//��ȡ���߿ͷ��Ŷ�ȫ����ϸ��ϢӦ��
typedef struct tagGSRV_GET_ONLINE_CSQINFO_ACK
{
	uint8 result;
	uint8 count;
	struct CSQinfo
	{ 
		uint32 teamID;				//��ǰclientservice��ID��
		vector<CSQList_t> m_Customerlist;//��ͷ���ѯ���û�[�û�id-----�û���Ϣ]
	} CSQInfo[1];
 
} GSRV_GET_ONLINE_CSQINFO_ACK,*LPGSRV_GET_ONLINE_CSQINFO_ACK;

//��ȡ���߿ͷ��Ŷ�ȫ����ϸ��ϢӦ��
typedef struct tagGSRV_GET_ONLINE_CSQLEN_ACK
{
	uint8 result;
	uint8 count;
	struct CSQinfo
	{ 
		uint32 teamId;				//��ǰteamID
		uint32 queueLength;			//��ǰteamID���Ŷ�����
	} CSQInfo[1];
 
} GSRV_GET_ONLINE_CSQLEN_ACK,*LPGSRV_GET_ONLINE_CSQLEN_ACK;


//���/�޸�ȫ�ֿͷ��Ŷ�����
typedef struct tagGSRV_ADDMODIFY_CSQINFO_QUERY
{
	enum action { ACTION_ADD = 0,ACTION_MODIFY = 1 ,ACTION_DEL = 2};
	uint8	action;
	uint8	count;	
	struct CSinfo
	{		
		uint32 teamID;				//��ǰ�ͷ������
		uint32 customerID;			//��ǰ�Ŷ��û�
		uint32 customerLevel;		//��ǰ�Ŷ��û��ȼ�
		uint32 customerReqTime;		//��ǰ�Ŷ��û���¼ʱ��
	} csInfo[1];
	
	tagGSRV_ADDMODIFY_CSQINFO_QUERY()
	{
		memset(this,0,sizeof(tagGSRV_ADDMODIFY_CSINFO_QUERY));
	}
}GSRV_ADDMODIFY_CSQINFO_QUERY,*LPGSRV_ADDMODIFY_CSQINFO_QUERY;

//���/�޸�ȫ�ֿͷ��Ŷ�����Ӧ��
typedef struct tagGSRV_ADDMODIFY_CSQINFO_ACK
{
	uint8 result;
	uint8 count;
	uint32 user[1];
}GSRV_ADDMODIFY_CSQINFO_ACK,*LPGSRV_ADDMODIFY_CSQINFO_ACK;

//ɾ��ȫ�ֿͷ��Ŷ���������
typedef struct tagGSRV_DEL_ONLINE_CSQINFO_QUERY
{
	uint8	count;
	uint32	teamID[1];
}GSRV_DEL_ONLINE_CSQINFO_QUERY,*LPGSRV_DEL_ONLINE_CSQINFO_QUERY;

//ɾ��ȫ�ֿͷ��Ŷ�����Ӧ��
typedef struct tagGSRV_DEL_ONLINE_CSQINFO_ACK
{
	uint8	result;
	uint8	count;
	uint32	userid[1];
}GSRV_DEL_ONLINE_CSQINFO_ACK,*LPGSRV_DEL_ONLINE_CSQINFO_ACK;

//֪ͨҵ��������û����ص�¼
typedef struct tagGSRV_NOTIFY_RELOGIN
{
	uint32 userid[1];
}GSRV_NOTIFY_RELOGIN,*LPGSRV_NOTIFY_RELOGIN;

//-----------����ȫ�����߲���---------------------------------

//�̶�����
#define FAMILYINFO_QUERY_TYPE_PERM PERM_FAMILY
//��ʱ����
#define FAMILYINFO_QUERY_TYPE_TEMP TEMP_FAMILY

//��ѯ���߼���ȫ������
typedef struct tagGSRV_FAMILYINFO_QUERY
{
	uint8 count;
	uint8 type;
	uint32 fid[1];
}GSRV_FAMILYINFO_QUERY, * LPGSRV_FAMILYINFO_QUERY;

//��ѯ���߼���ȫ������Ӧ��
typedef struct tagGSRV_FAMILYINFO_QUERY_ACK
{
	uint8 ret;
	uint8 count;
	struct tagFamilyInfo
	{
		uint32 fid;
		uint32 srvip;
	}FamilyInfo[1];
}GSRV_FAMILYINFO_QUERY_ACK, * LPGSRV_FAMILYINFO_QUERY_ACK;

//���ȫ�ּ�������
typedef struct tagGSRV_FAMILYINFO_ADD_QUERY
{
	uint8  count;
	uint8  type;
	struct tagFamilyInfo
	{
		uint32 FamilyID;
		uint32 ServerIp;
	} FamilyInfo[1];

}GSRV_FAMILYINFO_ADD_QUERY,*LPGSRV_FAMILYINFO_ADD_QUERY;

//���ȫ�ּ�������Ӧ��
typedef struct tagGSRV_FAMILYINFO_ADD_ACK
{
	uint8	result;
	uint8	count;
	uint32	FamilyID[1];

}GSRV_FAMILYINFO_ADD_ACK,*LPGSRV_FAMILYINFO_ADD_ACK;


//ɾ��ȫ�ּ�������
typedef struct tagGSRV_FAMILYINFO_DEL_QUERY
{
	uint8  count;
	uint8  type;
	uint32 FamilyID[1];
}GSRV_FAMILYINFO_DEL_QUERY,*LPGSRV_FAMILYINFO_DEL_QUERY;

//ɾ��ȫ�ּ�������Ӧ��
typedef struct tagGSRV_FAMILYINFO_DEL_ACK
{
	uint8	result;
	uint8	count;
	uint32	FamilyID[1];

}GSRV_FAMILYINFO_DEL_ACK,*LPGSRV_FAMILYINFO_DEL_ACK;

//------------------��Ⱥ����ҵ��----------------------------------------------

//�����û�/���嶨λ��Ϣ֪ͨ
typedef struct tagCS_UPDATELOCATION_NOTIFY
{
	uint8       emDestType;
	uint32      destID;
}CS_UPDATELOCATION_NOTIFY, * LPCS_UPDATELOCATION_NOTIFY;


/***
�������������̬����ҵ��Э�鶨��
***/
//������ע��
//#define CMD_SERVER_REGISTER             0x8001
typedef struct tagSERVER_REGISTER
{
	uint16 maxCount;
	uint32 dnsIP;
	uint16 udpPort;
	uint16 tcpPort;
	uint8  flags; //0x00 ��ͨ������ 0x01 vip������
	enum {FLAGS_NORMAL = 0x00,FLAGS_VIP = 0x01};
	uint8  srvType; //0x01 ҵ������� 0x02 ��ת������
	enum {TYPE_BUSI = 0x01,TYPE_TRANS = 0x02};
}SERVER_REGISTER, * LPSERVER_REGISTER;

//#define CMD_SERVER_REGISTER_ACK         0x8002
typedef struct tagSERVER_REGISTER_ACK
{
	uint8 ret;
}SERVER_REGISTER_ACK, * LPSERVER_REGISTER_ACK;


//������ע��
//#define CMD_SERVER_UNREGISTER           0x8003
typedef struct tagSERVER_UNREGISTER
{
	uint8 authData; //У������ = 0x00
}SERVER_UNREGISTER, * LPSERVER_UNREGISTER;



//������״̬����
//#define CMD_SERVER_STATUS_REPORT        0x8005
typedef struct tagSERVER_STATUS_REPORT
{
	uint16 linkCount;
	uint8  capValue;
}SERVER_STATUS_REPORT, * LPSERVER_STATUS_REPORT;

//������״̬�б�����
//#define CMD_SERVER_STATUSLIST_REQ       0x8007
typedef struct tagSERVER_STATUSLIST_REQ
{
	uint16 startPos;
	uint8  srvType;  // 0x01 ҵ������� 0x02 ��ת������

}SERVER_STATUSLIST_REQ, * LPSERVER_STATUSLIST_REQ;
//#define CMD_SERVER_STATUSLIST_REQ_ACK   0x8008
struct tagSrvStatus
{
	uint32 srvID;
	SERVER_REGISTER srvBaseInfo;
	SERVER_STATUS_REPORT srvDynamicInfo;
};
typedef struct tagSERVER_STATUSLIST_REQ_ACK
{
	uint8 ret;
	uint16 nextPos;
	uint8 retCount;
	tagSrvStatus SrvStatus[1];
}SERVER_STATUSLIST_REQ_ACK, * LPSERVER_STATUSLIST_REQ_ACK;


//IMҵ�������ͣ��֪ͨ
typedef struct tagSERVER_IMSERVER_POWEROFF_NOTIFY
{
	uint32	severIp;
}SERVER_IMSERVER_POWEROFF_NOTIFY,*LPSERVER_IMSERVER_POWEROFF_NOTIFY;
//IMҵ�������ͣ��֪ͨӦ��
typedef struct tagSERVER_IMSERVER_POWEROFF_ACK
{
	uint32	severIp;
}SERVER_IMSERVER_POWEROFF_ACK,*LPSERVER_IMSERVER_POWEROFF_ACK;




//������͸��ת���ͻ�������
//#define CMD_SERVER_CLARITY_TRANSMIT     0x8009
typedef struct tagSERVER_CLARITY_TRANSMIT
{
	char data[1]; //�ͻ����Զ����Э�����ݣ��ÿͻ����Լ���p2p session key ����
}SERVER_CLARITY_TRANSMIT, * LPSERVER_CLARITY_TRANSMIT;


typedef struct tagSERVER_RELOGIN_NOTIFY
{
	uint32 userid;
	uint8 chgpwd;
	uint32 userip;
	uint32 lanip;

} SERVER_RELOGIN_NOTIFY,*LPSERVER_RELOGIN_NOTIFY;

typedef struct tagSERVER_RELOGIN_ACK
{
	uint32 userid;

} SERVER_RELOGIN_ACK,*LPSERVER_RELOGIN_ACK;


//��ѯ�����û���teamID����
typedef struct tagGSRV_GET_USER_TEAMID_QUERY
{
	uint8	count;
	uint32	userid[1];
} GSRV_GET_USER_TEAMID_QUERY,*LPGSRV_GET_USER_TEAMID_QUERY;


//��ѯ�����û���teamIDӦ��
typedef struct tagGSRV_GET_USER_TEAMID_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
		uint32 csid;
	} UserStatus[1];
} GSRV_GET_USER_TEAMID_ACK ,*LPGSRV_GET_USER_TEAMID_ACK;

//�޸������û���teamID����
typedef struct tagGSRV_MODIFY_ONLINE_TEAMID_QUERY
{
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
		uint32 csid;
	} user[1];
} GSRV_MODIFY_ONLINE_TEAMID_QUERY,*LPGSRV_MODIFY_ONLINE_TEAMID_QUERY;

//�޸������û���teamIDӦ��
typedef struct tagGSRV_MODIFY_ONLINE_TEAMID_ACK
{
	uint8 result;
	uint8 count;
	struct tagUserTeamID
	{
		uint32 id;
		uint8  teamID;
	} user[1];
} GSRV_MODIFY_ONLINE_TEAMID_ACK,*LPGSRV_MODIFY_ONLINE_TEAMID_ACK;

// �ֲ��û���Ϣ
struct STRU_PART_USERINFO
{
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 �����ʼ�
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 ����
	char    face_url[MAX_HEADURL_LEN+1] ;
	uint8   sex          ;
	uint8   province	 ;//5 ʡ
	uint8   country		 ;//6 ����
	uint8   city	     ;//7 ����
	uint8   career       ;//9 ְҵ
	uint8   rule         ;//10 ��Ϊ���ѵ���֤��ʽ
	uint8	familyflag	 ;// ��Ϊ�����Ա��֤��ʽ    0������     1���ܾ�
	uint8   linkpop		 ;//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;//11 ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;//12 �ֻ�
	char    description[MAX_DESCRIPTION_LEN+1]; //13 ����˵��

	STRU_PART_USERINFO()
	{
		memset(this,0,sizeof(STRU_PART_USERINFO));
	}
};


//#pragma pack(pop)
#endif
