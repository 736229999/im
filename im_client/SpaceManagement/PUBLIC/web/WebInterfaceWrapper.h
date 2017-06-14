#pragma once

#include "SpaceTypeDef.h"


#ifdef _DEBUG
#import "..\..\..\binDebug\webinterface.dll" named_guids no_namespace
#else
#import "..\..\..\binRelease\webinterface.dll" named_guids no_namespace
#endif


#ifdef _DEBUG
#import "..\..\..\binDebug\UserInfo.dll" named_guids no_namespace
#else
#import "..\..\..\binRelease\UserInfo.dll" named_guids no_namespace
#endif

void ParaseWebAudios(BSTR strWebResult,vector<TWebAudioInfo *> &info, CString & strServerDate, int nMaxCount=10000);
void ParaseWebArticles(BSTR strWebResult,vector<TWebArticleInfo *> &info, CString & strServerDate, int nMaxCount=10000);
void ParaseWebPictures(BSTR strWebResult,vector<TWebPictureInfo *> &info, CString & strServerDate, int nMaxCount=10000);

class CWebInterfaceWrapper
{
public:
	CWebInterfaceWrapper(void);
	virtual ~CWebInterfaceWrapper(void);

public:
	void    InitInterface(long nUserId,CString strUserName, CString strNickName);
	IWebInterfaceXPtr & GetWebInterface() { return m_pWebInterface;	}

public:
	void    GetUserAudioList(long nFriendId, CString strBeginTime, CString strEndTime,vector<TWebAudioInfo *> &info, CString & strServerDate);
	void    GetUserArticleList(long nFriendId, CString strBeginTime, CString strEndTime,vector<TWebArticleInfo *> &info, CString & strServerDate);
	void    GetUserArticleTagList(vector<TTagInfo> &info);
	CString GetArticleContent(long nArticleId);

	//返回值：文章ID
	long    CreateArticle(long nOperationFlag/*0,create;1 modify*/,
						  long nArticleId,
						  CString strTitle,
						  CString strContent,
						  long nBookId,
						  long nViewProp,
						  CString strTag,
						  long nTopFlag,
						  CString & strError
		                  );

	bool   SetArticleTop(long nArticleId, long nTopFlag/*0,not top; 1 top*/,CString &strError);
	bool   SetArticleTitle(long nArticleId, CString strTitle,CString &strError);
	bool   SetArticleViewProp(long nArticleId,long nViewProp, CString &strError);
	bool   PutArticleToBook(long nArticleId,CString strBookIds,CString &strError);
	bool   DelArticleInBook(long nArticleId, long nBookId, CString &strError);
	bool   DeleteArticle(CString strArticleId,CString &strError);
	void   GetArticleBookList(long nArticleId,TArticleBookInfo &info);
	void   GetUserBookList(long nFriendId,TUserBookInfo &info);
	
	//返回值：Book ID
	long   CreateBook(long nOperationFlag/*0,create;1 modify*/,
					  long nBookId,
					  CString strBookName,
					  long nViewProp,
					  CString & strError
					  );

	bool   DeleteBook(CString strBookId,CString &strError);
	void   GetUserPictureList(long nFriend,CString strBeginTime, CString strEndTime,vector<TWebPictureInfo *> &info, CString & strServerDate);
	void   GetUserPictureTagList(vector<TTagInfo> &info);
	long   UploadPicture(CString strPictureFile, CString strTitle, CString strContent, CString strTag, int nViewProp, int nAlbumID, CString &strPictureUrl, CString & strError);
	bool   UploadFace(CString strPictureFile,CString & strPictureUrl, CString & strError);
	bool   SetPictureTitle(long nPictureId, CString strTitle, CString &strError);
	bool   SetPictureViewProp(long nPictureId, long nViewProp, CString &strError);
	bool   PutPictureToAlbum(long nPictureId, long nAlbumId, CString &strError);
	bool   DelPictureInAlbum(long nPictureId, long nAlbumId, CString &strError);
	bool   DeletePicture(CString strPictureId, CString &strError);
	void   GetPictureAlbumList(long nPictureId, TPicturelbumInfo &info);
	void   GetUserAlbumList(long nFriendId, TUserAlbumInfo &info);

	//返回值：Album ID
	long   CreateAlbum(long nOperationFlag/*0,create;1 modify*/,
		long nAlbumId,
		CString strAlbumName,
		long nViewProp,
		CString & strError
		);

	bool   DeleteAlbum(CString strAlbumId, CString &strError);
	bool   SetUserFace(CString strPictureUrl, CString &strError);
	bool   PayPhotoFrame(long nFrameId,float & fPaied, CString &strError);
	long   GetSpaceSize();
	void   SyncPicture();
	void   MiniOnline();

protected:
	IWebInterfaceXPtr m_pWebInterface;
	HRESULT			  m_hrCom;
};
