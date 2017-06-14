#pragma once

#include "SyncCritical.h"

//业务数据包的缓冲管理类，是一个先进先出的队列，该类从网络层取得数据，缓存下来供业务类处理,支持多线程

//缓冲管理的包数
#define MAX_BUF_PACKET_COUNT     10000
//每个包的最大尺寸
#define MAX_BUF_PACKET_SIZE		 MAX_RECV_BUFFERS


class CSyncSemaphore;

class CFIFOPacketBuffer
{
public:
	CFIFOPacketBuffer(void);
	CFIFOPacketBuffer(int nCount,int nSize);
	~CFIFOPacketBuffer(void);

	void SetBufferSize(int nMaxCount ,int nMaxSize );

	void Push(BYTE* pNewData,int nSize);

	void Pop(BYTE* pOutBuf,int& nOutSize);

	int GetSize() { if(this){  return m_nBufLength;}else{return 0;}	}

private:
	BYTE*	m_pPacketData;			//缓冲的原始数据
	int*	m_pPacketSize;			//记录每个缓冲包的大小

	int		m_nMaxPacketCount;		//最大包数
	int		m_nMaxPacketSize;			//每个包的最大字节数

	CSyncSemaphore*		m_pDataLockFull;	//数据的同步互斥锁,用于控制是否缓冲满了
	CSyncSemaphore*		m_pDataLockEmpty;	//数据的同步互斥锁,用于控制是否缓冲为空
	CSyncCritical		m_pDataLockOperate;	//用于真正操作数据的同步互斥锁

	int		m_nBufHead;			//缓冲的头
	int		m_nBufLength;		//缓冲的实际大小

};
