/* -*- Mode: C; indent-tabs-mode: t; c-basic-offset: 4; tab-width: 4 -*- */

/******************************************************************
** 文件名:	string_t.h
** 版  权:	(C) 
** 创建人:	Tony
** 日  期:	2007-5-15   
** 描  述:	在STL string基础上扩展的string类

	
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __STRING_T_UNICODE_ANSI_H__
#define __STRING_T_UNICODE_ANSI_H__

#include "Config.h"
#include <stdarg.h>
#include <stdio.h>
#	include <mbstring.h>
#	include <tchar.h>

#include <wchar.h>
#include <string>
#include <algorithm>
#include <locale.h>
#include <sstream>
#include <strstream>
//#include "common/fcl_common.h"
using namespace std;

#ifdef _MSC_VER
#	pragma warning(push)
#	pragma warning(disable:4251)
#endif

BEGIN_NAMESPACE_FCL

 
#	define MAX_FORMAT_LEN	1024	//  最长能格式化的字符长度,这里以后优化,应该能动态计算出长度
#	define CHAR_SPACE		_T(' ')	//  调用trim函数默认处理的空格字符



/// MBS与WCS内码转换器

template<class _Elem>
struct converter
{
	static const char* to_char(const _Elem* _Str, size_t _Size, void*& _Buffer)
	{
		return _Str;
	}
	static const wchar_t* to_wchar(const _Elem* _Str, size_t _Size, void*& _Buffer)
	{
		static bool _init_locale = false;
		if (!_init_locale)
		{
			setlocale(LC_CTYPE, "");
			_init_locale = true;
		}

		if (_Buffer)
			delete [] _Buffer;
		size_t _Len = _Size + 1;
		wchar_t* _Buf = new wchar_t[_Len];
		size_t _Ret;
		if (_Buf == 0 || mbstowcs_s(&_Ret, _Buf, _Len, _Str, _Len) <= 0)
			return L"";
		_Buffer = _Buf;
		return _Buf;
	}
};

template<>
struct FCL_DECL converter<wchar_t>
{
	typedef wchar_t _Elem;
	static const char* to_char(const _Elem* _Str, size_t _Size, void*& _Buffer)
	{
		static bool _init_locale = false;
		if (!_init_locale)
		{
			setlocale(LC_CTYPE, "");
			_init_locale = true;
		}

		if (_Buffer)
			delete [] _Buffer;
		size_t _Len = _Size * MB_CUR_MAX + 1;
		char* _Buf = new char[_Len];
		size_t _Ret;
		if (_Buf == 0 || wcstombs_s(&_Ret, _Buf, _Len, _Str, _Len) != 0)
			return "";
		_Buffer = _Buf;
		return _Buf;
	}
	static const wchar_t* to_wchar(const _Elem* _Str, size_t _Size, void*& _Buffer)
	{
		return _Str;
	}
};


/// 字符串特征扩展

template<class _Elem>
struct char_traits_ex
{
	static _Elem __cdecl to_lower(const _Elem& _Ch)
	{
		return ::_mbctolower(_Ch);
	}
	static _Elem __cdecl to_upper(const _Elem& _Ch)
	{
		return ::_mbctoupper(_Ch);
	}
	static int __cdecl compare_nocase(const _Elem* _First1, const _Elem* _First2)
	{
		return ::_mbsicmp((const unsigned char*)_First1, (const unsigned char*)_First2);
	}
	static int __cdecl vsnprintf(_Elem* _First, size_t _Size, const _Elem* _Format, va_list _List)
	{
		return ::_vsnprintf_s(_First, _Size,_TRUNCATE, _Format, _List);
	}
	static void __cdecl reverse(_Elem* _First)
	{
		::_mbsrev((unsigned char*)_First);
	}
	static int __cdecl to_int(const _Elem* _First)
	{
		return ::atoi(_First);
	}
	static long __cdecl to_long(const _Elem* _First)
	{
		return ::atol(_First);
	}
	static int64 __cdecl to_int64(const _Elem* _First)
	{
		return ::_atoi64(_First);
	}
	static double __cdecl to_double(const _Elem* _First)
	{
		return ::atof(_First);
	}
	static void __cdecl from_int(int _Val, _Elem* _First, int _Radix)
	{
		::_itoa(_Val, _First, _Radix);
	}
	static void __cdecl from_long(long _Val, _Elem* _First, int _Radix)
	{
		::_ltoa(_Val, _First, _Radix);
	}
	static void __cdecl from_int64(int64 _Val, _Elem* _First, int _Radix)
	{
		::_i64toa(_Val, _First, _Radix);
	}
	static void __cdecl from_double(std::basic_string<_Elem>& _Str, double _Val, 
		int _Precision, int _Width, _Elem _Fill, std::ios::fmtflags _Flags)
	{
		std::stringstream _Stream;
		_Stream.precision(_Precision);
		_Stream.width(_Width);
		_Stream.fill(_Fill);
		if (_Flags)
			_Stream.setf(_Flags);
		_Stream << _Val;
		_Str = _Stream.str();
	}
};
template<>
struct FCL_DECL char_traits_ex<wchar_t>
{
	typedef wchar_t	_Elem;
	static _Elem __cdecl to_lower(const _Elem& _Ch)
	{
		return towlower(_Ch);
	}
	static _Elem __cdecl to_upper(const _Elem& _Ch)
	{
		return towupper(_Ch);
	}
	static int __cdecl compare_nocase(const _Elem* _First1, const _Elem* _First2)
	{
		return ::_wcsicmp(_First1, _First2);
	}
	static int __cdecl vsnprintf(_Elem* _First, size_t _Size, const _Elem* _Format, va_list _List)
	{
		return ::_vsnwprintf_s(_First, _Size,_TRUNCATE, _Format, _List);
	}
	static void __cdecl reverse(_Elem* _First)
	{
		::_wcsrev(_First);
	}
	static int __cdecl to_int(const _Elem* _First)
	{
		return ::_wtoi(_First);
	}
	static long __cdecl to_long(const _Elem* _First)
	{
		return ::_wtol(_First);
	}
	static int64 __cdecl to_int64(const _Elem* _First)
	{
		return ::_wtoi64(_First);
	}
	static double __cdecl to_double(const _Elem* _First)
	{
		return ::_wtof(_First);
	}
	static void __cdecl from_int(int _Val, _Elem* _First, int _Radix)
	{
		::_itow(_Val, _First, _Radix);
	}
	static void __cdecl from_long(long _Val, _Elem* _First, int _Radix)
	{
		::_ltow(_Val, _First, _Radix);
	}
	static void __cdecl from_int64(int64 _Val, _Elem* _First, int _Radix)
	{
		::_i64tow(_Val, _First, _Radix);
	}
	static void __cdecl from_double(std::basic_string<_Elem>& _Str, double _Val, 
		int _Precision, int _Width, _Elem _Fill, std::ios::fmtflags _Flags)
	{
		std::wstringstream _Stream;
		_Stream.precision(_Precision);
		_Stream.width(_Width);
		_Stream.fill(_Fill);
		if (_Flags)
			_Stream.setf(_Flags);
		_Stream << _Val;
		_Str = _Stream.str();
	}
};



/*
 *	功能描述:
 *  1.方便UNICODE编程，方便的转换过程
 *  2.以stl的string为模版,增强其中对字符串的处理功能

 */
