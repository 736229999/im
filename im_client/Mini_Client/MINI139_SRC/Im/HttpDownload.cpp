#include "stdafx.h"
#include "httpdownload.h"
#include <fstream>


using namespace std;

bool CFaceDownload::m_bProcExit = true;

CFaceDownload::CFaceDownload():m_bDownloading(false),m_bDownloadErr(false)
{
	m_nTask=0;

}

CFaceDownload::~CFaceDownload()
{
	
}

bool FileInList(const vector<string>&sList,const string& sFile)
{
	size_t nC=sList.size();
	for(size_t i=0;i<nC;i++)
	{
		if(sList[i]==sFile)
			return true;
	}
	return false;	
}


// ���߳�ֻ�ܱ�һ��download�������һ��
UINT DownloadProc( LPVOID pParam )
{
	CoInitialize(NULL);
	CFaceDownload *p=(CFaceDownload*)pParam;

	// ���̿�ʼ��־
	CFaceDownload::m_bProcExit = false;
	p->m_faceFileList.clear();
	p->m_faceFileList.SetDir(p->m_sSavedDir);
	p->m_bDownloadErr=false;
	size_t i,j,nC;

	if(!p->DownLoadWebFile( p->m_sUrlCfg.c_str(),p->m_sTempCfg.c_str()))
	{
		CFaceDownload::m_bProcExit=true;
		p->m_bDownloading=false;
		p->m_bDownloadErr=true;
		return 1;
	}
	if(CFaceDownload::m_bProcExit)
	{
		return 1;
	}
	FACE_LIB lib;
	FACE_INFO face;
	string sPath;
	string sName;
	string sDirName;
	int nErr=0;
	if(!::GetFaceLib(p->m_sTempCfg.c_str(),lib))
	{
		p->m_sErr="��ȡ�����ļ���Ϣʧ��";
		p->m_bDownloading=false;
		p->m_bDownloadErr=true;
		CFaceDownload::m_bProcExit=true;
		return 1;
	};
#ifndef _DEBUG
	DeleteFile(p->m_sTempCfg.c_str());
#endif
	p->m_nTask=lib.Total();
	for( i=0;i<lib.size();i++)
	{
		if(CFaceDownload::m_bProcExit)
		{
			return 0;
		}
		sDirName=lib[i].sName;
		sPath=p->m_sSavedDir+sDirName;
		nC=lib[i].size();
		for(j=0;j<nC;j++)
		{
			p->m_nProcess++;
			face=lib[i][j];
			sName=face.sFile;
			// ������ļ����ڣ��򲻴���վ����
			if( FileInList(p->m_sExistFiles,sDirName+sName))
			{
				continue;
			}
			// �жϱ��ػ������Ƿ���ڸ��ļ�,�����ظ�����
			if(_access((sPath+sName).c_str(),0)!=-1)
			{
				// ������ļ���
				face.sFile=sDirName+sName;
				face.sUrl = p->m_sUrlDir + sDirName+"/"+sName;
				p->m_faceFileList.push_back(face);
				continue;
			}
			face.sUrl = p->m_sUrlDir + sDirName+"/"+sName;
			if(!p->DownLoadWebFile( face.sUrl.c_str(),(sPath+sName).c_str()) )
			{
				nErr++;
				continue;
			}
			if(CFaceDownload::m_bProcExit)
			{
				return 0;
			}
			// ������ļ���
			face.sFile = sDirName+sName;
			
			p->m_faceFileList.push_back(face);
		}
	}

	// ���ؽ���
	p->m_bDownloading=false;
	p->m_bDownloadErr=false;
	//p->UnInit();
	CoUninitialize();
	if(nErr!=0)
	{
		p->m_bDownloading=false;
		p->m_bDownloadErr=true;
		CFaceDownload::m_bProcExit=true;
		return nErr;
	}
	CFaceDownload::m_bProcExit=true;
	return 0;
};



