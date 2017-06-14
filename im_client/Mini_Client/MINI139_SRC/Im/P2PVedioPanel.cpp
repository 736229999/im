// im\P2PVedioPanel.cpp : 实现文件
//

#include "stdafx.h"
#include "..\ImClient.h"
#include "P2PVedioPanel.h"
#include ".\p2pvediopanel.h"
#include ".\richeditctrlex.h"
#include "../tone.h"

// CP2PVedioPanel 对话框

IMPLEMENT_DYNAMIC(CP2PVedioPanel, CXSkinDlg)
CP2PVedioPanel::CP2PVedioPanel(CMMTalker *pMMTalker, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CP2PVedioPanel::IDD, pParent)
{
	m_pMMTalker       = pMMTalker;
	m_pVideoEngie = NULL;
	m_nVideoChannel = -1;
}

CP2PVedioPanel::~CP2PVedioPanel()
{
	if(m_pVideoEngie)
		delete m_pVideoEngie;

	m_imgIconOn.Destroy();
	//m_imgIconOff.Destroy();
	DeleteObject(m_hBrushBackGround);
}

void CP2PVedioPanel::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_SLIDER_HORN,m_wndSliderHorn);
	DDX_Control(pDX, IDC_SLIDER_SPEAKER,m_wndSliderSpeaker);
	DDX_Control(pDX, IDC_BTN_VEDIO,m_btnVedio);
	DDX_Control(pDX, IDC_BTN_SNATCH, m_btnSnatch);

	DDX_Control(pDX, IDC_BUTTON_VIDEO_SOUND,m_btnSound);
	DDX_Control(pDX, IDC_BUTTON_VIDEO_VOICE,m_btnVoice);
	DDX_Control(pDX, IDC_BTN_ANSWER,m_btnAnswer);
	DDX_Control(pDX, IDC_BTN_STOP, m_btnStop);
}


BEGIN_MESSAGE_MAP(CP2PVedioPanel, CXSkinDlg)
	ON_WM_ERASEBKGND()
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BTN_VEDIO, OnBnClickedBtnVedio)
	ON_BN_CLICKED(IDC_BTN_SNATCH, OnBnClickedBtnSnatch)
	ON_WM_CTLCOLOR()

	ON_BN_CLICKED(IDC_BUTTON_VIDEO_SOUND, OnBnClickedBtnVoice)
	ON_BN_CLICKED(IDC_BUTTON_VIDEO_VOICE, OnBnClickedBtnSound)
	ON_BN_CLICKED(IDC_BTN_ANSWER, OnBnClickedBtnAnswer)

	ON_BN_CLICKED(IDC_BTN_STOP, OnBnClickedBtnStop)
	ON_WM_HSCROLL()
	ON_WM_CLOSE()
END_MESSAGE_MAP()


// CP2PVedioPanel 消息处理程序

BOOL CP2PVedioPanel::PreTranslateMessage(MSG* pMsg)
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

BOOL CP2PVedioPanel::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	pDC->FillSolidRect(&rc, 0); // 黑色背景
	return FALSE;
}

