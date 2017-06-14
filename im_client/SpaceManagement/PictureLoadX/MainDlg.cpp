#include "..\mypictures\maindlg.h"
// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureLoadX.h"
#include "MainDlg.h"
#include ".\maindlg.h"
#include "io.h"
#include "WebPictureProcess.h"

#define   RC_MIN        0
#define   RC_POPUPMENU   1
#define   RC_PIC_SCROLL_DLG   2
#define   RC_PICTURE_SHOW    3
#define   RC_CLICK_PICTURE   1
#define   RC_CLICK_PICTURESBROAD_DLG   2
#define   RC_CLICK_POPMENU      3
#define   RC_CLICK_MIN			4
// CMainDlg 对话框
//CMainDlg  *pInstance;  //ok

CString UrlToLocalFile(CString strUrl)
{
	CString strLocalFile;
	strLocalFile.Format("%s%s",g_strCachePath,MakeGUID((char *)(LPCSTR)strUrl));
	return strLocalFile;
}

CMainDlg *  CMainDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CMainDlg, CDialog)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMainDlg::IDD, pParent)
{
	m_bPicturesListDownLoadEnd=FALSE;
	m_nThreadRef=0;
	m_bStopDownload = false;
	m_nBitmapIndex = -1;
	m_nPictureIndex = 0;
	m_fontWnd.CreateFont(
		14,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");                 // lpszFacename
	m_nInterval=3000;
	m_bAlwaysTop=TRUE;
	m_bEmptyPictures=TRUE;
	m_buttonDownPos=0;
	m_nBroadButtonState=0;
	m_nMenuButtonState=0;
	m_nCloseButtonState=0;	
	m_bStopDown=FALSE;	
	
	m_pDlgShowPics = NULL;
	m_hThreadInit = NULL;

	int m_nUserCount=g_vecUserID.size();
	if(m_nUserCount>0)  m_nDownPicturesCount=PICTURES_MAX/m_nUserCount;

	Create(CMainDlg::IDD,pParent);
}

CMainDlg::~CMainDlg()
{
	pInstance = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CMainDlg, CDialog)
	ON_WM_NCMOUSEMOVE()
	ON_WM_ERASEBKGND()
	ON_WM_CLOSE()
	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_NCHITTEST()
	ON_WM_DESTROY()
	ON_WM_SETCURSOR()
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_MESSAGE(WM_WEBPICTURE_NOTIFY,OnWebPictureNotify)
	ON_WM_TIMER()
	ON_MESSAGE(WM_SHOWTRAY,  OnTrayClicked)
	ON_COMMAND(ID_TASKBAR_EXIT,OnTaskbarExit)
	ON_COMMAND(ID_TASKBAR_MIN,OnTaskbarMin)
	ON_COMMAND(ID_TASKBAR_SHOWDLG,OnTaskbarShowdlg)
	ON_COMMAND(ID_TASKBAR_OPTION, OnTaskbarOption)
	ON_MESSAGE(WM_DATA_UPDATE,OnDataUpdate)

END_MESSAGE_MAP()

void CMainDlg::Show()
{
	if(g_vecUserID.size()>0)
	{
		if ( pInstance == NULL )
		{
			pInstance = new CMainDlg(GetDesktopWindow());
			pInstance->ShowWindow(SW_SHOW);
		}
		else
		{
			pInstance->ShowWindow(SW_SHOW);
		}
	}
}



void CMainDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
	
}

void  CMainDlg::SetWindowTop(BOOL bWndTop)
{
	l_bWndTop=bWndTop;
}

void  CMainDlg::SetInterval(int nInterval)
{
	l_nInterval=nInterval;
}

void  CMainDlg::IsEmptyPictures(BOOL bEmptyPictures)
{
	l_bEmptyPictures=bEmptyPictures;
}

void CMainDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CDialog::OnClose();
	
}


