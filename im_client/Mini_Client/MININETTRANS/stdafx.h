// stdafx.h : ��׼ϵͳ�����ļ��İ����ļ���
// ���ǳ��õ��������ĵ���Ŀ�ض��İ����ļ�
//

#pragma once


#define WIN32_LEAN_AND_MEAN		// �� Windows ͷ���ų�����ʹ�õ�����


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