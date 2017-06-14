#pragma once

#include "MiniAppNetwork.h"
#include "ThreadManager.h"
#include "HashTable.h"
#include "ConfigInfo.h"
#include "SyncEvent.h"


/************************************************************************ 
�ļ�������Ӧ�ó�����������ļ� ����������ֻ�и����һ��ʵ��

�����ˣ�hechang
ʱ�䣺	2006-6-19
************************************************************************/

class CAppManager
{
public:
	~CAppManager(void);

	//��ʼ��
	//����ֵ:true--�ɹ���false--ʧ��
	bool Init();

	CMiniAppNetwork &   GetAppNetWork() {return m_AppNetwork;}
	CConfigInfo&	GetConfig() { return m_ConfigInfo;	}
	
	//�õ���������������ֵ
	inline uint32 GetSendSeq() 
	{
		m_syncSeqLock.Lock();
		uint32 nNewSeq = ++m_SendPacketSeq;
		m_syncSeqLock.Unlock();
		return nNewSeq;
	}
	inline void  GetSendSeq(uint32& nNewSeq)
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

	void Stop();

	//֪ͨ�ػ������ź�
	void ResetProtectEvent();

private:
	CAppManager(void);

	bool IniConnectServer();
	bool UnregistConnectServer();
	void LogoutAllUser();

private:
	CMiniAppNetwork			m_AppNetwork;		    //�����
	CThreadManager			m_ThreadManager;	    //���̹߳���
	CConfigInfo				m_ConfigInfo;			//������Ϣ
	
	uint32					m_SendPacketSeq;		//��������������ֵ
	CSyncCritical			m_syncSeqLock;			//��������ֵ���̰߳�ȫ��

	time_t					m_start;				//��������ʱ��

	CSyncEvent				m_hBlock;				//����������
};

