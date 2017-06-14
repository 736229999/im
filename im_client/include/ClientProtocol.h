/************************************************************************ 
 文件描述：客户端协议定义，定义服务器与客户端之间的交互协议
		   
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/
#pragma once

#include "protocol.h"
#include "publictype.h"
#include "..\Mini_Client\MINI139_SRC\MemCrypt.h"

//#ifndef _MINI_CLIENT_
#pragma pack(push,1)
//#endif

#define CPIC_PACKET_LEN	1352

//协议定义 ...

//应答ACK,无需返回结果的应答ack
typedef struct  tagRESPONSE_ACK
{
	uint8 ret; // 总是0
}RESPONSE_ACK, * LPRESPONSE_ACK;

//所有的错误结果返回协议
typedef struct  tagERROR_RET_ACK
{
	uint8 ret;
	char error[40];
}ERROR_RET_ACK, * LPERROR_RET_ACK;


//IM 用户信息
typedef struct XT_USER_INFO
{
private:
	uint32  id           ;	// 0 用户id
	char    username[MAX_USERNAME_LEN+1] ;	// 3 用户名
public:
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 电子邮件
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
private:
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 密码
public:
	char	comment_name[MAX_COMMENTNAME_LEN+1];// 新增好友备注名称

	char    face_url[MAX_HEADURL_LEN+1] ;
	char    flash_url[MAX_HEADURL_LEN+1] ;
	uint8   bind_mobile  ;//手机邦定
	uint8   sex          ;
	uint8   age			 ;//4 年龄
	//---为了避免新增字段，现使用下面三个字段表示地址（省，市，县区）---孙丁 2015/1/19
	uint8   province	 ;//5 省			（省）
	uint8   country		 ;//6 国家			（县区）	
	uint8   city	     ;//7 城市			（市）
	//--------------------------------------------------------------------------------
	uint8   career       ;//9 职业
	uint8   rule         ;//10 加为好友的认证方式
	uint8	familyflag	 ;// 列为群成员认证方式    0：允许     1：拒绝
	uint8   linkpop		 ;//联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char    address[MAX_ADDRESS_LEN+1]  ;//11 联系地址
	char    mobile[MAX_PHONE_LEN+1]   ;//12 手机
	float   gold_money   ;//金币数目
	uint32  jifen		 ;//积分	/*新定义：标记是否为主管*/
	uint16  message_count;//消息数
	uint32  online_time  ;//在线时长

	char    user_sign[MAX_USERSIGN_LEN+1]   ;//个性签名
	char    description[MAX_DESCRIPTION_LEN+1]; //13 个人说明
	char    profile_name[MAX_CLUSTER_MEMBER_PROFILE + 1]; //14好友群名片
private:
	char	szGUID[MAX_GUID_LENGTH + 1];	// 自营GUID(MAX_GUID_LENGTH = 40）

public:
	XT_USER_INFO()
	{
		memset(this,0,sizeof(XT_USER_INFO));
		sex	 = 3;
	}

public:
	// 设置ID
	void SetId(uint32 uId)
	{
		this->id = EncryptUint32(uId);
	}
	// 得到ID
	uint32 GetId()
	{
		return DecryptUint32(this->id);
	}

	// 设置用户名
	void SetUserName(string strUserName)
	{
		EncryptStr(strUserName, this->username, sizeof(this->username));
	}
	// 得到用户名
	string GetUserName()
	{
		return DecryptStr(this->username, sizeof(this->username));
	}

	// 得到文件名
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

	// 设置密码
	void SetPassword(string strPassword)
	{
		EncryptStr(strPassword, this->password, sizeof(this->password));
	}
	// 得到密码
	string GetPassword()
	{
		return DecryptStr(this->password, sizeof(this->password));
	}

	// 设置手机号
	void SetMobile(string strMobile)
	{
		EncryptStr(strMobile, this->mobile, sizeof(this->mobile));
	}
	// 得到手机号
	string GetMobile()
	{
		return DecryptStr(this->mobile, sizeof(this->mobile));
	}

	// 设置Guid
	void SetGuid(string strGuid)
	{
		EncryptStr(strGuid, this->szGUID, sizeof(this->szGUID));
	}
	// 得到Guid
	string GetGuid()
	{
		return DecryptStr(this->szGUID, sizeof(this->szGUID));
	}

	// 自加密（只是为了兼容以前的版本，再添加此函数，尽量少调用）
	void SelfEncrypt()
	{
		SetId(this->id);
		SetUserName(this->username);
		SetPassword(this->password);
		SetGuid(this->szGUID);
	}
	// 自解密（只是为了兼容以前的版本，再添加此函数，尽量少调用）
	void SelfDecrypt()
	{
		id = GetId();
		strcpy(this->username, GetUserName().c_str());
		strcpy(this->password, GetPassword().c_str());
		strcpy(this->szGUID, GetGuid().c_str());
	}
} XT_USER_INFO, * LPXT_USER_INFO;//IM通讯协议协议命令字定义,结构定义


//老的用户信息，现定义这个结构体是为了兼容老版本
typedef struct XT_USER_INFO_OLD
{
	uint32  id           ;	// 0 用户id
	char    username[MAX_USERNAME_LEN_LAST+1] ;	// 3 用户名
	char    nickname[MAX_NICKNAME_LEN+1] ;
	char    email[MAX_EMAIL_LEN+1]	  ;	// 1 电子邮件
	char    birthday[MAX_BIRTHDAY_LEN+1] ;
	char    password[MAX_PASSWORD_LEN+1] ;	// 2 密码
	char	comment_name[MAX_COMMENTNAME_LEN+1];// 新增好友备注名称

	char    face_url[MAX_HEADURL_LEN+1] ;
	char    flash_url[MAX_HEADURL_LEN+1] ;
	uint8   bind_mobile  ;//手机邦定
	uint8   sex          ;
	uint8   age			 ;//4 年龄
	//---为了避免新增字段，现使用下面三个字段表示地址（省，市，县区）---孙丁 2015/1/19
	uint8   province	 ;//5 省			（省）
	uint8   country		 ;//6 国家			（县区）	
	uint8   city	     ;//7 城市			（市）
	//--------------------------------------------------------------------------------
	uint8   career       ;//9 职业
	uint8   rule         ;//10 加为好友的认证方式
	uint8	familyflag	 ;// 列为群成员认证方式    0：允许     1：拒绝
	uint8   linkpop		 ;//联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char    address[MAX_ADDRESS_LEN+1]  ;//11 联系地址
	char    mobile[MAX_PHONE_LEN+1]   ;//12 手机
	float   gold_money   ;//金币数目
	uint32  jifen		 ;//积分
	uint16  message_count;//消息数
	uint32  online_time  ;//在线时长

	char    user_sign[MAX_USERSIGN_LEN+1]   ;//个性签名
	char    description[MAX_DESCRIPTION_LEN+1]; //13 个人说明
	char    profile_name[MAX_CLUSTER_MEMBER_PROFILE + 1]; //14好友群名片
	char	szGUID[MAX_GUID_LENGTH + 1];	// 自营GUID(MAX_GUID_LENGTH = 40）

	XT_USER_INFO_OLD()
	{
		memset(this,0,sizeof(XT_USER_INFO_OLD));
	}
} XT_USER_INFO_OLD, * LPXT_USER_INFO_OLD;//IM通讯协议协议命令字定义,结构定义

//群成员列表信息

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

//群信息
typedef struct XT_CLUSTER_INFO
{
	uint32    nFamilyID;                            //群号
	uint8     nFamilyProp;							//群属性 FIRM_CLUSTER|TEMP_FAMILY
	uint8     nFamilyType;							//群类型 FAMILY_COMMON 普通群，FAMILY_VIP VIP群
	uint16    nFamilyClass;							//分类
	uint32    nCreatorID;                           //群的创建者ID

	uint8	  nFamilyFaceID;							//族徽ID
	char      szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1];	///修改为群名片标记。(族徽链接地址)

	uint8     nJoinFlag;							//加入规则 FAMILY_JOIN_ANY|FAMILY_JOIN_VALI|FAMILY_JOIN_UNALLOW
	uint8     nViewFlag;							//页面浏览权限	FAMILY_VIEW_MEM|FAMILY_VIEW_REG|FAMILY_VIEW_ALL
	char      szFamilyName[MAX_FAMILYNAME_LEN + 1]; //群名称
	char      szFamilyDesc[MAX_FAMILYDESC_LEN + 1]; //群描述
	char      szFamilyDomain[MAX_FAMILYDOMAIN_LEN + 1];//群域名长度
    uint16    nNumberLimit;      
	char      szFamilySign[MAX_FAMILYNAME_LEN + 1];//群签名长度
	char      szFamilyNotice[MAX_FAMILYNAME_LEN + 1];  //群公告设置(新增)

	XT_CLUSTER_INFO()
	{
		memset(this,0,sizeof(XT_CLUSTER_INFO));
	}

} XT_CLUSTER_INFO, * LPXT_CLUSTER_INFO;

