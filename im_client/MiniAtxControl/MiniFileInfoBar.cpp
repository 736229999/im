// MiniFileInfoBar.cpp : CMiniFileInfoBar 的实现
#include "stdafx.h"
#include "MiniFileInfoBar.h"
#include ".\minifileinfobar.h"


// CMiniFileInfoBar

STDMETHODIMP CMiniFileInfoBar::SetFileInfo(BSTR strFileName, LONG nFileSize)
{
	HWND hwndDesktop    = GetDesktopWindow();
	HDC  hdcDesktop		= ::GetDC(hwndDesktop);

	TCHAR temp[255];
	::GetTempPath(sizeof(temp)/sizeof(TCHAR),temp);

	CreateDirectory(temp,NULL);

	wcscat_s(temp,sizeof(temp),strFileName);

	if ( m_hBitmap )
	{
		DeleteObject(m_hBitmap);
		m_hBitmap=NULL;
	}

	if ( hdcDesktop )
	{
		TCHAR szText[255];

		HANDLE hFile = CreateFile(temp,FILE_ALL_ACCESS,FILE_SHARE_WRITE,NULL,CREATE_ALWAYS,FILE_FLAG_OVERLAPPED,NULL);
		if ( hFile!=INVALID_HANDLE_VALUE )
		{
			CloseHandle(hFile);
		}

		SHFILEINFO sfi;
		memset(&sfi,0,sizeof(SHFILEINFO));
		HIMAGELIST hImageList =(HIMAGELIST)SHGetFileInfo(temp,FILE_ATTRIBUTE_NORMAL,&sfi,sizeof(sfi),SHGFI_SYSICONINDEX|SHGFI_LARGEICON);


		HDC hdcTemp = CreateCompatibleDC(hdcDesktop);
		int nSaved  = SaveDC(hdcTemp);

		m_hBitmap = CreateCompatibleBitmap(hdcDesktop,32,32);

		SelectObject(hdcTemp,m_hBitmap);

		HBRUSH hbr = CreateSolidBrush(0xFFFFFF);
		RECT rect={0,0,32,32,};
		FillRect(hdcTemp,&rect,hbr);

		if ( hImageList )
		{
			//::ImageList_Draw()
			ImageList_Draw(hImageList,sfi.iIcon,hdcTemp,0,0,0);

		}

		DeleteObject(hbr);
		RestoreDC(hdcTemp,nSaved);

		if (nFileSize<1024)
		{
			swprintf_s(szText,sizeof(szText),TEXT("%s (%d 字节)"),strFileName,nFileSize);
		}
		else
		{
			nFileSize = nFileSize/1024;

			if (nFileSize<8192)
				swprintf_s(szText,sizeof(szText),TEXT("%s (%d KB)"),strFileName,nFileSize);
			else
				swprintf_s(szText,sizeof(szText),TEXT("%s (%.2f MB)"),strFileName,(float)nFileSize/1024.0);
		}

		wcsncpy_s(m_szInfo,sizeof(m_szInfo),szText,sizeof(m_szInfo)/sizeof(TCHAR));

		SIZE sz1;
		GetTextExtentPoint(hdcDesktop,szText,wcslen(szText),&sz1);

		SIZE sz2;
		sz2.cy=(40+sz1.cy)*26.4;
		sz2.cx=(10+sz1.cx)*26.4;

		SetExtent(DVASPECT_CONTENT,&sz2);

		::ReleaseDC(hwndDesktop,hdcDesktop);

		DeleteFile(temp);

		FireViewChange();
	}

	return S_OK;
}

STDMETHODIMP CMiniFileInfoBar::get_OwnerData(ULONG* pVal)
{
	// TODO: 在此添加实现代码
	*pVal = m_nOwnerData;

	return S_OK;
}

STDMETHODIMP CMiniFileInfoBar::put_OwnerData(ULONG newVal)
{
	// TODO: 在此添加实现代码
	m_nOwnerData = newVal;

	return S_OK;
}