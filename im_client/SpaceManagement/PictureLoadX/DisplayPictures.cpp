// DisplayPictures.cpp : 实现文件
//

#include "stdafx.h"
#include "PictureLoadX.h"
#include "DisplayPictures.h"
#include ".\displaypictures.h"
#include "atlimage.h"
#include "Gdiplusgraphics.h"
#include "Gdiplus.h"

using namespace   Gdiplus;
#define   RC_CLOSE   0
#define   RC_POPUPMENU   1
#define   RC_PICTURE_SHOW  3

#define   CAPTION_HEIGHT   20
#define   BUTTON_HEIGHT     24
#define   SPACE_CAPTION_BUTTON  3
#define   SPACE_BUTTON_PICTURE  3
#define   SPACE_CAPTION_PICTURE  30
#define   BMP_SCROLL_SPACE  1

/*   图片显示速度定义*/
#define   SPEED_LOW			40
#define	  SPEED_MEDIUM	    20
#define   SPEED_FAST		10
// CDisplayPictures 对话框


/*定时器常量定义*/
#define   TIMER_MENU_DOWN    1
#define   TIMER_MENU_UP    2
#define   TIMER_BUTTON_UP    3
#define   TIMER_BUTTON_DOWN  4
#define   TIMER_PICTURE_INIT  5
static  int nOldPictureIndex=-1;

IMPLEMENT_DYNAMIC(CDisplayPictures, CDialog)
CDisplayPictures::CDisplayPictures(CWnd* pParent /*=NULL*/)
: CDialog(CDisplayPictures::IDD, pParent)

{
	m_nPictureScrollSpace=0;   //初始化图片滚动;
	m_nTimer=0;  
	
	//m_bHand=FALSE;
	m_nPicScrollSpeed=SPEED_LOW;
	m_nUpDownTimer=0;
	m_nPicturesDownState=0;
	m_nPicturesUpState=0;
	
	m_nMenuButtonState=0;		
	m_nCloseButtonState=0;
	m_nIndex=0;
	m_nOffset=0;
	m_bLButtonDown=FALSE;
}

CDisplayPictures::~CDisplayPictures()
{
}

void CDisplayPictures::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CDisplayPictures, CDialog)
	ON_WM_PAINT()
	ON_WM_NCHITTEST()	
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
   ON_COMMAND(ID_PICTURES_UP, OnPicturesUp)
   ON_COMMAND(ID_PICTURES_DOWN,OnPicturesDown)
	ON_COMMAND(ID_PICTURES_STOP,OnPicturesStop)
	ON_WM_MOUSEMOVE()
	ON_WM_SETCURSOR()
	ON_WM_LBUTTONUP()
	ON_WM_SHOWWINDOW()
	ON_COMMAND(ID_SPEED_LOW, OnSpeedLow)
	ON_COMMAND(ID_SPEED_MEDIUM, OnSpeedMedium)
	ON_COMMAND(ID_SPEED_FAST, OnSpeedFast)
	ON_WM_LBUTTONDOWN()
	ON_WM_NCLBUTTONUP()
	ON_WM_NCMOUSEMOVE()
	ON_WM_DESTROY()
END_MESSAGE_MAP()


