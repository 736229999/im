#ifndef _HYPRILNK_H_
#define _HYPRILNK_H_

//////////////////
// Simple text hyperlink derived from CString
//
class CHyperLinkString : public CString
{
public:
	CHyperLinkString(LPCTSTR lpLink = NULL) : CString(lpLink) { }
	~CHyperLinkString() { }
	const CHyperLinkString& operator=(LPCTSTR lpsz) 
	{
		CString::operator=(lpsz);
		return *this;
	}
	operator LPCTSTR() 
	{
		return CString::operator LPCTSTR(); 
	}
	virtual HINSTANCE Navigate()
	{
		//能打开任何东西
		return IsEmpty() ? NULL :
			ShellExecute(0, _T("open"), *this, 0, 0, SW_SHOWNORMAL);
	}
};

#endif	//_HYPRILNK_H_
