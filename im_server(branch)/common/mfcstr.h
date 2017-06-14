
#ifndef MFCSTR_H
#define MFCSTR_H
 

#include <iostream>
#include <list>
#include <deque>
#include <map>
#include <numeric>     //accumulate
#include <set>
#include <stack>
#include <vector>
#include <algorithm>
#include <functional>
#include <string>
#include <string.h>
#include <fstream>
#include <memory>
#include <queue>
#include <complex>
//#include <sstream>
#include <cctype>   //isalpha()
#include <new>
#include <cstdarg>  //number not knowing arguments.such as :pringf
#include <utility>
#include <cstdio>
#include <cstdlib>
#include <cassert>

#define _min(a,b) ((a)<(b)?(a):(b))
#define _max(a,b) ((a)>(b)?(a):(b))
#define bzero(ptr,n) memset(ptr,0,n)
//////////以下是一个string的包装类/////////////////////
class CString  
{	
public:
	std::string m_str;
	int Delete( int nIndex, int nCount = 1 )
	{
		m_str.erase(nIndex,nCount);
		return m_str.length();
	}
	int Insert( int nIndex, const char * pstr )
	{
		m_str.insert(nIndex,pstr);
		return m_str.length();	
	}
	int Insert( int nIndex, char ch )
	{	
		CString strTmp(ch);
		m_str.insert(nIndex,strTmp);
		strTmp.Empty();
		return m_str.length();
	}
	int Remove( char ch )
	{
		std::string::iterator iter;
		int count = 0;
		for(iter = m_str.begin(); iter != m_str.end();)
		{
			if(*iter == ch)
			{
				iter = m_str.erase(iter);count++;
				continue;
			}
			iter ++;
		}
		return count;
	}
	void MakeReverse( )
	{
		std::string strTmp;
		std::string::iterator iter;
		iter=m_str.end();
		iter--;
		for(; iter != m_str.begin(); iter--)
		{
			strTmp += *iter;
		}
		strTmp += *iter;
		m_str = strTmp;
	}
	int Find( char ch ) const
	{
		return m_str.find(ch);
	}
	int Find( const char * lpszSub ) const
	{
		return m_str.find(lpszSub);
	}
	int Find( char ch, int nStart ) const
	{
		return m_str.find(ch,nStart);
	}
	int Find( const char * pstr, int nStart ) const
	{
		return m_str.find(pstr,nStart);
	}
	int ReverseFind( char ch ) const
	{
		return m_str.find_last_of(ch);	
	}
	int FindOneOf( const char * lpszCharSet ) const
	{
		return m_str.find_first_of(lpszCharSet);
	}
	int Format(const char* pstrFormat, ... )
	{///本函数仅仅支持ANSI标准字符集 '%[flags] [width] [.precision] [{h | l | I64 | L}]type'	 	
		assert(pstrFormat!=NULL);
		va_list argList;
		va_start(argList,pstrFormat); 
		int nMaxLen = 0;
		for (const char * p = pstrFormat; *p != '\0';p++ )
		{
			if (*p != '%' || *(++p) == '%')
			{// 如果不是'%'就直接累计长度，如果是'%%'也使长度加1
				nMaxLen += 1;
				continue;
			}
			int nItemLen = 0; //用来保存每个参数的长度
			int nWidth = 0; //用来保存每一项的宽度
			for (; *p != '\0'; p ++)
			{
				if (*p == '#')
					nMaxLen += 2;   // 处理 '0x'
				else if (*p == '*')
					nWidth = va_arg(argList, int);  //如：'%5f' 中的5
				else if (*p == '-' || *p == '+' || *p == '0'|| *p == ' ')
					;  //忽略该符号
				else // 不是标志字符就退出循环
					break;
			}
			if (nWidth == 0)
			{ //提取宽度
				nWidth = atoi(p);
				for (; *p != '\0' && isdigit(*p); p ++)
					;
			}
			assert(nWidth >= 0);//有效宽度
			int nPrecision = 0; //精度位数
			if (*p == '.')
			{
				p++;// 跳过 '.'字符 (宽度.精度)			
				if (*p == '*')
				{ //有参数给出
					nPrecision = va_arg(argList, int);
					p ++;// 取得精度，跳过字符
				}
				else
				{ //在格式串中有宽度
					nPrecision = atoi(p);
					for (; *p != '\0' && isdigit(*p);	p ++)
						;
				}
				assert(nPrecision >= 0);//有效宽度
			}
			switch (*p)
			{
				case 'h':     //short int 型
					p ++;
					break;
				case 'l':	 //long double 型
					p ++;
					break;
				case 'F':	 //近指针
				case 'N':	//远指针	
				case 'L':	//long double 型
					p++;
					break;
			}
			switch (*p)
			{
				case 'c':   //// 单个字符
				case 'C':
					nItemLen = 2;
					va_arg(argList, int);
					break;
				case 's':	//// 字符串
				case 'S':
					nItemLen = strlen(va_arg(argList, const char*));
					nItemLen = ((1) > (nItemLen)) ? (1) : (nItemLen);//如果是空串就使用1 即保存'\0'
					break;
			}
			if (nItemLen != 0)		
			{
				nItemLen = ((nItemLen) > (nWidth)) ? (nItemLen) : (nWidth);//使用大者
				if (nPrecision != 0)
					nItemLen = ((nItemLen) < (nPrecision)) ? (nItemLen) : (nPrecision);
			}
			else
			{
				switch (*p)
				{
					case 'd':    //整数的处理
					case 'i':
					case 'u':
					case 'x':
					case 'X':
					case 'o':
						va_arg(argList, int);
						nItemLen = 32;  //四字节
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者
						break;
					case 'e':	//浮点数
					case 'f':
					case 'g':
					case 'G':
						va_arg(argList, double);
						nItemLen = 32;//四字节
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
						break;
					case 'p':	//指针
						va_arg(argList, void*);
						nItemLen = 32;
						nItemLen = ((nItemLen) > (nWidth+nPrecision)) ? (nItemLen) : (nWidth+nPrecision);//使用大者;
						break;
					case 'n':
						va_arg(argList, int*); //指向整数的指针,见BorlanderC++3.1库函数P352
						break;
					default:
						assert(false);  //不能处理的格式，给出警告
					}
				}
				nMaxLen += nItemLen;//把该项的长度累计
			}
			va_end(argList);
			va_start(argList, pstrFormat);  // 重新开始提取参数
			char* ch = new char[nMaxLen+1]; //分配内存
			vsprintf(ch, pstrFormat, argList);
			//assert(vsprintf(ch, pstrFormat, argList) <= nMaxLen);
			m_str.append(ch); //加到string的尾部
			delete[] ch; //释放内存
			va_end(argList);
			return nMaxLen;
	}
	int GetLength() const
	{
		return m_str.length();
	}
	CString Left(int nCount) const
	{
		if (nCount <=0) 
			return CString("");	
		CString strTmp;
		strTmp = m_str.substr(0,nCount);
		return strTmp;
	}
	CString Right(int nCount) const
	{
		if (nCount <=0) 
			return CString("");
		CString strTmp;
		if (nCount > GetLength())
			strTmp = m_str.substr(0);
		else
			strTmp = m_str.substr(GetLength()-nCount);
		return strTmp;
	}
	CString Mid(int nFirst) const
	{
		CString strTmp;
		if (nFirst >= GetLength())
			return CString("");
		if (nFirst <= 0)
			strTmp = m_str.substr(0);
		else
			strTmp = m_str.substr(nFirst);
		return strTmp;		
	}
	CString Mid( int nFirst, int nCount) const
	{
		if (nCount <= 0) 
			return CString("");
		if (nFirst >= GetLength())
			return CString("");
		CString strTmp;
		if (nFirst <= 0)
			strTmp = m_str.substr(0,nCount);
		else
			strTmp = m_str.substr(nFirst,nCount);
		return strTmp;				
	}

