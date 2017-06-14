/************************************************************************ 
 �ļ��������ͻ���Э�鶨�壬�����������ͻ���֮��Ľ���Э��
		   
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/
#pragma once

#include "protocol.h"
#include "publictype.h"
#include "..\Mini_Client\MINI139_SRC\MemCrypt.h"

//#ifndef _MINI_CLIENT_
#pragma pack(push,1)
//#endif

#define CPIC_PACKET_LEN	1352

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
private:
	uint32  id           ;	// 0 �û�id
	char    username[MAX_USERNAME_LEN+1] ;	// 3 �û���
public:
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 �����ʼ�
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
private:
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 ����
public:
	char	comment_name[MAX_COMMENTNAME_LEN+1];// �������ѱ�ע����

	char    face_url[MAX_HEADURL_LEN+1] ;
	char    flash_url[MAX_HEADURL_LEN+1] ;
	uint8   bind_mobile  ;//�ֻ��
	uint8   sex          ;
	uint8   age			 ;//4 ����
	//---Ϊ�˱��������ֶΣ���ʹ�����������ֶα�ʾ��ַ��ʡ���У�������---�ﶡ 2015/1/19
	uint8   province	 ;//5 ʡ			��ʡ��
	uint8   country		 ;//6 ����			��������	
	uint8   city	     ;//7 ����			���У�
	//--------------------------------------------------------------------------------
	uint8   career       ;//9 ְҵ
	uint8   rule         ;//10 ��Ϊ���ѵ���֤��ʽ
	uint8	familyflag	 ;// ��ΪȺ��Ա��֤��ʽ    0������     1���ܾ�
	uint8   linkpop		 ;//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;//11 ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;//12 �ֻ�
	float   gold_money   ;//�����Ŀ
	uint32  jifen		 ;//����	/*�¶��壺����Ƿ�Ϊ����*/
	uint16  message_count;//��Ϣ��
	uint32  online_time  ;//����ʱ��

	char    user_sign[MAX_USERSIGN_LEN+1]   ;//����ǩ��
	char    description[MAX_DESCRIPTION_LEN+1]; //13 ����˵��
	char    profile_name[MAX_CLUSTER_MEMBER_PROFILE + 1]; //14����Ⱥ��Ƭ
private:
	char	szGUID[MAX_GUID_LENGTH + 1];	// ��ӪGUID(MAX_GUID_LENGTH = 40��

public:
	XT_USER_INFO()
	{
		memset(this,0,sizeof(XT_USER_INFO));
		sex	 = 3;
	}

public:
	// ����ID
	void SetId(uint32 uId)
	{
		this->id = EncryptUint32(uId);
	}
	// �õ�ID
	uint32 GetId()
	{
		return DecryptUint32(this->id);
	}

	// �����û���
	void SetUserName(string strUserName)
	{
		EncryptStr(strUserName, this->username, sizeof(this->username));
	}
	// �õ��û���
	string GetUserName()
	{
		return DecryptStr(this->username, sizeof(this->username));
	}

	// �õ��ļ���
	string GetUserFileName()
	{
		string name = DecryptStr(this->username, sizeof(this->username));
		CString strName = name.c_str();
		strName.Replace("\\", "&#92");
		strName.Replace("/", "&#47");
		strName.Replace(":", "&#58;");
		strName.Replace("*", "&#42");
		strName.Replace("?", "&#63;");
		strName.Replace("<", "&lt;");
		strName.Replace(">", "&gt;");
		strName.Replace("|", "&#124;");
		return LPCSTR(strName);
	}

	// ��������
	void SetPassword(string strPassword)
	{
		EncryptStr(strPassword, this->password, sizeof(this->password));
	}
	// �õ�����
	string GetPassword()
	{
		return DecryptStr(this->password, sizeof(this->password));
	}

	// �����ֻ���
	void SetMobile(string strMobile)
	{
		EncryptStr(strMobile, this->mobile, sizeof(this->mobile));
	}
	// �õ��ֻ���
	string GetMobile()
	{
		return DecryptStr(this->mobile, sizeof(this->mobile));
	}

	// ����Guid
	void SetGuid(string strGuid)
	{
		EncryptStr(strGuid, this->szGUID, sizeof(this->szGUID));
	}
	// �õ�Guid
	string GetGuid()
	{
		return DecryptStr(this->szGUID, sizeof(this->szGUID));
	}

	// �Լ��ܣ�ֻ��Ϊ�˼�����ǰ�İ汾������Ӵ˺����������ٵ��ã�
	void SelfEncrypt()
	{
		SetId(this->id);
		SetUserName(this->username);
		SetPassword(this->password);
		SetGuid(this->szGUID);
	}
	// �Խ��ܣ�ֻ��Ϊ�˼�����ǰ�İ汾������Ӵ˺����������ٵ��ã�
	void SelfDecrypt()
	{
		id = GetId();
		strcpy(this->username, GetUserName().c_str());
		strcpy(this->password, GetPassword().c_str());
		strcpy(this->szGUID, GetGuid().c_str());
	}
} XT_USER_INFO, * LPXT_USER_INFO;//IMͨѶЭ��Э�������ֶ���,�ṹ����


//�ϵ��û���Ϣ���ֶ�������ṹ����Ϊ�˼����ϰ汾
typedef struct XT_USER_INFO_OLD
{
	uint32  id           ;	// 0 �û�id
	char    username[MAX_USERNAME_LEN_LAST+1] ;	// 3 �û���
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 �����ʼ�
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 ����
	char	comment_name[MAX_COMMENTNAME_LEN+1];// �������ѱ�ע����

	char    face_url[MAX_HEADURL_LEN+1] ;
	char    flash_url[MAX_HEADURL_LEN+1] ;
	uint8   bind_mobile  ;//�ֻ��
	uint8   sex          ;
	uint8   age			 ;//4 ����
	//---Ϊ�˱��������ֶΣ���ʹ�����������ֶα�ʾ��ַ��ʡ���У�������---�ﶡ 2015/1/19
	uint8   province	 ;//5 ʡ			��ʡ��
	uint8   country		 ;//6 ����			��������	
	uint8   city	     ;//7 ����			���У�
	//--------------------------------------------------------------------------------
	uint8   career       ;//9 ְҵ
	uint8   rule         ;//10 ��Ϊ���ѵ���֤��ʽ
	uint8	familyflag	 ;// ��ΪȺ��Ա��֤��ʽ    0������     1���ܾ�
	uint8   linkpop		 ;//��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;//11 ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;//12 �ֻ�
	float   gold_money   ;//�����Ŀ
	uint32  jifen		 ;//����
	uint16  message_count;//��Ϣ��
	uint32  online_time  ;//����ʱ��

	char    user_sign[MAX_USERSIGN_LEN+1]   ;//����ǩ��
	char    description[MAX_DESCRIPTION_LEN+1]; //13 ����˵��
	char    profile_name[MAX_CLUSTER_MEMBER_PROFILE + 1]; //14����Ⱥ��Ƭ
	char	szGUID[MAX_GUID_LENGTH + 1];	// ��ӪGUID(MAX_GUID_LENGTH = 40��

	XT_USER_INFO_OLD()
	{
		memset(this,0,sizeof(XT_USER_INFO_OLD));
	}
} XT_USER_INFO_OLD, * LPXT_USER_INFO_OLD;//IMͨѶЭ��Э�������ֶ���,�ṹ����

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
	uint32    nFamilyID;                            //Ⱥ��
	uint8     nFamilyProp;							//Ⱥ���� FIRM_CLUSTER|TEMP_FAMILY
	uint8     nFamilyType;							//Ⱥ���� FAMILY_COMMON ��ͨȺ��FAMILY_VIP VIPȺ
	uint16    nFamilyClass;							//����
	uint32    nCreatorID;                           //Ⱥ�Ĵ�����ID

	uint8	  nFamilyFaceID;							//���ID
	char      szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	///�޸�ΪȺ��Ƭ��ǡ�(������ӵ�ַ)

	uint8     nJoinFlag;							//������� FAMILY_JOIN_ANY|FAMILY_JOIN_VALI|FAMILY_JOIN_UNALLOW
	uint8     nViewFlag;							//ҳ�����Ȩ��	FAMILY_VIEW_MEM|FAMILY_VIEW_REG|FAMILY_VIEW_ALL
	char      szFamilyName[MAX_FAMILYNAME_LEN + 1]; //Ⱥ����
	char      szFamilyDesc[MAX_FAMILYDESC_LEN + 1]; //Ⱥ����
	char      szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];//Ⱥ��������
    uint16    nNumberLimit;      
	char      szFamilySign[MAX_FAMILYNAME_LEN + 1];//Ⱥǩ������
	char      szFamilyNotice[MAX_FAMILYNAME_LEN + 1];  //Ⱥ��������(����)

	XT_CLUSTER_INFO()
	{
		memset(this,0,sizeof(XT_CLUSTER_INFO));
	}

} XT_CLUSTER_INFO, * LPXT_CLUSTER_INFO;

/*
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
*/
//��¼����
typedef struct XT_LOGIN_REQ
{
	uint8     login_flag; //78��λΪ��½��ʽ��6λ�����һ���ң�5λ��ʾ�ͷ� //4λ�����ж��Ƿ�У����ص�¼
	enum{
		LOGINBYID = 0,
		LOGINBYNAME = 1,
		LOGINBYMOBILENO = 2,
	};

	enum {
		SELLER = 0,
		BUYER = 1,
		CS    = 2,///��ţ�ͷ�ƽ̨
		WORK = 3,//������
	};

	enum {
		CHECK_DIFF_PLACES = 0,		// Ϊ0��ʾУ����ص�¼��Ĭ��
		DONT_CHECK_DIFF_PLACES = 1,	// Ϊ1��ʾ��У����ص�¼
	};

	union ULoginKey
	{
		uint32    id            ;//0 �û�id
		char      username[MAX_USERNAME_LEN + 1]  ;
		char      mobileno[MAX_PHONE_LEN + 1];
	}uLoginKey;

	char      random_key[16];
	char      password[24]  ;//1 ��¼����MD5(MD5(plain)),�������ĵ�2�ؼ����ټ���һ���մ�		ԭ����16
	uint8     status        ;//2 ��¼״̬(�뿪,����..)
	uint32    lanip         ;//�ͻ��˾�����ip��*�����ڴ�����ļ���ת������ͨѶʱ�Ŀͻ���IP��
	uint16    lanport       ;//�ͻ��˾�����port��*�����ڴ�����ļ���ת������ͨѶʱ�Ŀͻ��˶˿ڣ�
    char      sessionID[128];///web session
	char      mobilephoneno[MAX_PHONE_LEN+1]; 
	char      email[MAX_EMAIL_LEN+1];  
    uint8     auto_session;//TOKENֵ 	
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
	uint8	  needChangeAccount;
	char	  szGUID[MAX_GUID_LENGTH + 1];	// ��ӪGUID(MAX_GUID_LENGTH = 40��
	char	  username[MAX_USERNAME_LEN+1] ;	// 3 �û���
	uint32    login_time;
	XT_LOGIN_ACK()
	{
		memset(this,0,sizeof(XT_LOGIN_ACK));
	}
} XT_LOGIN_ACK, * LPXT_LOGIN_ACK;

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
		uint8   nMobileStatus                   ;//�ֻ���״̬
		char	szGUID[MAX_GUID_LENGTH + 1];	// ��ӪGUID(MAX_GUID_LENGTH = 40��
	} FriendInfo[1];
	XT_GET_FRIEND_BASICINFO_NOTIFY()
	{
		memset(this,0,sizeof(XT_GET_FRIEND_BASICINFO_NOTIFY));
	}
}XT_GET_FRIEND_BASICINFO_NOTIFY,*LPXT_GET_FRIEND_BASICINFO_NOTIFY;
/*
typedef struct XT_USERFAMILYFLAG_GET_REQ
{
	uint8 count;
	uint32 userID[1];
}XT_USERFAMILYFLAG_GET_REQ, *LPXT_USERFAMILYFLAG_GET_REQ;
*/
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

//���������Ⱥ ����
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

//���������Ⱥ Ӧ��
typedef struct XT_CLUSTER_INVITE_ACK
{
	uint8 ret;//Ӧ��Ϊ0
	XT_CLUSTER_INVITE_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_ACK));
	}
}XT_CLUSTER_INVITE_ACK, *LPXT_CLUSTER_INVITE_ACK;

