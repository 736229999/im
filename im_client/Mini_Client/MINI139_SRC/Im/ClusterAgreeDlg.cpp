// ClusterAgreeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ClusterAgreeDlg.h"
#include "afxdialogex.h"
#include "improtocol.h"
#include "webprocess.h"
//#include "FriendInfoDlg.h"
#include "..\FriendInfoDlg.h"
#include "..\ClusterParamSetting.h"
#include "ClusterInfoDlg.h"
#include "hintdlg.h"
#include "MessageHandleWnd.h"
// CClusterAgreeDlg 对话框
unsigned int CClusterAgreeDlg::HintID  = 0;

static CResBitmap bmpHintBackgnd  ;
static CResBitmap bmpHintIcon     ;

vector<CClusterAgreeDlg *>  CClusterAgreeDlg::m_vecInstance;

IMPLEMENT_DYNAMIC(CClusterAgreeDlg, CXSkinDlg)

	CClusterAgreeDlg::CClusterAgreeDlg(CWnd* pParent /*=NULL*/,int iHintType)
	: CXSkinDlg(CClusterAgreeDlg::IDD, pParent)
{
	if ( bmpHintBackgnd.m_hObject==NULL )
	{
		bmpHintBackgnd.LoadBitmap(IDB_HINT_CLUSTER_DLG);
	}

	if ( bmpHintIcon.m_hObject==NULL )
	{
		bmpHintIcon.LoadBitmap(IDB_HINT_ICON);
	}

	HintID++;
	m_bMouseOverHint  = false;
	m_bMouseOverClose = false;
	m_bMouseOverFriend = false;
	m_bMouseOverCluster = false;
	m_bMouseOverAccept = false;
	m_bMouseOverRefuse = false;

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

	m_colorBorder		= BORDER_COLOR;
	m_bDrawBorder = false;
	m_hBkGround = ::CreateSolidBrush(RGB(255, 255, 255));

	m_pFaceCtrl = NULL;
	Create(IDD,pParent);
}

CClusterAgreeDlg::~CClusterAgreeDlg()
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

	if(m_pCluster != NULL)
	{
		delete m_pCluster;
		m_pCluster = NULL;
	}

	if(m_pFaceCtrl != NULL)
	{
		delete m_pFaceCtrl;
		m_pFaceCtrl = NULL;
	}
	m_bitmapFace = NULL;
}

void CClusterAgreeDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	// 	DDX_Control(pDX, IDC_STATIC_FRIEND, m_staticFriend);
	// 	DDX_Control(pDX, IDC_STATIC_CLUSTER, m_staticCluster);
}


BEGIN_MESSAGE_MAP(CClusterAgreeDlg, CXSkinDlg)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_ACTIVATE()
	ON_WM_PAINT()
	ON_XT_MESSAGE()
	ON_WM_CTLCOLOR()
	ON_WM_NCLBUTTONDOWN()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CClusterAgreeDlg)
ON_XT_COMMAND(CMD_CLUSTER_SEARCH_ACK,OnClusterSearchAck)
END_XT_MESSAGE_MAP()
// CClusterAgreeDlg 消息处理程序

void CClusterAgreeDlg::Show(CLUSTER_INFO * pCluster,FRIEND_INFO* pFriend,CString sTitle,CString sHint,HWND hCmdWnd)
{
	CClusterAgreeDlg *pHintDlg = new CClusterAgreeDlg(GetDesktopWindow(),0);
	pHintDlg->m_nLinkType = LINKT_CLUSTER;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetClusterInfo(pCluster);
	pHintDlg->SetFriendInfo(pFriend);
	pHintDlg->SetHintText(sTitle,sHint);
	pHintDlg->m_bAutoSink = true;
	pHintDlg->ShowWindow(SW_SHOW);
}