void CMainDlg::DrawFrame(CDC *pDC)
{
	CRect rcClient;
	GetClientRect(&rcClient);
	int cx = rcClient.Width();
	int cy = rcClient.Height();

	pDC->FillSolidRect(0,0,cx,20,RGB(203,235,248));
	pDC->FillSolidRect(0,20,cx,cy-20,RGB(255,255,255));
	pDC->Draw3dRect(&rcClient,RGB(128,128,128),RGB(128,128,128));
	//pDC->RoundRect(&m_rcMin,CPoint(100,100));
	pDC->Draw3dRect(&m_rcMin,RGB(0,0,0),RGB(0,0,0));
	pDC->Draw3dRect(&m_rcPopMenu,RGB(0,0,0),RGB(0,0,0));
	pDC->Draw3dRect(&m_rcPictureBroadDlg,RGB(0,0,0),RGB(0,0,0));
	pDC->SetBkMode(TRANSPARENT);
	CFont font;
	font.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");                 // lpszFacename
	pDC->SelectObject(&font);
	pDC->SetTextColor(RGB(0,84,188));
	pDC->DrawText("Mini图片联播",CRect(25,5,rcClient.right,20),DT_LEFT);	
   /*   标题  */
	CResBitmap bmpCaption,bmpPicsDlg,bmpMenu,bmpMin;
	bmpCaption.LoadBitmap(IDB_PICTURE_BROAD_CAPTION);
	::DrawBitmap(pDC,2,2,&bmpCaption,RGB(0,255,0));

	/*  窗口右上系统按钮*/
	switch(m_nBroadButtonState)
	{
	case 0:
		bmpPicsDlg.LoadBitmap(IDB_PICTURE_BROAD_SCROLL_DLG_NORMAL);
		break;
	case 1:
		bmpPicsDlg.LoadBitmap(IDB_PICTURE_BROAD_SCROLL_DLG_HOT);
		break;
	case 2:
		bmpPicsDlg.LoadBitmap(IDB_PICTURE_BROAD_SCROLL_DLG_CLICK);
		break;
	}	
	::DrawBitmap(pDC,m_rcPictureBroadDlg.left,m_rcPictureBroadDlg.top,&bmpPicsDlg,0);

	switch(m_nMenuButtonState)
	{
	case 0:
		bmpMenu.LoadBitmap(IDB_PICTURE_BROAD_MENU_NORMAL);
		break;
	case 1:
		bmpMenu.LoadBitmap(IDB_PICTURE_BROAD_MENU_HOT);
		break;
	case 2:
		bmpMenu.LoadBitmap(IDB_PICTURE_BROAD_MENU_CLICK);
		break;
	}	
	::DrawBitmap(pDC,m_rcPopMenu.left,m_rcPopMenu.top,&bmpMenu,0);

	switch(m_nCloseButtonState)
	{
	case 0:
		bmpMin.LoadBitmap(IDB_MIN_HOT);
		break;
	case 1:
		bmpMin.LoadBitmap(IDB_MIN_HOT);
		break;
	case 2:
		bmpMin.LoadBitmap(IDB_MIN_HOT);
		break;
	}	

	::DrawBitmap(pDC,m_rcMin.left,m_rcMin.top,&bmpMin,0);

}