//���������Ⱥ ת��
typedef struct XT_CLUSTER_INVITE_IND
{
	uint32     invite_id;//������ID
	char       clustername[MAX_FAMILYNAME_LEN+1];//Ⱥ����
	char       nickname[MAX_NICKNAME_LEN+1] ;//Ⱥ�ǳ�
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	XT_CLUSTER_INVITE_IND ()
	{
		memset(this,0,sizeof(XT_CLUSTER_INVITE_IND));
	}
} XT_CLUSTER_INVITE_IND, *LPXT_CLUSTER_INVITE_IND;

//�����Ƿ�ͬ����Ⱥ ����
typedef struct XT_CLUSTER_AGREE_REQ
{
	uint8      action;//0��ͬ�� 1���ܾ�
	uint32     invite_id;//������ID
	uint32     friend_id;//��������ID
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	XT_CLUSTER_AGREE_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_REQ));
	}
}XT_CLUSTER_AGREE_REQ, *LPXT_CLUSTER_AGREE_REQ;

//�����Ƿ�ͬ����Ⱥ Ӧ��
typedef struct XT_CLUSTER_AGREE_ACK
{
	uint8 ret;
	XT_CLUSTER_AGREE_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_ACK));
	}
}XT_CLUSTER_AGREE_ACK, *LPXT_CLUSTER_AGREE_ACK;

//�����Ƿ�ͬ����Ⱥ ת��
typedef struct XT_CLUSTER_AGREE_IND
{
	uint8      action;//0��ͬ�� 1���ܾ�
	uint32     invite_id;//������ID
	uint32     friend_id;//��������ID
	uint32     cluster_id   ;//ȺID
	uint8      cluster_type ;//Ⱥ����
	XT_CLUSTER_AGREE_IND()
	{
		memset(this, 0, sizeof(XT_CLUSTER_AGREE_IND));
	}
}XT_CLUSTER_AGREE_IND, *LPXT_CLUSTER_AGREE_IND;

///////////////////////////////////////////////
//�û��޸ĵ�¼��������
typedef struct XT_MOD_PSW_REQ
{
	uint32  uid;      ///�û�id
	char    szPsw[60];///���ݿ�洢����ܵ�����
} XT_MOD_PSW_REQ, *LPXT_MOD_PSW_REQ;

//�û��޸ĵ�¼�����������Ӧ
typedef struct XT_MOD_PSW_ACK
{
	uint8   ret;
	uint32  uid;    ///�û�id
	char    szPsw[60];///����
	XT_MOD_PSW_ACK()
	{
		memset(this,0,sizeof(XT_MOD_PSW_ACK));
	}
} XT_MOD_PSW_ACK, *LPXT_MOD_PSW_ACK;
/////////////////////////////////////////////

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
	uint16	face		 ;					// ͷ��ID
	char    face_url[MAX_HEADURL_LEN+1] ;	// ͷ��URL
	uint8   career       ;					// ְҵ
	uint8   rule         ;					// ��Ϊ���ѵ���֤��ʽ
	uint8   familyflag	 ;					// ��ΪȺ��Ա��֤��ʽ
	uint16  gold_money   ;					// �����Ŀ
	uint32  online_time  ;					// ����ʱ��
	uint8   linkpop		 ;					// ��ϵ��ʽ��˽���ԣ�0 ˽��;1 ���ѿɼ���3 ������
	char    address[MAX_ADDRESS_LEN+1]  ;	// ��ϵ��ַ
	char    mobile[MAX_PHONE_LEN+1]   ;		// �ֻ�
	char    description[MAX_DESCRIPTION_LEN+1]; //����˵��
	char    usersign[MAX_USERSIGN_LEN + 1];	//����ǩ��

	XT_SERVER_USERINFO_GET_ACK()
	{
		memset(this,0,sizeof(XT_SERVER_USERINFO_GET_ACK));
	}

} XT_SERVER_USERINFO_GET_ACK, *LPXT_SERVER_USERINFO_GET_ACK;



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
	uint32      merchant_id;     //�̼�id
	uint32		id			;
	uint16		start_pos	;
	uint8		nType;			 // ��ʶ����̨�汾

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
	uint8	nType;		// ��ʶ����̨�汾

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
	uint8		group_pid	;
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
	// �����޸ı��ṹʱ���°汾��
	const static uint32 VER = 0x00000001;

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

	uint32	 message_sequence_id;

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
		WIN_SHAKE = 6,		// ���ڶ���
		PUSH_EVALUA = 7,	// ��������
		SF_CFILE_REQ=8,
		MAGIC_FACE=9,
		USER_SIGN=10,
		FILE_REQ=11,
		MM_REQ=12,
		MM_REPLY=13,
		MM_CANCEL=14,
		MM_STOP=15,
		VEDIO_EQUIPMENT=16,
		EVALUA_ACK=17,			// �������۽��
		OPEN_P2PSENDMSG_DLG=18,	// �����촰��
		SF_IMG_URL = 19,
		CUST_SER_TRAN_REQ = 20,	// �ͷ�ת������
		AUTO_REPLY = 21,		// �Զ��Ӵ�
		DEVICE_REPLY = 22,		//���豸ģʽ�£��Լ������豸���͵���Ϣ
		IM_MSG_INSERT=26,       //����Ự
		IM_MSG_ORDER_SEQ=27,    //������
		MAX_TYPE=27,
	};

#ifdef _MOBILE_TEST_
	char 	uuid[MAX_UUID_LEN+1];		// ��Ϣ��Ψһ��ʾ	
#endif

	char	 data[1315]    ;
	int      data_len	   ;

	XT_MSG()
	{
		memset(this,0,sizeof(XT_MSG));
	}

} XT_MSG, *LPXT_MSG;

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
	uint8      action       ;// 0:���/���룻1��ɾ��/�˳���2����Ϊ����Ա��3��ȡ������Ա��4���������˾ܾ���Ⱥ�� 5��Ⱥ���ܾ�����������Ⱥ�� 6����ɢȺ
	uint32     member_id    ;
	char       clustername[MAX_FAMILYNAME_LEN+1];
	char       nickname[MAX_NICKNAME_LEN+1] ;
	uint32     cluster_id   ;
	uint8      cluster_type ;
	uint32     invite_id    ;//��Ա��������ID

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
	char		info[120+1]	;//��֤��Ϣ

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
	uint8		count;			// Ҫ�󷵻���Ŀ��0������
	uint32		id;				// ��ID����
	char 		user_name[MAX_USERNAME_LEN];
	char		nick_name[MAX_NICKNAME_LEN];
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
		uint16	HeadId;
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
		uint8        nMemCount;////����Ⱥ��Ա����				;	
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
	uint16		nFamilyClass								;   //���ͬ�£�ͬѧ
	uint8		nFamilyFaceID										;
	char		szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1]	;	//������ӵ�ַ
	uint8		nViewFlag									;	//FAMILY_VIEW_MEM	Ⱥ��Ա��˽�У�FAMILY_VIEW_REG	ע���û����빫����FAMILY_VIEW_ALL ���У�������

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
	uint32     nInviteID         ;//��Ҫ�����ĳ�Ա��������
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
	uint32     nInviteID               ;//��Ա��������ID
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

