#include "StdAfx.h"
#include ".\configinfo.h"
#include "ConfigDef.h"


CConfigInfo::CConfigInfo(void)
{
	m_szListenIP[0] = '\0';
	m_szOnlineUserServerIP[0] = '\0';
	m_szOnlineFamilyServerIP[0] = '\0';
	m_szConnectServerIP[0] = '\0';
	m_ServerNo = -1;
	m_nMaxUserCount = -1;
}

CConfigInfo::~CConfigInfo(void)
{
}


char* CConfigInfo::GetListenAddr()
{
	if( m_szListenIP[0] == '\0')	
	{
		strncpy(m_szListenIP,
			m_IniFile.GetString(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_IMIPADDR,"127.0.0.1"),
			sizeof(m_szListenIP));
	}

	return m_szListenIP;
}


char* CConfigInfo::GetOnlineUserServerIP()
{
	if( m_szOnlineUserServerIP[0] == '\0')
	{
		strncpy(m_szOnlineUserServerIP,
			m_IniFile.GetString(CONFIG_SECTION_ONLINEUSER_COMMON,CONFIG_KEY_GLOBALONLINE_USER_SERVER,"127.0.0.1"),
			sizeof(m_szOnlineUserServerIP));
	}

	return m_szOnlineUserServerIP;
}

char* CConfigInfo::GetOnlineFamilyServerIP()
{
	if( m_szOnlineFamilyServerIP[0] == '\0' )
	{
		strncpy(m_szOnlineFamilyServerIP,
				m_IniFile.GetString(CONFIG_SECTION_ONLINEFAMILY_COMMON,
									CONFIG_KEY_ONLINEFAMILY_SERVER,
									"127.0.0.1"),
				sizeof(m_szOnlineFamilyServerIP));
	}
	return m_szOnlineFamilyServerIP;
}

char* CConfigInfo::GetConnectServerIP()
{
	if( m_szConnectServerIP[0] == '\0' )
	{
		strncpy(m_szConnectServerIP,
			m_IniFile.GetString(CONFIG_SECTION_CONNECT_SERVER_COMMON,
			CONFIG_KEY_CONNECT_SERVER_IP,
			"127.0.0.1"),
			sizeof(m_szConnectServerIP));
	}
	return m_szConnectServerIP;
}

int CConfigInfo::GetServerNo()
{
	if( -1 == m_ServerNo)
	{
		m_ServerNo = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_SERVERNO,0);
	}
	return m_ServerNo;
}

void CConfigInfo::GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid)
{
	strcpy(miniuid,m_IniFile.GetString("MINIUID","MINIUID","mini139"));
	strcpy(minipwd,m_IniFile.GetString("MINIPWD",  "MINIPWD","mini139"));
	strcpy(minisid,m_IniFile.GetString("MINISID",  "MINISID","mini139"));
	strcpy(webuid,m_IniFile.GetString("WEBUID",  "WEBUID","mini139"));
	strcpy(webpwd,m_IniFile.GetString("WEBPWD",  "WEBPWD","mini139"));
	strcpy(websid,m_IniFile.GetString("WEBSID",  "WEBSID","mini139"));
}
void CConfigInfo::GetUpdateUrl(char * updateUrl,int &canusever, int& agreever, char* info)
{
	strcpy(updateUrl,m_IniFile.GetString("UPDATE","UpdateServerAddr","http://pub.139.com/mini139/LiveUpdate.asp"));
	canusever = m_IniFile.GetInt("LOWVERSION",  "LowVersionNo",0x0100);
	agreever = m_IniFile.GetInt("NEWVERSION",  "NewVersionNo",0x0100);
	strcpy(info,m_IniFile.GetString("UPDATEINFO",  "InfoMation",""));
}

uint32 CConfigInfo::GetMaxUserCount()
{
	if( -1 == m_nMaxUserCount)
	{
		m_nMaxUserCount = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_IMMAX_USER_COUNT,2000);
	}
	return m_nMaxUserCount;
}