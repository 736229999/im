#include "stdafx.h"
#include "hmgcrtlib.h"

#include "Atlbase.h"
//�ļ��������

/////////////////////////////////////////////////////////////////////////////////////////
//  GetFileSize
//  ���ܣ���ȡ�ļ��Ĵ�С����λ���ֽڣ�
//  ���룺strFile, �ļ�ȫ·��
//  �������
//  ����ֵ���ļ��ֽ���
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
DWORD GetFileSize(CString strFile)
{
	CFileStatus st;

	if(CFile::GetStatus(strFile,st) == false)
	{
		return -1;
	}
	else
	{
		return (DWORD)st.m_size;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  FileAccess
//  ���ܣ��ж��ļ��Ƿ��ܹ��Կɶ��ķ�ʽ�򿪣�ע����ļ��Ƿ����������
//  ���룺strFile, �ļ�ȫ·��
//  �������
//  ����ֵ��bool
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
bool FileAccess(CString strFile)
{
	CFile f;
	if ( f.Open((LPCSTR)strFile,CFile::modeRead) )
	{
		f.Close();
		return true;
	}
	else
	{
		return false;
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  FileExist
//  ���ܣ��ж��ļ��Ƿ����
//  ���룺strFile, �ļ�ȫ·��
//  �������
//  ����ֵ��bool
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
bool FileExist(CString strFile)
{
	if ( _access((LPCSTR)strFile,0)==-1 )
		return false;
	else
		return true;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  GetFileNameFromPath
//  ���ܣ���ȡһ���ļ�ȫ·���ַ����е��ļ���
//  ���룺strFullPathName, �ļ�ȫ·��
//  �����
//  ����ֵ���ļ���
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString GetFileNameFromPath(CString strFullPathName)
{
	return strFullPathName.Right(strFullPathName.GetLength() - strFullPathName.ReverseFind('\\') - 1);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  GetModulePath
//  ���ܣ���ȡģ�����ڵ�·��
//  ���룺
//  �����
//  ����ֵ��ģ��·��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString GetModulePath()
{
	char szPath[MAX_PATH];
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL,szPath,MAX_PATH);
	//_splitpath( szPath, drive, dir, fname, ext );
	_splitpath_s( szPath, drive,sizeof(drive), dir,sizeof(dir), fname, sizeof(fname),ext,sizeof(ext) );
	//_makepath(szPath, drive,dir,"","");
	_makepath_s(szPath,sizeof(szPath), drive,dir,"","");
	return szPath;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  SplitFilePath
//  ���ܣ��ָ�һ���ļ�ȫ·��
//  ���룺strFullPathName, �ļ�ȫ·��
//  �����strPath, �ļ����ڵ�·��
//        strName, �ļ���
//        strExt, �ļ���չ��
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void SplitFilePath(CString strFullPathName, CString &strPath, CString &strName, CString &strExt)
{
	CString strFileName;
	int l = strFullPathName.GetLength();

	while( l>-1 )
	{
		if ( strFullPathName[l]!='\\' && strFullPathName[l]!='/')
		{
			l--;
			continue;
		}

		break;
	}

	strFileName = strFullPathName.Right(strFullPathName.GetLength()-l-1);
	strPath = strFullPathName.Left(l+1);

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

/////////////////////////////////////////////////////////////////////////////////////////
//  DeleteDirectory
//  ���ܣ����һ���ļ���
//  ���룺strDirName�� �ļ���
//  �������
//  ����ֵ��bool
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
bool DeleteDirectory(CString strDirName) 
{ 
	CFileFind tempFind; 
	char tempFileFind[256]; 
	//sprintf(tempFileFind,"%s\\*.*",strDirName); 
	sprintf_s(tempFileFind,sizeof(tempFileFind),"%s\\*.*",strDirName); 
	bool IsFinded=(bool)tempFind.FindFile(tempFileFind); 
	while(IsFinded) 
	{
		IsFinded=(bool)tempFind.FindNextFile(); 
		if(!tempFind.IsDots()) 
		{ 
			char foundFileName[256]; 
			//strcpy(foundFileName,tempFind.GetFileName().GetBuffer(256)); 
			strcpy_s(foundFileName,sizeof(foundFileName),tempFind.GetFileName().GetBuffer(256)); 
			if(tempFind.IsDirectory()) 
			{ 
				char tempDir[256]; 
				//sprintf(tempDir,"%s\\%s",strDirName,foundFileName); 
				sprintf_s(tempDir,sizeof(tempDir),"%s\\%s",strDirName,foundFileName); 
				DeleteDirectory(tempDir); 
			} 
			else 
			{ 
				char tempFileName[256]; 
				//sprintf(tempFileName,"%s\\%s",strDirName,foundFileName); 
				sprintf_s(tempFileName,sizeof(tempFileName),"%s\\%s",strDirName,foundFileName);
				DeleteFile(tempFileName); 
			} 
		} 
	} 
	tempFind.Close(); 

	if(!RemoveDirectory(strDirName)) 
	{ 
		return false; 
	} 
	else
	{
		return true; 
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  GetWebServerName
//  ���ܣ���ȡһ������url���ļ���
//  ���룺strUrl, URL(��http://www.sina.com/index.html)
//  �������
//  ����ֵ�������ļ���(��index.html)
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString GetWebServerName(CString strUrl)
{
	int pos = strUrl.Find("/",0);
	return strUrl.Mid(0,pos);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  GetWebFileName
//  ���ܣ���ȡһ������url���ļ���
//  ���룺strUrl, URL(��http://www.sina.com/index.html)
//  �������
//  ����ֵ�������ļ���(��index.html)
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString GetWebFileName(CString strUrl)
{
	int l = strUrl.GetLength();
	int c = 0;
	for ( int i=l-1; i>=0; i-- )
	{
		if ( strUrl[i]=='/' )
		{
			return strUrl.Right(c);
		}
		else
		{
			c++;
		}
	}
	return "";
}



//�ַ�����������

/////////////////////////////////////////////////////////////////////////////////////////
//  GetSectionValue
//  ���ܣ���ȡָ���ַ�����2���ؼ���֮����ִ�
//  ���룺strInfo��ָ�����ַ���
//        str1��ǰһ���ؼ���
//        str2����һ���ؼ���
//  �����strRet��2���ؼ���֮����ַ���
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void GetSectionValue(const CString &strInfo, CString str1,CString str2, CString &strRet)
{
	int		iPos1 , iPos2, iLen;

	iPos1 = strInfo.Find(str1);
	iPos2 = strInfo.Find(str2);

	iLen  = str1.GetLength();

	if ( iPos2>iPos1 && iPos1>-1 )
	{
		strRet = strInfo.Mid(iPos1+iLen,iPos2-iPos1-iLen);
	}
	else
	{
		strRet = "";
	}
}


/////////////////////////////////////////////////////////////////////////////////////////
//  GetHtmlImageList
//  ���ܣ���html�ļ��л�ȡ���е�img��ǵ�src��ֵ
//  ���룺strHtml��HTML����
//  ����ֵ��slImages��ͼƬurl�б�
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void GetHtmlImageList(const CString &strHtml, CStringList &slImages)
{
	CString strLower = strHtml;
	strLower.MakeLower();

	int iPos1=0 , iPos2=0;

	int iPosImg = 0;

	while (1)
	{
		iPos1 = strLower.Find("<img",iPosImg);

		if ( iPos1>-1 )
		{
			iPos2 = strLower.Find(">",iPos1);

			if ( iPos2>-1 )
			{
				CString sFile ;
				sFile = strHtml.Mid(iPos1,iPos2-iPos1+1);

				CString sPath;
				GetSectionValue(sFile,"src=",">",sPath);
				sPath.Replace("\"","");
				slImages.AddTail(sPath);

				iPosImg = iPos2;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}
}

/////////////////////////////////////////////////////////////////////////////////////////
//  OpenHtmlPage
//  ���ܣ��ò���ϵͳĬ�ϵ��������һ��URL
//  ���룺strUrl,ָ����URL
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void  OpenHtmlPage(CString strUrl)
{
	CString strExplorePath ;

	HKEY hKey;
	LONG ret ;
	LPCTSTR data_Set = "HTTP\\shell\\open\\command";
	::RegOpenKeyEx(HKEY_CLASSES_ROOT,data_Set,0,KEY_ALL_ACCESS,&hKey);

	DWORD dwType = REG_SZ;
	DWORD dwSize = 255;
	char  szValue[255];

	ret = ::RegQueryValueEx(hKey,"",NULL,&dwType,(LPBYTE)szValue,&dwSize);

	bool bAutoStart = false;

	if ( ret==ERROR_SUCCESS )
	{
		strExplorePath =szValue;
		strExplorePath.Trim();

		if ( strExplorePath=="")
		{
			ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
		}
		else
		{		
			int iPos=strExplorePath.Find("\"",0);

			if ( iPos==0 )
			{
				iPos = strExplorePath.Find("\"",1);
				strExplorePath=strExplorePath.Mid(1,iPos-1);
			}
			else
			{
				iPos = strExplorePath.Find(" ",0 );
				strExplorePath.Mid(0,iPos);
			}

			CString strPath,strName,strExt;
			SplitFilePath(strExplorePath,strPath,strName,strExt);

			if ( _access(strExplorePath,0)!=-1 )
				ShellExecute(NULL,"open",strName+strExt,strUrl,strPath,SW_SHOW);
			else
				ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
		}
	}
	else
	{
		ShellExecute(NULL,"open","iexplore.exe",strUrl,"",SW_SHOW);
	}

	::RegCloseKey(hKey);
}


/////////////////////////////////////////////////////////////////////////////////////////
//  ToHexString
//  ���ܣ���һ���ڴ�������16�����ִ���ʾ
//  ���룺bytes,�ڴ������׵�ַ
//        len, �ڴ����г���
//  ����ֵ���ڴ�����16�����ִ�
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString ToHexString(BYTE bytes[] ,int len) 
{
	static char hexDigits[] = {   '0', '1', '2', '3', '4', '5', '6', '7',
		'8', '9', 'a', 'b', 'c', 'd', 'e', 'f'};

	CString strResult ="";

	for (int i = 0; i < len; i++) 
	{
		int b = bytes[i];
		strResult += hexDigits[b >> 4];
		strResult += hexDigits[b & 0xF];
	}

	return strResult;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  UIntToStr
//  ���ܣ���һunsinged longת�����ַ���
//  ���룺n��unsinged longֵ
//  ����ֵ���ַ�����ʾ��ֵ
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString  UIntToStr(unsigned long n)
{
	CString str;
	str.Format("%u",n);
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  IntToStr
//  ���ܣ���һlongת�����ַ���
//  ���룺n��longֵ
//  ����ֵ���ַ�����ʾ��ֵ
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString	 IntToStr(long n)
{
	CString str;
	str.Format("%d",n);
	return str;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  UrlGetStr
//  ���ܣ���HTTP GET��ȡһ��webҳ���HTML����
//  ���룺lpszUrl,URL
//  ����ֵ�����ص�HTML����
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString UrlGetStr(LPCSTR lpszUrl)
{
	HINTERNET hFile, hInet;
	char buffer[8193];
	DWORD dwRead, dwSize;

	CString strResult="";

	hInet = InternetOpen("xTreamLiveUpdate",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, lpszUrl, NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT ,//| INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{
			try
			{
				dwSize = sizeof(buffer)-1;

				while ( InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
				{
					buffer[dwRead]=0;
					strResult=strResult+buffer;
				}
			}
			catch(...)
			{
			}
			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInet);
	}

	return strResult;
}


//Following is Standard TEA algorithm, don't modify, just call it by xTEA algorithm
//Linda added at 2005/06/13
#define TEA_DELTA 0x9E3779B9
#define TEA_SUM 0xE3779B90

inline void safe_memcpy(void *_Dest,const void *_Src, size_t _Size)
{
	int MemSpace = ((char *)_Src)-((char *)_Dest);

	//memcpy(_Dest,_Src,_Size);
	if ( MemSpace>0  && MemSpace<(int)_Size )
	{
		char * temp = new char[_Size];
		memcpy(temp,_Src,_Size);
		memcpy(_Dest,temp,_Size);
		delete []temp;
	}
	else
	{
		memcpy(_Dest,_Src,_Size);
	}
}


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
* �� �� ����xTEADecryptWithKey 
** Decrypt the cipher text to plain text with the key

* �� ���� 
* 
* const unsigned long *crypt [IN] : the Cipher text 
* DWORD dwCryptLen[IN]: cipher text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *plain [[IN,OUT]] : the pointer to plain text(net order byte)
* DWORD * pdwPlainLen[IN,OUT]: Valid plain text length
* 
* �� �� ֵ��bool-	SUCCESS:true
*							Fail:NULL 
* 
* �� �ߣ������� 2005/6/17 
* 
================================================================*/ 
bool TEADecrypt(const char *crypt, long crypt_len, char key[16], char *plain, long * plain_len)
{
	const unsigned long *tkey   = (const unsigned long *)key;
	const unsigned long *tcrypt = (const unsigned long *)crypt;

	if( crypt_len<1 || crypt_len%8 )
	{
		return false;
	}


	long *tplain = new long[crypt_len/4+1];

	if ( tplain==NULL )
	{
		return false;
	}
	//try
	//{
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
			return false;
		}

		padLength = *((unsigned char *)tplain) & 0x07;

		//Remove padding data
		safe_memcpy(tplain,(byte*)tplain+padLength+3,length);

		*plain_len = crypt_len - (padLength+3) -7;/*(pad 7 bytes 0x00 at the end)*/

		safe_memcpy(plain,tplain,*plain_len);		
		delete [] tplain;

	//}
	//catch(...)
	//{
	//	return false;
	//}
	
	return true;
}
/*================================================================ 
* 
* �� �� ����xTEAEncryptWithKey 
** Encrypt the plain text to cipher text with the key

* �� ���� 
* 
* const unsigned long *plain [IN] : the plain text 
* DWORD dwPlainLen[IN]: plain text length
* const unsigned long *theKey [IN] : the key
* DWORD dwKeyLen[IN]: key length
* unsigned long *crypt [[IN,OUT]] : the pointer to cipher text(net order byte)
* DWORD * pdwCryptLen[IN,OUT]: Valid cipher text length
* 
* �� �� ֵ��bool-	SUCCESS:true
*							Fail:NULL 
* 
* �� �ߣ������� 2005/6/17 
* 
================================================================*/ 

bool TEAEncrypt(const char *plain, long plain_len, char key[16], char *crypt, long * crypt_len )
{
	const unsigned char pad[9] = {0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad,0xad};

	unsigned long *tkey = (unsigned long *)key;
	unsigned long *tplain = (unsigned long *)plain;

	if ( plain_len<1 )
	{
		return false;
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

	if ( tcrypt==NULL )
	{
		return false;
	}

	*((unsigned char*)tcrypt) = 0xa8 | (unsigned char)padLength;//first pad byte: total padding bytes - 2 or 0xa8 
	safe_memcpy ( (byte*)tcrypt+1, (byte*)pad, padLength+2);//add other padding data
	safe_memcpy ( (byte*)tcrypt+padLength+3, (byte*)tplain, plain_len);//add plain data
	memset ( (byte*)tcrypt+padLength+3+plain_len, 0, 7);  //pad 7 0x00 at the end      

	//Interlace algorithm(��֯�㷨)
	for (i = 0; i < length/4; i+=2) {
		p_buf[0] = *(tcrypt+i) ^ pre_crypt[0];
		p_buf[1] = *(tcrypt+i+1) ^ pre_crypt[1];
		tinyEncrypt( p_buf, tkey, c_buf, 4);
		*(tcrypt+i) = c_buf[0] ^ pre_plain[0];
		*(tcrypt+i+1) = c_buf[1] ^ pre_plain[1];
		memcpy(pre_crypt, tcrypt+i, 8);
		memcpy(pre_plain, p_buf, 8);
	}

	safe_memcpy(crypt,tcrypt,length);

	delete []tcrypt;
	

	return true;
}


//ͼ�δ���

/////////////////////////////////////////////////////////////////////////////////////////
//  CBitmapFromFile
//  ���ܣ���һ������ͼƬ����CBitmap����
//  ���룺strFile, ����ͼƬ�ļ�ȫ·��
//        width, ����CBitmap����Ŀ�
//        height,����CBitmap����ĸ�
//  �������
//  ����ֵ�����ɵ�CBitmap����ָ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CBitmap * CBitmapFromFile(CString strImageFile,int nWidth, int nHeight)
{
	Bitmap * bitmap = Bitmap::FromFile(strImageFile.AllocSysString());

	CBitmap *pBitmap = NULL;

	if ( bitmap )
	{
		CWnd *pWnd = CWnd::GetDesktopWindow();

		CDC *pDC = 0;

		try
		{
			pDC = pWnd->GetDC();
		}
		catch(...)
		{
			pDC  = 0;
			pWnd = NULL;
		}
		if( pDC )
		{
			pBitmap = new CBitmap();
			pBitmap->CreateCompatibleBitmap(pDC,nWidth,nHeight);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);

			CBitmap *pOldbmp = dcTemp.SelectObject(pBitmap);

			dcTemp.FillSolidRect(0,0,nWidth,nHeight,0xffffff);

			Graphics g(dcTemp.m_hDC);
			Unit  units;

			float cx = bitmap->GetWidth();
			float cy = bitmap->GetHeight();

			float d1 = cx/nWidth;
			float d2 = cy/nHeight;
			float d  = d1<d2? d1:d2;

			cx=cx/d; cy=cy/d;

			POINT pt;

			pt.x = (nWidth-cx)/2;
			pt.y = (nHeight-cy)/2;

			RectF destRect(REAL(pt.x),	REAL(pt.y),	REAL(cx),	REAL(cy)),
				srcRect;

			bitmap->GetBounds(&srcRect, &units);
			g.DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);
			dcTemp.SelectObject(pOldbmp);
		}

		if ( pDC )
		{
			pWnd->ReleaseDC(pDC);
		}

		delete bitmap;
	}
	return pBitmap;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  CBitmapFromFile
//  ���ܣ���һ��ͼƬ�ļ��ڴ���������CBitmap����
//  ���룺pData, ����ͼƬ�ļ�ȫ·��
//        width, ����CBitmap����Ŀ�
//        height,����CBitmap����ĸ�
//  �������
//  ����ֵ�����ɵ�CBitmap����ָ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CBitmap * CBitmapFromStream(const char *pData, int dataLen, int nWidth, int nHeight)
{
	IStream *pStm = NULL;

	//	BOOL bResult; 

	//����ȫ�ִ洢�ռ� 
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, dataLen ); 
	LPVOID pvData = NULL; 

	if (hGlobal == NULL) 
		return NULL; 

	if ((pvData = GlobalLock(hGlobal)) == NULL)//���������ڴ�� 
		return NULL; 

	memcpy(pvData,pData,dataLen);

	GlobalUnlock(hGlobal); 

	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm); 

	Bitmap * bitmap   = Bitmap::FromStream(pStm);
	CBitmap * pBitmap = NULL;

	if ( bitmap )
	{
		float cx = bitmap->GetWidth();
		float cy = bitmap->GetHeight();

		if ( nWidth==0 )
			nWidth  = cx;
		if ( nHeight==0 )
			nHeight = cy;

		CWnd * pWnd = CWnd::GetDesktopWindow();//AfxGetMainWnd();

		CDC *pDC = 0;

		try
		{
			pDC=pWnd->GetDC();
		}
		catch(...)
		{
			pDC=0;
			pWnd=NULL;
		}

		if(pDC)
		{
			pBitmap = new CBitmap();
			pBitmap->CreateCompatibleBitmap(pDC,nWidth,nHeight);

			CDC dcTemp;
			dcTemp.CreateCompatibleDC(pDC);

			CBitmap *pOldbmp = dcTemp.SelectObject(pBitmap);

			dcTemp.FillSolidRect(0,0,nWidth,nHeight,0xffffff);

			Graphics g(dcTemp.m_hDC);
			Unit  units;

			float d1 = cx/nWidth;
			float d2 = cy/nHeight;


			//		float d  = d1>d2? d1:d2;//����������
			float d  = d1<d2? d1:d2;

			cx=cx/d; cy=cy/d;

			POINT pt;

			pt.x = (nWidth-cx)/2;
			pt.y = (nHeight-cy)/2;

			RectF destRect(REAL(pt.x),	REAL(pt.y),	REAL(cx),	REAL(cy)),
				srcRect;
			bitmap->GetBounds(&srcRect, &units);
			g.DrawImage(bitmap, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);

			dcTemp.SelectObject(pOldbmp);
		}

		if ( pDC )
		{
			pWnd->ReleaseDC(pDC);
		}

		delete bitmap;
	}

	GlobalFree(hGlobal);

	return pBitmap;
}

///////////////////////////////////////////////////////////////////////////////////////////
//  HorizStretchBlt
//  ���ܣ�ˮƽ������������λͼ
//  ���룺pDC,�豸������ָ��
//        x,λͼ��DC�ϵ�left��ʼλ��
//        y,λͼ��DC�ϵ�top��ʼλ��
//        cx����DC����ʾλͼ�Ŀ��
//        pBitmap��ָ��һ��λͼ�����ָ��
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void HorizStretchBlt(CDC *pDC,int x, int y, int cx, CBitmap *pBitmap)
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);

	int nMuli = cx/bi.bmWidth;
	int nRema = cx%bi.bmWidth;

	CDC dcMem;
	dcMem.CreateCompatibleDC(pDC);
	CBitmap *pOldobj1 = dcMem.SelectObject(pBitmap);

	int left ;
	for ( int i=0; i<nMuli; i++ )
	{
		left = x+i*bi.bmWidth;
		pDC->BitBlt(left,y,bi.bmWidth,bi.bmHeight,&dcMem,0,0,SRCCOPY);
	}

	left = x+nMuli*bi.bmWidth;
	pDC->BitBlt(left,y,nRema,bi.bmHeight,&dcMem,0,0,SRCCOPY);

	dcMem.SelectObject(pOldobj1);
}


/////////////////////////////////////////////////////////////////////////////////////////
//  DrawBitmap
//  ���ܣ�����λͼ
//  ���룺pDC���豸������ָ��
//        x,λͼ��DC�ϵ�left��ʼλ��
//        y,λͼ��DC�ϵ�top��ʼλ��
//        clrTrans,͸��ɫ
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
void DrawBitmap(CDC *pDC,int x, int y, CBitmap *pBitmap, COLORREF clrTrans )
{
	BITMAP bi;
	pBitmap->GetBitmap(&bi);

	CDC dc;
	dc.CreateCompatibleDC(pDC);

	CBitmap *pOldbmp = dc.SelectObject(pBitmap);

	if ( clrTrans==0 )
		pDC->BitBlt(x,y,bi.bmWidth,bi.bmHeight,&dc,0,0,SRCCOPY);
	else
		TransparentBlt(pDC->m_hDC,x,y,bi.bmWidth,bi.bmHeight,dc.m_hDC,0,0,bi.bmWidth,bi.bmHeight,clrTrans);

	dc.SelectObject(pOldbmp);
}

/////////////////////////////////////////////////////////////////////////////////////////
//  DownLoadWebFile
//  ���ܣ����������ļ�
//  ���룺strUrl,�����ļ�URL
//        strLocalFile,����·���ļ�
//  ����ֵ��bool
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
bool DownLoadWebFile(CString strUrl, CString strLocalFile)
{
	HINTERNET hFile, hInet;
	char buffer[8192];

	bool bOk = false;

	DeleteFile(strLocalFile);

	DWORD dwRead, dwSize;

	hInet = InternetOpen("DownLoadWebFile",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, strUrl,NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT | 
			INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{	
			CStdioFile fileDownload;

			DWORD dwStatus     = 0;
			dwSize             = sizeof(buffer);

			if ( HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE, buffer, &dwSize ,NULL ) )
			{
				dwStatus = atoi(buffer);
			}

			if ( dwStatus==200 )
			{
				DWORD dwFileSize = 0;
				DWORD dwDownloadSize = 0;

				if ( HttpQueryInfo(hFile, HTTP_QUERY_CONTENT_LENGTH, buffer, &dwSize ,NULL ) )
				{
					dwFileSize = atol(buffer);
				}

				bool bOpened = fileDownload.Open(strLocalFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

				if ( bOpened )
				{
					dwSize             = sizeof(buffer);
					while ( InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
					{
						dwDownloadSize+=dwRead;

						if ( bOpened )
						{
							fileDownload.Write(buffer,dwRead);	
						}
					}

					bOk = true;

					fileDownload.Close();	

					if ( dwFileSize>0 && dwFileSize>dwDownloadSize )//û���������
					{
						bOk=false;
						DeleteFile(strLocalFile);
					}
				}	
			}

			InternetCloseHandle(hFile);
		}
		InternetCloseHandle(hInet);
	}

	return bOk;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  WideStrToStr
//  ���ܣ����ַ���ת��ΪAnsi�ַ���
//  ���룺
//  ����ֵ��CString
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString WideStrToStr(BSTR str)
{
	CComBSTR b=str;
	CString  r(b);
	return r;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  StrToWideStr
//  ���ܣ�Ansi�ַ�ת��Ϊ�����ַ���
//  ���룺
//  ����ֵ��BSTR,ע��Ҫ����SysFree
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
BSTR  StrToWideStr(CString str)
{
	CComBSTR b=str;
	BSTR     r=L"";
	b.CopyTo(&r);
	return r;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  UTF8_Convert
//  ���ܣ�UTF8�ַ����ӡ�����
//  ���룺str �����ַ���
//        bEncode, true,�������ַ�������UTF8���룬 false�������ַ�������UTF8����
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString  UTF8_Convert(CString  str, BOOL bEncode )  
{  
	int  len=str.GetLength();  
	int sourceCodepage, targetCodepage;
	if ( bEncode )
	{
		sourceCodepage = 936;
		targetCodepage = CP_UTF8;
	}
	else
	{
		sourceCodepage = CP_UTF8;
		targetCodepage = 936;
	}

	int  unicodeLen=MultiByteToWideChar(sourceCodepage,0,str,-1,NULL,0);  

	wchar_t            *  pUnicode;  
	pUnicode=new  wchar_t[unicodeLen+1];  

	memset(pUnicode,0,(unicodeLen+1)*sizeof(wchar_t));  

	MultiByteToWideChar(sourceCodepage,0,str,-1,(LPWSTR)pUnicode,unicodeLen);  

	int  targetLen=WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char  *)NULL,0,NULL,NULL);  

	BYTE  *  pTargetData =new  BYTE[targetLen+1];  
	memset(pTargetData,0,targetLen+1);  

	WideCharToMultiByte(targetCodepage,0,(LPWSTR)pUnicode,-1,(char  *)pTargetData,targetLen,NULL,NULL);  

	CString  rt;  
	rt.Format("%s",pTargetData);  

	delete  pUnicode;  
	delete  pTargetData;  

	return  rt;   
}  


/////////////////////////////////////////////////////////////////////////////////////////
//  CountSubStr
//  ���ܣ�����һ���ַ������ж��ٸ���ͬ�����ַ���
//  ���룺
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
int CountSubStr(CString str,CString substr)
{
	int iPos1=0;
	int n=0;
	int nsublen=substr.GetLength();

	while(1)
	{
		iPos1=str.Find(substr,iPos1);
		if ( iPos1>-1 )
		{
			iPos1+=nsublen;
			n++;
		}
		else
		{
			break;
		}
	}

	return n;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  ConvertSpecialCharToHtml
//  ���ܣ����ı��е������ַ�ת����hmtlת�Ʒ�
//  ���룺
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString ConvertSpecialCharToHtml(CString strText)
{	
	strText.Replace(" ","&nbsp;");
	strText.Replace("\"","&quot;");
	strText.Replace("\'","&apos;");
	strText.Replace("&","&amp;");
	strText.Replace("<","&lt;");
	strText.Replace(">","&gt;");

	return strText;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  TrimHtmlCode
//  ���ܣ�����HTML�����еı�ǡ��ű�
//  ���룺strHtml HTML����
//  ����ֵ�����˺���ı�
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString TrimHtmlCode(CString strHtml)
{
	CString str1 = strHtml;
	str1.MakeLower();

	int iPos1;
	int iPos2;
	//trim scripts

	while(1)
	{
		iPos1 = str1.Find("<script>",0);

		if ( iPos1>-1 )
		{
			iPos2 = str1.Find("</script>",iPos1+7);

			if ( iPos2>iPos1 )
			{
				CString str2, str3;

				str2 = str1.Left(iPos1);
				str3 = str1.Right(str1.GetLength()-iPos2-9);

				str1 = str2+str3;


				str2 = strHtml.Left(iPos1);
				str3 = strHtml.Right(strHtml.GetLength()-iPos2-9);

				strHtml = str2+str3;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	while(1)
	{
		iPos1 = str1.Find("<",0);

		if ( iPos1>-1 )
		{
			iPos2 = str1.Find(">",iPos1+1);

			if ( iPos2>iPos1 )
			{
				CString strSection = str1.Mid(iPos1+1,iPos2-iPos1-1);

				int n = CountSubStr(strSection,"\"");

				if ( (n%2)!=0 )
				{
					iPos2 = str1.Find(">",iPos2+1);
				}
			}

			if ( iPos2>iPos1 )
			{
				CString str2, str3;

				str2 = str1.Left(iPos1);
				str3 = str1.Right(str1.GetLength()-iPos2-1);

				str1 = str2+str3;


				str2 = strHtml.Left(iPos1);
				str3 = strHtml.Right(strHtml.GetLength()-iPos2-1);

				strHtml = str2+str3;
			}
			else
			{
				break;
			}
		}
		else
		{
			break;
		}
	}

	strHtml.Replace("&nbsp;"," ");
	strHtml.Replace("&quot;","\"");
	strHtml.Replace("&apos;","\'");
	strHtml.Replace("&amp;","&");
	strHtml.Replace("&lt;","<");
	strHtml.Replace("&gt;",">");

	return strHtml;
}


/////////////////////////////////////////////////////////////////////////////////////////
//  Base64Encode
//  ���ܣ����ַ�������Base64����
//  ���룺
//  ����ֵ��
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString Base64Encode( CString strInput)
{
	static char Base64Str[]="ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";
	int		L,I,J;
	byte	B1,B2,B3,B4;
	CString strTemp,strResult;

	L = strInput.GetLength();
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

			strResult=strResult+Base64Str[B1];
			strResult=strResult+Base64Str[B2];
			strResult=strResult+Base64Str[B3];
			strResult=strResult+Base64Str[B4];

			strTemp="";
		}
	}

	if ( J==1 )
	{
		B1=(byte)strTemp[0]>>2;
		B2=((byte)strTemp[0]&3)<<4; 

		strResult=strResult+Base64Str[B1];
		strResult=strResult+Base64Str[B2];
		strResult=strResult+"==";
	}

	if ( J==2 )
	{
		B1=(byte)strTemp[0]>>2;
		B2=(((byte)strTemp[0]&3)<<4)+((byte)strTemp[1]>>4);
		B3=((byte)strTemp[1]&0x0F)<<2; 

		strResult=strResult+Base64Str[B1];
		strResult=strResult+Base64Str[B2];
		strResult=strResult+Base64Str[B3];
		strResult=strResult+"=";
	}

	return strResult;
}

inline BYTE toHex(const BYTE &x)
{
	return x > 9 ? x + 55: x + 48;
}

/////////////////////////////////////////////////////////////////////////////////////////
//  UrlEncode
//  ���ܣ����ַ�������URL Encode����
//  ���룺strHtml HTML����
//  ����ֵ�����˺���ı�
//  �޸ļ�¼��
/////////////////////////////////////////////////////////////////////////////////////////
CString UrlEncode(CString strIn)
{
	CString strOut;

	strIn = UTF8_Convert(strIn,TRUE);

	const int nLen = strIn.GetLength() + 1;

	register LPBYTE pOutTmp = NULL;

	LPBYTE pOutBuf = NULL;

	register LPBYTE pInTmp = NULL;

	LPBYTE pInBuf =(LPBYTE)strIn.GetBuffer(nLen);

	BYTE b = 0;

	//alloc out buffer

	pOutBuf = (LPBYTE)strOut.GetBuffer(nLen*3 - 2);//new BYTE [nLen  * 3];

	if(pOutBuf)
	{

		pInTmp   = pInBuf;

		pOutTmp  = pOutBuf;
		// do encoding

		while (*pInTmp)
		{

			if(isalnum(*pInTmp))

				*pOutTmp++ = *pInTmp;

			else

				if(isspace(*pInTmp))

					*pOutTmp++ = '+';

				else
				{

					*pOutTmp++ = '%';

					*pOutTmp++ = toHex(*pInTmp>>4);

					*pOutTmp++ = toHex(*pInTmp%16);
				}

				pInTmp++;
		}

		*pOutTmp = '\0';

		//sOut=pOutBuf;

		//delete [] pOutBuf;

		strOut.ReleaseBuffer();

	}

	strIn.ReleaseBuffer();

	return strOut;

}
