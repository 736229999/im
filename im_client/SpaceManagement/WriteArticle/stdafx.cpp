// stdafx.cpp : ֻ������׼�����ļ���Դ�ļ�
// WriteArticle.pch ����ΪԤ����ͷ
// stdafx.obj ������Ԥ����������Ϣ

#include "stdafx.h"

IUserInfoX *    g_pUserInfo;
IWebConfigXPtr  g_pWebConfig;
IMyArticlesX *  g_pMyArticles;
CSpaceData      g_spaceData ;

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
	MessageBox(GetDesktopWindow(),lpszMsg,"��������",nType);
}

INT CALLBACK EnumFontNameProc(LOGFONT *plf, TEXTMETRIC* /*ptm*/, INT /*nFontType*/, LPARAM lParam)
{
	CComboBox* comboBox = (CComboBox*) lParam;
	comboBox->AddString(plf->lfFaceName);

	return TRUE;
}
