#pragma once

#include "WinCommonForLinux.h"
#include "GlobalBusi.h"
#include "DataBaseLib.h"
#include "improtocol.pb.h"
#include <vector>
#include <map>
#include <string>
#include "SyncCritical.h"


// ��Ϣ��������
struct StruMsgNotifyData
{
	EuMsgPush enType;				// ��Ϣ��������
	std::string strData;			// ��Ϣ����	
	std::vector<uint32>	vecID;		// ��Ҫ���͵��̼�ID
};

class CThreadMsgNotify
{
public:
    CThreadMsgNotify();
    ~CThreadMsgNotify();
    int StartWork();

	static void SendToSeller(CGlobalBusi *pGlobalBusi, CDataBaseMini *pDB, uint32 did, const Buf_ClientSend &ack);
	// ɾ��һ���̼ҵ�������Ϣ�������ݣ����̼��˳�ʱ����
	static void DeleteSellerData(uint32 userid);
	
protected:
	static void* Proc( LPVOID lpParam );

	// �յ��¶���
	void RecvNewOrder(const char *pData, int nLength);
	// �յ��Ѹ����
	void RecvPayOrder(const char *pData, int nLength);
	// �յ���Ʒ����
	void RecvGoodsComment(const char *pData, int nLength);
	// �յ��˻�����
	void RecvReturnOrder(const char *pData, int nLength);
	// ��ȡ��Ҫͨ����̼�ID
	void GetNotifySellerID(const char *pGuid, int merchantID, std::vector<uint32> &vecSellerID);
	// �������ݴ������ݿ�󣬽���Ϣ���͸������̼�
	void SendToOnlineSeller(const std::vector<uint32> &vecSellerID, const char *pUUID);

	static BOOL InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const std::string &buffer, EuMsgPush nBufferType);
	static void DeleteMsgNotify(const char *pUuid, uint32 userid);
	static BOOL SearchMsgNotify(const char *pUuid, std::string &buffer, EuMsgPush &nBufferType);
	
private:
	CGlobalBusi *m_pGlobalBusi;
	CDataBaseMini m_DatabaseMini;
	ImMemCache *m_pMemCache;

	static std::map<std::string, StruMsgNotifyData> g_mapMsgNotify;		// ��Ϣ�����ڴ�
	static CSyncCritical m_lockMsgNotify;	
};

