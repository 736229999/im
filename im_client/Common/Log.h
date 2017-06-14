#pragma once

/************************************************************************ 
�ļ���������־��������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-6-13 
************************************************************************/

#define LOG_MAX_DESCRIPTION_SIZE 1024

class CSyncCritical;

//��־�࣬��¼�����ƽ����־�������Ϣ
 
class CLog
{
public:

	~CLog(void);

	enum TYPE_LOG {TYPE_DEBUG,TYPE_NORMAL,TYPE_IMPORTANT,TYPE_ERROR };

	//��¼��־
	static void Log(const char* pszModule,TYPE_LOG type,char* pInfo,...);

	//��¼����Ϊ��ͨ����־
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
	int			m_nLogRecordNum;		//�ڵ�ǰ��־�ļ��ϼ�¼����־��
	char		m_szLogInfoBuf[LOG_MAX_DESCRIPTION_SIZE];
	CSyncCritical*		m_pCritical;

	char		m_szMoudleName[MAX_PATH];

	
};

