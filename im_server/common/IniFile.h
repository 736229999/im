#pragma once

/************************************************************************ 
文件描述：读取INI文件的类的声明文件 

创建人：hechang
时间：	2006-7-5
************************************************************************/

#define MAX_INIFILE_RETURN_STRING 1024

class CIniFile
{
public:
	CIniFile(void);
	CIniFile(const char *strFileName);
	~CIniFile(void);

	//得到配置字串
	char* GetString(const char* pSection,const char* pKey,const char* pDefault);

	//得到配置值
	int	 GetInt(const char* pSection,const char* pKey,int nDefault);

	//设置配置值

	int SetInt(const char* pSection,const char* pKey,int nDefault);

private:
	char		m_szPath[MAX_PATH];
	char		m_szReturnString[MAX_INIFILE_RETURN_STRING];
};
