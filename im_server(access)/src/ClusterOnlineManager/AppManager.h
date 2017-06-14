#pragma once

/************************************************************************ 
文件描述：应用程序管理类的声明文件 

创建人：hechang
时间：	2006-6-13 
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

	CUdpNetTrans			m_Network;			//网络传送层
	CThreadManager			m_ThreadManager;	//多线程管理
	CClusterManager			m_ClusterManager;	//家族在线管理

	CConfigInfo				m_ConfigInfo;			//配置信息

	static CAppManager*		m_pInstance;
	CSyncEvent					m_hBlock;				//阻塞主程序

};
