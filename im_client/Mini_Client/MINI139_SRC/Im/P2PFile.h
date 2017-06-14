// ----------------------------------------------------------------
// �ļ�:P2PFile.h
// �汾: 
// ����: xy
// ����: 2007-1-9
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include "..\utils\Function.h"
#include <ClientProtocol.h>
#include "MiniNetTrans.h"
#include "improtocol.h"
#include <set>
#include <map>

static const int	P2P_PACKET_SIZE = 1396;			// P2P����ʱ����С
static const int	TRANSER_PACKET_SIZE = 1024*50;	// ͨ���ļ�������ת������ʱ����С

typedef unsigned int	uint32;


enum P2PTRAN_ERR{
	P2P_OK,					// �����ɹ�
	P2P_FILE_NOT_OPEN,		// �ļ�û�д�
	P2P_FILE_OPEN_FAILED,	// �ļ���ʧ��
	P2P_PACKETSEQ_OUTRANGE,	// ָ���İ���ų����ļ���Χ
	P2P_PACKET_EXIST,		// ���յİ��Ѿ�����
	P2P_PACKET_OUTWND,		// ���յİ����ڴ��ڷ�Χ��
	P2P_NEED_QUICK_RESEND,	// ��Ҫ�����ش�
	P2P_SEND_TIMEOUT,		// ���ͳ�ʱ
	P2P_REQFILE_NOTEXIST,	// ������ļ�������
};

class CSyncCritical
{
public:
	CSyncCritical(void)
	{ ::InitializeCriticalSection(&m_sect); };

	~CSyncCritical(void)
	{ ::DeleteCriticalSection(&m_sect); };

	void Lock()
	{ ::EnterCriticalSection(&m_sect); };

	void Unlock()
	{ ::LeaveCriticalSection(&m_sect); };

	CRITICAL_SECTION * Get()
	{ return &m_sect;};

private:
	CRITICAL_SECTION	m_sect;
};


class CMyLock
{
public:
	CMyLock(CRITICAL_SECTION *pSec)
	{
		m_pSec = pSec;
		::EnterCriticalSection(pSec);
	}

	~CMyLock()
	{
		::LeaveCriticalSection(m_pSec);
	}

private:
	CRITICAL_SECTION *	m_pSec;
};





class CP2PRecvBuf;

class CAdvancedFileTransSession;

struct P2PID
{
	int		uid;	// �û�ID
	int		fid;	// �ļ�ID

	P2PID(int nUid,int nFid)
	{
		uid = nUid;
		fid = nFid;
	}

	P2PID()
	{

	}

	void Set(int nUid,int nFid)
	{
		uid = nUid;
		fid = nFid;
	}

	bool operator < (P2PID id) const 
	{
		return (fid < id.fid) || (fid == id.fid) && (uid < id.uid);
	}

	bool operator == (P2PID id) const
	{
		return fid == id.fid && uid == id.uid;
	}

};



//------------------------------------------------------
// ���ݽ��ջ�����
//------------------------------------------------------
class CP2PRecvBuf
{
protected:
	CP2PRecvBuf(const CP2PRecvBuf & buf);
	void operator = (const CP2PRecvBuf & buf);
public:

	explicit CP2PRecvBuf(int nBufSize = 1024*128)
	{
		m_pFile = NULL;
		m_nBuf = nBufSize;
		m_pBuf = new char[m_nBuf] ;
		m_pCursor = m_pBuf;
	};

	~CP2PRecvBuf()
	{
		if(m_pBuf)
		{
			delete [] m_pBuf;
		}
	};

	void	SetFile(CFile * pFile)
	{ m_pFile = pFile; };

	void	Flush()
	{
		assert(m_pFile);
		int nC = m_pCursor - m_pBuf;
		if(nC > 0)
		{
			m_pFile->Write(m_pBuf,nC);
			m_pCursor = m_pBuf;
		}
	}

	void	Clear()
	{
		m_pCursor = m_pBuf;
	}

	int		GetDataLen()
	{
		return m_pCursor - m_pBuf;
	}

	char *	GetData()
	{
		return m_pBuf;
	}

