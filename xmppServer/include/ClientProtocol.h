/************************************************************************ 
 �ļ��������ͻ���Э�鶨�壬�����������ͻ���֮��Ľ���Э��
		   
 �����ˣ� zouyf, 2006-6-9
 
 �޸ļ�¼��
************************************************************************/
#pragma once

#include "protocol.h"
#include "PublicType.h"
#include <string>
#include <map>

#ifndef _MINI_CLIENT_
#pragma pack(push,1)
#endif



//Э�鶨�� ...

//Ӧ��ACK,���践�ؽ����Ӧ��ack
typedef struct  tagRESPONSE_ACK
{
	uint8 ret; // ����0
}RESPONSE_ACK, * LPRESPONSE_ACK;

//���еĴ���������Э��
typedef struct  tagERROR_RET_ACK
{
	uint8 ret;
	char error[40];
}ERROR_RET_ACK, * LPERROR_RET_ACK;


//IM �û���Ϣ
typedef struct XT_USER_INFO
{
	uint32  id           ;	// 0 �û�id
	char    username[MAX_USERNAME_LEN+1] ;	// 3 �û���
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 �����ʼ�
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 ����
	
	char	comment_name[MAX_COMMENTNAME_LEN+1];// �������ѱ�ע����

	char    face_url[MAX_HEADURL_LEN+1] ;
	uint8   sex          ;
	uint8   age			 ;//4 ����
	uint8   province	 ;//5 ʡ
	uint8   country		 ;//6 ����
	uint8   city	     ;//7 ����
	uint8   career       ;//9 ְҵ

	uint8   rule         ;//10 ��Ϊ���ѵ���֤��ʽ
	uint8	familyflag	 ;// ��Ϊ�����Ա��֤��ʽ    0������     1���ܾ�
	uint8   linkpop		 ;//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;//11 ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;//12 �ֻ�
	uint32  gold_money   ;//�����Ŀ
	uint32  online_time  ;//����ʱ��

	char    description[MAX_DESCRIPTION_LEN+1]; //13 ����˵��
	char    user_sign[MAX_USERSIGN_LEN+1]   ;//����ǩ��
	char	avatoruri[MAX_AVATOR_LEN + 1];	//�����ַ
	uint8   mobilestatus				;	//�û��ֻ�״̬ 0δ�󶨣�1�Ѱ� 2�Ѱ󶨲��Ҷ�����ҵ��


	XT_USER_INFO()
	{
		memset(this,0,sizeof(XT_USER_INFO));
		sex	 = 3;
	}

} XT_USER_INFO, * LPXT_USER_INFO;//IMͨѶЭ��Э�������ֶ���,�ṹ����

//Ⱥ��Ա�б���Ϣ

typedef struct FAMILYMEMBER_ACKINFO
{
	uint32		id;
	uint8       prop;
	uint8       status;

	FAMILYMEMBER_ACKINFO()
	{
		memset(this,0,sizeof(FAMILYMEMBER_ACKINFO));
	}

}FAMILYMEMBER_ACKINFO, *LPFAMILYMEMBER_ACKINFO;

//Ⱥ��Ϣ
typedef struct XT_CLUSTER_INFO
{
	uint32    nFamilyID;
	uint8     nFamilyProp;							//Ⱥ���� FIRM_CLUSTER|TEMP_FAMILY
	uint8     nFamilyType;							//�������� FAMILY_COMMON ��ͨ���壬FAMILY_VIP VIP����
	uint16    nFamilyClass;							//����
	uint32    nCreatorID;

	uint8	  nFamilyFaceID;							//���ID
	char      szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	//������ӵ�ַ

	uint8     nJoinFlag;							//������� FAMILY_JOIN_ANY|FAMILY_JOIN_VALI|FAMILY_JOIN_UNALLOW
	uint8     nViewFlag;							//ҳ�����Ȩ��	FAMILY_VIEW_MEM|FAMILY_VIEW_REG|FAMILY_VIEW_ALL
	char      szFamilyName[MAX_FAMILYNAME_LEN + 1];
	char      szFamilyDesc[MAX_FAMILYDESC_LEN + 1];
	char      szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];
	uint16    nMemberLimit;	
	char      szFamilySign[MAX_FAMILYNAME_LEN + 1];//Ⱥǩ��
	char      szFamilyNotice[MAX_FAMILYNAME_LEN + 1];//Ⱥ����

	XT_CLUSTER_INFO()
	{
		memset(this,0,sizeof(XT_CLUSTER_INFO));
	}

} XT_CLUSTER_INFO, * LPXT_CLUSTER_INFO;


//IM �û���Ϣ
typedef struct XT_REGIST_REQ
{
	char	key[16]      ;//�����Կ
	uint8   regfrom	     ;//ע����Դ
	char    username[MAX_USERNAME_LEN + 1] ;//3 �û���
	uint8   sex          ;
	char    nickname[MAX_NICKNAME_LEN + 1] ;
	char    password[MAX_PASSWORD_LEN + 1] ;//2 ����
	char    email[MAX_EMAIL_LEN + 1]	   ;//1 �����ʼ�
	XT_REGIST_REQ()
	{
		memset(this,0,sizeof(XT_REGIST_REQ));
	}

} XT_REGIST_REQ, * LPXT_REGIST_REQ;

//ע���Ӧ
typedef struct XT_REGIST_ACK
{
	uint8    ret		  ;//0 ����ֵ(IM_FAIL, IM_SUCCES)
	uint32   id           ;//1 ϵͳ�����user id
	char     error[40]    ;//2 ������Ϣ,����������

	XT_REGIST_ACK()
	{
		memset(this,0,sizeof(XT_REGIST_ACK));
	}

} XT_REGIST_ACK, * LPXT_REGIST_ACK;

//��¼����
typedef struct XT_LOGIN_REQ
{
	uint8     login_flag    ; //78��λΪ��½��ʽ��6λ�����һ����
	// ռlogin_flag�����λ
	enum{
		LOGINBYID = 0,
		LOGINBYNAME = 1,
		LOGINBYMOBILENO = 2,
	};

	// ռlogin_flag����������Ϊ
	enum {
		SELLER = 0,
		BUYER = 1,
	};

	// ռlogin_flag������λ
	enum{
		MOBILE=0, 		// ���ֻ��˵�¼
	};

	union ULoginKey
	{
		uint32    id            ;//0 �û�id
		char      username[MAX_USERNAME_LEN + 1]  ;
		char      mobileno[MAX_PHONE_LEN + 1];
	}uLoginKey;

	char      random_key[16];
	char      password[24]  ;//1 ��¼����MD5(MD5(plain)),�������ĵ�2�ؼ����ټ���һ���մ�
	uint8     status        ;//2 ��¼״̬(�뿪,����..)
	uint32    lanip         ;//�ͻ��˾�����ip
	uint16    lanport       ;//�ͻ��˾�����port
	char      sessionID[128];//web session id
    char      mobilephoneno[MAX_PHONE_LEN + 1];
    char      email[MAX_EMAIL_LEN+1];//emailaddress
	XT_LOGIN_REQ()
	{
		memset(this,0,sizeof(XT_LOGIN_REQ));
	}

} XT_LOGIN_REQ, * LPXT_LOGIN_REQ;

//��¼��Ӧ
typedef struct XT_LOGIN_ACK
{
	uint8	  ret		       ;//0 ����ֵ

	uint8     session_key[16]  ;//1 Session Key
	//  ���û������MD5 Digest��MD5 Digest
	//  ��Ϊ��Կ��KEY��������һ��4Bytes������ִ���
	//  �õ�һ��16bytes�ļ��ܺ��ִ������ִ�����Ϊ��
	//  ��ͨѶ��Session Key��16bytes�����ͻ��˽���
	//  �ݽ��ܺ�����Ƿ�7�������ж��Ƿ���ȷ�Ľ��ܡ�
	uint32    user_id;
	uint32    merchant_id;
	uint32	  publicip;		//����ip
	uint8     version_flag     ;//0,���°汾��1��Ҫ���£������Լ���ʹ�ã�2�����������°汾
	char      version_url[MAX_VERSION_URL + 1] ;//�汾����URL
	char      update_config[MAX_UPDATECONFIG_URL + 1];   
	char	  update_info[MAX_UPDATE_INFO + 1] ;//������Ϣ
	char      sessionID[128];//web session id	
	uint8	  needChangeAccount;
	char	  szGUID[MAX_GUID_LEN + 1];	// ��ӪGUID(MAX_GUID_LENGTH = 40��
	char	  username[MAX_USERNAME_LEN+1] ;	// 3 �û���
	uint32	  login_time;
	XT_LOGIN_ACK()
	{
		memset(this,0,sizeof(XT_LOGIN_ACK));
	}
} XT_LOGIN_ACK, * LPXT_LOGIN_ACK;

typedef struct XT_LOGIN_ERROR
{
	uint8 ret;
	uint8 error[MAX_ERROR_BUF];	
	
	XT_LOGIN_ERROR()
	{
		memset(this,0,sizeof(XT_LOGIN_ERROR));
	}
}XT_LOGIN_ERROR, * LPXT_LOGIN_ERROR;

typedef struct XT_LOGOUT
{
	uint32		id;
	uint8 authData; //У������ = 0x00

} XT_LOGOUT;

//����Ŀ¼��Ϣ����
typedef struct XT_DIRECTORY_REQ
{
	uint32	  id        ;//0 �û�id
	uint32	  local_ip  ;//1 ���� ip ��ַ
	uint16    local_port;//2 ���� �˿�

	XT_DIRECTORY_REQ()
	{
		memset(this,0,sizeof(XT_DIRECTORY_REQ));
	}

} XT_DIRECTORY_REQ, * LPXT_DIRECTORY_REQ;

//����Ŀ¼��Ϣ��Ӧ
typedef struct XT_DIRECTORY_ACK
{
	uint8		ret;
	char		error[40] ;
	
	XT_DIRECTORY_ACK()
	{
		memset(this,0,sizeof(XT_DIRECTORY_ACK));
	}

} XT_DIRECTORY_ACK, * LPXT_DIRECTORY_ACK;

//---------------------------------���������Ϲ���----------------------

//��ȡ���ѣ��Լ����������ϵ�������ṹ
typedef struct XT_GET_FRIEND_BASICINFO_REQ
{
	uint8	type;
	uint16	count		;
	uint32	FriendID[1]	;

	enum { TYPE_FRIEND,TYPE_OTHER};

}XT_GET_FRIEND_BASICINFO_REQ,*LPXT_GET_FRIEND_BASICINFO_REQ;

//��ȡ���ѣ��Լ����������ϵ�Ӧ����ṹ
typedef struct XT_GET_FRIEND_BASICINFO_ACK
{
	uint8  ret;

}XT_GET_FRIEND_BASICINFO_ACK,*LPXT_GET_FRIEND_BASICINFO_ACK;