/*
//IM 用户信息
typedef struct XT_REGIST_REQ
{
	char	key[16]      ;//随机密钥
	uint8   regfrom	     ;//注册来源
	char    username[MAX_USERNAME_LEN + 1] ;//3 用户名
	uint8   sex          ;
	char    nickname[MAX_NICKNAME_LEN + 1] ;
	char    password[MAX_PASSWORD_LEN + 1] ;//2 密码
	char    email[MAX_EMAIL_LEN + 1]	   ;//1 电子邮件
	XT_REGIST_REQ()
	{
		memset(this,0,sizeof(XT_REGIST_REQ));
	}

} XT_REGIST_REQ, * LPXT_REGIST_REQ;

//注册回应
typedef struct XT_REGIST_ACK
{
	uint8    ret		  ;//0 返回值(IM_FAIL, IM_SUCCES)
	uint32   id           ;//1 系统分配的user id
	char     error[40]    ;//2 错误信息,服务器返回

	XT_REGIST_ACK()
	{
		memset(this,0,sizeof(XT_REGIST_ACK));
	}

} XT_REGIST_ACK, * LPXT_REGIST_ACK;
*/
//登录请求
typedef struct XT_LOGIN_REQ
{
	uint8     login_flag; //78两位为登陆方式，6位是卖家或买家，5位表示客服 //4位用于判断是否校验异地登录
	enum{
		LOGINBYID = 0,
		LOGINBYNAME = 1,
		LOGINBYMOBILENO = 2,
	};

	enum {
		SELLER = 0,
		BUYER = 1,
		CS    = 2,///飞牛客服平台
		WORK = 3,//工作版
	};

	enum {
		CHECK_DIFF_PLACES = 0,		// 为0表示校验异地登录，默认
		DONT_CHECK_DIFF_PLACES = 1,	// 为1表示不校验异地登录
	};

	union ULoginKey
	{
		uint32    id            ;//0 用户id
		char      username[MAX_USERNAME_LEN + 1]  ;
		char      mobileno[MAX_PHONE_LEN + 1];
	}uLoginKey;

	char      random_key[16];
	char      password[24]  ;//1 登录密码MD5(MD5(plain)),密码明文的2重加密再加密一个空串		原长度16
	uint8     status        ;//2 登录状态(离开,上线..)
	uint32    lanip         ;//客户端局域网ip（*现用于存放与文件中转服务器通讯时的客户端IP）
	uint16    lanport       ;//客户端局域网port（*现用于存放与文件中转服务器通讯时的客户端端口）
    char      sessionID[128];///web session
	char      mobilephoneno[MAX_PHONE_LEN+1]; 
	char      email[MAX_EMAIL_LEN+1];  
    uint8     auto_session;//TOKEN值 	
XT_LOGIN_REQ()
	{
		memset(this,0,sizeof(XT_LOGIN_REQ));
	}

} XT_LOGIN_REQ, * LPXT_LOGIN_REQ;

//登录响应
typedef struct XT_LOGIN_ACK
{
	uint8	  ret		       ;//0 返回值

	uint8     session_key[16]  ;//1 Session Key
	//  以用户密码的MD5 Digest的MD5 Digest 
	//  做为密钥（KEY）加密了一个4Bytes的随机字串，
	//  得到一个16bytes的加密后字串，此字串将作为以
	//  后通讯的Session Key（16bytes），客户端将根
	//  据解密后最后是否7个零来判断是否正确的解密。
	uint32    user_id;
	uint32    merchant_id;
	uint32	  publicip;		//公网ip
	uint8     version_flag     ;//0,最新版本；1需要更新，但可以继续使用；2必须下载最新版本
	char      version_url[MAX_VERSION_URL + 1] ;//版本更新URL
	char      update_config[MAX_UPDATECONFIG_URL + 1];   
	char	  update_info[MAX_UPDATE_INFO + 1] ;//升级信息
	uint8	  needChangeAccount;
	char	  szGUID[MAX_GUID_LENGTH + 1];	// 自营GUID(MAX_GUID_LENGTH = 40）
	char	  username[MAX_USERNAME_LEN+1] ;	// 3 用户名
	uint32    login_time;
	XT_LOGIN_ACK()
	{
		memset(this,0,sizeof(XT_LOGIN_ACK));
	}
} XT_LOGIN_ACK, * LPXT_LOGIN_ACK;

typedef struct XT_LOGOUT
{
	uint32		id;
	uint8 authData; //校验数据 = 0x00

} XT_LOGOUT;

//更新目录信息请求
typedef struct XT_DIRECTORY_REQ
{
	uint32	  id        ;//0 用户id
	uint32	  local_ip  ;//1 内网 ip 地址
	uint16    local_port;//2 内网 端口

	XT_DIRECTORY_REQ()
	{
		memset(this,0,sizeof(XT_DIRECTORY_REQ));
	}

} XT_DIRECTORY_REQ, * LPXT_DIRECTORY_REQ;

//更新目录信息响应
typedef struct XT_DIRECTORY_ACK
{
	uint8		ret;
	char		error[40] ;
	
	XT_DIRECTORY_ACK()
	{
		memset(this,0,sizeof(XT_DIRECTORY_ACK));
	}

} XT_DIRECTORY_ACK, * LPXT_DIRECTORY_ACK;

//---------------------------------好友与资料管理----------------------

//获取好友（自己）基本资料的请求包结构
typedef struct XT_GET_FRIEND_BASICINFO_REQ
{
	uint8	type;
	uint16	count		;
	uint32	FriendID[1]	;

	enum { TYPE_FRIEND,TYPE_OTHER};

}XT_GET_FRIEND_BASICINFO_REQ,*LPXT_GET_FRIEND_BASICINFO_REQ;

//获取好友（自己）基本资料的应答包结构
typedef struct XT_GET_FRIEND_BASICINFO_ACK
{
	uint8  ret;

}XT_GET_FRIEND_BASICINFO_ACK,*LPXT_GET_FRIEND_BASICINFO_ACK;


//服务器对获取好友（自己）基本资料的通知
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
		char	szCommentName[MAX_COMMENTNAME_LEN+1];// 新增好友备注名称
		uint8	LoveFlag;							 // 关注标记
		char	szUserSign[MAX_USERSIGN_LEN + 1];// 个人签名 
		uint8   nMobileStatus                   ;//手机绑定状态
		char	szGUID[MAX_GUID_LENGTH + 1];	// 自营GUID(MAX_GUID_LENGTH = 40）
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

//邀请好友入群 请求
typedef struct XT_CLUSTER_INVITE_REQ
{
	uint8      count;//邀请好友个数
	char       clustername[MAX_FAMILYNAME_LEN+1];//群名字
	char       nickname[MAX_NICKNAME_LEN+1] ;//群昵称
	uint32     cluster_id   ;//群ID
	uint8      cluster_type ;//群类型
	uint32     userID[1];//邀请好友ID集合
	XT_CLUSTER_INVITE_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_REQ));
	}
}XT_CLUSTER_INVITE_REQ, *LPXT_CLUSTER_INVITE_REQ;

//邀请好友入群 应答
typedef struct XT_CLUSTER_INVITE_ACK
{
	uint8 ret;//应答为0
	XT_CLUSTER_INVITE_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_ACK));
	}
}XT_CLUSTER_INVITE_ACK, *LPXT_CLUSTER_INVITE_ACK;

//邀请好友入群 转发
typedef struct XT_CLUSTER_INVITE_IND
{
	uint32     invite_id;//邀请人ID
	char       clustername[MAX_FAMILYNAME_LEN+1];//群名字
	char       nickname[MAX_NICKNAME_LEN+1] ;//群昵称
	uint32     cluster_id   ;//群ID
	uint8      cluster_type ;//群类型
	XT_CLUSTER_INVITE_IND ()
	{
		memset(this,0,sizeof(XT_CLUSTER_INVITE_IND));
	}
} XT_CLUSTER_INVITE_IND, *LPXT_CLUSTER_INVITE_IND;

