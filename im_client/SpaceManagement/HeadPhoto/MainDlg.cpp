#include "..\mypictures\maindlg.h"
// MainDlg.cpp : 实现文件
//
// im\HeadPhotoDlg.cpp : 实现文件
//

#include "stdafx.h"
#include ".\maindlg.h"

#include "WebBusiness.h"
#include "WebInterfaceWrapper.h"

CMainDlg * CMainDlg::pInstance = NULL;

IMPLEMENT_DYNAMIC(CMainDlg, CXSkinDlg)
CMainDlg::CMainDlg(CWnd* pParent /*=NULL*/)
: CXSkinDlg(CMainDlg::IDD, pParent)
{
	m_hThread      = NULL;
	m_pWndProgress = NULL;

	Create(IDD,pParent);
}

CMainDlg::~CMainDlg()
{
	StopUpload();
	pInstance = NULL;
}

void CMainDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_EXPLORER_WEB_RESOURCE, this->m_wndIEFrame);
	DDX_Control(pDX, IDC_HPACTXCTRL, m_wndHeadPhotoOcx);
}



BEGIN_MESSAGE_MAP(CMainDlg, CXSkinDlg)
	ON_WM_NCDESTROY()
	ON_WM_CLOSE()
	ON_MESSAGE(WM_THREAD_NOTIFY,OnUploadFinished)
	ON_MESSAGE(WM_CANCEL_NOTIFY,OnUploadCancel)
END_MESSAGE_MAP()


void CMainDlg::Show()
{
	if ( pInstance == NULL )
	{
		pInstance = new CMainDlg(GetDesktopWindow());
		//pInstance = new CMainDlg(::AfxGetApp()->GetMainWnd());
		pInstance->ShowWindow(SW_SHOW);
		pInstance->CenterWindow();
	}
	else
	{
		if ( pInstance->IsWindowVisible()==FALSE)
			pInstance->ShowWindow(SW_SHOW);

		if ( pInstance->IsIconic() )
			pInstance->ShowWindow(SW_RESTORE);

		pInstance->BringWindowToTop();
	}
}

void CMainDlg::Close()
{
	if ( pInstance )
		pInstance->DestroyWindow();
}
// CMainDlg 消息处理程序

BOOL CMainDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("Mini - 大头贴");
	SetDlgStyle(MINCLOSE);
	EnableHelp(true);

	MoveWindow(0,0,643,505);
	m_wndHeadPhotoOcx.MoveWindow(3,25,640,480);
	m_wndIEFrame.MoveWindow(270,30,363,470);
	this->SetTitleIcon(GetResDLLHandle(),IDI_BIGHEAD);

	m_wndIEFrame.Navigate(g_pWebConfig->GetWebPageUrl(PHOTO_FRAME,L""),NULL,NULL,NULL,NULL);

	return TRUE;  
}
BEGIN_EVENTSINK_MAP(CMainDlg, CXSkinDlg)
	ON_EVENT(CMainDlg, IDC_EXPLORER_WEB_RESOURCE, 250, BeforeNavigate2ExplorerWebResource, VTS_DISPATCH VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PVARIANT VTS_PBOOL)
	ON_EVENT(CMainDlg, IDC_HPACTXCTRL, 2, OnUploadPictureHpactxctrlControl, VTS_PI1)
	ON_EVENT(CMainDlg, IDC_HPACTXCTRL, 1, OnUploadWebiconHpactxctrlControl, VTS_PI1)
	ON_EVENT(CMainDlg, IDC_HPACTXCTRL, 3, OnBeforeSaveFileHpactxctrlControl, VTS_PI1)
	ON_EVENT(CMainDlg, IDC_HPACTXCTRL, 4, OnBeforePrintHpactxctrlControl, VTS_PI1)
END_EVENTSINK_MAP()


void CMainDlg::BeforeNavigate2ExplorerWebResource(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel)
{
	// TODO: 在此处添加消息处理程序代码
	CString strLink(URL->bstrVal);

	int iPos1 = strLink.Find("((");
	int iPos2 = strLink.Find("))");

	if ( iPos2>iPos1 && iPos1>-1 )
	{
		CString strName, strId, strPrice, strUrl ;

		int iDot1 = strLink.Find(",");

		strName = strLink.Mid(iPos1+2,iDot1-iPos1-2);

		int iDot2 = strLink.Find(",",iDot1+1);

		strId     = strLink.Mid(iDot1+1,iDot2-iDot1-1);

		int iDot3 = strLink.Find(",",iDot2+1);

		strPrice  = strLink.Mid(iDot2+1,iDot3-iDot2-1);

		strUrl    = strLink.Mid(iDot3+1,iPos2-iDot3-1);

		CString strType = strUrl.Right(4);
		strType.MakeLower();

		*Cancel =  TRUE;

		if( strType == ".bmp" ||
			strType == ".gif" ||
			strType == ".png" ||
			strType == ".tif"  )
		{
			AddPhotoFrameMap(atoi((LPCSTR)strId),atof((LPCSTR)strPrice),strName);
			m_wndHeadPhotoOcx.SetFrameURL(strUrl,strId);
		}
	}
}

