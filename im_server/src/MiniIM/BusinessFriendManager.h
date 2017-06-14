#pragma once
#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"
#include "BusinessWebIM.h"
#include "UserManager.h"

#include <mutex>


class CBusinessCSQueue: public CBusinessExIMSrv
{
	public:
		CBusinessCSQueue(CRouterBusiness* pRouter);
		~CBusinessCSQueue();
//		void updateCsQueue(ClientService *clientService);
		void GetSubAccountInfo(uint32 id,SubAccountInf_t &Subinfo);
		void SendWebQueueInfoIND(CSendPacket& m_SendPacket);
		void ClearWebContext(uint32 toid,uint32 &teamid);
		void WebLogOut(uint32 fromid,uint32 toid,uint16 type = 0);		
		void SetWebContext(uint32 toid,uint32 csid,uint32 teamid);
		virtual int DoWork();
	private:
		SendDataStruct*	pData;
};


class CBusinessCSQueueInfo: public CBusinessExIMSrv
{
	public:		
		CBusinessCSQueueInfo(CRouterBusiness* pRouter);
		int NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange);	  //֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
		virtual int DoWork();
};

class CBusinessCloseReceptionByCustomer: public CBusinessExIMSrv
{
	public:		
		CBusinessCloseReceptionByCustomer(CRouterBusiness* pRouter);
		int NotifyBuyerQuitInfo(XT_BUYERINFO_CHANGE* QueueChange);	  //֪ͨ����������Ա�Ŷ�״�� 0 �����Լ� 1 �������Լ�
		virtual int DoWork();
};


class CBusinessForceOfflineCs: public CBusinessExIMSrv
{
	public:		
		CBusinessForceOfflineCs(CRouterBusiness* pRouter);
		int ForceOfflineCs(XT_FORCE_OFFLINE_CS* xtOfflineCs);	  //ǿ�ƿͷ�����
		virtual int DoWork();
};



//�޸Ĺ���
class CBusinessModHangUp : public CBusinessExIMSrv
{
public:
	CBusinessModHangUp(CRouterBusiness* pRouter);
	virtual int DoWork();
};


//��ȡ����״̬
class CBusinessGetOnlineStatus : public CBusinessExIMSrv
{
public:
	CBusinessGetOnlineStatus(CRouterBusiness* pRouter);
	virtual int DoWork();
};

//�Ƿ������˻�����
class CBusinessCheckSubOnline : public CBusinessExIMSrv
{
public:
	CBusinessCheckSubOnline(CRouterBusiness* pRouter);
	virtual int DoWork();
};

//��ȡ������˻�ID
class CBusinessGetSubAccount : public CBusinessExIMSrv
{
public:
	
	CBusinessGetSubAccount(CRouterBusiness* pRouter);
	virtual int DoWork();	

	int AddToQueueAndNotify(CUser* User,uint32 buyerId,XT_GET_SUB_ACCOUNT_REQ *req);
	int NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange);	//֪ͨ����������Ա�Ŷ�״��

private:
	using LastAccountMap = std::map<uint32_t, uint32_t>;
	static LastAccountMap account_load_map_;
	static std::mutex account_load_mutex_;
};


//��ȡ���ѻ�����Ϣ
class CBusinessGetFriendBasicInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendBasicInfo(CRouterBusiness* pRouter);
	~CBusinessGetFriendBasicInfo();

	virtual int DoWork();
private:
	void InitNotifyHead();
	bool AddToNotifyPacket(CSendPacket* pTempPacket);
	void SendNofity();


	SendDataStruct* m_pNotifyData;
	CSendPacket		m_NotifyPacket;
	uint16*			m_pCount;

	XT_GET_FRIEND_BASICINFO_NOTIFY::tagFriendInfo*  m_pFriendInfo;

};

//��ȡ������ϸ��Ϣ
class CBusinessGetFriendFullInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendFullInfo(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	void ReturnUserInfo(CUser* pUser, XT_SERVER_USERINFO_GET_ACK* pUserInfoAck);

};

//�޸��û�����
class CBusinessModifyUserInfo : public CBusinessExIMSrv
{
public:
	CBusinessModifyUserInfo(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	void MD5Password(char* pPassword);

};


//�õ����ѷ���
class CBusinessGetGroupInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetGroupInfo(CRouterBusiness* pRouter);

