#include "stdafx.h"
#include "RoundFaceCtrl.h"
#include "../../HmgSkin/HmgSkin/FlatPopupMenu.h"

IMPLEMENT_DYNAMIC(CRountFaceCtrl, CStatic)
CRountFaceCtrl::CRountFaceCtrl(UINT idRountNormal, UINT idRoundSel, LPCTSTR szFacePath /*= NULL*/)
{
	LoadImageFromResource(&m_imgRoundNormal, AfxGetResourceHandle(), idRountNormal, "PNG");
	LoadImageFromResource(&m_imgRoundSel, AfxGetResourceHandle(), idRoundSel, "PNG");

	m_nSize = m_imgRoundNormal.GetWidth();

	if(szFacePath == NULL)
	{
		LoadImageFromResource(&m_imgFace, AfxGetResourceHandle(), IDB_ICON_ON, "PNG");		
	}
	else
	{
		SetPicPath(szFacePath);
	}

	m_bSel = FALSE;
	m_bTracking = FALSE;
	m_bSaveBackground = FALSE;
	m_bHintFace = FALSE;
	m_strOldHeadFile = "";

	m_hCursor    = ::LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND));
}

CRountFaceCtrl::~CRountFaceCtrl()
{
	if(!m_imgRoundNormal.IsNull())
		m_imgRoundNormal.Destroy();
	if(!m_imgRoundSel.IsNull())
		m_imgRoundSel.Destroy();
	if(!m_imgFace.IsNull())
		m_imgFace.Destroy();
	m_bitmap.Detach();
}


BEGIN_MESSAGE_MAP(CRountFaceCtrl, CStatic)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_MESSAGE(WM_MOUSELEAVE,OnMouseLeave)
	ON_WM_ERASEBKGND()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()

// CFaceIconCtrl 消息处理程序
void CRountFaceCtrl::OnPaint()
{
	CPaintDC dc(this); 

	CRect rcClient;
	GetClientRect(&rcClient);
	
	CDC dcMem;
	dcMem.CreateCompatibleDC(&dc);
	CBitmap *pOldBitmap = dcMem.SelectObject(&m_bitmap);

	// 先绘制圆形头像
	CRgn rgn;
	rgn.CreateEllipticRgn(0, 0, m_nSize, m_nSize);
	dcMem.SelectClipRgn(&rgn);
	rgn.DeleteObject();
	int OldMode = dcMem.SetStretchBltMode(HALFTONE);

	if(!m_imgFace.IsNull())
	{
		// 绘制背景色
		dcMem.FillSolidRect(1, 1, m_nSize - 2, m_nSize - 2, RGB(211, 232, 245));
		m_imgFace.Draw(dcMem, 1, 1, m_nSize - 2, m_nSize - 2);
	}

	// 后绘制圆形边框
	rgn.CreateRectRgn(0, 0, m_nSize, m_nSize);
	dcMem.SelectClipRgn(&rgn);
	rgn.DeleteObject();

	if(m_bSel || m_bHintFace)
	{	// 选中
		int nW = m_imgRoundSel.GetWidth();
		int nH = m_imgRoundSel.GetHeight();
		if(!m_imgRoundSel.IsNull())
		{
			m_imgRoundSel.Draw(dcMem, 0, 0, nW, nH);
		}
	}
	else
	{	// 未选中
		int nW = m_imgRoundNormal.GetWidth();
		int nH = m_imgRoundNormal.GetHeight();
		if(!m_imgRoundNormal.IsNull())
		{
			m_imgRoundNormal.Draw(dcMem, 0, 0, nW, nH);
		}
	}

	dc.BitBlt(0,0,rcClient.Width(),rcClient.Height(),&dcMem,0,0,SRCCOPY);
	dcMem.SelectObject(pOldBitmap);
	dcMem.SetStretchBltMode(OldMode); 
	
	dcMem.DeleteDC();
}

void CRountFaceCtrl::PreSubclassWindow()
{
	CStatic::PreSubclassWindow();
	ModifyStyle(0, SS_NOTIFY, TRUE);

	// 根据图片大小改变控件大小
	CRect rc;
	GetWindowRect(&rc);
	GetParent()->ScreenToClient(&rc);
	MoveWindow(rc.top, rc.left, m_nSize, m_nSize);

	m_TMS.cbSize = sizeof(TRACKMOUSEEVENT);
	m_TMS.dwFlags = TME_HOVER | TME_LEAVE;
	m_TMS.hwndTrack = GetSafeHwnd();
	m_TMS.dwHoverTime = 10;
	TrackMouseEvent(&m_TMS); 
}

void CRountFaceCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonDown(nFlags, point);
}

void CRountFaceCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	CStatic::OnLButtonUp(nFlags, point);
}

void CRountFaceCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	if(m_bSel)
		return;

	CStatic::OnMouseMove(nFlags, point);
	m_bSel = TRUE;
	Invalidate();

	if(!m_bTracking && !m_bHintFace)
	{
		m_TMS.hwndTrack = GetSafeHwnd();
		m_bTracking = TrackMouseEvent(&m_TMS);
	}
}

