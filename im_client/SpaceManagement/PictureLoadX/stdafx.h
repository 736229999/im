// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once

#ifndef STRICT
#define STRICT
#endif

// ���������ʹ��������ָ����ƽ̨֮ǰ��ƽ̨�����޸�����Ķ��塣
// �йز�ͬƽ̨����Ӧֵ��������Ϣ����ο� MSDN��
#ifndef WINVER				// ����ʹ���ض��� Windows 95 �� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define WINVER 0x0400		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define WINVER 0x0501		// ���˸���Ϊ����� Windows 98 �� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_WINNT		// ����ʹ���ض��� Windows NT 4 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINNT 0x0400	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINNT 0x0501	// ���˸���Ϊ����� Windows 2000 ����߰汾�ĺ��ʵ�ֵ��
#endif						

#ifndef _WIN32_WINDOWS		// ����ʹ���ض��� Windows 98 ����߰汾�Ĺ��ܡ�
//#define _WIN32_WINDOWS 0x0410 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_WINDOWS 0x0501 // ���˸���Ϊ����� Windows Me ����߰汾�ĺ��ʵ�ֵ��
#endif

#ifndef _WIN32_IE			// ����ʹ���ض��� IE 4.0 ����߰汾�Ĺ��ܡ�
//#define _WIN32_IE 0x0400	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#define _WIN32_IE 0x0500	// ���˸���Ϊ����� IE 5.0 ����߰汾�ĺ��ʵ�ֵ��
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// ĳЩ CString ���캯����Ϊ��ʽ��

// �ر� ATL ��ĳЩ������ȫ���Եĳ���������Ϣ������
#define _ATL_ALL_WARNINGS
  
#pragma warning (disable:4996)
#pragma warning (disable:4224)
#pragma warning (disable:4312)

#include <afxcmn.h>	
#include <afxwin.h>
#include <afxdisp.h>
#include <winuser.h>
#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>
#include <windows.h>
#include <afxext.h>
#include "resource.h"
using namespace ATL;

#include <list>
#include <vector>
using namespace std;

#include <GdiPlus.h>
using namespace Gdiplus;

#include <io.h>
#include "resource.h"
#include "resgdiobj.h"

#include "spacedata.h"
#include "..\hmgcrtlib.h"
#include "WebInterfaceWrapper.h"
#include "WebPictureProcess.h"


#define WM_WEBDATA_NOTIFY		WM_USER+1
#define WM_WEBPICTURE_NOTIFY    WM_USER+2
#define WM_THREAD_NOTIFY		WM_USER+3
#define  WM_SHOWTRAY          WM_USER+4
#define  WM_DATA_UPDATE       WM_USER+5        //ѡ������ݵ�����֪ͨ������;
#define  PICTURES_MAX     200

#define BMP_WIDTH				135
#define BMP_HEIGHT				110
#define BMP_LEFT              10
#define BMP_SPACE				1


struct  TBitmap
{
	DWORD		   dwPictureID;
	CBitmap  * pBitmap;
};
struct  TThreadParam 
{
	HWND		hWndNotify;
	DWORD		dwPictureID;
	char		szPictureUrl[255];
	int        nThreadIndex;
};

extern  CString				 g_strCachePath;
extern  vector<TWebPictureInfo *>  g_webPictureList;
extern  vector<TBitmap> g_vecBitmap;
extern  CCriticalSection		g_secWebPictureList;
extern  CCriticalSection		g_secBitmap;
extern  CSpaceData              g_spaceData;
extern	 BOOL	g_bPicsDlgVisible;
extern	 vector   <long>					 g_vecUserID;
extern  HWND			 g_hMessageWnd;

extern	long		g_nUserID;
extern	CString     g_strUserName, g_strNickName;

CString MakeGUID(char* szData);
