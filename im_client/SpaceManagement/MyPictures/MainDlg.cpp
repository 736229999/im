#include "..\writearticle\maindlg.h"
// MainDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MyPictures.h"
#include "MainDlg.h"
#include "webdataprocess.h"
#include "WebPictureProcess.h"
#include "WebInterfaceWrapper.h"

CMainDlg * CMainDlg::pInstance = NULL;

#define ID_SAVEAS      1002
#define ID_DELETEPIC   1003
#define ID_OPEN		   1004
#define ID_COPYURL	   1005

// CMainDlg 对话框
#define KEYDOWN(vk_code) ((GetAsyncKeyState(vk_code) & 0x8000) ? TRUE : FALSE )

IMPLEMENT_DYNAMIC(CMainDlg, CXSkinDlg)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
: CXSkinDlg(CMainDlg::IDD, pParent),
m_myPictures(g_pUserInfo->GetUserID())
{
	m_pFocusPicture  = NULL;
	m_iPicturePosition = 0;
	m_nFocusPictureID   = -1;
	m_bmpPicFrame.LoadBitmap(IDB_PICTURE_FRAME);
	m_bmpPicPig.LoadBitmap(IDB_PICTURE_FLAG);
	m_bmpError.LoadBitmap(IDB_PICTURE_ERROR);

	m_iHotPicture      = -1;

	m_bPictureTitleModified = false;
	m_bPictureViewModified  = false;

	m_hLoadFocusPic         = NULL;
	m_bUpdatingMyPictures   = false;
	m_bWaittingFlag         = true;
	m_pFormProgress3         = NULL;
	m_pFormProgress2        = NULL;

	memset(m_hExportThreads,0,sizeof(m_hExportThreads));

	m_bModified = false;
	m_nDownloadThreadRef = 0;
	m_hDeletePicProc = NULL;
	m_hLoadAlbumListProc = NULL;

	m_fontText.CreateFont(
		12,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_NORMAL,                  // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		"宋体");

	m_nAlbumID = 0;

	Create(CMainDlg::IDD,pParent);
}

CMainDlg::~CMainDlg()
{
	pInstance = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_BUTTON_LEFT,m_btnLeft);
	DDX_Control(pDX,IDC_BUTTON_LEFT2,m_btnBigLeft);
	DDX_Control(pDX,IDC_BUTTON_RIGHT,m_btnRight);
	DDX_Control(pDX,IDC_BUTTON_RIGHT2,m_btnBigRight);
	DDX_Control(pDX,IDC_SLIDER1,m_wndSlider);
	DDX_Control(pDX, IDC_BUTTON_PIC, m_btnPic);
	DDX_Control(pDX, IDC_BUTTON_BIGPHOTO, m_btnBigHead);
	DDX_Control(pDX, IDC_BUTTON_REFRESH, m_btnRefresh);
	DDX_Control(pDX, IDC_BUTTON_EXPORT, m_btnExport);
	DDX_Control(pDX, IDC_BUTTON_SETTING, m_btnSetting);
	DDX_Control(pDX, IDC_EDIT_DESCRIPTION, m_edtDescription);
	DDX_Control(pDX, IDC_COMBO_VIEW, m_comboView);
	DDX_Control(pDX, IDC_BUTTON_SAVE, m_btnSave);
	DDX_Control(pDX, IDC_BUTTON_DELETEALBUM,m_btnDeleteAlbum);
	DDX_Control(pDX, IDC_BUTTON_DELETEPIC,m_btnDeletePic);
	DDX_Control(pDX, IDC_ALBUM_FOLDER,m_wndAlbumFolder);
}


BEGIN_MESSAGE_MAP(CMainDlg, CXSkinDlg)
	ON_WM_NCDESTROY()
	ON_WM_ERASEBKGND()
	ON_BN_CLICKED(IDC_BUTTON_LEFT, OnBnClickedButtonLeft)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT, OnBnClickedButtonRight)
	ON_WM_CLOSE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_CTLCOLOR()
	ON_WM_MOUSEMOVE()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_HSCROLL()
	ON_WM_PAINT()
	ON_CBN_SELCHANGE(IDC_COMBO_VIEW, OnCbnSelchangeComboView)
	ON_MESSAGE(WM_WEBDATA_NOTIFY,OnWebDataNotify)
	ON_MESSAGE(WM_WEBPICTURE_NOTIFY,OnWebPictureNotify)
	ON_MESSAGE(WM_THREAD_NOTIFY,OnThreadNotify)
	ON_WM_TIMER()
	ON_BN_CLICKED(IDC_BUTTON_LEFT2, OnBnClickedButtonLeft2)
	ON_BN_CLICKED(IDC_BUTTON_RIGHT2, OnBnClickedButtonRight2)
	ON_EN_CHANGE(IDC_EDIT_DESCRIPTION, OnEnChangeEditDescription)
	ON_BN_CLICKED(IDC_BUTTON_SAVE, OnBnClickedButtonSave)
	ON_BN_CLICKED(IDC_BUTTON_SETTING, OnBnClickedButtonSetting)
	ON_BN_CLICKED(IDC_BUTTON_PIC, OnBnClickedButtonPic)
	ON_BN_CLICKED(IDC_BUTTON_BIGPHOTO, OnBnClickedButtonBigphoto)
	ON_WM_GETMINMAXINFO()
	ON_BN_CLICKED(IDC_BUTTON_EXPORT, OnBnClickedButtonExport)
	ON_BN_CLICKED(IDC_BUTTON_REFRESH, OnBnClickedButtonRefresh)
	ON_BN_CLICKED(IDC_BUTTON_DELETEPIC, OnBnClickedButtonDeletePic)
	ON_BN_CLICKED(IDC_BUTTON_DELETEALBUM, OnBnClickedButtonDeleteAlbum)
	ON_COMMAND(ID_DELETEPIC,OnBnClickedButtonDeletePic)
	ON_COMMAND(ID_COPYURL,OnBnClickedButtonCopyUrl)
	ON_COMMAND(ID_SAVEAS,OnPictureSaveAs)
	ON_COMMAND(ID_OPEN,OnPictureOpen)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnCancelBackup)
	ON_WM_MEASUREITEM()
	ON_WM_RBUTTONUP()
	ON_STN_CLICKED(IDC_STATIC_DOWNLOAD, OnStnClickedStaticDownload)
	ON_BN_CLICKED(IDC_ALBUM_FOLDER, OnBnClickedAlbumFolder)
END_MESSAGE_MAP()


// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	EnableHelp(true);

	m_myPictures.uID = g_pUserInfo->GetUserID();

	CString strTitle;
	strTitle.Format("我的图片 - %s",(LPCSTR)g_pUserInfo->GetNickName());
	SetWindowText(strTitle);
	SetTitleIcon(GetResDLLHandle(),IDI_MY_PIC);

	m_btnBigRight.SetStyle(1);
	m_btnRight.SetStyle(1);
	m_wndSlider.SetBackColor(IM_SPACE_COLOR);
	m_wndAlbumFolder.AddItem("手机相册",IDB_ALBUM_MOBILE,1);
	m_wndAlbumFolder.AddItem("插图相册",IDB_ALBUM_DRAFT,2);
	m_wndAlbumFolder.AddItem("所有相册",IDB_ALBUM_ALL,0);

	UpdateAlbumNum();
	AdjustUI();

	ComboInitViewProp(&m_comboView);

	SetTimer(0x03,100,NULL);

	m_edtDescription.SetLimitText(50);
    
	UpdateButtonState();

	SetMinMaxInfo(800,600,0,0);

	return TRUE;  
}

void CMainDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CMainDlg::UpdateAlbumNum()
{
	int n = m_wndAlbumFolder.GetCount();

	for ( int i=0; i<n; i++ )
	{
		int nAlbumId    = m_wndAlbumFolder.GetItemData(i);
		int nPictureNum = 0;

		int nTotoal = m_myPictures.vecPics.size();
		for ( int ii=0; ii<nTotoal; ii++ )
		{
			if ( m_myPictures.vecPics[ii]->nAlbumId==nAlbumId )
				nPictureNum++;
		}

		CString strItemText;

		if ( nAlbumId==0 )//所有图片
			strItemText.Format("所有图片 (%d)",nPictureNum);
		else
		if ( nAlbumId==1 )
			strItemText.Format("手机相册 (%d)",nPictureNum);
		else
		if ( nAlbumId==2 )
			strItemText.Format("插图相册 (%d)",nPictureNum);
		else
		{
			int nAlbumCount = m_myAlbumInfo.vecAlbum.size();
			for ( int iii=0; iii<nAlbumCount; iii++ )
			{
				if ( m_myAlbumInfo.vecAlbum[iii].nAlbumId==nAlbumId )
				{
					strItemText.Format("%s (%d)",m_myAlbumInfo.vecAlbum[iii].szAlbumName,nPictureNum);
					break;
				}
			}
		}

		m_wndAlbumFolder.SetItemText(i,strItemText);
	}
}

