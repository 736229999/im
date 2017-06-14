
#pragma once

#ifdef DATABASELIB_EXPORTS
#define DATABASELIB_API 
#else
#define DATABASELIB_API 
#endif

#include "DataBaseStruct.h"
#include "ClientProtocol.h"

#include "improtocol.pb.h"


/****************************数据库操作接口******************************
所有接口成功返回DB_SUCCESS
其它错误码可通过CError::DB_Error(errCode, szError[255], true)获取错误描述
***********************************************************************/
// 此类是从 DataBaseLib.dll 导出的
class DATABASELIB_API CDataBaseMini
{
public:
	CDataBaseMini(void);
	~CDataBaseMini(void);
/////////////////////*****************外部接口定义********************///////////////
	//连接mysql数据库
	bool ConnectMysql();

	//连接oracle数据库
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	void DisConnect();
	int TestContext(void);

	int Get_Cust_Num(uint32 id, char* btime, char* etime, uint32& nRetCount);
	//================================================================
	int Get_Cust_Info(uint32 id, char* btime, char* etime, uint32 count, uint16 start_pos, uint16 iNeedCustCount, uint8 bDirect, XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo* CliInfo, uint32& nRetCount);
	//================================================================ 
	int Mod_Cust_Info(uint32 id, char* account, char* time);
	int Mod_Cust_Remark(uint32 id, char* account, char* time,char* remark);
	
	int Get_Cust_Remark(uint32 id, char* account, char* time, char* remark);
	int Modify_CS_Pwd(uint32 id, char* pwd);
	
	/*================================================================ 
	*
	* 函 数 名：Get_User_GroupInfo
	** 根据用户名分组信息
	* 参 数：
	* uint32 c_iUserID [[IN]] : 用户ID
	* XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo [IN,OUT]: 用户分组信息
	* int &nGroupNum [IN,OUT] : 所要获取的分组数和返回的分组数，如果输入为0，则输出存在的数目。
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;
	* 
	================================================================*/
	int Get_User_GroupInfo(const uint32 c_iUserID, XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo, uint8 &nGroupNum, uint8 flag);
	int WebMsgSave(XT_TALK* id,uint32 logintime);
	int WebRecommSave(uint32 fid,uint32 did,uint16 recomm,uint32 time,uint32 logintime,char* msg);
	int WebServiceSave(uint32 fid,uint32 did,uint16 recomm,uint32 time);
	int WebMsgReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,uint32& nRetCount);	
	int WebMsgMoreReq(XT_WEB_MORE_MSG_REQ* msgReq,XT_WEB_MORE_MSG_ACK::MsgInfo* msgInfo,std::vector<uint32_t> &ids,uint32& nRetCount);
	int WebBusiListReq(XT_WEB_BUSI_LIST_REQ* BusiListReq,XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo,uint32& nRetCount);
	/*================================================================ 
	*
	* 函 数 名：Get_Family_MaxMsgID
	** 根据家族ID获取当前家族最大消息ID
	* 参 数：
	* uint32 c_iFamilyID [[IN]] : 家族ID
	* uint32 & nMsgID [IN,OUT]: 家族当前消息ID
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;  如果家族创建后无任何消息，nMsgID = 0;
	* 
	================================================================*/
	int Get_Family_MaxMsgID(const uint32 c_iFamilyID, uint32 & nMaxMsgID, uint32 &nMinMsgID);

	/*================================================================ 
	*
	* 函 数 名：Save_Family_RequestMsg
	** 保存家族请求消息ID获取当前家族最大消息ID
	* 参 数：
	* uint32 c_iFamilyID	[IN] : 家族ID
	* uint32 iSenderID		[IN] : 发送者ID
	* uint32 iSendTime		[IN] : 请求时间
	* char   *strMsg		[IN] : 请求内容
	* int	 iMsgLen		[IN] : 内容长度
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;
	* 
	================================================================*/ 
	int Save_Family_RequestMsg(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen);
	
	/*================================================================ 
	*
	* 函 数 名：Save_User_Msg
	** 保存用户的消息，包括系统消息
	* 参 数：
	* uint32 iRecvID	[IN] : 家族ID
	* uint32 iSendID		[IN] : 发送者ID
	* uint32 iNatIP,uint16 iNatPort,uint32 iTime [IN] 
	* uint16 iMsgType		[IN] : 消息类型
	* char   *strMsg		[IN] : 消息内容
	* uint16	 iMsgLen		[IN] : 内容长度
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;
	* 
	================================================================*/ 
	int Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen, const char *strText);

	/*================================================================ 
	*
	* 函 数 名：Delete_Family_Msg
	** 删除家族的消息
	* 参 数：
	const uint32 iFamilyID	[IN] : 删除家族
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Delete_Family_Msg(const uint32 iFamilyID);

	int Save_Family_Msg(uint32 familyMsgID,uint32 familyID,uint32 senderID, int tm, int msgType,int msgLen,const char *msg);
	
	int Get_User_OnlineTime(uint32 iUserID,uint32 &OnlineTime);	//获取用户在线时长


	/*================================================================ 
	*
	* 函 数 名：Add_Group
	** 添加分组
	* 参 数：
	* uint32 iUserID [[IN]] : 用户ID
	* uint8 &iGroupID [IN,OUT]: 组ID，输入指定的ID。输出数据库中存储的ID
	* char * strGroupName	[IN] : 组名
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 数据库错误:DB_ERROR;  用户能创建的组已满：DB_GROUP_FULL; 组名已经存在:DB_GROUP_EXIST;
	* 
	================================================================*/ 
	int Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName, uint8 &iGroupPid);
	int Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName);
	int Delete_Group(uint32 iUserID, uint8 iGroupID);
	int Save_User_Login(uint32 userid,uint32 userip,uint16 userport,int serverno);
	int Save_User_Logout(uint32 userid);
	int Update_All_Logout(int serverno);
	int Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count);
	int Save_User_OnlineTime(uint32 clientno,int logintm,int logouttm, int &money);//返回DB_SUCCESS,money返回该增加的金币数 调用Update_User_Money
	int Save_User_Timelen(uint32 userid, int logintm, int logouttm);


	int Save_Merchant_Login(uint32 userid, int logintm);
	int Save_Merchant_Logout(uint32 userid, int logouttm);
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

	int Import_CS_Req(char *username,int permission,int receptionlimit,char *importname,uint32 &userid,uint32 importtime);
	int Modify_CS_Req(char* username, char* csRealName,int receptionLimit,char* password,uint32 &importtime,uint32 csid,int permission);
	int Modify_CS_Status(char* username, int status);
	int Get_Vaild_CS_List(XT_CS_INFO * CSList, int &nRetCount);

	
	int Qurery_CS_Info(XT_CS_INFO * CSInfoList,char *username, char *importname, char *realname, int status,uint32 begintime,uint32 endtime,int &nRetCount);
	
	int CS_Get_Groups(uint32 userid,vector<uint32> &groups);
	int CS_Get_Group_Account(uint32 groupId,vector<uint32> &userlist);
	int CS_Get_Group_Friend(uint32, vector<csTeam_t>& teamInfo);
	int CS_Get_RecptionLimit(uint32 iUserID,uint32 &RecpLimit);	
	int WebLastMsgReq(XT_WEB_BUSI_LAST_MSG_REQ* msgReq,XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList * msgInfo,uint32& nRetCount);
	int Get_Userid_Map_CSid(uint32 userid,uint32 &csid);
	
	//int Get_Basic_CSInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount);
	int Get_CS_Info(uint32 iUserID, XT_SERVER_CS_GET_ACK &lpAck);

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
	
