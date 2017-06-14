// stdafx.cpp : 只包括标准包含文件的源文件
// PictureLoadX.pch 将成为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"

CSpaceData       g_spaceData;
CString			 g_strCachePath;

HWND			 g_hMessageWnd = NULL;


long			g_nUserID;
CString			g_strUserName, g_strNickName;

CString MakeGUID(char* szData)
{
	unsigned char digest[16];

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)szData,(unsigned int)strlen(szData) );
	md5T.MD5Final (digest);

	return ToHexString(digest,16);
}

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

CCriticalSection          g_secWebPictureList;
CCriticalSection		   g_secBitmap;
vector<TWebPictureInfo *>  g_webPictureList;
vector<TBitmap>	g_vecBitmap;
BOOL	g_bPicsDlgVisible;
vector<long>					 g_vecUserID;
int  g_UserPictureListCount[20];
