// ----------------------------------------------------------------
// �ļ�:MMAudio.h
// �汾: 
// ����: xy
// ����: 2007-5-18
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------

#pragma once

#include <mmsystem.h>
#include <mmreg.h>

#include <Msacm.h>
#pragma  comment(lib,"Msacm32.lib")

#include "Mmsystem.h"
#pragma comment(lib,"Winmm.lib")
#include "XDataPool.h"

#include "DirectSound.h"

typedef  void (*AudioCapDataFnc)( BYTE * pCmpWave,int nLen,DWORD dwUser);


class CMMAudio
{
public:

	CDirectSound	m_ds;
	
	CMMAudio(void);
	
	~CMMAudio(void);

	static const int	CAPBUF_LEN = 1024 * 1;
	
public:

	//------------------------------------------------------
	// ������Ƶ�ɼ����峤��
	//------------------------------------------------------
	void					SetCapBufLen(int nLen);

	//------------------------------------------------------
	// ����ָ���ĸ�ʽ���ҵ���Ӧ����������ID
	//------------------------------------------------------
	static HACMDRIVERID		FindAcmDriverID(WORD wFormatTag);

	//------------------------------------------------------
	// �����������в���ָ����ʽ�����֧���򷵻ز��θ�ʽ��ָ���ⲿ�ͷ�
	//------------------------------------------------------
	static WAVEFORMATEX *	FindWaveFormat(HACMDRIVERID hadid,WORD wFormat);

	//------------------------------------------------------
	// ��ʼ¼��
	//------------------------------------------------------
	BOOL					StartRecord();

	//------------------------------------------------------
	// ֹͣ¼��
	//------------------------------------------------------
	void					StopRecord();
	
	//------------------------------------------------------
	// ����ѹ������Ƶ����ص�����
	//------------------------------------------------------
	void					SetCallBackOnCmpData(AudioCapDataFnc pFnc,DWORD dwUser);

	//------------------------------------------------------
	// ������Ƶ���� 
	//------------------------------------------------------
	int						PlaySound(const BYTE * pData,int nLen);
		
	//------------------------------------------------------
	// ѹ����Ƶ���� 
	//------------------------------------------------------
	BYTE *					CmpWaveData(/*IN*/BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// ��ѹ��Ƶ����
	//------------------------------------------------------
	BYTE *					DecmpWaveData(/*IN*/BYTE * pData,/*IN-OUT*/int & nLen);

protected:

	//------------------------------------------------------
	// ��Ƶ����
	//------------------------------------------------------
	void					OnWaveData(WAVEHDR * pData);

	//------------------------------------------------------
	// �ر���Ƶ�����¼�
	//------------------------------------------------------
	void					OnWaveInClose();

	//------------------------------------------------------
	// ��ʼ��
	//------------------------------------------------------
	BOOL					Init();

	//------------------------------------------------------
	// ��Ƶ����������
	//------------------------------------------------------
	void					OnWaveOutDone();


private:

	//------------------------------------------------------
	// �ڲ�ʹ�õĻص�����
	//------------------------------------------------------
	static void CALLBACK WaveInProc( HWAVEIN hwi, UINT uMsg,DWORD dwInstance, DWORD dwParam1,DWORD dwParam2);

	//------------------------------------------------------
	// �ڲ�ʹ�õĻص�����
	//------------------------------------------------------
	static void CALLBACK WaveOutProc(HWAVEOUT hwo,UINT uMsg,DWORD dwInstance,DWORD dwParam1,DWORD dwParam2);


protected:

	WAVEFORMATEX			m_waveformIn;		// ��Ƶ��׽��ʽ
	
	HWAVEIN					m_hWaveIn ;			// ��Ƶ�����豸

	HWAVEOUT				m_hWaveOut;			// ��Ƶ����豸

	AudioCapDataFnc			m_pCallBackCmpData;	// ��ѹ������Ƶ��׽����

	DWORD					m_dwUser;			// �û�����

	PWAVEHDR				m_pInHeader1;		// ��Ƶ���뻺��1
	
	PWAVEHDR				m_pInHeader2;		// ��Ƶ���뻺��2

	PWAVEHDR				m_pOutHeader1;		// ��Ƶ�������1

	PWAVEHDR				m_pOutHeader2;		// ��Ƶ�������2

	BYTE *					m_pInBuf1;			// ��Ƶ�������ݻ���1

	BYTE *					m_pInBuf2;			// ��Ƶ�������ݻ���2

	BYTE *					m_pOutBuf1;			// ��Ƶ������ݻ���1

	BYTE *					m_pOutBuf2;			// ��Ƶ������ݻ���2

	BYTE *					m_pCmpData;			// ��ѹ������

	BYTE *					m_pCmpBufPcm;		// pcmѹ��������

	BYTE *					m_pDecmpData;		// ��ѹ������

	BYTE *					m_pCmpBufDst;		// Ŀ�ĸ�ʽѹ��������

	HACMDRIVERID			m_acmDID;			// ѹ��������id

	HACMDRIVER				m_had;				// ѹ����������
	
	WAVEFORMATEX *			m_pWfxCmp;			// ѹ����ʽ

	WAVEFORMATEX *			m_pWfxPcm;			// pcm�м��ʽ

	int						m_nCapBufLen;		// ��Ƶ�ɼ����峤��

	int						m_nCmpBufLen;		// ��Ƶѹ�����峤��

	bool					m_bRecording;		// �Ƿ���¼��

	XDataPool				m_audioData;		// ����ɼ�����Ƶ����

	//Common::CXEvent			m_eventPlay;		// ��Ƶ��������¼�
	
	bool					m_bPlayOk;			// ��Ƶ�������
};