CMainDlg * CMainDlg::Show()
{
	if ( pInstance==NULL )
	{
		pInstance = new CMainDlg(GetDesktopWindow());
		pInstance->CenterWindow(GetDesktopWindow());
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if (pInstance->IsIconic())
		{
			pInstance->ShowWindow(SW_RESTORE);
		}
	}

	pInstance->BringWindowToTop();

	return pInstance;
}

void CMainDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}

void CMainDlg::OnHScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	if ( pScrollBar && pScrollBar->m_hWnd==m_wndSlider.m_hWnd )
	{
		int iPosition = m_wndSlider.GetPos();	
		m_iPicturePosition = iPosition;
		ReDrawList();

		UpdateButtonState();
	}

	CDialog::OnHScroll(nSBCode, nPos, pScrollBar);
}


HBRUSH CMainDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	UINT nID = pWnd->GetDlgCtrlID();

	if ( CTLCOLOR_STATIC==nCtlColor  && nID!=IDC_ALBUM_FOLDER )
	{
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
		hbr = brushBack;
	}

	return hbr;
}

void CMainDlg::OnSize(UINT nType, int cx, int cy)
{
	CXSkinDlg::OnSize(nType, cx, cy);

	if ( m_hWnd && m_btnLeft.m_hWnd && nType!=SIZE_MINIMIZED )
	{
		AdjustUI();

		CRgn rgn;
		GetRepaintRgn(rgn);
		InvalidateRgn(&rgn);

		ReDraw();
		m_edtDescription.Invalidate();
		GetDlgItem(IDC_STATIC2)->Invalidate();
		GetDlgItem(IDC_STATIC3)->Invalidate();
		m_btnSave.Invalidate();

		UpdateButtonState();
	}
}

void CMainDlg::AdjustUI()
{
	CRect rcClient;
	GetClientRect(&rcClient);

	int cx = rcClient.Width();
	int cy = rcClient.Height();

	m_wndAlbumFolder.MoveWindow(3,65,155,cy-68);

	int nSpace = 160;

	m_ListRect.left   = 25 + nSpace;
	m_ListRect.top    = cy - 93;
	m_ListRect.bottom = m_ListRect.top + 90;
	m_ListRect.right  = cx - 28 ;

	int nBtnCy  = 75;
	int nBtnTop = m_ListRect.top + (m_ListRect.Height()-nBtnCy)/2;

	m_btnLeft.MoveWindow(nSpace+5,nBtnTop,13,nBtnCy);
	m_btnRight.MoveWindow(cx-20,nBtnTop,13,nBtnCy);

	int nSliderTop = m_ListRect.top - 22;
	int nSliderCy  = 18;

	m_wndSlider.MoveWindow(nSpace+15,nSliderTop,cx-nSpace-30,nSliderCy);

	m_PictureRect.top    = 80;
	m_PictureRect.left   = nSpace+25;
	m_PictureRect.right  = cx - 28;
	m_PictureRect.bottom = nSliderTop - 35;

	int nBigBtnCy  = m_PictureRect.Height()-40;
	int nBigBtnTop = m_PictureRect.top + 20;

	m_btnBigLeft.MoveWindow(nSpace+5,nBigBtnTop,13,nBigBtnCy);
	m_btnBigRight.MoveWindow(cx-20,nBigBtnTop,13,nBigBtnCy);

	int nToolTop = nSliderTop-25;

	int nLeft = (cx-422)/2;
	int nOffset = 0;

	CRect rc;
	CWnd *pWnd ;

	pWnd = GetDlgItem(IDC_STATIC_ETCHED1);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);
	rc.left = 3;
	rc.right = cx-3;
	pWnd->MoveWindow(&rc);

	pWnd = GetDlgItem(IDC_STATIC2);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.top = nToolTop + 3;
	rc.bottom = rc.top+18;

	nOffset = rc.left - nLeft;
	rc.left-=nOffset;
	rc.right-=nOffset;

	pWnd->MoveWindow(&rc);

	pWnd = GetDlgItem(IDC_STATIC3);
	pWnd->GetWindowRect(&rc);
	ScreenToClient(&rc);	

	rc.top =  nToolTop+3;
	rc.bottom = rc.top + 18;
	rc.left-=nOffset;
	rc.right-=nOffset;

	pWnd->MoveWindow(&rc);

	m_edtDescription.GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.top = nToolTop+1;
	rc.bottom = rc.top + 22;
	rc.left-=nOffset;
	rc.right-=nOffset;

	m_edtDescription.MoveWindow(&rc);

	m_comboView.GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.top = nToolTop+1;
	rc.bottom = rc.top + 122;
	rc.left-=nOffset;
	rc.right-=nOffset;

	m_comboView.MoveWindow(&rc,FALSE);

	m_btnSave.GetWindowRect(&rc);
	ScreenToClient(&rc);

	rc.top = nToolTop;
	rc.bottom = rc.top + 22;
	rc.left-=nOffset;
	rc.right-=nOffset;

	m_btnSave.MoveWindow(&rc);

}

void CMainDlg::ReDrawList()
{
	CDC *pDC = GetDC();
	DrawList(pDC);
	ReleaseDC(pDC);
}

void CMainDlg::ReDraw()
{
	CDC *pDC = GetDC();
	DrawPicture(pDC);
	DrawList(pDC);
	ReleaseDC(pDC);
}

void CMainDlg::ReDrawPicture()
{
	CDC *pDC = GetDC();
	DrawPicture(pDC);
	ReleaseDC(pDC);
}

void CMainDlg::DrawList(CDC *pDC)
{
	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);

	CDC dcTemp;
	dcTemp.CreateCompatibleDC(pDC);

	int cx  = m_ListRect.Width();
	int cy  = m_ListRect.Height();

	int nSavedMemDC  = dcMem.SaveDC();
	int nSavedTempDC = dcTemp.SaveDC();

	CBitmap bmMem;
	bmMem.CreateCompatibleBitmap(pDC,cx,cy);
	dcMem.SelectObject(&bmMem);

	dcMem.FillSolidRect(0,0,cx,cy,IM_SPACE_COLOR);

	m_secBitmaps.Lock();

	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = cx/(PICTURE_CX+PICTURE_SPACE);

	int nTop         = (cy-PICTURE_CY)/2+1;

	int nOffset      = 0;

	for ( int i=0; i<(nDrawBitmaps+2); i++ )
	{
		int iPicture = i + m_iPicturePosition;
		int iPictureLeft = i*(PICTURE_CX+PICTURE_SPACE);

		if ( iPicture>=0 && iPicture<=nBitmaps )
		{
			CBitmap *pBitmap;

			if ( iPicture==nBitmaps )
			{
				pBitmap = &m_bmpPicPig;
			}
			else
			{
				pBitmap = FindBitmap(m_vecBitmaps[iPicture]->dwPictureID);// m_vecBitmaps[iPicture]->pBitmap;
			}

			if ( pBitmap==NULL )
				pBitmap=&m_bmpError;

			dcTemp.SelectObject(&m_bmpPicFrame);
			dcMem.BitBlt(iPictureLeft,nTop,FRAME_SZ,FRAME_SZ,&dcTemp,0,0,SRCCOPY);
			dcTemp.SelectObject(pBitmap);
			dcMem.BitBlt(iPictureLeft+1,nTop+1,PICTURE_CX,PICTURE_CY,&dcTemp,0,0,SRCCOPY);

			CRect rcLine;
			rcLine.left   = iPictureLeft+1+nOffset;
			rcLine.top    = nTop;
			rcLine.right  = rcLine.left+PICTURE_CX;
			rcLine.bottom = rcLine.top+PICTURE_CY;

			if ( iPicture<nBitmaps )
			{
				DWORD dwPicID = m_vecBitmaps[iPicture]->dwPictureID;

				if ( dwPicID==m_nFocusPictureID )
				{
					dcMem.Draw3dRect(&rcLine,RGB(255,0,0),RGB(255,0,0));
				}

				if ( iPicture == m_iHotPicture )
				{
					dcMem.Draw3dRect(&rcLine,RGB(0,0,255),RGB(0,0,255));
				}

				int n = this->m_vecMulSelPics.size();
				for ( int iSel=0; iSel<n; iSel++ )
				{
					if ( m_vecMulSelPics[iSel]==dwPicID )
					{
						dcMem.Draw3dRect(&rcLine,RGB(255,0,255),RGB(255,0,255));
						rcLine.InflateRect(1,1);
						dcMem.Draw3dRect(&rcLine,RGB(255,0,255),RGB(255,0,255));
						break;
					}
				}
			}
		}

	}

	m_secBitmaps.Unlock();

	pDC->BitBlt(m_ListRect.left,m_ListRect.top,cx,cy,&dcMem,0,0,SRCCOPY);
	dcMem.RestoreDC(nSavedMemDC);
	dcTemp.RestoreDC(nSavedTempDC);
}

