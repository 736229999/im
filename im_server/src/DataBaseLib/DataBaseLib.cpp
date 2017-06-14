// DataBaseLib.cpp : 定义 DLL 应用程序的入口点。
//

#include "stdafx.h"
#include "DataBaseLib.h"
#include "DatabaseMini.h"
#include "DatabaseWeb.h"
#include "DatabaseFamily.h"

BOOL APIENTRY DllMain( HANDLE hModule, 
                       DWORD  ul_reason_for_call, 
                       LPVOID lpReserved
					 )
{
	switch (ul_reason_for_call)
	{
	case DLL_PROCESS_ATTACH:
	case DLL_THREAD_ATTACH:
	case DLL_THREAD_DETACH:
	case DLL_PROCESS_DETACH:
		break;
	}
    return TRUE;
}




// 这是已导出类的构造函数。
// 有关类定义的信息，请参阅 DataBaseLib.h
CDataBaseMini::CDataBaseMini()
{ 
	m_MiniDB = new CDatabaseMini();
}

CDataBaseMini::~CDataBaseMini(void)
{
	if(m_MiniDB != NULL) delete (CDatabaseMini*)m_MiniDB;
}

int CDataBaseMini::TestContext(void)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return	MiniDB->TestContext();
}
void CDataBaseMini::SetConnectDB(char* uid, char* pwd, char* sid)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return	MiniDB->SetConnectDB(uid, pwd, sid);
}

bool CDataBaseMini::ConnectDB()
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return	MiniDB->ConnectDB();
}

void CDataBaseMini::DisConnect()
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return	MiniDB->DisConnect();
}

int CDataBaseMini::Mod_Cust_Info(uint32 id, char* account, char* time)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Mod_Cust_Info(id, account, time);
}

int CDataBaseMini::Get_Cust_Info(uint32 id, char* btime, char* etime, uint32 count, uint16 start_pos, uint16 iNeedCustCount, uint8 bDirect, XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo* CliInfo, uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Cust_Info(id, btime, etime, count, start_pos, iNeedCustCount, bDirect, CliInfo, nRetCount);
}

int CDataBaseMini::Mod_Cust_Remark(uint32 id, char* account, char* time, char* remark)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Mod_Cust_Remark(id, account, time, remark);
}

int CDataBaseMini::Get_Cust_Remark(uint32 id, char* account, char*time, char* remark)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Cust_Remark(id, account, time, remark);
}



int CDataBaseMini::Get_Cust_Num(uint32 id, char* btime, char* etime, uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Cust_Num(id, btime, etime, nRetCount);
}

int CDataBaseMini::Get_User_GroupInfo(const uint32 c_iUserID, XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo, uint8 &nGroupNum, uint8 flag)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_User_GroupInfo(c_iUserID, pGroupInfo, nGroupNum, flag);
}

int CDataBaseMini::WebMsgSave(XT_TALK* msg,uint32 logintime)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebMsgSave(msg,logintime);
}

int CDataBaseMini::WebRecommSave(uint32 fid,uint32 did,uint16 recomm,uint32 time,uint32 logintime,char *msg)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebRecommSave(fid,did,recomm,time,logintime,msg);
}

int CDataBaseMini::WebServiceSave(uint32 fid,uint32 did,uint16 recomm,uint32 time)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebServiceSave(fid,did,recomm,time);
}

int CDataBaseMini::WebMsgReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebMsgReq(msgReq,msgInfo,nRetCount);
}

int CDataBaseMini::WebMsgMoreReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,std::vector<uint32_t> &ids,uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebMsgMoreReq(msgReq,msgInfo,ids,nRetCount);
}

int CDataBaseMini::WebBusiListReq(XT_WEB_BUSI_LIST_REQ* BusiListReq,XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebBusiListReq(BusiListReq,pListInfo,nRetCount);
}

