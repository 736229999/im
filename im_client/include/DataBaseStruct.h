/***************
*数据库操作结构定义
***************/

#ifndef __DATASTRUCT_H__
#define __DATASTRUCT_H__

#include "StdPublic.h"
#include "PublicType.h"


//用户基本信息
typedef struct STRU_BASIC_USERINFO
{
	uint32			iUserID								;	//用户ID
	char			szUserName[MAX_USERNAME_LEN + 1]	;	//用户名
	char			szNickName[MAX_NICKNAME_LEN + 1]	;	//用户昵称
	uint8		    iSex								;	//用户性别
	uint8		    iHead								;	//头像代码
	char			szHeadUrl[MAX_HEADURL_LEN + 1]		;	//头像链接

	STRU_BASIC_USERINFO()
	{
		memset(this,0,sizeof(STRU_BASIC_USERINFO));
	}

}STRU_BASIC_USERINFO;


//群成员列表信息
typedef struct ST_FAMILY_MEMBER
{
	uint32		nUserID		;					//用户ID
	uint8		nProp		;					//用户属性
	ST_FAMILY_MEMBER()
	{
		memset(this, 0, sizeof(ST_FAMILY_MEMBER));
	};
}ST_FAMILY_MEMBER, *LPST_FAMILY_MEMBER;


typedef struct ST_OFFLINE_MSG
{	
	uint32	    recvID   ;
	uint32		sendID   ;
	uint32		tm;
	uint16		msgtype       ;  
	char        msg[MAX_MSG_LEN+1];
	uint16		msglen;

	ST_OFFLINE_MSG()
	{
		memset(this,0,sizeof(ST_OFFLINE_MSG));
	}

} ST_OFFLINE_MSG, *LPST_OFFLINE_MSG;

typedef struct ST_WEB_MSG
{	
	uint32     recID	 ;	//流水号。发送后需要修改数据库状态
	uint32	    recvID   ;
	uint32		sendID   ;
	char		senderName[MAX_USERNAME_LEN + 1];
	char		sendTime[32];
	uint16		msgtype       ;  
	char        msg[MAX_MSG_LEN + 1];
	uint16		msglen;

	ST_WEB_MSG()
	{
		memset(this,0,sizeof(ST_OFFLINE_MSG));
	}

} ST_WEB_MSG, *LPST_WEB_MSG;
#endif