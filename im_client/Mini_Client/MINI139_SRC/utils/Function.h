#ifndef __FUNCTION_H__
#define __FUNCTION_H__


DWORD    GetLocalIpAddress(); //获取本机IP地址


bool	 CheckEmail(CString strEmail, CString &strError);//email地址检测

void	 ExtractFilePath(CString strFilePathName, CString &strPath, CString &strName, CString &strExt);


bool operator != (const CRect& lhs, const CRect& rhs);


#endif


