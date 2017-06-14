#include "StdAfx.h"
#include "..\HmgSkinRes\resource.h"
#include ".\hmgskinpublic.h"
#include "SkinBitmap.h"
#include "XSkinDlg.h"

//////////////////////////////////////////////////////////////////
//static HMODULE g_hResourceDLL,资源DLL
//创建者:xuexiaofeng
//说明:
//////////////////////////////////////////////////////////////////

static HMODULE g_hResourceDLL  = NULL;

void InitHMGScrollbar();
void ReleaseHMGScrollbar();

HINSTANCE   GetHMGResourceDLLHandle()
{
	return g_hResourceDLL;
}

bool LoadHMGResourceDLL(CString strDLLFile)
{
	if ( g_hResourceDLL==NULL )
	{
		g_hResourceDLL = LoadLibrary((LPCSTR)strDLLFile);

		if(g_hResourceDLL==NULL)
			return false;
	}

	return true;
}

void UnLoadHMGResourceDLL()
{
	if (g_hResourceDLL)
		FreeLibrary(g_hResourceDLL);

	g_hResourceDLL=NULL;
}

HCURSOR  HMG_ResLoadCursor(UINT nIdResource)
{
	return ::LoadCursor(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}

HBITMAP  HMG_ResLoadBitmap(UINT nIdResource)
{
	return ::LoadBitmap(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}

HICON HMG_ResLoadIcon(UINT nIdResource)
{
	return ::LoadIcon(g_hResourceDLL,MAKEINTRESOURCE(nIdResource));
}


CSkinBitmap::CSkinBitmap()
{
	m_hResObj = NULL;
}

CSkinBitmap::~CSkinBitmap()
{
	if ( m_hResObj )
	{
		::DeleteObject(m_hResObj);
		__super::Detach();
	}
}

BOOL CSkinBitmap::Attach(HGDIOBJ hObject)
{
	if ( m_hResObj )
	{		
		__super::Detach();

		::DeleteObject(m_hResObj);
		m_hResObj = NULL;
	}

	return __super::Attach(hObject);
}

HGDIOBJ CSkinBitmap::Detach()
{
	if ( m_hResObj )
	{
		m_hResObj = NULL;
	}

	return __super::Detach();
}

BOOL CSkinBitmap::LoadBitmap(UINT nIDResource)
{
	if ( m_hResObj )
	{
		__super::Detach();
	}

	m_hResObj = (HGDIOBJ)::HMG_ResLoadBitmap(nIDResource);

	if ( m_hResObj )
	{
		__super::Attach(m_hResObj);
	}

	return m_hResObj != NULL;
}


bool InitHmgSkinControls(CString strResourceDLL)
{
	if ( LoadHMGResourceDLL(strResourceDLL) )	
	{
		brushBack  = ::CreateSolidBrush(RGB(203,235,248));
		brushWhite = ::CreateSolidBrush(RGB(255,255,255));

		//CXSkinDlg::InitResource();
		InitHMGScrollbar();
		return true;
	}
	else
	{
		return false;
	}
}

void UnInitHmgSkinControls()
{
	if ( brushBack )
	{
		DeleteObject(brushBack);
		brushBack = NULL;
	}

	if ( brushWhite )
	{
		DeleteObject(brushWhite);
		brushWhite = NULL;
	}

	ReleaseHMGScrollbar();
	UnLoadHMGResourceDLL();
}
