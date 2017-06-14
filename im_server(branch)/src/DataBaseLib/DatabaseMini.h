#pragma once
#include "DatabaseGW.h"
#include "atlTime.h"
#include "../MiniIM/AppManager.h"
#include <string>
#include <vector>
#include "improtocol.pb.h"


class CDatabaseMini :
	public CDatabaseGW
{
public:
	CDatabaseMini(void);
	~CDatabaseMini(void);
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	int Get_Cust_Num(uint32 id, char* btime, char* etime, uint32& nRetCount);
	int Get_Cust_Info(uint32 id, char* btime, char* etime, uint32 count, uint16 start_pos, uint16 iNeedCustCount, uint8 bDirect, XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo* CliInfo, uint32& nRetCount);
	int Mod_Cust_Info(uint32 id, char* account, char* time);
	int Mod_Cust_Remark(uint32 id, char* account, char* time,char* remark);
	int Get_Cust_Remark(uint32 id, char* account, char* time, char* remark);
	int Get_User_GroupInfo(const uint32 c_iUserID, XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo, uint8 &nGroupNum, uint8 flag);
	int WebMsgSave(XT_TALK* msg,uint32 logintime);
	int WebRecommSave(uint32 fid,uint32 did,uint16 recomm,uint32 time,uint32 logintime,char* msg);
	int WebServiceSave(uint32 fid,uint32 did,uint16 recomm,uint32 time);
	int WebMsgReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,uint32& nRetCount);	
	int WebMsgMoreReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,std::vector<uint32_t> &ids,uint32& nRetCount);
	int WebBusiListReq(XT_WEB_BUSI_LIST_REQ* BusiListReq,XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount);
	int XmppBusiListReq(XT_XMPP_BUSI_LIST_REQ* BusiListReq,XT_XMPP_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount);
	int Get_User_OnlineTime(uint32 iUserID,uint32 &OnlineTime);

	int Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName, uint8 &iGroupPid);
	int Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName);
	int Delete_Group(uint32 iUserID, uint8 iGroupID);

	int Save_User_Login(uint32 userid,uint32 userip, uint16 userport, int serverno);
	int Save_User_Logout(uint32 userid);
	int Update_All_Logout(int serverno);
	int Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count);
	int Save_User_OnlineTime(uint32 userid,int logintm,int logouttm, int &money);
	int Save_User_Timelen(uint32 userid, int logintm, int logouttm);

    int Save_Merchant_Login(uint32 userid,int logintm);

    int Save_Merchant_Logout(uint32 userid,int logouttm);

	int Save_Buyer_Login(uint32 userid,int logintm);
    int Save_Buyer_Logout(uint32 userid,int logouttm);
	//消息处理
	int Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen, const char *strText);
	int Save_Offline_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,
						uint16 iMsgType,const char *strMsg, uint16 iMsgLen, const char *strText,uint32 sendtime,char *uuid);

	int Get_Family_MaxMsgID(const uint32 c_iFamilyID,  uint32 & nMaxMsgID, uint32 &nMinMsgID);
	int Save_Family_RequestMsg(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen);
	int Save_Family_RequestMsg2(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen);
	int Delete_Family_Msg(const uint32 iFamilyID);
	int Save_Family_Msg(uint32 familyMsgID,uint32 familyID,uint32 senderID, int tm, int msgType,int msgLen,char *msg);

	int Save_User_RecvFamilyMsgID(uint32 userID, uint32 familyID, uint32 msgID);
	int Get_User_FamilyMsgID(uint32 userID, uint32 familyID, uint32 &msgID);

	int Delete_All_Offline_Msg(uint32 userID);
	int Get_Family_Offline_Msg(uint32 userID, uint32 familyID, uint32 &msgID, ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount);
	int Get_Family_Offline_SysMsg(uint32 userID, uint32 familyID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount);
	int Get_User_Offline_Msg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount);
	int Get_User_Offline_SysMsg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount);

	int Get_Pub_Msg(uint32 userID,ST_WEB_MSG *pMsg, uint8 &nRetCount);
	int Save_OnlineUser_Num(uint32 onlinenum);
	int Delete_FamilyMem_RecvMsg(const uint32 iFamilyID, uint32 iUserID);
	int SetWokuMsgRecvID(uint32 userID, uint8 Type, uint32 recID);
	int GetWokuMsgID(uint32 userID,uint32 &nOperationID,uint32 &nUserMsgID,uint32 &nSysMsgID,uint32 &nBroadcastID,uint32 &nUsercommentID,uint32 &nLogcommentID);
	//int Get_WebIM_Msg(int ServerNo,ST_WEBIM_MSG *pWebIMMsg, uint16 &nRetCount);

	// 获取时间段之间的历史消息
	int WebHistoryMsg(uint32 nFromID, uint32 nToID, uint32 nStartMsgID, uint32 nStartTime, uint32 nEndTime, uint32 nMaxMsg, uint32& nRetCount, MsgList &lsMsg);

	int TestContext(void);
	int Modify_CS_Pwd(uint32 id, char* pwd);
	int Import_CS_Req(char *username,int permission,int receptionlimit,char *importname,uint32 &userid,uint32 importtime);
	int Modify_CS_Req(char* username, char* csRealName,int receptionLimit,char* password,uint32 &importtime,uint32 csid,int permission);
	int Modify_CS_Status(char* username, int status);
	int Get_Vaild_CS_List(XT_CS_INFO * CSList, int &nRetCount);
	int Qurery_CS_Info(XT_CS_INFO * CSInfoList,char *username, char *importname, char *realname, int status,uint32 begintime,uint32 endtime,int &nRetCount);
	
	int CS_Get_Groups(uint32 userid,vector<uint32> &groups );
	int CS_Get_Group_Account(uint32 groupId,vector<uint32> &userlist);
	int CS_Get_Group_Friend(uint32, vector<csTeam_t>& teamInfo);	
	int Get_CS_Info(uint32 iUserID, XT_SERVER_CS_GET_ACK &lpAck);
	int CS_Get_RecptionLimit(uint32 iUserID,uint32 &RecpLimit);	
	int WebLastMsgReq(XT_WEB_BUSI_LAST_MSG_REQ* msgReq,XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList * msgInfo,uint32& nRetCount);
	int Get_Userid_Map_CSid(uint32 userid,uint32 &csid);
	// 插入一条消息推送数据
	//int InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const std::string &buffer, int nBufferType);
	// 查询推送消息数据
	//int SearchMsgNotify(const char *pUuid, std::string &buffer, int &nBufferType);
	// 查找订单提醒方式
	int GetMsgRemind(uint32 userid, MsgRemindAck &ack);
	// 写入订单提醒方式
	int SetMsgRemind(uint32 userid, const MsgRemindAck &ack);
	// 获取最近联系人
	int GetRecentContact(const char *pGuid, uint32 starttime, std::vector<uint32> &arrID);
	// 删除一条记录
	//int DeleteMsgNotify(const char *pUuid, uint32 userid);
	// 获取商家需要推送的消息记录
	int GetMsgNotifyDataOnePerson(uint32 userid, std::vector<std::string> &arrUUID);
};
