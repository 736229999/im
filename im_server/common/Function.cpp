
#include "stdafx.h"
#include "Function.h"
#include "time.h"
#include <iconv.h>

#include <openssl/aes.h>



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
    strcpy_s(chChar, (LPCTSTR)str);
    return  atoi(chChar);
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
//base64编码
std::string Base64Encode(const std::string& strInput)
{
    static char Base64Str[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int     L,I,J;
    byte    B1,B2,B3,B4;
    std::string strTemp,strResult;

    L = strInput.length();
    J = 0;

    for ( I=0; I<L; I++)
    {
        strTemp=strTemp+strInput[I];

        if ( J<2 )
        {
            J++;
        }
        else
        {
            J=0;
            B1=((byte)strTemp[0])>>2;
            B2=((((byte)strTemp[0])&3)<<4)+(((byte)strTemp[1])>>4);
            B3=((byte)strTemp[2]>>6) + (((byte)strTemp[1]&0x0F)<<2);
            B4=(byte)strTemp[2]&0x3F;

            strResult+=Base64Str[B1];
            strResult+=Base64Str[B2];
            strResult+=Base64Str[B3];
            strResult+=Base64Str[B4];

            strTemp="";
        }
    }

    if ( J==1 )
    {
        B1=(byte)strTemp[0]>>2;
        B2=((byte)strTemp[0]&3)<<4;

        strResult+=Base64Str[B1];
        strResult+=Base64Str[B2];
        strResult+="==";
    }

    if ( J==2 )
    {
        B1=(byte)strTemp[0]>>2;
        B2=(((byte)strTemp[0]&3)<<4)+((byte)strTemp[1]>>4);
        B3=((byte)strTemp[1]&0x0F)<<2;

        strResult+=Base64Str[B1];
        strResult+=Base64Str[B2];
        strResult+=Base64Str[B3];
        strResult+="=";
    }

    return strResult;
}

//base64编码
std::string Base64Encode(const char* szInput, int inByte)
{
    static char Base64Str[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
    int     L,I,J;
    byte    B1,B2,B3,B4;
    std::string strTemp,strResult;

    L = inByte;
    J = 0;

    for ( I=0; I<L; I++)
    {
        strTemp=strTemp+szInput[I];

        if ( J<2 )
        {
            J++;
        }
        else
        {
            J=0;
            B1=((byte)strTemp[0])>>2;
            B2=((((byte)strTemp[0])&3)<<4)+(((byte)strTemp[1])>>4);
            B3=((byte)strTemp[2]>>6) + (((byte)strTemp[1]&0x0F)<<2);
            B4=(byte)strTemp[2]&0x3F;

            strResult+=Base64Str[B1];
            strResult+=Base64Str[B2];
            strResult+=Base64Str[B3];
            strResult+=Base64Str[B4];

            strTemp="";
        }
    }

    if ( J==1 )
    {
        B1=(byte)strTemp[0]>>2;
        B2=((byte)strTemp[0]&3)<<4;

        strResult+=Base64Str[B1];
        strResult+=Base64Str[B2];
        strResult+="==";
    }

    if ( J==2 )
    {
        B1=(byte)strTemp[0]>>2;
        B2=(((byte)strTemp[0]&3)<<4)+((byte)strTemp[1]>>4);
        B3=((byte)strTemp[1]&0x0F)<<2;

        strResult+=Base64Str[B1];
        strResult+=Base64Str[B2];
        strResult+=Base64Str[B3];
        strResult+="=";
    }

    return strResult;
}


//base64解码
string Base64Decode(const string& strData/*, int& OutByte*/)
{
    char szData[1024];
    strcpy(szData, strData.c_str());
    return Base64Decode(szData, strlen(szData)/*, OutByte*/);

}

//base64解码
string Base64Decode(const char* Data,int DataByte/*,int& OutByte*/)
{
    //解码表
    const char DecodeTable[] =
    {
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0, 0,
        62, // '+'
        0, 0, 0,
        63, // '/'
        52, 53, 54, 55, 56, 57, 58, 59, 60, 61, // '0'-'9'
        0, 0, 0, 0, 0, 0, 0,
        0, 1, 2, 3, 4, 5, 6, 7, 8, 9, 10, 11, 12,
        13, 14, 15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, // 'A'-'Z'
        0, 0, 0, 0, 0, 0,
        26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38,
        39, 40, 41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, // 'a'-'z'
    };
    //返回值
    string strDecode;
    int nValue;
    int i= 0;
    while (i < DataByte)
    {
        if (*Data != '\r' && *Data!='\n')
        {
            nValue = DecodeTable[(uint8)(*Data++)] << 18;
            nValue += DecodeTable[(uint8)(*Data++)] << 12;
            strDecode += (nValue & 0x00FF0000) >> 16;
            // OutByte++;
            if (*Data != '=')
            {
                nValue += DecodeTable[(uint8)(*Data++)] << 6;
                strDecode += (nValue & 0x0000FF00) >> 8;
                //OutByte++;
                if (*Data != '=')
                {
                    nValue += DecodeTable[(uint8)(*Data++)];
                    strDecode += nValue & 0x000000FF;
                    // OutByte++;
                }
            }
            i += 4;
        }
        else// 回车换行,跳过
        {
            Data++;
            i++;
        }
    }
    return strDecode;
}


string ToHexString(BYTE bytes[] ,int len)
{
    static char hexDigits[] = {   '0', '1', '2', '3', '4', '5', '6', '7',
                                  '8', '9', 'a', 'b', 'c', 'd', 'e', 'f'
                              };

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

void tinyEncrypt ( const unsigned int * plain, const unsigned int * key, unsigned int *crypt, unsigned int power)
{
    unsigned int y,z,a,b,c,d;
    unsigned int sum = 0;
    unsigned int i;
    unsigned int rounds = 1<<power;

    y = plain[0];
    z = plain[1];
    a = key[0];
    b = key[1];
    c = key[2];
    d = key[3];

    for (i = 0; i < rounds; i++)
    {
        sum += TEA_DELTA;
        y += ( (z << 4) + a ) ^ ( z + sum ) ^ ( (z >> 5) + b );
        z += ( (y << 4) + c ) ^ ( y + sum ) ^ ( (y >> 5) + d );
    }

    crypt[0] = y;
    crypt[1] = z;
}

void tinyDecrypt ( const unsigned int * crypt, const unsigned int * key, unsigned int *plain, unsigned int power)
{
    unsigned int y,z,a,b,c,d;
    unsigned int rounds = 1<<power;
    unsigned int sum = TEA_DELTA<<power;
    unsigned int i;

    y = crypt[0];
    z = crypt[1];
    a = key[0];
    b = key[1];
    c = key[2];
    d = key[3];

    for (i = 0; i < rounds; i++)
    {
        z -= ( (y << 4) + c ) ^ ( y + sum ) ^ ( (y >> 5) + d );
        y -= ( (z << 4) + a ) ^ ( z + sum ) ^ ( (z >> 5) + b );
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
* const unsigned int *crypt [IN] : the Cipher text
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned int *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned int *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
*
* 返 回 值：BOOL-   SUCCESS:TRUE
*                           Fail:NULL
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
* const unsigned int *crypt [IN] : the Cipher text
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned int *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned int *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
*
* 返 回 值：BOOL-   SUCCESS:TRUE
*                           Fail:NULL
*
* 作 者：刘晓敏 2005/6/17
*
================================================================*/
BOOL xTEADecryptWithKey(const char *crypt, int crypt_len,char key[16], char *plain, int * plain_len)
{
    try
    {

        const unsigned int *tkey   = (const unsigned int *)key;
        const unsigned int *tcrypt = (const unsigned int *)crypt;

        if( crypt_len<1 || crypt_len%8 )
            return FALSE;

        int *tplain = new int[crypt_len/4+1];
        memset(tplain, 0, (crypt_len/4+1)*sizeof(int));

        unsigned int  length = crypt_len;
        unsigned int pre_plain[2] = {0,0};
        unsigned int p_buf[2];
        unsigned int c_buf[2];

        int padLength;
        int i;

        //Decrypt the first 8 bytes(64 bits)
        tinyDecrypt(tcrypt, tkey, p_buf, 4);

        memcpy(pre_plain, p_buf, 8);
        memcpy(tplain, p_buf, 8);

        //Decrype with TEA and interlace algorithm
        for (i = 2; i < (int)length/4; i+=2)
        {
            c_buf[0] = *(tcrypt+i) ^ pre_plain[0];
            c_buf[1] = *(tcrypt+i+1) ^ pre_plain[1];
            tinyDecrypt((const unsigned int *)c_buf, tkey, p_buf, 4);
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
		*plain_len = crypt_len - (padLength+3) -7;/*(pad 7 bytes 0x00 at the end)*/
		if(padLength + 3 + *plain_len > (int)(crypt_len/4+1)*(int)sizeof(int))
		{
			delete [] tplain;
			return FALSE;
		}
		//Remove padding data
		memcpy(tplain,(byte*)tplain+padLength+3,*plain_len);
		memcpy(plain,tplain,*plain_len);

        delete [] tplain;
    }
    catch(...)
    {
        return FALSE;
    }
    return TRUE;
}
/*================================================================
*
* 函 数 名：xTEAEncryptWithKey
** Encrypt the plain text to cipher text with the key

* 参 数：
*
* const unsigned int *plain [IN] : the plain text
* DWORD dwPlainLen[IN]: plain text length
* const unsigned int *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned int *crypt [[IN,OUT]] : the pointer to cipher text(net order byte)
* DWORD * pdwCryptLen[IN,OUT]: Valid cipher text length
*
* 返 回 值：BOOL-   SUCCESS:TRUE
*                           Fail:NULL
*
* 作 者：刘晓敏 2005/6/17
*
================================================================*/

BOOL xTEAEncryptWithKey(const char *plain, int plain_len, char key[16], char *crypt, int * crypt_len )
{
    try
    {
        const unsigned char pad[9] = {0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad};

        unsigned int *tkey = (unsigned int *)key;
        unsigned int *tplain = (unsigned int *)plain;

        if ( plain_len<1 )
        {
            return FALSE;
        }

        unsigned int pre_plain[2] = {0,0};
        unsigned int pre_crypt[2] = {0,0};
        unsigned int p_buf[2];
        unsigned int c_buf[2];

        int padLength;
        int i;

        // padding data
        padLength = (plain_len+10)%8;//at least pad 2 bytes
        padLength = padLength ? 8 - padLength : 0;//total pad length -2

        int length = padLength+3+plain_len+7;
        *crypt_len = length;

        int *tcrypt = new int[length/4];
        if (!tcrypt)
            return FALSE;

        *((unsigned char*)tcrypt) = 0xa8 | (unsigned char)padLength;//first pad byte: total padding bytes - 2 or 0xa8
        memcpy ( (byte*)tcrypt+1, (byte*)pad, padLength+2);//add other padding data
        memcpy ( (byte*)tcrypt+padLength+3, (byte*)tplain, plain_len);//add plain data
        memset ( (byte*)tcrypt+padLength+3+plain_len, 0, 7);  //pad 7 0x00 at the end

        //Interlace algorithm(交织算法)
        for (i = 0; i < length/4; i+=2)
        {
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
    }
    catch (...)
    {
        return FALSE;
    }
    return TRUE;
}
void getrandomstring(char * str)
{
    int rNum;
    char ch[16];

    srand((unsigned)timeGetTime());
    for ( int i = 0; i < 16; ++ i)
    {
        rNum=1+(int)((rand()/(double)RAND_MAX)*36); //求随机值
        switch (rNum)
        {
            case 1:
                ch[i]='a';
                break ;
            case 2:
                ch[i]='b';
                break ;
            case 3:
                ch[i]='c';
                break ;
            case 4:
                ch[i]='d';
                break ;
            case 5:
                ch[i]='e';
                break ;
            case 6:
                ch[i]='f';
                break ;
            case 7:
                ch[i]='g';
                break ;
            case 8:
                ch[i]='h';
                break ;
            case 9:
                ch[i]='i';
                break ;
            case 10:
                ch[i]='j';
                break ;
            case 11:
                ch[i]='k';
                break ;
            case 12:
                ch[i]='l';
                break ;
            case 13:
                ch[i]='m';
                break ;
            case 14:
                ch[i]='n';
                break ;
            case 15:
                ch[i]='o';
                break ;
            case 16:
                ch[i]='p';
                break ;
            case 17:
                ch[i]='q';
                break ;
            case 18:
                ch[i]='r';
                break ;
            case 19:
                ch[i]='s';
                break ;
            case 20:
                ch[i]='t';
                break ;
            case 21:
                ch[i]='u';
                break ;
            case 22:
                ch[i]='v';
                break ;
            case 23:
                ch[i]='w';
                break ;
            case 24:
                ch[i]='x';
                break ;
            case 25:
                ch[i]='y';
                break ;
            case 26:
                ch[i]='z';
                break ;
            case 27:
                ch[i]='0';
                break;
            case 28:
                ch[i]='1';
                break;
            case 29:
                ch[i]='2';
                break;
            case 30:
                ch[i]='3';
                break;
            case 31:
                ch[i]='4';
                break;
            case 32:
                ch[i]='5';
                break;
            case 33:
                ch[i]='6';
                break;
            case 34:
                ch[i]='7';
                break;
            case 35:
                ch[i]='8';
                break;
            case 36:
                ch[i]='9';
                break;
        }
    }
    memcpy(str, ch, 16);
    return;
}

bool  CheckEmail(const std::string& strEmail)
{
    if ( strEmail.length()<5 )
    {
        return false;
    }

    size_t i = strEmail.find("@");

    if (i == std::string::npos)
    {
        return false;
    }

    for ( size_t i=0; i<strEmail.length(); i++ )
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
            return false;
        }
    }

    return true;
}


int gbk2utf8(char *utfstr, const char *srcstr)
{
    wchar_t unicodestr[LOG_MAX_UTF8_DESCRIPTION_SIZE] = {0};

    if(NULL == srcstr)
    {
        return -1;
    }
    if(strlen(srcstr) == 0)
    {
        return -7;
    }
    //首先先将gbk编码转换为unicode编码
    if(NULL == setlocale(LC_ALL, "zh_CN.gbk"))//设置转换为unicode前的,当前为gbk编码
    {
        return -2;
    }
    int unicodelen = mbstowcs(NULL, srcstr, 0);//计算转换后的长度  的长度
    if(unicodelen <= 0)
    {
        return -3;
    }
    mbstowcs(unicodestr, srcstr, strlen(srcstr));//将gbk转换为unicode

    //将unicode编码转换为utf8编码
    if(NULL == setlocale(LC_ALL, "zh_CN.utf8"))//设置unicode转换后的,当前为utf8
    {
        return -4;
    }
    int utflen = wcstombs(NULL, unicodestr, 0);//计算转换后的长度  的长度
    if(utflen <= 0)
    {
        return -5;
    }
    else if(utflen >= LOG_MAX_DESCRIPTION_SIZE)//判断空间是否足够
    {
        return -6;
    }
    wcstombs(utfstr, unicodestr, utflen);
    utfstr[utflen] = '\0';//添加结束  加结束

    return utflen;
}


/*------------------------------------------------------------------------------------
url转义符编码
---------------------------------------------------------------------------*/

string urlencode(string &str_source)
{
    char const *in_str = str_source.c_str();
    int in_str_len = strlen(in_str);
//    int out_str_len = 0;
    string out_str;
    register unsigned char c;
    unsigned char *to, *start;
    unsigned char const *from, *end;
    unsigned char hexchars[] = "0123456789ABCDEF";

    from = (unsigned char *)in_str;
    end = (unsigned char *)in_str + in_str_len;
    start = to = (unsigned char *) malloc(3*in_str_len+1);

    while (from < end)
    {
        c = *from++;

        if (c == ' ')
        {
            *to++ = '+';
        }
        else if ((c < '0' && c != '-' && c != '.') ||
                 (c < 'A' && c > '9') ||
                 (c > 'Z' && c < 'a' && c != '_') ||
                 (c > 'z'))
        {
            to[0] = '%';
            to[1] = hexchars[c >> 4];
            to[2] = hexchars[c & 15];
            to += 3;
        }
        else
        {
            *to++ = c;
        }
    }
    *to = 0;

    //  out_str_len = to - start;

    out_str = (char *) start;
    free(start);
    return out_str;
}

/*------------------------------------------------------------------------------------
字符串中子串替换

original:  原始字符串
subset:  被替换的字串
section:  替换后的字串

---------------------------------------------------------------------------*/
string replace( string &original, const string &subset, const string &section )
{
    if( subset == section or subset.empty() )
        return original;
    for( string::size_type index = original.find( subset ); index not_eq string::npos; index = original.find( subset, index ) )
    {
        original.replace( index, subset.size(), section);
        index += section.size();
    }
    return original;
}

/*
    utf8 转gbk
*/
int utf2gbk(char *inbuf,int inlen, char *outbuf, int outlen)
{
	wchar_t unicodestr[LOG_MAX_UTF8_DESCRIPTION_SIZE*3] = {0};
	
	if(NULL == inbuf)
	{
		return -1;
	}
	if(strlen(inbuf) == 0)
	{
		return -7;
	}
	//首先先将utf8编码转换为unicode编码
	if(NULL == setlocale(LC_ALL, "zh_CN.utf8"))//设置转换为unicode前的,当前为utf8编码
	{
		return -2;
	}
	int unicodelen = mbstowcs(NULL, inbuf, 0);//计算转换后的长度  的长度
	if(unicodelen <= 0)
	{
		return -3;
	}
	else if(unicodelen >= LOG_MAX_UTF8_DESCRIPTION_SIZE*3)
	{
		return -8;
	}
	mbstowcs(unicodestr, inbuf, inlen);//将gbk转换为unicode

	//将unicode编码转换为gbk编码
	if(NULL == setlocale(LC_ALL, "zh_CN.gbk"))//设置unicode转换后的,当前为gbk编码
	{
		return -4;
	}
	int utflen = wcstombs(NULL, unicodestr, 0);//计算转换后的长度  的长度
	if(utflen <= 0)
	{
		return -5;
	}
	else if(utflen >= outlen)//判断空间是否足够
	{
		return -6;
	}
	wcstombs(outbuf, unicodestr, utflen);
	outbuf[utflen] = '\0';//添加结束
	
	return utflen;
}

// 字符串AES加密
int AesEncode(const char *key, int keylen, const char *inbuf,int inlen, char *outbuf, int &outlen)
{
    AES_KEY aes_key;
    if(AES_set_encrypt_key((const unsigned char*)key, keylen * 8, &aes_key) < 0)
    {
        return -1;
    }
    
    int padding = AES_BLOCK_SIZE - inlen % AES_BLOCK_SIZE;
    outlen = inlen + padding;
    
    char pad[AES_BLOCK_SIZE] = {0};
    ::memset(pad,padding,padding);

    char *pNewInBuf = (char*)malloc(outlen);
    memcpy(pNewInBuf, inbuf, inlen);
    memcpy(pNewInBuf+inlen, pad, padding);
    
    for(unsigned int i = 0; i < outlen/AES_BLOCK_SIZE; i++)
    {
        AES_encrypt((const unsigned char*)(pNewInBuf+i*AES_BLOCK_SIZE), (unsigned char*)(outbuf+i*AES_BLOCK_SIZE), &aes_key);
    }

   	free(pNewInBuf);
   	pNewInBuf = NULL;

   	return 0;
}

int utf2GB18030(char *inbuf,int inlen, char *outbuf, int outlen)
{
    iconv_t cd;
//  int rc;
    char **pin = &inbuf;
    char **pout = &outbuf;

    size_t olen = outlen;
    size_t ilen = inlen;

    cd = iconv_open("GB18030","utf-8");

    if (cd==(iconv_t)-1)
        return -1;

    memset(outbuf,0,olen);

    iconv(cd,pin,&ilen,pout,&olen);

    iconv_close(cd);

    return 0;
}

char *random_uuid( char buf[37] )
{
    const char *c = "89ab";
    char *p = buf;
    int n;
    for( n = 0; n < 16; ++n )
    {
        int b = rand()%255;
        switch( n )
        {
            case 6:
                sprintf(p, "4%x", b%15 );
            break;
            case 8:
                sprintf(p, "%c%x", c[rand()%strlen(c)], b%15 );
            break;
            default:
                sprintf(p, "%02x", b);
            break;
        }
 
        p += 2;
        switch( n )
        {
            case 3:
            case 5:
            case 7:
            case 9:
                *p++ = '-';
                break;
        }
    }
    *p = 0;
    return buf;
}

//注意：当字符串为空时，也会返回一个空字符串
void split(string str, string separator, vector<string>* ret)
{
	int cutAt;
	while( (cutAt = str.find_first_of(separator)) != (int)string::npos )
	{
		if(cutAt > 0)
		{
			ret->push_back(str.substr(0, cutAt));
		}
		str = str.substr(cutAt + 1);
	}
	if(str.length() > 0)
    {
	   ret->push_back(str);
    }
}

