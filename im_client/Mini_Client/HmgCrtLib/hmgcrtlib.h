#ifndef _HMG_CRT_LIB_H_
#define _HMG_CRT_LIB_H_


#ifdef _HMG_CRT_LIB_
#define DLL_DECLSPEC  __declspec(dllexport)
#else
#define DLL_DECLSPEC  __declspec(dllimport)
#endif

//#ifdef __cplusplus
//extern "C" {
//#endif
	
//输出全局变量及类＼结构＼函数
DLL_DECLSPEC CString Base64Encode( CString strInput);
DLL_DECLSPEC CBitmap * CBitmapFromFile(CString strImageFile,int nWidth, int nHeight);
DLL_DECLSPEC CBitmap * CBitmapFromStream(const char *pData, int dataLen, int nWidth, int nHeight);
DLL_DECLSPEC int ComboGetViewProp(CComboBox *pCombo);
DLL_DECLSPEC void ComboInitViewProp(CComboBox *pCombo);
DLL_DECLSPEC void ComboSelViewProp(CComboBox *pCombo,int iViewProp);
DLL_DECLSPEC void ComboInitFamilyClass(CComboBox *pComboBox );
DLL_DECLSPEC void ComboSelFamilyClass(CComboBox *pComboBox, int iClass);
DLL_DECLSPEC DWORD ComboGetFamilyClass(CComboBox *pComboBox);
DLL_DECLSPEC int CountSubStr(CString str,CString substr);
DLL_DECLSPEC DWORD DateStr2Long(CString strDate);
DLL_DECLSPEC bool DeleteDirectory(CString strDirName);
DLL_DECLSPEC bool DownLoadWebFile(CString strUrl, CString strLocalFile);
DLL_DECLSPEC void DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, COLORREF clrTrans );
DLL_DECLSPEC bool FileAccess(CString strFile);
DLL_DECLSPEC bool FileExist(CString strFile);
DLL_DECLSPEC CString GetFileNameFromPath(CString strFullPathName);
DLL_DECLSPEC DWORD GetFileSize(CString strFile);
DLL_DECLSPEC CString GetModulePath();
DLL_DECLSPEC void GetHtmlImageList(const CString &strHtml, CStringList &slImages);
DLL_DECLSPEC void GetSectionValue(const CString &strInfo, CString str1,CString str2, CString &strRet);
DLL_DECLSPEC CString GetWebFileName(CString strUrl);
DLL_DECLSPEC CString GetWebServerName(CString strUrl);
DLL_DECLSPEC void HorizStretchBlt(CDC *pDC,int x, int y, int cx, CBitmap *pBitmap);
DLL_DECLSPEC CString IntToStr(long n);
DLL_DECLSPEC CString MakeIMAuthString(CString strUserName);
DLL_DECLSPEC CString MakeMD5Password(char* pPassword);
DLL_DECLSPEC CString MakePassportUrl(CString	strUrl,
									 long		nUserID,
									 CString	strUserName,
									 CString    strNickName,
									 int		nSex,
									 CString	strEmail,
									 CString	strFaceUrl,
									 int		nGoldMoney,
									 int		nMessageCount
						);
DLL_DECLSPEC void OpenHtmlPage(CString strUrl);
DLL_DECLSPEC void SplitFilePath(CString strFullPathName, CString &strPath, CString &strName, CString &strExt);
DLL_DECLSPEC BSTR StrToWideStr(CString str);
DLL_DECLSPEC CString ToHexString(BYTE bytes[] ,int len) ;
DLL_DECLSPEC CString ConvertSpecialCharToHtml(CString strText);
DLL_DECLSPEC CString TrimHtmlCode(CString strHtml);
DLL_DECLSPEC CString UIntToStr(unsigned long n);
DLL_DECLSPEC CString UrlEncode(CString strIn);
DLL_DECLSPEC CString UrlGetStr(LPCSTR lpszUrl);
DLL_DECLSPEC CString UTF8_Convert(CString  str, BOOL bEncode );
DLL_DECLSPEC CString WideStrToStr(BSTR str);
DLL_DECLSPEC bool TEAEncrypt(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len );
DLL_DECLSPEC bool TEADecrypt(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len );

/* MD5 Class. */
class AFX_EXT_CLASS MD5_CTX {
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



//#ifdef __cplusplus
//}
//#endif

#endif