int CDataBaseMini::Get_Family_MaxMsgID(const uint32 c_iFamilyID, uint32 & nMaxMsgID, uint32 &nMinMsgID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Family_MaxMsgID(c_iFamilyID, nMaxMsgID,nMinMsgID);
}

int CDataBaseMini::Save_Family_RequestMsg(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Family_RequestMsg(c_iFamilyID, iSenderID, iSendTime, strMsg, iMsgLen);
}
int CDataBaseMini::Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen, const char *strText)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_Msg(iRecvID,iSendID,iNatIP,iNatPort,iTime,iMsgType,strMsg,iMsgLen,strText);
}

int CDataBaseMini::Delete_Family_Msg(const uint32 iFamilyID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Delete_Family_Msg(iFamilyID);
}

int CDataBaseMini::Save_Family_Msg(uint32 familyMsgID,uint32 familyID,uint32 senderID, int tm, int msgType,int msgLen,const char *msg)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Family_Msg(familyMsgID,familyID,senderID, tm, msgType, msgLen,(char*)msg);
}
int CDataBaseMini:: Get_User_OnlineTime(uint32 iUserID,uint32 &OnlineTime)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_User_OnlineTime(iUserID, OnlineTime);
}

int CDataBaseMini::Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName,uint8 &iGroupPid)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Add_Group(iUserID, iGroupID, strGroupName, iGroupPid);
}

int CDataBaseMini::Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Modify_Group(iUserID, iGroupID, strGroupName);
}

int CDataBaseMini::Delete_Group(uint32 iUserID, uint8 iGroupID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Delete_Group(iUserID, iGroupID);
}
//这里是获取可以分流的客服
int CDataBaseMini::CS_Get_Group_Account(uint32 groupId,vector<uint32> &userlist)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->CS_Get_Group_Account(groupId, userlist);
}

int CDataBaseMini::CS_Get_Groups(uint32 userid,vector<uint32> &groups )
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->CS_Get_Groups(userid, groups);

}

int CDataBaseMini::CS_Get_RecptionLimit(uint32 iUserID,uint32 &RecpLimit)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->CS_Get_RecptionLimit(iUserID, RecpLimit);
}

int CDataBaseMini::CS_Get_Group_Friend(uint32 iUserID,vector<csTeam_t>&teamInfo)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->CS_Get_Group_Friend(iUserID, teamInfo);
}

int CDataBaseMini::Get_CS_Info(uint32 iUserID, XT_SERVER_CS_GET_ACK &lpAck)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_CS_Info(iUserID, lpAck);
}

int CDataBaseMini::Save_User_Login(uint32 userid,uint32 userip,uint16 userport,int serverno)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_Login(userid,userip,userport,serverno);
}

int CDataBaseMini::Save_User_Logout(uint32 userid)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_Logout(userid);
}

int CDataBaseMini::Update_All_Logout(int serverno)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Update_All_Logout(serverno);
}

int CDataBaseMini::Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_Action(userid,action,count);
}
int CDataBaseMini::Save_User_Timelen(uint32 userid, int logintm, int logouttm)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_Timelen(userid,logintm,logouttm);
}

int CDataBaseMini::Save_Merchant_Login(uint32 userid, int logintm)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Merchant_Login(userid,logintm);
}

int CDataBaseMini::Save_Merchant_Logout(uint32 userid, int logouttm)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Merchant_Logout(userid,logouttm);
}

int CDataBaseMini::Save_Buyer_Login(uint32 userid, int logintm)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Buyer_Login(userid,logintm);
}

int CDataBaseMini::Save_Buyer_Logout(uint32 userid, int logouttm)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_Buyer_Logout(userid,logouttm);
}

int CDataBaseMini::Save_User_OnlineTime(uint32 clientno,int logintm,int logouttm, int &money)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_OnlineTime(clientno,logintm,logouttm, money);
}

int CDataBaseMini::Save_User_RecvFamilyMsgID(uint32 userID, uint32 familyID, uint32 msgID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_User_RecvFamilyMsgID(userID, familyID, msgID);
}