typedef XT_CLUSTER_INFO  XT_SET_CINFO_REQ, *LPXT_SET_CINFO_REQ;

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


//typedef struct ST_OFFLINE_MSG
//{	
//	uint32	    recvID   ;
//	uint32		sendID   ;
//	uint32		tm;
//	uint16		msgtype       ;  
//	char        msg[MAX_MSG_LEN+1];
//	uint16		msglen;
//	uint32		msgid;
//
//	ST_OFFLINE_MSG()
//	{
//		memset(this,0,sizeof(ST_OFFLINE_MSG));
//	}
//
//} ST_OFFLINE_MSG, *LPST_OFFLINE_MSG;

//type	1 Bytes ��Ϣ����
//0.�û�������Ϣ 1.�ռ��䣬2.ϵͳ����,3.�û�����,4.������ҳ���� 5.������������
//recID	4 Bytes��ˮ�š������޸����ݿⷢ�ͱ�־��
//recvID	4 Bytes  ������ID
//title	Max 200 Bytes��text����Ϣ����
//content	Max 900 Bytes��text����Ϣ����
//classID	2 Bytes �������ͣ�
//1=������־,2=�ϴ�ͼƬ,3=�ϴ�����,4=�ϴ���Ƶ,5=�ϴ������ļ�
//srcuri	Max 500 Bytes, test ��������Ŀ������
//sendID	4 Bytes ������ID
//nickname	Max 32 Bytes �������ǳ�
//sendTime	Max 32 Bytes text ����ʱ��

typedef struct  XT_WEB_MSG
{	
	uint8       msg_type			;//0.�û�������Ϣ 1.�ռ��䣬2.ϵͳ����,3.�û�����,4.������ҳ���� 5.������������
	
	enum
	{
		USER_UPDATE  =0,
		USER_MSG_BOX =1,
		SYS_MSG      =2,
		FRIEND_MSG   =3,
		MAINPAGE_COMMENT =4,
        CONTENT_COMMENT  =5,
	};

	uint32		msg_id				;//��Ϣ��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
	uint32	    recv_id				;//������
	char        title[200]			;
	char        content[900]		;
	uint8		sub_type    ;//�û��������� 1=������־,2=�ϴ�ͼƬ,3=�ϴ�����,4=�ϴ���Ƶ,5=�ϴ������ļ�
							 //�������ͣ�0�������ۣ�1ͼƬ���ۣ�2������ۡ�3��Ƶ���ۣ�4��Ƶ����
							 //�ռ��� 0վ����Ϣ 2--��Ϊ���� 3--����Ⱥ����  4--�ͷ���Ϣ	5--�����Ϊ����
	uint32		log_id		;//��������ID  

	enum 
	{
		BLOG  =1,
		PIC   =2,
		MUSIC =3,
		VEDIO =4,
		FILE  =5,
	};

	char		src_url[255]		;
	uint32		sender_id			;//������id
	char		sender_nickname[MAX_NICKNAME_LEN+1] ;//�������ǳ�
	char        sender_username[MAX_USERNAME_LEN+1] ;//������username
	char        publish_time[32]	;

	XT_WEB_MSG()
	{
		memset(this,0,sizeof(XT_WEB_MSG));
	}

} XT_WEB_MSG, *LPXT_WEB_MSG;


typedef struct XT_WEB_ACK
{
	uint32      msg_id;

	XT_WEB_ACK()
	{
		memset(this,0,sizeof(XT_WEB_ACK));
	}

} XT_WEB_ACK, *LPXT_WEB_ACK;


//�����������·�
typedef struct XT_SMS_IND
{
	uint32			msg_id		;//������������ˮ��
	uint32			sender_id	;//������id
	char            send_time[32];
	char			user_name[25];
	char			nick_name[25];
	char			link_id[25];
	uint16			biz_type; //ҵ������ 400-499 Mini
	uint8			msg_type    ;//�������ͣ� 0-��ͨ���� 1-wap push
	char			message[255];//

	XT_SMS_IND()
	{
		memset(this,0,sizeof(XT_SMS_IND));
	}

} XT_SMS_IND, *LPXT_SMS_IND;


typedef struct XT_SMS_REQ
{
	uint32			recv_id;     //���շ�id
	char			mobile[16];  //���շ��ֻ�����
	char			link_id[25];
	uint16			biz_type; //ҵ������ 400-499 Mini
	uint32			biz_id  ;
	uint8			msg_type    ;//�������ͣ� 0-��ͨ���� 1-wap push
	char			message[255];

	XT_SMS_REQ()
	{
		memset(this,0,sizeof(XT_SMS_REQ));
	}

} XT_SMS_REQ, *LPXT_SMS_REQ;


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

//Ⱥ��Ƭ
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

//////////////////////////////////////////////////////////////////////////////
/////Ⱥ��Ƭ��ȡ����
typedef struct XT_CMEM_IFNO
{
	uint32	   nMemID;      // ��ǰȺ�ĳ�ԱID
	char       ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1]; // Ⱥ��Ƭ
}XT_CMEM_IFNO;


typedef struct XT_PROFILENAME_GET_REQ
{
	uint32		nFamilyID;			// ��ǰȺid
	XT_PROFILENAME_GET_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_REQ));
	}

}XT_PROFILENAME_GET_REQ, *LPXT_PROFILENAME_GET_REQ;


