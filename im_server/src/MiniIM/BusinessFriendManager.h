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
		int NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange);	  //通知组内其他成员排队状况 0 包括自己 1 不包括自己
		virtual int DoWork();
};

class CBusinessCloseReceptionByCustomer: public CBusinessExIMSrv
{
	public:		
		CBusinessCloseReceptionByCustomer(CRouterBusiness* pRouter);
		int NotifyBuyerQuitInfo(XT_BUYERINFO_CHANGE* QueueChange);	  //通知组内其他成员排队状况 0 包括自己 1 不包括自己
		virtual int DoWork();
};


class CBusinessForceOfflineCs: public CBusinessExIMSrv
{
	public:		
		CBusinessForceOfflineCs(CRouterBusiness* pRouter);
		int ForceOfflineCs(XT_FORCE_OFFLINE_CS* xtOfflineCs);	  //强制客服下线
		virtual int DoWork();
};



//修改挂起
class CBusinessModHangUp : public CBusinessExIMSrv
{
public:
	CBusinessModHangUp(CRouterBusiness* pRouter);
	virtual int DoWork();
};


//获取在线状态
class CBusinessGetOnlineStatus : public CBusinessExIMSrv
{
public:
	CBusinessGetOnlineStatus(CRouterBusiness* pRouter);
	virtual int DoWork();
};

//是否有子账户在线
class CBusinessCheckSubOnline : public CBusinessExIMSrv
{
public:
	CBusinessCheckSubOnline(CRouterBusiness* pRouter);
	virtual int DoWork();
};

//获取最佳子账户ID
class CBusinessGetSubAccount : public CBusinessExIMSrv
{
public:
	
	CBusinessGetSubAccount(CRouterBusiness* pRouter);
	virtual int DoWork();	

	int AddToQueueAndNotify(CUser* User,uint32 buyerId,XT_GET_SUB_ACCOUNT_REQ *req);
	int NotifyTeamQueueInfo(XT_WAITQUEU_CHANGE* QueueChange);	//通知组内其他成员排队状况

private:
	using LastAccountMap = std::map<uint32_t, uint32_t>;
	static LastAccountMap account_load_map_;
	static std::mutex account_load_mutex_;
};


//获取好友基本信息
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

//获取好友详细信息
class CBusinessGetFriendFullInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendFullInfo(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	void ReturnUserInfo(CUser* pUser, XT_SERVER_USERINFO_GET_ACK* pUserInfoAck);

};

//修改用户资料
class CBusinessModifyUserInfo : public CBusinessExIMSrv
{
public:
	CBusinessModifyUserInfo(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	void MD5Password(char* pPassword);

};


//得到好友分组
class CBusinessGetGroupInfo : public CBusinessExIMSrv
{
public:
	CBusinessGetGroupInfo(CRouterBusiness* pRouter);

	virtual int DoWork();
	//CGetBackendGroup CSGroup;
private:
	//组信息缓存
	XT_GROUPINFO_GET_ACK::tagGroupInfo	m_GroupInfo[MAX_GROUP_NUM];
	bool GetFriendGroupList(int id,uint8 &count, vector<GROUPEX> &friendgrouplist);

};





//得到好友列表
class CBusinessGetFriendList : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendList(CRouterBusiness* pRouter);	
	virtual int DoWork();


private:	
	bool GetFriendList(int id, int merchantId,uint32 &count, XT_FRIENDLIST_ACK::tagFriendInfo *friendlist);
};

//修改好友分组信息
class CBusinessGroupInfoModify : public CBusinessExIMSrv
{
public:
	CBusinessGroupInfoModify(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
	// 添加好友分组
	bool AddGroup(int uId, int groupPid, int groupId, const char *groupName, std::string &error);
	// 修改好友分组
	bool ModGroup(int uId, int groupId, const char *groupName, std::string &error);
	// 删除好友分组
	bool DelGroup(int uId, int groupId, string &error);
};

//查找好友
class CBusinessSearchFriend : public CBusinessExIMSrv
{
public:
	CBusinessSearchFriend(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

	uint8			m_nReturnCount;

	XT_XTREAM_SEARCH_REQ*		m_pSearchFriendReq;
	XT_XTREAM_SEARCH_ACK*		m_pSearchFriendAck;

	int FindOnlineUser();		//查找在线用户
	int	FindPrecision() ;		//精确查找


};

//添加用户、修改用户所属组或删除用户
class CBusinessFriendModify : public CBusinessExIMSrv
{
public:
	CBusinessFriendModify(CRouterBusiness* pRouter);

