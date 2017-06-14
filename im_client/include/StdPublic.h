/************************************************************************ 
 �ļ�������Stdafx.hԤ����ͷ�ļ���Ҫ�����ĳ��õ�ͷ�ļ������Ͷ���(��Ҫ�����޸Ĵ��ļ�)
		   
 �����ˣ� zouyf, 2006-6-9 
 
 �޸ļ�¼�� 
************************************************************************/

#ifndef __STDPUBLICTYPE_H_
#define __STDPUBLICTYPE_H_

// TODO: �ڴ˴����ó���Ҫ��ĸ���ͷ�ļ�
typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;

#define SAFE_DELETE(x) { if (x) delete x; x = NULL; }

#include "Windows.h"

//#include "winsock2.h"
//#pragma comment(lib,"Ws2_32.lib")

//#include "Mmsystem.h"
//#pragma comment(lib,"Winmm.lib")

#include "assert.h"
#include <io.h>
#include "tchar.h"

#include "../../common/Log.h"

// STL includes
#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
using namespace std;

#endif