	CString& operator+=(const char* pszSrc)
	{
		m_str += pszSrc;
		return *this;
	}

	CString& operator+=(const char cChar)
	{
		m_str += cChar;
		return *this;
	}

	CString& operator=(const std::string str)
	{
		if (m_str.compare(str) == 0) return *this;
		m_str.assign(str);
		return *this;
	}
	CString& operator=(char ch)
	{
		m_str = ch;
		return *this;
	}
	CString& operator =( const char * lpsz )
	{
		Empty();
		m_str.append(lpsz);
		return *this;
	}
	CString& operator =(  char * lpsz )
	{
		Empty();
		m_str.append(lpsz);
		return *this;
	}
	void Append(const CString& strSrc )
	{
		m_str.append(strSrc.m_str);
	}
	void Append(const char* pszSrc,int nLength)
	{
		m_str.append(pszSrc,nLength);
	}
	void Append(const char* pszSrc)
	{
		m_str.append(pszSrc);
	}
	void MakeUpper()
	{
		std::transform(m_str.begin (),
			m_str.end (),m_str.begin (),
			toupper);
	}
	void MakeLower()
	{
		std::transform(m_str.begin (),
			m_str.end (),m_str.begin (),
			tolower);
	}
	bool IsEmpty( ) const
	{
		return m_str.empty(); 
	}
	void Empty( )
	{//清除
		m_str.erase(m_str.begin(),m_str.end());
	}
	char GetAt( int nIndex ) const
	{
		return m_str.at(nIndex);
	}
	char operator []( int nIndex ) const
	{
		return m_str.at(nIndex);
	}
	void SetAt( int nIndex, char ch )
	{
		m_str.at(nIndex) = ch;
	}
	operator const char * ( ) const
	{
		return m_str.c_str();
	}

	friend CString operator + (const CString& string1, const CString& string2)
	{
		CString str;
		str.Append(string1);
		str.Append(string2);
		return str;
	}
	friend CString operator + ( const CString& string1, char ch )
	{
		CString str;
		str.Append(string1);
		str.Append(ch);
		return str;
	}
	friend CString operator + ( const CString& string1, char* ch )
	{
		CString str;
		str.Append(string1);
		str.Append(ch);
		return str;
	}

