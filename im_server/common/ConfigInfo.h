#pragma once

/************************************************************************ 
文件描述：配置信息类的声明文件

创建人：hechang
时间：	2006-7-5
************************************************************************/
 #include <stdlib.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/socket.h>
#include <string.h>
#include <vector>
#include <string>
#include <sys/types.h>
#include <arpa/inet.h>
#include <features.h>    /* for the glibc version number */
#if __GLIBC__ >= 2 && __GLIBC_MINOR >= 1
#include <netpacket/packet.h>
#include <net/ethernet.h>     /* the L2 protocols */
#else
#include <asm/types.h>
#include <linux/if_packet.h>
#include <linux/if_ether.h>   /* The L2 protocols */
#endif
#include <stdio.h>
#include <netinet/in.h>
#include <net/if.h>
#include <sys/ioctl.h>
#include <errno.h>
#include "IniFile.h"


class CConfigInfo
{
public:
	CConfigInfo(void);
	~CConfigInfo(void);

	//获取SSDB ip地址
	char* GetSSDBAddr();
	//获取httpserver ip 地址
	char* GetHttpServerAddr();
	//获取httpserver 端口
	int GetHttpServerPort();

	//获取图片服务器地址
	char* GetImgServerAddr();

	//获取监听ip地址
	char* GetListenAddr();

	//获取服务器内部通信IP
	char* GetinListenAddr();

	//获取接入服务器ip
	char* GetConnectServerIP();

	//获取全局在线用户管理服务器ip
	char* GetOnlineUserServerIP();

	//获取全局在线家族管理服务器ip
	char* GetOnlineFamilyServerIP();
	
	//获取中转服务器IP
	char* GetTransferServerIP();

	//获取网站消息通知服务器IP
	char* GetWebMsgNotifyServerIP();

	char* GetTcpIP();
	
	char* GetImLoggerBaseUrl();

	//MySQL配置
	char* GetMySQLSid();
	char* GetMySQLUser();
	char* GetMySQLPwd();
	char* GetMySQLDb();

	void GetWebMsgID(uint32 &OperationID,uint32 &UserMsgID,uint32 &SysPubID,uint32 &UserCommentID,uint32 &LogCommentID);
	void SetWebMsgID(uint32 OperationID,uint32 UserMsgID,uint32 SysPubID,uint32 UserCommentID,uint32 LogCommentID);
	void GetSysLogRec(uint32 &recid);
	void SetSysLogRec(uint32 recid);
	void GetMobileRec(uint32 &recid);
	void SetMobileRec(uint32 recid);

	//获取服务器编号
	
	int GetServerNo();

	//获取数据库连接配置
	void GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid,char* familyuid,char* familypwd,char* familysid);
	void GetUpdateUrl(char* strUpdateConfigBuyer, char* strUpdateConfigSeller, int& nCanUseVer, int& nAgreeVer, char* strUpdateOption, char* strUpdateInfo);
	void GetCSUpdateUrl(char* strUpdateConfigBuyer, char* strUpdateConfigSeller, int& nCanUseVer, int& nAgreeVer, char* strUpdateOption, char* strUpdateInfo);

	//本台IM服务器能支持服务的最大用户数
	uint32 GetMaxUserCount();
	uint32 GetMaxTransCount();
	void GetServerList(list<uint32> *pList);

	//获取日志级别
	//参见CLog::LOG_CLASS: 0-全部关闭 1-只屏幕输出 2-只文件输出 3-屏幕和文件输出 4-全部输出，包括DEBUG信息
	/*int GetIMLogClass();
	int GetUserLogClass();
	int GetFamilyLogClass();
	int GetNotifyLogClass();
	*/

	const char* GetIMLogPath();	
	const char* GetUserLogPath();	
	const char* GetFamilyLogPath();	
	const char* GetNotifyLogPath();	
	const char* GetTransLogPath();
	const char* GetAccessServerLogPath();
	
	//新的日志配置，以json串来配置
	
	const char* GetIMLogClass();
	const char* GetUserLogClass();
	const char* GetFamilyLogClass();
	const char* GetNotifyLogClass();
	const char* GetTransLogClass();
	const char* GetAccessServerLogClass();

	//获取最大线程数设置
	int GetMaxTdGrpA();
	int GetMaxTdGrpB();
	int GetMaxTdGrpC();
	int GetTMaxTdGrp();

	const char* GetBuyerURL();
	const char* GetSellerURL();
	//工作版
	const char* GetWorkURL();
	const char* GetBuyerLevelURL();

	const char* GetBuyerInfoURL();

	const char* GetHeadImgURL();

	//Memcached获取IP和端口
	char* GetMemcachedAddr();
	int GetMemcachedPort();
	uint32 GetMemcachedMagic();
	//hessian获取IP和方法
	char *GetHessianUrl();
	//获取主IP
	char *GetMasterIp();
	//判断是否是主服务器
	int IsMaster();
	int GetDeviceIp(void);
	int GetDeviceIp(char *ipaddr_io);
	//获取zookeeperip和端口
	char* GetZookeeperAddr();
	int GetZookeeperPort();
	char *GetKafuKaBroker();
	char *GetKafuKaTopic();	
	char *GetKafuKaTalkMsgTopic();

