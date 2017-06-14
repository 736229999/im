#include "StdAfx.h"
#include ".\httpobject.h"
#include <afxInet.h>

CHttpObject::CHttpObject(void)
{
	m_nFormDataLen  = 0;
	m_nParamCounter = 0;

	m_strValid1     = "";
	m_strValid2     = "";
	m_httpPostType  = hptMultipart_FormData;
}

CHttpObject::~CHttpObject(void)
{
	ClearParam();
}

void CHttpObject::AddFile(CString strParamName, CString strFileName, char *pFileData, long nDataLen)
{
	HttpParamInfo info;
	info.strParamName = strParamName;
	info.strFileName  = ::UTF8_Convert(strFileName,TRUE);
	info.pData        = new char[nDataLen];

	info.strFileName.Trim();
	info.strParamName.Trim();

	if ( info.pData )
	{
		memcpy(info.pData,pFileData,nDataLen);
		info.nDataLen = nDataLen;
	}
	else
	{
		info.nDataLen = 0;
	}

	m_nFormDataLen += (101+nDataLen+info.strParamName.GetLength()+info.strFileName.GetLength());
	m_vecParams.push_back(info);
}

void CHttpObject::AddParam(CString strParamName, float fValue)
{
	CString str;
	str.Format("%f",fValue);
	AddParam(strParamName,str);
}

void CHttpObject::AddParam(CString strParamName, int nValue)
{
	CString str;
	str.Format("%d",nValue);
	AddParam(strParamName,str);
}

void CHttpObject::AddParam(CString strParamName, CString strValue)
{
	HttpParamInfo info;

	strValue.Trim();

	info.strParamName = strParamName;
	info.nDataLen     = strValue.GetLength();
	info.pData        = new char[info.nDataLen+1];

	info.strParamName.Trim();

	if ( info.pData )
	{
		strcpy(info.pData,(LPCSTR)strValue);
	}
	else
	{
		info.nDataLen = 0;
	}

	m_nFormDataLen += (63+info.nDataLen+strParamName.GetLength());

	m_vecParams.push_back(info);

	if ( strParamName!="valid")// && strParamName!="content" )
	{
		if ( m_nParamCounter<3 )
		{
			m_strValid1 += strValue;
		}
		else
		{
			m_strValid2 += strValue;
		}
		
		m_nParamCounter++;
	}

}

void CHttpObject::ClearParam()
{
	for ( int i=0; i<m_vecParams.size(); i++ )
	{
		delete m_vecParams[i].pData;
	}

	m_vecParams.clear();

	m_nFormDataLen  = 0;
	m_nParamCounter = 0;

	m_strValid1 = "";
	m_strValid2 = "";
}

CString CHttpObject::GetValidString()
{
	CString strPlan =  m_strValid1+ "asdfds!@#$1234@!#$ASDFasdfQ@#4345asDF" + m_strValid2;
	unsigned char digest[16];
	MD5_CTX md5Tx;
	md5Tx.MD5Update((unsigned char *)(LPCSTR)strPlan,strPlan.GetLength());
	md5Tx.MD5Final(digest);
	
	return ToHexString(digest,16);
}

CString CHttpObject::GetHeaders()
{
	if ( m_httpPostType==hptUrlEncode )
		return "Content-Type: application/x-www-form-urlencoded";
	else
		return "Content-Type: multipart/form-data; boundary=7d33a816d302b6";
}

long CHttpObject::WriteFormDataItem(char *pData, const HttpParamInfo &info)
{
	CString strTemp("");
	long nWrite=0;

	if ( info.strFileName=="" )
	{
		strTemp += "Content-Disposition: form-data; name=\"";
		strTemp += info.strParamName;
		strTemp += "\"";
		strTemp += "\r\n";
		strTemp += "\r\n";
		strTemp += info.pData;
		strTemp += "\r\n";
		strTemp += "--7d33a816d302b6";
 		strTemp += "\r\n";

		nWrite = strTemp.GetLength();
		memcpy(pData,(LPCSTR)strTemp,nWrite);
	}
	else
	{
		long nTemp = 0;
		char *pCursor = pData;

		strTemp += "Content-Disposition: form-data; name=\"";
		strTemp += info.strParamName;
		strTemp += "\"; filename=\"";
		strTemp += info.strFileName;
		strTemp += "\"";
		strTemp += "\r\n";
		strTemp += "\r\n";

		nTemp = strTemp.GetLength();
		memcpy(pCursor,(LPCSTR)strTemp,nTemp);

		pCursor += nTemp;
		nWrite  += nTemp;

		memcpy(pCursor,info.pData,info.nDataLen);
		pCursor += info.nDataLen;
		nWrite  += info.nDataLen;


		strTemp = "";
		strTemp += "\r\n";
		strTemp += "--7d33a816d302b6";
 		strTemp += "\r\n";

		nTemp = strTemp.GetLength();
		memcpy(pCursor,(LPCSTR)strTemp,nTemp);

		nWrite  += nTemp;
	}

	return nWrite;
}

