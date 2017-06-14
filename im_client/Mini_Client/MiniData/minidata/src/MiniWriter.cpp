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
		// 清空缓冲内容
		BUF_DATA::Clear();
		return;
	}
	BUF_DATA * pBuf = this;
	while(pBuf)
	{
		// 当前缓冲中数据的长度
		int nLen = int(pBuf->m_p - pBuf->m_pData);
		assert(nLen >= 0 && nLen <= (int)pBuf->m_nLen);
		// 保存当前缓冲中的数据
		if(nLen > 0)
		{
			m_pFile->Write(pBuf->m_pData,nLen);
		}
		pBuf = pBuf->m_pNext;
	}
	m_pFile->Flush();
	// 清除缓冲内容
	BUF_DATA::Clear();
}




_MINIDATA_END