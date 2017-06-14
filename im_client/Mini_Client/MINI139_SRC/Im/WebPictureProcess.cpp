#include "StdAfx.h"
#include ".\webpictureprocess.h"

extern CSpaceData g_spaceData;
//class CWebPictureProcess
CWebPictureProcess::CWebPictureProcess()
{
}

CWebPictureProcess::~CWebPictureProcess()
{
	SafeTerminate();
}

void CWebPictureProcess::SafeTerminate()
{
	int nRun  = m_vecRunThread.size();

	//m_secLockRun.Lock();
	for ( int i=0; i<m_vecRunThread.size(); i++ )
	{
		tagThreadParam *p=m_vecRunThread[i];
		try
		{
			TerminateThread(p->hThread,0);
			CloseHandle(p->hThread);
		}
		catch(...)
		{

		}

		delete p;
	}

	m_vecRunThread.clear();
	//m_secLockRun.Unlock();

	//m_secLockWait.Lock();

	list<tagThreadParam *>::iterator it;

	for ( it=m_fifoWaitThread.begin(); it!=m_fifoWaitThread.end(); it++ )
	{
		delete (*it);
	}
	m_fifoWaitThread.clear();
	//m_secLockWait.Unlock();
}

CBitmap * CWebPictureProcess::CreateBitmap(const tagThreadPictureNotify *pNotify, int nBmpWidth, int nBmpHeight)
{
	char *pData = new char[pNotify->data_len];
	char *pCursor = pData;
	int n = pNotify->vecData.size();

	for ( int i=0; i<n; i++)
	{
		memcpy(pCursor,pNotify->vecData[i]->pData,pNotify->vecData[i]->len);
		pCursor+=pNotify->vecData[i]->len;
	}

	CBitmap * pBitmap = CBitmapFromStream(pData,pNotify->data_len,nBmpWidth,nBmpHeight);
	delete [] pData;
	return pBitmap;
}

int CWebPictureProcess::GetWebPicture(long nPicOnwerID,const TWebPictureInfo & info,UINT iPictureSize, CWnd *pWndNotify)
{
	tagThreadParam * p = new tagThreadParam();

	p->pic_uid		= nPicOnwerID;
	p->pClass		= this;
	p->hWndNotify	= pWndNotify->m_hWnd;
	p->pic_id		= info.dwPictureID;
	p->iSize        = iPictureSize;
	p->dwCreateDate = info.dwCreateDate;

	if ( iPictureSize==0 )
	{
		strcpy(p->url,info.szUrl);
	}
	else
	{
		CString strFileName = GetWebFileName(info.szUrl);
		int i=strFileName.Find(".");
		CString strNewFileName;
		strNewFileName.Format("%s_%d.jpg",strFileName.Left(i),iPictureSize);
		CString strUrl = info.szUrl;
		strUrl.Replace(strFileName,strNewFileName);
		strcpy(p->url,strUrl);
	}

	m_secLockRun.Lock();

	int n = m_vecRunThread.size();

	for ( int i=0; i<n; i++ )
	{
		tagThreadParam * pTemp = m_vecRunThread[i];
		if ( pTemp->pic_id == p->pic_id && 
			pTemp->pic_uid==p->pic_uid && 
			pTemp->iSize == p->iSize && 
			pTemp->hWndNotify==p->hWndNotify )
		{
			m_secLockRun.Unlock();

			delete p;
			return 0;
		}
	}

	if ( m_vecRunThread.size()<10 || iPictureSize==0 )
	{
		m_vecRunThread.push_back(p);
		m_secLockRun.Unlock();

		DWORD dwThreadID = 0;
		p->hThread  = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)ThreadProc,
			p,
			0,
			&dwThreadID);

		if ( iPictureSize==0 )
			SetThreadPriority(p->hThread,THREAD_PRIORITY_ABOVE_NORMAL);
	}
	else
	{
		m_secLockRun.Unlock();

		m_secLockWait.Lock();
		m_fifoWaitThread.push_front(p);
		m_secLockWait.Unlock();
	}

	return 0;
}

