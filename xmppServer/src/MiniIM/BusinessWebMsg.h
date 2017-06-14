#pragma once
#include "BusinessExIMSrv.h"
#include "DataBaseLib.h"


//服务器转发来自客户端的消息或直接发送系统消息的应答,
//该业务类仅处理接收数据,不再返回数据
class CBusinessWebMsg : public CBusinessExIMSrv
{
public:
	CBusinessWebMsg(CRouterBusiness* pRouter);
	virtual ~CBusinessWebMsg(void);
	virtual int DoWork();
	virtual int SendReply();
	void split(string& id, string& pattern, vector<string>* sub_id);
};

//客户端应答服务器通知
class CBusinessWebMsgAck :
	public CBusinessExIMSrv
{
public:
	CBusinessWebMsgAck(CRouterBusiness* pRouter);
	virtual ~CBusinessWebMsgAck(void);

	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);

	virtual int DoWork();
	virtual int SendReply();
};

class CBusinessOfflineWebMsg
{
public:
	CBusinessOfflineWebMsg(void);
	virtual ~CBusinessOfflineWebMsg(void);

	virtual int GetOfflineWokuMsg(CUser *pUser, char* strFriend,CDataBaseWeb* pDataBaseWeb,CDataBaseMini* pDataBaseMini);
	void SendOfflineWokuMsg(ST_WOKU_MSG *pWebMsg);

private:
	ST_WOKU_MSG*	m_pOperationBuf;
};


class CBusinessSystemLog : public CBusinessExIMSrv
{
public:
	CBusinessSystemLog(CRouterBusiness* pRouter);
	virtual ~CBusinessSystemLog(void);
	virtual int DoWork();
	virtual int SendReply();
};


