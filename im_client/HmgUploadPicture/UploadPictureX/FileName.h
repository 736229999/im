#pragma once

class CFileName  
{
public:
	CFileName(CString szFileName="");
	virtual ~CFileName();

	void		SetFileName(CString szFileName);
	CString		GetFileName();
	CString		GetRoot();
	CString		GetFileTitle();
	CString		GetDescription();
	bool		Exist();

private:
	CString		m_szFileName;
	char		m_szDrive[_MAX_DRIVE];
	char		m_szDir[_MAX_DIR];
	char		m_szFname[_MAX_FNAME];
	char		m_szExt[_MAX_EXT];
};
