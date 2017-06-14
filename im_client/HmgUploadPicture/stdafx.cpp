// stdafx.cpp : 只包括标准包含文件的源文件
// HmgUploadPicture.pch 将作为预编译头
// stdafx.obj 将包含预编译类型信息
#include "stdafx.h"
#include <io.h>

BOOL		g_bEnableLogInfo=FALSE;
CConfigInfo g_AppConfigInfo;
BOOL        g_bEnableContinueOperation = TRUE;


/////////////////////////////////////////////////////////////////////////////////////////
//  WideStrToStr
//  功能：宽字符串转换为Ansi字符串
//  输入：
//  返回值：CString
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
CString WideStrToStr(BSTR str)
{
	CComBSTR b=str;
	CString  r(b);
	return r;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  StrToWideStr
//  功能：Ansi字符转换为串宽字符串
//  输入：
//  返回值：BSTR,注意要调用SysFree
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
BSTR  StrToWideStr(CString str)
{
	CComBSTR b=str;
	BSTR     r=L"";
	b.CopyTo(&r);
	return r;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  ToHexString
//  功能：将一个内存序列用16进制字串显示
//  输入：bytes,内存序列首地址
//        len, 内存序列长度
//  返回值：内存序列16进制字串
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
CString ToHexString(BYTE bytes[] ,int len) 
{
	static char hexDigits[] = {   '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	CString strResult ="";

	for (int i = 0; i < len; i++) 
	{
		int b = bytes[i];
		strResult += hexDigits[b >> 4];
		strResult += hexDigits[b & 0xF];
	}

	return strResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  GetFileNameFromPath
//  功能：获取一个文件全路径字符串中的文件名
//  输入：strFullPathName, 文件全路径
//  输出：
//  返回值：文件名
//  修改记录：
/////////////////////////////////////////////////////////////////////////////////////////
CString GetFileNameFromPath(CString strFullPathName)
{
	return strFullPathName.Right(strFullPathName.GetLength() - strFullPathName.ReverseFind('\\') - 1);
}


void GetSectionValue(const CString &strInfo, CString strSec1,CString strSec2, CString &strRet)
{
	int		iPos1 , iPos2, iLen;

	CString strTemp = strInfo;

	strTemp.MakeLower();
	strSec1.MakeLower();
	strSec2.MakeLower();

	iPos1 = strTemp.Find(strSec1);
	iPos2 = strTemp.Find(strSec2);

	iLen  = strSec1.GetLength();

	if ( iPos2>iPos1 && iPos1>-1 )
	{
		strRet = strInfo.Mid(iPos1+iLen,iPos2-iPos1-iLen);
	}
	else
	{
		strRet = "";
	}
}


CString  UTF8_Convert(CString  str, BOOL bEncode )  
{  
	int  len=str.GetLength();  
	int sourceCodepage, targetCodepage;
	if ( bEncode )
	{
		sourceCodepage = 936;
		targetCodepage = CP_UTF8;
	}
	else
	{
		sourceCodepage = CP_UTF8;
		targetCodepage = 936;
	}

	int  unicodeLen=MultiByteToWideChar(sourceCodepage,0,str,-1,NULL,0);  

	wchar_t            *  pUnicode;  
	pUnicode=new  wchar_t[unicodeLen+1];  

	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  

	MultiByteToWideChar(sourceCodepage,0,str,-1,(LPWSTR)pUnicode,unicodeLen);  

	int  targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char  *)NULL,0,NULL,NULL);  

	BYTE  *  pTargetData =new  BYTE[targetLen+1];  
	memset(pTargetData,0,targetLen+1);  

	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char  *)pTargetData,targetLen,NULL,NULL);  

	CString  rt;  
	rt.Format("%s",pTargetData);  

	delete  pUnicode;  
	delete  pTargetData;  

	return  rt;   
}  


CString IntToStr(long n)
{
	CString s;
	s.Format("%d",n);
	return s;
}

int BOOL2Int(const BOOL b)
{
	return b ? 1 : 0;
}

CString UIntToStr(unsigned long n)
{
	CString s;
	s.Format("%u",n);
	return s;
}

void  OpenHtmlPage(CString strUrl)
{
	CString strExplorePath ;

	HKEY hKey;
	LONG ret ;
	LPCTSTR data_Set = "HTTP\\shell\\open\\command";
	::RegOpenKeyEx(HKEY_CLASSES_ROOT,data_Set,0,KEY_ALL_ACCESS,&hKey);

	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	char  szValue[255];

	ret = ::RegQueryValueEx(hKey,"",NULL,&dwType,(LPBYTE)szValue,&dwSize);

	bool bAutoStart = false;

	if ( ret==ERROR_SUCCESS )
	{
		strExplorePath =szValue;
		strExplorePath.Trim();
		strExplorePath.MakeLower();

		if ( strExplorePath=="")
		{
			ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
		}
		else
		{		
			int iPos=strExplorePath.Find("\"",0);

			if ( iPos==0 )
			{
				iPos = strExplorePath.Find("\"",1);
				strExplorePath=strExplorePath.Mid(1,iPos-1);
			}
			else
			{
				iPos = strExplorePath.Find(" ",0 );
				strExplorePath.Mid(0,iPos);
			}

			CString strPath,strName,strExt;

			ExtractFilePath(strExplorePath,strPath,strName,strExt);

			if ( _access(strExplorePath,0)!=-1 )
				ShellExecute(NULL,"open",strName+strExt,strUrl,strPath,SW_SHOW);
			else
				ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
		}
	}
	else
	{
		ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
	}

	::RegCloseKey(hKey);

}