	//------------------------------------------------------
	// �򻺳�д�����ݣ����سɹ�д��ĳ��ȡ����������ʱ����
	// ���صĳ���С��������ֽڳ���
	//------------------------------------------------------
	int		Write(const char * pData,int nLen)
	{
		int nSpace = m_pBuf + m_nBuf - m_pCursor;
		assert(nSpace >= 0);
		if(nSpace < nLen)
		{
			memcpy(m_pCursor,pData,nSpace);
			m_pCursor += nSpace;
			return nSpace;
		}
		else
		{
			memcpy(m_pCursor,pData,nLen);
			m_pCursor += nLen;
			return nLen;
		}
	}

protected:
	char *			m_pBuf;		// ���ݽ��ջ���
	char *			m_pCursor;	// ���ݽ��ջ����α�
	int				m_nBuf;		// ���ݽ��ջ����С
	CFile *			m_pFile;	// �ļ�����
};


//struct TRANWND_ITEM
//{
//protected:
//	void operator = (const TRANWND_ITEM & wnd);
//	TRANWND_ITEM(const TRANWND_ITEM & wnd);
//public:
//	uint32		seq;
//	uint8		status;		// ����״̬
//
//	char *		buf;		
//	int			nLen;		
//
//	explicit TRANWND_ITEM(int nBufLen = P2P_PACKET_SIZE)
//	{
//		seq = 0;
//		status = 0;
//		buf = new char[nBufLen];
//		nLen = 0;
//	}
//
//	void	Exchange(TRANWND_ITEM & it)
//	{
//		char * temp = buf;
//		seq = it.seq;
//		status = it.status;
//		buf = it.buf;
//		nLen = it.nLen;
//		it.buf = temp;
//	}
//
//	void	Clear()
//	{
//		status = 0;
//		seq = 0;
//		nLen = 0;
//	}
//
//	~TRANWND_ITEM()
//	{
//		if(buf)
//		{
//			delete [] buf;
//		}
//	}
//
//
//};

