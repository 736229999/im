// AdvancedFileTransSession.cpp : ʵ���ļ�
//

// ���þɵ��ļ����䷽ʽ

#include "stdafx.h"
#include "localdata.h"
#include "AdvancedFileTransSession.h"

#ifdef _DEBUG

//#define _TEMP_DEBUG
#include <Windows.h>

#endif


const int	TIMER_SPEED_CTRL = 1000;
const int	TIME_SPEED_CTRL = 1000;


const char * GetFileName(const char * szPath)
{
	size_t nLen = strlen(szPath);
	if(nLen < 2)
		return szPath;
	const char * pB = szPath;
	const char * p = pB + nLen;
	while((--p) >= pB)
	{
		if(*p == '\\' || *p == '/')
		{
			return p + 1;
		}
	}
	return szPath;
}

CAdvancedFileTransSession::CAdvancedFileTransSession()
{
	//m_nCanSend = 0;
	//m_nAccCanSend = 0;
	//m_nMaxSpeed = 100*1024*1024;	// 100MB/s

	//m_nCanSend = 0;
	//m_nAccCanSend = 0;
	m_mrTimer = 0;

	m_nFriendId = 0;
	m_nMyId = 0;
	m_destIp = 0;
	m_destPort = 0;

	/*m_connStatus = CS_NO;
	m_ptConn.type = P2P_ENDPOINT::TRANS;
	m_ptConn.ip = 0;
	m_ptConn.port = 0;
	m_ptConn.uid = -1;*/

	/*g_nQResend = 0;
	g_nResend = 0;

	m_step = P2PCS_END;*/
	
	if ( m_mrTimer == 0)
	{
		m_mrTimer = timeSetEvent(TIME_SPEED_CTRL,0,AF_TimerSpeedContrlProc,(DWORD)this,TIME_PERIODIC);
	}
	m_bRecvFileTranNotifyAck = FALSE;
}


CAdvancedFileTransSession::~CAdvancedFileTransSession()
{
	P2PRecvPool::iterator itRecv,itRecvE;
	itRecvE = m_poolRecv.end();
	itRecv = m_poolRecv.begin();
	for(;itRecv != itRecvE; itRecv++)
	{
		delete itRecv->second;
	}

	P2PSendPool::iterator itSend,itSendE;
	itSendE = m_poolSend.end();
	itSend = m_poolSend.begin();
	for(;itSend != itSendE; itSend++)
	{
		delete itSend->second;
	}

	if ( m_mrTimer )
	{
		timeKillEvent(m_mrTimer);
		m_mrTimer=0;
	}

}




void CAdvancedFileTransSession::AF_TimerSpeedContrlProc(UINT uTimerID, UINT uMsg, DWORD_PTR dwUser, DWORD_PTR dw1, DWORD_PTR dw2)
{
	CAdvancedFileTransSession * pSession = (CAdvancedFileTransSession *)dwUser;
	pSession->AF_NotifyRecvSpeed();
	pSession->AF_NotifySendSpeed();
}


void CAdvancedFileTransSession::AF_NotifyRecvSpeed()
{
	//CMyLock lock(m_scRecvPool.Get());
	P2PRecvPool::iterator itRecv,itRecvE;
	itRecvE = m_poolRecv.end();
	itRecv = m_poolRecv.begin();
	int nBPS = 0;
	float fPercent = 0;
	for(;itRecv != itRecvE; itRecv++)
	{
		/*if( !itRecv->second->IsDownloading())
		{
			continue;
		}*/
		nBPS = itRecv->second->ClearCurRecvSize() ;
		fPercent = itRecv->second->GetOkPos() / (float)itRecv->second->GetSize();
		fPercent *= 100.0f;
		if(fPercent > 100.0f)
		{
			fPercent = 99.99f;
		}
		AF_OnRecvSpeedNotify(itRecv->second->GetFID(),nBPS,fPercent);
	}

	
}

void CAdvancedFileTransSession::AF_NotifySendSpeed()
{
	CMyLock lock(m_scSendPool.Get());
	int nBPS = 0;
	float fPercent = 0;
	P2PSendPool::iterator itSend,itSendE;
	itSendE = m_poolSend.end();
	itSend = m_poolSend.begin();
	for(;itSend != itSendE; itSend++)
	{
		/*if(!itSend->second->IsSending())
		{
			continue;
		}*/
		nBPS = itSend->second->GetCurSendSize();
		fPercent = itSend->second->GetSuccSend()/(float)itSend->second->GetSize();
		fPercent *= 100.0f;
		if(fPercent > 100.0f)
		{
			fPercent = 99.99f;
		}
		AF_OnSendSpeedNotify(itSend->second->GetID(),nBPS,fPercent);
	}
}


bool CAdvancedFileTransSession::AF_AcceptFile(uint32 nFileID, uint32 nSize, const CString & sLocalName, uint32 nStartPos, uint32 nTranIp, uint16 nTranPort, bool bAccept /* = true  */)
{
	bool bRt = false;
	// ����ͬ���ܾ������
	P2P_DOWNLOAD_FILE_REQ acc;

	if(bAccept)
	{
		acc.flag = P2P_DOWNLOAD_FILE_REQ::ACCEPT;
	}
	else
	{
		acc.flag = P2P_DOWNLOAD_FILE_REQ::DENEY;
	}
	acc.fid = nFileID;
	acc.packetSize = P2P_PACKET_SIZE;
	acc.friendId = m_nFriendId;
	acc.startPos = nStartPos;	// �ļ����ؿ�ʼλ��

	if(acc.flag == P2P_DOWNLOAD_FILE_REQ::DENEY)
	{
		// ���;ܾ��ļ�������
		g_pProtocol->SendDownloadFileReq(acc);
		return true;
	}

	CP2PRecvFile * pFile = NULL;
	
	//
	// �����Ƿ���ڸý�������
	//

	pFile = AF_FindRecvFile(nFileID);
	if(pFile)
	{
		TRACE("���ճ��г�����ͬid���ļ�����\n");
		AF_DeleteRecvFile(nFileID);
	}

	pFile = new CP2PRecvFile();

	CAdvancedFileTransSession::P2PRecvPool::iterator it;

	CString strSaveFile = sLocalName;

	int nTemp=1;
	
	while ( 1 )
	{
		bool bfind=false;

		for ( it=m_poolRecv.begin(); it!=m_poolRecv.end(); it++ )
		{
			if ( CString(it->second->m_sPath.c_str())==strSaveFile )
			{
				CString strPath,strName,strExt;
				ExtractFilePath(sLocalName,strPath,strName,strExt);

				strSaveFile = strPath+strName+"("+IntToStr(nTemp++)+")"+strExt;
				
				bfind = true;
				break;
			}
		}

		if ( bfind==false )
			break;
	}

	if(pFile->Create((LPCTSTR)strSaveFile,nFileID,nStartPos,nSize) != P2P_OK)
	{
		TRACE("�ļ�����ʧ��,ֹͣ���գ���\n");
		delete pFile;
		bRt = false;
		//AF_OnDownloadFaild();
	}
	else
	{
		TRACE("�ļ������ɹ�\n");
		// ��ӵ������
		m_poolRecv[nFileID] = pFile;

		// �������ӣ���ʼ����
		bRt = pFile->Start(m_nMyId,m_nFriendId,m_destIp,m_destPort,0,0,this);
		// ���ͽ����ļ�������
		g_pProtocol->SendDownloadFileReq(acc);
	}

	return bRt;
}