BOOL CP2PVedioPanel::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	m_eVideoStatus = NONE;

	SetDlgStyle(MINCLOSE);


	//麦克风和扬声器
	m_btnSound.MoveWindow(190,377+10,22,22);
	m_btnSound.SetPng(IDB_PNG_SOUND_HOVER, IDB_PNG_SOUND_NORMAL, IDB_PNG_SOUND_PRESSED, IDB_PNG_SOUND_DISABLE);
	m_btnVoice.MoveWindow(190,377+40,22,22);
	m_btnVoice.SetPng(IDB_PNG_VOICE_HOVER, IDB_PNG_VOICE_NORMAL, IDB_PNG_VOICE_PRESSED, IDB_PNG_VOICE_DISABLE);


	m_wndSliderHorn.SetBackColor(RGB(0xDA,0xE7,0xEF));
	m_wndSliderHorn.SetRange(0,100);
	m_wndSliderHorn.MoveWindow(220,377+10,100,22);
	
	m_wndSliderSpeaker.SetBackColor(RGB(0xDA,0xE7,0xEF));
	m_wndSliderSpeaker.SetRange(0,100);
	m_wndSliderSpeaker.MoveWindow(220,377+40,100,22);
	m_btnVedio.SetBitmap(IDB_VEDIO_ON,IDB_VEDIO_ON,RGB(0,255,0));
	m_btnSnatch.SetBitmap(IDB_VEDIO_SNATCH,IDB_VEDIO_SNATCH,RGB(0,255,0));



	//拒绝按钮
	m_btnStop.MoveWindow(287,349,63,24);
	m_btnStop.SizeToContent();
	m_btnStop.SetTextColor(RGB(89,89,89));
	m_btnStop.SetPng(IDB_PNG_REFUSE_HOVER, IDB_PNG_REFUSED_NORMAL, IDB_PNG_REFUSED_PRESSED, IDB_PNG_REFUSED_NORMAL);
	m_btnStop.SetFont(&g_font12);
	
	//接受按钮
	m_btnAnswer.MoveWindow(219,349,63,24);
	m_btnAnswer.SizeToContent();
	m_btnAnswer.SetTextColor(RGB(89,89,89));
	m_btnAnswer.SetPng(IDB_PNG_ACCEPT_HOVER, IDB_PNG_ACCEPT_NORMAL, IDB_PNG_ACCEPT_PRESSED, IDB_PNG_ACCEPT_NORMAL);
	//m_btnAnswer.SetBitmap(IDB_BTN_SCORE_HOT, IDB_BTN_SCORE_NORMAL, IDB_BTN_SCORE_DOWN, IDB_BTN_SCORE_DISABLE);
	m_btnAnswer.SetFont(&g_font12);




	//中间视频icon
	LoadImageFromResource(&m_imgIconOn, IDB_PNG_VIDEO, _T("PNG"));

	m_hBrushBackGround =  CreateSolidBrush(RGB(0xDA,0xE7,0xEF)); //黑色背景
	GetDlgItem(IDC_STATIC_TIP)->SetFont(&g_font12);
	GetDlgItem(IDC_STATIC_TIME)->SetFont(&g_font12);

	//"视频聊天邀请中"
	GetDlgItem(IDC_STATIC_TIP)->MoveWindow(1 ,TITLE_BAR_CY + BORDER_W + 6 ,100,20);
	GetDlgItem(IDC_STATIC_TIME)->MoveWindow(251 ,TITLE_BAR_CY + BORDER_W + 6,100,20);


	//对面视频画面
	CRect rc;
	rc.left = 1;
	rc.right = 359;
	rc.top = TITLE_BAR_CY + BORDER_W + 35;
	rc.bottom = rc.top + 280;
	GetDlgItem(IDC_VEDIO_FRIEND)->MoveWindow(&rc);
	
	//自己视频画面
	rc.left = 10;
	rc.right = rc.left +  160;
	rc.top = 377 + 10;
	rc.bottom = rc.top + 125;
	GetDlgItem(IDC_VEDIO_SELF)->MoveWindow(&rc);

	return TRUE;  
}

void CP2PVedioPanel::VideoCall()
{
	m_eVideoStatus = CALLING;
	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_HIDE);

	
}

void CP2PVedioPanel::VideoRecvRequest()
{
	
	m_eVideoStatus = RECV_REQUEST;

	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_SHOW);
}

void CP2PVedioPanel::VideoStartTalk()
{
	m_eVideoStatus = TALKING;
	
	m_btnStop.ShowWindow(SW_SHOW);
	m_btnAnswer.ShowWindow(SW_HIDE);
}

afx_msg void CP2PVedioPanel::OnBnClickedBtnVoice()
{
}

afx_msg void CP2PVedioPanel::OnBnClickedBtnSound()
{

}


afx_msg void CP2PVedioPanel::OnBnClickedBtnAnswer()
{
	StopTone();
	switch(m_eVideoStatus)
	{
		//接听
		case RECV_REQUEST:
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_ACCEPT,10086);
			//m_eVideoStatus = TALKING;
			VideoStartTalk();
			break;
		default: 
			break;
	}
}


afx_msg void CP2PVedioPanel::OnBnClickedBtnStop()
{
	StopTone();
	switch(m_eVideoStatus)
	{
		//挂断
		case CALLING:
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_CANCEL,10086);
			break;
		//拒绝
		case RECV_REQUEST:
			GetParent()->PostMessage(WM_RICHEDIT_NOTIFY,RN_VEDIO_REFUSE,10086);
			break;
		//中止
		case TALKING:
			GetParent()->PostMessage(WM_P2PVEDIO_NOTIFY,0,1);
			break;
		default:
			break;
	}
	m_eVideoStatus = NONE;
	this->ShowWindow(SW_HIDE);
}

