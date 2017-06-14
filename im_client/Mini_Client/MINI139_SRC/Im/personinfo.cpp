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
//	//�Ҳ������������Ϣ,�����
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
//		AfxMessageBox("ͨѶ¼�ļ���ʧ�ܣ����ܱ���");
//		return FALSE;
//	}
//
//	BOOL bSavedMyInfo = FALSE;
//	POSITION pos = m_listAddressBook.GetHeadPosition();
//	while(pos)
//	{
//		PersonInfo* pPInfo = NULL;
//		
//		if( !bSavedMyInfo)		//���ȱ����Լ�����Ƭ��Ϣ
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
////����ͨѶ¼�����·��
//void CAddressBook::SetSavePath(CString strPath)
//{
//	m_strSavePath = strPath;
//
//	FILE* pfAddr = NULL;
//
//
//	//ͬʱ����Ƿ��оɵ�ͨѶ¼���ڣ��������װ��
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
//	pCurrentPI = &m_MyPersonInfo;  //���ȶ�ȡ�Լ�����Ƭ��Ϣ���Լ�����Ƭ��Ϣ���ڵ�һ��
//	while( !feof(pfAddr) ) 
//	{
//		
//		nReadCount = fread(pFileReadPos,1,sizeof(szbuf)- (pDataIndex - szbuf),pfAddr);
//
//
//		while( nReadCount > 0 )
//		{
//			pszItem = (char*)memchr(pDataIndex,'\0',nReadCount);
//			if( (!pszItem) || (pszItem == pDataIndex+nReadCount-1) )	//�Ҳ���'\0'�������һ���ֽڲ��ҵ�,���ܴ������һ���������ж���Ϣ����
//			{
//				//Ϊ�������ļ�׼��
//				memcpy(szbuf,pDataIndex,nReadCount);
//				pFileReadPos = szbuf + nReadCount;
//				pDataIndex = szbuf;
//				break;
//			}
//			else
//			{
//				//����һ���µ���Ϣ��
//				pCurrentPI->AddTail(CString(pDataIndex));
//
//				nReadCount -= pszItem - pDataIndex + 1;
//
//				//�����µ�һ����ϵ�˵ķָ����
//				if( *(pszItem + 1) == '\0')
//				{
//					pCurrentPI = AddNewPersoInfo();
//					++pszItem;
//
//				}
//				pDataIndex = pszItem + 1;	//ָ����һ����Ч��λ��
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