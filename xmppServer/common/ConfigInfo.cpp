#include "stdafx.h"
#include "ConfigInfo.h"
#include "ConfigDef.h"
#include "des.h"

#define _PATH_PROCNET_DEV               "/proc/net/dev"


void DecPass(char *indata, char *outdata)
{
//	DesDec((unsigned char *)outdata, (unsigned char *)indata, strlen(indata), (const unsigned char *) "secret11"); 
	strncpy(outdata, indata, 32);
}

CConfigInfo::CConfigInfo(void)
{
	m_szListenIP[0] = '\0';	
	m_szinListenIP[0] = '\0';
	m_szOnlineUserServerIP[0] = '\0';
	m_szOnlineFamilyServerIP[0] = '\0';
	m_szConnectServerIP[0] = '\0';
	m_szTransferServerIP[0] = '\0';
	m_ServerNo = -1;
	m_nMaxUserCount = 0;
	m_nMaxTransCount= 0;
	m_nIMLogClass=-1;
	m_nUserLogClass=-1;
	m_nFamilyLogClass=-1;
	m_nNotifyLogClass=-1;
	m_nMaxTdGrpA = -1;
	m_nMaxTdGrpB = -1;
	m_nMaxTdGrpC = -1;
	m_nTMaxTdGrp = -1;

	m_MySQLSid[0] = 0;
	m_MySQLUser[0] = 0;
	m_MySQLPwd[0] = 0;
	m_MySQLDb[0] = 0;
	
	m_szBuyerLoginURL[0] = '\0';
	m_szSellerLoginURL[0] = '\0';
	m_szMemcachedServerIp[0] = '\0';	//memcached服务器IP
	m_szMemcachedServerPort = 0;	//memcached服务器Port
	m_szMemcachedServerMagic = 0;

	m_szHeadImgURL[0] = '\0';
	m_szStoreNameURL[0] = '\0';
	m_HessianUrl[0] = '\0';		//hessian url地址	
    m_HttpServerIP[0] = '\0';   //httpserver ip地址
	m_HttpServerServerPort = 0;
 
	m_MasterIp[0] = '\0';				//主服务器IP
 
	GetDeviceIp(m_ServerIp);

#ifdef PRESS_TEST
	m_szMerchantInfo[0] = '\0';
#endif

}

CConfigInfo::~CConfigInfo(void)
{
}

char* CConfigInfo::GetMySQLSid()
{
	if( m_MySQLSid[0] == '\0')	
	{
		strncpy_s(m_MySQLSid,
			m_IniFile.GetString(CONFIG_SECTION_MYSQLSID_COMMON,CONFIG_KEY_MYSQLSID,"wh-testdb.fn.com"),
			sizeof(m_MySQLSid));
	}

	return m_MySQLSid;
}

char* CConfigInfo::GetMySQLUser()
{
	if( m_MySQLUser[0] == '\0')	
	{
		strncpy_s(m_MySQLUser,
			m_IniFile.GetString(CONFIG_SECTION_MYSQLUSER_COMMON,CONFIG_KEY_MYSQLUSER,"testdb"),
			sizeof(m_MySQLUser));
	}

	return m_MySQLUser;
}

char* CConfigInfo::GetMySQLPwd()
{
	if( m_MySQLPwd[0] == '\0')	
	{
		strncpy_s(m_MySQLPwd,
			m_IniFile.GetString(CONFIG_SECTION_MYSQLPWD_COMMON,CONFIG_KEY_MYSQLPWD,"P@ssw5rd1"),
			sizeof(m_MySQLPwd));
	}

	return m_MySQLPwd;
}

char* CConfigInfo::GetMySQLDb()
{
	if( m_MySQLDb[0] == '\0')	
	{
		strncpy_s(m_MySQLDb,
			m_IniFile.GetString(CONFIG_SECTION_MYSQLDB_COMMON,CONFIG_KEY_MYSQLDB,"member"),
			sizeof(m_MySQLDb));
	}

	return m_MySQLDb;
}