CString CMainDlg::GetAlbumName(uint32 album_id)
{
	if ( album_id==0 )
	{
		return "所有图片";
	}
	else
	if ( album_id==1 )
		return "手机相册";
	else
	if ( album_id==2 )
		return "插图相册";
	else
	{
		int c = m_myAlbumInfo.vecAlbum.size();
		for ( int i=0; i<c; i++ )
		{
			if ( m_myAlbumInfo.vecAlbum[i].nAlbumId==album_id )
				return m_myAlbumInfo.vecAlbum[i].szAlbumName;
		}
	}

	return "自定义相册";
}

void CMainDlg::DrawPicture(CDC *pDC)
{
	Bitmap *pBitmap ;

	if ( m_pFocusPicture==NULL )
		pBitmap = Bitmap::FromHBITMAP((HBITMAP)m_bmpPicPig.m_hObject,NULL);
	else
		pBitmap = m_pFocusPicture;

	if ( pBitmap )
	{
		int cx = m_PictureRect.Width();
		int cy = m_PictureRect.Height();

		CDC dcMem;
		dcMem.CreateCompatibleDC(pDC);

		int nSavedMemDC = dcMem.SaveDC();

		CBitmap bmMem;
		bmMem.CreateCompatibleBitmap(pDC,cx,cy);

		dcMem.SelectObject(&bmMem);
		dcMem.FillSolidRect(0,0,cx,cy,IM_SPACE_COLOR);

		Graphics g(dcMem.m_hDC);

		int pic_cx = pBitmap->GetWidth();
		int pic_cy = pBitmap->GetHeight();

		float fx  = (float)pic_cx/cx;
		float fy  = (float)pic_cy/(cy-10);

		float f   = ( fx>fy ? fx : fy ) + 0.1;

		if ( f<1.0 )
			f = 1.0;

		float   destWidth  = pic_cx/f;
		float   destHeight = pic_cy/f;

		int   destX  = (cx - destWidth)/2;
		int   destY  = (cy - destHeight)/2;

		g.DrawImage(pBitmap,Rect(destX,destY,destWidth,destHeight));

		COLORREF clrLine = RGB(1,72,100);

		dcMem.SetBkMode(TRANSPARENT);
		dcMem.SetTextColor(RGB(0,83,166));
		dcMem.SelectObject(&m_fontText);

		dcMem.Draw3dRect(destX-1,destY-1,destWidth+2,destHeight+2,clrLine,clrLine);
		dcMem.Draw3dRect(destX,destY,destWidth,destHeight,clrLine,clrLine);

		dcMem.Draw3dRect(0,0,cx,cy,0xAAAAAA,0xFFFFFF);

		if ( m_pFocusPicture==NULL && this->m_iHotPicture!=-1 )
		{
			dcMem.SetTextAlign(TA_CENTER|TA_BOTTOM);

			if ( m_bWaittingFlag )
				dcMem.TextOut(destX+destWidth/2,destY-4,"图片加载中，请等几秒钟 :)");
		}

		pDC->BitBlt(m_PictureRect.left,m_PictureRect.top,cx,cy,&dcMem,0,0,SRCCOPY);

		dcMem.RestoreDC(nSavedMemDC);
	}
	else
	{
		pDC->FillSolidRect(&m_PictureRect,IM_SPACE_COLOR);
		pDC->Draw3dRect(&m_PictureRect,0xAAAAAA,0xFFFFFF);
	}

	if ( pBitmap!=m_pFocusPicture )
		delete pBitmap;
}

void CMainDlg::OnMouseMove(UINT nFlags, CPoint point)
{
	if ( m_ListRect.PtInRect(point) )
	{
		int iHot = HitTest(point);

		if ( iHot!=m_iHotPicture && iHot!=-1 )
		{
			m_iHotPicture = iHot;
			ReDrawList();
			SetTimer(0x01,100,NULL);

			vector<TWebPictureInfo *> & w = m_myPictures.vecPics;
			for ( int i=0; i<w.size(); i++ )
			{
				if ( w[i]->dwPictureID==this->m_vecBitmaps[iHot]->dwPictureID )
				{
					CString strTip;
					strTip.Format("描述:%s\n上传日期:%s\n发表方式:%s",
						w[i]->szTitle,
						w[i]->szCreateDate,
						WebDataFrom2Str(w[i]->dwFrom));
					ShowToolTip(strTip);
					break;
				}
			}

		}
	}
	else
	{
		ShowToolTip("");
	}

	CXSkinDlg::OnMouseMove(nFlags, point);
}

void CMainDlg::OnLButtonDown(UINT nFlags, CPoint point)
{
	CXSkinDlg::OnLButtonDown(nFlags, point);
}

CBitmap * CMainDlg::FindBitmap(uint32 pic_id)
{
	map<uint32,CBitmap *>::iterator  it= m_mapBitmaps.find(pic_id);
	if ( it==m_mapBitmaps.end() )
		return &m_bmpPicPig;
	else
		return it->second;
}

void CMainDlg::AddBitmap(uint32 pic_id, CBitmap *pBitmap)
{
	m_secBitmaps.Lock();

	map<uint32,CBitmap *>::iterator  it= m_mapBitmaps.find(pic_id);
	if ( it==m_mapBitmaps.end() )
		m_mapBitmaps[pic_id]=pBitmap;

	m_secBitmaps.Unlock();
}

void CMainDlg::UpdateButtonState()
{
	int nBitmaps = m_vecBitmaps.size();
	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	if ( m_iPicturePosition>0 )
		m_btnLeft.EnableWindow(TRUE);
	else
		m_btnLeft.EnableWindow(FALSE);

	if ( (m_iPicturePosition+nDrawBitmaps)>=nBitmaps)
		m_btnRight.EnableWindow(FALSE);
	else
		m_btnRight.EnableWindow(TRUE);
}

void CMainDlg::SetLabelPictureNum(int n)
{
	CString s;
	if(m_nDownloadThreadRef>0)
		s.Format("正在同步,%d/%d",n,this->m_myPictures.vecPics.size());
	else
		s.Format("共%d张",n);

	UpdateWindowText(s);
}

void CMainDlg::ShowPicture(int iPicturePosition)
{
	if ( iPicturePosition<0 || iPicturePosition>=m_vecBitmaps.size() )
	{
		return ;
	}

	if ( m_pFocusPicture )
	{
		delete m_pFocusPicture;
		m_pFocusPicture = NULL;
	}

	m_strDescription = m_vecBitmaps[iPicturePosition]->szTitle;
	this->m_edtDescription.SetWindowText(m_strDescription);
	ComboSelViewProp(&m_comboView,m_vecBitmaps[iPicturePosition]->dwViewType);

	m_btnSave.EnableWindow(FALSE);

	//焦点图片
	g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*(m_vecBitmaps[iPicturePosition]),0,this);

	m_nFocusPictureID = m_vecBitmaps[iPicturePosition]->dwPictureID;
	ReDraw();
	
}

void CMainDlg::OnLButtonUp(UINT nFlags, CPoint point)
{
	int iHotPicture = HitTest(point);

	if ( iHotPicture!=-1 )
	{
		if ( !KEYDOWN(VK_CONTROL) )
		{		
			m_vecMulSelPics.clear();
			ShowPicture(iHotPicture);

			m_wndSlider.SetPos(iHotPicture);
		}
		else
		{
			DWORD dwPicID = m_vecBitmaps[iHotPicture]->dwPictureID;
			bool  bSel=false;
			for ( int i=0; i<m_vecMulSelPics.size(); i++ )
			{
				if ( m_vecMulSelPics[i]==dwPicID )
				{
					bSel = true;
					m_vecMulSelPics.erase(m_vecMulSelPics.begin()+i);
					break;
				}
			}

			if ( bSel==false )
			{
				m_vecMulSelPics.push_back(dwPicID);
			}

			if ( m_nFocusPictureID!=-1 && dwPicID!=m_nFocusPictureID)
			{
				bool bSet=false;
				for ( int i=0; i<m_vecMulSelPics.size(); i++ )
				{
					if ( m_vecMulSelPics[i]==m_nFocusPictureID )
					{
						bSet = true;
					}
				}

				if ( bSet==false )
					m_vecMulSelPics.push_back(m_nFocusPictureID);
			}


			ReDrawList();
		}	
	}

	CXSkinDlg::OnLButtonUp(nFlags, point);
}

void CMainDlg::OnBnClickedButtonLeft()
{
	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = m_PictureRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	int nScrolls     = 0;

	int iPrePositon  = m_iPicturePosition;
	int iCurPositon  = m_iPicturePosition-nDrawBitmaps;

	if ( iCurPositon<0 )
		iCurPositon=0;

	nScrolls = iPrePositon-iCurPositon;

	if ( nScrolls>0 )
	{
		while ( nScrolls!=0 )
		{
			m_iPicturePosition--;
			nScrolls--;
			ReDrawList();
			Sleep(20);
		}
	}

	UpdateButtonState();
	m_wndSlider.SetPos(m_iPicturePosition);
}

