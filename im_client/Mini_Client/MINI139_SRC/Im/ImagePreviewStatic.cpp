/*
 *	$Header: $
 *
 *	$History: $
 */
#include "stdafx.h"
#include "ImagePreviewStatic.h"


// CImagePrieviewStatic
IMPLEMENT_DYNAMIC(CImagePreviewStatic, CStatic)

CImagePreviewStatic::CImagePreviewStatic() : CStatic()
{
	m_img = (Image *) NULL;
//	m_graphics = (Graphics *) NULL;
}

CImagePreviewStatic::~CImagePreviewStatic()
{
	delete m_img;
	//delete m_graphics;
}

BOOL CImagePreviewStatic::Create()
{
	if (GetSafeHwnd() != HWND(NULL))
	{
		m_img =	new	Image(m_wsFilename);
		return TRUE;
	}

	return FALSE;
}

void CImagePreviewStatic::SetFilename(LPCTSTR szFilename)
{
#ifndef	_UNICODE
	USES_CONVERSION;
#endif

	ASSERT(szFilename);
	ASSERT(AfxIsValidString(szFilename));

	TRACE("%s\n", szFilename);

#ifndef	_UNICODE
	wcscpy(m_wsFilename, A2W(szFilename));
#else
	wcscpy(m_wsFilename, szFilename);
#endif
	CFile file;
	m_dwFileSize=0;

	if (file.Open(szFilename,CFile::typeBinary|CFile::modeRead) )
	{
		m_dwFileSize = file.GetLength();
		file.Close();
	}

	delete m_img;
	m_img =	new	Image(m_wsFilename,	FALSE);

	GetParent()->Invalidate();
	//Invalidate(1);
}

void CImagePreviewStatic::GetImageSize(SIZE &sz)
{
	sz.cx=0;
	sz.cy=0;

	if ( m_img )
	{
		sz.cx=m_img->GetWidth();
		sz.cy=m_img->GetHeight();
	}
}

void CImagePreviewStatic::DrawItem(LPDRAWITEMSTRUCT	lpDrawItemStruct)
{
	Unit  units;
	CRect rect;

	if (m_img != NULL)
	{
		GetClientRect(&rect);
	
		RectF destRect(REAL(rect.left),	REAL(rect.top),	REAL(rect.Width()),	REAL(rect.Height())),
			  srcRect;
		m_img->GetBounds(&srcRect, &units);

		Graphics *graphics = new Graphics(lpDrawItemStruct->hDC);
		graphics->DrawImage(m_img, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);
		
		delete graphics;
	}
}

BEGIN_MESSAGE_MAP(CImagePreviewStatic, CStatic)
END_MESSAGE_MAP()

// CImagePrieviewStatic message handlers
