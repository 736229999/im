#ifndef LOG_H
#define LOG_H

#include "WinCommonForLinux.h"



/************************************************************************ 
�ļ���������־��������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-13 
************************************************************************/
#define LOG_MAX_DESCRIPTION_SIZE 1024
#define LOG_MAX_UTF8_DESCRIPTION_SIZE 1536

class CSyncCritical;

//��־�࣬��¼�����ƽ����־�������Ϣ
 
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

	//��¼��־
	static void Log(const char* pszModule,TYPE_LOG type,const char* pInfo,...);

	//��¼����Ϊ��ͨ����־
	static void Log(const char* pszModule,const char* pInfo,...);

	//ͳ����־
	static void Log(const char* str);

	//�趨��־����
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
	char 		m_szFilePath[MAX_PATH]; //��־����·��
	int			m_nLogRecordNum;		//�ڵ�ǰ��־�ļ��ϼ�¼����־��
	char		m_szLogInfoBuf[LOG_MAX_DESCRIPTION_SIZE];
	char		m_szLogInfoBufUtf8[LOG_MAX_UTF8_DESCRIPTION_SIZE];
	CSyncCritical*		m_pCritical;
	static int			m_nLogClass;
	static int	m_eLogEncoding;

	char		m_szMoudleName[MAX_PATH];

	
};

#endif