CString CMainDlg::GetCurTime()
{
	SYSTEMTIME sysCurTime;
	GetLocalTime(&sysCurTime);
	CString  strLocalTime;
	CString strMonth,strDay,strHour,strMinute,strSecond;

	if(sysCurTime.wMonth<10)
		strMonth.Format("0%d",sysCurTime.wMonth);
	else
		strMonth.Format("%d",sysCurTime.wMonth);
	if(sysCurTime.wDay<10)
		strDay.Format("0%d",sysCurTime.wDay);
	else
		strDay.Format("%d",sysCurTime.wDay);
	if(sysCurTime.wHour<10)
		strHour.Format("0%d",sysCurTime.wHour);
	else
		strHour.Format("%d",sysCurTime.wHour);
	if(sysCurTime.wMinute<10)
		strMinute.Format("0%d",sysCurTime.wMinute);
	else
		strMinute.Format("%d",sysCurTime.wMinute);
	if(sysCurTime.wSecond<10)
		strSecond.Format("0%d",sysCurTime.wSecond);
	else
		strSecond.Format("%d",sysCurTime.wSecond);
	strLocalTime.Format("%d-%s-%s %s:%s:%s",sysCurTime.wYear,strMonth,strDay,strHour,strMinute,strSecond);
	return strLocalTime;
}
CString CMainDlg::GetLastMonth()   //前30天的时间
{
	CTime  tm=CTime::GetCurrentTime();	
	CTimeSpan  tsp(30,0,0,0);
	CTime  tm1=tm-tsp;	
	CString strLastMonth;
	CString strMonth,strDay,strHour,strMinute,strSecond;
	int    nYear,nMonth,nDay,nHour,nMinute,nSecond;
	nYear=tm1.GetYear();
	nMonth=tm1.GetMonth();
	nDay=tm1.GetDay();
	nHour=tm1.GetHour();
	nMinute=tm1.GetMinute();
	nSecond=tm1.GetSecond();	

	if(nMonth<10)
		strMonth.Format("0%d",nMonth);
	else
		strMonth.Format("%d",nMonth);
	if(nDay<10)
		strDay.Format("0%d",nDay);
	else
		strDay.Format("%d",nDay);
	if(nHour<10)
		strHour.Format("0%d",nHour);
	else
		strHour.Format("%d",nHour);
	if(nMinute<10)
		strMinute.Format("0%d",nMinute);
	else
		strMinute.Format("%d",nMinute);
	if(nSecond<10)
		strSecond.Format("0%d",nSecond);
	else
		strSecond.Format("%d",nSecond);
	strLastMonth.Format("%d-%s-%s %s:%s:%s",nYear,strMonth,strDay,strHour,strMinute,strSecond);
	return  strLastMonth;
}
CString  CMainDlg::GetLastTwoMonth()
{
	CTime  tm=CTime::GetCurrentTime();

	CTimeSpan  tsp(60,0,0,0);
	CTime  tm1=tm-tsp;	
	CString strLastTwoMonth;
	CString strMonth,strDay,strHour,strMinute,strSecond;
	int    nYear,nMonth,nDay,nHour,nMinute,nSecond;
	nYear=tm1.GetYear();
	nMonth=tm1.GetMonth();
	nDay=tm1.GetDay();
	nHour=tm1.GetHour();
	nMinute=tm1.GetMinute();
	nSecond=tm1.GetSecond();	

	if(nMonth<10)
		strMonth.Format("0%d",nMonth);
	else
		strMonth.Format("%d",nMonth);
	if(nDay<10)
		strDay.Format("0%d",nDay);
	else
		strDay.Format("%d",nDay);
	if(nHour<10)
		strHour.Format("0%d",nHour);
	else
		strHour.Format("%d",nHour);
	if(nMinute<10)
		strMinute.Format("0%d",nMinute);
	else
		strMinute.Format("%d",nMinute);
	if(nSecond<10)
		strSecond.Format("0%d",nSecond);
	else
		strSecond.Format("%d",nSecond);
	strLastTwoMonth.Format("%d-%s-%s %s:%s:%s",nYear,strMonth,strDay,strHour,strMinute,strSecond);
	return  strLastTwoMonth;
}

UINT  CMainDlg::InitWebPictureListProc(LPVOID lpParam)
{	
	CMainDlg * pDlg = (CMainDlg *)lpParam;
	CString strServerDate="";
	
	CString strLocalTime, strLocalTimeOfLastMonth,strLocalTimeOfLastTwoMonth;

	strLocalTime = GetCurTime();
	strLocalTimeOfLastMonth = GetLastMonth();
	strLocalTimeOfLastTwoMonth = GetLastTwoMonth();
	
	vector <TWebPictureInfo *> tempWebPicturesList;
	int nUserCount=g_vecUserID.size();


	for(int i=0;i<nUserCount;i++)
	{     
		tempWebPicturesList.clear();
		
		try
		{
			CWebInterfaceWrapper  objWi;	
			objWi.InitInterface(g_nUserID,(LPCSTR)g_strUserName,(LPCSTR)g_strNickName);   
			objWi.GetUserPictureList(g_vecUserID[i],strLocalTimeOfLastMonth,strLocalTime,tempWebPicturesList,strServerDate);
		}
		catch(...)
		{
		}

		int nTempListCount=tempWebPicturesList.size();

		for(int j=0;j<nTempListCount;j++)
		{
			tempWebPicturesList[j]->uID = g_vecUserID[i];
			g_webPictureList.push_back(tempWebPicturesList[j]);
		}

		if( nTempListCount>0)
		{
			pDlg->PostMessage(WM_THREAD_NOTIFY,1,0);
		}	
		
		if ( pDlg->m_bStopDownload )
		{
			pDlg->m_bStopDownload = false;
			break;
		}

		if ( g_webPictureList.size()>150 )
		{
			break;
		}
	}

	pDlg->PostMessage(WM_THREAD_NOTIFY,1,2);


	return 1;
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	
	CRect rc;
	GetClientRect(&rc);

	CDC dcMem;
	CBitmap bmpMem;

	dcMem.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());

	int nSaveDC = dcMem.SaveDC();
	dcMem.SelectObject(&bmpMem);	
	dcMem.FillSolidRect(&rc,RGB(203,235,248));

	DrawFrame(&dcMem);
	DrawPicture(&dcMem);
	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSaveDC);
}