//void CAdvancedFileTransSession::AF_RecvSendFileReq(const char *data,WORD wRet)
//{
//	// �Ѿ��ظ�ack
//
//	
//	if(wRet != 0)
//	{
//		return;
//	}
//	LPP2P_SENDFILE_REQ pReq = (LPP2P_SENDFILE_REQ)data;
//	const P2P_SENDFILE_REQ  & req = *pReq;
//
//	// ����ͬ���ܾ������
//	P2P_DOWNLOAD_FILE_REQ acc;
//	CString sLocalName;
//	int nStartPos = 0;
//	if(AF_OnRecvSendFileReq(req,sLocalName,nStartPos))
//	{
//		acc.flag = P2P_DOWNLOAD_FILE_REQ::ACCEPT;
//	}
//	else
//	{
//		acc.flag = P2P_DOWNLOAD_FILE_REQ::DENEY;
//	}
//	acc.fid = req.fid;
//	acc.packetSize = P2P_PACKET_SIZE;
//	acc.pt = req.pt;
//	acc.startPos = nStartPos;	// �ļ����ؿ�ʼλ��
//
//	if(acc.flag == P2P_DOWNLOAD_FILE_REQ::DENEY)
//	{
//		// ���;ܾ��ļ�������
//		g_pProtocol->SendDownloadFileReq(acc);
//		return;
//	}
//
//	CP2PRecvFile * pFile = NULL;
//	P2PID id;
//	id.fid = req.fid;
//	id.uid = req.pt.uid;
//
//	//
//	// �����Ƿ���ڸý�������
//	//
//
//	pFile = AF_FindRecvFile(id);
//	if(pFile)
//	{
//		TRACE("���ճ��г�����ͬid���ļ�����\n");
//		AF_DeleteRecvFile(id);
//	}
//
//	pFile = new CP2PRecvFile();
//	if(pFile->Create((LPCTSTR)sLocalName,id.uid,id.fid,0,req.size.sizel,acc.packetSize,req.pt) != P2P_OK)
//	{
//		TRACE("�ļ�����ʧ��,ֹͣ���գ���\n");
//		delete pFile;
//		//AF_OnDownloadFaild();
//	}
//	else
//	{
//		TRACE("�ļ������ɹ�\n");
//		// ��ӵ������
//		m_poolRecv[id] = pFile;
//		// ���ͽ����ļ�������
//		g_pProtocol->SendDownloadFileReq(acc);
//	}
//};
//

void CAdvancedFileTransSession::AF_RecvSendFileAck(const char *data,WORD wRet)
{
	LPP2P_SENDFILE_ACK pAck = (LPP2P_SENDFILE_ACK)data;
	P2PID id;
	if(wRet != 0)
	{	
		SendDataStruct * pSend = (SendDataStruct *)data;
		LPXT_HEAD  pHead = (LPXT_HEAD)pSend->data;
		P2PID id(pHead->did,pSend->vlink);
		if(pHead->did != m_nFriendId)
			return;
		TRACE("���͵��û�%d���ļ�%d����ʱ\n",pHead->did,pSend->vlink);
		AF_OnSendFailed(pSend->vlink,FTE_SEND_FILE_TIMEOUT);
		AF_DeleteSendFile(pSend->vlink);
		return;
	}
}

// ���ͷ��յ����շ�ͬ���ܾ�������
void CAdvancedFileTransSession::AF_RecvDownloadFileReq(const char *data,WORD wRet)
{
	LPP2P_DOWNLOAD_FILE_REQ pReq = (LPP2P_DOWNLOAD_FILE_REQ)data;
	if(wRet != 0)
	{
		return;
	}

	P2P_DOWNLOAD_FILE_REQ & req = *pReq;

	CP2PSendFile * pSendFile = NULL;

	if(req.friendId != m_nFriendId)
	{
		return;
	}

	if(req.flag == P2P_DOWNLOAD_FILE_REQ::DENEY)
	{
		TRACE("�Է���ͬ������ļ���\n");
		AF_OnSendFileReturn(req.fid,false);
		AF_DeleteSendFile(req.fid);
		return ;
	}
	else
	{
		AF_OnSendFileReturn(req.fid,true);
	}
	pSendFile = AF_FindSendFile(req.fid);
	if(pSendFile == NULL)
	{
		TRACE("�Է��������ص��ļ�%d������\n",pReq->fid);
		return;
	}

	//TRACE("�Է������ļ���%d���ȴ��Է����ء�����\n",pSendFile->GetPacket());
	pSendFile->Start(req.startPos,req.fid,m_nMyId,m_nFriendId,m_destIp,m_destPort,0,0,this);

	//
	// ��ʱ���Է��Ѿ�ͬ������ļ�����Ҫ׼�����ļ���������Ҫ������
	//


//#ifdef _TEMP_DEBUG
//	// ��ǰʹ����ת������������Ҫ����ת��������������
//	XT_TRANS_LOGIN_REQ login;
//	login.data = 0;
//	m_arrSendConn.push_back(id.fid);
//	g_pProtocol->C2TSendLogin(login);
//	AF_OnSendStatusNotify(id,TS_TRANS_START_CONNECT);
//	return;
//#endif

	//bool bCanP2P = (m_connStatus == CS_P2P) && ( AF_HasSendingFile() || AF_HasDownloadingFile());

	//if(bCanP2P)
	//{
	//	// ��ǰ�Ѿ���P2P���ӣ���ֱ��ʹ�ø�����
	//	// �ȴ��Է�����
	//	AF_OnSendStatusNotify(id,TS_P2P_WAIT_DOWNLOAD);
	//}
	//else
	//{
	//	// �յ����շ�B�������ļ�����ʱ���Ϳ�ʼp2p���Ӳ���
	//	//if(m_step == P2PCS_END || m_step == P2PCS_GETIPFAILED)
	//	if(m_step == P2PCS_GETIPFAILED)
	//		m_step = P2PCS_BEG;
	//	
	//	// ��ǰû����Ч��p2p���ӣ���ʼ����p2p����
	//	if(m_step  == P2PCS_BEG)
	//	{
	//		m_step = P2PCS_GETIP;
	//		// ��ȡ��ǰ���ѵ�ip�Ͷ˿�
	//		XT_STATUS_GET_REQ reqStatus;
	//		reqStatus.fid = m_ptConn.uid;
	//		reqStatus.id = g_ImUserInfo.GetId();
	//		g_pProtocol->SendReq(reqStatus);
	//		// �������ʧ����Ҫ������ת������
	//	}
	//	// ��ʼ����p2p����
	//	AF_OnSendStatusNotify(id,TS_P2P_START_CONNECT);
	//	// p2pʧ�ܺ���Ҫ��ת������
	//	m_arrWaitNotify.push_back(id.fid);
	//}
	//}
	//else if(m_connStatus == CS_P2P)
	//{
	//	// ��ǰ�Ѿ���P2P���ӣ���ֱ��ʹ�ø�����
	//	// �ȴ��Է�����
	//	AF_OnSendStatusNotify(id,TS_P2P_WAIT_DOWNLOAD);
	//}
	//else if(m_connStatus == CS_TRANS)
	//{
	//	// ��ǰʹ����ת������������Ҫ����ת��������������
	//	
	//	XT_TRANS_LOGIN_REQ login;
	//	login.data = 0;
	//	m_arrSendConn.push_back(id.fid);
	//	g_pProtocol->C2TSendLogin(login);
	//	
	//	AF_OnSendStatusNotify(id,TS_TRANS_START_CONNECT);
	//}

}


