#pragma once

/************************************************************************ 
�ļ�������������Ϣ��������ļ�

�����ˣ�hechang
ʱ�䣺	2006-7-5
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

	//��ȡSSDB ip��ַ
	char* GetSSDBAddr();
	//��ȡhttpserver ip ��ַ
	char* GetHttpServerAddr();
	//��ȡhttpserver �˿�
	int GetHttpServerPort();

	//��ȡͼƬ��������ַ
	char* GetImgServerAddr();

	//��ȡ����ip��ַ
	char* GetListenAddr();

	//��ȡ�������ڲ�ͨ��IP
	char* GetinListenAddr();

	//��ȡ���������ip
	char* GetConnectServerIP();

	//��ȡȫ�������û����������ip
	char* GetOnlineUserServerIP();

	//��ȡȫ�����߼�����������ip
	char* GetOnlineFamilyServerIP();
	
	//��ȡ��ת������IP
	char* GetTransferServerIP();

	//��ȡ��վ��Ϣ֪ͨ������IP
	char* GetWebMsgNotifyServerIP();

	char* GetTcpIP();
	
	char* GetImLoggerBaseUrl();

	//MySQL����
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

	//��ȡ���������
	
	int GetServerNo();

	//��ȡ���ݿ���������
	void GetDataBaseConfig(char* miniuid, char* minipwd, char* minisid, char* webuid, char* webpwd, char* websid,char* familyuid,char* familypwd,char* familysid);
	void GetUpdateUrl(char* strUpdateConfigBuyer, char* strUpdateConfigSeller, int& nCanUseVer, int& nAgreeVer, char* strUpdateOption, char* strUpdateInfo);
	void GetCSUpdateUrl(char* strUpdateConfigBuyer, char* strUpdateConfigSeller, int& nCanUseVer, int& nAgreeVer, char* strUpdateOption, char* strUpdateInfo);

	//��̨IM��������֧�ַ��������û���
	uint32 GetMaxUserCount();
	uint32 GetMaxTransCount();
	void GetServerList(list<uint32> *pList);

	//��ȡ��־����
	//�μ�CLog::LOG_CLASS: 0-ȫ���ر� 1-ֻ��Ļ��� 2-ֻ�ļ���� 3-��Ļ���ļ���� 4-ȫ�����������DEBUG��Ϣ
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
	
	//�µ���־���ã���json��������
	
	const char* GetIMLogClass();
	const char* GetUserLogClass();
	const char* GetFamilyLogClass();
	const char* GetNotifyLogClass();
	const char* GetTransLogClass();
	const char* GetAccessServerLogClass();

	//��ȡ����߳�������
	int GetMaxTdGrpA();
	int GetMaxTdGrpB();
	int GetMaxTdGrpC();
	int GetTMaxTdGrp();

	const char* GetBuyerURL();
	const char* GetSellerURL();
	//������
	const char* GetWorkURL();
	const char* GetBuyerLevelURL();

	const char* GetBuyerInfoURL();

	const char* GetHeadImgURL();

	//Memcached��ȡIP�Ͷ˿�
	char* GetMemcachedAddr();
	int GetMemcachedPort();
	uint32 GetMemcachedMagic();
	//hessian��ȡIP�ͷ���
	char *GetHessianUrl();
	//��ȡ��IP
	char *GetMasterIp();
	//�ж��Ƿ�����������
	int IsMaster();
	int GetDeviceIp(void);
	int GetDeviceIp(char *ipaddr_io);
	//��ȡzookeeperip�Ͷ˿�
	char* GetZookeeperAddr();
	int GetZookeeperPort();
	char *GetKafuKaBroker();
	char *GetKafuKaTopic();	
	char *GetKafuKaTalkMsgTopic();

#ifdef PRESS_TEST
	char* GetMerchantInfo();
#endif
	// ��ȡ�ͷ�����̨�ͷ��������õ�url
	char * GetCsGroupUrl();
	// ��ȡ�ͷ�����ID
	int GetCsGroupID();
	// ��ȡ�ͷ�����̨�ͷ���ʵ������url
	char * GetCsNameUrl();

	// ��ȡAes�����㷨��key
	char * GetAesKey();

	char* GetKafkaZookeeper();
	char* GetTopic_PayOrder();
	char* GetTopic_NewOrder();
	char* GetTopic_GoodsComment();
	char* GetTopic_ReturnOrder();

	char* GetUrlOrderDetail();

	char* GetUrlIsSameAreaLogin();
	char* GetUrlIsBindMobile();

	// ��ȡ�˻�����Url
	char* GetReturnInfoUrl();
	// ��ȡimapi�ӿ�http��Url
	char* GetimapiUrl();
	// ��ȡimweb���״̬�ӿ�http��Url
	char* GetwebimbStateUrl();

	// ��ȡ��Ϣ���ͷ����IP
	void GetMsgNotifyIP(std::vector<std::string> &vecIPs);

	// ��ȡ���������IP�б�
	void GetAccessServerIPs(std::vector<std::string> &vecIPs);

	// ��ȡ����������˿�
	int GetAccessServerPort();

	// ��ȡ�����б��url
	char* GetFriendListUrl();
	//��ȡ�������б��URL
	char* GetFriendGroupListUrl();
	//��ȡ������Ϣ��URL
	char* GetUserOfflineMSGUrl();

	// ��ӷ���url
	char* GetAddGroupUrl();
	// �޸ķ���url
	char* GetModGroupUrl();
	// ɾ������url
	char* GetDelGroupUrl();

	// ��Ӻ���url
	char* GetAddFriendUrl();
	// �޸ĺ��ѷ���url
	char* GetModFriendGroupUrl();
	// ɾ������url
	char* GetDelFriendUrl();

	// ��ȡ�����ϵ�̼�url
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
	
	

	
	CIniFile		m_IniFile;			//����ini�ļ�
	char	m_szImgServerIp[128];		//ͼƬ������ip
	char    m_szSSDBIP[20];				//SSDB������ip
	char	m_szListenIP[20];			//����ip��ַ
	char	m_szinListenIP[20];			//�ڲ�����ip��ַ
	char	m_szOnlineUserServerIP[20];	//ȫ�������û����������ip
	char	m_szOnlineFamilyServerIP[20];//ȫ�����߼�����������ip
	char	m_szTransferServerIP[20];	//��ת������IP
	char	m_szConnectServerIP[20];	//���������ip
	char	m_szWebMsgNotifyServerIP[20]; //��վ��Ϣ֪ͨ������IP
	char	m_szTcpIP[20];
	int		m_ServerNo;					//���������
	char	m_ServerIp[20];				//����IP��ַ
	char    m_MasterIp[20];				//��������IP
	char    m_MySQLSid[32];
	char    m_MySQLUser[32];
	char    m_MySQLPwd[32];
	char    m_MySQLDb[32];

	int		m_nIMLogClass;				//IM������־����
	int		m_nUserLogClass;			//ȫ�����߷�����־����
	int		m_nFamilyLogClass;		//ȫ�ּ���������־����
	int		m_nNotifyLogClass;		//��Ϣ���ѷ�����־����

	int		m_nMaxTdGrpA;				//BusinessProcA����߳���
	int		m_nMaxTdGrpB;				//BusinessProcB����߳���
	int		m_nMaxTdGrpC;				//BusinessProcC����߳���
	int		m_nTMaxTdGrp;				//�ļ���������߳���

	uint32	m_nMaxUserCount;//��̨IM��������֧�ַ��������û���
	uint32 m_nMaxTransCount; //�д�������֧������û���
	
	char	m_szBuyerLoginURL[128];		//��ҵ�¼URL
	char	m_szSellerLoginURL[128];	//���ҵ�¼URL
	char	m_szWorkLoginURL[128];		//�������¼URL
	char	m_szBuyerSearchURL[128]; //��Ա��ѯURL
	char 	m_szBuyerLevelURL[128];		//��ҵȼ�URL
	
	char    m_szHeadImgURL[128];//ͷ��url

	char	m_szImLogConfigPath[256];		//imserver��־·��
	char 	m_szUserLogConfigPath[256];		//User  ��־·��
	char	m_szFamilyLogConfigPath[256];	//iFamily	��־·��
	char 	m_szNotifyLogConfigPath[256];	//Notify  ��־·��
	char    m_szTransferLogConfigPath[256]; //Transfer ��־·��
	char	m_szAccessServerLogConfigPath[256];		//�����������־·��
	
	char 	m_szImLogConfig[256];		//imserver  ��־����
	char 	m_szUserLogConfig[256];		//imserver  ��־����
	char	m_szFamilyLogConfig[256];	//imserver	��־����
	char 	m_szNotifyLogConfig[256];	//imserver  ��־����
	char    m_szTransferLogConfig[256]; //imTransfer ��־����
	char 	m_szAccessServerLogConfig[256];		//���������  ��־����

	char  	m_szMemcachedServerIp[256];	//memcached������IP
	int	 	m_szMemcachedServerPort;	//memcached������Port
	uint32	m_szMemcachedServerMagic;		//memcached��־λ

	char    m_HessianUrl[512];		//hessian url��ַ
	

	char    m_HttpServerIP[20];   //httpserver ip��ַ
	int     m_HttpServerServerPort; //httpserver port
#ifdef PRESS_TEST
	char   m_szMerchantInfo[512]; //����ID��������
#endif
	
	char    m_ZookeeperServerIP[20];//zookeeper server ip��ַ
	int 	m_ZookeeperServerPort;//zookeeper server port

	char    m_KafKa_brokers[128];
	char    m_KafKa_topics[128];
	char 	m_KafKa_talkmsg_topics[128];
	
	char 	m_szCsGroupUrl[128];	// ��ȡ�ͷ�����̨�ͷ������url
	int		m_nCsGroupID;			// ����id
	char 	m_szCsNameUrl[128];		// ��ȡ�ͷ���ʵ������url

	char 	m_szAesKey[64];			// AES���ܵ�key	

	char 	m_szOrderDetail[128];	// ��������

	char 	m_szIsSameAreaLoginUrl[128];	// �Ƿ�ͬһ��ַ��¼
	char 	m_szIsBindMobileUrl[128];		// �Ƿ���ֻ�

	char 	m_szReturnInfo[128];	// ��������
	char 	m_imApiUrl[128];		//imapi http�ӿ�
	std::vector<std::string> 	m_vecMsgNotifyIP;	// ��Ϣ���ͷ������ڵĻ���	

	std::vector<std::string> m_vecAccessServerIP;		// ���������IP
	int m_nAccessServerPort;							// ����������˿�
	char    m_sImLoggerBaseUrl[128];  //im-logger http �ӿ�
	char 	m_webimbStateUrl[128];		 //im-webim http �ӿ�
	char 	m_szFriendListUrl[128];		// im-api http �ӿ� ��ȡ�����б�
	char 	m_szFriendGroupListUrl[128];		// im-api http �ӿ� ��ȡ���ѷ����б�
	char 	m_szUserOfflineMSG[128];		// im-api http �ӿ� ��ȡ�û�������Ϣ
	
	char 	m_szAddGroupUrl[128];		// im-api�ӿ� ��ӷ���
	char 	m_szModGroupUrl[128];		// im-api�ӿ� �޸ķ���
	char 	m_szDelGroupUrl[128];		// im-api�ӿ� ɾ������

	char 	m_szAddFriendUrl[128];		// im-api�ӿ� ��Ӻ���
	char 	m_szModFriendGroupUrl[128];	// im-api�ӿ� �޸ĺ��ѷ���
	char 	m_szDelFriendUrl[128];		// im-api�ӿ� ɾ������

	char 	m_szGetRecentSubMerchantUrl[128];	// im-api�ӿ� ��ȡ�����ϵ�̼�
};