//好友是否同意入群 请求
typedef struct XT_CLUSTER_AGREE_REQ
{
	uint8      action;//0：同意 1：拒绝
	uint32     invite_id;//邀请人ID
	uint32     friend_id;//被邀请人ID
	uint32     cluster_id   ;//群ID
	uint8      cluster_type ;//群类型
	XT_CLUSTER_AGREE_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_REQ));
	}
}XT_CLUSTER_AGREE_REQ, *LPXT_CLUSTER_AGREE_REQ;

//好友是否同意入群 应答
typedef struct XT_CLUSTER_AGREE_ACK
{
	uint8 ret;
	XT_CLUSTER_AGREE_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTER_INVITE_ACK));
	}
}XT_CLUSTER_AGREE_ACK, *LPXT_CLUSTER_AGREE_ACK;

//好友是否同意入群 转发
typedef struct XT_CLUSTER_AGREE_IND
{
	uint8      action;//0：同意 1：拒绝
	uint32     invite_id;//邀请人ID
	uint32     friend_id;//被邀请人ID
	uint32     cluster_id   ;//群ID
	uint8      cluster_type ;//群类型
	XT_CLUSTER_AGREE_IND()
	{
		memset(this, 0, sizeof(XT_CLUSTER_AGREE_IND));
	}
}XT_CLUSTER_AGREE_IND, *LPXT_CLUSTER_AGREE_IND;

///////////////////////////////////////////////
//用户修改登录密码请求
typedef struct XT_MOD_PSW_REQ
{
	uint32  uid;      ///用户id
	char    szPsw[60];///数据库存储后加密的密码
} XT_MOD_PSW_REQ, *LPXT_MOD_PSW_REQ;

//用户修改登录密码服务器响应
typedef struct XT_MOD_PSW_ACK
{
	uint8   ret;
	uint32  uid;    ///用户id
	char    szPsw[60];///密码
	XT_MOD_PSW_ACK()
	{
		memset(this,0,sizeof(XT_MOD_PSW_ACK));
	}
} XT_MOD_PSW_ACK, *LPXT_MOD_PSW_ACK;
/////////////////////////////////////////////

//察看用户资料请求
typedef struct XT_USERINFO_GET_REQ
{
	//uint32	uid;
	uint32  fid;

} XT_USERINFO_GET_REQ, *LPXT_USERINFO_GET_REQ;

//察看用户资料响应

//客户端使用
typedef struct XT_USERINFO_GET_ACK : public XT_USER_INFO
{
	uint8   ret;	

	XT_USERINFO_GET_ACK()
	{
		memset(this,0,sizeof(XT_USERINFO_GET_ACK));
	}

} XT_USERINFO_GET_ACK, *LPXT_USERINFO_GET_ACK;

//服务器使用
typedef struct XT_SERVER_USERINFO_GET_ACK
{
	uint8   ret;	
	uint32  id           ;					// 用户id
	char    username[MAX_USERNAME_LEN+1] ;	// 用户名
	char    nickname[MAX_NICKNAME_LEN+1] ;	// 呢称
	char    email[MAX_EMAIL_LEN+1]	  ;		// 电子邮件
	char    birthday[MAX_BIRTHDAY_LEN+1] ;	// 出生日期
	uint8   sex          ;					// 性别
	uint8   province	 ;					// 省
	uint8   country		 ;					// 国家
	uint8   city	     ;					// 城市
	uint16	face		 ;					// 头像ID
	char    face_url[MAX_HEADURL_LEN+1] ;	// 头像URL
	uint8   career       ;					// 职业
	uint8   rule         ;					// 加为好友的认证方式
	uint8   familyflag	 ;					// 列为群成员认证方式
	uint16  gold_money   ;					// 金币数目
	uint32  online_time  ;					// 在线时长
	uint8   linkpop		 ;					// 联系方式隐私属性（0 私有;1 好友可见；3 公开）
	char    address[MAX_ADDRESS_LEN+1]  ;	// 联系地址
	char    mobile[MAX_PHONE_LEN+1]   ;		// 手机
	char    description[MAX_DESCRIPTION_LEN+1]; //个人说明
	char    usersign[MAX_USERSIGN_LEN + 1];	//个人签名

	XT_SERVER_USERINFO_GET_ACK()
	{
		memset(this,0,sizeof(XT_SERVER_USERINFO_GET_ACK));
	}

} XT_SERVER_USERINFO_GET_ACK, *LPXT_SERVER_USERINFO_GET_ACK;



//用户资料修改请求
typedef XT_USER_INFO XT_USERINFO_MOD_REQ;

//用户资料修改响应
typedef struct XT_USERINFO_MOD_ACK
{
	uint8	ret;
	char	error[40];

	XT_USERINFO_MOD_ACK()
	{	
		memset(this,0,sizeof(XT_USERINFO_MOD_ACK));
	}
	
} XT_USERINFO_MOD_ACK, *LPXT_USERINFO_MOD_ACK;


//用户状态更改请求
typedef struct XT_STATUS_MOD_REQ
{
	uint32  id;
	uint8   status;

} XT_STATUS_MOD_REQ, * LPXT_STATUS_MOD_REQ;

//服务器响应用户状态更改
typedef struct XT_STATUS_MOD_ACK
{
	uint8  ret;
	uint8  status;

} XT_STATUS_MOD_ACK, *LPXT_STATUS_MOD_ACK;


#define FLS_TOTAL     4 //获取全部好友
#define FLS_ONLINE	  5 //获取在线好友


//获取好友列表请求
typedef struct XT_FRIENDLIST_REQ
{
	uint32      merchant_id;     //商家id
	uint32		id			;
	uint16		start_pos	;
	uint8		nType;			 // 标识工作台版本

	XT_FRIENDLIST_REQ()
	{
		memset(this,0,sizeof(XT_FRIENDLIST_REQ));
	}

} XT_FRIENDLIST_REQ, * LPXT_RIENDLIST_REQ;


//获取好友列表应答
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

//好友列表结束,没有后续数据需要继续接收
#define FRIENDLIST_NEXTPOS_END 0xffff

