// UserInfoX.h : CUserInfoX 的声明

#pragma once
#include "resource.h"       // 主符号
////
//typedef 
//struct 
//{
//	unsigned long  id           ;	// 0 用户id
//	char    username[21] ;	// 3 用户名
//	char    nickname[331] ;
//	char    password[17];
//	char    email[61]	  ;	// 1 电子邮件
//
//	char			face_url[201] ;
//	unsigned short  sex          ;
//	unsigned short  age			 ;//4 年龄
//	unsigned short	face		 ;//8 头像
//
//	unsigned short  gold_money   ;//金币数目
//	unsigned short  message_count;//消息数
//	unsigned long   online_time  ;//在线时长
//
//} TWokuUserInfo;
//

// IUserInfoX
[
	object,
	uuid("53FCA6FC-62AC-4896-BBAC-AD36AC825C44"),
	dual,	helpstring("IUserInfoX 接口"),
	pointer_default(unique)
]


__interface IUserInfoX : IDispatch
{
	[propget, id(1), helpstring("属性 UserID")] HRESULT UserID([out, retval] LONG* pVal);
	[propput, id(1), helpstring("属性 UserID")] HRESULT UserID([in] LONG newVal);
	[propget, id(2), helpstring("属性 UserName")] HRESULT UserName([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("属性 UserName")] HRESULT UserName([in] BSTR newVal);
	[propget, id(3), helpstring("属性 NickName")] HRESULT NickName([out, retval] BSTR* pVal);
	[propput, id(3), helpstring("属性 NickName")] HRESULT NickName([in] BSTR newVal);
	[propget, id(4), helpstring("属性 Sex")] HRESULT Sex([out, retval] SHORT* pVal);
	[propput, id(4), helpstring("属性 Sex")] HRESULT Sex([in] SHORT newVal);
	[propget, id(5), helpstring("属性 Email")] HRESULT Email([out, retval] BSTR* pVal);
	[propput, id(5), helpstring("属性 Email")] HRESULT Email([in] BSTR newVal);
	[propget, id(6), helpstring("属性 Password")] HRESULT Password([out, retval] BSTR* pVal);
	[propput, id(6), helpstring("属性 Password")] HRESULT Password([in] BSTR newVal);
	[propget, id(7), helpstring("属性 FaceUrl")] HRESULT FaceUrl([out, retval] BSTR* pVal);
	[propput, id(7), helpstring("属性 FaceUrl")] HRESULT FaceUrl([in] BSTR newVal);
	[propget, id(8), helpstring("属性 Age")] HRESULT Age([out, retval] USHORT* pVal);
	[propput, id(8), helpstring("属性 Age")] HRESULT Age([in] USHORT newVal);
	[propget, id(9), helpstring("属性 FaceID")] HRESULT FaceID([out, retval] USHORT* pVal);
	[propput, id(9), helpstring("属性 FaceID")] HRESULT FaceID([in] USHORT newVal);
	[propget, id(10), helpstring("属性 MessageCount")] HRESULT MessageCount([out, retval] USHORT* pVal);
	[propput, id(10), helpstring("属性 MessageCount")] HRESULT MessageCount([in] USHORT newVal);
	[propget, id(11), helpstring("属性 GoldMoney")] HRESULT GoldMoney([out, retval] FLOAT* pVal);
	[propput, id(11), helpstring("属性 GoldMoney")] HRESULT GoldMoney([in] FLOAT newVal);
	[propget, id(12), helpstring("属性 OnlineTime")] HRESULT OnlineTime([out, retval] LONG* pVal);
	[propput, id(12), helpstring("属性 OnlineTime")] HRESULT OnlineTime([in] LONG newVal);
};



// CUserInfoX

[
	coclass,
	threading("apartment"),
	vi_progid("UserInfo.UserInfoX"),
	progid("UserInfo.UserInfoX.1"),
	version(1.0),
	uuid("D03F4A24-8CB7-4DC8-BA67-C52E0D5F2BA9"),
	helpstring("UserInfoX Class")
]
class ATL_NO_VTABLE CUserInfoX : 
	public IUserInfoX
{
public:
	CUserInfoX()
	{
		m_strFaceUrl  = "";
		m_strEmail    = "";
		m_strNickName = "";
		m_strUserName = "";
		m_strPassword = "";
	}


	DECLARE_PROTECT_FINAL_CONSTRUCT()

	HRESULT FinalConstruct()
	{
		return S_OK;
	}
	
	void FinalRelease() 
	{
	}

public:

	STDMETHOD(get_UserID)(LONG* pVal);
	STDMETHOD(put_UserID)(LONG newVal);
	STDMETHOD(get_UserName)(BSTR* pVal);
	STDMETHOD(put_UserName)(BSTR newVal);
	STDMETHOD(get_NickName)(BSTR* pVal);
	STDMETHOD(put_NickName)(BSTR newVal);
	STDMETHOD(get_Sex)(SHORT* pVal);
	STDMETHOD(put_Sex)(SHORT newVal);
	STDMETHOD(get_Email)(BSTR* pVal);
	STDMETHOD(put_Email)(BSTR newVal);
	STDMETHOD(get_Password)(BSTR* pVal);
	STDMETHOD(put_Password)(BSTR newVal);

protected:
	long		m_nUserID;
	short       m_nSex;
	short       m_nAge;
	short       m_nFaceID;
	float       m_nGoldMoney;
	short       m_nMessageCount;
	long        m_nOnlineTime;
	CComBSTR	m_strUserName;
	CComBSTR    m_strNickName;
	CComBSTR    m_strEmail;
	CComBSTR    m_strFaceUrl;
	CComBSTR    m_strPassword;

public:
	STDMETHOD(get_FaceUrl)(BSTR* pVal);
	STDMETHOD(put_FaceUrl)(BSTR newVal);
	STDMETHOD(get_Age)(USHORT* pVal);
	STDMETHOD(put_Age)(USHORT newVal);
	STDMETHOD(get_FaceID)(USHORT* pVal);
	STDMETHOD(put_FaceID)(USHORT newVal);
	STDMETHOD(get_MessageCount)(USHORT* pVal);
	STDMETHOD(put_MessageCount)(USHORT newVal);
	STDMETHOD(get_GoldMoney)(FLOAT* pVal);
	STDMETHOD(put_GoldMoney)(FLOAT newVal);
	STDMETHOD(get_OnlineTime)(LONG* pVal);
	STDMETHOD(put_OnlineTime)(LONG newVal);
};

