#pragma once


#include "BusinessExIMSrv.h"
#include "DataBaseStruct.h"

class CBusinessOfflineMsg:
	public CBusinessExIMSrv
{
public:
	CBusinessOfflineMsg(CRouterBusiness* pRouter);
	~CBusinessOfflineMsg(void);

	virtual int DoWork();

private:
	ST_OFFLINE_MSG*		m_pOfflineMsgBuf;
	ST_WEB_MSG*			m_pPubMsgBuf;
	uint8				m_nMaxOfflineMsgNum;


	void SendOfflineMsg(ST_OFFLINE_MSG* pOfflineMsg);
	void SendFamilyMsg(ST_OFFLINE_MSG* pOfflineMsg);
	void SendPubMsg(uint32 nRcvId,ST_WEB_MSG* pWebMsg);
	//void CBusinessOfflineMsg::SendOfflineFamilyMsg(CUser *pUser);
	void SendOfflineFamilyMsg(CUser *pUser);
};
