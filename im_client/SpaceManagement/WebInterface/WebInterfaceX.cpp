// WebInterfaceX.cpp : CWebInterfaceX 的实现

#include "stdafx.h"
#include ".\webinterfacex.h"
#include "HttpObject.h"

bool GetUploadResult(CString strHtml, CString &strDomain)
{
	CString strRet;
	long    nRetCode;

	GetSectionValue(strHtml,"<retcode>","</retcode>",strRet);
	nRetCode  = atol((LPCSTR)strRet);

	if ( nRetCode==1 )
	{
		GetSectionValue(strHtml,"<domain>","</domain>",strDomain);
		return true;
	}
	else
	{
		return false;
	}
}

// CWebInterfaceX

STDMETHODIMP CWebInterfaceX::SetUserInfo(LONG nUserID, BSTR strUserName, BSTR strNickName)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	m_nUserID = nUserID;
	m_strUserName = WideStrToStr(strUserName);
	m_strNickName = WideStrToStr(strNickName);

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::UploadPicture(BSTR strPictureFile, LONG* nFileSize, BSTR* strPictureUrl, BSTR* strError)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(UPLOAD_PICTURE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		::CoUninitialize();

	CString strFileDir;
	long    nUserId = m_nUserID;

	strFileDir.Format("%d\\%d\\%d",nUserId/1000000,nUserId/1000,nUserId);

	objHttp.AddParam("clienttype","1");
	objHttp.AddParam("filedir",strFileDir);

	CString strFullFilePath = WideStrToStr(strPictureFile);
	CString strPath, strFileName, strFileExt;//     = GetFileNameFromPath(strFullFilePath);
	SplitFilePath(strFullFilePath,strPath,strFileName,strFileExt);

	CStdioFile objFile;

	CString strFileParamName;

	CTime tm = CTime::GetCurrentTime();
	strFileParamName.Format("%s%03d",tm.Format("%Y%m%d%H%M%S"), 1000+rand()%1000);

	if ( objFile.Open((LPCSTR)strFullFilePath,CFile::modeRead|CFile::typeBinary) )
	{
		char * pData;
		long   nFileLen = objFile.GetLength();

		pData = new char[nFileLen];
		objFile.Read(pData,nFileLen);
		objHttp.AddFile(strFileParamName,strFileName+strFileExt,pData,nFileLen);
		delete [] pData;

		objFile.Close();

		* nFileSize    = nFileLen;
	}

	objHttp.AddParam("returnUrl","");

	CString strDomain;
	CString strHtml = objHttp.SendRequest();
	
	if ( strHtml=="" )
		*strError = objHttp.GetLastError().AllocSysString();
	else
	{
		if ( GetUploadResult(strHtml,strDomain) )
		{
			strFileDir.Replace("\\","/");
			CString csPicUrl = strDomain+"/"+strFileDir+"/"+strFileParamName+strFileExt;
			*strPictureUrl   = csPicUrl.AllocSysString();
		}
		else
		{
			*strError = L"服务器处理繁忙，图片上传失败";
		}
	}

	return S_OK;
}


