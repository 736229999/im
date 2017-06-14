/************************************************************************ 
* 
* 文件名：PreviewDlg.cpp 
* 
* 文件描述：图片文件放大预鉴操作集 
* 
* 创建人： LWL， 2005年12月9日 
* 
* 版本号：1.0 
* 
* 修改记录： 
* 
************************************************************************/ 

#include "..\stdafx.h"
//#include "UpLoadPhoto.h"
#include "PreviewDlg.h"
#include ".\previewdlg.h"
#include "uploadphotodlg.h"
//#include "memdc.h"


// CPreviewDlg 对话框
CPreviewDlg * CPreviewDlg::pInstance = NULL;


IMPLEMENT_DYNAMIC(CPreviewDlg, CXSkinDlg)
CPreviewDlg::CPreviewDlg(CWnd* pParent /*=NULL*/)
: CXSkinDlg(CPreviewDlg::IDD, pParent)
{
	m_pBitmap			  = NULL;
	Create(IDD,pParent);
}

CPreviewDlg::~CPreviewDlg()
{
	if ( m_pBitmap )
		delete m_pBitmap;
	m_pBitmap = NULL;

	pInstance = NULL;
}

void CPreviewDlg::Show(const CString & strFile)
{
	if ( pInstance==NULL )
	{
		pInstance = new CPreviewDlg(GetDesktopWindow());
		pInstance->SetImageFile(strFile);
		
		pInstance ->SetWindowPos(&wndTopMost, 0, 0, 0, 0, SWP_NOSIZE | SWP_NOMOVE | SWP_SHOWWINDOW);
		pInstance->ShowWindow(SW_SHOW);
	}
	else
	{
		if ( pInstance->IsWindowVisible()==FALSE)
			pInstance->ShowWindow(SW_SHOW);

		if ( pInstance->IsIconic() )
			pInstance->ShowWindow(SW_RESTORE);

		pInstance->SetImageFile(strFile);
		pInstance->SetActiveWindow();
	}
}

void CPreviewDlg::UnShow()
{
	if ( pInstance )
	{

		pInstance->DestroyWindow();
		delete pInstance;
		pInstance=NULL;
	}
}

void CPreviewDlg::SetImageFile(const CString & strFile)
{
	Bitmap * bitmap = Bitmap::FromFile(strFile.AllocSysString());

	if ( m_pBitmap )		delete m_pBitmap;	m_pBitmap = NULL;

	if ( bitmap )
	{
		float cx = (float)bitmap->GetWidth();
		float cy = (float)bitmap->GetHeight();

		m_pBitmap = new CBitmap();
		CWnd * pWnd = CWnd::GetDesktopWindow();
		CDC *pDC = pWnd->GetDC();

		CDC dcTemp;
		dcTemp.CreateCompatibleDC(pDC);
		m_pBitmap->CreateCompatibleBitmap(pDC,(int)cx,(int)cy);

		CBitmap *pOldbmp = dcTemp.SelectObject(m_pBitmap);

		dcTemp.FillSolidRect(0, 0, (int)cx, (int)cy, 0xffffff);

		Graphics g(dcTemp.m_hDC);

		Unit  units;
		int x=0, y=0;

		RectF destRect(REAL(x),	REAL(y), REAL(cx), REAL(cy)),	srcRect;

		bitmap->GetBounds(&srcRect, &units);

		g.DrawImage( bitmap, Rect(0, 0, (int)cx, (int)cy) );
		pWnd->ReleaseDC(pDC);

		delete bitmap;
	}

	if ( m_hWnd )
	{
		Invalidate();
	}
}

void CPreviewDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CPreviewDlg, CXSkinDlg)
	ON_WM_CREATE()
	ON_WM_SIZE()
	ON_BN_CLICKED(IDC_CANCEL, OnBnClickedCancel)
	ON_WM_CLOSE()
	ON_WM_NCDESTROY()
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	ON_WM_GETMINMAXINFO()
END_MESSAGE_MAP()


// CPreviewDlg 消息处理程序

int CPreviewDlg::OnCreate( LPCREATESTRUCT lpCreateStruct ) 
{
	if( CXSkinDlg::OnCreate( lpCreateStruct ) == -1 )
	{
		return -1;
	}

	return 0;
}
//緢制图片
void CPreviewDlg::OnDrawItem( int nIDCtl, LPDRAWITEMSTRUCT lpDrawItemStruct ) 
{
	/*if( lpDrawItemStruct->itemAction & ODA_DRAWENTIRE )
	{
		CMemDC *pMemDC = NULL;
		pMemDC = new CMemDC( lpDrawItemStruct->hDC );
		lpDrawItemStruct->hDC = pMemDC->m_hDC;

		CRect rect;
		GetClientRect( rect );
		CRect newRect(rect.left, rect.top, rect.right,rect.bottom);
		HBRUSH hBrush = ::CreateSolidBrush( RGB(255, 255, 255) );

		::FillRect( lpDrawItemStruct->hDC, newRect, hBrush );

		DeleteObject( hBrush );

		if( m_pSelectedImage != NULL )
		{
			Graphics graphics( lpDrawItemStruct->hDC );
			graphics.SetInterpolationMode(InterpolationModeHighQualityBicubic);
			graphics.DrawImage( m_pSelectedImage,	
				Rect( lpDrawItemStruct->rcItem.left, 
				lpDrawItemStruct->rcItem.top,
				lpDrawItemStruct->rcItem.right - lpDrawItemStruct->rcItem.left,
				lpDrawItemStruct->rcItem.bottom - lpDrawItemStruct->rcItem.top)); 
		}

		delete pMemDC;
	}*/
}

