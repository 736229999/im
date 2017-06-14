#include "stdafx.h"
#include <io.h>
#include "SpaceData.h"
#include "..\..\hmgcrtlib.h"

CSpaceData::CSpaceData()
{
} 


CSpaceData::~CSpaceData()
{
}

void CSpaceData::UpdateSavedArticle(TSavedArticleItem &item)
{
	CString str;
	str.Format("%s\\%s\\article\\",m_strPath,m_strUserName);

	if ( _access((LPCSTR)str,0)==-1 )
	{
		CreateDirectory((LPCSTR)str,NULL);
	}

	str = str+"cfg.dat";

	const int dataLen = sizeof(TSavedArticleItem);

	CStdioFile f;
	if ( _access((LPCSTR)str,0)==-1 )
	{
		if (f.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary))
		{
			f.Write(&item,dataLen);
			f.Close();
		}
	}
	else
	{
		if ( f.Open(str,CFile::modeReadWrite|CFile::typeBinary) )
		{
			bool b=false;
			TSavedArticleItem temp;
			long size=0;
			while ( f.Read(&temp,dataLen)==dataLen )
			{
				size += dataLen;
				if ( temp.dwSavedID==item.dwSavedID )
				{
					size-=dataLen;
					f.SeekToBegin();
					f.Seek(size,f.begin);
					f.Write(&item,dataLen);
					b=true;
					break;
				}
			}
			if ( b==false )
				f.Write(&item,dataLen);

			f.Close();
		}
	}

}

void CSpaceData::SetPath(CString strPath)
{
	m_strPath     = strPath;
	if ( _access((LPCSTR)m_strPath,0)==-1 )
	{
		CreateDirectory((LPCSTR)m_strPath,NULL);
	}
}

void CSpaceData::SetUserInfo(CString strUserName, long nUserID)
{
	m_strUserName = strUserName;
	m_nUserID     = nUserID;

	CString strUserPath = m_strPath+strUserName+"\\";
	
	if ( _access((LPCSTR)strUserPath,0)==-1 )
	{
		CreateDirectory((LPCSTR)strUserPath,NULL);
	}
}

CString & CSpaceData::GetPath()
{
	return m_strPath;
}

void CSpaceData::GetZoneSetting(TZoneSetting &setting)
{
	CString strIni;
	strIni.Format("%s\\%s\\zone_a.ini",m_strPath,m_strUserName);

	char szValue[255];

	GetPrivateProfileString("Article","backup_path",m_strPath,szValue,sizeof(szValue),strIni);

	strncpy(setting.szBackupPath,szValue,sizeof(setting.szBackupPath)-1);

	int l = strlen(setting.szBackupPath);

	while (l>0)
	{
		l--;
		if ( setting.szBackupPath[l]!=' ' )
		{
			if(setting.szBackupPath[l]!='\\')
			{
				setting.szBackupPath[l+1]='\\';
				setting.szBackupPath[l+2]=0;
			}
			else
			{
				break;
			}
		}		
	}
}


void CSpaceData::DeleteSavedArticle(DWORD dwSavedID)
{
	CString str;
	str.Format("%s\\%s\\article\\cfg.dat",m_strPath,m_strUserName);

	if ( _access((LPCSTR)str,0)==-1 )
	{
		return ;
	}

	CString strxmlFile;

	strxmlFile.Format("%s\\%s\\article\\%u.xml",m_strPath,m_strUserName,dwSavedID);
	DeleteFile(strxmlFile);
	DeleteDirectory((char *)(LPCSTR)strxmlFile.Mid(0,strxmlFile.GetLength()-4));


	TSavedArticleList articles;
	LoadMySavedArticleList(articles);

	const int dataLen = sizeof(TSavedArticleItem);

	DeleteFile(str);

	CStdioFile f;
	if ( f.Open(str,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary) )
	{
		int n=articles.GetCount();
		for ( int i=0; i<n; i++ )
		{
			TSavedArticleItem item = articles.GetAt( articles.FindIndex(i) );
			if ( item.dwSavedID!=dwSavedID )
			{
				f.Write(&item,dataLen);
			}
		}

		f.Close();
	}

}


CString CSpaceData::GetMySavedArticlePath()
{
	CString str;
	str.Format("%s\\%s\\article\\",m_strPath,m_strUserName);

	if ( _access((LPCSTR)str,0)==-1 )
		CreateDirectory((LPCSTR)str,NULL);

	return str;
}

