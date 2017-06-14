#include "StdAfx.h"
#include ".\webprocess.h"

#include "spacetypedef.h"
#include "localdata.h"

#include "curl.h"

CString MakeGuid(const char *szText)
{
	MD5_CTX md5T;
	unsigned char digest[16];

	md5T.MD5Update ((unsigned char*)szText, strlen(szText) );
	md5T.MD5Final (digest);
	
	CString str;
	str.Format("%x-%x-%x-%x",
			(((long *)digest))[0],
			(((long *)digest))[1],
			(((long *)digest))[2],
			(((long *)digest))[3]);

	return str;
}

// 获取要下载文件的长度
double GetTotalFileLenth(const char* url)
{
	static map<string, double> g_mapUrlFileLength;

	if(g_mapUrlFileLength.find(url) != g_mapUrlFileLength.end())
	{
		return g_mapUrlFileLength[url];
	}

	char mUrl[512] = {0};
	strcpy(mUrl, url);
	double downloadFileLenth = 0;
	CURL* pCurl = curl_easy_init();
	curl_easy_setopt(pCurl, CURLOPT_URL, mUrl);
	curl_easy_setopt(pCurl, CURLOPT_HEADER, 1L);
	curl_easy_setopt(pCurl, CURLOPT_NOBODY, 1L);
	curl_easy_setopt(pCurl, CURLOPT_TIMEOUT, 1);   // 1s超时
	if(curl_easy_perform(pCurl) == CURLE_OK)
	{
		curl_easy_getinfo(pCurl, CURLINFO_CONTENT_LENGTH_DOWNLOAD, &downloadFileLenth);
	}
	else
	{
		downloadFileLenth = -1;
	}
	curl_easy_cleanup(pCurl);
	g_mapUrlFileLength[url] = downloadFileLenth;
	return downloadFileLenth;
}

CString MD5Password(char* pPassword)
{
	unsigned char digest[16];

	char password[255] = {0};
	sprintf(password,"%sasdfds!@#$1234@!#$ASDFasdfQ@#4345asDF",pPassword);

	MD5_CTX md5T;
	md5T.MD5Update ((unsigned char*)password,(uint32)strlen(password) );
	md5T.MD5Final (digest);

	return ToHexString(digest,16);
}

//更改个人形象
void CMagicFaceProcess::SafeTerminate()
{
	map<DWORD,tagThreadParam *>::iterator it;

	this->m_secLock.Lock();

	for ( it= m_mapThread.begin(); it!=m_mapThread.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;
		
		try
		{
			TerminateThread(hThread,0);
			CloseHandle(hThread);
		}
		catch(...)
		{
		}
		
		DeleteFile(GetMagicFaceFile(it->second->face_url));

		delete it->second;
	}

	m_mapThread.clear();

	this->m_secLock.Unlock();

}

CString CMagicFaceProcess::GetMagicFaceFile(char * face_url)
{
	CString str;
	str.Format("%s%s",m_strCachePath,MakeGuid(face_url) );
	return str;
}

void CMagicFaceProcess::SetMagicPlayerPath(CString strPath, CString strPlayer)
{
	this->m_strPlayerPath = strPath;
	this->m_strPlayer     = strPlayer;
}

void CMagicFaceProcess::ShowMagicFace(const CString &strFaceUrl)
{
	map<DWORD,tagThreadParam *>::iterator it;

	this->m_dwIndex++;

	tagThreadParam * param = new tagThreadParam;
	param->dwIndex = m_dwIndex;
	param->pClass  = this;
	sprintf(param->face_url,strFaceUrl);

	DWORD dwThreadID = 0;
	param->hThread  = CreateThread(NULL,0,
								(LPTHREAD_START_ROUTINE)ThreadProc,
								param,
								0,
								&dwThreadID);


	m_mapThread.insert(map<DWORD,tagThreadParam *>::value_type(m_dwIndex,param));

}

