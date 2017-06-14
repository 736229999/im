#include "stdafx.h"
#include "BusinessAddModifyOnlineUser.h"
#include "protocol.h"
#include "AppManager.h"
#include "Function.h"
#include "xtpacket.h"


CBusinessAddModifyOnlineUser::CBusinessAddModifyOnlineUser(CRouterBusiness* pRouter):
CBusinessExGSrv(pRouter,CMDGSRV_ADDMODIFY_USER_FULLINFO_QUERY,CMDGSRV_ADDMODIFY_USER_FULLINFO_ACK) 
{
	TransferIP = inet_addr(m_ConfigInfo.GetTransferServerIP());
}

CBusinessAddModifyOnlineUser::~CBusinessAddModifyOnlineUser(void)
{
}


int CBusinessAddModifyOnlineUser::DoWork()
{
	COnlineManager* pOnlineManager = CAppManager::GetInstance().GetOnlineManager();
	
	ReloginUser	ReloginUser[256];
	uint8	ReloginUserCount;
	
	//通知文件传输服务器用户登录
	NotifyTransferLogin(m_RecvPacket);
	
	pOnlineManager->AddModifyOnlineUser(m_RecvPacket,m_SendPacket,ReloginUser,ReloginUserCount);

	//添加时可能产生重登录用户，这时必须通知旧的IM服务器
	for(int i = 0 ; i < ReloginUserCount; ++i )
	{
		NotifyRelogin(ReloginUser[i].serverip,ReloginUser[i].userid, ReloginUser[i].userip, ReloginUser[i].lanip);
	}

	return 0;
}

void CBusinessAddModifyOnlineUser::NotifyTransferLogin(CRecvPacket& RecvPacket)
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
	pSendHead->cmd  = CMDGSRV_NOTIFY_TRANSFER_LOGIN;
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

void CBusinessAddModifyOnlineUser::NotifyRelogin(uint32 ImIp,uint32 userid,uint32 uip, uint32 lanip)
{
	SendDataStruct SendData;
	XT_HEAD* pSendHead = (XT_HEAD*)SendData.data;
	//RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;

	CSendPacket SendPacket;
	SendPacket.SetData(SendData.data+ sizeof(XT_HEAD),
		MAX_SEND_BUFFERS - sizeof(XT_HEAD) );
	SendPacket << userid
				<<(uint8)0
				<<uip
				<<lanip;



	//构建网络层的信息
	SendData.dest_ip = ImIp;
	SendData.port	   = PORT_APPNET2GLOBAL;
	SendData.vlink   = 0;
	SendData.retry_times = SEND_RETRY_TIME;
	SendData.data_len  = (uint16)SendPacket.GetSize() + sizeof(XT_HEAD);


	//构建协议头的信息
	//XT_HEAD* pRecvHead = (XT_HEAD*)pRecData->data;

	pSendHead->flag = XT_HEAD::FLAG_SRVACTION;
	pSendHead->from = XT_HEAD::FROM_SERVER;
	pSendHead->cmd  = CMDGSRV_USERRELOGIN_NOTIFY;
	pSendHead->seq  = CAppManager::GetInstance().GetSendSeq();
	pSendHead->sid  = XT_HEAD::SID_SERVER;
	pSendHead->dtype= XT_HEAD::DEST_TYPE_SERVER;
	pSendHead->did  = XT_HEAD::SID_SERVER;
	pSendHead->len  = SendPacket.GetSize();


	//加密数据
	int nDataLength = SendData.data_len;
	::xTEAEncryptWithKey(SendData.data + sizeof(XT_HEAD) ,SendData.data_len - sizeof(XT_HEAD) ,
		m_globalSessionKey,
		SendData.data + sizeof(XT_HEAD) ,&nDataLength ) ;
	SendData.data_len = (uint16)nDataLength;

	SendData.data_len += sizeof(XT_HEAD);

	//发送数据
	assert(m_pNetTrans);
	m_pNetTrans->SendDataTo(SendData);

}