//获取组信息
typedef struct XT_GROUPINFO_GET_REQ
{
	uint32	id;
	uint8	group_count;
	uint8	nType;		// 标识工作台版本

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

//修改组信息
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

//添加用户到组、修改用户组别或删除组内用户
typedef struct XT_FRIENDGROUP_MOD_REQ
{
	uint32   uid			;
	uint8	 action			;
	//action的取值意义
	enum { ACTION_ADD = 0 ,	//添加
		   ACTION_MODIFY,	//修改
		   ACTION_DEL,		//删除
		   ACTION_BLACK};	//设为黑名单并从对方对方好友列表删除自己
	
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


/******服务器发送给客户端的系统消息或转发消息,需要服务器参与的转发消息*****/
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

//转发的具体协议定义如下(XT_MSG,XT_USERAUTH_REQ,XT_SERVERMSG,XT_CMEMBER_IND,XT_CLUSTER_AUTH_IND,
//XT_CLUSTER_DISMISS_IND)
//P2P聊天信息
typedef struct XT_MSG
{
	// 请在修改本结构时更新版本号
	const static uint32 VER = 0x00000001;

	uint8    msg_type     ;//聊天消息类型0：聊天消息,1群聊消息,10:离线聊天消息，11：离线群聊消息
	uint32   cluster_id   ;//群聊时有用
	uint8    cluster_type ;//群聊时有用
	
	uint8    recv_flag    ;//0, 接收自p2p; 1 接收自server
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

	uint8    data_type	  ;//0即时聊天消息，1图片文件发送请求,
	                       //2 图片文件应答，3图片文件中止 ，4 图片文件传送完毕，
	                       //5 图片数据
						   //6~8 群图片文件
						   //9 魔法表情

	enum DATA_TYPE
	{
		IM_MSG=0,
		SF_FILE_REQ=1,
		SF_FILE_ACK=2,
		SF_PACKET_REQ=3,
		SF_PACKET_ACK=4,
		SF_ABORT=5,
		WIN_SHAKE = 6,		// 窗口抖动
		PUSH_EVALUA = 7,	// 推送评价
		SF_CFILE_REQ=8,
		MAGIC_FACE=9,
		USER_SIGN=10,
		FILE_REQ=11,
		MM_REQ=12,
		MM_REPLY=13,
		MM_CANCEL=14,
		MM_STOP=15,
		VEDIO_EQUIPMENT=16,
		EVALUA_ACK=17,			// 服务评价结果
		OPEN_P2PSENDMSG_DLG=18,	// 打开聊天窗口
		SF_IMG_URL = 19,
		CUST_SER_TRAN_REQ = 20,	// 客服转接请求
		AUTO_REPLY = 21,		// 自动接待
		DEVICE_REPLY = 22,		//多设备模式下，自己其他设备发送的消息
		IM_MSG_INSERT=26,       //插入会话
		IM_MSG_ORDER_SEQ=27,    //订单号
		MAX_TYPE=27,
	};

#ifdef _MOBILE_TEST_
	char 	uuid[MAX_UUID_LEN+1];		// 消息的唯一标示	
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
	uint32		nCid;		// 群id
	uint32		nFileId;	// 文件id
	uint16		nPacketId;	// 文件块编号
}XT_UPLOAD_CPIC_ACK,*LPXT_UPLOAD_CPIC_ACK;

// 来自图片拥有者向下载者的通知
typedef struct _XT_DOWNLOAD_CPIC_NOTIFY
{
	uint32	nSrcId;				// 文件拥有者
	uint32	nFileId;			// 文件id
	uint32	nFileSize;			// 文件大小
	uint16	nPacketSize;		// 文件包大小
	char	szFileName[255];	// 文件名
}XT_DOWNLOAD_CPIC_NOTIFY,*LPXT_DOWNLOAD_CPIC_NOTIFY;


typedef struct _XT_DOWNLOAD_CPIC_REQ
{
	uint32	nFileSid;		// 图片文件拥有者id
	uint32  nFileId;		// 图片文件id
	uint16	nPacketId;		// 图片块编号
	uint32	nCId;			// 群id
}XT_DOWNLOAD_CPIC_REQ,*LPXT_DOWNLOAD_CPIC_REQ;

typedef struct _XT_DOWNLOAD_CPIC_ACK
{
	enum { RET_SUCCESS = 0, RET_WAIT, RET_FAILED } ;
	uint8		ret;
	uint32		nCId;		// 群id
	uint32		nSrcId;		// 图片拥有者id
	uint32		nFileId;	// 图片id	
	uint16		nPacketId;	// 图片包id
	uint16		nDataLen;	// 图片包长度
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

//服务器通知用户别人加该用户为好友
typedef struct tagXT_FRIENDADD_NOTIFY
{
	uint32	ThatOneID;
} XT_FRIENDADD_NOTIFY ,*LPXT_FRIENDADD_NOTIFY;

typedef struct tagXT_FRIENDADD_NOTIFY  tagXT_FRIENDDEL_NOTIFY;

typedef struct XT_CMEMBER_IND
{
	uint32     uid          ;
	uint8      action       ;// 0:添加/加入；1：删除/退出；2：设为管理员；3：取消管理员；4：被邀请人拒绝入群； 5：群主拒绝被邀请人入群； 6：解散群
	uint32     member_id    ;
	char       clustername[MAX_FAMILYNAME_LEN+1];
	char       nickname[MAX_NICKNAME_LEN+1] ;
	uint32     cluster_id   ;
	uint8      cluster_type ;
	uint32     invite_id    ;//成员的邀请人ID

	XT_CMEMBER_IND()
	{
		memset(this,0,sizeof(XT_CMEMBER_IND));
	}

} XT_CMEMBER_IND, *LPXT_CMEMBER_IND;

//添加好友验证请求
typedef struct XT_USERAUTH_REQ
{
	uint8		action		;//1 Byte, 0x00=请求; 0x01=同意; 0x02=拒绝
	enum { ACTION_REQUEST = 0, ACTION_AGREE,ACTION_DENIAL };

	uint32		from_id		;//发送方id
	uint32		to_id		;//对方id
	uint8		groupId		;//好友分组id
	uint32		send_time	;
	char		info[120+1]	;//验证信息

	XT_USERAUTH_REQ()
	{
		memset(this,0,sizeof(XT_USERAUTH_REQ));
	}

} XT_USERAUTH_REQ, *LPXT_USERAUTH_REQ;

typedef struct XT_USERAUTH_ACK
{
	uint8		ret		;
	uint8		action	;
	uint32		to_id	; //对方号码

	XT_USERAUTH_ACK()
	{
		memset(this,0,sizeof(XT_USERAUTH_ACK));
	}

} XT_USERAUTH_ACK, *LPXT_USERAUTH_ACK;

//服务器给客户端的群验证消息通知
typedef struct XT_CLUSTER_AUTH_IND
{
	uint32       uid;
	uint8        action      ;// 0x00=请求; 0x01=同意; 0x02=拒绝
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
	uint8        nAction    ;	// 0x00=请求; 0x01=同意; 0x02=拒绝
  	uint32       nRequestID ;	// 请求方ID(即结束接收方ID)
	uint32       nManagerID  ;	//同意或拒绝时填请求方ID， 请求时填0
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
	uint8        nAction      ;// 0x00=请求; 0x01=同意; 0x02=拒绝

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


//服务器好友状态更改通知
typedef struct XT_STATUS_IND
{
 	uint32		fid;	//好友id
	uint8		status;
	uint16		version;

	XT_STATUS_IND()
	{
		memset(this,0,sizeof(XT_STATUS_IND));
	}

} XT_STATUS_IND, * LPXT_STATUS_IND;

//服务器好友状态更改通知应答
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


//客户端通过服务器转发聊天消息的头部
enum SERVER_P2PMSG_TYPE
{
	SERVER_P2PMSG_TYPE_MESSAGE = 0,	//聊天消息
	SERVER_P2PMSG_TYPE_MYWORD  = 10,//个性签名
	SERVER_P2PMSG_TYPE_CUSTOM_EMOTION	//自定义表情
};
typedef struct XT_SERVER_P2PMSG_SEND
{
	uint32 Sender;
	uint32 Recver;
	uint8  type;


}XT_SERVER_P2PMSG_SEND,*LPXT_SERVER_P2PMSG_SEND;

//服务器转发聊天消息应答
typedef struct XT_SERVER_P2PMSG_ACK
{
	uint32 Sender;
	uint32 Receiver;
	uint8  type;

} XT_SERVER_P2PMSG_ACK,*LPXT_SERVER_P2PMSG_ACK;



//对方接收到P2P聊天信息响应
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

//查找用户

#define	 XTREAM_SEARCH_ONLINE		0 //看谁在线
#define  XTREAM_SEARCH_PRECISION	1 //精确查找

typedef struct XT_XTREAM_SEARCH_REQ
{
	uint16		startpos;
	uint8		search_type;
	uint8		count;			// 要求返回数目。0无限制
	uint32		id;				// 按ID查找
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

//获取好友连接状态信息请求
typedef struct XT_STATUS_GET_REQ
{
	uint32		id;
	uint32		fid;

	XT_STATUS_GET_REQ()
	{
		memset(this,0,sizeof(XT_STATUS_GET_REQ));
	}

} XT_STATUS_GET_REQ, *LPXT_STATUS_GET_REQ;

//获取好友连接状态信息应答
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

//搜索群
#define  CLUSTER_SEARCH_ID			0 //精确搜索,按ID搜索
#define  CLUSTER_SEARCH_CLASS		1 //按类型搜索
#define  CLUSTER_SEARCH_NAME		2 //按名称搜索

//#define CMD_CLUSTER_SEARCH_REQ      0x909
typedef struct XT_CLUSTER_SEARCH_REQ
{
//	uint32		uid;
	uint16		start_pos;
	uint8		search_type;
	uint8		nCount;//要求返回数目。0无限制
	union TYPE_DATA
	{
		uint32		nFamilyID;
		uint16      nFamilyClass;//群分类
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
		uint8        nMemCount;////返回群成员个数				;	
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
	uint8		nFamilyProp									;	//1：固定群，2：临时群
	uint8		nJoinFlag									;	//验证：FAMILY_JOIN_ANY 任意加入 FAMILY_JOIN_VALIDATE 验证 FAMILY_JOIN_UNALLOW 拒绝
	uint16		nFamilyClass								;   //类别：同事，同学
	uint8		nFamilyFaceID										;
	char		szFamilyFaceUrl[MAX_FAMILYFACEURL_LEN + 1]	;	//族徽链接地址
	uint8		nViewFlag									;	//FAMILY_VIEW_MEM	群成员（私有）FAMILY_VIEW_REG	注册用户（半公开）FAMILY_VIEW_ALL 所有（公开）

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
//获取群信息
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


//设置群成员
typedef struct XT_SET_CMEMBERS_REQ
{
	uint32     nFamilyID			;
	uint8      nFamilyProp		    ;
	uint8      nAction		        ;
	uint8      nCount	            ;
	uint32     nInviteID         ;//需要操作的成员的邀请人
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
	uint32     nInviteID               ;//成员的邀请人ID
	char     szError[MAX_ERROR_BUF]    ;
	XT_SET_CMEMBERS_ACK()
	{
		memset(this,0,sizeof(XT_SET_CMEMBERS_ACK));
	}

} XT_SET_CMEMBERS_ACK, *LPXT_SET_CMEMBERS_ACK;

//申请加入、退出群
typedef struct XT_CLUSTER_APPLY_REQ
{
	uint32    nFamilyID							;
	uint8     nFamilyProp						; //1:固定群  2：临时群
	uint8     nAction							; //FAMILY_REQ_JOIN|FAMILY_REQ_QUIT
	uint32    nUserID							; //请求者ID

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
	uint8    nFlag;// 00, 成功; 01, 需要验证; 02, 对方不许任何人加入该群

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
	uint8		nFamilyProp		;					//群属性 FIRM_CLUSTER|TEMP_FAMILY
	uint8		online_type		; //0:所有,1在线好友
	uint16		req_count		;
	uint8		userstatus		;//用户当前状态
// 	uint8       user_property	; //用户在群中的地位(普通成员0,管理员1,族长2)

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
	uint8        online_type          ;//0所有,1在线
	uint16       return_count       ;
	FAMILYMEMBER_ACKINFO  members[1]       ;
// 	char         error[40]          ;

	XT_GET_CMEMBERS_ACK()
	{
		memset(this,0,sizeof(XT_GET_CMEMBERS_ACK));
	}

} XT_GET_CMEMBERS_ACK, *LPXT_GET_CMEMBERS_ACK;


//获取用户拥有的固定群清单
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
typedef struct XT_FAMILY_SENDMSG_REQ	//只描述部分信息，其它信息直接转发给客户端
{
	uint32			familyID;
	uint32			senderID;
	uint16			version;
	uint8           familyProp;
	uint8           msgType;	//0x00，普通群消息。0x01，自动回复消息，离线时不保存
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

//type	1 Bytes 消息类型
//0.用户更新消息 1.收件箱，2.系统公告,3.用户公告,4.个人主页评论 5.博客内容评论
//recID	4 Bytes流水号。用于修改数据库发送标志。
//recvID	4 Bytes  接收者ID
//title	Max 200 Bytes，text，消息标题
//content	Max 900 Bytes，text，消息内容
//classID	2 Bytes 更新类型，
//1=发表日志,2=上传图片,3=上传音乐,4=上传视频,5=上传共享文件
//srcuri	Max 500 Bytes, test 操作对象目标链接
//sendID	4 Bytes 发送者ID
//nickname	Max 32 Bytes 发送者昵称
//sendTime	Max 32 Bytes text 发送时间

typedef struct  XT_WEB_MSG
{	
	uint8       msg_type			;//0.用户更新消息 1.收件箱，2.系统公告,3.用户公告,4.个人主页评论 5.博客内容评论
	
	enum
	{
		USER_UPDATE  =0,
		USER_MSG_BOX =1,
		SYS_MSG      =2,
		FRIEND_MSG   =3,
		MAINPAGE_COMMENT =4,
        CONTENT_COMMENT  =5,
	};

	uint32		msg_id				;//消息流水号。发送后需要修改数据库状态
	uint32	    recv_id				;//接收者
	char        title[200]			;
	char        content[900]		;
	uint8		sub_type    ;//用户更新类型 1=发表日志,2=上传图片,3=上传音乐,4=上传视频,5=上传共享文件
							 //评论类型：0文章评论，1图片评论，2相册评论、3音频评论，4视频评论
							 //收件箱 0站内消息 2--加为好友 3--加入群邀请  4--客服消息	5--申请加为好友
	uint32		log_id		;//博客内容ID  

	enum 
	{
		BLOG  =1,
		PIC   =2,
		MUSIC =3,
		VEDIO =4,
		FILE  =5,
	};

	char		src_url[255]		;
	uint32		sender_id			;//发送者id
	char		sender_nickname[MAX_NICKNAME_LEN+1] ;//发送者昵称
	char        sender_username[MAX_USERNAME_LEN+1] ;//发送者username
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


//服务器短信下发
typedef struct XT_SMS_IND
{
	uint32			msg_id		;//服务器短信流水号
	uint32			sender_id	;//发送者id
	char            send_time[32];
	char			user_name[25];
	char			nick_name[25];
	char			link_id[25];
	uint16			biz_type; //业务类型 400-499 Mini
	uint8			msg_type    ;//短信类型， 0-普通短信 1-wap push
	char			message[255];//

	XT_SMS_IND()
	{
		memset(this,0,sizeof(XT_SMS_IND));
	}

} XT_SMS_IND, *LPXT_SMS_IND;


typedef struct XT_SMS_REQ
{
	uint32			recv_id;     //接收方id
	char			mobile[16];  //接收方手机号码
	char			link_id[25];
	uint16			biz_type; //业务类型 400-499 Mini
	uint32			biz_id  ;
	uint8			msg_type    ;//短信类型， 0-普通短信 1-wap push
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
	uint32		id;				// 当前用户id
	uint32		fid;			// 好友id
	char		commName[MAX_COMMENTNAME_LEN+1];	// 备注名称
}XT_MOD_FRIEND_COMMNAME_REQ,*LPXT_MOD_FRIEND_COMMNAME_REQ;

typedef struct _XT_MOD_FRIEND_COMMNAME_ACK
{
	uint8		rt;				// 返回值，0成功，1失败
	uint32		fid;			// 好友id
	char		commName[MAX_COMMENTNAME_LEN+1];	// 备注名称
}XT_MOD_FRIEND_COMMNAME_ACK,*LPXT_MOD_FRIEND_COMMNAME_ACK;

//群名片
typedef struct XT_PROFILENAME_SET_REQ
{
	uint32		nFamilyID;			// 当前群id
	uint32		nMemID;			// 当前群的成员ID
	char		ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1];	// 群名片
	XT_PROFILENAME_SET_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_SET_REQ));
	}

}XT_PROFILENAME_SET_REQ, *LPXT_PROFILENAME_MOD_REQ;

typedef struct XT_PROFILENAME_SET_ACK
{
	uint8       ret;
	uint32		nFamilyID;			// 当前群id
	uint32		nMemID;			// 当前群的成员ID
	char		ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1];	// 群名片
	XT_PROFILENAME_SET_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_SET_ACK));
	}
}XT_PROFILENAME_SET_ACK, *LPXT_PROFILENAME_SET_ACK;

//////////////////////////////////////////////////////////////////////////////
/////群名片获取请求
typedef struct XT_CMEM_IFNO
{
	uint32	   nMemID;      // 当前群的成员ID
	char       ProfileName[MAX_CLUSTER_MEMBER_PROFILE + 1]; // 群名片
}XT_CMEM_IFNO;


typedef struct XT_PROFILENAME_GET_REQ
{
	uint32		nFamilyID;			// 当前群id
	XT_PROFILENAME_GET_REQ()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_REQ));
	}

}XT_PROFILENAME_GET_REQ, *LPXT_PROFILENAME_GET_REQ;