void CMainDlg::DrawPicture(CDC *pDC)
{
	
	if ( m_nBitmapIndex==-1 || g_vecBitmap.size()==0 )
	{
		pDC->SetBkMode(TRANSPARENT);
		pDC->SelectObject(&m_fontWnd);

		pDC->SetTextColor(0xAAAAAA);
		pDC->DrawText("正在下载图片",&m_rcPicture,DT_CENTER|DT_VCENTER|DT_SINGLELINE);
	}
	else
	{		
		CBitmap * pBitmap = g_vecBitmap[m_nBitmapIndex].pBitmap;
		::DrawBitmap(pDC,m_rcPicture.left,m_rcPicture.top,pBitmap,0);
	}	
}


void CMainDlg::InitDlgInfo()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx, cy;

	cx=rcClient.Width();
	cy=rcClient.Height();

	m_rcMin.left=cx-17;
	m_rcMin.top=3;
	m_rcMin.right=cx-3;
	m_rcMin.bottom=17;
	m_rcPopMenu.left=cx-37;
	m_rcPopMenu.top=3;
	m_rcPopMenu.right=cx-23;
	m_rcPopMenu.bottom=17;
	m_rcPictureBroadDlg.left=cx-57;
	m_rcPictureBroadDlg.top=3;
	m_rcPictureBroadDlg.right=cx-43;
	m_rcPictureBroadDlg.bottom=17;

	m_rcPicture = rcClient;

	m_rcPicture.left+=10;
	m_rcPicture.top += 27;
	m_rcPicture.right=m_rcPicture.left+BMP_WIDTH;
	m_rcPicture.bottom=m_rcPicture.top+BMP_HEIGHT;
}
void CMainDlg::InitDlgPosition()
{
	CRect rcWindow;
	CRect rcClient;

	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);

	
	int screen_cx = GetSystemMetrics(SM_CXSCREEN);
	int screen_cy = GetSystemMetrics(SM_CYSCREEN);

	CWnd *pShellTrayWnd = FindWindow(_T("Shell_TrayWnd"),NULL);

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

	CRect rcDlgShow;
	rcDlgShow.left=screen_cx-rcClient.Size().cx-2;
	rcDlgShow.right=screen_cx-2;
	rcDlgShow.bottom= screen_cy;
	rcDlgShow.top   = rcDlgShow.bottom-rcClient.Height();

	MoveWindow(&rcDlgShow);	
}

BOOL CMainDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOSIZE|SWP_NOMOVE);

	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW);
	InitDlgPosition();	
	
    InitDlgInfo();

	SetTimer(0x01,l_nInterval*1000,NULL);
	//ShowTray(TRUE);
	EnableToolTips(TRUE);
	InitToolTip();	
	
	DWORD dwThreadID;
	m_hThreadInit  = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)InitWebPictureListProc,
			(LPVOID)this,
			0,
			&dwThreadID);

	SetWindowText("MiniPictureBroadCast");

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

int CMainDlg::HitTest(CPoint point)
{
	if (PtInRect(&m_rcMin,point))
	{
		return RC_MIN;
	}
	if (PtInRect(&m_rcPopMenu,point))
	{
		return  RC_POPUPMENU;
	}
	if (PtInRect(m_rcPictureBroadDlg,point)) 
	{
		return RC_PIC_SCROLL_DLG;
	}
	if (PtInRect(m_rcPicture,point)) 
	{
		return RC_PICTURE_SHOW;
	}	
	return  -1;
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	int HitSel=HitTest(point);
	switch(HitSel) 			
	{
	case  RC_MIN:
		{
			m_nCloseButtonState=2;
			m_buttonDownPos=4;
		}

		break;
	case RC_POPUPMENU:
		{	
		m_nMenuButtonState=2;
		m_buttonDownPos=3;
		}
		break;
	case RC_PIC_SCROLL_DLG:
		{
			m_nBroadButtonState=2;
			m_buttonDownPos=2;
		}	
		break;
	case  RC_PICTURE_SHOW:
		{
			m_buttonDownPos=1;			
		}
		break;
	default:
		{
			m_nCloseButtonState=0;
			m_nMenuButtonState=0;
			m_nBroadButtonState=0;
		}
	}	
	if(-1!=HitSel)
		Invalidate();
	CDialog::OnLButtonDown(nFlags, point);
}

