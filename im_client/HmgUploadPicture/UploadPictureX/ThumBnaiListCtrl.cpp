// ThumBnaiListCtrl.cpp : 实现文件
//
#include "..\stdafx.h"
#include "ThumBnaiListCtrl.h"
#include "pubheader.h"
#include "UpLoadPhotodlg.h"
#include <io.h>
#include ".\thumbnailistctrl.h"

#include <commctrl.h>

static DWORD g_dwFileID  = 1;
static int   g_fileIndex = 1;


IMPLEMENT_DYNAMIC(CThumBnaiListCtrl, CListCtrl)
CThumBnaiListCtrl::CThumBnaiListCtrl()
{
	m_bStopFlag   = FALSE;
	m_bRedraw     = FALSE;
	m_bRunning    = FALSE;

	m_hThreadLoad = NULL;
	m_bMouseOver  = false;
	m_nPreHoverItem = -1;
	m_nHoverItem    = -1;
	m_bAppendFlag   = false;

	//m_LeftBitmap.CreateCompatibleBitmap(&dc,16,16);
	m_LeftBitmap.LoadBitmap(IDB_BITMAP_LEFT);
	//m_LeftBitmap2.CreateCompatibleBitmap(&dc,16,16);
	m_LeftBitmap2.LoadBitmap(IDB_BITMAP_LEFT2);
	//m_RightBitmap.CreateCompatibleBitmap(&dc,16,16);
	m_RightBitmap.LoadBitmap(IDB_BITMAP_RIGHT);
	//m_RightBitmap2.CreateCompatibleBitmap(&dc,16,16);
	m_RightBitmap2.LoadBitmap(IDB_BITMAP_RIGHT2);
	//m_BigBitmap.CreateCompatibleBitmap(&dc,16,16);
	m_BigBitmap.LoadBitmap(IDB_BITMAP_BIG);
	//m_BigBitmap2.CreateCompatibleBitmap(&dc,16,16);
	m_BigBitmap2.LoadBitmap(IDB_BITMAP_BIG2);

	m_hSelIcon   = AfxGetApp()->LoadIcon(IDI_SEL_PIC);
	m_hUpdIcon   = AfxGetApp()->LoadIcon(IDI_UPD_PIC);

	m_bDraging = FALSE;
	m_nFlags = 0;

	m_nToopTipTextFlags = -1;

	m_pParentWnd = NULL;
}

CThumBnaiListCtrl::~CThumBnaiListCtrl()
{
	TerminateLoadThread();	
	DestroyIcon(m_hSelIcon);
	DestroyIcon(m_hUpdIcon);

	m_bDraging = FALSE;
}


BEGIN_MESSAGE_MAP(CThumBnaiListCtrl, CListCtrl)
	//	ON_WM_PAINT()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_TIMER()
	ON_WM_ERASEBKGND()
//	ON_WM_PAINT()
	ON_WM_LBUTTONUP()
	ON_NOTIFY_REFLECT(NM_CLICK, OnNMClick)

	ON_NOTIFY_REFLECT(LVN_GETINFOTIP, &CThumBnaiListCtrl::OnListCtrlGetInfoTip)
#ifdef USE_LISTCTRL_GETDISPINFO
	ON_NOTIFY_REFLECT(LVN_GETDISPINFO, &CThumBnaiListCtrl::OnListCtrlGetDispInfo)
#endif

#ifdef _USE_CUSTOME_DRAWITEM_
	ON_WM_MEASUREITEM_REFLECT()
#endif
	
	ON_MESSAGE(WM_LISTCTRL_TEXTTIP_LBUTTONDOWN, OnMsgOfListCtrlTextTipLBUTTONDOWN)
	ON_WM_DROPFILES()
END_MESSAGE_MAP()

// CThumBnaiListCtrl 消息处理程序

void CThumBnaiListCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	m_nToopTipTextFlags = -1;
	if(m_bDraging == TRUE)
	{
	//	TRACE("CThumBnaiListCtrl::OnMouseMove m_bDraging = %d.\n", (int) m_bDraging);
		CListCtrl::OnMouseMove(nFlags, point);		return ;
	}

	if(!g_bEnableContinueOperation)
	{
		CListCtrl::OnMouseMove(nFlags, point);		return ;
	}

	CRect rcClient;
	GetClientRect(&rcClient);
	if(rcClient.PtInRect(point))
	{
		int nIndex = -1;
		DWORD dwPos = ::GetMessagePos ();
		CPoint pt ((int) LOWORD (dwPos), (int) HIWORD (dwPos));

		static int nLastItem = -1;

//		CPoint pt(point);
		ScreenToClient(&pt);
		if((nIndex = HitTest(pt)) != -1) // && nLastItem != nIndex)
		{
		//	TRACE("CThumBnaiListCtrl::OnMouseMove Index = %d.\n", nIndex);

			CRect rcItem;
		
			m_nToopTipTextFlags = -1;
			GetItemRect(nIndex, &rcItem, LVIR_ICON);
			if(rcItem.PtInRect(pt))
			{
				m_nToopTipTextFlags = 0;
				ShowToolTipText("图片pic向左旋转90度", 1, rcItem);
			}
			
			if(m_nToopTipTextFlags == -1)
			{
				GetItemRect(nIndex, &rcItem, LVIR_LABEL);
				if(rcItem.PtInRect(pt))
				{
					m_nToopTipTextFlags = 1;
					ShowToolTipText("文件file向左旋转90度", 1, rcItem);
				}
			}
		
#if 0
			//////////////////////////////////////////////////////////////////////////
			
			RECT rc;	
			RECT rc2 = {0, 0, 16, 16};

			RECT rcleft = {rc.left+8,rc.bottom-19,rc.left + 8 + 16,rc.bottom-3};
			RECT rcright = {rc.right - 17 - 8,rc.bottom - 19,rc.right - 9,rc.bottom-3};
			RECT rcblowup = {rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 19,rc.right - (rc.right - rc.left)/2 + 8,rc.bottom-3};

			CPoint point1(pt);
				
			if(PtInRect(&rcleft, point1))
			{
				//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("向左旋转90度", 0, rcleft);
					
					m_nToopTipTextFlags = 2;
				}
			}
			else if(PtInRect(&rcright, point1))
			{
				//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("向右旋转90度", 0, rcright);
					
					m_nToopTipTextFlags = 3;
				}
			}
			else if(PtInRect(&rcblowup, point1))
			{
				//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("放大预览图片", 0, rcblowup);
					
					m_nToopTipTextFlags = 4;
				}
			}
			//////////////////////////////////////////////////////////////////////////
#endif
			nLastItem = nIndex;
		}
		else
		{
			// ...
		}
	} // end if(rcClient.PtInRect(point))

	if ( m_bMouseOver==false )
	{
		m_bMouseOver=true;
		SetTimer(0x01,100,NULL);
	}

	RECT clientrect;
	GetClientRect(&clientrect);
	if (GetCapture() != this)
	{
		if (PtInRect(&clientrect, point))
		{
		//	TRACE("SetCapture()\n");
		}
	}
	else
	{
		if (!PtInRect(&clientrect, point))
		{
			Invalidate();
	//		TRACE("ReleaseCapture()\n");

			// nofate
			CListCtrl::OnMouseMove(nFlags, point);
		
			return;
		}
	}

	//	RedrawItems(n, n);

	RECT rc;	
	RECT rc2 = {0, 0, 16, 16};

	int nHoverItem = -1;
	static int nLastItem = -1;

	for(int i = 0; i< GetItemCount(); ++i)
	{
		GetItemRect(i, &rc, LVIR_ICON);
			
		BYTE byItemFlag = 0;
		DWORD dwFileID = DWORD(GetItemData(i));
		FILEINFO *pFileINFO = GetFileData(dwFileID);
		if(pFileINFO != NULL)
		{
			byItemFlag = pFileINFO ->bFlag;
		}

		if(PtInRect(&rc,point))
		{
			m_bRedraw       = TRUE;
			nHoverItem      = i;

			RECT rcleft = {rc.left+8,rc.bottom-19,rc.left + 8 + 16,rc.bottom-3};
			RECT rcright = {rc.right - 17 - 8,rc.bottom - 19,rc.right - 9,rc.bottom-3};
			RECT rcblowup = {rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 19,rc.right - (rc.right - rc.left)/2 + 8,rc.bottom-3};

			if(PtInRect(&rcleft, point))
			{
				::SetCursor(LoadCursor(NULL, IDC_HAND)); 
				
			//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("向左旋转90度", 0, rcleft);
					nLastItem = nHoverItem;

					m_nToopTipTextFlags = 2;
				}
			}
			else if(PtInRect(&rcright, point))
			{
				::SetCursor(LoadCursor(NULL, IDC_HAND)); 

			//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("向右旋转90度", 0, rcright);
					nLastItem = nHoverItem;

					m_nToopTipTextFlags = 3;
				}
			}
			else if(PtInRect(&rcblowup, point))
			{
				::SetCursor(LoadCursor(NULL, IDC_HAND)); 
			
			//	if(nLastItem != nHoverItem)
				{
					ShowToolTipText("放大预览图片", 0, rcblowup);
					nLastItem = nHoverItem;

					m_nToopTipTextFlags = 4;
				}
			}
			else if(byItemFlag == 0)
			{
				::SetCursor(LoadCursor(NULL, IDC_HAND)); 
			}
			else
			{
				::SetCursor(LoadCursor(NULL, IDC_ARROW)); 
				m_nToopTipTextFlags = 0;
			}

			if(g_bEnableContinueOperation == TRUE)		DrawTool(i,point);
			break;
		}
	}

