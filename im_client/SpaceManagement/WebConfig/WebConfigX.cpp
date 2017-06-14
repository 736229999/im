// WebConfigX.cpp : CWebConfigX 的实现

#include "stdafx.h"
#include "WebConfigX.h"
#include ".\webconfigx.h"
#include <WinInet.h>

bool CWebConfigX::m_bInit=false;

CString CWebConfigX::strPhpAPI;
CString CWebConfigX::strMiniOnline;
CString CWebConfigX::strUploadPictureASPX;
CString CWebConfigX::strFixUserNameASPX;
CString CWebConfigX::strMagicFaceASPX;
CString CWebConfigX::strPhotoFrameASPX;
CString CWebConfigX::strZoneURL;
CString CWebConfigX::strAccountURL;
CString CWebConfigX::strUserHomeURL;
CString CWebConfigX::strUserPhotoURL;
CString CWebConfigX::strUserArticleURL;
CString CWebConfigX::strArticleEmotionURL;
CString CWebConfigX::strArticlePictureURL;
CString CWebConfigX::strFamilyHomeURL;
CString CWebConfigX::strFamilyDiscussURL;
CString CWebConfigX::strFamilyAlbumURL;
CString CWebConfigX::strFamilySettingURL;
CString CWebConfigX::strWebFaceURL;
CString CWebConfigX::strWebChanelURL;
CString CWebConfigX::strWokuShopURL;
CString CWebConfigX::strPicLoginUrl;
CString CWebConfigX::strAdvertiseURL;//广告链接
CString CWebConfigX::strJifenURL;//积分中心
CString CWebConfigX::strUploadMediaUrl;

CCriticalSection  CWebConfigX::m_secLock;
// CWebConfigX

/////////////////////////////////////////////////////////////////////////////////////////
//  UrlGetStr
//  功能：用HTTP GET获取一个web页面的HTML代码
//  输入：lpszUrl,URL
//  返回值：返回的HTML代码
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
CString UrlGetStr(LPCSTR lpszUrl)
{
	HINTERNET hFile, hInet;
	char buffer[8193];
	DWORD dwRead, dwSize;

	CString strResult="";

	hInet = InternetOpen("xTreamLiveUpdate",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, lpszUrl, NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT ,//| INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{
			try
			{
				dwSize = sizeof(buffer)-1;

				while ( InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
				{
					buffer[dwRead]=0;
					strResult=strResult+buffer;
				}
			}
			catch(...)
			{
			}
			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInet);
	}

	return strResult;
}



UINT CWebConfigX::InitProc(LPVOID param)
{
	CWebConfigX *pX = (CWebConfigX *)param;
	::CoInitialize(0);
	pX->DoInit();
	::CoUninitialize();

	return 0;
}

void CWebConfigX::DoInit()
{
	CString strConfigFile;
	char szSystemDir[MAX_PATH]={0};
	GetSystemDirectory(szSystemDir, MAX_PATH-1);
	strConfigFile.Format("%s\\woku_url.xml",szSystemDir);

	HANDLE hFileRead;
	hFileRead = CreateFile(strConfigFile, GENERIC_READ, 0, NULL, OPEN_EXISTING, 0, 0);

	if (hFileRead != INVALID_HANDLE_VALUE)
	{
		char    szBuff[1025]={0};
		CString sText = "";

		DWORD   dwRead;
		while(ReadFile(hFileRead,szBuff,1024,&dwRead,NULL))
		{
			if ( dwRead>0 )
			{
				szBuff[dwRead]='\0';
				sText = sText+szBuff;
			}
			else
			{
				break;
			}
		}

		ParaseUrl(sText);
		CloseHandle(hFileRead);
	}

	
	CString strRet = UrlGetStr("http://api.woku.com/api/miniinterface.xml");

	strRet.Trim();

	if ( strRet!="" )
	{
		ParaseUrl(strRet);

		DeleteFile(strConfigFile);

		HANDLE hFileWrite;
		hFileWrite = CreateFile(strConfigFile, GENERIC_WRITE, 0, NULL, CREATE_NEW, 0, 0);

		if (hFileWrite != INVALID_HANDLE_VALUE)
		{
			DWORD dwWrite;
			WriteFile(hFileWrite,(LPCSTR)strRet,strRet.GetLength()+1,&dwWrite,NULL);
			CloseHandle(hFileWrite);
		}	
	}
}

