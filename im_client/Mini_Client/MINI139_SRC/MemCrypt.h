#pragma once
#include <string>
#include "..\..\include\PublicType.h"

// 内存加解密类
class CMemCrypt
{
private:
	CMemCrypt();
	~CMemCrypt();

public:
	// 加密
	void Encrypt(const char *pSourceData, int nLength, char *pDestData);
	// 解密
	void Decrypt(const char *pSourceData, int nLength, char *pDestData);

public:
	static CMemCrypt g_MemCrypt;

private:
	byte m_szKey[24];	
	byte m_szIv[8];
};

// string加密
void EncryptStr(std::string strSource, char *pDest, int nDestLength);
// string解密
std::string DecryptStr(char *pSource, int nSourceLength);

// uint8加密
uint8 EncryptUint8(uint8 uSource);
// uint8解密
uint8 DecryptUint8(uint8 uSource);

// uint32加密
uint32 EncryptUint32(uint32 uSource);
// uint32解密
uint32 DecryptUint32(uint32 uSource);

// float加密
float EncryptFloat(float uSource);
// uint32解密
float DecryptFloat(float uSource);