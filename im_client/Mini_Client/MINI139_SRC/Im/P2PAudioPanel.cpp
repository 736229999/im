// im\P2PAudioPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "P2PAudioPanel.h"
#include ".\p2paudiopanel.h"
#include ".\richeditctrlex.h"
#include "../tone.h"
// CP2PAudioPanel 对话框

IMPLEMENT_DYNAMIC(CP2PAudioPanel, CXSkinDlg)
CP2PAudioPanel::CP2PAudioPanel(CMMTalker * pMMTalker,CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CP2PAudioPanel::IDD, pParent)
{
	m_pMMTalker = pMMTalker;

	m_pVoiceEngin = NULL;
	m_uVoiceChan  = -1;
}

CP2PAudioPanel::~CP2PAudioPanel()
{
	if(m_pVoiceEngin)
		delete m_pVoiceEngin;
	
	m_imgIconOn.Destroy();
	m_imgIconOff.Destroy();
	DeleteObject(m_hBrushBackGround);
}

void CP2PAudioPanel::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);	
	DDX_Control(pDX, IDC_SLIDER_HORN,m_wndSliderHorn);
	DDX_Control(pDX, IDC_SLIDER_SPEAKER,m_wndSliderSpeaker);
	DDX_Control(pDX, IDC_SLIDER_SPEAKER,m_wndSliderSpeaker);
	
	DDX_Control(pDX, IDC_BUTTON_SOUND,m_btnSound);
	DDX_Control(pDX, IDC_BUTTON_VOICE,m_btnVoice);
	DDX_Control(pDX, IDC_BTN_ANSWER,m_btnAnswer);
	DDX_Control(pDX, IDC_BTN_STOP,m_btnStop);
}


BEGIN_MESSAGE_MAP(CP2PAudioPanel, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_SOUND, OnBnClickedBtnVoice)
	ON_BN_CLICKED(IDC_BUTTON_VOICE, OnBnClickedBtnSound)
	ON_BN_CLICKED(IDC_BTN_ANSWER, OnBnClickedBtnAnswer)
	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	
	ON_WM_HSCROLL()
	ON_WM_CTLCOLOR()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CP2PAudioPanel 消息处理程序

BOOL CP2PAudioPanel::PreTranslateMessage(MSG* pMsg)
{
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case 0x0D:
		case VK_ESCAPE:
			GetParent()->PostMessage(pMsg->message,pMsg->wParam,pMsg->lParam);
			return TRUE;
		default:
			;
		}  
	}

	return CXSkinDlg::PreTranslateMessage(pMsg);
}

void CP2PAudioPanel::OnPaint()
{
	CPaintDC dc(this); 

	DrawCaptionBar();
	DrawBorder();
/*
	CResBitmap bmpHorn, bmpSpeaker;
	bmpHorn.LoadBitmap(IDB_HORN);
	bmpSpeaker.LoadBitmap(IDB_MICROPHONE);

	CWnd *pWnd ;
	CRect rc;

	pWnd = GetDlgItem(IDC_ICON_HORN);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		::DrawBitmap(&dc,rc.left,rc.top,&bmpHorn,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_ICON_SPEAKER);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		::DrawBitmap(&dc,rc.left,rc.top,&bmpSpeaker,RGB(0,255,0));
	}
*/
	m_imgIconOn.Draw(dc,100,140,43,65);

	CRect rcWin;
	this->GetWindowRect(rcWin);
	ScreenToClient(&rcWin);


	CRect rcBar;
	rcBar.left = 1;
	rcBar.right = 264;

	rcBar.top = TITLE_BAR_CY + BORDER_W;
	rcBar.bottom = rcBar.top + 35;
	dc.FillSolidRect(&rcBar,RGB(0xDA,0xE7,0xEF));

	
	rcBar.top = 280;
	rcBar.bottom = rcBar.top + 35;
	dc.FillSolidRect(&rcBar,0x393939);


	rcBar.top = 315;
	rcBar.bottom = rcBar.top + 89;
	dc.FillSolidRect(&rcBar,RGB(0xDA,0xE7,0xEF));



}

BOOL CP2PAudioPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	//黑色背景
	pDC->FillSolidRect(&rc,RGB(0,0,0));
	return false;
}

void CP2PAudioPanel::AudioCall()
{
	m_eAudioStatus = CALLING;
	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_HIDE);

	
}

