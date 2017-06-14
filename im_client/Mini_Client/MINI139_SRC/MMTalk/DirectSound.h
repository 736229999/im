// ----------------------------------------------------------------
// �ļ�:DirectSound.h
// �汾: 
// ����: xy
// ����: 2007-5-29
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------
#pragma once

#include "MMCommon.h"
#include "XDataPool.h"
#include <fcl.h>
using namespace fcl;

typedef void (*AudioCapDataFnc)( BYTE * pWave,int nLen,DWORD dwUser);

typedef void (*WaveNotifyFnc)(BYTE val,DWORD dwUser);

//------------------------------------------------------
// ��Ƶ���Ż�����
//------------------------------------------------------

//class CAudioBuf
//{
//public:
//
//	friend class CDirectSound;
//
//	CAudioBuf()
//	{
//		m_nExtSize = 0;
//		m_nNotifySize = 0;
//		m_nIntSize = 0;
//		m_pDSB = NULL;
//		m_nNeedBuff = 0;
//		m_nBuffingC = 0;
//		m_nLastPlayPos = 0;
//		m_bHasData = false;
//		m_nLeft = 0;
//		m_nLastNotify = 0;
//	};
//
//	~CAudioBuf()
//	{
//
//	};
//
//
//protected:
//
//	//------------------------------------------------------
//	// ���ⲿ���������д���ڲ�����
//	//------------------------------------------------------
//	void					WriteBuf();
//
//	//------------------------------------------------------
//	// ��ʼ����Ƶ���Ż����� 
//	//------------------------------------------------------
//	void					Init(LPDIRECTSOUNDBUFFER pDSB,int nExtBufSize,BYTE bFill,int nNotifySize,int nNotifyCount)
//	{
//		m_pDSB = pDSB;
//		m_nExtSize = nExtBufSize;
//		m_nIntSize = nNotifySize * nNotifyCount;
//		m_nNotifySize = nNotifySize;
//		m_dataPool.SetStackLen(nExtBufSize);
//		m_nLastNotify = m_nIntSize - nNotifySize;
//		m_bFill = bFill;
//	}
//
//	//------------------------------------------------------
//	// ��ȡ�ⲿ����Ĵ�С
//	//------------------------------------------------------
//	int						GetSize() const
//	{ return m_nExtSize; };
//
//
//	//------------------------------------------------------
//	// ������ݵ���Ƶ���Ż�������������úͻ�����Сһ��
//	//------------------------------------------------------
//	int						PushData(const BYTE * pData,int nLen);
//
//
//	//------------------------------------------------------
//	// ��Ƶ�鲥�Ž���֪ͨ
//	//------------------------------------------------------
//	void					OnBufNotify(int nPos);
//
//	//------------------------------------------------------
//	// 
//	//------------------------------------------------------
//	void					SetBuffing(int nBuffing)
//	{
//		m_nNeedBuff = nBuffing > m_nNotifySize ?  nBuffing : m_nNotifySize;
//		if(m_nNeedBuff > m_nIntSize)
//			m_nNeedBuff = m_nIntSize;
//		m_nBuffingC = m_nNeedBuff;
//		if(nBuffing == 0)
//		{
//			m_nNeedBuff = 0;
//			m_nBuffingC = 0;
//		}
//	}
//
//	void					RestoreBuff();
//
//	LPDIRECTSOUNDBUFFER		m_pDSB;			// directSound����
//
//	int						m_nExtSize;		// ��ǰ�ⲿ����Ĵ�С
//
//	int						m_nNotifySize;	// ֪ͨ�����С
//
//	int						m_nIntSize;		// �ڲ�����Ĵ�С
//
//	//int						m_nIntDataSize;	// �ڲ��������ݵĴ�С
//
//	int						m_nLastPlayPos;	// �ϴβ���λ��
//
//	BYTE					m_bFill;		// ��侲��
//
//	XStreamStack			m_dataPool;		// ���ݻ�����
//
//	int						m_nNeedBuff;	// ����ʱ��Ҫ��������ݴ�С
//
//	int						m_nBuffingC;	// ����ʱ��Ƶ�������Ĵ�С
//
//	//int						m_nDataEndPos;	// �ڲ����������ݵĽ�βλ��
//
//	bool					m_bHasData;		// �ڲ��������Ƿ��������
//
//	//bool					m_bCarry;		// ���ݵ�ַ�Ƿ��н�λ
//
//	int						m_nLastNotify;	// �ϴ�֪ͨ��λ��
//
//	//int						m_nDataBeg;		// ���ݿ�ʼ��ַ
//
//	int						m_nDataEnd;		// ���ݽ�����ַ
//
//	int						m_nLeft;			// �ڲ�������ʣ��û�в��ŵ����ݳ���
//};