//	if(m_nFlags != 0)
	{
	//	CListCtrl::OnMouseMove(nFlags, point);		return ;
	}

	if ( nHoverItem!=m_nHoverItem )
	{
		RedrawItems(m_nHoverItem,m_nHoverItem);
		m_nHoverItem = nHoverItem;
	}

	if ( m_nHoverItem!=m_nPreHoverItem )
	{
		RedrawItems(m_nPreHoverItem,m_nPreHoverItem);
		m_nPreHoverItem = m_nHoverItem;
	}

	CListCtrl::OnMouseMove(nFlags, point);
}


void CThumBnaiListCtrl::DrawTool(int nItem, CPoint point)
{
	RECT rc;
	RECT rc2 = {0, 0, 16, 16};

	CDC *pDC = GetDC();

	CDC &dc = *pDC;

	for(int i = 0; i< GetItemCount(); ++i)
	{
		if(i==nItem)
		{
			GetItemRect(i, &rc, LVIR_ICON);

			CDC MemDC; 

			MemDC.CreateCompatibleDC(pDC);

			m_LeftBitmap, m_LeftBitmap2, m_RightBitmap, m_RightBitmap2, m_BigBitmap, m_BigBitmap2;

			CBitmap *pOldBit = MemDC.SelectObject(&m_LeftBitmap2);
			dc.TransparentBlt(rc.left+8,rc.bottom-19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));

			pOldBit = MemDC.SelectObject(&m_RightBitmap2);
			dc.TransparentBlt(rc.right - 17 - 8,rc.bottom - 19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));
			pOldBit = MemDC.SelectObject(&m_BigBitmap2);
			dc.TransparentBlt(rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));

			RECT rcleft = {rc.left+8,rc.bottom-19,rc.left + 8 + 16,rc.bottom-3};
			RECT rcright = {rc.right - 17 - 8,rc.bottom - 19,rc.right - 9,rc.bottom-3};
			RECT rcblowup = {rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 19,rc.right - (rc.right - rc.left)/2 + 8,rc.bottom-3};
			if(PtInRect(&rcleft, point))
			{
				pOldBit = MemDC.SelectObject(&m_LeftBitmap);
				dc.TransparentBlt(rc.left+8,rc.bottom-19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));

			}
			else if(PtInRect(&rcright, point))
			{
				pOldBit = MemDC.SelectObject(&m_RightBitmap);
				dc.TransparentBlt(rc.right - 17 - 8,rc.bottom - 19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));
			}
			else if(PtInRect(&rcblowup, point))
			{
				pOldBit = MemDC.SelectObject(&m_BigBitmap);
				dc.TransparentBlt(rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 19,16,16,&MemDC,0,0,16,16,RGB(0,255,0));
			}


			MemDC.SelectObject(pOldBit);
			//MemBitmap.DeleteObject();
			MemDC.DeleteDC();
			dc.SelectStockObject(ANSI_FIXED_FONT);

			break;
		}
	}

	dc.SelectStockObject(ANSI_FIXED_FONT);

	ReleaseDC(pDC);
}

void CThumBnaiListCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CListCtrl::OnLButtonDown(nFlags, point);
}

void CThumBnaiListCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// GetParent() ->PostMessage(WM_LBUTTONUP, 0, 0);
	CListCtrl::OnLButtonUp(nFlags, point);
}

void CThumBnaiListCtrl::Rotate(const int nIndex, const _EM_ROTATE em)
{
	try
	{
		const CString strTmpDir = _get_tmp_directroy();

		DWORD dwFileID = DWORD(GetItemData(nIndex));
		FILEINFO *rotateFile = GetFileData(dwFileID);
		if (rotateFile == NULL)		return	;

		CString strfix = rotateFile->strFileName.Right(3);
		strfix.MakeLower();

		HBITMAP hbmReturn = NULL; 
		Bitmap  *bmPhoto  = NULL;
		CBitmap Bmp1;

		CString path;
		path.Format( "%s\\%s", rotateFile->strFileDir, rotateFile->strFileName);

		//BSTR 
		Bitmap image(path.AllocSysString());// path.AllocSysString() )

		if(em == EM_ROTATE_LEFT)	image.RotateFlip(Rotate270FlipNone);
		else						image.RotateFlip(Rotate90FlipNone);

		CString strEncoder = strfix;
		if (strEncoder == "jpg")	strEncoder = "jpeg";
		strEncoder = "image/" + strEncoder;

		CLSID clsID;
		int iBet = GetEncoderClsid(strEncoder.AllocSysString(), &clsID);

		CString strNewFilePathName, strNewFileName;
		strNewFileName.Format("tmp_%s",  rotateFile ->strFileName);
		strNewFilePathName.Format("%s\\%s", strTmpDir, strNewFileName);

		image.Save(strNewFilePathName.AllocSysString(), &clsID);

		ULONG length = rotateFile->ulFileSize;

		CFile file;
		if(file.Open(strNewFilePathName, CFile::modeRead))		length = ULONG(file.GetLength());
		file.Close();

		int sourceWidth  = image.GetWidth();
		int sourceHeight = image.GetHeight();

		int destX = 0,		destY = 0; 

		float nPercent  = 0;
		float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);

		if(nPercentH < nPercentW)
		{
			nPercent = nPercentH;
			destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		}
		else
		{
			nPercent = nPercentW;
			destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		}

		int destWidth  = (int)(sourceWidth * nPercent);
		int destHeight = (int)(sourceHeight * nPercent);

		bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

		Graphics *grPhoto = Graphics::FromImage( bmPhoto );
		Color colorW(255, 255, 255, 255);
		grPhoto->Clear( colorW );
		grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );
		grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
		Pen newPen(Color(192,192,192,192));
		grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);

		bmPhoto->GetHBITMAP( colorW, &hbmReturn );

		Bmp1.Attach( hbmReturn );

		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = nIndex;
		lvItem.iSubItem = 0;
		GetItem(&lvItem);

		m_ImageListThumb.Replace(lvItem.iImage, &Bmp1, NULL );

		RedrawItems(nIndex, nIndex);

		delete grPhoto;
		delete bmPhoto;

		Bmp1.Detach();
		DeleteObject( hbmReturn );

		rotateFile ->bIsChanged = TRUE;
		rotateFile ->strGeneratedTmpFilePath = strNewFilePathName;

		rotateFile->strFileDir  = _get_tmp_directroy();
		rotateFile->strFileName = strNewFileName;
	//	rotateFile->bFlag       = 0;	//图片属性不改变

		rotateFile->ulFileSize  = length;

		DrawFileFlag(nIndex, rotateFile ->bFlag);
	}
	catch( ... )
	{
		MessageBox("CThumBnaiListCtrl::Rotate unknown exception caught!", APP_NAME, MB_ICONERROR);
	}
}

void CThumBnaiListCtrl::Initilization()
{
#ifdef _USE_CUSTOME_DRAWITEM_
//	ModifyStyle(LVS_TYPEMASK,  LVS_ICON);
//	ModifyStyle(0, LVS_SHOWSELALWAYS | LVS_OWNERDRAWFIXED);

	ModifyStyle(LVS_TYPEMASK, LVS_SHOWSELALWAYS | LVS_ICON | LVS_OWNERDRAWFIXED);

#endif // _USE_CUSTOME_DRAWITEM_

	m_ImageListThumb.Create( THUMBNAIL_WIDTH,
		THUMBNAIL_HEIGHT,
		ILC_COLOR32,
		0,
		1 );

	SetImageList( &m_ImageListThumb, LVSIL_NORMAL );

#if USE_INPLACE_TEXTTIP_CTRL
	
	m_wndToolTipCtrl2.Create(this);
#else
	if (m_wndToolTipCtrl.m_hWnd == NULL)
	{
		m_wndToolTipCtrl.Create( this , TTS_ALWAYSTIP ) ;
		m_wndToolTipCtrl.Activate( TRUE ) ;
	} 
#endif

	EnableToolTips(TRUE);

//	SetExtendedStyle(GetExtendedStyle() & ~LVS_EX_LABELTIP);
	SetExtendedStyle(GetExtendedStyle() | LVS_EX_INFOTIP | LVS_EX_BORDERSELECT | LVS_EX_LABELTIP);

	if(m_nFlags == 1)
	{
		ModifyStyleEx(0, WS_EX_ACCEPTFILES);
	}
}

void CThumBnaiListCtrl::ShowToolTipText(const CString & strToolTipText, const UINT nFlags, const CRect &rcTitle)
{
	if(nFlags == 1)	return ;

	if(strToolTipText.GetLength() < 1 || GetItemCount() < 1)	return ;

	CRect rc(rcTitle);

#if USE_INPLACE_TEXTTIP_CTRL
		
	
//	GetItemRect(0, &rc, LVIR_LABEL);
	
//	ClientToScreen(&rc);

//	rc.top += 20;
//	rc.bottom += 20;

	m_wndToolTipCtrl2.ShowTip(rc, strToolTipText, 20);
		
#else

	if (m_wndToolTipCtrl.m_hWnd == NULL)
	{
		m_wndToolTipCtrl.Create( this , TTS_ALWAYSTIP ) ;
		m_wndToolTipCtrl.Activate( TRUE ) ;
	} 

	if (m_wndToolTipCtrl.GetToolCount() != 0)
	{
		m_wndToolTipCtrl.DelTool(this,1);
	}

	CRect rect; 
	GetClientRect(rect);
	BOOL bRet =	m_wndToolTipCtrl.AddTool(this, strToolTipText, rect, 1);

	//m_ToolTip.UpdateTipText(strTip,this,1);

	m_wndToolTipCtrl.Activate(TRUE);

	POINT tmpPoint;

	GetCursorPos(&tmpPoint);
	ClientToScreen( &tmpPoint );

	MSG msg;
	msg.hwnd = m_hWnd;
	msg.message = WM_MOUSEMOVE;
	msg.wParam = nFlags;
	msg.lParam = tmpPoint.x + ((tmpPoint.y)<<8);
	msg.time = GetTickCount();

	msg.pt= tmpPoint;
	m_wndToolTipCtrl.RelayEvent( &msg ); 

//	if (m_wndToolTipCtrl.GetToolCount() == 0 && GetItemCount() > 0)
//	{
//		m_wndToolTipCtrl.AddTool(this, strToolTipText, rc, 12345);
//	} // if
//
//	m_wndToolTipCtrl.UpdateTipText(strToolTipText, this, 12345);
//	m_wndToolTipCtrl.Activate(TRUE);
//
//#if 0

//
//	m_wndToolTipCtrl.Activate( FALSE );
//	m_wndToolTipCtrl.Activate( TRUE ) ;
//	m_wndToolTipCtrl.AddTool( this , strToolTipText) ;
//	//TRACE( "ToolTip : %s \r\n" , m_ControlButton[i].m_szTip ) ;
//
//	MSG msg;
//	msg.hwnd = m_hWnd;
//	msg.message = WM_MOUSEMOVE;
//	msg.wParam = nFlags;
//	msg.lParam = tmpPoint.x + ((tmpPoint.y)<<8);
//	msg.time = GetTickCount();
//
//	msg.pt= tmpPoint;
//	m_wndToolTipCtrl.RelayEvent( &msg ); 
//#endif

#endif
}

