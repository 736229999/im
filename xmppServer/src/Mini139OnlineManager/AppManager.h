#pragma once

/************************************************************************ 
�ļ�������Ӧ�ó��������������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-13 
************************************************************************/

#include "MiniNetTrans.h"
#include "OnlineManager.h"
#include "ThreadManager.h"
#include "ConfigInfo.h"
#include "DataBaseLib.h"
#include "SyncEvent.h"
//#include "CPClientContext.h"


class CAppManager
{
public:
	CAppManager(void);
	~CAppManager(void);

	bool Init();
	void Stop();

	static CAppManager& GetInstance();

	COnlineManager* GetOnlineManager() { return &m_OnlineManager; 	}	
	CUdpNetTrans &   GetAppNetWork() {return m_Network;}

	CConfigInfo&    GetConfig() { return m_ConfigInfo;  }
		
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
	CDataBaseMini m_DatabaseMini;

protected:
/*
	static INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
	static BOOL MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
*/

	static void OnReceive(void *pUserKey,int nEvent,char * pRecvData,int nlen);

private:

	CUdpNetTrans			m_Network;			//���紫�Ͳ�
	CThreadManager			m_ThreadManager;	//���̹߳���
	COnlineManager			m_OnlineManager;	//ȫ�������û�����
	
	CConfigInfo				m_ConfigInfo;			//������Ϣ

	uint32					m_SendPacketSeq;		//��������������ֵ
	CSyncCritical			m_syncSeqLock;			//��������ֵ���̰߳�ȫ��

	time_t					m_start;				//��������ʱ��

	static CAppManager*  m_pInstance;
	MMRESULT m_hMMT;
	CSyncEvent					m_hBlock;				//����������
//	static CTcpNetTrans	m_BackNet;
};