private:
	void * m_MiniDB;
};




class DATABASELIB_API CDataBaseWeb 
{
public:
	CDataBaseWeb(void);
	~CDataBaseWeb();
/////////////////////*****************外部接口定义********************///////////////
	//连接WEB数据库
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	void DisConnect();

	int Add_Register_Info(LPXT_REGIST_REQ pRegInfo, uint32 &RetUserID, uint8 &ReturnCode);

	/*================================================================ 
	*
	* 函 数 名：Get_UserID_PWD
	** 根据用户名获取ID与密码
	* 参 数：
	* char *c_pcUserName [IN] : 用户名
	* uint32 &iUserID [[IN,OUT]] : 用户ID
	* char *pcPassWord [IN,OUT]: 用户密码
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER;
	* 
	================================================================*/
	int Get_UserID_PWD(const char *c_pcUserName,uint32 &iUserID,char *pcPassWord);
	int Get_UserID_PWD_ByMobileNo(const char *c_pcmobileno,uint32 &iUserID,char *pcPassWord,char *username);
	int Get_PWD_ByID(uint32 id,char* cPass,char* username,char* mobileno);
	int Get_CS_PWD(char *c_pcUserName, char *pcPassWord, uint32 &iUserID, uint8 &status);
	int SaveAccount(char* account,char* pwd,uint32 id,char* truename);
	int SaveAccountBuyer(char* account,char* pwd,char* guid, uint32& id,char* mobileno,char* emailaddress,char*nickname,char*headImg);
	int SaveAccountWork(char* account,char* pwd, uint32& id, char* mobileno, char* emailaddress, char*nickname, char*headImg, char*groupname);
	int SaveSubAccount(uint32 uid,uint32 fid,uint32 groupid,char* password,char* nickname,char* truename);