////群名片获取应答
typedef struct XT_PROFILENAME_GET_ACK
{
	uint8         ret;
	uint32		  nFamilyID;		  // 当前群id
	uint32        nCount;             // 当前群成员数
    XT_CMEM_IFNO  MemInfo[1];         // 当前群成员和群名片组
	XT_PROFILENAME_GET_ACK()
	{
		memset(this, 0, sizeof(XT_PROFILENAME_GET_ACK));
	}
}XT_PROFILENAME_GET_ACK, *LPXT_PROFILENAME_GET_ACK;


/////////////////////////////////////////////////////////////////////////////////////////////////////////////
//群信息接收设置
typedef struct XT_CLUSTERMSG_RECV_SET_REQ
{
	uint32		nFamilyID;			// 当前群id
	uint32		nMemID;			    // 当前群的成员ID
	uint8		MsgRecType;         // 群消息接收方式：RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4;
	XT_CLUSTERMSG_RECV_SET_REQ()
	{
		memset(this, 0, sizeof(XT_CLUSTERMSG_RECV_SET_REQ));
	}

}XT_CLUSTERMSG_RECV_SET_REQ, *LPXT_CLUSTERMSG_RECV_SET_REQ;

typedef struct XT_CLUSTERMSG_RECV_SET_ACK
{
	uint8       ret;
	uint32		nFamilyID;			// 当前群id
	uint32		nMemID;			    // 当前群的成员ID
    uint8		MsgRecType;         // 群消息接收方式：RECV_SHOW = 1,RECV_NOSHOW = 2,NORECV = 3,SHOW_NUM = 4;
	XT_CLUSTERMSG_RECV_SET_ACK()
	{
		memset(this, 0, sizeof(XT_CLUSTERMSG_RECV_SET_ACK));
	}
}XT_CLUSTERMSG_RECV_SET_ACK, *LPXT_CLUSTERMSG_RECV_SET_ACK;
/////////////////////////////////////////////////////////////////////////////////////////////////////////////
/////////////////////////////////////////////////////////////////////////////////////////////////////////////

