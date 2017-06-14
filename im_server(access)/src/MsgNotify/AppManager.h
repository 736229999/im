#pragma once
#include "SyncEvent.h"
#include <vector>


class CFIFOPacketBuffer;
class CKafKaConsumer;
class CConfigInfo;
class CThreadResponseKafka;
class CAppManager
{
public:
	~CAppManager(void);

	//初始化
	//返回值:true--成功　false--失败
	bool Init();
	CConfigInfo* GetConfig()	{	return m_pConfig;	}

	//得到单件实例
	static CAppManager& GetInstance();

	void Exit();
	void Stop();

	static bool g_bRun;

	CFIFOPacketBuffer* GetFifoKafkaMsg()	{	return m_pKafKaMsg;	}

private:
	CAppManager(void);

private:
	CConfigInfo*			m_pConfig;				// 配置信息
	CFIFOPacketBuffer*		m_pKafKaMsg;			// 收到的所有kafka数据都丢到此管道中
	std::vector<CKafKaConsumer*> m_vecpKafkaConsumer;
	CSyncEvent				m_hBlock;				// 阻塞主程序

	CThreadResponseKafka*	m_pThreadResponseKafka;	// 响应kafka消息的线程
};

