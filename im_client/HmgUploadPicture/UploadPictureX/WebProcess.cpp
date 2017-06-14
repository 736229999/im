#include "..\stdafx.h"
#include ".\webprocess.h"
#include "HttpObject.h"

#define PHP_API_URL "http://api.woku.com/api/index.php"

CString GetServerName(CString strWebLocation)
{
	int pos = strWebLocation.Find("/",0);
	return strWebLocation.Mid(0,pos);
}

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

bool EnablePicture(CString strPictureUrl, CString strTitle, CString strContent, CString strTag, LONG nViewProp, LONG nFileSize, LONG nAlbumId, CString & strError)
{
	CHttpObject objHttp;

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( PHP_API_URL );

	if ( strTitle.GetLength()>30 )
	{
		strTitle = strTitle.Left(30);
	}

	objHttp.AddParam("actionCode",31);
	objHttp.AddParam("userId",(long)g_AppConfigInfo.userinfo.nUserId);
	objHttp.AddParam("userName",(LPCSTR)g_AppConfigInfo.userinfo.strUserName);
	objHttp.AddParam("operFlag",0);
	objHttp.AddParam("picId",0);
	objHttp.AddParam("userNick",(LPCSTR)UTF8_Convert(g_AppConfigInfo.userinfo.strNickName,TRUE));
	objHttp.AddParam("albumId",nAlbumId);
	objHttp.AddParam("title",(LPCSTR)UTF8_Convert(strTitle,TRUE));
	objHttp.AddParam("content",(LPCSTR)UTF8_Convert(strContent,TRUE));
	objHttp.AddParam("usrTag",(LPCSTR)UTF8_Convert(strTag,TRUE));
	objHttp.AddParam("picUrl",strPictureUrl);
	objHttp.AddParam("contPop",nViewProp);

	if  (g_AppConfigInfo.userinfo.bIsMiniClinet)
		objHttp.AddParam("from",4);
	else
		objHttp.AddParam("from",0);

	objHttp.AddParam("fileSize",nFileSize);

	objHttp.AddParam("valid",objHttp.GetValidString());

	CString strReturn = objHttp.SendRequest();

	CString strTemp;

	GetSectionValue(strReturn,"<return_code>","</return_code>",strTemp);

	strTemp.Trim();

	if ( strTemp=="0" )
	{
		return true;
	}
	else
	{
		GetSectionValue(strReturn,"<error_msg>","</error_msg>",strError);
		return false;
	}
}


bool UploadPicture(CString strPictureFile, CString strTitle, CString strTag, CString strContent, int nViewProp, int nAlbumID,  CString &strPictureUrl,CString & strError)
{
	CHttpObject objHttp;

	strPictureUrl = "";

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( g_AppConfigInfo.userinfo.strPicUrl );

	CString strFileDir;
	long    nUserId = g_AppConfigInfo.userinfo.nUserId;

	strFileDir.Format("%d\\%d\\%d",nUserId/1000000,nUserId/1000,nUserId);

	objHttp.AddParam("clienttype","1");
	objHttp.AddParam("filedir",strFileDir);

	CString strPath, strFileName, strFileExt;//     = GetFileNameFromPath(strFullFilePath);
	ExtractFilePath(strPictureFile,strPath,strFileName,strFileExt);

	CStdioFile objFile;
	bool       bResult  = false;
	long       nFileLen = 0;

	CString strFileParamName;

	CTime tm = CTime::GetCurrentTime();
	strFileParamName.Format("%s%03d",tm.Format("%Y%m%d%H%M%S"), 1000+rand()%1000);

	if ( objFile.Open((LPCSTR)strPictureFile,CFile::modeRead|CFile::typeBinary) )
	{
		char * pData;
		nFileLen = objFile.GetLength();

		pData    = new char[nFileLen];

		objFile.Read(pData,nFileLen);
		objHttp.AddFile(strFileParamName,strFileName+strFileExt,pData,nFileLen);
		delete [] pData;

		objFile.Close();
	}

	objHttp.AddParam("returnUrl","");

	CString strDomain;
	CString strHtml = objHttp.SendRequest();

	if ( strHtml=="" )
		strError = objHttp.GetLastError();
	else
	{
		if ( GetUploadResult(strHtml,strDomain) )
		{
			strFileDir.Replace("\\","/");
			strPictureUrl = strDomain+"/"+strFileDir+"/"+strFileParamName+strFileExt;

			strTitle.Trim();

			if ( strTitle=="" )
			{
				strTitle = strFileName+strFileExt;
				while(strTitle.Find("tmp_"))
					strTitle.Replace("tmp_","");
			}

			bResult = EnablePicture(strPictureUrl,strTitle,strContent,strTag,nViewProp,nFileLen,nAlbumID, strError);
		}
		else
		{
			strError = L"服务器处理繁忙，图片上传失败";
		}
	}

	return bResult;
}

