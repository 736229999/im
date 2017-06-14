
#pragma once

#ifdef DATABASELIB_EXPORTS
#define DATABASELIB_API 
#else
#define DATABASELIB_API 
#endif

#include "DataBaseStruct.h"
#include "ClientProtocol.h"

#include "improtocol.pb.h"


/****************************���ݿ�����ӿ�******************************
���нӿڳɹ�����DB_SUCCESS
�����������ͨ��CError::DB_Error(errCode, szError[255], true)��ȡ��������
***********************************************************************/
// �����Ǵ� DataBaseLib.dll ������
class DATABASELIB_API CDataBaseMini
{
public:
	CDataBaseMini(void);
	~CDataBaseMini(void);
/////////////////////*****************�ⲿ�ӿڶ���********************///////////////
	//����mysql���ݿ�
	bool ConnectMysql();

	//����oracle���ݿ�
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
	* �� �� ����Get_User_GroupInfo
	** �����û���������Ϣ
	* �� ����
	* uint32 c_iUserID [[IN]] : �û�ID
	* XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo [IN,OUT]: �û�������Ϣ
	* int &nGroupNum [IN,OUT] : ��Ҫ��ȡ�ķ������ͷ��صķ��������������Ϊ0����������ڵ���Ŀ��
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;
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
	* �� �� ����Get_Family_MaxMsgID
	** ���ݼ���ID��ȡ��ǰ���������ϢID
	* �� ����
	* uint32 c_iFamilyID [[IN]] : ����ID
	* uint32 & nMsgID [IN,OUT]: ���嵱ǰ��ϢID
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;  ������崴�������κ���Ϣ��nMsgID = 0;
	* 
	================================================================*/
	int Get_Family_MaxMsgID(const uint32 c_iFamilyID, uint32 & nMaxMsgID, uint32 &nMinMsgID);

	/*================================================================ 
	*
	* �� �� ����Save_Family_RequestMsg
	** �������������ϢID��ȡ��ǰ���������ϢID
	* �� ����
	* uint32 c_iFamilyID	[IN] : ����ID
	* uint32 iSenderID		[IN] : ������ID
	* uint32 iSendTime		[IN] : ����ʱ��
	* char   *strMsg		[IN] : ��������
	* int	 iMsgLen		[IN] : ���ݳ���
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;
	* 
	================================================================*/ 
	int Save_Family_RequestMsg(const uint32 c_iFamilyID, uint32 iSenderID,uint32 iSendTime,const char *strMsg, int iMsgLen);
	
	/*================================================================ 
	*
	* �� �� ����Save_User_Msg
	** �����û�����Ϣ������ϵͳ��Ϣ
	* �� ����
	* uint32 iRecvID	[IN] : ����ID
	* uint32 iSendID		[IN] : ������ID
	* uint32 iNatIP,uint16 iNatPort,uint32 iTime [IN] 
	* uint16 iMsgType		[IN] : ��Ϣ����
	* char   *strMsg		[IN] : ��Ϣ����
	* uint16	 iMsgLen		[IN] : ���ݳ���
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;
	* 
	================================================================*/ 
	int Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen, const char *strText);

	/*================================================================ 
	*
	* �� �� ����Delete_Family_Msg
	** ɾ���������Ϣ
	* �� ����
	const uint32 iFamilyID	[IN] : ɾ������
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Delete_Family_Msg(const uint32 iFamilyID);

	int Save_Family_Msg(uint32 familyMsgID,uint32 familyID,uint32 senderID, int tm, int msgType,int msgLen,const char *msg);
	
	int Get_User_OnlineTime(uint32 iUserID,uint32 &OnlineTime);	//��ȡ�û�����ʱ��


	/*================================================================ 
	*
	* �� �� ����Add_Group
	** ��ӷ���
	* �� ����
	* uint32 iUserID [[IN]] : �û�ID
	* uint8 &iGroupID [IN,OUT]: ��ID������ָ����ID��������ݿ��д洢��ID
	* char * strGroupName	[IN] : ����
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;  �û��ܴ�������������DB_GROUP_FULL; �����Ѿ�����:DB_GROUP_EXIST;
	* 
	================================================================*/ 
	int Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName, uint8 &iGroupPid);
	int Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName);
	int Delete_Group(uint32 iUserID, uint8 iGroupID);
	int Save_User_Login(uint32 userid,uint32 userip,uint16 userport,int serverno);
	int Save_User_Logout(uint32 userid);
	int Update_All_Logout(int serverno);
	int Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count);
	int Save_User_OnlineTime(uint32 clientno,int logintm,int logouttm, int &money);//����DB_SUCCESS,money���ظ����ӵĽ���� ����Update_User_Money
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

	// ����һ����Ϣ��������
	//int InsertMsgNotify(const char *pUuid, const std::vector<uint32> &vecUserID, const std::string &buffer, int nBufferType);

	// ��ѯ������Ϣ����
	//int SearchMsgNotify(const char *pUuid, std::string &buffer, int &nBufferType);

	// ���Ҷ������ѷ�ʽ
	int GetMsgRemind(uint32 userid, MsgRemindAck &ack);
	// д�붩�����ѷ�ʽ
	int SetMsgRemind(uint32 userid, const MsgRemindAck &ack);
	// ��ȡ�����ϵ��
	int GetRecentContact(const char *pGuid, uint32 starttime, std::vector<uint32> &arrID);
	// ɾ��һ����¼
	//int DeleteMsgNotify(const char *pUuid, uint32 userid);
	// ��ȡ�̼���Ҫ���͵���Ϣ��¼
	int GetMsgNotifyDataOnePerson(uint32 userid, std::vector<std::string> &arrUUID);
	