template<class _Elem >
class powerful_string : public basic_string<_Elem>
{
public:
	typedef typename basic_string<_Elem>::size_type size_type;
	
private:
	void* _Buffer;	/// 用于记录编码转换后的地址

public:
	typedef powerful_string<_Elem>		_MyType;
	typedef basic_string<_Elem> 		_MyBase;


	// 构造（析构）

public:
	powerful_string() : _MyBase(), _Buffer(0)
	{
	}
	powerful_string(const _MyType& _Right) : _MyBase(_Right.c_str()), _Buffer(0)
	{
	}
	
	powerful_string(const _MyType& _Right, size_type _Roff, size_type _Count = basic_string<_Elem>::npos) : _MyBase(_Right.c_str(), _Roff, _Count), _Buffer(0)
	{
	}
	powerful_string(const _MyBase& _Right) : _MyBase(_Right), _Buffer(0)
	{
	}
	powerful_string(const _MyBase& _Right, size_type _Roff, size_type _Count = basic_string<_Elem>::npos) : _MyBase(_Right, _Roff, _Count), _Buffer(0)
	{
	}
	powerful_string(const _Elem* _Ptr) : _MyBase(_Ptr), _Buffer(0)
	{
	}
	powerful_string(const _Elem* _Ptr, size_type _Count) : _MyBase(_Ptr, _Count), _Buffer(0)
	{
	}
	powerful_string(const _Elem* _Ptr, size_type _Roff, size_type _Count) : _MyBase(_Ptr, _Roff, _Count), _Buffer(0)
	{
	}
	powerful_string(_Elem _Ch) : _MyBase(1, _Ch), _Buffer(0)
	{
	}
	powerful_string(size_type _Count, _Elem _Ch) : _MyBase(_Count, _Ch), _Buffer(0)
	{
	}