////Ⱥ��Ƭ��ȡӦ��
typedef struct XT_PROFILENAME_GET_ACK
{
	uint8         ret;
	uint32		  nFamilyID;		  // ��ǰȺid
	uint32        nCount;             // ��ǰȺ��Ա��
    XT_CMEM_IFNO  MemInfo[1];         // ��ǰȺ��Ա��Ⱥ��Ƭ��
	XT_PROFILENAME_GET_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_ACK));
	}
}XT_PROFILENAME_GET_ACK, *LPXT_PROFILENAME_GET_ACK;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//Ⱥ��Ϣ��������
typedef struct XT_CLUSTERMSG_RECV_SET_REQ
{
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			    // ��ǰȺ�ĳ�ԱID
	uint8		MsgRecType;         // Ⱥ��Ϣ���շ�ʽ��RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4;
	XT_CLUSTERMSG_RECV_SET_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTERMSG_RECV_SET_REQ));
	}

}XT_CLUSTERMSG_RECV_SET_REQ, *LPXT_CLUSTERMSG_RECV_SET_REQ;

typedef struct XT_CLUSTERMSG_RECV_SET_ACK
{
	uint8       ret;
	uint32		nFamilyID;			// ��ǰȺid
	uint32		nMemID;			    // ��ǰȺ�ĳ�ԱID
    uint8		MsgRecType;         // Ⱥ��Ϣ���շ�ʽ��RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4;
	XT_CLUSTERMSG_RECV_SET_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTERMSG_RECV_SET_ACK));
	}
}XT_CLUSTERMSG_RECV_SET_ACK, *LPXT_CLUSTERMSG_RECV_SET_ACK;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

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
	uint32		ip;			// Ӧ��ip
	uint16		port;		// Ӧ��port

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
	uint8 ret;
}XT_FRIENDINVITE_ACK, *LPXT_FRIENDINVITE_ACK;


typedef struct _P2P_ENDPOINT
{
	enum ENDPOINT_TYPE{
		P2P = 0,
		BUSI,	
		TRANS,	
	};
	uint32		uid;
	uint32		ip;		// �����ֽ�
	uint16		port;	// �����ֽ�
	uint8		type;	// ip����

}P2P_ENDPOINT,*LPP2P_ENDPOINT;


typedef struct _P2P_SENDFILE_REQ
{
	uint32			fid;
	union SIZE{
		struct  
		{
			uint32		sizel;
			uint32		sizeh;
		};
		ULONGLONG	size64;
	}				size;
	uint64			time;
	uint32			friendId;
	//uint32			ip;
	//uint32			port;
	uint32			tranIp;
	uint16			tranPort;
	char			name[256];

}P2P_SENDFILE_REQ,*LPP2P_SENDFILE_REQ;


typedef struct _P2P_SENDFILE_ACK
{
	uint8		nRt;
}P2P_SENDFILE_ACK,*LPP2P_SENDFILE_ACK;


typedef struct _P2P_DOWNLOAD_FILE_REQ
{
	enum FLAG{
		ACCEPT,
		DENEY
	};
	uint32			fid;		// �ļ�ID
	uint8			flag;		// ���
	uint32			startPos;	// ��ʼλ��
	uint32			packetSize;	// �ְ���С
	uint32			friendId;
}P2P_DOWNLOAD_FILE_REQ,*LPP2P_DOWNLOAD_FILE_REQ;

typedef struct _P2P_DOWNLOAD_FILE_ACK
{
	uint8			nRt;
	uint32			nFID;
	char			szErr[40];
	uint32			nFriendId;
	//P2P_ENDPOINT	pt;
}P2P_DOWNLOAD_FILE_ACK,*LPP2P_DOWNLOAD_FILE_ACK;


typedef struct _P2P_DOWNLOAD_REQ
{
	uint32			fid;
	uint32			packetID;
	// 
	P2P_ENDPOINT	pt;
	uint32			seq;
	uint32			rtt;
}P2P_DOWNLOAD_REQ,*LPP2P_DOWNLOAD_REQ;

typedef struct _P2P_DOWNLOAD_ACK
{
	uint8		nRt;
	uint32		fid;
	uint32		packetID;
	uint32		len;
	char		data[1500];
	//
	P2P_ENDPOINT	pt;
	uint32			seq;
}P2P_DOWNLOAD_ACK,*LPP2P_DOWNLOAD_ACK;

typedef struct _P2P_FIN_TRAN_REQ
{
	uint32			fid;
	//P2P_ENDPOINT	pt;
	uint32			friendId;
}P2P_FIN_TRAN_REQ,*LPP2P_FIN_TRAN_REQ;

typedef struct _P2P_FINISH_ACK
{
	uint8		nRt;
}P2P_FINISH_ACK,*LPP2P_FINISH_ACK;

typedef struct _P2P_ABORT_TRAN_REQ
{
	enum ABORT_TYPE{ 
		STOP_RECV = 1,
		STOP_SEND = 2,
	};
	uint8			type;	// ��ֹ���ͣ�ABORT_TYPE��
	uint32			fid;	// �ļ�ID
	uint32			friendId;
	//P2P_ENDPOINT	pt;		// P2P�˵�
}P2P_ABORT_TRAN_REQ,*LPP2P_ABORT_TRAN_REQ;

typedef struct _P2P_ABORT_TRAN_ACK
{
	uint8	nRt;
}P2P_ABORT_TRAN_ACK,*LPP2P_ABORT_TRAN_ACK;



typedef struct XT_TRANS_LOGIN_REQ
{
	uint8    data            ;//0 
	XT_TRANS_LOGIN_REQ()
	{
		memset(this,0,sizeof(XT_TRANS_LOGIN_REQ));
	}

} XT_TRANS_LOGIN_REQ, * LPXT_TRANS_LOGIN_REQ;

//��¼��Ӧ
typedef struct _XT_TRANS_LOGIN_ACK
{
	uint8	ret				;// 0 ����ֵ
	uint32	sessionid		;// SessionID
	char	sessioncode[16]	;// SessionCode

	uint32	ip;
	uint16	port;
	_XT_TRANS_LOGIN_ACK()
	{
		memset(this,0,sizeof(_XT_TRANS_LOGIN_ACK));
	}
} XT_TRANS_LOGIN_ACK, * LPXT_TRANS_LOGIN_ACK;

// �����߹رջỰ
typedef struct _XT_TRANS_CLOSE_SEESION_REQ
{
	uint8	count;
	uint32	sessionID[300];
}XT_TRANS_CLOSE_SEESION_REQ,*LPXT_TRANS_CLOSE_SEESION_REQ;

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

