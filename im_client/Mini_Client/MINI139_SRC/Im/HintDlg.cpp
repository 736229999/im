//-------------------------------------------------------------------------

// 文件名称：HintDlg.cpp
// 摘    要：消息提示窗实现
// 
// 当前版本：
// 作    者：xuexiaofeng
// 完成日期：
//-----------------------------------------------------------------------------

#include "stdafx.h"
#include "HintDlg.h"
#include "..\resource.h"
#include ".\hintdlg.h"
#include "improtocol.h"
#include "webprocess.h"

unsigned int CHintDlg::HintID  = 0;

static CResBitmap bmpHintBackgnd  ;
static CResBitmap bmpHintIcon     ;

vector<CHintDlg *>  CHintDlg::m_vecInstance;


IMPLEMENT_DYNAMIC(CHintDlg, CXSkinDlg)
CHintDlg::CHintDlg(CWnd* pParent ,int iHintType)
	: CXSkinDlg(CHintDlg::IDD, pParent)
{
	if ( bmpHintBackgnd.m_hObject==NULL )
	{
		//bmpHintBackgnd.LoadBitmap(IDB_HINT_DLG);
	}

	if ( bmpHintIcon.m_hObject==NULL )
	{
		bmpHintIcon.LoadBitmap(IDB_HINT_ICON);
	}

	HintID++;
	m_bMouseOverHint  = false;
	m_bMouseOverClose = false;
	m_bNotifySended = false;
	m_bitmapFace    = NULL ;
	m_hWndNotify    = NULL ;
	m_crHintText    = RGB(0,100,190);

	m_iActiveProcessed = 0;
	m_iHintType        = 0;
	m_bAutoSink        = true;
	
	m_pCluster		= NULL;
	m_pFriend		= NULL;
	m_nLinkType		= 0;

	m_colorBackground	= /*BACKGND_COLOR*/RGB(255,255,255);
	m_colorBorder		= BORDER_COLOR;

	m_bDrawBorder = false;
	m_hBkGround = ::CreateSolidBrush(RGB(255, 255, 255));

	m_pFaceCtrl = NULL;
	Create(IDD,pParent);
}
//
//CHintDlg::CHintDlg()
//{
//	if ( bmpHintBackgnd.m_hObject==NULL )
//	{
//		bmpHintBackgnd.LoadBitmap(IDB_HINT_DLG);
//	}
//
//	if ( bmpHintIcon.m_hObject==NULL )
//	{
//		bmpHintIcon.LoadBitmap(IDB_HINT_ICON);
//	}
//
//	m_bMouseOverHint  = false;
//	m_bMouseOverClose = false;
//	m_bNotifySended = false;
//	m_bitmapFace    = NULL ;
//	m_hWndNotify    = NULL ;
//	m_crHintText    = RGB(0,100,190);
//
//	m_iActiveProcessed = 0;
//}

CHintDlg::~CHintDlg()
{
	HintID--;

	for ( int i=0; i<m_vecInstance.size(); i++ )
	{
		if ( m_vecInstance[i]==this )
		{
			m_vecInstance.erase(m_vecInstance.begin()+i);
			break;
		}
	}
	if(m_pFaceCtrl != NULL)
	{
		delete m_pFaceCtrl;
		m_pFaceCtrl = NULL;
	}

	m_bitmapFace = NULL;
}

void CHintDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CHintDlg, CXSkinDlg)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_WM_CTLCOLOR()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

// CHintDlg 消息处理程序

void CHintDlg::Show(FRIEND_INFO *pFriend,CString strTitle, CString strHint, HWND hCmdWnd)
{ 
	CHintDlg *pHintDlg = new CHintDlg(GetDesktopWindow(),0);
	pHintDlg->m_nLinkType = LINKT_FRIEND;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetFriendInfo(pFriend);
	pHintDlg->SetHintText(strTitle,strHint);
	pHintDlg->m_bAutoSink = true;
	pHintDlg->ShowWindow(SW_SHOW);
}

void CHintDlg::Show(CLUSTER_INFO * pCluster,CString sTitle,CString sHint,HWND hCmdWnd)
{
	CHintDlg *pHintDlg = new CHintDlg(GetDesktopWindow(),0);
	pHintDlg->m_nLinkType = LINKT_CLUSTER;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetClusterInfo(pCluster);
	pHintDlg->SetHintText(sTitle,sHint);
	pHintDlg->m_bAutoSink = true;
	pHintDlg->ShowWindow(SW_SHOW);
}