UINT CMagicFaceProcess::ThreadProc(LPVOID param)
{
	tagThreadParam *pThread    = (tagThreadParam *)param;
	CMagicFaceProcess * pClass = pThread->pClass;

	CString strDownloadFile    = pClass->GetMagicFaceFile(pThread->face_url);

	bool bOk = false;

	if ( FileExist(strDownloadFile)==FALSE )
	{
		bOk = DownLoadWebFile(pThread->face_url,strDownloadFile);
	}
	else
	{
	/*	CString strDownloadFile1 = strDownloadFile+"1";
		bOk = DownLoadWebFile(pThread->face_url,strDownloadFile1);

		if ( bOk )
		{
			DeleteFile(strDownloadFile);
			
			try
			{
				CFile::Rename(strDownloadFile1,strDownloadFile);
			}
			catch(...)
			{
				bOk=false;
			}
		}*/
		bOk=true;
	}

	if ( bOk )
	{
		CString strParam;
		strParam.Format("(-%s-)",strDownloadFile);
		ShellExecute(NULL,"open",pClass->m_strPlayer,strParam,pClass->m_strPlayerPath,SW_SHOW);
	}

	map<DWORD,tagThreadParam *>::iterator it;

	pClass->m_secLock.Lock();
	it = pClass->m_mapThread.find(pThread->dwIndex);

	if ( it!=pClass->m_mapThread.end() )
	{
		try
		{
			CloseHandle(it->second->hThread);
		}
		catch(...)
		{
		}

		pClass->m_mapThread.erase(it);		
	}

	pClass->m_secLock.Unlock();

	delete pThread;

	return 0;
}

/////
CWebUserHeadIconProcess::CWebUserHeadIconProcess(void)
{
}

CWebUserHeadIconProcess::~CWebUserHeadIconProcess(void)
{
	SafeTerminate();
}

UINT CWebUserHeadIconProcess::ThreadProc(LPVOID param)
{
	tagThreadParam  * pThread = (tagThreadParam *)param;
	CWebUserHeadIconProcess * pWebIconProcess=pThread->pClass;

	CString strUrl     ;
    CString strDownloadFile ;

	strDownloadFile = pThread->pClass->Url2File(pThread->face_url);

	bool bOk = false;

	if ( FileExist(strDownloadFile)==FALSE )
	{
		char pic_url[255];
		if ( strstr(pThread->face_url,"://")==NULL )
			sprintf(pic_url,"http://%s",pThread->face_url);
		else
			strcpy(pic_url,pThread->face_url);

		pWebIconProcess->m_secLockID.Lock();

		map<CString,vector<tagFidHwnd *> *>::iterator it = pWebIconProcess->m_mapFace.find(pThread->face_url);

		if ( it==pWebIconProcess->m_mapFace.end() )
		{
			vector<tagFidHwnd *> * pvec = new vector<tagFidHwnd *>;
			pWebIconProcess->m_mapFace.insert(map<CString,vector<tagFidHwnd *> *>::value_type(pThread->face_url,pvec));
		}

		pWebIconProcess->m_secLockID.Unlock();

		//图片改造
		CString strUrl = pic_url;
		if (strUrl.Find("http://") == -1)
		{
			strUrl = GetPicRandomUrl() + strUrl;
		}
		bOk = DownLoadWebFile(/*pic_url*/strUrl,strDownloadFile);
	}//end if (FileExist(.....
	else
	{
		bOk=true;//文件已经存在了
	}

	LPARAM lParam = 0;

	if ( bOk )
	{
		lParam = (LPARAM)(LPCSTR)strDownloadFile;
	}

	if ( pThread->hNotifyWnd )
	{
		::SendMessage( pThread->hNotifyWnd,WM_USER_HEAD_NOTIFY,pThread->fid, lParam);
	}

	int count = pWebIconProcess->m_vecHWnd.size();

	for ( int i=0; i<count; i++ )
	{
		::SendMessage(pWebIconProcess->m_vecHWnd[i],WM_USER_HEAD_NOTIFY,pThread->fid,lParam);
	}

	pWebIconProcess->m_secLockID.Lock();

	map<CString,vector<CWebUserHeadIconProcess::tagFidHwnd *> *>::iterator it=pWebIconProcess->m_mapFace.find(pThread->face_url);
	
	if ( it!=pWebIconProcess->m_mapFace.end())
	{
		vector<CWebUserHeadIconProcess::tagFidHwnd *> * pvec = it->second;
		int n= pvec->size();

		for ( int i=0; i<n; i++ )
		{
			CWebUserHeadIconProcess::tagFidHwnd * fh= (*pvec)[i];
			::SendMessage( fh->hWnd,WM_USER_HEAD_NOTIFY,fh->fid,lParam);
			delete fh;
		}

		pvec->clear();
		delete pvec;

		pWebIconProcess->m_mapFace.erase(it);

		pWebIconProcess->m_secLockID.Unlock();
	}

	map<uint32,tagThreadParam *>::iterator itTh;

	pWebIconProcess->m_secLockTh.Lock();
	itTh = pWebIconProcess->m_mapThreadRun.find(pThread->fid);

	if ( itTh!=pWebIconProcess->m_mapThreadRun.end() )
	{
		try
		{
			CloseHandle(itTh->second->hThread);
		}
		catch(...)
		{

		}

		pWebIconProcess->m_mapThreadRun.erase(itTh);		
	}


	pWebIconProcess->m_secLockTh.Unlock();

	delete pThread;
	return 0;
}

