#include "StdAfx.h"
#include ".\webinterfacewrapper.h"

#pragma warning(disable:4996)

void wiLOG(CString str)
{
#ifdef _DEBUG
	try
	{
		CString strLogFile="D:\\wwww.txt";

		CStdioFile file;

		bool bOpenFlag = false;

		bOpenFlag = file.Open(strLogFile,CFile::typeText|CFile::modeCreate|CFile::modeWrite);
		
		if ( bOpenFlag )
		{
			file.SeekToEnd();
			file.WriteString(str);
			file.Close();
		}

	}
	catch(...)
	{
	}
#endif
}

void ParaseWebArticles(BSTR strWebResult, vector<TWebArticleInfo *> &info, CString & strServerDate, int nMaxCount)
{
	HRESULT  hr_com = CoInitialize(0);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strWebResult,&bSuccess);

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";
		CString strError;

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
					strError = W2A(error_msg);
				}
			}
		}

		if ( return_code==0 )
		{
			int article_count;

			strcpy(szItem,"result/articlelist/count");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				article_count = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/articlelist/servertime");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				strServerDate =  WideStrToStr(text);
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/articlelist");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				for ( int n=0; n<article_count; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes = NULL;

					sprintf(szItem,"article[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen ;
					pNodes->get_length(&nLen);

					if ( nLen>=10 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TWebArticleInfo * pW       = new TWebArticleInfo();
//						pW->uID                    = nFriendId;

						//文章标题
						pNodes->get_item(0,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szTitle,(LPCSTR)WideStrToStr(text),sizeof(pW->szTitle)-1);
						pItem.Release();

						//文章ID
						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						pW->dwArticleID = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//文集ID
						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						pW->dwDirID = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//发表途径
						pNodes->get_item(3,&pItem);
						pItem->get_text(&text);
						pW->dwFrom = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//摘要
						pNodes->get_item(4,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szOutlines,(LPCSTR)WideStrToStr(text),sizeof(pW->szOutlines)-1);
						pItem.Release();

						//查看权限
						pNodes->get_item(5,&pItem);
						pItem->get_text(&text);
						pW->dwViewType = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//发表日期
						pNodes->get_item(6,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(pW->szCreateDate)-1);
						pItem.Release();

						//最新修改日期
						pNodes->get_item(7,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(pW->szCreateDate)-1);
						pItem.Release();

						pW->dwCreateDate = DateStr2Long(pW->szCreateDate);

						//全文查看的url
						pNodes->get_item(8,&pItem);
						pItem->get_text(&text);

						CString strUrl = WideStrToStr(text);
						strUrl.Trim();

						if ( strUrl.Find("http://")==-1 )
						{
							strcpy(pW->szUrl,"http://");
							strncpy(pW->szUrl+7,(LPCSTR)strUrl,sizeof(pW->szUrl)-8);
						}
						else
						{
							strncpy(pW->szUrl,(LPCSTR)strUrl,sizeof(pW->szUrl)-1);
						}

						pItem.Release();

						//是否置顶
						pNodes->get_item(9,&pItem);
						pItem->get_text(&text);
						pW->dwTopFlag = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						info.push_back(pW);
					}

					pNodes.Release();

					if ( info.size()>nMaxCount )
					{
						break;
					}
				}
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	if ( hr_com==S_OK )
		CoUninitialize();
}



