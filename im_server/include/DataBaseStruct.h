/***************
*���ݿ�����ṹ����
***************/

#ifndef __DATASTRUCT_H__
#define __DATASTRUCT_H__

#include "StdPublic.h"
#include "PublicType.h"


//�û�������Ϣ
typedef struct STRU_BASIC_USERINFO
{
	uint32			iUserID								;	//�û�ID
	char			szUserName[MAX_USERNAME_LEN + 1]	;	//�û���
	char			szNickName[MAX_NICKNAME_LEN + 1]	;	//�û��ǳ�
	uint8		    iSex								;	//�û��Ա�
	uint8		    iHead								;	//ͷ�����
	char			szHeadUrl[MAX_HEADURL_LEN + 1]		;	//ͷ������

	STRU_BASIC_USERINFO()
	{
		memset(this,0,sizeof(STRU_BASIC_USERINFO));
	}

}STRU_BASIC_USERINFO;


//Ⱥ��Ա�б���Ϣ
typedef struct ST_FAMILY_MEMBER
{
	uint32		nUserID		;					//�û�ID
	uint8		nProp		;					//�û�����			
	char		nProfile[MAX_CLUSTER_MEMBER_PROFILE + 1];	// Ⱥ��Ƭ
	ST_FAMILY_MEMBER()
	{
		memset(this, 0, sizeof(ST_FAMILY_MEMBER));
	};
}ST_FAMILY_MEMBER, *LPST_FAMILY_MEMBER;


#endif

