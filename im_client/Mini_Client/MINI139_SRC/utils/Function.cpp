
#include "StdAfx.h"
#include "Function.h"

// 返回网络字节
DWORD GetLocalIpAddress()
{
	char host_name[255];
	gethostname(host_name,255);

	hostent * host = gethostbyname(host_name);

	if ( host )
	{
		in_addr addr;
		addr.S_un.S_addr = 0;
		int i = 0;
		while(host->h_addr_list[i])
		{
			memcpy(&addr.S_un.S_addr,host->h_addr_list[i],
				host->h_length);
			i++;
			if( addr.S_un.S_addr == 0x04000100 ||
				addr.S_un.S_addr == 0
				)
			{
				continue;
			}
			else
			{
				break;
			}
		}
		

		return addr.S_un.S_addr;
	}
	else
	{
		return 0;
	}
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
BOOL xTEADecryptWithKey(const char *crypt, long crypt_len, char key[16], char *plain, long * plain_len)
{
	const unsigned long *tkey   = (const unsigned long *)key;
	const unsigned long *tcrypt = (const unsigned long *)crypt;

	if( crypt_len<1 || crypt_len%8 )
	{
		TRACE("xTEADecryptWithKey Failed! crypt_len %d\r\n",crypt_len);
		return FALSE;
	}

	try
	{
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
			TRACE("[]tplain\r\n");
			return FALSE;
		}

		padLength = *((unsigned char *)tplain) & 0x07;
		*plain_len = crypt_len - (padLength+3) -7;/*(pad 7 bytes 0x00 at the end)*/
		if(padLength + 3 + *plain_len > (crypt_len/4+1)*sizeof(long))
		{
			delete [] tplain;
			return FALSE;
		}
		//Remove padding data
		memcpy(tplain,(byte*)tplain+padLength+3,*plain_len);
		memcpy(plain,tplain,*plain_len);

		delete [] tplain;

		return TRUE;
	}
	catch(...)
	{
		TRACE("xTEADecryptWithKey Exception\r\n");
		return FALSE;
	}	
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

bool  CheckEmail(CString strEmail, CString &strError)
{
	strEmail.Trim();

	if ( strEmail.GetLength()<5 )
	{
		strError = "Email地址长度不能少于5个字节!";//长度不能少于5";
		return false;
	}

	int p1 = strEmail.Find("@",0);

	if ( p1<1 )
	{
		strError = "非法的Email地址，地址中缺少@字符。";
		return false;
	}
	else
	if ( strEmail.Find("@",p1+1)>p1 )
	{
		strError = "非法的Email地址，地址中不能有多个@字符";
		return false;
	}

	strEmail.MakeLower();

	for ( int i=0; i<strEmail.GetLength(); i++ )
	{
		char c=strEmail[i];

		if ( (c<='z' && c>='a') || 
			 (c>='0' && c<='9') ||
			 c=='-' ||
			 c=='.' ||
			 c=='_' ||
			 c=='\'' ||
			 c==' '||
			 c=='@' 
			)
		{
			;
		}
		else
		{
			strError.Format("Email地址中不能有非法的字符: %c",c);
			return false;
		}
	}

	return true;
}

void ExtractFilePath(CString strFilePathName, CString &strPath, CString &strName, CString &strExt)
{
	CString strFileName;
	int l = strFilePathName.GetLength();

	while( l>-1 )
	{
		if ( strFilePathName[l]!='\\' && strFilePathName[l]!='/')
		{
            l--;
			continue;
		}
	
		break;
	}

	strFileName = strFilePathName.Right(strFilePathName.GetLength()-l-1);
	strPath = strFilePathName.Left(l+1);

	l = strFileName.GetLength();

	while( l>-1 )
	{
		if ( strFileName[l]!='.' )
		{
            l--;
			continue;
		}
	
		break;
	}

	strName = strFileName.Left(l);
	strExt = strFileName.Right(strFileName.GetLength()-l);
}

bool operator != (const CRect& lhs, const CRect& rhs)
{
	return lhs.top != rhs.top || lhs.bottom != rhs.bottom || lhs.left != rhs.left || lhs.right != rhs.left;
}

BOOL DeleteDirectory(const char *DirName) 
{ 
	CFileFind tempFind; 
	char tempFileFind[256]; 
	sprintf(tempFileFind,"%s\\*.*",DirName); 
	BOOL IsFinded=(BOOL)tempFind.FindFile(tempFileFind); 
	while(IsFinded) 
	{
		IsFinded=(BOOL)tempFind.FindNextFile(); 
    if(!tempFind.IsDots()) 
    { 
     char foundFileName[256]; 
     strcpy(foundFileName,tempFind.GetFileName().GetBuffer(256)); 
     if(tempFind.IsDirectory()) 
     { 
       char tempDir[256]; 
       sprintf(tempDir,"%s\\%s",DirName,foundFileName); 
       DeleteDirectory(tempDir); 
     } 
     else 
     { 
       char tempFileName[256]; 
       sprintf(tempFileName,"%s\\%s",DirName,foundFileName); 
       DeleteFile(tempFileName); 
     } 
    } 
  } 
  tempFind.Close(); 

  if(!RemoveDirectory(DirName)) 
  { 
 //   MessageBox(0,"删除目录失败！","警告信息",MK_OK); 
    return FALSE; 
  } 
  return TRUE; 
}