void ParaseWebAudios(BSTR strWebResult, vector<TWebAudioInfo *> &info, CString & strServerDate, int nMaxCount)
{
	HRESULT  hr_com = CoInitialize(0);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strWebResult,&bSuccess);

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";
		CString strError;

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
					strError = W2A(error_msg);
				}
			}
		}

		if ( return_code==0 )
		{
			int article_count;

			strcpy(szItem,"result/audiolist/count");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				article_count = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/audiolist/servertime");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				strServerDate =  WideStrToStr(text);
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/audiolist");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				for ( int n=0; n<article_count; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes = NULL;

					sprintf(szItem,"audio[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen ;
					pNodes->get_length(&nLen);

					if ( nLen>=8 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TWebAudioInfo * pW       = new TWebAudioInfo();
//						pW->uID                    = nFriendId;

						//歌曲名字
						pNodes->get_item(0,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szName,(LPCSTR)WideStrToStr(text),sizeof(pW->szName)-1);
						pItem.Release();

						//歌曲ID
						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						pW->dwAudioID = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//歌词
						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szLyric,(LPCSTR)WideStrToStr(text),sizeof(pW->szLyric)-1);
						pItem.Release();

						//专辑
						pNodes->get_item(3,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szSpecial,(LPCSTR)WideStrToStr(text),sizeof(pW->szSpecial)-1);
						pItem.Release();

						//歌手
						pNodes->get_item(4,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szSinger,(LPCSTR)WideStrToStr(text),sizeof(pW->szSinger)-1);
						pItem.Release();

						//查看权限
						pNodes->get_item(5,&pItem);
						pItem->get_text(&text);
						pW->dwViewType = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//发表日期
						pNodes->get_item(6,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(pW->szCreateDate)-1);
						pItem.Release();

						pW->dwCreateDate = DateStr2Long(pW->szCreateDate);

						//全文查看的url
						pNodes->get_item(7,&pItem);
						pItem->get_text(&text);

						CString strUrl = WideStrToStr(text);
						strUrl.Trim();

						if ( strUrl.Find("http://")==-1 )
						{
							strcpy(pW->szUrl,"http://");
							strncpy(pW->szUrl+7,(LPCSTR)strUrl,sizeof(pW->szUrl)-8);
						}
						else
						{
							strncpy(pW->szUrl,(LPCSTR)strUrl,sizeof(pW->szUrl)-1);
						}

						pItem.Release();

						info.push_back(pW);
					}

					pNodes.Release();

					if ( info.size()>nMaxCount )
					{
						break;
					}
				}
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	if ( hr_com==S_OK )
		CoUninitialize();
}


void ParaseWebPictures(BSTR strWebResult, vector<TWebPictureInfo *> &info, CString & strServerDate, int nMaxCount)
{
	HRESULT  hr_com = CoInitialize(0);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strWebResult,&bSuccess);

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
				}
			}
		}

		if ( return_code==0 )
		{
			int pic_count;

			strcpy(szItem,"result/piclist/count");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				pic_count = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/piclist/servertime");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				strServerDate = WideStrToStr(text);
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/piclist");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				for ( int n=0; n<pic_count; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes = NULL;

					sprintf(szItem,"pic[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen ;
					pNodes->get_length(&nLen);

					if ( nLen>=7 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TWebPictureInfo * pW       = new TWebPictureInfo();

						//图片标题
						pNodes->get_item(0,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szTitle,(LPCSTR)WideStrToStr(text),sizeof(pW->szTitle)-1);
						pItem.Release();

						//图片ID
						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						pW->dwPictureID = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//相册ID
						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);


						CString strAlbumId = WideStrToStr(text);
						int nTemp;
						if ( (nTemp=strAlbumId.Find(","))!=-1 )
							pW->nAlbumId = atoi((LPCSTR)strAlbumId.Left(nTemp));
						else
							pW->nAlbumId = atoi((LPCSTR)strAlbumId);
						pItem.Release();

						//图片上传途径
						pNodes->get_item(3,&pItem);
						pItem->get_text(&text);
						pW->dwFrom = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();


						//察看权限
						pNodes->get_item(4,&pItem);
						pItem->get_text(&text);
						pW->dwViewType = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						//发表日期
						pNodes->get_item(5,&pItem);
						pItem->get_text(&text);
						strncpy(pW->szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(pW->szCreateDate)-1);
						pItem.Release();

						pW->dwCreateDate = DateStr2Long(pW->szCreateDate);

						//全文查看的url
						pNodes->get_item(6,&pItem);
						pItem->get_text(&text);

						CString strUrl = WideStrToStr(text);
						strUrl.Trim();

						//if ( strUrl.Find("http://")==-1 )
						//{
						//	strcpy(pW->szUrl,"http://");
						//	strncpy(pW->szUrl+7,(LPCSTR)strUrl,sizeof(pW->szUrl)-8);
						//}
						//else
						//{
							strncpy(pW->szUrl,(LPCSTR)strUrl,sizeof(pW->szUrl)-1);
						//}

						pItem.Release();

						info.push_back(pW);
					}

					pNodes.Release();

					if ( info.size()>nMaxCount )
					{
						break;
					}
				}
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	if ( hr_com==S_OK )
		CoUninitialize();
}

