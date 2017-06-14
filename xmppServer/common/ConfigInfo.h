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
#include <list>

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
	
	//�µ���־���ã���json��������
	
	const char* GetIMLogClass();
	const char* GetUserLogClass();
	const char* GetFamilyLogClass();
	const char* GetNotifyLogClass();
	const char* GetTransLogClass();

	//��ȡ����߳�������
	int GetMaxTdGrpA();
	int GetMaxTdGrpB();
	int GetMaxTdGrpC();
	int GetTMaxTdGrp();

	const char* GetBuyerURL();
	const char* GetSellerURL();

	const char* GetBuyerInfoURL();

	const char* GetHeadImgURL();
	const char* GetStoreNameURL();

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
#ifdef PRESS_TEST
	char* GetMerchantInfo();
#endif
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
	char	m_szBuyerSearchURL[128]; //��Ա��ѯURL

	char    m_szHeadImgURL[128];//ͷ��url
	char 	m_szStoreNameURL[128];	//��������URL
	char	m_szImLogConfigPath[256];		//imserver��־·��
	char 	m_szUserLogConfigPath[256];		//User  ��־·��
	char	m_szFamilyLogConfigPath[256];	//iFamily	��־·��
	char 	m_szNotifyLogConfigPath[256];	//Notify  ��־·��
	char    m_szTransferLogConfigPath[256]; //Transfer ��־·��
	
	char 	m_szImLogConfig[256];		//imserver  ��־����
	char 	m_szUserLogConfig[256];		//imserver  ��־����
	char	m_szFamilyLogConfig[256];	//imserver	��־����
	char 	m_szNotifyLogConfig[256];	//imserver  ��־����
	char    m_szTransferLogConfig[256]; //imTransfer ��־����

	char  	m_szMemcachedServerIp[20];	//memcached������IP
	int	 	m_szMemcachedServerPort;	//memcached������Port
	uint32	m_szMemcachedServerMagic;		//memcached��־λ

	char    m_HessianUrl[512];		//hessian url��ַ
	

	char    m_HttpServerIP[20];   //httpserver ip��ַ
	int     m_HttpServerServerPort; //httpserver port
#ifdef PRESS_TEST
	char   m_szMerchantInfo[512]; //����ID��������
#endif
	
	
};

