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
	uint32     recID	 ;	//��ˮ�š����ͺ���Ҫ�޸����ݿ�״̬
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