int CDataBaseMini::Get_User_FamilyMsgID(uint32 userID, uint32 familyID, uint32 &msgID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_User_FamilyMsgID(userID, familyID, msgID);
}

int CDataBaseMini::Get_Family_Offline_Msg(uint32 userID, uint32 familyID, uint32 &msgID, ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Family_Offline_Msg(userID, familyID, msgID, pFamilyMsg, nRetCount);
}

int CDataBaseMini::Delete_All_Offline_Msg(uint32 userID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Delete_All_Offline_Msg(userID);
}

int CDataBaseMini::Get_Family_Offline_SysMsg(uint32 userID, uint32 familyID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Family_Offline_SysMsg(userID, familyID,pFamilyMsg, nRetCount);
}

int CDataBaseMini::Get_User_Offline_Msg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_User_Offline_Msg(userID,pMsg, nRetCount);
}

int CDataBaseMini::Get_User_Offline_SysMsg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_User_Offline_SysMsg(userID,pMsg, nRetCount);
}
int CDataBaseMini::Get_Pub_Msg(uint32 userID,ST_WEB_MSG *pMsg, uint8 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Pub_Msg(userID,pMsg, nRetCount);
}
int CDataBaseMini::Save_OnlineUser_Num(uint32 onlinenum)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Save_OnlineUser_Num(onlinenum);
}
int CDataBaseMini::Delete_FamilyMem_RecvMsg(const uint32 iFamilyID, uint32 iUserID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Delete_FamilyMem_RecvMsg(iFamilyID,iUserID);
}

int CDataBaseMini::SetWokuMsgRecvID(uint32 userID, uint8 Type, uint32 recID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->SetWokuMsgRecvID(userID, Type, recID);
}
int CDataBaseMini::GetWokuMsgID(uint32 userID,uint32 &nOperationID,uint32 &nUserMsgID,uint32 &nSysMsgID,uint32 &nBroadcastID,uint32 &nUsercommentID,uint32 &nLogcommentID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->GetWokuMsgID(userID,nOperationID,nUserMsgID,nSysMsgID,nBroadcastID,nUsercommentID,nLogcommentID);
}
/*
int  CDataBaseMini::Get_WebIM_Msg(int ServerNo,ST_WEBIM_MSG *pWebIMMsg, uint16 &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_WebIM_Msg(ServerNo,pWebIMMsg, nRetCount);
}
*/
int  CDataBaseMini::Modify_CS_Pwd(uint32 id, char* pwd)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Modify_CS_Pwd(id,pwd);
}
int CDataBaseMini::Import_CS_Req(char *username,int permission,int receptionlimit,char *importname,uint32 &userid,uint32 importtime)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Import_CS_Req(username,permission,receptionlimit,importname,userid,importtime);
}
int CDataBaseMini::Modify_CS_Req(char* username, char* csRealName,int receptionLimit,char* password,uint32 &importtime,uint32 csid,int permission)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Modify_CS_Req(username,csRealName,receptionLimit,password,importtime,csid,permission);
}

int CDataBaseMini::Modify_CS_Status(char* username, int status)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Modify_CS_Status(username,status);
}

int CDataBaseMini::Get_Vaild_CS_List(XT_CS_INFO * CSList, int &nRetCount)

{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Vaild_CS_List(CSList,nRetCount);
}
int CDataBaseMini::Qurery_CS_Info(XT_CS_INFO * CSInfoList,char *username, char *importname, char *realname, int status,uint32 begintime,uint32 endtime,int &nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Qurery_CS_Info(CSInfoList,username, importname, realname,status,begintime,endtime,nRetCount);
}

