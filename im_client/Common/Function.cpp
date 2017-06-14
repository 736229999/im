
#include "StdAfx.h"
#include "Function.h"
#include "time.h"
typedef unsigned char byte;
/*
CString BCDByteArray2HexString(BYTE data[],int len,CString& str)
{
	CString strTmp="";
	str.Empty();
	for(int i=0;i<len;i++)
	{
		if(data[i] <= 9)
			strTmp.Format("0%x",data[i]);
		else
			strTmp.Format("%x",data[i]);
		str+=strTmp;
	}
	return str;
}

CString ByteToString(BYTE * chBuf,int len)
{
	CString str = CString("");
	CString str1 = CString(" ");
	int i=0;
	for(i=0;i<len;i++)
	{
		if((i>0) && (i%2 == 0))
			str+=CString(" ");

		if(chBuf[i] <= 15)
			str1.Format("0%x",chBuf[i]);
		else
			str1.Format("%x",chBuf[i]);
		str+=str1;
	}
	return str;
}

CString IntToStringUser(int iInt)
{
	CString str = _T("");
	str.Format("%d", iInt);
	return str;
}

int ByteToString(CString str)
{
	char chChar[100];
	strcpy(chChar, (LPCTSTR)str); 
	return	atoi(chChar);
}

int HexStr2BYTEBCDArray(const char *chBuf, int len, BYTE *chBYTE)
{
	int i;

	char  chtmp;
	BYTE itmp;

	//check PN length
	if(len != (int)strlen(chBuf))
	{
		return -1;	
	}

	//Transter Hex string(chBuf) to BCD BYTE Array(chBYTE)
	for(i=0 ;i<len;i++)
	{
		chtmp = chBuf[i];
		switch(chtmp)
		{
		case '0':
			itmp = 0;
			break;
		case '1':
			itmp = 1;
			break;
		case '2':
			itmp = 2;
			break;
		case '3':
			itmp = 3;
			break;
		case '4':
			itmp = 4;
			break;
		case '5':
			itmp = 5;
			break;
		case '6':
			itmp = 6;
			break;
		case '7':
			itmp = 7;
			break;
		case '8':
			itmp = 8;
			break;
		case '9':
			itmp = 9;
			break;
		default:
			return -2;
			break;
		}
		if(i%2 == 0)
			chBYTE[i/2] = (itmp&0x0f)<<4;
		else
			chBYTE[i/2] += (itmp&0x0f);
	}

	chBYTE[i/2] = 0;
	return 0;
}

CString GetExePath()
{
	char path_buffer[_MAX_PATH];
	char drive[_MAX_DRIVE];
	char dir[_MAX_DIR];
	char fname[_MAX_FNAME];
	char ext[_MAX_EXT];

	CString defaultPath = _T("");
	GetModuleFileName(NULL,path_buffer,MAX_PATH);
	_splitpath(path_buffer, drive, dir, fname, ext );
	defaultPath.Format("%s%s",drive,dir);

	return defaultPath;
}



BOOL CheckRecvPNData(BYTE* pBuf)
{
	CString sClientNo;
	DWORD dwNO = *(DWORD*)pBuf;
	sClientNo.Format("%d",dwNO);

	if (sClientNo.IsEmpty())
		return FALSE;
	int iLen=sClientNo.GetLength();
	for(int i=0;i<iLen;i++ )
	{
		if(!(sClientNo[i]>='0' && sClientNo[i]<='9'))
			return FALSE;
	}

	return TRUE;
}

*/

 string ToHexString(BYTE bytes[] ,int len) 
 {
	static char hexDigits[] = {   '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	string strResult ="";

	for (int i = 0; i < len; i++) 
	{
		int b = bytes[i];
		strResult += hexDigits[b >> 4];
		strResult += hexDigits[b & 0xF];
	}
 
	return strResult;
}

BYTE MyRand(BYTE bMin)
{
	srand( (unsigned)time( NULL ) );
	return (BYTE)rand()/4+bMin;
}

//Following is Standard TEA algorithm, don't modify, just call it by xTEA algorithm
//Linda added at 2005/06/13
#define TEA_DELTA 0x9E3779B9
#define TEA_SUM 0xE3779B90

void tinyEncrypt ( const unsigned long * plain, const unsigned long * key, unsigned long *crypt, unsigned int power)
{
    unsigned long y,z,a,b,c,d;
    unsigned long sum = 0;
    unsigned int i;
    unsigned int rounds = 1<<power;
            
    y = plain[0];
    z = plain[1];
    a = key[0];
    b = key[1];
    c = key[2];
    d = key[3];

    for (i = 0; i < rounds; i++) {
        sum += TEA_DELTA;
        y += (z << 4) + a ^ z + sum ^ (z >> 5) + b;
        z += (y << 4) + c ^ y + sum ^ (y >> 5) + d;
    }
    
    crypt[0] = y;
    crypt[1] = z;
}

void tinyDecrypt ( const unsigned long * crypt, const unsigned long * key, unsigned long *plain, unsigned int power)
{
    unsigned long y,z,a,b,c,d;
    unsigned int rounds = 1<<power;
    unsigned long sum = TEA_DELTA<<power;
    unsigned int i;

    y = crypt[0];
    z = crypt[1];
    a = key[0];
    b = key[1];
    c = key[2];
    d = key[3];

    for (i = 0; i < rounds; i++) {
        z -= (y << 4) + c ^ y + sum ^ (y >> 5) + d;
        y -= (z << 4) + a ^ z + sum ^ (z >> 5) + b;
        sum -= TEA_DELTA;
    }
    
    plain[0] = y;
    plain[1] = z;
}


/*================================================================ 
* 
* 函 数 名：xTEADecryptWithKey 
** Decrypt the cipher text to plain text with the key
 
* 参 数： 
* 
* const unsigned long *crypt [IN] : the Cipher text 
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 
/*================================================================ 
* 
* 函 数 名：xTEADecryptWithKey 
** Decrypt the cipher text to plain text with the key

* 参 数： 
* 
* const unsigned long *crypt [IN] : the Cipher text 
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 
BOOL xTEADecryptWithKey(const char *crypt, long crypt_len,char key[16], char *plain, long * plain_len)
{
	const unsigned long *tkey   = (const unsigned long *)key;
	const unsigned long *tcrypt = (const unsigned long *)crypt;

	if( crypt_len<1 || crypt_len%8 )
		return FALSE;

	long *tplain = new long[crypt_len/4+1];

	unsigned int  length = crypt_len;
	unsigned long pre_plain[2] = {0,0};
	unsigned long p_buf[2];
	unsigned long c_buf[2];

	int padLength;
	int i;

	//Decrypt the first 8 bytes(64 bits)
	tinyDecrypt(tcrypt, tkey, p_buf, 4);

	memcpy(pre_plain, p_buf, 8);
	memcpy(tplain, p_buf, 8);

	//Decrype with TEA and interlace algorithm 
	for (i = 2; i < (int)length/4; i+=2) {
		c_buf[0] = *(tcrypt+i) ^ pre_plain[0];
		c_buf[1] = *(tcrypt+i+1) ^ pre_plain[1];
		tinyDecrypt((const unsigned long *)c_buf, tkey, p_buf, 4);
		memcpy(pre_plain, p_buf, 8);
		*(tplain+i) = p_buf[0] ^ *(tcrypt+i-2);
		*(tplain+i+1) = p_buf[1] ^ *(tcrypt+i-1);
	}

	//check the last 7 bytes is 0x00
	if ( tplain[length/4-1] || (tplain[length/4-2]&0xffffff00)) 
	{
		delete[] tplain;
		return FALSE;
	}

	padLength = *((unsigned char *)tplain) & 0x07;

	//Remove padding data
	memcpy(tplain,(byte*)tplain+padLength+3,length);

	*plain_len = crypt_len - (padLength+3) -7;/*(pad 7 bytes 0x00 at the end)*/

	memcpy(plain,tplain,*plain_len);


	delete [] tplain;

	return TRUE;
}
/*================================================================ 
* 
* 函 数 名：xTEAEncryptWithKey 
** Encrypt the plain text to cipher text with the key

* 参 数： 
* 
* const unsigned long *plain [IN] : the plain text 
* DWORD dwPlainLen[IN]: plain text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *crypt [[IN,OUT]] : the pointer to cipher text(net order byte)
* DWORD * pdwCryptLen[IN,OUT]: Valid cipher text length
* 
* 返 回 值：BOOL-	SUCCESS:TRUE
*							Fail:NULL 
* 
* 作 者：刘晓敏 2005/6/17 
* 
================================================================*/ 

