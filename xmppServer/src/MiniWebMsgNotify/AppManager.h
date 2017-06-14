#pragma once

#include "MiniAppNetwork.h"
#include "ThreadManager.h"
/*#include "User.h"*/
/*#include "Family.h"*/
#include "HashTable.h"
#include "ConfigInfo.h"
#include "SyncEvent.h"


/************************************************************************ 
文件描述：应用程序类的声明文件 ，整个程序只有该类的一个实例

创建人：hechang
时间：	2006-6-19
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

	//初始化
	//返回值:true--成功　false--失败
	bool Init();

	//得到在线用户管理对象，该对象负责管理连接到IM服务器上的所有用户
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

	//得到单件实例
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
	CMiniAppNetwork			m_AppNetwork;		    //网络层
	CThreadManager			m_ThreadManager;	    //多线程管理
// 	CUserManager			m_UserManager;		    //在线用户管理
// 	CPermFamilyManager      m_PermFamilyMng;        //固定家族管理
// 	CTempFamilyManager      m_TempFamilyMng;        //临时家族管理
	CConfigInfo				m_ConfigInfo;			//配置信息

	uint32					m_SendPacketSeq;		//服务器发包序列值
	CSyncCritical			m_syncSeqLock;			//发包序列值多线程安全锁

	GlobalBusiControl*		m_pGlobalBusiControl;		//全局在线用户查询服务器的连接
	GlobalBusiControl*		m_pGlobalFamilyBusiControl;	//全局在线家族查询服务器的连接

	CSystemLogCheck*		m_pSystemLogCheck;				//网站消息管理
	COperationCheck*		m_pOperationCheck;			//用户更新消息
	CMobileMsgCheck*		m_pMobileMsgCheck;			//手机短信下发


	list<uint32>			m_ServerList;				//服务器列表

	int m_Canusever,m_Agreever;

	time_t					m_start;				//程序启动时间

	HANDLE					m_hNameEvent;				

	CSyncEvent				m_hBlock;				//阻塞主程序
};
