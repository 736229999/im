#include "StdAfx.h"
#include ".\p2pfile.h"
#include <assert.h>
#include "AdvancedFileTransSession.h"

//const char * CP2PRecvFile::GetFileName(const char * szPath)
//{
//	size_t nLen = strlen(szPath);
//	if(nLen < 2)
//		return szPath;
//	const char * pB = szPath;
//	const char * p = pB + nLen;
//	while((--p) >= pB)
//	{
//		if(*p == '\\' || *p == '/')
//		{
//			return p + 1;
//		}
//	}
//	return szPath;
//}


CP2PRecvFile::CP2PRecvFile(void)
{
	m_id = 0;
	m_nSize = 0;
	m_nCurRecvSize = 0;
	m_nRecvSize = 0;

	m_bWork = false;
	m_bOpen = false;
	m_pFile = NULL;
	m_hThread = NULL;
	m_pDataTran = NULL;


	m_pDataBuf = new char[RECV_DATA_BUF_SIZE];
	memset(m_pDataBuf,0,RECV_DATA_BUF_SIZE);
	m_nBufLen = RECV_DATA_BUF_SIZE;
	m_nBufData = 0;
	m_xs = 0;
	m_nRightPacketId = 0;
	m_hMutex = CreateMutex(NULL,FALSE,NULL);
	m_bP2PTest = FALSE;
}

CP2PRecvFile::~CP2PRecvFile(void)
{
	Stop();

	if(m_pDataBuf)
	{
		delete [] m_pDataBuf;
	}
	ReleaseMutex(m_hMutex);
}

void CP2PRecvFile::Close()
{
	if(m_pFile)
	{
		if(m_bOpen)
		{
			Flush();
			m_bOpen = false;
			m_pFile->Close();
		}
		delete m_pFile;
		m_pFile = NULL;
	}

}

void CP2PRecvFile::Stop()
{
	m_bWork = false;
	if(m_xs)
	{
		m_pDataTran->CloseSocket(m_xs);
		m_xs = 0;

		if(m_hThread)
		{
			if(WaitForSingleObject(m_hThread,1000) == WAIT_TIMEOUT)
			{
				::TerminateThread(m_hThread,-1);
			};
		}
		m_hThread = NULL;
	}

	Close();

}