// CDisplayPictures 消息处理程序
void CDisplayPictures::DrawFrame(CDC *pDC)
{
	pDC->FillSolidRect(&m_rcClient,RGB(203,235,248));
	pDC->FillSolidRect(0,20,m_cx,m_cy,RGB(255,255,255));
	pDC->Draw3dRect(&m_rcClient,RGB(128,128,128),RGB(128,128,128));
	pDC->Draw3dRect(&m_rcClose,RGB(0,0,0),RGB(0,0,0));
	pDC->Draw3dRect(&m_rcPopMenu,RGB(0,0,0),RGB(0,0,0));

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

	//pDC->DrawText("Mini图片滚动",CRect(25,5,m_rcClient.right,20),DT_LEFT);	
	CResBitmap  bmpClose,bmpMenu,bmpCaption;

	bmpCaption.LoadBitmap(IDB_PICTURE_BROAD_CAPTION);
	::DrawBitmap(pDC,3,3,&bmpCaption,0);

	//bmpClose.LoadBitmap(IDB_PICTURE_BROAD_MIN);
	//bmpMenu.LoadBitmap(IDB_PICTURE_BROAD_MENU);
	
	switch(m_nMenuButtonState)   /*MenuButton*/
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

	switch(m_nCloseButtonState)   /*CloseButton*/
	{
	case 0:
		bmpClose.LoadBitmap(IDB_MIN_HOT);		
		break;
	case 1:
		bmpClose.LoadBitmap(IDB_MIN_HOT);
		break;
	case 2:
		bmpClose.LoadBitmap(IDB_MIN_HOT);
		break;
	}
	::DrawBitmap(pDC,m_rcClose.left,m_rcClose.top,&bmpClose,0);
	
	
	CResBitmap		bmpDownButton,bmpUpButton;	
	switch(m_nPicturesDownState)   /*upButton*/
	{
	case 0:
		bmpDownButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_DOWN_NORMAL);		
		break;
	case 1:
		bmpDownButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_DOWN_HOT);
		break;
	case 2:
		bmpDownButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_DOWN_CLICK);
		break;
	}
	
	::DrawBitmap(pDC,BMP_LEFT,m_rcClient.bottom-SPACE_BUTTON_PICTURE-BUTTON_HEIGHT-4,&bmpDownButton,0);

	switch(m_nPicturesUpState)   /*downButton*/
	{
	case 0:
		bmpUpButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_UP_NORMAL);
		break;
	case 1:
		bmpUpButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_UP_HOT);
		break;
	case 2:
		bmpUpButton.LoadBitmap(IDB_PICTURE_BROAD_BUTTON_UP_CLICK);
		break;
	}	
	::DrawBitmap(pDC,BMP_LEFT,SPACE_BUTTON_PICTURE+CAPTION_HEIGHT,&bmpUpButton,0);
}

void CDisplayPictures::DrawPicture(CDC *pDC)
{	
	CRect rc;
	GetClientRect(&rc);	
	CRgn   rgnDrawPics;
   BOOL bSuccess=rgnDrawPics.CreateRectRgn(BMP_LEFT,CAPTION_HEIGHT+SPACE_CAPTION_PICTURE,BMP_LEFT+BMP_WIDTH,rc.Height()-SPACE_CAPTION_PICTURE-3);
	ASSERT(bSuccess==TRUE);
	pDC->SelectObject(&rgnDrawPics);
	
	int nBitmaps=g_vecBitmap.size();

	if(nBitmaps>0)
	{	
		if(0==m_nIndex)  //没有图片显示时，从第一张开始显示;
		{
			m_nIndex=1;
			m_nOffset=0;
		}

		for(int i=0;i<5;i++)     //从显示的第一张开始，显示连续五张图片			
		{   
			int n_Index=m_nIndex+i-1;				
			DrawBitmap(pDC,BMP_LEFT,CAPTION_HEIGHT+SPACE_CAPTION_PICTURE+m_nOffset+(BMP_HEIGHT+BMP_SPACE)*i,g_vecBitmap[n_Index%nBitmaps].pBitmap,0);					
		}			
	}	

	DeleteObject(&rgnDrawPics);
}

void CDisplayPictures::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	CRect  rc;
	GetClientRect(&rc);

	CDC		dcMem;
	CBitmap bmpMem;
	dcMem.CreateCompatibleDC(&dc);
	bmpMem.CreateCompatibleBitmap(&dc,rc.Width(),rc.Height());
	
	int nSaveDC=dcMem.SaveDC();
	dcMem.SelectObject(&bmpMem);
	dcMem.FillSolidRect(&rc,RGB(203,235,248));

	DrawFrame(&dcMem);
	DrawPicture(&dcMem);	

	dc.BitBlt(0,0,rc.Width(),rc.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSaveDC);
}

