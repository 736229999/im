#include "StdAfx.h"
#include ".\webdataprocess.h"
#include "WebInterfaceWrapper.h"

//class CWebDataProcess
CWebDataProcess::CWebDataProcess()
{
	m_iSeq = 0;
}

CWebDataProcess::~CWebDataProcess()
{
}


void CWebDataProcess::SafeTerminate()
{
	//m_secRun.Lock();

	map<int,tagThreadParam *>::iterator it;
	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		HANDLE hThread = it->second->hThread;

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

	//m_secRun.Unlock();

	list<tagThreadParam *>::iterator itWait;
	for ( itWait=m_lsThreadWait.begin();
		itWait!=m_lsThreadWait.end();
		itWait++ )
	{
		delete (*itWait);
	}

	m_lsThreadWait.clear();
}

int CWebDataProcess::GetWebPictureList(CString strID, LPCSTR lpszBeginDate,LPCSTR lpszEndDate,CWnd *pWndNotify)
{
	map<int,tagThreadParam *>::iterator it;

	tagThreadParam * param = new tagThreadParam;
	param->iSeq        = m_iSeq++;
	param->hWnd        = pWndNotify->m_hWnd;
	strncpy(param->szBeginDate,lpszBeginDate,sizeof(param->szBeginDate)-1);
	strncpy(param->szEndDate,lpszEndDate,sizeof(param->szEndDate)-1);

	param->pClass      = this;
	param->strID       = strID;
	param->iThreadType = 0;
	DWORD dwThreadID   = 0;

	m_secRun.Lock();

	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		tagThreadParam *p = it->second;
		if ( p->iThreadType==param->iThreadType &&
			p->strID==param->strID &&
			strcmp(p->szBeginDate,param->szBeginDate)==0 &&
			strcmp(p->szEndDate,param->szEndDate)==0 )
		{

			int iSeq = m_iSeq-1;

			if ( p->hWnd==param->hWnd )
			{
				iSeq = -1;
				delete param;
			}
			else
			{
				p->vecOtherWnd.push_back(param->hWnd);
			}

			m_secRun.Unlock();
			return iSeq;
		}
	}

	if ( m_mapThreadRun.size()<MAX_THREAD_COUNT )
	{        
		DWORD dwThreadID = 0;
		m_mapThreadRun.insert(map<int,tagThreadParam *>::value_type(param->iSeq,param));

		m_secRun.Unlock();

		param->hThread  = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)GetWebPictureListProc,
			param,
			0,
			&dwThreadID);	

	}
	else
	{
		m_secRun.Unlock();

		//	m_secWait.Lock();
		m_lsThreadWait.push_back(param);
		//	m_secWait.Unlock();
	}

	return param->iSeq;
}

UINT CWebDataProcess::GetWebPictureListProc(LPVOID p)
{
	tagThreadParam *pThread     = (tagThreadParam *)p;
	CWebDataProcess	   * pClass = pThread->pClass;

	map<int,tagThreadParam *>::iterator it;

	USES_CONVERSION;

	::CoInitialize(0);

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

	tagThreadResult result;

	result.strID = pThread->strID;
	result.iSeq  = pThread->iSeq;
	result.strBeginDate = pThread->szBeginDate;
	objWI.GetWebInterface()->raw_GetUserPictureList(atol((LPCSTR)result.strID),A2W(pThread->szBeginDate),A2W(pThread->szEndDate),&result.strWebResult);

	pClass->m_secRun.Lock();
	it = pClass->m_mapThreadRun.find(pThread->iSeq);

	if ( it!=pClass->m_mapThreadRun.end() )
	{
		pClass->m_mapThreadRun.erase(it);		
	}

	pClass->m_secWait.Lock();

	if ( pClass->m_lsThreadWait.size()>0 )
	{
		tagThreadParam * pNewT = pClass->m_lsThreadWait.front();
		pClass->m_lsThreadWait.pop_front();
		pClass->StartProc(pNewT);
	}


	pClass->m_secWait.Unlock();
	pClass->m_secRun.Unlock();

	::CoUninitialize();

	::SendMessage(pThread->hWnd,WM_WEBDATA_NOTIFY,0,(LPARAM)&result);

	for ( int i=0; i<pThread->vecOtherWnd.size(); i++ )
		::SendMessage(pThread->vecOtherWnd[i],WM_WEBDATA_NOTIFY,0,(LPARAM)&result);

	try
	{
		CloseHandle(pThread->hThread);
		delete pThread;
	}
	catch(...)
	{
	}

	//TRACE("GetWebPictureListProc delete pThread and SendMessage\n ");

	return 0;
}