char* CConfigInfo::GetSSDBAddr()
{
	if( m_szSSDBIP[0] == '\0')	
	{
		strncpy_s(m_szSSDBIP,
			m_IniFile.GetString(CONFIG_SECTION_SSDB,CONFIG_KEY_SSDBADDR,"127.0.0.1"),
			sizeof(m_szSSDBIP));
	}

	return m_szSSDBIP;
}


char* CConfigInfo::GetImgServerAddr()
{
	if( m_szImgServerIp[0] == '\0')	
	{
		char * r = m_IniFile.GetString(CONFIG_SECTION_IMG,CONFIG_KEY_IMGADDR, "");
		strncpy_s(m_szImgServerIp,r, sizeof(m_szImgServerIp));
	}

	return m_szImgServerIp;
}


char* CConfigInfo::GetListenAddr()
{
	if( m_szListenIP[0] == '\0')	
	{
		strncpy_s(m_szListenIP,
			m_IniFile.GetString(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_IMIPADDR,m_ServerIp),
			sizeof(m_szListenIP));
	}

	return m_szListenIP;
}

char* CConfigInfo::GetinListenAddr()
{
	if( m_szinListenIP[0] == '\0')	
	{
		strncpy_s(m_szinListenIP,
			m_IniFile.GetString(CONFIG_SECTION_IMP2P,CONFIG_KEY_IMINIPADDR,m_ServerIp),
			sizeof(m_szinListenIP));
	}

	return m_szinListenIP;
}


char* CConfigInfo::GetWebMsgNotifyServerIP()
{
	if( m_szWebMsgNotifyServerIP[0] == '\0')
	{
		strncpy_s(m_szWebMsgNotifyServerIP,
			m_IniFile.GetString(CONFIG_SECTION_WEBMSGNOTIFY_COMMON,CINFIG_KEY_WEBMSGNOTIFY_SERVER,m_ServerIp),
			sizeof(m_szWebMsgNotifyServerIP));
	}
	return m_szWebMsgNotifyServerIP;
}

char* CConfigInfo::GetOnlineUserServerIP()
{
	if( m_szOnlineUserServerIP[0] == '\0')
	{
		strncpy_s(m_szOnlineUserServerIP,
			m_IniFile.GetString(CONFIG_SECTION_ONLINEUSER_COMMON,CONFIG_KEY_GLOBALONLINE_USER_SERVER,m_ServerIp),
			sizeof(m_szOnlineUserServerIP));
	}

	return m_szOnlineUserServerIP;
}
char* CConfigInfo::GetTransferServerIP()
{
	if( m_szTransferServerIP[0] == '\0')
	{
		strncpy_s(m_szTransferServerIP,
			m_IniFile.GetString(CONFIG_SECTION_TRANSFER_COMMON,CONFIG_KEY_TRANSFER_SERVER,m_ServerIp),
			sizeof(m_szTransferServerIP));
	}

	return m_szTransferServerIP;
}

char* CConfigInfo::GetOnlineFamilyServerIP()
{
	if( m_szOnlineFamilyServerIP[0] == '\0' )
	{
		strncpy_s(m_szOnlineFamilyServerIP,
				m_IniFile.GetString(CONFIG_SECTION_ONLINEFAMILY_COMMON,
									CONFIG_KEY_ONLINEFAMILY_SERVER,
									m_ServerIp),
				sizeof(m_szOnlineFamilyServerIP));
	}
	return m_szOnlineFamilyServerIP;
}

char* CConfigInfo::GetConnectServerIP()
{
	if( m_szConnectServerIP[0] == '\0' )
	{
		strncpy_s(m_szConnectServerIP,
			m_IniFile.GetString(CONFIG_SECTION_CONNECT_SERVER_COMMON,
			CONFIG_KEY_CONNECT_SERVER_IP,
			m_ServerIp),
			sizeof(m_szConnectServerIP));
	}
	return m_szConnectServerIP;
}

char* CConfigInfo::GetTcpIP()
{
	if( m_szTcpIP[0] == '\0' )
	{
		strncpy_s(m_szTcpIP,
			m_IniFile.GetString(CONFIG_SECTION_TCP,
			CONFIG_KEY_TCP,
			m_ServerIp),
			sizeof(m_szTcpIP));
	}
	return m_szTcpIP;
}

