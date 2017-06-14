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
	void jsonToBin(RecvDataStruct& recvDataStruct, char* strRecvFromWeb);
	uint32 binToJson(const char* SendData, char* w_str);

/*************************************************/
	void ParseLogin(XT_LOGIN_REQ* login_req, const Json::Value &recv);
	void WriteLogin(XT_LOGIN_ACK* login_ack, Json::Value &write);
	
//	unsigned int ParseP2PMsgSend(char* data, Json::Value recv);
	void WriteP2PMsgSend(XT_SERVER_P2PMSG_ACK* P2PMsgSend_ack, Json::Value &write);
	void NotifyP2PMsgSend(char* data, Json::Value &write);
	void ParseP2PMsgSend(CSendPacket& m_SendPacket, const Json::Value &recv);

	void ParseOfflineMsg(XT_OFFLINEMSG_REQ* offlinemsgReq, const Json::Value &recv);
	void WriteOfflineMsg(XT_OFFLINEMSG_ACK* login_ack, Json::Value &write);

	void ParseUserInfoGet(XT_USERINFO_GET_REQ* pUserInfoGetReq, const Json::Value &recv);
	void WriteUserInfoGet(XT_SERVER_USERINFO_GET_ACK* pUserInfoGetAck, Json::Value &write);

	void ParseUserInfoMod(XT_USERINFO_MOD_REQ* pUserInfoModReq, const Json::Value &recv);
	void WriteUserInfoMod(XT_USERINFO_MOD_ACK* pUserInfoModAck, Json::Value &write);

	void ParseFriendListReq(XT_FRIENDLIST_REQ* pFriendListReq, const Json::Value &recv);
	void WriteFriendListAck(CRecvPacket& m_RecvPacket, Json::Value &write);
	
	void ParseGroupInfoGet(XT_GROUPINFO_GET_REQ* pGroupInfoGet, const Json::Value &recv);
	void WriteGroupInfoGet(XT_GROUPINFO_GET_ACK* pGroupInfoGetAck, Json::Value &write);
	
	void ParseLogout(XT_LOGOUT* pLogout, const Json::Value &recv);
	
	void ParseHeartTick(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteHeartTick(CRecvPacket& m_RecvPacket, Json::Value &write);

	void ParseGetUserBasicInfo(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteGetUserBasicInfo(CRecvPacket& m_RecvPacket, Json::Value &write);	
	void NotifyGetUserBasicInfo(CRecvPacket& m_RecvPacket, Json::Value &write);

	void ParseClusterListReq(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteClusterListAck(CRecvPacket& m_RecvPacket, Json::Value &write);

	void ParseClusterInfoReq(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteClusterInfoAck(CRecvPacket& m_RecvPacket, Json::Value &write);
	
	void ParseWebMoreMsg(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteWebMoreMsgAck(CRecvPacket& m_RecvPacket, Json::Value &write);

	void ParseWebListReq(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteWebListAck(CRecvPacket& m_RecvPacket, Json::Value &write);

	//获取分流子帐号
	void ParseGetSubAccount(XT_GET_SUB_ACCOUNT_REQ* pReq, const Json::Value &recv);
	void WriteGetSubAccount(CRecvPacket& m_RecvPacket, Json::Value &write);

	//是否有子账户在线
	void ParseCheckSubOnline(CSendPacket& packet, const Json::Value &recv);
	void WriteCheckSubOnline(XT_GET_SUB_ONLINE_ACK* pAck, Json::Value &write);

	//获取在线状态
	void ParseGetOnlineStatus(CSendPacket& packet, const Json::Value &recv);
	void WriteGetOnlineStatus(XT_GET_USER_STATUS_ACK* pAck, Json::Value &write);


	void ParseWebLastMsgListReq(CSendPacket& m_SendPacket, const Json::Value &recv);
	void WriteWebLastMsgListAck(CRecvPacket& m_RecvPacket, Json::Value &write);

	
	void WriteWebLogoutCSAck(CRecvPacket& m_RecvPacket, Json::Value &write);

	void WriteWebQueueInfoAck(CRecvPacket& m_RecvPacket, Json::Value &write);
	
protected:
	CSendPacket	m_SendPacket;		

};

#endif
