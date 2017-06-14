#ifndef PUBHEADER_H
#define PUBHEADER_H

#pragma once

#define FORDER_NUM			100

#define THUMBNAIL_WIDTH		80
#define THUMBNAIL_HEIGHT	80

#define FILELARGESIZE		2.0
#define RARFILELARGESIZE	2.0
#define SPACELARGE			30

#define IDC_HAND MAKEINTRESOURCE(32649)
#define IDC_ARROW           MAKEINTRESOURCE(32512)

#include <list>
#include <vector>
#include <map>
#include <algorithm>
using namespace std;

// CUpLoadPhotoDlg 对话框
typedef struct tagFileInfo
{
	DWORD		dwFileID;
	CString		strFileDir;
	ULONG		ulFileSize;
	CString		strFileName;
	BYTE		bFlag;			 //0 normal, 1 sel, 2 uploaded
	
	int			nLimittedMaxSize;		// 最长边
	CString		strDescription;
	
	int			nImageHeight, nImageWidth;				// current size
	int			nSoruceImageHeight, nSoruceImageWidth;	// soruce size
	BOOL		bImageFile;

	CString		strSourceFileName, strSourceFileDir;		//
	// bIsChanged 为TRUE时就应该从TmpFile中去读取
	BOOL		bIsChanged;
	CString		strGeneratedTmpFilePath;
	tagFileInfo():dwFileID(0), ulFileSize(0),bFlag(0),nLimittedMaxSize(0),nImageHeight(0), nImageWidth(0),bImageFile(TRUE),bIsChanged(FALSE){}
}FILEINFO;

typedef struct
{
	CString strFilePathName;
	CString strSourceFileName;
	CString strSourceFilePathName;
	CString	strDescription;

	BOOL		bIsChanged;
	CString		strGeneratedTmpFilePath;

	CString		strFileDir;
	ULONG		ulFileSize;
	CString		strFileName;

	int			nImageHeight, nImageWidth;				// current size
	int			nSoruceImageHeight, nSoruceImageWidth;	// soruce size

	BOOL	bRemoveIt;
}tagUpFileInfo;

typedef list<tagUpFileInfo> _lstUpFileINFO;
typedef _lstUpFileINFO::iterator _lstUpFileINFO_iterator;

struct IDCompare : binary_function<FILEINFO, DWORD, bool>
{
	bool operator()(const FILEINFO& _X, const DWORD& _Y) const
	{
		return (_X.dwFileID == _Y); 
	}
};

typedef list<FILEINFO>  FileInfoList;
typedef FileInfoList::iterator FileInfoList_iterator;

struct _tagUserInfo
{
	BOOL	bNeedvalidate;

	CString strNickName;
	CString strUserName;
	
	BOOL bIsMiniClinet;

	CString strPassword;
	CString strAuthKey ;
	CString strAuthUrl;

	CString strPicUrl;
	CString strParamInfo;
	CString strParamKey;
	CString strDefaultAlbum;

	unsigned long nControlVersion;
	unsigned long nUserId;
};	

struct _tagUploadFileProp
{
	int		iViewProp;		// ACL	

	int     iAlbumDirId;	// Album id
	CString	strAlbumName;

	CString strDescription;	// i.e. filename
	CString strTag;
};



#endif