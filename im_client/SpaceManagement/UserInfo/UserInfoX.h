// UserInfoX.h : CUserInfoX ������

#pragma once
#include "resource.h"       // ������
////
//typedef 
//struct 
//{
//	unsigned long  id           ;	// 0 �û�id
//	char    username[21] ;	// 3 �û���
//	char    nickname[331] ;
//	char    password[17];
//	char    email[61]	  ;	// 1 �����ʼ�
//
//	char			face_url[201] ;
//	unsigned short  sex          ;
//	unsigned short  age			 ;//4 ����
//	unsigned short	face		 ;//8 ͷ��
//
//	unsigned short  gold_money   ;//�����Ŀ
//	unsigned short  message_count;//��Ϣ��
//	unsigned long   online_time  ;//����ʱ��
//
//} TWokuUserInfo;
//

// IUserInfoX
[
	object,
	uuid("53FCA6FC-62AC-4896-BBAC-AD36AC825C44"),
	dual,	helpstring("IUserInfoX �ӿ�"),
	pointer_default(unique)
]


__interface IUserInfoX : IDispatch
{
	[propget, id(1), helpstring("���� UserID")] HRESULT UserID([out, retval] LONG* pVal);
	[propput, id(1), helpstring("���� UserID")] HRESULT UserID([in] LONG newVal);
	[propget, id(2), helpstring("���� UserName")] HRESULT UserName([out, retval] BSTR* pVal);
	[propput, id(2), helpstring("���� UserName")] HRESULT UserName([in] BSTR newVal);
	[propget, id(3), helpstring("���� NickName")] HRESULT NickName([out, retval] BSTR* pVal);
	[propput, id(3), helpstring("���� NickName")] HRESULT NickName([in] BSTR newVal);
	[propget, id(4), helpstring("���� Sex")] HRESULT Sex([out, retval] SHORT* pVal);
	[propput, id(4), helpstring("���� Sex")] HRESULT Sex([in] SHORT newVal);
	[propget, id(5), helpstring("���� Email")] HRESULT Email([out, retval] BSTR* pVal);
	[propput, id(5), helpstring("���� Email")] HRESULT Email([in] BSTR newVal);
	[propget, id(6), helpstring("���� Password")] HRESULT Password([out, retval] BSTR* pVal);
	[propput, id(6), helpstring("���� Password")] HRESULT Password([in] BSTR newVal);
	[propget, id(7), helpstring("���� FaceUrl")] HRESULT FaceUrl([out, retval] BSTR* pVal);
	[propput, id(7), helpstring("���� FaceUrl")] HRESULT FaceUrl([in] BSTR newVal);
	[propget, id(8), helpstring("���� Age")] HRESULT Age([out, retval] USHORT* pVal);
	[propput, id(8), helpstring("���� Age")] HRESULT Age([in] USHORT newVal);
	[propget, id(9), helpstring("���� FaceID")] HRESULT FaceID([out, retval] USHORT* pVal);
	[propput, id(9), helpstring("���� FaceID")] HRESULT FaceID([in] USHORT newVal);
	[propget, id(10), helpstring("���� MessageCount")] HRESULT MessageCount([out, retval] USHORT* pVal);
	[propput, id(10), helpstring("���� MessageCount")] HRESULT MessageCount([in] USHORT newVal);
	[propget, id(11), helpstring("���� GoldMoney")] HRESULT GoldMoney([out, retval] FLOAT* pVal);
	[propput, id(11), helpstring("���� GoldMoney")] HRESULT GoldMoney([in] FLOAT newVal);
	[propget, id(12), helpstring("���� OnlineTime")] HRESULT OnlineTime([out, retval] LONG* pVal);
	[propput, id(12), helpstring("���� OnlineTime")] HRESULT OnlineTime([in] LONG newVal);
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