//�������Ի�ȡ���ѣ��Լ����������ϵ�֪ͨ
typedef struct XT_GET_FRIEND_BASICINFO_NOTIFY
{
	uint16  count;
	struct tagFriendInfo
	{
		uint32  nID;
		char	szHeadUrl[MAX_HEADURL_LEN+1];
		uint8	nSexy;
		char	szNickName[MAX_NICKNAME_LEN+1];
		char	szUserName[MAX_USERNAME_LEN+1];
		char	szCommentName[MAX_COMMENTNAME_LEN+1];// �������ѱ�ע����
		uint8	LoveFlag;							 // ��ע���
		char	szUserSign[MAX_USERSIGN_LEN + 1];// ����ǩ��
		uint8	mobileStatus;					//�ֻ���״̬ 0δ�󶨣�1�Ѱ� 2�Ѱ󶨲��Ҷ�����ҵ��
		char	szGuid[MAX_GUID_LEN];			//guid

	} FriendInfo[1];
	XT_GET_FRIEND_BASICINFO_NOTIFY()
	{
		memset(this,0,sizeof(XT_GET_FRIEND_BASICINFO_NOTIFY));
	}
}XT_GET_FRIEND_BASICINFO_NOTIFY,*LPXT_GET_FRIEND_BASICINFO_NOTIFY;

typedef struct XT_USERFAMILYFLAG_GET_REQ
{
	uint8 count;
	uint32 userID[1];
}XT_USERFAMILYFLAG_GET_REQ, *LPXT_USERFAMILYFLAG_GET_REQ;

typedef struct XT_USERFAMILYFLAG_GET_ACK
{
	uint8 ret;
	uint8 count;
	struct tagUserFamilyFlag
	{
		uint32 uID;
		uint8  flag;
	} JoinFlag[1];
	XT_USERFAMILYFLAG_GET_ACK()
	{
		memset(this, 0, sizeof(XT_USERFAMILYFLAG_GET_ACK));
	}
}XT_USERFAMILYFLAG_GET_ACK, *LPXT_USERFAMILYFLAG_GET_ACK;

//�쿴�û���������
typedef struct XT_USERINFO_GET_REQ
{
	//uint32	uid;
	uint32  fid;

} XT_USERINFO_GET_REQ, *LPXT_USERINFO_GET_REQ;

//�쿴�û�������Ӧ

//�ͻ���ʹ��
typedef struct XT_USERINFO_GET_ACK : public XT_USER_INFO
{
	uint8   ret;	

	XT_USERINFO_GET_ACK()
	{
		memset(this,0,sizeof(XT_USERINFO_GET_ACK));
	}

} XT_USERINFO_GET_ACK, *LPXT_USERINFO_GET_ACK;

//������ʹ��
typedef struct XT_SERVER_USERINFO_GET_ACK
{
	uint8   ret;	
	uint32  id           ;					// �û�id
	char    username[MAX_USERNAME_LEN+1] ;	// �û���
	char    nickname[MAX_NICKNAME_LEN+1] ;	// �س�
	char    email[MAX_EMAIL_LEN+1]	  ;		// �����ʼ�
	char    birthday[MAX_BIRTHDAY_LEN+1] ;	// ��������
	uint8   sex          ;					// �Ա�
	uint8   province	 ;					// ʡ
	uint8   country		 ;					// ����
	uint8   city	     ;					// ����
	char    face_url[MAX_HEADURL_LEN+1] ;	// ͷ��URL
	uint8   career       ;					// ְҵ
	uint8   rule         ;					// ��Ϊ���ѵ���֤��ʽ
	uint8   familyflag	 ;					// ��Ϊ�����Ա��֤��ʽ
	uint32  gold_money   ;					// �����Ŀ
	uint32  online_time  ;					// ����ʱ��
	uint8   linkpop		 ;					// ��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;	// ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;		// �ֻ�
	char    description[MAX_DESCRIPTION_LEN+1]; //����˵��
	char    usersign[MAX_USERSIGN_LEN + 1];	//����ǩ��
	char	avatoruri[MAX_AVATOR_LEN + 1];	//�����ַ
	uint8   mobilestatus				;	//�û��ֻ�״̬ 0δ�󶨣�1�Ѱ� 2�Ѱ󶨲��Ҷ�����ҵ��
	uint32  integral					;	//�û�����

	XT_SERVER_USERINFO_GET_ACK()
	{
		memset(this,0,sizeof(XT_SERVER_USERINFO_GET_ACK));
	}

} XT_SERVER_USERINFO_GET_ACK, *LPXT_SERVER_USERINFO_GET_ACK;

struct XT_MERCHANT_INFO_GET_ACK
{
	int32_t		id{0};
	std::string	uuid;
	std::string	username;
	std::string	password;
	int32_t		type{0};
	int32_t		state{0};
	std::string	email;
	std::string	mobile;
	std::string	truename;
	int32_t		merchant_id{0};
	std::string	merchant_code;
	int32_t		higherup_id{0};
	int32_t		role_id{0};
	int32_t		department_id{0};
	std::string	office_address;
	std::string	job_number;
	std::string	entry_time;
	std::string	id_no;
	std::string	create_time;
	std::string	update_time;
	std::string	job;
	std::string	random_key;
};

//�û������޸�����
typedef XT_USER_INFO XT_USERINFO_MOD_REQ;

//�û������޸���Ӧ
typedef struct XT_USERINFO_MOD_ACK
{
	uint8	ret;
	char	error[40];

	XT_USERINFO_MOD_ACK()
	{	
		memset(this,0,sizeof(XT_USERINFO_MOD_ACK));
	}
	
} XT_USERINFO_MOD_ACK, *LPXT_USERINFO_MOD_ACK;


//�û�״̬��������
typedef struct XT_STATUS_MOD_REQ
{
	uint32  id;
	uint8   status;

} XT_STATUS_MOD_REQ, * LPXT_STATUS_MOD_REQ;

//��������Ӧ�û�״̬����
typedef struct XT_STATUS_MOD_ACK
{
	uint8  ret;
	uint8  status;

} XT_STATUS_MOD_ACK, *LPXT_STATUS_MOD_ACK;


#define FLS_TOTAL     4 //��ȡȫ������
#define FLS_ONLINE	  5 //��ȡ���ߺ���


//��ȡ�����б�����
typedef struct XT_FRIENDLIST_REQ
{
	uint32      merchant_id;
	uint32		id			;
	uint16		start_pos	;

	XT_FRIENDLIST_REQ()
	{
		memset(this,0,sizeof(XT_FRIENDLIST_REQ));
	}

} XT_FRIENDLIST_REQ, * LPXT_RIENDLIST_REQ;


//��ȡ�����б�Ӧ��
typedef struct XT_FRIENDLIST_ACK
{
	uint8		result   ;
	uint16		next_pos ;
	uint8		count    ;

	struct tagFriendInfo
	{
		uint32	friendId ;
		uint8	groupId  ;
		uint8	status   ;
	} FriendInfo[1];

} XT_FRIENDLIST_ACK, *LPXT_FRIENDLIST_ACK;

//�����б����,û�к���������Ҫ��������
#define FRIENDLIST_NEXTPOS_END 0xffff



//��ȡ����Ϣ
typedef struct XT_GROUPINFO_GET_REQ
{
	uint32	id;
	uint8	group_count;

	XT_GROUPINFO_GET_REQ()
	{
		memset(this,0,sizeof(XT_GROUPINFO_GET_REQ));
	}

} XT_GROUPINFO_GET_REQ, *LPXT_GROUPINFO_GET_REQ;

typedef struct XT_GROUPINFO_GET_ACK
{
	uint8	ret;
	uint8	group_count;
	struct tagGroupInfo
	{
		uint8   nGroupPid;
		uint8	nGroupID;
		char	szGroupName[MAX_GROUPNAME_LEN+1];
	}GroupInfo[1];
}XT_GROUPINFO_GET_ACK, *LPXT_GROUPINFO_GET_ACK;

//�޸�����Ϣ
#define GROUP_ADD			0x00
#define GROUP_MODIFY		0x01
#define GROUP_DEL			0x02

typedef struct XT_GROUPINFO_MOD_REQ
{
    uint32		uid		;
	uint8		action	;
	uint8		group_pid;
	uint8		group_id;
	char		name[MAX_GROUPNAME_LEN+1];

	XT_GROUPINFO_MOD_REQ()
	{
		memset(this,0,sizeof(XT_GROUPINFO_MOD_REQ));
	}

} XT_GROUPINFO_MOD_REQ, *LPXT_GROUPINFO_MOD_REQ;

typedef struct XT_GROUPINFO_MOD_ACK
{
	uint8		ret			;
	uint8		action		;
	uint8		group_pid;
	uint8		group_id	;
	char		name[MAX_GROUPNAME_LEN+1]	;
	char		error[40]	;

	XT_GROUPINFO_MOD_ACK()
	{
		memset(this,0,sizeof(XT_GROUPINFO_MOD_ACK));
	}

} XT_GROUPINFO_MOD_ACK, *LPXT_GROUPINFO_MOD_ACK;

//����û����顢�޸��û�����ɾ�������û�
typedef struct XT_FRIENDGROUP_MOD_REQ
{
	uint32   uid			;
	uint8	 action			;
	//action��ȡֵ����
	enum { ACTION_ADD = 0 ,	//���
		   ACTION_MODIFY,	//�޸�
		   ACTION_DEL,		//ɾ��
		   ACTION_BLACK};	//��Ϊ���������ӶԷ��Է������б�ɾ���Լ�
	
	uint8	 group_id	;
	uint32	 fid			;	

	XT_FRIENDGROUP_MOD_REQ()
	{
		memset(this,0,sizeof(XT_FRIENDGROUP_MOD_REQ));
	}

} XT_FRIENDGROUP_MOD_REQ, *LPXT_FRIENDGROUP_MOD_REQ;

typedef struct XT_FRIENDGROUP_MOD_ACK
{
	uint8		ret;
	uint8		action;
	uint8	    group_id;
	uint32		fid;
	uint8		flag;		

	XT_FRIENDGROUP_MOD_ACK()
	{
		memset(this,0,sizeof(XT_FRIENDGROUP_MOD_ACK));
	}

} XT_FRIENDGROUP_MOD_ACK,*LPXT_FRIENDGROUP_MOD_ACK;


/******���������͸��ͻ��˵�ϵͳ��Ϣ��ת����Ϣ,��Ҫ�����������ת����Ϣ*****/
//#define CMD_SERVERMSG_IND			0x60d
typedef struct XT_SERVER_TRANSMIT
{
	uint32     origin_id;
	enum { ORIGIN_ID_SERVER = 0 };

	uint16      msgtype;
	enum SRVMSGTYPE
	{
		TYPE_SERVER_CLOSE		= 0x000c,
		TYPE_SYS_MESSAGE		= 0x000d,
		TYPE_FRIEND_ADD			= 0x000e,
		TYPE_FRIEND_DEL			= 0x000f,
		TYPE_VERIFY_FRIEND		= 0x0607,
	};
} XT_SERVER_TRANSMIT, * LPXT_SERVER_TRANSMIT;


typedef struct _XT_SERVER_TRANSMIT_ACK
{
	uint32	fromId;
	uint32	toId;
} XT_SERVER_TRANSMIT_ACK,* LPXT_SERVER_TRANSMIT_ACK;

