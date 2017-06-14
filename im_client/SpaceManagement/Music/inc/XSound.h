#pragma once

#include <Windows.h>
#include <MMReg.h>

#ifdef AUDIOPLAYERDLL_EXPORTS
#define AUDIOPLAYERDLL_API __declspec(dllexport)
#else
#define AUDIOPLAYERDLL_API __declspec(dllimport)
#endif

class  XSound
{
public:

	XSound(void);

	virtual ~XSound(void);

	enum STATE{
		OK = 0,
		NEED_MORE,
		EOSF,
		FAILED,
	};

public:

	virtual STATE		GetWaveData(void * pData,int & nLen) = 0;

	virtual int			GetCurTime() = 0;

	virtual STATE		Seek(int nTime) = 0;

	virtual int			GetTotalTime() = 0;

	virtual bool		IsEnd() = 0;

	virtual STATE		GetWaveFormat(WAVEFORMATEX & wfx) = 0;

};
