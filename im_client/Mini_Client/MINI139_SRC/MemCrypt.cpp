#include "stdafx.h"
#include "MemCrypt.h"
#include "..\..\include\cryptopp\dll.h"
USING_NAMESPACE(CryptoPP)
USING_NAMESPACE(std)

CMemCrypt CMemCrypt::g_MemCrypt;

CMemCrypt::CMemCrypt()
{
	// 生成随机密钥
	for(int i = 0; i < 24; i++)
	{
		m_szKey[i] = rand()%256;
	}
	for(int i = 0; i < 8; ++i)
	{
		m_szIv[i] = rand()%256;
	}
}

CMemCrypt::~CMemCrypt()
{
}

// 加密
void CMemCrypt::Encrypt(const char *pSourceData, int nLength, char *pDestData)
{
	CFB_FIPS_Mode<DES_EDE3>::Encryption encryption_DES_EDE3_CFB;
	encryption_DES_EDE3_CFB.SetKeyWithIV(m_szKey, sizeof(m_szKey), m_szIv);
	byte *pOut = new byte[nLength];
	encryption_DES_EDE3_CFB.ProcessString(pOut, (const byte*)pSourceData, nLength);
	memcpy(pDestData, pOut, nLength);
	delete[] pOut;
	pOut = NULL;
}

// 解密
void CMemCrypt::Decrypt(const char *pSourceData, int nLength, char *pDestData)
{
	CFB_FIPS_Mode<DES_EDE3>::Decryption decryption_DES_EDE3_CFB;
	decryption_DES_EDE3_CFB.SetKeyWithIV(m_szKey, sizeof(m_szKey), m_szIv);
	byte *pOut = new byte[nLength];
	decryption_DES_EDE3_CFB.ProcessString(pOut, (const byte*)pSourceData, nLength);
	memcpy(pDestData, pOut, nLength);
	delete[] pOut;
	pOut = NULL;
}

// string加密
void EncryptStr(string strSource, char *pDest, int nDestLength)
{
	memset(pDest, 0, nDestLength);
	strcpy(pDest, strSource.c_str());
	CMemCrypt::g_MemCrypt.Encrypt(pDest, nDestLength, pDest);
}

// string解密
string DecryptStr(char *pSource, int nSourceLength)
{
	char *pDest = new char[nSourceLength];
	memset(pDest, 0, nSourceLength);
	CMemCrypt::g_MemCrypt.Decrypt(pSource, nSourceLength, pDest);
	string strOut = pDest;
	delete[] pDest;
	pDest = NULL;
	return strOut;
}

// uint8加密
uint8 EncryptUint8(uint8 uSource)
{
	uint8 uDest = 0;
	CMemCrypt::g_MemCrypt.Encrypt((const char*)&uSource, sizeof(uint8), (char*)&uDest);
	return uDest;
}

// uint8解密
uint8 DecryptUint8(uint8 uSource)
{
	uint8 uDest = 0;
	CMemCrypt::g_MemCrypt.Decrypt((const char*)&uSource, sizeof(uint8), (char*)&uDest);
	return uDest;
}

// uint32加密
uint32 EncryptUint32(uint32 uSource)
{
	uint32 uDest = 0;
	CMemCrypt::g_MemCrypt.Encrypt((const char*)&uSource, sizeof(uint32), (char*)&uDest);
	return uDest;
}

// uint32解密
uint32 DecryptUint32(uint32 uSource)
{
	uint32 uDest = 0;
	CMemCrypt::g_MemCrypt.Decrypt((const char*)&uSource, sizeof(uint32), (char*)&uDest);
	return uDest;
}

// float加密
float EncryptFloat(float uSource)
{
	float uDest = 0;
	CMemCrypt::g_MemCrypt.Encrypt((const char*)&uSource, sizeof(float), (char*)&uDest);
	return uDest;
}

// uint32解密
float DecryptFloat(float uSource)
{
	float uDest = 0;
	CMemCrypt::g_MemCrypt.Decrypt((const char*)&uSource, sizeof(float), (char*)&uDest);
	return uDest;
}