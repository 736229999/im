// ----------------------------------------------------------------
// 文件:P2PFile.h
// 版本: 
// 作者: xy
// 日期: 2007-1-9
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#include "..\utils\Function.h"
#include <ClientProtocol.h>
#include "MiniNetTrans.h"
#include "improtocol.h"
#include <set>
#include <map>

static const int	P2P_PACKET_SIZE = 1396;			// P2P传输时包大小
static const int	TRANSER_PACKET_SIZE = 1024*50;	// 通过文件传输中转服务器时包大小

typedef unsigned int	uint32;


enum P2PTRAN_ERR{
	P2P_OK,					// 操作成功
	P2P_FILE_NOT_OPEN,		// 文件没有打开
	P2P_FILE_OPEN_FAILED,	// 文件打开失败
	P2P_PACKETSEQ_OUTRANGE,	// 指定的包序号超出文件范围
	P2P_PACKET_EXIST,		// 接收的包已经存在
	P2P_PACKET_OUTWND,		// 接收的包不在窗口范围内
	P2P_NEED_QUICK_RESEND,	// 需要快速重传
	P2P_SEND_TIMEOUT,		// 发送超时
	P2P_REQFILE_NOTEXIST,	// 请求的文件不存在
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
	int		uid;	// 用户ID
	int		fid;	// 文件ID

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
// 数据接收缓冲区
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
	// 向缓冲写入数据，返回成功写入的长度。（缓冲溢出时，则
	// 返回的长度小于输入的字节长度
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
	char *			m_pBuf;		// 数据接收缓冲
	char *			m_pCursor;	// 数据接收缓冲游标
	int				m_nBuf;		// 数据接收缓冲大小
	CFile *			m_pFile;	// 文件对象
};