int CDataBaseMini::WebLastMsgReq(XT_WEB_BUSI_LAST_MSG_REQ* msgReq,XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList * msgInfo,uint32& nRetCount)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->WebLastMsgReq(msgReq,msgInfo,nRetCount);

}
int CDataBaseMini::Get_Userid_Map_CSid(uint32 userid,uint32 &csid)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->Get_Userid_Map_CSid(userid,csid);
}
/*
// 插入一条消息推送数据
int CDataBaseMini::InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const string &buffer, int nBufferType)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->InsertMsgNotify(pUuid, vecUserID, buffer, nBufferType);
}

// 查询推送消息数据
int CDataBaseMini::SearchMsgNotify(const char *pUuid, std::string &buffer, int &nBufferType)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->SearchMsgNotify(pUuid, buffer, nBufferType);
}
*/
// 查找订单提醒方式
int CDataBaseMini::GetMsgRemind(uint32 userid, MsgRemindAck &ack)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->GetMsgRemind(userid, ack);
}

// 写入订单提醒方式
int CDataBaseMini::SetMsgRemind(uint32 userid, const MsgRemindAck &ack)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->SetMsgRemind(userid, ack);
}

// 获取最近联系人
int CDataBaseMini::GetRecentContact(const char *pGuid, uint32 starttime, std::vector<uint32> &arrID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->GetRecentContact(pGuid, starttime, arrID);
}
/*
// 删除一条记录
int CDataBaseMini::DeleteMsgNotify(const char *pUuid, uint32 userid)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->DeleteMsgNotify(pUuid, userid);
}
*/
// 获取商家需要推送的消息记录
int CDataBaseMini::GetMsgNotifyDataOnePerson(uint32 userid, std::vector<std::string> &arrUUID)
{
	CDatabaseMini * MiniDB = (CDatabaseMini * )m_MiniDB;
	return MiniDB->GetMsgNotifyDataOnePerson(userid, arrUUID);
}




CDataBaseWeb::CDataBaseWeb()
{ 
	m_WebDB = new CDatabaseWeb();
}

CDataBaseWeb::~CDataBaseWeb()
{
	if(m_WebDB != NULL) delete (CDatabaseWeb*)m_WebDB;
}
void CDataBaseWeb::SetConnectDB(char* uid, char* pwd, char* sid)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return	WebDB->SetConnectDB(uid, pwd, sid);
}

bool CDataBaseWeb::ConnectDB()
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->ConnectDB();
}
void CDataBaseWeb::DisConnect()
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->DisConnect();
}


int CDataBaseWeb::Add_Register_Info(LPXT_REGIST_REQ pRegInfo, uint32 &RetUserID, uint8 &ReturnCode)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Add_Register_Info(pRegInfo, RetUserID, ReturnCode);
}
int CDataBaseWeb::Get_UserID_PWD(const char *c_pcUserName,uint32 &iUserID,char *pcPassWord)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_UserID_PWD(c_pcUserName, iUserID, pcPassWord);
}
int CDataBaseWeb::Get_CS_PWD(char *c_pcUserName,char *pcPassWord,uint32 &iUserID,uint8 &state)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_CS_PWD(c_pcUserName, pcPassWord, iUserID, state);
}
int CDataBaseWeb::Get_PWD_ByID(uint32 id,char* cPass,char* username,char* mobileno)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_PWD_ByID(id,cPass, username,mobileno);
}
int CDataBaseWeb::Get_UserID_PWD_ByMobileNo(const char *c_pcmobileno,uint32 &iUserID,char *pcPassWord,char *username)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_UserID_PWD_ByMobileNo(c_pcmobileno,iUserID, pcPassWord,username);
}
int CDataBaseWeb::SaveAccount(char* account,char* pwd,uint32 id,char* truename)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SaveAccount(account,pwd,id,truename);
}
int CDataBaseWeb::SaveAccountBuyer(char* account,char* pwd, char* guid, uint32& id, char* mobileno, char* emailaddress, char*nickname, char*headImg)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SaveAccountBuyer(account, pwd, guid, id, mobileno, emailaddress, nickname, headImg);
}