/*================================================================ 
* 
* 函 数 名：MatchExtension()
* 
* 参 数：CString file [IN]：文件名
* 
* 功能描述: 验证文件是否是图片或压缩文件
* 
* 返 回 值：是TRUE，否FALSE 
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/ 
bool CThumBnaiListCtrl::MatchExtension(CString file)
{
	file.MakeLower();
	if (file.Right(4) == CString(".jpg") ||
		file.Right(5) == CString(".jpeg") ||
		file.Right(4) == CString(".gif") ||
		file.Right(4) == CString(".png"))
	{
		return true;
	}

	return false;
}

/*================================================================ 
* 
* 函 数 名：IsImageGDIPLUSValid() 
* 
* 参 数：CString file [IN] 
* 
* 功能描述: 验证文件是否可生成预览图
* 
* 返 回 值：是TRUE，否FALSE 
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/ 
bool CThumBnaiListCtrl::IsImageGDIPLUSValid(CString filePath)
{
	CComBSTR bst(filePath);
	Bitmap image(bst.m_str);

	if( image.GetFlags() == ImageFlagsNone )
		return FALSE;
	else
		return TRUE;
}

/*================================================================ 
* 
* 函 数 名：AddFileToUpLoadList() 
* 
* 参 数：CString filename [IN]:文件名;FileInfoList *fileInfoList [IN]:存放的列表 
* 
* 功能描述: 将要生成缩略图的文件加入列表
* 
* 返 回 值：成功TRUE，失败FALSE 
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/
//bool CThumBnaiListCtrl::AddFileToUpLoadList(FileInfoList *fileInfoList,FILEINFO *fileInfo)
//{
//	if (fileInfo->strFileName == "")
//		return false;
//	FILEINFO temp;
//	temp.dwFileID = fileInfo->dwFileID;
//	temp.strFileName = fileInfo->strFileName;
//	temp.ulFileSize = fileInfo->ulFileSize;
//	temp.strFileDir = fileInfo->strFileDir;
//	temp.bFlag = FALSE;//fileInfo->bFlag;
//	fileInfoList->push_back(temp);
//	return true;	
//}

/*================================================================ 
* 
* 函 数 名：LoadThumbnailImages() 
* 
* 功能描述: 启动緢制缩略图线程
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/ 
void CThumBnaiListCtrl::StartLoadThread(void)
{
	if ( this->m_bAppendFlag )
	{
		if (this->m_AddFileList.empty() )			return ;
	}
	else
	{
		if (this->m_FileList.empty() )				return ;
	}

#if 0
	RunLoadThumbnailThread_run();
#else
	if ( this->m_bRunning )		this->TerminateLoadThread();

	m_bRunning = TRUE;

	DWORD dwThreadId=0;
	m_hThreadLoad = CreateThread( NULL,
		0,
		(LPTHREAD_START_ROUTINE)RunLoadThumbnailThread,
		this,
		0,
		&dwThreadId );

	if (m_hThreadLoad == NULL)		m_bRunning = FALSE; 
#endif
}

/*================================================================ 
* 
* 函 数 名：RunLoadThumbnailThread()
* 
* 参 数：CThumBnaiListCtrl *pThum [IN]
* 
* 功能描述: 緢制图片缩略图线程
* 
* 返 回 值：线程结束代码
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/ 
UINT  CThumBnaiListCtrl::RunLoadThumbnailThread( LPVOID lpParam )
{
	CThumBnaiListCtrl *pThum = (CThumBnaiListCtrl *)lpParam;

	CListCtrl &ListCtrl  = *pThum;
	
	pThum->m_bRunning = true;

	try
	{
		CoInitialize(NULL);

		try
		{
		//	if(pThum->m_pParentWnd != NULL && ::IsWindow(pThum->m_pParentWnd ->GetSafeHwnd()))
			{	
			//	((CUpLoadPhotoDlg *)pThum->m_pParentWnd) ->m_bEnableSelectDir = FALSE;
			}
		}
		catch( ... )
		{
		}

		
		if(pThum != NULL)	
		{
			//g_bEnableContinueOperation = FALSE;
			pThum ->RunLoadThumbnailThread_run();
			//g_bEnableContinueOperation = TRUE;
			
			pThum->m_bRunning=false;
		}

		CoUninitialize();
	}
	catch ( ... )
	{
		AfxMessageBox("RunLoadThumbnailThread unknown exception caught!", MB_ICONERROR);
	}

	try
	{
		if(pThum->m_pParentWnd != NULL && ::IsWindow(pThum->m_pParentWnd ->GetSafeHwnd()))
		{	
			pThum ->m_pParentWnd ->SendMessage(WM_NOTIFY_LOADFINISHED, 0, (LPARAM)&ListCtrl);

			if(pThum ->m_nFlags == 0 && pThum ->m_bStopFlag == FALSE)
			{
				pThum->m_pParentWnd ->SendMessage(WM_THREAD_NOTIFY, THREAD_NOTIFY_MSG_LOAD_FOLDER_FILES_FINISHED, 0);
			}
		}
	}
	catch( ... )
	{
	}


	return 0;
}

void CThumBnaiListCtrl::RunLoadThumbnailThread_run()
{
	FileInfoList *pFileInfoList;

	if (m_bAppendFlag)
		pFileInfoList = &m_AddFileList;
	else
		pFileInfoList = &m_FileList;

	CListCtrl &ListCtrl  = *this;
	CImageList *pImgList = &(m_ImageListThumb);

	int iImageCount = pImgList->GetImageCount();
	int nIndex = iImageCount;
	pImgList->SetImageCount(UINT(pFileInfoList->size()) + iImageCount);

	int i = 0;
	ListCtrl.SetRedraw(FALSE);
	FileInfoList::iterator iter;

	CString strItemText;
	for(iter =  pFileInfoList->begin();	iter != pFileInfoList->end() && m_bStopFlag == FALSE;
		iter++, i++ , nIndex++)
	{
		FILEINFO temStru = *iter;
		
		if(m_nFlags == 0)	strItemText = temStru.strFileName;
		else	strItemText = temStru.strDescription;

		CString path;
		path.Format( "%s\\%s", temStru.strFileDir, temStru.strFileName);

		// 当文件不是图片时就显示该文件类型的图标
		if(!IsImageGDIPLUSValid(path))
		{
			continue;
		} // end if(!pThum->IsImageGDIPLUSValid(path))


#ifndef USE_LISTCTRL_GETDISPINFO 
		int nItem = ListCtrl.InsertItem(i, strItemText, nIndex);
		ListCtrl.SetItemData(nItem, temStru.dwFileID);

	//	EnsureVisible(nItem, FALSE);
	//	SetItemState(nItem,  LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
#else	
				LV_ITEM lvi;
		lvi.mask = LVIF_TEXT | LVIF_IMAGE | LVIF_PARAM; 
		lvi.iItem = i; 
		lvi.iSubItem = 0; 
		lvi.iImage = 0;
		lvi.pszText = LPSTR_TEXTCALLBACK; 
		lvi.lParam = (LPARAM) temStru.dwFileID;

		int nItem =	ListCtrl.InsertItem(&lvi);
#endif // USE_LISTCTRL_GETDISPINFO
	}

	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();

	nIndex=iImageCount;
	i = 0;

	for(iter =  pFileInfoList->begin();	iter != pFileInfoList->end() && m_bStopFlag == FALSE;
		iter++, i++ , nIndex++)
	{
	//	Sleep(10);

#ifndef USE_LISTCTRL_GETDISPINFO
		HBITMAP hbmReturn = NULL; 
		Bitmap  *bmPhoto  = NULL;
		CBitmap Bmp1;

		FILEINFO temStru = *iter;
		FILEINFO FileINFO = *iter;

		CString path;
		path.Format( "%s\\%s", temStru.strFileDir, temStru.strFileName);

		// 当文件不是图片时就显示该文件类型的图标
		if(!IsImageGDIPLUSValid(path))
		{
			continue;
		} // end if(!pThum->IsImageGDIPLUSValid(path))
 
		CComBSTR bst(path);		Bitmap image(bst);

		int sourceWidth  = image.GetWidth();
		int sourceHeight = image.GetHeight();

		(*iter).nImageWidth = sourceWidth;	(*iter).nImageHeight = sourceHeight;
		(*iter).nSoruceImageWidth = sourceWidth;	(*iter).nSoruceImageHeight = sourceHeight;

		int destX = 0,			destY = 0; 

		float nPercent  = 0;
		float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
	
		if(nPercentH < nPercentW)
		{
			nPercent = nPercentH;
			destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		}
		else
		{
			nPercent = nPercentW;
			destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		}

		int destWidth  = (int)(sourceWidth * nPercent);
		int destHeight = (int)(sourceHeight * nPercent);

		bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

		Graphics *grPhoto = Graphics::FromImage( bmPhoto );
		Color colorW(255, 255, 255, 255);
		grPhoto->Clear( colorW );
		grPhoto->SetInterpolationMode(InterpolationModeHighQualityBicubic);

#if 0
		SolidBrush br(Color(0, 0, 255));
		grPhoto ->FillRectangle(&br, 0, 0, THUMBNAIL_WIDTH - 1, THUMBNAIL_HEIGHT - 1);

		grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
#else
		
		grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );

		Pen newPen(Color(192,192,192,192));
		grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);
#endif

	//	Brush _brush;
	//	grPhoto ->FillRectangle(&_brush, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);

		if(temStru.bFlag != 0)
		{
			Bitmap *bmp;
			if ( temStru.bFlag==1 )
				bmp = Bitmap::FromHICON(m_hSelIcon);
			else
				bmp = Bitmap::FromHICON(m_hUpdIcon);

			grPhoto->DrawImage(bmp, Rect(0, 0, 14, 14));
			delete bmp;
		}
		bmPhoto->GetHBITMAP( colorW, &hbmReturn );

		Bmp1.Attach( hbmReturn );
		pImgList->Replace( nIndex, &Bmp1, NULL );
		ListCtrl.RedrawItems(i, i);

		delete grPhoto;
		delete bmPhoto;

		Bmp1.Detach();
		DeleteObject( hbmReturn );

#if 0
		/////////////////////////////////////////////////////////////////////////////
		// 如果图片的最长边已超出设置的，则对图片进行缩放.
		//const int nLimittedMaxSideSize = (*iter).nLimittedMaxSize;
		//const int nMaxSideSize =  max(sourceWidth, sourceHeight);
		//if(m_nFlags == 1 && nMaxSideSize > nLimittedMaxSideSize)
		//{
		//	const float fltScale =  (float)nLimittedMaxSideSize  / (float)nMaxSideSize;

		//	const int nNewWidth  = (*iter).nImageWidth * fltScale;
		//	const int nNewHeight = (*iter).nImageHeight * fltScale;

		//	CDC *pDC =GetDC();

		//	CDC dcMem;
		//	CBitmap bmpMem;

		//	dcMem.CreateCompatibleDC(pDC);
		//	bmpMem.CreateCompatibleBitmap(pDC, nNewWidth, nNewHeight);

		//	CBitmap *pOldBitmap = dcMem.SelectObject(&bmpMem);

		//	Graphics g(dcMem.m_hDC);
		//	Image _Image(path.AllocSysString());

		//	Bitmap * bitmap = Bitmap::FromFile(path.AllocSysString());
		//	g.DrawImage(bitmap, 0, 0, nNewWidth, nNewHeight);
		//	delete bitmap;

		//	CImage image;
		//	image.Attach((HBITMAP)bmpMem.m_hObject);
		//	
		//	BITMAP bm;
		//	bmpMem.GetBitmap(&bm);

		//	const	int _nNewWidth  = nNewWidth;
		//	const	int _nNewHeight = nNewHeight;

		//	CString strFileName;
		//	CString strNewFilePathName;

		//	strFileName.Format("tmp_New_%s", (*iter).strFileName);
		//	strNewFilePathName.Format("%s\\%s\0", _get_tmp_directroy(), strFileName);

		//	image.Save(strNewFilePathName);
		//	image.Detach();

		//	dcMem.SelectObject(pOldBitmap);

		//	if(m_pParentWnd != NULL && m_pParentWnd ->GetSafeHwnd() && ::IsWindow(m_pParentWnd ->GetSafeHwnd()))
		//	{
		//		((CUpLoadPhotoDlg *)m_pParentWnd) ->ReplaceSelFile(path, strNewFilePathName);
		//	}

		//	(*iter).bIsChanged = TRUE;
		//	(*iter).strGeneratedTmpFilePath = strNewFilePathName;

		//	 不改变原来的文件路径
		//	(*iter).strFileName = strFileName;
		//	(*iter).strFileDir = _get_tmp_directroy();

		//	(*iter).nImageHeight = _nNewHeight;
		//	(*iter).nImageWidth	 = _nNewWidth;

		//	(*iter).ulFileSize = GetFileSize(strNewFilePathName);
		//} // end if
#endif
#endif // USE_LISTCTRL_GETDISPINFO
		if (m_bAppendFlag)		m_FileList.push_back( (*iter) );
	} // end for

	if(m_bStopFlag == TRUE)	return ;

	try
	{
		if(m_pParentWnd != NULL && ::IsWindow(m_pParentWnd ->GetSafeHwnd()))
		{	
			if(((CUpLoadPhotoDlg *)m_pParentWnd) ->m_bLoadFolder == FALSE)
			{
				SelectItem(*pFileInfoList);
			}
		}
	}
	catch( ...)
	{
	}

//	ListCtrl.SetRedraw(TRUE);
	ListCtrl.Invalidate();

	g_bEnableContinueOperation = TRUE;
}

void CThumBnaiListCtrl::OnListCtrlGetDispInfo(NMHDR* pNMHDR, LRESULT* pResult)
{
#ifdef USE_LISTCTRL_GETDISPINFO
	//LV_DISPINFO* pDispInfo = (LV_DISPINFO*) pNMHDR;
	//
	//const int nItem = pDispInfo ->item.iItem;
	//if(pDispInfo ->item.iSubItem != 0)	return ;

	//DWORD dwFileID = (DWORD) pDispInfo ->item.lParam;
	//FILEINFO *pFile = GetFileData(dwFileID);
	//if(pFile == NULL)	return ;

	//FILEINFO temStru = *pFile;
	//FILEINFO FileINFO = *pFile;

	//if(pDispInfo ->item.mask & LVIF_TEXT)
	//{
	//	::lstrcpy (pDispInfo->item.pszText, temStru.strFileName);
	//}
	//
	//if(pDispInfo ->item.mask & LVIF_IMAGE)
	//{
	//	HBITMAP hbmReturn = NULL; 
	//	Bitmap  *bmPhoto  = NULL;
	//	CBitmap Bmp1;

	//	CImageList *pImgList = &(m_ImageListThumb);

	//	CString path;
	//	path.Format( "%s\\%s", temStru.strFileDir, temStru.strFileName);

	//	// 当文件不是图片时就显示该文件类型的图标
	//	if(!IsImageGDIPLUSValid(path))
	//	{
	//		return ;
	//	} // end if(!pThum->IsImageGDIPLUSValid(path))

	//	CComBSTR bst(path);		Bitmap image(bst);

	//	int sourceWidth  = image.GetWidth();
	//	int sourceHeight = image.GetHeight();

	//	pFile-> nImageWidth = sourceWidth;	pFile-> nImageHeight = sourceHeight;

	//	int destX = 0,			destY = 0; 

	//	float nPercent  = 0;
	//	float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
	//	float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);

	//	if(nPercentH < nPercentW)
	//	{
	//		nPercent = nPercentH;
	//		destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
	//	}
	//	else
	//	{
	//		nPercent = nPercentW;
	//		destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
	//	}

	//	int destWidth  = (int)(sourceWidth * nPercent);
	//	int destHeight = (int)(sourceHeight * nPercent);

	//	bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
	//	bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

	//	Graphics *grPhoto = Graphics::FromImage( bmPhoto );
	//	Color colorW(255, 255, 255, 255);
	//	grPhoto->Clear( colorW );
	//	grPhoto->SetInterpolationMode(InterpolationModeHighQualityBicubic);

	//	grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
	//	Pen newPen(Color(192,192,192,192));
	//	grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);

	//	if(temStru.bFlag != 0)
	//	{
	//		Bitmap *bmp;
	//		if ( temStru.bFlag==1 )
	//			bmp = Bitmap::FromHICON(m_hSelIcon);
	//		else
	//			bmp = Bitmap::FromHICON(m_hUpdIcon);

	//		grPhoto->DrawImage(bmp, Rect(0, 0, 14, 14));
	//		delete bmp;
	//	}
	//	bmPhoto->GetHBITMAP( colorW, &hbmReturn );

	//	Bmp1.Attach( hbmReturn );
	//	BOOL bRet =	pImgList->Replace( nItem, &Bmp1, NULL );
	//	RedrawItems(nItem, nItem);

	//	delete grPhoto;
	//	delete bmPhoto;

	//	Bmp1.Detach();
	//	DeleteObject( hbmReturn );

	//	/////////////////////////////////////////////////////////////////////////////
	//	// 如果图片的最长边已超出设置的，则对图片进行缩放.
	//	const int nLimittedMaxSideSize = pFile-> nLimittedMaxSize;
	//	const int nMaxSideSize =  max(sourceWidth, sourceHeight);
	//	if(m_nFlags == 1 && nMaxSideSize > nLimittedMaxSideSize)
	//	{
	//		const float fltScale =  (float)nLimittedMaxSideSize  / (float)nMaxSideSize;

	//		const int nNewWidth  = pFile->nImageWidth * fltScale;
	//		const int nNewHeight = pFile->nImageHeight * fltScale;

	//		CDC *pDC =GetDC();

	//		CDC dcMem;
	//		CBitmap bmpMem;

	//		dcMem.CreateCompatibleDC(pDC);
	//		bmpMem.CreateCompatibleBitmap(pDC, nNewWidth, nNewHeight);

	//		CBitmap *pOldBitmap = dcMem.SelectObject(&bmpMem);

	//		Graphics g(dcMem.m_hDC);
	//		Image _Image(path.AllocSysString());

	//		Bitmap * bitmap = Bitmap::FromFile(path.AllocSysString());
	//		g.DrawImage(bitmap, 0, 0, nNewWidth, nNewHeight);
	//		delete bitmap;

	//		CImage image;
	//		image.Attach((HBITMAP)bmpMem.m_hObject);

	//		CString strFileName;
	//		CString strNewFilePathName;

	//		strFileName.Format("tmp_New_%s", pFile->strFileName);
	//		strNewFilePathName.Format("%s\\%s\0", _get_tmp_directroy(), strFileName);

	//		image.Save(strNewFilePathName);
	//		image.Detach();

	//		dcMem.SelectObject(pOldBitmap);

	//		pFile->bIsChanged = TRUE;
	//		//	(*iter).strGeneratedTmpFilePath
	//		pFile->strFileName = strFileName;
	//		pFile->strFileDir = _get_tmp_directroy();

	//		pDispInfo ->item.iImage = nItem;
	//	} // end if
	//} // end if(pDispInfo ->item.mask & LVIF_IMAGE)

#endif	// USE_LISTCTRL_GETDISPINFO
}

void CThumBnaiListCtrl::AddFile(FILEINFO fileInfo)
{
	while ( m_bRunning )
	{
		MSG msg;
		if ( ::PeekMessage(&msg,NULL,0, 0, PM_REMOVE) )
		{
			::DispatchMessage(&msg);
		}
	}

	if ( m_bRunning==false )
	{
		FileInfoList l;
		l.push_back(fileInfo);
		AddFileList(&l, fileInfo.nLimittedMaxSize);
	}
}
//
void CThumBnaiListCtrl::AddFileList(FileInfoList * pFileList, const int nMaxSideSize)
{
	this->TerminateLoadThread();
	this->m_bAppendFlag = true;

	FileInfoList::iterator it;

	m_AddFileList.clear();

	for ( it=pFileList->begin(); it!=pFileList->end(); it++ )
	{
		if(m_nFlags == 1)	(*it).nLimittedMaxSize = nMaxSideSize;

		m_AddFileList.push_back( (*it) );
	}

	this->StartLoadThread();
}

/*================================================================ 
* 
* 函 数 名：AddForderFileToVector() 
* 
* 参 数：无
* 
* 功能描述: 将目录内的图片和压缩文件加入到Vector中,如果是压缩文件直接显示
* 
* 返 回 值：成功TRUE，失败FALSE 
* 
* 作 者：LWL 2005/12/7 
* 
================================================================*/ 
bool CThumBnaiListCtrl::LoadFolder(const CString & strFolder)
{
	if (_access((LPCSTR) strFolder,0) == -1 )	
	{
		g_bEnableContinueOperation = TRUE; 
		return false;
	}
	
#if 1
	this->TerminateLoadThread();
#endif
	m_bAppendFlag = false;

	Clear();

	CString pathWildcard = strFolder + "\\*.*";

	CFileFind finder;
	BOOL bWorking = finder.FindFile(pathWildcard);
	srand(NULL);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDots() || finder.IsDirectory() )
		{
			continue;
		}
		else
		{
			if ( MatchExtension(finder.GetFileName()))
			{
				CString strFileName = finder.GetFileName();
				strFileName.MakeLower();

				FILEINFO fileInfo;

				fileInfo.dwFileID   = g_dwFileID++;
				fileInfo.strFileDir = strFolder;
				fileInfo.ulFileSize = finder.GetLength();  // GetFileSize(strFolder + _T("\\") + filePath);
				fileInfo.strFileName = strFileName;

				fileInfo.strSourceFileDir = strFolder;
				fileInfo.strSourceFileName = strFileName;

				fileInfo.bFlag = 0;
				fileInfo.strDescription = finder.GetFileTitle();

				m_FileList.push_back( fileInfo );
			}
		}
	} // end while (bWorking)

	finder.Close();
	StartLoadThread();

	return true;
}

