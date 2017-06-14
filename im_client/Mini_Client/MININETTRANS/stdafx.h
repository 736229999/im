// stdafx.h : 标准系统包含文件的包含文件，
// 或是常用但不常更改的项目特定的包含文件
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// 从 Windows 头中排除极少使用的资料


#ifdef _DEBUG
#include <afx.h>
#else
#define TRACE()
#endif

#include "../../include/stdpublic.h"

#ifdef _DEBUG
#include "vld.h"
#endif

//#ifdef _DEBUG
//
//#define TRACE()
//
//#else
//#define TRACE()
//#endif