void CClusterAgreeDlg::Show(CString strHint, int iHintType, HWND hCmdWnd, bool bAutoSink)
{
	CClusterAgreeDlg *pHintDlg = new CClusterAgreeDlg(GetDesktopWindow(),iHintType);
	pHintDlg->m_nLinkType = LINKT_UNKNOWN;
	pHintDlg->SetCmdWnd(hCmdWnd,WM_HINTDLG_CLICK);
	pHintDlg->SetFriendInfo(NULL);
	pHintDlg->SetHintText("",strHint);
	pHintDlg->m_bAutoSink = bAutoSink;
	pHintDlg->ShowWindow(SW_SHOW);
}

void CClusterAgreeDlg::Show(CString strTitle,CString strHint, int iHintType, HWND hCmdWnd,bool bAutoSink)
{
	CClusterAgreeDlg *pHintDlg = new CClusterAgreeDlg(GetDesktopWindow(),iHintType);
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
BOOL CClusterAgreeDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	//	m_btnClose.SetBitmap(IDB_HINT_CLOSE_NORMAL,IDB_HINT_CLOSE_HOVER,IDB_HINT_CLOSE_HOVER,IDB_HINT_CLOSE_NORMAL);
	SetDlgStyle(CLOSE);
	SetTimer( UP_TIMER , 10 , NULL );//启动一个计时器，用来关闭本对话窗

	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW);

	int cx = 200;
	int cy = 110;

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

	//m_rcClose.SetRect(185,3,198,14);
	//m_rcFace.SetRect(3,45,47,89);
	m_rcHint.SetRect(56,28,162,78);

	m_rcTitle.SetRect(56,28,162,78);

	g_pProtocol->RegistCmd(CMD_CLUSTER_SEARCH_ACK,m_hWnd);

	m_pFaceCtrl = new CRountFaceCtrl(IDB_PNG_ROUND_NORMAL, IDB_PNG_ROUND_SEL);
	m_pFaceCtrl->SetBitmapHint(TRUE);
	m_pFaceCtrl->Create(this, 40001);
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
void CClusterAgreeDlg::OnTimer(UINT nIDEvent)
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
				SetTimer(SHOW_TIMER,300000,NULL);
				m_bDrawBorder = true;
			}
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

			CRect rcFriend	= m_rcFriend;
			ClientToScreen(&rcFriend);
			bool bInFriend = rcFriend.PtInRect(ptCursor);
			if ( bInFriend==FALSE )
			{
				m_bMouseOverFriend=false;
				Invalidate();
			}

			CRect rcCluster	= m_rcCluster;
			ClientToScreen(&rcCluster);
			bool bInCluster = rcCluster.PtInRect(ptCursor);
			if ( bInCluster==FALSE )
			{
				m_bMouseOverCluster=false;
				Invalidate();
			}

			CRect rcAccept	= m_rcAccept;
			ClientToScreen(&rcAccept);
			bool bInAccept = rcAccept.PtInRect(ptCursor);
			if ( bInAccept==FALSE )
			{
				m_bMouseOverAccept=false;
				Invalidate();
			}

			CRect rcRefuse	= m_rcRefuse;
			ClientToScreen(&rcRefuse);
			bool bInRefuse = rcRefuse.PtInRect(ptCursor);
			if ( bInRefuse==FALSE )
			{
				m_bMouseOverRefuse=false;
				Invalidate();
			}

			if ( bInClose==FALSE )
			{
				m_bMouseOverClose=false;
				Invalidate();
			}

			if ( /*bInHint==FALSE*/m_bMouseOverFriend==FALSE && m_bMouseOverCluster==FALSE && m_bMouseOverAccept == FALSE && m_bMouseOverRefuse ==FALSE && bInClose==FALSE  )
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


void CClusterAgreeDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();
	delete this;
}

int CClusterAgreeDlg::DoModal()
{
	return CDialog::DoModal();
}