void CMainDlg::OnOpenWebPicture(CPoint  pt)
{	
	int nBitmaps=g_vecBitmap.size();
	for(int i=0;i<nBitmaps;i++)
	{	
		if(i==m_nBitmapIndex&&PtInRect(m_rcPicture,pt))
		{
			for(int j=0;j<nBitmaps;j++)
			{
				DWORD   dwSID=g_vecBitmap[m_nBitmapIndex].dwPictureID;
				DWORD   dwDID=g_webPictureList[j]->dwPictureID;
		
				if(dwSID==dwDID)
				{ 
					::SendMessage(::g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MiniPictureBroadCast,(LPARAM)g_webPictureList[j]);					
					break;					
				}			
			 }

			
			break;
		}
	}
}

void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{	
	int HitSel=HitTest(point);
	if(RC_MIN==HitSel)  
	{
		m_nCloseButtonState=1;
	}
		
	else
	{
		m_nCloseButtonState=0;
	}
		
	if(RC_PIC_SCROLL_DLG==HitSel)   
		m_nBroadButtonState=1;
	else
		m_nBroadButtonState=0;
	if(RC_POPUPMENU==HitSel)   
		m_nMenuButtonState=1;
	else
		m_nMenuButtonState=0;
	if(-1!=HitSel)
		Invalidate();

	if(RC_MIN==HitSel)
	{
		if(m_nCurBtn!=3)  
		{
			ShowToolTip("隐藏");
			m_nCurBtn=3;
		}
	}
	else	
		if(m_nCurBtn==3)
		{
			ShowToolTip("");
		}


	if(RC_POPUPMENU==HitSel)
	{
		if(m_nCurBtn!=2)  
		{
			ShowToolTip("设置菜单");
			m_nCurBtn=2;
		}
	}
	else	
		if(m_nCurBtn==2)
		{
			ShowToolTip("");
		}


		if(RC_PIC_SCROLL_DLG==HitSel)
		{
			if(m_nCurBtn!=1)  
			{
				ShowToolTip("图片联播窗口");
				m_nCurBtn=1;
			}
		}
		else	
			if(m_nCurBtn==1)
			{
				ShowToolTip("");
			}
		//if(-1==HitSel)
		if(m_nCurBtn==0)
		{
		//	m_nCurBtn=0;
			ShowToolTip("");			
		   Invalidate();
		}
	if(HitSel==RC_PICTURE_SHOW)
	{
		if(m_nCurBtn!=4)
		{
			if(m_nBitmapIndex>=0 && m_nBitmapIndex<g_vecBitmap.size() )
			{
				int nBitmaps	=	g_webPictureList.size();
				int nPictureIndex=-1;
				for(int i=0;i<nBitmaps;i++)
				{
					if(g_vecBitmap[m_nBitmapIndex].dwPictureID==g_webPictureList[i]->dwPictureID)						
					{
						nPictureIndex=i;
						break;
					}
				}
				if(nPictureIndex!=-1)
				{
					CString  str=g_webPictureList[nPictureIndex]->szTitle;
					ShowToolTip(str);
					m_nCurBtn=4;
				}
			}
		}

	}

	CRect  rcPicture;
	
	if (PtInRect(m_rcPicture,point)) 
	{
		KillTimer(0x01);
	}
	

	CDialog::OnMouseMove(nFlags, point);
}

void CMainDlg::ShowToolTip( CString strTip)
{
	InitToolTip();
	// If there is no tooltip defined then add it
	if (m_ToolTip.GetToolCount() != 0)
	{
		m_ToolTip.DelTool(this,1);
	}

	if (strTip == "") 
	{
		return;
	}

	CRect rect; 
	GetClientRect(rect);
	m_ToolTip.AddTool(this, strTip, rect, 1);

	//m_ToolTip.UpdateTipText(strTip,this,1);

	m_ToolTip.Activate(TRUE);
}