#ifdef PRESS_TEST
	char* GetMerchantInfo();
#endif
	// 获取客服工作台客服分组配置的url
	char * GetCsGroupUrl();
	// 获取客服分组ID
	int GetCsGroupID();
	// 获取客服工作台客服真实姓名的url
	char * GetCsNameUrl();

	// 获取Aes加密算法的key
	char * GetAesKey();

	char* GetKafkaZookeeper();
	char* GetTopic_PayOrder();
	char* GetTopic_NewOrder();
	char* GetTopic_GoodsComment();
	char* GetTopic_ReturnOrder();

	char* GetUrlOrderDetail();

	char* GetUrlIsSameAreaLogin();
	char* GetUrlIsBindMobile();

	// 获取退货详情Url
	char* GetReturnInfoUrl();
	// 获取imapi接口http的Url
	char* GetimapiUrl();
	// 获取imweb买家状态接口http的Url
	char* GetwebimbStateUrl();

	// 获取消息推送服务的IP
	void GetMsgNotifyIP(std::vector<std::string> &vecIPs);

	// 获取接入服务器IP列表
	void GetAccessServerIPs(std::vector<std::string> &vecIPs);

	// 获取接入服务器端口
	int GetAccessServerPort();

	// 获取好友列表的url
	char* GetFriendListUrl();
	//获取好友组列表的URL
	char* GetFriendGroupListUrl();
	//获取离线消息的URL
	char* GetUserOfflineMSGUrl();

	// 添加分组url
	char* GetAddGroupUrl();
	// 修改分组url
	char* GetModGroupUrl();
	// 删除分组url
	char* GetDelGroupUrl();

	// 添加好友url
	char* GetAddFriendUrl();
	// 修改好友分组url
	char* GetModFriendGroupUrl();
	// 删除好友url
	char* GetDelFriendUrl();

	// 获取最近联系商家url
	char* GetRecentSubMerchantUrl();

