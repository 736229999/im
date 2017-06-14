#include "stdafx.h"
#include "webbusiness.h"

void MiniOpenUrl(CString strUrl)
{
	BSTR			strUserName;
	BSTR			strNickName;
	BSTR			strEmail;
	BSTR			strFaceUrl;
	long			nUserID;
	short			nSex;
	float			nGoldMoney;
	unsigned short	nMessageCount;

	g_pUserInfo->get_UserID(&nUserID);
	g_pUserInfo->get_Sex(&nSex);
	g_pUserInfo->get_GoldMoney(&nGoldMoney);
	g_pUserInfo->get_MessageCount(&nMessageCount);
	g_pUserInfo->get_UserName(&strUserName);
	g_pUserInfo->get_FaceUrl(&strFaceUrl);
	g_pUserInfo->get_NickName(&strNickName);
	g_pUserInfo->get_Email(&strEmail);

	
	OpenHtmlPage( MakePassportUrl(  strUrl,
									nUserID,
									WideStrToStr(strUserName),
									WideStrToStr(strNickName),
									nSex,
									WideStrToStr(strEmail),
									WideStrToStr(strFaceUrl),
									(long)nGoldMoney,
									nMessageCount));
}

void ShowArticle(char *username)
{
	CString strUrl = g_pWebConfig->GetWebPageUrl(USER_ARTICLE,StrToWideStr(username));//>GetStaticUrl(stUserArticle,username);
	MiniOpenUrl(strUrl);
}

void ShowAlbum(char *username)
{
	CString strUrl = g_pWebConfig->GetWebPageUrl(USER_PHOTO,StrToWideStr(username));//>GetStaticUrl(stUserPhoto,username);
	MiniOpenUrl(strUrl);
}

bool GetUploadResult(CString strHtml, CString &strPicUrl, CString &strError)
{
	CString strLower = strHtml.MakeLower();

	CStringList slName, slContent;

	int pos1=0 , pos2=0;
	int pos3=0;

	strPicUrl = "";
	strError  = "";

	bool bRet = false;

	while ( 1)
	{	
		pos1 = strLower.Find("<meta",pos3);

		if ( pos1>-1 )
		{
			pos2 = strLower.Find(">",pos1);
		}
		else
		{
			if ( pos3==0 )
				strError = UTF8_Convert(strHtml, FALSE);
			break;
		}

		pos3     = pos2+1;

		if ( pos1>0 && pos2>pos1 )
		{
			CString strMeta = strHtml.Mid(pos1,pos2-pos1+1);
			CString strName ;
			CString strContent ;

			int lmeta    = strMeta.GetLength();
			int name1    = strMeta.Find("name=",0);
			int content1 = strMeta.Find("content=",0);

			int i;

			for ( i=name1+5;i<lmeta; i++ )
			{
				if ( strMeta[i]==' ' || strMeta[i]=='>' )
				{
					break;
				}
				else
				{
					strName=strName+strMeta[i];
				}
			}	

			strName.Replace("\"","");
			strName.Trim();

			for ( i=content1+8;i<lmeta; i++ )
			{
				if ( strMeta[i]==' '|| strMeta[i]=='>' )
				{
					break;
				}
				else
				{
					strContent=strContent+strMeta[i];
				}
			}	

			strContent.Replace("\"","");
			strContent.Trim();

			if ( strName=="return_value" )
			{
				if ( strContent=="1" )
					bRet=true;
				else
					bRet=false;
			}
			else
			if (strName=="return_info" )
			{
				strError = UTF8_Convert(strContent,FALSE);
			}
			else
			if (strName=="pic_url") 
			{
				strPicUrl = strContent;
			}
		}
		else
		{
			break;
		}
	}

	if ( bRet )
	{
		strPicUrl.Trim();

		if ( strPicUrl=="" )
		{
			strError ="���ش����ͼƬ����";
			bRet=false;
		}
		else
		{
			strPicUrl.MakeLower();
			strPicUrl.Replace("/z/","/s/");

			if ( strPicUrl.Left(4)!="http" )
				strPicUrl = "http://"+strPicUrl;
		}
	}

	if ( bRet==false )
	{
		strError.Trim();
		if (strError=="" )
		{
			strError="�����ͼƬ������Ϣ";
		}
	}

	return bRet;
}

CString WebDataFrom2Str(int iFrom)
{
	switch ( iFrom )
	{
	case 0:
		return "��վ";
	case 1:
		return "WAP";
	case 2:
		return "����";
	case 3:
		return "����";
	case 4:
		return "Mini";
	case 5:
		return "���ʹ���";
	case 6:
	case 7:
		return "����";
	case 8:
		return "�����ϴ��ؼ�";
	case 9:
		return "Mini - ��ͷ��";
	}

	return "";
}

CString WebViewProp2Str(int iViewProp)
{
	switch ( iViewProp )
	{
		//0���Լ���1�����ѣ�2��ע���û���3������
	case 0:
		return "˽�е�,ֻ���Լ���";
	case 1:
		return "���Ը����ѿ�";
	case 2:
		return "139��վ�û���";
	case 3:
		return "����";
	}

	return "";
}
