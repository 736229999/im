  // FmtString.cpp: implementation of the CFmtString class.
//
//////////////////////////////////////////////////////////////////////

#include "stdafx.h"
#include "FmtString.h"

#ifdef _DEBUG
#undef THIS_FILE
static char THIS_FILE[]=__FILE__;
#define new DEBUG_NEW
#endif

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

CFmtString::CFmtString() : m_point(0)
{

}

CFmtString::CFmtString(const _bstr_t & str) : m_str(str), m_point(0)
{

}

CFmtString::~CFmtString()
{

}

void CFmtString::operator+=(const _bstr_t& sub)
{
	if( sub.length() == 0 )
	{
		m_str += L"0;";
	}
	else
	{
		wostringstream tmp;
		tmp << sub.length() << L";" << (wchar_t *)sub;
		
		m_str += tmp.str();
	}
}

void CFmtString::operator+=(const long& sub)
{
	wostringstream tmp1, tmp2;

	tmp1 << sub;
	tmp2 << tmp1.str().length() << L";" << tmp1.str();

	m_str += tmp2.str();
}

void CFmtString::operator+=(const short& sub)
{
	wostringstream tmp1, tmp2;

	tmp1 << sub;
	tmp2 << tmp1.str().length() << L";" << tmp1.str();

	m_str += tmp2.str();
}

void CFmtString::operator+=(const double& sub)
{
	wchar_t szTmp[32] = {0};
	swprintf(szTmp, L"%.9f", sub);

	wostringstream tmp;
	tmp << wcslen(szTmp) << L";" << szTmp;

	m_str += tmp.str();
}

CFmtString::operator short()
{
	wstring str = GetSubString();
	return _wtoi(str.c_str());
}

CFmtString::operator long()
{
	wstring str = GetSubString();
	return _wtol(str.c_str());
}

CFmtString::operator double()
{
	wchar_t* endstr;
	wstring str = GetSubString();
	return wcstod(str.c_str(), &endstr);
}

CFmtString::operator _bstr_t()
{
	wstring str = GetSubString();
	return str.c_str();
}

wstring CFmtString::GetSubString()
{
	long end = m_str.find(L';', m_point);

	wstring str = m_str.substr(m_point, end - m_point);
	
	long len = _wtol(str.c_str());

	str = m_str.substr(end + 1, len);

	m_point = end + len + 1;

	return str;
}


BOOL CFmtString::End() const
{
	return  ! ( m_point >= m_str.length() );
}
