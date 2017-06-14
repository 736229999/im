// im\VersionUpdatgeDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include "VersionUpdatgeDlg.h"
#include ".\versionupdatgedlg.h"

// CVersionUpdatgeDlg 对话框
#define WM_DOWNLOAD_NOTIFY   WM_CHECKVERSION+0x0F0

IMPLEMENT_DYNAMIC(CVersionUpdatgeDlg, CDialog)
CVersionUpdatgeDlg::CVersionUpdatgeDlg(int nVersionFlag, CString strVersionUrl)
	: CDialog(CVersionUpdatgeDlg::IDD, GetDesktopWindow())
{
	m_strVersionUrl = strVersionUrl ;
	m_nVersionFlag  = nVersionFlag;


	m_hThread	    = NULL;
	m_bStopDownload = false;

	Create(IDD,GetDesktopWindow());
}

CVersionUpdatgeDlg::~CVersionUpdatgeDlg()
{
	TerminateDownload();
}

void CVersionUpdatgeDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_PROGRESS1, m_wndProgress);
}


BEGIN_MESSAGE_MAP(CVersionUpdatgeDlg, CDialog)
	ON_MESSAGE(WM_DOWNLOAD_NOTIFY,OnDownLoadNotify)
	ON_WM_TIMER()
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_BN_CLICKED(IDC_BTN_INSTALL, OnBnClickedInstall)
END_MESSAGE_MAP()


// CVersionUpdatgeDlg 消息处理程序
UINT CVersionUpdatgeDlg::DownloadProc(LPVOID param)
{
	CVersionUpdatgeDlg *pClass = (CVersionUpdatgeDlg *)(param);

	bool bGetDownloadInfo=false;

	/*
	CString strXml = UrlGetStr(pClass->m_strVersionUrl);

	::CoInitialize(0);

	CComPtr<IXMLDOMDocument> pDoc;
	pDoc.CoCreateInstance(__uuidof(DOMDocument));

	VARIANT_BOOL bSuccess = false;
	BSTR szXml = strXml.AllocSysString();
	pDoc->loadXML(szXml,&bSuccess);
	::SysFreeString(szXml);
	CComBSTR bstr(L"mm_hmg/update_list/update_item[0]/*"); 
	CComPtr<IXMLDOMNodeList> pNodes; 

	pDoc->selectNodes(bstr,&pNodes);

	if ( pNodes )
	{
		long nNodes; pNodes->get_length(&nNodes);

		if ( nNodes==5 )
		{
			CComPtr<IXMLDOMNode> pNode = NULL;

			pNodes->get_item(0, &pNode);

			USES_CONVERSION;

			pNode->get_text(&bstr);
			pClass->m_strVersion = W2A(bstr);

			pNode.Release();

			pNodes->get_item(1, &pNode);

			pNode->get_text(&bstr);
			pClass->m_strDate = W2A(bstr);

			pNode.Release();

			pNodes->get_item(2, &pNode);

			pNode->get_text(&bstr);
			pClass->m_strIssueName = W2A(bstr);

			pNode.Release();

			pNodes->get_item(3, &pNode);

			pNode->get_text(&bstr);
			pClass->m_strSize = W2A(bstr);

			pNode.Release();

			pNodes->get_item(4, &pNode);

			pNode->get_text(&bstr);
			pClass->m_strDownloadUrl = W2A(bstr);

			pNode.Release();

			bGetDownloadInfo = true;
		}
	}

	if ( bGetDownloadInfo==false && pClass->m_bStopDownload==false )
	{
		::PostMessage(pClass->m_hWnd,WM_DOWNLOAD_NOTIFY,0,2);
		goto END;
	}

	if ( pClass->m_bStopDownload==false )
		::PostMessage(pClass->m_hWnd,WM_DOWNLOAD_NOTIFY,0,3);

	*/
	HINTERNET hFile, hInet;
	char buffer[1024];
	DWORD dwRead, dwSize;

	hInet = InternetOpen("xTreamLiveUpdate",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);

	//pClass->m_strDownloadUrl = "http://dldir1.qq.com/qqfile/qq/QQ6.9/13786/QQ6.9.exe";
	//pClass->m_strIssueName = "QQ6.9.exe";

	//pClass->m_strDownloadUrl = "http://webim.beta.fn.com/webim/static/exe/MouMou_1.0.1.3.exe";
	//pClass->m_strIssueName = "MouMou_1.0.1.3.exe";
	if ( hInet!=NULL )
	{
		hFile = InternetOpenUrl(hInet, pClass->m_strDownloadUrl, NULL, 0, 
			INTERNET_FLAG_RELOAD | 
			INTERNET_FLAG_NO_CACHE_WRITE |
			INTERNET_FLAG_EXISTING_CONNECT | 
			INTERNET_FLAG_NO_COOKIES,
			0
			);

		if ( hFile!=NULL )
		{
			char  pcBuffer[255];
			DWORD dwLength = 255;
			DWORD dwIndex = 0;

			DWORD dwStatus = 0;

			if ( HttpQueryInfo(hFile,HTTP_QUERY_STATUS_CODE,pcBuffer,&dwLength,&dwIndex) )
			{
				dwStatus = atoi(pcBuffer);
			}

			if ( dwStatus==200 ) //file exist
			{
				CStdioFile fileDownload;

				DWORD dwBufferSize=sizeof(buffer);
			
				if ( HttpQueryInfo(hFile, HTTP_QUERY_CONTENT_LENGTH, buffer, &dwBufferSize ,NULL ) )
				{
					pClass->m_dwRealFileSize = atoi(buffer)/1024.0;
				}
				else
				{
					pClass->m_dwRealFileSize = atoi(pClass->m_strSize);
				}

				CString str;
				/*
				if(pClass->m_dwRealFileSize > 1024)
				{
					str.Format("大小: %d M",pClass->m_dwRealFileSize/1024); 
				}
				else
				*/
				{
					str.Format("大小: %d K",pClass->m_dwRealFileSize);
				}
					
				pClass->SetDlgItemText(IDC_STATIC2,str);
				pClass->m_dwDownLoadSize = 0; /*单位是byte*/

				bool bOpened = fileDownload.Open(pClass->m_strDownloadFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);

				try
				{
					dwSize = sizeof(buffer);

					int count=0;
					while ( pClass->m_bStopDownload==false && InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead>0 )
					{
						if ( bOpened )
						{
							fileDownload.Write(buffer,dwRead);
							pClass->m_dwDownLoadSize += dwRead;
						}
						count++;

						::PostMessage(pClass->m_hWnd,WM_DOWNLOAD_NOTIFY,dwRead,0);
						if(dwRead <1024)
							dwRead = dwRead;
					}
				}
				catch(...)
				{
				}

				if ( bOpened )
					fileDownload.Close();

				if ( pClass->m_bStopDownload==false )
					::PostMessage(pClass->m_hWnd,WM_DOWNLOAD_NOTIFY,0,1);
			}
			else
			{
				::PostMessage(pClass->m_hWnd,WM_DOWNLOAD_NOTIFY,0,2);
			}

			InternetCloseHandle(hFile);
		}
        
		InternetCloseHandle(hInet);
	}

END:
	/*
	if ( pDoc )
		pDoc.Release();
	*/
	::CoUninitialize();

	pClass->m_bStopDownload=false;
	CloseHandle(pClass->m_hThread);
	pClass->m_hThread = NULL;

	return 0;
}

