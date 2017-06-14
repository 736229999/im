#include "StdAfx.h"
#include ".\winaddrbook.h"

CWinAddrBook::CWinAddrBook(void)
{
	HINSTANCE hinstWAB=NULL;
	HRESULT hr=E_FAIL;
	HKEY keyResult;
	BYTE keyValue[MAX_PATH];
	DWORD dataout=800;

	RegOpenKeyEx(HKEY_LOCAL_MACHINE, WAB_DLL_PATH_KEY, 0, KEY_ALL_ACCESS, &keyResult); //"Software\\Microsoft\\WAB\\DLLPath"
	long result = RegQueryValueEx(keyResult, "", 0, 0, keyValue, &dataout);
	RegCloseKey(keyResult);

	//Get the full path of WAB and store in PathToWAB
	//m_strWabFile = (char*)keyValue;

	//Now let us load the library
	m_hModule = LoadLibrary((char*)keyValue);

	m_lpAddrBook  = NULL;
	m_lpWABObject = NULL;

	Init();
}

CWinAddrBook::~CWinAddrBook(void)
{
	FreeLibrary(m_hModule);
}

void CWinAddrBook::Init()
{
	if (m_hModule!=NULL)
	{
		ssWABOpen = (fWABOpen)GetProcAddress(m_hModule, "WABOpen");
		if (ssWABOpen ) 
			(ssWABOpen)(&m_lpAddrBook, &m_lpWABObject, NULL, 0);
	}

}

void CWinAddrBook::LoadContacts(ContactList &cl)
{
	cl.Clear();

	if ( m_lpWABObject && m_lpWABObject )
	{
		ULONG lpcbEntryID;
		ENTRYID* lpEntryID  ;

		HRESULT hr = m_lpAddrBook->GetPAB(&lpcbEntryID, &lpEntryID);

		if (hr!=S_OK) return ;

		ULONG ulFlags = MAPI_BEST_ACCESS,
			ulObjType = NULL;
		LPUNKNOWN lpIUnknown = NULL;

		hr = m_lpAddrBook->OpenEntry(lpcbEntryID,
			lpEntryID,
			NULL,									
			ulFlags,
			&ulObjType,
			&lpIUnknown);

		ulFlags = NULL;

		if (ulObjType==MAPI_ABCONT)
		{
			IABContainer* lpABContainer = static_cast<IABContainer*>(lpIUnknown); 
			LPMAPITABLE lpMAPItbl = NULL;
			hr = lpABContainer->GetContentsTable(ulFlags, &lpMAPItbl);
			//ASSERT(lpMAPItbl);

			if(lpMAPItbl == NULL)
			{
				return;
			}
			char buff[1024];

			SPropTagArray * propArray = (SPropTagArray *)buff;
			propArray->cValues = 12;
			propArray->aulPropTag[0] =PR_DISPLAY_NAME_A;
			propArray->aulPropTag[1] =PR_NICKNAME_A;
			propArray->aulPropTag[2] =PR_EMAIL_ADDRESS_A;
			propArray->aulPropTag[3] =PR_HOME_ADDRESS_CITY_A;
			propArray->aulPropTag[4] =PR_HOME_ADDRESS_COUNTRY_A;
			propArray->aulPropTag[5] =PR_HOME_ADDRESS_STREET_A;
			propArray->aulPropTag[6] =PR_BIRTHDAY;
			propArray->aulPropTag[7] =PR_COMPANY_NAME_A;
			propArray->aulPropTag[8] =PR_MOBILE_TELEPHONE_NUMBER_A;//PR_COMPANY_NAME_A;
			propArray->aulPropTag[9] = PR_COMMENT_A;
			propArray->aulPropTag[10] = PR_PERSONAL_HOME_PAGE_A;
			propArray->aulPropTag[11] = PR_ENTRYID;

			if (lpMAPItbl->SetColumns(propArray,TBL_BATCH) != S_OK)
			{
			//	return ;
			}

			ULONG ulRows;
			hr = lpMAPItbl->GetRowCount(0, &ulRows);

			if (hr!=S_OK) return ;


			SRowSet* lpRowSet = NULL;
			hr = lpMAPItbl->QueryRows(		//Return all the rows found in the address book
				ulRows,
				0,
				&lpRowSet);
	
			if ( lpRowSet )
			{
				for (ULONG x = 0; x < lpRowSet->cRows; x++) 
				{
					CONTACTS * contact = new CONTACTS();								

					SRow* lpRow = &lpRowSet->aRow[x]; //Get this specific row

					for (ULONG y = 0; y < lpRow->cValues; y++)
					{
						//Loop through the fields in the address book and assign
						//to our variable and put it in the address book variable					
						SPropValue* lPropVal = &lpRow->lpProps[y];

						try
						{		
							switch (lPropVal->ulPropTag)
							{
							case PR_DISPLAY_NAME_A:		
								contact->DisplayName = lPropVal->Value.lpszA;				
								break;

							case PR_EMAIL_ADDRESS_A:						
								contact->EmailAddr = lPropVal->Value.lpszA;		
								break;

							case PR_NICKNAME_A:						
								contact->NickName = lPropVal->Value.lpszA;
								break;

							case PR_MOBILE_TELEPHONE_NUMBER_A:
								contact->Mobile   = lPropVal->Value.lpszA;
								break;

							case PR_COMPANY_NAME_A:
								contact->Company   = lPropVal->Value.lpszA;
								break;

							case PR_BIRTHDAY:
								contact->BirthDay = lPropVal->Value.ul;
								break;

							case PR_HOME_ADDRESS_CITY_A:
								contact->City = lPropVal->Value.lpszA;
								break;

							case PR_HOME_ADDRESS_COUNTRY_A:
								contact->Country = lPropVal->Value.lpszA;
								break;

							case PR_HOME_ADDRESS_STREET_A:
								contact->Street = lPropVal->Value.lpszA;
								break;
							
							case PR_PERSONAL_HOME_PAGE_A:
								contact->HomePage = lPropVal->Value.lpszA;
								break;

							default:
								break;
							}
						}
						catch(...)
						{
						}
					}
					
					m_lpWABObject->FreeBuffer(lpRow);

					contact->EmailAddr.Trim();
					if ( contact->EmailAddr!="")
					{
						cl.push_back(contact);
					}
					else
					{
						delete contact;
					}
				}
			}

			m_lpWABObject->FreeBuffer(lpRowSet);
		}
	}
}
