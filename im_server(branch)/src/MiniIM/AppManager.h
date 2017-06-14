#pragma once

#include "MiniAppNetwork.h"
#include "ThreadManager.h"
#include "User.h"
#include "Family.h"
#include "HashTable.h"
#include "ConfigInfo.h"
#include "FileTransport.h"
#include "SyncEvent.h"
#include "../include/json/json.h"
//#include "../../ssdb/SSDBConnCheck.h"
#ifdef IM_USE_CACHE
#include "GlobalBusi.h"
#include "BusiMCheck.h"
#endif
#include "ClientService.h"
#include "CsQueue.h"
#ifdef IM_USE_ZOOKEEPER
#include "./zookeeper/ZookeeperClient.h"
#endif
#include "kafukaProducer.h"
#ifdef XMPP_BUILD
#include "jabberdIm.h"
#include "XmppSession.h"
#endif

/************************************************************************ 
文件描述：应用程序类的声明文件 ，整个程序只有该类的一个实例

创建人：hechang
时间：	2006-6-19
************************************************************************/

class GlobalBusiControl;
class CConnBusiControl;
class CConnBusi;
class CHeartTickCheck;
//class CTimeLenSave;
//class CWebIMMsgCheck;
class CChatMsgSave;
class CHttpServer;
class CZookeeper;
class CGetBackendGroup;
class Testpacket;
class CThreadMsgNotify;
typedef CHashTable<CPermFamily> CPermFamilyManager;
typedef CHashTable<CTempFamily>	CTempFamilyManager;
typedef CHashTable<ClientService> CSManager;
typedef CHashTable<CsQueue> CSQueueManager;

#define MSGNOTIFY_BUF_LENGTH 5120	// 消息推送buf的数据长度

class CAppManager
{
public:
	~CAppManager(void);

	//初始化
	//返回值:true--成功　false--失败
	bool Init();

	//得到在线用户管理对象，该对象负责管理连接到IM服务器上的所有用户
	CSManager& GetCSManager() { return m_CSManager; }
	CSQueueManager& GetCSQueueManager() { return m_CSQueueManager; }

	CPermFamilyManager& GetPermFamilyManager() { return m_PermFamilyMng; }
	CTempFamilyManager& GetTempFamilyManager() { return m_TempFamilyMng; }
	CMiniAppNetwork &   GetAppNetWork() {return m_AppNetwork;}
	CConfigInfo&	GetConfig() { return m_ConfigInfo;	}
	CTransFileBufManager& GetTransFileManager() { return m_FileBufManager;	}
	GlobalBusiControl* GetGlobalBusiControl() { return m_pGlobalBusiControl;}
	GlobalBusiControl* GetGlobalFamilyBusiControl() { return m_pGlobalFamilyBusiControl;}
	CConnBusiControl*  GetConnBusiControl(){return m_pConnBusiControl;}
	CFIFOPacketBuffer* GetChatMsgBuffer(){static int i = 0; i++;if(i>=MAX_MSG_PTHREAD_NUM){i = 0;};return m_pChatMsgPacket[i];};
	CFIFOPacketBuffer* GetChatMsgBuffer0(){return m_pChatMsgPacket[0];};
	CFIFOPacketBuffer* GetKafKaMsgBuffer(){return m_pKafKaPacket;};

//	CFIFOPacketBuffer* GetChatMsgBuffer1(){return m_pChatMsgPacket[1];};
	list <uint32> *GetServerList();
	void	UpdateVersion()
	{
		m_ConfigInfo.GetUpdateUrl(m_UpdateConfigBuyer, m_UpdateConfigSeller, m_Canusever,m_Agreever,m_UpdateOption,m_UpdateInfo);
		if (strcmp(m_UpdateOption,"") == 0)
			return;
		m_ConfigInfo.GetCSUpdateUrl(m_CSUpdateConfigBuyer, m_CSUpdateConfigSeller, m_CSCanusever,m_CSAgreever,m_CSUpdateOption,m_CSUpdateInfo);
		if (strcmp(m_CSUpdateOption,"") == 0)
			return;
		m_UpdateOptionMap.clear();		
		m_CSUpdateOptionMap.clear();
		char szUpdateOption[255];
		char szCSUpdateOption[255];
		strcpy_s(szUpdateOption, m_UpdateOption);		
		strcpy_s(szCSUpdateOption, m_CSUpdateOption);
		typedef pair <uint16, uint8> Int_Pair;
		//char *p = szUpdateOption;
		//char *begin = szUpdateOption;
		//char strOption[32] = {0};
		//char strKey[8] = {0};
		//char strVal[8] = {0};

		Json::Reader jReaderOption(Json::Features::strictMode());
		Json::Value jValueOption;
		int nVer = 0, nCfg = 0;
		if(jReaderOption.parse(szUpdateOption, jValueOption))
		{
			int size = jValueOption.size();
			for (int i=0; i<size; ++i)
		    {
		        nVer = jValueOption[i]["ver"].asUInt();
		        nCfg = jValueOption[i]["cfg"].asUInt();
				m_UpdateOptionMap.insert(Int_Pair((uint16)nVer,(uint8)nCfg  ));
		 	}
		}

		Json::Reader jCSReaderOption(Json::Features::strictMode());
		Json::Value jCSValueOption;
		int nCSVer = 0, nCSCfg = 0;
		if(jCSReaderOption.parse(szCSUpdateOption, jCSValueOption))
		{
			int size = jCSValueOption.size();
			for (int i=0; i<size; ++i)
		    {
		        nCSVer = jCSValueOption[i]["ver"].asUInt();
		        nCSCfg = jCSValueOption[i]["cfg"].asUInt();
				m_CSUpdateOptionMap.insert(Int_Pair((uint16)nCSVer,(uint8)nCSCfg ));
		 	}
		}
/*
		while(*p)
		{
			if (*p == '=')
			{
				*p = 0;
				p++;
				strcpy_s(strKey, begin);
				begin = p;
			}
			else if (*p == ',')
			{
				*p = 0;
				p++;
				strcpy_s(strVal, begin);
				begin = p;
				m_UpdateOptionMap.insert(Int_Pair((uint16)atoi(strKey),(uint8)atoi(strVal)));
// 				uint8 ival = GetUpdateOption(306);
// 				int i = 0;
			}
			else
				p++;
		}
		*/
		//m_UpdateOptionMap

	}