BOOL CRountFaceCtrl::OnEraseBkgnd(CDC* pDC)
{
	CRect rc;
	GetWindowRect(rc);
	CWnd* pParent = GetParent(); 
	pParent->ScreenToClient(rc);

	if(!m_bSaveBackground)
	{
		// 截取子窗口所掩盖的父窗口区域，刷新背景
		CPaintDC dc(pParent);

		m_bitmap.DeleteObject();
		m_bitmap.CreateCompatibleBitmap(&dc, rc.Width(), rc.Height());
		CDC memDC;
		memDC.CreateCompatibleDC(&dc);

		CBitmap *oldmemBitmap = memDC.SelectObject(&m_bitmap);
		memDC.BitBlt(0, 0, rc.Width(),rc.Height(), &dc, rc.left, rc.top, SRCCOPY);  
		memDC.SelectObject(oldmemBitmap);
		
		m_bSaveBackground = TRUE;
		memDC.DeleteDC();
	}

	CDC memDC;
	memDC.CreateCompatibleDC(pDC);
	CBitmap *pOldBitmap = memDC.SelectObject(&m_bitmap);
	pDC->BitBlt(0, 0, rc.Width(),rc.Height(), &memDC, 0, 0, SRCCOPY);  
	memDC.SelectObject(pOldBitmap);
	memDC.DeleteDC();

	return FALSE;
}

// 创建
BOOL CRountFaceCtrl::Create(CWnd *pParent, UINT id)
{
	CRect rc(0, 0, m_nSize, m_nSize);
	return CStatic::Create("", WS_CHILD | WS_VISIBLE, rc, pParent, id);
}

LRESULT CRountFaceCtrl::OnMouseLeave(WPARAM wParam,LPARAM lParam)
{
	m_bTracking = FALSE;
	m_bSel = FALSE;
	Invalidate();

	return 0;
}

// 转换为灰度图
void CRountFaceCtrl::TurnGray()
{
	if(m_imgFace.IsNull())  
		return;  
	if(!m_imgFace.IsIndexed())  
	{  
		//直接修改像素颜色  
		COLORREF pixel;  
		int maxY=m_imgFace.GetHeight();  
		int maxX=m_imgFace.GetWidth();  
		byte r,g,b,avg;  
		for(int x=0;x<maxX;x++)  
		{  
			for(int y=0;y<maxY;y++)  
			{  
				pixel=m_imgFace.GetPixel(x,y);  
				r=GetRValue(pixel);  
				g=GetGValue(pixel);  
				b=GetBValue(pixel);  
				avg=(int)(((int)r+g+b)/3);  
				m_imgFace.SetPixelRGB(x,y,avg,avg,avg);  
			}         
		}  
	}  
	else  
	{  
		//获取并修改颜色表  
		int maxColors=m_imgFace.GetMaxColorTableEntries();  
		RGBQUAD* lpColorTable;  
		lpColorTable=new RGBQUAD[maxColors];  
		m_imgFace.GetColorTable(0,maxColors,lpColorTable);  
		for(int i=0;i<maxColors;i++)  
		{  
			int avg=(lpColorTable[i].rgbRed+lpColorTable[i].rgbGreen+lpColorTable[i].rgbBlue)/3;  
			lpColorTable[i].rgbRed=avg;  
			lpColorTable[i].rgbGreen=avg;  
			lpColorTable[i].rgbBlue=avg;  
		}  
		m_imgFace.SetColorTable(0,maxColors,lpColorTable);  
		delete(lpColorTable);  
	}  
}

// 设置图片
void CRountFaceCtrl::SetPicPath(LPCTSTR szPicPath, BOOL bOnline /*= TRUE*/)
{
	if(!FileExist(szPicPath))
		return;

	if(!m_imgFace.IsNull())
	{
		m_imgFace.Destroy();
	}



	m_imgFace.Load(szPicPath);
	if (!m_imgFace.IsNull() && m_imgFace.GetBPP() == 32) //确认该图像包含Alpha通道
	{
		for (int i = 0; i < m_imgFace.GetWidth(); i++)
		{
			for (int j = 0; j < m_imgFace.GetHeight(); j++)
			{
				byte *pByte = (byte *)m_imgFace.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	if(!bOnline)
	{	// 转换为灰度图
		TurnGray();
	}

	// 刷新
	if(::IsWindow(GetSafeHwnd()))
	{
		Invalidate();
	}
}

void CRountFaceCtrl::SetBitmap(CBitmap* pBitmap, CString strFilePath)
{
	if ( FileExist(m_strOldHeadFile) )
		DeleteFile(m_strOldHeadFile);
	if(!m_imgFace.IsNull())
	{
		m_imgFace.Destroy();
	}
	HBITMAP hbmp = (HBITMAP)pBitmap->GetSafeHandle();
	m_imgFace.Attach(hbmp);
	m_imgFace.Save(strFilePath);
	if(!m_imgFace.IsNull())
	{
		m_imgFace.Destroy();
	}
	/*if(_access(strFilePath, 0) == -1)
	{
		MessageBox(strFilePath);
	}*/
	
	m_imgFace.Load(strFilePath);
	if (!m_imgFace.IsNull() && m_imgFace.GetBPP() == 32) //确认该图像包含Alpha通道
	{
		for (int i = 0; i < m_imgFace.GetWidth(); i++)
		{
			for (int j = 0; j < m_imgFace.GetHeight(); j++)
			{
				byte *pByte = (byte *)m_imgFace.GetPixelAddress(i, j);
				pByte[0] = pByte[0] * pByte[3] / 255;
				pByte[1] = pByte[1] * pByte[3] / 255;
				pByte[2] = pByte[2] * pByte[3] / 255;
			}
		}
	}

	if(::IsWindow(GetSafeHwnd()))
	{
		Invalidate();
	}

	pBitmap->DeleteObject();
	m_strOldHeadFile = strFilePath;
}

BOOL CRountFaceCtrl::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	::SetCursor(m_hCursor);
	return FALSE;
}


void CRountFaceCtrl::SetBitmapHint(BOOL bStatic)
{
	m_bHintFace = TRUE;
}
void CRountFaceCtrl::SetSaveBackground(BOOL bSaveBackground)
{
	m_bSaveBackground = bSaveBackground;
}