	/*================================================================ 
	*
	* 函 数 名：Get_Basic_UserInfo
	** 根据用户ID获取用户基本信息
	* 参 数：
	*const uint32 requestID [IN] : 请求者的ID
	* const uint32 c_iUserID [IN] : 用户ID
	* XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo & st_BasicUserInfo [[IN,OUT]] : 用户基本信息结构
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Get_Basic_UserInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount);
	
	

	/*================================================================ 
	*
	* 函 数 名：Get_User_Info
	** 根据用户ID获取用户详细信息
	* 参 数：
	* uint32 iUserID [IN] : 用户ID
	* XT_USERINFO_GET_ACK *lpAck [[IN,OUT]] : 用户基本信息结构
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
	* 注意 用户的在线时长需从minidb里获取.Get_User_OnlineTime();
	================================================================*/
	int Get_User_Info(uint32 iUserID, XT_SERVER_USERINFO_GET_ACK &lpAck);	
	int Get_Agent_Merchant(uint32 iUserID, uint32 &MerchantId);
	int Modify_User_Info(XT_USERINFO_MOD_REQ *pUserInfo);
	int Get_User_FriendList(uint32 iUserID, uint16 startPos, XT_FRIENDLIST_ACK::tagFriendInfo * friendInfo, int &nRetCount, uint8 flag=XT_HEAD::FLAG_CNTREQUEST);
	int Get_User_JoinFlag(uint32 iUserID, uint8 &joinFlag);

	int Modify_User_UnderWrite(uint32 UserID, char* userUnderWrite);

	int Get_UserFriend_Comment(uint32 UserID, uint32 friendID, char* comment);
	int Modify_Friend_Comment(uint32 UserID, uint32 friendno, char * comment);
	int Modify_Friend_Group(uint32 userID, uint32 friendID, uint8 groupID);
	int Get_User_FriendGroup(uint32 UserID, uint32 friendno, uint8 &groupID);


	int Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID);//好友数已满 DB_FRIENDS_FULL
	int Delete_Friend(uint32 iUserID, uint32 friendID);
	int Search_User_WithUserID(uint32 userid, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount);
	int Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount);
	int	Update_User_Money(uint32 iUserID, int sendmoney);
	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag);

	int Get_Offline_WebMsg(uint32 userid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount);//用户登录时获取网站消息
	int Set_WebMsg_Status(uint32 recID);
	int SendFriendInvite(uint32 userID, char *emailAddress, char *emailTitle, char *emailContent);
	int Change_User_PWD(uint32 userid);

	int  Get_User_FamilyFlag(uint32 *iUserID, XT_USERFAMILYFLAG_GET_ACK::tagUserFamilyFlag *pUserFamilyFlag, uint8 &nRetCount);

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
	int SetUserMessageStatus(uint32 recID);
	int Get_System_Log(uint32 &recid, ST_SYSTEM_LOG *pWebMsg, uint16 &nRetCount);
	int GetNickName(uint32 userid,char *nickname);
	int GetUserPicUri(uint32 userid,char *picuri);
	int Write_System_Log(uint32 projid,char* szUserid,char* szLogid,uint32 flatid,uint32 operid);
	int PayMoney(uint32 userid,uint32 consumetype, uint32 logid,float money);
	int GetSubAccount(uint32 groupid,std::vector<uint32_t>& ids);
	int GetFlowMode(uint32 id,int& mode);
	int SearchConnAccount(uint32 sid,std::vector<uint32_t>& ids, uint32& did);
	int GetNoReplyNum(uint32 id,uint32 time,uint32& NoReplyNum);





private:
	void * m_WebDB;
};



class DATABASELIB_API CDataBaseFamily 
{
public:
	CDataBaseFamily(void);
	~CDataBaseFamily();
	/////////////////////*****************外部接口定义********************///////////////
	//连接WEB数据库
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	void DisConnect();


