#include "StdAfx.h"
#include "SyncSemaphore.h"
#include ".\packetbuffer.h"
#include "PublicType.h"


CFIFOPacketBuffer::CFIFOPacketBuffer(void)
{
	m_pPacketData   = NULL;
	m_pPacketSize   = NULL;
	m_pDataLockFull	= NULL;
	m_pDataLockEmpty= NULL;
	m_nBufHead		= 0;
	m_nBufLength	= 0;

	SetBufferSize(MAX_BUF_PACKET_COUNT,sizeof(RecvDataStruct));
}

//参数：
//nCount--缓冲的包数
//nSize---每个包的大小，以字节为单位
CFIFOPacketBuffer::CFIFOPacketBuffer(int nCount,int nSize)
{
	m_pPacketData   = NULL;
	m_pPacketSize   = NULL;
	m_pDataLockFull	= NULL;
	m_pDataLockEmpty= NULL;
	m_nBufHead		= 0;
	m_nBufLength	= 0;

	SetBufferSize(nCount,nSize);

}

CFIFOPacketBuffer::~CFIFOPacketBuffer(void)
{
	SAFE_DELETE(m_pPacketData);
	SAFE_DELETE(m_pPacketSize);
	SAFE_DELETE(m_pDataLockFull);
	SAFE_DELETE(m_pDataLockEmpty);
}

//设置缓冲的大小
//nCount--包数
//nSize --包大小
void CFIFOPacketBuffer::SetBufferSize(int nMaxCount,int nMaxSize)
{

	if(nMaxCount*nMaxSize)
	{
		m_pPacketSize = new int[nMaxCount];
		m_pPacketData = new BYTE[nMaxCount*nMaxSize];
		if( NULL == m_pPacketData )
		{
			m_nMaxPacketCount	= 0;
			m_nMaxPacketSize	= 0;
		}
		else
		{
			m_nMaxPacketCount	= nMaxCount;
			m_nMaxPacketSize	= nMaxSize;

			m_pDataLockFull = new CSyncSemaphore(nMaxCount,nMaxCount);
			m_pDataLockEmpty= new CSyncSemaphore(0,nMaxCount);

		}
	}

	m_nBufHead		= 0;
	m_nBufLength = 0 ;
}

//添加一个新数据包
//pNewData-- 新数据的指针
//nSize ----　新数据包的大小
void CFIFOPacketBuffer::Push(BYTE* pNewData,int nSize)
{
	assert(pNewData);
	assert(nSize>0 && nSize<=m_nMaxPacketSize);

	if( nSize<0 || nSize>m_nMaxPacketSize )
	{
		return;
	}

	m_pDataLockFull->Lock();

	m_pDataLockOperate.Lock();

	//没满载才保存新的数据包，否则丢弃
	if( m_nBufLength < m_nMaxPacketCount )
	{
		int nNewInsertPos = (m_nBufHead+m_nBufLength)%m_nMaxPacketCount;
		memcpy((void*)(m_pPacketData + nNewInsertPos*m_nMaxPacketSize),pNewData,nSize);
		m_pPacketSize[nNewInsertPos] = nSize;
		++m_nBufLength;
	}
	m_pDataLockOperate.Unlock();

	m_pDataLockEmpty->Unlock();
}

//输出一个旧数据包
//pOutBuf-- 输出数据的保存指针
//nOutSize ---　输出数据包的大小
void CFIFOPacketBuffer::Pop(BYTE* pOutBuf,int& nOutSize)
{
	assert(pOutBuf);

	m_pDataLockEmpty->Lock();
	m_pDataLockOperate.Lock();

	if( m_nBufLength > 0 )
	{
		nOutSize = m_pPacketSize[m_nBufHead];
		memcpy( (void*)pOutBuf , (const void*)(m_pPacketData+m_nBufHead*m_nMaxPacketSize),nOutSize );
		m_pPacketSize[m_nBufHead] = 0;
		m_nBufHead = ++m_nBufHead % m_nMaxPacketCount;
		--m_nBufLength;
	}

	m_pDataLockOperate.Unlock();	
	m_pDataLockFull->Unlock();

}