CString CWebUserHeadIconProcess::Url2File(char * face_url)
{
	if ( strcmp(face_url,"")!=0 )
	{
		CString str;
		str.Format("%s%s",m_strPath,MakeGuid(face_url) );
		return str;
	}
	else
	{
		return "";
	}
}

CString CWebUserHeadIconProcess::GetFaceFile(const XT_USER_INFO *pUser)
{
	CString strFaceFile ;
	CString strFacceUrl = (char *)pUser->face_url;
	if(strFacceUrl.IsEmpty() || strFacceUrl.Find(".gif") != -1)//如果头像是gif图片也显示为默认图片
	{	// 如果为设置头像，则取默认头像
		CString strIcon;
		strIcon.Format("%s%d%s","icon%28",1,"%29.png");
		strFacceUrl = HEADICON_URL + strIcon;
	}
	else
	{
		CString strUrl = strFacceUrl;
		if (strUrl.Find("http://") == -1)
		{
			strUrl = GetPicRandomUrl() + strUrl;
		}
		strFacceUrl = strUrl;
		
		if(GetTotalFileLenth(strFacceUrl) < 0.001)
		{
			CString strIcon;
			strIcon.Format("%s%d%s","icon%28",1,"%29.png");
			strFacceUrl = HEADICON_URL + strIcon;
		}
	}
	
	strFaceFile = Url2File((LPSTR)(LPCTSTR)strFacceUrl);

	// 如果文件不存在，就下载下来
	if(!FileExist(strFaceFile))
	{
		DownLoadWebFile((LPSTR)(LPCTSTR)strFacceUrl, strFaceFile);
	}

	return strFaceFile;
}

void CWebUserHeadIconProcess::GetUserHeadIcon(const XT_USER_INFO *pFriend, CWnd *pWndNotify)
{
	map<uint32,tagThreadParam *>::iterator it;
	it=m_mapThreadRun.find(((XT_USER_INFO *)pFriend)->GetId());

	if ( it==m_mapThreadRun.end() )
	{
		tagThreadParam * param = new tagThreadParam;
		param->fid			= ((XT_USER_INFO *)pFriend)->GetId();
		param->hNotifyWnd   = pWndNotify->m_hWnd;
		strncpy(param->face_url,pFriend->face_url,sizeof(param->face_url)-1);

		try
		{
			map<CString,vector<tagFidHwnd *> *>::iterator it = m_mapFace.find(param->face_url);//当前wor

			if ( it!=m_mapFace.end() )//系统头像结果队列中有相同的头像在下在
			{
				vector<tagFidHwnd *> *pvec = it->second;
				tagFidHwnd *pfh = new tagFidHwnd;
				pfh->fid        = ((XT_USER_INFO *)pFriend)->GetId();
				pfh->hWnd       = pWndNotify->m_hWnd;
				pvec->push_back(pfh);

			//	m_secLockID.Unlock();

				delete param;

				return ;
			}
			else
			{
				vector<tagFidHwnd *> *pvec = new vector<tagFidHwnd *>;
				tagFidHwnd *pfh = new tagFidHwnd;
				pfh->fid   = ((XT_USER_INFO *)pFriend)->GetId();
				pfh->hWnd  = pWndNotify->m_hWnd;
				pvec->push_back(pfh);

				CString strUrl = param->face_url;
				if (strUrl.Find("http://") == -1)
				{
					strUrl = GetPicRandomUrl() + strUrl;
					strcpy(param->face_url, strUrl);
				}
				m_mapFace.insert(map<CString,vector<tagFidHwnd *> *>::value_type(param->face_url,pvec));
			}
		}
		catch(...)
		{
			ASSERT(FALSE);
			return ;
		}
	

		param->pClass = this;


		m_mapThreadRun.insert(map<uint32,tagThreadParam *>::value_type(((XT_USER_INFO *)pFriend)->GetId(),param));
		DWORD dwThreadID = 0;
		param->hThread  = CreateThread(NULL,0,
									(LPTHREAD_START_ROUTINE)ThreadProc,
									param,
									0,
									&dwThreadID);
	}	
	else
	{
		RegistIconNotify(pWndNotify->m_hWnd);
	}
}