void CP2PAudioPanel::AudioRecvRequest()
{
	
	m_eAudioStatus = RECV_REQUEST;

	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_SHOW);
}

void CP2PAudioPanel::AudioStartTalk()
{
	m_eAudioStatus = TALKING;
	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_HIDE);
}




BOOL CP2PAudioPanel::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	m_eAudioStatus = NONE;

	SetDlgStyle(MINCLOSE);




	//拒绝按钮
	m_btnStop.MoveWindow(192,285,63,24);
	m_btnStop.SizeToContent();
	m_btnStop.SetTextColor(RGB(89,89,89));
	m_btnStop.SetPng(IDB_PNG_REFUSE_HOVER, IDB_PNG_REFUSED_NORMAL, IDB_PNG_REFUSED_PRESSED, IDB_PNG_REFUSED_NORMAL);
	m_btnStop.SetFont(&g_font12);

	//接受按钮
	m_btnAnswer.MoveWindow(119,285,63,24);
	m_btnAnswer.SizeToContent();
	m_btnAnswer.SetTextColor(RGB(89,89,89));
	m_btnAnswer.SetPng(IDB_PNG_ACCEPT_HOVER, IDB_PNG_ACCEPT_NORMAL, IDB_PNG_ACCEPT_PRESSED, IDB_PNG_ACCEPT_NORMAL);
	m_btnAnswer.SetFont(&g_font12);


	//麦克风音量
	m_btnSound.SetPng(IDB_PNG_SOUND_HOVER, IDB_PNG_SOUND_NORMAL, IDB_PNG_SOUND_PRESSED, IDB_PNG_SOUND_DISABLE);
	m_btnVoice.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);
	m_btnSound.MoveWindow(20,325,22,22);
	m_btnVoice.MoveWindow(20,355,22,22);



	m_wndSliderSpeaker.SetBackColor(RGB(0xDA,0xE7,0xEF));//黑色背景
	m_wndSliderHorn.SetBackColor(RGB(0xDA,0xE7,0xEF));
	m_wndSliderSpeaker.MoveWindow(45,325,60,22);
	m_wndSliderHorn.MoveWindow(45,355,60,22);

	//界面中间icon
	LoadImageFromResource(&m_imgIconOn, IDB_PNG_SOUND_ON, _T("PNG"));
	LoadImageFromResource(&m_imgIconOff, IDB_PNG_SOUND_OFF, _T("PNG"));

	m_hBrushBackGround =  CreateSolidBrush(RGB(0xDA,0xE7,0xEF)); //黑色背景
	
	GetDlgItem(IDC_STATIC_TIP)->SetFont(&g_font12);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&g_font12);
	GetDlgItem(IDC_STATIC_TIP)->MoveWindow(1 ,TITLE_BAR_CY + BORDER_W + 6 ,100,20);
	GetDlgItem(IDC_STATIC_TIME)->MoveWindow(151 ,TITLE_BAR_CY + BORDER_W + 6,100,20);
	
	return TRUE;  
}

afx_msg void CP2PAudioPanel::OnBnClickedBtnVoice()
{
}

afx_msg void CP2PAudioPanel::OnBnClickedBtnSound()
{

}


afx_msg void CP2PAudioPanel::OnBnClickedBtnAnswer()
{
	StopTone();
	switch(m_eAudioStatus)
	{
		//接受
		case RECV_REQUEST:
			StopTone();
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_ACCEPT,10086);
			//m_eAudioStatus = TALKING;
			AudioStartTalk();
			break;
		default: 
			break;
	}
}


afx_msg void CP2PAudioPanel::OnBnClickedBtnStop()
{
	StopTone();
	switch(m_eAudioStatus)
	{
		//取消语音通话
		case CALLING:
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_CANCEL,10086);
			break;
		//拒绝
		case RECV_REQUEST:
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_AUDIO_REFUSE,10086);
			break;
		//挂断
		case TALKING:
			GetParent()->PostMessage(WM_P2PAUDIO_NOTIFY,0,1);

			break;
		default:
			break;
	}
	m_eAudioStatus = NONE;
	this->ShowWindow(SW_HIDE);
}




