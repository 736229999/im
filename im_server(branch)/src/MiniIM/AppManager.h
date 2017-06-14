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
�ļ�������Ӧ�ó�����������ļ� ����������ֻ�и����һ��ʵ��

�����ˣ�hechang
ʱ�䣺	2006-6-19
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

#define MSGNOTIFY_BUF_LENGTH 5120	// ��Ϣ����buf�����ݳ���

class CAppManager
{
public:
	~CAppManager(void);

	//��ʼ��
	//����ֵ:true--�ɹ���false--ʧ��
	bool Init();

	//�õ������û�������󣬸ö�����������ӵ�IM�������ϵ������û�
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
	
//���ӿͷ�����̨����ѡ��--duyong
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

	//�õ���������������ֵ
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

	//�õ�����ʵ��
	static CAppManager& GetInstance();

	void Exit();
	void Stop();

	//֪ͨ�ػ������ź�
	void ResetProtectEvent();

	// ��ȡ��Ӫ��̨����
	//CGetBackendGroup* GetBackendGroupIns()	{	return m_pGetGroup;}

	// ��ȡ��Ϣ���͹ܵ�
	CFIFOPacketBuffer* GetFifoMsgNotify()	{	return m_pMsgNotifyBuffer; }

#ifdef ACCESS_SERVER
#ifdef XMPP_BUILD	
	// ��jabberdIM�̷߳�������
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
	CMiniAppNetwork			m_AppNetwork;		    //�����
	CThreadManager			m_ThreadManager;	    //���̹߳���
	CSManager				m_CSManager;			//�ͷ�����
	CSQueueManager			m_CSQueueManager;		//�û��Ŷӹ���

	CPermFamilyManager      m_PermFamilyMng;        //�̶��������
	CTempFamilyManager      m_TempFamilyMng;        //��ʱ�������
	CConfigInfo				m_ConfigInfo;			//������Ϣ
	
	uint32					m_SendPacketSeq;		//��������������ֵ
	CSyncCritical			m_syncSeqLock;			//��������ֵ���̰߳�ȫ��

	GlobalBusiControl*		m_pGlobalBusiControl;		//ȫ�������û���ѯ������������
	GlobalBusiControl*		m_pGlobalFamilyBusiControl;	//ȫ�����߼����ѯ������������

	CConnBusiControl*       m_pConnBusiControl;        //����������������
	CConnBusi  *            m_pConnBusi;

	CTransFileBufManager	m_FileBufManager;			//�ļ�(ͼƬ)���ͻ������
#ifdef ACCESS_SERVER
	CHeartTickCheck*		m_pHeartTickCheck;			//IM����������������
#ifdef XMPP_BUILD	
	CXmppSession			m_XmppSession;
	JabberdIm *				m_pJabberdIm;
	int	m_nFD;		// ����jabberdIM�̵߳�fd
#endif	
#endif	
	//CWebIMMsgCheck*         m_pWebIMMsgCheck;           //WEBIM��ͨ(����)��Ϣ����
	//CTimeLenSave*			m_pTimeLenSave;				//��¼�����û�����ʱ��
	//SSDBConnCheck*          m_pSSDBConnCheck;			//SSDB���Ӽ��
#ifdef IM_USE_CACHE

	ImMemCacheConnCheck *	m_pMEMCConnCheck;			//memcached���Ӽ��
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

	time_t					m_start;				//��������ʱ��

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//����������

	CFIFOPacketBuffer*		m_pMsgNotifyBuffer;			// ���մ���Ϣ���ͷ�������������buf	
};

