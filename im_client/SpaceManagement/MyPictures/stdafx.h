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

#include <afxwin.h>
#include <afxdisp.h>
#include <afxdlgs.h>

#include "resource.h"
#include <atlbase.h>
#include <atlcom.h>

using namespace ATL;


#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

#include <gdiplus.h>
using namespace Gdiplus;

#include <hmgskincontrol.h>
#include "..\hmgcrtlib.h"
#include "resgdiobj.h"

#ifdef _DEBUG
#import "..\..\binDebug\UserInfo.dll" named_guids no_namespace
#import "..\..\binDebug\HeadPhoto.dll" named_guids no_namespace
#else
#import "..\..\binRelease\UserInfo.dll" named_guids no_namespace
#import "..\..\binRelease\HeadPhoto.dll" named_guids no_namespace
#endif

#import "..\WebConfig\Debug\WebConfig.tlb" named_guids no_namespace
#include "spacedata.h"

#include <io.h>
#include <WebBusiness.h>

#define WM_WEBDATA_NOTIFY		WM_USER+1
#define WM_WEBPICTURE_NOTIFY    WM_USER+2
#define WM_THREAD_NOTIFY		WM_USER+3

extern IUserInfoX *     g_pUserInfo;
extern IWebConfigXPtr   g_pWebConfig;
extern IHeadPhotoX *    g_pHeadPhoto;
extern CSpaceData		g_spaceData;
extern HWND				g_hMessageWnd;

void ShowMessage(LPCSTR lpszMsg,UINT nType=MB_OK);