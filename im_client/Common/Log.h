#pragma once

/************************************************************************ 
文件描述：日志类的声明文件 

创建人：hechang
时间：	2006-6-13 
************************************************************************/

#define LOG_MAX_DESCRIPTION_SIZE 1024

class CSyncCritical;

//日志类，记录程序的平常日志与错误信息
 
class CLog
{
public:

	~CLog(void);

	enum TYPE_LOG {TYPE_DEBUG,TYPE_NORMAL,TYPE_IMPORTANT,TYPE_ERROR };

	//记录日志
	static void Log(const char* pszModule,TYPE_LOG type,char* pInfo,...);

	//记录类型为普通的日志
	static void Log(const char* pszModule,char* pInfo,...);

private:
	CLog(void);
	static CLog* GetInstance();
	const char* GetLogFilePath();
	void  SimpleLog(const char* pszModule,char* pInfo,TYPE_LOG type );
	void  OpenFile();
	void  CloseFile();

private:
	static CLog*  m_pInstance;
	FILE*		m_pfLogFile;
	char		m_szPath[MAX_PATH];
	int			m_nLogRecordNum;		//在当前日志文件上记录的日志数
	char		m_szLogInfoBuf[LOG_MAX_DESCRIPTION_SIZE];
	CSyncCritical*		m_pCritical;

	char		m_szMoudleName[MAX_PATH];

	
};