void CAdvancedFileTransSession::AF_RecvDownloadFileAck(const char *data,WORD wRet)
{
	LPP2P_DOWNLOAD_FILE_ACK pAck = (LPP2P_DOWNLOAD_FILE_ACK)data;
	if(wRet != 0)
	{
		SendDataStruct * pSend = (SendDataStruct *)data;
		LPXT_HEAD  pHead = (LPXT_HEAD)pSend->data;
		//P2PID id(pHead->did,pSend->vlink);
		if(pHead->did != m_nFriendId)
		{
			return;
		}
		/*TRACE("�����û�%d���ļ�%dʧ��\n",id.uid,id.fid);
		AF_OnDownloadFailed(id,FTE_DOWNLOADFILE_TIMEOUT);
		AF_DeleteRecvFile(id);
		AF_AbortRecvFile(id);*/
		return;
	}

	 P2P_DOWNLOAD_FILE_ACK & ack = *pAck;

	// P2PID id(ack.pt.uid,ack.nFID);
	 if(ack.nFriendId != m_nFriendId)
	 {
		 return;
	 }

	 /*CP2PRecvFile * pRecvFile = NULL;

	 pRecvFile = AF_FindRecvFile(id);
	 if(pRecvFile == NULL)
	 {
		 TRACE("�����յ��ļ��ڽ���������в�����\n");
		 return;
	 }*/

	/* TRACE("���ͷ��Ѿ�׼���ã���������...\n");
	 TRACE("�������ļ���С%dbyte,%d����\n",pRecvFile->GetSize(),pRecvFile->GetPacket());*/

	 //int nPackets = pRecvFile->GetPacket();
	 //pRecvFile->m_nBegTime = GetTickCount();


	 //if(nPackets == 0)
	 //{
		// // ���ؽ��������ļ���
		// TRACE("���ؽ���!���ļ���\n");
		// AF_OnDownloadOK(id,10*1024*1024);
		// AF_DeleteRecvFile(id);
		// return;
	 //}

//	 // ��ʼ�����ļ�������
//	 P2P_DOWNLOAD_REQ req;
//	 req.pt = m_ptConn;
//	 req.fid = ack.nFID;
//	 req.packetID = pRecvFile->GetNextTask(); 
//	 // ��ʼ������
//	 pRecvFile->AddReq();
//
//#ifdef _TEMP_DEBUG
//	 // ��ʱ����
//	 // �ȴ�A֪ͨ��¼��ת������
//	 AF_OnRecvStatusNotify(id,TS_TRANS_WAIT);
//	 // ��ǰʹ����ת������������Ҫ����ת��������������
//	 m_arrDownload.push_back(req);
//	 goto SET_TIMER;
//	 return;
//#endif
//
//	bool bCanP2P = (m_connStatus == CS_P2P) && ( AF_HasSendingFile() || AF_HasDownloadingFile());
//	if(bCanP2P)
//	{
//		// ��ǰ�Ѿ���P2P���ӣ���ֱ��ʹ�ø�����
//		AF_OnRecvStatusNotify(id,TS_P2P_START_DOWNLOAD);
//		// ��ʼp2p����
//		g_pProtocol->SendDownloadReq(req,pRecvFile->GetRecvWnd());
//	}
//	else
//	{
//		if(m_step == P2PCS_END || m_step == P2PCS_GETIPFAILED)
//			m_step = P2PCS_BEG;
//		
//		// ��ǰû����Ч��p2p���ӣ���ʼ����p2p����
//		if(m_step  == P2PCS_BEG)
//		{
//			m_step = P2PCS_GETIP;
//			// ��ȡ��ǰ���ѵ�ip�Ͷ˿�
//			XT_STATUS_GET_REQ reqStatus;
//			reqStatus.fid = m_ptConn.uid;
//			reqStatus.id = g_ImUserInfo.GetId();
//			g_pProtocol->SendReq(reqStatus);
//		}
//		// ��ʼ׼������p2p����
//		AF_OnRecvStatusNotify(id,TS_P2P_START_CONNECT);
//		// p2p���ӳɹ�����Ҫ���͵�����
//		m_arrDownload.push_back(req);
//	}
	
	 //}
	 //else if(m_connStatus == CS_P2P)
	 //{
		// // ��ʼp2p����
		// AF_OnRecvStatusNotify(id,TS_P2P_START_DOWNLOAD);
		// // ��ǰ�Ѿ���P2P���ӣ���ֱ��ʹ�ø�����
		// g_pProtocol->SendDownloadReq(req,pRecvFile->GetRecvWnd());
	 //}

#ifdef _TEMP_DEBUG
SET_TIMER: 
#endif

	
	
	//AF_SetMaxSpeed(m_nMaxSpeed);
}


//
//void CAdvancedFileTransSession::AF_RecvDownloadReq(const char *data,WORD wRet)
//{
//	LPP2P_DOWNLOAD_REQ pReq = (LPP2P_DOWNLOAD_REQ)data;
//	if(wRet != 0)
//	{
//		return;
//	}
//	P2P_DOWNLOAD_REQ  & req = *pReq;
//	if(req.pt.uid != m_ptConn.uid)
//		return;
//
//	CMyLock lock(m_scSendPool.Get());
//	P2P_DOWNLOAD_ACK ack;
//	ack.fid = req.fid;
//	ack.packetID = req.packetID;
//	ack.pt = req.pt;
//	ack.seq = req.seq;
//
//	
//
//	P2PID id(req.pt.uid,req.fid);
//	CP2PSendFile * pSendFile = AF_FindSendFile(id);
//	if(pSendFile == NULL)
//	{
//		TRACE("err,û���ҵ���������ָ�����ļ�\n");
//		ack.nRt = P2P_REQFILE_NOTEXIST;
//		ack.fid = req.fid;
//		ack.packetID = req.packetID;
//		ack.len = 0;
//		// �������ݸ�������
//		g_pProtocol->SendDownloadAck(ack);
//		return ;
//	}
//
//	P2PTRAN_ERR err;
//	err = pSendFile->GetData(req.packetID,ack.data,ack.len);
//	if( err == P2P_OK)
//	{
//		ack.nRt = 0;
//	}
//	else
//	{
//		ack.len = 0;
//		ack.nRt = err;
//	}
//	pSendFile->SetSending();
//	pSendFile->AddTimerSend(ack.len);
//	// �������ݸ�������
//	g_pProtocol->SendDownloadAck(ack);
//}


