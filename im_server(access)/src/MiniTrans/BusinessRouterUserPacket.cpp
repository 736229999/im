#include "stdafx.h"
#include "BusinessRouterUserPacket.h"
#include "AppManager.h"
#include "RouterBusiness.h"
#include "protocol.h"

CBusinessTransfer::CBusinessTransfer(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_FILE_TRANS_REQ,CMD_FILE_TRANS_ACK)
{
}

CBusinessTransfer::~CBusinessTransfer()
{
}

int CBusinessTransfer::DoWork()
{
	SendDataStruct sendData;
	SEND_FILE_PACK_REQ* FileReq = (SEND_FILE_PACK_REQ*)m_RecvPacket.GetData();

	m_SendPacket<<FileReq->send_id
				<<FileReq->fid
				<<FileReq->flag
				<<FileReq->file_id
				<<FileReq->pack_id
				<<FileReq->size;

	CLog::Log("CBusinessTransfer::DoWork",CLog::TYPE_IMPORTANT,"fid:%d, did:%d, file:%d, pack:%d, size:%d",
		FileReq->send_id,FileReq->fid,FileReq->file_id,FileReq->pack_id,FileReq->size);
	
	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
	sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());

	int len = sendPacket.GetSize();
	XT_HEAD* pHead = (XT_HEAD*)sendData.data;
	
	CAppManager::GetInstance().InitHead(pHead,CMD_FILE_TRANS_REQ,len,FileReq->fid);
	sendData.data_len = len + (int)sizeof(XT_HEAD);	

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
		sendData.data,sendData.data_len,0);

	CancelReply();
	return RESULT_SUCCESS;
}

CBusinessTransferInd::CBusinessTransferInd(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_FILE_TRANS_IND,CMD_FILE_TRANS_IND_ACK)
{
}

CBusinessTransferInd::~CBusinessTransferInd()
{
}

int CBusinessTransferInd::DoWork()
{	
	SendDataStruct sendData;
	SEND_FILE_PACK_IND* FileInd = (SEND_FILE_PACK_IND*)m_RecvPacket.GetData();
	
	m_SendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
	
	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
	sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
	
	int len = sendPacket.GetSize();
	XT_HEAD* pHead = (XT_HEAD*)sendData.data;
	
	CAppManager::GetInstance().InitHead(pHead,CMD_FILE_TRANS_IND,len,FileInd->fid);
	sendData.data_len = len + (int)sizeof(XT_HEAD);	

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(m_pRouter->GetGlobalBusi(),
		sendData.data,sendData.data_len,0);
	
	CancelReply();
	return RESULT_SUCCESS;
}

CBusinessTransferNotify::CBusinessTransferNotify(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_FILE_TRANS_NOTIFY,CMD_FILE_TRANS_NOTIFY_ACK)
{
}

CBusinessTransferNotify::~CBusinessTransferNotify()
{
}

int CBusinessTransferNotify::DoWork()
{	
	m_SendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());

	return RESULT_SUCCESS;
}