void CP2PVedioPanel::OnPaint()
{
	CPaintDC dc(this);
	DrawCaptionBar();
	DrawBorder();

	m_imgIconOn.Draw(dc,140,167,79,65);

	CRect rcWin;
	this->GetWindowRect(rcWin);
	ScreenToClient(&rcWin);

	CRect rcBar;
	rcBar.left = 1;
	rcBar.right = 359;

	rcBar.top = TITLE_BAR_CY + BORDER_W;
	rcBar.bottom = rcBar.top + 35;
	dc.FillSolidRect(&rcBar,RGB(0xDA,0xE7,0xEF));

	
	rcBar.top = 342;
	rcBar.bottom = rcBar.top + 35;
	dc.FillSolidRect(&rcBar,0x393939);


	rcBar.top = 377;
	rcBar.bottom = rcBar.top + 139;
	dc.FillSolidRect(&rcBar,RGB(0xDA,0xE7,0xEF));

/*

	CWnd *pWnd ;
	CRect rc;

	pWnd = GetDlgItem(IDC_ICON_HORN);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmpHorn;		
		bmpHorn.LoadBitmap(IDB_HORN);

		::DrawBitmap(&dc,rc.left,rc.top,&bmpHorn,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_ICON_SPEAKER);

	if ( pWnd )
	{
		pWnd->GetWindowRect(&rc);
		ScreenToClient(&rc);

		CResBitmap bmpSpeaker;
		bmpSpeaker.LoadBitmap(IDB_MICROPHONE);

		::DrawBitmap(&dc,rc.left,rc.top,&bmpSpeaker,RGB(0,255,0));
	}

	pWnd = GetDlgItem(IDC_VEDIO_SELF);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	//CResBitmap bmpVedio;
	//bmpVedio.LoadBitmap(IDB_VEDIO_SMALL);

	//::DrawBitmap(&dc,rc.left,rc.top,&bmpVedio,RGB(0,255,0));

	
	rc.InflateRect(1,1);
	dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);

	pWnd = GetDlgItem(IDC_VEDIO_FRIEND);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	
	if ( m_bmpLastFrame.GetSafeHandle() )
	{		
		::DrawBitmap(&dc,rc.left,rc.top,&m_bmpLastFrame,RGB(0,255,0));
	}
	else
	{
		//CResBitmap bmpVedio;
		//bmpVedio.LoadBitmap(IDB_VEDIO_BIG);

		//::DrawBitmap(&dc,rc.left,rc.top,&bmpVedio,RGB(0,255,0));
	}

	dc.Draw3dRect(&rc,IM_LINE_COLOR,IM_LINE_COLOR);
	dc.Draw3dRect(3,3,330,290,IM_LINE_COLOR,IM_LINE_COLOR);
	*/
}

void CP2PVedioPanel::OnBnClickedBtnVedio()
{
	static int i=0;
	if ( i==0 )
	{
		i=1;
		m_btnVedio.SetBitmap(IDB_VEDIO_OFF,IDB_VEDIO_OFF,RGB(0,255,0));
	}
	else
	{
		i=0;
		m_btnVedio.SetBitmap(IDB_VEDIO_ON,IDB_VEDIO_ON,RGB(0,255,0));
	}

}

void CP2PVedioPanel::OnBnClickedBtnSnatch()
{
	// TODO: 在此添加控件通知处理程序代码
}

HBRUSH CP2PVedioPanel::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		UINT nID = pWnd->GetDlgCtrlID();

		if ( nID == IDC_STATIC_TIP || nID == IDC_STATIC_TIME )
		{
			pDC->SetBkMode(TRANSPARENT);
			hbr = m_hBrushBackGround;
			pDC->SetTextColor(0x4F4F4F);
		}
	}

	return hbr;
}


void CP2PVedioPanel::StartVedio(int nCameraIndex, int uiFriendIp)
{
/*
	CWnd *pWnd = GetDlgItem(IDC_VEDIO_SELF);
	CRect rcCapWnd;
	pWnd->GetClientRect(&rcCapWnd);

	bool bInitVedio, bInitAudio;

	if ( m_pMMTalker->InitVideo(rcCapWnd,pWnd->m_hWnd,0,nCameraIndex) )
	{   
		pWnd->ShowWindow(SW_SHOW);
		m_pMMTalker->StartVideoCap();

		bInitVedio = true;
	}
	else
	{
		pWnd->ClientToScreen(&rcCapWnd);		
		pWnd->ShowWindow(SW_HIDE);
		ScreenToClient(&rcCapWnd);
		InvalidateRect(&rcCapWnd);

		bInitVedio = false;
	}

	if ( m_pMMTalker->InitAudio(pWnd->m_hWnd) )
	{
		m_pMMTalker->StartAudioCap();

		bInitAudio = true;
	}
	else
	{
		bInitAudio = false;
	}
	*/

	if(m_pVideoEngie == NULL)
		m_pVideoEngie = GetGipsVideoEngineByNew();
	if(m_pVideoEngie == NULL)
		return ;
	m_pVideoEngie->InitVideo();
	m_pVideoEngie->CreateChannal(m_nVideoChannel);
	int nCaptureId = 0;
	m_pVideoEngie->ConnctDevice(m_nVideoChannel, nCaptureId);

	m_pVideoEngie->RegisterVideoRenderModule(m_nVideoChannel,nCaptureId,
		GetDlgItem(IDC_VEDIO_SELF)->GetSafeHwnd(),GetDlgItem(IDC_VEDIO_FRIEND)->GetSafeHwnd());
	
	webrtc::VideoCodec video_send_codec;
	m_pVideoEngie->GetCodec(0,video_send_codec);
	//video_send_codec.width = 176;
	//video_send_codec.height = 144;
	//video_send_codec.startBitrate = 100;
	//video_send_codec.minBitrate = 100;
	//video_send_codec.maxBitrate = 1000;
	//video_send_codec.maxFramerate = 30;
	//video_send_codec.codecSpecific.VP8.numberOfTemporalLayers = 0;

	m_pVideoEngie->SetSendCodec(m_nVideoChannel,video_send_codec);
	m_pVideoEngie->SetReceiveCodec(m_nVideoChannel,video_send_codec);


	struct in_addr addr1;
    memcpy(&addr1, &uiFriendIp, 4);
	TRACE("视频：对方IP  %s\n",inet_ntoa(addr1));

	m_pVideoEngie->SetSendDestination(m_nVideoChannel,inet_ntoa(addr1),11111);	
	m_pVideoEngie->SetLocalRecv(m_nVideoChannel,11111);
	m_pVideoEngie->StartRecv(m_nVideoChannel);
	m_pVideoEngie->StartSend(m_nVideoChannel);

}