//void CAdvancedFileTransSession::AF_RecvDownloadAck(const char *data,WORD wRet)
//{
//	CMyLock lock(m_scRecvPool.Get());
//	LPP2P_DOWNLOAD_ACK pAck = (LPP2P_DOWNLOAD_ACK)data;
//	
//	if(wRet != 0)
//	{
//		SendDataStruct * pSend = (SendDataStruct *)data;
//		LPXT_HEAD  pHead = (LPXT_HEAD)pSend->data;
//		P2PID id(pHead->did,pSend->vlink);
//		if(id.uid != m_ptConn.uid)
//		{
//			return;
//		}
//
//		TRACE("�����û�%d���ļ�%d��ĳ��ʧ��\n",id.uid,id.fid);
//		CP2PRecvFile *pFile = AF_FindRecvFile(id);
//		if(pFile)
//		{
//#ifdef _DEBUG
//			TRACE("�ش�����%d\n",pFile->m_nResend);
//#endif
//			AF_OnDownloadFailed(id,FTE_DOWNLOADDATA_TIMEOUT);
//			AF_DeleteRecvFile(id);
//			AF_AbortSendFile(id);
//		}
//		return;
//	}
//
//	P2PID id(pAck->pt.uid,pAck->fid);
//	if(pAck->pt.uid != m_ptConn.uid)
//	{
//		return;
//	}
//	if(pAck->nRt != 0)
//	{
//		if(pAck->nRt == P2P_REQFILE_NOTEXIST)
//		{
//			TRACE("������ļ�%d�Ѿ������ڣ�\n",pAck->fid);
//			AF_OnDownloadFailed(id,FTE_DOWNLOADFILE_NOTEXIST);
//		}
//		else
//		{
//			TRACE("��ȡ%d���ݰ�%d����ʧ��\n",pAck->fid,pAck->packetID);
//			AF_OnDownloadFailed(id,FTE_DOWNLOADDATA_FAILED);
//			
//		}
//		AF_DeleteRecvFile(id);
//		return;
//	}
//	TRACE("ack pack=%d\n",pAck->packetID);
//	m_nCanSend -= pAck->len;
//
//	CP2PRecvFile * pRecvFile = AF_FindRecvFile(id);
//	if(pRecvFile == NULL)
//	{
//		TRACE("���յ��������޷��ҵ���Ӧ�ļ�%d\n",id.fid);
//		return;
//	}
//
//	CP2PRecvWnd * pRecvWnd = pRecvFile->GetRecvWnd();
//
//	P2P_DOWNLOAD_REQ req;
//	req.fid = pAck->fid;
//	req.pt = pAck->pt;
//
//	P2PTRAN_ERR err;
//	int nResend = 0;
//	// ���õ�ǰ�������ص�״̬
//	pRecvFile->SetDownloading();
//	err = pRecvFile->RecvData(*pAck,nResend) ;
//	if(err !=  P2P_OK)
//	{
//		// �жϿ����ش�
//		if(err == P2P_NEED_QUICK_RESEND && nResend >= 0 )
//		{
//			TRACE("quick resend %d\n",nResend);
//			// ��Ҫ�����ش�
//			//pRecvFile->OnCongestion();
//			int seq;
//			if(pRecvWnd->GetItemSeq(nResend,seq))
//			{
//				g_nSend ++;
//				g_nQResend ++;
//				req.seq = seq;
//				req.packetID = nResend;
//				g_pProtocol->QuickResendDownloadReq(req,pRecvWnd);
//			}
//			else
//			{
//				ASSERT(0);
//			}
//			return ;
//		}
//		else if(err == P2P_PACKET_EXIST)
//		{
//			TRACE("!!repeat ack!!");
//		}
//		else
//		{
//			TRACE("!!err ack!!\n");
//		}
//	
//	}
//
//	if(pRecvFile->HasTask() == false)
//	{
//		if(pRecvFile->IsFinish())
//		{
//
//			pRecvFile->m_nEndTime = GetTickCount();
//			pRecvFile->Flush();
//			pRecvFile->Close();
//
//			//
//			// ֪ͨ���ͷ��Ѿ��������
//			//
//
//			P2P_FIN_TRAN_REQ req;
//			req.fid = pAck->fid;
//			req.pt = pAck->pt;
//			g_pProtocol->SendFinishTranReq(req);
//			if(m_poolRecv.size() == 0 && m_poolSend.size() == 0)
//			{
//				if ( m_mrTimer )
//				{
//					timeKillEvent(m_mrTimer);
//					m_mrTimer=0;
//				}
//			}
//			int nTime = (pRecvFile->m_nEndTime - pRecvFile->m_nBegTime);
//			double dbSpeed = 0;
//			double dbSize = (pRecvFile->GetSize() - pRecvFile->GetStartPos());
//			if(nTime)
//				dbSpeed =  dbSize * 1000.0 / nTime;
//			else
//				dbSpeed = 10*1024*1024;
//
//			CP2PRecvFile *pFile = AF_FindRecvFile(id);
//
//			CString strRecvFile="";
//
//			if ( pFile )
//			{
//				strRecvFile = pFile->GetFilePath();
//			}
//
//			CString strDestFile;
//
//			if ( strRecvFile!="" )
//			{
//				if ( FileExist(strRecvFile) )
//				{
//					CString strPath,strName,strExt;
//					ExtractFilePath(strRecvFile,strPath,strName,strExt);
//
//					for ( int i=0; i<1000; i++ )
//					{
//						strDestFile = strPath+strName+"("+IntToStr(i+1)+")"+strExt;
//						
//						if ( FileExist(strDestFile)==FALSE )
//						{
//							break;
//						}
//					}
//				}
//				else
//				{
//					strDestFile = strRecvFile;
//				}
//			}
//
//			pFile->m_sPath = strDestFile;
//
//			AF_OnDownloadOK(id,dbSpeed);
//			AF_DeleteRecvFile(id);
//
//			rename((char *)(LPCTSTR)(strRecvFile+".~tmp"),(LPCTSTR)strDestFile);
//		}
//		return;
//	}
//
//	int nPacketID = 0;
//	if(m_nCanSend <= 0)
//	{
//		nPacketID = pRecvFile->GetCurTask();
//		if(pRecvWnd->IsInWnd(nPacketID) == false)
//		{
//			TRACE("�ȴ����ڻ���\n");
//			return;
//		}
//		req.packetID = pRecvFile->GetNextTask();
//		if((int)req.packetID < 0)
//			return;
//		TRACE("!!!��������!!!\n");
//		pRecvFile->AddReq();
//		m_arrWait.push_back(req);
//	}
//	else
//	{
//		int nCReq = pRecvFile->m_nCReq;
//		int nReq = pRecvFile->m_fReq;
//		while(nCReq < nReq)
//		{
//			nPacketID = pRecvFile->GetCurTask();
//			if(pRecvWnd->IsInWnd(nPacketID) == false)
//			{
//				TRACE("�ȴ����ڻ���\n");
//				return;
//			}
//			req.packetID = pRecvFile->GetNextTask();
//			if((int)req.packetID < 0)
//				break;
//			g_nSend++;
//			pRecvFile->AddReq();
//			nCReq++;
//			g_pProtocol->SendDownloadReq(req,pRecvWnd);
//			Sleep(0);
//		}	
//	}
//
//}	


