#include "AppManager.h"
#include <iostream>
#include <string>
#include <cstdlib>
#include <cstdio>
#include <csignal>
#include <cstring>

#include <getopt.h>
#include <unistd.h>

#include "StdPublic.h"
#include "PublicType.h"
#include "PacketBuffer.h"
#include "ConfigInfo.h"
#include "KafkaConsumer.h"
#include "KafkaNewOrder.h"
#include "KafkaPayOrder.h"
#include "KafkaReturnOrder.h"
#include "KafkaGoodsComment.h"

#include "ThreadResponseKafka.h"

using namespace std;

bool CAppManager::g_bRun = true;

static void sigterm (int sig)
{
    CAppManager::g_bRun = false;
}

CAppManager::CAppManager(void):
	m_hBlock(false,false)
{ 
	m_pKafKaMsg = NULL;
	m_pConfig = NULL;
	m_pThreadResponseKafka = NULL;
}

CAppManager::~CAppManager(void)
{
	SAFE_DELETE(m_pKafKaMsg);
	SAFE_DELETE(m_pConfig);
	SAFE_DELETE(m_pThreadResponseKafka);
}

bool CAppManager::Init()
{
	signal(SIGINT, sigterm);
    signal(SIGTERM, sigterm);

	m_pKafKaMsg = new CFIFOPacketBuffer(MAX_MSG_BUFFER_NUM, 4096);
	
	m_pConfig = new CConfigInfo();
	string strZookeeper = m_pConfig->GetKafkaZookeeper();
	string strTopicNewOrder = m_pConfig->GetTopic_NewOrder();
	string strTopicPayOrder = m_pConfig->GetTopic_PayOrder();
	string strTopicReturnOrder = m_pConfig->GetTopic_ReturnOrder();
	string strTopicGoodsComment = m_pConfig->GetTopic_GoodsComment();	

	CLog::SetLogPath(m_pConfig->GetIMLogPath());
	CLog::SetLogClass(m_pConfig->GetIMLogClass());
	
	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"kafka代理:%s; 新订单topic:%s", strZookeeper.c_str(), strTopicNewOrder.c_str());
	CKafKaConsumer *pKafkaConsumer = new CKafkaNewOrder(strZookeeper.c_str(), strTopicNewOrder.c_str());
	m_vecpKafkaConsumer.push_back(pKafkaConsumer);
	pKafkaConsumer->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"kafka代理:%s; 已付款topic:%s", strZookeeper.c_str(), strTopicPayOrder.c_str());
	pKafkaConsumer = new CKafkaPayOrder(strZookeeper.c_str(), strTopicPayOrder.c_str());
	m_vecpKafkaConsumer.push_back(pKafkaConsumer);
	pKafkaConsumer->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"kafka代理:%s; 退换货topic:%s", strZookeeper.c_str(), strTopicReturnOrder.c_str());
	pKafkaConsumer = new CKafkaReturnOrder(strZookeeper.c_str(), strTopicReturnOrder.c_str());
	m_vecpKafkaConsumer.push_back(pKafkaConsumer);
	pKafkaConsumer->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"kafka代理:%s; 商品评价topic:%s", strZookeeper.c_str(), strTopicGoodsComment.c_str());
	pKafkaConsumer = new CKafkaGoodsComment(strZookeeper.c_str(), strTopicGoodsComment.c_str());
	m_vecpKafkaConsumer.push_back(pKafkaConsumer);
	pKafkaConsumer->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"开启响应kafka数据的线程");
	m_pThreadResponseKafka = new CThreadResponseKafka();
	m_pThreadResponseKafka->StartWork();

	CLog::Log("CAppManager",CLog::TYPE_IMPORTANT,"Init OK");

	if (::WaitForSingleObject(m_hBlock, INFINITE) == WAIT_OBJECT_0)
		return true;
		
	return true;
}

CAppManager& CAppManager::GetInstance()
{
	static CAppManager	SingleInstance;
	return SingleInstance;
}

void CAppManager::Exit()
{
	
}

void CAppManager::Stop()
{
	m_hBlock.SetEvent();
}