LRESULT CVersionUpdatgeDlg::OnDownLoadNotify(WPARAM w,LPARAM l)
{
	switch ( l )
	{
		//下载完成
		case 1:
		{
			CString str;
			int nPer = 100;
			str.Format("%d%%",nPer);
			SetDlgItemText(IDC_STATIC_INFO2,str);
			SetDlgItemText(IDC_STATIC1,"下载完成");
			m_wndProgress.SetPos(nPer);
			GetDlgItem(IDC_BTN_INSTALL)->EnableWindow(TRUE);
			break;
		}
		case 2:
		{
			static int n=0;

			if ( n++<2 )
			{
				StartDownload();
			}
			else
			{
				if ( CMessageBox::Model(GetSafeHwnd(),"自动更新竟然失败了,您可去 http://im.feiniu.com/ 下载最新版本哞哞","哞哞自动更新",MB_OKCANCEL|MB_ICONINFORMATION)==IDOK)
				{
					n=0;
					StartDownload();
				}
				else
				{
					DestroyWindow();
				}
			}
			
			break;
		}
		case 3:
			/*
			SetDlgItemText(IDC_STATIC1,"文件: "+m_strIssueName);
			SetDlgItemText(IDC_STATIC2,"大小: "+m_strSize+" kb");
			SetDlgItemText(IDC_STATIC3,"版本: "+m_strVersion);
			SetDlgItemText(IDC_STATIC4,"日期: "+m_strDate);
			*/
			break;
		case 0:
			{
				DWORD dwRead = w;
				CString str;

				if ( m_dwPreTick!=0 )
				{
					DWORD dwTick = GetTickCount();

					static int nDownloaded=0;

					if ( dwTick - m_dwPreTick > 1000 )
					{
						float fDownloadSpeed = nDownloaded*1000/((dwTick-m_dwPreTick)); //kb/s
					
						if ( fDownloadSpeed>1024.0 )
						{
							str.Format("下载速度: %d kb/s",(int)(fDownloadSpeed/1024));
						}
						else
						{
							str.Format("下载速度: %d b/s",(int)(fDownloadSpeed));
						}

						SetDlgItemText(IDC_STATIC_SPEED,str);

						int nPer = 100 * ( m_dwDownLoadSize /1024 ) / m_dwRealFileSize ;

						str.Format("%d%%",nPer);
						SetDlgItemText(IDC_STATIC_INFO2,str);
						m_wndProgress.SetPos(nPer);
			
						m_dwPreTick = dwTick;
						nDownloaded = 0;
					}
					else
					{
						nDownloaded+=dwRead;
					}				
				}
				else
				{
					m_dwPreTick = GetTickCount();
				}
			}
		default:
			break;
	}

	return 1;
}