void CMainDlg::OnNcDestroy()
{
	CXSkinDlg::OnNcDestroy();
	delete this;
	pInstance = NULL;
}

void CMainDlg::OnClose()
{
	DestroyWindow();
}

void CMainDlg::StopUpload()
{
	if ( m_hThread )
	{
		try
		{
			TerminateThread(m_hThread,0);
			CloseHandle(m_hThread);
		}
		catch(...)
		{
		}

		m_hThread = NULL;
	}

	if ( m_pWndProgress )
	{
		m_pWndProgress->DestroyWindow();
		delete m_pWndProgress;
	}

	m_pWndProgress = NULL;
}

UINT CMainDlg::ThreadProc(LPVOID param)
{
	CoInitialize(0);

	BOOL bRet =FALSE ;

	static CString strPicUrl, strError;

	CWebInterfaceWrapper objWI;
	objWI.InitInterface(g_pUserInfo->GetUserID(),(LPCSTR)g_pUserInfo->GetUserName(),(LPCSTR)g_pUserInfo->GetNickName());

	if ( pInstance && pInstance->m_hWnd )
	{
		if ( pInstance->m_iUploadType==0 )
			bRet = objWI.UploadFace((LPCSTR)param,strPicUrl,strError);//UploadFace((LPCSTR)param,g_pUserInfo,strPicUrl,strError );
		else
		if ( pInstance->m_iUploadType==1 )
			bRet = objWI.UploadPicture((LPCSTR)param,"","","",3,0,strPicUrl,strError);//UploadPicture((LPCSTR)param,g_pUserInfo,3,9,strPicUrl,strError );
	}

	::CoUninitialize();

	if ( bRet )//success
	{
		pInstance->PostMessage(WM_THREAD_NOTIFY,(WPARAM)bRet,(LPARAM)(LPCSTR)strPicUrl);
	}
	else
	{
		pInstance->PostMessage(WM_THREAD_NOTIFY,(WPARAM)bRet,(LPARAM)(LPCSTR)strError);
	}

	return 0;
}

LRESULT CMainDlg::OnUploadFinished(WPARAM w,LPARAM l)
{
	BOOL bUpload = (BOOL)w;

	UnLockOperation();

	if ( m_hThread )
	{
		CloseHandle(m_hThread);
	}

	m_hThread = NULL;

	StopUpload();

	if ( bUpload )
	{
		PayPhotoFrame();

		//更改个人形象成功
		if ( m_iUploadType==0 )
		{
			CString strFaceUrl = (LPCSTR)l;
			g_pUserInfo->PutFaceUrl((LPCSTR)strFaceUrl);
			g_pUserInfo->PutFaceID(0xFF);

			::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_HeadPhoto,0);
		}
		else
		{
			//ShowAlbum((char *)(LPCSTR)g_pUserInfo->GetUserName());
			CString strUrl;
			strUrl.Format("http://%s.woku.com/album/0.html",(char *)(LPCSTR)g_pUserInfo->GetUserName());
			MiniOpenUrl(strUrl);
			ShowMessage("上传到个人相册成功！");
		}
	}
	else
	{
		ShowMessage((LPCSTR)l);
	}

	return 0;
}

LRESULT CMainDlg::OnUploadCancel(WPARAM w,LPARAM l)
{
	StopUpload();

	this->UnLockOperation();

	return 0;
}

void CMainDlg::OnUploadPictureHpactxctrlControl(signed char* szPictureFile)
{
	static CString str = "";

	str=(char *)szPictureFile;

	if ( BuyPhotoFrame()==false )
	{
		return ;
	}

	m_iUploadType = 1;

	StopUpload();

	DWORD dwThreadId ;

	m_hThread = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID)(LPCSTR)str,
		0,
		&dwThreadId);

	if ( m_hThread )
	{
		CFormProgress3 *pProgress = new CFormProgress3(m_hWnd,this);
		pProgress->SetTip("正在上传大头贴...");
		pProgress->ShowWindow(SW_SHOW);
		pProgress->CenterWindow(this);

		this->m_pWndProgress = pProgress;

		LockOperation();
	}
}

