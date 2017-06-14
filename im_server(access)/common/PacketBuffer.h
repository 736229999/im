#pragma once

#include "SyncCritical.h"

//ҵ�����ݰ��Ļ�������࣬��һ���Ƚ��ȳ��Ķ��У�����������ȡ�����ݣ�����������ҵ���ദ��,֧�ֶ��߳�

//�������İ���
#define MAX_BUF_PACKET_COUNT     10000
//ÿ���������ߴ�
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
	BYTE*	m_pPacketData;			//�����ԭʼ����
	int*	m_pPacketSize;			//��¼ÿ��������Ĵ�С

	int		m_nMaxPacketCount;		//������
	int		m_nMaxPacketSize;			//ÿ����������ֽ���

	CSyncSemaphore*		m_pDataLockFull;	//���ݵ�ͬ��������,���ڿ����Ƿ񻺳�����
	CSyncSemaphore*		m_pDataLockEmpty;	//���ݵ�ͬ��������,���ڿ����Ƿ񻺳�Ϊ��
	CSyncCritical		m_pDataLockOperate;	//���������������ݵ�ͬ��������

	int		m_nBufHead;			//�����ͷ
	int		m_nBufLength;		//�����ʵ�ʴ�С

};