class CAudioBuf
{
public:

	friend class CDirectSound;

	CAudioBuf()
	{
		m_nExtSize = 0;
		m_nNotifySize = 0;
		m_nIntSize = 0;
		m_pDSB = NULL;
		m_nNeedBuff = 0;
		m_nBuffingC = 0;
		m_nLastPlayPos = 0;
		m_bHasData = false;
		m_nLeft = 0;
		m_nLastNotify = 0;
		m_nNotifyCount = 0;
	};

	~CAudioBuf()
	{

	};


protected:

	//------------------------------------------------------
	// ���ⲿ���������д���ڲ�����
	//------------------------------------------------------
	void					WriteBuf();

	//------------------------------------------------------
	// ��ʼ����Ƶ���Ż����� 
	//------------------------------------------------------
	void					Init(LPDIRECTSOUNDBUFFER pDSB,int nExtBufSize,BYTE bFill,int nNotifySize,int nNotifyCount)
	{
		m_pDSB = pDSB;
		m_nExtSize = nExtBufSize;
		m_nIntSize = nNotifySize * nNotifyCount;
		m_nNotifyCount = nNotifyCount;
		m_nNotifySize = nNotifySize;
		m_dataPool.SetStackLen(nExtBufSize);
		m_nLastNotify = m_nIntSize - nNotifySize;
		m_bFill = bFill;
	}

	//------------------------------------------------------
	// ��ȡ�ⲿ����Ĵ�С
	//------------------------------------------------------
	int						GetSize() const
	{ return m_nExtSize; };


	//------------------------------------------------------
	// ������ݵ���Ƶ���Ż�������������úͻ�����Сһ��
	//------------------------------------------------------
	int						PushData(const BYTE * pData,int nLen);


	//------------------------------------------------------
	// ��Ƶ�鲥�Ž���֪ͨ
	//------------------------------------------------------
	void					OnBufNotify(int nPos);

	//------------------------------------------------------
	// 
	//------------------------------------------------------
	void					SetBuffing(int nBuffing)
	{
		m_nNeedBuff = nBuffing > m_nNotifySize ?  nBuffing : m_nNotifySize;
		if(m_nNeedBuff > m_nIntSize)
			m_nNeedBuff = m_nIntSize;
		m_nBuffingC = m_nNeedBuff;
		if(nBuffing == 0)
		{
			m_nNeedBuff = 0;
			m_nBuffingC = 0;
		}
	}

	void					RestoreBuff();

	int						m_nNotifyCount;

	LPDIRECTSOUNDBUFFER		m_pDSB;			// directSound����

	int						m_nExtSize;		// ��ǰ�ⲿ����Ĵ�С

	int						m_nNotifySize;	// ֪ͨ�����С

	int						m_nIntSize;		// �ڲ�����Ĵ�С

	//int						m_nIntDataSize;	// �ڲ��������ݵĴ�С

	int						m_nLastPlayPos;	// �ϴβ���λ��

	BYTE					m_bFill;		// ��侲��

	XStreamStack			m_dataPool;		// ���ݻ�����

	int						m_nNeedBuff;	// ����ʱ��Ҫ��������ݴ�С

	int						m_nBuffingC;	// ����ʱ��Ƶ�������Ĵ�С

	//int						m_nDataEndPos;	// �ڲ����������ݵĽ�βλ��

	bool					m_bHasData;		// �ڲ��������Ƿ��������

