#pragma once


#define WININET

#include <stdio.h>
#ifdef	WININET
#include <wininet.h>
#pragma comment(lib,"Wininet.lib")
#else
#include <winsock2.h>
#endif

#include <string>
#include <vector>
#import  "msxml3.dll"
using namespace std;


class FACE_INFO
{
public:
	void	Clear()
	{ sFile = ""; sShortCut = ""; sTip = "";  };
public:
	string	sFile;		// 文件名
	string	sShortCut;	// 快捷键
	string	sTip;		// 备注
	string	sUrl;		// 链接地址
};

class FACE_DIR:public vector<FACE_INFO>
{
public:
	string		sName;
};

class FACE_LIB :public vector<FACE_DIR>
{
public:
	int		Total()
	{ 
		int nRt=0;
		size_t i,n;
		n=size();
		for(i=0;i<size();i++)
			nRt+=(*this)[i].size();
		return nRt;
	};
};


bool GetFaceLib(const string& sConfig, FACE_LIB& lib);

class CHttpDownload
{

public:

	CHttpDownload(void);

	~CHttpDownload(void);

#ifdef WININET

	static bool	DownLoadWebFile(const char* szUrl, const char * szLocalFile);

#else

public:

	bool	Init();

	void	UnInit();

	bool	ConnectSite(const char* szServer);

	bool	GetHTTPFile(const char* szUrl,const char* szSavedFile);
	
	
protected:

	bool		m_bConnectOK;
	LPHOSTENT	m_lpHostEntry;
	SOCKADDR_IN m_saServer;
	SOCKET		m_socket;
#endif

public:
	const char* GetLastErrMsg() const
	{ return m_sErr.c_str(); };
protected:
	std::string	m_sErr;
};

class CFaceFileList:public vector<FACE_INFO>
{
public:
	void		clear()
	{ m_sDir = ""; vector<FACE_INFO>::clear(); };

	void		SetDir(const string& sDir)
	{ m_sDir = sDir; };

	string		GetFaceFullPath(int n) const
	{ return m_sDir+(*this)[n].sFile; };
protected:
	string		m_sDir;
};

class CFaceDownload:public CHttpDownload
{

public:

	CFaceDownload();

	virtual ~CFaceDownload();

public:

	bool	DownloadFace(
		const char * szUrlDir,
		const char * szSavedPath,
		const vector<string>& sExistFiles 
	);
	//------------------------------------------------------
	// 当前是否有下载线程
	//------------------------------------------------------
	bool	IsDownloading() const
	{ return m_bProcExit==false; };
	//------------------------------------------------------
	// 当前对象是否为下载器
	//------------------------------------------------------
	bool	IsDownloader() const
	{ return m_bDownloading; };
	
	bool	HasDownloadErr() const
	{ return m_bDownloadErr; };

	const CFaceFileList&	GetFaceList() const
	{ return m_faceFileList; };

	int		GetTask() const
	{ return m_nTask;};

	int		GetProcess() const
	{ return m_nProcess; };

	void	StopDownload()
	{ 
		// 仅关闭属于自己的下载进程
		if(m_bDownloading = true)
		{
			m_bDownloading = false;
			m_bProcExit = true; 
		}
	};

protected:

	friend	UINT DownloadProc( LPVOID pParam );

protected:

	CFaceFileList	m_faceFileList;	// 下载到本地的文件列表
	vector<string>	m_sExistFiles;	// 当前存在的web文件列表
	string			m_sServer;		// 被连接的主机
	string			m_sTempCfg;		// 本地临时缓存配置文件
	string			m_sSavedDir;	// 下载到本地的目录
	string			m_sUrlCfg;		// 服务器上的配置文件
	string			m_sUrlDir;		// 服务器上的目录
	bool			m_bDownloading;	// 是否正在下载任务
	bool			m_bDownloadErr;	// 是否有下载错误 
	int				m_nTask;		// 服务器上需要下载的文件总数
	int				m_nProcess;		// 当前下载的文件数
public:

	static	bool	m_bProcExit;	// 当前的下载进程是否退出
};
