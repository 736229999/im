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
#include "../../ssdb/SSDBConnCheck.h"
#include "jabberdIm.h"

/************************************************************************ 
文件描述：应用程序类的声明文件 ，整个程序只有该类的一个实例

创建人：hechang
时间：	2006-6-19
************************************************************************/

class GlobalBusiControl;
class CConnBusiControl;
class CConnBusi;
class CHeartTickCheck;
class CTimeLenSave;
class CWebIMMsgCheck;
class CChatMsgSave;
class CHttpServer;
typedef CHashTable<CUser> CUserManager;
typedef CHashTable<CPermFamily> CPermFamilyManager;
typedef CHashTable<CTempFamily>	CTempFamilyManager;

class CAppManager
{
public:
	~CAppManager(void);

	//初始化
	//返回值:true--成功　false--失败
	bool Init();

	//得到在线用户管理对象，该对象负责管理连接到IM服务器上的所有用户
	CUserManager& GetUserManager() { return m_UserManager; }

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
//	CFIFOPacketBuffer* GetChatMsgBuffer1(){return m_pChatMsgPacket[1];};

	void	UpdateVersion()
	{
		m_ConfigInfo.GetUpdateUrl(m_UpdateConfigBuyer, m_UpdateConfigSeller, m_Canusever,m_Agreever,m_UpdateOption,m_UpdateInfo);
		if (strcmp(m_UpdateOption,"") == 0)
			return;
		m_UpdateOptionMap.clear();
		char szUpdateOption[255];
		strcpy_s(szUpdateOption, m_UpdateOption);
		typedef pair <uint16, uint8> Int_Pair;
		//char *p = szUpdateOption;
		//char *begin = szUpdateOption;
		//char strOption[32] = {0};
		//char strKey[8] = {0};
		//char strVal[8] = {0};

		Json::Reader jReaderOption;
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

	// 向jabberdIM线程发送数据
	bool SendToJabberdIM(BYTE data[], int nLength, uint32 fdCl = 0);

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
	CUserManager			m_UserManager;		    //在线用户管理
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

	CHeartTickCheck*		m_pHeartTickCheck;			//IM服务器的心跳管理
	CWebIMMsgCheck*         m_pWebIMMsgCheck;           //WEBIM互通(离线)消息处理
	CTimeLenSave*			m_pTimeLenSave;				//记录当天用户在线时长
	SSDBConnCheck*          m_pSSDBConnCheck;			//SSDB连接检测
	CFIFOPacketBuffer*		m_pChatMsgPacket[MAX_MSG_PTHREAD_NUM];
	CChatMsgSave*			m_pChatMsgSave[MAX_MSG_PTHREAD_NUM];
	CHttpServer*			m_pHttpServer;
	JabberdIm *				m_pJabberdIm;
	char m_UpdateInfo[255], m_UpdateConfigBuyer[255], m_UpdateConfigSeller[255], m_UpdateOption[512];
	int m_Canusever,m_Agreever;


	time_t					m_start;				//程序启动时间

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//阻塞主程序

	int	m_nFD;		// 连接jabberdIM线程的fd
};

