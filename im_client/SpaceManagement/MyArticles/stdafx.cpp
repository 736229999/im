// stdafx.cpp : 只包括标准包含文件的源文件
// MyArticles.pch 将成为预编译头
// stdafx.obj 将包含预编译类型信息

#include "stdafx.h"
#include <io.h>

IUserInfoX *     g_pUserInfo     = NULL;
IWebConfigXPtr   g_pWebConfig    = NULL;
IWriteArticleX * g_pWriteArticle = NULL;
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
	MessageBox(NULL,lpszMsg,"提示",nType);
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
