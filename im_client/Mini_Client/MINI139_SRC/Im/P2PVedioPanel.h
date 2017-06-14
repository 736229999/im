#pragma once

#include "..\controls\voiceslider.h"
#include "..\mmtalk\mmtalker.h"
#include "..\Gips\GipsVideoEngineInterface.h"
#include "..\Gips\GipsVoiceEngine.h"

// CP2PVedioPanel 对话框

class CP2PVedioPanel : public CXSkinDlg
{
	DECLARE_DYNAMIC(CP2PVedioPanel)

public:
	CP2PVedioPanel(CMMTalker * pMMTalker, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PVedioPanel();

// 对话框数据
	enum { IDD = IDD_P2PVEDIO_PANEL };

public:
	void StartVedio(int nCameraIndex, int uiFriendIp);
	void StopVedio();
	void SetStatusText(CString str);
	void VideoCall();
	void VideoRecvRequest();
	void VideoStartTalk();

public:
	void DrawVedioFrame(BYTE *pData);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnPaint();
	afx_msg void OnBnClickedBtnVedio();
	afx_msg void OnBnClickedBtnVoice();
	afx_msg void OnBnClickedBtnSound();
	afx_msg void OnBnClickedBtnAnswer();
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	afx_msg void OnBnClickedBtnStop();
	afx_msg void OnBnClickedBtnSnatch();
	afx_msg void OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar);
	DECLARE_MESSAGE_MAP()

public: 
		static const int m_WinW = 360;
		static const int m_WinH = 517;


protected:
	CVoiceSliderCtrl m_wndSliderHorn;
	CVoiceSliderCtrl m_wndSliderSpeaker;

	CFlatButton      m_btnVedio;
	CFlatButton      m_btnSnatch;

	CPngBtn			 m_btnStop;
	CPngBtn			 m_btnAnswer;
	CBitmap			 m_bmpLastFrame;


	CPngBtn 		 m_btnSound;
	CPngBtn 		 m_btnVoice;
	//CBmpBtn 		 m_btnStop;
	CImage			 m_imgIconOn;
	//CImage			 m_imgIconOff;
	HBRUSH			 m_hBrushBackGround;

	enum VIDEO_STATUS { 
				NONE = 0,
				CALLING = 1, 
				RECV_REQUEST = 2,
				TALKING = 3} m_eVideoStatus;



protected:
	CMMTalker *		 m_pMMTalker;
	//音频
	GipsVoiceEngineInterface *m_pVoiceEngin;
	int m_uVoiceChannel;
	//视频
	GipsVideoEngineInterface* m_pVideoEngie;
	int m_nVideoChannel;
public:
	afx_msg void OnClose();
};
