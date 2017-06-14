// SEH.cpp: implementation of the CSEH class.
//
//////////////////////////////////////////////////////////////////////

#include "..\stdafx.h"
#include "SEH.h"

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

static void TranslateSEH2EH(unsigned, EXCEPTION_POINTERS *info)
{
	throw CSEH(*info);
}

CSEH::CSEH(EXCEPTION_POINTERS const &info)throw()
{
	EXCEPTION_RECORD  const & e = *(info.ExceptionRecord);

	code_	 = e.ExceptionCode;
	address_ = e.ExceptionAddress;
	
}

CSEH::~CSEH()
{

}

void CSEH::MapSEtoEH()throw()
{
	_set_se_translator(TranslateSEH2EH);
};

unsigned CSEH::What() const throw()
{
	return code_;
}

void const * CSEH::Where() const throw()
{
	return address_;
}
