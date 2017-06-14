#include "StdAfx.h"
#include ".\log.h"
#include "time.h"
#include "SyncCritical.h"

CLog* CLog::m_pInstance = NULL;

CLog::CLog(void)
{
	m_szPath[0] = '\0';
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

//��¼��־
//pszModule--ģ����
//pInfo��--Ҫ��¼������
//type   --����
//			CLog::TYPE_NORMAL-----  һ�����־������ʾ
//			CLog::TYPE_IMPORTANT-- �ؼ��Ե���־�����¼���ļ�
//			CLog::TYPE_ERROR	---- ���󣬼�¼���ļ�

void CLog::Log(const char* pszModule,TYPE_LOG type,char* pInfo,...)
{
	
	GetInstance()->m_pCritical->Lock();

	//�ɱ��������
	va_list argList;
	va_start(argList, pInfo);
	_vstprintf(GetInstance()->m_szLogInfoBuf, pInfo, argList);
	va_end(argList);

	//��¼��־
	GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBuf,type);

	GetInstance()->m_pCritical->Unlock();
}

void CLog::Log(const char* pszModule,char* pInfo,...)
{

	GetInstance()->m_pCritical->Lock();

	//�ɱ��������
	va_list argList;
	va_start(argList, pInfo);
	_vstprintf(GetInstance()->m_szLogInfoBuf, pInfo, argList);
	va_end(argList);

	//��¼��־
	GetInstance()->SimpleLog(pszModule,GetInstance()->m_szLogInfoBuf,CLog::TYPE_NORMAL);

	GetInstance()->m_pCritical->Unlock();
}


void  CLog::SimpleLog(const char* pszModule,char* pInfo,TYPE_LOG type)
{
	static const	char szTypeNormal[] = "";
	static const	char szTypeImportant[] = "**Important**";
	static const	char szTypeError[] = "!!!!!!!!!!!!!!!Error!!!!!!!!!!!";

	char	szItem[LOG_MAX_DESCRIPTION_SIZE];
	const	char*	pszType;
	bool	bIsWriteFile = false;

	//��ʽ����־��Ϣ
	switch(type) 
	{
	case TYPE_DEBUG:

		#ifdef _DEBUG
			pszType = szTypeNormal;
			bIsWriteFile = false;
		#else
			//�ǵ���״̬����ʾ
			return;
		#endif

		break;
	case TYPE_NORMAL:
		pszType = szTypeNormal;
		bIsWriteFile = false;
		break;
	case TYPE_IMPORTANT:
		pszType = szTypeImportant;
		bIsWriteFile = true;
		break;
	case TYPE_ERROR:
		pszType = szTypeError;
		bIsWriteFile = true;
		break;
	default:
		return;
	}

	struct tm *newtime;
	time_t aclock;

	time( &aclock );   
	newtime = localtime( &aclock );   
	char szTime[32];
	strftime(szTime,sizeof(szTime), "%Y-%m-%d %H:%M:%S", newtime);

	_snprintf(szItem,sizeof(szItem),"[time]%s [Module]%s %s\n  [Info]%s\n",
							szTime,pszModule,pszType,pInfo);


	printf("%s",szItem);

	if( bIsWriteFile )
	{
		fprintf(m_pfLogFile,"%s",szItem);
		fflush(m_pfLogFile);
		if( ++m_nLogRecordNum > 10000)
		{
			//�����־����¼̫���ˣ���һ����־�ļ������ⵥ���ļ�̫��
			OpenFile();
			m_nLogRecordNum = 0 ;
		}
	}



}

//�õ���־�ļ�·��
const char* CLog::GetLogFilePath()
{

	GetModuleFileName(NULL,m_szPath,sizeof(m_szPath));
	strcpy(m_szMoudleName,strrchr(m_szPath,'\\')+1);
	char* pEndPos = strrchr(m_szMoudleName,'.');
	*pEndPos		  = '\0';


	strcpy(strrchr(m_szPath,'\\'),"\\Log");

	CreateDirectory(m_szPath,NULL);

	struct tm *newtime;
	time_t aclock;

	time( &aclock );   
	newtime = localtime( &aclock );   

	strcat(m_szPath,"\\");

	char szTimeName[64];
	strftime(szTimeName,sizeof(szTimeName), "%Y%m%d%H%M%S.txt", newtime);
	char szFileName[64];
	sprintf(szFileName,"%s%s",m_szMoudleName,szTimeName);

	strcat(m_szPath,szFileName);

	return m_szPath;
    

}

void  CLog::OpenFile()
{
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
	if( m_pfLogFile )
	{
		fclose(m_pfLogFile);
		m_pfLogFile = NULL;
	}
}