void CConfigInfo::GetWebMsgID(uint32 &OperationID,uint32 &UserMsgID,uint32 &SysPubID,uint32 &UserCommentID,uint32 &LogCommentID)
{
	CIniFile iniFile("webmsg.ini");
	OperationID = iniFile.GetInt("OperationMsgID","OperationMsgID",0);
	UserMsgID = iniFile.GetInt("UserMsgID","UserMsgID",0);
	SysPubID = iniFile.GetInt("SysMsgID","SysMsgID",0);
	UserCommentID = iniFile.GetInt("UserCommentID", "UserCommentID", 0);
	LogCommentID = iniFile.GetInt("LogCommentID", "LogCommentID", 0);
}

void CConfigInfo::SetWebMsgID(uint32 OperationID,uint32 UserMsgID,uint32 SysPubID,uint32 UserCommentID,uint32 LogCommentID)
{
	CIniFile iniFile("webmsg.ini");	
	iniFile.SetInt("OperationMsgID","OperationMsgID",OperationID);
	iniFile.SetInt("UserMsgID","UserMsgID",UserMsgID);
	iniFile.SetInt("SysMsgID","SysMsgID",SysPubID);
	iniFile.SetInt("UserCommentID","UserCommentID",UserCommentID);
	iniFile.SetInt("LogCommentID", "LogCommentID",LogCommentID);
}


char * CConfigInfo::GetNetInterfaceName(char *name, char *p)
{
    while (isspace(*p))
	{
		p++;
    }
	
    while (*p) 
	{
		if (isspace(*p))
	    {
	    	break;
		}
		if (*p == ':') 	/* could be an alias */
		{
			    char *dot = p, *dotname = name;
			    *name++ = *p++;
			    while (isdigit(*p))
				{
					*name++ = *p++;
			    }
		    if (*p != ':')/* it wasn't, backup */
			{	
				p = dot;
				name = dotname;
		    }
		    if (*p == '\0')
			{
				return NULL;
		    }
			p++;
		   	break;
		}
		
		*name++ = *p++;
    }
    *name++ = '\0';
    return p;
}

/**
 * read_netdev_proc - read net dev names form proc/net/dev
 * @devname: where to store dev names, devname[num][len]
 */
int CConfigInfo::ReadNetDevProc(void *devname, const int num, const int len)
{
    FILE *fh;
    char buf[512];
	int cnt = 0;
	char *dev = (char *)devname;

	if(devname == NULL || num < 1 || len < 4)
	{
//		printf("read_netdev_proc: para error\n");
		return -1;
	}

	memset(devname, 0, len * num);

    fh = fopen(_PATH_PROCNET_DEV, "r");
    if (!fh)
	{
		fprintf(stderr, "Warning: cannot open %s (%s). Limited output.\n",
			_PATH_PROCNET_DEV, strerror(errno)); 
		return -1;
	 }

    fgets(buf, sizeof buf, fh);	/* eat two line */
    fgets(buf, sizeof buf, fh);

	cnt = 0;
    while (fgets(buf, sizeof buf, fh) && cnt < num) 
	{
		char name[IFNAMSIZ];
		GetNetInterfaceName(name, buf);    

		strncpy((char *)dev, name, len);
		dev += len;
    }

    if (ferror(fh))
	{
		perror(_PATH_PROCNET_DEV);
    }

    fclose(fh);
     return 0;
}

/**
 * get_hwaddr - get netdevice mac addr 
 * @name: device name, e.g: eth0
 * @hwaddr: where to save mac, 6 byte hwaddr[6]
 * @return: 0 on success, -1 on failure
 */