void CAdvancedFileTransSession::AF_RecvFinTranReq(const char *data,WORD wRet)
{
	LPP2P_FIN_TRAN_REQ pReq = (LPP2P_FIN_TRAN_REQ)data;
	if(wRet != 0)
	{
		return;
	}
	AF_OnRecvFinTranReq(*pReq);
}

void CAdvancedFileTransSession::AF_OnRecvFinTranReq(const P2P_FIN_TRAN_REQ & req)
{
	//CMyLock lock(m_scSendPool.Get());
	//P2PID id(req.pt.uid,req.fid);
	
	if(req.friendId != m_nFriendId)
	{
		return;
	}

	CP2PSendFile * pSendFile = AF_FindSendFile(req.fid);
	if(pSendFile == NULL)
	{
		TRACE("�յ�һ��δ֪���ļ����ؽ���֪ͨ\n");
		return;
	}
	pSendFile->m_nEndTime = GetTickCount();
	int nTime = (pSendFile->m_nEndTime - pSendFile->m_nBegTime);
	double dbSpeed;
	double dbSize = (pSendFile->GetSize() - pSendFile->GetStartPosition());
	if(nTime)
		dbSpeed = dbSize * (1000.0 / nTime);
	else
		dbSpeed = 10*1024*1024;
	AF_OnSendOK(req.fid);
	AF_DeleteSendFile(req.fid);
}

////------------------------------------------------------
//// B��¼��ת������Ӧ��
////------------------------------------------------------
//void CAdvancedFileTransSession::AF_RecvBLoginTransAck(const char *data,WORD wRet)
//{
//	if(wRet != 0)
//	{
//		TRACE("B��¼/�ǳ���ת������ʧ��\n");
//		return;
//	}
//	TRACE("B��¼/�ǳ���ת�������ɹ�\n");
//	LPXT_TRANS_RECVLOGIN_ACK pAck = (LPXT_TRANS_RECVLOGIN_ACK)data;
//	if(pAck->action == 1)
//	{
//		TRACE("��ת���������ӳɹ�-��ʼ����....\n");
//		// ��¼��ת�������ɹ����������������������ļ�������
//		m_connStatus = CS_TRANS;
//		m_ptConn.type = P2P_ENDPOINT::TRANS;
//		m_ptConn.ip = pAck->ip;
//		m_ptConn.port = pAck->port;
//		
//		// ���������ļ���������
//		int i;
//		int c = (int)m_arrDownload.size();
//		P2PID id;
//		CP2PRecvFile * pRecvFile;
//		for(i = 0; i < c; i++)
//		{
//			P2P_DOWNLOAD_REQ & req = m_arrDownload[i];
//			req.pt = m_ptConn;
//			id.Set(req.pt.uid,req.fid);
//			pRecvFile = AF_FindRecvFile(id);
//			if(pRecvFile)
//			{
//				// ��ʼͨ����ת�����������ļ�����
//				AF_OnRecvStatusNotify(id,TS_TRANS_START_DOWNLOAD);
//				g_pProtocol->SendDownloadReq(req,pRecvFile->GetRecvWnd());
//			}
//			else
//			{
//				TRACE("err!û���ҵ������ļ�����\n");
//			}
//		}
//		m_arrDownload.clear();
//	}
//}

////------------------------------------------------------
//// �յ�����A�������¼��ת����������
////------------------------------------------------------
//void CAdvancedFileTransSession::AF_RecvNotifyLoginReq(const char *data,WORD wRet)
//{
//	TRACE("B�յ�A��֪ͨ��¼��ת������\n");
//	// �Ѿ��ظ�ack
//	LPXT_TRANS_NOTIFY_LOGIN_REQ pReq = (LPXT_TRANS_NOTIFY_LOGIN_REQ)data;
//
//	if(pReq->sid != m_ptConn.uid)
//	{
//		return;
//	}
//	
//	// ���̵�¼��ת������
//	XT_TRANS_RECVLOGIN_REQ req;
//	req.action = 1;
//	req.sessionid = pReq->sessionID;
//	req.ip = pReq->transSrvIP;
//	req.port = pReq->port;
//	req.uid = m_ptConn.uid;	// ������A��id
//	memcpy(req.sessioncode,pReq->check,16);
//	
//	g_pProtocol->C2TSendRecvLogin(req);
//	
//}

////------------------------------------------------------
//// A��¼��ת������Ӧ��
////------------------------------------------------------
//void CAdvancedFileTransSession::AF_RecvALoginTransAck(const char *data,WORD wRet)
//{
//	if(wRet != 0)
//	{
//		P2PID id;
//		for(int i = 0;i < m_arrSendConn.size();i++)
//		{
//			id.Set(m_ptConn.uid,m_arrSendConn[i]);
//			AF_OnSendFailed(id,FTE_CONNECT_TRANS_FAILED);
//			AF_DeleteSendFile(id);
//			AF_AbortSendFile(id);
//		}
//		m_arrSendConn.clear();
//		TRACE("A��¼��ת��������ʱʧ��\n");
//		//::MessgeBox(NULL,"A��¼��ת��������ʱʧ��");
//		return;
//	}
//	m_arrSendConn.clear();
//	LPXT_TRANS_LOGIN_ACK pAck = (LPXT_TRANS_LOGIN_ACK)data;
//
//	TRACE("A֪ͨBҲ��¼��ת������\n");
//
//	// ����֪ͨBҲ��¼��ת������
//	XT_TRANS_NOTIFY_LOGIN_REQ req;
//	req.linkType = 0;	// udp
//	req.sessionID = pAck->sessionid;
//	req.transSrvIP = pAck->ip;
//	req.port = pAck->port;
//	req.uid = m_ptConn.uid;
//	
//
//	g_pProtocol->P2SSendNotifyLogin(req);
//
//}

