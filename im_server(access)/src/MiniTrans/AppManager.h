#pragma once

#include "MiniAppNetwork.h"
#include "ThreadManager.h"
#include "HashTable.h"
#include "ConfigInfo.h"
#include "SyncEvent.h"


/************************************************************************ 
文件描述：应用程序类的声明文件 ，整个程序只有该类的一个实例

创建人：hechang
时间：	2006-6-19
************************************************************************/

class CAppManager
{
public:
	~CAppManager(void);

	//初始化
	//返回值:true--成功　false--失败
	bool Init();

	CMiniAppNetwork &   GetAppNetWork() {return m_AppNetwork;}
	CConfigInfo&	GetConfig() { return m_ConfigInfo;	}
	
	//得到服务器发送序列值
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

	//得到单件实例
	static CAppManager& GetInstance();

	void Stop();

	//通知守护进程信号
	void ResetProtectEvent();

private:
	CAppManager(void);

	bool IniConnectServer();
	bool UnregistConnectServer();
	void LogoutAllUser();

private:
	CMiniAppNetwork			m_AppNetwork;		    //网络层
	CThreadManager			m_ThreadManager;	    //多线程管理
	CConfigInfo				m_ConfigInfo;			//配置信息
	
	uint32					m_SendPacketSeq;		//服务器发包序列值
	CSyncCritical			m_syncSeqLock;			//发包序列值多线程安全锁

	time_t					m_start;				//程序启动时间

	CSyncEvent				m_hBlock;				//阻塞主程序
};

