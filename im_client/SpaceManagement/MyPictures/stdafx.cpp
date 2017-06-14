// stdafx.cpp : 只包括标准包含文件的源文件
// MyPictures.pch 将成为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

IUserInfoX *     g_pUserInfo     = NULL;
IWebConfigXPtr   g_pWebConfig    = NULL;
IHeadPhotoX *    g_pHeadPhoto    = NULL;
HWND			 g_hMessageWnd   = NULL;
CSpaceData      g_spaceData;

/////////////////////////////////////////////////////////////////////////////////////////
//  WideStrToStr
//  功能：宽字符串转换为Ansi字符串
//  输入：
//  返回值：CString
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
/*CString WideStrToStr(BSTR str)
{
	CComBSTR b=str;
	CString  r(b);
	return r;
}*/

/////////////////////////////////////////////////////////////////////////////////////////
//  StrToWideStr
//  功能：Ansi字符转换为串宽字符串
//  输入：
//  返回值：BSTR,注意要调用SysFree
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
/*BSTR  StrToWideStr(CString str)
{
	CComBSTR b=str;
	BSTR     r=L"";
	b.CopyTo(&r);
	return r;
}*/

void ShowMessage(LPCSTR lpszMsg,UINT nType)
{
	MessageBox(NULL,lpszMsg,"我的图片",nType);
}