//------------------------------------------------------
// �յ�p2p���԰�
//------------------------------------------------------
//void CAdvancedFileTransSession::AF_RecvP2PTestReq(const char *data,WORD wRet)
//{
//
//	LPXT_P2PTEST_IND  pReq = (LPXT_P2PTEST_IND)data;
//	if(pReq->id != m_ptConn.uid)
//		return;
//	
//	if(m_step == P2PCS_P2PTEST2)
//	{
//		TRACE("�յ��Է�����p2p���԰�,p2p�����ɹ�\n");
//		//
//		// p2p���ӳɹ�
//		//
//
//		// ���Ӳ��Խ���
//		m_step = P2PCS_END;
//
//		// A����Ҫ֪ͨB��¼��ת������
//		m_arrWaitNotify.clear();
//		m_arrSendConn.clear();
//
//		m_connStatus = CS_P2P;
//
//		m_ptConn.type = P2P_ENDPOINT::P2P;
//		m_ptConn.ip = pReq->ip;
//		m_ptConn.port = pReq->port;
//
//		// ���շ���ʼ���������ļ�����
//
//		int i;
//		int c = (int)m_arrDownload.size();
//		P2PID id;
//		CP2PRecvFile * pRecvFile;
//		for(i = 0; i < c; i++)
//		{
//			P2P_DOWNLOAD_REQ & req = m_arrDownload[i];
//			req.pt = m_ptConn;
//			id.Set(req.pt.uid,req.fid);
//			pRecvFile = AF_FindRecvFile(id);
//			if(pRecvFile)
//				g_pProtocol->SendDownloadReq(req,pRecvFile->GetRecvWnd());	
//		}
//		m_arrDownload.clear();
//	}
//	else
//	{
//		if(m_step != P2PCS_END)
//		{
//			TRACE("�յ��Է�p2p���԰�,p2p��ʼ�������ȴ���������\n");
//			m_step = P2PCS_P2PTEST2;
//		}
//		else
//		{
//			TRACE("�յ����õ�p2p���԰�\n");
//		}
//	}
//}
//
////------------------------------------------------------
//// �յ�͸����תӦ����ͳ�ʱ��͸����ת����
////------------------------------------------------------
//void CAdvancedFileTransSession::AF_RecvTransDataAck(const char * data,WORD wRet)
//{
//	if(wRet != 0)
//	{
//		// ͸����ת��ʱ
//		SendDataStruct * pSendData = (SendDataStruct*)data;
//		uint16 cmd;
//		const char * pSubData = pSendData->data +sizeof(XT_HEAD);
//		cmd = *((uint16*)(pSubData));
//		pSubData += 2;
//		SendDataStruct subData;
//		subData.data_len = pSendData->data_len - sizeof(XT_HEAD) - 2;
//		memcpy(subData.data,pSubData,subData.data_len);
//		subData.dest_ip = pSendData->dest_ip;
//		subData.port = pSendData->port;
//		subData.vlink = pSendData->vlink;
//		g_pProtocol->SendNotify(MAKEWPARAM(wRet,cmd+1),(LPARAM)&subData);
//		return;
//	}
//}

void CAdvancedFileTransSession::AF_SetRecvFilePath(int nFileID, CString strPath)
{
	map<int,CString>::iterator it = m_mapRecvFilePath.find(nFileID);
	if ( it==m_mapRecvFilePath.end() )
	{
		m_mapRecvFilePath.insert(map<int,CString>::value_type(nFileID,strPath));
	}
	else
	{
		it->second = strPath;
	}
}

void CAdvancedFileTransSession::AF_RecvFinTranAck(const char *data,WORD wRet)
{
	if(wRet != 0)
	{
		return;
	}
}

void CAdvancedFileTransSession::AF_RecvAbortTranReq(const char *data,WORD wRet)
{
	LPP2P_ABORT_TRAN_REQ pReq = (LPP2P_ABORT_TRAN_REQ )data;
	if(wRet != 0)
	{
		return;
	}
	
	if(pReq->friendId != m_nFriendId)
	{
		return;
	}
	
	AF_OnRecvAbortTranReq(*pReq);

	if(pReq->type == P2P_ABORT_TRAN_REQ::STOP_RECV)
	{
		AF_DeleteSendFile(pReq->fid);
	}
	else if(pReq->type == P2P_ABORT_TRAN_REQ::STOP_SEND)
	{
		AF_DeleteRecvFile(pReq->fid);
	}
}



void CAdvancedFileTransSession::AF_RecvAbortTranAck(const char *data,WORD wRet)
{
	LPP2P_ABORT_TRAN_ACK pAck = (LPP2P_ABORT_TRAN_ACK )data;
	if(wRet != 0)
	{
		return;
	}

}


void	CAdvancedFileTransSession::AF_SetInfo(uint32 nMyId,uint32 nFriendId,uint32 destIP,uint16 destPort,uint32 myTranIp,uint16 myTranPort,CUdpNetTrans * pTran)
{
	m_pDataTran = pTran;
	m_nMyId = nMyId;
	m_nFriendId = nFriendId;
	m_destIp = destIP;
	m_destPort = destPort;
	m_uMyTranIP = myTranIp;
	m_uMyTranPort = myTranPort;
};

CAdvancedFileTransSession::SENDFILE_RESULT 
CAdvancedFileTransSession::AF_SendFile(const CString & sFullPath, long nFileID)
{
	CFileStatus st;
	if(CFile::GetStatus(sFullPath,st) == false)
	{
		return SR_GET_FILE_INFO_FAILED;
	}

	if ( st.m_attribute == 0x10 )
	{
		return SR_DIRECTORY;
	}

	P2P_SENDFILE_REQ req;

	req.fid = nFileID;
	req.size.size64 = st.m_size;
	req.time = st.m_mtime.GetTime();
	req.tranIp = m_uMyTranIP;
	req.tranPort = m_uMyTranPort;
	req.friendId = m_nFriendId;

	CString str1,str2,str3;
	::ExtractFilePath(sFullPath,str1,str2,str3);

	CString strFileName = str2+str3;

	strncpy(req.name,strFileName,sizeof(req.name));
	//req.pt = m_ptConn;
	
	if(st.m_size == 0)
	{
		TRACE("�����͵��ļ���СΪ0\n");
		return SR_SIZE_ZERO;
	}

	//CMyLock lock(m_scSendPool.Get());

	CP2PSendFile * pSendFile = NULL;
	pSendFile = AF_FindSendFile(nFileID);
	if(pSendFile)
	{
		TRACE("�����͵��ļ�id������������������ͻ\n");
		return SR_ID_REPEAT;
	}

	pSendFile = new CP2PSendFile();
	// ��ʱ�ְ���С
	if(pSendFile->Create(sFullPath,nFileID,req.time) != P2P_OK)
	{
		TRACE("�ļ���ʧ��,ֹͣ�����ļ�!\n");
		return SR_OPEN_FAILED;
	};

	// ��ӷ�������
	m_poolSend[nFileID] = pSendFile;

	// �����ļ�����
	g_pProtocol->SendSendFileReq(req);

	return SR_OK;
}

void CAdvancedFileTransSession::AF_OnGetStatusFailed()
{
	


}

