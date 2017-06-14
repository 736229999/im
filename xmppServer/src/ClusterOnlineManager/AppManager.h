#pragma once

/************************************************************************ 
�ļ�������Ӧ�ó��������������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-13 
************************************************************************/

#include "MiniNetTrans.h"
#include "ThreadManager.h"
#include "ClusterManager.h"
#include "ConfigInfo.h"
#include "SyncEvent.h"

class CAppManager
{
public:
	CAppManager(void);
	~CAppManager(void);

	bool Init();
	void Stop();

	static CAppManager& GetInstance();

	CClusterManager* GetClusterManager() { return &m_ClusterManager; 	}
	CConfigInfo&    GetConfig() { return m_ConfigInfo;  }
	
private:

	/*
	static INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
	static BOOL MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
	*/

	CUdpNetTrans			m_Network;			//���紫�Ͳ�
	CThreadManager			m_ThreadManager;	//���̹߳���
	CClusterManager			m_ClusterManager;	//�������߹���

	CConfigInfo				m_ConfigInfo;			//������Ϣ

	static CAppManager*		m_pInstance;
	CSyncEvent					m_hBlock;				//����������

};
