
#pragma once

//��������
#define CONFIG_SECTION_IMCOMMON				"IMCommon"
#define CONFIG_KEY_IMMAX_USER_COUNT			"MaxUserCount"
//SSDB������ip
#define CONFIG_SECTION_SSDB					"SSDBConn"
#define CONFIG_KEY_SSDBADDR   				"SSDBAddr"
//hessianLog����������
#define CONFIG_SECTION_HESSIAN				"Hessian"
#define CONFIG_HESSIAN_URL					"URL"
//ѹ������
#define CONFIG_SECTION_PRESS				"Press"
#define CONFIG_PRESS_MID					"MerchantId"


//SHELL�ű�����ͨ����
#define CONFIG_SECTION_SHELL				"ShellConfig"
//Master������IP
#define CONFIG_MASTER_ADDR					"MasterIp"

//httpserver����������
#define CONFIG_SECTION_HTTPSERVER           "HttpServer"
#define CONFIG_HTTPSERVER_IPADDR			"HttpServerIP"
#define CONFIG_HTTPSERVER_PORT              "HttpServerPort"


//ͼƬ������ip
#define CONFIG_SECTION_IMG					"ImgServer"
#define CONFIG_KEY_IMGADDR   				"ImgServerIp"

//im��������ip
#define CONFIG_KEY_IMIPADDR					"IpAddr"
//im�������ڲ�ͨ�ŵip
#define CONFIG_SECTION_IMP2P			"IMP2P"
#define CONFIG_KEY_IMINIPADDR					"IpinAddr"
//���������
#define CONFIG_KEY_SERVERNO					"ServerNo"
//IM��־����
#define CONFIG_KEY_IM_LOG_PATH 				"IMLogPath"
#define CONFIG_KEY_IM_LOGCLASS				"IMLogClass"
//�߳���������
#define CONFIG_KEY_MAXTHREADGROUPA			"MaxThreadGroupA"
#define CONFIG_KEY_MAXTHREADGROUPB			"MaxThreadGroupB"
#define CONFIG_KEY_MAXTHREADGROUPC			"MaxThreadGroupC"
#define CONFIG_KEY_TMAXTHREADGROUP			"MaxThreadGroup"

//�������������
#define CONFIG_SECTION_CONNECT_SERVER_COMMON	"ConnectCommon"
//���������IP
#define CONFIG_KEY_CONNECT_SERVER_IP		"ConnectServerIP"

//�������������
#define CONFIG_SECTION_TCP	"WebConn"
//���������IP
#define CONFIG_KEY_TCP		"TcpAddr"

//�����û�
#define CONFIG_SECTION_ONLINEUSER_COMMON	"OnlineUserCommon"
//ȫ�������û����������ip
#define	CONFIG_KEY_GLOBALONLINE_USER_SERVER "OnlineUserServer"
//�����û���־����
#define CONFIG_KEY_ONLINEUSER_LOGCLASS		"OnlineUserLogClass"
#define CONFIG_KEY_ONLINEUSER_LOG_PATH		"OnlineUserLogPath"

//��ת������IP
#define CONFIG_SECTION_TRANSFER_COMMON		 "TransferCommon"
#define CONFIG_KEY_TRANSFER_SERVER			 "TransferServer"
#define CONFIG_KEY_MAX_TRANS_COUNT			 "MaxTransCount"
//�ļ�������־����
#define CONFIG_KEY_TRANSFER_LOGCLASS		"ImTransferLogClass"
#define CONFIG_KEY_TRANSFER_LOG_PATH		"ImTransferLogPath"


//���߼���
#define CONFIG_SECTION_ONLINEFAMILY_COMMON	"OnlineFamilyCommon"
//ȫ�����߼�����������ip
#define	CONFIG_KEY_ONLINEFAMILY_SERVER		"OnlineFamilyServer"
//���߼�����־����
#define CONFIG_KEY_ONLINEFAMILY_LOGCLASS	"OnlineFamilyLogClass"
#define CONFIG_KEY_ONLINEFAMILY_LOG_PATH	"OnlineFamilyLogPath"


//��վ��Ϣ֪ͨ
#define CONFIG_SECTION_WEBMSGNOTIFY_COMMON	"WebMsgNotifyCommon"
#define CINFIG_KEY_WEBMSGNOTIFY_SERVER	"WebMsgNotifyServer"
//��վ��Ϣ֪ͨ��־����
#define CONFIG_KEY_WEBMSGNOTIFY_LOGCLASS	"WebMsgNotifyLogClass"
#define CONFIG_KEY_WEBMSGNOTIFY_LOG_PATH	"WebMsgNotifyLogPath"

#define CONFIG_SECTION_LOGIN_URL			"LoginURL"
#define CONIFG_KEY_BUYER_LOGIN_URL			"BuyerLoginURL"
#define CONIFG_KEY_SELLER_LOGIN_URL			"SellerLoginURL"

#define CONFIG_SECTION_SEARCH_URL           "SearchMemberURL"
#define CONFIG_KEY_SEARCH_URL 			    "SearchMemberURL"

//headImgUrl����
#define CONFIG_SECTION_HEADIMG_URL  "HeadImgURL"
#define CONFIG_KEY_HEADIMG_URL		"HeadImgURL"
//�̼ҵ���ID��ȡ������URL
#define CONFIG_SECTION_COMMON_URL  "CommonURL"
#define CONFIG_KEY_STORENAME_URL	"StoreNameURL"

//MySQL����
#define CONFIG_SECTION_MYSQLSID_COMMON  "MYSQLSID"
#define CONFIG_KEY_MYSQLSID  "MYSQLSID"

#define CONFIG_SECTION_MYSQLUSER_COMMON  "MYSQLUSER"
#define CONFIG_KEY_MYSQLUSER "MYSQLUSER"

#define CONFIG_SECTION_MYSQLPWD_COMMON  "MYSQLPWD"
#define CONFIG_KEY_MYSQLPWD  "MYSQLPWD"

#define CONFIG_SECTION_MYSQLDB_COMMON  "MYSQLDB"
#define CONFIG_KEY_MYSQLDB  "MYSQLDB"

//MEMCACHED����
#define CONFIG_SECTION_MEMCACHED_COMMON "MEMCACHEDCommon"
#define CONFIG_KEY_MEMCACHED_IPADDR "MEMCACHEDAddr"
#define CONFIG_KEY_MEMCACHED_IPPORT "MEMCACHEDPort"
#define CONFIG_KEY_MEMCACHED_MAGIC  "MEMCACHEDMagic"