void CWebUserHeadIconProcess::RegistIconNotify(HWND hWnd)
{
	int count = m_vecHWnd.size();

	int i;

	for ( i=0; i<count; i++ )
	{
		if ( m_vecHWnd[i]==hWnd )
			break;
	}

	if ( i==count )
	{
		m_vecHWnd.push_back(hWnd);
	}
}

void CWebUserHeadIconProcess::UnRegistIconNotify(HWND hWnd)
{
	int count = m_vecHWnd.size();

	for ( int i=0; i<count; i++ )
	{
		if ( m_vecHWnd[i]==hWnd )
		{
			m_vecHWnd.erase(m_vecHWnd.begin()+i);
			break;
		}
	}
}

void CWebUserHeadIconProcess::SafeTerminate()
{
	map<uint32,tagThreadParam *>::iterator it;
	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;

		DeleteFile(Url2File(it->second->face_url));

		try
		{
			delete it->second;
			TerminateThread(hThread,0);
			CloseHandle(hThread);
		}
		catch(...)
		{
		}
	}

	m_mapThreadRun.clear();

	this->m_vecHWnd.clear();

	map<CString,vector<tagFidHwnd *> *>::iterator itMap;

	for ( itMap=m_mapFace.begin(); itMap!=m_mapFace.end(); itMap++ )
	{
		int n = itMap->second->size();
		for ( int i=0; i<n; i++ )
		{
			delete (*itMap->second)[i];
		}

		itMap->second->clear();
		delete itMap->second;
	}

	m_mapFace.clear();
}

//CWebFamilyHeadIconProcess

CWebFamilyHeadIconProcess::CWebFamilyHeadIconProcess(void)
{
}

CWebFamilyHeadIconProcess::~CWebFamilyHeadIconProcess(void)
{
	SafeTerminate();
}


UINT CWebFamilyHeadIconProcess::ThreadProc(LPVOID param)
{
	tagThreadParam  * pThread = (tagThreadParam *)param;
	CWebFamilyHeadIconProcess * pWebIconProcess=pThread->pClass;

	CString strUrl     ;
    CString strDownloadFile ;

	strDownloadFile = pThread->pClass->Url2File(pThread->face_url);

	bool bOk = false;

	if ( FileExist(strDownloadFile)==FALSE )
	{
		char pic_url[255];
		if ( strstr(pThread->face_url,"://")==NULL )
			sprintf(pic_url,"http://%s",pThread->face_url);
		else
			strcpy(pic_url,pThread->face_url);

		bOk = DownLoadWebFile(pic_url,strDownloadFile);
	}//end if (FileExist(.....
	else
	{
		bOk=true;//文件已经存在了
	}

	LPARAM lParam = 0;

	if ( bOk )
	{
		lParam = (LPARAM)(LPCSTR)strDownloadFile;
	}

	int i;

	for (i=0; i<pThread->vecHNotifyWnd.size(); i++ )
	{
		::SendMessage( pThread->vecHNotifyWnd[i],WM_FAMILY_HEAD_NOTIFY,pThread->family_id, lParam);
	}

	for (i=0; i<pThread->vecSysFaceJob.size(); i++ )
	{
		::SendMessage( pThread->vecSysFaceJob[i].hWnd,WM_FAMILY_HEAD_NOTIFY,pThread->vecSysFaceJob[i].family_id, lParam);
	}

	map<uint32,tagThreadParam *>::iterator itTh;

	pWebIconProcess->m_secLockTh.Lock();

	itTh = pWebIconProcess->m_mapThreadRun.find(pThread->family_id);

	if ( itTh!=pWebIconProcess->m_mapThreadRun.end() )
	{
		try
		{
			CloseHandle(itTh->second->hThread);
		}
		catch(...)
		{

		}

		pWebIconProcess->m_mapThreadRun.erase(itTh);		
	}


	pWebIconProcess->m_secLockTh.Unlock();

	pWebIconProcess->m_secLockHW.Lock();

	for ( i=0; i<pWebIconProcess->m_vecHWnd.size(); i++ )
	{
		::SendMessage( pWebIconProcess->m_vecHWnd[i],WM_FAMILY_HEAD_NOTIFY,pThread->family_id, lParam);
	}

	pWebIconProcess->m_secLockHW.Unlock();

	try
	{        
		delete pThread;
	}
	catch(...)
	{

	}

	return 0;
}