CWebInterfaceWrapper::CWebInterfaceWrapper(void)
{
	m_pWebInterface = NULL;
	m_hrCom         = S_FALSE;
}

CWebInterfaceWrapper::~CWebInterfaceWrapper(void)
{
	if ( m_pWebInterface )
	{
		m_pWebInterface.Release();

		if ( m_hrCom==S_OK )
			::CoUninitialize();

		m_pWebInterface = NULL;
	}
}

void CWebInterfaceWrapper::InitInterface(long nUserId, CString strUserName, CString strNickName)
{
	if ( m_pWebInterface==NULL )
	{
		m_hrCom = ::CoInitialize(0);
		m_pWebInterface.CreateInstance(CLSID_WebInterfaceX);

		if ( m_pWebInterface )
			m_pWebInterface->SetUserInfo(nUserId,strUserName.AllocSysString(),strNickName.AllocSysString());
	}
}

void CWebInterfaceWrapper::GetUserArticleList(long nFriendId, CString strBeginTime, CString strEndTime, vector<TWebArticleInfo *> &info, CString & strServerDate)
{
	BSTR strReturn = L"";
	
	if ( m_pWebInterface )
	{
		m_pWebInterface->raw_GetUserArticleList(nFriendId,strBeginTime.AllocSysString(),strEndTime.AllocSysString(),&strReturn);
		
		try
		{
			ParaseWebArticles(strReturn,info,strServerDate);
		}
		catch(...)
		{
		}
	}
}

void CWebInterfaceWrapper::GetUserAudioList(long nFriendId, CString strBeginTime, CString strEndTime, vector<TWebAudioInfo *> &info, CString & strServerDate)
{
	BSTR strReturn = L"";
	
	if ( m_pWebInterface )
	{
		m_pWebInterface->raw_GetUserAudioList(nFriendId,strBeginTime.AllocSysString(),strEndTime.AllocSysString(),&strReturn);
		
		try
		{
			ParaseWebAudios(strReturn,info,strServerDate);
		}
		catch(...)
		{
		}
	}
}


void CWebInterfaceWrapper::GetUserArticleTagList(vector<TTagInfo> &info)
{
	BSTR strReturn = L"";
	
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
				}
			}
		}

		if ( return_code==0 )
		{
			int tag_count;

			strcpy(szItem,"result/taglist/count");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				tag_count = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/taglist");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				for ( int n=0; n<tag_count; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes = NULL;

					sprintf(szItem,"tag[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen ;
					pNodes->get_length(&nLen);

					if ( nLen>=2 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TTagInfo	         ti;

						pNodes->get_item(0,&pItem);
						pItem->get_text(&text);
						strncpy(ti.szTagName,(LPCSTR)WideStrToStr(text),sizeof(ti.szTagName)-1);
						pItem.Release();

						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						ti.nTagId = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						info.push_back(ti);
					}

					pNodes.Release();
				}
			}

		}
	}
	
	if ( pDoc )
		pDoc.Release();
}

CString CWebInterfaceWrapper::GetArticleContent(long nArticleId)
{
	if ( m_pWebInterface==NULL )
	{
		return "";
	}

	BSTR strReturn  = L"";
	m_pWebInterface->raw_GetArticleContent(nArticleId,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	CString strArticleContent;

	try
	{
		pDoc->loadXML(strReturn,&bSuccess);
	}
	catch(...)
	{
	}

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
				}
			}
		}

		if ( return_code==0 )
		{
			strcpy(szItem,"result/article/content");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			pNode->get_text(&text);
			strArticleContent = WideStrToStr(text);
			pNode.Release();
			pNode = NULL;
		}
	}

	if ( pDoc )
		pDoc.Release();

	return strArticleContent;
}