//struct TRANWND_ITEM
//{
//protected:
//	void operator = (const TRANWND_ITEM & wnd);
//	TRANWND_ITEM(const TRANWND_ITEM & wnd);
//public:
//	uint32		seq;
//	uint8		status;		// 发送状态
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
//// 接收窗口
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
//	static const int	MAX_WND_SIZE = 128;	// 必须要大于快速重传规定的个数 
//	static const int	QUICK_RESEND = 3;
//
//	enum STATUS{
//		WAITING = 0,	// 等待发送中
//		SENDING = 1,	// 发送中
//		CONFIRM = 2,	// 已经确认
//		RESEND = 3,		// 重发
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
//	// 返回值：	0	成功确认
//	//			-1	收到重复的ACK
//	//			-2	收到非法的ACK
//	//			>0	需要快速重传的报文序号
//	//------------------------------------------------------
//	int		Confirm(const P2P_DOWNLOAD_ACK & ack,CP2PRecvBuf * pBuf)
//	{
//		CMyLock l(lock.Get());
//		int nNewLeft = ack.packetID;
//		if(nNewLeft >= nBeg && nNewLeft < nEnd)
//		{
//			// 确认该报文
//			int nIdx = nNewLeft - nBeg;
//			if(pItems[nIdx].status == CONFIRM)
//			{
//				return -1;
//			}
//			// 确认状态
//			pItems[nIdx].status = CONFIRM;
//			// 保存数据
//			memcpy(pItems[nIdx].buf,ack.data,ack.len);
//			pItems[nIdx].nLen = ack.len;
//			int size = Size();
//			int nMove = 0;
//			int nLen = 0; 
//			int i;
//			//
//			// 计算目前窗口左边连续确认报文的个数
//			//
//			int nDataWrite = 0;
//			int nLeftData = 0;
//			char * pData;
//			for(i = 0;i < size;i++)
//			{
//				if(pItems[i].status == CONFIRM)
//				{
//					// 已经确认的数据写入缓冲
//					nLeftData = pItems[i].nLen;
//					pData = pItems[i].buf;
//					while(nLeftData > 0)
//					{
//						nDataWrite = pBuf->Write(pData,nLeftData);
//						pData += nDataWrite;
//						nLeftData -= nDataWrite;
//						if(nLeftData > 0)
//						{
//							//  缓冲已经满
//							pBuf->Flush();
//						}
//					}
//					nLen++;
//				}
//				else
//					break;
//			}
//			//
//			// 窗口左边如果有确认则窗口右移
//			//
//			if(nLen > 0)
//			{
//				nBeg += nLen;
//				nEnd += nLen;
//				nMove = size - nLen;
//
//				// 状态位左移
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
//			// 若窗口没有移动，则判断是否有报文需要快速重传
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
//									// 判断快速重传
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
// 被管理的接收文件
//------------------------------------------------------
// 接收数据包
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
	// 创建一个被管理的待接收文件
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
	// 将数据写入文件
	//------------------------------------------------------
	void Write(char * buf,int nLen)
	{
		if(nLen <= 0)
			return;
		if(m_bOpen)
		{
			if(m_nBufData + nLen > m_nBufLen)
			{
				// 先将缓冲数据写入文件
				if(m_nBufData > 0)
					m_pFile->Write(m_pDataBuf,m_nBufData);
				// 再写入新数据
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
	// 将缓冲数据写入文件
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
		//保存当前断点位置
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
	// 获取待下载文件的大小
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
	// 获取已经成功接收的文件位置
	//------------------------------------------------------
	size_t			GetOkPos()
	{ return m_nRecvSize + m_nStartPos; };

	size_t			GetStartPos()
	{ return m_nStartPos; };

	bool			Start(uint32 myId,uint32 destId,uint32 ip,uint16 port,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis);

	// 接收到数据包
	void			RecvPacket(uint32 pack_id, const char *pBuf, uint32 nSize);

protected:
	
	static  DWORD	WINAPI RecvThread(void *p);

	void			DoRecv(char * pBuf,int nBufLen);

protected:

	uint32			m_id;			// 文件标识符
	string			m_sPath;		// 文件保存路径
	size_t			m_nSize;		// 文件大小
	size_t			m_nPacketSize;	// 发送包大小
	size_t			m_nStartPos;	// 上次下载的断点位置

	DWORD			m_dwStartTick;  // 开始下载的时间tick

	CFile *			m_pFile;		// 文件
	bool			m_bOpen;		// 文件的打开状态
	bool			m_bWork;		// 文件是否正在下载
		

	char *			m_pDataBuf;		// 文件数据缓冲
	int				m_nBufLen;		// 缓冲区的大小
	int				m_nBufData;		// 缓冲区中数据的大小
	
private:
	
	HANDLE			m_hThread;		// 接收文件工作线程
	int				m_nCurRecvSize;	// 在定时内接收的字节数
	size_t			m_nRecvSize;	// 当前已经接收的数据大小
	
	uint32			m_nMyId;
	uint32			m_nDestId;
	uint32			m_destIp;
	uint16			m_destPort;
	uint32			m_tranIp;
	uint16			m_tranPort;

	XSOCKET			m_xs;
	CUdpNetTrans *	m_pDataTran;
	CAdvancedFileTransSession * m_pThis;
	int m_nRightPacketId;			// 正确接收到的数据包Id 
	map<int, STRU_RECV_PECKET_BUF*>	m_mapRecvPacket;
	HANDLE m_hMutex;				// 互斥量（保证对m_setPackArrive的操作唯一）
	BOOL m_bP2PTest;				// P2P测试是否成功
};


//------------------------------------------------------
// 提供数据访问服务的文件类
//------------------------------------------------------
// 数据包结构体
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
	// 建立一个待发送的文件
	//------------------------------------------------------
	P2PTRAN_ERR		Create(const char * szFile,uint32 fid,uint64 time);


	//------------------------------------------------------
	// 关闭文件
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

	// 开始文件传输
	bool			Start(int nStartPos,uint32 fid,uint32 myId,uint32 destId,uint32 destIp,uint16 destPort,uint32 tranIp,uint16 tranPort,CAdvancedFileTransSession * pThis);

	// 发送方接收到应答
	void			RecvArriveAck(uint32 pack_id);

private:
	// 添加未接收到确认信息的包
	void AddPacketId(uint32 pid)
	{
		WaitForSingleObject(m_hMutex,INFINITE);
		if(m_setPackArrive.find(pid) == m_setPackArrive.end())
			m_setPackArrive.insert(pid);
        ReleaseMutex(m_hMutex);
	}

	// 删除已收到确认信息的包
	void DelPacketId(uint32 pid)
	{
		WaitForSingleObject(m_hMutex,INFINITE);
		if(m_setPackArrive.find(pid) != m_setPackArrive.end())
			m_setPackArrive.erase(pid);
        ReleaseMutex(m_hMutex);
	}

	// 检查包集合是否为空
	bool PacketIdSetIsEmpty()
	{
		bool bEmpty = false;
		WaitForSingleObject(m_hMutex,INFINITE);
		bEmpty = m_setPackArrive.empty();
        ReleaseMutex(m_hMutex);
		return bEmpty;
	}

	// 检查包集合是否存在包
	bool ExistPacketId(uint32 pid)
	{
		bool bExist = false;
		WaitForSingleObject(m_hMutex,INFINITE);
		bExist = (m_setPackArrive.find(pid) != m_setPackArrive.end());
        ReleaseMutex(m_hMutex);
		return bExist;
	}

	// 添加线程句柄
	void AddThreadHandle(DWORD nThreadID, STRU_SEND_PACKET *pPacket)
	{
		WaitForSingleObject(m_hMutexTranThread,INFINITE);
		std::map<DWORD, STRU_SEND_PACKET*>::iterator it = m_mapTranThread.find(nThreadID);
		if( it == m_mapTranThread.end())
			m_mapTranThread[nThreadID] = pPacket;
		ReleaseMutex(m_hMutexTranThread);
	}

	// 删除线程句柄
	void DelThreadHandle(DWORD nThreadID)
	{
		WaitForSingleObject(m_hMutexTranThread,INFINITE);
		std::map<DWORD, STRU_SEND_PACKET*>::iterator it = m_mapTranThread.find(nThreadID);
		if( it != m_mapTranThread.end())
			m_mapTranThread.erase(it);
		ReleaseMutex(m_hMutexTranThread);
	}

	// 关闭所有线程
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

	// 发送数据包的线程（发送数据包好，间隔1s后检查数据包是否成功接收到，如果未接收，重发此数据包）
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
	
	int				m_nStartPos;	// 开始位置
	uint32			m_id;			// 文件ID
	string			m_sPath;		// 文件读取路径
	string			m_sName;		// 文件名
	size_t			m_nSize;		// 文件大小
	uint64			m_time;			// 文件修改时间
	CFile *			m_pFile;		// 文件

	bool			m_bOpen;		// 文件是否被打开
	int				m_nLastSuccSend;
	//int				m_nSendSize;	// 已经发送数据的大小
	//int				m_nCurSendSize;	// 当前一段时间发送的数据大小
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

	std::set<int> m_setPackArrive;	// 标示包是否顺利传达
	HANDLE m_hMutex;				// 互斥量（保证对m_setPackArrive的操作唯一）
	std::map<DWORD, STRU_SEND_PACKET*> m_mapTranThread;	// 保存开启的线程
	HANDLE m_hMutexTranThread;		// 互斥量（保证对m_setTranThread的操作唯一）
	char m_szSendBuf[10*TRANSER_PACKET_SIZE];	
	BOOL m_bP2PTest;				// P2P打洞是否成功
};