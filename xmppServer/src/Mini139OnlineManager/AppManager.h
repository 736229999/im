#pragma once

/************************************************************************ 
文件描述：应用程序管理类的声明文件 

创建人：hechang
时间：	2006-6-13 
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
	CDataBaseMini m_DatabaseMini;

protected:
/*
	static INT_PTR CALLBACK MainDlgProc( HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam );
	static BOOL MainDlgOnCommand(HWND hDlg, UINT msg, WPARAM wParam, LPARAM lParam);
*/

	static void OnReceive(void *pUserKey,int nEvent,char * pRecvData,int nlen);

private:

	CUdpNetTrans			m_Network;			//网络传送层
	CThreadManager			m_ThreadManager;	//多线程管理
	COnlineManager			m_OnlineManager;	//全局在线用户管理
	
	CConfigInfo				m_ConfigInfo;			//配置信息

	uint32					m_SendPacketSeq;		//服务器发包序列值
	CSyncCritical			m_syncSeqLock;			//发包序列值多线程安全锁

	time_t					m_start;				//程序启动时间

	static CAppManager*  m_pInstance;
	MMRESULT m_hMMT;
	CSyncEvent					m_hBlock;				//阻塞主程序
//	static CTcpNetTrans	m_BackNet;
};