// �����ߵ�¼��ת����������
typedef struct XT_TRANS_RECVLOGIN_REQ
{
	uint32		sessionid;
	uint8		action;
	enum {RECV_LOGIN = 0x01, RECV_LOGOUT = 0x02};
	char		sessioncode[16]	;//SessionCode

	uint32		ip;
	uint32		port;
	uint32		uid;	// ������A��id
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

	uint32		ip;
	uint16		port;
	XT_TRANS_RECVLOGIN_ACK()
	{
		memset(this, 0, sizeof(XT_TRANS_RECVLOGIN_ACK));
	}
}XT_TRANS_RECVLOGIN_ACK, *LPXT_TRANS_RECVLOGIN_ACK;

// ֪ͨ������B��¼��ת������
typedef struct _XT_TRANS_NOTIFY_LOGIN_REQ
{
	uint32		sessionID;	// �ỰID
	uint8		linkType;	// 0 = udp;1 = tcp
	uint32		transSrvIP;	// ��ת������IP	
	uint16		port;		// ��ת�������˿�	
	uint8		check[16];	// ��֤��
	
	uint32		uid;		// ������B��id
	uint32		sid;		// ������A��id
}XT_TRANS_NOTIFY_LOGIN_REQ,*LPXT_TRANS_NOTIFY_LOGIN_REQ;


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




// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
typedef struct XT_GET_NEAR_COMM_CUST_REQ
{
	uint32  uiBusinessID;		// �̻�ID	
	char szBgnTime[32];			// ��ʼʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szEndTime[32];			// ����ʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	uint32 iMaxCustCount;		// ��ȡ������û�����Ĭ��ֵΪ1000����ָ��ʱ������û���������ֵʱ��ֻ��ȡ�������������ͻ�	
	uint16 iStartPos;			// �����Ϣ��ʼλ�ã����Ϊ999��ֻ����ͻ��˷��ؿͻ�ʵ��������
	uint16 iNeedCustCount;		// Ҫ��ȡ���������
	uint8 bDirect;				// �����Ϣ����ʽ��0��ʾ�������ϵʱ��Ӵ�С����1��ʾ�������ϵʱ���С��������

	XT_GET_NEAR_COMM_CUST_REQ()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REQ));
		iMaxCustCount = 1000;
	}
}XT_GET_NEAR_COMM_CUST_REQ, *LPXT_GET_NEAR_COMM_CUST_REQ;

// ��ȡ�̻������ϵ�Ŀͻ���Ϣ(������Ϣ)
typedef struct XT_GET_NEAR_COMM_CUST_ACK
{
	uint32  ret;						// �������
	uint32  uiBusinessID;				// �̻�ID

	uint8   bMask;						// ֻ��Ϊ����ö��
	enum
	{
		MK_ALL_CUST_COUNT = 0x00,		// ��ʾactualCount���ص��������ϵ�ͻ�����Ŀ����XT_GET_NEAR_COMM_CUST_REQ.iStartPos == 999ʱ��
		MK_CUR_CUST_COUNT = 0x01,		// ��ʾactualCount����CommInfo����Ĵ�С
	};

	uint32  actualCount;				

	struct TagCommInfo
	{
		char szCommAccount[32];			// �ͻ��˺�
		char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
		char szTransInfo[64];			// ��ʾ��ϵ�����һ�ݽ��׶����š����û�н��׶���������ʾ�հ�
		char szRemarks[128];			// ��ʾ�����촰�����ҶԿͻ��ı�ע��Ϣ
	}CommInfo[1];

	XT_GET_NEAR_COMM_CUST_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_ACK));
	}
}XT_GET_NEAR_COMM_CUST_ACK, *LPXT_GET_NEAR_COMM_CUST_ACK;

typedef XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo XT_COMMINFO_GET;





// ���������ϵ�ͻ���Ϣ����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_REQ
{
	uint32  uiBusinessID;				// �̻�ID	
	uint32	uiCommCount;					// �ͻ�����
	struct TagCommInfo
	{
		char szCommAccount[32];			// �ͻ��˺�
		char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	}CommInfo[1];

	XT_UPDATE_NEAR_COMM_CUST_INFO_REQ()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_REQ));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_REQ, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_REQ; 

typedef XT_UPDATE_NEAR_COMM_CUST_INFO_REQ::TagCommInfo XT_COMMINFO_UPDATE;

// ���������ϵ�ͻ���Ϣ����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_ACK
{
	uint8		ret;					// ���������ص���Ϣ����ʶ�ɹ����

	XT_UPDATE_NEAR_COMM_CUST_INFO_ACK()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_ACK));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_ACK, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_ACK;


#ifdef _BUSINESS_VESION

// ���������ϵ�ͻ���ע����̨��������Ϣ��
typedef struct XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ
{
	uint32  uiBusinessID;			// �̻�ID	
	char szCommAccount[32];			// �ͻ��˺�
	char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szRemarks[201];			//�Ҳ�ҳ��ע

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
	uint8	ret;				// ���������ص���Ϣ����ʶ�ɹ����
	uint32  uiBusinessID;			// �̻�ID	
	char szCommAccount[32];			// �ͻ��˺�
	char szCommTime[32];			// �����ϵʱ�䣬��ʽΪYY-MM-DD hh:mm:ss
	char szRemarks[128];

	XT_GET_NEAR_COMM_CUST_REMARK_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REMARK_ACK));
	}
}XT_GET_NEAR_COMM_CUST_REMARK_ACK, *LPXT_GET_NEAR_COMM_CUST_REMARK_ACK;

#endif

// ϵͳ��Ϣ�����º�����Ϣ
typedef struct XT_INFO_IND
{
	uint32		fid;	//����id

	XT_INFO_IND()
	{
		memset(this,0,sizeof(XT_INFO_IND));
	}

} XT_INFO_IND, * LPXT_INFO_IND;


//�ϴ�ͷ���ļ�Ӧ��Ľṹ��
typedef struct XT_HEAD_FILE_UPLOAD_ACK
{
	uint8    ret;
	uint8    type;//1:�ϴ��ļ� 2:�ϴ��ļ���
	uint8    uploadFinish;//0:δ�ϴ��� 1:���ϴ���
	char    face_url[MAX_HEADURL_LEN+1] ;//ͷ��url
	XT_HEAD_FILE_UPLOAD_ACK()
	{
		memset(this,0,sizeof(XT_HEAD_FILE_UPLOAD_ACK));
	}

} XT_HEAD_FILE_UPLOAD_ACK, *LPXT_HEAD_FILE_UPLOAD_ACK;

