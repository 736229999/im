#include "stdafx.h"
#include "IniFile.h"

CIniFile::CIniFile(void)
{

	::GetModuleFileName(NULL,m_szPath,sizeof(m_szPath));
	strcpy_s(strrchr(m_szPath,'/'),MAX_PATH,"/../config/Config.ini");
	cout << "path " << m_szPath  << endl;

}

CIniFile::CIniFile(const char *strFileName)
{
	::GetModuleFileName(NULL,m_szPath,sizeof(m_szPath));
	strcpy_s(strrchr(m_szPath,'/'),MAX_PATH,"/../config/");
	strcat_s(m_szPath, strFileName);
}

CIniFile::~CIniFile(void)
{
}


char* CIniFile::GetString(const char* pSection,const char* pKey,const char* pDefault)
{
	::GetPrivateProfileString(pSection,pKey,pDefault,
								m_szReturnString,
								sizeof(m_szReturnString),
								m_szPath);

	return m_szReturnString;
}

int	 CIniFile::GetInt(const char* pSection,const char* pKey,int nDefault)
{

	return ::GetPrivateProfileInt( pSection ,pKey,nDefault,m_szPath);
}

int CIniFile::SetInt(const char* pSection,const char* pKey,int nDefault)
{
	char id[10] = {0};
//	_itoa_s(nDefault, id, 10);
	sprintf(id,"%d",nDefault);
	return ::WritePrivateProfileString(pSection, pKey,id, m_szPath);
}