	~powerful_string()
	{
		if (_Buffer)
		{
			delete[] _Buffer;
			_Buffer = 0;
		}
	}



	// 操作
public:
	_MyType& operator=(const _MyType& _Right)
	{	// assign _Right
		assign(_Right);
		return *this;
	}

	_MyType& operator=(const _MyBase& _Right)
	{	// assign _Right
		assign(_Right);
		return *this;
	}

	_MyType& operator=(const _Elem* _Ptr)
	{	// assign [_Ptr, <null>)
		assign(_Ptr);
		return *this;
	}

	_MyType& operator=(_Elem _Ch)
	{	// assign 1 * _Ch
		assign(1, _Ch);
		return *this;
	}

	_MyType& operator+=(const _MyType& _Right)
	{	// append _Right
		append(_Right.c_str());
		return *this;
	}

	_MyType& operator+=(const _MyBase& _Right)
	{	// append _Right
		append(_Right.c_str());
		return *this;
	}

	_MyType& operator+=(const _Elem *_Ptr)
	{	// append [_Ptr, <null>)
		append(_Ptr);
		return *this;
	}

	_MyType& operator+=(_Elem _Ch)
	{	// append 1 * _Ch
		append((size_type)1, _Ch);
		return *this;
	}



	// 转换(MBS和WCS间编码转换)
public:
	const char* to_char()
	{
		return converter<_Elem>::to_char(this->c_str(), this->length(), _Buffer);
	}
	const wchar_t* to_wchar()
	{
		return converter<_Elem>::to_wchar(this->c_str(), this->length(), _Buffer);
	}




	// 一些便利的方法
public:
	/// 转换成小写

	void to_lower()
	{
		std::transform( this->begin(), this->end(), this->begin(), char_traits_ex<_Elem>::to_lower);
	}

	/// 转换成大写

	void to_upper()
	{
		std::transform( this->begin(), this->end(), this->begin(), char_traits_ex<_Elem>::to_upper);
	}

	/// 格式化一个字符串
	void format(const _Elem* _Format, ...)
	{
		va_list _Args;
		va_start(_Args, _Format);
		const size_type _Size = MAX_FORMAT_LEN;
		_Elem _Tempbuf[_Size];
		size_type _Ret =char_traits_ex<_Elem>::vsnprintf(_Tempbuf, _Size - sizeof(_Elem), _Format, _Args);
		if (_Ret == -1 || _Ret == _Size - sizeof(_Elem))
		{
			_Tempbuf[_Size - sizeof(_Elem)] = '\0';
		}
		va_end(_Args);
		assign(_Tempbuf);
	}

	/// 忽略大小写的字符串比较

	int compare_nocase(const _MyType& _Right) const
	{
		return char_traits_ex<_Elem>::compare_nocase(this->c_str(), _Right.c_str());
	}

	/// 左端裁剪一个字符

	void trim_left(_Elem _Space = CHAR_SPACE)
	{
		_Elem _S[2] = {_Space, 0};
		trim_left(_S);
	}

	/// 左端裁剪一个字符集合

	void trim_left(const _Elem* _Charset)
	{
		if (this->empty() || !_Charset || *_Charset == 0)
			return;

		size_type _Pos = this->npos;
		size_type _Spacelen = char_traits<_Elem>::length(_Charset);
		while (this->c_str()[++_Pos])
		{
			size_type _Inpos = this->npos;
			while (_Charset[++_Inpos] && _Charset[_Inpos] != this->c_str()[_Pos]);
			if (_Inpos >= _Spacelen) // not found
				break;
		}

		if (_Pos > 0)
			this->erase(0, _Pos);
	}

	/// 右端裁剪一个字符

	void trim_right(_Elem _Space = CHAR_SPACE)
	{
		_Elem _S[2] = {_Space, 0};
		trim_right(_S);
	}