void CWebConfigX::ParaseUrl(CString strXml)
{
	m_secLock.Enter();

	CComPtr<IXMLDOMDocument> pDoc;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	BSTR bstr = strXml.AllocSysString();
	pDoc->loadXML(bstr,&bSuccess);
	::SysFreeString(bstr);

	char szItem[255];
	int  nItem=0;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{		
		CComPtr<IXMLDOMNode> pNode = NULL;

		while(1)
		{
			sprintf(szItem,"interfaceUrl/item[%d]",nItem++);
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				CComPtr<IXMLDOMNode> pNode2 = NULL;
				CComPtr<IXMLDOMNamedNodeMap> pAttr = NULL;

				BSTR itemName=L"", itemValue=L"";

				pNode->get_attributes(&pAttr);

				if ( pAttr )
					pAttr->get_item(0,&pNode2);

				if ( pNode2 )
					pNode2->get_text(&itemName);

				if ( pAttr==NULL || pNode2==NULL )
				{
					break;
				}

				pNode2.Release();
				pNode2 = NULL;

				pAttr->get_item(1,&pNode2);

				pNode2->get_text(&itemValue);

				CString strType = W2A(itemName);
				CString strUrl  = W2A(itemValue);

				strType.Trim();
				strType.MakeLower();

				strUrl.Trim();
				strUrl.MakeLower();

				if ( strType==CString("spaceApi").MakeLower() )
				{
					strPhpAPI = strUrl;
				}
				else
				if ( strType==CString("webUserIndex").MakeLower() )
				{
					this->strUserHomeURL = strUrl;
				}
				else
				if ( strType==CString("webUserZone").MakeLower() )
				{
					this->strZoneURL = strUrl;
				}
				else
				if ( strType==CString("wokuShop").MakeLower() )
				{
					this->strWokuShopURL = strUrl;
				}
				else
				if ( strType==CString("webUserAccount").MakeLower() )
				{
					this->strAccountURL = strUrl;
				}
				else
				if ( strType==CString("webUserPhoto").MakeLower() )
				{
					this->strUserPhotoURL = strUrl;
				}
				else
				if ( strType==CString("webUserArticle").MakeLower() )
				{
					this->strUserArticleURL = strUrl;
				}
				else
				if ( strType==CString("upLoadPicture").MakeLower() )
				{
					strUploadPictureASPX = strUrl;
				}
				else
				if ( strType==CString("webArticleFace").MakeLower() )//写文章表情链接
				{
					strArticleEmotionURL = strUrl;
				}
				else
				if ( strType==CString("webArticlePicture").MakeLower() )//写文章已上传图片url
				{
					strArticlePictureURL = strUrl;
				}
				else
				if ( strType==CString("webMagicFace").MakeLower() )
				{
					strMagicFaceASPX = strUrl;
				}
				else
				if ( strType==CString("webHeadPhoto").MakeLower() )
				{
					this->strPhotoFrameASPX = strUrl;
				}
				else if(strType==CString("fixUserName").MakeLower())
				{
					strFixUserNameASPX = strUrl;
				}
				else
				if ( strType==CString("familyIndex").MakeLower() )
				{
				this->strFamilyHomeURL = strUrl;
				}
				else
				if ( strType==CString("familyDiscuss").MakeLower() )
				{
					this->strFamilyDiscussURL = strUrl;
				}
				else
				if ( strType==CString("familySetting").MakeLower() )
				{
					this->strFamilySettingURL = strUrl;
				}
				else
				if ( strType==CString("familyAlbum").MakeLower() )
				{
					this->strFamilyAlbumURL = strUrl;
				}
				else
				if ( strType==CString("webFace").MakeLower() )
				{
					strWebFaceURL = strUrl;
				}
				else
				if ( strType==CString("picMiniLogin").MakeLower() )
				{
					strPicLoginUrl = strUrl;
				}
				else
				if ( strType==CString("miniChanel").MakeLower() )//
				{
					strWebChanelURL = strUrl;
				}
				else
				if ( strType==CString("miniAdvertise").MakeLower() )
				{
					strAdvertiseURL = strUrl;
				}
				else
				if ( strType==CString("miniOnline").MakeLower() )
				{
					strMiniOnline = strUrl;
				}
				else
				if ( strType==CString("jifen").MakeLower() )
				{
					strJifenURL = strUrl;
				}
				pNode.Release();
				pNode = NULL;
			}
			else
			{
				break;
			}
		}
	}

	m_secLock.Leave();

	pDoc.Release();
}