bool CFaceDownload::DownloadFace( const char * szUrlDir, 
								 const char * szSavedPath,
								 const vector<string>& sExistFiles )
{
	if(IsDownloading())
	{
		m_sErr="��ǰ����������";
		return false;
	}
	m_nTask=0;
	m_nProcess=0;
	m_bDownloading=true;
	m_bDownloadErr=false;
	// �������ص�ȫ�ֱ�־
	m_bProcExit=false;
	// ����ͼƬ�洢������Ŀ¼
	m_sUrlDir=szUrlDir;
	// ���������ļ���url
	m_sUrlCfg=m_sUrlDir+"face.xml";
	// �����ļ��ı��ش��Ŀ¼
	m_sSavedDir=szSavedPath;

	char sExePath[MAX_PATH];
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL,sExePath,MAX_PATH);
	_splitpath( sExePath, drive, dir, fname, ext );
	_makepath(sExePath, drive,dir,"","");


	// ���ض������ļ��Ļ���

	m_sTempCfg = sExePath;
	m_sTempCfg += "~temp~.temp";
	
	// ��ǰ�Ѿ����ڵ��ļ��б�
	m_sExistFiles=sExistFiles;

	if( m_sUrlDir=="" )
	{
		m_sErr="�����������";
		return false;
	}
	AfxBeginThread(DownloadProc,this);
	return true;
}

bool GetFaceLib(const string& sConfig, FACE_LIB& lib)
{
	// load a document
	_variant_t varXml(sConfig.c_str());
	_variant_t varOut((bool)TRUE);

	CComPtr<MSXML2::IXMLDOMDocument> docPtr;	
	HRESULT hr = docPtr.CoCreateInstance(__uuidof(DOMDocument));

	if(hr<0)
	{
		//"msxml����������ʧ�ܣ�����Ƿ��Ѿ���װ";
		//m_bErr=TRUE;
		return FALSE;
	}
	docPtr->PutresolveExternals(FALSE);
	docPtr->PutvalidateOnParse(FALSE);

	varOut = docPtr->load(varXml);
	if ((bool)varOut == FALSE)
	{
		//(char *)docPtr->parseError->Getreason();
		//="�ļ�\""+sFile+"\"�޷�����";
		return FALSE;
	}

	CComPtr<MSXML2::IXMLDOMNode>	nodePtr;
	nodePtr=docPtr->GetlastChild();
	string sNode=(char*)nodePtr->nodeName;
	if(sNode!="d")
	{
		return FALSE;
	}	
	
	CComPtr<MSXML2::IXMLDOMNodeList> nodeListPtr;
	nodeListPtr=nodePtr->childNodes;
	int iLen=nodeListPtr->length;
	int nCount;
	int j;
	
	CComPtr<MSXML2::IXMLDOMNamedNodeMap> attrPtr;
	CComPtr<MSXML2::IXMLDOMNode>  itPtr,namePtr,filePtr;
	CComPtr<MSXML2::IXMLDOMNode> *	pNameNode=NULL;

	FACE_DIR dir;
	FACE_INFO face;
	for(int i=0;i<iLen;i++)
	{
		itPtr=nodeListPtr->Getitem(i);
		if(itPtr->nodeType==MSXML2::NODE_TEXT ||
			itPtr->nodeType==MSXML2::NODE_COMMENT)
			continue;
		else if(itPtr->nodeType==MSXML2::NODE_ELEMENT)
		{
			dir.clear();
			attrPtr=itPtr->attributes;
			namePtr=attrPtr->getNamedItem("name");
			if(namePtr!=NULL)
				dir.sName=namePtr->text;
			else
				dir.sName="";
			nCount=itPtr->childNodes->length;
			for(j=0;j<nCount;j++)
			{
				filePtr=itPtr->childNodes->Getitem(j);
				if(filePtr->nodeType==MSXML2::NODE_TEXT ||
					filePtr->nodeType==MSXML2::NODE_COMMENT)
					continue;
				face.Clear();
				attrPtr=filePtr->attributes;
				if(attrPtr)
				{
					// ��ÿ�ݼ�
					namePtr=attrPtr->getNamedItem("sc");
					if(namePtr)
					{
						face.sShortCut=namePtr->text;
					}
					// �����ʾ
					namePtr=attrPtr->getNamedItem("tip");
					if(namePtr)
					{
						face.sTip=namePtr->text;
					}
				}
				
				// ����ļ���
				face.sFile=(const char*)filePtr->text;
				dir.push_back(face);
			}
			lib.push_back(dir);
		}
	}
	return true;
}

#ifndef WININET

//
// �ص� I/O ʵ�ֺ���
//
void CALLBACK RecvComplete(DWORD dwError, 
						   DWORD cbTransferred, 
						   LPWSAOVERLAPPED lpOverlapped, 
						   DWORD dwFlags); 

#define BUFFER_SIZE 1024*8


