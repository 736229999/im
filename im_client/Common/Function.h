#ifndef __FUNCTION_H__
#define __FUNCTION_H__

#ifndef __OUTPUT_DEBUG_STRING__
#define __OUTPUT_DEBUG_STRING__
#endif
#include <string>
using namespace std;
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
string ToHexString(BYTE bytes[],int len);
BYTE MyRand(BYTE bMin);

BOOL xTEAEncryptWithKey(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len );
BOOL xTEADecryptWithKey(const char *crypt, long crypt_len, char key[16], char *plain, long * plain_len);
//BOOL xTEADecryptWithKey(const unsigned long *crypt, DWORD dwCryptLen, const unsigned long *theKey, DWORD dwKeyLen,unsigned long *plain,DWORD* pdwPlainLen);
//BOOL xTEAEncryptWithKey(const unsigned long *plain, DWORD dwPlainLen, const unsigned long *theKey, DWORD dwKeyLen,unsigned long *crypt,DWORD* pdwCryptLen);

#endif


