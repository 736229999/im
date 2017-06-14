// FmtString.h: interface for the CFmtString class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_FMTSTRING_H__F717E942_AEEA_4BC6_99CC_8328E3BE98D0__INCLUDED_)
#define AFX_FMTSTRING_H__F717E942_AEEA_4BC6_99CC_8328E3BE98D0__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <sstream>
using namespace std;

class CFmtString
{
public:
	CFmtString(const _bstr_t& str);
	CFmtString();
	virtual ~CFmtString();

	void operator+=(const _bstr_t& sub);
	void operator+=(const long& sub);
	void operator+=(const short& sub);
	void operator+=(const double& sub);

	operator short();
	operator long();
	operator double();
	operator _bstr_t();

protected:
	wstring GetSubString();

public:
	BOOL End()const;
	wstring	m_str;

private:
	long	m_point;
};

#endif // !defined(AFX_FMTSTRING_H__F717E942_AEEA_4BC6_99CC_8328E3BE98D0__INCLUDED_)
