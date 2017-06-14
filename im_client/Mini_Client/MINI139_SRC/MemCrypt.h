#pragma once
#include <string>
#include "..\..\include\PublicType.h"

// �ڴ�ӽ�����
class CMemCrypt
{
private:
	CMemCrypt();
	~CMemCrypt();

public:
	// ����
	void Encrypt(const char *pSourceData, int nLength, char *pDestData);
	// ����
	void Decrypt(const char *pSourceData, int nLength, char *pDestData);

public:
	static CMemCrypt g_MemCrypt;

private:
	byte m_szKey[24];	
	byte m_szIv[8];
};

// string����
void EncryptStr(std::string strSource, char *pDest, int nDestLength);
// string����
std::string DecryptStr(char *pSource, int nSourceLength);

// uint8����
uint8 EncryptUint8(uint8 uSource);
// uint8����
uint8 DecryptUint8(uint8 uSource);

// uint32����
uint32 EncryptUint32(uint32 uSource);
// uint32����
uint32 DecryptUint32(uint32 uSource);

// float����
float EncryptFloat(float uSource);
// uint32����
float DecryptFloat(float uSource);