void CMainDlg::OnUploadWebiconHpactxctrlControl(signed char* szPictureFile)
{
	static CString str = "";

	str = (char *)szPictureFile;

	m_iUploadType = 0;

	if ( BuyPhotoFrame()==false )
	{
		return ;
	}

	StopUpload();

	DWORD dwThreadId ;

	m_hThread = CreateThread(NULL,0,
		(LPTHREAD_START_ROUTINE)ThreadProc,
		(LPVOID)(LPCSTR)str,
		0,
		&dwThreadId);

	if ( m_hThread )
	{
		if ( m_pWndProgress )
		{
			m_pWndProgress->DestroyWindow();
			delete m_pWndProgress;
			m_pWndProgress=NULL;
		}

		CFormProgress3 *pProgress = new CFormProgress3(m_hWnd,this);
		pProgress->SetTip("正在设置个人形象...");
		pProgress->ShowWindow(SW_SHOW);
		pProgress->CenterWindow(this);

		this->m_pWndProgress = pProgress;

		LockOperation();
	}
}


void CMainDlg::OnBeforeSaveFileHpactxctrlControl(signed char* szFile)
{
	if ( BuyPhotoFrame()==false )
	{
		this->m_wndHeadPhotoOcx.DisableSave();
	}
	else
	{
		PayPhotoFrame();
	}
}

void CMainDlg::OnBeforePrintHpactxctrlControl(signed char* szFile)
{
	if ( BuyPhotoFrame()==false )
	{
		this->m_wndHeadPhotoOcx.DisablePrint();
	}
}

void CMainDlg::AddPhotoFrameMap(int id, float fPrice, CString str)
{
	map<int,tagPhotoFrame>::iterator it = m_mapFrame.find(id);

	if ( it==m_mapFrame.end())
	{
		tagPhotoFrame photoFrame;
		photoFrame.fPrice=fPrice;
		strcpy(photoFrame.szName,(LPCSTR)str);

		m_mapFrame.insert(map<int,tagPhotoFrame>::value_type(id,photoFrame));
	}
}

bool CMainDlg::GetPhotoFrame(int id, tagPhotoFrame &photoFrame)
{
	map<int,tagPhotoFrame>::iterator it = m_mapFrame.find(id);

	if ( it!=m_mapFrame.end())
	{
		photoFrame = it->second;
		return true;
	}
	else
	{
		return false;
	}
}

void CMainDlg::OnHelpClick()
{
	OpenHtmlPage("www.feiniu.com");
}

void CMainDlg::PayPhotoFrame()
{
	//CString str = m_wndHeadPhotoOcx.GetFrameID();
	//int nFrameId = atoi((LPCSTR)str);
	int nFrameId = atoi((LPCSTR)m_wndHeadPhotoOcx.GetFrameID());

	tagPhotoFrame photoFrame;

	if ( GetPhotoFrame(nFrameId,photoFrame) )
	{
		CString str;

		if ( photoFrame.fPrice>0.0 )
		{
			::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_HeadPhoto,2);//begin to pay

			struct tagPrice
			{
				float fPrice;
				long  nProductId;
				long  nType;
			} tP;

			tP.fPrice = photoFrame.fPrice;
			tP.nProductId = nFrameId;
			tP.nType = 130;

			LRESULT lRet=::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_HeadPhoto,(LPARAM)&tP);// tell the price
            
			if ( lRet==0 )
			{
				float      fRemain = g_pUserInfo->GetGoldMoney()-photoFrame.fPrice;

				g_pUserInfo->PutGoldMoney(fRemain);

				str.Format("购买成功！您还有 %.2f 个金币！",(float)fRemain);
				MessageBox(str,"提示",MB_OK|MB_ICONINFORMATION);

				::SendMessage(g_hMessageWnd,WM_MODULE_NOTIFY,MODULE_HeadPhoto,1);	
			}
			else
			{
				MessageBox((LPCSTR)lRet,"提示",MB_OK|MB_ICONINFORMATION);
			}
		}		
	}
}

bool CMainDlg::BuyPhotoFrame()
{
	CString str = m_wndHeadPhotoOcx.GetFrameID();
	int nFrameId = atoi((LPCSTR)str);
	// int nFrameId = atoi((LPCSTR)m_wndHeadPhotoOcx.GetFrameID());

	tagPhotoFrame photoFrame;

	if ( GetPhotoFrame(nFrameId,photoFrame) )
	{
		CString str;

		float fRemain = g_pUserInfo->GetGoldMoney()-photoFrame.fPrice;

		if ( photoFrame.fPrice>0.000 )
		{
			if (  fRemain>-0.0001 )
			{
				str.Format("您需要支付 %.2f 金币才能使用相框 \"%s\"!",photoFrame.fPrice,photoFrame.szName);

				if ( MessageBox(str,"购买相框",MB_OKCANCEL|MB_ICONINFORMATION)!=IDOK )
				{
					return false;
				}
			}
			else
			{
				ShowMessage("对不起,你的余额太少!");
				return false;
			}
		}
		else
		{
			return true;
		}
	}

	return true;
}

