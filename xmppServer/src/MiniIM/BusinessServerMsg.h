#pragma once
#include "BusinessExIMSrv.h"


//服务器转发来自客户端的消息或直接发送系统消息的应答,
//该业务类仅处理接收数据,不再返回数据
class CBusinessServerMsg :
	public CBusinessExIMSrv
{
public:
	CBusinessServerMsg(CRouterBusiness* pRouter);
	~CBusinessServerMsg(void);

	virtual int DoWork();
	virtual int SendReply();

private:
	void VerifyFriend();
	void DelFriend();
	void FriendStatusChanged();
	void P2PMessage();

	//家族中转消息
	void FamilyMsg();
	void FamilyDismissMsg();
	void FamilyVerifyMsg();
	void FamilyMemJoinMsg();
//	void FamilyApplyMsg();
	void FamilySetMemMsg();


	XT_SERVER_TRANSMIT*  m_pServerMsgReq ;
};


//客户端应答服务器通知
class CBusinessServerMsgAck :
	public CBusinessExIMSrv
{
public:
	CBusinessServerMsgAck(CRouterBusiness* pRouter);
	~CBusinessServerMsgAck(void);

	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);

	virtual int DoWork();
	virtual int SendReply();

private:

};

