
#pragma once

#include <afxtempl.h>

//个人信息
//typedef CList<CString,CString&> PersonInfo;



#define PERSON_INFO_SECTION_TYPE "类别"
#define PERSON_INFO_SECTION_USERNAME "用户名"
#define PERSON_INFO_SECTION_NAME "名字"
#define PERSON_INFO_SECTION_SEX  "性别"
#define PERSON_INFO_SECTION_BIRTHDAY "日期"
#define PERSON_INFO_SECTION_SCHOOL  "学校"
#define PERSON_INFO_SECTION_COMPANY "公司"
#define PERSON_INFO_SECTION_CAREER  "职业"
#define PERSON_INFO_SECTION_ADDRESS "地址"
#define PERSON_INFO_SECTION_EMAIL   "邮箱"
#define PERSON_INFO_SECTION_PHONE   "电话"
#define PERSON_INFO_SECTION_MSNNUM  "MSN"
#define PERSON_INFO_SECTION_QQNUM   "QQ"
#define PERSON_INFO_SECTION_BLOG    "网站"
#define PERSON_INFO_SECTION_NOTE    "备注"


//
////通讯录
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
//	//根据用户名选出名片
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