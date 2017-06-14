/************************************************************************ 
 文件描述：Stdafx.h预编译头文件需要包含的常用的头文件与类型定义(不要轻易修改此文件)
		   
 创建人： zouyf, 2006-6-9 
 
 修改记录： 
************************************************************************/

#ifndef __STDPUBLICTYPE_H_
#define __STDPUBLICTYPE_H_

// TODO: 在此处引用程序要求的附加头文件
typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned long long uint64;

#define SAFE_DELETE(x) { if (x) delete x; x = NULL; }


#include "assert.h"
//#include <io.h>
//#include "tchar.h"

#include "Log.h"

// STL includes
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#endif