	//bool					m_bCarry;		// ���ݵ�ַ�Ƿ��н�λ

	int						m_nLastNotify;	// �ϴ�֪ͨ��λ��

	//int						m_nDataBeg;		// ���ݿ�ʼ��ַ

	int						m_nDataEnd;		// ���ݽ�����ַ

	int						m_nLeft;			// �ڲ�������ʣ��û�в��ŵ����ݳ���
};



//------------------------------------------------------
// ��Ƶ������
//------------------------------------------------------

class CDirectSound
{
public:
	
	CDirectSound(void);

	~CDirectSound(void);

	//static const int AUDIO_EXTBUF_SIZE = 1024 * 50;			// �ⲿ�����С

	static const int AUDIO_BUF_COUNT = 6;					// �ڲ��������

	//static const int AUDIO_NOTIFY_SIZE = 1024 * 2  ;		// �ڲ�����֪ͨ��С

public:

	//------------------------------------------------------
	// ����
	//------------------------------------------------------
	void						Play(const BYTE * pData,int nLen);

	//------------------------------------------------------
	// ���ų�ʼ��
	//------------------------------------------------------
	BOOL						Init(
		HWND			hWnd,
		WAVEFORMATEX *	pWfx,
		int				nBufNotifySize 	,	// �ڲ�����֪ͨ��С
		int				nExtBufSize			// �ⲿ�����С
	);

	//------------------------------------------------------
	// �ò���������
	//------------------------------------------------------
	void						SetBuffering(int nBuffing)
	{ 
		m_audioBuf.SetBuffing(nBuffing);
	};

	//------------------------------------------------------
	// ��������(0~100)
	//------------------------------------------------------
	void						SetVolume(int nPercent);

	//------------------------------------------------------
	// ��ȡ����(0~100)
	//------------------------------------------------------
	int							GetVolume();

	//------------------------------------------------------
	// ֹͣ
	//------------------------------------------------------
	void						Stop();

protected:

	//------------------------------------------------------
	// �����߳�
	//------------------------------------------------------
	static DWORD WINAPI			PlayThread(LPVOID lpParam);

	//------------------------------------------------------
	// ����״̬֪ͨ
	//------------------------------------------------------
	void						PlayNotify();

protected:

	LPDIRECTSOUNDBUFFER			m_pDSB;							// directSound����

	LPDIRECTSOUND				m_pDirectSound;					// directSound���� 

	//HANDLE *					m_phNotifEvents;				// �������֪ͨ

	HANDLE						m_hNotifyEvent;					// �������֪ͨ�¼�

	HANDLE						m_hStopEvent;					// ֹͣ�����¼�

	int							m_nCurNotify;					// ��ǰ֪ͨ��λ��

	DSBPOSITIONNOTIFY *			m_pPosNotify;					// ����֪ͨ��־������

	HANDLE						m_hThreadPlay;					// ��Ƶ�����߳�

	WAVEFORMATEX 				m_wfx;							// ��ǰ���Ÿ�ʽ

	CAudioBuf					m_audioBuf;						// �����ŵ���Ƶ������

	bool						m_bWork;
	//HANDLE						m_hCanPush;						// ��Ƶ�������ʹ�õ��¼�


};



//------------------------------------------------------
// ��Ƶ��׽��
//------------------------------------------------------

class CMMAudioCap: public ITimerSink
{
public:
	
	CMMAudioCap();

	~CMMAudioCap();

	static const int	NUM_REC_NOTIFICATIONS = 8;

	static const int	SIZE_REC_BLOCK = 1024 * 4 ;

	static const int	NUM_CAP_BUF = 16;

public:

	//------------------------------------------------------
	// ��ʼ��¼���豸
	//------------------------------------------------------
	BOOL						Init(
		WAVEFORMATEX * pWfx,
		AudioCapDataFnc pFnc,
		DWORD dwUser
	);

	//------------------------------------------------------
	// ��ʼ��Ƶ�ɼ�
	//------------------------------------------------------
	void						Start();

	//------------------------------------------------------
	// ֹͣ��Ƶ�ɼ�
	//------------------------------------------------------
	void						Stop();