long CWebInterfaceWrapper::CreateArticle(long nOperationFlag, long nArticleId, CString strTitle, CString strContent, long nBookId, long nViewProp, CString strTag, long nTopFlag, CString & strError )
{
	if ( m_pWebInterface==NULL )
	{
		return 0;
	}

	CString strBody;
	GetSectionValue(strContent,"<BODY>","</BODY>",strBody);

	BSTR strReturn = L"";
	m_pWebInterface->raw_CreateArticle(nOperationFlag,nArticleId,strTitle.AllocSysString(),strBody.AllocSysString(),nBookId,nViewProp,strTag.AllocSysString(),nTopFlag,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	long nReturnArticleId = 0;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
		}

		if ( return_code==0 )
		{
			strcpy(szItem,"result/article_id");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				nReturnArticleId = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}
		}

	}

	if ( pDoc )
		pDoc.Release();

	return nReturnArticleId;
}

bool CWebInterfaceWrapper::SetArticleTop(long nArticleId, long nTopFlag, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}


	BSTR strReturn = m_pWebInterface->SetArticleTop(nArticleId,nTopFlag);

	CComPtr<IXMLDOMDocument> pDoc;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

bool CWebInterfaceWrapper::SetArticleTitle(long nArticleId, CString strTitle, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}


	BSTR strReturn = m_pWebInterface->SetArticleTitle(nArticleId,strTitle.AllocSysString());

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

bool CWebInterfaceWrapper::SetArticleViewProp(long nArticleId, long nViewProp, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->SetArticleViewProp(nArticleId,nViewProp);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}


bool CWebInterfaceWrapper::PutArticleToBook(long nArticleId, CString strBookIds, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->PutArticleToBook(nArticleId,strBookIds.AllocSysString());

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}


bool CWebInterfaceWrapper::DelArticleInBook(long nArticleId, long nBookId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->DelArticleInBook(nArticleId,nBookId);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

bool CWebInterfaceWrapper::DeleteArticle(CString strArticleId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->DeleteArticle(strArticleId.AllocSysString());

	wiLOG(WideStrToStr(strReturn));

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

void CWebInterfaceWrapper::GetArticleBookList(long nArticleId, TArticleBookInfo &info)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
	
	m_pWebInterface->raw_GetArticleBookList(nArticleId,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	USES_CONVERSION;

	info.uArticleId = nArticleId;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				strcpy(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					//strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				strcpy(szItem,"result/booklist/count");
				pDoc->selectSingleNode(A2W(szItem),&pNode);
				pNode->get_text(&text);

				int nBookCount = atoi((LPCSTR)WideStrToStr(text));

				pNode.Release();
				strcpy(szItem,"result/booklist");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				for ( int n=0; n<nBookCount; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes;

					sprintf(szItem,"book[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen;
					pNodes->get_length(&nLen);

					if ( nLen==3 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TBookInfo            bookInfo;

						pNodes->get_item(0,&pItem);//book  name

						pItem->get_text(&text);
						strncpy(bookInfo.szBookName,(LPCSTR)WideStrToStr(text),sizeof(bookInfo.szBookName)-1);

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						bookInfo.nBookId = atoi((LPCSTR)WideStrToStr(text));

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						strncpy(bookInfo.szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(bookInfo.szCreateDate)-1);

						pItem.Release();
						pItem = NULL;

						info.vecBook.push_back(bookInfo);
					}

					pNodes.Release();
				}

			}
		}
	}

	if ( pDoc )
		pDoc.Release();
}

void CWebInterfaceWrapper::GetUserBookList(long nFriendId,TUserBookInfo &info)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
	
	m_pWebInterface->raw_GetUserBookList(nFriendId,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	USES_CONVERSION;

	info.uID = nFriendId;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				strcpy(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					//strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				strcpy(szItem,"result/booklist/count");
				pDoc->selectSingleNode(A2W(szItem),&pNode);
				pNode->get_text(&text);

				int nBookCount = atoi((LPCSTR)WideStrToStr(text));

				pNode.Release();
				strcpy(szItem,"result/booklist");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				for ( int n=0; n<nBookCount; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes;

					sprintf(szItem,"book[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen;
					pNodes->get_length(&nLen);

					if ( nLen==3 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TBookInfo            bookInfo;

						pNodes->get_item(0,&pItem);//book  name

						pItem->get_text(&text);
						strncpy(bookInfo.szBookName,(LPCSTR)WideStrToStr(text),sizeof(bookInfo.szBookName)-1);

						pItem.Release();
						pItem = NULL;
	
						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						bookInfo.nBookId = atoi((LPCSTR)WideStrToStr(text));

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						strncpy(bookInfo.szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(bookInfo.szCreateDate)-1);

						pItem.Release();
						pItem = NULL;

						info.vecBook.push_back(bookInfo);
					}

					pNodes.Release();
				}

			}
		}
	}

	if ( pDoc )
		pDoc.Release();
}

