
#pragma once

#include <afxtempl.h>

//������Ϣ
//typedef CList<CString,CString&> PersonInfo;



#define PERSON_INFO_SECTION_TYPE "���"
#define PERSON_INFO_SECTION_USERNAME "�û���"
#define PERSON_INFO_SECTION_NAME "����"
#define PERSON_INFO_SECTION_SEX  "�Ա�"
#define PERSON_INFO_SECTION_BIRTHDAY "����"
#define PERSON_INFO_SECTION_SCHOOL  "ѧУ"
#define PERSON_INFO_SECTION_COMPANY "��˾"
#define PERSON_INFO_SECTION_CAREER  "ְҵ"
#define PERSON_INFO_SECTION_ADDRESS "��ַ"
#define PERSON_INFO_SECTION_EMAIL   "����"
#define PERSON_INFO_SECTION_PHONE   "�绰"
#define PERSON_INFO_SECTION_MSNNUM  "MSN"
#define PERSON_INFO_SECTION_QQNUM   "QQ"
#define PERSON_INFO_SECTION_BLOG    "��վ"
#define PERSON_INFO_SECTION_NOTE    "��ע"


//
////ͨѶ¼
//class CAddressBook
//{
//public:
//	CAddressBook();
//
//	~CAddressBook();
//
//	PersonInfo* AddNewPersoInfo();
//
//	void SetSavePath(CString strPath);
//
//	void RemoveLastest();
//
//	BOOL SaveToDisk();
//
//	//�����û���ѡ����Ƭ
//	PersonInfo* FindPersonInfoFromUserName(char* pszUserName);
//
//	CList<PersonInfo*,PersonInfo*> m_listAddressBook;
//
//	PersonInfo						m_MyPersonInfo;
//
//	void ChangeType(PersonInfo* pPersonInfo,CString& strNewType);
//
//protected:
//
//	CString m_strSavePath;
//	
//
//
//};