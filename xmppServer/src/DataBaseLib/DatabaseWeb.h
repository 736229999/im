#pragma once
#include "DatabaseGW.h"

class CDatabaseWeb :
	public CDatabaseGW
{
public:
	CDatabaseWeb(void);
	~CDatabaseWeb(void);
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();	//连接WEB数据库

	//用户业务
	int Add_Register_Info(LPXT_REGIST_REQ pRegInfo, uint32 &RetUserID, uint8 &ReturnCode);
	int Get_UserID_PWD(const char *c_pcUserName,uint32 &iUserID,char *pcPassWord);
	int Get_PWD_ByID(uint32 id,char* cPass,char* username,char* mobileno);
	int Get_UserID_PWD_ByMobileNo(const char *c_pcmobileno,uint32 &iUserID,char *pcPassWord,char *username);
	int SaveAccount(char* account,char* pwd,uint32 id,char* truename);
	int SaveAccountBuyer(char* account,char* pwd,char*guid,uint32& id,char* mobileno,char* emailaddress,char*nickname,char*headImg);
	int SaveSubAccount(uint32 uid,uint32 fid,uint32 groupid,char* password,char* nickname,char* truename);
	int Get_Basic_UserInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *st_BasicUserInfo,uint8 &nRetCount);
	int Get_Basic_UserInfo2(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount);
	int Get_User_Info(uint32 iUserID, XT_SERVER_USERINFO_GET_ACK &lpAck);	
	int Get_Agent_Merchant(uint32 iUserID, uint32 &MerchantId);
	int Modify_User_Info(XT_USERINFO_MOD_REQ *pUserInfo);
	int Get_User_JoinFlag(uint32 iUserID, uint8 &joinFlag);	
	int Search_User_WithUserID(uint32 userid, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount);
	int Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount);
	int	Update_User_Money(uint32 clientno, int sendmoney);
	int Modify_User_UnderWrite(uint32 UserID, char* userUnderWrite);
	//好友业务
	int Get_User_FriendList(uint32 iUserID, uint16 startPos, XT_FRIENDLIST_ACK::tagFriendInfo * friendInfo, int &nRetCount, uint8 flag=XT_HEAD::FLAG_CNTREQUEST);
	int Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID);
	int Delete_Friend(uint32 iUserID, uint32 friendID);
	int Get_UserFriend_Comment(uint32 UserID, uint32 friendID, char* comment);
	int Modify_Friend_Comment(uint32 UserID, uint32 friendno, char * comment);
	int Modify_Friend_Group(uint32 UserID, uint32 friendID, uint8 groupID);
	int Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag);
	int Get_User_FriendGroup(uint32 UserID, uint32 friendno, uint8 &groupID);


	//其它业务
	int Get_Web_Msg(uint32 &nPos, ST_WEB_MSG *pWebMsg, uint16 &nRetCount);//每隔一定时间查一次表
	int Get_Operation_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount);//用户更新
	int Get_User_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//收件箱
	int Get_Sys_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//系统广播
	int Get_SysOff_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount, char* uid);//离线系统消息
	int Get_Broadcast_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//用户广播
	int Get_UserComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//个人主页评论
	int Get_LogComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//博客内容评论
	int Get_Mobile_Msg(uint32 &recid, ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount);//手机短信
	int Get_OneMobile_Msg(uint32 &recid, uint32 UserID,ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount);

	int Send_Mobile_Msg(uint32 UserID,char *szNickName, ST_MOBILE_SEND *pMsg);

	int Get_OneOperation_Msg(uint32 &recid, char* strFriend,ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount);
	int Get_OneUser_Msg(uint32 &recid, uint32 userID,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);
	int Get_OneUserComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);
	int Get_OneLogComment(uint32 &recid, uint32 userid,ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);

	int Set_WebMsg_Status(uint32 recID);

	int SendFriendInvite(uint32 userID, char *emailAddress, char *emailTitle, char *emailContent);

	int Get_Offline_WebMsg(uint32 userid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount);
	int Change_User_PWD(uint32 userid);
	int Get_User_FamilyFlag(uint32 *iUserID, XT_USERFAMILYFLAG_GET_ACK::tagUserFamilyFlag *pUserFamilyFlag, uint8 &nRetCount);
	int SetUserMessageStatus(uint32 recID);
	int Get_System_Log(uint32 &recid, ST_SYSTEM_LOG *pWebMsg, uint16 &nRetCount);
	int GetNickName(uint32 userid,char *nickname);
	int GetUserPicUri(uint32 userid,char *picuri);
	int Write_System_Log(uint32 projid,char* szUserid,char* szLogid,uint32 flatid,uint32 operid);
	int PayMoney(uint32 userid,uint32 consumetype, uint32 logid,float money);
	int GetSubAccount(uint32 groupid,std::vector<uint32_t>& ids);
	int GetFlowMode(uint32 id,int& mode);
	int SearchConnAccount(uint32 sid,std::vector<uint32_t>& ids,uint32& did);
	int GetNoReplyNum(uint32 id,uint32 time,uint32& NoReplyNum);
};