long CWebInterfaceWrapper::CreateBook(long nOperationFlag, long nBookId, CString strBookName, long nViewProp, CString & strError )
{
	if ( m_pWebInterface==NULL )
	{
		return 0;
	}

	BSTR strReturn = m_pWebInterface->CreateBook(nOperationFlag,nBookId,strBookName.AllocSysString(),nViewProp);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	long nReturnBookId = 0;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
		}

		if ( return_code==0 )
		{
			strcpy(szItem,"result/book_id");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			pNode->get_text(&text);
			nReturnBookId = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;
		}
	}

	if ( pDoc )
		pDoc.Release();

	return nReturnBookId;
}


bool CWebInterfaceWrapper::DeleteBook(CString strBookId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->DeleteBook(atol((LPCSTR)strBookId));

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

void CWebInterfaceWrapper::GetUserPictureList(long nFriendId, CString strBeginTime, CString strEndTime,vector<TWebPictureInfo *> &info, CString & strServerDate)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
	m_pWebInterface->raw_GetUserPictureList(nFriendId,strBeginTime.AllocSysString(),strEndTime.AllocSysString(), &strReturn);
	
	try
	{
		ParaseWebPictures(strReturn,info,strServerDate);
	}
	catch(...)
	{
	}
}

void CWebInterfaceWrapper::GetUserPictureTagList(vector<TTagInfo> &info)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
    	
	m_pWebInterface->raw_GetUserPictureTagList(&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);


	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
				}
			}
		}

		if ( return_code==0 )
		{
			int tag_count;

			strcpy(szItem,"result/taglist/count");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				pNode->get_text(&text);
				tag_count = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;
			}

			strcpy(szItem,"result/taglist");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			if ( pNode )
			{
				for ( int n=0; n<tag_count; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes = NULL;

					sprintf(szItem,"tag[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen ;
					pNodes->get_length(&nLen);

					if ( nLen>=2 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TTagInfo	         ti;

						pNodes->get_item(0,&pItem);
						pItem->get_text(&text);
						strncpy(ti.szTagName,(LPCSTR)WideStrToStr(text),sizeof(ti.szTagName)-1);
						pItem.Release();

						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						ti.nTagId = atoi((LPCSTR)WideStrToStr(text));
						pItem.Release();

						info.push_back(ti);
					}

					pNodes.Release();
				}
			}

		}
	}

	if ( pDoc )
		pDoc.Release();
}


