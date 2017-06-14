#ifndef LOG_H
#define LOG_H

#include "WinCommonForLinux.h"



/************************************************************************ 
文件描述：日志类的声明文件 

创建人：hechang
时间：	2006-6-13 
************************************************************************/
#define LOG_MAX_DESCRIPTION_SIZE 1024
#define LOG_MAX_UTF8_DESCRIPTION_SIZE 1536

class CSyncCritical;

//日志类，记录程序的平常日志与错误信息
 
class CLog
{
public:

	~CLog(void);

	enum TYPE_LOG 
		{ 
			TYPE_DEBUG =     1,		//0x01<<0
			TYPE_NORMAL =    2,		//0x01<<1,
			TYPE_IMPORTANT = 4,		//0x01<<2
			TYPE_STATISTIC = 8,		//0x01<<3
			TYPE_ERROR 	   = 16,	//0x01<<4 
			TYPE_NET  	   = 32,    //0x01<<5
		};
	enum ENCODE_LOG
		{
			ENCODE_GBK =  1,
			ENCODE_UTF8 = 2,
		};

	//enum LOG_CLASS { NOLOG /*, LOG_SCR, LOG_FILE, LOG_ALL, LOG_DEBUG*/ };

	//记录日志
	static void Log(const char* pszModule,TYPE_LOG type,const char* pInfo,...);

	//记录类型为普通的日志
	static void Log(const char* pszModule,const char* pInfo,...);

	//统计日志
	static void Log(const char* str);

	//设定日志级别
	//static void SetLogClass(int nLogClass=LOG_ALL);
	static void SetLogClass(const char* szLogConfig);
	static void SetLogPath(const char* szLogPath);
private:
	CLog(void);
	static CLog* GetInstance();
	const char* GetLogFilePath();
	void  SimpleLog(const char* pszModule,const char* pInfo,TYPE_LOG type );
	void  OpenFile();
	void  CloseFile();

private:
	static CLog*  m_pInstance;
	FILE*		m_pfLogFile;
	static char		m_szPath[MAX_PATH];
	char 		m_szFilePath[MAX_PATH]; //日志完整路径
	int			m_nLogRecordNum;		//在当前日志文件上记录的日志数
	char		m_szLogInfoBuf[LOG_MAX_DESCRIPTION_SIZE];
	char		m_szLogInfoBufUtf8[LOG_MAX_UTF8_DESCRIPTION_SIZE];
	CSyncCritical*		m_pCritical;
	static int			m_nLogClass;
	static int	m_eLogEncoding;

	char		m_szMoudleName[MAX_PATH];

	
};

#endif

