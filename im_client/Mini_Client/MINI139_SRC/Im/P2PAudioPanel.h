#pragma once

#include "..\controls\voiceslider.h"
#include "..\mmtalk\mmtalker.h"
#include "..\Gips\GipsVoiceEngine.h"

// CP2PAudioPanel �Ի���

class CP2PAudioPanel : public CXSkinDlg
{
	DECLARE_DYNAMIC(CP2PAudioPanel)

public:
	CP2PAudioPanel(CMMTalker * pMMTalker, CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CP2PAudioPanel();

// �Ի�������
	enum { IDD = IDD_P2PAUDIO_PANEL };

public:
	void StartAudio(uint32 uiFriendIp);
	void StopAudio();
	void SetStatusText(CString str);
	
	void AudioCall();
	void AudioRecvRequest();
	void AudioStartTalk();



protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedBtnStop();	
	afx_msg void OnBnClickedBtnVoice();
	afx_msg void OnBnClickedBtnSound();
	afx_msg void OnBnClickedBtnAnswer();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);


public:	
	static const int m_WinW = 265;
	static const int m_WinH = 395; //�����ֱ����10��������ֻ��25


protected:
	CVoiceSliderCtrl m_wndSliderHorn;
	CVoiceSliderCtrl m_wndSliderSpeaker;
	CPngBtn			 m_btnSound;
	CPngBtn			 m_btnVoice;
	CPngBtn			 m_btnAnswer;
	CPngBtn 		 m_btnStop;
	CImage 			 m_imgIconOn;
	CImage			 m_imgIconOff;
	HBRUSH			 m_hBrushBackGround;

	enum AUDIO_STATUS { 
				NONE = 0,
				CALLING = 1, 
				RECV_REQUEST = 2,
				TALKING = 3} m_eAudioStatus;

protected:
	CMMTalker *		 m_pMMTalker;
	GipsVoiceEngineInterface *m_pVoiceEngin;
	int m_uVoiceChan;
public:
	afx_msg void OnClose();
};