//
//// ���մ���
//class CP2PRecvWnd
//{
//private:
//
//	CP2PRecvWnd(const CP2PRecvWnd & o);
//
//	void operator = (const CP2PRecvWnd & o);
//
//public:
//
//	static const int	MAX_REQ_COUNT = 64;
//	static const int	MAX_WND_SIZE = 128;	// ����Ҫ���ڿ����ش��涨�ĸ��� 
//	static const int	QUICK_RESEND = 3;
//
//	enum STATUS{
//		WAITING = 0,	// �ȴ�������
//		SENDING = 1,	// ������
//		CONFIRM = 2,	// �Ѿ�ȷ��
//		RESEND = 3,		// �ط�
//	};
//
//	bool	IsInWnd(int n)
//	{ 
//		CMyLock l(lock.Get());
//		return n < nEnd && n >= nBeg; 
//	};
//
//	void	SetItem(int n,int seq,int status)
//	{
//		if(IsInWnd(n))
//		{
//			lock.Lock();
//			int idx = n - nBeg;
//			pItems[idx].seq = seq;
//			pItems[idx].status = status;
//			lock.Unlock();
//		}
//	}
//
//	bool	GetItemSeq(int n,int & seq)
//	{
//		if(IsInWnd(n))
//		{
//			CMyLock l(lock.Get());
//			seq = pItems[n - nBeg].seq;
//			return true;
//		}
//		return false;
//	}
//
//	//------------------------------------------------------
//	// ����ֵ��	0	�ɹ�ȷ��
//	//			-1	�յ��ظ���ACK
//	//			-2	�յ��Ƿ���ACK
//	//			>0	��Ҫ�����ش��ı������
//	//------------------------------------------------------
//	int		Confirm(const P2P_DOWNLOAD_ACK & ack,CP2PRecvBuf * pBuf)
//	{
//		CMyLock l(lock.Get());
//		int nNewLeft = ack.packetID;
//		if(nNewLeft >= nBeg && nNewLeft < nEnd)
//		{
//			// ȷ�ϸñ���
//			int nIdx = nNewLeft - nBeg;
//			if(pItems[nIdx].status == CONFIRM)
//			{
//				return -1;
//			}
//			// ȷ��״̬
//			pItems[nIdx].status = CONFIRM;
//			// ��������
//			memcpy(pItems[nIdx].buf,ack.data,ack.len);
//			pItems[nIdx].nLen = ack.len;
//			int size = Size();
//			int nMove = 0;
//			int nLen = 0; 
//			int i;
//			//
//			// ����Ŀǰ�����������ȷ�ϱ��ĵĸ���
//			//
//			int nDataWrite = 0;
//			int nLeftData = 0;
//			char * pData;
//			for(i = 0;i < size;i++)
//			{
//				if(pItems[i].status == CONFIRM)
//				{
//					// �Ѿ�ȷ�ϵ�����д�뻺��
//					nLeftData = pItems[i].nLen;
//					pData = pItems[i].buf;
//					while(nLeftData > 0)
//					{
//						nDataWrite = pBuf->Write(pData,nLeftData);
//						pData += nDataWrite;
//						nLeftData -= nDataWrite;
//						if(nLeftData > 0)
//						{
//							//  �����Ѿ���
//							pBuf->Flush();
//						}
//					}
//					nLen++;
//				}
//				else
//					break;
//			}
//			//
//			// ������������ȷ���򴰿�����
//			//
//			if(nLen > 0)
//			{
//				nBeg += nLen;
//				nEnd += nLen;
//				nMove = size - nLen;
//
//				// ״̬λ����
//				for(i = 0;i < nMove; i++)
//				{
//					pItems[i].Exchange(pItems[i+nLen]);
//				}
//				for(;i < size;i++)
//				{
//					pItems[i].Clear() ;
//				}
//				return 0;
//			}
//			//
//			// ������û���ƶ������ж��Ƿ��б�����Ҫ�����ش�
//			//
//			else
//			{
//				int j;
//				int nAck = 0;
//				nLen = nNewLeft - nBeg;
//				if(nLen > QUICK_RESEND)
//				{
//					for(i = 0;i < nLen;i++)
//					{
//						nAck = 0;
//						if(pItems[i].status == SENDING && ((nLen - i) > QUICK_RESEND) )
//						{
//							for(j = i + 1;j < nLen;j++)
//							{
//								if(pItems[j].status == CONFIRM)
//								{
//									nAck++;
//									// �жϿ����ش�
//									if(nAck >= QUICK_RESEND)
//										return nBeg + i;
//								}
//							}
//						}
//					}
//				}
//				return 0;
//			}
//
//		}
//		return -2;
//	}
//
//	int		Size()
//	{ 
//		CMyLock l(lock.Get());
//		return nEnd - nBeg; 
//	};
//
//	CP2PRecvWnd(int nSize = MAX_WND_SIZE)
//	{
//		nBeg = 0;
//		nEnd = nSize;
//		pItems = new TRANWND_ITEM[nSize];
//		
//	};
//
//	~CP2PRecvWnd()
//	{
//		if(pItems)
//		{
//			delete [] pItems;
//			pItems = NULL;
//		}
//	}
//
//
//	int		Left()
//	{ 
//		CMyLock l(lock.Get());
//		return nBeg; 
//	};
//
//	int		Right()
//	{ 
//		CMyLock l(lock.Get());
//		return nEnd; 
//	};
//
//	
//
//private:
//
//	
//
//	CSyncCritical	lock;
//	TRANWND_ITEM *	pItems;
//	int				nBeg;
//	int				nEnd;
//
//};



//------------------------------------------------------
// ������Ľ����ļ�
//------------------------------------------------------
// �������ݰ�
struct STRU_RECV_PECKET_BUF
{
	char *m_pBuf;
	int m_nBufSize;

	STRU_RECV_PECKET_BUF(const char *pBuf, int nBufSize)
	{
		m_nBufSize = nBufSize;
		if(nBufSize > 0)
		{
			m_pBuf = new char[nBufSize];
			memcpy(m_pBuf, pBuf, nBufSize);
		}
		else
		{
			m_pBuf = NULL;
		}
	}

	~STRU_RECV_PECKET_BUF()
	{
		if(m_pBuf != NULL)
		{
			delete[] m_pBuf;
			m_pBuf = NULL;
		}
	}

private:
	STRU_RECV_PECKET_BUF()
	{

	}
};

class CP2PRecvFile
{
public:

	CP2PRecvFile(void);

	~CP2PRecvFile(void);

	friend class CAdvancedFileTransSession;

	static const int RECV_DATA_BUF_SIZE = 1024*128;
	
public:

	//------------------------------------------------------
	// ����һ��������Ĵ������ļ�
	//------------------------------------------------------
	P2PTRAN_ERR		Create(
		const char *	szPath,
		uint32			fileId,
		uint32			nStartPos,
		size_t			nSize
	);