//void CAdvancedFileTransSession::AF_OnGetStatusOK(const PEER_ENDPOINT & pt)
//{
//	if(m_step != P2PCS_GETIP)
//		return;
//	m_step = P2PCS_P2PTEST;
//
//	// ����״̬��ȡ�ɹ���ʼ����p2p����
//	
//	XT_P2PTEST_REQ req;
//	// �������ip��ͬ���������ͬһ��������������������
//	if(pt.ip == g_ImUserInfo.ip)
//	{
//		req.dest_ip = pt.localIp;
//		req.dest_port = pt.localPort;
//		req.ipType = XT_P2PTEST_REQ::LAN;
//	}
//	else
//	{
//		req.dest_ip = pt.ip;
//		req.dest_port = pt.port;
//		req.ipType = XT_P2PTEST_REQ::P2P;	
//	}
//	req.id = g_ImUserInfo.GetId();
//	req.toid = m_ptConn.uid;
//	// ����p2p���԰�,��ʼ���Ӳ���
//	g_pProtocol->SendP2pTestPacket(req);
//	
//}


//void CAdvancedFileTransSession::AF_OnP2PTestFailed()
//{
//	if(m_step != P2PCS_P2PTEST && m_step != P2PCS_P2PTEST2)
//		return;
//	m_step = P2PCS_END;
//	
//	TRACE("p2p����ʧ��\n");
//	m_connStatus = CS_TRANS;
//	
//	// p2p����ʧ�ܺ�,ͨ��ҵ���������ת
//	// ���շ�B�ȴ�A֪ͨ��¼��ת������
//	// ���ͷ�A�ж��Ƿ���Ҫ��¼��ת������
//	XT_TRANS_LOGIN_REQ login;
//	login.data = 0;
//	P2PID id;
//	id.uid = m_ptConn.uid;
//	int i;
//	int bNeedConnTrans = false;
//	int c = m_arrWaitNotify.size();
//	for(i = 0 ;i < c;i++)
//	{
//		id.fid = m_arrWaitNotify[i];
//		if(AF_CanTrans(id))
//		{
//			// ��ӵ�ǰ��Ҫ���͵��ļ��б��Ա��ڵ�¼ʧ��ʱ�����ļ�ʧ�ܵĴ���
//			m_arrSendConn.push_back(id.fid);
//			bNeedConnTrans = true;
//		}
//		else
//		{
//			AF_OnSendFailed(id,FTE_P2P_CONN_FAILED);
//			AF_AbortSendFile(id);
//		}
//	}
//	if(bNeedConnTrans)
//		g_pProtocol->C2TSendLogin(login);
//}

void CAdvancedFileTransSession::AF_ConnectTrans()
{
	
}

//void CAdvancedFileTransSession::AF_OnP2PTestOK(XT_P2PTEST_ACK * pAck)
//{
//	if(m_step != P2PCS_P2PTEST && m_step != P2PCS_P2PTEST2)
//		return;
//
//	TRACE("p2p�����ɹ�!\n");
//	// A����Ҫ֪ͨB��¼��ת������
//	m_arrWaitNotify.clear();
//	m_arrSendConn.clear();
//
//	// p2p���ӽ����ɹ�
//	m_connStatus = CS_P2P;
//	m_ptConn.type = P2P_ENDPOINT::P2P;
//	m_ptConn.ip = pAck->ip;
//	m_ptConn.port = pAck->port;
//
//	// p2p�������,״̬��Ϊ����
//	m_step = P2PCS_END;
//
//	// ���������ļ���������
//	int i;
//	int c = (int)m_arrDownload.size();
//	P2PID id;
//	CP2PRecvFile * pRecvFile;
//	for(i = 0; i < c; i++)
//	{
//		P2P_DOWNLOAD_REQ & req = m_arrDownload[i];
//		req.pt = m_ptConn;
//		id.Set(req.pt.uid,req.fid);
//		pRecvFile = AF_FindRecvFile(id);
//		if(pRecvFile)
//		{
//			AF_OnRecvStatusNotify(id,TS_P2P_START_DOWNLOAD);
//			TRACE("B��ʼ�����ļ�%d\n",id.fid);
//			g_pProtocol->SendDownloadReq(req,pRecvFile->GetRecvWnd());	
//		}
//	}
//	m_arrDownload.clear();
//
//	TRACE("p2p������ʼ��������\n");
//	XT_P2PTEST_REQ req;
//	req.dest_ip = pAck->ip;
//	req.dest_port = pAck->port;
//	req.ipType = pAck->testType;
//	req.id = g_ImUserInfo.GetId();
//	req.toid = m_ptConn.uid;
//	// ����p2p���԰�,��ʼ�ڶ�������
//	g_pProtocol->SendP2pTestPacket(req);
//	
//}
 
 
 



void CAdvancedFileTransSession::AF_RegistXtCmd(HWND hWnd)
{
	
	g_pProtocol->RegistCmd(CMD_SEND_FILE_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_SEND_FILE_ACK,hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_FILE_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_FILE_ACK,hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_REQ	,hWnd);
	g_pProtocol->RegistCmd(CMD_DOWNLOAD_ACK	,hWnd);
	g_pProtocol->RegistCmd(CMD_FIN_TRAN_REQ	,hWnd);
	g_pProtocol->RegistCmd(CMD_FIN_TRAN_ACK	,hWnd);
	g_pProtocol->RegistCmd(CMD_ABORT_TRAN_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_ABORT_TRAN_ACK,hWnd);
	g_pProtocol->RegistResendCmd(CMD_DOWNLOAD_REQ,hWnd);

	g_pProtocol->RegistCmd(CMD_P2P_NOTIFY_LOGIN_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_TRANS_LOGIN_ACK,hWnd);
	g_pProtocol->RegistCmd(CMD_TRANS_RECVLOGIN_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_TRANS_RECVLOGIN_ACK,hWnd);
	g_pProtocol->RegistCmd(CMD_P2P_TEST_REQ,hWnd);
	g_pProtocol->RegistCmd(CMD_ROUTERUSERACKPACKET_ACK,hWnd);	

	g_pProtocol->RegistCmd(CMD_FILE_TRANS_REQ,hWnd);	  ////������Ϣ
	g_pProtocol->RegistCmd(CMD_FILE_TRANS_IND,hWnd);	  ////�ͷ�����ͨ��	
	g_pProtocol->RegistCmd(CMD_FILE_TRANS_NOTIFY_ACK,hWnd);	  ////�ļ����俪ʼʱͨ����Ϣȷ��	
}

void CAdvancedFileTransSession::AF_AbortAllRecvFiles(uint32 senderID)
{
	//CMyLock lock(m_scRecvPool.Get());

	P2PRecvPool::iterator it;

	for ( it=m_poolRecv.begin(); it!=m_poolRecv.end(); it++ )
	{
		uint32 fileId = it->first;
		// ������ֹ���������

		P2P_ABORT_TRAN_REQ req;
		req.fid = fileId;
		req.friendId = m_nFriendId;
		//req.pt =  it->second->GetPeer();
		req.type = P2P_ABORT_TRAN_REQ::STOP_RECV;
		g_pProtocol->SendAbortTranReq(req);

		it->second->SaveFilePosition(senderID);
		it->second->Stop();
		delete it->second;
	}

	m_poolRecv.clear();
}