CString CSpaceData::GetUserWebArticlePath(uint32 friend_id)
{
	CString str;
	str.Format("%s\\%s\\log%u\\",m_strPath,m_strUserName,friend_id);

	if (_access((LPCSTR)str,0)==-1 )
	{
		CreateDirectory(str,NULL);
	}

	return str;
}

CString CSpaceData::GetUserWebPicturePath(uint32 friend_id)
{
	CString str;
	str.Format("%s\\%s\\pic%u\\",m_strPath,m_strUserName,friend_id);

	if (_access((LPCSTR)str,0)==-1 )
	{
		CreateDirectory(str,NULL);
	}

	return str;
}

bool CSpaceData::LoadMySavedArticle(TLocalSavedArticleInfo &info,DWORD dwSavedID)
{
	CString str;
	str.Format("%s\\%s\\article\\%u.xml",m_strPath,m_strUserName,dwSavedID);

	if ( access((LPCSTR)str,0)==-1 )
	{
		return false;
	}

	CString strImagePath = str.Mid(0,str.GetLength()-4)+"\\";

	CComPtr<IXMLDOMDocument> pDoc;	
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	USES_CONVERSION;

	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = str.AllocSysString();
	pDoc->load(var,&bSuccess);
	SysFreeString(var.bstrVal);

	CComBSTR text;
	CComPtr<IXMLDOMNodeList> pNodes = NULL;
	pDoc->selectNodes(L"config/*",&pNodes);

	long nLen ;
	pNodes->get_length(&nLen);

	if ( pNodes && nLen>=9 )
	{
		CComPtr<IXMLDOMNode> pNode = NULL;

		pNodes->get_item(0,&pNode);
		pNode->get_text(&text);
		info.strTitle = text;//title
		pNode.Release();

		pNodes->get_item(1,&pNode);//content
		pNode->get_text(&text);
		info.strContent = text;
		pNode.Release();

		pNodes->get_item(2,&pNode);//create date
		pNode->get_text(&text);
		info.strCreateDate = text;
		pNode.Release();

		pNodes->get_item(3,&pNode);//tag
		pNode->get_text(&text);
		info.strTag = text;
		pNode.Release();

		pNodes->get_item(4,&pNode);//tag
		pNode->get_text(&text);
		info.strDirName = W2A(text);
		pNode.Release();

		pNodes->get_item(5,&pNode);//dir id
		pNode->get_text(&text);
		info.dwDirID = atol(W2A(text));
		pNode.Release();

		pNodes->get_item(6,&pNode);//view type
		pNode->get_text(&text);
		info.dwViewType = atol(W2A(text));
		pNode.Release();

		pNodes->get_item(7,&pNode);//top flag
		pNode->get_text(&text);
		info.dwTopFlag = atol(W2A(text));
		pNode.Release();

		pNodes->get_item(8,&pNode);//article id
		pNode->get_text(&text);
		info.dwArticleID = atol(W2A(text));
		pNode.Release();


		pNodes.Release();
	}

	pDoc.Release();

	CStringList sl;
	GetHtmlImageList(info.strContent,sl);	

	int n=sl.GetCount();
	for ( int i=0; i<n; i++ )
	{
		CString strImage = sl.GetAt( sl.FindIndex(i) );
		CString strFileName ;
		GetSectionValue(strImage,"([{-","-}])",strFileName);
		strFileName.Trim();

		if ( strFileName!="" )
		{
			CString strFullPathImageFile = strImagePath + strFileName;
			info.strContent.Replace(strImage,strFullPathImageFile);
		}
	}

	return true;
}


void CSpaceData::LoadMySavedArticleList(TSavedArticleList &articles)
{
	CString str;
	str.Format("%s\\%s\\article\\cfg.dat",m_strPath,m_strUserName);

	if ( access((LPCSTR)str,0)==-1 )
	{
		return ;
	}

	CStdioFile f;
	if ( f.Open(str,CFile::modeRead|CFile::typeBinary) )
	{
		TSavedArticleItem si;

		int dataLen = sizeof(si);

		while ( f.Read(&si,dataLen)==dataLen )
		{
			articles.AddTail(si);
		}

		f.Close();
	}
}