STDMETHODIMP CWebInterfaceX::GetUserArticleList(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR *strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_ARTICLE_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",10);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("beginTime",WideStrToStr(strBeginTime));
	objHttp.AddParam("endTime",WideStrToStr(strEndTime));

	CString strValid = objHttp.GetValidString();
	objHttp.AddParam("valid",strValid);

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetArticleContent(LONG nArticleId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(ARTICLE_CONTENT,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",11);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::CreateArticle(LONG nOperationFlag, LONG nArticleId, BSTR strTitle, BSTR strContent, LONG nBookId, LONG nViewProp, BSTR strTag, LONG nTopFlag, BSTR * strReturn )
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(CREATE_ARTICLE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",12);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("userNick",UTF8_Convert((LPCSTR)m_strNickName,TRUE));
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("operFlag",nOperationFlag);
	objHttp.AddParam("from",4);
	objHttp.AddParam("title",UTF8_Convert(WideStrToStr(strTitle).Trim(),TRUE));
	objHttp.AddParam("content",UTF8_Convert(WideStrToStr(strContent).Trim(),TRUE));
	objHttp.AddParam("bookId",nBookId);
	objHttp.AddParam("contPop",nViewProp);
	objHttp.AddParam("commentFlag",0);
	objHttp.AddParam("usrTag",UTF8_Convert(WideStrToStr(strTag),TRUE));	
	objHttp.AddParam("topFlag",nTopFlag);
	objHttp.AddParam("authFlag",1);

	CString strValid = objHttp.GetValidString();
	objHttp.AddParam("valid",strValid);

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetArticleTop(LONG nArticleId, LONG nTopFlag, BSTR * strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_ARTICLE_TOP,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",13);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("flag",nTopFlag);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetArticleTitle(LONG nArticleId, BSTR strTitle, BSTR *strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_ARTICLE_TITLE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",14);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("title",WideStrToStr(strTitle));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetArticleViewProp(LONG nArticleId, LONG nViewProp, BSTR * strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_ARTICLE_VIEW_PROP,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",15);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("contPop",nViewProp);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::PutArticleToBook(LONG nArticleId, BSTR strBookIds, BSTR *strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(PUT_ARTICLE_TO_BOOK,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",16);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("bookId",WideStrToStr(strBookIds));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DeleteArticle(BSTR strArticleId, BSTR * strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(DELETE_ARTICLE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",17);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",WideStrToStr(strArticleId));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetUserBookList(LONG nFriendId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_BOOK_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",20);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}


STDMETHODIMP CWebInterfaceX::CreateBook(LONG nOperationFlag, LONG nBookId, BSTR strBookName, LONG nViewProp, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(CREATE_BOOK,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",21);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("operFlag",nOperationFlag);
	objHttp.AddParam("bookId",nBookId);
	objHttp.AddParam("bookName",UTF8_Convert(WideStrToStr(strBookName).Trim(),TRUE));
	objHttp.AddParam("contPop",nViewProp);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DeleteBook(LONG nBookId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_BOOK_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",22);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("bookId",nBookId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetUserPictureList(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_PICTURE_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",30);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("beginTime",WideStrToStr(strBeginTime));
	objHttp.AddParam("endTime",WideStrToStr(strEndTime));

	CString strValid = objHttp.GetValidString();
	objHttp.AddParam("valid",strValid);

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::PutPictureToAlbum(LONG nPictureId, LONG nAlbumId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_PICTURE_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",34);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picId",nPictureId);
	objHttp.AddParam("albumId",nAlbumId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DeletePicture(BSTR strPictureId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_PICTURE_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",35);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picId",WideStrToStr(strPictureId));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetUserAlbumList(LONG nFriendId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_ALBUM_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",40);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::CreateAlbum(LONG nOperationFlag, LONG nAlbumId, BSTR strAlbumName, LONG nViewProp, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(CREATE_ALBUM,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",41);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("userNick",UTF8_Convert((LPCSTR)m_strNickName,TRUE));
	objHttp.AddParam("operFlag",nOperationFlag);
	objHttp.AddParam("albumId",nAlbumId);
	objHttp.AddParam("albumName",WideStrToStr(strAlbumName));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DeleteAlbum(LONG nAlbumId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(CREATE_ALBUM,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",42);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("albumId",nAlbumId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetUserArticleTagList(BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_ARTICLE_TAG_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",53);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetUserPictureTagList(BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_PICTURE_TAG_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",53);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetSpaceSize(BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SPACE_SIZE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",54);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::MiniOnline(void)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CString ss;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

    CString strUrl = (LPCSTR)pWebConfig->GetWebInterfaceUrl(MINI_ONLINE,L"");
	CString strParam;
	strParam.Format("userid=%d",m_nUserID);

	pWebConfig.Release();

	if ( hr_com==S_OK )
		::CoUninitialize();


	ss=UrlGetStr(strUrl+"?"+strParam);

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetPictureTitle(LONG nPictureId, BSTR strTitle, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_PICTURE_TITLE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",32);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picId",nPictureId);
	objHttp.AddParam("title",UTF8_Convert((LPCSTR)WideStrToStr(strTitle).Trim(),TRUE));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetPictureViewProp(LONG nPictureId, LONG nViewProp, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_PICTURE_VIEW_PROP,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",33);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picId",nPictureId);
	objHttp.AddParam("contPop",nViewProp);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SetUserFace(BSTR strPictureUrl, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SET_USER_FACE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",51);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picUrl",WideStrToStr(strPictureUrl));
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();


	return S_OK;
}

STDMETHODIMP CWebInterfaceX::PayPhotoFrame(LONG nFrameId, FLOAT* fPaied, BSTR* strError, VARIANT_BOOL* pReturnVal)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	//CHttpObject objHttp;

	//HRESULT hr_com = ::CoInitialize(0);

	//IWebConfigXPtr pWebConfig;
	//pWebConfig.CreateInstance(CLSID_WebConfigX);

	//objHttp.SetHttpPostType(hptMultipart_FormData);
	//objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(PAY_PHOTO_FRAME,L"") );

	//pWebConfig.Release();

	//if ( hr_com==S_OK )
	//	CoUninitialize();

	//objHttp.AddParam("actionCode",51);
	//objHttp.AddParam("userId",m_nUserID);
	//objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	//objHttp.AddParam("picUrl",WideStrToStr(strPictureUrl));
	//objHttp.AddParam("valid",objHttp.GetValidString());

	//*strReturn = objHttp.SendRequest().AllocSysString();



	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DelArticleInBook(LONG nArticleId, LONG nBookId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(DEL_ARTICLE_IN_BOOK,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",18);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("bookId",nBookId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::DelPictureInAlbum(LONG nPictureId, LONG nAlbumId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(DEL_PICTURE_IN_ALBUM,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",36);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("picId",nPictureId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetArticleBookList(LONG nArticleId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(ARTICLE_BOOK_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",19);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("articleId",nArticleId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::GetPictureAlbumList(LONG nPictureId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(PICTURE_ALBUM_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",18);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("pictureId",nPictureId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}

STDMETHODIMP CWebInterfaceX::SyncPicture(BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(SYNC_PICTURE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",75);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("albumId",0);
	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();


	return S_OK;
}

STDMETHODIMP CWebInterfaceX::EnablePicture(BSTR strPictureUrl, BSTR strTitle, BSTR strContent, BSTR strTag, LONG nViewProp, LONG nFileSize, LONG nAlbumId, BSTR* strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(ENABLE_PICTURE,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",31);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("operFlag",0);
	objHttp.AddParam("picId",0);
	objHttp.AddParam("userNick",(LPCSTR)UTF8_Convert((LPCSTR)m_strNickName,TRUE) );
	objHttp.AddParam("albumId",nAlbumId);
	objHttp.AddParam("title",(LPCSTR)UTF8_Convert(WideStrToStr(strTitle).Trim(),TRUE));
	objHttp.AddParam("content",(LPCSTR)UTF8_Convert(WideStrToStr(strContent).Trim(),TRUE));
	objHttp.AddParam("usrTag",(LPCSTR)UTF8_Convert(WideStrToStr(strTag).Trim(),TRUE));
	objHttp.AddParam("picUrl",WideStrToStr(strPictureUrl));
	objHttp.AddParam("contPop",nViewProp);
	objHttp.AddParam("from",4);
	objHttp.AddParam("fileSize",nFileSize);

	objHttp.AddParam("valid",objHttp.GetValidString());

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}


STDMETHODIMP CWebInterfaceX::GetUserAudioList(LONG nFriendId, BSTR strBeginTime, BSTR strEndTime, BSTR *strReturn)
{
	AFX_MANAGE_STATE(AfxGetStaticModuleState());

	CHttpObject objHttp;

	HRESULT hr_com = ::CoInitialize(0);

	IWebConfigXPtr pWebConfig;
	pWebConfig.CreateInstance(CLSID_WebConfigX);

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( (LPCSTR)pWebConfig->GetWebInterfaceUrl(USER_ARTICLE_LIST,L"") );

	pWebConfig.Release();

	if ( hr_com==S_OK )
		CoUninitialize();

	objHttp.AddParam("actionCode",89);
	objHttp.AddParam("userId",m_nUserID);
	objHttp.AddParam("userName",(LPCSTR)m_strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("beginTime",WideStrToStr(strBeginTime));
	objHttp.AddParam("endTime",WideStrToStr(strEndTime));

	CString strValid = objHttp.GetValidString();
	objHttp.AddParam("valid",strValid);

	*strReturn = objHttp.SendRequest().AllocSysString();

	return S_OK;
}