void CMainDlg::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;

		m_ToolTip.Activate( TRUE ) ;
	} 
}

LRESULT CMainDlg::OnNcHitTest(CPoint point)
{
	CRect rc;
	GetClientRect(&rc);

	CRect rcButtons;
	rcButtons.left=rc.Width()-60;
	rcButtons.top=0;
	rcButtons.right=rc.Width();
	rcButtons.bottom=20;
	ScreenToClient(&point);
	 if(PtInRect(&rcButtons,point))
	{
		return  CDialog::OnNcHitTest(point);
	}
	 else	
	 {		if(PtInRect(&m_rcPicture,point))
				return HTCLIENT;
			else	
			  return HTCAPTION;
	 }	 
}

LRESULT CMainDlg::OnWebPictureNotify(WPARAM wParam, LPARAM lParam)
{
	if ( wParam==1 )
	{
		CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;
		
		CBitmap * pBitmap = CBitmapFromFile(result->szLocalFile,BMP_WIDTH,BMP_HEIGHT);

		if ( pBitmap )
		{
			TWebPictureInfo *pPictureInfo = (TWebPictureInfo *)lParam;
			TBitmap b;
			b.dwPictureID = pPictureInfo->dwPictureID;
			b.pBitmap	  = pBitmap;		
			g_vecBitmap.push_back(b);
		
			m_nThreadRef--;    

			int nPictures = g_webPictureList.size()-m_nPictureIndex;

			if ( nPictures>0 )
			{
				g_webPictureProcess.GetWebPicture(0,*g_webPictureList[m_nPictureIndex++],240,this);									
				m_nThreadRef++;
			}
		}		
	}
	
	return 0;
}

BOOL  CMainDlg::IsPictureDownLoad(int nUserIndex)
{

	return FALSE;
	
}

LRESULT CMainDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	switch (wParam)
	{
	case 1://图片列表下载成功 
		{			
			if ( lParam==0 )
			{
				if(m_nThreadRef<MAX_THREAD )				
				{
					int nPictures   = g_webPictureList.size()-m_nPictureIndex;
					int nNewThreads = MAX_THREAD-m_nThreadRef;

					int nThreads = 0;

					if ( nNewThreads>0 )
					{
						if ( nPictures>0 )
						{
							if ( nNewThreads<nPictures )
							{
								nThreads = nNewThreads;
							}
							else
							{
								nThreads = nPictures;
							}
						}
					}

					for(int i=0;i<nThreads;i++)
					{					
						if(m_nPictureIndex<g_webPictureList.size())
						{
							g_webPictureProcess.GetWebPicture(0,*g_webPictureList[m_nPictureIndex++],240,this);									
							m_nThreadRef++;
						}
					}
				}	
			}
			else
			if ( lParam==2 )
			{				
				m_bPicturesListDownLoadEnd=TRUE;

				if ( m_hThreadInit )
				{
					CloseHandle(m_hThreadInit);
					m_hThreadInit=NULL;
				}
			}
		}
		break;
	case 0:    //
		{
		
		}
		break;
	}
	return 1;
}

void CMainDlg::OnTimer(UINT nIDEvent)
{	
	if ( nIDEvent==0x01 )
	{
		static int nCount=0;
		nCount++;
		if ( nCount++>=1 )
		{
			m_nBitmapIndex++;

			int nBitmaps = g_vecBitmap.size();

			if ( m_nBitmapIndex>=nBitmaps )
			{
				m_nBitmapIndex=0;
			}
			nCount = 0;
			Invalidate();
		}
	}


	CDialog::OnTimer(nIDEvent);
}

LRESULT CMainDlg::OnTrayClicked(WPARAM wParam, LPARAM lParam)
{
	if(wParam != IDI_PICTURE_BROADCAST_TRAY)  
		return 1;  

	switch(lParam)  
	{  
	case   WM_RBUTTONUP: 
		{  

			CPoint  point;
			GetCursorPos(&point);//得到鼠标位置  
			CMenu   menu, *pmenu;   //弹出式菜单
			menu.LoadMenu(IDR_MENU_TASKBAR);			
			pmenu=menu.GetSubMenu(0);
			SetForegroundWindow();			
			pmenu->TrackPopupMenu(TPM_LEFTALIGN, point.x, point.y, pInstance);  
			pmenu=NULL;				
		}  
		break;  
	case   WM_LBUTTONDBLCLK:
		{  
			ShowWindow(SW_SHOW);
			ShowTray(FALSE);			
		}  
		break;  
	}

	return 0;
}