BOOL xTEAEncryptWithKey(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len )
{
	const unsigned char pad[9] = {0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad};

	unsigned long *tkey = (unsigned long *)key;
	unsigned long *tplain = (unsigned long *)plain;

	if ( plain_len<1 )
	{
		return FALSE;
	}

	unsigned long pre_plain[2] = {0,0};
	unsigned long pre_crypt[2] = {0,0};
	unsigned long p_buf[2];
	unsigned long c_buf[2];

	int padLength;
	int i;

	// padding data
	padLength = (plain_len+10)%8;//at least pad 2 bytes
	padLength = padLength ? 8 - padLength : 0;//total pad length -2

	long length = padLength+3+plain_len+7;
	*crypt_len = length;

	long *tcrypt = new long[length/4];

	*((unsigned char*)tcrypt) = 0xa8 | (unsigned char)padLength;//first pad byte: total padding bytes - 2 or 0xa8 
	memcpy ( (byte*)tcrypt+1, (byte*)pad, padLength+2);//add other padding data
	memcpy ( (byte*)tcrypt+padLength+3, (byte*)tplain, plain_len);//add plain data
	memset ( (byte*)tcrypt+padLength+3+plain_len, 0, 7);  //pad 7 0x00 at the end      

	//Interlace algorithm(交织算法)
	for (i = 0; i < length/4; i+=2) {
		p_buf[0] = *(tcrypt+i) ^ pre_crypt[0];
		p_buf[1] = *(tcrypt+i+1) ^ pre_crypt[1];
		tinyEncrypt( p_buf, tkey, c_buf, 4);
		*(tcrypt+i) = c_buf[0] ^ pre_plain[0];
		*(tcrypt+i+1) = c_buf[1] ^ pre_plain[1];
		memcpy(pre_crypt, tcrypt+i, 8);
		memcpy(pre_plain, p_buf, 8);
	}

	memcpy(crypt,tcrypt,length);

	delete []tcrypt;

	return TRUE;

}