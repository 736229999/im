#pragma once


//------------------------------------------------------
// 可读写数据流
//------------------------------------------------------

class XStream
{
public:

	enum RESULT
	{
		STREAM_NOT_OPEN = -3,
		NEED_MORE = -2,
		EOS = -1,
		OK = 0,
	};

public:

	virtual int			Pop(void * pData,int nLen) = 0;

	virtual int			Push(const void * pData,int nLen) = 0;

	virtual bool		IsOpen() = 0;

	virtual int			GetTotalSize() = 0;

	virtual int			GetSize() = 0;

};
