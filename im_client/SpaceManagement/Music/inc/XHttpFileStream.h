#pragma once
#include <XMemStream.h>
#include <wininet.h>
#include <string>
using namespace std;


#ifdef AUDIOPLAYERDLL_EXPORTS
#define AUDIOPLAYERDLL_API __declspec(dllexport)
#else
#define AUDIOPLAYERDLL_API __declspec(dllimport)
#endif


class AUDIOPLAYERDLL_API XHttpFileStream : public XMemStream
{
public:

	XHttpFileStream(void);

	virtual ~XHttpFileStream(void);

public:

	bool				Open(const char * szUrl,const char * szSaveTemp = NULL);

	bool			    GetCacheFilePath(char * szBuff, int nBufLen);

protected:

	static DWORD WINAPI DownloadThread(void * p);

	void				Download();

protected:

	bool				m_bDownload;

	HANDLE				m_hDownload;

	HINTERNET			m_hFile;

	HINTERNET			m_hInet;

	string				m_sUrl;

	string				m_sSave;

};