//ת���ľ���Э�鶨������(XT_MSG,XT_USERAUTH_REQ,XT_SERVERMSG,XT_CMEMBER_IND,XT_CLUSTER_AUTH_IND,
//XT_CLUSTER_DISMISS_IND)
//P2P������Ϣ
typedef struct XT_MSG
{
	uint8    msg_type     ;//������Ϣ����0��������Ϣ,1Ⱥ����Ϣ,10:����������Ϣ��11������Ⱥ����Ϣ
	uint32   cluster_id   ;//Ⱥ��ʱ����
	uint8    cluster_type ;//Ⱥ��ʱ����
	
	uint8    recv_flag    ;//0, ������p2p; 1 ������server
	uint32   from_id      ;
	char     from_nickname[MAX_NICKNAME_LEN+1];
	uint32   to_id        ;
	uint16   ver          ;

	uint32	 dest_ip      ;
	uint16   dest_port    ;
	uint32	 send_time    ;

	uint8	 face;
	uint8    charset      ;
	char     fontName[50] ;
	uint8	 fontSize     ;
	uint16	 fontStyle    ;
	uint32   fontColor    ;

	uint8    data_type	  ;//0��ʱ������Ϣ��1ͼƬ�ļ���������,
	                       //2 ͼƬ�ļ�Ӧ��3ͼƬ�ļ���ֹ ��4 ͼƬ�ļ�������ϣ�
	                       //5 ͼƬ����
						   //6~8 ȺͼƬ�ļ�
						   //9 ħ������

	enum DATA_TYPE
	{
		IM_MSG=0,
		SF_FILE_REQ=1,
		SF_FILE_ACK=2,
		SF_PACKET_REQ=3,
		SF_PACKET_ACK=4,
		SF_ABORT=5,
		WIN_SHAKE = 6,
		XT_SERVICE = 7, //��������
		SF_CFILE_REQ=8,
		MAGIC_FACE=9,
		USER_SIGN=10,
		FILE_REQ=11,
		MM_REQ=12,
		MM_REPLY=13,
		MM_CANCEL=14,
		MM_STOP=15,
		VEDIO_EQUIPMENT=16,
		XT_RECOMENT=17,
		OPEN_P2PDLG=18,
		SF_IMG_URL = 19,
		CUST_SER_TRAN_REQ = 20, // �ͷ�ת������
		AUTO_REPLY =21,
		MAX_TYPE=22,		
	};

	char 	uuid[MAX_UUID_LEN+1];		// ��Ϣ��Ψһ��ʾ
	
	char	 data[1315]    ;
	int      data_len	   ;

	XT_MSG()
	{
		memset(this,0,sizeof(XT_MSG));
	}

} XT_MSG, *LPXT_MSG;

typedef struct XT_TALK
{
	uint32   from_id;
	uint32   to_id;
	uint8    data_type;
	uint16   ver;
	uint32	 send_time;
	uint8    recv_flag;
	uint8	 fontSize;
	uint32   fontColor;
	uint16	 fontStyle;
	char     fontName[50];
	char	 data[1315];
	char     from_nickname[MAX_NICKNAME_LEN+1];
	time_t	 recv_time;
	uint8	 from_terminal;			//��ϢԴ�ն�
	uint8	 to_terminal;			//��ϢĿ���ն�
	char	 uuid[MAX_UUID_LEN+1];	//��Ϣ��Ψһ��ʾ

	enum MSG_TERMINAL
	{
		PC_TERMINAL = 1, 
		WEB_TERMINAL = 2,
		TOUCH_TERMINAL = 3
		
	};
	
	XT_TALK()
	{
		memset(this,0,sizeof(XT_TALK));
	}
	
}XT_TALK, *LPXT_TALK;

struct RECV_FILE_STRUCT
{
	char		file_name[255];
	uint32		file_size ;
	uint32		file_id   ;
	uint32		from_id   ;
	uint16		nNextPacketId;
	uint16		nPacketCount;
	FILE* 		fileObject;
	
	
	uint32   recv_size ;
//	unsigned int    recv_tick ;
	std::map<int,int> mapPacketRecv;

	RECV_FILE_STRUCT()
	{
		memset(file_name,0,sizeof(file_name));
		nNextPacketId = 0;
		nPacketCount = 0;
		fileObject = 0;
		file_size  = 0;
		file_id    = 0;
		recv_size  = 0;
		from_id    = 0;
		//recv_tick  = 0;
	}
};

struct IMAGE_FILE_STRUCT
{
	char    file_name[255]		;
	uint32  file_session_id     ;
	uint32  file_size           ;
	uint32  image_type          ;//0 image, 1 custom emotion,2 web emotion

	uint8 ret;
	uint32 file_id;;
	uint32 last_packet_id;;
	uint32 last_file_pointer;
	uint32 packet_id;
	uint32 file_pointer;

	IMAGE_FILE_STRUCT()
	{
		memset(this,0,sizeof(IMAGE_FILE_STRUCT));
	}
};


typedef struct _XT_PIC_DATA
{
	uint32		sid;
	uint32		fid;
	uint16		packetId;
	uint16		dataLen;
	char		data[CPIC_PACKET_LEN];
} XT_PIC_DATA, *LPXT_PIC_DATA;

typedef struct _XT_CPIC_DATA
{
	uint32		cid;
	uint32		sid;
	uint32		fid;
	uint16		packetId;
	uint16		dataLen;
	char		data[CPIC_PACKET_LEN];
} XT_CPIC_DATA,*LPXT_CPIC_DATA;


typedef struct _XT_UPLOAD_CPIC_ACK
{
	uint8		ret;		
	uint32		nCid;		// Ⱥid
	uint32		nFileId;	// �ļ�id
	uint16		nPacketId;	// �ļ�����
}XT_UPLOAD_CPIC_ACK,*LPXT_UPLOAD_CPIC_ACK;

// ����ͼƬӵ�����������ߵ�֪ͨ
typedef struct _XT_DOWNLOAD_CPIC_NOTIFY
{
	uint32	nSrcId;				// �ļ�ӵ����
	uint32	nFileId;			// �ļ�id
	uint32	nFileSize;			// �ļ���С
	uint16	nPacketSize;		// �ļ�����С
	char	szFileName[255];	// �ļ���
}XT_DOWNLOAD_CPIC_NOTIFY,*LPXT_DOWNLOAD_CPIC_NOTIFY;


typedef struct _XT_DOWNLOAD_CPIC_REQ
{
	uint32	nFileSid;		// ͼƬ�ļ�ӵ����id
	uint32  nFileId;		// ͼƬ�ļ�id
	uint16	nPacketId;		// ͼƬ����
	uint32	nCId;			// Ⱥid
}XT_DOWNLOAD_CPIC_REQ,*LPXT_DOWNLOAD_CPIC_REQ;

typedef struct _XT_DOWNLOAD_CPIC_ACK
{
	enum { RET_SUCCESS = 0, RET_WAIT, RET_FAILED } ;
	uint8		ret;
	uint32		nCId;		// Ⱥid
	uint32		nSrcId;		// ͼƬӵ����id
	uint32		nFileId;	// ͼƬid
	uint16		nPacketId;	// ͼƬ��id
	uint16		nDataLen;	// ͼƬ������
	char		data[CPIC_PACKET_LEN];
}XT_DOWNLOAD_CPIC_ACK,*LPXT_DOWNLOAD_CPIC_ACK;


typedef struct XT_SERVERMSG
{
	uint32	send_time;
	uint8   type;
	char	message[MAX_MSG_LEN+1];

	XT_SERVERMSG()
	{
		memset(this,0,sizeof(XT_SERVERMSG));
	}

} XT_SERVERMSG, *LPXT_SERVERMSG;

//������֪ͨ�û����˼Ӹ��û�Ϊ����
typedef struct tagXT_FRIENDADD_NOTIFY
{
	uint32	ThatOneID;
} XT_FRIENDADD_NOTIFY ,*LPXT_FRIENDADD_NOTIFY;

typedef struct tagXT_FRIENDADD_NOTIFY  tagXT_FRIENDDEL_NOTIFY;

typedef struct XT_CMEMBER_IND
{
	uint32     uid          ;
	uint8      action       ;// 1:���/���룻2��ɾ��/�˳�
	uint32     member_id    ;
	char       clustername[MAX_FAMILYNAME_LEN+1];
	char       nickname[MAX_NICKNAME_LEN+1] ;
	uint32     cluster_id   ;
	uint8      cluster_type ;

	XT_CMEMBER_IND()
	{
		memset(this,0,sizeof(XT_CMEMBER_IND));
	}

} XT_CMEMBER_IND, *LPXT_CMEMBER_IND;

//��Ӻ�����֤����
typedef struct XT_USERAUTH_REQ
{
	uint8		action		;//1 Byte, 0x00=����; 0x01=ͬ��; 0x02=�ܾ�
	enum { ACTION_REQUEST = 0, ACTION_AGREE,ACTION_DENIAL };

	uint32		from_id		;//���ͷ�id
	uint32		to_id		;//�Է�id
	uint8		groupId		;//���ѷ���id
	uint32		send_time	;
	char		info[50]	;//��֤��Ϣ

	XT_USERAUTH_REQ()
	{
		memset(this,0,sizeof(XT_USERAUTH_REQ));
	}

} XT_USERAUTH_REQ, *LPXT_USERAUTH_REQ;

typedef struct XT_USERAUTH_ACK
{
	uint8		ret		;
	uint8		action	;
	uint32		to_id	; //�Է�����

	XT_USERAUTH_ACK()
	{
		memset(this,0,sizeof(XT_USERAUTH_ACK));
	}

} XT_USERAUTH_ACK, *LPXT_USERAUTH_ACK;

//���������ͻ��˵�Ⱥ��֤��Ϣ֪ͨ
typedef struct XT_CLUSTER_AUTH_IND
{
	uint32       uid;
	uint8        action      ;// 0x00=����; 0x01=ͬ��; 0x02=�ܾ�
	uint32       cluster_id  ;
	uint8        cluster_type;
	uint32       manager_id  ;
	uint32       send_time   ;
	char         username[MAX_USERNAME_LEN+1] ;
	char         nickname[MAX_NICKNAME_LEN+1];
	char         info[100]   ;

	XT_CLUSTER_AUTH_IND()
	{
		memset(this,0,sizeof(XT_CLUSTER_AUTH_IND));
	}

} XT_CLUSTER_AUTH_IND, *LPXT_CLUSTER_AUTH_IND;

typedef struct XT_CLUSTER_AUTH_REQ
{
// 	char         username[20];
// 	char         nickname[32];
	uint32       nFamilyID  ;
	uint8        nFamilyProp;
	uint8        nAction    ;	// 0x00=����; 0x01=ͬ��; 0x02=�ܾ�
  	uint32       nRequestID ;	// ����ID(���������շ�ID)
	uint32       nManagerID  ;	//ͬ���ܾ�ʱ������ID�� ����ʱ��0
	uint32       send_time   ;

	char         info[100]   ;

	XT_CLUSTER_AUTH_REQ()
	{
		memset(this,0,sizeof(XT_CLUSTER_AUTH_REQ));
	}

} XT_CLUSTER_AUTH_REQ, *LPXT_CLUSTER_AUTH_REQ;