typedef struct tagIOREQUEST
{
	WSAOVERLAPPED	over; // ��������λ
	SOCKET			Socket;
	BOOL			fFinished;
	LPBYTE			pBuffer;
	ofstream *		pOfs;
}IOREQUEST, *LPIOREQUEST;

#endif

CHttpDownload::CHttpDownload(void)
{
#ifndef WININET
	m_bConnectOK=false;
	m_lpHostEntry=NULL;
	m_socket=INVALID_SOCKET;
#endif
}

CHttpDownload::~CHttpDownload(void)
{
#ifndef WININET
	if(m_socket!=INVALID_SOCKET)
		closesocket(m_socket);
	UnInit();
#endif
}


#ifndef WININET
bool CHttpDownload::ConnectSite(const char * szServer)
{
	m_bConnectOK=false;
	//
	// ���������ַ
	//
	m_lpHostEntry = gethostbyname(szServer);
	if (m_lpHostEntry == NULL)
	{
		m_sErr="�޷����ӵ�����:";
		m_sErr+=szServer;
		return false;
	}

	//// ���� TCP/IP ���׽���
	//m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (m_socket == INVALID_SOCKET)
	//{
	//	m_sErr="�����׽���ʧ��";
	//	return false;
	//}
	////
	//// ���Ϸ�������ַ�ṹ
	////
	//m_saServer.sin_family = AF_INET;
	//m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
	//m_saServer.sin_port = htons(80);
	//int			nRet;

	////
	//// �����׽���
	////
	//nRet = connect(m_socket, (LPSOCKADDR)&m_saServer, sizeof(SOCKADDR_IN));
	//if (nRet == SOCKET_ERROR)
	//{
	//	m_sErr="��������ʧ��";
	//	closesocket(m_socket);	
	//	return false;
	//}

	m_bConnectOK=true;
	return true;
}



bool CHttpDownload::Init()
{
	//WORD wVersionRequested = WINSOCK_VERSION;
	//WSADATA wsaData;
	//int nRet;
	////
	//// ��ʼ�� WinSock.dll
	////
	//nRet = WSAStartup(wVersionRequested, &wsaData);
	//if (nRet)
	//{
	//	char szErr[100];
	//	sprintf(szErr,"WSAStartup()����(%d)\n",nRet);
	//	m_sErr=szErr;
	//	WSACleanup();
	//	return false;
	//}
	////
	//// ���WinSock�İ汾
	////
	//if (wsaData.wVersion != wVersionRequested)
	//{
	//	m_sErr="��ǰWinSock�İ汾��֧��";
	//	WSACleanup();
	//	return false;
	//}
	return true;
}

void CHttpDownload::UnInit()
{
//	WSACleanup();
}



void CALLBACK RecvComplete(DWORD dwError, 
						   DWORD cbRecv, 
						   LPWSAOVERLAPPED lpOver, 
						   DWORD dwFlags)
{
	//
	// �ж��Ƿ��д���
	//
	if (dwError)
	{

		fprintf(stderr,"\nRecvComplete() error: %ld", 
			dwError);
		return;
	}

	LPIOREQUEST pReq = (LPIOREQUEST)lpOver;

	//
	// ���û�д����û�����ݷ��أ���ر�����
	//	
	if (cbRecv == 0)
	{
		pReq->fFinished = TRUE;
		return;
	}

	// �� %ld bytes received", cbRecv��
	//
	// �����յ������ݱ��浽�ļ�
	//
	if(pReq->pOfs)
		pReq->pOfs->write((const char*)pReq->pBuffer,cbRecv);
	// 
	// ���·��ͽ������ݵĻ�����
	//
	WSABUF wsabuf;
	wsabuf.len = BUFFER_SIZE;
	wsabuf.buf = (char *)pReq->pBuffer;

	DWORD dwRecv;
	dwFlags = 0;
	int nRet;
	nRet = WSARecv(pReq->Socket, 
		&wsabuf,
		1,
		&dwRecv,
		&dwFlags,
		lpOver,
		RecvComplete);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			SHOWERR("RePost with WSARecv()");
			pReq->fFinished = TRUE;
		}
	}
}