void CClusterAgreeDlg::SetFriendInfo(FRIEND_INFO *pFriend)
{
	m_pFriend = pFriend ; 
	m_bitmapFace = NULL;

	//if ( pFriend )
	//{
	//	m_bitmapFace = ::GetUserBitmap(pFriend,false);
	//}
	//else
	//{
	//	m_bitmapFace = &bmpHintIcon;
	//}
}

void CClusterAgreeDlg::SetClusterInfo(CLUSTER_INFO* pCluster)
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

void CClusterAgreeDlg::SetHintText(CString strTitle,CString strHint)
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
	//m_staticCluster.SetWindowText(strHint);
}

BOOL CClusterAgreeDlg::PreTranslateMessage(MSG* pMsg)
{
	return CDialog::PreTranslateMessage(pMsg);
}

void CClusterAgreeDlg::SetCmdWnd(HWND hNotify, UINT uCmd)
{
	m_hWndNotify = hNotify;
	m_uMessageID = uCmd;
}

void CClusterAgreeDlg::FlipBackGround(CDC *pDC, CDC * pSafeDC)
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

	CRect rcCluster;
	//m_staticCluster.GetClientRect(rcCluster);

	CRect rcTitle = m_rcHint;
	//rcTitle.InflateRect(2,2);
	rcTitle.bottom=rcTitle.top+14;

	pDC->SetBkMode(TRANSPARENT);

	//BOOL bUnderLine = ( m_bMouseOverHint && m_iHintType==0 ) ? TRUE:FALSE;

	BOOL bUnderLineFriend = ( m_bMouseOverFriend && m_iHintType==0 ) ? TRUE:FALSE;
	BOOL bUnderLineCluster = ( m_bMouseOverCluster && m_iHintType==0 ) ? TRUE:FALSE;
	BOOL bUnderLineAccept = ( m_bMouseOverAccept && m_iHintType==0 ) ? TRUE:FALSE;
	BOOL bUnderLineRefuse = ( m_bMouseOverRefuse && m_iHintType==0 ) ? TRUE:FALSE;
	BOOL bUnderLine = (bUnderLineFriend || bUnderLineCluster);
	CFont fontTitle, fontHint, fontFriend, fontCluster, fontNormal, fontAccept, fontRefuse;
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

	fontNormal.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,						// bItalic
		false,                 // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	fontFriend.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,						// bItalic
		bUnderLineFriend,                 // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	fontCluster.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,						// bItalic
		bUnderLineCluster,                 // bUnderline
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

	fontAccept.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,						// bItalic
		bUnderLineAccept,               // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	fontRefuse.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                  // nWeight
		FALSE,						// bItalic
		bUnderLineRefuse,               // bUnderline
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

		pDC->DrawText(m_strTitle,/*rcTitle*/m_rcTitle,DT_END_ELLIPSIS|DT_TOP|DT_LEFT);

		pDC->SelectObject(&fontHint);

		if ( m_bMouseOverFriend ||  m_bMouseOverCluster)
		{
			pDC->SetTextColor(RGB(0,84,166));
		}
		else
		{
			pDC->SetTextColor(m_crHintText);
		}


		pDC->SelectObject(&fontNormal);
		//CRect rcFriendText;
		CSize sizeFriendText = pDC->GetTextExtent("好友");
		m_rcFriendText.SetRect(rcTitle.left,rcTitle.bottom+2,rcTitle.left+sizeFriendText.cx,rcTitle.bottom+2+sizeFriendText.cy);
		pDC->DrawText("好友",/*rcHint*/m_rcFriendText, DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

		pDC->SelectObject(&fontFriend);
		//CRect rcFriend;
		CString strNickName = m_pFriend->nickname ;
		for(int n = 0; n < strNickName.GetLength(); n++)
		{
			CString strr = strNickName.GetAt(n);
		}
		CSize sizeFriend = pDC->GetTextExtent("【" + strNickName + "】");
		m_rcFriend.SetRect(m_rcFriendText.right+1, m_rcFriendText.top, m_rcFriendText.right+1+sizeFriend.cx, m_rcFriendText.bottom);
		pDC->DrawText(/*m_pFriend->nickname*/"【" + strNickName + "】",/*rcHint*/m_rcFriend, DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

		pDC->SelectObject(&fontNormal);
		//CRect rcClusterText;
		CSize sizeClusterText = pDC->GetTextExtent("邀请您加入群");
		m_rcClusterText.SetRect(m_rcFriendText.left,m_rcFriend.bottom+2,m_rcFriendText.left+sizeClusterText.cx, m_rcFriend.bottom+2+sizeClusterText.cy);
		pDC->DrawText("邀请您加入群",/*rcHint*/m_rcClusterText,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

		pDC->SelectObject(&fontCluster);
		//CRect rcCluster;
		CSize sizeCluster = pDC->GetTextExtent("【"+m_strHint+"】");
		m_rcCluster.SetRect(m_rcClusterText.left,m_rcClusterText.bottom+2,m_rcClusterText.left+sizeCluster.cx,m_rcClusterText.bottom+2+sizeCluster.cy);
		pDC->DrawText("【"+m_strHint+"】",/*rcHint*/m_rcCluster,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

		pDC->SelectObject(&fontAccept);
		//CRect rcCluster;
		CSize sizeAccept = pDC->GetTextExtent("【接受】");
		m_rcAccept.SetRect(m_rcCluster.left,m_rcCluster.bottom+5,m_rcCluster.left+sizeAccept.cx,m_rcCluster.bottom+5+sizeAccept.cy);
		pDC->DrawText("【接受】",/*rcHint*/m_rcAccept,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);

		pDC->SelectObject(&fontRefuse);
		//CRect rcCluster;
		CSize sizeRefuse = pDC->GetTextExtent("【拒绝】");
		m_rcRefuse.SetRect(m_rcFace.left+120,m_rcCluster.bottom+5,m_rcFace.left+120+sizeAccept.cx,m_rcCluster.bottom+5+sizeRefuse.cy);
		pDC->DrawText("【拒绝】",/*rcHint*/m_rcRefuse,DT_END_ELLIPSIS|DT_TOP|DT_LEFT|DT_WORDBREAK);
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

BOOL CClusterAgreeDlg::OnEraseBkgnd(CDC* pDC)
{	
	CRect rcDlg;
	GetClientRect(&rcDlg);
	pDC->FillSolidRect(&rcDlg, RGB(255, 255, 255));
	return FALSE;
}

void CClusterAgreeDlg::OnClose()
{
	DestroyWindow();
}

void CClusterAgreeDlg::DrawBitmap(CDC *pDC,int x, int y, UINT nIDBmp)
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

void CClusterAgreeDlg::OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized)
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

void CClusterAgreeDlg::OnPaint()
{
	CPaintDC dc(this);

// 	CDC memDC;
// 	memDC.CreateCompatibleDC(&dc);
// 
// 	CRect rc;
// 	GetClientRect(&rc);
// 
// 	CBitmap bmp;
// 	bmp.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
// 
// 	CBitmap *pOldbmp = memDC.SelectObject(&bmp);

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

LRESULT CClusterAgreeDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

		// 		if ( rcHint.PtInRect(ptCursor) )
		// 		{
		// 			switch ( m_iHintType )
		// 			{
		// 			case 0:
		// 				if ( m_hWndNotify && m_bNotifySended==false )
		// 				{		
		// 					if(m_nLinkType==LINKT_FRIEND)
		// 					{
		// 						::PostMessage(m_hWndNotify,m_uMessageID,0,(LPARAM)m_pFriend);
		// 					}
		// 					else 
		// 						if(m_nLinkType==LINKT_CLUSTER)
		// 						{
		// 							::PostMessage(m_hWndNotify,m_uMessageID,0,(LPARAM)m_pCluster);
		// 						}	
		// 						else
		// 						{
		// 							::PostMessage(m_hWndNotify,m_uMessageID,0,0);
		// 						}
		// 
		// 						m_bNotifySended = true;
		// 						return TRUE;
		// 				}
		// 				break;
		// 			case 1:
		// 				break;
		// 			}
		// 		}
		CRect rcFriend	= m_rcFriend;
		ClientToScreen(&rcFriend);
		bool bMouseOverFriend = rcFriend.PtInRect(ptCursor);
		if ( bMouseOverFriend )
		{
			CFriendInfoDlg::NewDlg(m_pFriend);
		}

		CRect rcCluster	= m_rcCluster;
		ClientToScreen(&rcCluster);
		bool bMouseOverCluster = rcCluster.PtInRect(ptCursor);
		if(bMouseOverCluster)
		{
			//查看群信息
			XT_CLUSTER_SEARCH_REQ req;
			req.start_pos = 0;
			req.search_type = 0;
			req.nCount = 0;
			req.type_data.nFamilyID = m_pCluster->nFamilyID;
			g_pProtocol->SendReq(req);
		}

		CRect rcAccept	= m_rcAccept;
		ClientToScreen(&rcAccept);
		bool bMouseOverAccept = rcAccept.PtInRect(ptCursor);
		if(bMouseOverAccept)
		{
			//接受请求
			//判断如果是群主则直接入群，否则需要群主确认
			//if(m_pCluster->GetCreator() == m_pFriend)
			//{
			//	char buff[MAX_SEND_BUFFERS];
			//	XT_SET_CMEMBERS_REQ  & req = *((LPXT_SET_CMEMBERS_REQ)buff);
			//	req.nFamilyID   = m_pCluster->nFamilyID;
			//	req.nAction     = CMEMBER_ADD;
			//	req.nFamilyProp = PERM_FAMILY;
			//	req.nCount      = 1;

			//	g_pProtocol->SendReq(req);
			//}
			//else
			//{
			//	m_pCluster->GetCreator()->id;

			//	XT_CLUSTER_AUTH_REQ req;

			//	req.nAction = 0;
			//	req.nFamilyProp = m_pCluster->nFamilyProp;
			//	req.nFamilyID = m_pCluster->nFamilyID;
			//	req.nRequestID = g_ImUserInfo.GetId();
			//	req.nManagerID = 0;
			//	req.send_time  = time(NULL);
			//	g_pProtocol->SendReq(req);
			//}

			

 			char buff[MAX_SEND_BUFFERS];
 			XT_CLUSTER_AGREE_REQ & req = *((LPXT_CLUSTER_AGREE_REQ)buff);
 			req.action = 0;
 			req.invite_id = m_pFriend->GetId();
 			req.friend_id = g_ImUserInfo.GetId();
 			req.cluster_id = m_pCluster->nFamilyID;
 			req.cluster_type = m_pCluster->nFamilyProp;
 			g_pProtocol->ClusterAgree(req);

			PostMessage(WM_CLOSE,0,0);
		}

		CRect rcRefuse	= m_rcRefuse;
		ClientToScreen(&rcRefuse);
		bool bMouseOverRefuse = rcRefuse.PtInRect(ptCursor);
		if(bMouseOverRefuse)
		{
			char buff[MAX_SEND_BUFFERS];
			XT_CLUSTER_AGREE_REQ & req = *((LPXT_CLUSTER_AGREE_REQ)buff);
			req.action = 1;
			req.invite_id = m_pFriend->GetId();
			req.friend_id = g_ImUserInfo.GetId();
			req.cluster_id = m_pCluster->nFamilyID;
			req.cluster_type = m_pCluster->nFamilyProp;
			g_pProtocol->ClusterAgree(req);

			PostMessage(WM_CLOSE,0,0);
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

		//bool bMouseOverHint = rcHint.PtInRect(ptCursor);
		CRect rcFriend	= m_rcFriend;
		ClientToScreen(&rcFriend);
		bool bMouseOverFriend = rcFriend.PtInRect(ptCursor);
		if ( bMouseOverFriend )
		{
			if ( m_bMouseOverFriend==false )
			{
				m_bMouseOverFriend=true;
				Invalidate();
				SetTimer(MOUSE_TIMER,100,NULL);
			}
		}
		CRect rcCluster	= m_rcCluster;
		ClientToScreen(&rcCluster);
		bool bMouseOverCluster = rcCluster.PtInRect(ptCursor);
		if(bMouseOverCluster)
		{
			if ( m_bMouseOverCluster==false )
			{
				m_bMouseOverCluster=true;
				Invalidate();
				SetTimer(MOUSE_TIMER,100,NULL);
			}
		}
		CRect rcAccept	= m_rcAccept;
		ClientToScreen(&rcAccept);
		bool bMouseOverAccept = rcAccept.PtInRect(ptCursor);
		if(bMouseOverAccept)
		{
			if ( m_bMouseOverAccept==false )
			{
				m_bMouseOverAccept=true;
				Invalidate();
				SetTimer(MOUSE_TIMER,100,NULL);
			}
		}
		CRect rcRefuse	= m_rcRefuse;
		ClientToScreen(&rcRefuse);
		bool bMouseOverRefuse = rcRefuse.PtInRect(ptCursor);
		if(bMouseOverRefuse)
		{
			if ( m_bMouseOverRefuse==false )
			{
				m_bMouseOverRefuse=true;
				Invalidate();
				SetTimer(MOUSE_TIMER,100,NULL);
			}
		}

		switch(m_iHintType)
		{
		case 0:
			if ( m_hWndNotify && (bMouseOverFriend || bMouseOverCluster) && m_bNotifySended==false )
			{
				::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
			}
			break;
		case 1:
			if ( (bMouseOverFriend || bMouseOverCluster) )
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
		//SetTimer(MOUSE_TIMER,100,NULL);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CClusterAgreeDlg::OnClusterSearchAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_CLUSTER_SEARCH_ACK ack = (LPXT_CLUSTER_SEARCH_ACK)data;
		if(ack->nCount == 0)
		{

		}
		else
		{
			for ( int i=0; i<ack->nCount; i++ )
			{
				XT_CLUSTER_INFO * pInfo = new XT_CLUSTER_INFO();

				pInfo->nFamilyID  = ack->FamilySearchData[i].nFamilyID;
				pInfo->nCreatorID = ack->FamilySearchData[i].nCreaterID;
				pInfo->nJoinFlag  = ack->FamilySearchData[i].nJoinFlag;
				pInfo->nFamilyClass  = ack->FamilySearchData[i].nFamilyClass;
				strcpy(pInfo->szFamilyName,ack->FamilySearchData[i].szFamilyName);
				strcpy(pInfo->szFamilyDomain,ack->FamilySearchData[i].szFamilyDomain);

				std::tr1::shared_ptr<CClusterInfoDlg> pWnd=CClusterInfoDlg::NewDlg(pInfo);
				if(pWnd)
				{
					// 向服务器请求群信息
					XT_GET_CINFO_REQ req;
					req.nFamilyID   = pInfo->nFamilyID;
					req.nFamilyProp = PERM_FAMILY;
					g_pProtocol->SendReq(req);
					// 显示群信息
					//pWnd->EnableUpdateBtn(FALSE);//不显示更新按钮
					pWnd->ShowWindow(SW_SHOW);
					//pWnd->CenterWindow(GetDesktopWindow());
				}
			}
		}
	}	
}

HBRUSH CClusterAgreeDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
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

void CClusterAgreeDlg::OnNcLButtonDown(UINT nHitTest, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	return;
	CXSkinDlg::OnNcLButtonDown(nHitTest, point);
}