int CConfigInfo::GetHwAddr(char *name, unsigned char *hwaddr)
{
	struct ifreq ifr;
	unsigned char memzero[6];
	int sock;

	if(name == NULL || hwaddr == NULL)
	{
//		printf("get_hwaddr: NULL para\n");
		return -1;
	}
 	sock = socket(AF_INET, SOCK_STREAM, 0);
	if(sock < 0)
	{
//		printf("get_hwaddr: socket error\n");
		return -1;
	}
 	//get eth1 mac addr
	memset(hwaddr, 0, 6);
	memset(&ifr, 0, sizeof(ifr));
	strncpy(ifr.ifr_name, name, 6);
	int ret = 0;
	if ((ret = ioctl(sock, SIOCGIFHWADDR, &ifr)) < 0)
	{
//		perror("get_hwaddr ioctl:");
		close(sock);
		return -1;
	}
	else
	{
		memcpy(hwaddr, ifr.ifr_hwaddr.sa_data, 6);
	}
	memset(memzero, 0, 6);
	if(memcmp(memzero, hwaddr, 6) == 0)
	{
//		printf("no mac\n");
		close(sock);
		return -1;
	}
	close(sock);
	return 0;
}

int CConfigInfo::GetIpAddr(char *name, struct in_addr &ipaddr)
{
	int inet_sock;
	struct ifreq ifr;
	inet_sock = socket(AF_INET, SOCK_DGRAM, 0);

	strcpy(ifr.ifr_name, name);
	//SIOCGIFADDR标志代表获取接口地址
	if (ioctl(inet_sock, SIOCGIFADDR, &ifr) <  0)
	{
		perror("ioctl");
	}
	else
	{
		ipaddr =((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr;
//		printf("%s\n", inet_ntoa(((struct sockaddr_in*)&(ifr.ifr_addr))->sin_addr));
	}
	close(inet_sock);
	return 0;
}

void CConfigInfo::PrintHex(void *hex, int len, char *tag)
{
	int i;
	unsigned char *p = (unsigned char *)hex;

	if(len < 1)
		return;

	for(i = 0; i < len - 1; i++){
		if(*p < 0x10)
			printf("0%x%s", *p++, tag);
		else
			printf("%2x%s", *p++, tag);
	}

	if(*p < 0x10)
		printf("0%x\n", *p++);
	else
		printf("%2x\n", *p++);
}

int CConfigInfo::GetDeviceNoEth(void)
{
	unsigned char hwaddr[6] = {0};
	char devname[3][7] = {0};
	ReadNetDevProc(devname, 3, 7);
	int mac = 0;

	for(int i = 0; i < 3; i++)
	{	
		if(GetHwAddr(devname[i], hwaddr) == 0)
		{
			if(strncmp(devname[i],"eth",3) == 0)
			{
				memcpy(&mac,hwaddr + 2,4);
				
				return mac;
			}
		}
	}	
	return rand();
}

int CConfigInfo::GetDeviceIp(void)
{
	struct in_addr ipaddr = {0};
	char devname[3][7] = {0};
	ReadNetDevProc(devname, 3, 7);
	int ip_ret = 0;

	for(int i = 0; i < 3; i++)
	{	
		if(GetIpAddr(devname[i], ipaddr) == 0)
		{
			if(strncmp(devname[i],"eth",3) == 0)
			{
				ip_ret = ipaddr.s_addr;
				
				return ip_ret;
			}
		}
	}	
	return rand();
}


int CConfigInfo::GetDeviceIp(char *ipaddr_io)
{
	struct in_addr ipaddr = {0};
	char devname[3][7] = {0};
	ReadNetDevProc(devname, 3, 7);
	int ip_ret = 0;

	for(int i = 0; i < 3; i++)
	{	
		if(GetIpAddr(devname[i], ipaddr) == 0)
		{
			if(strncmp(devname[i],"eth",3) == 0)
			{
				ip_ret = ipaddr.s_addr;
				strcpy(ipaddr_io,inet_ntoa(ipaddr));
				return ip_ret;
			}
		}
	}	
	return rand();
}


int CConfigInfo::GetServerNo()
{
	if( -1 == m_ServerNo)
	{
		m_ServerNo = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_SERVERNO,GetDeviceIp());
	}
	return m_ServerNo;
	
}

void CConfigInfo::GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid,char* familyuid,char* familypwd,char* familysid)
{
	char e_minipwd[128]={0}, e_webpwd[128]={0}, e_familypwd[128]={0};
	memset(miniuid, 0, 32);
	memset(minipwd, 0, 32);

	strncpy_s(miniuid,32,m_IniFile.GetString("MINIUID","MINIUID","mini139"),32);
	strncpy_s(e_minipwd,128,m_IniFile.GetString("MINIPWD",  "MINIPWD","mini139"),128);
	strncpy_s(minisid,32,m_IniFile.GetString("MINISID",  "MINISID","mini139"),32);
	strncpy_s(webuid,32,m_IniFile.GetString("WEBUID",  "WEBUID","mini139"),32);
	strncpy_s(e_webpwd,128,m_IniFile.GetString("WEBPWD",  "WEBPWD","mini139"),128);
	strncpy_s(websid,32,m_IniFile.GetString("WEBSID",  "WEBSID","mini139"),32);
	strncpy_s(familyuid,32,m_IniFile.GetString("FAMILYUID",  "FAMILYUID","mini139"),32);
	strncpy_s(e_familypwd,128,m_IniFile.GetString("FAMILYPWD",  "FAMILYPWD","mini139"),128);
	strncpy_s(familysid,32,m_IniFile.GetString("FAMILYSID",  "FAMILYSID","mini139"),32);

	DecPass(e_minipwd, minipwd);
	DecPass(e_webpwd, webpwd);
	DecPass(e_familypwd, familypwd);

//	printf("miniuid=%s,len=%d, minipwd=%s,len=%d, webuid=%s,len=%d, webpwd=%s,len=%d \n",
//		miniuid,strlen(miniuid),minipwd,strlen(minipwd),webuid,strlen(webuid),webpwd,strlen(webpwd));
}

