#pragma once

/************************************************************************ 
文件描述：配置信息类的声明文件 

创建人：hechang
时间：	2006-7-5 
************************************************************************/

#include "IniFile.h"


class CConfigInfo
{
public:
	CConfigInfo(void);
	~CConfigInfo(void);

	//获取监听ip地址
	char* GetListenAddr();

	//获取接入服务器ip
	char* GetConnectServerIP();

	//获取全局在线用户管理服务器ip
	char* GetOnlineUserServerIP();

	//获取全局在线家族管理服务器ip
	char* GetOnlineFamilyServerIP();

	//获取服务器编号
	int GetServerNo();

	//获取数据库连接配置
	void GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid);
	void GetUpdateUrl(char * updateUrl,int &canusever, int &agreever, char* info);
	//本台IM服务器能支持服务的最大用户数
	uint32 GetMaxUserCount();

private:
	CIniFile		m_IniFile;			//配置ini文件
	char	m_szListenIP[20];			//监听ip地址
	char	m_szOnlineUserServerIP[20];	//全局在线用户管理服务器ip
	char	m_szOnlineFamilyServerIP[20];//全局在线家族管理服务器ip
	char	m_szConnectServerIP[20];	//接入服务器ip
	int		m_ServerNo;					//服务器编号

	uint32	m_nMaxUserCount;//本台IM服务器能支持服务的最大用户数
};
