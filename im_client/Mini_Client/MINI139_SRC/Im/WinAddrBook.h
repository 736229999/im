#pragma once

#include <wab.h>
#include <vector>
using namespace std;

//Typedef the function pointer to WABOpen
typedef HRESULT (WINAPI *fWABOpen)( 
	LPADRBOOK* ,
	LPWABOBJECT* ,
	LPWAB_PARAM ,
	DWORD 
);

typedef struct __CONTACTS {
	CString DisplayName; //char DisplayName[200];
	CString NickName; //char NickName[200];
	CString EmailAddr; //char EmailAddr[200];
	CString Mobile;
	CString HomePage;
	CString Company;
	CString Country;
	CString City;
	CString Street;
	CString Comment;
	DWORD   BirthDay;
} CONTACTS, *LPCONTACTS;


class ContactList : public vector<CONTACTS *>
{
public:
	~ContactList()
	{
		Clear();
	}

	void Clear()
	{
		ContactList::iterator it;
		for ( it=begin(); it!=end(); it++ )
		{
			delete (*it);
		}

		clear();
	}
};

class CWinAddrBook
{
public:
	CWinAddrBook(void);
	~CWinAddrBook(void);

public:
	void LoadContacts(ContactList &cl);

protected:
	void Init();

protected:
	LPADRBOOK		m_lpAddrBook;
	LPWABOBJECT		m_lpWABObject;
	HMODULE			m_hModule;
protected:
	fWABOpen		ssWABOpen;
	CString         m_strWabFile ;
};
