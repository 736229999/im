#pragma once

#include <vector>
using namespace std;

enum HttpPostType
{
	hptUrlEncode,
	hptMultipart_FormData
};

struct HttpParamInfo
{
	CString strParamName;
	CString strFileName;
	char *  pData;
	long    nDataLen;

	HttpParamInfo()
	{
		pData       = NULL;
		nDataLen    = 0;
		strFileName = "";;
	}
};

class CHttpObject
{
public:
	CHttpObject(void);
	virtual ~CHttpObject(void);

public:
	void SetUrl(CString strUrl) { m_strURL = strUrl; }
	void SetHttpPostType(HttpPostType type) { m_httpPostType = type; }
	void AddParam(CString strParamName, CString strValue);
	void AddParam(CString strParamName, int nValue);
	void AddParam(CString strParamName, float fValue);
	void AddFile(CString strParamName, CString strFileName, char *pData, long nDataLen);

	CString GetValidString();

	void ClearParam();
	
	CString GetLastError() { return m_strError;	}
	CString SendRequest();

protected:
	CString GetHeaders();
	void  GetFormData(char **ppData, long *pDataLen);
	long  WriteFormDataItem(char *pData,const HttpParamInfo &info);

protected:
	CString			m_strURL;
	CString         m_strError;
	CString			m_strValid1, m_strValid2;
	int				m_nParamCounter;
	long            m_nFormDataLen;
	vector<HttpParamInfo> m_vecParams;
	HttpPostType	m_httpPostType;
};
