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
	char		nProfile[MAX_CLUSTER_MEMBER_PROFILE + 1];	// 群名片
	ST_FAMILY_MEMBER()
	{
		memset(this, 0, sizeof(ST_FAMILY_MEMBER));
	};
}ST_FAMILY_MEMBER, *LPST_FAMILY_MEMBER;


#endif

