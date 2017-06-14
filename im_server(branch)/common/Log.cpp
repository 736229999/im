#include "stdafx.h"
#include "Log.h"
#include "time.h"
#include "SyncCritical.h"
#include <stdarg.h>
#include "../include/json/json.h"
#include "Function.h"
#include <sys/syscall.h>


CLog* CLog::m_pInstance = NULL;
int CLog::m_nLogClass = CLog::TYPE_ERROR;
int CLog::m_eLogEncoding = CLog::ENCODE_GBK;
char CLog::m_szPath[] = {0};

pid_t gettid() 
{      
    return syscall(SYS_gettid); 
} 

CLog::CLog(void)
{
	//m_szPath[0] = '\0';
	m_nLogRecordNum = 0;
	m_pCritical = new CSyncCritical;
	m_pfLogFile = NULL;

	OpenFile();
} 

CLog::~CLog(void)
{
	CloseFile();
	if(m_pCritical)
	{
		delete m_pCritical;
		m_pCritical = NULL;
	}

	m_pInstance = NULL;
}

CLog* CLog::GetInstance()
{
	if( NULL == m_pInstance )
	{
		m_pInstance = new CLog;
	}

	return m_pInstance;
}

/*
void CLog::SetLogClass(int nLogClass)
{
	m_nLogClass=nLogClass;
}
*/

void CLog::SetLogPath(const char* szLogPath)
{	
	if(strlen(szLogPath) > 0 && strlen(szLogPath) < 256)
	{
		strcpy(m_szPath,szLogPath);
	}
	else
	{
		strcpy(m_szPath,"./Log");
	}
	return;
}

void CLog::SetLogClass(const char* szLogConfig)
{
	Json::Reader jReader(Json::Features::strictMode());
	Json::Value jValue;
	if(szLogConfig[0] == '{')
	{
		if(jReader.parse(szLogConfig, jValue))
		{
			if(jValue["debug"].asBool() == true)
				m_nLogClass |= TYPE_DEBUG;

			if(jValue["normal"].asBool() == true)
				m_nLogClass |= TYPE_NORMAL;
			
			if(jValue["important"].asBool() == true)
				m_nLogClass |= TYPE_IMPORTANT;
			
			if(jValue["statistic"].asBool() == true)
				m_nLogClass |= TYPE_STATISTIC;
			
			if(jValue["net"].asBool() == true)
				m_nLogClass |= TYPE_NET;
			
			if(jValue["encode"].asString() == "utf8")
				m_eLogEncoding = ENCODE_UTF8;

		}
		else
		{			
			m_nLogClass = TYPE_ERROR;
		}
	}
	else
	{
		m_nLogClass = TYPE_ERROR;
	}
	CLog::Log("LogConfig",CLog::TYPE_ERROR,"0x%.4x %s",m_nLogClass,szLogConfig);
}


//记录日志
//pszModule--模块名
//pInfo　--要记录的内容
//type   --类型
//			CLog::TYPE_NORMAL-----  一般的日志，仅显示
//			CLog::TYPE_IMPORTANT-- 关键性的日志，会记录到文件
//			CLog::TYPE_ERROR	---- 错误，记录到文件

void CLog::Log(const char* pszModule,TYPE_LOG type,const char* pInfo,...)
{
	if( (type & m_nLogClass) > 0 || type == TYPE_ERROR) 
	//增加无须输出日志判断	
	{
	
		GetInstance()->m_pCritical->Lock();
		memset(GetInstance()->m_szLogInfoBuf,0,LOG_MAX_DESCRIPTION_SIZE);		
		memset(GetInstance()->m_szLogInfoBufUtf8,0,LOG_MAX_UTF8_DESCRIPTION_SIZE);

		//可变参数处理
		va_list argList;
		va_start(argList, pInfo);
		//_vstprintf_s(GetInstance()->m_szLogInfoBuf, pInfo, argList);
		vsnprintf(GetInstance()->m_szLogInfoBuf, LOG_MAX_DESCRIPTION_SIZE-1, pInfo, argList);
		va_end(argList);
		
 		if(m_eLogEncoding == ENCODE_UTF8)
		{			
			int ret = gbk2utf8(GetInstance()->m_szLogInfoBufUtf8, GetInstance()->m_szLogInfoBuf);
		 	
			if(ret < 0 && ret != -5 )
			{
				if(ret!= -7) //src messages is empty but not null
				{				
					GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBuf,type);
				}
				else
				{
					char strError[256];
					sprintf(strError, "(utf8Error:%d)", ret);
					strcat(GetInstance()->m_szLogInfoBufUtf8, strError);
				}
			}
			//记录日志			
			GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBufUtf8,type);
		}
		else
		{			
			//记录日志
			GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBuf,type);
		}
		GetInstance()->m_pCritical->Unlock();
	}


}

void CLog::Log(const char* pszModule,const char* pInfo,...)
{

	//if(m_nLogClass==NOLOG) return;

	GetInstance()->m_pCritical->Lock();

	//可变参数处理
	va_list argList;
	va_start(argList, pInfo);
	//_vstprintf_s(GetInstance()->m_szLogInfoBuf, pInfo, argList);
	vsnprintf(GetInstance()->m_szLogInfoBuf, LOG_MAX_DESCRIPTION_SIZE-1, pInfo, argList);
	va_end(argList);

	//记录日志
	GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBuf,CLog::TYPE_NORMAL);

	GetInstance()->m_pCritical->Unlock();
}