void CThumBnaiListCtrl::TerminateLoadThread(void)
{
	try
	{
		if(!m_bRunning)		return	;

		if(m_bRunning)
		{
			MSG msg;
			m_bStopFlag = TRUE;
			while(m_bRunning)
			{
				if(::PeekMessage(&msg,NULL,0, 0, PM_REMOVE))
				{
					::DispatchMessage(&msg);
				}
			} // end while

#if 0
			DWORD dwRet = WaitForSingleObject(m_hThreadLoad, 3000);
			if(dwRet != WAIT_OBJECT_0)
			{
				TerminateThread(m_hThreadLoad, -1);
			}
#endif

			CloseHandle(m_hThreadLoad);

			Sleep(3000);

			m_hThreadLoad = NULL;
			m_bStopFlag   = FALSE;

			g_bEnableContinueOperation = TRUE;
			if(m_pParentWnd != NULL && m_pParentWnd ->GetSafeHwnd() && ::IsWindow(m_pParentWnd ->GetSafeHwnd()))
			{
//				((CUpLoadPhotoDlg *)m_pParentWnd) ->m_bEnableSelectDir = TRUE;
			}

		} // end if(m_bRunning)
	}
	catch ( ... )
	{
	}

}



FILEINFO * CThumBnaiListCtrl::GetFileData(DWORD dwFileID)
{
	FileInfoList::iterator iter;
	iter = find_if(m_FileList.begin(), m_FileList.end(), bind2nd(IDCompare(), dwFileID));

	if (iter != m_FileList.end())
	{
		return &(*iter);
	}

	return NULL;
}