typedef struct XT_CLUSTER_AUTH_ACK
{
	uint8        ret          ;
	uint32       nFamilyID   ;
	uint8        nFamilyProp ;
	uint8        nAction      ;// 0x00=����; 0x01=ͬ��; 0x02=�ܾ�

	XT_CLUSTER_AUTH_ACK()
	{
		memset(this,0,sizeof(XT_CLUSTER_AUTH_ACK));
	}

} XT_CLUSTER_AUTH_ACK, *LPXT_CLUSTER_AUTH_ACK;

typedef struct XT_CLUSTER_DISMISS_IND
{
	uint32     destructor_id;
	uint32     cluster_id;
	uint8      cluster_type;

	XT_CLUSTER_DISMISS_IND()
	{
		memset(this,0,sizeof(XT_CLUSTER_DISMISS_IND));
	}

} XT_CLUSTER_DISMISS_IND, *LPXT_CLUSTER_DISMISS_IND;


//����������״̬����֪ͨ
typedef struct XT_STATUS_IND
{
 	uint32		fid;	//����id
	uint8		status;
	uint16		version;

	XT_STATUS_IND()
	{
		memset(this,0,sizeof(XT_STATUS_IND));
	}

} XT_STATUS_IND, * LPXT_STATUS_IND;

typedef struct XT_INFO_IND
{
 	uint32		fid;	//����id
 	
	XT_INFO_IND()
	{
		memset(this,0,sizeof(XT_INFO_IND));
	}

} XT_INFO_IND, * LPXT_INFO_IND;

//����������״̬����֪ͨӦ��
typedef struct XT_STATUS_IND_ACK
{
	uint8		result;	
	uint32		OtherID;
	uint32		MyID;

	XT_STATUS_IND_ACK()
	{
		memset(this,0,sizeof(XT_STATUS_IND_ACK));
	}

} XT_STATUS_IND_ACK, * LPXT_STATUS_IND_ACK;


//�ͻ���ͨ��������ת��������Ϣ��ͷ��
enum SERVER_P2PMSG_TYPE
{
	SERVER_P2PMSG_TYPE_MESSAGE = 0,	//������Ϣ
	SERVER_P2PMSG_TYPE_MYWORD  = 10,//����ǩ��
	SERVER_P2PMSG_TYPE_CUSTOM_EMOTION	//�Զ������
};
typedef struct XT_SERVER_P2PMSG_SEND
{
	uint32 Sender;
	uint32 Recver;
	uint8  type;


}XT_SERVER_P2PMSG_SEND,*LPXT_SERVER_P2PMSG_SEND;

//������ת��������ϢӦ��
typedef struct XT_SERVER_P2PMSG_ACK
{
	uint32 Sender;
	uint32 Receiver;
	uint8  type;

} XT_SERVER_P2PMSG_ACK,*LPXT_SERVER_P2PMSG_ACK;

typedef struct XT_WEB_BUSI_LIST_REQ
{
	uint32 id;//��ȡ�б��id��
	uint32 msgnum;//�������Ϣ������
}XT_WEB_BUSI_LIST_REQ,*LPXT_WEB_BUSI_LIST_REQ;

typedef struct XT_WEB_BUSI_LIST_ACK
{
	uint8 ret;
	uint32 id;//id
	uint32 msgnum;//�������Ϣ������
	uint32 actualnum;//������Ϣ��ʵ������
	struct BusiList
	{
		uint32 id;
		char nickname[MAX_NICKNAME_LEN+1];
		uint32 time;
		uint8 state;
		char headImgUrl[MAX_HEADURL_LEN+1];
		char usersign[MAX_USERSIGN_LEN+1];
	}busiList[1];
}XT_WEB_BUSI_LIST_ACK,*LPXT_WEB_BUSI_LIST_ACK;

typedef struct XT_XMPP_BUSI_LIST_REQ
{
	uint32 id;//��ȡ�б��id��
	uint32 msgnum;//�������Ϣ������
}XT_XMPP_BUSI_LIST_REQ,*LPXT_XMPP_BUSI_LIST_REQ;

typedef struct XT_XMPP_BUSI_LIST_ACK
{
	uint8 ret;
	uint32 id;//id
	uint32 msgnum;//�������Ϣ������
	uint32 actualnum;//������Ϣ��ʵ������
	struct BusiList
	{
		uint32 id;//�����̼�id
		uint32 merchantId;//�̼����˺�id
		char nickname[MAX_NICKNAME_LEN+1];
		uint32 time;
		uint8 state;
		char headImgUrl[MAX_HEADURL_LEN+1];
		char usersign[MAX_USERSIGN_LEN+1];		
		char storename[MAX_NICKNAME_LEN+1];
		uint32 subCount;
		uint32 subMerchants[20];
	}busiList[1];
}XT_XMPP_BUSI_LIST_ACK,*LPXT_XMPP_BUSI_LIST_ACK;

typedef struct XT_WEB_MORE_MSG_REQ
{
	uint32 sid;//�Լ���id
	uint32 did;//�Է���id
	uint32 msgtime;//����msgtime֮ǰ������
	uint32 msgnum;//�������Ϣ������
	uint32 msgid;
	char uuid[MAX_UUID_LEN + 1];
	uint32 isMerchant; //Merchant :1 or subMerchant :0
}XT_WEB_MORE_MSG_REQ,*LPXT_WEB_MORE_MSG_REQ;

typedef struct XT_WEB_MORE_MSG_ACK
{
	uint8 ret;
	uint32 sid;//�Լ���id
	uint32 did;//�Է���id
	uint32 msgtime;//����msgtime֮ǰ������
	uint32 msgnum;//�������Ϣ������
	uint32 returnnum;//ʵ�ʷ��ص�����
	uint8 over;//����������Ƿ��� 1 ���� 0 δ����
	uint32 actualnum;
	char uuid[MAX_UUID_LEN + 1];
	struct MsgInfo//��Ϣ����Ϣ
	{
		uint32	 id;
		uint32	 send_time;
		uint32   msgid;
		uint8	 fontSize;
		uint32	 fontColor;
		uint16	 fontStyle;
		char	 fontName[50];
		char	 nickName[MAX_NICKNAME_LEN+1];
		char	 data[1315];
	}msgInfo[1];
}XT_WEB_MORE_MSG_ACK,*LPXT_WEB_MORE_MSG_ACK;

//�񶯺��Ѵ�������
typedef struct XT_SHAKE_WIN_REQ
{
	uint32 Sender;
	uint32 Recver;
	
}XT_SHAKE_WIN_REQ,*LPXT_SHAKE_WIN_REQ;

//�񶯺��Ѵ���Ӧ��
typedef struct XT_SHAKE_WIN_ACK
{
	uint32 Sender;
	uint32 Recver;
	
}XT_SHAKE_WIN_ACK,*LPXT_SHAKE_WIN_ACK;


//�Է����յ�P2P������Ϣ��Ӧ
typedef struct XT_P2PMSG_ACK
{
	uint32	src_id;
	uint32  des_id;
	uint32  src_ip;
	uint16  src_port;
	uint16  ver;
	uint8   data_type ;

	XT_P2PMSG_ACK()
	{
		memset(this,0,sizeof(XT_P2PMSG_ACK));
	}

} XT_P2PMSG_ACK, *LPXT_P2PMSG_ACK;



typedef struct XT_OFFLINEMSG_REQ
{
	uint32		id;

	XT_OFFLINEMSG_REQ()
	{
		memset(this,0,sizeof(XT_OFFLINEMSG_REQ));
	}

} XT_OFFLINEMSG_REQ, *LPXT_OFFLINEMSG_REQ;

typedef struct XT_OFFLINEMSG_ACK
{
	uint8	nResult;

	XT_OFFLINEMSG_ACK()
	{
		memset(this,0,sizeof(XT_OFFLINEMSG_ACK));
	}

} XT_OFFLINEMSG_ACK, *LPXT_OFFLINEMSG_ACK;

//�����û�

#define	 XTREAM_SEARCH_ONLINE		0 //��˭����
#define  XTREAM_SEARCH_PRECISION	1 //��ȷ����

typedef struct XT_XTREAM_SEARCH_REQ
{
	uint16		startpos;
	uint8		search_type;
	uint8		count;//Ҫ�󷵻���Ŀ��0������
	uint32		id;	//��ID����
	char 		user_name[MAX_USERNAME_LEN];
	char		nick_name[MAX_NICKNAME_LEN+1];
	char		user_email[MAX_EMAIL_LEN];

	XT_XTREAM_SEARCH_REQ()
	{
		memset(this,0,sizeof(XT_XTREAM_SEARCH_REQ));
	}

} XT_XTREAM_SEARCH_REQ, *LPXT_XTREAM_SEARCH_REQ;


typedef struct XT_XTREAM_SEARCH_ACK
{
	uint8		result;
	uint8		count;
	uint16		nextpos;
	uint32		nTotalNum;

	struct tagFriendInfo
	{
		uint32	id;
		uint8	sexy;
		uint8	status;
		uint8	rule;
		char	UserName[MAX_USERNAME_LEN+1];
		char	NickName[MAX_NICKNAME_LEN+1];
	} FriendInfo[1];

} XT_XTREAM_SEARCH_ACK, *LPXT_XTREAM_SEARCH_ACK;

//��ȡ��������״̬��Ϣ����
typedef struct XT_STATUS_GET_REQ
{
	uint32		id;
	uint32		fid;

	XT_STATUS_GET_REQ()
	{
		memset(this,0,sizeof(XT_STATUS_GET_REQ));
	}

} XT_STATUS_GET_REQ, *LPXT_STATUS_GET_REQ;

//��ȡ��������״̬��ϢӦ��
typedef struct XT_STATUS_GET_ACK
{
	uint8		ret;
	uint32		id ;
	uint32		ip;
	uint16		port;
	uint32		lanip;
	uint16		lanport;
	uint8		status;
	uint16		ver;
	char		p2pkey[MAX_SESSIONKEY_LEN];


	XT_STATUS_GET_ACK()
	{
		memset(this,0,sizeof(XT_STATUS_GET_ACK));
	}

} XT_STATUS_GET_ACK, *LPXT_STATUS_GET_ACK;

//����Ⱥ
#define  CLUSTER_SEARCH_ID			0 //��ȷ����,��ID����
#define  CLUSTER_SEARCH_CLASS		1 //����������
#define  CLUSTER_SEARCH_NAME		2 //����������

//#define CMD_CLUSTER_SEARCH_REQ      0x909
typedef struct XT_CLUSTER_SEARCH_REQ
{
//	uint32		uid;
	uint16		start_pos;
	uint8		search_type;
	uint8		nCount;//Ҫ�󷵻���Ŀ��0������
	union TYPE_DATA
	{
		uint32		nFamilyID;
		uint16      nFamilyClass;//Ⱥ����
		char		szFamilyName[MAX_FAMILYNAME_LEN + 1];
	} type_data;
	
	XT_CLUSTER_SEARCH_REQ()
	{
		memset(this,0,sizeof(XT_CLUSTER_SEARCH_REQ));
	}

} XT_CLUSTER_SEARCH_REQ, * LPXT_CLUSTER_SEARCH_REQ;

// CLUSTER_SEARCH_RESULT, *LPCLUSTER_SEARCH_RESULT;