	char*	GetUpdateConfigBuyer(){return m_UpdateConfigBuyer;}
	char*	GetUpdateConfigSeller(){return m_UpdateConfigSeller;}
	int		GetCanUserver(){return m_Canusever;}
	int		GetAgreever(){return m_Agreever;}
	char*	GetUpdateOption(){return m_UpdateOption;}
	uint8	GetUpdateOption(uint16 from)
	{
		uint8 ival = 2;
		map<uint16, uint8>::iterator it = m_UpdateOptionMap.find(from);
		if (it != m_UpdateOptionMap.end())
			ival = (*it).second;

		return ival;
	}
	map<uint16, uint8> m_UpdateOptionMap;
	char*	GetUpdateInfo(){return m_UpdateInfo;}
	
//增加客服工作台升级选项--duyong
char*	GetCSUpdateConfigBuyer(){return m_CSUpdateConfigBuyer;}
char*	GetCSUpdateConfigSeller(){return m_CSUpdateConfigSeller;}
int 	GetCSCanUserver(){return m_CSCanusever;}
int 	GetCSAgreever(){return m_CSAgreever;}
char*	GetCSUpdateOption(){return m_CSUpdateOption;}
uint8	GetCSUpdateOption(uint16 from)
{
	uint8 ival = 2;
	map<uint16, uint8>::iterator it = m_CSUpdateOptionMap.find(from);
	if (it != m_CSUpdateOptionMap.end())
		ival = (*it).second;

	return ival;
}
map<uint16, uint8> m_CSUpdateOptionMap;
char*	GetCSUpdateInfo(){return m_CSUpdateInfo;}

// 	void StartGetWebMsg();
// 	void StopGetWebMsg();
// 	bool GetWebMsgStatus();
// 	bool m_WebMsgStatus;
// 

	//得到服务器发送序列值
	inline uint32 GetSendSeq() 
	{
		m_syncSeqLock.Lock();
		uint32 nNewSeq = ++m_SendPacketSeq;
		m_syncSeqLock.Unlock();
		return nNewSeq;
	}
	inline void   GetSendSeq(uint32& nNewSeq)
	{
		m_syncSeqLock.Lock();
		nNewSeq = ++m_SendPacketSeq;
		m_syncSeqLock.Unlock();
	}