void CThumBnaiListCtrl::DrawFileFlag(const int nItem, BYTE bFlag)
{
	if(m_bDraging == TRUE)	return ;

	Color colorW(255, 255, 255, 255);

	if ( nItem < 0 || nItem >= GetItemCount() )
	{
		return ;
	}

	LVITEM lvItem;
	lvItem.mask = LVIF_IMAGE;
	lvItem.iItem = nItem;
	lvItem.iSubItem = 0;
	GetItem(&lvItem);

	IMAGEINFO imageInfo;
	m_ImageListThumb.GetImageInfo(lvItem.iImage, &imageInfo );
	Bitmap *bm = NULL;
	bm = Bitmap::FromHBITMAP(imageInfo.hbmImage, NULL);
	Rect rect(imageInfo.rcImage.left, imageInfo.rcImage.top,imageInfo.rcImage.right - imageInfo.rcImage.left, imageInfo.rcImage.bottom - imageInfo.rcImage.top);
	Bitmap *rectBmp = bm->Clone(rect, PixelFormat24bppRGB);

	Graphics *grPhoto = Graphics::FromImage( rectBmp );

	if ( bFlag != 0 )
	{
		Bitmap *bmp ;

		if ( bFlag==1 )
			bmp = Bitmap::FromHICON(m_hSelIcon);
		else
			bmp = Bitmap::FromHICON(m_hUpdIcon);

		grPhoto->DrawImage(bmp, Rect(0, 0, 14, 14));
		delete bmp;
	}

	CBitmap cB;
	HBITMAP hBm;
	rectBmp->GetHBITMAP(colorW,&hBm);
	cB.Attach(hBm);
	m_ImageListThumb.Replace( lvItem.iImage, &cB, NULL);

	RedrawItems(nItem, nItem);
	cB.Detach();
	delete bm;

}

int CThumBnaiListCtrl::FindFileIndex(DWORD dwFileId)
{
	for (int nIndex = 0; nIndex < GetItemCount(); ++nIndex)
	{
		if ( GetItemData(nIndex) == dwFileId )
		{
			return nIndex;
		}
	}

	return -1;
}

int CThumBnaiListCtrl::SetFileFlagEx(const DWORD dwFileID, const BYTE bFlag)
{
	// must be called by self

	for(int i = 0; i < GetItemCount(); ++ i)
	{
		DWORD dwItemData = (DWORD) GetItemData(i);
		if(dwItemData == dwFileID)
		{
			return	SetFileFlag(i, bFlag);
		}
	} // end for

	return -1;
}

int CThumBnaiListCtrl::SetAllFileFlag(const BYTE bFlag)
{
	for(int i = 0; i < GetItemCount(); ++ i)
	{
		SetFileFlag(i, bFlag);
	}
	return -1;
}

int CThumBnaiListCtrl::SetFileFlagEx(FILEINFO *_pFileInfo, const BYTE bFlag)
{
	if (!_pFileInfo)
		return -1;
	CString strFilePathName;
	CString strCurFilePathName = _pFileInfo ->strSourceFileDir + "\\" + _pFileInfo ->strSourceFileName;

	for(int i = 0; i < GetItemCount(); ++ i)
	{
		DWORD dwItemData = (DWORD) GetItemData(i);
		
		FILEINFO *pFileINFO = GetFileData(dwItemData);

		if(pFileINFO != NULL)
		{
			strFilePathName = pFileINFO ->strSourceFileDir + "\\" + pFileINFO ->strSourceFileName;
			if(strCurFilePathName == strFilePathName)
			{
				return SetFileFlag(i, bFlag);
			}
		}
	} // end for

	return -1;
}

