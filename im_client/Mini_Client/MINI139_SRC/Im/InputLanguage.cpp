#include "StdAfx.h"
#include ".\inputlanguage.h"

CInputLanguage::CInputLanguage(void)
{
  HKEY hKey,hKey1;
 DWORD cp=16;
 char lp[15];
 CString szID;
 CString szKeyName,szKeyName1;
 szKeyName = "Keyboard Layout\\Preload";
 szKeyName1 = "System\\CurrentControlSet\\Control\\Keyboard Layouts\\";

 int i=1;
 szID.Format("%d",i);

 DWORD lpT=REG_SZ;
 if(::RegOpenKey(HKEY_CURRENT_USER,szKeyName,&hKey)==ERROR_SUCCESS )
 { 
  while( ::RegQueryValueEx(hKey,szID,NULL,&lpT,(LPBYTE)lp,&cp) == ERROR_SUCCESS )
  { 
   CString szTempName;
   szTempName = szKeyName1 + (LPCTSTR)(LPTSTR)lp;
   if(RegOpenKey(HKEY_LOCAL_MACHINE,szTempName,&hKey1)==ERROR_SUCCESS )
   {
    char lpD[100];
    DWORD lpS=100; //DataSize
    if(RegQueryValueEx(hKey1,"Layout text",NULL,&lpT,(LPBYTE)lpD,&lpS)==ERROR_SUCCESS)
    {
     IL* p1,*p2;
     p1 = m_pILHead;
     p2 = new(IL);
     strcpy(p2->ilID,lp);
     strcpy(p2->szName,lpD);
     p2->pNext = NULL;
     if( p1 )
     {
      while( p1->pNext ){ p1 = p1->pNext ; }
      p1->pNext = p2;
     }
     else
     {
      m_pILHead = p2;
     }
    }
   }
   ::RegCloseKey(hKey1);
   i++;
   szID.Format("%d",i); 
  } 
 }

 ::RegCloseKey(hKey);
// return (m_pILHead != NULL );
}

CInputLanguage::~CInputLanguage(void)
{
	IL* p1,*p2;
 	p1 = m_pILHead;

 	while( p1 )
 	{
  	p2 = p1;
  	p1 = p1->pNext;
  	delete(p2);
 	}

 	m_pILHead = NULL;
}

CInputLanguage::IL * CInputLanguage::GetInputLanguageList()
{
 return m_pILHead;
} 


BOOL CInputLanguage::SelectInputLanguage(IL *pIL)
{
 if( !pIL ) return FALSE;
  HKL hkl;
  hkl=LoadKeyboardLayout(pIL->ilID,KLF_ACTIVATE);//装载输入法
  if(hkl==NULL) return FALSE;
  else{
   ActivateKeyboardLayout(hkl,KLF_SETFORPROCESS);//激活输入法
  }
  return TRUE;

}
