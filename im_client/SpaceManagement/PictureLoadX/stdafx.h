// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once

#ifndef STRICT
#define STRICT
#endif

// 如果您必须使用下列所指定的平台之前的平台，则修改下面的定义。
// 有关不同平台的相应值的最新信息，请参考 MSDN。
#ifndef WINVER				// 允许使用特定于 Windows 95 和 Windows NT 4 或更高版本的功能。
//#define WINVER 0x0400		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#define WINVER 0x0501		// 将此更改为针对于 Windows 98 和 Windows 2000 或更高版本的合适的值。
#endif

#ifndef _WIN32_WINNT		// 允许使用特定于 Windows NT 4 或更高版本的功能。
//#define _WIN32_WINNT 0x0400	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#define _WIN32_WINNT 0x0501	// 将此更改为针对于 Windows 2000 或更高版本的合适的值。
#endif						

#ifndef _WIN32_WINDOWS		// 允许使用特定于 Windows 98 或更高版本的功能。
//#define _WIN32_WINDOWS 0x0410 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#define _WIN32_WINDOWS 0x0501 // 将此更改为针对于 Windows Me 或更高版本的合适的值。
#endif

#ifndef _WIN32_IE			// 允许使用特定于 IE 4.0 或更高版本的功能。
//#define _WIN32_IE 0x0400	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#define _WIN32_IE 0x0500	// 将此更改为针对于 IE 5.0 或更高版本的合适的值。
#endif

#define _ATL_APARTMENT_THREADED
#define _ATL_NO_AUTOMATIC_NAMESPACE

#define _ATL_CSTRING_EXPLICIT_CONSTRUCTORS	// 某些 CString 构造函数将为显式的

// 关闭 ATL 对某些常被安全忽略的常见警告消息的隐藏
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
#define  WM_DATA_UPDATE       WM_USER+5        //选项窗口数据到达是通知主窗口;
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
