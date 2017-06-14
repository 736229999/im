// MiniMMInfoBar.cpp : CMiniMMInfoBar µÄÊµÏÖ
#include "stdafx.h"
#include "MiniMMInfoBar.h"
#include ".\minimminfobar.h"

//CComModule _Module;
// CMiniMMInfoBar

STDMETHODIMP CMiniMMInfoBar::get_OwnerData(LONG* pVal)
{
	*pVal = m_nOwnerData;
	return S_OK;
}

STDMETHODIMP CMiniMMInfoBar::put_OwnerData(LONG newVal)
{
	m_nOwnerData=newVal;
	return S_OK;
}

STDMETHODIMP CMiniMMInfoBar::get_MMType(BSTR* pVal)
{
	*pVal = m_strType;
	return S_OK;
}

STDMETHODIMP CMiniMMInfoBar::put_MMType(BSTR newVal)
{
	m_strType = newVal;	

	if ( _wcsicmp((WCHAR *)newVal,L"vedio")==0 )
		m_bAudio=false;
	else
		m_bAudio=true;

	return S_OK;
}