void CConfigInfo::GetServerList(list<uint32> *pList)
{
	char szPath[MAX_PATH] = {0};
	::GetModuleFileName(NULL,szPath,sizeof(szPath));
	strcpy(strrchr(szPath,'/'),"/../config/ServerList.ini");
	FILE *pFile;
	pFile = fopen(szPath, "r");
	printf("GetServerList path is %s\r\n",szPath);
	fflush(stdout);
	if (pFile == NULL)
		return;
	char lines[100];
	while (fgets(lines, 100, pFile) != NULL)
	{
		uint32 ip = inet_addr(lines);
		pList->push_back(ip);
		printf("ServerList %s\r\n",lines);
	}
	fclose(pFile);
}

void CConfigInfo::GetSysLogRec(uint32 &recid)
{
	CIniFile iniFile("webmsg.ini");
	recid = iniFile.GetInt("SysLogID","SysLogID",0);
}

void CConfigInfo::SetSysLogRec(uint32 recid)
{
	CIniFile iniFile("webmsg.ini");
	iniFile.SetInt("SysLogID","SysLogID",recid);
}

void CConfigInfo::GetMobileRec(uint32 &recid)
{
	CIniFile iniFile("webmsg.ini");
	recid = iniFile.GetInt("MobileMsgID","MobileMsgID",0);
}

void CConfigInfo::SetMobileRec(uint32 recid)
{
	CIniFile iniFile("webmsg.ini");
	iniFile.SetInt("MobileMsgID","MobileMsgID",recid);
}


void CConfigInfo::GetUpdateUrl(char* strUpdateConfigBuyer, char* strUpdateConfigSeller, int& nCanUseVer, int& nAgreeVer, char* strUpdateOption, char* strUpdateInfo)
{
	strncpy_s(strUpdateConfigBuyer, 255, m_IniFile.GetString("UPDATE", "BuyerDownloadUrl", "http://downloads.woku.com/mini/UpdateConfig.xml"),255);
	strncpy_s(strUpdateConfigSeller, 255, m_IniFile.GetString("UPDATE", "SellerDownloadUrl", "http://downloads.woku.com/mini/UpdateConfig.xml"),255);
	nCanUseVer = m_IniFile.GetInt("UPDATE",  "LowVersionNo", 1001);
	nAgreeVer = m_IniFile.GetInt("UPDATE",  "NewVersionNo", 1003);
	strncpy_s(strUpdateOption, 512, m_IniFile.GetString("UPDATE", "UpdateOptionNo",""), 512);
	strncpy_s(strUpdateInfo, 255, m_IniFile.GetString("UPDATE",  "InfoMation",""), 255);
}