bool CWebInterfaceWrapper::UploadFace(CString strPictureFile,CString & strPictureUrl, CString & strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR bstrPictureUrl = L"";
	BSTR bstrError      = L"";

	long nFileSize ;

	m_pWebInterface->UploadPicture(strPictureFile.AllocSysString(),&nFileSize, &bstrPictureUrl,&bstrError);

	strPictureUrl = WideStrToStr(bstrPictureUrl);
	strError      = WideStrToStr(bstrError);

	strPictureUrl.Trim();

	bool bResult  = false;

	BSTR strReturn ;

	if ( strPictureUrl!="" )
	{
		strReturn= m_pWebInterface->SetUserFace(bstrPictureUrl);

		CComPtr<IXMLDOMDocument> pDoc = NULL;
		pDoc.CoCreateInstance(__uuidof(DOMDocument));

		VARIANT_BOOL bSuccess=VARIANT_FALSE;

		pDoc->loadXML(strReturn,&bSuccess);

		bool bReturn=false;

		USES_CONVERSION;

		if ( bSuccess==VARIANT_TRUE )
		{
			char szItem[255];

			CComPtr<IXMLDOMNodeList> pNodes = NULL;
			CComPtr<IXMLDOMNode> pNode = NULL;

			BSTR text=L"";

			strcpy(szItem,"result/return_code");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			int  return_code = -1;

			if (pNode)
			{
				pNode->get_text(&text);
				return_code = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;

				if ( return_code!=0 )
				{
					sprintf(szItem,"result/error_msg");
					pDoc->selectSingleNode(A2W(szItem),&pNode);

					if ( pNode )
					{
						pNode->get_text(&text);
						pNode.Release();
						pNode = NULL;

						strError = WideStrToStr(text);
					}
				}	
				else
				{
					bResult = true;
				}
			}
		}

		if ( pDoc )
			pDoc.Release();
	}


	return bResult;
}


long CWebInterfaceWrapper::UploadPicture(CString strPictureFile, CString strTitle, CString strContent, CString strTag, int nViewProp, int nAlbumID, CString &strPictureUrl,CString & strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR bstrPictureUrl = L"";
	BSTR bstrError      = L"";

	long nPictureId = 0;
	long nFileSize ;
	
	m_pWebInterface->UploadPicture(strPictureFile.AllocSysString(),&nFileSize, &bstrPictureUrl,&bstrError);

	strPictureUrl = WideStrToStr(bstrPictureUrl);
	strError      = WideStrToStr(bstrError);

	strTitle.Trim();

	if ( strTitle=="" )
		strTitle = GetFileNameFromPath(strPictureFile);

	if ( strTitle.GetLength()>30 )
	{
		strTitle=strTitle.Left(30);
	}

	strPictureUrl.Trim();
	BSTR strReturn = L"";

	if ( strPictureUrl!="" )
	{
		m_pWebInterface->raw_EnablePicture(bstrPictureUrl,
			strTitle.AllocSysString(),
			strContent.AllocSysString(), 
			strTag.AllocSysString(),
			nViewProp,nFileSize,nAlbumID,
			&strReturn);
		
		CComPtr<IXMLDOMDocument> pDoc = NULL;
		pDoc.CoCreateInstance(__uuidof(DOMDocument));

		VARIANT_BOOL bSuccess=VARIANT_FALSE;

		pDoc->loadXML(strReturn,&bSuccess);

		bool bReturn=false;

		USES_CONVERSION;

		if ( bSuccess==VARIANT_TRUE )
		{
			char szItem[255];

			CComPtr<IXMLDOMNodeList> pNodes = NULL;
			CComPtr<IXMLDOMNode> pNode = NULL;

			BSTR text=L"";

			strcpy(szItem,"result/return_code");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			int  return_code = -1;

			if (pNode)
			{
				pNode->get_text(&text);
				return_code = atoi((LPCSTR)WideStrToStr(text));
				pNode.Release();
				pNode = NULL;

				if ( return_code!=0 )
				{
					sprintf(szItem,"result/error_msg");
					pDoc->selectSingleNode(A2W(szItem),&pNode);

					if ( pNode )
					{
						pNode->get_text(&text);
						pNode.Release();
						pNode = NULL;

						strError = WideStrToStr(text);
					}
				}
				else
				{
					sprintf(szItem,"result/picture_id");
					pDoc->selectSingleNode(A2W(szItem),&pNode);

					if ( pNode )
					{
						pNode->get_text(&text);
						pNode.Release();
						pNode = NULL;

						nPictureId = atol((LPCSTR)WideStrToStr(text));
					}
				}
			}
 		}

		if ( pDoc )
			pDoc.Release();
	}
	

	return nPictureId;
}


