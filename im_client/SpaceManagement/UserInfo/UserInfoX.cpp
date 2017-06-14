// UserInfoX.cpp : CUserInfoX 的实现

#include "stdafx.h"
#include "UserInfoX.h"
#include ".\userinfox.h"
#include "comutil.h"

// CUserInfoX


STDMETHODIMP CUserInfoX::get_UserID(LONG* pVal)
{
	*pVal = m_nUserID;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_UserID(LONG newVal)
{
	m_nUserID = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_UserName(BSTR* pVal)
{
	m_strUserName.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_UserName(BSTR newVal)
{
	m_strUserName = newVal;
	//strncpy(m_UserInfo.username,(LPCSTR)_bstr_t(newVal),sizeof(m_UserInfo.username)-1);
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_NickName(BSTR* pVal)
{
	//USES_CONVERSION;
	//*pVal = A2W((LPCSTR)m_UserInfo.nickname);
	m_strNickName.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_NickName(BSTR newVal)
{
	m_strNickName = newVal;
	//strncpy(m_UserInfo.nickname, (LPCSTR)_bstr_t(newVal),sizeof(m_UserInfo.nickname)-1);
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_Sex(SHORT* pVal)
{
	*pVal = m_nSex;// m_UserInfo.sex;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_Sex(SHORT newVal)
{
	m_nSex = newVal;
	//m_UserInfo.sex = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_Email(BSTR* pVal)
{
	m_strEmail.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_Email(BSTR newVal)
{
	m_strEmail = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_Password(BSTR* pVal)
{
	m_strPassword.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_Password(BSTR newVal)
{
	m_strPassword = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_FaceUrl(BSTR* pVal)
{
	m_strFaceUrl.CopyTo(pVal);
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_FaceUrl(BSTR newVal)
{
	m_strFaceUrl = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_Age(USHORT* pVal)
{
	*pVal = m_nAge;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_Age(USHORT newVal)
{
	// TODO: 在此添加实现代码
	m_nAge = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_FaceID(USHORT* pVal)
{
	*pVal = m_nFaceID;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_FaceID(USHORT newVal)
{
	m_nFaceID = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_MessageCount(USHORT* pVal)
{
	*pVal = m_nMessageCount;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_MessageCount(USHORT newVal)
{
	m_nMessageCount = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_GoldMoney(FLOAT* pVal)
{
	*pVal = m_nGoldMoney;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_GoldMoney(FLOAT newVal)
{
	m_nGoldMoney = newVal;
	return S_OK;
}

STDMETHODIMP CUserInfoX::get_OnlineTime(LONG* pVal)
{
	*pVal = m_nOnlineTime;
	return S_OK;
}

STDMETHODIMP CUserInfoX::put_OnlineTime(LONG newVal)
{
	m_nOnlineTime = newVal;
	return S_OK;
}