//#define CMD_CLUSTER_SEARCH_ACK      0x90a
typedef struct XT_CLUSTER_SEARCH_ACK
{
	uint8        ret                ;
	uint16       next_pos           ;
	uint8        nCount				;	

	struct tagFamilyFindRet
	{
		uint32       nFamilyID          ;
		uint32       nCreaterID			;
		uint8        nJoinFlag			;
		uint16       nFamilyClass		;
		char		 szFamilyName[MAX_FAMILYNAME_LEN + 1]		;
		char		 szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1]	;
		uint8        nMemCount;////����Ⱥ��Ա����	

		tagFamilyFindRet()
		{
			memset(this,0,sizeof(tagFamilyFindRet));
		}
	} FamilySearchData[1];

	XT_CLUSTER_SEARCH_ACK()
	{
		memset(this,0,sizeof(XT_CLUSTER_SEARCH_ACK));
	}
} XT_CLUSTER_SEARCH_ACK, *LPXT_CLUSTER_SEARCH_ACK;

typedef struct XT_CREATE_CLUSTER_REQ
{
// 	uint32		nCreaterID									;	
	uint8		nFamilyProp									;	//1���̶�Ⱥ��2����ʱȺ
	uint8		nJoinFlag									;	//��֤��FAMILY_JOIN_ANY ������� FAMILY_JOIN_VALIDATE ��֤ FAMILY_JOIN_UNALLOW �ܾ�
	uint16		nFamilyClass								;
	uint8		nFamilyFaceID										;
	char		szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1]	;	//������ӵ�ַ
	uint8		nViewFlag									;	//FAMILY_VIEW_MEM	�����Ա��˽�У�FAMILY_VIEW_REG	ע���û����빫����FAMILY_VIEW_ALL ���У�������

	char		szFamilyName[MAX_FAMILYNAME_LEN + 1]		;
	char		szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1]	;
	char		szFamilyDesc[MAX_FAMILYDESC_LEN + 1]		;

	XT_CREATE_CLUSTER_REQ()
	{
		memset(this,0,sizeof(XT_CREATE_CLUSTER_REQ));
	}

} XT_CREATE_CLUSTER_REQ, * LPXT_CREATE_CLUSTER_REQ;


typedef struct XT_CREATE_CLUSTER_ACK
{
	uint8    ret;
	uint32   nFamilyID;

	XT_CREATE_CLUSTER_ACK()
	{
		memset(this,0,sizeof(XT_CREATE_CLUSTER_ACK));
	}

} XT_CREATE_CLUSTER_ACK, *LPXT_CREATE_CLUSTER_ACK;


//#define CMD_GET_CINFO_REQ           0x907
//��ȡȺ��Ϣ
typedef struct XT_GET_CINFO_REQ
{
// 	uint32     id;
	uint32     nFamilyID;
	uint8      nFamilyProp;

	XT_GET_CINFO_REQ()
	{
		memset(this,0,sizeof(XT_GET_CINFO_REQ));
	}

} XT_GET_CINFO_REQ, *LPXT_GET_CINFO_REQ;

//#define CMD_GET_CINFO_ACK           0x908
typedef struct XT_GET_CINFO_ACK : public XT_CLUSTER_INFO
{
	uint8     ret;


	XT_GET_CINFO_ACK()
	{
		memset(this,0,sizeof(XT_GET_CINFO_ACK));
	}

} XT_GET_CINFO_ACK, *LPXT_GET_CINFO_ACK;


//����Ⱥ��Ա
typedef struct XT_SET_CMEMBERS_REQ
{
	uint32     nFamilyID			;
	uint8      nFamilyProp		    ;
	uint8      nAction		        ;
	uint8      nCount	            ;
	uint32	   nInviteID 			;//��Ҫ�����ĳ�Ա��������
	uint32     nMemID[1]		    ;

	XT_SET_CMEMBERS_REQ()
	{
		memset(this,0,sizeof(XT_SET_CMEMBERS_REQ));
	}

} XT_SET_CMEMBERS_REQ, *LPXT_SET_CMEMBERS_REQ;

typedef struct XT_SET_CMEMBERS_ACK
{
	uint8    ret			;
	uint32   nFamilyID		;
	uint8    nFamilyProp	;
	uint8    nAction	    ;
	uint32   nMemberID      ;
	uint32   nInviteID      ;//��Ա��������ID
	char     szError[MAX_ERROR_BUF]    ;

	XT_SET_CMEMBERS_ACK()
	{
		memset(this,0,sizeof(XT_SET_CMEMBERS_ACK));
	}

} XT_SET_CMEMBERS_ACK, *LPXT_SET_CMEMBERS_ACK;

//������롢�˳�Ⱥ
typedef struct XT_CLUSTER_APPLY_REQ
{
	uint32    nFamilyID							;
	uint8     nFamilyProp						; //1:�̶�Ⱥ  2����ʱȺ
	uint8     nAction							; //FAMILY_REQ_JOIN|FAMILY_REQ_QUIT
	uint32    nUserID							; //������ID

	XT_CLUSTER_APPLY_REQ()
	{
		memset(this,0,sizeof(XT_CLUSTER_APPLY_REQ));
	}

} XT_CLUSTER_APPLY_REQ, *LPXT_CLUSTER_APPLY_REQ;


typedef struct XT_CLUSTER_APPLY_ACK
{
	uint8    ret;
	uint32   nFamilyID;
	uint8    nFamilyProp;
	uint8    nAction;
	uint8    nFlag;// 00, �ɹ�; 01, ��Ҫ��֤; 02, �Է������κ��˼����Ⱥ

	XT_CLUSTER_APPLY_ACK()
	{
		memset(this,0,sizeof(XT_CLUSTER_APPLY_ACK));
	}

} XT_CLUSTER_APPLY_ACK, *LPXT_CLUSTER_APPLY_ACK;

typedef struct XT_CLUSTERMSG_ACK
{
	uint8   ret;
	uint32  cluster_id ;
	uint8   data_type  ;
	XT_CLUSTERMSG_ACK()
	{
		memset(this,0,sizeof(XT_CLUSTERMSG_ACK));
	}

} XT_CLUSTERMSG_ACK, *LPXT_CLUSTERMSG_ACK;

typedef struct XT_GET_CMEMBERS_REQ
{
// 	uint32		id				;
	uint16		start_pos		;
	uint32      nFamilyID		;
	uint8		nFamilyProp		;					//Ⱥ���� FIRM_CLUSTER|TEMP_FAMILY
	uint8		online_type		; //0:����,1���ߺ���
	uint16		req_count		;
	uint8		userstatus		;//�û���ǰ״̬
// 	uint8       user_property	; //�û���Ⱥ�еĵ�λ(��ͨ��Ա0,����Ա1,�峤2)

	XT_GET_CMEMBERS_REQ()
	{
		memset(this,0,sizeof(XT_GET_CMEMBERS_REQ));
	}

} XT_GET_CMEMBERS_REQ, *LPXT_GET_CMEMBERS_REQ;

typedef struct XT_GET_CMEMBERS_ACK
{
	uint8        ret                ;
	uint16       next_pos           ;
	uint32       nFamilyID         ;
	uint8        nFamilyProp       ;
	uint8        online_type          ;//0����,1����
	uint16       return_count       ;
	FAMILYMEMBER_ACKINFO  members[1]       ;
// 	char         error[40]          ;

	XT_GET_CMEMBERS_ACK()
	{
		memset(this,0,sizeof(XT_GET_CMEMBERS_ACK));
	}

} XT_GET_CMEMBERS_ACK, *LPXT_GET_CMEMBERS_ACK;


//��ȡ�û�ӵ�еĹ̶�Ⱥ�嵥
typedef struct XT_GET_CLIST_REQ
{
	uint8       nFamilyProp;
	uint8       nCount;

	XT_GET_CLIST_REQ()
	{
		memset(this,0,sizeof(XT_GET_CLIST_REQ));
	}

} XT_GET_CLIST_REQ, *LPXT_GET_CLIST_REQ;

typedef struct XT_GET_CLIST_ACK
{
	uint8       ret           ;
	uint8       nFamilyProp   ;
	uint8       nCount        ;
	uint32      nFamilyID[1]	  ;

	XT_GET_CLIST_ACK()
	{
		memset(this,0,sizeof(XT_GET_CLIST_ACK));
	}

} XT_GET_CLIST_ACK, *LPXT_GET_CLIST_ACK;

typedef struct XT_CLUSTER_DISMISS_REQ
{
// 	uint32      nUserID;
	uint32      nFamilyID;
	uint8       nFamilyProp;

	XT_CLUSTER_DISMISS_REQ()
	{
		memset(this,0,sizeof(XT_CLUSTER_DISMISS_REQ));
	}

} XT_CLUSTER_DISMISS_REQ, *LPXT_CLUSTER_DISMISS_REQ;

typedef struct XT_CLUSTER_DISMISS_ACK
{
	uint8      ret;
	uint32     nFamilyID;
	uint8      nFamilyProp;
// 	char       error[40];

	XT_CLUSTER_DISMISS_ACK()
	{
		memset(this,0,sizeof(XT_CLUSTER_DISMISS_ACK));
	}

} XT_CLUSTER_DISMISS_ACK, *LPXT_CLUSTER_DISMISS_ACK;

//#define CMD_CLUSTERMSG_SEND         0x911
typedef struct XT_FAMILY_SENDMSG_REQ	//ֻ����������Ϣ��������Ϣֱ��ת�����ͻ���
{
	uint32			familyID;
	uint32			senderID;
	uint16			version;
	uint8           familyProp;
	uint8           msgType;	//0x00����ͨȺ��Ϣ��0x01���Զ��ظ���Ϣ������ʱ������
}XT_FAMILY_SENDMSG_REQ,* LPXT_FAMILY_SENDMSG_REQ ;

typedef struct XT_FAMILY_SENDMSG_ACK
{
	uint8           ret;
	uint32			familyID;
}XT_FAMILY_SENDMSG_ACK, *LPXT_FAMILY_SENDMSG_ACK;

//typedef XT_CLUSTER_INFO  XT_SET_CINFO_REQ, *LPXT_SET_CINFO_REQ;
typedef struct XT_SET_CINFO_REQ
{
	uint32    nFamilyID;
	uint8     nFamilyProp;							//Ⱥ���� FIRM_CLUSTER|TEMP_FAMILY
	uint8     nFamilyType;							//�������� FAMILY_COMMON ��ͨ���壬FAMILY_VIP VIP����
	uint16    nFamilyClass;							//����
	uint32    nCreatorID;

	uint8	  nFamilyFaceID;							//���ID
	char      szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	//������ӵ�ַ

	uint8     nJoinFlag;							//������� FAMILY_JOIN_ANY|FAMILY_JOIN_VALI|FAMILY_JOIN_UNALLOW
	uint8     nViewFlag;							//ҳ�����Ȩ��	FAMILY_VIEW_MEM|FAMILY_VIEW_REG|FAMILY_VIEW_ALL
	char      szFamilyName[MAX_FAMILYNAME_LEN + 1];
	char      szFamilyDesc[MAX_FAMILYDESC_LEN + 1];
	char      szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];
	uint16    nMemberLimit;	
	char      szFamilySign[MAX_FAMILYNAME_LEN + 1];//Ⱥǩ��
	char      szFamilyNotice[MAX_FAMILYNAME_LEN + 1];//Ⱥ����
	uint8	  nAction;///��������

	XT_SET_CINFO_REQ()
	{
		memset(this,0,sizeof(XT_SET_CINFO_REQ));
	}
	
}XT_SET_CINFO_REQ,*LPXT_SET_CINFO_REQ;