uint32 CConfigInfo::GetMaxUserCount()
{
	if( 0 == m_nMaxUserCount)
	{
		m_nMaxUserCount = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_IMMAX_USER_COUNT,20000);
	}
	return m_nMaxUserCount;
}

uint32 CConfigInfo::GetMaxTransCount()
{
	if( 0 == m_nMaxTransCount)
	{
		m_nMaxTransCount = m_IniFile.GetInt(CONFIG_SECTION_TRANSFER_COMMON,CONFIG_KEY_MAX_TRANS_COUNT,20000);
	}
	return m_nMaxTransCount;
}
/*
int CConfigInfo::GetIMLogClass()
{
	if(-1==m_nIMLogClass)
	{
		m_nIMLogClass = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_IM_LOGCLASS,3);
	}

	return m_nIMLogClass;
}
*/
	

const char* CConfigInfo::GetIMLogPath()
{
	if (m_szImLogConfigPath[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_IMCOMMON, CONFIG_KEY_IM_LOG_PATH, "./Log");
		strncpy_s(m_szImLogConfigPath, r, sizeof(m_szImLogConfigPath));
	}
	return m_szImLogConfigPath;
}

const char* CConfigInfo::GetUserLogPath()
{
	if (m_szUserLogConfigPath[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_ONLINEUSER_COMMON, CONFIG_KEY_ONLINEUSER_LOG_PATH, "./Log");
		strncpy_s(m_szUserLogConfigPath, r, sizeof(m_szUserLogConfigPath));
	}
	return m_szUserLogConfigPath;
}

const char* CConfigInfo::GetFamilyLogPath()
{
	if (m_szFamilyLogConfigPath[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_ONLINEFAMILY_COMMON, CONFIG_KEY_ONLINEFAMILY_LOG_PATH, "./Log");
		strncpy_s(m_szFamilyLogConfigPath, r, sizeof(m_szFamilyLogConfigPath));
	}
	return m_szFamilyLogConfigPath;
}

const char* CConfigInfo::GetNotifyLogPath()
{
	if (m_szNotifyLogConfigPath[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_WEBMSGNOTIFY_COMMON, CONFIG_KEY_WEBMSGNOTIFY_LOG_PATH, "./Log");
		strncpy_s(m_szNotifyLogConfigPath, r, sizeof(m_szNotifyLogConfigPath));
	}
	return m_szNotifyLogConfigPath;
}

const char* CConfigInfo::GetTransLogPath()
{
	if (m_szTransferLogConfigPath[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_TRANSFER_COMMON, CONFIG_KEY_TRANSFER_LOG_PATH, "./Log");
		strncpy_s(m_szTransferLogConfigPath, r, sizeof(m_szTransferLogConfigPath));
	}
	return m_szTransferLogConfigPath;
}


const char* CConfigInfo::GetIMLogClass()
{
	if (m_szImLogConfig[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_IMCOMMON, CONFIG_KEY_IM_LOGCLASS, "{\"error\":1}");
		strncpy_s(m_szImLogConfig, r, sizeof(m_szImLogConfig));
	}
	return m_szImLogConfig;
}


const char* CConfigInfo::GetUserLogClass()
{
	if (m_szUserLogConfig[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_ONLINEUSER_COMMON, CONFIG_KEY_ONLINEUSER_LOGCLASS, "{\"error\":1}");
		strncpy_s(m_szUserLogConfig, r, sizeof(m_szUserLogConfig));
	}
	return m_szUserLogConfig;
}


const char* CConfigInfo::GetFamilyLogClass()
{
	if (m_szFamilyLogConfig[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_ONLINEFAMILY_COMMON, CONFIG_KEY_ONLINEFAMILY_LOGCLASS, "{\"error\":1}");
		strncpy_s(m_szFamilyLogConfig, r, sizeof(m_szFamilyLogConfig));
	}
	return m_szFamilyLogConfig;
}


