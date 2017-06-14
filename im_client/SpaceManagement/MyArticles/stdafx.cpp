// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// MyArticles.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"
#include <io.h>

IUserInfoX *     g_pUserInfo     = NULL;
IWebConfigXPtr   g_pWebConfig    = NULL;
IWriteArticleX * g_pWriteArticle = NULL;
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
	MessageBox(NULL,lpszMsg,"��ʾ",nType);
}

void WriteArticlePreviewFile(const TWebArticleInfo &info,CString strAuthor, CString strLocalFile)
{
	CString strHTML = ResLoadHtmlResource(IDR_HTML1);

	CString strDot  = GetModulePath()+"Image\\dot.gif";

	strHTML.Replace("VVVAAA-dot-AAAVVV",strDot);
	strHTML.Replace("VVVAAA-author-AAAVVV",strAuthor);
	strHTML.Replace("VVVAAA-link-AAAVVV",info.szUrl);
	strHTML.Replace("VVVAAA-content-AAAVVV",info.szOutlines);
	strHTML.Replace("VVVAAA-title-AAAVVV",info.szTitle);
	strHTML.Replace("VVVAAA-id-AAAVVV",IntToStr(info.uID));
	CString strDate = info.szModifyDate;
	strDate.Replace(" ","&nbsp;");
	strHTML.Replace("VVVAAA-date-AAAVVV",strDate);

	if ( access((LPCSTR)strLocalFile,0)!=-1 )
	{
		DeleteFile(strLocalFile);
	}

	CStdioFile f;

	if ( f.Open(strLocalFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		f.WriteString(strHTML);
		f.Close();
	}
}
