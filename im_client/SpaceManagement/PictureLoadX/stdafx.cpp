// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// PictureLoadX.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

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
//  ���ܣ����ַ���ת��ΪAnsi�ַ���
//  ���룺
//  ����ֵ��CString
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
/*CString WideStrToStr(BSTR str)
{
	CComBSTR b=str;
	CString  r(b);
	return r;
}*/

/////////////////////////////////////////////////////////////////////////////////////////
//  StrToWideStr
//  ���ܣ�Ansi�ַ�ת��Ϊ�����ַ���
//  ���룺
//  ����ֵ��BSTR,ע��Ҫ����SysFree
//  �޸ļ�¼��
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
