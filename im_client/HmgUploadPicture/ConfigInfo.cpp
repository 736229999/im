
#include "stdafx.h"
#include ".\configinfo.h"

IMPLEMENT_SERIAL (CConfigInfo, CObject, 1)

CConfigInfo::CConfigInfo(void)
{
	
	CString strSystemDir;
	GetSystemDirectory(strSystemDir.GetBufferSetLength(MAX_PATH), MAX_PATH);

	m_strConfigFilePathName.Format("%s\\mini139upload.config\0", strSystemDir);

	nLimittedMaxSideSize = 0;

	userinfo.bIsMiniClinet = FALSE;
	userinfo.bNeedvalidate = FALSE;
	userinfo.nControlVersion = 0;
	userinfo.nUserId = 0;
	
	upfileprop.iAlbumDirId = 0;
	upfileprop.iViewProp = 0;

	bCalledByShellExt = FALSE;
	bAutoDeleteTempFiles = FALSE;
}

CConfigInfo::~CConfigInfo(void)
{
}

CConfigInfo & CConfigInfo::operator=(const CConfigInfo & _configinfo)
{
	if(this != &_configinfo)
	{
		nLimittedMaxSideSize = _configinfo.nLimittedMaxSideSize;

		userinfo.bIsMiniClinet = _configinfo.userinfo.bIsMiniClinet;
		userinfo.bNeedvalidate = _configinfo.userinfo.bNeedvalidate;

		userinfo.nControlVersion = _configinfo.userinfo.nControlVersion;
		userinfo.nUserId		=  _configinfo.userinfo.nUserId;
		userinfo.strAuthKey		=  _configinfo.userinfo.strAuthKey;
		
		userinfo.strAuthUrl		=  _configinfo.userinfo.strAuthUrl;
		userinfo.strNickName	=  _configinfo.userinfo.strNickName;

		userinfo.strPassword	=  _configinfo.userinfo.strPassword;
		userinfo.strPicUrl		=  _configinfo.userinfo.strPicUrl;
		userinfo.strUserName	=  _configinfo.userinfo.strUserName;
		
        upfileprop.iAlbumDirId	= _configinfo.upfileprop.iAlbumDirId;
		upfileprop.iViewProp	= _configinfo.upfileprop.iViewProp;
		upfileprop.strAlbumName	= _configinfo.upfileprop.strAlbumName;
		upfileprop.strDescription	= _configinfo.upfileprop.strDescription;
		upfileprop.strTag			= _configinfo.upfileprop.strTag;

		bCalledByShellExt			= _configinfo.bCalledByShellExt;
		bAutoDeleteTempFiles		= _configinfo.bAutoDeleteTempFiles;
		bEnableLogInfo				= _configinfo.bEnableLogInfo;
	}
	
	return *this;
}

void CConfigInfo::Serialize (CArchive& ar)
{
//	CObject::Serialize (ar);
	
	try
	{
		if (ar.IsStoring ())
		{
			ar << nLimittedMaxSideSize
				<<	userinfo.bIsMiniClinet 
				<<	userinfo.bNeedvalidate
				<<	userinfo.nControlVersion
				<< userinfo.nUserId		
				<< userinfo.strAuthKey		
				<< userinfo.strAuthUrl		
				<< userinfo.strNickName	
				<< userinfo.strPassword	
				<< userinfo.strPicUrl		
				<< userinfo.strUserName;

			ar << upfileprop.iAlbumDirId	
				<<  upfileprop.iViewProp	
				<< upfileprop.strAlbumName	
				<< upfileprop.strDescription
				<< upfileprop.strTag;	


		}
		else // Loading, not storing
		{
			ar >> nLimittedMaxSideSize >>
				userinfo.bIsMiniClinet >>
				userinfo.bNeedvalidate >>
				userinfo.nControlVersion >>
				userinfo.nUserId		 >>
				userinfo.strAuthKey		 >>
				userinfo.strAuthUrl		>>
				userinfo.strNickName	>>	
				userinfo.strPassword	>>
				userinfo.strPicUrl		>>
				userinfo.strUserName;

			ar >> upfileprop.iAlbumDirId	
				>> upfileprop.iViewProp	
				>> upfileprop.strAlbumName	
				>> upfileprop.strDescription
				>> upfileprop.strTag;	
		}
	}
	catch ( ... )
	{
	}
}

BOOL CConfigInfo::IsConfigOK() const
{
	return	userinfo.strUserName.GetLength() > 0 ? TRUE : FALSE;
}

BOOL CConfigInfo::SaveConfigInfo()
{
//	strConfigPath =	AfxGetApp() ->GetProfileString("shellext", "configpath", "");
	
//	if(strConfigPath.GetLength() < 1)
	

	CFile file;
	CFileException ex;
	if(!file.Open(m_strConfigFilePathName, CFile::modeCreate | CFile::modeWrite, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);

//		write_log_error("读取配置文件出错. %s", szError);
	//	MessageBox(strParam, "图片上传", MB_ICONERROR);		

		return FALSE;
	}

	CArchive ar(&file, CArchive::store);
	
	Serialize(ar);

	return TRUE;
}

#include <io.h>
BOOL CConfigInfo::LoadConfigInfo()
{
	if(_access(m_strConfigFilePathName, 06) == -1)	return FALSE;

	CFile file;
	CFileException ex;
	if(!file.Open(m_strConfigFilePathName, CFile::modeRead, &ex))
	{
		TCHAR szError[1024];
		ex.GetErrorMessage(szError, 1024);

		return FALSE;
	}

	{
		CArchive ar(&file, CArchive::load);			Serialize(ar);
	}
	
	bEnableLogInfo = FALSE;

	int nRet = AfxGetApp() ->GetProfileInt(_T("1.0"), _T("EnableLogInfo"), 0);
	bEnableLogInfo = nRet > 0 ? TRUE : FALSE;
    g_bEnableLogInfo = bEnableLogInfo;

	return TRUE;
}