void  CMainDlg::ShowTray(BOOL bShow)
{
	NOTIFYICONDATA nid;  
	nid.cbSize            = (DWORD)sizeof(NOTIFYICONDATA);  
	nid.hWnd              = m_hWnd;  
	nid.uID                =IDI_PICTURE_BROADCAST_TRAY; 
	nid.uFlags            = NIF_ICON|NIF_MESSAGE|NIF_TIP   ;  
	nid.uCallbackMessage= WM_SHOWTRAY; //自定义的消息
	//nid.hIcon            = LoadIcon(AfxGetInstanceHandle(),MAKEINTRESOURCE(IDR_MAINFRAME));  
	//nid.hIcon            = m_hIcon;
	nid.hIcon			   =::ResLoadIcon(IDI_PICTURE_BROADCAST_TRAY);
	CString str;
	str.Format("%s 的好友图片",g_strNickName);	
	sprintf(nid.szTip,str);
	//strcpy(nid.szTip,("Mini图片联播"));
	if (bShow)
		Shell_NotifyIcon(NIM_ADD,&nid);
	else
		Shell_NotifyIcon(NIM_DELETE,&nid);
}

void CMainDlg::OnTaskbarMin()
{	
	ShowWindow(SW_HIDE);
	ShowTray(TRUE);
	if (m_pDlgShowPics) 
		m_pDlgShowPics->ShowWindow(SW_HIDE);
	
}
void CMainDlg::OnTaskbarExit()
{	
	m_bStopDown=TRUE;
	DestroyWindow();
}

BOOL CMainDlg::EmptyPictures(char* sDirName)
{
	CFileFind tempFind; 
	char sTempFileFind[200] ;
	sprintf(sTempFileFind,"%s\\*.*",sDirName); 
	BOOL IsFinded = tempFind.FindFile(sTempFileFind);  
		while (IsFinded) 
		{
				IsFinded = tempFind.FindNextFile();
				if (!tempFind.IsDots()) 
				{	
				char sFoundFileName[200]; 
				strcpy(sFoundFileName,tempFind.GetFileName().GetBuffer(200)); 

				if (tempFind.IsDirectory())
					{  
					char sTempDir[200]; 
				sprintf(sTempDir,"%s\\%s",sDirName,sFoundFileName); 
				DeleteDirectory(sTempDir); 
					} 
				else 
					{  
					char sTempFileName[200]; 
				sprintf(sTempFileName,"%s\\%s",sDirName,sFoundFileName); 
				DeleteFile(sTempFileName); 
					} 
				}	
			} 
			tempFind.Close(); 
			if(!RemoveDirectory(sDirName)) 
				{  
				return FALSE; 
				} 
				return TRUE; 
} 


void CMainDlg::OnTaskbarShowdlg()
{
	ShowWindow(SW_SHOW);	
	ShowTray(FALSE);

	if(m_pDlgShowPics&&g_bPicsDlgVisible)
		m_pDlgShowPics->ShowWindow(SW_SHOW);
}

