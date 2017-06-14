#pragma once
#include "BusinessExIMSrv.h"
#include "ClientProtocol.h"
#include "PublicType.h"
#include "BusinessWebIM.h"


#include <mutex>
#ifdef IM_USE_SSDB
#include "../../ssdb/SSDBConn.h"
#endif


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

private:
	using LastAccountMap = std::map<uint32_t, uint32_t>;
	static LastAccountMap account_load_map_;
	static std::mutex account_load_mutex_;
};

//振动好友窗口
class CBusinessShakeFriendWin : public CBusinessExIMSrv
{
public:
	CBusinessShakeFriendWin(CRouterBusiness* pRouter);

	virtual int DoWork();
	void RouterShakeNotify(uint32 to_id);
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
private:
	//组信息缓存
	XT_GROUPINFO_GET_ACK::tagGroupInfo	m_GroupInfo[MAX_GROUP_NUM];

};





//得到好友列表
class CBusinessGetFriendList : public CBusinessExIMSrv
{
public:
	CBusinessGetFriendList(CRouterBusiness* pRouter);	
	virtual int DoWork();

private:
	
};

//修改好友分组信息
class CBusinessGroupInfoModify : public CBusinessExIMSrv
{
public:
	CBusinessGroupInfoModify(CRouterBusiness* pRouter);

	virtual int DoWork();

private:
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
	uint8	DelFriendGroup();


	inline void  SendFriendNotify(uint32 BeAddedID,uint32 ToAddID);
	inline void  SendFriendDelMe(uint32 friendID,uint32 myID);


private:

	XT_FRIENDGROUP_MOD_REQ* m_pFriendGroupReq ;
	XT_FRIENDGROUP_MOD_ACK* m_pFriendGroupAck ;
	CUser*					m_pUser;

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

	void RouterMsgNotify(uint32 to_id);
public:
	void SaveWebMsg();
#ifdef IM_USE_SSDB
	void SaveTalkMsg();
#endif

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

class CBusinessXmppBusiList: public CBusinessExIMSrv
{
	public:		
		CBusinessXmppBusiList(CRouterBusiness* pRouter);		
		~CBusinessXmppBusiList();		
		virtual int DoWork();
		bool getMerchantInfo(uint32 merchantid,char *storename);
	private:		
		SendDataStruct* pData;
		void SendXmppBusiListAck(CSendPacket& m_SendPacket);
		XT_XMPP_BUSI_LIST_ACK::BusiList* pListInfo;
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

//重登陆通知
class CBusinessReloginNotify : public CBusinessExIMSrv
{
public:
	CBusinessReloginNotify(CRouterBusiness* pRouter);

	virtual int DoWork();

	virtual int SendReply();

	//清除异地重登录用户信息
	void LogoutLocalIm(uint32& userid);

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


class CBusinessHistoryMsg : public CBusinessExIMSrv
{
public:
	CBusinessHistoryMsg(CRouterBusiness* pRouter);
	~CBusinessHistoryMsg();
	virtual int DoWork();
private:
	SendDataStruct*	pData;
	void SendWebMoreMsgAck(CSendPacket& SendPacket);
	uint32 seq;
};