void CSpaceData::LoadUserWebArticles(uint32 uid,TUserWebArticleInfo &info)
{
	CString str;
	str.Format("%s\\%s\\log%u\\log_cfg.xml",m_strPath,m_strUserName,uid);

	if ( access((LPCSTR)str,0)==-1 )
	{
		return ;
	}

	CComPtr<IXMLDOMDocument> pDoc;	
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	USES_CONVERSION;

	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	VARIANT var;
	var.vt			= VT_BSTR;
	var.bstrVal		= str.AllocSysString();
	pDoc->load(var,&bSuccess);
	SysFreeString(var.bstrVal);

	if ( bSuccess==VARIANT_TRUE )
	{
		CComPtr<IXMLDOMNode> pNodeItem;

		pNodeItem = NULL;
		pDoc->selectSingleNode(L"config/update_date",&pNodeItem);

		if ( pNodeItem )
		{
			CComBSTR text;
			pNodeItem->get_text(&text);

			CString strValue = W2A(text);
			strValue.Trim();

			strncpy(info.szLastUpdateDate,(LPCSTR)strValue,sizeof(info.szLastUpdateDate)-1);

			pNodeItem.Release();
		}

		pNodeItem = NULL;

		pDoc->selectSingleNode(L"config/is_read",&pNodeItem);

		if ( pNodeItem )
		{
			CComBSTR text;
			pNodeItem->get_text(&text);
			info.isRead = atoi(W2A(text));

			pNodeItem.Release();
		}


		pNodeItem = NULL;


		int  nArticleId = 0;
		info.vecArticles.clear();

		while (1)
		{
			char szItem[255];
			sprintf(szItem,"config/log_list/log[%d]/*",nArticleId++);

			CComBSTR bstr(szItem); 
			CComPtr<IXMLDOMNodeList> pNodes; 

			pDoc->selectNodes(bstr,&pNodes);

			if ( pNodes )
			{
				long nNodes; 
				pNodes->get_length(&nNodes);

				CString strValue;

				if ( nNodes>=10 )
				{
					TWebArticleInfo * pW = new TWebArticleInfo();
					pW->uID = uid;

					CComPtr<IXMLDOMNode> pNode = NULL;
					pNodes->get_item(0, &pNode);

					pNode->get_text(&bstr);		//title

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szTitle,(LPCSTR)strValue,sizeof(pW->szTitle)-1);
					pNode.Release(); 

					pNodes->get_item(1, &pNode);

					pNode->get_text(&bstr);		//outlines

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szOutlines,(LPCSTR)strValue,sizeof(pW->szOutlines)-1);
					pNode.Release();

					pNodes->get_item(2, &pNode);//url
					pNode->get_text(&bstr);
					strncpy(pW->szUrl,W2A(bstr),sizeof(pW->szUrl)-1);
					pNode.Release();

					pNodes->get_item(3, &pNode);	// create date
					pNode->get_text(&bstr);

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szCreateDate,(LPCSTR)strValue,sizeof(pW->szCreateDate)-1);
					pNode.Release();


					pNodes->get_item(4, &pNode);	// modify date
					pNode->get_text(&bstr);

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szModifyDate,(LPCSTR)strValue,sizeof(pW->szModifyDate)-1);
					pNode.Release();

					pNodes->get_item(5, &pNode);	// from
					pNode->get_text(&bstr);
					pW->dwFrom = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(6,&pNode);	// article id
					pNode->get_text(&bstr);
					pW->dwArticleID = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(7,&pNode);	// view type
					pNode->get_text(&bstr);
					pW->dwViewType = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(8,&pNode);	// dir id
					pNode->get_text(&bstr);
					pW->dwDirID = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(9,&pNode);	// state
					pNode->get_text(&bstr);
					pW->iState = atol(W2A(bstr));
					pNode.Release();

					pW->dwCreateDate = DateStr2Long(pW->szCreateDate);
					pW->dwModifyDate = DateStr2Long(pW->szModifyDate);

					info.vecArticles.push_back(pW);
				}
				else
				{
					break;
				}
			}//end if (pNodes)
		}//end while(1)
	}

	pDoc.Release();
}

