#ifndef __FUNCTION_H__
#define __FUNCTION_H__


DWORD    GetLocalIpAddress(); //��ȡ����IP��ַ


bool	 CheckEmail(CString strEmail, CString &strError);//email��ַ���

void	 ExtractFilePath(CString strFilePathName, CString &strPath, CString &strName, CString &strExt);


bool operator != (const CRect& lhs, const CRect& rhs);


#endif