// ��Ҷ����ҵ����۵ȼ�
enum EN_EVALUA_GRADE
{
	eIgnore = 0,					// ���ԣ�δ����
	eVerySatisfied = 5,				// �ǳ�����
	eSatisfied = 4,					// ����
	eGeneral = 3,					// һ��
	eDissatisfied = 2,				// ������	
	eVeryDissatisfied = 1,			// �ǳ�������
};

#ifdef _BUSINESS_VESION
// �޸Ĺ���״̬
//#define CMD_MOD_HANGUP_STATE_REQ	0x0A11
//#define CMD_MOD_HANGUP_STATE_ACK	0x0A12
typedef struct XT_MOD_HANGUP_STATE_REQ
{
	uint32 uUserId;					// �û�Id
	uint8 uHangupState;				// ����״̬��0--������1--����
	XT_MOD_HANGUP_STATE_REQ()
	{
		memset(this,0,sizeof(XT_MOD_HANGUP_STATE_REQ));
	}

} XT_MOD_HANGUP_STATE_REQ, * LPXT_MOD_HANGUP_STATE_REQ;
#endif

#ifdef _VER_INTERNAL_	// �ڲ��棬��ȡԱ����Ϣ
//#define CMD_GET_EMPLOYEE_INFO_REQ	0x0A13
//#define CMD_GET_EMPLOYEE_INFO_ACK	0x0A14
// ����
typedef struct XT_GET_EMPLOYEE_INFO_REQ
{
	uint32 uUserId;					// �û�Id
	XT_GET_EMPLOYEE_INFO_REQ()
	{
		memset(this,0,sizeof(XT_GET_EMPLOYEE_INFO_REQ));
	}
} XT_GET_EMPLOYEE_INFO_REQ, * LPXT_GET_EMPLOYEE_INFO_REQ;

// Ӧ��
typedef struct XT_GET_EMPLOYEE_INFO_ACK
{
	uint8		ret;								// ���������ص���Ϣ����ʶ�ɹ����
	uint32		uUserId;							// �û�Id
	char		username[MAX_USERNAME_LEN+1] ;		// �û���
	char		job[MAX_JOB_LENGTH+1];				// ְ��
	char		dept[MAX_DEPT_LENGTH+1];			// ����
	char		extension[MAX_PHONE_LEN+1];			// �ֻ���
	char		mobile[MAX_PHONE_LEN+1];			// �ֻ�

	XT_GET_EMPLOYEE_INFO_ACK()
	{
		memset(this,0,sizeof(XT_GET_EMPLOYEE_INFO_ACK));
	}
} XT_GET_EMPLOYEE_INFO_ACK, * LPXT_GET_EMPLOYEE_INFO_ACK;
#endif

//#define CMD_CUSTSER_TRAN_REQ	0x801B		// �ͻ�ת�ӷ��ͷ�����ת���������������������ֱ�Ӱ�������Ϣת�����շ�
//#define CMD_CUSTSER_TRAN_ACK	0x801C		// �ͷ�ת�ӽ��շ��ӵ�ת������󣬷���ȷ�ϰ�����������������ֱ��ת�������ͷ�
// ����
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

//#ifndef _MINI_CLIENT_
#pragma pack(pop)
//#endif

//�ͷ���ǰ�������
typedef struct XT_MONITOR_SET_BUYER_REQ //�ͷ����õ�ǰ��������
{
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//�ͷ�������������
	
	XT_MONITOR_SET_BUYER_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_SET_BUYER_REQ));
	}
}XT_MONITOR_SET_BUYER_REQ, *LPXT_MONITOR_SET_BUYER_REQ;

typedef struct XT_SET_BUYER_ACK //�ͷ����õ�ǰ��������Ӧ��
{
	uint8      ret;
	XT_SET_BUYER_ACK()
	{
		memset(this, 0, sizeof(XT_SET_BUYER_ACK));
	}
}XT_SET_BUYER_ACK, *LPXT_SET_BUYER_ACK;

typedef struct XT_MONITOR_SET_BUYER_IND //���ܻ�ȡ�ͷ���ǰ�������
{
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//�ͷ�������������
	XT_MONITOR_SET_BUYER_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_SET_BUYER_IND));
	}
}XT_MONITOR_SET_BUYER_IND, *LPXT_MONITOR_SET_BUYER_IND;

//���
typedef struct XT_MONITOR_START_REQ //���ܼ�ؿͷ�
{
	uint32     idMaster;//����ID
	uint32     idSeller;//�ͷ�ID
	XT_MONITOR_START_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_REQ));
	}
}XT_MONITOR_START_REQ, *LPXT_MONITOR_START_REQ;

typedef struct XT_MONITOR_START_ACK //���ܻ�ȡ�ͷ������нӴ����
{
	uint8      ret;
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//�ͷ���ǰ�������
	XT_MONITOR_START_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_ACK));
	}
}XT_MONITOR_START_ACK, *LPXT_MONITOR_START_ACK;

typedef struct XT_MONITOR_START_IND //���ܻ�ȡ�ͷ��Ķ���������
{
	uint8      action;//0.�ͷ�������Ϣ��1.�ͷ��յ���Ϣ��
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//���������
	XT_MSG     msg;//�Ự��Ϣ
	XT_MONITOR_START_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_IND));
	}
}XT_MONITOR_START_IND, *LPXT_MONITOR_START_IND;

//ȡ�����
typedef struct XT_MONITOR_END_REQ //����ȡ����ؿͷ�
{
	uint32     idMaster;
	char       nicknameMaster[MAX_NICKNAME_LEN+1] ;//�����ǳ�
	uint32     idSeller;//�ͷ�ID
	XT_MONITOR_END_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_END_REQ));
	}
}XT_MONITOR_END_REQ, *LPXT_MONITOR_END_REQ;

typedef struct XT_MONITOR_END_ACK //����ȡ����ؿͷ�Ӧ��
{
	uint8      ret;
	XT_MONITOR_END_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_END_ACK));
	}
}XT_MONITOR_END_ACK, *LPXT_MONITOR_END_ACK;

