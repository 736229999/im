// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// MyPictures.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

IUserInfoX *     g_pUserInfo     = NULL;
IWebConfigXPtr   g_pWebConfig    = NULL;
IHeadPhotoX *    g_pHeadPhoto    = NULL;
HWND			 g_hMessageWnd   = NULL;
CSpaceData      g_spaceData;

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

void ShowMessage(LPCSTR lpszMsg,UINT nType)
{
	MessageBox(NULL,lpszMsg,"�ҵ�ͼƬ",nType);
}
