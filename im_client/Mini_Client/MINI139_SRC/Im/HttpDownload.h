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
	string	sFile;		// �ļ���
	string	sShortCut;	// ��ݼ�
	string	sTip;		// ��ע
	string	sUrl;		// ���ӵ�ַ
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
	// ��ǰ�Ƿ��������߳�
	//------------------------------------------------------
	bool	IsDownloading() const
	{ return m_bProcExit==false; };
	//------------------------------------------------------
	// ��ǰ�����Ƿ�Ϊ������
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
		// ���ر������Լ������ؽ���
		if(m_bDownloading = true)
		{
			m_bDownloading = false;
			m_bProcExit = true; 
		}
	};

protected:

	friend	UINT DownloadProc( LPVOID pParam );

protected:

	CFaceFileList	m_faceFileList;	// ���ص����ص��ļ��б�
	vector<string>	m_sExistFiles;	// ��ǰ���ڵ�web�ļ��б�
	string			m_sServer;		// �����ӵ�����
	string			m_sTempCfg;		// ������ʱ���������ļ�
	string			m_sSavedDir;	// ���ص����ص�Ŀ¼
	string			m_sUrlCfg;		// �������ϵ������ļ�
	string			m_sUrlDir;		// �������ϵ�Ŀ¼
	bool			m_bDownloading;	// �Ƿ�������������
	bool			m_bDownloadErr;	// �Ƿ������ش��� 
	int				m_nTask;		// ����������Ҫ���ص��ļ�����
	int				m_nProcess;		// ��ǰ���ص��ļ���
public:

	static	bool	m_bProcExit;	// ��ǰ�����ؽ����Ƿ��˳�
};
