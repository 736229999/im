
#pragma once

#ifdef DATABASELIB_EXPORTS
#define DATABASELIB_API __declspec(dllexport)
#else
#define DATABASELIB_API __declspec(dllimport)
#endif

#include "DataBaseStruct.h"
#include "ClientProtocol.h"

/****************************���ݿ�����ӿ�******************************
���нӿڳɹ�����DB_SUCCESS
�����������ͨ��CError::DB_Error(errCode, szError[255], true)��ȡ��������
***********************************************************************/
// �����Ǵ� DataBaseLib.dll ������
class DATABASELIB_API CDataBaseMini
{
public:
	CDataBaseMini(void);
/////////////////////*****************�ⲿ�ӿڶ���********************///////////////
	//�������ݿ�
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();

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
	int Get_User_GroupInfo(const uint32 c_iUserID, XT_GROUPINFO_GET_ACK::tagGroupInfo* pGroupInfo, uint8 &nGroupNum);

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
	int Get_Family_MaxMsgID(const uint32 c_iFamilyID, uint32 & nMsgID);

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
	int Save_User_Msg(uint32 iRecvID,uint32 iSendID,uint32 iNatIP,uint16 iNatPort,uint32 iTime,uint16 iMsgType,const char *strMsg, uint16 iMsgLen);

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
	* �� �� ֵ��int -	SUCCESS: DB_SUCCESS; ���ݿ����:DB_ERROR;  �û��ܴ�������������DB_GROUT_FULL; �����Ѿ�����:DB_GROUP_EXIST;
	* 
	================================================================*/ 
	int Add_Group(uint32 iUserID, uint8 &iGroupID, char * strGroupName);
	int Modify_Group(uint32 iUserID, uint8 iGroupID, char *strGroupName);
	int Delete_Group(uint32 iUserID, uint8 iGroupID);
	int Save_User_Login(uint32 userid,uint32 userip);
	int Save_User_Action(uint32 userid,XT_USER_ACTION_AUDIT_REQ::Action *action,uint16 count);
	int Save_User_OnlineTime(uint32 clientno,long logintm,long logouttm, int &money);//����DB_SUCCESS,money���ظ����ӵĽ���� ����Update_User_Money
	int Save_User_RecvFamilyMsgID(uint32 userID, uint32 familyID, uint32 msgID);
	int Get_User_FamilyMsgID(uint32 userID, uint32 familyID, uint32 &msgID);

	int Delete_All_Offline_Msg(uint32 userID);
	int Get_Family_Offline_Msg(uint32 userID, uint32 familyID, uint32 &msgID, ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount);
	int Get_Family_Offline_SysMsg(uint32 userID, uint32 familyID,ST_OFFLINE_MSG *pFamilyMsg, uint8 &nRetCount);
	int Get_User_Offline_Msg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount);
	int Get_User_Offline_SysMsg(uint32 userID,ST_OFFLINE_MSG *pMsg, uint8 &nRetCount);
	int Get_Pub_Msg(uint32 userID,ST_WEB_MSG *pMsg, uint8 &nRetCount);
	int Save_OnlineUser_Num(uint32 onlinenum);


private:
	void * m_MiniDB;
	
};




class DATABASELIB_API CDataBaseWeb 
{
public:
	CDataBaseWeb(void);
/////////////////////*****************�ⲿ�ӿڶ���********************///////////////
	//����WEB���ݿ�
	void SetConnectDB(char* uid, char* pwd, char* sid);
	bool ConnectDB();

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
	int Modify_User_Info(XT_USERINFO_MOD_REQ *pUserInfo);
	int Get_User_FriendList(uint32 iUserID, uint16 startPos, XT_FRIENDLIST_ACK::tagFriendInfo * friendInfo, int &nRetCount);
	int Get_User_JoinFlag(uint32 iUserID, uint8 &joinFlag);
	int Modify_User_UnderWrite(uint32 UserID, char* userUnderWrite);

	int Get_UserFriend_Comment(uint32 UserID, uint32 friendID, char* comment);
	int Modify_Friend_Comment(uint32 UserID, uint32 friendno, char * comment);
	int Modify_Friend_Group(uint32 userID, uint32 friendID, uint8 groupID);

	int Add_Friend(uint32 iUserID, uint32 friendID, uint8 groupID);//���������� DB_FRIENDS_FULL
	int Delete_Friend(uint32 iUserID, uint32 friendID);
	int Search_User_WithUserName(char* UserName, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo);
	int Search_User_WithNickName(char* NickName, uint16 startPos, XT_XTREAM_SEARCH_ACK::tagFriendInfo *friendInfo, uint8 &nRetCount);
	int Search_User_WithEmail(char* Email,uint16 startPos,XT_XTREAM_SEARCH_ACK::tagFriendInfo * friendInfo, uint8 &nRetCount);
	int	Update_User_Money(uint32 iUserID, int sendmoney);
	int	Get_FamilyMem_Porp(uint32 familyID, uint32 userID,  uint8 &userProp);
	int Modify_Friend_Flag(uint32 UserID, uint32 friendno, uint8 flag);


	int Get_Web_Msg(uint32 &nPos, ST_WEB_MSG *pWebMsg, uint8 &nRetCount);//ÿ��һ��ʱ���һ�α�
	int Set_WebMsg_Status(uint32 recID);

private:
	void * m_WebDB;
};