	virtual int DoWork();
	//CGetBackendGroup CSGroup;
private:
	//����Ϣ����
	XT_GROUPINFO_GET_ACK::tagGroupInfo	m_GroupInfo[MAX_GROUP_NUM];
	bool GetFriendGroupList(int id,uint8 &count, vector<GROUPEX> &friendgrouplist);

};





//�õ������б�
class CBusinessGetFriendList : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendList(CRouterBusiness* pRouter);	
	virtual int DoWork();


private:	
	bool GetFriendList(int id, int merchantId,uint32 &count, XT_FRIENDLIST_ACK::tagFriendInfo *friendlist);
};

//�޸ĺ��ѷ�����Ϣ
class CBusinessGroupInfoModify : public CBusinessExIMSrv
{
public:
	CBusinessGroupInfoModify(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	// ��Ӻ��ѷ���
	bool AddGroup(int uId, int groupPid, int groupId, const char *groupName, std::string &error);
	// �޸ĺ��ѷ���
	bool ModGroup(int uId, int groupId, const char *groupName, std::string &error);
	// ɾ�����ѷ���
	bool DelGroup(int uId, int groupId, string &error);
};

//���Һ���
class CBusinessSearchFriend : public CBusinessExIMSrv
{
public:
	CBusinessSearchFriend(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

	uint8			m_nReturnCount;

	XT_XTREAM_SEARCH_REQ*		m_pSearchFriendReq;
	XT_XTREAM_SEARCH_ACK*		m_pSearchFriendAck;

	int FindOnlineUser();		//���������û�
	int	FindPrecision() ;		//��ȷ����


};

//����û����޸��û��������ɾ���û�
class CBusinessFriendModify : public CBusinessExIMSrv
{
public:
	CBusinessFriendModify(CRouterBusiness* pRouter);

	virtual int DoWork();

	//��Ӻ��ѵ���
	uint8  AddFriendToGroup();
	//��Ӻ��ѵ����ִ�в���
	uint8  DoAddFriend();

	//�޸ĺ��ѷ���
	uint8  ModifyFriendGroup();

	//ɾ������
	uint8  DoDelFriendGroup(int userid, int friendid);


	inline void  SendFriendNotify(uint32 BeAddedID,uint32 ToAddID);
	inline void  SendFriendDelMe(uint32 friendID,uint32 myID);

	// ���ýӿ��޸ĺ��ѷ���
	uint8 DoModFriendGroup(int uid, int friendid, int groupid);

private:

	XT_FRIENDGROUP_MOD_REQ* m_pFriendGroupReq ;
	XT_FRIENDGROUP_MOD_ACK* m_pFriendGroupAck ;
	shared_ptr<CUser>					m_pUser;

};


//��������û�����֤��Ϣ����ͬ�⡢�ܾ�����Ϊ������Ϣ��������
class CBusinessVerifyFriend : public CBusinessExIMSrv
{
public:
	CBusinessVerifyFriend(CRouterBusiness* pRouter);

	virtual int DoWork();

	void SendVerifyFriendNotify(uint32 to_id,uint8 groupid);

private:

};

//�޸��û�״̬
class CBusinessModifyStatus : public CBusinessExIMSrv
{
public:
	CBusinessModifyStatus(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

};

//�����ѷ���������Ϣ
class CBusinessSendMessage : public CBusinessExIMSrv
{
public:
	CBusinessSendMessage(CRouterBusiness* pRouter);
	virtual int DoWork();
	void RouterMsgNotify(uint32 to_id,uint32 deviceId = 0);
public:
	void SaveWebMsg();
	
//	void updateCsQueue(ClientService *clientService);

#ifdef PRESS_TEST