int CThumBnaiListCtrl::SetFileFlag(const int nItemIndex, const BYTE bFlag)
{
	DWORD dwFileID = DWORD(GetItemData(nItemIndex));

	FILEINFO *fileInfo = GetFileData(dwFileID);
	if(fileInfo == NULL)	return -1;

	fileInfo->bFlag =  bFlag;

	if(bFlag == 0)	// 去重新加载图片文件信息
	{
		HBITMAP hbmReturn  = NULL; 
		Bitmap  *bmPhoto   = NULL;

		CBitmap Bmp1;
		CString path;

		path.Empty();
		path.Format( "%s\\%s", fileInfo->strFileDir, fileInfo->strFileName);

		if(!IsImageGDIPLUSValid(path))
		{
			SHFILEINFO info;
			SHGetFileInfo(path,0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
			Bitmap image2(info.hIcon);
			int sourceWidth  = image2.GetWidth();
			int sourceHeight = image2.GetHeight();

			int destX = 0,
				destY = 0; 

			float nPercent  = 0;
			float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
			float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);

			if(nPercentH < nPercentW)
			{
				nPercent = nPercentH;
				destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
			}
			else
			{
				nPercent = nPercentW;
				destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
			}

			int destWidth  = (int)(sourceWidth * nPercent);
			int destHeight = (int)(sourceHeight * nPercent);

			bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
			bmPhoto->SetResolution( image2.GetHorizontalResolution(), image2.GetVerticalResolution() );

			Graphics *grPhoto = Graphics::FromImage( bmPhoto );
			Color colorW(255, 255, 255, 255);
			grPhoto->Clear( colorW );
			grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );

			grPhoto->DrawImage( &image2, Rect(destX, destY, destWidth, destHeight) );
			Pen newPen(Color(192,192,192,192));
			grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);
			//if(fileInfo->bFlag)
			//{
			//	Bitmap bmp(L".\\res\\flagicon.ico");
			//	grPhoto->DrawImage(&bmp, Rect(0, 0, 14, 14));
			//}

			if ( bFlag!=0 )
			{
				Bitmap *bmp ;

				if ( bFlag==1 )
					bmp=Bitmap::FromHICON(m_hSelIcon);
				else
					bmp=Bitmap::FromHICON(m_hUpdIcon);

				grPhoto->DrawImage(bmp, Rect(0, 0, 14, 14));
				delete bmp;
			}

			bmPhoto->GetHBITMAP( colorW, &hbmReturn );

			Bmp1.Attach( hbmReturn );
			LVITEM lvItem;
			lvItem.mask = LVIF_IMAGE;
			lvItem.iItem = nItemIndex;
			lvItem.iSubItem = 0;
			GetItem(&lvItem);
			m_ImageListThumb.Replace( lvItem.iImage, &Bmp1, NULL );	
			RedrawItems(nItemIndex, nItemIndex);
			delete grPhoto;
			delete bmPhoto;
			Bmp1.Detach();
			DeleteObject( hbmReturn );

			return nItemIndex;
		} // end if(!IsImageGDIPLUSValid(path))

		CComBSTR bst(path);
		Bitmap image(bst.m_str);
		int sourceWidth  = image.GetWidth();
		int sourceHeight = image.GetHeight();

		int destX = 0, destY = 0; 

		float nPercent  = 0;
		float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
		float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);

		if(nPercentH < nPercentW)
		{
			nPercent = nPercentH;
			destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
		}
		else
		{
			nPercent = nPercentW;
			destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
		}

		int destWidth  = (int)(sourceWidth * nPercent);
		int destHeight = (int)(sourceHeight * nPercent);

		bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
		bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );

		Graphics *grPhoto = Graphics::FromImage( bmPhoto );
		Color colorW(255, 255, 255, 255);
		grPhoto->Clear( colorW );
		grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );

		grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
		Pen newPen(Color(192,192,192,192));
		grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);

		//if(fileInfo->bFlag)
		//{
		//	Bitmap bmp(L".\\res\\flagicon.ico");
		//	grPhoto->DrawImage(&bmp, Rect(0, 0, 14, 14));
		//}
		bmPhoto->GetHBITMAP( colorW, &hbmReturn );

		Bmp1.Attach( hbmReturn );
		LVITEM lvItem;
		lvItem.mask = LVIF_IMAGE;
		lvItem.iItem = nItemIndex;
		lvItem.iSubItem = 0;
		GetItem(&lvItem);
		m_ImageListThumb.Replace( lvItem.iImage, &Bmp1, NULL );	
		RedrawItems(nItemIndex, nItemIndex);
		delete grPhoto;
		delete bmPhoto;
		Bmp1.Detach();
		DeleteObject( hbmReturn );
	}
	else
	{
	//	break;	// 找到之后就直接返回
	}

	return nItemIndex;
}

//void CThumBnaiListCtrl::RemoveSelFlag(DWORD dwFileID)
//{
//	for (int nIndex = 0; nIndex < GetItemCount(); ++nIndex)
//	{
//		DWORD dwItemData = DWORD(GetItemData(nIndex));
//		if(dwItemData != dwFileID)
//			continue;
//		FILEINFO *fileInfo = GetFileData(dwFileID);
//		if(fileInfo == NULL)
//			continue;
//
//		fileInfo->bFlag   = 0;
//		HBITMAP hbmReturn = NULL; 
//		Bitmap  *bmPhoto  = NULL;
//		CBitmap Bmp1;
//		CString path;
//		path.Empty();
//		path.Format( "%s\\%s", fileInfo->strFileDir, fileInfo->strFileName);
//
//		if(!IsImageGDIPLUSValid(path))
//		{
//			SHFILEINFO info;
//			SHGetFileInfo(path,0,&info,sizeof(&info),SHGFI_DISPLAYNAME | SHGFI_ICON);
//			Bitmap image2(info.hIcon);
//			int sourceWidth  = image2.GetWidth();
//			int sourceHeight = image2.GetHeight();
//
//			int destX = 0,
//				destY = 0; 
//
//			float nPercent  = 0;
//			float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
//			float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
//
//			if(nPercentH < nPercentW)
//			{
//				nPercent = nPercentH;
//				destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
//			}
//			else
//			{
//				nPercent = nPercentW;
//				destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
//			}
//
//			int destWidth  = (int)(sourceWidth * nPercent);
//			int destHeight = (int)(sourceHeight * nPercent);
//
//			bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
//			bmPhoto->SetResolution( image2.GetHorizontalResolution(), image2.GetVerticalResolution() );
//
//			Graphics *grPhoto = Graphics::FromImage( bmPhoto );
//			Color colorW(255, 255, 255, 255);
//			grPhoto->Clear( colorW );
//			grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );
//
//			grPhoto->DrawImage( &image2, Rect(destX, destY, destWidth, destHeight) );
//			Pen newPen(Color(192,192,192,192));
//			grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);
//			//if(fileInfo->bFlag)
//			//{
//			//	Bitmap bmp(L".\\res\\flagicon.ico");
//			//	grPhoto->DrawImage(&bmp, Rect(0, 0, 14, 14));
//			//}
//			bmPhoto->GetHBITMAP( colorW, &hbmReturn );
//
//			Bmp1.Attach( hbmReturn );
//			LVITEM lvItem;
//			lvItem.mask = LVIF_IMAGE;
//			lvItem.iItem = nIndex;
//			lvItem.iSubItem = 0;
//			GetItem(&lvItem);
//			m_ImageListThumb.Replace( lvItem.iImage, &Bmp1, NULL );	
//			RedrawItems(nIndex, nIndex);
//			delete grPhoto;
//			delete bmPhoto;
//			Bmp1.Detach();
//			DeleteObject( hbmReturn );
//			return;
//		}
//		CComBSTR bst(path);
//		Bitmap image(bst.m_str);
//		int sourceWidth  = image.GetWidth();
//		int sourceHeight = image.GetHeight();
//
//		int destX = 0,
//			destY = 0; 
//
//		float nPercent  = 0;
//		float nPercentW = ((float)THUMBNAIL_WIDTH/(float)sourceWidth);;
//		float nPercentH = ((float)THUMBNAIL_HEIGHT/(float)sourceHeight);
//
//		if(nPercentH < nPercentW)
//		{
//			nPercent = nPercentH;
//			destX    = (int)((THUMBNAIL_WIDTH - (sourceWidth * nPercent))/2);
//		}
//		else
//		{
//			nPercent = nPercentW;
//			destY    = (int)((THUMBNAIL_HEIGHT - (sourceHeight * nPercent))/2);
//		}
//
//		int destWidth  = (int)(sourceWidth * nPercent);
//		int destHeight = (int)(sourceHeight * nPercent);
//
//		bmPhoto = new Bitmap( THUMBNAIL_WIDTH, THUMBNAIL_HEIGHT, PixelFormat24bppRGB );
//		bmPhoto->SetResolution( image.GetHorizontalResolution(), image.GetVerticalResolution() );
//
//		Graphics *grPhoto = Graphics::FromImage( bmPhoto );
//		Color colorW(255, 255, 255, 255);
//		grPhoto->Clear( colorW );
//		grPhoto->SetInterpolationMode( InterpolationModeHighQualityBicubic );
//
//		grPhoto->DrawImage( &image, Rect(destX, destY, destWidth, destHeight) );
//		Pen newPen(Color(192,192,192,192));
//		grPhoto->DrawRectangle(&newPen, 0, 0, THUMBNAIL_WIDTH -1 , THUMBNAIL_HEIGHT -1);
//		//if(fileInfo->bFlag)
//		//{
//		//	Bitmap bmp(L".\\res\\flagicon.ico");
//		//	grPhoto->DrawImage(&bmp, Rect(0, 0, 14, 14));
//		//}
//		bmPhoto->GetHBITMAP( colorW, &hbmReturn );
//
//		Bmp1.Attach( hbmReturn );
//		LVITEM lvItem;
//		lvItem.mask = LVIF_IMAGE;
//		lvItem.iItem = nIndex;
//		lvItem.iSubItem = 0;
//		GetItem(&lvItem);
//		m_ImageListThumb.Replace( lvItem.iImage, &Bmp1, NULL );	
//		RedrawItems(nIndex, nIndex);
//		delete grPhoto;
//		delete bmPhoto;
//		Bmp1.Detach();
//		DeleteObject( hbmReturn );
//	}
//}

int CThumBnaiListCtrl::GetEncoderClsid(const WCHAR *format, CLSID *pClsid)
{
	int nRet = -1;
	ImageCodecInfo * pCodecInfo = NULL;
	UINT nNum = 0,nSize = 0;
	GetImageEncodersSize(&nNum,&nSize);
	if (nSize<0)
	{
		return nRet;
	}
	pCodecInfo = new ImageCodecInfo[nSize];
	if (pCodecInfo==NULL)
	{
		return nRet;
	}
	GetImageEncoders(nNum,nSize,pCodecInfo);
	for (UINT i=0; i<nNum; i++)
	{
		if (wcscmp(pCodecInfo[i].MimeType,format)==0)
		{
			*pClsid = pCodecInfo[i].Clsid;
			nRet = i;

			delete[] pCodecInfo;
			return nRet;
		}
		else
		{
			continue;
		}
	}
	delete[] pCodecInfo;
	return nRet;
}