void CSpaceData::LoadUserWebPictures(uint32 uid,TUserWebPictureInfo &info)
{
	CString str;
	str.Format("%s\\%s\\pic%u\\pic_cfg.xml",m_strPath,m_strUserName,uid);

	if ( access((LPCSTR)str,0)==-1 )
	{
		return ;
	}

	CComPtr<IXMLDOMDocument> pDoc;	
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	USES_CONVERSION;

	VARIANT_BOOL bSuccess = VARIANT_FALSE;
	VARIANT var;
	var.vt = VT_BSTR;
	var.bstrVal = str.AllocSysString();
	pDoc->load(var,&bSuccess);
	SysFreeString(var.bstrVal);

	if ( bSuccess==VARIANT_TRUE )
	{
		CComPtr<IXMLDOMNode> pNodeItem;

		pNodeItem = NULL;
		pDoc->selectSingleNode(L"config/update_date",&pNodeItem);

		if ( pNodeItem )
		{
			CComBSTR text;
			pNodeItem->get_text(&text);

			CString strValue = W2A(text);
			strValue.Trim();

			strncpy(info.szLastUpdateDate,(LPCSTR)strValue,sizeof(info.szLastUpdateDate)-1);

			pNodeItem.Release();
		}

		pNodeItem = NULL;

		pDoc->selectSingleNode(L"config/is_read",&pNodeItem);

		if ( pNodeItem )
		{
			CComBSTR text;
			pNodeItem->get_text(&text);
			info.isRead = atoi(W2A(text));

			pNodeItem.Release();
		}

		pNodeItem = NULL;

		int  nPicId = 0;
		info.vecPics.clear();

		while (1)
		{
			char szItem[255];
			sprintf(szItem,"config/pic_list/pic[%d]/*",nPicId++);

			CComBSTR bstr(szItem); 
			CComPtr<IXMLDOMNodeList> pNodes; 

			pDoc->selectNodes(bstr,&pNodes);

			CString strValue;

			if ( pNodes )
			{
				long nNodes; pNodes->get_length(&nNodes);

				if ( nNodes>=9 )
				{
					TWebPictureInfo *pW = new TWebPictureInfo();
					pW->uID = uid;

					CComPtr<IXMLDOMNode> pNode = NULL;

					pNodes->get_item(0, &pNode);

					pNode->get_text(&bstr);		//title

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szTitle,(LPCSTR)strValue, sizeof(pW->szTitle)-1);
					pNode.Release();

					pNodes->get_item(1, &pNode);//url
					pNode->get_text(&bstr);
					strncpy(pW->szUrl,W2A(bstr),sizeof(pW->szUrl)-1);
					pNode.Release();

					pNodes->get_item(2, &pNode);	// create date
					pNode->get_text(&bstr);

					strValue = W2A(bstr);
					strValue.Trim();

					strncpy(pW->szCreateDate,(LPCSTR)strValue,sizeof(pW->szCreateDate)-1);
					pNode.Release();

					pNodes->get_item(3, &pNode);	// from
					pNode->get_text(&bstr);
					pW->dwFrom = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(4,&pNode);	// picture id
					pNode->get_text(&bstr);
					pW->dwPictureID = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(5,&pNode);	// view type
					pNode->get_text(&bstr);
					pW->dwViewType = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(6,&pNode);	// dir id
					pNode->get_text(&bstr);
					pW->nAlbumId = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(7,&pNode);	// height
					pNode->get_text(&bstr);
					pW->iHeight = atol(W2A(bstr));
					pNode.Release();

					pNodes->get_item(8,&pNode);	// width
					pNode->get_text(&bstr);
					pW->iWidth = atol(W2A(bstr));
					pNode.Release();

					pW->dwCreateDate = DateStr2Long(pW->szCreateDate);

					info.vecPics.push_back(pW);
				}
				else
				{
					break;
				}
			}//end if (pNodes)
		}//end while(1)
	}

	pDoc.Release();
}

