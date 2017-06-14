#pragma once
#include "xstream.h"

//class XFileStream:public XStream
//{
//public:
//
//	XFileStream(const char * szFile);
//
//	virtual ~XFileStream();
//
//public:
//
//	virtual	bool	IsOpen()
//	{
//		return m_bOpen;
//	};
//
//	virtual RESULT	Pop(void * pData,int nLen)
//	{
//		if(m_bOpen == false)
//			return STREAM_NOT_OPEN;
//		int nRead;
//		/*if(m_ifs.eof())
//		return EOS;*/
//		if(pData)
//		{
//			nRead = m_file.Read((char*)pData,nLen);
//			if(nRead < nLen)
//				return EOS;
//		}
//		else
//		{
//			nRead = m_file.Seek(nLen,CFile::current);
//			if(nRead >= m_file.GetLength())
//				return EOS;
//		}
//		return OK;
//	};
//
//	virtual RESULT	Push(const void * pData,int nLen)
//	{
//		return OK;
//	}
//
//	virtual int		GetTotalSize();
//
//	virtual int		GetSize();
//
//protected:
//
//	bool			m_bOpen;
//
//	int				m_nTotalSize;
//
//	CFile			m_file;
//
//};