void CThumBnaiListCtrl::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 && m_bDraging == FALSE)
	{
		CPoint pt;
		::GetCursorPos(&pt);
		ScreenToClient(&pt);

		CRect rc;
		GetClientRect(rc);

		if (rc.PtInRect(pt)==FALSE )
		{
			m_bMouseOver=false;

			if ( m_nHoverItem != -1 )			RedrawItems(m_nHoverItem,m_nHoverItem);

			m_nHoverItem    = -1;
			m_nPreHoverItem = -1;

			KillTimer(0x01);
		}
	}

	CListCtrl::OnTimer(nIDEvent);
}

BOOL CThumBnaiListCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
//	GetClientRect(&rc);
//	pDC ->FillSolidRect(&rc, RGB(255, 255, 255));

//	return FALSE;

	return CListCtrl::OnEraseBkgnd(pDC);
}

void CThumBnaiListCtrl::OnPaint()
{

	if(GetItemCount() < 1)
	{
		CRect rc;
		GetClientRect(&rc);

		CDC * pDC = GetDC();
		pDC ->FillSolidRect(rc, RGB(0xff, 0xff, 0xff));
		ReleaseDC(pDC);

//		return ;
	}


	CListCtrl::OnPaint();
}

BOOL CThumBnaiListCtrl::DeleteItem(int nItem)
{
	DWORD dwFileId = GetItemData(nItem);

	FileInfoList::iterator iter;
	iter = find_if(m_FileList.begin(), m_FileList.end(), bind2nd(IDCompare(), dwFileId));

	if (iter != m_FileList.end())
	{
		m_FileList.erase(iter);
	}

	return CListCtrl::DeleteItem(nItem);
}

void CThumBnaiListCtrl::Clear()
{
	this->TerminateLoadThread();

	SetRedraw(FALSE);
	this->DeleteAllItems();
	SetRedraw();

	if(m_ImageListThumb.GetImageCount() > 0)
	{
		for(int i = m_ImageListThumb.GetImageCount() - 1; i >= 0; i --)
		{
			m_ImageListThumb.Remove(i);
		}
	}
	
	this->m_FileList.clear();
}

BOOL CThumBnaiListCtrl::PreCreateWindow(CREATESTRUCT& cs)
{
	cs.style |= LVS_SHOWSELALWAYS|LVS_ALIGNTOP|LVS_ICON|LVS_SINGLESEL|LVS_AUTOARRANGE;

	return CListCtrl::PreCreateWindow(cs);
}

void CThumBnaiListCtrl::OnListCtrlGetInfoTip(NMHDR *pNMHDR, LRESULT *pResult)
{
	*pResult = 1;	
//	return ;

	if(m_nToopTipTextFlags > 1)			return ;
	LPNMLVGETINFOTIP pGetInfoTip = reinterpret_cast<LPNMLVGETINFOTIP>(pNMHDR);

	static	CString str;
	str.Empty();
	
	if(pGetInfoTip ->iItem < 0 || pGetInfoTip ->iItem > GetItemCount())	return ;

	DWORD dwFileID = DWORD(GetItemData(pGetInfoTip ->iItem));
	FILEINFO *pFileINFO = GetFileData(dwFileID);
	if(pFileINFO != NULL)
	{
		CString strText;	

#if  0

		if(m_nToopTipTextFlags == 0)			str = "图片处的提示:\r\n";
		else if(m_nToopTipTextFlags == 1)		str = "文字处的提示:\r\n";
		else	str = "未知处的提示:\r\n";
#endif

		float fltTotalKB = float(pFileINFO ->ulFileSize) / 1024.0;

		CString strToolTipText;
	//	if(m_nToopTipTextFlags < 2)
		{
			if(m_nFlags == 0)
			{
				strToolTipText.Format("%s:%s\r\n大小:%.2fKB.",  _T("文件名"), pFileINFO ->strFileName, fltTotalKB);
			}
			else if(m_nFlags == 1)
			{
				strToolTipText.Format("%s:%s(选中后单击描述处或按F2键可进行修改)\r\n大小:%.2fKB.",  _T("描述"), pFileINFO ->strDescription, fltTotalKB);
			}
		}
	//	else
		{
		//	if(m_nToopTipTextFlags == 2)		strToolTipText.Format("向左旋转90度");
		//	else if(m_nToopTipTextFlags == 3)	strToolTipText.Format("向右旋转90度");
		//	else if(m_nToopTipTextFlags == 4)	strToolTipText.Format("放大预览图片");
		}
		
		str = strToolTipText;
	}

//	TRACE("CThumBnaiListCtrl::OnListCtrlGetInfoTip strToolTipText = %s.\n", str);

	pGetInfoTip ->dwFlags = 0; // LVGIT_UNFOLDED;
	pGetInfoTip ->cchTextMax = str.GetLength();
	pGetInfoTip ->pszText = (LPSTR)(LPCTSTR) str;

//	m_nToopTipTextFlags = -1;
}

void CThumBnaiListCtrl::SelectItem(FileInfoList & _lstFileInfo)
{
#if 1
	const int nCount = GetItemCount();
	for(int i = 0; i < nCount; ++ i)	
	{
	//	EnsureVisible(i, FALSE);
		SetItemState(i, ~LVIS_SELECTED, LVIS_SELECTED);
	}

	FileInfoList_iterator it = _lstFileInfo.begin();
	for(; it != _lstFileInfo.end(); ++ it)
	{	
		FILEINFO _FileInfo = (*it); 
		
		int nIndex;
		if((nIndex = FindFileIndex(_FileInfo.dwFileID)) != -1)
		{
			SelectItem(nIndex);
		}
	} // end for

	SetFocus();
#endif
}


void CThumBnaiListCtrl::SelectItem(const int nItem)
{
#if 1
	if(nItem > -1 && nItem < GetItemCount())
	{
		EnsureVisible(nItem, FALSE);
	//	SetItemState(nItem,  LVIS_FOCUSED | LVIS_SELECTED, LVIS_FOCUSED | LVIS_SELECTED);
			
		ListView_SetItemState(m_hWnd, nItem, LVIS_SELECTED | LVIS_FOCUSED, 0x000F)

		SetFocus();
	}
#endif
}

void CThumBnaiListCtrl::DeselectItem(const int nItem)
{
	if(nItem == -1)
	{
		for(int i = 0; i < GetItemCount(); ++ i)
		{
			SetItemState(i,  0 , LVIS_FOCUSED | LVIS_SELECTED);
		//	SetItemState(i,  ~LVIS_SELECTED, LVIS_SELECTED);

		//	SetItemState(i, NULL, NULL);
		}
	}
	else
	{
		if(nItem > -1 && nItem < GetItemCount())
		{
		//	SetItemState(nItem,  ~LVIS_FOCUSED , LVIS_FOCUSED);
		//	SetItemState(nItem,  ~LVIS_SELECTED, LVIS_SELECTED);

			SetItemState(nItem,  0 , LVIS_FOCUSED | LVIS_SELECTED);
		}
	}
}

#ifdef _USE_CUSTOME_DRAWITEM_
void CThumBnaiListCtrl::MeasureItem(LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	lpMeasureItemStruct ->itemHeight = THUMBNAIL_HEIGHT;
}