void CSpaceData::SaveMySavedArticle(const TLocalSavedArticleInfo &info,DWORD dwSavedID)
{
	CString strPath, strImagePath, strArticleFileName;
	strPath.Format("%s\\%s\\article\\",m_strPath,m_strUserName);

	if ( _access((LPCSTR)strPath,0)==-1 )
	{
		CreateDirectory((LPCSTR)strPath,NULL);
	}	

	strImagePath=strPath+UIntToStr(dwSavedID)+"\\";
	CreateDirectory((LPCSTR)strImagePath,NULL);

	strArticleFileName = strPath + UIntToStr(dwSavedID)+".xml";

	DeleteFile(strArticleFileName);

	CStringList sl;
	GetHtmlImageList(info.strContent,sl);

	CString strContent = info.strContent;

	int n = sl.GetCount();

	for (int i=0; i<n; i++ )
	{
		CString strLocalFile = sl.GetAt( sl.FindIndex(i) );

		if ( strLocalFile.Find("://")==-1 )
		{
			CString strFileName  = strLocalFile.Right(strLocalFile.GetLength() - strLocalFile.ReverseFind('\\') - 1);
			CString strNewLabelFile;
			strNewLabelFile.Format("([{-%s-}])",strFileName);
			strContent.Replace(strLocalFile,strNewLabelFile);
			CopyFile(strLocalFile,strImagePath+strFileName,FALSE);
		}
	}

	CStdioFile file;

	if ( file.Open((LPCSTR)strArticleFileName,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		file.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\" ?>");
		file.WriteString("<config>");

		file.WriteString("<title><![CDATA[");
		file.WriteString(info.strTitle);
		file.WriteString("]]></title>");

		file.WriteString("<content><![CDATA[");
		file.WriteString(strContent);
		file.WriteString("]]></content>");

		file.WriteString("<date>");
		file.WriteString(info.strCreateDate);
		file.WriteString("</date>");

		file.WriteString("<tag><![CDATA[");
		file.WriteString(info.strTag);
		file.WriteString("]]></tag>");

		file.WriteString("<dir_name>");
		file.WriteString(info.strDirName);
		file.WriteString("</dir_name>");

		file.WriteString("<dir_id>");
		file.WriteString(UIntToStr(info.dwDirID));
		file.WriteString("</dir_id>");

		file.WriteString("<view>");
		file.WriteString(UIntToStr(info.dwViewType));
		file.WriteString("</view>");

		file.WriteString("<topflag>");
		file.WriteString(UIntToStr(info.dwTopFlag));
		file.WriteString("</topflag>");

		file.WriteString("<articleID>");
		file.WriteString(UIntToStr(info.dwArticleID));
		file.WriteString("</articleID>");

		file.WriteString("</config>");
		file.Close();
	}	
}

void CSpaceData::SaveUserWebArticles(const TUserWebArticleInfo &info)
{
	CString str;
	str.Format("%s\\%s\\log%u\\",m_strPath,m_strUserName,info.uID);

	if ( _access((LPCSTR)str,0)==-1 )
	{
		CreateDirectory((LPCSTR)str,NULL);
	}

	str = str+"log_cfg.xml";

	//DeleteFile(str);

	CStdioFile file;

	if ( file.Open((LPCSTR)str,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		file.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\" ?>");
		file.WriteString("<config>");

		file.WriteString("\n");

		file.WriteString("<update_date><![CDATA[ ");
		file.WriteString(info.szLastUpdateDate);
		file.WriteString(" ]]></update_date>");

		file.WriteString("\n");

		file.WriteString("<is_read>");
		file.WriteString(IntToStr(info.isRead));
		file.WriteString("</is_read>");

		file.WriteString("\n");

		file.WriteString("<log_list>");
		for ( int i=0; i<info.vecArticles.size(); i++ )
		{
			TWebArticleInfo *pA=info.vecArticles[i];

			file.WriteString("<log>");

			file.WriteString("<title>\n<![CDATA[ ");
			file.WriteString(info.vecArticles[i]->szTitle);		
			file.WriteString(" ]]>\n</title>");

			file.WriteString("\n");

			file.WriteString("<outlines>\n<![CDATA[ ");
			file.WriteString(info.vecArticles[i]->szOutlines);
			file.WriteString(" ]]>\n</outlines>");

			file.WriteString("\n");

			file.WriteString("<url>");
			file.WriteString(info.vecArticles[i]->szUrl);
			file.WriteString("</url>");
			file.WriteString("\n");

			file.WriteString("<createdate><![CDATA[ ");
			file.WriteString(info.vecArticles[i]->szCreateDate);
			file.WriteString(" ]]></createdate>");
			file.WriteString("\n");

			file.WriteString("<modifydate><![CDATA[ ");
			file.WriteString(info.vecArticles[i]->szModifyDate);
			file.WriteString(" ]]></modifydate>");
			file.WriteString("\n");

			file.WriteString("<from>");
			file.WriteString(IntToStr(info.vecArticles[i]->dwFrom));
			file.WriteString("</from>");
			file.WriteString("\n");

			file.WriteString("<id>");
			file.WriteString(UIntToStr(info.vecArticles[i]->dwArticleID));
			file.WriteString("</id>");
			file.WriteString("\n");

			file.WriteString("<view>");
			file.WriteString(UIntToStr(info.vecArticles[i]->dwViewType));
			file.WriteString("</view>");
			file.WriteString("\n");

			file.WriteString("<dir>");
			file.WriteString(UIntToStr(info.vecArticles[i]->dwDirID));
			file.WriteString("</dir>");
			file.WriteString("\n");

			file.WriteString("<state>");
			file.WriteString(UIntToStr(info.vecArticles[i]->iState));
			file.WriteString("</state>");
			file.WriteString("\n");

			file.WriteString("</log>");

			file.WriteString("\n");
		}

		file.WriteString("</log_list>");
		file.WriteString("</config>");

		file.Close();
	}	
}