//P2p测试包
typedef struct _XT_P2PTEST_REQ
{
	// ip类型
	enum TYPE{ 
		P2P,	// p2p公网地址
		LAN		// 内网地址
	};
	uint32		dest_ip;
	uint16      dest_port;
	uint8		ipType;
	uint32      id;			// 发起方id
	uint32		toid;		// 目的方id
	_XT_P2PTEST_REQ()
	{
		memset(this,0,sizeof(_XT_P2PTEST_REQ));
	}

} XT_P2PTEST_REQ, *LPXT_P2PTEST_REQ;

typedef struct _XT_P2PTEST_IND
{
	// ip类型
	enum TYPE{ 
		P2P,	// p2p公网地址
		LAN		// 内网地址
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
	uint32		id;			// 应答方id
	uint8		testType;	// 测试类型，内网连接和公网连接
	enum TYPE{
		P2P,	// p2p公网地址
		LAN		// 内网地址
	};
	uint32		ip;			// 应答方ip
	uint16		port;		// 应答方port

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
	uint32		ip;		// 网络字节
	uint16		port;	// 主机字节
	uint8		type;	// ip类型

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
	uint32			fid;		// 文件ID
	uint8			flag;		// 意见
	uint32			startPos;	// 起始位置
	uint32			packetSize;	// 分包大小
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
	uint8			type;	// 中止类型（ABORT_TYPE）
	uint32			fid;	// 文件ID
	uint32			friendId;
	//P2P_ENDPOINT	pt;		// P2P端点
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

//登录响应
typedef struct _XT_TRANS_LOGIN_ACK
{
	uint8	ret				;// 0 返回值
	uint32	sessionid		;// SessionID
	char	sessioncode[16]	;// SessionCode

	uint32	ip;
	uint16	port;
	_XT_TRANS_LOGIN_ACK()
	{
		memset(this,0,sizeof(_XT_TRANS_LOGIN_ACK));
	}
} XT_TRANS_LOGIN_ACK, * LPXT_TRANS_LOGIN_ACK;

// 发送者关闭会话
typedef struct _XT_TRANS_CLOSE_SEESION_REQ
{
	uint8	count;
	uint32	sessionID[300];
}XT_TRANS_CLOSE_SEESION_REQ,*LPXT_TRANS_CLOSE_SEESION_REQ;

//向中转服务器添加接收者
typedef struct XT_TRANS_SETRECV_REQ
{
	uint32	sessionid;
	uint8   action;	//操作 0x01 添加   0x02 删除
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

// 接收者登录中转服务器请求
typedef struct XT_TRANS_RECVLOGIN_REQ
{
	uint32		sessionid;
	uint8		action;
	enum {RECV_LOGIN = 0x01, RECV_LOGOUT = 0x02};
	char		sessioncode[16]	;//SessionCode

	uint32		ip;
	uint32		port;
	uint32		uid;	// 发送者A的id
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

// 通知接收者B登录中转服务器
typedef struct _XT_TRANS_NOTIFY_LOGIN_REQ
{
	uint32		sessionID;	// 会话ID
	uint8		linkType;	// 0 = udp;1 = tcp
	uint32		transSrvIP;	// 中转服务器IP	
	uint16		port;		// 中转服务器端口	
	uint8		check[16];	// 验证码
	
	uint32		uid;		// 接收者B的id
	uint32		sid;		// 发送者A的id
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




// 获取商户最近联系的客户信息(发送消息)
typedef struct XT_GET_NEAR_COMM_CUST_REQ
{
	uint32  uiBusinessID;		// 商户ID	
	char szBgnTime[32];			// 起始时间，格式为YY-MM-DD hh:mm:ss
	char szEndTime[32];			// 结束时间，格式为YY-MM-DD hh:mm:ss
	uint32 iMaxCustCount;		// 获取的最大用户数，默认值为1000，当指定时间段内用户数超过此值时，只获取最近的最大数量客户	
	uint16 iStartPos;			// 买家信息开始位置（如果为999，只需给客户端返回客户实际数量）
	uint16 iNeedCustCount;		// 要获取的买家数量
	uint8 bDirect;				// 买家信息排序方式（0表示按最后联系时间从大到小排序，1表示按最后联系时间从小到大排序）

	XT_GET_NEAR_COMM_CUST_REQ()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REQ));
		iMaxCustCount = 1000;
	}
}XT_GET_NEAR_COMM_CUST_REQ, *LPXT_GET_NEAR_COMM_CUST_REQ;

// 获取商户最近联系的客户信息(接收消息)
typedef struct XT_GET_NEAR_COMM_CUST_ACK
{
	uint32  ret;						// 错误编码
	uint32  uiBusinessID;				// 商户ID

	uint8   bMask;						// 只能为下面枚举
	enum
	{
		MK_ALL_CUST_COUNT = 0x00,		// 表示actualCount返回的是最近联系客户总数目（当XT_GET_NEAR_COMM_CUST_REQ.iStartPos == 999时）
		MK_CUR_CUST_COUNT = 0x01,		// 表示actualCount返回CommInfo数组的大小
	};

	uint32  actualCount;				

	struct TagCommInfo
	{
		char szCommAccount[32];			// 客户账号
		char szCommTime[32];			// 最近联系时间，格式为YY-MM-DD hh:mm:ss
		char szTransInfo[64];			// 显示联系人最近一份交易订单号。如果没有交易订单，则显示空白
		char szRemarks[128];			// 显示在聊天窗口卖家对客户的备注信息
	}CommInfo[1];

	XT_GET_NEAR_COMM_CUST_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_ACK));
	}
}XT_GET_NEAR_COMM_CUST_ACK, *LPXT_GET_NEAR_COMM_CUST_ACK;

typedef XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo XT_COMMINFO_GET;





// 更新最近联系客户信息到后台（发送消息）
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_REQ
{
	uint32  uiBusinessID;				// 商户ID	
	uint32	uiCommCount;					// 客户数量
	struct TagCommInfo
	{
		char szCommAccount[32];			// 客户账号
		char szCommTime[32];			// 最近联系时间，格式为YY-MM-DD hh:mm:ss
	}CommInfo[1];

	XT_UPDATE_NEAR_COMM_CUST_INFO_REQ()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_REQ));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_REQ, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_REQ; 

typedef XT_UPDATE_NEAR_COMM_CUST_INFO_REQ::TagCommInfo XT_COMMINFO_UPDATE;

// 更新最近联系客户信息到后台（接收消息）
typedef struct XT_UPDATE_NEAR_COMM_CUST_INFO_ACK
{
	uint8		ret;					// 服务器返回的消息，标识成功与否

	XT_UPDATE_NEAR_COMM_CUST_INFO_ACK()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_INFO_ACK));
	}
}XT_UPDATE_NEAR_COMM_CUST_INFO_ACK, *LPXT_UPDATE_NEAR_COMM_CUST_INFO_ACK;