void CMainDlg::OnBnClickedButtonRight()
{
	//m_iPicturePosition--;
	//if ( m_iPicturePosition<0 )
	//	m_iPicturePosition=0;
	//m_wndSlider.SetPos(m_iPicturePosition);
	//ReDraw();


	int nBitmaps     = m_vecBitmaps.size();
	int nDrawBitmaps = m_PictureRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	int nScrolls     = 0;

	int iPrePositon  = m_iPicturePosition;
	int iCurPositon  = m_iPicturePosition+nDrawBitmaps;

	if ( iCurPositon>=nBitmaps )
		iCurPositon=nBitmaps-nDrawBitmaps;

	nScrolls = iCurPositon-iPrePositon;

	if ( nScrolls>0 )
	{
		while ( nScrolls!=0 )
		{
			m_iPicturePosition++;
			nScrolls--;
			ReDrawList();
			Sleep(20);
		}
	}

	UpdateButtonState();
	m_wndSlider.SetPos(m_iPicturePosition);
}

void CMainDlg::GetRepaintRgn(CRgn &rgn)
{
	CRect rc;
	GetClientRect(&rc);

	rgn.CreateRectRgn(rc.left,55,rc.right,rc.bottom);

	//	RemoveRgnOfWnd(rgn,&m_wndSlider);
	RemoveRgnOfWnd(rgn,&m_btnRight);
	RemoveRgnOfWnd(rgn,&m_btnLeft);
	RemoveRgnOfWnd(rgn,&m_btnBigRight);
	RemoveRgnOfWnd(rgn,&m_btnBigLeft);
	RemoveRgnOfWnd(rgn,&this->m_btnSave);
	RemoveRgnOfWnd(rgn,&this->m_comboView);
	RemoveRgnOfWnd(rgn,&this->m_edtDescription);
	RemoveRgnOfWnd(rgn,GetDlgItem(IDC_STATIC2));
	RemoveRgnOfWnd(rgn,GetDlgItem(IDC_STATIC3));

	CRgn rgn1;
	rgn1.CreateRectRgn(m_PictureRect.left,m_PictureRect.top,m_PictureRect.right,m_PictureRect.bottom);

	rgn.CombineRgn( &rgn, &rgn1, RGN_DIFF);

	CRgn rgn2;
	rgn2.CreateRectRgn(m_ListRect.left,m_ListRect.top,m_ListRect.right,m_ListRect.bottom);

	rgn.CombineRgn( &rgn, &rgn2, RGN_DIFF);
}

int CMainDlg::HitTest(const CPoint & point)
{
	if ( m_ListRect.PtInRect(point) )
	{	
		int cx           =  m_ListRect.Width();
		int cy           =  m_ListRect.Height();

		int nBitmaps     = m_vecBitmaps.size();
		int nDrawBitmaps = cx/(PICTURE_CX+PICTURE_SPACE);

		int nTop         = (cy-PICTURE_CY)/2+1;

		for ( int i=-1; i<(nDrawBitmaps+2); i++ )
		{
			int   iPicture   = m_iPicturePosition+i;

			if ( iPicture>=0 && iPicture<nBitmaps )
			{
				CRect rcPicture  ;

				rcPicture.top  = nTop + m_ListRect.top;
				rcPicture.left = i*(PICTURE_CX+PICTURE_SPACE) + m_ListRect.left;
				rcPicture.right  =  rcPicture.left + PICTURE_CX;
				rcPicture.bottom =  rcPicture.top  + PICTURE_CY;

				if ( rcPicture.PtInRect(point) )
				{
					return iPicture;
				}
			}
		}
	}

	return -1;
}

void CMainDlg::OnClose()
{
	if ( m_bModified )
	{
		g_spaceData.SaveUserWebPictures(m_myPictures);
	}

	DestroyWindow();
}

void CMainDlg::OnNcDestroy()
{	
	CXSkinDlg::OnNcDestroy();

	SafeTerminateThreads();
	g_webDataProcess.SafeTerminate();
	g_webPictureProcess.SafeTerminate();
	ClearBitmaps();
	m_vecMulSelPics.clear();
	delete this;
}

BOOL CMainDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(&rc);
	CRgn rgn;
	rgn.CreateRectRgn(rc.left,rc.top+23,rc.right,rc.bottom);

	RemoveRgnOfWnd(rgn,&m_btnBigRight);
	RemoveRgnOfWnd(rgn,&m_btnBigLeft);
	RemoveRgnOfWnd(rgn,&m_comboView);
	RemoveRgnOfWnd(rgn,&m_wndSlider);


	//CRgn rgn1;
	//rgn1.CreateRectRgn(m_PictureRect.left,m_PictureRect.top,m_PictureRect.right,m_PictureRect.bottom);

	//rgn.CombineRgn( &rgn, &rgn1, RGN_DIFF);
	//
	//CRgn rgn2;
	//rgn2.CreateRectRgn(m_ListRect.left,m_ListRect.top,m_ListRect.right,m_ListRect.bottom);

	//rgn.CombineRgn( &rgn, &rgn2, RGN_DIFF);

	//pDC->SelectObject(&rgn);

	pDC->FillSolidRect(&rc,IM_SPACE_COLOR);

	CWnd *	pWnd	;
	CRect	rc1		;

	pWnd = GetDlgItem(IDC_STATIC_ETCHED1);

	if (pWnd && pWnd->m_hWnd)
	{
		pWnd->GetWindowRect(&rc1);
		ScreenToClient(&rc1);
		pDC->Draw3dRect(&rc1,RGB(118,188,214),0xFFFFFF);
	}

	return FALSE;
}

void CMainDlg::OnPaint()
{
	CPaintDC dc(this); 

	DrawPicture(&dc);
	DrawList(&dc);

	DrawBorder();
	DrawCaptionBar();
}

void CMainDlg::OnCbnSelchangeComboView()
{
	m_bPictureViewModified = true;
	m_btnSave.EnableWindow(TRUE);
}

LRESULT CMainDlg::OnWebPictureNotify(WPARAM wParam, LPARAM lParam)
{	
	switch ( wParam )
	{
	case 0:
		{	
			CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

			if ( m_nFocusPictureID==result->pic_id ) 
			{
				m_strFocusPictureFile = result->szLocalFile;
				KillTimer(0x02);
				SetTimer(0x02,100,NULL);
			}
		}
		break;
	case 1:
		{
			CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

			if ( result->iSize==PICTURE_CX )
			{
				tagThreadParam *p = new tagThreadParam;

				p->pic_id  = result->pic_id;
				p->album_id = result->album_id;
				p->dwCreateDate = result->dwCreateDate;
				strcpy(p->szFile,result->szLocalFile);

				DWORD dwThreadId;
				p->hThread = CreateThread( NULL,
					0,
					(LPTHREAD_START_ROUTINE)LoadBitmapProc,
					(LPVOID)p,
					0,
					&dwThreadId );	

				m_secThreads.Lock();
				m_vecThreads.push_back(p->hThread);
				m_secThreads.Unlock();

				m_nDownloadThreadRef--;

				TWebPictureInfo * wi;
				if ( m_fifoPics.Pop(wi) )
				{
					m_nDownloadThreadRef++;
					g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*wi,PICTURE_CX,this);
				}

				SetLabelPictureNum(m_vecBitmaps.size());
				ReDrawList();
			}
			else
				if ( result->iSize==0 && result->pic_id==m_nFocusPictureID)
				{
					if ( m_pFocusPicture )
					{
						delete m_pFocusPicture;
						m_pFocusPicture = NULL;
					}

					//USES_CONVERSION;
					//m_pFocusPicture = Bitmap::FromFile(A2W(result->szLocalFile));
					////ReDraw();
					tagThreadParam *p = new tagThreadParam;

					p->pic_id  = result->pic_id;
					strcpy(p->szFile,result->szLocalFile);

					DWORD dwThreadId;
					p->hThread = CreateThread( NULL,
						0,
						(LPTHREAD_START_ROUTINE)LoadFocusPictureProc,
						(LPVOID)p,
						0,
						&dwThreadId );	
					m_hLoadFocusPic = p->hThread;

					if ( result->isFromCache==false )//从网站下载
					{
						//::RegUserOperation(0x0098);
					}

					KillTimer(0x02);
				}
		}
		break;
	case 2://download failed
		{
			CWebPictureProcess::tagThreadResult * result = (CWebPictureProcess::tagThreadResult *)lParam;

			if ( result->iSize==PICTURE_CX )
			{
				TWebPictureInfo * wi;
				m_nDownloadThreadRef--;

			//	tagPicture * pic = new tagPicture();
			//	pic->pBitmap = NULL;
			//	pic->pic_id  = result->pic_id;
			//	pic->album_id = result->album_id;
			//	pic->dwCreateDate = result->dwCreateDate;
			//	m_vecBitmaps.push_back(pic);
				AddBitmap(result->pic_id,NULL);
				
				if ( m_fifoPics.Pop(wi) )
				{
					m_nDownloadThreadRef++;
					g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*wi,PICTURE_CX,this);
				}

				SetLabelPictureNum(m_vecBitmaps.size());
				ReDrawList();
			}
		}
		break;
	case 3:
		{
			CWebPictureProcess::tagThreadPictureNotify * notify = (CWebPictureProcess::tagThreadPictureNotify *)lParam;

			if ( m_nFocusPictureID==notify->pic_id )
			{
				DWORD dwSize     = notify->data_len;

				IStream *pStm = NULL;

				//BOOL bResult; 


				//分配全局存储空间 
				HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dwSize ); 
				LPVOID pvData = NULL; 

				if (hGlobal == NULL) 
					return 0; 

				if ((pvData = GlobalLock(hGlobal)) == NULL)//锁定分配内存块 
					return 0; 

				char * pCursor = (char *)pvData;

				for ( int i=0; i<notify->vecData.size(); i++ )
				{
					memcpy(pCursor,notify->vecData[i]->pData,notify->vecData[i]->len);
					pCursor+=notify->vecData[i]->len;
				}

				GlobalUnlock(hGlobal); 

				CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

				if ( m_pFocusPicture )
				{
					delete m_pFocusPicture;
					m_pFocusPicture = NULL;
				}

				m_pFocusPicture = Bitmap::FromStream(pStm);

				ReDrawPicture();

				GlobalFree(hGlobal);
			}
		}
		break;

	}


	return 0;
}