void CAdvancedFileTransSession::AF_AbortRecvFile(uint32 id)
{
	CP2PRecvFile * pRecvFile = AF_FindRecvFile(id);
	if(pRecvFile == NULL)
		return;

	// ������ֹ���������
	
	P2P_ABORT_TRAN_REQ req;
	req.fid = id;
	req.friendId = m_nFriendId;
	req.type = P2P_ABORT_TRAN_REQ::STOP_RECV;
	g_pProtocol->SendAbortTranReq(req);

	AF_DeleteRecvFile(id);

	if(m_poolRecv.size() == 0)
	{
		/*if ( m_mrTimer )
			timeKillEvent(m_mrTimer);
		m_mrTimer=0;*/
	}
}

void CAdvancedFileTransSession::AF_AbortAllSendFiles()
{
	//CMyLock lock(m_scSendPool.Get());

	P2PSendPool::iterator it;
	
	for ( it=m_poolSend.begin(); it!=m_poolSend.end(); it++ )
	{
		uint32 fileId = it->first;

		P2P_ABORT_TRAN_REQ req;
		req.fid = fileId;
		req.friendId = m_nFriendId;//pSendFile->GetPeer();
		req.type = P2P_ABORT_TRAN_REQ::STOP_SEND;
		g_pProtocol->SendAbortTranReq(req);

		delete it->second;
	}

	m_poolSend.clear();
}

void CAdvancedFileTransSession::AF_AbortSendFile(uint32 id)
{
	//CMyLock lock(m_scSendPool.Get());
	CP2PSendFile * pSendFile = AF_FindSendFile(id);
	if(pSendFile == NULL)
		return;

	// ������ֹ���������

	P2P_ABORT_TRAN_REQ req;
	req.fid = id;
	req.friendId = m_nFriendId;
	req.type = P2P_ABORT_TRAN_REQ::STOP_SEND;
	g_pProtocol->SendAbortTranReq(req);

	AF_DeleteSendFile(id);
}

CP2PRecvFile *	CAdvancedFileTransSession::AF_FindRecvFile(uint32  id)
{
	//CMyLock lock(m_scRecvPool.Get());
	P2PRecvPool::iterator it;
	it = m_poolRecv.find(id);
	if(it != m_poolRecv.end())
		return it->second;
	else
		return NULL;
};

void	CAdvancedFileTransSession::AF_DeleteRecvFile(uint32 id)
{
	//CMyLock lock(m_scRecvPool.Get());
	P2PRecvPool::iterator it;
	it = m_poolRecv.find(id);
	if(it != m_poolRecv.end())
	{
		delete it->second;
		m_poolRecv.erase(it);
	}
}


CP2PSendFile *	CAdvancedFileTransSession::AF_FindSendFile(uint32  id)
{
	//CMyLock lock(m_scSendPool.Get());
	P2PSendPool::iterator it;
	it = m_poolSend.find(id);
	if(it != m_poolSend.end())
		return it->second;
	else
		return NULL;
};

void	CAdvancedFileTransSession::AF_DeleteSendFile(uint32 id)
{
	//CMyLock lock(m_scSendPool.Get());
	P2PSendPool::iterator it;
	it = m_poolSend.find(id);
	if(it != m_poolSend.end())
	{
		delete it->second;
		m_poolSend.erase(it);
	}
}




bool CAdvancedFileTransSession::AF_HasDownloadingFile()
{
	//CMyLock lock(m_scRecvPool.Get());
	if(m_poolRecv.size() == 0)
		return false;
	P2PRecvPool::const_iterator it,itE;
	itE = m_poolRecv.end();
	for(it = m_poolRecv.begin(); it != itE; it++)
	{
		if(it->second->IsDownloading())
			return true;
	}
	return false;
}

bool CAdvancedFileTransSession::AF_HasSendingFile()
{
	//CMyLock lock(m_scSendPool.Get());
	if(m_poolSend.size() == 0)
		return false;
	P2PSendPool::const_iterator it,itE;
	itE = m_poolSend.end();
	for(it = m_poolSend.begin(); it != itE; it++)
	{
		if(it->second->IsSending())
			return true;
	}
	return false;
}



void CAdvancedFileTransSession::AF_NotifyRecvOk(uint32 fid)
{
	//
	// ֪ͨ���ͷ��Ѿ��������
	//

	P2P_FIN_TRAN_REQ req;
	req.fid = fid;
	req.friendId = m_nFriendId;
	g_pProtocol->SendFinishTranReq(req);


	CP2PRecvFile *pFile = AF_FindRecvFile(fid);

	CString strRecvFile="";

	if ( pFile )
	{
		strRecvFile = pFile->GetFilePath();
	}

	CString strDestFile;

	if ( strRecvFile!="" )
	{
		if ( FileExist(strRecvFile) )
		{
			CString strPath,strName,strExt;
			ExtractFilePath(strRecvFile,strPath,strName,strExt);

			for ( int i=0; i<1000; i++ )
			{
				strDestFile = strPath+strName+"("+IntToStr(i+1)+")"+strExt;

				if ( FileExist(strDestFile)==FALSE )
				{
					break;
				}
			}
		}
		else
		{
			strDestFile = strRecvFile;
		}
	}

	pFile->m_sPath = strDestFile;
	rename((char *)(LPCTSTR)(strRecvFile+".~tmp"),(LPCTSTR)strDestFile);	
	AF_OnDownloadOK(fid);
	AF_DeleteRecvFile(fid);
}

////A��B���ļ����ɷ����ת����B����Ӧ
void  CAdvancedFileTransSession::AF_OnRecvFileAck(const char *data,WORD wRet)
{
	//////////////////////////////////////////////////////////////////
	if ( wRet == 0)
	{
		LPSEND_FILE_PACK_ACK pAck = (LPSEND_FILE_PACK_ACK)data;
		if ( pAck)
		{
			if(pAck->flag == 0x02)////������ļ�����
			{
				CP2PRecvFile *pFile = AF_FindRecvFile(pAck->file_id);
				if(pFile != NULL)
				{
					pFile->RecvPacket(pAck->pack_id, pAck->buf, pAck->size);
				}
			}
			else if(pAck->flag == 0x03)////������ļ���β
			{
				CP2PRecvFile *pFile = AF_FindRecvFile(pAck->file_id);
				pFile->Close();
				pFile->m_pThis->AF_NotifyRecvOk(pAck->file_id);
			}
		}
	}
}

// ������յ�Ӧ��
void  CAdvancedFileTransSession::AF_OnSenderRecvAck(const char *data,WORD wRet)
{
	if ( wRet == 0)////B��Aת���ɹ�
	{
		LPSEND_FILE_PACK_IND pAck = (LPSEND_FILE_PACK_IND)data;
		CP2PSendFile *pSendFile = AF_FindSendFile(pAck->file_id);
		if(pSendFile != NULL)
		{
			pSendFile->RecvArriveAck(pAck->pack_id);
		}
	}
}

// �ļ����俪ʼʱ���ͻ��˷��͸������ͨ����Ϣ���յ�Ӧ��
void CAdvancedFileTransSession::AF_OnRecvTranFileNotifyAck(const char *data,WORD wRet)
{
	m_bRecvFileTranNotifyAck = TRUE;
}