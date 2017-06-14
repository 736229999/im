// MiniProgressBar.cpp : CMiniProgressBar ��ʵ��
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
	// TODO: �ڴ����ʵ�ִ���
	*pVal = m_nOwnerData;

	return S_OK;
}

STDMETHODIMP CMiniProgressBar::put_OwnerData(ULONG newVal)
{
	// TODO: �ڴ����ʵ�ִ���
	m_nOwnerData = newVal;

	return S_OK;
}
