#include "stdafx.h"
#include "BusinessFileTrans.h"
#include "AppManager.h"

typedef struct XT_UPLOAD_CPIC_REQ
{
	uint32	fileid;
	uint16  filepos;
	uint16	datalen;
	uint32	FamilyId;
	XT_UPLOAD_CPIC_REQ()
	{
		memset(this,0,sizeof(XT_UPLOAD_CPIC_REQ));
	}
}XT_UPLOAD_CPIC_REQ, *LPXT_UPLOAD_CPIC_REQ;

CBusinessFileUpload::CBusinessFileUpload(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_SEND_CPIC_REQ,CMD_SEND_CPIC_ACK)
{

}


int CBusinessFileUpload::DoWork()
{
	XT_UPLOAD_CPIC_REQ req;

	m_RecvPacket >>req.FamilyId >> req.fileid >> req.filepos>>req.datalen;


	FID fid;
	fid.sid = m_pRecvHead->sid;
	fid.fid = req.fileid;

	CTransFileBuf* pTransFile = CAppManager::GetInstance().GetTransFileManager().Find(&fid);
	if( NULL == pTransFile)
	{
		SetErrorReturn("找不到相应fid");
		return RESULT_SUCCESS;
	}


	if( !pTransFile->AddPacket(m_RecvPacket.GetCursor(),m_RecvPacket.GetSize() -sizeof(req),req.filepos))
	{
		SetErrorReturn("pos错误");
		return RESULT_SUCCESS;
	}

	m_SendPacket << (uint8)0 << req.FamilyId << req.fileid << req.filepos;

	return RESULT_SUCCESS;
}


//////////////////////////////////////////////////////////////////////////
CBusinessFileDownload::CBusinessFileDownload(CRouterBusiness* pRouter):
CBusinessExIMSrv(pRouter,CMD_GET_CPIC_REQ,CMD_GET_CPIC_ACK)
{

}


int CBusinessFileDownload::DoWork()
{
	XT_DOWNLOAD_CPIC_REQ* pReq = (XT_DOWNLOAD_CPIC_REQ*)m_RecvPacket.GetData();
	XT_DOWNLOAD_CPIC_ACK* pAck = (XT_DOWNLOAD_CPIC_ACK*)m_SendPacket.GetData();

	FID fid;
	fid.fid = pReq->nFileId;
	fid.sid = pReq->nFileSid;

	CTransFileBufManager& TransFileBufManager = CAppManager::GetInstance().GetTransFileManager();
	CTransFileBuf* pTransFile = TransFileBufManager.Find(&fid);
	if( NULL == pTransFile)
	{
		pAck->ret = XT_DOWNLOAD_CPIC_ACK::RET_FAILED;
		pAck->nSrcId	 = pReq->nFileSid;
		pAck->nFileId	 = pReq->nFileId;
		pAck->nPacketId	 = pReq->nPacketId;
		pAck->nCId		 = m_pRecvHead->sid;
		pAck->nDataLen	 = 0;
		m_SendPacket.SetCursor(sizeof(XT_DOWNLOAD_CPIC_ACK)-sizeof(pAck->data));
	}
	else
	{
		if( pReq->nPacketId == 0xFFFF ) //下载者通知服务器数据收全了
		{
			pTransFile->ReleaseUser();
			if( pTransFile->IsNoneUser())	//是否该文件已经提供给所以需要下载的用户了
			{
				TransFileBufManager.DelTransFile(&fid);
			}

			pAck->ret = XT_DOWNLOAD_CPIC_ACK::RET_SUCCESS;
			pAck->nSrcId	 = pReq->nFileSid;
			pAck->nFileId	 = pReq->nFileId;
			pAck->nPacketId	 = pReq->nPacketId;
			pAck->nCId		 = pTransFile->GetFamilyId();
			pAck->nDataLen	 = 0;

			return RESULT_SUCCESS;
		}

		CFilePacket* pFilePacket = pTransFile->GetPacket(pReq->nPacketId);
		if( NULL == pFilePacket )
		{
			//还没准备好包数据
			pAck->ret = XT_DOWNLOAD_CPIC_ACK::RET_WAIT;
			pAck->nSrcId	 = pReq->nFileSid;
			pAck->nFileId	 = pReq->nFileId;
			pAck->nPacketId	 = pReq->nPacketId;
			pAck->nCId		 = pTransFile->GetFamilyId();
			pAck->nDataLen	 = 0;
			m_SendPacket.SetCursor(sizeof(XT_DOWNLOAD_CPIC_ACK)-sizeof(pAck->data));
		}
		else
		{
			pAck->ret = XT_DOWNLOAD_CPIC_ACK::RET_SUCCESS;
			pAck->nSrcId	 = pReq->nFileSid;
			pAck->nFileId	 = pReq->nFileId;
			pAck->nPacketId	 = pReq->nPacketId;
			pAck->nCId		 = pTransFile->GetFamilyId();
			pAck->nDataLen	 = pFilePacket->size;

			memcpy(pAck->data,pFilePacket->data,pFilePacket->size);
			m_SendPacket.SetCursor(sizeof(XT_DOWNLOAD_CPIC_ACK) -sizeof(pAck->data) + pFilePacket->size );
		}
	}

	return RESULT_SUCCESS;
}

