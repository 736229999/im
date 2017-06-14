#pragma once
#include "lame.h"
#include <Windows.h>
#include <MMReg.h>

typedef unsigned char byte;

#ifdef AUDIOPLAYERDLL_EXPORTS
#define AUDIOPLAYERDLL_API __declspec(dllexport)
#else
#define AUDIOPLAYERDLL_API __declspec(dllimport)
#endif

class AUDIOPLAYERDLL_API  XMp3Codec
{
protected:
	
	XMp3Codec(const XMp3Codec & codec);

	void operator = (const XMp3Codec & codec);

public:

	XMp3Codec(void);

	~XMp3Codec(void);

public:

	bool					Init();
	
	const byte *			Decode(byte * pMp3Data,int & nLen);

	const WAVEFORMATEX *	GetWaveFormat() const;

	int						GetTotalFrames(int nFileLen);

	double					GetFrameTime();

	bool					IsVBR();

protected:

	void					MakePcm(int nLen);

protected:

	lame_global_flags *	m_gf;
	
	short *				m_pcm_r;

	short *				m_pcm_l;

	short *				m_pcm;

	int					m_nPcm;

	int					m_nBuf;

	mp3data_struct		m_mp3data;

	int					m_enc_delay;             /* if decoder finds a Xing header */ 

	int					m_enc_padding;           /* if decoder finds a Xing header */ 

	bool				m_bWfx;

	WAVEFORMATEX		m_wfx;

	bool				m_bDecodeInit;

	double				m_fFrameTime;

};