BOOL CVersionUpdatgeDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_dwPreTick = 0;

	m_wndProgress.SetRange(0,100);
	m_wndProgress.SetPos(0);

	SetWindowPos(&wndTopMost,0,0,0,0,SWP_NOMOVE|SWP_NOSIZE);

	m_strDownloadUrl = m_strVersionUrl;

	m_strIssueName= m_strDownloadUrl.Right(m_strDownloadUrl.GetLength() - m_strDownloadUrl.ReverseFind('/') - 1);



	char tempDownLoadPath[255];
	::GetTempPath(sizeof(tempDownLoadPath), tempDownLoadPath);
	CreateDirectory(tempDownLoadPath, NULL);
	m_strDownloadFile.Format("%s\%s", tempDownLoadPath, m_strIssueName);

	GetDlgItem(IDC_BTN_INSTALL)->EnableWindow(FALSE);
	StartDownload();

	return TRUE; 
}

void CVersionUpdatgeDlg::OnTimer(UINT nIDEvent)
{
	if ( nIDEvent==0x01 )
	{
		KillTimer(nIDEvent);
		CDialog::OnCancel();
	}

	CDialog::OnTimer(nIDEvent);
}

void CVersionUpdatgeDlg::OnNcDestroy()
{
	CDialog::OnNcDestroy();

	if ( m_nVersionFlag==2 ||  m_nVersionFlag==1 )
	{
		theApp.m_pMainWnd->DestroyWindow();
	}

	delete this;
}

void CVersionUpdatgeDlg::OnClose()
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	TerminateDownload();
	DestroyWindow();
}

bool CVersionUpdatgeDlg::StartDownload()
{
	TerminateDownload();

	DWORD dwThreadId ;

	m_hThread = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)DownloadProc,
		(LPVOID)this,
		0,
		&dwThreadId);

	return true;
}

bool CVersionUpdatgeDlg::TerminateDownload()
{
	if ( m_hThread )
	{
		if ( m_bStopDownload==false )
		{
			m_bStopDownload=true;
			Sleep(300);
		}

		if ( m_hThread )
		{            
			try{
				TerminateThread(m_hThread,0);
				CloseHandle(m_hThread);
			}
			catch (...) {
			}

			m_hThread = NULL;
		}
	}

	return true;
}

void CVersionUpdatgeDlg::OnBnClickedCancel()
{
	DestroyWindow();
}

void CVersionUpdatgeDlg::OnBnClickedInstall()
{
	ShellExecute(this->m_hWnd,"open",this->m_strDownloadFile,"","", SW_SHOW );
	DestroyWindow();
}






CVersionUpdateDlgDui::CVersionUpdateDlgDui(int nVersionFlag, CString strVersionUrl)
{
	m_strVersionUrl = strVersionUrl ;
	m_nVersionFlag	= nVersionFlag;

	m_hThread		= NULL;
	m_bStopDownload = false;

}


CVersionUpdateDlgDui::~CVersionUpdateDlgDui()
{
	TerminateDownload();

}


extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

void CVersionUpdateDlgDui::UpdateBk()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		DuiLib::CControlUI* background = pInstance->m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, pInstance->GetHWND());
		}
	}
}



