#pragma once

// ������������� Microsoft Visual C++ ������ IDispatch ��װ��

// ע��: ��Ҫ�޸Ĵ��ļ������ݡ����������
//  Microsoft Visual C++ �������ɣ������޸Ľ�����д��

/////////////////////////////////////////////////////////////////////////////
// CShowimage1 ��װ��

class CShowimage1 : public CWnd
{
protected:
	DECLARE_DYNCREATE(CShowimage1)
public:
	CLSID const& GetClsid()
	{
		static CLSID const clsid
			= { 0xE81F4AAD, 0xF89C, 0x4444, { 0x82, 0x4A, 0xB2, 0xB7, 0x62, 0x58, 0x73, 0x99 } };
		return clsid;
	}
	virtual BOOL Create(LPCTSTR lpszClassName, LPCTSTR lpszWindowName, DWORD dwStyle,
						const RECT& rect, CWnd* pParentWnd, UINT nID, 
						CCreateContext* pContext = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID); 
	}

    BOOL Create(LPCTSTR lpszWindowName, DWORD dwStyle, const RECT& rect, CWnd* pParentWnd, 
				UINT nID, CFile* pPersist = NULL, BOOL bStorage = FALSE,
				BSTR bstrLicKey = NULL)
	{ 
		return CreateControl(GetClsid(), lpszWindowName, dwStyle, rect, pParentWnd, nID,
		pPersist, bStorage, bstrLicKey); 
	}

// ����
public:


// ����
public:

// IShowImage

// Functions
//

	void LoadImageFile(LPCTSTR bsPathName)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x1, DISPATCH_METHOD, VT_EMPTY, NULL, parms, bsPathName);
	}
	CString get_Image()
	{
		CString result;
		InvokeHelper(0x2, DISPATCH_PROPERTYGET, VT_BSTR, (void*)&result, NULL);
		return result;
	}
	void put_Image(LPCTSTR newValue)
	{
		static BYTE parms[] = VTS_BSTR ;
		InvokeHelper(0x2, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}
	long * get_OwnData()
	{
		long * result;
		InvokeHelper(0x3, DISPATCH_PROPERTYGET, VT_I4|VT_BYREF, (void*)&result, NULL);
		return result;
	}
	void put_OwnData(long * newValue)
	{
		static BYTE parms[] = VTS_PI4 ;
		InvokeHelper(0x3, DISPATCH_PROPERTYPUT, VT_EMPTY, NULL, parms, newValue);
	}

// Properties
//



};