void ExtractFilePath(CString strFilePathName, CString &strPath, CString &strName, CString &strExt)
{
	CString strFileName;
	int l = strFilePathName.GetLength();

	while( l>-1 )
	{
		if ( strFilePathName[l]!='\\' && strFilePathName[l]!='/')
		{
			l--;
			continue;
		}

		break;
	}

	strFileName = strFilePathName.Right(strFilePathName.GetLength()-l-1);
	strPath = strFilePathName.Left(l+1);

	l = strFileName.GetLength();

	while( l>-1 )
	{
		if ( strFileName[l]!='.' )
		{
			l--;
			continue;
		}

		break;
	}

	strName = strFileName.Left(l);
	strExt = strFileName.Right(strFileName.GetLength()-l);
}


CString _get_tmp_directroy()
{
	TCHAR szBuf[MAX_PATH] = {'\0'};
	GetTempPath(MAX_PATH, szBuf);

	CString strDirName;
	strDirName.Format("%s\\min139_uploadpic", szBuf);
	CreateDirectory(strDirName, NULL);

	return strDirName;
}

// 获取文件大小
ULONG GetFileSize(const CString & filepath)
{
	CFile pFile;

	try
	{
		pFile.Open(filepath,CFile::modeRead);
		ULONG ulSize = ULONG(pFile.GetLength());
		pFile.Close();
		return ulSize;
	}
	catch(...)
	{
		return 0;
	}
}

void _delete_all_tmp_files()
{
	CString strName, strFilePathName;
	strName.Format("%s\\*.*\0", _get_tmp_directroy());

	CFileFind finder;
	BOOL bWorking = finder.FindFile(strName);
	while (bWorking)
	{
		bWorking = finder.FindNextFile();
		if( finder.IsDots() || finder.IsDirectory() )
		{
			continue;
		}
		else
		{
			strFilePathName =  finder.GetFilePath();
			::DeleteFile(strFilePathName);
		}
	} // end while (bWorking)

	finder.Close();
	::RemoveDirectory(_get_tmp_directroy());
}

double Round(double d, int iPrecision)
{
	double	dResult;

	const double n = 10.0f;
	double	dTmp = pow(n, iPrecision);
	dResult = floor((d + 0.5/dTmp) * dTmp) / dTmp;

	return (dResult);
}

#ifdef _ENABLE_LOGINFO_
#import "LogiNFO.dll" no_namespace
#endif // _ENABLE_LOGINFO_

void write_log_info(const char *lpszfunc, const char *format, ...)
{
	try
	{
#ifdef _ENABLE_LOGINFO_
		char szbuffer[4096 + 1] = {'\0'};

		va_list valist;
		va_start (valist, format);

		if (_vsnprintf ( (char *) szbuffer, 4096, format, valist) == -1)
		{
			return;
		}

		va_end (valist);

		CString strMsgINFO;
		strMsgINFO.Format("%s %s\0", lpszfunc, szbuffer);
		
		CString strUnit;
		ILogiNFOPtr LogiNFOPtr = NULL;

		strUnit.Format("uploadpic %s [%d]\0", APP_VERSION, GetCurrentThreadId());
		HRESULT hr  =  LogiNFOPtr.CreateInstance(__uuidof(LogiNFO));
		if(SUCCEEDED(hr))
		{	
			hr = LogiNFOPtr ->LogiNFO2(strUnit.AllocSysString(), strMsgINFO.AllocSysString());
		}
#endif // _ENABLE_LOGINFO_
	}
	catch ( ... )
	{
	//	AfxGetMainWnd() ->MessageBox(lpszfunc, "write_log_info", MB_ICONERROR);
	}
}


void write_log_error(const char *lpszfunc, const char *format, ...)
{
	try
	{
#ifdef _ENABLE_LOGINFO_
		char szbuffer[4096 + 1] = {'\0'};

		va_list valist;
		va_start (valist, format);

		if (_vsnprintf ( (char *) szbuffer, 4096, format, valist) == -1)
		{
			return;
		}

		va_end (valist);

		CString strMsgINFO;
		strMsgINFO.Format("%s ** ERROR **  %s\0", lpszfunc, szbuffer);

		CString strUnit;
		ILogiNFOPtr LogiNFOPtr = NULL;

		strUnit.Format("uploadpic %s [%d]\0",APP_VERSION, GetCurrentThreadId());
		HRESULT hr  =  LogiNFOPtr.CreateInstance(__uuidof(LogiNFO));
		if(SUCCEEDED(hr))
		{	
			hr = LogiNFOPtr ->LogiNFO2(strUnit.AllocSysString(), strMsgINFO.AllocSysString());
		}
#endif // _ENABLE_LOGINFO_
	}
	catch ( ... )
	{
		//	AfxGetMainWnd() ->MessageBox(lpszfunc, "write_log_info", MB_ICONERROR);
	}
}