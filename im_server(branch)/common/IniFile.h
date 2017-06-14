#pragma once

/************************************************************************ 
�ļ���������ȡINI�ļ�����������ļ� 

�����ˣ�hechang
ʱ�䣺	2006-7-5
************************************************************************/

#define MAX_INIFILE_RETURN_STRING 1024

class CIniFile
{
public:
	CIniFile(void);
	CIniFile(const char *strFileName);
	~CIniFile(void);

	//�õ������ִ�
	char* GetString(const char* pSection,const char* pKey,const char* pDefault);

	//�õ�����ֵ
	int	 GetInt(const char* pSection,const char* pKey,int nDefault);

	//��������ֵ

	int SetInt(const char* pSection,const char* pKey,int nDefault);

private:
	char		m_szPath[MAX_PATH];
	char		m_szReturnString[MAX_INIFILE_RETURN_STRING];
};