STDMETHODIMP CWebConfigX::GetWebInterfaceUrl(WebInterfaceType type, BSTR param, BSTR* pVal)
{
	m_secLock.Enter();

	CString strUrl="";

	USES_CONVERSION;

	switch(type)
	{
	case UPLOAD_PICTURE:
		strUrl = strUploadPictureASPX;
		break;
	case MINI_ONLINE:
		strUrl = strMiniOnline;
		break;
	case ENABLE_PICTURE:
	case SET_USER_FACE:
	case USER_ARTICLE_LIST:
	case SYNC_PICTURE:
	case ARTICLE_CONTENT:
	case CREATE_ARTICLE:
	case SET_ARTICLE_TOP:
	case SET_ARTICLE_TITLE:
	case SET_ARTICLE_VIEW_PROP:
	case PUT_ARTICLE_TO_BOOK:
	case DEL_ARTICLE_IN_BOOK:
	case DELETE_ARTICLE:
	case USER_BOOK_LIST:
	case ARTICLE_BOOK_LIST:
	case CREATE_BOOK:
	case DELETE_BOOK:
	case USER_PICTURE_LIST:
	case SET_PICTURE_TITLE:
	case SET_PICTURE_VIEW_PROP:
	case PUT_PICTURE_TO_ALBUM:
	case DEL_PICTURE_IN_ALBUM:
	case USER_ALBUM_LIST:
	case PICTURE_ALBUM_LIST:
	case CREATE_ALBUM:
	case DELETE_ALBUM:
	case USER_ARTICLE_TAG_LIST:
	case USER_PICTURE_TAG_LIST:
	case SPACE_SIZE:
		strUrl = strPhpAPI;
		break;
	}

	*pVal = strUrl.AllocSysString();

	m_secLock.Leave();

	return S_OK;
}

STDMETHODIMP CWebConfigX::GetWebPageUrl(WebPageType type, BSTR param, BSTR* pVal)
{
	m_secLock.Enter();

	CString strUrl="";

	USES_CONVERSION;

	switch(type)
	{
	case MINI_LOGIN_PICTURE:
		strUrl = strPicLoginUrl;
		break;
	case WOKU_SHOP:
		strUrl = strWokuShopURL;
		break;
	case MAGIC_FACE:
		strUrl.Format("%s?userid=%s",(LPCSTR)strMagicFaceASPX,W2A(param));
		break;
	case PHOTO_FRAME:
		strUrl = strPhotoFrameASPX;
		break;
	case FIX_USERNAME:
		strUrl = strFixUserNameASPX;
		break;
	case USER_ACCOUNT:
		strUrl = strAccountURL;
		strUrl.Replace("[username]",W2A(param));
		break;
	case USER_ZONE:
		strUrl = strZoneURL;
		strUrl.Replace("[username]",W2A(param));
		break;
	case USER_HOME:
		strUrl = strUserHomeURL;
		strUrl.MakeLower();
		strUrl.Replace("[username]",W2A(param));
		break;
	case USER_PHOTO:
		strUrl = strUserPhotoURL;
		strUrl.Replace("[username]",W2A(param));
		break;
	case USER_ARTICLE:
		strUrl = strUserArticleURL;
		strUrl.Replace("[username]",W2A(param));
		break;
	case ARTICLE_EMOTION:
		strUrl = strArticleEmotionURL;
		strUrl.Replace("[username]",W2A(param));
		break;
	case FAMILY_ALBUM:
		strUrl = strFamilyAlbumURL;
		strUrl.Replace("[familydomain]",W2A(param));
		break;
	case FAMILY_DISCUSS:
		strUrl = strFamilyDiscussURL;
		strUrl.Replace("[familydomain]",W2A(param));
		break;
	case FAMILY_HOME:
		strUrl = strFamilyHomeURL;
		strUrl.Replace("[familydomain]",W2A(param));
		break;
	case FAMILY_SETTING:
		strUrl = strFamilySettingURL;
		strUrl.Replace("[familydomain]",W2A(param));
		break;
	case MINI_WEB_FACE_PATH:
		strUrl = strWebFaceURL;
		break;
	case MINI_WEB_CHANEL:
		strUrl = strWebChanelURL;
		break;
	case MINI_ADVERTISE:
		strUrl = strAdvertiseURL;
		break;
	case JIFEN_CENTER:
		strUrl = strJifenURL;
		break;
	case UPLOAD_MEDIA:
		strUrl = strUploadMediaUrl;
		strUrl.Replace("[username]",W2A(param));
		break;
	}

	*pVal = strUrl.AllocSysString();

	m_secLock.Leave();

	return S_OK;
}
