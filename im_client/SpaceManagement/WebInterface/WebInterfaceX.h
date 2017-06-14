// WebInterfaceX.h : CWebInterfaceX µÄÉùÃ÷

#pragma once
#include "resource.h"       // Ö÷·ûºÅ

#include "WebInterface.h"


// CWebInterfaceX

class ATL_NO_VTABLE CWebInterfaceX : 
	public CComObjectRootEx<CComSingleThreadModel>,
	public CComCoClass<CWebInterfaceX, &CLSID_WebInterfaceX>,
	public IDispatchImpl<IWebInterfaceX, &IID_IWebInterfaceX, &LIBID_WebInterfaceLib, /*wMajor =*/ 1, /*wMinor =*/ 0>
{
public:
	CWebInterfaceX()
	{
		m_nUserID     = 0;
		m_strUserName = "";
		m_strNickName = "";
	}


DECLARE_REGISTRY_RESOURCEID(IDR_WEBINTERFACEX)


BEGIN_COM_MAP(CWebInterfaceX)
	COM_INTERFACE_ENTRY(IWebInterfaceX)
	COM_INTERFACE_ENTRY(IDispatch)
END_COM_MAP()


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{

		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:
	STDMETHOD(SetUserInfo)(LONG nUserID, BSTR strUserName, BSTR strNickName);
	STDMETHOD(UploadPicture)(BSTR strPictureFile, LONG *nFileSize, BSTR* strPictureUrl, BSTR* strError);
	STDMETHOD(GetUserArticleList)(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR *strReturn);
	STDMETHOD(GetArticleContent)(LONG nArticleId, BSTR* strReturn);
	STDMETHOD(CreateArticle)(LONG nOperationFlag, LONG nArticleId, BSTR strTitle, BSTR strContent, LONG nBookId, LONG nViewProp, BSTR strTag, LONG nTopFlag, BSTR* strReturn);
	STDMETHOD(SetArticleTop)(LONG nArticleId, LONG nTopFlag, BSTR *strReturn);
	STDMETHOD(SetArticleTitle)(LONG nArticleId, BSTR strTitle, BSTR *strReturn);
	STDMETHOD(SetArticleViewProp)(LONG nArticleId, LONG nViewProp, BSTR *strReturn);
	STDMETHOD(PutArticleToBook)(LONG nArticleId, BSTR strBookIds, BSTR *strReturn);
	STDMETHOD(DeleteArticle)(BSTR strArticleId, BSTR *strReturn);
	STDMETHOD(CreateBook)(LONG nOperationFlag, LONG nBookId, BSTR strBookName, LONG nViewProp, BSTR* strReturn);
	STDMETHOD(GetUserBookList)(LONG nFriendId, BSTR* strReturn);
	STDMETHOD(DeleteBook)(LONG nBookId, BSTR* strReturn);
	STDMETHOD(GetUserPictureList)(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR* strReturn);
	STDMETHOD(PutPictureToAlbum)(LONG nPictureId, LONG nAlbumId, BSTR* strReturn);
	STDMETHOD(DeletePicture)(BSTR strPictureId, BSTR* strReturn);
	STDMETHOD(GetUserAlbumList)(LONG nFriendId, BSTR* strReturn);
	STDMETHOD(CreateAlbum)(LONG nOperationFlag, LONG nAlbumId, BSTR strAlbumName, LONG nViewProp, BSTR* strReturn);
	STDMETHOD(DeleteAlbum)(LONG nAlbumId, BSTR* strReturn);
	STDMETHOD(GetUserArticleTagList)(BSTR* strReturn);
	STDMETHOD(GetUserPictureTagList)(BSTR* strReturn);
	STDMETHOD(GetSpaceSize)(BSTR* strReturn);
	STDMETHOD(MiniOnline)(void);
	STDMETHOD(SetPictureTitle)(LONG nPictureId, BSTR strTitle, BSTR* strReturn);
	STDMETHOD(SetPictureViewProp)(LONG nPictureId, LONG nViewProp, BSTR* strReturn);
	STDMETHOD(SetUserFace)(BSTR strPictureUrl, BSTR* strReturn);
	STDMETHOD(PayPhotoFrame)(LONG nFrameId, FLOAT* fPaied, BSTR* strError, VARIANT_BOOL* pReturnVal);
	STDMETHOD(DelArticleInBook)(LONG nArticleId, LONG nBookId, BSTR* strReturn);
	STDMETHOD(DelPictureInAlbum)(LONG nPictureId, LONG nAlbumId, BSTR* strReturn);
	STDMETHOD(GetArticleBookList)(LONG nArticleId, BSTR* strReturn);
	STDMETHOD(GetPictureAlbumList)(LONG nPictureId, BSTR* strReturn);
	STDMETHOD(SyncPicture)(BSTR* strReturn);
	STDMETHOD(EnablePicture)(BSTR strPictureUrl, BSTR strTitle, BSTR strContent, BSTR strTag, LONG nViewProp, LONG nFileSize, LONG nAlbumId, BSTR* strReturn);
	STDMETHOD(GetUserAudioList)(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR* strReturn);

protected:
	long    m_nUserID;
	CString m_strUserName, m_strNickName;
};

OBJECT_ENTRY_AUTO(__uuidof(WebInterfaceX), CWebInterfaceX)