bool CWebInterfaceWrapper::SetPictureTitle(long nPictureId, CString strTitle, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->SetPictureTitle(nPictureId,strTitle.AllocSysString());

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

bool CWebInterfaceWrapper::SetPictureViewProp(long nPictureId, long nViewProp, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->SetPictureViewProp(nPictureId,nViewProp);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}


bool CWebInterfaceWrapper::PutPictureToAlbum(long nPictureId, long nAlbumId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->PutPictureToAlbum(nPictureId,nAlbumId);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}


bool CWebInterfaceWrapper::DelPictureInAlbum(long nPictureId, long nAlbumId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	return true;
}

bool CWebInterfaceWrapper::DeletePicture(CString strPictureId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->DeletePicture(strPictureId.AllocSysString());

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

void CWebInterfaceWrapper::GetPictureAlbumList(long nPictureId, TPicturelbumInfo &info)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
	
	m_pWebInterface->raw_GetPictureAlbumList(nPictureId,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	USES_CONVERSION;

	info.uPictureId = nPictureId;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				strcpy(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					//strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				strcpy(szItem,"result/albumlist/count");
				pDoc->selectSingleNode(A2W(szItem),&pNode);
				pNode->get_text(&text);

				int nBookCount = atoi((LPCSTR)WideStrToStr(text));

				pNode.Release();
				strcpy(szItem,"result/albumlist");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				for ( int n=0; n<nBookCount; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes;

					sprintf(szItem,"album[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen;
					pNodes->get_length(&nLen);

					if ( nLen==3 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TAlbumInfo           albumInfo;

						pNodes->get_item(0,&pItem);//album  name

						pItem->get_text(&text);
						strncpy(albumInfo.szAlbumName,(LPCSTR)WideStrToStr(text),sizeof(albumInfo.szAlbumName)-1);

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						albumInfo.nAlbumId = atoi((LPCSTR)WideStrToStr(text));

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						strncpy(albumInfo.szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(albumInfo.szCreateDate)-1);

						pItem.Release();
						pItem = NULL;

						info.vecAlbum.push_back(albumInfo);
					}

					pNodes.Release();
				}
			}
		}
	}

	if ( pDoc )
		pDoc.Release();
}

void CWebInterfaceWrapper::GetUserAlbumList(long nFriendId,TUserAlbumInfo &info)
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	BSTR strReturn = L"";
	
	m_pWebInterface->raw_GetUserAlbumList(nFriendId,&strReturn);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	USES_CONVERSION;

	info.uID = nFriendId;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				strcpy(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					//strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				strcpy(szItem,"result/albumlist/count");
				pDoc->selectSingleNode(A2W(szItem),&pNode);
				pNode->get_text(&text);

				int nBookCount = atoi((LPCSTR)WideStrToStr(text));

				pNode.Release();
				strcpy(szItem,"result/albumlist");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				for ( int n=0; n<nBookCount; n++ )
				{
					CComPtr<IXMLDOMNodeList> pNodes;

					sprintf(szItem,"album[%d]/*",n);
					pNode->selectNodes(A2W(szItem),&pNodes);

					long nLen;
					pNodes->get_length(&nLen);

					if ( nLen==3 )
					{
						CComPtr<IXMLDOMNode> pItem = NULL;
						TAlbumInfo           albumInfo;

						pNodes->get_item(0,&pItem);//album  name

						pItem->get_text(&text);
						strncpy(albumInfo.szAlbumName,(LPCSTR)WideStrToStr(text),sizeof(albumInfo.szAlbumName)-1);

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(1,&pItem);
						pItem->get_text(&text);
						albumInfo.nAlbumId = atoi((LPCSTR)WideStrToStr(text));

						pItem.Release();
						pItem = NULL;

						pNodes->get_item(2,&pItem);
						pItem->get_text(&text);
						strncpy(albumInfo.szCreateDate,(LPCSTR)WideStrToStr(text),sizeof(albumInfo.szCreateDate)-1);

						pItem.Release();
						pItem = NULL;

						info.vecAlbum.push_back(albumInfo);
					}

					pNodes.Release();
				}
			}
		}
	}

	if ( pDoc )
		pDoc.Release();
}


