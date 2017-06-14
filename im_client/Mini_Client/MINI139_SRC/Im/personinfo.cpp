#include "stdafx.h"
#include "personinfo.h"
//
//
//CAddressBook::CAddressBook()
//{
//
//}
//
//
//CAddressBook::~CAddressBook()
//{
//	PersonInfo* pPersoInfo;
//	while( m_listAddressBook.GetCount() )
//	{
//		pPersoInfo = m_listAddressBook.RemoveTail();
//		delete pPersoInfo;
//	}
//}
//
//PersonInfo* CAddressBook::AddNewPersoInfo()
//{
//	PersonInfo* pNewInfo = new PersonInfo;
//
//	m_listAddressBook.AddTail(pNewInfo);
//	return m_listAddressBook.GetTail();
//}
//
//
//void CAddressBook::RemoveLastest()
//{
//	PersonInfo* pInfo = m_listAddressBook.RemoveTail();
//	delete pInfo;
//}
//
//void CAddressBook::ChangeType(PersonInfo* pPersonInfo,CString& strNewType)
//{
//	CString strItem,strType,strValue;
//	POSITION posInfo = pPersonInfo->GetHeadPosition();
//	while( posInfo )
//	{
//		CString& strItem = pPersonInfo->GetNext(posInfo);
//
//		int nSeparator = strItem.Find(':');
//		strType = strItem.Left(nSeparator);
//		strValue = strItem.Right(strItem.GetLength()-nSeparator-1);
//
//		if( strType == PERSON_INFO_SECTION_TYPE )
//		{
//			strItem.Format("%s:%s",PERSON_INFO_SECTION_TYPE,strNewType);
//			return;
//		}
//	}
//
//	//找不到类别这项信息,则添加
//	strItem.Format("%s:%s",PERSON_INFO_SECTION_TYPE,strNewType);
//	pPersonInfo->AddHead(strItem);
//
//}
//
//
//BOOL CAddressBook::SaveToDisk()
//{
//	FILE*  pfAdd = NULL;
//	char cNull = 0;
//
//	if( NULL == (pfAdd = fopen(m_strSavePath,"wb") ) )
//	{
//		AfxMessageBox("通讯录文件打开失败，不能保存");
//		return FALSE;
//	}
//
//	BOOL bSavedMyInfo = FALSE;
//	POSITION pos = m_listAddressBook.GetHeadPosition();
//	while(pos)
//	{
//		PersonInfo* pPInfo = NULL;
//		
//		if( !bSavedMyInfo)		//首先保存自己的名片信息
//		{
//			pPInfo = &m_MyPersonInfo;
//			bSavedMyInfo = TRUE;
//		}
//		else
//		{
//			pPInfo = m_listAddressBook.GetNext(pos);
//		}
//
//		POSITION posInfo = pPInfo->GetHeadPosition();
//		while( posInfo )
//		{
//			CString& strItem = pPInfo->GetNext(posInfo);
//			fwrite((LPCTSTR)strItem,strItem.GetLength()+1,1,pfAdd);
//		}
//		
//		fwrite(&cNull,1,1,pfAdd);
//
//	}
//
//	fclose(pfAdd);
//	return TRUE;
//}
//
////设置通讯录保存的路径
//void CAddressBook::SetSavePath(CString strPath)
//{
//	m_strSavePath = strPath;
//
//	FILE* pfAddr = NULL;
//
//
//	//同时检查是否有旧的通讯录存在，如果有则装载
//	if( NULL == (pfAddr = fopen(strPath,"rb")) )
//	{
//		return ;
//	}
//
//	char  szbuf[4096];
//	char* pDataIndex = szbuf;
//	char* pFileReadPos = szbuf;
//	char* pszItem = NULL;
//	int nReadCount;
//	PersonInfo* pCurrentPI;
////	pCurrentPI = AddNewPersoInfo();
//	pCurrentPI = &m_MyPersonInfo;  //首先读取自己的名片信息，自己的名片信息放在第一个
//	while( !feof(pfAddr) ) 
//	{
//		
//		nReadCount = fread(pFileReadPos,1,sizeof(szbuf)- (pDataIndex - szbuf),pfAddr);
//
//
//		while( nReadCount > 0 )
//		{
//			pszItem = (char*)memchr(pDataIndex,'\0',nReadCount);
//			if( (!pszItem) || (pszItem == pDataIndex+nReadCount-1) )	//找不到'\0'或在最后一个字节才找到,不能处于最后一个，那样判断信息不够
//			{
//				//为继续读文件准备
//				memcpy(szbuf,pDataIndex,nReadCount);
//				pFileReadPos = szbuf + nReadCount;
//				pDataIndex = szbuf;
//				break;
//			}
//			else
//			{
//				//生成一个新的信息项
//				pCurrentPI->AddTail(CString(pDataIndex));
//
//				nReadCount -= pszItem - pDataIndex + 1;
//
//				//遇到新的一个联系人的分割点了
//				if( *(pszItem + 1) == '\0')
//				{
//					pCurrentPI = AddNewPersoInfo();
//					++pszItem;
//
//				}
//				pDataIndex = pszItem + 1;	//指到下一个有效的位置
//				
//			}
//		}
//
//	}
//
//	fclose(pfAddr);
//}
//
//PersonInfo* CAddressBook::FindPersonInfoFromUserName(char* pszUserName)
//{
//	POSITION pos = m_listAddressBook.GetHeadPosition();
//	CString strType,strValue;
//
//	while(pos)
//	{
//		PersonInfo* pPInfo = NULL;
//		pPInfo = m_listAddressBook.GetNext(pos);
//
//		POSITION posInfo = pPInfo->GetHeadPosition();
//		while( posInfo )
//		{
//			CString& strItem = pPInfo->GetNext(posInfo);
//
//			int nSeparator = strItem.Find(':');
//			strType = strItem.Left(nSeparator);
//			strValue = strItem.Right(strItem.GetLength()-nSeparator-1);
//
//			if( strType == PERSON_INFO_SECTION_USERNAME )
//			{
//				if(strValue == pszUserName)
//				{
//					return pPInfo;
//				}
//				break;
//			}
//		}
//
//
//	}
//	return NULL;
//}