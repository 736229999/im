#include "stdafx.h"
#include "NotifyProtocol.h"
#include "AppManager.h"
#include "GlobalBusi.h"

CNotifyProtocol::CNotifyProtocol(void)
{
}

CNotifyProtocol::~CNotifyProtocol(void)
{
}


void CNotifyProtocol::Relogin(CGlobalBusi *pGlobalBusi, uint32 uid)
{
	int len = 0;
	SendDataStruct sendData;
	LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
	len += sizeof(XT_HEAD);
	LPXT_SERVER_TRANSMIT lpSrvTransmit = (LPXT_SERVER_TRANSMIT)(sendData.data + sizeof(XT_HEAD));
	lpSrvTransmit->origin_id = 0;
	lpSrvTransmit->msgtype = XT_SERVER_TRANSMIT::TYPE_SYS_MESSAGE;
	len += sizeof(XT_SERVER_TRANSMIT);

	XT_SERVERMSG srvMsg;
	srvMsg.send_time = timeGetTime();
	srvMsg.type = 1;
	strcpy_s(srvMsg.message,"该帐号在其他地方登陆！\n");
	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD) + sizeof(XT_SERVER_TRANSMIT));
	sendPacket<<srvMsg.send_time<<srvMsg.type<<srvMsg.message;
	len += sendPacket.GetSize();	
	sendData.data_len = len;

	CAppManager::GetInstance().InitHead(lpSendHead,CMD_SERVERMSG_IND,len,uid);
	CAppManager::GetInstance().GetAppNetWork().SendDataTo(pGlobalBusi, sendData.data,sendData.data_len,SEND_RETRY_TIME);

}