int CDataBaseWeb::SaveAccountWork(char* account,char* pwd, uint32& id, char* mobileno, char* emailaddress, char*nickname, char*headImg, char*groupname)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SaveAccountWork(account, pwd, id, mobileno, emailaddress, nickname, headImg,groupname);
}

int CDataBaseWeb::SaveSubAccount(uint32 uid,uint32 fid,uint32 groupid,char* password,char* nickname,char* truename)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SaveSubAccount(uid,fid,groupid,password,nickname,truename);
}

int CDataBaseWeb::Get_Basic_UserInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
    return WebDB->Get_Basic_UserInfo2(requestID,iUserID,pBasicUserInfo, nRetCount);
}

int CDataBaseWeb::Get_User_Info(uint32 iUserID, XT_SERVER_USERINFO_GET_ACK &lpAck)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_Info(iUserID, lpAck);
}

int CDataBaseWeb::Get_Agent_Merchant(uint32 iUserID, uint32 &MerchantId)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Agent_Merchant(iUserID, MerchantId);
}

int CDataBaseWeb::Get_User_FriendList(uint32 iUserID, uint16 startPos, XT_FRIENDLIST_ACK::tagFriendInfo * friendInfo, int &nRetCount, uint8 flag)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_FriendList(iUserID, startPos, friendInfo, nRetCount, flag);
}

int CDataBaseWeb::Get_User_JoinFlag(uint32 iUserID, uint8 &joinFlag)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_JoinFlag(iUserID, joinFlag);
}

int CDataBaseWeb::Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Add_Friend(iUserID, friendID, groupID);
}

int CDataBaseWeb::Delete_Friend(uint32 iUserID, uint32 friendID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Delete_Friend(iUserID, friendID);
}
int CDataBaseWeb::Search_User_WithUserID(uint32 userid, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Search_User_WithUserID(userid, friendInfo);
}

int CDataBaseWeb::Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Search_User_WithUserName(UserName, friendInfo);
}

int CDataBaseWeb::Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Search_User_WithNickName(NickName,startPos, friendInfo,nRetCount);
}

int CDataBaseWeb::Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Search_User_WithEmail(Email,startPos,friendInfo, nRetCount);
}

int CDataBaseWeb::Modify_User_Info(XT_USERINFO_MOD_REQ *pUserInfo)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Modify_User_Info(pUserInfo);
}

int CDataBaseWeb::Modify_Friend_Comment(uint32 UserID, uint32 friendno, char * comment)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Modify_Friend_Comment(UserID,friendno,comment);
}

int CDataBaseWeb::Update_User_Money(uint32 clientno, int sendmoney)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Update_User_Money(clientno, sendmoney);
}

int CDataBaseWeb::Modify_User_UnderWrite(uint32 UserID, char* userUnderWrite)
{
	CDatabaseWeb *WebDB = (CDatabaseWeb *)m_WebDB;
	return WebDB->Modify_User_UnderWrite(UserID, userUnderWrite);
}

int CDataBaseWeb::Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag)
{
	CDatabaseWeb *WebDB = (CDatabaseWeb *)m_WebDB;
	return WebDB->Modify_Friend_Flag(UserID, friendno,flag);
}

int CDataBaseWeb::Get_Web_Msg(uint32 &nPos, ST_WEB_MSG *pWebMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Web_Msg(nPos, pWebMsg, nRetCount);
}