void CDisplayPictures::InitDlgPosition()
{
	CRect rcWindow;
	CRect rcClient;
	
	GetWindowRect(&rcWindow);
	GetClientRect(&rcClient);
	int cx=GetSystemMetrics(SM_CXSCREEN);
	int cy=GetSystemMetrics(SM_CYSCREEN);
	int cyt=GetSystemMetrics(SM_CYFULLSCREEN);   //出去任务栏的窗口高度;

    CRect rcDlgShow;
	rcDlgShow.left=cx-rcClient.Size().cx-2;
	rcDlgShow.right=cx-2;		
	rcDlgShow.bottom=(cy+cyt)/2-146-20;   //146——MaiDlg的高度
	rcDlgShow.top=rcDlgShow.bottom-rcClient.Size().cy;
	MoveWindow(rcDlgShow);	
}

BOOL CDisplayPictures::OnInitDialog()
{
	CDialog::OnInitDialog();

	InitDlgPosition();

	SetWindowPos(&CWnd::wndTopMost,0,0,0,0,SWP_NOMOVE | SWP_NOSIZE);
	ModifyStyleEx(0,WS_EX_TOOLWINDOW|0x08000000L|WS_EX_PALETTEWINDOW);
	InitDlgInfo();

	EnableToolTips(TRUE);
	InitToolTip();
	
	SetTimer(TIMER_PICTURE_INIT,1000,NULL);

	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}

void CDisplayPictures::InitDlgInfo()
{
	GetClientRect(&m_rcClient);
	m_cx=m_rcClient.Size().cx;
	m_cy=m_rcClient.Size().cy;

	m_rcClose.left=m_cx-17;
	m_rcClose.top=3;
	m_rcClose.right=m_cx-3;
	m_rcClose.bottom=17;

	m_rcPopMenu.left=m_cx-37;
	m_rcPopMenu.top=3;
	m_rcPopMenu.right=m_cx-23;
	m_rcPopMenu.bottom=17;
	
	CRect  rcClient;
	GetClientRect(&rcClient);
	int cx=rcClient.Size().cx;
	int cy=rcClient.Size().cy;
	m_rcPicturesUp.left=BMP_LEFT;
	m_rcPicturesUp.top=CAPTION_HEIGHT+SPACE_CAPTION_BUTTON;
	m_rcPicturesUp.right=cx-BMP_LEFT;
	m_rcPicturesUp.bottom=CAPTION_HEIGHT+SPACE_CAPTION_BUTTON+BUTTON_HEIGHT;

	m_rcPicturesDown.left=BMP_LEFT;
	m_rcPicturesDown.top=cy-BUTTON_HEIGHT-SPACE_BUTTON_PICTURE;
	m_rcPicturesDown.right=cx-BMP_LEFT;
	m_rcPicturesDown.bottom=cy-SPACE_BUTTON_PICTURE;	
}
int CDisplayPictures::HitTest(CPoint point)
{
	if (PtInRect(&m_rcClose,point))
	{
		return RC_CLOSE;
	}
	if (PtInRect(&m_rcPopMenu,point))
	{
		return  RC_POPUPMENU;
	}

	CRect   rcPicturesShow;
	CRect   rcClient;
	GetClientRect(&rcClient);

	rcPicturesShow.left=BMP_LEFT;
	rcPicturesShow.right=BMP_LEFT+BMP_WIDTH;
	rcPicturesShow.top=CAPTION_HEIGHT+SPACE_CAPTION_PICTURE;
	rcPicturesShow.bottom=rcClient.Size().cy-SPACE_CAPTION_PICTURE;
	if(PtInRect(&rcPicturesShow,point))
	{
		return RC_PICTURE_SHOW;
	}	
	return  -1;
}

BOOL CDisplayPictures::NotifyMainDlg()
{
	return  FALSE;
}