void CHttpObject::GetFormData(char **ppData, long *pDataLen)
{
	int nParams = m_vecParams.size();

	if ( m_httpPostType==hptUrlEncode )
	{
		CString strTemp("");
		for ( int i=0; i<nParams; i++ )
		{
			strTemp+=m_vecParams[i].strParamName;
			strTemp+="=";
			strTemp+=UrlEncode(m_vecParams[i].pData);
			strTemp+="&";
		}

		strTemp+="Submit=ok";

		*pDataLen = strTemp.GetLength();
		*ppData   = new char[*pDataLen];
		memcpy(*ppData,(LPCSTR)strTemp,*pDataLen);
	}
	else
	{
		*pDataLen = m_nFormDataLen+18;
		*ppData   = new char[*pDataLen];

		if ( *ppData==NULL )
		{
			*pDataLen=0;
		}
		else
		{
			char *pCursor = *ppData;

			strcpy(pCursor,"--7d33a816d302b6\r\n");
			pCursor += 18;

			for ( int i=0; i<nParams; i++ )
			{
				long nWrite = WriteFormDataItem(pCursor,m_vecParams[i]);
				pCursor+=nWrite;
			}
		}
	}
}

CString CHttpObject::SendRequest()
{
	DWORD			  dwProtocol;
	CHttpConnection * pConnection = NULL;
	CHttpFile       * pFile       = NULL;

	CInternetSession session("HmgHttpObject",1,LOCAL_INTERNET_ACCESS,NULL,NULL,0);

	CString			  strServer, strObjectName;
	INTERNET_PORT	  wPort;

	CString strResult;

	if (!AfxParseURL(m_strURL,dwProtocol,strServer,strObjectName,wPort) )
	{		
		m_strError.Format("Error URL: %s",m_strURL );
		goto EXIT;
	}
	try
	{
		pConnection=session.GetHttpConnection(strServer,wPort);	
	}
	catch(CInternetException *pEx)
	{
		char szInfo[256];
		pEx->GetErrorMessage(szInfo,sizeof(szInfo));
		m_strError.Format("Connect to %s Failed:%s", strServer ,szInfo);
		goto EXIT;
	}

	try
	{
		//响应表单的页面文件
		pFile=pConnection->OpenRequest(CHttpConnection::HTTP_VERB_POST,strObjectName);
		Sleep(200);
	}
	catch(CInternetException *pEx)
	{
		char szInfo[256];
		pEx->GetErrorMessage(szInfo,sizeof(szInfo));
		m_strError.Format("Post to %s\\%s Failed:%s", strServer,strObjectName,szInfo );
		goto EXIT;
	}

	try
	{
		long   nSize;
		char * pData;

		GetFormData(&pData,&nSize);
		pFile->SendRequest(GetHeaders(), pData, nSize );

		delete [] pData;

		DWORD dwStatus;
		pFile->QueryInfoStatusCode(dwStatus);

		if ( dwStatus!=200 ) //post sucess 
		{
			m_strError.Format("server error:%d",dwStatus);
			goto EXIT;
		}
		else
		{
			CString strData;
			while(pFile->ReadString(strData))
			{
				strResult+=strData;
			}
		}

	}
	catch(CInternetException *pEx)
	{
		char szInfo[256];
		pEx->GetErrorMessage(szInfo,sizeof(szInfo));
		m_strError.Format("Error occurred, can't find url %s , connect failed.:%s", m_strURL, szInfo);	
		goto EXIT;
	}

EXIT:
	if ( pFile )
	{
		pFile->Close();
		delete pFile;
		pFile = NULL;
	}

	if ( pConnection )
	{
		pConnection->Close();
		delete pConnection;
		pConnection = NULL;
	}

	strResult.Replace("\"utf-8\"","\"gb2312\"");

	return UTF8_Convert(strResult,FALSE);
}