int CDataBaseWeb::Get_Operation_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Operation_Msg(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_User_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_Msg(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_Sys_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Sys_Msg(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_SysOff_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount, char* uid)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_SysOff_Msg(recid, pOperationMsg, nRetCount, uid);
}
int CDataBaseWeb::Get_Broadcast_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Broadcast_Msg(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_UserComment(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_UserComment(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_LogComment(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_LogComment(recid, pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_Mobile_Msg(uint32 &recid, ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Mobile_Msg(recid, pUserMsg, nRetCount);
}
int CDataBaseWeb::Get_OneMobile_Msg(uint32 &recid, uint32 UserID,ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_OneMobile_Msg(recid,UserID,pUserMsg, nRetCount);
}
int CDataBaseWeb::Send_Mobile_Msg(uint32 UserID,char *szNickName, ST_MOBILE_SEND *pMsg)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Send_Mobile_Msg(UserID,szNickName,pMsg);
}

int CDataBaseWeb::Get_OneOperation_Msg(uint32 &recid, char* strFriend,ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_OneOperation_Msg(recid, strFriend,pOperationMsg, nRetCount);
}
int CDataBaseWeb::Get_OneUser_Msg(uint32 &recid, uint32 userID,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_OneUser_Msg(recid, userID,pUserMsg, nRetCount);
}
int CDataBaseWeb::Get_OneUserComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_OneUserComment(recid, userid,pUserMsg, nRetCount);
}
int CDataBaseWeb::Get_OneLogComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_OneLogComment(recid, userid,pUserMsg, nRetCount);
}


int CDataBaseWeb::Get_Offline_WebMsg(uint32 userid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_Offline_WebMsg(userid, pWebMsg, nRetCount);
}

int CDataBaseWeb::Set_WebMsg_Status(uint32 recID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Set_WebMsg_Status(recID);
}

int CDataBaseWeb::Modify_Friend_Group(uint32 userID, uint32 friendID, uint8 groupID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Modify_Friend_Group(userID, friendID, groupID);
}
int CDataBaseWeb::Get_UserFriend_Comment(uint32 UserID, uint32 friendID, char* comment)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_UserFriend_Comment(UserID, friendID, comment);
}
int CDataBaseWeb::SendFriendInvite(uint32 userID, char *emailAddress, char *emailTitle, char *emailContent)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SendFriendInvite(userID, emailAddress,emailTitle, emailContent);
}

int CDataBaseWeb::Change_User_PWD(uint32 userid)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Change_User_PWD(userid);
}

int CDataBaseWeb::Get_User_FriendGroup(uint32 UserID, uint32 friendno, uint8 &groupID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_FriendGroup(UserID, friendno,groupID);
}

int  CDataBaseWeb::Get_User_FamilyFlag(uint32 *iUserID, XT_USERFAMILYFLAG_GET_ACK::tagUserFamilyFlag *pUserFamilyFlag, uint8 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_User_FamilyFlag(iUserID, pUserFamilyFlag, nRetCount);
}

int CDataBaseWeb::SetUserMessageStatus(uint32 recID)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SetUserMessageStatus(recID);
}

int CDataBaseWeb::Get_System_Log(uint32 &recid, ST_SYSTEM_LOG *pWebMsg, uint16 &nRetCount)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Get_System_Log(recid, pWebMsg, nRetCount);
}

int CDataBaseWeb::GetNickName(uint32 userid,char *nickname)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->GetNickName(userid,nickname);
}
int CDataBaseWeb::GetUserPicUri(uint32 userid,char *picuri)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->GetUserPicUri(userid,picuri);
}
int CDataBaseWeb::Write_System_Log(uint32 projid,char* szUserid,char* szLogid,uint32 flatid,uint32 operid)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->Write_System_Log(projid,szUserid,szLogid,flatid,operid);
}
int CDataBaseWeb::PayMoney(uint32 userid,uint32 consumetype, uint32 logid,float money)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->PayMoney(userid,consumetype,logid,money);
}
int CDataBaseWeb::GetSubAccount(uint32 groupid,std::vector<uint32_t>& ids)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->GetSubAccount(groupid,ids);
}
int CDataBaseWeb::GetFlowMode(uint32 id,int& mode)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->GetFlowMode(id,mode);
}
int CDataBaseWeb::SearchConnAccount(uint32 sid,std::vector<uint32_t>& ids, uint32& did)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->SearchConnAccount(sid,ids,did);
}
int CDataBaseWeb::GetNoReplyNum(uint32 id,uint32 time,uint32& NoReplyNum)
{
	CDatabaseWeb * WebDB = (CDatabaseWeb * )m_WebDB;
	return WebDB->GetNoReplyNum(id,time,NoReplyNum);
}