LRESULT CMainDlg::OnWebDataNotify(WPARAM wParam, LPARAM lParam)
{
	CWebDataProcess::tagThreadResult * result = (CWebDataProcess::tagThreadResult *)lParam;

	if ( wParam==0 )
	{        
		vector<TWebPictureInfo *> info;
		CString strServerDate;
		ParaseWebPictures(result->strWebResult,info,strServerDate);

		uint32 fid = atol((LPCSTR)result->strID);		

		m_bUpdatingMyPictures = false;
		SetLabelPictureNum(m_vecBitmaps.size());

		if ( fid==g_pUserInfo->GetUserID() )
		{
			strncpy(m_myPictures.szLastUpdateDate,(LPCSTR)strServerDate,sizeof(m_myPictures.szLastUpdateDate)-1);

			int n = info.size();

			if ( m_fifoPics.size()>0 )
			{
				n--;
				while (n>=0)
				{
					m_fifoPics.push_back(info[n]);
					n--;
				}
			}
			else
			{
				int i,c=0;

				for ( i=0; i<n; i++ )
				{
					int nItem = m_myPictures.FindPicture(info[i]->dwPictureID);
					if ( nItem==-1 )
					{
						m_myPictures.vecPics.push_back(info[i]);

						if ( ++c<5 )
						{
							g_webPictureProcess.GetWebPicture(fid/*info[i].uID*/,*(info[i]),80,this);	
							m_nDownloadThreadRef++;
						}
						else
							m_fifoPics.push_front(info[i]);
					}
					else
					{
						delete m_myPictures.vecPics[nItem];
						m_myPictures.vecPics[nItem]=info[i];					
					}					
				}
			}          

			if (info.size()>0 )
			{
				g_spaceData.SaveUserWebPictures(m_myPictures);
			}

		}

		UpdateAlbumNum();
		ShowAlbum(m_nAlbumID);

		int nBitmaps = m_vecBitmaps.size();
		for ( int i=0; i<nBitmaps; i++ )
		{
			if ( m_vecBitmaps[i]->dwPictureID==this->m_nFocusPictureID )
			{
				ShowPicture(i);
				break;
			}
		}

		if ( m_myPictures.vecPics.size()==0 )
			m_bWaittingFlag = false;
	}

	return 0;
}

void CMainDlg::ShowAlbum(uint32 album_id)
{
	m_vecBitmaps.clear();

	SetDlgItemText(IDC_STATIC_TITLE,GetAlbumName(album_id));

	if ( album_id==0 )
	{
		int n=m_myPictures.vecPics.size();
		for ( int i=0; i<n; i++ )
			m_vecBitmaps.push_back(m_myPictures.vecPics[i]);
	}
	else
	{
		int n=m_myPictures.vecPics.size();

		for ( int i=0; i<n; i++ )
		{
			if (m_myPictures.vecPics[i]->nAlbumId==album_id )
			{
				m_vecBitmaps.push_back(m_myPictures.vecPics[i]);
			}
		}
	}

	sort(m_vecBitmaps.begin(),m_vecBitmaps.end(),TWebPictureInfo::ComparePictureDate);

	static int nPreAlbumId=-1;

	if ( nPreAlbumId!=album_id )
	{
		m_wndSlider.SetRange(0,m_vecBitmaps.size());
		m_wndSlider.SetPos(0);

		m_iPicturePosition = 0;

		nPreAlbumId = album_id;
	}

	UpdateButtonState();

	CString s;
	s.Format("共%d张",m_vecBitmaps.size());
	UpdateWindowText(s);

	ReDraw();
}

void CMainDlg::InitPictures()
{
	g_spaceData.LoadUserWebPictures(g_pUserInfo->GetUserID(),m_myPictures);
	
	m_bWaittingFlag = true;

	int n = m_myPictures.vecPics.size();
	m_nDownloadThreadRef = 0;

	int i;
	for ( i=0; i<5; i++ )
	{
		if ( i<n )
		{
			g_webPictureProcess.GetWebPicture(g_pUserInfo->GetUserID(),*(m_myPictures.vecPics[i]),PICTURE_CX,this);
			m_nDownloadThreadRef++;
		}
	}

	this->m_fifoPics.clear();

	n--;
	while(n>=5)
	{
		m_fifoPics.Push(m_myPictures.vecPics[n]);
		n--;
	}

	m_vecMulSelPics.clear();
	m_bUpdatingMyPictures = true;
	UpdateWindowText("同步中...");
	//	SetLabelPictureNum(0);
	g_webDataProcess.GetWebPictureList(UIntToStr(g_pUserInfo->GetUserID()),m_myPictures.szLastUpdateDate,"",this);   

	DWORD dwThreadID;
	m_hLoadAlbumListProc = CreateThread(NULL,0,
				(LPTHREAD_START_ROUTINE)LoadAlbumListProc,
				NULL,
				0,
				&dwThreadID);
	
	UpdateAlbumNum();
	ShowAlbum(m_nAlbumID);
}

UINT CMainDlg::LoadFocusPictureProc(LPVOID p)
{
	tagThreadParam * pT = (tagThreadParam *)p;

	Bitmap *pBitmap = NULL;

	if ( pInstance )
	{
		USES_CONVERSION;
		pBitmap = Bitmap::FromFile(A2W(pT->szFile));
		if ( pBitmap )
			pInstance->SendMessage(WM_THREAD_NOTIFY,(WPARAM)pBitmap,(LPARAM)pT);
	}

	try
	{
		CloseHandle(pT->hThread);
		delete pT;
	}
	catch(...)
	{
	}

	return 0;
}


UINT CMainDlg::WebPictureExportProc(LPVOID p)
{
	tagExportThreadParam * param = (tagExportThreadParam *)p;

	if ( pInstance )
	{
		int l, i;

		char szCacheFile[255]={0};
		char szLocalFile[255]={0};

		char szTitle[255];
		strcpy(szTitle,pInstance->m_myPictures.vecPics[param->iPictureIndex]->szTitle);
		l = strlen(szTitle);
		for ( i=0; i<l; i++ )
		{
			char c = szTitle[i];
			if ( c=='\\' ||
				c=='/' ||
				c==':' ||
				c=='?' ||
				c=='<' ||
				c=='>' ||
				c=='|' ||
				c=='\"' )
				szTitle[i]='_';
		}

		CString strUrl   = pInstance->m_myPictures.vecPics[param->iPictureIndex]->szUrl;

		strUrl.Trim();
		if ( strUrl.Find("://")<0 )
		{
			strUrl = "http://"+strUrl;
		}

		CString sName = ::GetWebFileName(pInstance->m_myPictures.vecPics[param->iPictureIndex]->szUrl);
		CString s2;
		s2.Format("(%s).",szTitle);
		sName.Replace(".",s2);

		sprintf(szLocalFile,"%s%s",(LPCSTR)pInstance->m_strBackupPath,sName);

		sprintf(szCacheFile,"%s%s",(LPCSTR)g_spaceData.GetUserWebPicturePath(g_pUserInfo->GetUserID()),
			(LPCSTR)GetWebFileName(strUrl));

		if ( (_access(szCacheFile,0)==-1) )
		{
			strUrl.MakeLower();
			strUrl.Replace("/z/","/s/");

			DownLoadWebFile(strUrl,szLocalFile);
		}
		else
		{
			CopyFile(szCacheFile,szLocalFile,FALSE);
		}

		strcpy(param->szCache,szCacheFile);
		strcpy(param->szLocal,szLocalFile);

		pInstance->SendMessage(WM_THREAD_NOTIFY,1,(LPARAM)param);
	}

	delete param;
	return 0;
}