	void LoadMerchantId();
	
private:
	CConfigInfo m_ConfigInfo;
	vector<uint32> m_MerchInfo;

#endif
	
};

class CBusinessWebMoreMsg : public CBusinessExIMSrv
{
public:
	CBusinessWebMoreMsg(CRouterBusiness* pRouter);
	~CBusinessWebMoreMsg();
	virtual int DoWork();
private:
	SendDataStruct*	pData;
	void SendWebMoreMsgAck(CSendPacket& m_SendPacket);
	XT_WEB_MORE_MSG_ACK::MsgInfo* pMsgInfo;
	uint32 seq;
};

class CBusinessWebBusiList : public CBusinessExIMSrv
{
public:
	CBusinessWebBusiList(CRouterBusiness* pRouter);
	~CBusinessWebBusiList();
	virtual int DoWork();
private:
	SendDataStruct*	pData;
	void SendWebBusiListAck(CSendPacket& m_SendPacket);
	XT_WEB_BUSI_LIST_ACK::BusiList* pListInfo;
	uint32 seq;
};

//��ȡ���ѵ�����������Ϣ
class CBusinessGetLinkStatus : public CBusinessExIMSrv
{
public:
	CBusinessGetLinkStatus(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

};


//�޸ĺ��ѱ�ע
class CBusinessModFriendComment : public CBusinessExIMSrv
{
public:
	CBusinessModFriendComment(CRouterBusiness* pRouter);

	virtual int DoWork();


private:

};


//�û���Ϊͳ�Ʒ���
class CBusinessUserActionAudit : public CBusinessExIMSrv
{
public:
	CBusinessUserActionAudit(CRouterBusiness* pRouter);

	virtual int DoWork();


};

//�޸��û�����ǩ��
class CBusinessModifyUnderWrite : public CBusinessExIMSrv
{
public:
	CBusinessModifyUnderWrite(CRouterBusiness* pRouter);

	virtual int DoWork();

};

//�޸ĺ��ѹ�ע���
class CBusinessModifyFriendFlag : public CBusinessExIMSrv
{
public:
	CBusinessModifyFriendFlag(CRouterBusiness* pRouter);

	virtual int DoWork();

};

class CBusinessGetUserFamilyFlag : public CBusinessExIMSrv
{
public:
	CBusinessGetUserFamilyFlag(CRouterBusiness* pRouter);