typedef struct XT_SET_CINFO_ACK
{
	uint8        ret;
	uint32       nFamilyID;
	uint8        nFamilyProp;
	char		 szError[MAX_ERROR_BUF];

	XT_SET_CINFO_ACK()
	{
		memset(this,0,sizeof(XT_SET_CINFO_ACK));
	}

} XT_SET_CINFO_ACK, *LPXT_SET_CINFO_ACK;

typedef struct XT_COFFLINE_MSG_REQ
{
	uint32       uid;
	uint16	     start_pos;
	uint32       cluster_id;
	uint8        count;

	XT_COFFLINE_MSG_REQ()
	{
		memset(this,0,sizeof(XT_COFFLINE_MSG_REQ));
	}

}XT_COFFLINE_MSG_REQ, *LPXT_COFFLINE_MSG_REQ;


typedef struct ST_OFFLINE_MSG
{	
	uint32	    recvID   ;
	uint32		sendID   ;
	uint32		tm;
	uint16		msgtype       ;  
	char        msg[MAX_MSG_LEN+1];
	uint16		msglen;
	uint32		msgid;

	ST_OFFLINE_MSG()
	{
		memset(this,0,sizeof(ST_OFFLINE_MSG));
	}

} ST_OFFLINE_MSG, *LPST_OFFLINE_MSG;

typedef struct ST_WEBIM_MSG
{
    uint32      recvID   ;
    uint32      sendID   ;
    uint32      tm;
    uint16      msgtype       ;
    char        msg[MAX_MSG_LEN+1];
    uint16      msglen;
    uint32      msgid;
    char        from_nickname[MAX_NICKNAME_LEN+1];
    char        msgtext[MAX_MSG_LEN+1];
	char 		uuid[MAX_UUID_LEN + 1];

    ST_WEBIM_MSG()
    {
        memset(this,0,sizeof(ST_WEBIM_MSG));
    }

} ST_WEBIM_MSG, *LPST_WEBIM_MSG;

typedef struct ST_WEB_MSG
{	
	uint32     recID	 ;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    recvID   ;
	uint32		sendID   ;
	char		sendTime[32];
	uint16		msgtype   ;  
	char        msg[MAX_MSG_LEN + 1];
	ST_WEB_MSG()
	{
		memset(this,0,sizeof(ST_WEB_MSG));
	}

} ST_WEB_MSG, *LPST_WEB_MSG;

typedef struct ST_SYSTEM_LOG
{	
	uint32		recID			;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    projID			;	/*	ҵ����룬��100001		10-���У�11-������ҳ��12-���壬13-mini��14-wap��15-�����꣬16-��̳��17-���18-�ȿᣬ19-��̨			*/
	char	    strUserID[1024]	;	/*���������û���*/
	char		strLogID[1024];		/*�������ļ�¼�ɣĴ�*/
	uint8       flatID			;	/*ҵ��ID����projIdǰ2λһ�£���10*/
	uint32		operID			;	/*������Ա���û�ID*/
	enum{MODIFY_NICKNAME = 100101,MODIFY_FACEURI = 100201,MODIFY_FAMINAME = 120101,MODIFY_FAMIFACEURI = 120102,ADD_FRIENDS = 110501,DEL_FRIENDS = 110502,ADD_FAMIMEM = 120201,SET_FAMIMEM = 120202,DEL_FAMIMEM = 1202013, ROV_FAMIMEM = 120204};
	ST_SYSTEM_LOG()
	{
		memset(this,0,sizeof(ST_SYSTEM_LOG));
	}

} ST_SYSTEM_LOG, *LPST_SYSTEM_LOG;

typedef struct ST_OPERATION_MSG
{	
	uint32      recID	 ;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    userID   ;
	uint16		classID   ;	/* 1=������־,2=�ϴ�ͼƬ,3=�ϴ�����,4=�ϴ���Ƶ,5=�ϴ������ļ�*/
	char        nickname[MAX_NICKNAME_LEN+1];
	char		srcuri[500];/* ��������Ŀ������				*/
	char		title[200];/* ��ʷ��������					*/
	char		outlines[500];/* ��־����ժҪ					*/
	char		sendTime[32];
	ST_OPERATION_MSG()
	{
		memset(this,0,sizeof(ST_OPERATION_MSG));
	}

} ST_OPERATION_MSG, *LPST_OPERATION_MSG;

typedef struct ST_WOKU_MSG
{	
	uint8		type	 ;  //��Ϣ���ͣ�0.�û�������Ϣ 1.�ռ��䣬2.ϵͳ����,3.�û�����,4.������ҳ���� 5.������������
	uint32      recID	 ;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    recvID   ;  //������ID
	char		title[201];/* ����					*/
	char		content[901];/* ��־����ժҪ					*/
	uint8		classID  ;	/* 1=������־,2=�ϴ�ͼƬ,3=�ϴ�����,4=�ϴ���Ƶ,5=�ϴ������ļ�*/
	uint32		logID	 ;	/*����ID*/
	char		srcuri[501];/* ��������Ŀ������				*/
	uint32		sendID   ;
	char        username[MAX_USERNAME_LEN + 1];
	char        nickname[MAX_NICKNAME_LEN + 1];
	char		sendTime[32];
	char		toids[512];
	enum{USEROPERATION,USERRECVMSG,SYSTEMPUB,USERPUB,USERCOMMENT,LOGCOMMENT};

	ST_WOKU_MSG()
	{
		memset(this,0,sizeof(ST_WOKU_MSG));
	}

} ST_WOKU_MSG, *LPST_WOKU_MSG;


typedef struct ST_WOKU_ACK
{	
	uint8		type	 ;  //��Ϣ���ͣ�0.�û�������Ϣ
	uint32      recID	 ;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    recvID   ;

	ST_WOKU_ACK()
	{
		memset(this,0,sizeof(ST_WOKU_ACK));
	}

} ST_WOKU_ACK, *LPST_WOKU_ACK;

typedef struct ST_MOBILE_MSG
{	
	uint32      recID	 ;	//��ˮ��
	uint32	    recvID   ;  //������ID
	uint32		sendID	 ;  //������ID
	char		mobileno[17];	//�������ֻ���
	char		linkid[21];		//�㲥����ID
	uint16		bizType;		//����
	char        username[MAX_USERNAME_LEN + 1];
	char        nickname[MAX_NICKNAME_LEN + 1];
	char		content[201];/* ����					*/
	char		sendTime[32];

	ST_MOBILE_MSG()
	{
		memset(this,0,sizeof(ST_MOBILE_MSG));
	}

} ST_MOBILE_MSG, *LPST_MOBILE_MSG;


typedef struct ST_MOBILE_ACK
{	
	uint32      recID	 ;

	ST_MOBILE_ACK()
	{
		memset(this,0,sizeof(ST_MOBILE_ACK));
	}

} ST_MOBILE_ACK, *LPST_MOBILE_ACK;

typedef struct ST_MOBILE_SEND
{	
	uint32	    recvID   ;  //������ID
	char		mobileNO[17];	//�������ֻ���
	char		linkID[21];		//�㲥����ID
	uint16		bizType;		//����
	uint32		bizID;
	uint8		smsType;		/* ��������, 0-��ͨ���� 1-wap push */
	char		content[201];/* ����					*/
	char		sendTime[32];

	ST_MOBILE_SEND()
	{
		memset(this,0,sizeof(ST_MOBILE_SEND));
	}

} ST_MOBILE_SEND, *LPST_MOBILE_SEND;


typedef struct ST_MOBILE_SEND_ACK
{	
	uint8      ret	 ;
	char	   szError[128];

	ST_MOBILE_SEND_ACK()
	{
		memset(this,0,sizeof(ST_MOBILE_SEND_ACK));
	}

} ST_MOBILE_SEND_ACK, *LPST_MOBILE_SEND_ACK;



typedef struct ST_WEB_ACK
{
	uint32      recID;

	ST_WEB_ACK()
	{
		memset(this,0,sizeof(ST_WEB_ACK));
	}

} ST_WEB_ACK, *LPST_WEB_ACK;

typedef struct XT_WEB_IND
{
	uint32      from_id;
	char        from_username[MAX_USERNAME_LEN+1];
	uint32      recv_id;

	uint32      time;

	uint8       face;

	uint8       font_flag;

	char        message[900];

	XT_WEB_IND()
	{
		memset(this,0,sizeof(XT_WEB_IND));
	}
}XT_WEB_IND , *LPXT_WEB_IND;

typedef struct XT_WEB_ACK
{
	uint32      from_id;
	char        from_username[MAX_USERNAME_LEN+1];
	uint32      recv_id;

	XT_WEB_ACK()
	{
		memset(this,0,sizeof(XT_WEB_ACK));
	}

} XT_WEB_ACK, *LPXT_WEB_ACK;

//#define CMD_CLUSTER_NOTIFY_REQ         0x920
typedef struct XT_FAMILY_NOTIFY_REQ
{
	uint32      nUserID;
	uint8       nStat;
}XT_FAMILY_NOTIFY_REQ, *LPXT_FAMILY_NOTIFY_REQ;

typedef struct XT_USER_OPERATION_REQ
{
	uint32     uid        ;
	uint16     count      ;
	uint16     oper_ids[200]  ;
	uint16     oper_cnt[200]  ;

	XT_USER_OPERATION_REQ()
	{
		memset(this,0,sizeof(XT_USER_OPERATION_REQ));
	}

} XT_USER_OPERATION_REQ, *LPXT_USER_OPERATION_REQ;

typedef struct _XT_MOD_FRIEND_COMMNAME_REQ
{
	uint32		id;				// ��ǰ�û�id
	uint32		fid;			// ����id
	char		commName[MAX_COMMENTNAME_LEN+1];	// ��ע����
}XT_MOD_FRIEND_COMMNAME_REQ,*LPXT_MOD_FRIEND_COMMNAME_REQ;

typedef struct _XT_MOD_FRIEND_COMMNAME_ACK
{
	uint8		rt;				// ����ֵ��0�ɹ���1ʧ��
	uint32		fid;			// ����id
	char		commName[MAX_COMMENTNAME_LEN+1];	// ��ע����
}XT_MOD_FRIEND_COMMNAME_ACK,*LPXT_MOD_FRIEND_COMMNAME_ACK;

typedef struct XT_ROUTERPACKET_REQ
{
	uint32 uid;				//�Է�����
	XT_ROUTERPACKET_REQ()
	{
		memset(this,0,sizeof(XT_ROUTERPACKET_REQ));
	}
}XT_ROUTERPACKET_REQ,*LPXT_ROUTERPACKET_REQ;

typedef struct XT_ROUTERPACKET_ACK
{
	uint32 uid;		

	XT_ROUTERPACKET_ACK()
	{
		memset(this,0,sizeof(XT_ROUTERPACKET_ACK));
	}
}XT_ROUTERPACKET_ACK,*LPXT_ROUTERPACKET_ACK;