private:	
	char * GetNetInterfaceName(char *name, char *p);	
	
	/**
	 * read_netdev_proc - read net dev names form proc/net/dev
	 * @devname: where to store dev names, devname[num][len]
	 */
	int ReadNetDevProc(void *devname, const int num, const int len);
	
	/**
	 * get_hwaddr - get netdevice mac addr 
	 * @name: device name, e.g: eth0
	 * @hwaddr: where to save mac, 6 byte hwaddr[6]
	 * @return: 0 on success, -1 on failure
	 */
	int GetHwAddr(char *name, unsigned char *hwaddr);	
	void PrintHex(void *hex, int len, char *tag);	
	int GetDeviceNoEth(void);
	
	int GetIpAddr(char *name, struct in_addr &ipaddr);
	
	

	
	CIniFile		m_IniFile;			//配置ini文件
	char	m_szImgServerIp[128];		//图片服务器ip
	char    m_szSSDBIP[20];				//SSDB服务器ip
	char	m_szListenIP[20];			//监听ip地址
	char	m_szinListenIP[20];			//内部监听ip地址
	char	m_szOnlineUserServerIP[20];	//全局在线用户管理服务器ip
	char	m_szOnlineFamilyServerIP[20];//全局在线家族管理服务器ip
	char	m_szTransferServerIP[20];	//中转服务器IP
	char	m_szConnectServerIP[20];	//接入服务器ip
	char	m_szWebMsgNotifyServerIP[20]; //网站消息通知服务器IP
	char	m_szTcpIP[20];
	int		m_ServerNo;					//服务器编号
	char	m_ServerIp[20];				//本机IP地址
	char    m_MasterIp[20];				//主服务器IP
	char    m_MySQLSid[32];
	char    m_MySQLUser[32];
	char    m_MySQLPwd[32];
	char    m_MySQLDb[32];

	int		m_nIMLogClass;				//IM服务日志级别
	int		m_nUserLogClass;			//全局在线服务日志级别
	int		m_nFamilyLogClass;		//全局家族在线日志级别
	int		m_nNotifyLogClass;		//消息提醒服务日志级别

	int		m_nMaxTdGrpA;				//BusinessProcA最大线程数
	int		m_nMaxTdGrpB;				//BusinessProcB最大线程数
	int		m_nMaxTdGrpC;				//BusinessProcC最大线程数
	int		m_nTMaxTdGrp;				//文件传输最大线程数

	uint32	m_nMaxUserCount;//本台IM服务器能支持服务的最大用户数
	uint32 m_nMaxTransCount; //中传服务器支持最大用户数
	
	char	m_szBuyerLoginURL[128];		//买家登录URL
	char	m_szSellerLoginURL[128];	//卖家登录URL
	char	m_szWorkLoginURL[128];		//工作版登录URL
	char	m_szBuyerSearchURL[128]; //会员查询URL
	char 	m_szBuyerLevelURL[128];		//买家等级URL
	
	char    m_szHeadImgURL[128];//头像url

	char	m_szImLogConfigPath[256];		//imserver日志路径
	char 	m_szUserLogConfigPath[256];		//User  日志路径
	char	m_szFamilyLogConfigPath[256];	//iFamily	日志路径
	char 	m_szNotifyLogConfigPath[256];	//Notify  日志路径
	char    m_szTransferLogConfigPath[256]; //Transfer 日志路径
	char	m_szAccessServerLogConfigPath[256];		//接入服务器日志路径
	
	char 	m_szImLogConfig[256];		//imserver  日志配置
	char 	m_szUserLogConfig[256];		//imserver  日志配置
	char	m_szFamilyLogConfig[256];	//imserver	日志配置
	char 	m_szNotifyLogConfig[256];	//imserver  日志配置
	char    m_szTransferLogConfig[256]; //imTransfer 日志配置
	char 	m_szAccessServerLogConfig[256];		//接入服务器  日志配置

	char  	m_szMemcachedServerIp[256];	//memcached服务器IP
	int	 	m_szMemcachedServerPort;	//memcached服务器Port
	uint32	m_szMemcachedServerMagic;		//memcached标志位

	char    m_HessianUrl[512];		//hessian url地址
	

	char    m_HttpServerIP[20];   //httpserver ip地址
	int     m_HttpServerServerPort; //httpserver port
#ifdef PRESS_TEST
	char   m_szMerchantInfo[512]; //卖家ID号上下文
#endif
	
	char    m_ZookeeperServerIP[20];//zookeeper server ip地址
	int 	m_ZookeeperServerPort;//zookeeper server port

	char    m_KafKa_brokers[128];
	char    m_KafKa_topics[128];
	char 	m_KafKa_talkmsg_topics[128];
	
	char 	m_szCsGroupUrl[128];	// 获取客服工作台客服分组的url
	int		m_nCsGroupID;			// 分组id
	char 	m_szCsNameUrl[128];		// 获取客服真实姓名的url

	char 	m_szAesKey[64];			// AES加密的key	

	char 	m_szOrderDetail[128];	// 订单详情

	char 	m_szIsSameAreaLoginUrl[128];	// 是否同一地址登录
	char 	m_szIsBindMobileUrl[128];		// 是否绑定手机

	char 	m_szReturnInfo[128];	// 订单详情
	char 	m_imApiUrl[128];		//imapi http接口
	std::vector<std::string> 	m_vecMsgNotifyIP;	// 消息推送服务所在的机器	

	std::vector<std::string> m_vecAccessServerIP;		// 接入服务器IP
	int m_nAccessServerPort;							// 接入服务器端口
	char    m_sImLoggerBaseUrl[128];  //im-logger http 接口
	char 	m_webimbStateUrl[128];		 //im-webim http 接口
	char 	m_szFriendListUrl[128];		// im-api http 接口 获取好友列表
	char 	m_szFriendGroupListUrl[128];		// im-api http 接口 获取好友分组列表
	char 	m_szUserOfflineMSG[128];		// im-api http 接口 获取用户离线消息
	
	char 	m_szAddGroupUrl[128];		// im-api接口 添加分组
	char 	m_szModGroupUrl[128];		// im-api接口 修改分组
	char 	m_szDelGroupUrl[128];		// im-api接口 删除分组

	char 	m_szAddFriendUrl[128];		// im-api接口 添加好友
	char 	m_szModFriendGroupUrl[128];	// im-api接口 修改好友分组
	char 	m_szDelFriendUrl[128];		// im-api接口 删除好友

	char 	m_szGetRecentSubMerchantUrl[128];	// im-api接口 获取最近联系商家
};