long CWebInterfaceWrapper::CreateAlbum(long nOperationFlag, long nAlbumId, CString strAlbumName, long nViewProp, CString & strError )
{
	if ( m_pWebInterface==NULL )
	{
		return 0;
	}

	BSTR strReturn = m_pWebInterface->CreateAlbum(nOperationFlag,nAlbumId,strAlbumName.AllocSysString(),nViewProp);

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	long nReturnAlbumId = 0;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
		}

		if ( return_code==0 )
		{
			strcpy(szItem,"result/album_id");
			pDoc->selectSingleNode(A2W(szItem),&pNode);

			pNode->get_text(&text);
			nReturnAlbumId = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;
		}
	}

	if ( pDoc )
		pDoc.Release();

	return nReturnAlbumId;
}


bool CWebInterfaceWrapper::DeleteAlbum(CString strAlbumId, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->DeleteAlbum(atol((LPCSTR)strAlbumId));

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}


bool CWebInterfaceWrapper::SetUserFace(CString strPictureUrl, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	BSTR strReturn = m_pWebInterface->SetUserFace(strPictureUrl.AllocSysString());

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	bool bReturn=false;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;

					strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				bReturn = true;
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return bReturn;
}

long CWebInterfaceWrapper::GetSpaceSize()
{
	if ( m_pWebInterface==NULL )
	{
		return 0;
	}

	BSTR strReturn = m_pWebInterface->GetSpaceSize();

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn,&bSuccess);

	long nSpaceSize=0;

	USES_CONVERSION;

	if ( bSuccess==VARIANT_TRUE )
	{
		char szItem[255];

		CComPtr<IXMLDOMNodeList> pNodes = NULL;
		CComPtr<IXMLDOMNode> pNode = NULL;

		BSTR text=L"";

		strcpy(szItem,"result/return_code");
		pDoc->selectSingleNode(A2W(szItem),&pNode);

		int  return_code = -1;
		BSTR error_msg   = L"";

		if (pNode)
		{
			pNode->get_text(&text);
			return_code = atoi((LPCSTR)WideStrToStr(text));
			pNode.Release();
			pNode = NULL;

			if ( return_code!=0 )
			{
				sprintf(szItem,"result/error_msg");
				pDoc->selectSingleNode(A2W(szItem),&pNode);

				if ( pNode )
				{
					pNode->get_text(&error_msg);
					pNode.Release();
					pNode = NULL;
					//strError = WideStrToStr(error_msg);
				}
			}
			else
			{
				sprintf(szItem,"result/space/size");
				pDoc->selectSingleNode(A2W(szItem),&pNode);
				if ( pNode )
				{
					pNode->get_text(&text);
					pNode.Release();
					pNode = NULL;

					nSpaceSize = atol((LPCSTR)WideStrToStr(text));
				}				
			}
		}
	}

	if ( pDoc )
		pDoc.Release();

	return nSpaceSize;
}

void CWebInterfaceWrapper::MiniOnline()
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	m_pWebInterface->MiniOnline();
}

void CWebInterfaceWrapper::SyncPicture()
{
	if ( m_pWebInterface==NULL )
	{
		return ;
	}

	m_pWebInterface->SyncPicture();
}

bool CWebInterfaceWrapper::PayPhotoFrame(long nFrameId,float & fPaied, CString &strError)
{
	if ( m_pWebInterface==NULL )
	{
		return false;
	}

	VARIANT_BOOL bRet ;
	BSTR         bstrError = L"";

	bRet = m_pWebInterface->PayPhotoFrame(nFrameId,(FLOAT *)&fPaied,&bstrError);

	strError = WideStrToStr(bstrError);

	if ( bRet==VARIANT_TRUE )
		return true;
	else
		return false;
}
