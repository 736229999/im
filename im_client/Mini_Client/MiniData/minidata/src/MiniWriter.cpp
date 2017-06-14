#include "StdAfx.h"
#include "../inc/miniwriter.h"

_MINIDATA_BEG

CMiniWriter::CMiniWriter(CFile * pF /* = NULL */,uint32 nBufSize /* = BUF_DATA_DEFAULT_SIZE */):BUF_DATA(nBufSize)
{
	m_pFile = pF;
}

CMiniWriter::~CMiniWriter(void)
{

}

void CMiniWriter::Save()
{
	assert(m_pFile);
	if(m_pFile == NULL)
	{
		// ��ջ�������
		BUF_DATA::Clear();
		return;
	}
	BUF_DATA * pBuf = this;
	while(pBuf)
	{
		// ��ǰ���������ݵĳ���
		int nLen = int(pBuf->m_p - pBuf->m_pData);
		assert(nLen >= 0 && nLen <= (int)pBuf->m_nLen);
		// ���浱ǰ�����е�����
		if(nLen > 0)
		{
			m_pFile->Write(pBuf->m_pData,nLen);
		}
		pBuf = pBuf->m_pNext;
	}
	m_pFile->Flush();
	// �����������
	BUF_DATA::Clear();
}




_MINIDATA_END