CDataBaseFamily::CDataBaseFamily()
{ 
	m_FamilyDB = new CDatabaseFamily();
}

CDataBaseFamily::~CDataBaseFamily()
{
	if(m_FamilyDB != NULL) delete (CDataBaseFamily*)m_FamilyDB;
}
void CDataBaseFamily::SetConnectDB(char* uid, char* pwd, char* sid)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return	FamilyDB->SetConnectDB(uid, pwd, sid);
}

bool CDataBaseFamily::ConnectDB()
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->ConnectDB();
}
void CDataBaseFamily::DisConnect()
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->DisConnect();
}

int CDataBaseFamily::Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_FamilyMem_Porp(familyID, userID,  userProp);
}


int CDataBaseFamily::Get_Family_List(const uint32 c_iUserID, uint32 *FamilyID, int &FamilyCount)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_Family_List(c_iUserID, FamilyID, FamilyCount);
}

int CDataBaseFamily::Get_Family_Info(const uint32 c_iFamilyID, LPXT_GET_CINFO_ACK lpAck)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_Family_Info(c_iFamilyID, lpAck);

}
int CDataBaseFamily::Get_Family_Mem(const uint32 nFamilyID, LPST_FAMILY_MEMBER pFamilyMem, int &nMemNum)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_Family_Mem(nFamilyID, pFamilyMem, nMemNum);

}

int	CDataBaseFamily::Set_Family_Info(LPXT_SET_CINFO_REQ lpReq)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Set_Family_Info(lpReq);
}

int CDataBaseFamily::Create_Family(const uint32 iCreaterID, LPXT_CREATE_CLUSTER_REQ lpReq, uint32 &iFamilyID)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Create_Family(iCreaterID, lpReq, iFamilyID);
}

int	CDataBaseFamily::Family_Add_Member(const uint32 iFamilyID, uint32 *iUserID, int &nReqCount)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Family_Add_Member(iFamilyID, iUserID, nReqCount);
}

int CDataBaseFamily::Family_Delete_Member(const uint32 iFamilyID, uint32 iUserID)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Family_Delete_Member(iFamilyID, iUserID);
}

int CDataBaseFamily::Family_SetMem_Prop(const uint32 iFamilyID, uint32 iUserID, uint8 iMemProp)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Family_SetMem_Prop(iFamilyID, iUserID, iMemProp);
}

int CDataBaseFamily::Family_Find(XT_CLUSTER_SEARCH_REQ Req,XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet *FamilyData, int &nRetCount)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Family_Find(Req,FamilyData,nRetCount);
}

int CDataBaseFamily::Delete_Family(const uint32 iFamilyID)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Delete_Family(iFamilyID);
}
int	CDataBaseFamily::Get_Family_BlackList(uint32 familyID, uint32 userID, int &retCode)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_Family_BlackList(familyID,userID,retCode);
}

int	CDataBaseFamily::Get_Mem_Profile(uint32 familyID, uint32 userID, char* profileName)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_Mem_Profile(familyID, userID, profileName);
}

int	CDataBaseFamily::Get_All_Profile(uint32 iFamilyID, CMEM_INFO* profileName,uint32& nRetCnt)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Get_All_Profile(iFamilyID, profileName, nRetCnt);
}


int CDataBaseFamily::Set_Mem_Profile(uint32 iFamilyID, uint32 iUserID, const char* profileName)
{
	CDatabaseFamily * FamilyDB = (CDatabaseFamily * )m_FamilyDB;
	return FamilyDB->Set_Mem_Profile(iFamilyID, iUserID, profileName);
}