	/// 右端裁剪一个字符集合

	void trim_right(const _Elem* _Charset)
	{
		if (this->empty() || !_Charset || *_Charset == 0)
			return;

		size_type _Pos = this->length();
		size_type _Spacelen = char_traits<_Elem>::length(_Charset);
		while (--_Pos >= 0)
		{
			size_type _Inpos = this->npos;
			while (_Charset[++_Inpos] && _Charset[_Inpos] != this->c_str()[_Pos]);
			if (_Inpos >= _Spacelen) // not found
				break;
		}

		if (_Pos < this->length() - 1)
			this->erase(_Pos + 1);
	}

	/// 裁剪一个字符

	void trim(_Elem _Space = CHAR_SPACE)
	{
		trim_left(_Space);
		trim_right(_Space);
	}

	/// 裁剪一个字符集合

	void trim(const _Elem* _Charset)
	{
		trim_left(_Charset);
		trim_right(_Charset);
	}

	/// 取左边的子串
	_MyType left(size_type _Count) const
	{
		return substr(0, min(_Count, this->length()));
	}

	/// 取右边的子串
	_MyType right(size_type _Count) const
	{
		if (_Count > this->length())
			_Count = this->length();

		return substr(this->length() - _Count, _Count);
	}

	/// 取中间的子串
	_MyType mid(size_type _First) const
	{
		if (_First > this->length())
			_First = this->length();
		return substr(_First, this->length() - _First);
	}

	/// 取中间的子串
	_MyType mid(size_type _First, size_type _Count) const
	{
		if (_First > this->length())
			_First = this->length();
		if (_First + _Count > this->length())
			_Count = this->length() - _First;
		return this->substr(_First, _Count);
	}

	/// 返回某个字符左边的串
	_MyType left_ch(const _Elem _Ch) const
	{
		size_type _Pos = find_first_of(_Ch);
		if (_Pos == this->npos)
			return _MyType();
		return this->substr(0, _Pos);
	}

	/// 返回某个字符右边的串
	_MyType right_ch(const _Elem _Ch) const
	{
		size_type _Pos = find_last_of(_Ch);
		if (_Pos == this->npos)
			return _MyType();
		return substr(_Pos + 1, this->length() - _Pos - 1);
	}

	/// 前后顺序颠倒

	void reverse()
	{
		char_traits_ex<_Elem>::reverse((_Elem*)this->c_str());
	}

	/// 替换字符
	void replace(_Elem _Old, _Elem _New)
	{
		size_type _Pos = this->npos;
		while (this->c_str()[++_Pos])
		{
			if (this->c_str()[_Pos] == _Old)
				((_Elem*)(this->c_str()))[_Pos] = _New;
		}
	}

	/// 替换字符串

	void replace(const _MyType& _Old, const _MyType& _New)
	{
		size_type _Oldlen = _Old.length();
		if (_Oldlen == 0)
			return;

		size_type _Newlen = _New.length();
		size_type _Pos = this->npos;
		_Pos = find(_Old.c_str(), _Pos + 1);
		while (_Pos != this->npos)
		{
			_MyBase::replace(_Pos, _Oldlen, _New.c_str(), _Newlen);
			_Pos += _Newlen - _Oldlen;
			_Pos = find(_Old.c_str(), _Pos + 1);
		}
	}

	/// 获取字符串所占缓冲大小

	size_type getBufferSize() const
	{
		return sizeof(_Elem) * this->length();
	}

	/// 将字符串按分割符(如逗号)分割成字符串数组，成功返回分割后的字符串个数，否则返回0
	template<class StrArray>
	size_type split(const _MyType& _Sep, StrArray& _RetStrArray)
	{
		size_type _Count = 0;
		size_type _Seplen = _Sep.length();
		if (_Seplen == 0)
			return _Count;

		size_type _Pos = 0;
		size_type _Lastpos = 0;
		_Pos = find(_Sep.c_str(), _Pos);
		while (_Pos != this->npos)
		{
			++_Count;
			_RetStrArray.push_back(_MyType(*this, _Lastpos, _Pos - _Lastpos));
			_Pos += _Seplen;
			_Lastpos = _Pos;
			_Pos = find(_Sep.c_str(), _Pos);
		}

		if (_Count > 0 && _Lastpos < this->length())
		{
			++_Count;
			_RetStrArray.push_back(_MyType(*this, _Lastpos, _Pos - _Lastpos));
		}

		return _Count;
	}
	
	

// 一些转换函数

public:
	int to_int() const
	{
		return char_traits_ex<_Elem>::to_int(this->c_str());
	}

