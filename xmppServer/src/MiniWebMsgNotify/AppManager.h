#pragma once

#include "MiniAppNetwork.h"
#include "ThreadManager.h"
/*#include "User.h"*/
/*#include "Family.h"*/
#include "HashTable.h"
#include "ConfigInfo.h"
#include "SyncEvent.h"


/************************************************************************ 
�ļ�������Ӧ�ó�����������ļ� ����������ֻ�и����һ��ʵ��

�����ˣ�hechang
ʱ�䣺	2006-6-19
************************************************************************/

class GlobalBusiControl;
class CSystemLogCheck;
class COperationCheck;
class CMobileMsgCheck;


//typedef CHashTable<CUser> CUserManager;
//typedef CHashTable<CPermFamily> CPermFamilyManager;
//typedef CHashTable<CTempFamily>	CTempFamilyManager;

class CAppManager
{
public:
	~CAppManager(void);

	//��ʼ��
	//����ֵ:true--�ɹ���false--ʧ��
	bool Init();

	//�õ������û�������󣬸ö�����������ӵ�IM�������ϵ������û�
// 	CUserManager& GetUserManager() { return m_UserManager; }

// 	CPermFamilyManager& GetPermFamilyManager() { return m_PermFamilyMng; }

	CMiniAppNetwork &   GetAppNetWork() {return m_AppNetwork;}
	CConfigInfo&	GetConfig() { return m_ConfigInfo;	}
	GlobalBusiControl* GetGlobalBusiControl() { return m_pGlobalBusiControl;}
	GlobalBusiControl* GetGlobalFamilyBusiControl() { return m_pGlobalFamilyBusiControl;}
	
	void StartGetSystemLog();
	void StartGetOperation();
	void StopGetSystemLog();
	void StopGetOperation();
	bool GetWebMsgStatus();
	bool GetOperationStatus();
	bool m_SystemLogStatus;
	bool m_OperationStatus;
	list <uint32> *GetServerList();


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

	void InitHead(LPXT_HEAD lpHead,uint16 cmd,uint16 len,uint32 did,uint8 dtype = XT_HEAD::DEST_TYPE_USER)
	{
		lpHead->cmd = cmd;
		lpHead->did = did;
		lpHead->dtype = dtype;
		lpHead->flag = XT_HEAD::FLAG_SRVACTION;
		lpHead->from = XT_HEAD::FROM_SERVER;
		lpHead->len = len;
		lpHead->seq = GetSendSeq();
		lpHead->sid = GetConfig().GetServerNo();
	}

	//�õ�����ʵ��
	static CAppManager& GetInstance();

	void StartGetMobileMsg();
	void StopGetMobileMsg();
	bool GetMobileMsgStatus();
	bool m_MobileMsgStatus;


	void Exit();
	void Stop();


private:
	CAppManager(void);

//	static INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
//	static BOOL MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);


private:
	CMiniAppNetwork			m_AppNetwork;		    //�����
	CThreadManager			m_ThreadManager;	    //���̹߳���
// 	CUserManager			m_UserManager;		    //�����û�����
// 	CPermFamilyManager      m_PermFamilyMng;        //�̶��������
// 	CTempFamilyManager      m_TempFamilyMng;        //��ʱ�������
	CConfigInfo				m_ConfigInfo;			//������Ϣ

	uint32					m_SendPacketSeq;		//��������������ֵ
	CSyncCritical			m_syncSeqLock;			//��������ֵ���̰߳�ȫ��

	GlobalBusiControl*		m_pGlobalBusiControl;		//ȫ�������û���ѯ������������
	GlobalBusiControl*		m_pGlobalFamilyBusiControl;	//ȫ�����߼����ѯ������������

	CSystemLogCheck*		m_pSystemLogCheck;				//��վ��Ϣ����
	COperationCheck*		m_pOperationCheck;			//�û�������Ϣ
	CMobileMsgCheck*		m_pMobileMsgCheck;			//�ֻ������·�


	list<uint32>			m_ServerList;				//�������б�

	int m_Canusever,m_Agreever;

	time_t					m_start;				//��������ʱ��

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//����������
};