	char * GetFilePath() {
		return (char *)m_sPath.c_str();
	}

	
	//------------------------------------------------------
	// ������д���ļ�
	//------------------------------------------------------
	void Write(char * buf,int nLen)
	{
		if(nLen <= 0)
			return;
		if(m_bOpen)
		{
			if(m_nBufData + nLen > m_nBufLen)
			{
				// �Ƚ���������д���ļ�
				if(m_nBufData > 0)
					m_pFile->Write(m_pDataBuf,m_nBufData);
				// ��д��������
				if(nLen > 0)
				{
					m_pFile->Write(buf,nLen);
				}	
				m_nBufData = 0;
			}
			else
			{
				memcpy(m_pDataBuf + m_nBufData,buf,nLen);
				m_nBufData += nLen;
			}
		}
	}

	//------------------------------------------------------
	// ����������д���ļ�
	//------------------------------------------------------
	void			Flush()
	{ 
		if(m_bOpen)
		{
			if(m_nBufData > 0)
				m_pFile->Write(m_pDataBuf,m_nBufData);
			m_nBufData = 0;
		};
	};

	CString GetFileName() 
	{

		CString str1,str2,str3;
		ExtractFilePath(m_sPath.c_str(),str1,str2,str3);
		return str2+str3;
	}

	void  SaveFilePosition(uint32 senderUID)
	{
		
		LONG nPosition = GetOkPos();
		//���浱ǰ�ϵ�λ��
		CString    strCfgFile = CString(m_sPath.c_str())+".cfg";
		CStdioFile f;
		DeleteFile(strCfgFile);

		if ( f.Open(strCfgFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
		{
			f.Write(&senderUID,sizeof(senderUID));
			f.Write(&nPosition,sizeof(LONG));
			f.Close();
		}
		
	}

	
	

	//------------------------------------------------------
	// ��ȡ�������ļ��Ĵ�С
	//------------------------------------------------------
	size_t			GetSize() const
	{ return m_nSize; };

	
	int				ClearCurRecvSize()
	{
		int n = m_nCurRecvSize;
	//	m_nCurRecvSize= 0;
		m_nCurRecvSize = 0;
		//GetOkPos()/1024
		return n;
	}

	bool			IsDownloading()
	{
		return m_bWork;
	}

	void			Close();

	void			Stop();

	int				GetFID() const
	{ return m_id; };

	//------------------------------------------------------
	// ��ȡ�Ѿ��ɹ����յ��ļ�λ��
	//------------------------------------------------------
	size_t			GetOkPos()
	{ return m_nRecvSize + m_nStartPos; };

	size_t			GetStartPos()
	{ return m_nStartPos; };

	bool			Start(uint32 myId,uint32 destId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis);

	// ���յ����ݰ�
	void			RecvPacket(uint32 pack_id, const char *pBuf, uint32 nSize);

protected:
	
	static  DWORD	WINAPI RecvThread(void *p);

	void			DoRecv(char * pBuf,int nBufLen);

protected:

	uint32			m_id;			// �ļ���ʶ��
	string			m_sPath;		// �ļ�����·��
	size_t			m_nSize;		// �ļ���С
	size_t			m_nPacketSize;	// ���Ͱ���С
	size_t			m_nStartPos;	// �ϴ����صĶϵ�λ��

	DWORD			m_dwStartTick;  // ��ʼ���ص�ʱ��tick

	CFile *			m_pFile;		// �ļ�
	bool			m_bOpen;		// �ļ��Ĵ�״̬
	bool			m_bWork;		// �ļ��Ƿ���������
		

	char *			m_pDataBuf;		// �ļ����ݻ���
	int				m_nBufLen;		// �������Ĵ�С
	int				m_nBufData;		// �����������ݵĴ�С
	
private:
	
	HANDLE			m_hThread;		// �����ļ������߳�
	int				m_nCurRecvSize;	// �ڶ�ʱ�ڽ��յ��ֽ���
	size_t			m_nRecvSize;	// ��ǰ�Ѿ����յ����ݴ�С
	
	uint32			m_nMyId;
	uint32			m_nDestId;
	uint32			m_destIp;
	uint16			m_destPort;
	uint32			m_tranIp;
	uint16			m_tranPort;

	XSOCKET			m_xs;
	CUdpNetTrans *	m_pDataTran;
	CAdvancedFileTransSession * m_pThis;
	int m_nRightPacketId;			// ��ȷ���յ������ݰ�Id 
	map<int, STRU_RECV_PECKET_BUF*>	m_mapRecvPacket;
	HANDLE m_hMutex;				// ����������֤��m_setPackArrive�Ĳ���Ψһ��
	BOOL m_bP2PTest;				// P2P�����Ƿ�ɹ�
};


//------------------------------------------------------
// �ṩ���ݷ��ʷ�����ļ���
//------------------------------------------------------
// ���ݰ��ṹ��
class CP2PSendFile;
struct STRU_SEND_PACKET
{
	CP2PSendFile *m_pThis;
	int m_nPacketId;
	char *m_pBuf;
	int m_nBufSize;

	STRU_SEND_PACKET(CP2PSendFile *pThis, int nPacketId, const char *pBuf, int nBufSize)
	{
		m_pThis = pThis;
		m_nPacketId = nPacketId;
		m_nBufSize = nBufSize;
		if(m_nBufSize > 0)
		{
			m_pBuf = new char[m_nBufSize];
			memcpy(m_pBuf, pBuf, m_nBufSize);
		}
		else
		{
			m_pBuf = NULL;
		}
	}

	~STRU_SEND_PACKET()
	{
		if(m_pBuf != NULL)
		{
			delete[] m_pBuf;
			m_pBuf = NULL;
		}
	}

private:
	STRU_SEND_PACKET()
	{

	}
};

class CP2PSendFile
{
public:

	CP2PSendFile();

	~CP2PSendFile();

private:

	CP2PSendFile(const CP2PSendFile & file);

	void operator = (const CP2PSendFile & file);

	friend class CAdvancedFileTransSession;

public:

	CString GetFileName()
	{
		CString str1,str2,str3;
		ExtractFilePath(m_sPath.c_str(),str1,str2,str3);
		
		return (str2+str3);
	}

	CString GetFilePath()
	{
		return m_sPath.c_str();
	}

	//------------------------------------------------------
	// ����һ�������͵��ļ�
	//------------------------------------------------------
	P2PTRAN_ERR		Create(const char * szFile,uint32 fid,uint64 time);


	//------------------------------------------------------
	// �ر��ļ�
	//------------------------------------------------------
	void			Close();

	UINT			Read(void * p,int nLen)
	{
		if(m_bOpen)
			return m_pFile->Read(p,nLen);
		else
			return -1;
	}

	void			Stop();

	uint32			GetID() const
	{ return m_id; };

	size_t			GetSize() const
	{ return m_nSize; };

	uint32			GetTime() const
	{ return m_time; };

	size_t			GetStartPosition() const
	{ return m_nStartPos; };

	bool			IsSending()
	{
		return m_bWork;
	}

	int				GetSuccSend()
	{
		if(m_bP2PTest)
		{
			if(m_pDataTran && m_xs)
			{
				return m_nStartPos + m_pDataTran->GetSuccSend(m_xs);
			}
			else
			{
				return m_nStartPos + m_nLastSuccSend;
			};
		}
		else
		{
			return m_nSendSize;
		}
	};

	int				GetCurSendSize();

	// ��ʼ�ļ�����
	bool			Start(int nStartPos,uint32 fid,uint32 myId,uint32 destId,uint32 destIp,uint16 destPort,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis);

	// ���ͷ����յ�Ӧ��
	void			RecvArriveAck(uint32 pack_id);

private:
	// ���δ���յ�ȷ����Ϣ�İ�
	void AddPacketId(uint32 pid)
	{
		WaitForSingleObject(m_hMutex,INFINITE);
		if(m_setPackArrive.find(pid) == m_setPackArrive.end())
			m_setPackArrive.insert(pid);
        ReleaseMutex(m_hMutex);
	}

	// ɾ�����յ�ȷ����Ϣ�İ�
	void DelPacketId(uint32 pid)
	{
		WaitForSingleObject(m_hMutex,INFINITE);
		if(m_setPackArrive.find(pid) != m_setPackArrive.end())
			m_setPackArrive.erase(pid);
        ReleaseMutex(m_hMutex);
	}

	// ���������Ƿ�Ϊ��
	bool PacketIdSetIsEmpty()
	{
		bool bEmpty = false;
		WaitForSingleObject(m_hMutex,INFINITE);
		bEmpty = m_setPackArrive.empty();
        ReleaseMutex(m_hMutex);
		return bEmpty;
	}

	// ���������Ƿ���ڰ�
	bool ExistPacketId(uint32 pid)
	{
		bool bExist = false;
		WaitForSingleObject(m_hMutex,INFINITE);
		bExist = (m_setPackArrive.find(pid) != m_setPackArrive.end());
        ReleaseMutex(m_hMutex);
		return bExist;
	}

	// ����߳̾��
	void AddThreadHandle(DWORD nThreadID, STRU_SEND_PACKET *pPacket)
	{
		WaitForSingleObject(m_hMutexTranThread,INFINITE);
		std::map<DWORD, STRU_SEND_PACKET*>::iterator it = m_mapTranThread.find(nThreadID);
		if( it == m_mapTranThread.end())
			m_mapTranThread[nThreadID] = pPacket;
		ReleaseMutex(m_hMutexTranThread);
	}

	// ɾ���߳̾��
	void DelThreadHandle(DWORD nThreadID)
	{
		WaitForSingleObject(m_hMutexTranThread,INFINITE);
		std::map<DWORD, STRU_SEND_PACKET*>::iterator it = m_mapTranThread.find(nThreadID);
		if( it != m_mapTranThread.end())
			m_mapTranThread.erase(it);
		ReleaseMutex(m_hMutexTranThread);
	}

	// �ر������߳�
	void CloseAllThread()
	{
		WaitForSingleObject(m_hMutexTranThread,INFINITE);
		std::map<DWORD, STRU_SEND_PACKET*>::iterator it = m_mapTranThread.begin();
		for(; it != m_mapTranThread.end(); ++it)
		{
			HANDLE hThread = OpenThread(THREAD_ALL_ACCESS, FALSE, it->first);
			TerminateThread(hThread, -1);
			CloseHandle(hThread);
			delete it->second;
		}
		ReleaseMutex(m_hMutexTranThread);
	}

protected:

	static  DWORD	WINAPI SendThread(void *p);

	// �������ݰ����̣߳��������ݰ��ã����1s�������ݰ��Ƿ�ɹ����յ������δ���գ��ط������ݰ���
	static DWORD	WINAPI SendPacketThread(void *p);

	BOOL			DoSend();

protected:

	DWORD			m_nBegTime;
	DWORD			m_nEndTime;
	SESSION_ID		m_sessionID;
	HANDLE			m_hThread;
	bool			m_bTerminate;

protected:
	uint32			m_nfid;
	uint32			m_nMyId;
	uint32			m_nDestId;
	uint32			m_destIp;
	uint16			m_destPort;
	uint32			m_tranIp;
	uint16			m_tranPort;
	
	int				m_nStartPos;	// ��ʼλ��
	uint32			m_id;			// �ļ�ID
	string			m_sPath;		// �ļ���ȡ·��
	string			m_sName;		// �ļ���
	size_t			m_nSize;		// �ļ���С
	uint64			m_time;			// �ļ��޸�ʱ��
	CFile *			m_pFile;		// �ļ�

	bool			m_bOpen;		// �ļ��Ƿ񱻴�
	int				m_nLastSuccSend;
	//int				m_nSendSize;	// �Ѿ��������ݵĴ�С
	//int				m_nCurSendSize;	// ��ǰһ��ʱ�䷢�͵����ݴ�С
	bool			m_bWork;		// 
	bool			m_bStop;		//
	HANDLE			m_hSendThread;


	XSOCKET			m_xs;	
	CUdpNetTrans *	m_pDataTran;
	CAdvancedFileTransSession * m_pThis;

	LPSEND_FILE_PACK_REQ      m_sendPack;
    DWORD           m_dwSendCount;
	DWORD           m_nSendSize;
	DWORD           m_nCurSendSize;

	std::set<int> m_setPackArrive;	// ��ʾ���Ƿ�˳������
	HANDLE m_hMutex;				// ����������֤��m_setPackArrive�Ĳ���Ψһ��
	std::map<DWORD, STRU_SEND_PACKET*> m_mapTranThread;	// ���濪�����߳�
	HANDLE m_hMutexTranThread;		// ����������֤��m_setTranThread�Ĳ���Ψһ��
	char m_szSendBuf[10*TRANSER_PACKET_SIZE];	
	BOOL m_bP2PTest;				// P2P���Ƿ�ɹ�
};