	/*================================================================ 
	*
	* 函 数 名：Get_Family_List
	** 根据用户ID获取用户加入的固定群列表
	* 参 数：
	* const uint32 c_iUserID [IN] : 用户ID
	* uint32 *FamilyID [[IN,OUT]] : 用户加入的家族数组
	* int &FamilyCount	[IN, OUT] : 要获取的家族数目和返回的数目, 如果为0，则输出为用户加入的家族数目
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 用户不存在: DB_NOUSER; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_List(const uint32 c_iUserID, uint32 *FamilyID, int &FamilyCount);

	/*================================================================ 
	*
	* 函 数 名：Get_Family_Info
	** 根据家族ID获取家族信息
	* 参 数：
	* const uint32 c_iFamilyID [IN] : 家族ID
	* LPXT_GET_CINFO_ACK lpAck [[IN,OUT]] : 家族的详细信息
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_Info(const uint32 c_iFamilyID, LPXT_GET_CINFO_ACK lpAck);

	/*================================================================ 
	*
	* 函 数 名：Get_Family_Mem
	** 根据用户ID获取家族成员列表
	* 参 数：
	* const uint32 c_iFamilyID [IN] : 家族ID
	* LPST_FAMILY_MEMBER pFamilyMem [[IN,OUT]] : 成员结构。ID和属性
	* int &nMemNum	[IN, OUT] : 要获取的成员数目和返回的数目, 如果为0，则输出家族成员数
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_Mem(const uint32 nFamilyID, LPST_FAMILY_MEMBER pFamilyMem, int &nMemNum);

	/*================================================================ 
	*
	* 函 数 名：Set_Family_Info
	** 设置家族信息
	* 参 数：
	* LPXT_SET_CINFO_REQ lpReq [[IN]] : 家族的详细信息
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 传入的参数错误：DB_PARAM；家族不存在: DB_NOFAMILY; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int	Set_Family_Info(LPXT_SET_CINFO_REQ lpReq);

	/*================================================================ 
	*
	* 函 数 名：Create_Family
	** 设置家族信息
	* 参 数：
	* const uint32 iCreaterID [IN] : 创建者ID
	* LPXT_CREATE_CLUSTER_REQ lpReq [IN] : 家族的详细信息
	* uint32 &iFamilyID	[IN,OUT] : 输出创建的家族ID
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 传入的参数错误：DB_PARAM；操作失败: DB_ERROR;
	*					家族名称存在 DB_FAMILYNAME_EXIST; 域名存在 DB_FAMILYDOMAIN_EXIST;
	*					创建数达到限制 DB_CREATE_FAMILY_FULL;
	* 
	================================================================*/
	int Create_Family(const uint32 iCreaterID, LPXT_CREATE_CLUSTER_REQ lpReq, uint32 &iFamilyID);

	/*================================================================ 
	*
	* 函 数 名：Family_Add_Member
	** 添加家族普通成员
	* 参 数：
	* const uint32 iFamilyID [IN]	家族ID
	* uint32 *iUserID	[IN]		要添加的成员ID列表
	* int &nRetCount					请求个数，返回个数
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int	Family_Add_Member(const uint32 iFamilyID, uint32 *iUserID, int &nRetCount);

	/*================================================================ 
	*
	* 函 数 名：Family_Delete_Member
	** 删除家族成员
	* 参 数：
	* const uint32 iFamilyID [IN]	家族ID
	* uint32 iUserID	[IN]		要删除的成员ID
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Family_Delete_Member(const uint32 iFamilyID, uint32 iUserID);

	/*================================================================ 
	*
	* 函 数 名：Family_Delete_Member
	** 删除家族成员
	* 参 数：
	* const uint32 iFamilyID [IN]	家族ID
	* uint32 iUserID	[IN]		要修改的成员ID
	* uint8	iMemProp	[IN]		成员的属性	FAMILY_CREATER|FAMILY_MANAGER|FAMILY_NORMAL
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Family_SetMem_Prop(const uint32 iFamilyID, uint32 iUserID, uint8 iMemProp);

	/*================================================================ 
	*
	* 函 数 名：Family_Find
	** 查找家族
	* 参 数：
	* XT_CLUSTER_SEARCH_REQ Req			[IN]			查找家族请求
	* CLUSTER_SEARCH_RESULT *FamilyData	[IN/OUT]		查找结果
	* int &nRetCount					[IN/OUT]		输入请求个数，返回结果个数
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Family_Find(XT_CLUSTER_SEARCH_REQ Req,XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet *FamilyData, int &nRetCount);

	/*================================================================ 
	*
	* 函 数 名：Delete_Family
	** 删除家族
	* 参 数：
	const uint32 iFamilyID	[IN] : 删除家族
	* 返 回 值：int -	SUCCESS: DB_SUCCESS; 操作失败: DB_ERROR;
	* 
	================================================================*/
	int Delete_Family(const uint32 iFamilyID);



	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int Get_Family_BlackList(uint32 familyID, uint32 userID, int &retCode);

	//群名片
	int	Get_Mem_Profile(uint32 familyID, uint32 userID, char* profileName);
	int Get_All_Profile(uint32 iFamilyID, CMEM_INFO* profileName,uint32& nRetCnt);
	int Set_Mem_Profile(uint32 iFamilyID, uint32 iUserID, const char* profileName);

private:
	void * m_FamilyDB;
};
