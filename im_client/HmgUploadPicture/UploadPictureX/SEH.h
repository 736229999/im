// SEH.h: interface for the CSEH class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_eps_mfc_thread_SEH_H__A8407769_4395_42E4_866F_C16CF95A921D__INCLUDED_)
#define AFX_eps_mfc_thread_SEH_H__A8407769_4395_42E4_866F_C16CF95A921D__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000

#include <eh.h>
#include <windows.h>

class CSEH  
{
public:
	void const * Where()const throw();
	unsigned What()const throw();
	static void MapSEtoEH()throw ();
	CSEH(EXCEPTION_POINTERS const &info) throw();
	virtual ~CSEH()throw();

private:
	void const * address_;
	unsigned code_;
};

#endif // !defined(AFX_eps_mfc_thread_SEH_H__A8407769_4395_42E4_866F_C16CF95A921D__INCLUDED_)
