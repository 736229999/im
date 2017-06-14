#include "JsonParse.h"
#include "PublicType.h"
#include "protocol.h"
#include "Log.h"
#include <string>

using namespace std;
JsonParse::JsonParse()
{
	
}

JsonParse::~JsonParse()
{
	
}

JsonParse& JsonParse::GetInstance()
{
	static JsonParse SingleInstance;
	return SingleInstance;
}


// 收到来自webim的json，转化为RecvDataStruct
void JsonParse::jsonToBin(RecvDataStruct& recvDataStruct, char* strRecvFromWeb)
{
	//
	XT_HEAD* pHead = (XT_HEAD*)recvDataStruct.data;
	CSendPacket m_SendPacket;
	m_SendPacket.SetData(recvDataStruct.data + sizeof(XT_HEAD), MAX_SEND_BUFFERS - sizeof(XT_HEAD));

	//解析从web收到的json字符串
	CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "time:%lu, json:%s", timeGetTimeUsec(), strRecvFromWeb);
	
	Json::Reader jsonReader(Json::Features::strictMode());
	Json::Value jsonRecvFromWeb;
	if(jsonReader.parse(strRecvFromWeb, jsonRecvFromWeb))
	{
		recvDataStruct.vlink = jsonRecvFromWeb["vlink"].asUInt();

		pHead->flag = jsonRecvFromWeb["flag"].asUInt();
		pHead->from = jsonRecvFromWeb["from"].asUInt();
		pHead->cmd = jsonRecvFromWeb["cmd"].asUInt();
		pHead->seq = jsonRecvFromWeb["seq"].asUInt();
		pHead->sid = jsonRecvFromWeb["sid"].asUInt();
		pHead->dtype = jsonRecvFromWeb["dtype"].asUInt();
		pHead->did = jsonRecvFromWeb["did"].asUInt();
		pHead->len = jsonRecvFromWeb["len"].asUInt();
#ifdef IM_USE_MULTIDEVICE
		pHead->deviceId = jsonRecvFromWeb["deviceid"].asUInt();
#endif

		switch (pHead->cmd)
		{
			case CMD_LOGIN_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGIN_REQ);
					XT_LOGIN_REQ* login_req = (XT_LOGIN_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseLogin(login_req,jsonRecvFromWeb);
				}
			break;
			case CMD_P2PMSG_SEND:
				{
					ParseP2PMsgSend(m_SendPacket,jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_OFFLINEMSG_REQ:
			case CMD_GET_OFFLINE_MSG_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_OFFLINEMSG_REQ);
					XT_OFFLINEMSG_REQ* offlinemsgReq = (XT_OFFLINEMSG_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseOfflineMsg(offlinemsgReq,jsonRecvFromWeb);
				}
			break;
			case CMD_USERINFO_GET_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_USERINFO_GET_REQ);
					XT_USERINFO_GET_REQ* pUserInfoGetReq = (XT_USERINFO_GET_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseUserInfoGet(pUserInfoGetReq,jsonRecvFromWeb);
				}
			break;
			case CMD_USERINFO_MOD_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_USERINFO_MOD_REQ);
					XT_USERINFO_MOD_REQ* pUserInfoModReq = (XT_USERINFO_MOD_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseUserInfoMod(pUserInfoModReq,jsonRecvFromWeb);
				}
			break;
			case CMD_GROUPINFO_GET_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_GROUPINFO_GET_REQ);
					XT_GROUPINFO_GET_REQ* pGroupInfoGetReq = (XT_GROUPINFO_GET_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseGroupInfoGet(pGroupInfoGetReq,jsonRecvFromWeb);
				}
			break;	
			case CMD_FRIENDLIST_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_FRIENDLIST_REQ);
					XT_FRIENDLIST_REQ* pFriendListReq = (XT_FRIENDLIST_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseFriendListReq(pFriendListReq,jsonRecvFromWeb);
				}
			break;
			case CMD_LOGOUT:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_LOGOUT);
					XT_LOGOUT* pLogout = (XT_LOGOUT*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseLogout(pLogout,jsonRecvFromWeb);
				}
			break;
			case CMD_DIRECTORY_REQ:
				{
					ParseHeartTick(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_GET_FRIEND_BASICINFO_REQ:
				{
					ParseGetUserBasicInfo(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_GET_CLIST_REQ:
				{
					ParseClusterListReq(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_GET_CINFO_REQ:
				{
					ParseClusterInfoReq(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_WEB_MORE_MSG_REQ:
				{
					ParseWebMoreMsg(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_WEB_BUSI_LIST_REQ:
				{
					ParseWebListReq(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;	
			case CMD_GET_SUB_ACCOUNT_REQ:
				{
					recvDataStruct.data_len = sizeof(XT_HEAD) + sizeof(XT_GET_SUB_ACCOUNT_REQ);
					XT_GET_SUB_ACCOUNT_REQ* req = (XT_GET_SUB_ACCOUNT_REQ*)(recvDataStruct.data + sizeof(XT_HEAD));
					ParseGetSubAccount(req, jsonRecvFromWeb);
				}
			break;
			case CMD_GET_SUB_ONLINE_REQ:
				{
					ParseCheckSubOnline(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			case CMD_GET_USER_STATUS_REQ:
				{
					ParseGetOnlineStatus(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			case CMD_GET_LAST_MSG_REQ:
				{					
					ParseWebLastMsgListReq(m_SendPacket, jsonRecvFromWeb);
					recvDataStruct.data_len = sizeof(XT_HEAD) + m_SendPacket.GetSize();
				}
			break;
			default:
				break;
		}	
	}
	else
	{
		CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "jsonPraseFaill:%s", strRecvFromWeb);
	}
	
}

uint32 JsonParse::binToJson(const char* SendData, char* strSendToWeb)
{

	SendDataStruct* sendDataStruct = (SendDataStruct*)SendData;
	CRecvPacket	m_RecvPacket;	
	m_RecvPacket.SetData(sendDataStruct->data + sizeof(XT_HEAD), MAX_SEND_BUFFERS - sizeof(XT_HEAD));
	
	Json::Value jsonValueToWeb;
	
	//head部分的json串
	XT_HEAD* pHead = (XT_HEAD*)sendDataStruct->data;
	jsonValueToWeb["vlink"] = sendDataStruct->vlink;
	jsonValueToWeb["flag"] = pHead->flag;
	jsonValueToWeb["from"] = pHead->from;
	jsonValueToWeb["cmd"] = pHead->cmd;
	jsonValueToWeb["seq"] = pHead->seq;
	jsonValueToWeb["sid"] = pHead->sid;
	jsonValueToWeb["dtype"] = pHead->dtype;
	jsonValueToWeb["did"] = pHead->did;
	jsonValueToWeb["len"] = pHead->len;
#ifdef IM_USE_MULTIDEVICE
	jsonValueToWeb["deviceid"] = pHead->deviceId;
#endif
	//json串组装
	switch (pHead->cmd)
	{
		case CMD_LOGIN_ACK:
		{
			XT_LOGIN_ACK* login_ack = (XT_LOGIN_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteLogin(login_ack, jsonValueToWeb);
			break;
		}

		case CMD_P2PMSG_ACK:
		{
			XT_SERVER_P2PMSG_ACK* p2pmsg_ack = (XT_SERVER_P2PMSG_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteP2PMsgSend(p2pmsg_ack, jsonValueToWeb);
			break;
		}
		
		case CMD_SERVERMSG_IND:
		{
			LPXT_SERVER_TRANSMIT server_transmit = (LPXT_SERVER_TRANSMIT)(sendDataStruct->data + sizeof(XT_HEAD));
			switch(server_transmit->msgtype)
			{
				case CMD_P2PMSG_SEND:
					NotifyP2PMsgSend(sendDataStruct->data, jsonValueToWeb);
					break;	
				case CMD_GET_FRIEND_BASICINFO_NOTIFY:
					NotifyGetUserBasicInfo(m_RecvPacket, jsonValueToWeb);
					break;
				case CMD_LOGOUT_CS_WEB:						
					WriteWebLogoutCSAck(m_RecvPacket, jsonValueToWeb);
					break;				
				default:
					break;
			}
			break;
		}
		
		case CMD_USERINFO_GET_ACK:
		{
			XT_SERVER_USERINFO_GET_ACK* pUserInfoGetAck = (XT_SERVER_USERINFO_GET_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteUserInfoGet(pUserInfoGetAck, jsonValueToWeb);
			break;
		}
		
		case CMD_USERINFO_MOD_ACK:
		{
			XT_USERINFO_MOD_ACK* pUserInfoModAck = (XT_USERINFO_MOD_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteUserInfoMod(pUserInfoModAck, jsonValueToWeb);
			break;
		}
		
		case CMD_GROUPINFO_GET_ACK:
		{
			XT_GROUPINFO_GET_ACK* pGroupInfoGetAck = (XT_GROUPINFO_GET_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteGroupInfoGet(pGroupInfoGetAck, jsonValueToWeb);
			break;
		}
			
		case CMD_FRIENDLIST_ACK:
			WriteFriendListAck(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_DIRECTORY_ACK:
			WriteHeartTick(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_GET_CLIST_ACK:
 			WriteClusterListAck(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_GET_CINFO_ACK:
			WriteClusterInfoAck(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_WEB_MORE_MSG_ACK:
			WriteWebMoreMsgAck(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_WEB_BUSI_LIST_ACK:
			WriteWebListAck(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_GET_SUB_ACCOUNT_ACK:
			WriteGetSubAccount(m_RecvPacket, jsonValueToWeb);
			break;
			
		case CMD_GET_SUB_ONLINE_ACK:
		{
			XT_GET_SUB_ONLINE_ACK* pAck = (XT_GET_SUB_ONLINE_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteCheckSubOnline(pAck, jsonValueToWeb);
			break;
		}

		case CMD_GET_USER_STATUS_ACK:
		{
			XT_GET_USER_STATUS_ACK *pAck = (XT_GET_USER_STATUS_ACK*)(sendDataStruct->data + sizeof(XT_HEAD));
			WriteGetOnlineStatus(pAck, jsonValueToWeb);
			break;
		}
			
		case CMD_GET_LAST_MSG_ACK:
		{
			WriteWebLastMsgListAck(m_RecvPacket, jsonValueToWeb);
			break;
		}
		
		case CMD_QUEUE_LENGTH_IND:		//主动单独一个推送消息，队列长度
			WriteWebLogoutCSAck(m_RecvPacket, jsonValueToWeb);
			break;
		default:
			break;
	}	


	//json串拷贝到tcp发送buffer
	Json::FastWriter jsonWriter;
	string jsonString = jsonWriter.write(jsonValueToWeb);
	int jsonStringLength = jsonString.length() > (MAX_JSON_BUFFER-1) ? (MAX_JSON_BUFFER-1): jsonString.length();
	strncpy(strSendToWeb, jsonString.c_str(), jsonStringLength);

	CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "time:%lu, json:%s",timeGetTimeUsec(), jsonString.c_str());

	return jsonStringLength;
	
}

void JsonParse::ParseLogin(XT_LOGIN_REQ* login_req, const Json::Value &recv)
{
	uint32 len;

	login_req->login_flag = recv["login_flag"].asUInt();

	switch (login_req->login_flag & 0x3)
	{
		case XT_LOGIN_REQ::LOGINBYID:
			login_req->uLoginKey.id= recv["id"].asUInt();
			break;
		case XT_LOGIN_REQ::LOGINBYNAME:
			len = strlen(recv["username"].asCString());
			if(len > 30)
				len = 30;
			strncpy(login_req->uLoginKey.username,recv["username"].asCString(),len);
			login_req->uLoginKey.username[len] = 0;
			break;
		case XT_LOGIN_REQ::LOGINBYMOBILENO:
			len = strlen(recv["mobileno"].asCString());
			if(len > 20)
				len = 20;
			strncpy(login_req->uLoginKey.mobileno,recv["mobileno"].asCString(),len);
			login_req->uLoginKey.mobileno[len] = 0;
			break;
		default:
			break;
	}
	len = strlen(recv["random_key"].asCString());
	strncpy(login_req->random_key,recv["random_key"].asCString(),len>15?15:len);
	login_req->random_key[len] = 0;

	len = strlen(recv["password"].asCString());
	if(len > 15)
		len = 15;	
	strncpy(login_req->password,recv["password"].asCString(),len);
	login_req->password[len] = 0;
	
	login_req->status = recv["status"].asUInt();
	login_req->lanip = recv["lanip"].asUInt();
	login_req->lanport = recv["lanport"].asUInt();

	len = strlen(recv["sessionID"].asCString());
	if(len > 127)
		len = 127;
	strncpy(login_req->sessionID,recv["sessionID"].asCString(),len);
	login_req->sessionID[len] = 0;

	len = recv["mobilephoneno"].asString().size();
	strncpy(login_req->mobilephoneno,recv["mobilephoneno"].asString().c_str(),len > 20 ? 20: len);
	login_req->mobilephoneno[len] = 0;

	len = recv["email"].asString().size();
	strncpy(login_req->email,recv["email"].asString().c_str(),len > 60 ? 60: len);
	login_req->email[len] = 0;

	
}
#if 0
unsigned int JsonParse::ParseP2PMsgSend(char* data, Json::Value recv)
{
	int len;
	char* msg = NULL;
	
	XT_SERVER_P2PMSG_SEND* p2p_send = (XT_SERVER_P2PMSG_SEND*)(data + sizeof(XT_HEAD));
	p2p_send->Sender = recv["Sender"].asUInt();
	p2p_send->Recver = recv["Recver"].asUInt();
	p2p_send->type = recv["type"].asUInt();

	msg = data + sizeof(XT_HEAD) + sizeof(XT_SERVER_P2PMSG_SEND);
	len = strlen(recv["msg"].asCString());
	if(len > MAX_RECV_BUFFERS - 1 - sizeof(XT_SERVER_P2PMSG_SEND))
		len = MAX_RECV_BUFFERS - 1 - sizeof(XT_SERVER_P2PMSG_SEND);
	strncpy(msg, recv["msg"].asCString(), len);
	msg[len] = 0;

	return (len + 1);
}
#endif

void JsonParse::ParseP2PMsgSend(CSendPacket& m_SendPacket, const Json::Value &recv)
{	
	int len;
	char fontName[16] = {0};
	char nickName[48] = {0};
	uint8 XTF_ITALIC = 0;
	uint8 XTF_UNDERLINE = 0;
	uint8 XTF_BOLD = 0;
	Json::Value feature;
	
	uint32 Sender = recv["sender"].asUInt();
	uint32 Recver = recv["recver"].asUInt();
	uint8 type = recv["type"].asUInt();	
	uint32 time = recv["time"].asUInt();

	feature = recv["features"];
	uint8 weight = feature["weight"].asUInt();
	uint8 style = feature["style"].asUInt();
	uint8 line = feature["line"].asUInt();
	uint32 fontColor = feature["color"].asUInt();

	len = strlen(feature["family"].asCString());
	if(len > 15)
		len = 15;
	strncpy(fontName,feature["family"].asCString(),len);
	fontName[len] = 0;

	uint8 fontSize = feature["size"].asUInt();

	len = strlen(recv["nickname"].asCString());
	if(len > 40)
		len = 40;
	strncpy(nickName,recv["nickname"].asCString(),len);
	nickName[len] = 0;

	if(weight == 1)
		XTF_BOLD = 0x04;
	if(style == 1)
		XTF_ITALIC = 0x01;
	if(line == 1)
		XTF_UNDERLINE = 0x02;
	
	uint16 fontStyle = XTF_BOLD | XTF_ITALIC | XTF_UNDERLINE;

	uint16 version;
	if(type == XT_MSG::XT_RECOMENT)
		version = recv["version"].asUInt();
	else
		version= 285;
	uint8 recv_flag = 2;
	
	m_SendPacket<<Sender
				<<Recver
				<<type
				<<version
				<<time
				<<recv_flag;

	if ( type==XT_MSG::IM_MSG || type==XT_MSG::OPEN_P2PDLG || type==XT_MSG::AUTO_REPLY || type==XT_MSG::XT_SERVICE || type==XT_MSG::XT_RECOMENT)	
	{
		m_SendPacket<<fontSize
				<<fontColor
				<<fontStyle
				<<fontName;
	}

	char data[1315] = {0};
	strncpy(data, recv["msg"].asCString(), 1314);
	m_SendPacket.WriteData(data, /*sizeof(data)*/strlen(data));
	//len = strlen(recv["msg"].asCString());
	//m_SendPacket.WriteData(recv["msg"].asCString(),len);
	if ( type==XT_MSG::IM_MSG || type==XT_MSG::OPEN_P2PDLG || type==XT_MSG::AUTO_REPLY || type==XT_MSG::XT_SERVICE || type==XT_MSG::XT_RECOMENT)	
	{
		m_SendPacket<<nickName;
	}
}


void JsonParse::ParseOfflineMsg(XT_OFFLINEMSG_REQ* offlinemsgReq, const Json::Value &recv)
{
	offlinemsgReq->id = recv["sid"].asUInt();
}

void JsonParse::ParseUserInfoGet(XT_USERINFO_GET_REQ* pUserInfoGetReq, const Json::Value &recv)
{
	pUserInfoGetReq->fid = recv["fid"].asUInt();
}

void JsonParse::ParseUserInfoMod(XT_USERINFO_MOD_REQ* pUserInfoModReq, const Json::Value &recv)
{
	uint32 len;

	pUserInfoModReq->id = recv["id"].asUInt();
	
	len = strlen(recv["username"].asCString());
	if(len > MAX_USERNAME_LEN)
		len = MAX_USERNAME_LEN;
	strncpy(pUserInfoModReq->username, recv["username"].asCString(), len);
	pUserInfoModReq->username[len] = 0;

	len = strlen(recv["nickname"].asCString());
	if(len > MAX_NICKNAME_LEN)
		len = MAX_NICKNAME_LEN;
	strncpy(pUserInfoModReq->nickname, recv["nickname"].asCString(), len);
	pUserInfoModReq->nickname[len] = 0;

	len = strlen(recv["email"].asCString());
	if(len > MAX_EMAIL_LEN)
		len = MAX_EMAIL_LEN;
	strncpy(pUserInfoModReq->email, recv["email"].asCString(), len);
	pUserInfoModReq->email[len] = 0;
	
	len = strlen(recv["birthday"].asCString());
	if(len > MAX_BIRTHDAY_LEN)
		len = MAX_BIRTHDAY_LEN;
	strncpy(pUserInfoModReq->birthday, recv["birthday"].asCString(), len);
	pUserInfoModReq->birthday[len] = 0;

	len = strlen(recv["password"].asCString());
	if(len > MAX_PASSWORD_LEN)
		len = MAX_PASSWORD_LEN;
	strncpy(pUserInfoModReq->password, recv["password"].asCString(), len);
	pUserInfoModReq->password[len] = 0;

	len = strlen(recv["comment_name"].asCString());
	if(len > MAX_COMMENTNAME_LEN)
		len = MAX_COMMENTNAME_LEN;
	strncpy(pUserInfoModReq->comment_name, recv["comment_name"].asCString(), len);
	pUserInfoModReq->comment_name[len] = 0;
	
	len = strlen(recv["face_url"].asCString());
	if(len > MAX_HEADURL_LEN)
		len = MAX_HEADURL_LEN;
	strncpy(pUserInfoModReq->face_url, recv["face_url"].asCString(), len);
	pUserInfoModReq->face_url[len] = 0;

	pUserInfoModReq->sex = recv["sex"].asUInt();
	pUserInfoModReq->age = recv["age"].asUInt();
	pUserInfoModReq->province = recv["province"].asUInt();
	pUserInfoModReq->country = recv["country"].asUInt();
	pUserInfoModReq->city = recv["city"].asUInt();
	pUserInfoModReq->career = recv["career"].asUInt();
	pUserInfoModReq->rule = recv["rule"].asUInt();
	pUserInfoModReq->familyflag = recv["familyflag"].asUInt();
	pUserInfoModReq->linkpop = recv["linkpop"].asUInt();
	
	len = strlen(recv["address"].asCString());
	if(len > MAX_ADDRESS_LEN)
		len = MAX_ADDRESS_LEN;
	strncpy(pUserInfoModReq->address, recv["address"].asCString(), len);
	pUserInfoModReq->address[len] = 0;
		
	len = strlen(recv["mobile"].asCString());
	if(len > MAX_PHONE_LEN)
		len = MAX_PHONE_LEN;
	strncpy(pUserInfoModReq->mobile, recv["mobile"].asCString(), len);
	pUserInfoModReq->mobile[len] = 0;

	pUserInfoModReq->gold_money = recv["gold_money"].asUInt();
	pUserInfoModReq->online_time = recv["online_time"].asUInt();
	
	len = strlen(recv["description"].asCString());
	if(len > MAX_DESCRIPTION_LEN)
		len = MAX_DESCRIPTION_LEN;
	strncpy(pUserInfoModReq->description, recv["description"].asCString(), len);
	pUserInfoModReq->description[len] = 0;

	len = strlen(recv["user_sign"].asCString());
	if(len > MAX_USERSIGN_LEN)
		len = MAX_USERSIGN_LEN;
	strncpy(pUserInfoModReq->user_sign, recv["user_sign"].asCString(), len);
	pUserInfoModReq->user_sign[len] = 0;

	len = strlen(recv["avatoruri"].asCString());
	if(len > MAX_AVATOR_LEN)
		len = MAX_AVATOR_LEN;
	strncpy(pUserInfoModReq->avatoruri, recv["avatoruri"].asCString(), len);
	pUserInfoModReq->avatoruri[len] = 0;

	pUserInfoModReq->mobilestatus = recv["mobilestatus"].asUInt();
}

void JsonParse::ParseFriendListReq(XT_FRIENDLIST_REQ* pFriendListReq, const Json::Value &recv)
{
	pFriendListReq->id = recv["id"].asUInt();
	pFriendListReq->start_pos = recv["start_pos"].asUInt();
}

void JsonParse::ParseGroupInfoGet(XT_GROUPINFO_GET_REQ* pGroupInfoGet, const Json::Value &recv)
{
	pGroupInfoGet->id = recv["id"].asUInt();
	pGroupInfoGet->group_count = recv["group_count"].asUInt();	
}

void JsonParse::ParseLogout(XT_LOGOUT* pLogout, const Json::Value &recv)
{
	pLogout->id = recv["id"].asUInt();
	pLogout->authData = recv["authData"].asUInt();
}

void JsonParse::ParseHeartTick(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	XT_DIRECTORY_REQ pHeartTick;

	pHeartTick.id = recv["id"].asUInt();
	pHeartTick.local_ip = recv["ip"].asUInt();
	pHeartTick.local_port = recv["port"].asUInt();

	m_SendPacket<<pHeartTick.id
				<<pHeartTick.local_ip
				<<pHeartTick.local_port;
}

void JsonParse::ParseGetUserBasicInfo(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	uint8	type;
	uint16	count;
	uint32	id;
	
	Json::Value FriendID;
	int size;

	type = recv["type"].asUInt();
	count = recv["count"].asUInt();
	m_SendPacket<<type
				<<count;
	
	FriendID = recv["FriendID"];
	size = FriendID.size();
	for(int i = 0; i < size; i++)
	{
		id = FriendID[i]["id"].asUInt();
		m_SendPacket<<id;
	}
}

void JsonParse::ParseClusterListReq(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	uint8 nFamilyProp;
	uint8 nCount;

	nFamilyProp = recv["prop"].asUInt();
	nCount = recv["count"].asUInt();

	m_SendPacket<<nFamilyProp
				<<nCount;
}

void JsonParse::ParseClusterInfoReq(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	uint32 id;
	uint8 prop;

	id = recv["id"].asUInt();
	prop = recv["prop"].asUInt();

	m_SendPacket<<id
				<<prop;
}

void JsonParse::ParseWebMoreMsg(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	uint32 len;
	XT_WEB_MORE_MSG_REQ WebMsgReq;

	WebMsgReq.sid = recv["sid"].asUInt();
	//获取历史的请求head中的did必须为0 ，否则会把请求分到did登录的机器上//zhangmin
	//WebMsgReq.did = recv["did"].asUInt();
	WebMsgReq.did = recv["fid"].asUInt();
	WebMsgReq.msgtime = recv["time"].asUInt();
	WebMsgReq.msgnum = recv["msgnum"].asUInt();
	WebMsgReq.msgid = recv["msgid"].asUInt();

	len = strlen(recv["uuid"].asCString());
	if(len > MAX_UUID_LEN)
		len = MAX_UUID_LEN;
	strncpy(WebMsgReq.uuid, recv["uuid"].asCString(), len);
	WebMsgReq.uuid[len] = 0;
	WebMsgReq.isMerchant = recv["isMerchant"].asUInt();
	
	m_SendPacket<<WebMsgReq.sid
				<<WebMsgReq.did
				<<WebMsgReq.msgtime
				<<WebMsgReq.msgnum
				<<WebMsgReq.msgid
				<<WebMsgReq.uuid
				<<WebMsgReq.isMerchant;
	
	CLog::Log("JsonParse::ParseWebMoreMsg",CLog::TYPE_IMPORTANT,
		"sid:%d, did:%d, time:%d, msgnum:%d, msgid:%d, uuid:%s, isMerchant:%d",
		WebMsgReq.sid,WebMsgReq.did,WebMsgReq.msgtime,WebMsgReq.msgnum,
		WebMsgReq.msgid,WebMsgReq.uuid,WebMsgReq.isMerchant);
}

void JsonParse::ParseWebListReq(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	XT_WEB_BUSI_LIST_REQ WebListReq;

	WebListReq.id = recv["id"].asUInt();
	WebListReq.msgnum = recv["msgnum"].asUInt();

	m_SendPacket<<WebListReq.id
				<<WebListReq.msgnum;	
}

void JsonParse::ParseGetSubAccount(XT_GET_SUB_ACCOUNT_REQ* pReq, const Json::Value &recv)
{
	//读取id
	pReq->merchant_id = recv["merchant_id"].asUInt();
	pReq->group_id = recv["group_id"].asUInt();

	//读取type
	pReq->type = recv["type"].asUInt();
}

void JsonParse::ParseCheckSubOnline(CSendPacket& packet, const Json::Value &recv)
{
	//读取list
	Json::Value value = recv["list"];

	//写入list长度
	packet << (uint32_t)value.size();

	//读取全部账号
	for(size_t i = 0; i < value.size(); i++)
	{
		uint32_t id = (uint32_t)value[i].asInt();
		packet << id;
	}
}

void JsonParse::ParseGetOnlineStatus(CSendPacket& packet, const Json::Value &recv)
{
	//读取list
	Json::Value value = recv["list"];

	//写入list的长度
	packet << (uint32_t)value.size();

	//读取list的全部id
	for(size_t i = 0; i < value.size(); i++)
	{
		uint32_t id = value[i].asUInt();
		packet << id;
	}
}


void JsonParse::ParseWebLastMsgListReq(CSendPacket& m_SendPacket, const Json::Value &recv)
{
	XT_WEB_BUSI_LAST_MSG_REQ WebLastReq;

	WebLastReq.id = recv["id"].asUInt();
	WebLastReq.msgnum = recv["msgnum"].asUInt();

	m_SendPacket<<WebLastReq.id
				<<WebLastReq.msgnum;	
}


void JsonParse::WriteLogin(XT_LOGIN_ACK* login_ack, Json::Value &write)
{
	
	write["ret"] = login_ack->ret;
	if(login_ack->ret == 0)
	{
		write["session_key"] = login_ack->session_key;
		write["user_id"] = login_ack->user_id;
		write["publicip"] = login_ack->publicip;
		write["version_flag"] = login_ack->version_flag;
		write["version_url"] = login_ack->version_url;
		write["update_config"] = login_ack->update_config;
		write["update_info"] = login_ack->update_info;
		write["sessionID"] = login_ack->sessionID;
	}
	else
	{
		write["error"] = (char*)login_ack + sizeof(uint8);
	}
	
}

void JsonParse::WriteP2PMsgSend(XT_SERVER_P2PMSG_ACK* P2PMsgSend_ack, Json::Value &write)
{
	write["sender"] = P2PMsgSend_ack->Sender;
	write["receiver"] = P2PMsgSend_ack->Receiver;
	write["type"] = P2PMsgSend_ack->type;

}

void JsonParse::NotifyP2PMsgSend(char* data, Json::Value &write)
{	
	XT_MSG msg;	
	uint8 weight = 0, style = 0, line = 0;
	//uint8 style;
	//uint8 line;
	CRecvPacket	m_NotifyPacket;
	Json::Value feature;

	LPXT_SERVER_TRANSMIT transmit = (LPXT_SERVER_TRANSMIT)(data + sizeof(XT_HEAD));
	write["origin_id"] = transmit->origin_id;
	write["msgtype"] = transmit->msgtype;

	XT_SERVER_P2PMSG_SEND* p2pMsg_send = (XT_SERVER_P2PMSG_SEND*)(data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	write["sender"] = p2pMsg_send->Sender;
	write["recver"] = p2pMsg_send->Recver;
	write["type"] = p2pMsg_send->type;
	
	m_NotifyPacket.SetData(data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT), MAX_SEND_BUFFERS - sizeof(XT_HEAD) - sizeof(XT_SERVER_TRANSMIT));
	
	m_NotifyPacket>>msg.from_id
				>>msg.to_id
				>>msg.data_type
				>>msg.ver
				>>msg.send_time
				>>msg.recv_flag
				>>msg.fontSize
				>>msg.fontColor
				>>msg.fontStyle
				>>msg.fontName;

	if(msg.data_type == XT_MSG::IM_MSG_INSERT)
	{
		CLog::Log("JsonParse::NotifyP2PMsgSend",CLog::TYPE_IMPORTANT,"#########data_type 26#########");
	}

	weight = msg.fontStyle & 0x04;
	style = msg.fontStyle & 0x01;
	line = msg.fontStyle & 0x02;

	if(weight == 0x04)
		weight = 1;
	else
		weight = 0;

	if(style == 0x01)
		style = 1;
	else
		style = 0;

	if(line == 0x02)
		line = 1;
	else
		line = 0;

	feature["weight"] = weight;
	feature["style"] = style;
	feature["line"] = line;
	feature["color"] = msg.fontColor;
	feature["family"] = msg.fontName;
	feature["size"] = msg.fontSize;

	write["features"] = feature;
	write["time"] = msg.send_time;

	m_NotifyPacket.ReadData(msg.data,sizeof(msg.data));

	if(msg.data_type == XT_MSG::IM_MSG_INSERT)
	{
		CLog::Log("JsonParse::NotifyP2PMsgSend",CLog::TYPE_IMPORTANT,"###data:%s###", msg.data);
	}

	
	m_NotifyPacket>>msg.from_nickname
					>>msg.merchantID;

	write["merchantId"] = msg.merchantID;
	
	if(msg.data_type == 7)
		write["msg"] = "recomment";
	else
		write["msg"] = msg.data;
	write["nickname"] = msg.from_nickname;

}

void JsonParse::WriteUserInfoGet(XT_SERVER_USERINFO_GET_ACK* pUserInfoGetAck, Json::Value &write)
{
	write["ret"] = pUserInfoGetAck->ret;
	write["id"] = pUserInfoGetAck->id;
	write["username"] = pUserInfoGetAck->username;
	write["nickname"] = pUserInfoGetAck->nickname;
	write["email"] = pUserInfoGetAck->email;	
	write["birthday"] = pUserInfoGetAck->birthday;
	write["sex"] = pUserInfoGetAck->sex;
	write["province"] = pUserInfoGetAck->province;
	write["country"] = pUserInfoGetAck->country;
	write["city"] = pUserInfoGetAck->city;
	write["face_url"] = pUserInfoGetAck->face_url;	
	write["career"] = pUserInfoGetAck->career;
	write["rule"] = pUserInfoGetAck->rule;
	write["familyflag"] = pUserInfoGetAck->familyflag;	
	write["gold_money"] = pUserInfoGetAck->gold_money;	
	write["online_time"] = pUserInfoGetAck->online_time;
	write["linkpop"] = pUserInfoGetAck->linkpop;
	write["address"] = pUserInfoGetAck->address;
	write["mobile"] = pUserInfoGetAck->mobile;
	write["description"] = pUserInfoGetAck->description;
	write["usersign"] = pUserInfoGetAck->usersign;	
	write["avatoruri"] = pUserInfoGetAck->avatoruri;
	write["mobilestatus"] = pUserInfoGetAck->mobilestatus;
	write["integral"] = pUserInfoGetAck->integral;
	
}

void JsonParse::WriteUserInfoMod(XT_USERINFO_MOD_ACK* pUserInfoModAck, Json::Value &write)
{

	write["ret"] = pUserInfoModAck->ret;
	write["error"] = pUserInfoModAck->error;

}

void JsonParse::WriteGroupInfoGet(XT_GROUPINFO_GET_ACK* pGroupInfoGetAck, Json::Value &write)
{
	Json::Value GroupInfo;
	Json::Value item;

	if(pGroupInfoGetAck->ret == 0)
	{
		write["ret"] = pGroupInfoGetAck->ret;
		write["group_count"] = pGroupInfoGetAck->group_count;

		for(size_t i = 0; i < pGroupInfoGetAck->group_count; i++)
		{
			XT_GROUPINFO_GET_ACK::tagGroupInfo* pInfo = (XT_GROUPINFO_GET_ACK::tagGroupInfo*)((char*)pGroupInfoGetAck + sizeof(uint8) + sizeof(uint8) + sizeof(XT_GROUPINFO_GET_ACK::tagGroupInfo)*i) ;
			item["pid"] = pInfo->nGroupPid;
			item["id"] = pInfo->nGroupID;
			item["name"] = pInfo->szGroupName;
			GroupInfo.append(item);
		}	

		write["GroupInfo"] = GroupInfo;
	}
	else
	{
		write["ret"] = pGroupInfoGetAck->ret;
		write["error"] = (char*)pGroupInfoGetAck + sizeof(uint8);
	}
}

void JsonParse::WriteFriendListAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	XT_FRIENDLIST_ACK::tagFriendInfo FriendInfo;
	Json::Value friendInfo;
	Json::Value item;

	uint8		result;
	uint16		next_pos;
	uint8		count;

	m_RecvPacket>>result
				>>next_pos
				>>count;
	
	write["result"] = result;
	write["next_pos"] = next_pos;
	write["count"] = count;
	
	for(size_t i = 0; i < count; i++)
	{
		m_RecvPacket>>FriendInfo.friendId
					>>FriendInfo.groupId
					>>FriendInfo.status;
		item["fid"] = FriendInfo.friendId;
		item["gid"] = FriendInfo.groupId;
		item["stat"] = FriendInfo.status;
		friendInfo.append(item);
	}

}

void JsonParse::WriteHeartTick(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	XT_DIRECTORY_ACK pHeartTickAck;

	m_RecvPacket>>pHeartTickAck.ret;
	
	write["ret"] = pHeartTickAck.ret;
}

void JsonParse::NotifyGetUserBasicInfo(CRecvPacket& m_RecvPacket, Json::Value &write)
{

	XT_SERVER_TRANSMIT transmit;
	XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo FriendInfo;
	uint16  count;
	Json::Value friendInfo;
	Json::Value item;

	m_RecvPacket>>transmit.origin_id
				>>transmit.msgtype
				>>count;

	write["origin_id"] = transmit.origin_id;
	write["msgtype"] = transmit.msgtype;
	write["count"] = count;

	for(size_t i = 0; i < count; i++)
	{
		m_RecvPacket>>FriendInfo.nID
					>>FriendInfo.szHeadUrl
					>>FriendInfo.nSexy
					>>FriendInfo.szNickName
					>>FriendInfo.szUserName
					>>FriendInfo.szCommentName
					>>FriendInfo.LoveFlag
					>>FriendInfo.szUserSign
					>>FriendInfo.mobileStatus;
		
		item["id"] = FriendInfo.nID;
		item["url"] = FriendInfo.szHeadUrl;
		item["sexy"] = FriendInfo.nSexy;		
		item["nick"] = FriendInfo.szNickName;
		item["name"] = FriendInfo.szUserName;
		item["note"] = FriendInfo.szCommentName;
		item["flag"] = FriendInfo.LoveFlag;
		item["sign"] = FriendInfo.szUserSign;
		item["stat"] = FriendInfo.mobileStatus;

		friendInfo.append(item);
	}
	write["FriendInfo"] = friendInfo;

	
	
}

void JsonParse::WriteClusterListAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{

	uint8 ret,nFamilyProp,nCount;
	uint32 nFamilyID;
	char szError[MAX_ERROR_BUF];
	Json::Value clusterlist;
	Json::Value item;

	m_RecvPacket>>ret;

	write["ret"] = ret;
	if(ret == RESULT_SUCCESS)
	{
		m_RecvPacket>>nFamilyProp
					>>nCount;
		
		write["prop"] = nFamilyProp;
		write["count"] = nCount;
		
		for(size_t i = 0; i < nCount; i ++)
		{
			m_RecvPacket>>nFamilyID;
			item["id"] = nFamilyID;
			clusterlist.append(item);
		}
		write["clusterlist"] = clusterlist;
	}
	else
	{
		m_RecvPacket>>szError;
		write["error"] = szError;
	}
	
}

void JsonParse::WriteClusterInfoAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	uint8 ret, nFamily;
	char szError[MAX_ERROR_BUF];
	XT_CLUSTER_INFO clusterInfo;

	m_RecvPacket>>ret;
	write["ret"] = ret;

	if(ret == RESULT_SUCCESS)
	{
		m_RecvPacket>>clusterInfo.nFamilyID
					>>nFamily;

		write["fid"] = clusterInfo.nFamilyID;
		write["prop"] = nFamily;
		if(nFamily == PERM_FAMILY)
		{
			m_RecvPacket>>clusterInfo.nCreatorID
						>>clusterInfo.nFamilyType
						>>clusterInfo.nFamilyClass
						>>clusterInfo.nFamilyFaceID
						>>clusterInfo.szFamilyFaceUrl
						>>clusterInfo.nJoinFlag
						>>clusterInfo.nViewFlag
						>>clusterInfo.szFamilyName
						>>clusterInfo.szFamilyDesc
						>>clusterInfo.szFamilyDomain
						>>clusterInfo.nMemberLimit
						>>clusterInfo.szFamilySign
						>>clusterInfo.szFamilyNotice;
			
			write["cid"] = clusterInfo.nCreatorID;
			write["type"] = clusterInfo.nFamilyType;
			write["class"] = clusterInfo.nFamilyClass;
			write["face"] = clusterInfo.nFamilyFaceID;
			write["url"] = clusterInfo.szFamilyFaceUrl;
			write["jflag"] = clusterInfo.nJoinFlag;
			write["vflag"] = clusterInfo.nViewFlag;
			write["name"] = clusterInfo.szFamilyName;
			write["desc"] = clusterInfo.szFamilyDesc;
			write["domain"] = clusterInfo.szFamilyDomain;
			write["limit"] = clusterInfo.nMemberLimit;
			write["sign"] = clusterInfo.szFamilySign;
			write["notice"] = clusterInfo.szFamilyNotice;
		}
	}
	else
	{
		m_RecvPacket>>szError;
		write["error"] = szError;
	}
}

void JsonParse::WriteWebMoreMsgAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	uint8 ret,weight,style,line,over;
	uint32 sid,did,msgnum,returnnum,time,actualnum,msgid;
	char uuid[MAX_UUID_LEN] = {0};
	XT_WEB_MORE_MSG_ACK::MsgInfo MsgInfo;
	Json::Value msgInfo;
	Json::Value features;
	Json::Value item;

	m_RecvPacket>>ret
				>>sid
				>>did;
	write["ret"] = ret;
	write["sid"] = sid;
	write["did"] = did;
	if(ret == RESULT_SUCCESS)
	{
		m_RecvPacket>>time
					>>msgnum
					>>returnnum
					>>over
					>>actualnum
					>>uuid;
		write["msgnum"] = returnnum;
		write["time"] = time;
		write["returnnum"] = actualnum;
		write["over"] = over;
		write["uuid"] = uuid;
		if(actualnum > 25)
		{			
			CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "sid %u,actualnum is too big :%u turn into 25",sid,actualnum);
			actualnum = 25;			
		}
		for(size_t i = 0; i < actualnum; i++)
		{
			m_RecvPacket>>MsgInfo.id
						>>MsgInfo.send_time
						>>MsgInfo.msgid
						>>MsgInfo.fontSize
						>>MsgInfo.fontColor
						>>MsgInfo.fontStyle
						>>MsgInfo.fontName
						>>MsgInfo.nickName
						>>MsgInfo.data;
			
			weight = MsgInfo.fontStyle & 0x04;
			style = MsgInfo.fontStyle & 0x01;
			line = MsgInfo.fontStyle & 0x02;
						
			if(weight == 0x04)
				weight = 1;
			else
				weight = 0;
			
			if(style == 0x01)
				style = 1;
			else
				style = 0;
			
			if(line == 0x02)
				line = 1;
			else
				line = 0;

			features["weight"] = weight;
			features["style"] = style;
			features["line"] = line;
			features["size"] = MsgInfo.fontSize;
			features["family"] = MsgInfo.fontName;
			features["color"] = MsgInfo.fontColor;

			item["id"] = MsgInfo.id;
			item["time"] = MsgInfo.send_time;
			item["msgid"] = MsgInfo.msgid;
			item["nickName"] = MsgInfo.nickName;
			item["msg"] = MsgInfo.data;
			item["features"] = features;

			msgInfo.append(item);
		}	
		write["msgInfo"] = msgInfo;
	}
	else
	{
		m_RecvPacket>>time
					>>msgnum
					>>msgid
					>>uuid;
		write["msgnum"] = 0;
		write["time"] = time;
		write["msgInfo"] = msgInfo;
		write["returnnum"] = 0;
		write["over"] = 1;
		write["uuid"] = uuid;
	}
			
}

void JsonParse::WriteWebListAck(CRecvPacket& m_RecvPacket, Json::Value &write )
{
	uint32 id,msgnum,actualnum,state;
	uint8 ret;
	XT_WEB_BUSI_LIST_ACK::BusiList ListInfo;
	Json::Value item;
	Json::Value list;

	m_RecvPacket>>ret
				>>id
				>>msgnum
				>>actualnum;
	write["ret"] = ret;
	write["id"] = id;
	write["msgnum"] = msgnum;
	if(ret == RESULT_SUCCESS)
	{
		for(size_t i = 0;i < actualnum; i++)
		{
			m_RecvPacket>>ListInfo.id
						>>ListInfo.nickname
						>>ListInfo.time
						>>ListInfo.state
						>>ListInfo.headImgUrl
						>>ListInfo.usersign;
			item["id"] = ListInfo.id;
			item["nickname"] = ListInfo.nickname;
			item["time"] = ListInfo.time;
			if(ListInfo.state == XTREAM_OFFLINE)
				state = 1;
			else
				state = 0;
			item["state"] = state;

			item["headImgUrl"]= ListInfo.headImgUrl;
			item["usersign"]= ListInfo.usersign;
			list.append(item);
		}
		write["list"] = list;
	}
			
}

void JsonParse::WriteGetSubAccount(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	uint32_t id;
	uint32_t QueueLength;
	char tmp[128];

	//写入id
	m_RecvPacket >> id;
	write["id"] = id;
	
	m_RecvPacket >> QueueLength;
	write["queueLength"] = QueueLength;
	//客服分流失败返回
	//客服
	if( id == 0 && QueueLength != 0 ) //客服分流失败
	{
		CLog::Log("JsonParse::WriteGetSubAccount",CLog::TYPE_IMPORTANT,"客服分流排队中 id %u Length %u",
		id,QueueLength);		
	}
	else
	{
		//写入email
		m_RecvPacket >> tmp;
		write["email"] = tmp;

		//写入username
		m_RecvPacket >> tmp;
		write["username"] = tmp;

		//写入mobile
		m_RecvPacket >> tmp;
		write["mobile"] = tmp;

		//写入truename
		m_RecvPacket >> tmp;
		write["truename"] = tmp;

		//写入usersign
		m_RecvPacket >> tmp;
		write["usersign"] = tmp;
		//写入face_url
		m_RecvPacket >> tmp;	
		write["headImgUrl"] = tmp;
		
		//写入encoding
		write["encoding"] = "gbk";
	}
	
}

void JsonParse::WriteCheckSubOnline(XT_GET_SUB_ONLINE_ACK* pAck, Json::Value &write)
{
	Json::Value ids;

	for (uint32_t i = 0; i < pAck->cnt; ++i) {
		//写入id
		ids["id"] = pAck->stat[i].id;

		//写入online
		ids["online"] = pAck->stat[i].b == 0 ? false : true;

		//往list中加入一项
		write["list"].append(ids);
	}

}

void JsonParse::WriteGetOnlineStatus(XT_GET_USER_STATUS_ACK* pAck, Json::Value &write)
{	
	Json::Value ids;

	for (uint32_t i = 0; i < pAck->cnt; ++i) {
		//写入id
		ids["id"] = pAck->stat[i].id;

		//写入online
		ids["online"] = pAck->stat[i].b == 0 ? false : true;

		//添加一项到list
		write["list"].append(ids);
	}
}


void JsonParse::WriteWebLastMsgListAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	uint8 ret,over;
	uint32 sid,msgnum,returnnum,actualnum;
	XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList LastInfo;
	Json::Value msgInfo;
	Json::Value features;
	Json::Value item;	
	string json_str;

	m_RecvPacket>>ret
				>>sid;

	write["ret"] = ret;
	write["id"] = sid;

	if(ret == RESULT_SUCCESS)
	{
		m_RecvPacket>>msgnum
					>>returnnum
					>>over
					>>actualnum;

		write["msgnum"] = returnnum;
		write["returnnum"] = actualnum;
		write["over"] = over;	
		if(actualnum > 25)
		{			
			CLog::Log(__FUNCTION__, CLog::TYPE_ERROR, "sid %u,actualnum is too big :%u turn into 25",sid,actualnum);
			actualnum = 25;			
		}
		else
		{			
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT, "sid %u,actualnum is %u",sid,actualnum);
		}
		for(size_t i = 0; i < actualnum; i++)
		{
			m_RecvPacket>>LastInfo.merchantID
					>>LastInfo.subAccountID
					>>LastInfo.lastMsgTime
					>>LastInfo.nickname
					>>LastInfo.lastMsg;
			
			CLog::Log(__FUNCTION__, CLog::TYPE_IMPORTANT,
			"LastInfo : merchantID %u subAccountID %u lastMsgTime %u nickname [%u]:%s lastMsg [%u]:%s",
			LastInfo.merchantID,
			LastInfo.subAccountID,
			LastInfo.lastMsgTime,
			strlen(LastInfo.nickname),LastInfo.nickname,
			strlen(LastInfo.lastMsg),LastInfo.lastMsg		
			);

			item["merchantId"] = LastInfo.merchantID;
			item["subAccountId"] = LastInfo.subAccountID;
			item["lastMsgTime"] = LastInfo.lastMsgTime;
			item["subAccountNickname"] = LastInfo.nickname;
			item["lastMsg"] = LastInfo.lastMsg;

			msgInfo.append(item);
		}	
		write["list"] = msgInfo;
	}
	else
	{
		write["msgnum"] = 0;
		write["returnnum"] = 0;
		write["over"] = 1;
	}
}

void JsonParse::WriteWebLogoutCSAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	XT_SERVER_TRANSMIT transmit;
	Json::Value friendInfo;
	Json::Value item;

	m_RecvPacket>>transmit.origin_id
				>>transmit.msgtype;

	write["origin_id"] = transmit.origin_id;
	write["msgtype"] = transmit.msgtype;

	uint8 ret;
	uint32 sid;
	uint32 rid;
	uint16 type;
	
	m_RecvPacket>>ret
				>>sid
				>>rid
				>>type;

	write["ret"] = ret;
	write["id"] = sid;	
	write["sender"] = rid;
	write["type"] = type;	
	write["msg"] = "dy drop client";
	

}



void JsonParse::WriteWebQueueInfoAck(CRecvPacket& m_RecvPacket, Json::Value &write)
{
	uint8 ret;
	uint32 QueueId,QueueLength;
	XT_WEB_QUEUE_LENGTH_IND::WebQueueInd QueueInfo;
	Json::Value Json_QueueInfo;
	Json::Value item;	

	m_RecvPacket>>ret
				>>QueueId;

	write["ret"] = ret;
	write["QueueId"] = QueueId;

	if(ret == RESULT_SUCCESS)
	{
		m_RecvPacket>>QueueLength;

		write["QueueLength"] = QueueLength;


		for(size_t i = 0; i < QueueLength; i++)
		{
			m_RecvPacket>>QueueInfo.UserId
						>>QueueInfo.UserPos;

			item["UserId"] = QueueInfo.UserId;
			item["UserPos"] = QueueInfo.UserPos;

			Json_QueueInfo.append(item);
		}	
		write["QueueInfo"] = Json_QueueInfo;
	}
	else
	{
		write["QueueLength"] = 0;
	}
		
}