void CHintDlg::Show(CString strHint, int iHintType, HWND hCmdWnd, bool bAutoSink)
{
	CHintDlg *pHintDlg = new CHintDlg(GetDesktopWindow(),iHintType);
	pHintDlg->m_nLinkType = LINKT_UNKNOWN;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetFriendInfo(NULL);
	pHintDlg->SetHintText("",strHint);
	pHintDlg->m_bAutoSink = bAutoSink;
	pHintDlg->ShowWindow(SW_SHOW);
}

void CHintDlg::Show(CString strTitle,CString strHint, int iHintType, HWND hCmdWnd,bool bAutoSink)
{
	CHintDlg *pHintDlg = new CHintDlg(GetDesktopWindow(),iHintType);
	pHintDlg->m_nLinkType = LINKT_UNKNOWN;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetFriendInfo(NULL);
	pHintDlg->SetHintText(strTitle,strHint);
	pHintDlg->m_bAutoSink = bAutoSink;
	pHintDlg->ShowWindow(SW_SHOW);
}

//======================================================================
//函数名  ：OnInitDialog
//函数描述：对话窗初始化
//输入参数：无
//输出参数：无
//返回值  ：无
//其他说明：无
//=====================================================================
BOOL CHintDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

//	m_btnClose.SetBitmap(IDB_HINT_CLOSE_NORMAL,IDB_HINT_CLOSE_HOVER,IDB_HINT_CLOSE_HOVER,IDB_HINT_CLOSE_NORMAL);

	SetTimer( UP_TIMER , 10 , NULL );//启动一个计时器，用来关闭本对话窗
	SetDlgStyle(CLOSE);
	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW);

	int cx = 220;
	int cy = 130;

	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN);

	CWnd *pShellTrayWnd = FindWindow("Shell_TrayWnd",NULL);

	if (pShellTrayWnd)
	{
		CRect rcShellTrayWnd;
		pShellTrayWnd->GetWindowRect(&rcShellTrayWnd);

		if (rcShellTrayWnd.left<=0 && rcShellTrayWnd.bottom>=screen_cy )
		{
			//shell tray wnd on screen bottom
			screen_cy=rcShellTrayWnd.top;
		}
	}

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOOWNERZORDER|SWP_NOACTIVATE|SWP_NOMOVE|SWP_NOSIZE);
	m_nCy=cy;

	int nPort = /*g_pProtocol->GetUdpCommPort()-XTREAM_LOCAL_PORT*/0;
	MoveWindow(screen_cx-(nPort+1)*cx-20,screen_cy-cy*(HintID-1),cx,0);

	//m_rcClose.SetRect(145,3,158,14);
	//m_rcFace.SetRect(3,24,47,68);
	m_rcHint.SetRect(58,26,184,96);

	m_pFaceCtrl = new CRountFaceCtrl(IDB_PNG_ROUND_NORMAL, IDB_PNG_ROUND_SEL);
	m_pFaceCtrl->SetBitmapHint(TRUE);
	m_pFaceCtrl->Create(this, 40001);
	if(m_pFriend != NULL)
		m_pFaceCtrl->SetPicPath(g_webUserHeadIconProcess.GetFaceFile(m_pFriend));
	else
		m_pFaceCtrl->SetPicPath(g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo));

	CRect rcCtrl;
	m_pFaceCtrl->GetWindowRect(&rcCtrl);

	CRect rc;
	rc.left = 3;
	rc.top = 30;

	rc.right = 3 + rcCtrl.Width();
	rc.bottom = 30 + rcCtrl.Height();

	m_pFaceCtrl->MoveWindow(&rc);
	m_pFaceCtrl->ShowWindow(SW_SHOW);

	SetWindowText(g_ImUserInfo.nickname);
	return TRUE;  
}

