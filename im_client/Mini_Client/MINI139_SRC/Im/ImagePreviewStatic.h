/*
 *	$Header: $
 *
 *	$History: $
 */
#pragma once

// CImagePrieviewStatic
class CImagePreviewStatic :	public CStatic
{
	DECLARE_DYNAMIC(CImagePreviewStatic)
public:
					CImagePreviewStatic();
	virtual			~CImagePreviewStatic();

	virtual	BOOL	Create();
	virtual	void	DrawItem(LPDRAWITEMSTRUCT lpDrawItemStruct);

	void			SetFilename(LPCTSTR	szFilename);
	void            GetImageSize(SIZE &sz);
	DWORD           GetImageFileSize() { return m_dwFileSize; }

protected:
	WCHAR			m_wsFilename[_MAX_PATH];
	Image			*m_img;
	//Graphics		*m_graphics;

	DWORD           m_dwFileSize ;

	DECLARE_MESSAGE_MAP()
};