bool CHttpDownload::GetHTTPFile(const char * szUrl,const char * szSavedFile)
{
	if(m_bConnectOK==false)
	{
		m_sErr="��δ���ӵ�����";
		return false;
	}
	if(szUrl==NULL || szSavedFile==NULL)
	{
		m_sErr="��ȡ�ļ������������������";
		return false;
	}


	// ���� TCP/IP ���׽���
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		m_sErr="�����׽���ʧ��";
		return false;
	}
	//
	// ���Ϸ�������ַ�ṹ
	//
	m_saServer.sin_family = AF_INET;
	m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
	m_saServer.sin_port = htons(80);

	//
	// �����׽���
	//
	int nRet;
	nRet = connect(m_socket, (LPSOCKADDR)&m_saServer, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		m_sErr="��������ʧ��";
		closesocket(m_socket);	
		return false;
	}

	//
	// ��ʽ��HTTP���󲢷���
	//
	char szBuffer[1024];
	sprintf(szBuffer, "GET %s\n", szUrl);
	nRet = send(m_socket, szBuffer, (int)strlen(szBuffer), 0);
	if (nRet == SOCKET_ERROR)
	{
		m_sErr="���ͻ�ȡ�ļ�����ʧ��";
		closesocket(m_socket);	
		return false;
	}
	ofstream ofs;
	ofs.open(szSavedFile,ios::binary);
	if(ofs.fail())
	{
		m_sErr="�ļ�";
		m_sErr+=szSavedFile;
		m_sErr+="��ʧ��";
		return false;	
	}

	//
	// ������ӳɹ������ڿ��Է���һ�����ջ�����
	//

	// 
	// ����ʹ����һ��С���ɣ�ͨ����WSAOVERLAPPED����
	// ��������ݣ�ͨ��lpOverlapped �������͸�ʵ�ֺ���
	// ��ʵ�ֺ����оͿ���ʹ����Щ������Ϣ����
	//
	BYTE aBuffer[BUFFER_SIZE];
	IOREQUEST ioRequest;
	memset(&ioRequest.over, 0, sizeof(WSAOVERLAPPED));
	ioRequest.Socket = m_socket;
	ioRequest.fFinished = FALSE;
	ioRequest.pBuffer = aBuffer;
	ioRequest.pOfs = &ofs;

	WSABUF wsabuf;
	wsabuf.len = BUFFER_SIZE;
	wsabuf.buf = (char *)aBuffer;

	DWORD dwRecv;
	DWORD dwFlags = 0;
	nRet = WSARecv(m_socket, 
		&wsabuf,
		1,
		&dwRecv,
		&dwFlags,
		(LPWSAOVERLAPPED)&ioRequest,
		RecvComplete);
	if (nRet == SOCKET_ERROR)
	{
		if (WSAGetLastError() != WSA_IO_PENDING)
		{
			m_sErr="WSARecv()��������ʧ��";
			closesocket(m_socket);
			return false;
		}
	}

	// �����ļ����ݣ���д��Ӳ����
	while(1)
	{

		//
		// ʹ��SleepEx()����
		//
		SleepEx(0, TRUE);
		//
		// ���ʵ�ֺ������߽���������������
		//
		if (ioRequest.fFinished)
			break;
	}
	ofs.close();

	closesocket(m_socket);	
	return true;
}

#else

bool CHttpDownload::DownLoadWebFile(const char* szUrl, const char * szLocalFile)
{
	HINTERNET hFile, hInet;
	char buffer[1024*4];

	bool bOk = FALSE;

	DeleteFile(szLocalFile);

	DWORD dwRead, dwSize;

	hInet = InternetOpen("DownLoadWebFile",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, szUrl,NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT | 
			INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{	
			CStdioFile fileDownload;

			DWORD dwBufferSize=sizeof(buffer);

			DWORD dwStatus = 0;
			if ( HttpQueryInfo(hFile, HTTP_QUERY_STATUS_CODE, buffer, &dwBufferSize ,NULL ) )
			{
				dwStatus = atoi(buffer);
			}

			if ( dwStatus == 200 )
			{
				BOOL bOpened = fileDownload.Open(szLocalFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

				if ( bOpened )
				{
					dwSize = sizeof(buffer);

					while ( InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
					{
						if ( bOpened )
						{
							fileDownload.Write(buffer,dwRead);						
						}
					}

					bOk = TRUE;

					fileDownload.Close();	
				}	
				else
				{
					bOk = false;
				}
			}
			else
			{
				bOk = false;
			}

			InternetCloseHandle(hFile);
		}
		else
		{
			bOk = false;
		}
		InternetCloseHandle(hInet);
	}
	else
	{
		bOk = false;
	}

	return bOk;
}

#endif