void CThumBnaiListCtrl::DrawItem( LPDRAWITEMSTRUCT lpDrawItemStruct )
{
	//

	CDC *pDC = CDC::FromHandle(lpDrawItemStruct ->hDC);
	CRect rcItem(lpDrawItemStruct ->rcItem);
	const int nItem = lpDrawItemStruct ->itemID;

	CImageList *pImageList = NULL;

	const int nSaveDC = pDC ->SaveDC();

	LV_ITEM lvi;

	lvi.mask	= LVIF_IMAGE | LVIF_STATE;
	lvi.iItem	= nItem;
	lvi.iSubItem= 0;
	lvi.stateMask = 0xffff;

	GetItem(&lvi);

	// 条目是否应该被高亮显示
	const	BOOL bHighlight = ((lvi.state & LVIS_DROPHILITED) || 
		((lvi.state & LVIS_SELECTED) && ((GetFocus() == this) || GetStyle() & LVS_SHOWSELALWAYS)));

	// 得到绘制矩形
	CRect rcBounds, rcLabel, rcIcon;

	GetItemRect(nItem, &rcBounds, LVIR_BOUNDS);
	GetItemRect(nItem, rcLabel, LVIR_LABEL);
	GetItemRect(nItem, rcIcon, LVIR_ICON);

	CRect rcCol(rcBounds);

	CString strLabel = GetItemText(nItem, 0);
	int offset = pDC ->GetTextExtent(_T(" "), 1).cx * 2;

	CRect rcHighlight;

	CRect rcWnd;	int nExt = 0;

	rcHighlight = rcLabel;

	//绘制背景颜色
	if(bHighlight)
	{
#if 0
		// 修改此处的颜色能改变当前选中项的颜色
		pDC ->SetTextColor(m_clrItemSelectedFontForeColor);
		pDC ->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC ->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

		// 在此绘制当前选中项的列
	//	DrawSelectedSubItemRect(pDC, nItem);
#endif
	}
	else
	{
		
	}

	// 设置剪切区域

	CRgn rgn;
	rcCol.right = rcCol.left + GetColumnWidth(0);

	rgn.CreateRectRgnIndirect(&rcCol);
	pDC ->SelectClipRgn(&rgn);
	rgn.DeleteObject();

	// 绘制状态图标
	if(lvi.state & LVIS_STATEIMAGEMASK)
	{
		const int nImage = ((lvi.state & LVIS_STATEIMAGEMASK) >> 12) - 1;
		pImageList = GetImageList(LVSIL_STATE);
		if(pImageList != NULL)
		{
			pImageList ->Draw(pDC, nImage, CPoint(rcCol.left, rcCol.top), ILD_TRANSPARENT);
		}
	}

	// 绘制正常和重叠图像
	pImageList = GetImageList(LVSIL_NORMAL);
	if(pImageList != NULL)
	{
		const UINT nOverImageMask = lvi.state & LVIS_OVERLAYMASK;
		pImageList ->Draw(pDC, lvi.iImage, CPoint(rcIcon.left, rcIcon.top), (bHighlight ? ILD_BLEND50 : 0) | ILD_TRANSPARENT | nOverImageMask);
	}

	if(bHighlight)
	{
		// 修改此处的颜色能改变当前选中项的颜色
		pDC ->SetTextColor(::GetSysColor(COLOR_HIGHLIGHTTEXT));
		pDC ->SetBkColor(::GetSysColor(COLOR_HIGHLIGHT));

		pDC ->FillRect(rcHighlight, &CBrush(::GetSysColor(COLOR_HIGHLIGHT)));

		// 在此绘制当前选中项的列
//		DrawSelectedSubItemRect(pDC, nItem);
	}

	// 绘制第0列

	rcLabel.left += offset / 2;		rcLabel.right -= offset;
	pDC ->DrawText(strLabel, rcLabel, DT_LEFT | DT_SINGLELINE | DT_NOPREFIX |DT_NOCLIP | DT_VCENTER | DT_END_ELLIPSIS);

	// 绘制其它列
	LV_COLUMN lvc;
	lvc.mask = LVCF_FMT | LVCF_WIDTH;

	if(!bHighlight)
	{
		pDC ->SetTextColor(::GetSysColor(COLOR_WINDOWTEXT));
		pDC ->SetBkColor(::GetSysColor(COLOR_WINDOW));
	}
	rcBounds.right = rcHighlight.right > rcBounds.right ? rcHighlight.right : rcBounds.right;
	rgn.CreateRectRgnIndirect(&rcBounds);
	pDC ->SelectClipRgn(&rgn);

#if 0
	for(int nColumn = 1; CListCtrl::GetColumn(nColumn, &lvc); nColumn ++)
	{
		rcCol.left = rcCol.right;	rcCol.right += lvc.cx;

		if(!bHighlight)
		{
			// 在此设置每一项SubItem的属性(颜色,字体,字体大小)
			AskSubItemBaseInfoAttributes(nItem, nColumn, pDC);
		}

		CFont *pOldFont = NULL;

		if(m_Highlight == HIGHTLIGHT_NORMAL)
		{			
			pDC ->FillRect(rcCol, &CBrush(::GetSysColor(COLOR_WINDOW)));
		}
		else if(m_Highlight == HIGHTLIGHT_ROW && bHighlight && m_bEnableSelectedItemBoldFont)
		{
			// 在此绘制选中行的字体,加粗显示
			pOldFont = pDC ->SelectObject(&m_ftSelectionBold);
		}

		const CString strItemText = GetItemText(nItem, nColumn);
		if(strItemText.GetLength() < 1)	continue;

		UINT nJustify = DT_LEFT;
		switch(lvc.fmt & LVCFMT_JUSTIFYMASK)
		{
		case LVCFMT_LEFT:		break;
		case LVCFMT_CENTER:		nJustify = DT_CENTER;	break;
		case LVCFMT_RIGHT:		nJustify = DT_RIGHT;	break;
		default:	break;
		}

		rcLabel = rcCol;
		rcLabel.left += offset;		rcLabel.right -= offset;

		pDC ->DrawText(strItemText, rcLabel, nJustify | DT_SINGLELINE | DT_NOPREFIX | DT_VCENTER | DT_END_ELLIPSIS);

		if(pOldFont != NULL)	pDC ->SelectObject(pOldFont);
	} // end for
#endif

	// 如果条目具有焦点,则绘制焦点矩形
	if(lvi.state & LVIS_FOCUSED && (GetFocus() == this))	pDC ->DrawFocusRect(rcHighlight);

	pDC ->RestoreDC(nSaveDC);
}
#endif // _USE_CUSTOME_DRAWITEM_


void CThumBnaiListCtrl::RedrawPics(const int nMaxSideSize)
{
	int nItemCount = GetItemCount();
	for(int i = 0; i < nItemCount; ++ i)
	{
		DWORD dwFileID = (DWORD) GetItemData(i);
		FILEINFO *pFile = GetFileData(dwFileID);

		if(pFile == NULL)	continue;

	//	CompressFileIfNeed(pFile, nMaxSideSize);
	} // end for
}


BOOL CThumBnaiListCtrl::PreTranslateMessage(MSG* pMsg)
{
	if (m_wndToolTipCtrl.m_hWnd)		m_wndToolTipCtrl.RelayEvent(pMsg);

	if (    pMsg->message==WM_LBUTTONDOWN ||
		pMsg->message==WM_LBUTTONUP ||
		pMsg->message==WM_RBUTTONDOWN ||
		pMsg->message==WM_RBUTTONUP ||
		pMsg->message==WM_LBUTTONDBLCLK ||
		pMsg->message==WM_RBUTTONDBLCLK ||
		pMsg->message==WM_NCLBUTTONDOWN ||
		pMsg->message==WM_NCLBUTTONUP ||
		pMsg->message==WM_NCRBUTTONDOWN ||
		pMsg->message==WM_NCRBUTTONUP ||
		pMsg->message==WM_NCLBUTTONDBLCLK ||
		pMsg->message==WM_NCRBUTTONDBLCLK ||
		pMsg->message==WM_KEYDOWN ||
		pMsg->message==WM_KEYUP )
	{
	//	if(!g_bEnableDrawTool)	return TRUE;
	}


	return CListCtrl::PreTranslateMessage(pMsg);
}

int CThumBnaiListCtrl::GetCurSel(void)
{
	POSITION pos = GetFirstSelectedItemPosition();
	if(pos)	return GetNextSelectedItem(pos);

	return -1;
}

void CThumBnaiListCtrl::OnNMClick(NMHDR *pNMHDR, LRESULT *pResult)
{
//	ClickedInListCtrl();
	*pResult = 0;
}

LRESULT CThumBnaiListCtrl::OnMsgOfListCtrlTextTipLBUTTONDOWN(WPARAM wParam, LPARAM lParam)
{
	ClickedInListCtrl();
	return 1;
}

void CThumBnaiListCtrl::ClickedInListCtrl()
{
	//CWaitCursor wait_cursor;

	try
	{
		CPoint point;
		GetCursorPos(&point);
		ScreenToClient(&point);

		RECT rc;
		RECT rc2 = {0, 0, 16, 16};
		for(int i = 0; i< GetItemCount(); ++i)
		{
			GetItemRect(i, &rc, LVIR_ICON);

			if(PtInRect(&rc,point))
			{
				RECT rcleft = {rc.left+8,rc.bottom - 16,rc.left+8 + 16,rc.bottom};
				RECT rcright = {rc.right - 16 - 8,rc.bottom - 16,rc.right - 16,rc.bottom};
				RECT rcblowup = {rc.right - (rc.right - rc.left)/2 - 8, rc.bottom - 16,rc.right - (rc.right - rc.left)/2 + 8,rc.bottom};

				if(PtInRect(&rcleft, point))
				{
					Rotate(i, EM_ROTATE_LEFT);
				}

				if(PtInRect(&rcright, point))
				{
					Rotate(i, EM_ROTATE_RIGHT);
				}

				if(PtInRect(&rcblowup, point))
				{
					DWORD dwFileID = DWORD(GetItemData(i));
					FILEINFO *viewFile = GetFileData(dwFileID);
					if (viewFile == NULL)
						return;
					//((AfxGetApp()->m_pMainWnd))->SendMessage(WM_PREVIEW, i, (LPARAM)viewFile);//dwFileID);

					//	if(m_nFlags != 1)
					{
						GetParent()->SendMessage(WM_PREVIEW, i, (LPARAM)viewFile);//dwFileID);
					}
				}
			}
		} // end for
#if 0
		if(m_nFlags == 1)
		{
			int nCurSel = GetCurSel();
			if(nCurSel >= 0)
			{
				CEdit *pEdit = EditLabel(nCurSel);
			}
		}
#endif 
	}
	catch( ... )
	{
		MessageBox("CThumBnaiListCtrl::OnNMClick unknown exception caught!", APP_NAME, MB_ICONERROR);
	}

}

void CThumBnaiListCtrl::OnDropFiles(HDROP hDropInfo)
{
#if 0

	CString strTmp;
	CStringList strlstFileInfo;

	if (hDropInfo != NULL) 
	{
		int nCount = ::DragQueryFile (hDropInfo, (UINT) -1, NULL, 0);
		if (nCount)
		{
			TCHAR szFile[MAX_PATH];
			for (int i=0; i<nCount; i++) 
			{
				::DragQueryFile (hDropInfo, i, szFile,	sizeof (szFile) / sizeof (TCHAR));
				
				strTmp.Format("%s\r\n", szFile);	strlstFileInfo.AddTail(szFile);
				strFileInfo += strTmp;
			}
		}
	} // end if (hDropInfo != NULL) 

	MessageBox(strFileInfo);


	if(m_pParentWnd !=  NULL && m_pParentWnd ->m_hWnd != NULL && ::IsWindow(m_pParentWnd ->m_hWnd))
	{
		((CUpLoadPhotoDlg *) m_pParentWnd) ->AddUploadedFile(strlstFileInfo);
	}
#endif

	CListCtrl::OnDropFiles(hDropInfo);
}

