#include "stdafx.h"
#include "BusinessRouterUserPacket.h"
#include "AppManager.h"
#include "RouterBusiness.h"
#include "protocol.h"



CBusinessRouterPacket::CBusinessRouterPacket(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_ROUTERPACKET_REQ,CMD_ROUTERPACKET_REQ)
{
}

CBusinessRouterPacket::~CBusinessRouterPacket()
{
}

int CBusinessRouterPacket::DoWork()
{

	//转发该消息
	SendDataStruct sendData;
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	memcpy(sendData.data, pRecData->data, pRecData->data_len);
	sendData.data_len = pRecData->data_len;
	sendData.vlink = pRecData->vlink;


	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
	this->CancelReply();
	return RESULT_SUCCESS;
}


CBusinessRouterAckPacket::CBusinessRouterAckPacket(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_ROUTERPACKET_ACK,CMD_ROUTERPACKET_ACK)
{
}

CBusinessRouterAckPacket::~CBusinessRouterAckPacket()
{
}

int CBusinessRouterAckPacket::DoWork()
{

	//转发该消息
	SendDataStruct sendData;
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	memcpy(sendData.data, pRecData->data, pRecData->data_len);
	sendData.data_len = pRecData->data_len;
	sendData.vlink = pRecData->vlink;


	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
	this->CancelReply();
	return RESULT_SUCCESS;
}


CBusinessRouterUserPacket::CBusinessRouterUserPacket(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_ROUTERUSERPACKET_REQ,CMD_ROUTERUSERPACKET_REQ)
{
}

CBusinessRouterUserPacket::~CBusinessRouterUserPacket()
{
}

int CBusinessRouterUserPacket::DoWork()
{

	//转发该消息
	SendDataStruct sendData;
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	memcpy(sendData.data, pRecData->data, pRecData->data_len);
	sendData.data_len = pRecData->data_len;
	sendData.vlink = pRecData->vlink;


	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
	this->CancelReply();
	return RESULT_SUCCESS;
}

CBusinessRouterUserAckPacket::CBusinessRouterUserAckPacket(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_ROUTERUSERPACKET_ACK,CMD_ROUTERUSERPACKET_ACK)
{
}

CBusinessRouterUserAckPacket::~CBusinessRouterUserAckPacket()
{
}

int CBusinessRouterUserAckPacket::DoWork()
{

	//转发该消息
	SendDataStruct sendData;
	RecvDataStruct* pRecData = (RecvDataStruct*)m_RowPacketBuf;
	memcpy(sendData.data, pRecData->data, pRecData->data_len);
	sendData.data_len = pRecData->data_len;
	sendData.vlink = pRecData->vlink;


	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0,0,0,0,sendData.vlink);
	this->CancelReply();
	return RESULT_SUCCESS;
}


CBusinessReqTransFile::CBusinessReqTransFile(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_TRANSFILE_REQ,CMD_TRANSFILE_REQ)
{
}

CBusinessReqTransFile::~CBusinessReqTransFile()
{
}

int CBusinessReqTransFile::DoWork()
{
	CUser *pUser = CAppManager::GetInstance().GetUserManager().Find(m_pRecvHead->sid);
	if (pUser)
	{
		XT_ROUTERPACKET_REQ* pRouterReq  = (XT_ROUTERPACKET_REQ*)m_RecvPacket.GetData();
		
		//转发该消息
		SendDataStruct sendData;
		LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
		int len = 0;	

		CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
		sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
		sendPacket	<<pUser->m_nIP
					<<pUser->m_nPort;
		len += sendPacket.GetSize();	

		CAppManager::GetInstance().InitHead(lpSendHead,CMD_TRANSFILE_REQ,len,pRouterReq->uid);
		sendData.data_len = len + (int)sizeof(XT_HEAD);	

		CAppManager::GetInstance().GetAppNetWork().SendDataTo(
			sendData.data,sendData.data_len,0);
	}
	this->CancelReply();
	return RESULT_SUCCESS;
}

CBusinessAckTransFile::CBusinessAckTransFile(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_TRANSFILE_ACK,CMD_TRANSFILE_ACK)
{
}

CBusinessAckTransFile::~CBusinessAckTransFile()
{
}

int CBusinessAckTransFile::DoWork()
{
	CUser *pUser = CAppManager::GetInstance().GetUserManager().Find(m_pRecvHead->sid);
	if (pUser)
	{
		XT_ROUTERPACKET_ACK* pRouterAck  = (XT_ROUTERPACKET_ACK*)m_RecvPacket.GetData();

		//转发该消息
		SendDataStruct sendData;
		LPXT_HEAD lpSendHead = (LPXT_HEAD)sendData.data;
		int len = 0;	

		CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
		sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
		sendPacket	<<pUser->m_nIP
					<<pUser->m_nPort;
		len += sendPacket.GetSize();	

		CAppManager::GetInstance().InitHead(lpSendHead,CMD_TRANSFILE_ACK,len,pRouterAck->uid);
		sendData.data_len = len + (int)sizeof(XT_HEAD);	

		CAppManager::GetInstance().GetAppNetWork().SendDataTo(
			sendData.data,sendData.data_len,0);
	}
	this->CancelReply();
	return RESULT_SUCCESS;
}

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

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);

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

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);

	return RESULT_SUCCESS;
}

CBusinessReqCusterTran::CBusinessReqCusterTran(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CUSTSER_TRAN_REQ,CMD_CUSTSER_TRAN_REQ)
{
}
CBusinessReqCusterTran::~CBusinessReqCusterTran()
{
}

int CBusinessReqCusterTran::DoWork()
{	
	SendDataStruct sendData;
	XT_CUSTSER_TRAN_REQ* CusterTranReq = (XT_CUSTSER_TRAN_REQ*)m_RecvPacket.GetData();
	
	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
	sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
	
	int len = sendPacket.GetSize();
	XT_HEAD* pHead = (XT_HEAD*)sendData.data;
	
	CAppManager::GetInstance().InitHead(pHead,CMD_CUSTSER_TRAN_REQ,len,CusterTranReq->uDid);
	sendData.data_len = len + (int)sizeof(XT_HEAD);	

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);

	CancelReply();
	
	return RESULT_SUCCESS;

}
CBusinessAckCusterTran::CBusinessAckCusterTran(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_CUSTSER_TRAN_ACK,CMD_CUSTSER_TRAN_ACK)
{
}
CBusinessAckCusterTran::~CBusinessAckCusterTran()
{
}

int CBusinessAckCusterTran::DoWork()
{
	SendDataStruct sendData;
	XT_CUSTSER_TRAN_ACK* CusterTranAck = (XT_CUSTSER_TRAN_ACK*)m_RecvPacket.GetData();
	
	CSendPacket sendPacket(sendData.data + sizeof(XT_HEAD));
	sendPacket.WritePacket(m_RecvPacket.GetData(),m_RecvPacket.GetSize());
	
	int len = sendPacket.GetSize();
	XT_HEAD* pHead = (XT_HEAD*)sendData.data;
	
	CAppManager::GetInstance().InitHead(pHead,CMD_CUSTSER_TRAN_ACK,len,CusterTranAck->uDid);
	sendData.data_len = len + (int)sizeof(XT_HEAD);	

	CAppManager::GetInstance().GetAppNetWork().SendDataTo(
		sendData.data,sendData.data_len,0);
	CancelReply();
	
	return RESULT_SUCCESS;

}
