#include "stdafx.h"
#include "BusinessDelOnlineUser.h"
#include "protocol.h"
#include "AppManager.h"
#include "Function.h"

CBusinessDelOnlineUser::CBusinessDelOnlineUser(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_DEL_USER_FULLINFO_QUERY,CMDGSRV_DEL_USER_FULLINFO_ACK)
{
	TransferIP = inet_addr(m_ConfigInfo.GetTransferServerIP());
}

CBusinessDelOnlineUser::~CBusinessDelOnlineUser(void)
{
}


int CBusinessDelOnlineUser::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();
	RecvDataStruct * pRecvData = (RecvDataStruct*)m_RowPacketBuf;

	//通知文件传输服务器用户下线
	NotifyTransferLogout(m_RecvPacket);
	
	pOnlineManager->DelOnlineUser(m_RecvPacket,m_SendPacket,pRecvData->src_ip);

	return 0;
}

void CBusinessDelOnlineUser::NotifyTransferLogout(CRecvPacket& RecvPacket)
{
	SendDataStruct SendData;
	CSendPacket SendPacket;

	SendPacket.SetData(SendData.data+ sizeof(XT_HEAD),MAX_SEND_BUFFERS - sizeof(XT_HEAD));
	SendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
	SendData.dest_ip = TransferIP;
	SendData.port = PORT_TRANS2GLOBAL;
	SendData.vlink = 0;
	SendData.retry_times = 0;
	SendData.data_len = (uint16)SendPacket.GetSize() + sizeof(XT_HEAD);
	
	XT_HEAD* pSendHead = (XT_HEAD*)SendData.data;
	pSendHead->flag = XT_HEAD::FLAG_SRVACTION;
	pSendHead->from = XT_HEAD::FROM_SERVER;
	pSendHead->cmd  = CMDGSRV_NOTIFY_TRANSFER_LOGOUT;
	pSendHead->seq  = CAppManager::GetInstance().GetSendSeq();
	pSendHead->sid  = XT_HEAD::SID_SERVER;
	pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
	pSendHead->did  = XT_HEAD::SID_SERVER;
	pSendHead->len  = SendPacket.GetSize();

	int nDataLength = SendData.data_len;
	::xTEAEncryptWithKey(SendData.data + sizeof(XT_HEAD) ,SendData.data_len - sizeof(XT_HEAD) ,
		m_globalSessionKey,SendData.data + sizeof(XT_HEAD) ,&nDataLength ) ;
	
	SendData.data_len = (uint16)nDataLength;
	SendData.data_len += sizeof(XT_HEAD);

	assert(m_pNetTrans);
	m_pNetTrans->SendDataTo(SendData);
}