const char* CConfigInfo::GetNotifyLogClass()
{
	if (m_szNotifyLogConfig[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_WEBMSGNOTIFY_COMMON, CONFIG_KEY_WEBMSGNOTIFY_LOGCLASS, "{\"error\":1}");
		strncpy_s(m_szNotifyLogConfig, r, sizeof(m_szNotifyLogConfig));
	}
	return m_szNotifyLogConfig;
}

const char* CConfigInfo::GetTransLogClass()
{
	if (m_szTransferLogConfig[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_TRANSFER_COMMON, CONFIG_KEY_TRANSFER_LOGCLASS, "{\"error\":1}");
		strncpy_s(m_szTransferLogConfig, r, sizeof(m_szTransferLogConfig));
	}
	return m_szTransferLogConfig;
}


/*

int CConfigInfo::GetUserLogClass()
{
	if(-1==m_nUserLogClass)
	{
		m_nUserLogClass = m_IniFile.GetInt(CONFIG_SECTION_ONLINEUSER_COMMON,CONFIG_KEY_ONLINEUSER_LOGCLASS,3);
	}

	return m_nUserLogClass;
}

int CConfigInfo::GetFamilyLogClass()
{
	if(-1==m_nFamilyLogClass)
	{
		m_nFamilyLogClass = m_IniFile.GetInt(CONFIG_SECTION_ONLINEFAMILY_COMMON,CONFIG_KEY_ONLINEFAMILY_LOGCLASS,3);
	}

	return m_nFamilyLogClass;
}

int CConfigInfo::GetNotifyLogClass()
{
	if(-1==m_nNotifyLogClass)
	{
		m_nNotifyLogClass = m_IniFile.GetInt(CONFIG_SECTION_WEBMSGNOTIFY_COMMON,CONFIG_KEY_WEBMSGNOTIFY_LOGCLASS,3);
	}

	return m_nNotifyLogClass;
}
*/

int CConfigInfo::GetTMaxTdGrp()
{
	if( -1 == m_nTMaxTdGrp )
	{
		m_nTMaxTdGrp = m_IniFile.GetInt(CONFIG_SECTION_TRANSFER_COMMON,CONFIG_KEY_TMAXTHREADGROUP,1);
	}
	return m_nTMaxTdGrp;
}

int CConfigInfo::GetMaxTdGrpA()
{
	if( -1 == m_nMaxTdGrpA )
	{
		m_nMaxTdGrpA = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_MAXTHREADGROUPA,1);
	}
	return m_nMaxTdGrpA;
}

int CConfigInfo::GetMaxTdGrpB()
{
	if( -1 == m_nMaxTdGrpB )
	{
		m_nMaxTdGrpB = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_MAXTHREADGROUPB,1);
	}
	return m_nMaxTdGrpB;
}

int CConfigInfo::GetMaxTdGrpC()
{
	if( -1 == m_nMaxTdGrpC )
	{
		m_nMaxTdGrpC = m_IniFile.GetInt(CONFIG_SECTION_IMCOMMON,CONFIG_KEY_MAXTHREADGROUPC,1);
	}
	return m_nMaxTdGrpC;
}
const char* CConfigInfo::GetBuyerInfoURL()
{
	if (m_szBuyerSearchURL[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_SEARCH_URL, CONFIG_KEY_SEARCH_URL, "");
		strncpy_s(m_szBuyerSearchURL, r, sizeof(m_szBuyerSearchURL));
	}
	return m_szBuyerSearchURL;
}
const char* CConfigInfo::GetBuyerURL()
{
	if (m_szBuyerLoginURL[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_LOGIN_URL, CONIFG_KEY_BUYER_LOGIN_URL, "");
		strncpy_s(m_szBuyerLoginURL, r, sizeof(m_szBuyerLoginURL));
	}
	return m_szBuyerLoginURL;
}



const char* CConfigInfo::GetHeadImgURL()
{
	if (m_szHeadImgURL[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_HEADIMG_URL, CONFIG_KEY_HEADIMG_URL, "");
		strncpy_s(m_szHeadImgURL, r, sizeof(m_szHeadImgURL));
	}
	return m_szHeadImgURL;
}