void CDisplayPictures::OnLButtonUp(UINT nFlags, CPoint point)
{
	
	m_bLButtonDown=FALSE;
	if(3==m_nUpDownTimer||4==m_nUpDownTimer)
	{
		KillTimer(m_nUpDownTimer);
		m_nUpDownTimer=0;
	}	
	CPoint  ptPictures;
	CRect   rcPicturesShow;
	CRect   rcClient;
	GetClientRect(&rcClient);

	GetCursorPos(&ptPictures);
	ScreenToClient(&ptPictures);
	rcPicturesShow.left=BMP_LEFT;
	rcPicturesShow.right=BMP_LEFT+BMP_WIDTH;
	rcPicturesShow.top=CAPTION_HEIGHT+SPACE_CAPTION_PICTURE;
	rcPicturesShow.bottom=rcClient.Size().cy-SPACE_CAPTION_PICTURE;
	/*如果在显示的位置点击，则可以打开网上图片链接*/
	if(PtInRect(&rcPicturesShow,ptPictures))
	{
		int nIndex;
		nIndex=GetPictureIndex(ptPictures);
		
		if(nIndex)
		{
			OpenWebPicture(nIndex);	
		}
	}
	

	int HitSel=HitTest(point);	

	if(RC_POPUPMENU==HitSel)
	{
		//弹出菜单
		CPoint  pt;
		CMenu  menu;
		GetCursorPos(&pt);
		menu.LoadMenu(IDR_MENU_SCROLL);
		CMenu  *menupop=menu.GetSubMenu(0);
		menupop->TrackPopupMenu(TPM_LEFTALIGN |TPM_RIGHTBUTTON,pt.x,pt.y,this,0);
	}
	if (RC_CLOSE==HitSel)
	{
		//关闭窗口
		g_bPicsDlgVisible=FALSE;
		NotifyMainDlg();
		EndDialog(0);		
	}
	CDialog::OnLButtonUp(nFlags, point);
}

/*************************
打开图片的网上链接
***********************/
void CDisplayPictures::OpenWebPicture(int nIndex)
{
		int nBitmaps=g_vecBitmap.size();				
		DWORD   dwSID=g_vecBitmap[nIndex-1].dwPictureID;

		for(int j=0;j<nBitmaps;j++)				
		{					
			DWORD   dwDID=g_webPictureList[j]->dwPictureID;

			if(dwSID==dwDID)
			{
				::SendMessage(::g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MiniPictureBroadCast,(LPARAM)g_webPictureList[j]);					
				break;
			}				
		}		
}

/*************************
获得显示图片的序号
***********************/
int     CDisplayPictures::GetPictureIndex(CPoint pt)
{
		int nIndex;
		nIndex=(pt.y-(50+m_nOffset))/(BMP_HEIGHT+BMP_SPACE)+m_nIndex;
		int nBitmap=g_vecBitmap.size();

		if( nBitmap>0 && nIndex!=nBitmap )
			nIndex=nIndex%nBitmap;

		if( nIndex<=nBitmap )
			return nIndex;
		else
			return 0;      //点击的范围没有图片;  
}