#ifdef _BUSINESS_VESION

// 更新最近联系客户备注到后台（发送消息）
typedef struct XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ
{
	uint32  uiBusinessID;			// 商户ID	
	char szCommAccount[32];			// 客户账号
	char szCommTime[32];			// 最近联系时间，格式为YY-MM-DD hh:mm:ss
	char szRemarks[201];			//右侧页备注

	XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ));
	}
}XT_UPDATE_NEAR_COMM_CUST_REMARK_REQ, *LPXT_UPDATE_NEAR_COMM_CUST_REMARK_REQ; 

// 更新最近联系客户备注到后台（接收消息）
typedef struct XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK
{
	uint8		ret;					// 服务器返回的消息，标识成功与否

	XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK()
	{
		memset(this, 0, sizeof(XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK));
	}
}XT_UPDATE_NEAR_COMM_CUST_REMARK_ACK, *LPXT_UPDATE_NEAR_COMM_CUST_REMARK_ACK;



//从后台获取最近联系客户备注（发送消息）
typedef struct XT_GET_NEAR_COMM_CUST_REMARK_REQ
{
	uint32  uiBusinessID;				// 商户ID	
	char szCommAccount[32];			// 客户账号

	XT_GET_NEAR_COMM_CUST_REMARK_REQ()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REMARK_REQ));
	}
}XT_GET_NEAR_COMM_CUST_REMARK_REQ, *LPXT_GET_NEAR_COMM_CUST_REMARK_REQ; 

// 从后台获取最近联系客服备注（接收消息）
typedef struct XT_GET_NEAR_COMM_CUST_REMARK_ACK
{
	uint8	ret;				// 服务器返回的消息，标识成功与否
	uint32  uiBusinessID;			// 商户ID	
	char szCommAccount[32];			// 客户账号
	char szCommTime[32];			// 最近联系时间，格式为YY-MM-DD hh:mm:ss
	char szRemarks[128];

	XT_GET_NEAR_COMM_CUST_REMARK_ACK()
	{
		memset(this, 0, sizeof(XT_GET_NEAR_COMM_CUST_REMARK_ACK));
	}
}XT_GET_NEAR_COMM_CUST_REMARK_ACK, *LPXT_GET_NEAR_COMM_CUST_REMARK_ACK;

#endif

// 系统消息：更新好友信息
typedef struct XT_INFO_IND
{
	uint32		fid;	//好友id

	XT_INFO_IND()
	{
		memset(this,0,sizeof(XT_INFO_IND));
	}

} XT_INFO_IND, * LPXT_INFO_IND;


//上传头像文件应答的结构体
typedef struct XT_HEAD_FILE_UPLOAD_ACK
{
	uint8    ret;
	uint8    type;//1:上传文件 2:上传文件包
	uint8    uploadFinish;//0:未上传完 1:已上传完
	char    face_url[MAX_HEADURL_LEN+1] ;//头像url
	XT_HEAD_FILE_UPLOAD_ACK()
	{
		memset(this,0,sizeof(XT_HEAD_FILE_UPLOAD_ACK));
	}

} XT_HEAD_FILE_UPLOAD_ACK, *LPXT_HEAD_FILE_UPLOAD_ACK;

// 买家对卖家的评价等级
enum EN_EVALUA_GRADE
{
	eIgnore = 0,					// 忽略，未评价
	eVerySatisfied = 5,				// 非常满意
	eSatisfied = 4,					// 满意
	eGeneral = 3,					// 一般
	eDissatisfied = 2,				// 不满意	
	eVeryDissatisfied = 1,			// 非常不满意
};

#ifdef _BUSINESS_VESION
// 修改挂起状态
//#define CMD_MOD_HANGUP_STATE_REQ	0x0A11
//#define CMD_MOD_HANGUP_STATE_ACK	0x0A12
typedef struct XT_MOD_HANGUP_STATE_REQ
{
	uint32 uUserId;					// 用户Id
	uint8 uHangupState;				// 挂起状态，0--不挂起，1--挂起
	XT_MOD_HANGUP_STATE_REQ()
	{
		memset(this,0,sizeof(XT_MOD_HANGUP_STATE_REQ));
	}

} XT_MOD_HANGUP_STATE_REQ, * LPXT_MOD_HANGUP_STATE_REQ;
#endif

#ifdef _VER_INTERNAL_	// 内部版，获取员工信息
//#define CMD_GET_EMPLOYEE_INFO_REQ	0x0A13
//#define CMD_GET_EMPLOYEE_INFO_ACK	0x0A14
// 请求
typedef struct XT_GET_EMPLOYEE_INFO_REQ
{
	uint32 uUserId;					// 用户Id
	XT_GET_EMPLOYEE_INFO_REQ()
	{
		memset(this,0,sizeof(XT_GET_EMPLOYEE_INFO_REQ));
	}
} XT_GET_EMPLOYEE_INFO_REQ, * LPXT_GET_EMPLOYEE_INFO_REQ;

// 应答
typedef struct XT_GET_EMPLOYEE_INFO_ACK
{
	uint8		ret;								// 服务器返回的消息，标识成功与否
	uint32		uUserId;							// 用户Id
	char		username[MAX_USERNAME_LEN+1] ;		// 用户名
	char		job[MAX_JOB_LENGTH+1];				// 职务
	char		dept[MAX_DEPT_LENGTH+1];			// 部门
	char		extension[MAX_PHONE_LEN+1];			// 分机号
	char		mobile[MAX_PHONE_LEN+1];			// 手机

	XT_GET_EMPLOYEE_INFO_ACK()
	{
		memset(this,0,sizeof(XT_GET_EMPLOYEE_INFO_ACK));
	}
} XT_GET_EMPLOYEE_INFO_ACK, * LPXT_GET_EMPLOYEE_INFO_ACK;
#endif

//#define CMD_CUSTSER_TRAN_REQ	0x801B		// 客户转接发送方发送转接请求给服务器，服务器直接把这条消息转给接收方
//#define CMD_CUSTSER_TRAN_ACK	0x801C		// 客服转接接收方接到转接请求后，发送确认包给服务器，服务器直接转发给发送方
// 请求
typedef struct XT_CUSTSER_TRAN_REQ
{
	uint32 uSid;					// 发送方Id
	uint32 uDid;					// 接收方Id
	uint32 uCustomerId;				// 客户Id
	char szOrderId[64];				// 订单编号
	char szTranReason[512];			// 转接原因
	XT_CUSTSER_TRAN_REQ()
	{
		memset(this,0,sizeof(XT_CUSTSER_TRAN_REQ));
	}
} XT_CUSTSER_TRAN_REQ, * LPXT_CUSTSER_TRAN_REQ;

// 应答
typedef struct XT_CUSTSER_TRAN_ACK
{
	uint8 ret;						// 接收方接收到转接后，返个发送方的应答
	uint32 uSid;					// 发送方Id
	uint32 uDid;					// 接收方Id
	uint32 uCustomerId;				// 客户Id

	XT_CUSTSER_TRAN_ACK()
	{
		memset(this,0,sizeof(XT_CUSTSER_TRAN_ACK));
	}
} XT_CUSTSER_TRAN_ACK, * LPXT_CUSTSER_TRAN_ACK;

//#ifndef _MINI_CLIENT_
#pragma pack(pop)
//#endif

//客服当前聊天买家
typedef struct XT_MONITOR_SET_BUYER_REQ //客服设置当前聊天的买家
{
	uint32     idSeller;//客服ID
	uint32     idBuyer;//客服正在聊天的买家
	
	XT_MONITOR_SET_BUYER_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_SET_BUYER_REQ));
	}
}XT_MONITOR_SET_BUYER_REQ, *LPXT_MONITOR_SET_BUYER_REQ;

typedef struct XT_SET_BUYER_ACK //客服设置当前聊天的买家应答
{
	uint8      ret;
	XT_SET_BUYER_ACK()
	{
		memset(this, 0, sizeof(XT_SET_BUYER_ACK));
	}
}XT_SET_BUYER_ACK, *LPXT_SET_BUYER_ACK;

typedef struct XT_MONITOR_SET_BUYER_IND //主管获取客服当前聊天买家
{
	uint32     idSeller;//客服ID
	uint32     idBuyer;//客服正在聊天的买家
	XT_MONITOR_SET_BUYER_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_SET_BUYER_IND));
	}
}XT_MONITOR_SET_BUYER_IND, *LPXT_MONITOR_SET_BUYER_IND;