	long to_long() const
	{
		return char_traits_ex<_Elem>::to_long(this->c_str());
	}

	int64 to_int64() const
	{
		return char_traits_ex<_Elem>::to_int64(this->c_str());
	}

	double to_double() const
	{
		return char_traits_ex<_Elem>::to_double(this->c_str());
	}

	void from_int(int _Val, int _Radix = 10)
	{
		#define INT_MAX_LENGTH 40
		this->reserve(INT_MAX_LENGTH);
		char_traits_ex<_Elem>::from_int(_Val,(_Elem*)this->c_str(), _Radix);
	}

	void from_long(long _Val, int _Radix = 10)
	{
		#define LONG_MAX_LENGTH 40
		this->reserve(INT_MAX_LENGTH);
		char_traits_ex<_Elem>::from_long(_Val, (_Elem*)this->c_str(), _Radix);
	}

	void from_int64(int64 _Val, int _Radix = 10)
	{
		#define INT64_MAX_LENGTH 80
		this->reserve(INT64_MAX_LENGTH);
		char_traits_ex<_Elem>::from_int64(_Val, (_Elem*)this->c_str(), _Radix);
	}

	void from_double(double _Val, int _Precision = 6, int _Width = 0, _Elem _Fill = ' ', std::ios::fmtflags _Flags = std::ios::fmtflags(0))
	{
		char_traits_ex<_Elem>::from_double(*this, _Val, _Precision, _Width, _Fill, _Flags);
	}
};



typedef	 powerful_string<char>			string_t;
typedef  powerful_string<char>			string_a;
typedef  powerful_string<wchar_t>		string_w;
typedef  string_t						String;

template struct FCL_DECL char_traits_ex<char>;
template struct FCL_DECL converter<char>;
template class FCL_DECL powerful_string<char>;
template class FCL_DECL powerful_string<wchar_t>;

END_NAMESPACE

/********************** 常用转换宏 ***********************************/
#ifndef WtoA
#	define WtoA(wchar_star)				FCL_NAMESPACE::string_w(wchar_star).to_char()
#	define StringWtoA(w_string)			(w_string).to_char()
#endif
#ifndef AtoW
#	define AtoW(char_star)				FCL_NAMESPACE::string_a(char_star).to_wchar()
#	define StringAtoW(a_string)			(a_string).to_wchar()
#endif
#ifndef StringAtoA
#	define StringAtoA(a_string)			(a_string).c_str()
#	define StringWtoW(w_string)			(w_string).c_str()
#endif

#ifndef AtoT
#	ifdef _UNICODE
#		define AtoT			AtoW
#		define StringAtoT	StringAtoW
#		define WtoT
#		define StringWtoT	StringWtoW
#		define TtoA			WtoA
#		define StringTtoA	StringWtoA
#		define TtoW
#		define StringTtoW	StringWtoW
#	else
#		define AtoT
#		define StringAtoT	StringAtoA
#		define WtoT			WtoA
#		define StringWtoT	StringWtoA
#		define TtoA
#		define StringTtoA	StringAtoA
#		define TtoW			AtoW
#		define StringTtoW	StringAtoW
#	endif
#endif


////////////////////////////////////////////////////////////////////////////////
// 字符串哈希函数,你可以改写

namespace stdext
{
	inline size_t hash_value(const fcl::String& _Str)
	{
		#define _HASH_SEED	(size_t)0xdeadbeef
		size_t _Val = _HASH_SEED;
		uint32 _Size = (uint32)_Str.size();
		if (0 < _Size)
			{	// add one or more elements
			uint32 _Stride = (_Size / 16) + 1;
			_Size -= _Stride;	// protect against _Size near _Str.max_size()
			for(uint32 _Idx = 0; _Idx <= _Size; _Idx += _Stride)
				_Val += (size_t)_Str[_Idx];
			}
		return (_Val);
	};
}
		
#ifdef _MSC_VER
#	pragma warning(pop)
#endif

#endif // __STRING_T_UNICODE_ANSI_H__