//Ⱥ��Ƭ����
typedef struct XT_PROFILENAME_SET_REQ
{
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			// ��ǰȺ�ĳ�ԱID
	char		ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1];	// Ⱥ��Ƭ
	XT_PROFILENAME_SET_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_SET_REQ));
	}

}XT_PROFILENAME_SET_REQ, *LPXT_PROFILENAME_MOD_REQ;

typedef struct XT_PROFILENAME_SET_ACK
{
	uint8       ret;
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			// ��ǰȺ�ĳ�ԱID
	char		ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1];	// Ⱥ��Ƭ
	XT_PROFILENAME_SET_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_SET_ACK));
	}
}XT_PROFILENAME_SET_ACK, *LPXT_PROFILENAME_SET_ACK;


//Ⱥ��Ƭ��ȡbyid
typedef struct XT_PROFILENAME_GET_BYID_REQ
{
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			// ��ǰȺ�ĳ�ԱID
	XT_PROFILENAME_GET_BYID_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_BYID_REQ));
	}

}XT_PROFILENAME_GET_BYID_REQ, *LPXT_PROFILENAME_GET_BYID_REQ;

typedef struct XT_PROFILENAME_GET_BYID_ACK
{
	uint8       ret;
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			// ��ǰȺ�ĳ�ԱID
	char		ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1];	// Ⱥ��Ƭ
	XT_PROFILENAME_GET_BYID_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_BYID_ACK));
	}
}XT_PROFILENAME_GET_BYID_ACK, *LPXT_PROFILENAME_GET_BYID_ACK;



//Ⱥ��Ƭ��ȡ��ȫ��
typedef struct CMEM_INFO
{
	uint32	   nMemID;      // ��ǰȺ�ĳ�ԱID
	char       sProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1]; // Ⱥ��Ƭ
}CMEM_INFO;


////����Ⱥ��Ƭ��ȡ����
typedef struct XT_PROFILENAME_GET_REQ
{
	uint32		nFamilyID;     	// ��ǰȺid
	XT_PROFILENAME_GET_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_REQ));
	}

}XT_PROFILENAME_GET_REQ, *LPXT_PROFILENAME_GET_REQ;


////����������Ӧ��
typedef struct XT_PROFILENAME_GET_ACK
{
	uint8           ret;
	uint32	  		nFamilyID;      // ��ǰȺid
	uint32          nCount;         // ��ǰȺ��Ա����
	CMEM_INFO    	stMemInfo[MAX_MEM_CNT];   // ��ǰȺ��Ա��Ⱥ��Ƭ��
	XT_PROFILENAME_GET_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_ACK));
	}
}XT_PROFILENAME_GET_ACK, *LPXT_PROFILENAME_GET_ACK;




//���������Ⱥ����
typedef struct XT_CLUSTER_INVITE_REQ
{
	uint8      count;//������Ѹ���
	char       clustername[MAX_FAMILYNAME_LEN+1];//Ⱥ����
	char       nickname[MAX_NICKNAME_LEN+1] ;//Ⱥ�ǳ�
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	uint32     userID[1];//�������ID����
	XT_CLUSTER_INVITE_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_REQ));
	}
}XT_CLUSTER_INVITE_REQ, *LPXT_CLUSTER_INVITE_REQ;

//֪ͨ����������Ⱥ
typedef struct XT_CLUSTER_AGREE_IND
{
	uint32     invite_id;//������ID
	char       clustername[MAX_FAMILYNAME_LEN+1];//Ⱥ����
	char       nickname[MAX_NICKNAME_LEN+1] ;//Ⱥ�ǳ�
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	XT_CLUSTER_AGREE_IND ()
	{
		memset(this,0,sizeof(XT_CLUSTER_AGREE_IND));
	}
} XT_CLUSTER_AGREE_IND, *LPXT_CLUSTER_AGREE_IND;

typedef struct XT_CLUSTER_AGREE_REQ
{
	uint8      action;//0��ͬ�� 1���ܾ�
	uint32     invite_id;//������ID
	uint32     friend_id;//��������ID
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	XT_CLUSTER_AGREE_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTER_AGREE_REQ));
	}
}XT_CLUSTER_AGREE_REQ, *LPXT_CLUSTER_AGREE_REQ;

typedef struct XT_CLUSTER_AGREE_ACK
{
	uint8 ret;
	XT_CLUSTER_AGREE_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTER_AGREE_ACK));
	}
}XT_CLUSTER_AGREE_ACK, *LPXT_CLUSTER_AGREE_ACK;

//P2p���԰�
typedef struct _XT_P2PTEST_REQ
{
	// ip����
	enum TYPE{ 
		P2P,	// p2p������ַ
		LAN		// ������ַ
	};
	uint32		dest_ip;
	uint16      dest_port;
	uint8		ipType;
	uint32      id;			// ����id
	uint32		toid;		// Ŀ�ķ�id
	_XT_P2PTEST_REQ()
	{
		memset(this,0,sizeof(_XT_P2PTEST_REQ));
	}

} XT_P2PTEST_REQ, *LPXT_P2PTEST_REQ;

typedef struct _XT_P2PTEST_IND
{
	// ip����
	enum TYPE{ 
		P2P,	// p2p������ַ
		LAN		// ������ַ
	};
	uint32		id;
	uint32		ip;
	uint16		port;
	uint8		ipType;
	char		key[16];
	_XT_P2PTEST_IND()
	{
		memset(this,0,sizeof(_XT_P2PTEST_IND));
	}
} XT_P2PTEST_IND,*LPXT_P2PTEST_IND;

typedef struct _XT_P2PTEST_ACK
{
	uint32		id;			// Ӧ��id
	uint8		testType;	// �������ͣ��������Ӻ͹�������
	enum TYPE{
		P2P,	// p2p������ַ
		LAN		// ������ַ
	};

} XT_P2PTEST_ACK, *LPXT_P2PTEST_ACK;


typedef struct XT_USER_ACTION_AUDIT_REQ
{
	uint16	count;
	struct Action
	{
		uint16	OpeId;
		uint16	count;
	} action[1]; 
} XT_USER_ACTION_AUDIT_REQ, *LPXT_USER_ACTION_AUDIT_REQ;

typedef struct XT_MODIFY_FRIENDFLAG_REQ
{
	uint32	friendid;
	uint8	flag;
}XT_MODIFY_FRIENDFLAG_REQ, *LPXT_MODIFY_FRIENDFLAG_REQ;

typedef struct XT_USER_ACTION_AUDIT_ACK
{
	uint8 result;
} XT_USER_ACTION_AUDIT_ACK,*LPXT_USER_ACTION_AUDIT_ACK;

typedef struct XT_FRIENDINVITE_REQ
{
	char	emailAddress[500];
	char	emailTitle[100];
	char	emailContent[512];
	XT_FRIENDINVITE_REQ()
	{
		memset(this,0,sizeof(XT_FRIENDINVITE_REQ));
	}
}XT_FRIENDINVITE_REQ, *LPXT_FRIENDINVITE_REQ ;

typedef struct XT_FRIENDINVITE_ACK
{
	uint8 rusult;
}XT_FRIENDINVITE_ACK, *LPXT_FRIENDINVITE_ACK;

typedef struct XT_TRANS_LOGIN_REQ
{

	uint8    data            ;//0 

	XT_TRANS_LOGIN_REQ()
	{
		memset(this,0,sizeof(XT_TRANS_LOGIN_REQ));
	}

} XT_TRANS_LOGIN_REQ, * LPXT_TRANS_LOGIN_REQ;

//��¼��Ӧ
typedef struct XT_TRANS_LOGIN_ACK
{
	uint8	  ret				;//0 ����ֵ
	uint32    sessionid			;// SessionID
	//char     sessioncode[16]	;//SessionCode
	
	XT_TRANS_LOGIN_ACK()
	{
		memset(this,0,sizeof(XT_TRANS_LOGIN_ACK));
	}
} XT_TRANS_LOGIN_ACK, * LPXT_TRANS_LOGIN_ACK;

//����ת��������ӽ�����
typedef struct XT_TRANS_SETRECV_REQ
{
	uint32	sessionid;
	uint8   action;	//���� 0x01 ���   0x02 ɾ��
	enum {RECV_ADD = 0x01,RECV_DEL = 0x02};
	uint8   count;
	uint32  userid[1];
	XT_TRANS_SETRECV_REQ()
	{
		memset(this, 0, sizeof(XT_TRANS_SETRECV_REQ));
	}
}XT_TRANS_SETRECV_REQ, *LPXT_TRANS_SETRECV_REQ;

typedef struct XT_TRANS_SETRECV_ACK
{
	uint8	ret;
	uint32	sessionid;
	uint8	action;
	XT_TRANS_SETRECV_ACK()
	{
		memset(this, 0, sizeof(XT_TRANS_SETRECV_ACK));
	}
}XT_TRANS_SETRECV_ACK, *LPXT_TRANS_SETRECV_ACK;	

typedef struct XT_TRANS_RECVLOGIN_REQ
{
	uint32		sessionid;
	uint8		action;
	enum {RECV_LOGIN = 0x01, RECV_LOGOUT = 0x02};
	//char		sessioncode[16]	;//SessionCode
	XT_TRANS_RECVLOGIN_REQ()
	{
		memset(this, 0, sizeof(XT_TRANS_RECVLOGIN_REQ));
	}
}XT_TRANS_RECVLOGIN_REQ, *LPXT_TRANS_RECVLOGIN_REQ;

typedef struct XT_TRANS_RECVLOGIN_ACK
{
	uint8		ret;
	uint32		sessionid;
	uint8		action;
	XT_TRANS_RECVLOGIN_ACK()
	{
		memset(this, 0, sizeof(XT_TRANS_RECVLOGIN_ACK));
	}
}XT_TRANS_RECVLOGIN_ACK, *LPXT_TRANS_RECVLOGIN_ACK;

typedef struct XT_TRANS_NOTIFYSENDER_IND
{
	uint32		sessionID;
	uint8		action;
	uint32		receiverID;
	XT_TRANS_NOTIFYSENDER_IND()
	{
		memset(this, 0, sizeof(XT_TRANS_NOTIFYSENDER_IND));
	}
}XT_TRANS_NOTIFYSENDER_IND, *LPXT_TRANS_NOTIFYSENDER_IND;

typedef struct XT_TRANS_NOTIFYRECEIVER_IND
{
	uint32		sessionID;
	uint32		senderID;
	XT_TRANS_NOTIFYRECEIVER_IND()
	{
		memset(this, 0, sizeof(XT_TRANS_NOTIFYRECEIVER_IND));
	}
}XT_TRANS_NOTIFYRECEIVER_IND, *LPXT_TRANS_NOTIFYRECEIVER_IND;

typedef struct XT_TRANS_NOTIFYCLOSE_IND
{
	uint32		sessionID;
	XT_TRANS_NOTIFYCLOSE_IND()
	{
		memset(this, 0, sizeof(XT_TRANS_NOTIFYCLOSE_IND));
	}
}XT_TRANS_NOTIFYCLOSE_IND, *LPXT_TRANS_NOTIFYCLOSE_IND;