LRESULT CDisplayPictures::OnNcHitTest(CPoint point)
{
	CRect  rcButtons;
	CRect rcClient;
	GetClientRect(&rcClient);
	CRect  rcCaption;

	rcButtons.left=m_cx-40;
	rcButtons.top=0;
	rcButtons.right=m_cx;
	rcButtons.bottom=20;  

	rcCaption.left=0;
	rcCaption.top=0;
	rcCaption.bottom=CAPTION_HEIGHT;
	rcCaption.right=rcClient.Size().cx-rcButtons.Size().cx;

	CRect  rcClientLeft,rcClientRight;
	rcClientLeft.left=0;  
	rcClientLeft.right=BMP_LEFT;
	rcClientLeft.top=CAPTION_HEIGHT;
	rcClientLeft.bottom=rcClient.Size().cy;

	rcClientRight.left=rcClientLeft.right+BMP_WIDTH;
	rcClientRight.right=rcClient.Size().cx;
	rcClientRight.top=CAPTION_HEIGHT;
	rcClientRight.bottom=rcClient.Size().cy;

	CRect  rcPicturesDown,rcPicturesUp;
	CRect  rcPicturesShow;
	//CRect  rcClient;
	//GetClientRect(&rcClient);
	int cx=rcClient.Size().cx;
	int cy=rcClient.Size().cy;
	rcPicturesUp.left=BMP_LEFT;
	rcPicturesUp.top=CAPTION_HEIGHT+SPACE_CAPTION_BUTTON;
	rcPicturesUp.right=cx-BMP_LEFT;
	rcPicturesUp.bottom=rcPicturesUp.top+BUTTON_HEIGHT;

	rcPicturesDown.left=BMP_LEFT;
	rcPicturesDown.top=cy-BUTTON_HEIGHT-SPACE_BUTTON_PICTURE;
	rcPicturesDown.right=cx-BMP_LEFT;
	rcPicturesDown.bottom=cy-SPACE_BUTTON_PICTURE;

	rcPicturesShow.left=BMP_LEFT;
	rcPicturesShow.top=CAPTION_HEIGHT+SPACE_CAPTION_PICTURE;
	rcPicturesShow.right=rcPicturesShow.left+BMP_WIDTH;
	rcPicturesShow.bottom=cy-SPACE_CAPTION_PICTURE;	
	
	ClientToScreen(&rcButtons);
	ClientToScreen(&rcCaption);
	ClientToScreen(&rcClientLeft);
	ClientToScreen(&rcClientRight);
	ClientToScreen(&rcPicturesDown);
	ClientToScreen(&rcPicturesUp);
	ClientToScreen(&rcPicturesShow);

	if(PtInRect(&rcButtons,point))
   	{
		return  HTCLIENT;
	}	
	else
		//if(PtInRect(&rcCaption,point)||PtInRect(&rcClientLeft,point)||PtInRect(&rcClientRight,point))
	   if(!PtInRect(rcPicturesDown,point)&&!PtInRect(rcPicturesUp,point)&&!PtInRect(rcPicturesShow,point))
		return HTCAPTION;
	

	return CDialog::OnNcHitTest(point);
}

void CDisplayPictures::OnTimer(UINT nIDEvent)
{	
	
	switch(nIDEvent) 
	{
		case TIMER_MENU_DOWN:
			{
				m_nPictureScrollSpace=BMP_SCROLL_SPACE;
				if(m_nOffset==0)   //如果显示的第一张图片完全显示,则显示上一张图片
				{				
					m_nOffset=-(BMP_HEIGHT+BMP_SPACE);  
					if(1==m_nIndex)     
						m_nIndex=g_vecBitmap.size();   //显示最后一张;
					else
					{
						if(m_nIndex>0)
							m_nIndex--;            //显示它之前的一张图片;
					}
				}
				else	
				{				
					m_nOffset++;    //m_nOffset+=BMP_SCROLL_SPACE;
				}
				Invalidate();
			}
			break;
		case TIMER_MENU_UP:
			{
				m_nPictureScrollSpace=-BMP_SCROLL_SPACE;
				if(m_nOffset==-(BMP_HEIGHT+BMP_SPACE))   //如果显示的第一张图片完全显示,则显示上一张图片
				{				
					m_nOffset=0;  
					if(m_nIndex==g_vecBitmap.size())   
						m_nIndex=1;   //如果是最后一张则显示第一张;
					else
					{
						m_nIndex++;            //显示它之前的一张图片;
					}
				}
				else	
				{				
					m_nOffset--;			//m_nOffset-=BMP_SCROLL_SPACE;
				}
				Invalidate();
			}
			break;
		case TIMER_BUTTON_UP:
			{
				int nBitmap=g_vecBitmap.size();
				if(nBitmap>0)           //向下滚动
				{		
					m_nUpDownTimer=nIDEvent;		
					m_nPictureScrollSpace=BMP_SCROLL_SPACE*25;

					m_nOffset+=m_nPictureScrollSpace;			//m_nOffset-=m_nPictureScrollSpace;
					if(m_nOffset>=0)   //如果显示范围不是当前图片,则显示当前上一张图片
					{				
						m_nOffset-=((BMP_HEIGHT+BMP_SPACE));  
						if(m_nIndex==1)   
							m_nIndex=g_vecBitmap.size();   //如果是最后一张则显示第一张;
						else
						{
							if(m_nIndex>0)
								m_nIndex--;
							//显示它之前的一张图片;
						}
					}		
					Invalidate();
				}	
			}
			break;
		case TIMER_BUTTON_DOWN:
			{
				int nBitmap=g_vecBitmap.size();
				if(nBitmap>0)  
				{				
				m_nUpDownTimer=nIDEvent;
				m_nPictureScrollSpace=BMP_SCROLL_SPACE*25;

				m_nOffset-=m_nPictureScrollSpace;			//m_nOffset-=m_nPictureScrollSpace;
				if(m_nOffset<=-(BMP_HEIGHT+BMP_SPACE))   //如果显示范围不是当前图片,则显示当前上一张图片
				{				
					m_nOffset+=((BMP_HEIGHT+BMP_SPACE));
					if(m_nIndex==nBitmap)   
						m_nIndex=1;   //如果是最后一张则显示第一张;
					else
					{						
						m_nIndex++;       //显示它之前的一张图片;
					}
				}
				Invalidate();
				}
			}
			break;
		case TIMER_PICTURE_INIT:
			{				
				if(g_vecBitmap.size()>0)
				{				
					Invalidate();
					//KillTimer(TIMER_PICTURE_INIT);
					//static int i=0;
					if(l_nInit==0)
					{
					m_nTimer=TIMER_MENU_DOWN;
					m_nPicScrollSpeed=SPEED_LOW;
					SetTimer(TIMER_MENU_DOWN,m_nPicScrollSpeed,NULL);	
					l_nInit++;
					}
				}
				else
				if(g_vecBitmap.size()>=4)
					KillTimer(TIMER_PICTURE_INIT);		
			}
			break;
	}


	CDialog::OnTimer(nIDEvent);
}

