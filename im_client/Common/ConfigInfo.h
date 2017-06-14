#pragma once

/************************************************************************ 
�ļ�������������Ϣ��������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-7-5 
************************************************************************/

#include "IniFile.h"


class CConfigInfo
{
public:
	CConfigInfo(void);
	~CConfigInfo(void);

	//��ȡ����ip��ַ
	char* GetListenAddr();

	//��ȡ���������ip
	char* GetConnectServerIP();

	//��ȡȫ�������û����������ip
	char* GetOnlineUserServerIP();

	//��ȡȫ�����߼�����������ip
	char* GetOnlineFamilyServerIP();

	//��ȡ���������
	int GetServerNo();

	//��ȡ���ݿ���������
	void GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid);
	void GetUpdateUrl(char * updateUrl,int &canusever, int &agreever, char* info);
	//��̨IM��������֧�ַ��������û���
	uint32 GetMaxUserCount();

private:
	CIniFile		m_IniFile;			//����ini�ļ�
	char	m_szListenIP[20];			//����ip��ַ
	char	m_szOnlineUserServerIP[20];	//ȫ�������û����������ip
	char	m_szOnlineFamilyServerIP[20];//ȫ�����߼�����������ip
	char	m_szConnectServerIP[20];	//���������ip
	int		m_ServerNo;					//���������

	uint32	m_nMaxUserCount;//��̨IM��������֧�ַ��������û���
};
