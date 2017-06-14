#include "StdAfx.h"
#include ".\inifile.h"

CIniFile::CIniFile(void)
{

	::GetModuleFileName(NULL,m_szPath,sizeof(m_szPath));
	strcpy(strrchr(m_szPath,'\\'),"\\Config\\Config.ini");

}

CIniFile::~CIniFile(void)
{
}


char* CIniFile::GetString(char* pSection,char* pKey,char* pDefault)
{
	::GetPrivateProfileString(pSection,pKey,pDefault,
								m_szReturnString,
								sizeof(m_szReturnString),
								m_szPath);

	return m_szReturnString;
}

int	 CIniFile::GetInt(char* pSection,char* pKey,int nDefault)
{

	return ::GetPrivateProfileInt( pSection ,pKey,nDefault,m_szPath);
}