CString CWebFamilyHeadIconProcess::Url2File(char * face_url)
{
	if ( strcmp(face_url,"")!=0 )
	{
		CString str;
		str.Format("%s%s",m_strPath,MakeGuid(face_url) );
		return str;
	}
	else
	{
		return "";
	}
}

CString CWebFamilyHeadIconProcess::GetFaceFile(const XT_CLUSTER_INFO *pCluster)
{
	CString strFaceFile ;
	strFaceFile = Url2File((char *)pCluster->szFamilyFaceUrl);
	return strFaceFile;
}

void CWebFamilyHeadIconProcess::RegistIconNotify(HWND hWnd)
{
	m_secLockHW.Lock();

	int count = m_vecHWnd.size();

	int i;

	for ( i=0; i<count; i++ )
	{
		if ( m_vecHWnd[i]==hWnd )
			break;
	}

	if ( i==count )
	{
		m_vecHWnd.push_back(hWnd);
	}

	m_secLockHW.Unlock();
}

void CWebFamilyHeadIconProcess::UnRegistIconNotify(HWND hWnd)
{
	m_secLockHW.Lock();

	int count = m_vecHWnd.size();

	for ( int i=0; i<count; i++ )
	{
		if ( m_vecHWnd[i]==hWnd )
		{
			m_vecHWnd.erase(m_vecHWnd.begin()+i);
			break;
		}
	}

	m_secLockHW.Unlock();
}

void CWebFamilyHeadIconProcess::GetFamilyHeadIcon(const XT_CLUSTER_INFO *pCluster, CWnd *pWndNotify)
{
	map<uint32,tagThreadParam *>::iterator itTh;

	m_secLockTh.Lock();
	
	itTh = m_mapThreadRun.find(pCluster->nFamilyID);

	if ( itTh==m_mapThreadRun.end() )
	{
		tagThreadParam * param = new tagThreadParam;

		param->family_id	= pCluster->nFamilyID;
		param->face  		= pCluster->nFamilyFaceID;
		param->vecHNotifyWnd.push_back(pWndNotify->m_hWnd);
		param->pClass = this;

		strncpy(param->face_url,pCluster->szFamilyFaceUrl,sizeof(param->face_url)-1);
		param->bSystemFace = false;


		m_mapThreadRun.insert(map<uint32,tagThreadParam *>::value_type(pCluster->nFamilyID,param));
		DWORD dwThreadID = 0;
		param->hThread  = CreateThread(NULL,0,
									(LPTHREAD_START_ROUTINE)ThreadProc,
									param,
									0,
									&dwThreadID);
	}	


	m_secLockTh.Unlock();
	
}

void CWebFamilyHeadIconProcess::SafeTerminate()
{
	map<uint32,tagThreadParam *>::iterator it;

	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;

		DeleteFile(Url2File(it->second->face_url));

		try
		{
			delete it->second;
			TerminateThread(hThread,0);
			CloseHandle(hThread);
		}
		catch(...)
		{
		}
	}

	m_mapThreadRun.clear();
}



//CWebFileProcess 类
CWebFileProcess::CWebFileProcess(void)
{
}

CWebFileProcess::~CWebFileProcess(void)
{
	SafeTerminate();
}

