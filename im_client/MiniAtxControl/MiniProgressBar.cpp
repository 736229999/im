// MiniProgressBar.cpp : CMiniProgressBar 的实现
#include "stdafx.h"
#include "MiniProgressBar.h"
#include ".\miniprogressbar.h"


// CMiniProgressBar

STDMETHODIMP CMiniProgressBar::SetPos(LONG nPos)
{
	m_nPos = nPos;

	if ( m_nPos>100 )
		m_nPos=100;

	if ( m_nPos<0 )
		m_nPos=0;

	FireViewChange();

	return S_OK;
}

STDMETHODIMP CMiniProgressBar::SetTip(BSTR strTip)
{
	wcsncpy_s(m_szTip,sizeof(m_szTip),strTip,sizeof(m_szTip)/sizeof(TCHAR));

	return S_OK;
}

STDMETHODIMP CMiniProgressBar::get_OwnerData(ULONG* pVal)
{
	// TODO: 在此添加实现代码
	*pVal = m_nOwnerData;

	return S_OK;
}

STDMETHODIMP CMiniProgressBar::put_OwnerData(ULONG newVal)
{
	// TODO: 在此添加实现代码
	m_nOwnerData = newVal;

	return S_OK;
}
