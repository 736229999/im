#include "StdAfx.h"
#include ".\resgdiobj.h"

//////////////////////////////////////////////////////////////////
//static HMODULE g_hResourceDLL,资源DLL
//创建者:xuexiaofeng
//说明:
//////////////////////////////////////////////////////////////////
static HMODULE g_hResourceDLL = NULL;

HINSTANCE   GetResDLLHandle()
{
	return g_hResourceDLL;
}

bool InitResourceDLL(CString strDLLFile)
{
	if ( g_hResourceDLL==NULL )
	{
		g_hResourceDLL = LoadLibrary((LPCSTR)strDLLFile);

		if(g_hResourceDLL==NULL)
			return false;
	}

	return true;
}

void UnInitResourceDLL()
{
	if (g_hResourceDLL)
		FreeLibrary(g_hResourceDLL);

	g_hResourceDLL=NULL;
}

CString ResLoadHtmlResource(UINT nIdResource)
{
	CString s;
	HGLOBAL h=::LoadResource(g_hResourceDLL,::FindResource(g_hResourceDLL,MAKEINTRESOURCE(nIdResource),RT_HTML));
	LPVOID p = GlobalLock(h);
	s = (char *)p;
	GlobalUnlock(h);
	return s;
}

HCURSOR  ResLoadCursor(UINT nIdResource)
{
	return ::LoadCursor(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}

HBITMAP  ResLoadBitmap(UINT nIdResource)
{
	return ::LoadBitmap(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}

HICON ResLoadIcon(UINT nIdResource)
{
	return ::LoadIcon(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}

CResBitmap::CResBitmap()
{
	m_hResObj = NULL;
}

CResBitmap::~CResBitmap()
{
	if ( m_hResObj )
	{
		::DeleteObject(m_hResObj);
		__super::Detach();
	}
}

BOOL CResBitmap::Attach(HGDIOBJ hObject)
{
	if ( m_hResObj )
	{		
		__super::Detach();

		::DeleteObject(m_hResObj);
		m_hResObj = NULL;
	}

	return __super::Attach(hObject);
}

HGDIOBJ CResBitmap::Detach()
{
	if ( m_hResObj )
	{
		m_hResObj = NULL;
	}

	return __super::Detach();
}

BOOL CResBitmap::LoadBitmap(UINT nIDResource)
{
	if ( m_hResObj )
	{
		__super::Detach();
	}

	m_hResObj = (HGDIOBJ)::ResLoadBitmap(nIDResource);

	if ( m_hResObj )
	{
		__super::Attach(m_hResObj);
	}

	return TRUE;
}


//CResBitmap::operator CBitmap *() const
//{
//	return (CBitmap *)this;
//}