private:
	void * m_MiniDB;
};




class DATABASELIB_API CDataBaseWeb 
{
public:
	CDataBaseWeb(void);
	~CDataBaseWeb();
/////////////////////*****************�ⲿ�ӿڶ���********************///////////////
	//����WEB���ݿ�
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	void DisConnect();

	int Add_Register_Info(LPXT_REGIST_REQ pRegInfo, uint32 &RetUserID, uint8 &ReturnCode);

	/*================================================================ 
	*
	* �� �� ����Get_UserID_PWD
	** �����û�����ȡID������
	* �� ����
	* char *c_pcUserName [IN] : �û���
	* uint32 &iUserID [[IN,OUT]] : �û�ID
	* char *pcPassWord [IN,OUT]: �û�����
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; �û�������: DB_NOUSER;
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
	* �� �� ����Get_Basic_UserInfo
	** �����û�ID��ȡ�û�������Ϣ
	* �� ����
	*const uint32 requestID [IN] : �����ߵ�ID
	* const uint32 c_iUserID [IN] : �û�ID
	* XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo & st_BasicUserInfo [[IN,OUT]] : �û�������Ϣ�ṹ
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; �û�������: DB_NOUSER; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Get_Basic_UserInfo(const uint32 requestID, uint32 *iUserID, XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo *pBasicUserInfo, uint16 &nRetCount);
	
	

	/*================================================================ 
	*
	* �� �� ����Get_User_Info
	** �����û�ID��ȡ�û���ϸ��Ϣ
	* �� ����
	* uint32 iUserID [IN] : �û�ID
	* XT_USERINFO_GET_ACK *lpAck [[IN,OUT]] : �û�������Ϣ�ṹ
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; �û�������: DB_NOUSER; ����ʧ��: DB_ERROR;
	* ע�� �û�������ʱ�����minidb���ȡ.Get_User_OnlineTime();
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


	int Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID);//���������� DB_FRIENDS_FULL
	int Delete_Friend(uint32 iUserID, uint32 friendID);
	int Search_User_WithUserID(uint32 userid, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount);
	int Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount);
	int	Update_User_Money(uint32 iUserID, int sendmoney);
	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag);

	int Get_Offline_WebMsg(uint32 userid, ST_WEB_MSG *pWebMsg, uint16 &nRetCount);//�û���¼ʱ��ȡ��վ��Ϣ
	int Set_WebMsg_Status(uint32 recID);
	int SendFriendInvite(uint32 userID, char *emailAddress, char *emailTitle, char *emailContent);
	int Change_User_PWD(uint32 userid);

	int  Get_User_FamilyFlag(uint32 *iUserID, XT_USERFAMILYFLAG_GET_ACK::tagUserFamilyFlag *pUserFamilyFlag, uint8 &nRetCount);

	int Get_Web_Msg(uint32 &nPos, ST_WEB_MSG *pWebMsg, uint16 &nRetCount);//ÿ��һ��ʱ���һ�α�
	int Get_Operation_Msg(uint32 &recid, ST_WOKU_MSG *pOperationMsg, uint16 &nRetCount);//�û�����
	int Get_User_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//�ռ���
	int Get_Sys_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//ϵͳ�㲥	
	int Get_SysOff_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount, char* uid);//����ϵͳ��Ϣ
	int Get_Broadcast_Msg(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//�û��㲥
	int Get_UserComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//������ҳ����
	int Get_LogComment(uint32 &recid, ST_WOKU_MSG *pUserMsg, uint16 &nRetCount);//������������
	int Get_Mobile_Msg(uint32 &recid, ST_MOBILE_MSG *pUserMsg, uint16 &nRetCount);//�ֻ�����
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
	/////////////////////*****************�ⲿ�ӿڶ���********************///////////////
	//����WEB���ݿ�
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();
	void DisConnect();