//======================================================================
//函数名  ：OnTimer
//函数描述：定时器
//输入参数：句柄
//输出参数：无
//返回值  ：无
//其他说明：无
//=====================================================================
void CHintDlg::OnTimer(UINT nIDEvent)
{
	switch( nIDEvent )
	{
	case UP_TIMER :
		{
			CRect rc;
			GetWindowRect( &rc );

			rc.top -= 5;


			if( rc.Height() >= m_nCy )
			{
				rc.top=rc.bottom-m_nCy;
				KillTimer(UP_TIMER);
				SetTimer(SHOW_TIMER,3000,NULL);
				m_bDrawBorder = true;
			}

			if(m_pFaceCtrl != NULL)
				m_pFaceCtrl->SetSaveBackground(FALSE);
			MoveWindow( &rc , TRUE );	
		}
		break;

	case SHOW_TIMER :
		KillTimer(SHOW_TIMER);

		if ( m_bAutoSink )
			SetTimer(DOWN_TIMER,1,NULL);

		break;

	case DOWN_TIMER:
		{
			CRect rc;
			GetWindowRect( &rc );

			rc.top += 5;
			if( rc.Height() < 0 )
			{
				KillTimer( DOWN_TIMER );
				PostMessage(WM_CLOSE);
			}
			else
			{
				CPoint ptCursor;
				GetCursorPos(&ptCursor);

				//鼠标停留在窗体上则静止
				if ( !rc.PtInRect(ptCursor) )
				{
					MoveWindow( &rc , TRUE );
				}
			}
		}
		break;
	case MOUSE_TIMER:
		{				
			CPoint ptCursor;
			GetCursorPos(&ptCursor);

			CRect rcHint = m_rcHint;
			ClientToScreen(&rcHint);

			CRect rcClose = m_rcClose;
			ClientToScreen(&rcClose);

			BOOL bInHint  = rcHint.PtInRect(ptCursor);
			BOOL bInClose = rcClose.PtInRect(ptCursor);
			if ( bInHint==FALSE )
			{
				m_bMouseOverHint=false;
				Invalidate();
			}

			if ( bInClose==FALSE )
			{
				m_bMouseOverClose=false;
				Invalidate();
			}

			if ( bInHint==FALSE && bInClose==FALSE )
			{
				KillTimer(MOUSE_TIMER);
			}
		}
		break;
	default :
		break;
	}

	//try
	//{
	//	CDialog::OnTimer(nIDEvent);
	//}
	//catch(...)
	//{
	//}
}


void CHintDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

int CHintDlg::DoModal()
{
	return CDialog::DoModal();
}

void CHintDlg::SetFriendInfo(FRIEND_INFO *pFriend)
{
	m_pFriend = pFriend ; 
	m_bitmapFace = NULL;

	if(m_pFaceCtrl != NULL)
	{
		if(m_pFriend != NULL)
			m_pFaceCtrl->SetPicPath(g_webUserHeadIconProcess.GetFaceFile(m_pFriend));
		else
			m_pFaceCtrl->SetPicPath(g_webUserHeadIconProcess.GetFaceFile(&g_ImUserInfo));
	}

	//if ( pFriend )
	//{
	//	m_bitmapFace = ::GetUserBitmap(pFriend,false);
	//}
	//else
	//{
	//	m_bitmapFace = &bmpHintIcon;
	//}
}

void CHintDlg::SetClusterInfo(CLUSTER_INFO* pCluster)
{
	m_pCluster = pCluster;
	if ( pCluster )
	{
		m_bitmapFace = &bmpHintIcon;
	}
	else
	{
		m_bitmapFace = &bmpHintIcon;
	}

}

void CHintDlg::SetHintText(CString strTitle,CString strHint)
{
	m_strTitle = strTitle;

	int n=strHint.Find("<image:",0);

	while ( n>-1 )
	{
		int iImageStart=n;

		n=strHint.Find(">",iImageStart);

		if ( n>iImageStart )
		{
			strHint.Delete(iImageStart,n-iImageStart+1);
			strHint.Insert(iImageStart,"[图片]");
		}

		n=strHint.Find("<image:",0);
	}

	m_strHint = strHint;
}

BOOL CHintDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CHintDlg::SetCmdWnd(HWND hNotify, UINT uCmd)
{
	m_hWndNotify = hNotify;
	m_uMessageID = uCmd;
}