P2PTRAN_ERR CP2PRecvFile::Create(const char * szPath,uint32 fileId, uint32 nStartPos, size_t nSize )
{
	//
	// �ر������ļ�
	//
	m_sPath = szPath;

	if(m_pFile)
	{
		if(m_bOpen)
			m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
	m_bOpen = false;

	//
	// �½�һ���ļ�
	//
	CString sName = CString(szPath)+".~tmp";
	UINT flag = 0;
	if(_access(sName,0) == -1)
	{
		flag |= CFile::modeCreate;
	}
	m_pFile = new CFile();
	if(m_pFile->Open(sName,flag|CFile::modeWrite|CFile::typeBinary))
	{
		m_bOpen = true;
	}
	else
	{
		delete m_pFile;
		m_pFile = NULL;
		return P2P_FILE_OPEN_FAILED;
	}

	UINT nSeek = m_pFile->Seek(nStartPos,CFile::begin);

	m_id = fileId;
	m_nSize = nSize;
	m_nStartPos = nStartPos;

	return P2P_OK;
}


DWORD WINAPI CP2PRecvFile::RecvThread(void *p)
{
	CP2PRecvFile * pThis = (CP2PRecvFile *)p;

	const int nBufLen = 1024 * 10;
	char * pBuf = new char[nBufLen];
	pThis->DoRecv(pBuf,nBufLen);
	if(pBuf)
	{
		delete [] pBuf;
	}
	return 0;
}

void CP2PRecvFile::DoRecv(char * pBuf,int nBufLen)
{
	m_bWork = true;
	m_xs = m_pDataTran->Accept(m_nMyId,m_nDestId,m_destIp,m_destPort,m_tranIp,m_tranPort);
	if(m_xs == -1)
	{
		m_bP2PTest = FALSE;

		if(!m_pThis->m_bRecvFileTranNotifyAck)
		{	// ��ʶ10��ͨ�������ļ����俪ʼ
			for(int i = 0; i < 10; ++i)
			{
				if(m_pThis->m_bRecvFileTranNotifyAck)
					break;

				g_pProtocol->SendReq_FileTranNotify();
				Sleep(100);
			}
		}
		if(m_pThis->m_bRecvFileTranNotifyAck)
		{
			return;
		}

		// ����ʧ��
		m_xs = 0;
		m_bWork = false;
		try
		{
			// ��ֹ m_pThis��Ч
			m_pThis->AF_OnDownloadFailed(m_id,FTE_P2P_CONN_FAILED);
		}
		catch (...)
		{

		}
		return;
	}

	int nRecv;
	while(true)
	{
		if(m_bWork == false)
		{		
			m_pDataTran->CloseSocket(m_xs);
			m_xs = 0;
			Close();
			if(GetOkPos() < m_nSize)
			{
				m_pThis->AF_OnDownloadFailed(m_id,FTE_RECV_USER_CANCEL);
			}
			else
			{
				m_pThis->AF_NotifyRecvOk(m_id);
			}

			return;
		}
		nRecv = m_pDataTran->Recv(m_xs,pBuf,nBufLen);
		if(nRecv > 0)
		{
			Write(pBuf,nRecv);
			m_nRecvSize += nRecv;
			m_nCurRecvSize += nRecv;
		}
		else if(nRecv < 0)
		{
			m_pDataTran->CloseSocket(m_xs);
			m_xs = 0;
			m_bWork = false;
			Close();
			m_pThis->AF_OnDownloadFailed(m_id,FTE_DOWNLOAD_ERR);
			return;
		}
		else
		{			
			break;
		}
	}

	m_bWork = false;
	m_pDataTran->CloseSocket(m_xs);
	m_xs = 0;

	Close();

	if(GetOkPos() < m_nSize)
	{
		m_pThis->AF_OnDownloadFailed(m_id,FTE_RECV_USER_CANCEL);
	}
	else
	{
		m_pThis->AF_NotifyRecvOk(m_id);
	}
}

bool CP2PRecvFile::Start(uint32 myId,uint32 destId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis)
{
	if(m_bOpen == false)
		return false;
	m_nMyId = myId;
	m_nDestId = destId;
	m_destIp = ip;
	m_destPort = port;
	m_tranIp = tranIp;
	m_tranPort = tranPort;
	m_pThis = pThis;
	m_pDataTran = pThis->AF_GetDataTran();
	m_hThread = ::CreateThread(0,0,RecvThread,this,0,0);

	return true;
}

// ���յ����ݰ�
void CP2PRecvFile::RecvPacket(uint32 pack_id, const char *pBuf, uint32 nSize)
{
	WaitForSingleObject(m_hMutex,INFINITE);

	// ����Ӧ��
	SEND_FILE_PACK_IND req;
	memset(&req, 0, sizeof(SEND_FILE_PACK_IND));
	req.send_id = m_pThis->m_nMyId;		////���ͷ�
	req.fid     = m_pThis->m_nFriendId;  ////���շ�
	req.flag    = 2;								////�ļ���:1���ļ�����:2���ļ�����:3
	req.file_id = m_id;		////�ļ�ID
	req.pack_id = pack_id;			////�������
	req.size    = nSize;          ////��ȡ�ļ�����
	g_pProtocol->SendReq(req);

	if(pack_id <= m_nRightPacketId)
	{	// ���˵��Ѿ�д���ļ��İ�
		ReleaseMutex(m_hMutex);
		return;
	}

	if(m_mapRecvPacket.find(pack_id) == m_mapRecvPacket.end())
	{	
		m_mapRecvPacket[pack_id] = new STRU_RECV_PECKET_BUF(pBuf, nSize);
	}

	// �����Ƿ����д���ļ�
	BOOL bOK = TRUE;
	map<int, STRU_RECV_PECKET_BUF*>::iterator it_cur = m_mapRecvPacket.begin();
	map<int, STRU_RECV_PECKET_BUF*>::iterator it_next = ++it_cur;
	it_cur = m_mapRecvPacket.begin();
	for(; it_next != m_mapRecvPacket.end(); ++it_cur, ++it_next)
	{
		if(it_next->first != it_cur->first + 1)
		{
			bOK = FALSE;
			break;
		}
	}
	if(bOK)
	{
		if(m_mapRecvPacket.begin()->first != m_nRightPacketId+1)
		{
			bOK = FALSE;
		}
	}

	if(bOK)
	{	// д���ļ�
		it_cur = m_mapRecvPacket.begin();
		for(; it_cur != m_mapRecvPacket.end(); ++it_cur)
		{
			Write(it_cur->second->m_pBuf, it_cur->second->m_nBufSize);
			m_nRightPacketId = it_cur->first;

			/////��������ʾ
			m_nRecvSize += it_cur->second->m_nBufSize;
			m_nCurRecvSize += it_cur->second->m_nBufSize;

			// д���ļ����ͷ��ڴ�
			delete it_cur->second;
		}
		m_mapRecvPacket.clear();
	}
	ReleaseMutex(m_hMutex);
}

CP2PSendFile::CP2PSendFile()
{
	m_nSize = 0;
	m_bStop = false;
	m_bOpen = false;
	m_bWork = false;
	m_pFile = NULL;

	//m_nSendSize = 0;
	//m_nCurSendSize = 0;
	m_nLastSuccSend = 0;
	m_hSendThread = NULL;
	m_xs = 0;
	m_pDataTran = NULL;
	m_dwSendCount = 1;
	m_nSendSize   = 0;
	m_nCurSendSize= 0;
	m_hMutex = CreateMutex(NULL,FALSE,NULL);
	m_hMutexTranThread = CreateMutex(NULL,FALSE,NULL);
	m_bP2PTest = FALSE;
}


CP2PSendFile::~CP2PSendFile()
{
	Stop();
	ReleaseMutex(m_hMutex);
	ReleaseMutex(m_hMutexTranThread);
}



void CP2PSendFile::Stop()
{
	m_bStop = true;

	if(m_xs)
	{
		m_pDataTran->CloseSocket(m_xs);
	}

	m_bWork = false;
	if(m_hSendThread)
	{
		DWORD dwRst = ::WaitForSingleObject(m_hSendThread,1000);
		if(dwRst == WAIT_TIMEOUT)
		{
			::TerminateThread(m_hSendThread,-1);
		}
	}
	m_hSendThread = 0;

	CloseAllThread();

	Close();
}

void CP2PSendFile::Close()
{
	if(m_pFile)
	{
		if(m_bOpen)
			m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
	m_bOpen = false;

	//m_sPath = "";
	m_id = 0;
	m_nSize = 0;
	m_nStartPos = 0;

}

P2PTRAN_ERR CP2PSendFile::Create(const char * szFile,uint32 fid,uint64 time)
{
	if(m_pFile)
	{
		if(m_bOpen)
			m_pFile->Close();
		delete m_pFile;
		m_pFile = NULL;
	}
	m_bOpen = false;


	m_pFile = new CFile();
	if(m_pFile->Open(szFile,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone))
	{
		m_bOpen = true;
	}
	else
	{
		delete m_pFile;
		m_pFile = NULL;
		return P2P_FILE_OPEN_FAILED;
	};

	m_sPath = szFile;
	m_id = fid;
	m_time = time;
	m_nSize = (size_t)m_pFile->GetLength();
	return P2P_OK;
}

// ��ʼ�ļ�����
bool CP2PSendFile::Start(int nStartPos,uint32 fid,uint32 myId,uint32 destId,uint32 destIp,uint16 destPort,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis)
{
	if(m_bOpen == false)
		return false;

	m_bStop = false;

	m_pFile->Seek(nStartPos,CFile::begin);
	m_nStartPos = nStartPos;

	m_pDataTran = pThis->AF_GetDataTran();
	m_nfid = fid;
	m_nMyId = myId;
	m_nDestId = destId;
	m_destIp = destIp;
	m_destPort = destPort;
	m_tranIp = tranIp;
	m_tranPort = tranPort;
	m_pThis = pThis;

	m_hSendThread = ::CreateThread(NULL,0,SendThread,this,0,0);

	return true;
}

DWORD WINAPI CP2PSendFile::SendThread(void *p)
{
	CP2PSendFile * pThis = (CP2PSendFile*) p;

	BOOL bSuccess = pThis->DoSend();
	if(!bSuccess)
	{	// P2Pʧ�ܣ������ļ���������ת
		if(!pThis->m_pThis->m_bRecvFileTranNotifyAck)
		{	// ��ʶ10��ͨ�������ļ����俪ʼ
			for(int i = 0; i < 10; ++i)
			{
				if(pThis->m_pThis->m_bRecvFileTranNotifyAck)
					break;

				g_pProtocol->SendReq_FileTranNotify();
				Sleep(100);
			}
		}
		if(!pThis->m_pThis->m_bRecvFileTranNotifyAck)
		{	// ͨ��ʧ��
			pThis->m_bWork = false;
			pThis->m_pDataTran->CloseWait(pThis->m_xs);
			pThis->m_xs = 0;
			pThis->Close();
			pThis->m_pThis->AF_OnSendFailed(pThis->m_id,FTE_P2P_CONN_FAILED);
			return 0;
		}

		int nRead;
		while(true)
		{
			memset(pThis->m_szSendBuf, 0, sizeof(pThis->m_szSendBuf));
			int nPackCount = 0;
			nRead = pThis->Read(pThis->m_szSendBuf, sizeof(pThis->m_szSendBuf));
			if(nRead > 0)
			{
				nPackCount = nRead/TRANSER_PACKET_SIZE;
				if(nRead%TRANSER_PACKET_SIZE > 0)
				{
					nPackCount++;
				}

				for(int i = 0; i < nPackCount; ++i)
				{
					int nPacketSize = TRANSER_PACKET_SIZE;
					if(nRead%TRANSER_PACKET_SIZE > 0 && i == nPackCount-1)
						nPacketSize = nRead%TRANSER_PACKET_SIZE;
					pThis->AddPacketId(pThis->m_dwSendCount+i);
					STRU_SEND_PACKET *pSendPacket = new STRU_SEND_PACKET(pThis, pThis->m_dwSendCount+i, pThis->m_szSendBuf+i*TRANSER_PACKET_SIZE, nPacketSize);
					pThis->m_nSendSize += nPacketSize;
					pThis->m_nCurSendSize += nPacketSize;
					::CreateThread(NULL,0,SendPacketThread,pSendPacket,0,0);
					Sleep(80);
				}
				pThis->m_dwSendCount += nPackCount;

				// �ж����а��Ƿ�˳���������շ�
				while(TRUE)
				{
					Sleep(10);
					BOOL bAllArrive = pThis->PacketIdSetIsEmpty();		// ���а��Ƿ񶼵ִ�

					if(bAllArrive)
					{
						break;
					}
				}
			}
			else
			{
				// ���ͽ�����ʶ
				pThis->m_sendPack = (LPSEND_FILE_PACK_REQ)malloc(sizeof(SEND_FILE_PACK_REQ));
				pThis->m_sendPack->send_id = pThis->m_nMyId;     ////���ͷ�
				pThis->m_sendPack->fid     = pThis->m_nDestId;   ////���շ�
				pThis->m_sendPack->flag    = 3;           ////�ļ���:1���ļ�����:2���ļ�����:3
				pThis->m_sendPack->file_id = pThis->m_nfid;   ////�ļ�ID
				pThis->m_sendPack->pack_id = pThis->m_dwSendCount/*m_nFileSendCnt[nNum]*/;       ////�������
				pThis->m_sendPack->size    = nRead;           ////��ȡ�ļ�����\

				g_pProtocol->SendReq(*(pThis->m_sendPack));

				free(pThis->m_sendPack);

				break;
			}
		}
		pThis->m_bWork = false;
		pThis->m_pDataTran->CloseWait(pThis->m_xs);
		pThis->m_xs = 0;
		pThis->Close();
		pThis->m_pThis->AF_OnSendOK(pThis->m_id);
	}

	return 0;
}

// �������ݰ����̣߳��������ݰ��ã����1s�������ݰ��Ƿ�ɹ����յ������δ���գ��ط������ݰ���
DWORD WINAPI CP2PSendFile::SendPacketThread(void *p)
{
	STRU_SEND_PACKET *pSendPacket = (STRU_SEND_PACKET *)p;
	DWORD id = GetCurrentThreadId();
	pSendPacket->m_pThis->AddThreadHandle(id, pSendPacket);

PACKET_SEND:
	//���͵���������
	SEND_FILE_PACK_REQ req;
	memset(&req, 0, sizeof(SEND_FILE_PACK_REQ));
	req.send_id = pSendPacket->m_pThis->m_nMyId;    ////���ͷ�
	req.fid     = pSendPacket->m_pThis->m_nDestId;  ////���շ�
	req.flag    = 2;								////�ļ���:1���ļ�����:2���ļ�����:3
	req.file_id = pSendPacket->m_pThis->m_id;		////�ļ�ID
	req.pack_id = pSendPacket->m_nPacketId;			////�������
	req.size    = pSendPacket->m_nBufSize;          ////��ȡ�ļ�����
	memcpy(req.buf, pSendPacket->m_pBuf, pSendPacket->m_nBufSize);
	g_pProtocol->SendReq(req);

	// 1s�����Ƿ��յ�ȷ����Ϣ
	for(int i = 0; i < 100; ++i)
	{
		Sleep(10);
		if(pSendPacket->m_pThis->ExistPacketId(req.pack_id))
			continue;
		else
			break;
	}
	if(pSendPacket->m_pThis->ExistPacketId(req.pack_id))
	{	// δ�յ�Ӧ�����·���
		goto PACKET_SEND;
	}

	CP2PSendFile *pThis = pSendPacket->m_pThis;

	// �ͷ��ڴ�
	delete pSendPacket;
	pSendPacket = NULL;

	pThis->DelThreadHandle(id);

	return TRUE;
}

BOOL CP2PSendFile::DoSend()
{
	m_bP2PTest = TRUE;
	m_bWork = true;
	m_xs = m_pDataTran->Socket(128,10*1024);
	int nRt = m_pDataTran->Connect(m_xs,m_nMyId,m_nDestId,m_destIp,m_destPort,m_tranIp,m_tranPort);
	if(nRt != 0)
	{
		m_bP2PTest = FALSE;
		// ����ʧ��
		m_pDataTran->CloseSocket(m_xs);
		m_xs = 0;
		m_bWork = false;

		return FALSE;
	}

	int nRead;
	int nSend;
	while(true)
	{
		if(m_bWork == false)
		{		
			m_pDataTran->CloseSocket(m_xs);
			m_xs = 0;

			if ( m_bStop==false )
				m_pThis->AF_OnSendFailed(m_id,FTE_SEND_USER_CANCEL);

			return TRUE;
		}

		int nBufLen = 1024*10;
		char szBuf[1024*10] = {0};
		nRead = Read(szBuf,nBufLen);
		if(nRead > 0)
		{
			nSend = m_pDataTran->Send(m_xs,szBuf,nRead);
			if(nSend <= 0)
			{
				m_pDataTran->CloseSocket(m_xs);
				m_xs = 0;

				if ( m_bStop==false )
					m_pThis->AF_OnSendFailed(m_id,FTE_SEND_FILE_ERR);

				m_bWork = false;
				return TRUE;
			}
			else
			{
				//m_nSendSize += nSend;
				//m_nCurSendSize += nSend;
			}
		}
		else
		{
			break;
		}
	}
	m_bWork = false;
	m_pDataTran->CloseWait(m_xs);
	m_xs = 0;
	Close();
	m_pThis->AF_OnSendOK(m_id);
	return TRUE;
}


int CP2PSendFile::GetCurSendSize()
{
	if(m_bP2PTest)
	{
		int nSend;
		int nRt;
		if(m_pDataTran == NULL || m_xs == NULL)
			return 0;
		nSend = m_pDataTran->GetSuccSend(m_xs);
		nRt = nSend - m_nLastSuccSend ;
		m_nLastSuccSend = nSend;
		return nRt;
	}
	else
	{
		int n = m_nCurSendSize;
		m_nCurSendSize = 0;
		return n;
	}
}

// ���ͷ����յ�Ӧ��
void CP2PSendFile::RecvArriveAck(uint32 pack_id)
{
	DelPacketId(pack_id);
}