void CP2PAudioPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if ( pScrollBar && m_pVoiceEngin)
	{
		if ( pScrollBar->m_hWnd == m_wndSliderHorn.m_hWnd )//调节扬声器音量
		{
			//m_pMMTalker->SetPlayVolume(m_wndSliderHorn.GetPos());
			m_pVoiceEngin->SetSpeakerVolume(m_wndSliderHorn.GetPos());
		}
		else
		if ( pScrollBar->m_hWnd == m_wndSliderSpeaker.m_hWnd )//麦克风音量
		{
			//m_pMMTalker->SetRecordVolume(m_wndSliderSpeaker.GetPos());
			m_pVoiceEngin->SetMicVolume(m_wndSliderSpeaker.GetPos());
		}
	}

	CXSkinDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}

void CP2PAudioPanel::StartAudio(uint32 uiFriendIp )
{/*
	if ( m_pMMTalker->InitAudio(m_hWnd) )
	{
		m_pMMTalker->StartAudioCap();
	}
	else
	{
		SetStatusText("初始化音频设备失败!");
	}
*/
	//语音
	if(m_pVoiceEngin == NULL)
		m_pVoiceEngin = GetGipsVoiceEngineByNew();

	if(m_pVoiceEngin == NULL)
		return;
	m_pVoiceEngin->Init();
	m_uVoiceChan = m_pVoiceEngin->CreateChannel();

	struct in_addr addr1;
    memcpy(&addr1, &uiFriendIp, 4);
	m_pVoiceEngin->SetSendIP(m_uVoiceChan,1234,inet_ntoa(addr1));
	TRACE("语音：对方IP  %s\n",inet_ntoa(addr1));

	m_pVoiceEngin->SetRecPort(m_uVoiceChan,1234);

	GIPS_CodecInst cinst;
	m_pVoiceEngin->GetCodec(0, &cinst);
	m_pVoiceEngin->SetSendCodec(m_uVoiceChan,&cinst);

/*
	int kMaxNumChannels = 8;
	int channel_index = 0;  
	std::vector<int> channels(kMaxNumChannels);  
	int i;
	for (i = 0; i < kMaxNumChannels; ++i) 
	{    
		channels[i] = pVoiceEngin->CreateChannel();    
		int port = 1234 + (i + 1) * 2;    
		pVoiceEngin->SetSendIP(channels[i], port, "10.211.240.8");    
		pVoiceEngin->SetRecPort(channels[i], port);    
		pVoiceEngin->SetSendCodec(channels[i], &cinst);    
	}
	*/
	bool VAD = false;
	bool AGC = true;
	bool AEC = false;
	bool NS = false;
	
	m_pVoiceEngin->SetAGCStatus(AGC);
	m_pVoiceEngin->SetAECStatus(AEC);	
	m_pVoiceEngin->SetNSStatus(NS);

	m_pVoiceEngin->StartListen(m_uVoiceChan);
	m_pVoiceEngin->StartPlayout(m_uVoiceChan);
	m_pVoiceEngin->StartSend(m_uVoiceChan);



	//m_wndSliderSpeaker.SetPos()
	//m_wndSliderHorn.SetPos(int iPos)
}

void CP2PAudioPanel::StopAudio()
{
	//m_pMMTalker->Stop();
	if(m_pVoiceEngin == NULL)
		return;
	m_pVoiceEngin->StopSend(m_uVoiceChan);	
	m_pVoiceEngin->StopPlayout(m_uVoiceChan);
	m_pVoiceEngin->StopListen(m_uVoiceChan);

    m_pVoiceEngin->DeleteChannel(m_uVoiceChan);  

/*
	while (channel_index > 0) 
	{     
		--channel_index;    
		res = file->StopPlayingFileAsMicrophone(channels[channel_index]);      
		VALIDATE;  
		res = base1->StopSend(channels[channel_index]);   
		VALIDATE;  
		res = base1->StopPlayout(channels[channel_index]);   
		VALIDATE;   
		res = base1->StopReceive(channels[channel_index]);     
		VALIDATE;   
	}
	for (i = 0; i < kMaxNumChannels; ++i){  
		channels[i] = base1->DeleteChannel(channels[i]); 
		VALIDATE; 
	}
	
*/
	
}

void CP2PAudioPanel::SetStatusText(CString str)
{
	GetDlgItem(IDC_STATIC_TIP)->SetWindowText(str);
}

HBRUSH CP2PAudioPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nID = pWnd->GetDlgCtrlID();

		if ( nID ==IDC_STATIC_TIP || nID == IDC_STATIC_TIME)
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_hBrushBackGround;
			pDC->SetTextColor(0x4f4f4f);
		}
	}

	return hbr;
}


void CP2PAudioPanel::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedBtnStop();

	CXSkinDlg::OnClose();
}