UINT CWebFileProcess::ThreadProc(LPVOID param)
{
	tagThreadParam  * pTh     = (tagThreadParam *)param;
	CWebFileProcess * pClass  = pTh->pClass;

	CString strUrl     ;
    CString strDownloadFile ;

	strDownloadFile = pTh->pClass->Url2File(pTh->file_url);

	bool bOk = false;

	// 尝试三次下载图片
	for(int i = 0; i < 3; i++)
	{
		if ( FileExist(strDownloadFile)==FALSE )
		{
			char pic_url[255];
			if ( strstr(pTh->file_url,"://")==NULL )
				sprintf(pic_url,"http://%s",pTh->file_url);
			else
				strcpy(pic_url,pTh->file_url);

			CString strTempFile = strDownloadFile+".tmp";

			//图片改造
			CString strUrl;
			strUrl.Format("%s", pic_url);
			if (strUrl.Find("http://") == -1)
			{
				strUrl = GetPicRandomUrl() + strUrl;
			}

			bOk = DownLoadWebFile(/*pic_url*/strUrl,strTempFile);

			if ( bOk )
			{
				rename((char *)(LPCSTR)strTempFile,(char *)(LPCSTR)strDownloadFile);
			}
			else
			{
				DeleteFile(strTempFile);
			}
		}//end if (FileExist(.....
		else
		{
			bOk=true;//文件已经存在了
		}

		if(bOk)
		{
			break;
		}
	}

	LPARAM lParam = 0;

	if ( bOk )
	{
		lParam = (LPARAM)(LPCSTR)strDownloadFile;
	}
	else
	{	// 文件下载失败
		//g_LocalData.WriteLog(CLocalData::LL_INFORMATION, "图片下载失败。url:%s", pTh->file_url);
		CString str;
		str.Format("图片下载失败。url:%s", pTh->file_url);
		g_LocalData.WriteLog(CLocalData::LL_INFORMATION, str);
	}
	::SendMessage(pTh->hWnd,WM_WEBFILE_NOTIFY,pTh->dwUserFlagData, lParam);

	map<DWORD,tagThreadParam *>::iterator it;

	pClass->m_secLockTh.Lock();
	it = pClass->m_mapThread.find(pTh->dwUserFlagData);

	if ( it!=pClass->m_mapThread.end() )
	{
		try
		{
			CloseHandle(it->second->hThread);
		}
		catch(...)
		{
		}

		pClass->m_mapThread.erase(it);		
	}

	pClass->m_secLockTh.Unlock();

	delete pTh;
	return 0;
}

CString CWebFileProcess::Url2File(char * face_url)
{
	CString str;
	str.Format("%s{%s}",m_strPath,MakeGuid(face_url) );

	// 获取后缀
	string strURL = face_url;
	std::string::size_type nPos = strURL.find_last_of('.');
	if(nPos != string::npos)
	{
		str += strURL.substr(nPos).c_str();
	}

	return str;
}

void CWebFileProcess::GetWebFile(DWORD dwUserFlagData,char *file_url, HWND hNotifyWnd)
{
	map<DWORD,tagThreadParam *>::iterator it;
	it=m_mapThread.find(dwUserFlagData);

	if ( it==m_mapThread.end() )
	{
		tagThreadParam * param = new tagThreadParam;
		param->dwUserFlagData = dwUserFlagData;
		param->hWnd    = hNotifyWnd;
		strcpy(param->file_url,file_url);
	

		param->pClass = this;
	
		DWORD dwThreadID = 0;
		param->hThread  = CreateThread(NULL,0,
									(LPTHREAD_START_ROUTINE)ThreadProc,
									param,
									0,
									&dwThreadID);

		m_secLockTh.Lock();
		m_mapThread.insert(map<uint32,tagThreadParam *>::value_type(dwUserFlagData,param));
		m_secLockTh.Unlock();
	}	
}

void CWebFileProcess::SafeTerminate()
{
	m_secLockTh.Lock();

	map<DWORD,tagThreadParam *>::iterator it;
	for ( it=m_mapThread.begin(); it!=m_mapThread.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;

		DeleteFile(Url2File(it->second->file_url));

		try
		{
			delete it->second;
			TerminateThread(hThread,0);
			CloseHandle(hThread);
		}
		catch(...)
		{
		}
	}

	m_mapThread.clear();
	m_secLockTh.Unlock();
}


//CWebFlashProcess 类
CWebFlashProcess::CWebFlashProcess()
{
}


CWebFlashProcess::~CWebFlashProcess()
{
	SafeTerminate();
}


CString CWebFlashProcess::Url2File(char * flash_url)
{
	CString str;
	str.Format("%s%s",m_strPath,MakeGuid(flash_url) );
	return str;
}

void CWebFlashProcess::SafeTerminate()
{
	m_secLockRun.Lock();

	map<uint32,tagThreadParam *>::iterator it;
	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;

		DeleteFile(Url2File(it->second->flash_url));

		try
		{
			delete it->second;
			TerminateThread(hThread,0);
			CloseHandle(hThread);
		}
		catch(...)
		{
		}
	}

	m_mapThreadRun.clear();
	m_secLockRun.Unlock();
}

