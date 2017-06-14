#pragma once

#include "impublic.h"
#include "spacetypedef.h"

#include <map>
using namespace std;

// 获取要下载文件的长度
double GetTotalFileLenth(const char* url);

class CMagicFaceProcess
{
public:
	CMagicFaceProcess()
	{
		m_dwIndex = 0;
	}

	~CMagicFaceProcess() { SafeTerminate(); }

public:
	void ShowMagicFace(const CString& strFaceUrl);

	void SafeTerminate();
	void SetCachePath(CString strPath) { m_strCachePath      = strPath; }
	void SetMagicPlayerPath(CString strPath,CString strPlayer="MagicFace.exe");
	CString GetMagicFaceFile(char * face_url);

public:
	struct tagThreadParam
	{
		DWORD   dwIndex;
		HANDLE  hThread;
		char    face_url[255];
		CMagicFaceProcess * pClass;
	};

private:
	static UINT ThreadProc(LPVOID param);

private:
	map<DWORD,tagThreadParam *>  m_mapThread ;
	CString				m_strCachePath   ;
	CString             m_strPlayerPath, m_strPlayer;
	CCriticalSection	m_secLock   ;
	DWORD               m_dwIndex   ;
};
//下载web文件处理类
class CWebFileProcess
{
public:
	CWebFileProcess(void);
	~CWebFileProcess(void);

public:
	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		DWORD     dwUserFlagData         ;
		HWND      hWnd;
		char      file_url[200];
		CWebFileProcess * pClass;
	};

public:
	void SafeTerminate();
	void SetCachePath(CString strPath) { m_strPath=strPath;
		CreateDirectory(m_strPath,NULL); }
	void GetWebFile(DWORD dwUserFlagData, char * file_url, HWND hWndNotify);
	CString Url2File(char * file_url);

private:
	static UINT ThreadProc(LPVOID param);

private:
	map<DWORD,tagThreadParam *>      m_mapThread   ;//map<user flag data,thread *>
	
	CString						  m_strPath     ;	
	CCriticalSection			  m_secLockTh  ;
};

//用户头像下载管理类
class CWebUserHeadIconProcess
{
public:
	CWebUserHeadIconProcess(void);
	~CWebUserHeadIconProcess(void);

public:
	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		uint32    fid          ;
		//uint16    face         ;
	//	bool      bSystemFace  ;
		HWND	  hNotifyWnd   ;
		char      face_url[255];
		CWebUserHeadIconProcess * pClass;
	};

	struct tagFidHwnd
	{
		uint32 fid;
		HWND hWnd;
	};

public:
	void RegistIconNotify(HWND hWnd);
	void UnRegistIconNotify(HWND hWnd);

public:
	void SafeTerminate();
	void SetCachePath(CString strPath) { m_strPath=strPath; }
	void GetUserHeadIcon(const XT_USER_INFO *pUser, CWnd *pWndNotify);
	CString Url2File(char * face_url);
	CString GetFaceFile(const XT_USER_INFO *pUser);

private:
	static UINT ThreadProc(LPVOID param);

private:
	map<uint32,tagThreadParam *>  m_mapThreadRun ;
	map<CString,vector<tagFidHwnd *> *>   m_mapFace      ;//map<face url, HWNDs>
	
	vector<HWND>                  m_vecHWnd     ;
	CString						  m_strPath     ;	
	CCriticalSection			  m_secLockTh  ;
	CCriticalSection              m_secLockID   ;

};



//群族徽下载管理类
class CWebFamilyHeadIconProcess
{
public:
	CWebFamilyHeadIconProcess(void);
	~CWebFamilyHeadIconProcess(void);

public:
	struct tagSysFaceJob
	{
		uint32 family_id;
		HWND   hWnd;
	};

	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		uint32    family_id    ;
		uint16    face         ;
		bool      bSystemFace  ;

		vector<HWND>  vecHNotifyWnd   ;
		vector<tagSysFaceJob> vecSysFaceJob;

		char      face_url[255];
		CWebFamilyHeadIconProcess * pClass;
	};

public:
	void SafeTerminate();
	void SetCachePath(CString strPath) { m_strPath=strPath; }
	void GetFamilyHeadIcon(const XT_CLUSTER_INFO *pCluster, CWnd *pWndNotify);
	CString Url2File(char * face_url);
	CString GetFaceFile(const XT_CLUSTER_INFO *pCluster);

public:
	void RegistIconNotify(HWND hWnd);
	void UnRegistIconNotify(HWND hWnd);

protected:
	static UINT ThreadProc(LPVOID param);

protected:
	map<uint32, tagThreadParam *>	  m_mapThreadRun ;//map<family id, thread param>
	vector<HWND>				  m_vecHWnd     ;
	CString						  m_strPath     ;	
	CCriticalSection			  m_secLockTh   ;
	CCriticalSection			  m_secLockHW   ;
};

//下载闪秀
class CWebFlashProcess
{
public:
	CWebFlashProcess();
	~CWebFlashProcess();

public:
	struct tagThreadParam     
	{
		HANDLE    hThread      ;
		uint32    user_id      ;
		HWND      hWndNotify   ;

		char      flash_url[255];

		CWebFlashProcess * pClass;
	};

public:
	void SafeTerminate();
	void SetCachePath(CString strPath) { m_strPath=strPath; CreateDirectory((LPCSTR)m_strPath,NULL); }
	void GetUserFlashShow(const XT_USER_INFO * pUser, CWnd *pWndNotify);
	CString Url2File(char * flash_url);
	CString GetFlashFile(const XT_USER_INFO * pUser);

protected:
	static UINT ThreadProc(LPVOID param);
	map<uint32, tagThreadParam *>  m_mapThreadRun;//map<userid, thread param>
	CCriticalSection               m_secLockRun;
	CString                        m_strPath;
};

extern CWebUserHeadIconProcess		g_webUserHeadIconProcess;
extern CWebFamilyHeadIconProcess	g_webFamilyHeadIconProcess;
extern CMagicFaceProcess			g_magicFaceProcess;
extern CWebFileProcess				g_webFileProcess;
extern CWebFlashProcess             g_webFlashProcess;
