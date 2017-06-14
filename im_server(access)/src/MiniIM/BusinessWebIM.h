/************************************************************************ 
 文件描述：定时扫描处理WEBIM消息(及离线消息)
 
 创建人： Newton.Chao 2011/08/22
 
 修改记录： 
            
************************************************************************/
#pragma once
#include "DataBaseLib.h"
#include "ClientProtocol.h"
#include "SyncCritical.h"
#include "GlobalBusi.h"
#include "BusiLogin.h"
#include <leveldb/db.h>
#include <assert.h>
#include <sstream>

#define UPLOAD_PIC_LOGIC 0	// 服务端上传图片逻辑
#define MAX_CHECK_NUM 1000  //每次获取条数


typedef void (*pfnTestCallback)(void * pUserKey,int nEvent,const RecvDataStruct * pRecvData);
typedef void (*generatepkt)(int type,RecvDataStruct * pRecvData);


//void CommonSendP2PMsg(XT_TALK xtMsg,uint32 msgDataLen);

class Testpacket
{
public:
	Testpacket();
	~Testpacket();
	int StartWork();
	void StopWork();	
	static void* TestThread( LPVOID lpParam );	
	static void* Testzsets( LPVOID lpParam );	
	static void* Testzsets1( LPVOID lpParam );	
	static void* TestCSFlow( LPVOID lpParam );
	static void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 did,uint8 dtype , uint8 flag );
	static void generate_pkt(uint32 fd,int cmd,RecvDataStruct *RecvData);
	
	static void* TestThread1( LPVOID lpParam );
	
	static void* TestMemcachedset( LPVOID lpParam );	//测试排队

	static void* TestMemcachedget( LPVOID lpParam );	//测试排队
	void SetUserCallback(void * pUserKey,pfnTestCallback pfnCallback)
	{
		m_pUserKey = pUserKey;
		m_pfnUserCallback = pfnCallback;
	}		
private:
	void * m_pUserKey;
	pfnTestCallback m_pfnUserCallback;
	int begin;
	int count;
	
	CMemCBusi	m_cMemCBusi;
};
/*
class CWebIMMsgCheck
{
public:
	CWebIMMsgCheck();
	virtual ~CWebIMMsgCheck(void);
	int  StartWork();
	void StopWork();
private:
	friend void  MyWebIMTimeProc(void* dwUser);
	
	friend void  MyWebIMTimeProcNew(union signal v);
	void HandleTimer();

	void StartTimer();
	void StopTiemr();
	void SendP2PMsg(ST_WEBIM_MSG *pWebIMMsg);

private:
	CDataBaseMini m_pDataBaseMini;
	ST_WEBIM_MSG *m_pWebIMMsg;   //指向m_WebIMMsg的指针
    ST_WEBIM_MSG m_WebIMMsg[MAX_CHECK_NUM];
	PMMRESULT m_hMMT;
	timer_t m_timer;
	const unsigned int m_nTimerDelay = 1000 * 10 * 1;      		//Delay : 10s
	const unsigned int m_nTimerResolution = 1000 * 10 * 1; 		//Resolution : 10s
};
*/

#define LINUXOCCI //避免函数重定义错误
#include <occi.h>
#include <time.h>
using namespace std;
using namespace oracle::occi;


/*推送kafka*/
class CChatMsgSave
{
public:
	CChatMsgSave();
	virtual ~CChatMsgSave(void);
	int StartWork();
	static void* ChatMsgSavePorc( LPVOID lpParam );
	void KafKa_init();
	
private:

	std::string brokers;
	std::string errstr;
	std::string topic_str;
	std::string mode;
	RdKafka::Conf *conf;
	RdKafka::Conf *tconf;
	int32_t partition;
	int64_t start_offset;
	ExampleEventCb ex_event_cb; 	
	ExampleDeliveryReportCb ex_dr_cb;
	RdKafka::Producer *producer;
	RdKafka::Topic *topic;
};


class CHttpServer
{
public:
	CHttpServer();
	virtual ~CHttpServer(void);

	int StartWork();
	static void generic_handler(struct evhttp_request *req, void *arg);
	static void* HttpServerStart(LPVOID lpParam);
	int GetMerchantStatus(uint8 &cnt,uint32 *id,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus);
	int GetUserStatus(uint8 &cnt,uint32 *id,tagGSRV_GET_USER_STATUS_ACK::tagUserStatus * pUStatus);
	
	void HttpServiceGetUserInfo(const char *params, Json::Value& jvReply);
		
	void HttpServicePushQueuetoCs(const char *params, Json::Value& jvReply);
	void HttpServiceGetMerchantStatus(const char *params, Json::Value& jvReply);
	void HttpServiceGetUserStatus(const char *params, Json::Value& jvReply);
	void HttpServiceIMCSInfo(const char *params, Json::Value& jvReply);
	void HttpServiceModifyStatus(const char *params, Json::Value& jvReply);
	void HttpServiceQueryCS(const char *params, Json::Value& jvReply);
	void HttpServiceQueryUserToken(const char *params, Json::Value& jvReply);
	void MD5Password(char* pPassword);	
	void HttpServiceGetWorkGroupInfo(const char *params, Json::Value& jvReply);
	
	CDataBaseMini m_pDataBaseMini;
	CDataBaseWeb m_pDataBaseWeb;
private:
	CGlobalBusi *m_pGlobalBusi;
	CConfigInfo m_ConfigInfo;
	char m_HttpServerAddr[20];
	struct evbuffer *evBuf;
	Json::Reader jReader;
	ImMemCache *memcached;
};

class CGetBackendGroup
{
	public:
	CGetBackendGroup();
	virtual ~CGetBackendGroup(void);
	int StartWork();
	static void* GetBackendStart(LPVOID lpParam);
	static void* GetBackendGroupStart(LPVOID lpParam);
	static void* GetBackendGroupMemStart(LPVOID lpParam);
	void ConvertChildGroupJs(Json::Value &jvChildG, int nParentID, vector<GROUPEX> &vecG);
	void FindCSGroup(Json::Value &jvChildG, Json::Value &jvCS, BOOL &bFind);
	CDataBaseMini m_pDataBaseMini;
	vector<GROUPEX> m_vecCSPlatGroup;		// 客服工作台版本，从运营后台获取的组织架构
	vector<XT_CS_INFO> m_vecCSPlatPerson;	// 客服工作台版本，从运营后台获取得组织成员

	// 递归获取子组id
	void GetChildGroupID(uint8 nParentGroupID, vector<uint8> &vecChildGroupID);

private:
	Json::Reader jReader;

	friend void  MyBackendGroupTimeProc(void* dwUser);
	
	friend void  MyBackendGroupProcNew(union signal v);
	void HandleTimer(LPVOID lpParam);
	void StartTimer();
	void StopTiemr();
	timer_t m_timer;
	const unsigned int m_nTimerDelay = 1000 * 10 * 1;      		//Delay : 10s
	const unsigned int m_nTimerResolution = 1000 * 1800 * 1; 	
};

