/******************************************************************
** 文件名:	Path.h
** 版  权:	(C)  
** 创建人:	T
** 日  期:	2007-5-15   
** 描  述:
**************************** 修改记录 ******************************
** 修改人: 
** 日  期: 
** 描  述: 
********************************************************************/

#ifndef __PATH_H__
#define __PATH_H__

#ifdef  _MSC_VER
#pragma warning(disable: 4251)
#endif

#include "Config.h"
#include ".\string\string_t.h"

BEGIN_NAMESPACE_FCL


#define  CHAR_SLASH			'/'
#define  CHAR_DOT			'.'
#define  CHAR_SLASH2		'\\'
#define STR_LEN 255


FCL_DECL const char* getWorkDir();
FCL_DECL const char* resetWorkDir(const char* newDir);


template <class _Elem>
class basic_path : public powerful_string<_Elem>
{
public:
	typedef powerful_string<_Elem>		_String;
	typedef basic_path<_Elem>			_MyType;
	typedef typename powerful_string<_Elem>::size_type size_type;

	///////////////////////////////////////////////////////////////////////////////
	// 构造函数
	basic_path()
		: _isFile(true)
	{
	}
	basic_path(const _Elem* p, bool isFilePath = true)
		: _isFile(isFilePath), _String(p)
	{
		normalize();
	}
	basic_path(const _Elem* p, size_type roff, size_type count, bool isFilePath = true) 
		: _isFile(isFilePath), _String(p,roff,count)
	{
		normalize();
	}
	basic_path(_String& str, bool isFilePath = true)
		: _isFile(isFilePath), _String(str)
	{
		normalize();
	}
	basic_path(_MyType & path) 
		: _isFile(path._isFile), _String(path)
	{

	}
	basic_path(_MyType & path, bool isFilePath) 
		: _isFile(isFilePath), _String(path)
	{

	}
	_MyType& operator+(_Elem _Ch)
	{
		_String::assign(1, _Ch);
		return (*this);
	}

	_MyType& operator+(const _Elem* str)
	{
		_String::assign(str);
		return (*this);
	}
	/*
	~basic_path()
	{
		outputStringA("%s\n", (CPAChar)(*this));
	}*/
/*
	operator _String()
	{
		return (*this);
	}*/


	/** 标准化路径:
	 * 工序: 
	 *		1.修剪两边的空格
	 *		2.把['/']统一替换成['\']
	 */
	void normalize()
	{
		this->trim();

		_Elem slash[2] = {CHAR_SLASH};
		_Elem slash2[2] = {CHAR_SLASH2};
		replace(slash2, slash);
	}

	_String getFileName() const
	{
		if (!isFile())
			return _String();

		size_type pos = getLastSlashPos();

		return substr(pos + 1, this->length() - pos - 1);
	}

	_String getFileTitle() const
	{
		if (!isFile())
			return _String();

		size_type slashPos = getLastSlashPos();
		size_type dotPos = getLastDotPos();
		return substr(slashPos + 1, dotPos - slashPos - 1);
	}

	_String getFileExt() const
	{
		if (!isFile() || this->empty())
			return _String();

		size_type pos = getLastDotPos();
		if (pos == this->npos) // no ext
			return _String();

		return substr(pos + 1, this->length() - pos - 1);
	}

	_String getParentDir()
	{
		if (!isFile())
			removeTailSlash();

		size_type pos = getLastSlashPos();
		if (pos == this->npos)
			return _String();
		return substr(0, pos);
	}

	_MyType& addTailSlash()
	{
		if (*(this->end() - 1) != CHAR_SLASH)
			*this += CHAR_SLASH;
		return (*this);
	}

	_MyType& removeTailSlash()
	{
		if (*(this->end() - 1) == CHAR_SLASH)
			erase(this->end() - 1);
		return (*this);
	}

	void isFile(bool isfile)
	{
		_isFile = isfile;
	}
	bool isFile() const
	{
		return _isFile;
	}
protected:
	inline size_type getLastDotPos() const
	{
		return this->find_last_of(CHAR_DOT);
	}
	inline size_type getLastSlashPos() const
	{
		return this->find_last_of(CHAR_SLASH);
	}
	inline size_type getLastSlashPos(int32 count) const
	{
		size_type pos;
		pos = this->find_last_of(CHAR_SLASH);
		while (pos != this->npos && --count > 0)
			pos = this->find_last_of(CHAR_SLASH, pos - 1);
		return pos;
	}
private:
	bool	_isFile;
};

typedef basic_path<char>			CPathA;
typedef basic_path<wchar_t>			CPathW;
typedef basic_path<char >			CPath;

#ifdef FCL_EXPORTS
template class FCL_CLASS basic_path<char>;
template class FCL_CLASS basic_path<wchar_t>;
#endif

END_NAMESPACE

#endif // __PATH_H__
