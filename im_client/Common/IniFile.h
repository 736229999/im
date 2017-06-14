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
	~CIniFile(void);

	//�õ������ִ�
	char* GetString(char* pSection,char* pKey,char* pDefault);

	//�õ�����ֵ
	int	 GetInt(char* pSection,char* pKey,int nDefault);

private:
	char		m_szPath[MAX_PATH];
	char		m_szReturnString[MAX_INIFILE_RETURN_STRING];
};