UINT CMainDlg::LoadBitmapProc(LPVOID p)
{
	tagThreadParam *param = (tagThreadParam *)p;

	CBitmap * pBitmap = ::CBitmapFromFile(param->szFile,PICTURE_CX,PICTURE_CY);

	if ( pBitmap )
	{
		pInstance->AddBitmap(param->pic_id,pBitmap);
		pInstance->SendMessage(WM_THREAD_NOTIFY,0,(LPARAM)param);//refresh
	}

	pInstance->m_secThreads.Lock();
	int n = pInstance->m_vecThreads.size();

	for ( int i=0; i<n; i++ )
	{
		if ( param->hThread == pInstance->m_vecThreads[i] )
		{
			pInstance->m_vecThreads.erase(pInstance->m_vecThreads.begin()+i);
			break;
		}
	}
	pInstance->m_secThreads.Unlock();

	try
	{
		CloseHandle(param->hThread);
		delete param;
	}
	catch(...)
	{
	}

	return 0;
}

void CMainDlg::SafeTerminateThreads()
{
	m_secThreads.Lock();
	int n= m_vecThreads.size();
	for ( int i=0; i<n; i++ )
	{
		try
		{
			TerminateThread(m_vecThreads[i],0);
			CloseHandle(m_vecThreads[i]);
		}
		catch(...)
		{
		}
	}
	m_secThreads.Unlock();

	for ( int i=0; i<5; i++ )
	{
		if ( m_hExportThreads[i] )
		{
			try
			{
				TerminateThread(m_hExportThreads[i],0);
				CloseHandle(m_hExportThreads[i]);
			}
			catch(...)
			{
			}
		}
	}

	if ( m_hDeletePicProc )
	{
		try
		{
			TerminateThread(m_hDeletePicProc,0);
			CloseHandle(m_hDeletePicProc);
		}
		catch(...)
		{
		}
		m_hDeletePicProc=NULL;
	}

	if ( m_hLoadAlbumListProc )
	{
		try
		{
			TerminateThread(m_hLoadAlbumListProc,0);
			CloseHandle(m_hLoadAlbumListProc);
		}
		catch(...)
		{
		}
		m_hLoadAlbumListProc=NULL;
	}
}

void CMainDlg::ClearBitmaps()
{
	map<uint32,CBitmap *>::iterator it;
	for ( it=m_mapBitmaps.begin(); it!=m_mapBitmaps.end(); it++ )
	{
		delete it->second;
	}

	m_mapBitmaps.clear();
	//int n = m_vecBitmaps.size();
	//for ( int i=0; i<n; i++ )
	//{
	//	delete m_vecBitmaps[i]->pBitmap;
	//	delete m_vecBitmaps[i];
	//}
	//m_vecBitmaps.clear();
}

int CMainDlg::FindPicturePosition(uint32 pic_id)
{
	int n = m_vecBitmaps.size();
	for ( int i=0; i<n; i++ )
	{
		if ( m_vecBitmaps[i]->dwPictureID==pic_id )
			return i;
	}

	return -1;
}

LRESULT CMainDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	if ( wParam==0 )
	{
		m_wndSlider.SetRange(0,m_vecBitmaps.size());

		if ( m_nFocusPictureID==-1 )
		{
			ShowPicture(0);
		}

		SetLabelPictureNum(m_vecBitmaps.size());

		ReDrawList();
	}
	else
	if ( wParam==1 )
	{
		int nPictures = m_myPictures.vecPics.size();
		tagExportThreadParam * param = (tagExportThreadParam *)lParam;

		int iThreadIndex = param->iThreadIndex;
		CloseHandle(m_hExportThreads[iThreadIndex]);
		m_hExportThreads[iThreadIndex] = NULL;

		if ( m_iExportCursor<nPictures )
		{
			DWORD dwThreadID = 0;
			tagExportThreadParam * pNewParam = new tagExportThreadParam();

			pNewParam->iPictureIndex = m_iExportCursor;
			pNewParam->iThreadIndex  = iThreadIndex;
			pNewParam->hThread  = CreateThread(NULL,0,
				(LPTHREAD_START_ROUTINE)WebPictureExportProc,
				(LPVOID)pNewParam,
				0,
				&dwThreadID);

			m_hExportThreads[iThreadIndex] = pNewParam->hThread ;

			m_iExportCursor++;
		}

		m_iExportedCount++;

		CString str;
		str.Format("正在导出图片(%d/%d)...",m_iExportedCount,nPictures);

		m_pFormProgress3->SetTip(str);

		if ( m_iExportedCount==nPictures )
		{			
			if ( m_pFormProgress3 )
			{
				m_pFormProgress3->DestroyWindow();
				delete m_pFormProgress3;
				m_pFormProgress3 = NULL;
			}

			::ShellExecute(NULL,"explore","","",(LPCSTR)this->m_strBackupPath,SW_SHOWMAXIMIZED);
			UnLockOperation();
		}
	}
	else
	if ( wParam==2)
	{
		if ( m_pFormProgress2 )
		{
			m_pFormProgress2->DestroyWindow();
			delete m_pFormProgress2;
			m_pFormProgress2 = NULL;
		}

		this->UnLockOperation();

		if ( lParam==0 )//delete ok
		{
			int iPos = 0;
			int nBitmaps;
			int nItem    = m_myPictures.FindPicture(m_nFocusPictureID);

			int nSel = 0;
			int nMultiSel = m_vecMulSelPics.size();

			if ( nMultiSel>0 )
			{
				nSel = nMultiSel;

				bool bFocusDel = false;

				for ( int j=0; j<nMultiSel; j++ )
				{
					int nSelItem = m_myPictures.FindPicture(m_vecMulSelPics[j]);

					if ( nSelItem!=-1 )
					{
						delete m_myPictures.vecPics[nSelItem];
						m_myPictures.vecPics.erase(m_myPictures.vecPics.begin()+nSelItem);
					}
				}

				m_vecMulSelPics.clear();

				ShowAlbum(m_nAlbumID);

				 nBitmaps = m_vecBitmaps.size();

				if ( bFocusDel )
				{
					if (m_pFocusPicture)
					{
						delete m_pFocusPicture;
						m_pFocusPicture = NULL;
					}

					ShowPicture(0);
				}
			}
			else
			{
				nSel = 1;

				TWebPictureInfo *pInfo = m_myPictures.vecPics[nItem];
				m_myPictures.vecPics.erase(m_myPictures.vecPics.begin()+nItem);
				delete pInfo;


				ShowAlbum(m_nAlbumID);

				nBitmaps = m_vecBitmaps.size();

				if ( iPos>=nBitmaps)
					iPos=nBitmaps-1;

				if ( iPos<0 )
				{
					if ( m_pFocusPicture )
					{
						delete m_pFocusPicture;
						m_pFocusPicture = NULL;
						ReDraw();
					}
				}
				else
				{
					ShowPicture(iPos);
				}
			}

			SetLabelPictureNum(nBitmaps);
			ReDrawList();

			//while ( nSel-->0 )
			//	::RegUserOperation(0x0099);
			UpdateAlbumNum();

			m_bModified = true;
		}
		else
		{
			ShowMessage((LPCSTR)lParam,MB_OK|MB_ICONINFORMATION);
		}
	}
	else
	if ( wParam==3 )
	{
		for ( int i=0; i<m_myAlbumInfo.vecAlbum.size();i++ )
		{
			m_wndAlbumFolder.AddItem(m_myAlbumInfo.vecAlbum[i].szAlbumName,
				IDB_ALBUM_CUSTOM,m_myAlbumInfo.vecAlbum[i].nAlbumId,
				m_wndAlbumFolder.GetCount()-1);
		}

		UpdateAlbumNum();
	}
	else
	{
		tagThreadParam *pT = (tagThreadParam *)lParam;
		Bitmap *pNewBitmap = (Bitmap *)wParam;;

		if ( pT->pic_id == this->m_nFocusPictureID )
		{
			if ( this->m_pFocusPicture )
			{
				delete m_pFocusPicture;
			}

			m_pFocusPicture = pNewBitmap;
			ReDrawPicture();
		}
		else
		{
			delete pNewBitmap;
		}
	}

	UpdateButtonState();

	return 0;
}
void CMainDlg::OnTimer(UINT nIDEvent)
{
	CPoint pt;
	GetCursorPos(&pt);
	ScreenToClient(&pt);

	switch(nIDEvent)
	{
	case 0x01:
		{
			if ( m_ListRect.PtInRect(pt)==FALSE )
			{
				m_iHotPicture = -1;
				ReDrawList();
				KillTimer(nIDEvent);
			}
		}
		break;
	case 0x02:
		break;
	case 0x03:		
		KillTimer(0x03);
		InitPictures();
		break;
	}


	CXSkinDlg::OnTimer(nIDEvent);
}

