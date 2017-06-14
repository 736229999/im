#pragma once

#include "PublicType.h"

class COnlineUser
{
public:
	COnlineUser(void);
	~COnlineUser(void);
	uint32 m_UserID;			//�û�ID���

	uint32	m_ip;				//�ͻ�ip
	uint16	m_port;				//�ͻ��˿�
	uint32  m_ServerIp;			//�ͻ����ڷ�����ip
	char	m_UserName[MAX_USERNAME_LEN+1];		//�û���
	char	m_NickName[MAX_NICKNAME_LEN+1];		//�س�
	uint8	m_status;			//�ͻ�����״̬
	uint16	m_version;			//�ͻ��˰汾
	uint8	m_rule;				//������֤��ʽ
	uint8	m_sexy;				//�Ա�0-�У���-Ů����-δ֪
	uint32  m_LanIp;
	uint16	m_LanPort;
	uint8	m_sessionKey[MAX_SESSIONKEY_LEN];		//�ͻ���session��key
	uint8	m_p2pSessionKey[MAX_SESSIONKEY_LEN];	//�ͻ���Ե��session key
	uint8   flag;
	uint8	m_flow;									//����

	COnlineUser*	m_pNext;		//�������ĺ�ָ��
	COnlineUser*	m_pPre;			//��������ǰָ��
};