void CP2PVedioPanel::StopVedio()
{
	/*
	m_pMMTalker->Stop();

	if ( m_bmpLastFrame.GetSafeHandle() )
	{
		m_bmpLastFrame.DeleteObject();
	}
	*/
	if(m_pVideoEngie == NULL)
		return;
	m_pVideoEngie->SafeClose(m_nVideoChannel);
}

void CP2PVedioPanel::DrawVedioFrame(BYTE *pData)
{
	//CWnd * pWnd = GetDlgItem(IDC_STATIC_VEDIO);
	//CDC  * pDC  = pWnd->GetDC();
	//HDC hdc    = pDC->GetSafeHdc();

	//m_pBmpInfoOut = m_talker.GetVideoFormat();
	//CRect rct(0,0,320,240);
	//rct.MoveToXY(0,480);
	//StretchDIBits(
	//	hdc,
	//	rct.left,
	//	rct.top,
	//	rct.Width(),
	//	rct.Height(),
	//	0,
	//	0,
	//	rct.Width(),
	//	rct.Height(),
	//	pData,
	//	m_pBmpInfoOut,
	//	DIB_RGB_COLORS,SRCCOPY
	//	);

	//pWnd->ReleaseDC(pDC);
	
	CWnd * pWnd = GetDlgItem(IDC_VEDIO_FRIEND);
	CDC * pDC = pWnd->GetDC();

	CRect rct(0,0,320,240);

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	if ( m_bmpLastFrame.GetSafeHandle() )
	{
		m_bmpLastFrame.DeleteObject();
	}

	m_bmpLastFrame.CreateCompatibleBitmap(pDC,rct.Width(),rct.Height() );

	CBitmap *pOld = dcMem.SelectObject(&m_bmpLastFrame);

	HDC hdc   = dcMem.GetSafeHdc();

	BITMAPINFO * bi = m_pMMTalker->GetVideoFormat();

	StretchDIBits(
		hdc,
		0,
		0,
		rct.Width(),
		rct.Height(),
		0,
		0,
		rct.Width(),
		rct.Height(),
		pData,
		bi,
		DIB_RGB_COLORS,SRCCOPY
		);

	pDC->BitBlt(0,0,rct.Width(),rct.Height(),&dcMem,0,0,SRCCOPY);
	pDC->Draw3dRect(0,0,rct.Width(),rct.Height(),IM_LINE_COLOR,IM_LINE_COLOR);

	dcMem.SelectObject(pOld);

	pWnd->ReleaseDC(pDC);
}

void CP2PVedioPanel::SetStatusText(CString str)
{
	GetDlgItem(IDC_STATIC_TIP)->SetWindowText(str);
}

void CP2PVedioPanel::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar)
{
	if ( pScrollBar )
	{
		if ( pScrollBar->m_hWnd==m_wndSliderHorn.m_hWnd )//调节扬声器音量
		{
			m_pMMTalker->SetPlayVolume(m_wndSliderHorn.GetPos());
		}
		else
		if ( pScrollBar->m_hWnd==m_wndSliderSpeaker.m_hWnd )//麦克风音量
		{
			m_pMMTalker->SetRecordVolume(m_wndSliderSpeaker.GetPos());
		}
	}

	CXSkinDlg::OnHScroll(nSBCode, nPos, pScrollBar);
}


void CP2PVedioPanel::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	OnBnClickedBtnStop();
	CXSkinDlg::OnClose();
}