std::tr1::shared_ptr<CVersionUpdateDlgDui> CVersionUpdateDlgDui::pInstance = NULL;
std::tr1::shared_ptr<CVersionUpdateDlgDui> CVersionUpdateDlgDui::Show(int nVersionFlag, CString strVersionUrl)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		BringWindowToTop(pInstance->GetHWND());
		::ShowWindow(pInstance->GetHWND(),SW_NORMAL);
	}
	else
	{
		pInstance = std::tr1::shared_ptr<CVersionUpdateDlgDui>(new CVersionUpdateDlgDui(nVersionFlag, strVersionUrl));
		pInstance->Create(GetDesktopWindow(), _T("哞哞更新"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}
	return pInstance;
}


void CVersionUpdateDlgDui::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		if(msg.pSender == m_pBtnInstall)
		{	// 安装
			OnBnClickedInstall();
		}
	}
}

void CVersionUpdateDlgDui::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	UpdateBk();
	SetIcon(IDR_MAINFRAME);
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	m_pBtnInstall = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnInstall"));
	m_pLblDownloadTips = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("lblDownloadTips"));
	m_pLblDownloadRate = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("lblDownloadRate"));
	m_pPrgDownload  = static_cast<DuiLib::CProgressUI*>(m_PaintManager.FindControl("prgDownload"));
	m_pBtnInstall->SetEnabled(false);

	//获取url和文件名
	m_strDownloadUrl = m_strVersionUrl;
	m_strIssueName= m_strDownloadUrl.Right(m_strDownloadUrl.GetLength() - m_strDownloadUrl.ReverseFind('/') - 1);

	//获取下载路径和本地文件名
	char tempDownLoadPath[255];
	::GetTempPath(sizeof(tempDownLoadPath), tempDownLoadPath);
	CreateDirectory(tempDownLoadPath, NULL);
	m_strDownloadFile.Format("%s\%s", tempDownLoadPath, m_strIssueName);

	//开始下载
	StartDownload();

	
}

LRESULT CVersionUpdateDlgDui::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}
	if(WM_DOWNLOAD_NOTIFY == uMsg)
	{
		OnDownLoadNotify(wParam, lParam);
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);

}

void CVersionUpdateDlgDui::OnBnClickedInstall()
{
	ShellExecute(this->m_hWnd, "open", this->m_strDownloadFile, "", "", SW_SHOW );
	Close(IDOK);
}


bool CVersionUpdateDlgDui::StartDownload()
{
	TerminateDownload();

	DWORD dwThreadId ;
	m_hThread = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)DownloadProc,
		(LPVOID)this,
		0,
		&dwThreadId);

	return true;
}



bool CVersionUpdateDlgDui::TerminateDownload()
{
	if ( m_hThread )
	{
		if ( m_bStopDownload == false )
		{
			m_bStopDownload = true;
			Sleep(300);
		}

		if ( m_hThread )
		{            
			try{
				TerminateThread(m_hThread,0);
				CloseHandle(m_hThread);
			}
			catch (...) {
			}

			m_hThread = NULL;
		}
	}

	return true;
}


LRESULT CVersionUpdateDlgDui::OnDownLoadNotify(WPARAM w,LPARAM l)
{
	switch ( l )
	{
		//下载完成
		case DOWNLOAD_FINISHED:
		{
			m_pLblDownloadRate->SetText("总进度: 100%");
			m_pPrgDownload->SetValue(100);
			m_pBtnInstall->SetEnabled(true);
			break;
		}
		case DOWNLOAD_FAILED:
		{
			static int n = 0;
			if ( n++ < 2 )
			{
				StartDownload();
			}
			else
			{
				CMessageBox::Model(m_hWnd, "自动更新竟然失败了,您可去im.feiniu.com下载最新版本哞哞","哞哞更新", MB_OKCANCEL/*|MB_ICONINFORMATION*/ );
				Close(IDOK);

			}
		
			break;
		}
		case 3:
			break;
		case DOWNLOAD_RECVING:
			{
				DWORD dwRead = w;
				CString str;

				if ( m_dwPreTick!=0 )
				{
					DWORD dwTick = GetTickCount();
					static int nDownloaded=0;
					if ( dwTick - m_dwPreTick > 1000 )
					{
						//下载进度
						int nPer = 100 * ( m_dwDownLoadSize /1024 ) / m_dwRealFileSize ;
 						str.Format("总进度: %d%%",nPer);
						m_pLblDownloadRate->SetText(str);
 						m_pPrgDownload->SetValue(nPer);
						m_dwPreTick = dwTick;
						nDownloaded = 0;
					}
					else
					{
						nDownloaded += dwRead;
					}				
				}
				else
				{
					m_dwPreTick = GetTickCount();
				}
			
			}
		default:
			break;
	}

	return 1;
}