BOOL CDisplayPictures::OnEraseBkgnd(CDC* pDC)
{	
	return  FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}
void CDisplayPictures::OnPicturesUp()
{
	if(TIMER_MENU_DOWN==m_nTimer) 			
	KillTimer(TIMER_MENU_DOWN);	
	m_nTimer=TIMER_MENU_UP;
	SetTimer(TIMER_MENU_UP,m_nPicScrollSpeed,NULL);

}
void CDisplayPictures::OnPicturesDown()
{
	if(TIMER_MENU_UP==m_nTimer)	
	KillTimer(TIMER_MENU_UP);
	m_nTimer=TIMER_MENU_DOWN;
	
	SetTimer(TIMER_MENU_DOWN,m_nPicScrollSpeed,NULL);	
}
void CDisplayPictures::OnPicturesStop()
{
	if(TIMER_MENU_UP==m_nTimer)  
		KillTimer(TIMER_MENU_UP);
	if(TIMER_MENU_DOWN==m_nTimer)  
		KillTimer(TIMER_MENU_DOWN);
	m_nTimer=0;
	
}
void CDisplayPictures::OnMouseMove(UINT nFlags, CPoint point)
{	
	CRect  rcPicturesDown,rcPicturesUp;
	CRect  rcClient;
	GetClientRect(&rcClient);
	int cx=rcClient.Size().cx;
	int cy=rcClient.Size().cy;
	rcPicturesUp.left=BMP_LEFT;
	rcPicturesUp.top=CAPTION_HEIGHT+SPACE_CAPTION_BUTTON;
	rcPicturesUp.right=cx-BMP_LEFT;
	rcPicturesUp.bottom=rcPicturesUp.top+BUTTON_HEIGHT;

	rcPicturesDown.left=BMP_LEFT;
	rcPicturesDown.top=cy-BUTTON_HEIGHT-SPACE_BUTTON_PICTURE;
	rcPicturesDown.right=cx-BMP_LEFT;
	rcPicturesDown.bottom=cy-SPACE_BUTTON_PICTURE;	
	
	if(PtInRect(&rcPicturesUp,point))
		m_nPicturesUpState=1;        //进入图片向上按钮区域
	else
		m_nPicturesUpState=0;        //离开该区域	

	if(PtInRect(&rcPicturesDown,point))
		m_nPicturesDownState=1;     //进入图片向下按钮区域
	else
		m_nPicturesDownState=0;		//离开该区域
	
	if((TIMER_BUTTON_UP==m_nUpDownTimer)&&!PtInRect(&rcPicturesUp,point)&&!m_bLButtonDown)  
		KillTimer(TIMER_BUTTON_UP);
	if(!PtInRect(&rcPicturesDown,point)&&TIMER_BUTTON_DOWN==m_nUpDownTimer&&!m_bLButtonDown)
		KillTimer(TIMER_BUTTON_DOWN);
	
	/*  判断鼠标移动的区域  MenuButton   CloseButton*/
	{
			if(PtInRect(&m_rcPopMenu,point))
				m_nMenuButtonState=1;			
			else
				m_nMenuButtonState=0;
			if(PtInRect(&m_rcClose,point))
				m_nCloseButtonState=1;
			else
				m_nCloseButtonState=0;			
	}

	int HitSel=HitTest(point);
	if(RC_CLOSE==HitSel)
	{
		if(m_nCurBtn!=2)  
		{
			ShowToolTip("关闭");
			m_nCurBtn=2;
		}
	}
	else	
		if(m_nCurBtn==2)
		{
			ShowToolTip("");
		}
	if(RC_POPUPMENU==HitSel)
	{
		if(m_nCurBtn!=1)  
		{
			ShowToolTip("滚动菜单");
			m_nCurBtn=1;
		}
	}
	else	
		if(m_nCurBtn==1)
		{
			ShowToolTip("");
		}	

	if(RC_PICTURE_SHOW==HitSel)  
	{	
			int nPictureIndex	=	GetPictureIndex(point);
			if(nPictureIndex!=0)
			{
				if(m_nIndex>0&&nPictureIndex!=nOldPictureIndex)
				{				
					nOldPictureIndex=nPictureIndex;
					int nBitmaps  =g_webPictureList.size();
					for(int i=0;i<nBitmaps;i++)
					{
						if(g_vecBitmap[nPictureIndex-1].dwPictureID==g_webPictureList[i]->dwPictureID)						
						{
							nPictureIndex=i;
							break;
						}
					}
					CString  str=g_webPictureList[nPictureIndex]->szTitle;
					ShowToolTip(str);							
				}
			}
				
	}
	if(-1==HitSel)
	{
		m_nCurBtn=0;
		ShowToolTip("");

	}	
	if(m_nTimer==0)
	{
		m_nPictureScrollSpace=0;
		Invalidate();
	}

	CDialog::OnMouseMove(nFlags, point);
}