const char* CConfigInfo::GetStoreNameURL()
{
	if (m_szStoreNameURL[0] == 0) {
	char * r = m_IniFile.GetString(CONFIG_SECTION_COMMON_URL, CONFIG_KEY_STORENAME_URL, "");
	strncpy_s(m_szStoreNameURL, r, sizeof(m_szStoreNameURL));
	}
	return m_szStoreNameURL;
}



const char* CConfigInfo::GetSellerURL()
{
	if (m_szSellerLoginURL[0] == 0) {
		char * r = m_IniFile.GetString(CONFIG_SECTION_LOGIN_URL, CONIFG_KEY_SELLER_LOGIN_URL, "");
		strncpy_s(m_szSellerLoginURL, r, sizeof(m_szSellerLoginURL));
	}
	return m_szSellerLoginURL;
}

//获取memcached配置
char* CConfigInfo::GetMemcachedAddr()
{
	if( m_szMemcachedServerIp[0] == '\0')	
	{
		strncpy_s(m_szMemcachedServerIp,
			m_IniFile.GetString(CONFIG_SECTION_MEMCACHED_COMMON,
			CONFIG_KEY_MEMCACHED_IPADDR,m_ServerIp),
			sizeof(m_szMemcachedServerIp));
	}
	return m_szMemcachedServerIp;
}

int CConfigInfo::GetMemcachedPort()
{
	if( m_szMemcachedServerPort == 0)	
	{	
		m_szMemcachedServerPort = m_IniFile.GetInt(CONFIG_SECTION_MEMCACHED_COMMON,
			CONFIG_KEY_MEMCACHED_IPPORT,11211);
	}
	return m_szMemcachedServerPort;
}

uint32 CConfigInfo::GetMemcachedMagic()
{
	if( m_szMemcachedServerMagic == 0)	
	{	
		m_szMemcachedServerMagic = m_IniFile.GetInt(CONFIG_SECTION_MEMCACHED_COMMON,
			CONFIG_KEY_MEMCACHED_MAGIC,1221);
	}
	return m_szMemcachedServerMagic;
}

char * CConfigInfo::GetHessianUrl()
{
	if( m_HessianUrl[0] == '\0')	
		{
			strncpy_s(m_HessianUrl,
				m_IniFile.GetString(CONFIG_SECTION_HESSIAN,
				CONFIG_HESSIAN_URL,"{\"total\":1,\"url1\":\"http://10.211.240.128:18080/im-logger/helloService\"}" ),
				sizeof(m_HessianUrl));
		}
		return m_HessianUrl;
}

char *CConfigInfo::GetMasterIp()
{
		if( m_MasterIp[0] == '\0')	
		{
			strncpy_s(m_MasterIp,
				m_IniFile.GetString(CONFIG_SECTION_SHELL,
				CONFIG_MASTER_ADDR,m_ServerIp),
				sizeof(m_MasterIp));
		}
		return m_MasterIp;
}

int CConfigInfo::IsMaster()
{	
	return strncmp(m_MasterIp,m_ServerIp,strlen(m_MasterIp));
}


char *CConfigInfo::GetHttpServerAddr()
{
	if(m_HttpServerIP[0] == '\0')
	{
		strncpy_s(m_HttpServerIP,
			m_IniFile.GetString(CONFIG_SECTION_HTTPSERVER,CONFIG_HTTPSERVER_IPADDR,m_ServerIp),
			sizeof(m_HttpServerIP));
	}
	return m_HttpServerIP;
}

int CConfigInfo::GetHttpServerPort()
{
	if( m_HttpServerServerPort == 0)	
	{	
		m_HttpServerServerPort = m_IniFile.GetInt(CONFIG_SECTION_HTTPSERVER,
			CONFIG_HTTPSERVER_PORT,80);
	}
	return m_HttpServerServerPort;
}

#ifdef PRESS_TEST
char * CConfigInfo::GetMerchantInfo()
{
	if( m_szMerchantInfo[0] == '\0')	
		{
			strncpy_s(m_szMerchantInfo,
				m_IniFile.GetString(CONFIG_SECTION_PRESS,
				CONFIG_PRESS_MID,"{\"total\":1,\"mid1\":0}" ),
				sizeof(m_szMerchantInfo));
		}
		return m_szMerchantInfo;
}
#endif