	/*================================================================ 
	*
	* �� �� ����Get_Family_List
	** �����û�ID��ȡ�û�����Ĺ̶�Ⱥ�б�
	* �� ����
	* const uint32 c_iUserID [IN] : �û�ID
	* uint32 *FamilyID [[IN,OUT]] : �û�����ļ�������
	* int &FamilyCount	[IN, OUT] : Ҫ��ȡ�ļ�����Ŀ�ͷ��ص���Ŀ, ���Ϊ0�������Ϊ�û�����ļ�����Ŀ
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; �û�������: DB_NOUSER; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_List(const uint32 c_iUserID, uint32 *FamilyID, int &FamilyCount);

	/*================================================================ 
	*
	* �� �� ����Get_Family_Info
	** ���ݼ���ID��ȡ������Ϣ
	* �� ����
	* const uint32 c_iFamilyID [IN] : ����ID
	* LPXT_GET_CINFO_ACK lpAck [[IN,OUT]] : �������ϸ��Ϣ
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���岻����: DB_NOFAMILY; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_Info(const uint32 c_iFamilyID, LPXT_GET_CINFO_ACK lpAck);

	/*================================================================ 
	*
	* �� �� ����Get_Family_Mem
	** �����û�ID��ȡ�����Ա�б�
	* �� ����
	* const uint32 c_iFamilyID [IN] : ����ID
	* LPST_FAMILY_MEMBER pFamilyMem [[IN,OUT]] : ��Ա�ṹ��ID������
	* int &nMemNum	[IN, OUT] : Ҫ��ȡ�ĳ�Ա��Ŀ�ͷ��ص���Ŀ, ���Ϊ0������������Ա��
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���岻����: DB_NOFAMILY; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Get_Family_Mem(const uint32 nFamilyID, LPST_FAMILY_MEMBER pFamilyMem, int &nMemNum);

	/*================================================================ 
	*
	* �� �� ����Set_Family_Info
	** ���ü�����Ϣ
	* �� ����
	* LPXT_SET_CINFO_REQ lpReq [[IN]] : �������ϸ��Ϣ
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����Ĳ�������DB_PARAM�����岻����: DB_NOFAMILY; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int	Set_Family_Info(LPXT_SET_CINFO_REQ lpReq);

	/*================================================================ 
	*
	* �� �� ����Create_Family
	** ���ü�����Ϣ
	* �� ����
	* const uint32 iCreaterID [IN] : ������ID
	* LPXT_CREATE_CLUSTER_REQ lpReq [IN] : �������ϸ��Ϣ
	* uint32 &iFamilyID	[IN,OUT] : ��������ļ���ID
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����Ĳ�������DB_PARAM������ʧ��: DB_ERROR;
	*					�������ƴ��� DB_FAMILYNAME_EXIST; �������� DB_FAMILYDOMAIN_EXIST;
	*					�������ﵽ���� DB_CREATE_FAMILY_FULL;
	* 
	================================================================*/
	int Create_Family(const uint32 iCreaterID, LPXT_CREATE_CLUSTER_REQ lpReq, uint32 &iFamilyID);

	/*================================================================ 
	*
	* �� �� ����Family_Add_Member
	** ��Ӽ�����ͨ��Ա
	* �� ����
	* const uint32 iFamilyID [IN]	����ID
	* uint32 *iUserID	[IN]		Ҫ��ӵĳ�ԱID�б�
	* int &nRetCount					������������ظ���
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int	Family_Add_Member(const uint32 iFamilyID, uint32 *iUserID, int &nRetCount);

	/*================================================================ 
	*
	* �� �� ����Family_Delete_Member
	** ɾ�������Ա
	* �� ����
	* const uint32 iFamilyID [IN]	����ID
	* uint32 iUserID	[IN]		Ҫɾ���ĳ�ԱID
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Family_Delete_Member(const uint32 iFamilyID, uint32 iUserID);

	/*================================================================ 
	*
	* �� �� ����Family_Delete_Member
	** ɾ�������Ա
	* �� ����
	* const uint32 iFamilyID [IN]	����ID
	* uint32 iUserID	[IN]		Ҫ�޸ĵĳ�ԱID
	* uint8	iMemProp	[IN]		��Ա������	FAMILY_CREATER|FAMILY_MANAGER|FAMILY_NORMAL
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Family_SetMem_Prop(const uint32 iFamilyID, uint32 iUserID, uint8 iMemProp);

	/*================================================================ 
	*
	* �� �� ����Family_Find
	** ���Ҽ���
	* �� ����
	* XT_CLUSTER_SEARCH_REQ Req			[IN]			���Ҽ�������
	* CLUSTER_SEARCH_RESULT *FamilyData	[IN/OUT]		���ҽ��
	* int &nRetCount					[IN/OUT]		����������������ؽ������
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Family_Find(XT_CLUSTER_SEARCH_REQ Req,XT_CLUSTER_SEARCH_ACK::tagFamilyFindRet *FamilyData, int &nRetCount);

	/*================================================================ 
	*
	* �� �� ����Delete_Family
	** ɾ������
	* �� ����
	const uint32 iFamilyID	[IN] : ɾ������
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ����ʧ��: DB_ERROR;
	* 
	================================================================*/
	int Delete_Family(const uint32 iFamilyID);



	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int Get_Family_BlackList(uint32 familyID, uint32 userID, int &retCode);

	//Ⱥ��Ƭ
	int	Get_Mem_Profile(uint32 familyID, uint32 userID, char* profileName);
	int Get_All_Profile(uint32 iFamilyID, CMEM_INFO* profileName,uint32& nRetCnt);
	int Set_Mem_Profile(uint32 iFamilyID, uint32 iUserID, const char* profileName);

private:
	void * m_FamilyDB;
};
