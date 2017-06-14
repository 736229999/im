// stdafx.h : 标准系统包含文件的包含文件，
// 或是经常使用但不常更改的
// 特定于项目的包含文件

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
#include <afxwin.h>
#include <afxdisp.h>
#include <afxdlgs.h>
#include <afxext.h>
#include <afxpriv.h>
#include <afxinet.h>

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

using namespace ATL;

#include <comdef.h>

#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include <io.h>

#include <gdiplus.h>
using namespace Gdiplus;

#include <atlimage.h>

#include "ConfigInfo.h"

CString  WideStrToStr(BSTR str);
BSTR	 StrToWideStr(CString str);
CString  ToHexString(BYTE bytes[] ,int len);
CString  GetFileNameFromPath(CString strFullPathName);
CString  UTF8_Convert(CString  str, BOOL bEncode ) ;
CString  IntToStr(long n);
CString  UIntToStr(unsigned long n);
void	 OpenHtmlPage(CString strUrl);
void	 ExtractFilePath(CString strFilePathName, CString &strPath, CString &strName, CString &strExt);
bool	 GetUploadResult(CString strHtml, CString &strPicUrl, CString &strError);
void	 GetSectionValue(const CString &strInfo, CString strSec1,CString strSec2, CString &strRet);

#define WM_PREVIEW  WM_USER+0x1001
#define WM_DESTROYPREVIEW WM_USER+0x1002
#define WM_NOTIFY_LOADFINISHED WM_USER+0x1003
#define WM_CANCEL_NOTIFY  WM_USER+0x1004
#define WM_THREAD_NOTIFY WM_USER+0x1005
#define WM_LISTCTRL_TEXTTIP_LBUTTONDOWN	WM_USER+0x1006

#define THREAD_NOTIFY_MSG_USER_VALIDATE		2
#define THREAD_NOTIFY_MSG_LOAD_FOLDER_FILES_FINISHED	3
#define THREAD_NOTIFY_MSG_COMPRESSING_FILE	4
#define THREAD_NOTIFY_MSG_CLOSE_PROGRESSBAR	5




#define UPLOADING_THREAD_NOTIFIY_PROGRESS	1
#define UPLOADING_THREAD_NOTIFIY_UPLOAD_FINISHED	2


const	int MAX_SPACE_SIZE	= 1024;		// MB

CString _get_tmp_directroy();
void 	_delete_all_tmp_files();
int		_get_remain_space_size();


#include <math.h>
double Round(double d, int iPrecision);

#define APP_NAME	_T("Mini 上传图片")
#define APP_VERSION	_T("3.0.061110")

#define for if(0);else for

enum _CURSOR_TYPE{CURSOR_TYPE_NORMAL = 0, CURSOR_TYPE_NODRAG, CURSOR_TYPE_HANDLE, CURSOR_TYPE_WAIT};


int BOOL2Int(const BOOL b);
ULONG GetFileSize(const CString & filepath);


extern BOOL			g_bEnableLogInfo;
extern CConfigInfo  g_AppConfigInfo;
extern BOOL			g_bEnableContinueOperation;