	void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 did,uint8 dtype = XT_HEAD::DEST_TYPE_USER, uint8 flag = XT_HEAD::FLAG_SRVACTION)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = flag;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = GetSendSeq();
		lpHead->sid = GetConfig().GetServerNo();
	}

	//得到单件实例
	static CAppManager& GetInstance();

	void Exit();
	void Stop();

	//通知守护进程信号
	void ResetProtectEvent();

	// 获取运营后台对象
	//CGetBackendGroup* GetBackendGroupIns()	{	return m_pGetGroup;}

	// 获取消息推送管道
	CFIFOPacketBuffer* GetFifoMsgNotify()	{	return m_pMsgNotifyBuffer; }

#ifdef ACCESS_SERVER
#ifdef XMPP_BUILD	
	// 向jabberdIM线程发送数据
	bool SendToJabberdIM(BYTE data[], int nLength, uint32 fdCl = 0);
	CXmppSession* GetXmppSession()	{	return &m_XmppSession;	}
#endif	
#endif

private:
	CAppManager(void);

	bool IniConnectServer();
	bool UnregistConnectServer();
	void LogoutAllUser();

//	static INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
//	static BOOL MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


private:
	CMiniAppNetwork			m_AppNetwork;		    //网络层
	CThreadManager			m_ThreadManager;	    //多线程管理
	CSManager				m_CSManager;			//客服管理
	CSQueueManager			m_CSQueueManager;		//用户排队管理

	CPermFamilyManager      m_PermFamilyMng;        //固定家族管理
	CTempFamilyManager      m_TempFamilyMng;        //临时家族管理
	CConfigInfo				m_ConfigInfo;			//配置信息
	
	uint32					m_SendPacketSeq;		//服务器发包序列值
	CSyncCritical			m_syncSeqLock;			//发包序列值多线程安全锁

	GlobalBusiControl*		m_pGlobalBusiControl;		//全局在线用户查询服务器的连接
	GlobalBusiControl*		m_pGlobalFamilyBusiControl;	//全局在线家族查询服务器的连接

	CConnBusiControl*       m_pConnBusiControl;        //与接入服务器的连接
	CConnBusi  *            m_pConnBusi;

	CTransFileBufManager	m_FileBufManager;			//文件(图片)传送缓冲管理
#ifdef ACCESS_SERVER
	CHeartTickCheck*		m_pHeartTickCheck;			//IM服务器的心跳管理
#ifdef XMPP_BUILD	
	CXmppSession			m_XmppSession;
	JabberdIm *				m_pJabberdIm;
	int	m_nFD;		// 连接jabberdIM线程的fd
#endif	
#endif	
	//CWebIMMsgCheck*         m_pWebIMMsgCheck;           //WEBIM互通(离线)消息处理
	//CTimeLenSave*			m_pTimeLenSave;				//记录当天用户在线时长
	//SSDBConnCheck*          m_pSSDBConnCheck;			//SSDB连接检测
#ifdef IM_USE_CACHE

	ImMemCacheConnCheck *	m_pMEMCConnCheck;			//memcached连接检测
#endif
	CFIFOPacketBuffer*		m_pChatMsgPacket[MAX_MSG_PTHREAD_NUM];
	CChatMsgSave*			m_pChatMsgSave[MAX_MSG_PTHREAD_NUM];	
	
	CFIFOPacketBuffer*		m_pKafKaPacket;
	KafKa_Producer*			m_pKafKaProducer;

	CHttpServer*			m_pHttpServer;
	Testpacket *			m_test;
	#ifdef IM_USE_ZOOKEEPER
	CZookeeper*				m_pZookeeper;
	#endif
	//CGetBackendGroup* 		 m_pGetGroup;
	char m_UpdateInfo[255], m_UpdateConfigBuyer[255], m_UpdateConfigSeller[255], m_UpdateOption[512];
	int m_Canusever,m_Agreever;

	char m_CSUpdateInfo[255], m_CSUpdateConfigBuyer[255], m_CSUpdateConfigSeller[255], m_CSUpdateOption[512];
	int m_CSCanusever,m_CSAgreever;

	CThreadMsgNotify*		m_pThreadMsgNotify;

	time_t					m_start;				//程序启动时间

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//阻塞主程序

	CFIFOPacketBuffer*		m_pMsgNotifyBuffer;			// 接收从消息推送服务器发过来的buf	
};

