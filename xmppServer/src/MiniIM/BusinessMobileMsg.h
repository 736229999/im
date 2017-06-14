#pragma once

#include "BusinessExIMSrv.h"

class CBusinessMobileMsg :
	public CBusinessExIMSrv
{
public:
	CBusinessMobileMsg(CRouterBusiness* pRouter);
	~CBusinessMobileMsg(void);
	virtual int DoWork();
	virtual int SendReply();
};

//客户端应答服务器通知
class CBusinessMobileMsgAck :
	public CBusinessExIMSrv
{
public:
	CBusinessMobileMsgAck(CRouterBusiness* pRouter);
	~CBusinessMobileMsgAck(void);

	virtual void SetRowPacket(BYTE* pRowPacket,int nPacketLength);

	virtual int DoWork();
	virtual int SendReply();
};


class CBusinessOfflineMobileMsg
{
public:
	CBusinessOfflineMobileMsg(void);
	virtual ~CBusinessOfflineMobileMsg(void);

	virtual int GetOfflineMobileMsg(CUser *pUser,CDataBaseWeb* pDataBaseWeb);
	void SendOfflineMobileMsg(ST_MOBILE_MSG *pWebMsg);

private:
	ST_MOBILE_MSG*	m_pMobileMsgBuf;
};

class CBusinessSendMobileMessage : public CBusinessExIMSrv
{
public:
	CBusinessSendMobileMessage(CRouterBusiness* pRouter);
	virtual int DoWork();
private:

};

