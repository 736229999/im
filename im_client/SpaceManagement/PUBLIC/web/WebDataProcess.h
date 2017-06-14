#pragma once

#include <afxmt.h>
//下载图片列表，文章列表处理类
class CWebDataProcess
{
public:
	CWebDataProcess(void);
	~CWebDataProcess();

	enum { MAX_THREAD_COUNT=12, };

public:
	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		int       iSeq         ;
		int       iThreadType  ;//0 picture, 1 article
		HWND      hWnd;
		CString   strID;
		char      szBeginDate[45]    ;
		char	  szEndDate[45];

		vector<HWND> vecOtherWnd;

		CWebDataProcess * pClass   ;
	};

	struct tagThreadResult
	{
		BSTR	strWebResult;
		CString strBeginDate;
		CString strID;
		int     iSeq;

		tagThreadResult()
		{
			strWebResult = L"";
		}
	};

public:
	void SafeTerminate();

	int	 GetWebPictureList( 
			CString strID,
			LPCSTR lpszBeginDate,
			LPCSTR lpszEndDate,
			CWnd *pWndNotify);

	int  GetWebArticleList( CString strID,
			LPCSTR lpszBeginDate,
			LPCSTR lpszEndDate,
			CWnd *pWndNotify);


private:
	static UINT GetWebPictureListProc(LPVOID p);
	static UINT GetWebArticleListProc(LPVOID p);

	void StartProc(tagThreadParam *param);

private:
	map<int/*seq*/,tagThreadParam *>  m_mapThreadRun;
	list<tagThreadParam *>            m_lsThreadWait;
	int		 m_iSeq;
	CString	 m_strPath;
	::CCriticalSection			  m_secRun	  ;
	::CCriticalSection			  m_secWait   ;
};

extern CWebDataProcess g_webDataProcess;