void CMainDlg::OnTaskbarOption()
{
	/*if(!m_optionDlg)
	{
		m_optionDlg.Create(COptionDlg::IDD,pInstance);
		m_optionDlg.ShowWindow(SW_SHOW);
	}
	else
	m_optionDlg.ShowWindow(SW_SHOW);*/
	
}
LRESULT  CMainDlg::OnDataUpdate(WPARAM  wParam,LPARAM  lParam)
{
	//获取窗口设置
	/*m_nInterval=m_optionDlg.GetInterval();
	m_bAlwaysTop=m_optionDlg.IsAlwaysTop(); 
	m_bEmptyPictures=m_optionDlg.IsEmptyPictures();
	SetTimer(0x01,m_nInterval*1000,NULL);
	if(m_bAlwaysTop)
		SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	else
		SetWindowPos(&CWnd::wndNoTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);*/
		
	return 0;
}
BOOL CMainDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	CPoint  pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);
	int nBitmaps=g_vecBitmap.size();
	if(nBitmaps>0)
	{	
	if(this==pWnd&&m_rcPicture.PtInRect(pt))
	{
		m_hCursor=LoadCursor(NULL,IDC_HAND);
		if(m_hCursor==(HCURSOR)0)
		{
			m_hCursor=LoadCursor(NULL,IDC_HAND);
		}
		else
			::SetCursor(m_hCursor);		
		return TRUE;
	}
	}
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int HitSel=HitTest(point);
	switch(HitSel) 
	{
	case RC_MIN:   //最小化窗口
		{
			m_nCloseButtonState=0;
			ShowTray(TRUE);
			ShowWindow(SW_HIDE);
			if(m_pDlgShowPics)
				m_pDlgShowPics->ShowWindow(SW_HIDE);
		}
		break;
	case RC_POPUPMENU:
		{
			m_nMenuButtonState=0;	
			Invalidate(TRUE);
			CPoint  pt;
			CMenu  menu;
			GetCursorPos(&pt);
			menu.LoadMenu(IDR_MENU_TASKBAR);     //和任务栏的菜单一样
			CMenu  *menupop=menu.GetSubMenu(0);
			menupop->EnableMenuItem(ID_TASKBAR_SHOWDLG,MF_GRAYED);
			menupop->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,pt.x,pt.y,this,0);
			menupop=NULL;			
		}
		break;
	case RC_PIC_SCROLL_DLG:
		{
			m_nBroadButtonState=0;
			Invalidate(TRUE);
			//if(g_webPictureList.size()>0)
			{
				if(m_pDlgShowPics==NULL)
				{		
					m_pDlgShowPics = new CDisplayPictures(GetDesktopWindow());
					m_pDlgShowPics->Create(IDD_DIALOG_PICTURES_SHOW,GetDesktopWindow());
					m_pDlgShowPics->ShowWindow(SW_SHOW);
				}
				else
				{
					if(m_pDlgShowPics->IsWindowVisible())
					{
						m_pDlgShowPics->ShowWindow(SW_HIDE);
					}
					else
					{
						m_pDlgShowPics->ShowWindow(SW_SHOW);
						
					}
				}
			}			
		}	
		break;
	case  RC_PICTURE_SHOW:
		{
			OnOpenWebPicture(point);
		}
		break;
	}
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{	
	return  FALSE;
	//return CDialog::OnEraseBkgnd(pDC);
}
void CMainDlg::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	m_nBroadButtonState=0;
	m_nMenuButtonState=0;
	m_nCloseButtonState=0;
	m_nCurBtn=0;
	ShowToolTip("");
	Invalidate();
	
	SetTimer(0x01,l_nInterval*1000,NULL);
	

	CDialog::OnNcMouseMove(nHitTest, point);
}
LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if ( m_ToolTip.m_hWnd && message==WM_MOUSEMOVE )
	{
		MSG msg;
		msg.hwnd = m_hWnd;
		msg.message = message;
		msg.time = GetTickCount();
		msg.wParam = wParam;
		msg.lParam = lParam;
		GetCursorPos(&msg.pt);
		m_ToolTip.RelayEvent(&msg);
	}

	return CDialog::WindowProc(message, wParam, lParam);
}

void CMainDlg::OnDestroy()
{	
	if ( m_hThreadInit )
	{
	//	m_bStopDownload = true;
	//	while (m_bStopDownload) ;

		try
		{
			TerminateThread(m_hThreadInit,0);
			CloseHandle(m_hThreadInit);
		}
		catch(...)
		{
		}

		m_hThreadInit = NULL;		
	}

	ShowTray(FALSE);	
	g_webPictureProcess.SafeTerminate();	
	int n;	
	n = g_vecBitmap.size();

	for ( int i=0; i<n; i++ )
	{
		delete g_vecBitmap[i].pBitmap;
	}
	g_vecBitmap.clear();	
	n = g_webPictureList.size();

	for ( int i=0; i<n; i++ )
		delete g_webPictureList[i];
	
	g_webPictureList.clear();	
	g_vecUserID.clear();	
	
	if(m_pDlgShowPics)
	{
		m_pDlgShowPics->DestroyWindow();
		delete m_pDlgShowPics;
		m_pDlgShowPics = NULL;
	}
	
	CDialog::OnDestroy();

	delete this;	
}
