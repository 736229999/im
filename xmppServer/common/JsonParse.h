#ifndef JSONPARSE_H
#define JSONPARSE_H

#include "../include/json/json.h"
#include "ClientProtocol.h"
#include "xtpacket.h"

class JsonParse
{
public:
		
	JsonParse();
	~JsonParse();

	static JsonParse& GetInstance();
	void parseFile(char* RecvData,char* str);
	unsigned int WriteFile(const char* SendData,char* w_str);

/*************************************************/
	void ParseLogin(XT_LOGIN_REQ* login_req, Json::Value recv);
	unsigned int WriteLogin(XT_LOGIN_ACK* login_ack, Json::Value write, char* w_str);
	
//	unsigned int ParseP2PMsgSend(char* data, Json::Value recv);
	unsigned int WriteP2PMsgSend(XT_SERVER_P2PMSG_ACK* P2PMsgSend_ack, Json::Value write, char* w_str);
	unsigned int NotifyP2PMsgSend(char* data, Json::Value write, char* w_str);
	void ParseP2PMsgSend(CSendPacket& m_SendPacket, Json::Value recv);

	void ParseOfflineMsg(XT_OFFLINEMSG_REQ* offlinemsgReq, Json::Value recv);
	unsigned int WriteOfflineMsg(XT_OFFLINEMSG_ACK* login_ack, Json::Value write, char* w_str);

	void ParseUserInfoGet(XT_USERINFO_GET_REQ* pUserInfoGetReq, Json::Value recv);
	unsigned int WriteUserInfoGet(XT_SERVER_USERINFO_GET_ACK* pUserInfoGetAck, Json::Value write, char* w_str);

	void ParseUserInfoMod(XT_USERINFO_MOD_REQ* pUserInfoModReq, Json::Value recv);
	unsigned int WriteUserInfoMod(XT_USERINFO_MOD_ACK* pUserInfoModAck, Json::Value write, char* w_str);

	void ParseFriendListReq(XT_FRIENDLIST_REQ* pFriendListReq, Json::Value recv);
	unsigned int WriteFriendListAck(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);
	
	void ParseGroupInfoGet(XT_GROUPINFO_GET_REQ* pGroupInfoGet, Json::Value recv);
	unsigned int WriteGroupInfoGet(XT_GROUPINFO_GET_ACK* pGroupInfoGetAck, Json::Value write, char* w_str);
	
	void ParseLogout(XT_LOGOUT* pLogout, Json::Value recv);
	
	void ParseHeartTick(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteHeartTick(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	void ParseGetUserBasicInfo(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteGetUserBasicInfo(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);	
	unsigned int NotifyGetUserBasicInfo(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	void ParseClusterListReq(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteClusterListAck(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	void ParseClusterInfoReq(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteClusterInfoAck(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);
	
	void ParseWebMoreMsg(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteWebMoreMsgAck(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	void ParseWebListReq(CSendPacket& m_SendPacket, Json::Value recv);
	unsigned int WriteWebListAck(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	//获取分流子帐号
	void ParseGetSubAccount(XT_GET_SUB_ACCOUNT_REQ* pReq, Json::Value recv);
	unsigned int WriteGetSubAccount(CRecvPacket& m_RecvPacket, Json::Value write, char* w_str);

	//是否有子账户在线
	void ParseCheckSubOnline(CSendPacket& packet, Json::Value recv);
	unsigned int WriteCheckSubOnline(XT_GET_SUB_ONLINE_ACK* pAck, Json::Value write, char* w_str);

	//获取在线状态
	void ParseGetOnlineStatus(CSendPacket& packet, Json::Value recv);
	unsigned int WriteGetOnlineStatus(XT_GET_USER_STATUS_ACK* pAck, Json::Value write, char* w_str);

protected:
	CRecvPacket	m_RecvPacket;			
	CSendPacket	m_SendPacket;		

private:

	Json::Reader reader;
	Json::Value recv;
	Json::Value write;
	Json::FastWriter writer;
	
};

#endif