	virtual int DoWork();
};

class CBusinessFriendInvite : public CBusinessExIMSrv
{
public:
	CBusinessFriendInvite(CRouterBusiness* pRouter);
	~CBusinessFriendInvite();
	XT_FRIENDINVITE_REQ *m_Req; 
	virtual int DoWork();
};


class CBusinessPayMoney : public CBusinessExIMSrv
{
public:
	CBusinessPayMoney(CRouterBusiness* pRouter);
	~CBusinessPayMoney();
	virtual int DoWork();
};

class CBusinessCustListMod : public CBusinessExIMSrv
{	
	public:
		CBusinessCustListMod(CRouterBusiness* pRouter);
		virtual int DoWork();
};

class CBusinessCustListReq : public CBusinessExIMSrv
{	
	public:
		CBusinessCustListReq(CRouterBusiness* pRouter);
		~CBusinessCustListReq();
		virtual int DoWork();
	private:
		SendDataStruct*	pData;
		XT_GET_NEAR_COMM_CUST_ACK::TagCommInfo* pCustInfo;
		void SendCustListAck(CSendPacket& m_SendPacket);
};

class CBusinessSaveHeadFile : public CBusinessExIMSrv
{	
	private:
		vector<RECV_FILE_STRUCT*>m_vecRecvFiles;
/*�յ���ͼƬ�ṹ��*/
	public:
		CBusinessSaveHeadFile(CRouterBusiness* pRouter);
		virtual int DoWork();
};

class CBusinessModifyCustRemark : public CBusinessExIMSrv
{	
	public:
		CBusinessModifyCustRemark(CRouterBusiness* pRouter);
		virtual int DoWork();
};


class CBusinessGetCustRemark : public CBusinessExIMSrv
{	
	public:
		CBusinessGetCustRemark(CRouterBusiness* pRouter);
		virtual int DoWork();
};


class CBusinessGetOtherUser: public CBusinessExIMSrv
{
	public:
		CBusinessGetOtherUser(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};

class CBusinessGetOtherUserStatus: public CBusinessExIMSrv
{
	public:
		CBusinessGetOtherUserStatus(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};




class CBusinessGetOtherCtrlUser: public CBusinessExIMSrv
{
	public:
		CBusinessGetOtherCtrlUser(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};

class CBusinessGetOtherTUser: public CBusinessExIMSrv
{
	public:
		CBusinessGetOtherTUser(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};



#if 0
class CBusinessModiOtherUser: public CBusinessExIMSrv
{
	public:
		CBusinessModiOtherUser(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};


class CBusinessDelOtherUser: public CBusinessExIMSrv
{
	public:
		CBusinessDelOtherUser(CRouterBusiness* pRouter);
		virtual int DoWork();
		virtual int SendReply();		
};

#endif

class CBusinessModifyCSPWD : public CBusinessExIMSrv
{
	public:
		CBusinessModifyCSPWD(CRouterBusiness* pRouter);
		virtual int DoWork();	

};


/*********************************************���ÿͷ��Ӵ������***************************************************/
class CCSetChatWithBuyer : public CBusinessExIMSrv
{
public:
	CCSetChatWithBuyer(CRouterBusiness* pRouter);
	virtual int DoWork();

private:
	// ϵͳ��ת��Ϣ��֪ͨ��������صĿͷ��ı���������������
	void  SendChatBuyerChanged(uint32 nDirectorID, uint32 nCsId, uint32 nBuyerID);
};

/***************************************************���ܼ�ع���****************************************************/
class CDirectorMonitorStart : public CBusinessExIMSrv
{
public:
	CDirectorMonitorStart(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************������ع���****************************************************/
class CDirectorMonitorStop : public CBusinessExIMSrv
{
public:
	CDirectorMonitorStop(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************���ܲ���Ự****************************************************/
class CMonitorInsertChat : public CBusinessExIMSrv
{
public:
	CMonitorInsertChat(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************���ܽ��ֻỰ****************************************************/
class CMonitorTranChat : public CBusinessExIMSrv
{
public:
	CMonitorTranChat(CRouterBusiness* pRouter);
	virtual int DoWork();
};


class CBusinessWebLastMsg : public CBusinessExIMSrv
{
public:
	CBusinessWebLastMsg(CRouterBusiness* pRouter);
	~CBusinessWebLastMsg();
	virtual int DoWork();
private:
	SendDataStruct*	pData;
	void SendWebMoreLastMsgAck(CSendPacket& m_SendPacket);
	XT_WEB_BUSI_LAST_MSG_ACK::BusiLastList* pMsgInfo;
	uint32 seq;
};



class CBusinessUseridMapCSid : public CBusinessExIMSrv
{
public:
	CBusinessUseridMapCSid(CRouterBusiness* pRouter);
	virtual int DoWork();
};


class CBusinessClearWebClient: public CBusinessExIMSrv
{
public:
	CBusinessClearWebClient(CRouterBusiness* pRouter);
	~CBusinessClearWebClient();
	virtual int DoWork();
};


class CBusinessSetCSClient: public CBusinessExIMSrv		//����ͷ����ѽӴ��������û�����
{
public:
	CBusinessSetCSClient(CRouterBusiness* pRouter);	
//	void updateCsQueue(ClientService *clientService);	
	~CBusinessSetCSClient();
	virtual int DoWork();
};


/***************************************************��ȡ��Ϣ����****************************************************/
class CGetMsgRemind : public CBusinessExIMSrv
{
public:
	CGetMsgRemind(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************������Ϣ����****************************************************/
class CSetMsgRemind : public CBusinessExIMSrv
{
public:
	CSetMsgRemind(CRouterBusiness* pRouter);
	virtual int DoWork();
};

class CBusinessUpdateCsReq : public CBusinessExIMSrv
{
public:
	CBusinessUpdateCsReq(CRouterBusiness* pRouter);
	virtual int DoWork();
};

//��ȡ�ͷ���������Ϣ
class CBusinessCsTeamInfoReq : public CBusinessExIMSrv
{
public:
	CBusinessCsTeamInfoReq(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************��ȡ������Ϣ��������****************************************************/
class CGetOfflineMsgNotify : public CBusinessExIMSrv
{
public:
	CGetOfflineMsgNotify(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************����δ�������buf****************************************************/
class CGetRemainBuf : public CBusinessExIMSrv
{
public:
	CGetRemainBuf(CRouterBusiness* pRouter);
	virtual int DoWork();
};

class CBusinessGetOfflineMSG: public CBusinessExIMSrv
{
	public:		
		CBusinessGetOfflineMSG(CRouterBusiness* pRouter);
		bool GetOfflineMSGFromIMAPI(const uint32 UserId);
		bool SendOfflineMSGToClient(const uint32 UserId,const Json::Value& jvMSGItem);
		virtual int DoWork();
};
