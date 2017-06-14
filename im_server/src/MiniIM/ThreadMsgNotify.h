#pragma once

#include "WinCommonForLinux.h"
#include "GlobalBusi.h"
#include "DataBaseLib.h"
#include "improtocol.pb.h"
#include <vector>
#include <map>
#include <string>
#include "SyncCritical.h"


// 消息推送数据
struct StruMsgNotifyData
{
	EuMsgPush enType;				// 消息数据类型
	std::string strData;			// 消息数据	
	std::vector<uint32>	vecID;		// 需要推送的商家ID
};

class CThreadMsgNotify
{
public:
    CThreadMsgNotify();
    ~CThreadMsgNotify();
    int StartWork();

	static void SendToSeller(CGlobalBusi *pGlobalBusi, CDataBaseMini *pDB, uint32 did, const Buf_ClientSend &ack);
	// 删除一个商家的所有消息推送数据，在商家退出时调用
	static void DeleteSellerData(uint32 userid);
	
protected:
	static void* Proc( LPVOID lpParam );

	// 收到新订单
	void RecvNewOrder(const char *pData, int nLength);
	// 收到已付款订单
	void RecvPayOrder(const char *pData, int nLength);
	// 收到商品评价
	void RecvGoodsComment(const char *pData, int nLength);
	// 收到退货订单
	void RecvReturnOrder(const char *pData, int nLength);
	// 获取需要通告的商家ID
	void GetNotifySellerID(const char *pGuid, int merchantID, std::vector<uint32> &vecSellerID);
	// 推送数据存入数据库后，将消息推送给在线商家
	void SendToOnlineSeller(const std::vector<uint32> &vecSellerID, const char *pUUID);

	static BOOL InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const std::string &buffer, EuMsgPush nBufferType);
	static void DeleteMsgNotify(const char *pUuid, uint32 userid);
	static BOOL SearchMsgNotify(const char *pUuid, std::string &buffer, EuMsgPush &nBufferType);
	
private:
	CGlobalBusi *m_pGlobalBusi;
	CDataBaseMini m_DatabaseMini;
	ImMemCache *m_pMemCache;

	static std::map<std::string, StruMsgNotifyData> g_mapMsgNotify;		// 消息推送内存
	static CSyncCritical m_lockMsgNotify;	
};