void CMainDlg::OnBnClickedButtonLeft2()
{
	int nBitmaps = m_vecBitmaps.size();
	int iPos = this->FindPicturePosition(this->m_nFocusPictureID);
	iPos--;
	if ( iPos<0 )
		iPos=0;

	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	if ( iPos<m_iPicturePosition )
	{
		m_iPicturePosition--;
	}

	if ( nBitmaps>0 )
	{        
		ShowPicture(iPos);
		ReDraw();
	}
}

void CMainDlg::OnBnClickedButtonRight2()
{
	int nBitmaps = m_vecBitmaps.size();
	int iPos = this->FindPicturePosition(this->m_nFocusPictureID);
	iPos++;
	if ( iPos>=nBitmaps )
		iPos=nBitmaps-1;

	int nDrawBitmaps = m_ListRect.Width()/(PICTURE_CX+PICTURE_SPACE);

	if ( iPos>=(m_iPicturePosition+nDrawBitmaps) )
	{
		m_iPicturePosition++;
	}

	if ( nBitmaps>0 )
	{        
		ShowPicture(iPos);
		ReDraw();
	}
}

void CMainDlg::OnEnChangeEditDescription()
{
	m_bPictureTitleModified = true;
	m_btnSave.EnableWindow(TRUE);
}

void CMainDlg::OnBnClickedButtonSave()
{
	if ( this->m_pFocusPicture==NULL )
	{
		this->m_btnSave.EnableWindow(FALSE);
		return ;
	}

	//CZoneAlbum a((LPCSTR)g_pWebConfig->GetServiceUrl(wtZoneAlbum,g_pUserInfo->GetUserName()));
	//ServiceResponse ret;

	CWaitCursor w;

	CString strTitle;
	m_edtDescription.GetWindowText(strTitle);
	strTitle.Trim();
	if( strTitle.GetLength() == 0 )
	{
		ShowMessage("图片描述不能为空,请描述一下你的图片吧");
		return ;
	}

	int iView = ComboGetViewProp(&m_comboView);

	CWebInterfaceWrapper objWI;
		
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());    

	CString              strError;
	bool                 bOk;

	if ( m_bPictureTitleModified )
	{
		//USES_CONVERSION;
	    bOk = objWI.SetPictureTitle(m_nFocusPictureID,strTitle,strError);

		if (bOk==false)
		{
			CString strTip;
			strTip.Format("修改图片描述失败! 我们对此给您造成的不便深表歉意! 失败原因如下:\n<%s>",strError);
			ShowMessage(strTip);
		}
		else
		{
			m_bModified = true;
			m_bPictureTitleModified = false;
		}
	}

	if ( m_bPictureViewModified )
	{	
		bOk = objWI.SetPictureViewProp(m_nFocusPictureID,iView,strError);

		if (bOk==false)
		{
			CString strTip;
			strTip.Format("修改图片访问权限失败! 我们对此给您造成的不便深表歉意! 失败原因如下:\n<%s>",strError);
			ShowMessage(strTip);
		}
		else
		{
			m_bModified = true;
			m_bPictureViewModified = false;
		}
	}

	if ( m_bPictureViewModified==false && m_bPictureTitleModified==false )
	{
		int iPos = m_myPictures.FindPicture(m_nFocusPictureID);
		if ( iPos!=-1 )
		{
			strcpy(m_myPictures.vecPics[iPos]->szTitle,strTitle);
			m_myPictures.vecPics[iPos]->dwViewType = iView;
		}

		m_btnSave.EnableWindow(FALSE);
	}
}

void CMainDlg::OnBnClickedButtonSetting()
{
	::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MyPictures,0);
}

void CMainDlg::OnBnClickedButtonPic()
{
	CWaitCursor wait;
	::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MyPictures,1);
	//RegUserOperation(0x0016);
	//::ShowUploadPicDlg(g_ImUserInfo.username,(LPCSTR)g_pUserInfo->GetNickName(),(char *)(LPCSTR)MakeMD5Password(g_ImUserInfo.password),g_pUserInfo->GetUserID());
	//	CUpLoadPhotoDlg::ShowMain( g_pUserInfo->GetUserID(), g_ImUserInfo.username, "127.0.0.1", (LPCSTR)g_pUserInfo->GetNickName());
}

void CMainDlg::OnBnClickedButtonBigphoto()
{
	if ( g_pHeadPhoto )
		g_pHeadPhoto->Show();
}

void CMainDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 640;//最小宽度
	lpMMI->ptMinTrackSize.y = 350;//最小高度

	//CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}

void CMainDlg::OnBnClickedButtonExport()
{
	TZoneSetting setting;
	g_spaceData.GetZoneSetting(setting);

	if ( _access(setting.szBackupPath,0)==-1 )
	{
		MessageBox("你好像没有设置备份路径或者备份路径不可用！你先设置好备份路径再导出你的图片，谢谢！","温馨提示",MB_OK|MB_ICONINFORMATION);
		::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_MyPictures,0);
		return ;
	}

	int nPictures = m_myPictures.vecPics.size();
	int nThreads  = 5;

	if( nPictures==0 )
	{
		//ShowMessage("你没有图片需要备份呢！");
		MessageBox("你没有图片需要备份呢！","备份图片");
		return ;
	}

	CTime tm=CTime::GetCurrentTime();

	m_strBackupPath.Format("%s%s_的图片(%s)\\",setting.szBackupPath,(LPCSTR)g_pUserInfo->GetUserName(),tm.Format("%Y-%m-%d"));

	CreateDirectory(m_strBackupPath,NULL);
	LockOperation();

	memset(m_hExportThreads,0,sizeof(m_hExportThreads) );
	m_iExportCursor  = 0;
	m_iExportedCount = 0;

	if ( nPictures<5 )
	{
		nThreads = nPictures;
	}

	for ( int i=0; i<nThreads; i++ )
	{
		DWORD dwThreadID = 0;
		tagExportThreadParam * param = new tagExportThreadParam();

		param->iPictureIndex = i;
		param->iThreadIndex  = i;
		param->hThread = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)WebPictureExportProc,
			(LPVOID) param,
			0,
			&dwThreadID);

		m_hExportThreads[i] = param->hThread ;
	}

	m_iExportCursor = nThreads;

	m_pFormProgress3 = new CFormProgress3(m_hWnd,this);
	m_pFormProgress3->ShowWindow(SW_SHOW);
	m_pFormProgress3->CenterWindow(this);

	m_pFormProgress3->SetTip("正在导出,可能需要很长时间,不如喝杯咖啡吧...");
}

LRESULT CMainDlg::OnCancelBackup(WPARAM wParam, LPARAM lParam)
{
	for ( int i=0; i<5; i++ )
	{
		if ( m_hExportThreads[i] )
		{
			try
			{
				TerminateThread(m_hExportThreads[i],0);
				CloseHandle(m_hExportThreads[i]);
			}
			catch(...)
			{
			}
		}
	}

	if ( m_pFormProgress3 )
	{
		m_pFormProgress3->DestroyWindow();
		delete m_pFormProgress3;
		m_pFormProgress3 = NULL;
	}

	UnLockOperation();

	return 0;
}

void CMainDlg::OnBnClickedButtonRefresh()
{
	if ( m_bUpdatingMyPictures==false )
	{
		g_webDataProcess.GetWebPictureList(UIntToStr(g_pUserInfo->GetUserID()),"","",this);
		m_bUpdatingMyPictures = true;
		UpdateWindowText("正在更新...");
	}
}

void CMainDlg::OnBnClickedButtonCopyUrl()
{

	if ( m_nFocusPictureID!=-1 )
	{
		int nItem = m_myPictures.FindPicture(m_nFocusPictureID);

		if ( nItem!=-1 )
		{
			if(OpenClipboard())
			{
				HGLOBAL clipbuffer;
				char * buffer;
				EmptyClipboard();
				clipbuffer = GlobalAlloc(GMEM_DDESHARE, strlen(m_myPictures.vecPics[nItem]->szUrl)+10);
				buffer = (char*)GlobalLock(clipbuffer);
				strcpy(buffer,"http://");
				strcat(buffer,m_myPictures.vecPics[nItem]->szUrl);

				GlobalUnlock(clipbuffer);
				SetClipboardData(CF_TEXT,clipbuffer);
				CloseClipboard();

				CString strTip;
				strTip.Format("图片 %s 的url(%s)已经复制到剪贴板了。",m_myPictures.vecPics[nItem]->szTitle,m_myPictures.vecPics[nItem]->szUrl);

				ShowMessage(strTip,MB_OK|MB_ICONINFORMATION);
			}
		}
	}		

}

void CMainDlg::OnBnClickedButtonDeleteAlbum()
{
}

