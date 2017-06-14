#pragma once

#include "XSound.h"
#include <vector>
#include <list>
#include <xstream.h>
#include <xstreamStack.h>
#include "XMp3Codec.h"
#include "Mp3File.h"

using namespace std;

#ifdef AUDIOPLAYERDLL_EXPORTS
#define AUDIOPLAYERDLL_API __declspec(dllexport)
#else
#define AUDIOPLAYERDLL_API __declspec(dllimport)
#endif


class XFileStream;

class XMp3Frame
{
protected:

	XMp3Frame(const XMp3Frame & o);

	void operator = (const XMp3Frame & o);

public:
	
	XMp3Frame(char * pData,int nLen)
	{
		assert(nLen > 0);
		m_pData = new byte[nLen];
		memcpy(m_pData,pData,nLen);
		m_nLen = nLen;
	}

	XMp3Frame(int nLen)
	{
		assert(nLen > 0);
		m_pData = new byte[nLen];
		m_nLen = nLen;
	}

	~XMp3Frame()
	{
		if(m_pData)
		{
			delete m_pData;
			m_pData = NULL;
		}
	}

	byte *	GetData()
	{
		return m_pData;
	}

	int				GetLen() const
	{
		return m_nLen;
	}

protected:

	int			m_nLen;

	byte *		m_pData;


};



class AUDIOPLAYERDLL_API  XMp3Sound : public XSound
{
public:

	XMp3Sound(XMp3Codec * pCodec);

	virtual ~XMp3Sound(void);

	enum OPEN_RESLUT{
		OPEN_OK = 0,
		OPEN_FAILED,
		OPEN_NEED_MORE,
		OPEN_FORMAT_ERR,
	};

	enum LOADFRAME_RESLUT{
		FRAME_OK,
		FRAME_NEED_MORE,
		FRAME_FORMAT_ERR,
		FRAME_END,
	};

public:

	OPEN_RESLUT				Open(const char * szFile,bool bLoadAll = false);

	OPEN_RESLUT				Open(XStream * pStream,bool bLoadAll = false);

	void					Clear();

	int						GetLeftAvailableTime();

	int						GetOrgDataSize();

public:

	virtual XSound::STATE	GetWaveData(void * pData,int & nLen);

	virtual int				GetCurTime();

	virtual XSound::STATE	Seek(int nTime);

	virtual int				GetTotalTime();

	virtual bool			IsEnd();

	virtual XSound::STATE	GetWaveFormat(WAVEFORMATEX & wfx);

protected:

	LOADFRAME_RESLUT		LoadFrame();


protected:

	XMp3Codec *			m_pCodec;

	vector<XMp3Frame *>	m_frames;

	XStreamStack		m_waveData;

	int					m_nCurFrame;

	bool				m_bLoadOk;

	bool				m_bOpen;

	XStream *			m_pStream;

	ID3V2_HEADER		m_id3v2h;

	MP3FREAMHEADER		m_frameHeader;

	byte *				m_pFrameHead;

	XMp3Frame *			m_pFrame;

	int					m_nFrameHead;

	int					m_nFrameCache;

	bool				m_bId3v2Flg;

	bool				m_bWaveFormatInit;

	ID3V1 *				m_pId3v1;

	XFileStream *		m_pMp3File;

	HWND				m_hCooperWnd;

};
