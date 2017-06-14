// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���Ǿ���ʹ�õ��������ĵ�
// �ض�����Ŀ�İ����ļ�

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

#define APP_NAME	_T("Mini �ϴ�ͼƬ")
#define APP_VERSION	_T("3.0.061110")

#define for if(0);else for

enum _CURSOR_TYPE{CURSOR_TYPE_NORMAL = 0, CURSOR_TYPE_NODRAG, CURSOR_TYPE_HANDLE, CURSOR_TYPE_WAIT};


int BOOL2Int(const BOOL b);
ULONG GetFileSize(const CString & filepath);


extern BOOL			g_bEnableLogInfo;
extern CConfigInfo  g_AppConfigInfo;
extern BOOL			g_bEnableContinueOperation;