#define MAX_CUST_TIME_LEN 32
#define MAX_CUST_COUNT 1000
#define MAX_CUST_ACCOUNT_LEN 40
#define MAX_CUST_ORDER_LEN 64
#define MAX_CUST_REMAKE_LEN 201
typedef struct XT_GET_NEAR_COMM_CUST_REQ
{
	uint32  uiBusinessID;		// �̻�ID
	char szBgnTime[MAX_CUST_TIME_LEN];			// ��ʼʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szEndTime[MAX_CUST_TIME_LEN];			// ����ʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	uint32 iMaxCustCount;		// ��ȡ������û�����Ĭ��ֵΪ1000����ָ��ʱ������û���������ֵʱ��ֻ��ȡ�������������ͻ�
	uint16 start_pos;			//�����Ϣ��ʼλ��
	uint16 iNeedCustCount;		//��ȡ���������
	uint8 bDirect;             //��ȡ������ʽ

	XT_GET_NEAR_COMM_CUST_REQ()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REQ));
		iMaxCustCount = MAX_CUST_COUNT;
	}
}XT_GET_NEAR_COMM_CUST_REQ, *LPXT_GET_NEAR_COMM_CUST_REQ;

// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
typedef struct XT_GET_NEAR_COMM_CUST_ACK
{
	uint32 ret;
	uint32 BusinessID;
	
	uint8   bMask;						// ֻ��Ϊ����ö��
	enum
	{
		MK_ALL_CUST_COUNT = 0x00,		// ��ʾactualCount���ص��������ϵ�ͻ�����Ŀ����XT_GET_NEAR_COMM_CUST_REQ.iStartPos == 999ʱ��
		MK_CUR_CUST_COUNT = 0x01,		// ��ʾactualCount����CommInfo����Ĵ�С
	};

	uint32  actualCount;			// �ͻ�����
	struct TagCommInfo
	{
		char szCommAccount[MAX_CUST_ACCOUNT_LEN];			// �ͻ��˺�
		char szCommTime[MAX_CUST_TIME_LEN];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
		char szTransInfo[MAX_CUST_ORDER_LEN];			// ��ʾ��ϵ�����һ�ݽ��׶����š����û�н��׶���������ʾ�հ�
		char szRemarks[MAX_CUST_REMAKE_LEN];			// ��ʾ�����촰�����ҶԿͻ��ı�ע��Ϣ
	}CommInfo[1];

	XT_GET_NEAR_COMM_CUST_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_ACK));
	}
}XT_GET_NEAR_COMM_CUST_ACK, *LPXT_GET_NEAR_COMM_CUST_ACK;

// ���������ϵ�ͻ���Ϣ����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_REQ
{
	uint32  BusinessID;				// �̻�ID
	uint32	iCommCount;					// �ͻ�����
	struct TagCommInfo
	{
		char szCommAccount[MAX_CUST_ACCOUNT_LEN];			// �ͻ��˺�
		char szCommTime[MAX_CUST_TIME_LEN];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	}CommInfo[1];

	XT_UPDATE_NEAR_COMM_CUST_INFO_REQ()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_REQ));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_REQ, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_REQ; 

// ���������ϵ�ͻ���Ϣ����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_ACK
{
	uint8		ret;					// ���������ص���Ϣ����ʶ�ɹ����

	XT_UPDATE_NEAR_COMM_CUST_INFO_ACK()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_ACK));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_ACK, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_ACK;

//��ȡ������˻�id
typedef struct XT_GET_SUB_ACCOUNT_REQ
{
	uint32		merchant_id;					// �̼�id
	uint32 		group_id;						// �̼ҷ���id
	uint8       type;							//�������or���߷���

	XT_GET_SUB_ACCOUNT_REQ()
	{
		memset(this, 0, sizeof(XT_GET_SUB_ACCOUNT_REQ));
	}
}XT_GET_SUB_ACCOUNT_REQ, *LPXT_GET_SUB_ACCOUNT_REQ;

typedef struct XT_GET_SUB_ACCOUNT_ACK
{
	uint32		id;					// ���˻�

	XT_GET_SUB_ACCOUNT_ACK()
	{
		memset(this, 0, sizeof(XT_GET_SUB_ACCOUNT_ACK));
	}
}XT_GET_SUB_ACCOUNT_ACK, *LPXT_GET_SUB_ACCOUNT_ACK;

typedef struct XT_GET_SUB_ONLINE_REQ
{
	uint32_t	cnt;				//��Ҫ��ѯ������
	uint32_t	id[1];				//������Ҫ��ѯ�����˺�

	XT_GET_SUB_ONLINE_REQ()
	{
		memset(this, 0, sizeof(XT_GET_SUB_ONLINE_REQ));
	}
}XT_GET_SUB_ONLINE_REQ, *LPXT_GET_SUB_ONLINE_REQ;

typedef struct XT_GET_SUB_ONLINE_ACK
{
	uint32_t	cnt;				//����

	struct
	{
		uint32_t	id;
		uint32_t	b;				//�Ƿ����������˺�
	}stat[1];

	XT_GET_SUB_ONLINE_ACK()
	{
		memset(this, 0, sizeof(XT_GET_SUB_ONLINE_ACK));
	}

}XT_GET_SUB_ONLINE_ACK, *LPXT_GET_SUB_ONLINE_ACK;

typedef struct XT_GET_USER_STATUS_REQ
{
	uint32_t	cnt;				//��Ҫ��ѯ������
	uint32_t	id[1];				//������Ҫ��ѯ�����˺�

	XT_GET_USER_STATUS_REQ()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_REQ));
	}
}XT_GET_USER_STATUS_REQ, *LPXT_GET_USER_STATUS_REQ;

typedef struct XT_GET_USER_STATUS_ACK
{
	uint32_t	cnt;				//����

	struct
	{
		uint32_t	id;
		uint32_t	b;				//�Ƿ����������˺�
	}stat[1];

	XT_GET_USER_STATUS_ACK()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_ACK));
	}

}XT_GET_USER_STATUS_ACK, *LPXT_GET_USER_STATUS_ACK;



typedef struct XT_MOD_HANGUP_STATE_REQ
{
	uint32_t uUserId;	 		// �û�Id
	uint8 	uHangupState;	 	// ����״̬��0--������1--����

	enum
	{
		RECEPTION =	0,
		HANGUP	= 	1
	};
	XT_MOD_HANGUP_STATE_REQ()
	{
		memset(this,0,sizeof(XT_MOD_HANGUP_STATE_REQ));
	}

} XT_MOD_HANGUP_STATE_REQ, * LPXT_MOD_HANGUP_STATE_REQ;

#define BUFFER_SIZE 1024
typedef struct _SEND_FILE_PACK_REQ
{  
    uint32  send_id;         ////����ID
    uint32  fid;             ////����ID
	uint8  	flag;			////�ļ������ļ����ݡ��ļ�����
	enum{FILE_NAME = 0x01, FILE_CONTENT = 0x02, FILE_END = 0x03};
	uint32  file_id;		////�ļ�id
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
	char buf[BUFFER_SIZE];   ////�����ļ����� 
}SEND_FILE_PACK_REQ,*LPSEND_FILE_PACK_REQ; 

typedef struct _SEND_FILE_PACK_ACK
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
}SEND_FILE_PACK_ACK,*LPSEND_FILE_PACK_ACK; 

typedef struct _SEND_FILE_PACK_IND
{
	uint32  send_id;         ////����ID
    uint32  fid;             ////����ID
	uint8  	flag;			///�ļ������ļ����ݡ��ļ�����
	enum{FILE_NAME = 0x01, FILE_CONTENT = 0x02, FILE_END = 0x03};
	uint32  file_id;		////�ļ�id
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
}SEND_FILE_PACK_IND,*LPSEND_FILE_PACK_IND; 

typedef struct _SEND_FILE_PACK_IND_ACK
{  
	uint32  send_id;         ////����ID
	uint32  fid;             ////����ID
	uint8  	flag;			 ////�ļ������ļ����ݡ��ļ�����
	enum  { SENDFILE_NAME = 1, SENDFILE_CONTENT = 2, SENDFILE_NAMEEND = 3 };
	uint32  file_id;         ////�ļ�ID
	uint32  pack_id;         ////������� 
	uint32  size;            ////���鳤��  
}SEND_FILE_PACK_IND_ACK,*LPSEND_FILE_PACK_IND_ACK; 

// ���������ϵ�ͻ���ע����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ
{
	uint32  uiBusinessID;				// �̻�ID	

	char szCommAccount[32];			// �ͻ��˺�
	char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szRemarks[201];

	XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ));
	}
}XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ, *LPXT_UPDATE_NEAR_COMM_CUST_REMARK_REQ; 


// ���������ϵ�ͻ���ע����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK
{
	uint8		ret;					// ���������ص���Ϣ����ʶ�ɹ����

	XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK));
	}
}XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK, *LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK;



//�Ӻ�̨��ȡ�����ϵ�ͻ���ע��������Ϣ��
typedef struct XT_GET_NEAR_COMM_CUST_REMARK_REQ
{
	uint32  uiBusinessID;				// �̻�ID	
	char szCommAccount[32];			// �ͻ��˺�

	XT_GET_NEAR_COMM_CUST_REMARK_REQ()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REMARK_REQ));
	}
}XT_GET_NEAR_COMM_CUST_REMARK_REQ, *LPXT_GET_NEAR_COMM_CUST_REMARK_REQ; 

// �Ӻ�̨��ȡ�����ϵ�ͷ���ע��������Ϣ��
typedef struct XT_GET_NEAR_COMM_CUST_REMARK_ACK
{
	uint8		ret;					// ���������ص���Ϣ����ʶ�ɹ����
	uint32  uiBusinessID;		
	char szCommAccount[32];	
	char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szRemarks[201];

	XT_GET_NEAR_COMM_CUST_REMARK_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REMARK_ACK));
	}
}XT_GET_NEAR_COMM_CUST_REMARK_ACK, *LPXT_GET_NEAR_COMM_CUST_REMARK_ACK;


typedef struct XT_CUSTSER_TRAN_REQ
{
	uint32 uSid;					// ���ͷ�Id
	uint32 uDid;					// ���շ�Id
	uint32 uCustomerId;				// �ͻ�Id
	char szOrderId[64];				// �������
	char szTranReason[512];			// ת��ԭ��
	XT_CUSTSER_TRAN_REQ()
	{
		memset(this,0,sizeof(XT_CUSTSER_TRAN_REQ));
	}
} XT_CUSTSER_TRAN_REQ, * LPXT_CUSTSER_TRAN_REQ;

// Ӧ��
typedef struct XT_CUSTSER_TRAN_ACK
{
	uint8 ret;						// ���շ����յ�ת�Ӻ󣬷������ͷ���Ӧ��
	uint32 uSid;					// ���ͷ�Id
	uint32 uDid;					// ���շ�Id
	uint32 uCustomerId;				// �ͻ�Id

	XT_CUSTSER_TRAN_ACK()
	{
		memset(this,0,sizeof(XT_CUSTSER_TRAN_ACK));
	}
} XT_CUSTSER_TRAN_ACK, * LPXT_CUSTSER_TRAN_ACK;




#ifndef _MINI_CLIENT_
#pragma pack(pop)
#endif
