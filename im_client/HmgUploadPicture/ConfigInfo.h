#pragma once
#include "afx.h"

#include  "UploadPictureX\pubheader.h"

class CConfigInfo :	public CObject
{
	DECLARE_SERIAL(CConfigInfo)
public:
	CConfigInfo(void);
	~CConfigInfo(void);
public:
	_tagUserInfo userinfo;
	_tagUploadFileProp upfileprop;
	int		nLimittedMaxSideSize;
	BOOL	bCalledByShellExt;
	BOOL	bEnableLogInfo;
	BOOL	bAutoDeleteTempFiles;
public:
	CConfigInfo & operator=(const CConfigInfo & _configinfo);
public:
	void Serialize (CArchive& ar);
public:
	BOOL IsConfigOK() const;
	BOOL SaveConfigInfo();
	BOOL LoadConfigInfo();
private:
	CString m_strConfigFilePathName;
};
