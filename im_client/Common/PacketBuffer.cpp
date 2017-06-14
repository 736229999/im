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

//������
//nCount--����İ���
//nSize---ÿ�����Ĵ�С�����ֽ�Ϊ��λ
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

//���û���Ĵ�С
//nCount--����
//nSize --����С
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

//���һ�������ݰ�
//pNewData-- �����ݵ�ָ��
//nSize ----�������ݰ��Ĵ�С
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

	//û���زű����µ����ݰ���������
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

//���һ�������ݰ�
//pOutBuf-- ������ݵı���ָ��
//nOutSize ---��������ݰ��Ĵ�С
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