void CHintDlg::FlipBackGround(CDC *pDC, CDC * pSafeDC)
{
	/*BITMAP bi;
	bmpHintBackgnd.GetBitmap(&bi);*/

	CDC memDC;
	memDC.CreateCompatibleDC(pSafeDC);

	int nSavedMemDC = memDC.SaveDC();

	//memDC.SelectObject(&bmpHintBackgnd);

	int nSavedDC = pDC->SaveDC();
	
	//pDC->BitBlt(0,0,bi.bmWidth, bi.bmHeight,&memDC,0,0,SRCCOPY);

	//if ( m_bitmapFace )
	//{
	//	CDC dcTemp	; 

	//	if (pSafeDC)
	//	{
	//		if(m_bitmapFace->m_hObject == NULL)
	//			return;
	//		//m_bitmapFace->GetBitmap(&bi);
	//		CRect rc;
	//		GetWindowRect(rc);

	//		int cx=rc.Width(); 
	//		int cy=rc.Height();
	//		if ( dcTemp.CreateCompatibleDC(pSafeDC) )
	//		{
	//			CBitmap *pOld=dcTemp.SelectObject(m_bitmapFace);

	//			pDC->TransparentBlt(
	//				m_rcFace.left + ( m_rcFace.Width() - 40)/2,
	//				m_rcFace.top  + ( m_rcFace.Height() - 40)/2,
	//				40,40,
	//				&dcTemp,
	//				0,0,
	//				/*bi.bmWidth,bi.bmHeight,*/
	//				cx, cy,
	//				RGB(0,255,0));

	//			dcTemp.SelectObject(pOld);
	//		}
	//	}		
	//}
	//else
	//{
	//	POINT pt;
	//	pt.x = m_rcFace.left + ( m_rcFace.Width() -50)/2;//40
	//	pt.y = m_rcFace.top  + ( m_rcFace.Height() - 50)/2;//40
	//
	//	CBitmap * pHead=NULL;
	//	if(m_nLinkType==LINKT_FRIEND && m_pFriend)
	//	{
	//		 pHead = GetUserBitmap(m_pFriend,false);
	//	}

	//	if(pHead)
	//	{
	//		::DrawBitmap(pDC,pt.x,pt.y,pHead,RGB(0,255,0));
	//	}
	//	else
	//	{
	//		int sex=0;

	//		if ( m_pFriend )
	//			sex=m_pFriend->sex;

	//		g_imgStatus52.Draw(pDC,GetDefaultImage(sex,XTREAM_ONLINE),pt,ILD_NORMAL);
	//	}
	//}


	COLORREF clrBorder = RGB(200,200,200);

	/*pDC->Draw3dRect(&m_rcFace,clrBorder,clrBorder);
	DrawBitmap(pDC,m_rcClose.left,m_rcClose.top, this->m_bMouseOverClose? IDB_HINT_CLOSE_HOVER: IDB_HINT_CLOSE_NORMAL);*/

	CRect rcHint = m_rcHint;
	rcHint.InflateRect(-2,-2);
	rcHint.top+=18;
	rcHint.left+=4;

	CRect rcTitle = m_rcHint;
	rcTitle.InflateRect(-2,-2);
	rcTitle.bottom=rcTitle.top+16;

	pDC->SetBkMode(TRANSPARENT);

	BOOL bUnderLine = ( m_bMouseOverHint && m_iHintType==0 ) ? TRUE:FALSE;

	CFont fontTitle, fontHint;
	fontHint.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,						// bItalic
		bUnderLine,                 // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	fontTitle.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,						// bItalic
		FALSE,               // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");


	if (m_strTitle!="" )
	{
		pDC->SelectObject(&fontTitle);
	
		pDC->SetTextColor(RGB(0,50,170));

		pDC->DrawText(m_strTitle,rcTitle,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);

		pDC->SelectObject(&fontHint);

		if ( m_bMouseOverHint )
		{
			pDC->SetTextColor(RGB(0,84,166));
		}
		else
		{
			pDC->SetTextColor(m_crHintText);
		}

		pDC->DrawText(m_strHint,rcHint,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
	}
	else
	{
		rcHint = m_rcHint;
		rcHint.InflateRect(-2,-2);

		pDC->SelectObject(&fontHint);
		pDC->SetTextColor(m_crHintText);
		pDC->DrawText(m_strHint,rcHint,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
	}

	memDC.RestoreDC(nSavedMemDC);
	pDC->RestoreDC(nSavedDC);
}

BOOL CHintDlg::OnEraseBkgnd(CDC* pDC)
{	
// 	CRect rcClient, rcHide, rcBottom;
// 	GetClientRect(rcClient);
// 	ScreenToClient(&rcBottom);
// 	rcBottom.top = rcBottom.bottom;
// 	rcBottom.bottom = rcClient.bottom;
// 	rcBottom.left = rcClient.left;
// 
// 	CRgn rgnPaint;
// 
// 	pDC->FillSolidRect(rcClient,m_colorBackground);
// 	pDC->FillSolidRect(rcBottom, CHAT_BOTTOM_COLOR);


	CRect rcDlg;
	GetClientRect(&rcDlg);
	pDC->FillSolidRect(&rcDlg, RGB(255, 255, 255));

	return FALSE;
}

void CHintDlg::OnClose()
{
	DestroyWindow();
}

void CHintDlg::DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp)
{
	CResBitmap bmp;
	bmp.LoadBitmap(nIDBmp);

	BITMAP bi;
	bmp.GetBitmap(&bi);

	CDC dc;

	dc.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = dc.SelectObject(&bmp);

	TransparentBlt(pDC->m_hDC,x,y,bi.bmWidth,bi.bmHeight,dc.m_hDC,0,0,bi.bmWidth,bi.bmHeight,RGB(0,255,0));
//	pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dc,0,0,SRCCOPY);
	dc.SelectObject(pOldbmp);
}

void CHintDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
{
	CDialog::OnActivate(nState, pWndOther, bMinimized);

	if ( nState!=WA_INACTIVE )
	{
		if ( pWndOther && pWndOther->m_hWnd && m_iActiveProcessed<20)//具体窗体创建后要OnActive几次还不清楚，目前定为５
		{
			m_iActiveProcessed++;

			CString strText;
			pWndOther->GetWindowText(strText);

			if ( strText!="MiniPictureBroadCast" )
			{	
				pWndOther->BringWindowToTop();
				pWndOther->SetActiveWindow();
				pWndOther->SetFocus();
			}
		}		
	}
}

void CHintDlg::OnPaint()
{
	CPaintDC dc(this);


	CDC *pDC = GetWindowDC();
	if(pDC==NULL)
		return;
	int nSavedDC = pDC->SaveDC();
	DrawCaptionBar();
	if(m_bDrawBorder)
	{
		DrawBorder();
	}

	try
	{
		FlipBackGround(/*&memDC*/pDC, &dc);
	}
	catch(...)
	{
	}
	//dc.BitBlt(0,0,rc.Width(),rc.Height(),&memDC,0,0,SRCCOPY);

	//memDC.SelectObject(pOldbmp);
}

LRESULT CHintDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (message == WM_KEYDOWN)
	{
		switch (wParam)
		{
		case VK_ESCAPE:
		case 0x0D:
			return TRUE;
		default:
			break;
		}
	}

	if ( message == WM_LBUTTONUP )
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);

		CRect rcHint	= m_rcHint;
		ClientToScreen(&rcHint);

		CRect rcClose	= m_rcClose;
		ClientToScreen(&rcClose);

		if ( rcClose.PtInRect(ptCursor) )
		{
			PostMessage(WM_CLOSE,0,0);
		}

		if ( rcHint.PtInRect(ptCursor) )
		{
			switch ( m_iHintType )
			{
			case 0:
				if ( m_hWndNotify && m_bNotifySended==false )
				{		
					if(m_nLinkType==LINKT_FRIEND)
					{
						::PostMessage(m_hWndNotify,m_uMessageID,m_nLinkType,(LPARAM)m_pFriend);
					}
					else 
						if(m_nLinkType==LINKT_CLUSTER)
						{
							::PostMessage(m_hWndNotify,m_uMessageID,m_nLinkType,(LPARAM)m_pCluster);
						}	
						else
						{
							::PostMessage(m_hWndNotify,m_uMessageID,0,0);
						}

						m_bNotifySended = true;
						return TRUE;
				}
				break;
			case 1:
				break;
			}
		}
	}

	if ( message==WM_MOUSEMOVE )
	{
		CPoint ptCursor;
		GetCursorPos(&ptCursor);

		CRect rcHint	= m_rcHint;
		ClientToScreen(&rcHint);

		CRect rcClose	= m_rcClose;
		ClientToScreen(&rcClose);

		bool bMouseOverHint = rcHint.PtInRect(ptCursor);

		if ( bMouseOverHint )
		{
			if ( m_bMouseOverHint==false )
			{
				m_bMouseOverHint=true;
				Invalidate();
				SetTimer(MOUSE_TIMER,100,NULL);
			}
		}

		switch(m_iHintType)
		{
		case 0:
			if ( m_hWndNotify && bMouseOverHint && m_bNotifySended==false )
			{
				::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
			}
			break;
		case 1:
			if ( bMouseOverHint )
			{
				::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
			}
			break;
		}

		if ( m_bMouseOverClose==false && rcClose.PtInRect(ptCursor) )
		{
			m_bMouseOverClose=true;
			Invalidate();
			SetTimer(MOUSE_TIMER,100,NULL);
		}
	}

	return CDialog::WindowProc(message, wParam, lParam);
}


HBRUSH CHintDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	//HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  在此更改 DC 的任何特性
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
	// TODO:  如果默认的不是所需画笔，则返回另一个画笔
	return hbr;
}


void CHintDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
	CXSkinDlg::OnNcLButtonDown(nHitTest, point);
}