//监控
typedef struct XT_MONITOR_START_REQ //主管监控客服
{
	uint32     idMaster;//主管ID
	uint32     idSeller;//客服ID
	XT_MONITOR_START_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_REQ));
	}
}XT_MONITOR_START_REQ, *LPXT_MONITOR_START_REQ;

typedef struct XT_MONITOR_START_ACK //主管获取客服的所有接待买家
{
	uint8      ret;
	uint32     idSeller;//客服ID
	uint32     idBuyer;//客服当前聊天买家
	XT_MONITOR_START_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_ACK));
	}
}XT_MONITOR_START_ACK, *LPXT_MONITOR_START_ACK;

typedef struct XT_MONITOR_START_IND //主管获取客服的动作，聊天
{
	uint8      action;//0.客服发送消息；1.客服收到消息；
	uint32     idSeller;//客服ID
	uint32     idBuyer;//操作的买家
	XT_MSG     msg;//会话消息
	XT_MONITOR_START_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_START_IND));
	}
}XT_MONITOR_START_IND, *LPXT_MONITOR_START_IND;

//取消监控
typedef struct XT_MONITOR_END_REQ //主管取消监控客服
{
	uint32     idMaster;
	char       nicknameMaster[MAX_NICKNAME_LEN+1] ;//主管昵称
	uint32     idSeller;//客服ID
	XT_MONITOR_END_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_END_REQ));
	}
}XT_MONITOR_END_REQ, *LPXT_MONITOR_END_REQ;

typedef struct XT_MONITOR_END_ACK //主管取消监控客服应答
{
	uint8      ret;
	XT_MONITOR_END_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_END_ACK));
	}
}XT_MONITOR_END_ACK, *LPXT_MONITOR_END_ACK;

//插入会话
typedef struct XT_MONITOR_INSERT_CHAT_REQ //主管插入会话
{
	uint32     idMaster;//主管ID
	uint32     idSeller;//客服ID
	uint32     idBuyer;//买家ID
	XT_MONITOR_INSERT_CHAT_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_REQ));
	}
}XT_MONITOR_INSERT_CHAT_REQ, *LPXT_MONITOR_INSERT_CHAT_REQ;

typedef struct XT_MONITOR_INSERT_CHAT_ACK //主管插入会话应答
{
	uint8      ret;
	XT_MONITOR_INSERT_CHAT_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_ACK));
	}
}XT_MONITOR_INSERT_CHAT_ACK, *LPXT_MONITOR_INSERT_CHAT_ACK;

typedef struct XT_MONITOR_INSERT_CHAT_IND //客服和买家都要获取主管插入的会话
{
	uint32     idMaster;//主管ID
	//char       usernameMaster[MAX_USERNAME_LEN+1] ;//主管昵称
	uint8      action;//0.开启插入会话 1.插入会话
	XT_MSG     msg;//会话消息
	XT_MONITOR_INSERT_CHAT_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_INSERT_CHAT_IND));
	}
}XT_MONITOR_INSERT_CHAT_IND, *LPXT_MONITOR_INSERT_CHAT_IND;

//接手会话
typedef struct XT_MONITOR_TRAN_CHAT_REQ //主管接手会话
{
	uint32     idMaster;//主管ID
	uint32     idSeller;//客服ID
	uint32     idBuyer;//买家ID
	XT_MONITOR_TRAN_CHAT_REQ()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_REQ));
	}
}XT_MONITOR_TRAN_CHAT_REQ, *LPXT_MONITOR_TRAN_CHAT_REQ;

typedef struct XT_MONITOR_TRAN_CHAT_ACK //主管接手会话应答
{
	uint8      ret;
	XT_MONITOR_TRAN_CHAT_ACK()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_ACK));
	}
}XT_MONITOR_TRAN_CHAT_ACK, *LPXT_MONITOR_TRAN_CHAT_ACK;

typedef struct XT_MONITOR_TRAN_CHAT_IND //客服和买家收到主管接收会话
{
	uint32     idMaster;//主管ID
	XT_MONITOR_TRAN_CHAT_IND()
	{
		memset(this, 0, sizeof(XT_MONITOR_TRAN_CHAT_IND));
	}
}XT_MONITOR_TRAN_CHAT_IND, *LPXT_MONITOR_TRAN_CHAT_IND;

// 排队相关协议（服务端推送给客户端的，处于系统消息CMD_SERVERMSG_IND 0x60d之下）
//#define CMD_WAITQUEU_CHANGE		0x810F		// 等待队列增加成功或减少成员
typedef struct XT_WAITQUEU_CHANGE
{
	uint8	bAdd;		// 0：增加成员	1：减少成功
	uint32	idBuyer;	// 买家ID	
	uint32 	teamId;		// 所在组ID
//#if !defined(_VER_PREVIEW_) || !defined(_VER_ONLINE_)
	uint32  QueueLength;
//#endif
	char    nicknameBuyer[MAX_NICKNAME_LEN+1] ;// 买家昵称

	enum { TYPE_ADD = 0,TYPE_DELETE,TYPE_QUEUE};

	XT_WAITQUEU_CHANGE()
	{
		memset(this, 0, sizeof(XT_WAITQUEU_CHANGE));
	}
}XT_WAITQUEU_CHANGE, *LPXT_WAITQUEU_CHANGE;

// 排队相关协议（客户端发送给服务端的协议）
//#define CMD_RECEPTE_CUST_REQ	0x8110		// 接待客户请求
typedef struct XT_RECEPTE_CUST_REQ
{
	uint8	type;		// 请求类型  0、主动接待 1、退出接待
	uint32 	 teamId;	//所在组ID
	uint32	idBuyer;	// 买家ID	

	enum { TYPE_OFFER = 0,TYPE_QUIT,TYPE_AUTO_OFFER};

	XT_RECEPTE_CUST_REQ()
	{
		memset(this, 0, sizeof(XT_RECEPTE_CUST_REQ));
	}
}XT_RECEPTE_CUST_REQ, *LPXT_RECEPTE_CUST_REQ;
//#define CMD_RECEPTE_CUST_ACK	0x8111		// 接待客户应答
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

/////获取用户CSID请求
typedef struct XT_USER_MAP_CSID_REQ
{
	uint32     userid;
	XT_USER_MAP_CSID_REQ()
	{
		memset(this, 0, sizeof(XT_USER_MAP_CSID_REQ));
	}
}XT_USER_MAP_CSID_REQ, *LPXT_USER_MAP_CSID_REQ;

/////获取用户CSID响应
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
	uint32	cnt;				//需要查询的数量
	uint32	id[1];				//所有需要查询的主账号

	XT_GET_USER_STATUS_REQ()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_REQ));
	}
}XT_GET_USER_STATUS_REQ, *LPXT_GET_USER_STATUS_REQ;

typedef struct XT_GET_USER_STATUS_ACK
{
	uint32	cnt;				//数量

	struct
	{
		uint32	id;
		uint32	b;				//是否有在线子账号
	}stat[1];

	XT_GET_USER_STATUS_ACK()
	{
		memset(this, 0, sizeof(XT_GET_USER_STATUS_ACK));
	}

}XT_GET_USER_STATUS_ACK, *LPXT_GET_USER_STATUS_ACK;

typedef struct XT_WEB_MORE_MSG_REQ
{
	uint32 sid;//自己的id
	uint32 did;//对方的id
	uint32 msgtime;//请求msgtime之前的数据
	uint32 msgnum;//请求的消息的条数
	uint32 msgid;
	char uuid[MAX_UUID_LEN + 1];
	uint32 isMerchant; //Merchant :1 or subMerchant :0
}XT_WEB_MORE_MSG_REQ,*LPXT_WEB_MORE_MSG_REQ;

typedef struct XT_WEB_MORE_MSG_ACK
{
	uint8 ret;
	uint32 sid;//自己的id
	uint32 did;//对方的id
	uint32 msgtime;//请求msgtime之前的数据
	uint32 msgnum;//请求的消息的条数
	uint32 returnnum;//实际返回的条数
	uint8 over;//请求的数据是否发完 1 发完 0 未发完
	uint32 actualnum;
	char uuid[MAX_UUID_LEN + 1];
	struct MsgInfo//消息的信息
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
