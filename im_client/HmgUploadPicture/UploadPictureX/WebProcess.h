#pragma once

#include "..\stdafx.h"

typedef unsigned char    uint8;
typedef unsigned short   uint16;
typedef unsigned int     uint32;
typedef unsigned __int64 uint64;

struct TTagInfo
{
	long nTagId;
	char szTagName[128];
	int operator==(const TTagInfo &first)
	{
		return strcmp(first.szTagName,szTagName)==0;
	}
};

struct TAlbumInfo
{
	long nAlbumId;
	char szAlbumName[255];
	char szCreateDate[45];
};

struct TUserAlbumInfo
{
	uint32 uID;

	vector<TAlbumInfo> vecAlbum;

	TUserAlbumInfo()
	{
		uID=0;
	}
};

CString MakeIMAuthString(CString strUserName);
CString GetServerName(CString strWebLocation);
bool    GetUploadResult(CString strHtml, CString &strPicUrl, CString &strError);
bool	UploadPicture(CString strPictureFile, CString strTitle, CString strTag, CString strContent, int nViewProp, int nAlbumID,  CString &strPictureUrl,CString & strError);
void	GetUserAlbumList(long nFriendId,TUserAlbumInfo &info);
void	GetUserPictureTagList(vector<TTagInfo> &info);

//∑µªÿœ‡≤·ID
long    CreateAlbum(long nOperationFlag/*0,create;1 modify*/,
				    long nAlbumId,
				    CString strAlbumName,
				    long nViewProp,
				    CString & strError
				    );

long	GetSpaceSize();