void  CLog::SimpleLog(const char* pszModule,const char* pInfo,TYPE_LOG type)
{
	static const 	char szTypeDebug[] = "DEBUG";		 //Debug
	static const	char szTypeNormal[] = "NORMAL"; 		 //Normal
	static const	char szTypeImportant[] = "INFO";    // Important
	static const	char szTypeStatistic[] = "STATIS";    // Statistic
	static const	char szTypeError[] = "ERROR";
	static const 	char szTypeNet[] = "NET";
		
	char	szItem[LOG_MAX_DESCRIPTION_SIZE] = {0};
	const	char*	pszType;

	//if(m_nLogClass==NOLOG) return;

	//格式化日志信息
	switch(type) 
	{
		case TYPE_DEBUG:

			//#ifdef DEBUG
				pszType = szTypeDebug;
			//#else
			//非调试状态不显示
			//return;
			//#endif

			break;
		case TYPE_NORMAL:
			pszType = szTypeNormal;
			break;
		case TYPE_IMPORTANT:
			pszType = szTypeImportant;
			break;
		case TYPE_ERROR:
			pszType = szTypeError;
			break;
		case TYPE_STATISTIC:
			pszType = szTypeStatistic;
			break;
		case TYPE_NET:
			pszType = szTypeNet;
			break;
		default:
			return;
	}

	struct tm newtime;
	time_t aclock;

	time( &aclock ); 
	
	localtime_s(&newtime, &aclock );   
	char szTime[32];
	strftime(szTime,sizeof(szTime), "%Y-%m-%d %H:%M:%S", &newtime);

	_snprintf_s(szItem,sizeof(szItem),"[%s] %s [%u][%s] - %s ",
							szTime, pszType,gettid(),pszModule, pInfo);


	OutputDebugString(szItem);
	//printf(szItem);

/*
	if (type == TYPE_TEST)
	{
		
		char szPath[MAX_PATH],szMoudleName[MAX_PATH];

		GetModuleFileName(NULL,szPath,sizeof(szPath));
		strcpy_s(szMoudleName,strrchr(szPath,'/')+1);
		char* pEndPos = strrchr(szMoudleName,'.');
		*pEndPos		  = '\0';
		strcpy_s(strrchr(szPath,'/'),MAX_PATH,"/Log");

		char szfile[MAX_PATH],szfilename[MAX_PATH];
		strftime(szfile,sizeof(szfile), "%Y-%m-%d %H-%M-%S", &newtime);
		sprintf_s(szfilename, "%s/%s.log", szPath,szfile);
		FILE *pfile;
		pfile = fopen(szfilename, "a+");
		if(!pfile)
			return;
		fprintf(pfile, "%s", szItem);
		fflush(pfile);
		fclose(pfile);
		pfile = NULL;
		
	}
	*/

	//错误日志总是打印
	if( (type & m_nLogClass) > 0 || type == TYPE_ERROR)
	{
		fprintf(m_pfLogFile,"%s\n",szItem);
		fflush(m_pfLogFile);
		if( ++m_nLogRecordNum > 500000)
		{
			//如果日志数记录太多了，换一个日志文件，以免单个文件太大
			OpenFile();
			m_nLogRecordNum = 0 ;
		}
	}



}

void CLog::Log(const char* str)
{
	printf("%s\n",str);
}


//得到日志文件路径
const char* CLog::GetLogFilePath()
{
	char filename[128] = {0};
	memset(m_szFilePath,0,sizeof(m_szFilePath));
	GetModuleFileName(NULL,filename,sizeof(filename));
	strcpy_s(m_szMoudleName,strrchr(filename,'/')+1);
	char* pEndPos = strrchr(m_szMoudleName,'.');
	if( pEndPos )
	{
		*pEndPos		  = '\0';
	}

/*
	strcpy_s(strrchr(m_szPath,'/'),MAX_PATH,"/Log");
*/
	if(strlen(m_szPath) <= 0)
	{
		strcpy(m_szPath,"./Log");
	}
	
	CreateDirectory(m_szPath,NULL);

	//zhangmin
	//CreateDirectory("./ImageCache",NULL);
	//CreateDirectory("./ImageCache/ImageFile",NULL);
	//CreateDirectory("./ImageCache/ImageUrl",NULL);

	struct tm newtime;
	time_t aclock;

	time( &aclock );   
	localtime_s(&newtime, &aclock );   
	
	strcpy(m_szFilePath,m_szPath);
	strcat_s(m_szFilePath,"/");

	char szTimeName[64];
	strftime(szTimeName,sizeof(szTimeName), "_%Y%m%d_%H_%M.log", &newtime);
	char szFileName[64];
	sprintf_s(szFileName,"%s%s",m_szMoudleName,szTimeName);

	strcat_s(m_szFilePath,szFileName);
	return m_szFilePath;
    

}

void  CLog::OpenFile()
{
	//if(m_nLogClass==NOLOG) return;

	CloseFile();
	
	m_pfLogFile = fopen(GetLogFilePath(),"w");

	if( NULL == m_pfLogFile)
	{
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!error!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!error!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("!!!can't open log file,all log info will not save!!!!!");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!error!!!!!!!!!!!!!!!!!!!!!!!\n");
		printf("!!!!!!!!!!!!!!!!!!!!!!!!!!error!!!!!!!!!!!!!!!!!!!!!!!\n");
	}
}

void  CLog::CloseFile()
{
	//if(m_nLogClass==NOLOG) return;

	if( m_pfLogFile )
	{
		fclose(m_pfLogFile);
		m_pfLogFile = NULL;
	}
}