	//------------------------------------------------------
	// ���ò���֪ͨʱ����(��λms)
	//------------------------------------------------------
	void						SetWaveNotify(WaveNotifyFnc pFnc,DWORD dwUser,int nTime);

	//------------------------------------------------------
	// ������Ƶ��������(0 - 100)
	//------------------------------------------------------
	void						SetCapVolume(DWORD nV);

protected:

	//------------------------------------------------------
	// ���ݲ����߳�
	//------------------------------------------------------
	static DWORD WINAPI			ThreadRecord(LPVOID lpParam) ;

	//------------------------------------------------------
	// ��Ƶ���ݴ������߳�
	//------------------------------------------------------
	static DWORD WINAPI			ThreadWork(LPVOID lpParam);

	//------------------------------------------------------
	// ���ݲ����߳�
	//------------------------------------------------------
	void						ProcRecord();

	//------------------------------------------------------
	// ��Ƶ���ݴ������߳�
	//------------------------------------------------------
	void						ProcWork();

	//------------------------------------------------------
	// ���ݲ���֪ͨ 
	//------------------------------------------------------
	void						OnCapturedNotify();
	
	//------------------------------------------------------
	// ����֪ͨ��ʱ��Ӧ
	//------------------------------------------------------
	void						onTimer(uint32 lEventID );

	//------------------------------------------------------
	// ����������
	//------------------------------------------------------
	void						ProcWaveData(BYTE * pData,int nLen);

	//------------------------------------------------------
	// �ж������Ƿ�Ϊ����
	//------------------------------------------------------
	bool						IsSilence(BYTE * pData,int nLen);

	//------------------------------------------------------
	// �����Ƶ��·��Ϣ
	//------------------------------------------------------
	BOOL						GetLineInfo(LPMIXERLINE pmxl, DWORD dwComponentType);

	//------------------------------------------------------
	// ��ȡ��·������Ϣ
	//------------------------------------------------------
	BOOL						GetLineControl(LPMIXERCONTROL pmxc, LPMIXERLINE pmxl, DWORD dwType);

	//------------------------------------------------------
	// ������������
	//------------------------------------------------------
	void						SetControlDetails(DWORD dwControlID,DWORD dwValue);


protected:

	typedef fclList<int>		MMList ;

	WaveNotifyFnc				m_pNotifyFnc;			// ����֪ͨ�ص�����

	DWORD						m_dwUserNotify;			// �û�����

	int							m_nNotifyTime;			// ֪ͨʱ����

	MMList						m_lstWave;				// ���β���

	int							m_nNotify;				// ֪ͨ�������

	BYTE						m_bSilence;				// ����

	bool						m_bTimer;				// �Ƿ��ж�ʱ��

	LPDIRECTSOUNDCAPTURE		m_pDSC ;				// ¼���豸����ָ�� 

	LPDIRECTSOUNDCAPTUREBUFFER	m_pDSBCapture;			// ¼�뻺��������ָ��
	
	WAVEFORMATEX				m_wfxInput;				// �������Ƶ��ʽ

	DSBPOSITIONNOTIFY			m_aPosNotify[ NUM_REC_NOTIFICATIONS + 1 ]; //����֪ͨ��־������
	
	HANDLE						m_hNotificationEvent;	// ֪ͨ�¼� 

	HANDLE						m_hQuitRecord;			// �˳�¼���¼�

	HANDLE						m_hQuitWork;			// �˳������¼�

	HANDLE						m_hThreadWork;			// �����߳�
	
	HANDLE						m_hThreadNotify;		// ¼��֪ͨ�߳� 

	HANDLE						m_hSemSignal;			// �ź���
	
	DWORD						m_dwCaptureBufferSize;	// ¼���û������Ĵ�С
	
	DWORD						m_dwNextCaptureOffset;	// ƫ��λ��
	
	DWORD						m_dwNotifySize;			// ֪ͨλ��

	AudioCapDataFnc				m_pCallback;			// ��Ƶ���ݲ�׽֪ͨ�ص�����

	DWORD						m_dwUser;				// �û�����

	XDataPool					m_audioData;			// �������Ƶ���ݻ���

	UINT						m_uMixID;				// �������豸ID

	DWORD						m_dwMax;				// �������

	DWORD						m_dwMin;				// ��С����

};

//------------------------------------------------------
// ��Ƶѹ����ѹ��
//------------------------------------------------------

class CMMAudioCodec
{
public:
	
	CMMAudioCodec();

	~CMMAudioCodec();

public:

	//------------------------------------------------------
	// ����ָ���ĸ�ʽ���ҵ���Ӧ����������ID
	//------------------------------------------------------
	static HACMDRIVERID			FindAcmDriverID(WORD wFormatTag);

	//------------------------------------------------------
	// �����������в���ָ����ʽ�����֧���򷵻ز��θ�ʽ��ָ���ⲿ�ͷ�
	//------------------------------------------------------
	static WAVEFORMATEX *		FindWaveFormat(HACMDRIVERID hadid,WAVEFORMATEX * pWfx);

	//------------------------------------------------------
	// ��ָ��ѹ����ʽ��ʼ��������
	//------------------------------------------------------
	BOOL						Init(
		WAVEFORMATEX * pWfxSrc,
		WAVEFORMATEX * pWfxCmp,
		int nSrcBufLen = CMMAudioCap::SIZE_REC_BLOCK
	);

	//------------------------------------------------------
	// ѹ����Ƶ����,���볤�ȳ������峤�Ȼ�ض� 
	//------------------------------------------------------
	BYTE *						CmpWaveData(/*IN*/ BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// ��ѹ��Ƶ����
	//------------------------------------------------------
	BYTE *						DecmpWaveData(/*IN*/ BYTE * pData,/*IN-OUT*/int & nLen);

	//------------------------------------------------------
	// ��ȡ�м���Ƶ��ʽ
	//------------------------------------------------------
	WAVEFORMATEX *				GetPcmWaveFormat()
	{ return m_pWfxPcm; };

	//------------------------------------------------------
	// ��ȡԴ��Ƶ��ʽ
	//------------------------------------------------------
	WAVEFORMATEX *				GetSrcWaveFormat()
	{ return &m_wfxSrc; };
	
	//------------------------------------------------------
	// ��ȡѹ����Ƶ��ʽ
	//------------------------------------------------------
	WAVEFORMATEX *				GetCmpWaveFormat()
	{ return m_pWfxCmp; };

	//------------------------------------------------------
	// ��ȡ�м��ʽ���峤��
	//------------------------------------------------------
	int							GetPcmLen()
	{
		int nPcmLen = (int)((double)m_nSrcBufLen * ((double)m_pWfxPcm->nAvgBytesPerSec / (double)m_wfxSrc.nAvgBytesPerSec));
		nPcmLen = (nPcmLen + 3) / 4 * 4;
		return nPcmLen;
	};

protected:

	void						Clear();

private:

	static BOOL CALLBACK		FindFormatProc(HACMDRIVERID hadid, LPACMFORMATDETAILS pafd, DWORD dwInstance, DWORD fdwSupport) ;

	static BOOL CALLBACK		FindDriverProc(HACMDRIVERID hadid, DWORD dwInstance, DWORD fdwSupport); 


protected:

	WAVEFORMATEX 				m_wfxSrc;			// Դ��ʽ

	WAVEFORMATEX *				m_pWfxCmp;			// ѹ����ʽ

	WAVEFORMATEX *				m_pWfxPcm;			// �м��ʽ

	HACMDRIVERID				m_acmDID;			// ѹ��������id

	HACMDRIVER					m_had;				// ѹ����������

	BYTE *						m_pBufPcm;			// �м��ʽ����

	BYTE *						m_pBufCmp;			// ѹ������

	BYTE *						m_pBufDecmp;		// ��ѹ����

	int							m_nSrcBufLen;		// Դ���ݻ��峤�� 

	int							m_nPcmBufLen;		// pcm���峤��

	int							m_nCmpBufLen;		// cmp���峤��

};