	virtual int DoWork();

	//添加好友到组
	uint8  AddFriendToGroup();
	//添加好友到组的执行部分
	uint8  DoAddFriend();

	//修改好友分组
	uint8  ModifyFriendGroup();

	//删除好友
	uint8  DoDelFriendGroup(int userid, int friendid);


	inline void  SendFriendNotify(uint32 BeAddedID,uint32 ToAddID);
	inline void  SendFriendDelMe(uint32 friendID,uint32 myID);

	// 调用接口修改好友分组
	uint8 DoModFriendGroup(int uid, int friendid, int groupid);

private:

	XT_FRIENDGROUP_MOD_REQ* m_pFriendGroupReq ;
	XT_FRIENDGROUP_MOD_ACK* m_pFriendGroupAck ;
	shared_ptr<CUser>					m_pUser;

};


//发送添加用户的验证信息或发送同意、拒绝被加为好友信息给服务器
class CBusinessVerifyFriend : public CBusinessExIMSrv
{
public:
	CBusinessVerifyFriend(CRouterBusiness* pRouter);

	virtual int DoWork();

	void SendVerifyFriendNotify(uint32 to_id,uint8 groupid);

private:

};

//修改用户状态
class CBusinessModifyStatus : public CBusinessExIMSrv
{
public:
	CBusinessModifyStatus(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

};

//给好友发送聊天消息
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

//获取好友的在线连接信息
class CBusinessGetLinkStatus : public CBusinessExIMSrv
{
public:
	CBusinessGetLinkStatus(CRouterBusiness* pRouter);

	virtual int DoWork();

private:

};


//修改好友备注
class CBusinessModFriendComment : public CBusinessExIMSrv
{
public:
	CBusinessModFriendComment(CRouterBusiness* pRouter);

	virtual int DoWork();


private:

};


//用户行为统计分析
class CBusinessUserActionAudit : public CBusinessExIMSrv
{
public:
	CBusinessUserActionAudit(CRouterBusiness* pRouter);

	virtual int DoWork();


};

//修改用户个人签名
class CBusinessModifyUnderWrite : public CBusinessExIMSrv
{
public:
	CBusinessModifyUnderWrite(CRouterBusiness* pRouter);

	virtual int DoWork();

};

//修改好友关注标记
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
/*收到的图片结构体*/
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


/*********************************************设置客服接待的买家***************************************************/
class CCSetChatWithBuyer : public CBusinessExIMSrv
{
public:
	CCSetChatWithBuyer(CRouterBusiness* pRouter);
	virtual int DoWork();

private:
	// 系统中转消息，通知主管他监控的客服改变了正在聊天的买家
	void  SendChatBuyerChanged(uint32 nDirectorID, uint32 nCsId, uint32 nBuyerID);
};

/***************************************************主管监控功能****************************************************/
class CDirectorMonitorStart : public CBusinessExIMSrv
{
public:
	CDirectorMonitorStart(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************结束监控功能****************************************************/
class CDirectorMonitorStop : public CBusinessExIMSrv
{
public:
	CDirectorMonitorStop(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************主管插入会话****************************************************/
class CMonitorInsertChat : public CBusinessExIMSrv
{
public:
	CMonitorInsertChat(CRouterBusiness* pRouter);
	virtual int DoWork();
};

/***************************************************主管接手会话****************************************************/
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


class CBusinessSetCSClient: public CBusinessExIMSrv		//处理客服中已接待结束的用户请求
{
public:
	CBusinessSetCSClient(CRouterBusiness* pRouter);	
//	void updateCsQueue(ClientService *clientService);	
	~CBusinessSetCSClient();
	virtual int DoWork();
};


/***************************************************获取消息提醒****************************************************/
class CGetMsgRemind : public CBusinessExIMSrv
{
public:
	CGetMsgRemind(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************设置消息提醒****************************************************/
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

//获取客服所在组信息
class CBusinessCsTeamInfoReq : public CBusinessExIMSrv
{
public:
	CBusinessCsTeamInfoReq(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************获取离线消息推送数据****************************************************/
class CGetOfflineMsgNotify : public CBusinessExIMSrv
{
public:
	CGetOfflineMsgNotify(CRouterBusiness* pRouter);
	virtual int DoWork();
};


/***************************************************请求未接收完的buf****************************************************/
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
