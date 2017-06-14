#pragma once

#include <afxinet.h>
#include "SpaceTypeDef.h"
#include "SpaceData.h"

class CWebPictureProcess
{
public:
	CWebPictureProcess();
	~CWebPictureProcess();

public:
	struct tagThreadPictureNotify
	{
		long    pic_uid;
		long    pic_id;
		int     data_len;

		struct  buff_info
		{
			int    len   ;
			char * pData ;
		};

		vector<buff_info *> vecData;
	};

	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		long      pic_uid      ;
		long      pic_id       ;
		long	  album_id	   ;
		UINT      iSize        ;//０，默认尺寸
		DWORD     dwCreateDate ;
		char      url[255]	   ;
		HWND	  hWndNotify   ;
		CWebPictureProcess * pClass   ;

		tagThreadParam()
		{
			memset(this,0,sizeof(tagThreadParam));
		}
	};

	struct tagThreadResult
	{
		long     pic_uid;
		long     pic_id;
		long     album_id;
		DWORD    dwCreateDate;
		int		 iSize ;
		char     szLocalFile[256];
		bool	 isFromCache;//true 从本地文件加载,false从web加载
		tagThreadPictureNotify * picData;
	};


public:
	void SafeTerminate();
	int GetWebPicture(long nPicOnwerID,const TWebPictureInfo &info, UINT iPictureSize, CWnd *pWndNotify);
	CBitmap * CreateBitmap(const tagThreadPictureNotify *pNotify,int nBmpWidth, int nBmpHeight);
private:

	static UINT ThreadProc(LPVOID p);

	vector<tagThreadParam *>   m_vecRunThread   ;
	list<tagThreadParam *>     m_fifoWaitThread  ;

	CCriticalSection           m_secLockRun ;
	CCriticalSection           m_secLockWait    ;

};


extern CWebPictureProcess g_webPictureProcess;