void CDisplayPictures::ShowToolTip( CString strTip)
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

void CDisplayPictures::InitToolTip()
{
	if (m_ToolTip.m_hWnd == NULL)
	{
		m_ToolTip.Create( this , TTS_ALWAYSTIP ) ;

		m_ToolTip.Activate( TRUE ) ;
	} 
}

BOOL CDisplayPictures::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	
	CRect   rcPicturesShow;
	CRect   rcClient;
	GetClientRect(&rcClient);

	rcPicturesShow.left=BMP_LEFT;
	rcPicturesShow.right=BMP_LEFT+BMP_WIDTH;
	rcPicturesShow.top=CAPTION_HEIGHT+SPACE_CAPTION_PICTURE;
	rcPicturesShow.bottom=rcClient.Size().cy-SPACE_CAPTION_PICTURE;

	CPoint  pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	if(this==pWnd&&rcPicturesShow.PtInRect(pt))
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
	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}

void CDisplayPictures::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CDialog::OnShowWindow(bShow, nStatus);
	g_bPicsDlgVisible=TRUE;
	
}

void CDisplayPictures::OnSpeedLow()
{	
	if(m_nTimer!=0)		
	if(m_nPicScrollSpeed!=SPEED_LOW)
		{
			m_nPicScrollSpeed=SPEED_LOW;
			KillTimer(m_nTimer);
			SetTimer(m_nTimer,m_nPicScrollSpeed,NULL);
		}		
}