int CWebDataProcess::GetWebArticleList(CString strID, LPCSTR lpszBeginDate,LPCSTR lpszEndDate,CWnd *pWndNotify)
{
	map<int,tagThreadParam *>::iterator it;

	tagThreadParam * param = new tagThreadParam;
	param->iSeq        = m_iSeq++;
	param->hWnd        = pWndNotify->m_hWnd;
	strncpy(param->szBeginDate,lpszBeginDate,sizeof(param->szBeginDate)-1);
	strncpy(param->szEndDate,lpszEndDate,sizeof(param->szEndDate)-1);

	param->pClass      = this;
	param->strID       = strID;
	param->iThreadType = 1;

	m_secRun.Lock();

	for ( it=m_mapThreadRun.begin(); it!=m_mapThreadRun.end(); it++ )
	{
		tagThreadParam *p = it->second;
		if ( p->iThreadType==param->iThreadType &&
			p->strID==param->strID &&
			strcmp(p->szBeginDate,param->szBeginDate)==0 &&
			strcmp(p->szEndDate,param->szEndDate)==0 )
		{
			int iSeq = m_iSeq-1;

			if ( p->hWnd==param->hWnd )
			{
				iSeq = -1;
				delete param;
			}
			else
			{
				p->vecOtherWnd.push_back(param->hWnd);
			}

			m_secRun.Unlock();
			return iSeq;
		}
	}

	if ( m_mapThreadRun.size()<MAX_THREAD_COUNT )
	{        
		DWORD dwThreadID = 0;
		m_mapThreadRun.insert(map<int,tagThreadParam *>::value_type(param->iSeq,param));

		m_secRun.Unlock();

		param->hThread  = CreateThread(NULL,0,
			(LPTHREAD_START_ROUTINE)GetWebArticleListProc,
			param,
			0,
			&dwThreadID);	

	}
	else
	{
		m_secRun.Unlock();

		//	m_secWait.Lock();
		m_lsThreadWait.push_back(param);
		//	m_secWait.Unlock();
	}

	return param->iSeq;
}

void CWebDataProcess::StartProc(tagThreadParam *param)
{
	DWORD dwThreadID = 0;
	map<int,tagThreadParam*>::iterator it = m_mapThreadRun.find(param->iSeq);
	if ( it==m_mapThreadRun.end() )
		m_mapThreadRun.insert(map<int,tagThreadParam *>::value_type(param->iSeq,param));
	else
		it->second = param;

	param->hThread  = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)(param->iThreadType==0 ? GetWebPictureListProc : GetWebArticleListProc),
		param,
		0,
		&dwThreadID);	
}

UINT CWebDataProcess::GetWebArticleListProc(LPVOID p)
{
	tagThreadParam *pThread     = (tagThreadParam *)p;
	CWebDataProcess	   * pClass = pThread->pClass;

	map<int,tagThreadParam *>::iterator it;
	//TRACE("begin GetWebArticleListProc\n ");

	USES_CONVERSION;

	::CoInitialize(0);

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

	tagThreadResult result;
	result.strID = pThread->strID;
	result.iSeq  = pThread->iSeq;
	result.strBeginDate = pThread->szBeginDate;
	objWI.GetWebInterface()->raw_GetUserArticleList( atol((LPCSTR)pThread->strID),A2W(pThread->szBeginDate),A2W(pThread->szEndDate),&result.strWebResult);

	pClass->m_secRun.Lock();
	it = pClass->m_mapThreadRun.find(pThread->iSeq);

	if ( it!=pClass->m_mapThreadRun.end() )
	{
		pClass->m_mapThreadRun.erase(it);		
	}

	//TRACE("GetWebArticleListProc pClass->m_secRun.Unlock()\n ");
	pClass->m_secWait.Lock();

	if ( pClass->m_lsThreadWait.size()>0 )
	{
		tagThreadParam * pNewT = pClass->m_lsThreadWait.front();
		pClass->m_lsThreadWait.pop_front();
		pClass->StartProc(pNewT);
	}

	pClass->m_secWait.Unlock();	

	pClass->m_secRun.Unlock();

	::CoUninitialize();

	::SendMessage(pThread->hWnd,WM_WEBDATA_NOTIFY,1,(LPARAM)&result);

	for ( int i=0; i<pThread->vecOtherWnd.size(); i++ )
		::SendMessage(pThread->vecOtherWnd[i],WM_WEBDATA_NOTIFY,1,(LPARAM)&result);

	//TRACE("GetWebArticleListProc  	delete pThread & and SendMessage\n ");

	try
	{
		CloseHandle(pThread->hThread);
		delete pThread;
	}
	catch(...)
	{
	}

	return 0;
}


CWebDataProcess g_webDataProcess;