//计算图片在窗口中的缩放比例。
void CPreviewDlg::OnSize( UINT nType, int cx, int cy ) 
{
	CXSkinDlg::OnSize(nType, cx, cy);

	CWnd *pWnd=GetDlgItem(IDC_CANCEL);
	if (pWnd == NULL)		return;

//	pWnd->MoveWindow((cx - 75) / 2 - , cy - 25, 75, 22);
	
	try
	{
		CRect rc;
		pWnd ->GetWindowRect(&rc);

		int x = cx / 2 - ( rc.Width() / 2);
		int y = cy - rc.Height() - 5;
		pWnd ->SetWindowPos(NULL, x, y, 0, 0, SWP_NOSIZE);

		CDC *pDC = GetDC();
		DrawPic(pDC);
		ReleaseDC(pDC);

		CRgn rgn;
		GetRepaintRgn(rgn);
		InvalidateRgn(&rgn);
	}
	catch ( ... )
	{
	}
}

void CPreviewDlg::OnCancel()
{
	DestroyWindow();
}

BOOL CPreviewDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

    SetWindowText("预览图片");

	//{
	//	CRect rc;
	//	GetWindowRect(&rc);

	//	int x = rc.Width();
	//	int y = rc.Height();
	//}


	return TRUE;
}

void CPreviewDlg::OnBnClickedCancel()
{
	DestroyWindow();
}

void CPreviewDlg::OnClose()
{
	DestroyWindow();
}

void CPreviewDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();

	delete this;
}

void CPreviewDlg::OnPaint()
{
	CPaintDC dc(this); 

#ifdef _USE_XSKING_DLG
	CXSkinDlg::OnPaint();

	DrawCaptionBar();
	DrawBorder();

	DrawPic(&dc);
#endif
}

void CPreviewDlg::DrawPic(CDC *pDC)
{
	if ( m_pBitmap )
	{
		Bitmap *bitmap = Bitmap::FromHBITMAP((HBITMAP)m_pBitmap->m_hObject, NULL);
		Graphics g(pDC ->m_hDC);

		CRect rc = GetPicRect();
		g.DrawImage(bitmap, rc.left, rc.top, rc.right, rc.bottom);

		delete bitmap;
	}
}

BOOL CPreviewDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetClientRect(rc);

	CRgn rgn;
	GetRepaintRgn(rgn);

	pDC ->SelectClipRgn(&rgn);

	pDC->FillSolidRect(rc, 0xFFFFFF);

	return FALSE;
}

void CPreviewDlg::GetRepaintRgn(CRgn & rgn)
{
	CRect rc;
	GetClientRect(rc);
	rgn.CreateRectRgn(rc.left, rc.top, rc.right, rc.bottom);

	CRgn rgnpic;
	CRect rcpic = GetPicRect();
	rgnpic.CreateRectRgn(rcpic.left, rcpic.top, rcpic.right, rcpic.bottom);

	rgn.CombineRgn(&rgn, &rgnpic, RGN_DIFF);

	CRect rc_;
	CWnd *pWnd = GetDlgItem(IDC_CANCEL);
	if(pWnd)
	{
		pWnd ->GetWindowRect(&rc);	ScreenToClient(&rc);
		CRgn rgn_;
		rgn_.CreateRectRgn(rc_.left, rc_.top, rc_.right, rc_.bottom);
		rgn.CombineRgn(&rgn, &rgnpic, RGN_DIFF);
	}
}

CRect CPreviewDlg::GetPicRect()
{
	CRect rc;
	GetClientRect(rc);

	rc.top += 22;
	rc.bottom -= 40;

	BITMAP bi;
	m_pBitmap->GetBitmap(&bi);

	int sourceWidth  = bi.bmWidth;
	int sourceHeight = bi.bmHeight;

	int destX = 0,		destY = 0; 

	float nPercent  = 0;
	float nPercentW = ((float)rc.Width()/(float)sourceWidth);
	float nPercentH = ((float)rc.Height()/(float)sourceHeight);

	if(nPercentH < nPercentW)
	{
		nPercent = nPercentH;
		destX    = (int)((rc.Width() - (sourceWidth * nPercent))/2);
	}
	else
	{
		nPercent = nPercentW;
		destY    = (int)((rc.Height() - (sourceHeight * nPercent))/2);
	}

	int destWidth  = (int)(sourceWidth * nPercent);
	int destHeight = (int)(sourceHeight * nPercent);

	CRect rc_(destX + 0,destY + 40,destWidth,destHeight - 15);
	return rc_;
}


void CPreviewDlg::OnGetMinMaxInfo(MINMAXINFO* lpMMI)
{
	lpMMI->ptMinTrackSize.x = 500;
	lpMMI->ptMinTrackSize.y = 406;

	CXSkinDlg::OnGetMinMaxInfo(lpMMI);
}