CString CWebFlashProcess::GetFlashFile(const XT_USER_INFO *pUser)
{
	CString strAvatarUrl ;
	//strAvatarUrl.Format("http://%s.feiniu.com/avatar/display.swf",pUser->username);
	return strAvatarUrl;

	/*
	CString strFlashFile = Url2File((char *)(LPCSTR)strAvatarUrl);

	if ( !FileExist(strFlashFile) )
	{
		if ( pUser->sex==1 )//男
		{
			strFlashFile.Format("%sFlash\\boy.swf",g_LocalData.GetPath());
		}
		else
		{
			strFlashFile.Format("%sFlash\\girl.swf",g_LocalData.GetPath());
		}
	}

	return strFlashFile; */
}

void CWebFlashProcess::GetUserFlashShow(const XT_USER_INFO *pUser, CWnd *pWndNotify)
{
	map<uint32,tagThreadParam *>::iterator it;
	it=m_mapThreadRun.find(((XT_USER_INFO *)pUser)->GetId());

	if ( it==m_mapThreadRun.end() )
	{
		tagThreadParam * param = new tagThreadParam;
		param->user_id		= ((XT_USER_INFO *)pUser)->GetId();
		param->hWndNotify   = pWndNotify->m_hWnd;
		param->pClass       = this;
		//strncpy(param->flash_url,pUser->flash_url,sizeof(param->flash_url)-1);
		sprintf(param->flash_url,"http://%s.feiniu.com/avatar/display.swf",((XT_USER_INFO *)pUser)->GetUserName().c_str());
		//strncpy(param->flash_url,"d1.sina.com.cn/200707/20/99261_home-dingtong-46895.swf",sizeof(param->flash_url)-1);
	
		m_mapThreadRun.insert(map<uint32,tagThreadParam *>::value_type(((XT_USER_INFO *)pUser)->GetId(),param));

		DWORD dwThreadID = 0;
		param->hThread   = CreateThread(NULL,0,
									(LPTHREAD_START_ROUTINE)ThreadProc,
									param,
									0,
									&dwThreadID);
	}	
	
}

UINT CWebFlashProcess::ThreadProc(LPVOID param)
{
	tagThreadParam  * pTh      = (tagThreadParam *)param;
	CWebFlashProcess * pClass  = pTh->pClass;

	CString strUrl     ;
    CString strDownloadFile ;

	strDownloadFile = pTh->pClass->Url2File(pTh->flash_url);

	bool bOk = false;

	if ( FileExist(strDownloadFile)==FALSE )
	{
		char flash_url[255];
		if ( strstr(pTh->flash_url,"://")==NULL )
			sprintf(flash_url,"http://%s",pTh->flash_url);
		else
			strcpy(flash_url,pTh->flash_url);

		CString strTempFile = strDownloadFile+".tmp";
		bOk = DownLoadWebFile(flash_url,strTempFile);

		if ( bOk )
		{
			rename((char *)(LPCSTR)strTempFile,(char *)(LPCSTR)strDownloadFile);
		}
		else
		{
			DeleteFile(strTempFile);
		}
	}//end if (FileExist(.....
	else
	{
		bOk=true;//文件已经存在了
	}

	LPARAM lParam = 0;

	if ( bOk )
	{
		lParam = (LPARAM)(LPCSTR)strDownloadFile;
	}

	::SendMessage(pTh->hWndNotify,WM_WEBFLASH_NOTIFY,pTh->user_id, lParam);

	map<uint32,tagThreadParam *>::iterator it;

	pClass->m_secLockRun.Lock();
	it = pClass->m_mapThreadRun.find(pTh->user_id);


	if ( it!=pClass->m_mapThreadRun.end() )
	{
		try
		{
			CloseHandle(it->second->hThread);
		}
		catch(...)
		{
		}

		pClass->m_mapThreadRun.erase(it);		
	}

	pClass->m_secLockRun.Unlock();

	delete pTh;

	return 0;
}

CMagicFaceProcess			g_magicFaceProcess ;
CWebUserHeadIconProcess		g_webUserHeadIconProcess   ;
CWebFamilyHeadIconProcess   g_webFamilyHeadIconProcess ;
CWebFileProcess				g_webFileProcess;
CWebFlashProcess			g_webFlashProcess;
