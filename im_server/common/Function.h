#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#ifndef __OUTPUT_DEBUG_STRING__
#define __OUTPUT_DEBUG_STRING__
#endif
#include <string>
#include <vector>
#include <sys/time.h>
#include <time.h>

using namespace std;
#define LOG_MAX_UTF8_DESCRIPTION_SIZE 1536
//#ifdef __OUTPUT_DEBUG_STRING__
//#undef __OUTPUT_DEBUG_STRING__
//#endif
/*
CString BCDByteArray2HexString(BYTE data[],int len,CString& str);
CString ByteToString(BYTE * chBuf,int len);
CString IntToStringUser(int iInt);
int StringToInt(CString str);
int HexStr2BYTEBCDArray(const char *chBuf, int len, BYTE *chBYTE);

void LogDebugStringU(CString str);
CString GetExePath();


BOOL CheckRecvPNData(BYTE* pBuf);
int CheckCalledPN(CString sCalledPN, CString sLocalPN);

*/
std::string Base64Encode(const std::string& strInput);
std::string Base64Encode(const char* szInput, int inByte);

string Base64Decode(const string& strData/*, int& OutByte*/);
string Base64Decode(const char* Data,int DataByte/*, int& OutByte*/);

string ToHexString(BYTE bytes[],int len);
BYTE MyRand(BYTE bMin);

BOOL xTEAEncryptWithKey(const char *plain, int plain_len, char key[16], char *crypt, int * crypt_len );
BOOL xTEADecryptWithKey(const char *crypt, int crypt_len, char key[16], char *plain, int * plain_len);
//BOOL xTEADecryptWithKey(const unsigned int *crypt, DWORD dwCryptLen, const unsigned int *theKey, DWORD dwKeyLen,unsigned int *plain,DWORD* pdwPlainLen);
//BOOL xTEAEncryptWithKey(const unsigned int *plain, DWORD dwPlainLen, const unsigned int *theKey, DWORD dwKeyLen,unsigned int *crypt,DWORD* pdwCryptLen);

void getrandomstring(char * str);
bool  CheckEmail(const std::string& strEmail);
int gbk2utf8(char *utfstr,const char *srcstr);

string replace( string &original, const string &subset, const string &section );
string urlencode(string &str_source);
int utf2gbk(char *inbuf,int inlen,char *outbuf,int outlen);

// ×Ö·û´®AES¼ÓÃÜ
int AesEncode(const char *key, int keylen, const char *inbuf,int inlen, char *outbuf, int &outlen);
int utf2GB18030(char *inbuf,int inlen, char *outbuf, int outlen);

char *random_uuid( char buf[37] );

void split(std::string str, std::string separator, std::vector<std::string>* ret);


#endif


