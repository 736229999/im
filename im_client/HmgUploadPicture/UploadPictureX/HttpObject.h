#pragma once

#include <vector>
using namespace std;

class MD5_CTX {
public:
	MD5_CTX();
	virtual ~MD5_CTX();
	void MD5Update ( unsigned char *input, unsigned int inputLen);
	void MD5Final (unsigned char digest[16]);

private:
	unsigned long int state[4];					/* state (ABCD) */
	unsigned long int count[2];					/* number of bits, modulo 2^64 (lsb first) */
	unsigned char buffer[64];       /* input buffer */
	unsigned char PADDING[64];		/* What? */

private:
	void MD5Init ();
	void MD5Transform (unsigned long int state[4], unsigned char block[64]);
	void MD5_memcpy (unsigned char* output, unsigned char* input,unsigned int len);
	void Encode (unsigned char *output, unsigned long int *input,unsigned int len);
	void Decode (unsigned long int *output, unsigned char *input, unsigned int len);
	void MD5_memset (unsigned char* output,int value,unsigned int len);
};

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
