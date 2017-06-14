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


// 该线程只能被一个download对象调用一次
UINT DownloadProc( LPVOID pParam )
{
	CoInitialize(NULL);
	CFaceDownload *p=(CFaceDownload*)pParam;

	// 进程开始标志
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
		p->m_sErr="获取配置文件信息失败";
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
			// 如果该文件存在，则不从网站下载
			if( FileInList(p->m_sExistFiles,sDirName+sName))
			{
				continue;
			}
			// 判断本地缓存中是否存在该文件,避免重复下载
			if(_access((sPath+sName).c_str(),0)!=-1)
			{
				// 保存该文件名
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
			// 保存该文件名
			face.sFile = sDirName+sName;
			
			p->m_faceFileList.push_back(face);
		}
	}

	// 下载结束
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
		m_sErr="当前正在下载中";
		return false;
	}
	m_nTask=0;
	m_nProcess=0;
	m_bDownloading=true;
	m_bDownloadErr=false;
	// 设置下载的全局标志
	m_bProcExit=false;
	// 表情图片存储的网络目录
	m_sUrlDir=szUrlDir;
	// 网络配置文件的url
	m_sUrlCfg=m_sUrlDir+"face.xml";
	// 下载文件的本地存放目录
	m_sSavedDir=szSavedPath;

	char sExePath[MAX_PATH];
	char drive[MAX_PATH], dir[MAX_PATH], fname[MAX_PATH], ext[MAX_PATH];
	CString strPath;
	GetModuleFileName(NULL,sExePath,MAX_PATH);
	_splitpath( sExePath, drive, dir, fname, ext );
	_makepath(sExePath, drive,dir,"","");


	// 本地对配置文件的缓存

	m_sTempCfg = sExePath;
	m_sTempCfg += "~temp~.temp";
	
	// 当前已经存在的文件列表
	m_sExistFiles=sExistFiles;

	if( m_sUrlDir=="" )
	{
		m_sErr="输入参数错误";
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
		//"msxml解析器加载失败，检测是否已经安装";
		//m_bErr=TRUE;
		return FALSE;
	}
	docPtr->PutresolveExternals(FALSE);
	docPtr->PutvalidateOnParse(FALSE);

	varOut = docPtr->load(varXml);
	if ((bool)varOut == FALSE)
	{
		//(char *)docPtr->parseError->Getreason();
		//="文件\""+sFile+"\"无法加载";
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
					// 获得快捷键
					namePtr=attrPtr->getNamedItem("sc");
					if(namePtr)
					{
						face.sShortCut=namePtr->text;
					}
					// 获得提示
					namePtr=attrPtr->getNamedItem("tip");
					if(namePtr)
					{
						face.sTip=namePtr->text;
					}
				}
				
				// 获得文件名
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
// 重叠 I/O 实现函数
//
void CALLBACK RecvComplete(DWORD dwError, 
						   DWORD cbTransferred, 
						   LPWSAOVERLAPPED lpOverlapped, 
						   DWORD dwFlags); 

#define BUFFER_SIZE 1024*8


typedef struct tagIOREQUEST
{
	WSAOVERLAPPED	over; // 必须在首位
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
	// 获得主机地址
	//
	m_lpHostEntry = gethostbyname(szServer);
	if (m_lpHostEntry == NULL)
	{
		m_sErr="无法连接到主机:";
		m_sErr+=szServer;
		return false;
	}

	//// 创建 TCP/IP 流套接字
	//m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	//if (m_socket == INVALID_SOCKET)
	//{
	//	m_sErr="创建套接字失败";
	//	return false;
	//}
	////
	//// 填上服务器地址结构
	////
	//m_saServer.sin_family = AF_INET;
	//m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
	//m_saServer.sin_port = htons(80);
	//int			nRet;

	////
	//// 连接套接字
	////
	//nRet = connect(m_socket, (LPSOCKADDR)&m_saServer, sizeof(SOCKADDR_IN));
	//if (nRet == SOCKET_ERROR)
	//{
	//	m_sErr="连接主机失败";
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
	//// 初始化 WinSock.dll
	////
	//nRet = WSAStartup(wVersionRequested, &wsaData);
	//if (nRet)
	//{
	//	char szErr[100];
	//	sprintf(szErr,"WSAStartup()错误(%d)\n",nRet);
	//	m_sErr=szErr;
	//	WSACleanup();
	//	return false;
	//}
	////
	//// 检测WinSock的版本
	////
	//if (wsaData.wVersion != wVersionRequested)
	//{
	//	m_sErr="当前WinSock的版本不支持";
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
	// 判断是否有错误
	//
	if (dwError)
	{

		fprintf(stderr,"\nRecvComplete() error: %ld", 
			dwError);
		return;
	}

	LPIOREQUEST pReq = (LPIOREQUEST)lpOver;

	//
	// 如果没有错误或没有数据返回，则关闭连接
	//	
	if (cbRecv == 0)
	{
		pReq->fFinished = TRUE;
		return;
	}

	// “ %ld bytes received", cbRecv”
	//
	// 将接收到的数据保存到文件
	//
	if(pReq->pOfs)
		pReq->pOfs->write((const char*)pReq->pBuffer,cbRecv);
	// 
	// 重新发送接受数据的缓冲区
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
		m_sErr="还未连接到主机";
		return false;
	}
	if(szUrl==NULL || szSavedFile==NULL)
	{
		m_sErr="获取文件函数的输入参数错误";
		return false;
	}


	// 创建 TCP/IP 流套接字
	m_socket = socket(AF_INET, SOCK_STREAM, IPPROTO_TCP);
	if (m_socket == INVALID_SOCKET)
	{
		m_sErr="创建套接字失败";
		return false;
	}
	//
	// 填上服务器地址结构
	//
	m_saServer.sin_family = AF_INET;
	m_saServer.sin_addr = *((LPIN_ADDR)*m_lpHostEntry->h_addr_list);
	m_saServer.sin_port = htons(80);

	//
	// 连接套接字
	//
	int nRet;
	nRet = connect(m_socket, (LPSOCKADDR)&m_saServer, sizeof(SOCKADDR_IN));
	if (nRet == SOCKET_ERROR)
	{
		m_sErr="连接主机失败";
		closesocket(m_socket);	
		return false;
	}

	//
	// 格式化HTTP请求并发送
	//
	char szBuffer[1024];
	sprintf(szBuffer, "GET %s\n", szUrl);
	nRet = send(m_socket, szBuffer, (int)strlen(szBuffer), 0);
	if (nRet == SOCKET_ERROR)
	{
		m_sErr="发送获取文件请求失败";
		closesocket(m_socket);	
		return false;
	}
	ofstream ofs;
	ofs.open(szSavedFile,ios::binary);
	if(ofs.fail())
	{
		m_sErr="文件";
		m_sErr+=szSavedFile;
		m_sErr+="打开失败";
		return false;	
	}

	//
	// 如果连接成功，现在可以发送一个接收缓冲区
	//

	// 
	// 这里使用了一个小技巧，通过给WSAOVERLAPPED增添
	// 额外的数据，通过lpOverlapped 参数传送给实现函数
	// 在实现函数中就可以使用这些额外信息数据
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
			m_sErr="WSARecv()接受数据失败";
			closesocket(m_socket);
			return false;
		}
	}

	// 接收文件内容，并写到硬盘中
	while(1)
	{

		//
		// 使用SleepEx()函数
		//
		SleepEx(0, TRUE);
		//
		// 如果实现函数告诉接受任务完成则结束
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