void GetUserPictureTagList(vector<TTagInfo> &info)
{
	CHttpObject objHttp;

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( PHP_API_URL );

	objHttp.AddParam("actionCode",53);
	objHttp.AddParam("userId",(long)g_AppConfigInfo.userinfo.nUserId);
	objHttp.AddParam("userName",g_AppConfigInfo.userinfo.strUserName);
	objHttp.AddParam("valid",objHttp.GetValidString());

	CString strReturn = objHttp.SendRequest();

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn.AllocSysString(),&bSuccess);

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

						vector<TTagInfo>::iterator it = find(info.begin(), info.end(), ti);
						if (it  == info.end())
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


void GetUserAlbumList(long nFriendId,TUserAlbumInfo &info)
{
	CHttpObject objHttp;;

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( PHP_API_URL );

	objHttp.AddParam("actionCode",40);
	objHttp.AddParam("userId",(long)g_AppConfigInfo.userinfo.nUserId);
	objHttp.AddParam("userName",(LPCSTR)g_AppConfigInfo.userinfo.strUserName);
	objHttp.AddParam("friendId",nFriendId);
	objHttp.AddParam("valid",objHttp.GetValidString());

	CString strReturn = objHttp.SendRequest();


	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn.AllocSysString(),&bSuccess);

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


long CreateAlbum(long nOperationFlag, long nAlbumId, CString strAlbumName, long nViewProp, CString & strError )
{
	CHttpObject objHttp;

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( PHP_API_URL );

	objHttp.AddParam("actionCode",41);
	objHttp.AddParam("userId",(long)g_AppConfigInfo.userinfo.nUserId);
	objHttp.AddParam("userName",(LPCSTR)g_AppConfigInfo.userinfo.strUserName);
	objHttp.AddParam("userNick", UTF8_Convert((LPCSTR)(g_AppConfigInfo.userinfo.strNickName),true));
	objHttp.AddParam("operFlag",nOperationFlag);
	objHttp.AddParam("albumId",0);
	objHttp.AddParam("albumName",UTF8_Convert((LPCSTR)(strAlbumName),true));
	objHttp.AddParam("contPop",3);
	objHttp.AddParam("valid",objHttp.GetValidString());

	CString strReturn  = objHttp.SendRequest();

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn.AllocSysString(),&bSuccess);

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

long GetSpaceSize()
{
	CHttpObject objHttp;

	objHttp.SetHttpPostType(hptMultipart_FormData);
	objHttp.SetUrl( PHP_API_URL );

	objHttp.AddParam("actionCode",54);
	objHttp.AddParam("userId",(long)g_AppConfigInfo.userinfo.nUserId);
	objHttp.AddParam("userName",(LPCSTR)g_AppConfigInfo.userinfo.strUserName);
	objHttp.AddParam("valid",objHttp.GetValidString());

	CString strReturn = objHttp.SendRequest();

	CComPtr<IXMLDOMDocument> pDoc = NULL;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess=VARIANT_FALSE;

	pDoc->loadXML(strReturn.AllocSysString(),&bSuccess);

	long nSpaceSize=1024*1024*1024;//1G default

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