void CSpaceData::SaveUserWebPictures(const TUserWebPictureInfo &info)
{
	CString str;
	str.Format("%s\\%s\\pic%u\\",m_strPath,m_strUserName,info.uID);

	if ( _access((LPCSTR)str,0)==-1 )
	{
		CreateDirectory((LPCSTR)str,NULL);
	}

	str = str+"pic_cfg.xml";

	//DeleteFile(str);

	CStdioFile file;

	if ( file.Open((LPCSTR)str,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
	{
		file.WriteString("<?xml version=\"1.0\" encoding=\"gb2312\" ?>");
		file.WriteString("<config>");

		file.WriteString("<update_date><![CDATA[ ");
		file.WriteString(info.szLastUpdateDate);
		file.WriteString(" ]]></update_date>");

		file.WriteString("<is_read>");
		file.WriteString(IntToStr(info.isRead));
		file.WriteString("</is_read>");

		file.WriteString("<pic_list>");
		for ( int i=0; i<info.vecPics.size(); i++ )
		{
			file.WriteString("<pic>");

			file.WriteString("<title><![CDATA[ ");
			file.WriteString(info.vecPics[i]->szTitle);
			file.WriteString(" ]]></title>");

			file.WriteString("<url>");
			file.WriteString(info.vecPics[i]->szUrl);
			file.WriteString("</url>");

			file.WriteString("<date><![CDATA[ ");
			file.WriteString(info.vecPics[i]->szCreateDate);
			file.WriteString(" ]]></date>");


			file.WriteString("<from>");
			file.WriteString(IntToStr(info.vecPics[i]->dwFrom));
			file.WriteString("</from>");

			file.WriteString("<id>");
			file.WriteString(UIntToStr(info.vecPics[i]->dwPictureID));
			file.WriteString("</id>");

			file.WriteString("<view>");
			file.WriteString(UIntToStr(info.vecPics[i]->dwViewType));
			file.WriteString("</view>");

			file.WriteString("<dir>");
			file.WriteString(UIntToStr(info.vecPics[i]->nAlbumId));
			file.WriteString("</dir>");

			file.WriteString("<height>");
			file.WriteString(UIntToStr(info.vecPics[i]->iHeight));
			file.WriteString("</height>");

			file.WriteString("<width>");
			file.WriteString(UIntToStr(info.vecPics[i]->iWidth));
			file.WriteString("</width>");

			file.WriteString("</pic>");
		}

		file.WriteString("</pic_list>");
		file.WriteString("</config>");

		file.Close();
	}	
}

void CSpaceData::SaveHistoryFaceUrl(CString strUrl)
{
	CString strDefault;
	strDefault.Format("%s\\%s\\",m_strPath,m_strUserName);

	if ( _access((LPCSTR)strDefault,0)==-1 )
		CreateDirectory(strDefault,NULL);

	CString strFile = strDefault+"history_face.dat";

	CStdioFile f;

	if ( _access((LPCSTR)strFile,0) )
	{
		if (f.Open(strFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText) )
		{
			f.WriteString(strUrl);
			f.WriteString("\n");
		}
		f.Close();
	}
	else
	{
		if ( f.Open(strFile,CFile::modeReadWrite|CFile::typeText) )
		{
			CString s;
			strUrl.Trim();
			strUrl.MakeLower();

			bool bFind=false;

			while ( f.ReadString(s) )
			{
				s.Trim();
				s.MakeLower();
				if ( s==strUrl )
				{
					bFind = true;
					break;
				}
			}

			if ( bFind==false )
			{
				f.SeekToEnd();
				f.WriteString(strUrl);
				f.WriteString("\n");
			}

			f.Close();
		}
	}
}