//����Ự
typedef struct XT_MONITOR_INSERT_CHAT_REQ //���ܲ���Ự
{
	uint32     idMaster;//����ID
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//���ID
	XT_MONITOR_INSERT_CHAT_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_REQ));
	}
}XT_MONITOR_INSERT_CHAT_REQ, *LPXT_MONITOR_INSERT_CHAT_REQ;

typedef struct XT_MONITOR_INSERT_CHAT_ACK //���ܲ���ỰӦ��
{
	uint8      ret;
	XT_MONITOR_INSERT_CHAT_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_ACK));
	}
}XT_MONITOR_INSERT_CHAT_ACK, *LPXT_MONITOR_INSERT_CHAT_ACK;

typedef struct XT_MONITOR_INSERT_CHAT_IND //�ͷ�����Ҷ�Ҫ��ȡ���ܲ���ĻỰ
{
	uint32     idMaster;//����ID
	//char       usernameMaster[MAX_USERNAME_LEN+1] ;//�����ǳ�
	uint8      action;//0.��������Ự 1.����Ự
	XT_MSG     msg;//�Ự��Ϣ
	XT_MONITOR_INSERT_CHAT_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_IND));
	}
}XT_MONITOR_INSERT_CHAT_IND, *LPXT_MONITOR_INSERT_CHAT_IND;

//���ֻỰ
typedef struct XT_MONITOR_TRAN_CHAT_REQ //���ܽ��ֻỰ
{
	uint32     idMaster;//����ID
	uint32     idSeller;//�ͷ�ID
	uint32     idBuyer;//���ID
	XT_MONITOR_TRAN_CHAT_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_REQ));
	}
}XT_MONITOR_TRAN_CHAT_REQ, *LPXT_MONITOR_TRAN_CHAT_REQ;

typedef struct XT_MONITOR_TRAN_CHAT_ACK //���ܽ��ֻỰӦ��
{
	uint8      ret;
	XT_MONITOR_TRAN_CHAT_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_ACK));
	}
}XT_MONITOR_TRAN_CHAT_ACK, *LPXT_MONITOR_TRAN_CHAT_ACK;

typedef struct XT_MONITOR_TRAN_CHAT_IND //�ͷ�������յ����ܽ��ջỰ
{
	uint32     idMaster;//����ID
	XT_MONITOR_TRAN_CHAT_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_IND));
	}
}XT_MONITOR_TRAN_CHAT_IND, *LPXT_MONITOR_TRAN_CHAT_IND;

// �Ŷ����Э�飨��������͸��ͻ��˵ģ�����ϵͳ��ϢCMD_SERVERMSG_IND 0x60d֮�£�
//#define CMD_WAITQUEU_CHANGE		0x810F		// �ȴ��������ӳɹ�����ٳ�Ա
typedef struct XT_WAITQUEU_CHANGE
{
	uint8	bAdd;		// 0�����ӳ�Ա	1�����ٳɹ�
	uint32	idBuyer;	// ���ID	
	uint32 	teamId;		// ������ID
//#if !defined(_VER_PREVIEW_) || !defined(_VER_ONLINE_)
	uint32  QueueLength;
//#endif
	char    nicknameBuyer[MAX_NICKNAME_LEN+1] ;// ����ǳ�

	enum { TYPE_ADD = 0,TYPE_DELETE,TYPE_QUEUE};

	XT_WAITQUEU_CHANGE()
	{
		memset(this, 0, sizeof(XT_WAITQUEU_CHANGE));
	}
}XT_WAITQUEU_CHANGE, *LPXT_WAITQUEU_CHANGE;

// �Ŷ����Э�飨�ͻ��˷��͸�����˵�Э�飩
//#define CMD_RECEPTE_CUST_REQ	0x8110		// �Ӵ��ͻ�����
typedef struct XT_RECEPTE_CUST_REQ
{
	uint8	type;		// ��������  0�������Ӵ� 1���˳��Ӵ�
	uint32 	 teamId;	//������ID
	uint32	idBuyer;	// ���ID	

	enum { TYPE_OFFER = 0,TYPE_QUIT,TYPE_AUTO_OFFER};

	XT_RECEPTE_CUST_REQ()
	{
		memset(this, 0, sizeof(XT_RECEPTE_CUST_REQ));
	}
}XT_RECEPTE_CUST_REQ, *LPXT_RECEPTE_CUST_REQ;
//#define CMD_RECEPTE_CUST_ACK	0x8111		// �Ӵ��ͻ�Ӧ��
typedef struct XT_RECEPTE_CUST_ACK
{
	uint8	type;
	uint32	userid;
	uint32	teamid;
	
	enum { TYPE_OFFER = 0,TYPE_QUIT,TYPE_AUTO_OFFER};

	XT_RECEPTE_CUST_ACK()
	{
		memset(this, 0, sizeof(XT_RECEPTE_CUST_ACK));
	}
}XT_RECEPTE_CUST_ACK, *LPXT_RECEPTE_CUST_ACK;

/////��ȡ�û�CSID����
typedef struct XT_USER_MAP_CSID_REQ
{
	uint32     userid;
	XT_USER_MAP_CSID_REQ()
	{
		memset(this, 0, sizeof(XT_USER_MAP_CSID_REQ));
	}
}XT_USER_MAP_CSID_REQ, *LPXT_USER_MAP_CSID_REQ;

/////��ȡ�û�CSID��Ӧ
typedef struct XT_USER_MAP_CSID_ACK
{
	uint8      ret;
	uint32     csid;
	XT_USER_MAP_CSID_ACK()
	{
		memset(this, 0, sizeof(XT_USER_MAP_CSID_ACK));
	}
}XT_USER_MAP_CSID_ACK, *LPXT_USER_MAP_CSID_ACK;

typedef struct XT_GET_USER_STATUS_REQ
{
	uint32	cnt;				//��Ҫ��ѯ������
	uint32	id[1];				//������Ҫ��ѯ�����˺�

	XT_GET_USER_STATUS_REQ()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_REQ));
	}
}XT_GET_USER_STATUS_REQ, *LPXT_GET_USER_STATUS_REQ;

typedef struct XT_GET_USER_STATUS_ACK
{
	uint32	cnt;				//����

	struct
	{
		uint32	id;
		uint32	b;				//�Ƿ����������˺�
	}stat[1];

	XT_GET_USER_STATUS_ACK()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_ACK));
	}

}XT_GET_USER_STATUS_ACK, *LPXT_GET_USER_STATUS_ACK;

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