// CVersionUpdatgeDlg 消息处理程序
UINT CVersionUpdateDlgDui::DownloadProc(LPVOID param)
{
	CVersionUpdateDlgDui *pClass = (CVersionUpdateDlgDui *)(param);

	bool bGetDownloadInfo=false;
	
	HINTERNET hFile, hInet;
	char buffer[1024];
	DWORD dwRead, dwSize;

//下载地址
#ifdef _CS_PLAT_EX_
	#if defined _VER_PREVIEW_
		hInet = InternetOpen("xTreamLiveUpdate", INTERNET_OPEN_TYPE_PROXY, PROXY_URL, NULL, 0);
	#elif defined _VER_ONLINE_
		hInet = InternetOpen("xTreamLiveUpdate", INTERNET_OPEN_TYPE_PROXY, PROXY_URL, NULL, 0);
	#else
		hInet = InternetOpen("xTreamLiveUpdate",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);
	#endif
#else
 	#if defined _VER_PREVIEW_
 		hInet = InternetOpen("xTreamLiveUpdate", INTERNET_OPEN_TYPE_PROXY, PROXY_URL, NULL, 0);
 	#else
		hInet = InternetOpen("xTreamLiveUpdate",INTERNET_OPEN_TYPE_PRECONFIG,"",NULL,0);
	#endif
#endif

	if ( hInet != NULL )
	{
		hFile = InternetOpenUrl(hInet, pClass->m_strDownloadUrl, NULL, 0, 
				INTERNET_FLAG_RELOAD | 
				INTERNET_FLAG_NO_CACHE_WRITE |
				INTERNET_FLAG_EXISTING_CONNECT | 
				INTERNET_FLAG_NO_COOKIES,
				0
			);

		if ( hFile!=NULL )
		{
			char  pcBuffer[255];
			DWORD dwLength = 255;
			DWORD dwIndex = 0;

			DWORD dwStatus = 0;

			if ( HttpQueryInfo(hFile,HTTP_QUERY_STATUS_CODE,pcBuffer,&dwLength,&dwIndex) )
			{
				dwStatus = atoi(pcBuffer);
			}
			//要下载的文件存在
			if ( dwStatus == 200 ) 
			{
				CStdioFile fileDownload;
				DWORD dwBufferSize=sizeof(buffer);
				//获取下载文件的大小
				if ( HttpQueryInfo(hFile, HTTP_QUERY_CONTENT_LENGTH, buffer, &dwBufferSize ,NULL ) )
				{
					pClass->m_dwRealFileSize = atoi(buffer)/1024.0;
				}
				else
				{
					pClass->m_dwRealFileSize = atoi(pClass->m_strSize);
				}
					
				pClass->m_dwDownLoadSize = 0; /*单位是byte*/
				bool bOpened = fileDownload.Open(pClass->m_strDownloadFile,CFile::modeCreate|CFile::modeWrite|CFile::typeBinary);
				try
				{
					dwSize = sizeof(buffer);

					int count=0;
					while ( pClass->m_bStopDownload == false && InternetReadFile(hFile, buffer, dwSize, &dwRead ) && dwRead > 0 )
					{
						if ( bOpened )
						{
							fileDownload.Write(buffer,dwRead);
							pClass->m_dwDownLoadSize += dwRead;
						}
						count++;

						::PostMessage(pClass->m_hWnd, WM_DOWNLOAD_NOTIFY, dwRead, DOWNLOAD_RECVING);
						if(dwRead <1024)
							dwRead = dwRead;
					}
				}
				catch(...)
				{
				}

				if ( bOpened )
					fileDownload.Close();

				if ( pClass->m_bStopDownload==false )
					::PostMessage(pClass->m_hWnd, WM_DOWNLOAD_NOTIFY, NULL, DOWNLOAD_FINISHED);
			}
			else
			{
				::PostMessage(pClass->m_hWnd, WM_DOWNLOAD_NOTIFY, NULL, DOWNLOAD_FAILED);
			}

			InternetCloseHandle(hFile);
		}
        
		InternetCloseHandle(hInet);
	}

END:

	::CoUninitialize();

	pClass->m_bStopDownload=false;
	CloseHandle(pClass->m_hThread);
	pClass->m_hThread = NULL;

	return 0;
}