	friend bool operator==( const CString& str1, char ch2)
	{ 
		if( str1.m_str.length() == 1 && str1.m_str[0] == ch2 )
		{
			return true;
		}
		return false;
	}
	
	friend bool operator==(char ch1,const CString& str2)
	{
		if( str2.m_str.length() == 1 && str2.m_str[0] == ch1 )
		{
			return true;
		}
		return false;
	}
	
	friend bool operator==(const CString& str1,const CString& str2) 
	{
		return str1.m_str == str2.m_str;
	}
	friend bool operator==(const CString& str1,const char* psz2) 
	{
		return str1.m_str == psz2;
	}
	friend bool operator==(const char* psz1,const CString& str2)
	{
		return str2.m_str == psz1;
	}
	friend bool operator==(const CString& str1,char* psz2)
	{
		return str1.m_str == psz2;
	}
	friend bool operator==(char* psz1,const CString& str2	)
	{
		return str2.m_str == psz1;
	}

	friend bool operator!=( const CString& str1, char ch2)
	{ 
		if( str1.m_str.length() != 1 )
		{
			return true;
		}
		return str1.m_str[0] != ch2;
	}
	friend bool operator!=(char ch1,const CString& str2)
	{
		if( str2.m_str.length() != 1 )
		{
			return true;
		}
		return str2.m_str[0] != ch1;
	}
	friend bool operator!=(const CString& str1,const CString& str2) 
	{
		return str1.m_str != str2.m_str;
	}
	friend bool operator!=(const CString& str1,const char* psz2) 
	{
		return str1.m_str != psz2;
	}
	friend bool operator!=(const char* psz1,const CString& str2)
	{
		return str2.m_str != psz1;
	}
	friend bool operator!=(const CString& str1,char* psz2)
	{
		return str1.m_str != psz2;
	}
	friend bool operator!=(char* psz1,const CString& str2	)
	{
		return str2.m_str != psz1;
	}
		
	int Compare( const char * lpsz ) const
	{
		CString str;
		str.Append(lpsz);
		return m_str.compare(str);
	}
	int Compare( const CString& string1 ) const
	{
		return m_str.compare(string1);
	}
	int CompareNoCase( const char * lpsz ) const
	{
		CString str,strThis;
		str.Append(lpsz);
		strThis = (*this);
		str.MakeLower();
		strThis.MakeLower();
		return strThis.m_str.compare(str);
	}
	int CompareNoCase( const CString& string1 ) const
	{
		CString str,strThis;
		str = string1;
		strThis = (*this);
		str.MakeLower();
		strThis.MakeLower();
		return strThis.m_str.compare(str);
	}
	CString& Trim()
	{
		if( m_str.length() )
		{
			TrimLeft();
			TrimRight();			
		}

		return *this;
	}
	void TrimRight( )
	{
		if (m_str.empty()) 
		{  
        	return;  
  		}  
  
    	std::string::iterator c;  
    	// Erase whitespace after the string  
    	for (c = m_str.end(); c != m_str.begin() && iswspace(*--c);); m_str.erase(++c, m_str.end());  
	}
	void TrimLeft( )
	{   
		if (m_str.empty()) 
		{  
        	return;  
  		}  
  
    	std::string::iterator c;  
    	// Erase whitespace before the string  ' ' \r \n \t
    	for (c = m_str.begin(); c != m_str.end() && iswspace(*c++);); m_str.erase(m_str.begin(), --c);  
	}
	void TrimLeft( char chTarget )
	{
		std::string::size_type pos;
		pos = m_str.find_first_not_of(chTarget);
		if (pos == 0) return; 
		m_str.erase(m_str.begin(),m_str.begin()+pos);		
	}
	void TrimRight( char chTarget )
	{
		std::string::size_type pos;
		pos = m_str.find_last_not_of(chTarget);
		++pos;
		if (pos == m_str.length())
			return; 
		m_str.erase(m_str.begin()+pos,m_str.end());		
	}
	void Replace( char chOld, char chNew )
	{
		for(unsigned int i=0;i<m_str.length();i++)
		{
			if (m_str.at(i) == chOld)
				m_str.at(i) = chNew;
		}		
	}
	void Replace(const char* chOld,const char* chNew )
	{
		
		int nOldLength = strlen(chOld);
		int nNewLength = strlen(chNew);
		//unsigned int index = m_str.find(chOld);
		size_t index = m_str.find(chOld);
		while (index != std::string::npos)
		{
			m_str.erase(index,nOldLength);
			m_str.insert(index,chNew);
			index = m_str.find(chOld,index + nNewLength);
		}
	}		
	char * GetBuffer( int nMinBufLength )
	{
		m_str.resize(nMinBufLength);
		return (char*)m_str.c_str();
	}
	void ReleaseBuffer( int nNewLength = -1 )
	{
		TrimRight('\0');
	}
	CString(const CString& string1)
	{
		m_str.append(string1);
	}
	CString(const char *ch)
	{
		m_str.append(ch);
	}
	CString(const char ch)
	{
		m_str = ch;
	}
	CString()
	{}
};
#endif

