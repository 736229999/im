// ----------------------------------------------------------------
// 文件:MMData.h
// 版本: 
// 作者: xy
// 日期: 2007-6-11
// ----------------------------------------------------------------
// 版本历史			注释
// ----------------------------------------------------------------
// v0.1				初始版本
// 
// ----------------------------------------------------------------

#pragma once

#pragma pack(1)
#pragma warning(disable:4200)

#define CMD_NOTIFY_FRAME_PER_SEC 0x1001

struct MM_NOTIFY_FRAME_PER_SEC
{
	unsigned short	nFramePerSec;
};

struct MMFrame
{
	DWORD	dwFlag;
};

struct MMAudioFrame
{

	static const DWORD FCC_AUDIO = 0x49445541;

	MMAudioFrame()
	{
		dwFlag = FCC_AUDIO;
		dwLen = 0;
	}

public:

	DWORD		dwFlag;
	DWORD		dwLen;
	char		data[0];

};

struct MMVideoFrame
{

	static const DWORD FCC_VIDEO = 0x45444956;

	MMVideoFrame()
	{
		dwFlag = FCC_VIDEO;
		dwLen = 0;
		dwTime = 0;
	}

public:

	DWORD		dwFlag;
	DWORD		dwTime;
	DWORD		dwLen;
	WORD		wRes;
	char		data[0];

};

struct MMControlFrame
{

	static const DWORD FCC_CONTROL = 0x4c525443;

	MMControlFrame()
	{
		dwFlag = FCC_CONTROL;
		dwCmd = 0;
		dwLen = 0;
	}

public:

	DWORD		dwFlag;
	DWORD		dwCmd;
	DWORD		dwLen;
	char		data[0];

};

#pragma pack()


//------------------------------------------------------
// 音频视频数据流解析器
//------------------------------------------------------

class CMMDataParse
{
public:

	explicit	CMMDataParse(int nBufLen = 1024 * 128);

	virtual		~CMMDataParse(void);

public:

	//------------------------------------------------------
	// 解析流媒体数据,返回已经解析的数据长度
	//------------------------------------------------------
	int				ParseData(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// 清除解析缓冲
	//------------------------------------------------------
	void			Clear();

protected:

	virtual bool	OnVideFrame(const BYTE * pData,int nLen) = 0;

	virtual bool	OnAudioFrame(const BYTE * pData,int nLen) = 0;

	virtual bool	OnControlFrame(const BYTE * pData,int nLen) = 0;

protected:

	BYTE *			m_pBuf;			// 解析缓冲区
	DWORD			m_nBufLen;		// 解析缓冲区长度
	DWORD			m_nDataLen;		// 数据区的长度
};


