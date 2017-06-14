#pragma once

#include "PublicType.h"

class COnlineUser
{
public:
	COnlineUser(void);
	~COnlineUser(void);
	uint32 m_UserID;			//用户ID编号

	uint32	m_ip;				//客户ip
	uint16	m_port;				//客户端口
	uint32  m_ServerIp;			//客户所在服务器ip
	char	m_UserName[MAX_USERNAME_LEN+1];		//用户名
	char	m_NickName[MAX_NICKNAME_LEN+1];		//呢称
	uint8	m_status;			//客户在线状态
	uint16	m_version;			//客户端版本
	uint8	m_rule;				//好友验证方式
	uint8	m_sexy;				//性别，0-男，１-女，２-未知
	uint32  m_LanIp;
	uint16	m_LanPort;
	uint8	m_sessionKey[MAX_SESSIONKEY_LEN];		//客户的session　key
	uint8	m_p2pSessionKey[MAX_SESSIONKEY_LEN];	//客户点对点的session key
	uint8   flag;
	uint8	m_flow;									//分流

	COnlineUser*	m_pNext;		//组成链表的后指针
	COnlineUser*	m_pPre;			//组成链表的前指针
};