void CDisplayPictures::OnSpeedMedium()
{		
	if(m_nTimer!=0)		
	if(m_nPicScrollSpeed!=SPEED_MEDIUM)
		{
			m_nPicScrollSpeed=SPEED_MEDIUM;
			KillTimer(m_nTimer);
			SetTimer(m_nTimer,m_nPicScrollSpeed,NULL);
		}		
}

void CDisplayPictures::OnSpeedFast()
{		
	if(m_nTimer!=0)	
	if(m_nPicScrollSpeed!=SPEED_FAST)
		{
			m_nPicScrollSpeed=SPEED_FAST;
			KillTimer(m_nTimer);
			SetTimer(m_nTimer,m_nPicScrollSpeed,NULL);
		}
}

LRESULT CDisplayPictures::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
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

BOOL CDisplayPictures::PreTranslateMessage(MSG* pMsg)
{
	if(pMsg->hwnd)

	return CDialog::PreTranslateMessage(pMsg);
}

void CDisplayPictures::OnLButtonDown(UINT nFlags, CPoint point)
{
	m_bLButtonDown=TRUE;
	CRect  rcPicturesDown,rcPicturesUp;
	CRect  rcClient;
	GetClientRect(&rcClient);
	int cx=rcClient.Size().cx;
	int cy=rcClient.Size().cy;
	rcPicturesUp.left=BMP_LEFT;
	rcPicturesUp.top=CAPTION_HEIGHT+SPACE_CAPTION_BUTTON;
	rcPicturesUp.right=cx-BMP_LEFT;
	rcPicturesUp.bottom=rcPicturesUp.top+BUTTON_HEIGHT;

	rcPicturesDown.left=BMP_LEFT;
	rcPicturesDown.top=cy-BUTTON_HEIGHT-SPACE_BUTTON_PICTURE;
	rcPicturesDown.right=cx-BMP_LEFT;
	rcPicturesDown.bottom=cy-SPACE_BUTTON_PICTURE;	
			
	if(PtInRect(rcPicturesDown,point))
	{
		m_nPicturesDownState=2; 					
		m_nUpDownTimer=TIMER_BUTTON_DOWN;
		SetTimer(TIMER_BUTTON_DOWN,SPEED_LOW,NULL);	

	}
	if(PtInRect(rcPicturesUp,point))
	{
		m_nPicturesUpState=2;		
		m_nUpDownTimer=TIMER_BUTTON_UP;
		SetTimer(TIMER_BUTTON_UP,SPEED_LOW,NULL);		
	}

	/*  判断鼠标点击的区域  MenuButton   CloseButton*/
	{
		if(PtInRect(&m_rcPopMenu,point))
		
			m_nMenuButtonState=2;		
			
		if(PtInRect(&m_rcClose,point))
			m_nCloseButtonState=2;
		Invalidate();
			
	}

	CDialog::OnLButtonDown(nFlags, point);
}

void CDisplayPictures::OnNcLButtonUp(UINT nHitTest, CPoint point)
{
	if(TIMER_BUTTON_UP==m_nUpDownTimer||TIMER_BUTTON_DOWN==m_nUpDownTimer)
	{
		KillTimer(m_nUpDownTimer);
		m_nUpDownTimer=0;
	}
	CDialog::OnNcLButtonUp(nHitTest, point);
}

void CDisplayPictures::OnNcMouseMove(UINT nHitTest, CPoint point)
{
	if(TIMER_BUTTON_UP==m_nUpDownTimer||TIMER_BUTTON_DOWN==m_nUpDownTimer)
	{
		KillTimer(m_nUpDownTimer);
		m_nPictureScrollSpace=0;
		m_nUpDownTimer=0;
	}
	m_nCurBtn=0;
	ShowToolTip("");
	m_nPicturesDownState=0;
	m_nPicturesUpState=0;
	if(m_nTimer==0)
	Invalidate();
	CDialog::OnNcMouseMove(nHitTest, point);
}

void CDisplayPictures::OnDestroy()
{
	
	CDialog::OnDestroy();
	nOldPictureIndex=-1;
	l_nInit=0;
}