UINT CWebPictureProcess::ThreadProc(LPVOID p)
{
	tagThreadParam *pT   = (tagThreadParam *)p;

	tagThreadResult result;
	result.pic_uid = pT->pic_uid;
	result.pic_id  = pT->pic_id;
	result.iSize   = pT->iSize;
	result.dwCreateDate = pT->dwCreateDate;
	result.picData = NULL;
	sprintf(result.szLocalFile,"%s%s",(LPCSTR)g_spaceData.GetUserWebPicturePath(pT->pic_uid),(LPCSTR)GetWebFileName(pT->url));

	bool bDownloaded = false;

	char szFullUrl[256];
	sprintf(szFullUrl,"http://%s",pT->url);

	if ( pT->iSize==0 )//ԭͼ
	{
		char *pZ = strstr(szFullUrl,"/z/" );

		if ( pZ==NULL )
		{
			pZ=strstr(szFullUrl,"/Z/" );
		}

		if ( pZ )
		{
			*(pZ+1)='s';
		}
	}

	if ( (access(result.szLocalFile,0)==-1) )
	{
		result.isFromCache  = false;

		::SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,0,(LPARAM)&result);// begin to download

		HINTERNET hFile, hInet;

		hInet = InternetOpen("DownLoadWebFile",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

		if ( hInet!=NULL )
		{
			hFile = InternetOpenUrl(hInet, szFullUrl,NULL, 0, 
				INTERNET_FLAG_RELOAD | 
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_EXISTING_CONNECT | 
				INTERNET_FLAG_NO_COOKIES,
				0
				);

			if ( hFile!=NULL )
			{	
				CStdioFile fileDownload;

				DWORD dwStatus     = 0;
				char  buff[255];
				DWORD dwSize	   = sizeof(buff);

				if ( HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE, buff, &dwSize ,NULL ) )
				{
					dwStatus = atoi(buff);
				}

				if ( dwStatus==200 )
				{
					//if ( pT->iSize==0 )
					{
						const size_t   block_size = 8192;
						const size_t   buff_size  = 1024*128;
						DWORD  dwRead  = 0;
						tagThreadPictureNotify notify;
						tagThreadPictureNotify::buff_info * pBi = new tagThreadPictureNotify::buff_info();

						pBi->len   = 0;
						pBi->pData = new char[buff_size];

						notify.pic_id  = pT->pic_id;
						notify.pic_uid = pT->pic_uid;
						notify.vecData.push_back(pBi);

						notify.data_len = 0;

						char * pCursor = pBi->pData;

						int  nPackets = 0;

						DWORD dwTick1 = GetTickCount();
						DWORD dwTick2 = 0;

						while (InternetReadFile(hFile,pCursor,block_size,&dwRead) && dwRead>0 )
						{
							pBi->len  += dwRead;
							pCursor   += dwRead;
							notify.data_len += dwRead;

							if ( (pBi->len+block_size)>buff_size )
							{
								pBi        = new tagThreadPictureNotify::buff_info();
								pBi->len   = 0;
								pBi->pData = new char[buff_size];
								pCursor    = pBi->pData;
								notify.vecData.push_back(pBi);
							}

							if  ( pT->iSize==0 ) 
							{
								dwTick2 = GetTickCount();

								if ( nPackets<5 )
								{
									SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,3,(LPARAM)&notify);
									nPackets++;
									dwTick1 = dwTick2;
								}	
								else
								{
									if ( (dwTick2-dwTick1)>400 )
									{
										SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,3,(LPARAM)&notify);
										dwTick1 = dwTick2;
									}
								}
							}

						}

						bool bOpened = fileDownload.Open(result.szLocalFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

						if ( bOpened )
						{			
							int n = notify.vecData.size();

							for ( int i=0; i<n; i++ )
							{
								fileDownload.Write(notify.vecData[i]->pData,notify.vecData[i]->len);
							}
							fileDownload.Close();	

							bDownloaded    = true;
							result.picData = & notify;

							::SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,1,(LPARAM)&result);

							for ( int i=0; i<n; i++ )
							{
								delete [] notify.vecData[i]->pData;
								delete notify.vecData[i];
							}

							notify.vecData.clear();
						}	
					}
				}

				InternetCloseHandle(hFile);
			}
			InternetCloseHandle(hInet);
		}
	}
	else
	{
		result.isFromCache  = true;
		bDownloaded		    = true;

		::SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,1,(LPARAM)&result);
	}

	if ( bDownloaded==false )
		::SendMessage(pT->hWndNotify,WM_WEBPICTURE_NOTIFY,2,(LPARAM)&result);


	pT->pClass->m_secLockRun.Lock();

	for ( int i=0; i<pT->pClass->m_vecRunThread.size(); i++ )
	{
		if ( pT->pClass->m_vecRunThread[i]==pT )
		{
			pT->pClass->m_vecRunThread.erase(pT->pClass->m_vecRunThread.begin()+i);
			break;
		}
	}

	pT->pClass->m_secLockRun.Unlock();


	tagThreadParam *pNewT  =  NULL;
	pT->pClass->m_secLockWait.Lock();

	if ( pT->pClass->m_fifoWaitThread.size()>0 )
	{
		pNewT = pT->pClass->m_fifoWaitThread.front();
		pT->pClass->m_fifoWaitThread.pop_front();
	
		DWORD dwThreadID = 0;
		pNewT->hThread   = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)ThreadProc,
			pNewT,
			0,
			&dwThreadID);

		//SetThreadPriority(pNewT->hThread,THREAD_PRIORITY_LOWEST);

		pNewT->pClass->m_vecRunThread.push_back(pNewT);	
	}

	pT->pClass->m_secLockWait.Unlock();	

	delete pT;
	return 0;
}

CWebPictureProcess g_webPictureProcess;