void CMainDlg::OnBnClickedButtonDeletePic()
{
	if ( m_bUpdatingMyPictures==false )
	{
		if ( m_nFocusPictureID!=-1 )
		{
			int nItem = m_myPictures.FindPicture(m_nFocusPictureID);

			bool CanDel = false;
			CString strTag ;
			CString strPicID = "";

			int nMultiSel = m_vecMulSelPics.size();

			int nSel = 0;

			if ( nMultiSel>0 )
			{
				if ( nMultiSel==1 )
					strTag = "张";
				else
					strTag = "些";

				for ( int i=0; i<nMultiSel-1; i++ )
				{
					strPicID = strPicID+UIntToStr(m_vecMulSelPics[i]);
					strPicID = strPicID+",";
				}

				strPicID = strPicID+UIntToStr(m_vecMulSelPics[nMultiSel-1]);

				CanDel = true;

				nSel = nMultiSel;
			}
			else
			{
				if ( nItem!=-1 )
				{
					strTag = "张";
					CanDel = true;
					strPicID = UIntToStr(m_nFocusPictureID);

					nSel = 1;
				}
			}

			if ( CanDel )
			{
				CString strTip;
				strTip.Format("你真的要删除这%s图片,并且连网站上相应的图片也一起删除掉吗?",strTag);

				if ( MessageBox(strTip,"删除图片",MB_YESNO|MB_ICONQUESTION)==IDYES )
				{
					tagDelPicInfo *pD = new tagDelPicInfo;
					pD->pDlg      = this;
					pD->strPicIDs = strPicID;

					m_pFormProgress2  = new CFormProgress2("正在删除,请稍候...",this);
					m_pFormProgress2->CenterWindow(this);
					m_pFormProgress2->ShowWindow(SW_SHOW);

					LockOperation();

					DWORD dwThreadID;
					m_hDeletePicProc= CreateThread( NULL,
						0,
						(LPTHREAD_START_ROUTINE)DeletePicProc,
						(LPVOID)pD,
						0,
						&dwThreadID );	

				}
			}
		}		
	}
}

void CMainDlg::OnMeasureItem(int nIDCtl, LPMEASUREITEMSTRUCT lpMeasureItemStruct)
{
	CXSkinDlg::OnMeasureItem(nIDCtl, lpMeasureItemStruct);
	CMenuEx::MeasureItem(lpMeasureItemStruct);
}

void CMainDlg::OnRButtonUp(UINT nFlags, CPoint point)
{
	bool bPop  =false;
	int  iFlag = -1;

	if ( m_PictureRect.PtInRect(point) && m_nFocusPictureID!=-1 )
	{
		bPop  = true;
		iFlag = 0;
	}
	else
	{
		int iHotPicture = HitTest(point);

		if ( iHotPicture==m_iHotPicture && m_iHotPicture!=-1 )
		{
			if ( this->m_vecMulSelPics.size()==0 )
				ShowPicture(iHotPicture);

			bPop  = true;
			iFlag = 1;
		}
	}

	if ( bPop )
	{
		CMenuEx menu, *popmenu;

		menu.LoadMenu(IDR_MENU_TEMPLATE);
		popmenu = menu.GetSubMenu(0);
		popmenu->RemoveMenu(0,MF_BYPOSITION);

		CPoint pt;
		GetCursorPos(&pt);
		popmenu->AppendMenu(MF_STRING,ID_SAVEAS,"图片另存为...");
		popmenu->AppendMenu(MF_STRING,ID_DELETEPIC,"删除此图");

		//	if ( m_bUpdatingMyPictures==false )
		popmenu->AppendMenu(MF_STRING,ID_COPYURL,"复制链接到剪贴板");

		if ( iFlag==0 )
		{
			popmenu->AppendMenu(MF_SEPARATOR,0,"");
			popmenu->AppendMenu(MF_STRING,ID_OPEN,"打开");
		}

		if ( this->m_vecMulSelPics.size()>1 )
		{
			if ( iFlag==0 )
				popmenu->EnableMenuItem(ID_OPEN,MF_DISABLED);

			popmenu->EnableMenuItem(ID_SAVEAS,MF_DISABLED);
		}

		popmenu->TrackPopupMenu(TPM_LEFTBUTTON, pt.x, pt.y, this);
	}

	CXSkinDlg::OnRButtonUp(nFlags, point);
}

void CMainDlg::UpdateWindowText(CString s)
{
	CString strTitle;
	strTitle.Format("我的图片 - %s - %s",(LPCSTR)g_pUserInfo->GetNickName(),(LPCSTR)s);
	SetWindowText(strTitle);
}

void CMainDlg::OnPictureOpen()
{
	if ( m_nFocusPictureID!=-1 )
	{
		int nItem = m_myPictures.FindPicture(m_nFocusPictureID);

		if ( nItem!=-1 )
		{
			char * pstrUrl = m_myPictures.vecPics[nItem]->szUrl;
			ShellExecute(NULL,"open",(LPCSTR)GetWebFileName(pstrUrl),"",(LPCSTR)g_spaceData.GetUserWebPicturePath(g_pUserInfo->GetUserID()),SW_SHOWMAXIMIZED);
		}
	}
}

void CMainDlg::OnPictureSaveAs()
{
	if ( m_nFocusPictureID!=-1 )
	{
		int nItem = m_myPictures.FindPicture(m_nFocusPictureID);
		if ( nItem!=-1 )
		{
			char szLocalFile[255];
			char * pstrUrl = m_myPictures.vecPics[nItem]->szUrl;
			sprintf(szLocalFile,"%s%s",(LPCSTR)g_spaceData.GetUserWebPicturePath(g_pUserInfo->GetUserID()),(LPCSTR)GetWebFileName(pstrUrl));


			CFileDialog dlg(FALSE,"",(LPCSTR)GetWebFileName(pstrUrl),OFN_HIDEREADONLY| OFN_OVERWRITEPROMPT, NULL,this );

			if ( dlg.DoModal()==IDOK )
			{
				CopyFile(szLocalFile,dlg.GetPathName(),FALSE);
			}
		}
	}

}
void CMainDlg::OnStnClickedStaticDownload()
{
	//if ( m_bUpdatingMyPictures==false )
	//{
	//	CDateSpanSelectDlg dlg("下载\\更新指定时间段内上传的图片",this);
	//	if ( IDOK==dlg.DoModal() )
	//	{
	//		CString strBegin,strEnd;
	//		dlg.GetDateSpan(strBegin,strEnd);
	//		g_webDataProcess.GetWebPictureList(UIntToStr(g_pUserInfo->GetUserID()),(LPCSTR)strBegin,(LPCSTR)strEnd,this);
	//		UpdateWindowText("正在更新...");
	//		this->m_bUpdatingMyPictures = true;
	//	}
	//}
	//else
	//{
	//	ShowMessage("正在更新你的图片，请稍候再下载...");
	//}
}

BOOL CMainDlg::PreTranslateMessage(MSG* pMsg)
{
	return CXSkinDlg::PreTranslateMessage(pMsg);
}

LRESULT CMainDlg::WindowProc(UINT message, WPARAM wParam, LPARAM lParam)
{
	if  ( message  ==  WM_KEYDOWN  )  
	{  
		switch(  wParam  )  
		{
		case VK_UP:
		case VK_LEFT:
			this->OnBnClickedButtonLeft2();
			break;
		case VK_DOWN:
		case VK_RIGHT:
			this->OnBnClickedButtonRight2();
			break;
		case VK_PRIOR:
			this->OnBnClickedButtonLeft();
			break;
		case VK_NEXT:
			this->OnBnClickedButtonRight();
			break;
		}
	}

	return CXSkinDlg::WindowProc(message,wParam,lParam);
}

UINT CMainDlg::LoadAlbumListProc(LPVOID param)
{		
	if ( pInstance )
	{
		CWebInterfaceWrapper objWI;
		CString				 strError, strTip;
		objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());
		
		objWI.GetUserAlbumList(g_pUserInfo->GetUserID(),pInstance->m_myAlbumInfo);
	
		pInstance->PostMessage(WM_THREAD_NOTIFY,3,0);
	}

	return 0;
}

UINT CMainDlg::DeletePicProc(LPVOID p)
{
	tagDelPicInfo *pD = (tagDelPicInfo *)p;
	
	CWebInterfaceWrapper objWI;
	CString				 strError, strTip;

	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());


	bool  bOperationOk = objWI.DeletePicture(pD->strPicIDs,strError);
	
	if (bOperationOk==false)
	{
		strTip.Format("删除图片失败! 我们对此给您造成的不便深表歉意! 失败原因如下:\n<%s>",strError);
	}
	
	if ( bOperationOk )
		pD->pDlg->SendMessage(WM_THREAD_NOTIFY,2,0);
	else
		pD->pDlg->SendMessage(WM_THREAD_NOTIFY,2,(LPARAM)(LPCSTR)strError);

	return 0;
}
	
void CMainDlg::OnBnClickedAlbumFolder()
{
	int n=m_wndAlbumFolder.GetCurSel();
	if ( n>-1 )
	{
		int nSelAlbumId = m_wndAlbumFolder.GetItemData(n);
		if ( nSelAlbumId!=m_nAlbumID )
		{
			m_nAlbumID = nSelAlbumId;
			ShowAlbum(nSelAlbumId);
			ShowPicture(0);
		}
	}
}