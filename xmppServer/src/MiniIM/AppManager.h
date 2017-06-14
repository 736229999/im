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
�ļ�������Ӧ�ó�����������ļ� ����������ֻ�и����һ��ʵ��

�����ˣ�hechang
ʱ�䣺	2006-6-19
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

	//��ʼ��
	//����ֵ:true--�ɹ���false--ʧ��
	bool Init();

	//�õ������û�������󣬸ö�����������ӵ�IM�������ϵ������û�
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

	// ��jabberdIM�̷߳�������
	bool SendToJabberdIM(BYTE data[], int nLength, uint32 fdCl = 0);

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
	CUserManager			m_UserManager;		    //�����û�����
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

	CHeartTickCheck*		m_pHeartTickCheck;			//IM����������������
	CWebIMMsgCheck*         m_pWebIMMsgCheck;           //WEBIM��ͨ(����)��Ϣ����
	CTimeLenSave*			m_pTimeLenSave;				//��¼�����û�����ʱ��
	SSDBConnCheck*          m_pSSDBConnCheck;			//SSDB���Ӽ��
	CFIFOPacketBuffer*		m_pChatMsgPacket[MAX_MSG_PTHREAD_NUM];
	CChatMsgSave*			m_pChatMsgSave[MAX_MSG_PTHREAD_NUM];
	CHttpServer*			m_pHttpServer;
	JabberdIm *				m_pJabberdIm;
	char m_UpdateInfo[255], m_UpdateConfigBuyer[255], m_UpdateConfigSeller[255], m_UpdateOption[512];
	int m_Canusever,m_Agreever;


	time_t					m_start;				//��������ʱ��

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//����������

	int	m_nFD;		// ����jabberdIM�̵߳�fd
};

