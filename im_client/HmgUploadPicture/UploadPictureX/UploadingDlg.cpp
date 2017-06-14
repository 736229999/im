// UploadingDlg.cpp : 实现文件
//

#include "..\stdafx.h"
#include "..\HmgUploadPicture.h"
#include "UploadingDlg.h"
#include ".\uploadingdlg.h"
#include "WebProcess.h"

// CUploadingDlg 对话框

IMPLEMENT_DYNAMIC(CUploadingDlg, CDialog)
CUploadingDlg::CUploadingDlg(_lstUpFileINFO & lstUpFileINFO, CWnd* pParent /*=NULL*/)
	: CDialog(CUploadingDlg::IDD, pParent),m_lstUpLoadFleINFO(lstUpFileINFO)
{
	m_bStopUploadingThread = FALSE;
	m_CursorType = CURSOR_TYPE_NORMAL;
	m_strWindowText.Format(_T("图片上传"));
}

CUploadingDlg::~CUploadingDlg()
{
}

void CUploadingDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_DLG_UPLOADING_STATIC_PROMPT01, m_strProgressText);
	DDX_Text(pDX, IDC_DLG_UPLOADING_STATIC_PROMP02, m_strCurPicName);
	DDX_Control(pDX, IDC_DLG_UPLOADING_PROGRESSCTRL, m_wndProgressCtrl);
}


BEGIN_MESSAGE_MAP(CUploadingDlg, CDialog)
	ON_WM_DESTROY()
	ON_WM_TIMER()
	ON_MESSAGE(WM_UPLOADING_HTHREAD_MSG, OnMsgOfUploadingThread)
	ON_BN_CLICKED(IDC_DLG_UPLOADING_CANCEL, OnUploadingCancel)
	ON_WM_SETFOCUS()
	ON_WM_SETCURSOR()
END_MESSAGE_MAP()


// CUploadingDlg 消息处理程序

BOOL CUploadingDlg::OnInitDialog()
{
	CDialog::OnInitDialog();
 
	m_hIcon = AfxGetApp() ->LoadIcon(IDI_MAIN);
	SetIcon(m_hIcon, FALSE);	SetIcon(m_hIcon, TRUE);

	m_CursorType = CURSOR_TYPE_WAIT;
	SetWindowText(m_strWindowText);
	
	m_hWndProgressText = ::GetDlgItem(m_hWnd, IDC_DLG_UPLOADING_STATIC_PROMPT01);
	m_hWndCurPicName = ::GetDlgItem(m_hWnd, IDC_DLG_UPLOADING_STATIC_PROMP02);
	m_hWndProgressCtrl = ::GetDlgItem(m_hWnd, IDC_DLG_UPLOADING_PROGRESSCTRL);
	
	m_wndProgressCtrl.SendMessage (PBM_SETBARCOLOR, 0, (LPARAM) RGB (0, 0, 255));
	m_wndProgressCtrl.SendMessage (PBM_SETBKCOLOR, 0, (LPARAM) RGB (255, 255, 255));
	
	SetTimer(11, 500, NULL);

	return TRUE;
}

void CUploadingDlg::OnDestroy()
{
	CDialog::OnDestroy();
}

void CUploadingDlg::OnOK()
{
 // ...
}

void CUploadingDlg::OnCancel()
{ 
//	m_bStopUploadingThread = TRUE;
//	CDialog::OnCancel();
}

void CUploadingDlg::OnUploadingCancel()
{
	if(MessageBox(_T("确定需要取消当前图片上传吗?\t\t"), m_strWindowText, MB_ICONQUESTION | MB_YESNO) == IDNO)
	{
		return ;
	}

	m_bStopUploadingThread = TRUE;
	
	m_CursorType = CURSOR_TYPE_WAIT;

	CWnd *pWnd = GetDlgItem(IDC_DLG_UPLOADING_STATIC_CANCEL);
	if(pWnd != NULL)	pWnd ->ShowWindow(SW_SHOW);

	pWnd = GetDlgItem(IDC_DLG_UPLOADING_CANCEL);
	if(pWnd != NULL)	pWnd ->EnableWindow(FALSE);
}

UINT CUploadingDlg::UploadThreadProc( LPVOID pParam )
{
	try
	{
		CUploadingDlg *pDlg = (CUploadingDlg *)pParam;

		CoInitialize(NULL);

		if(pDlg != NULL)	pDlg ->UploadFile_run();

		CoUninitialize();
	}
	catch (...)
	{
	}

	return 0;
}

LRESULT CUploadingDlg::OnMsgOfUploadingThread(WPARAM wParam, LPARAM lParam)
{
	switch(wParam)
	{
	case UPLOADING_THREAD_NOTIFIY_PROGRESS:
		break;
	case UPLOADING_THREAD_NOTIFIY_UPLOAD_FINISHED:
		{
			m_CursorType = CURSOR_TYPE_WAIT;

			if ( ::g_AppConfigInfo.bCalledByShellExt )
			{
				CString strAlbum;
				strAlbum.Format("http://%s.woku.com/album/0.html",g_AppConfigInfo.userinfo.strUserName);
				ShellExecute(NULL,"open",strAlbum,NULL,NULL,SW_SHOW);
			}

			Sleep(200);
			CDialog::OnOK();
		}
		break;
	default:	break;
	}

	return 0;
}

int CUploadingDlg::UploadFile_run(void)
{
	m_CursorType = CURSOR_TYPE_WAIT;

	CString         strErrorInfoPrompt, strTemp;

	if(m_bStopUploadingThread)	
	{
		PostMessage(WM_UPLOADING_HTHREAD_MSG, UPLOADING_THREAD_NOTIFIY_UPLOAD_FINISHED, 0);
		return 0;
	}

	int nCurIndex = 0;
	CStringList	strFileUploadedlst;
	
	const int nUploadlstCount = (int)m_lstUpLoadFleINFO.size();

	m_wndProgressCtrl.SetRange(0, nUploadlstCount);

	for(_lstUpFileINFO_iterator _it = m_lstUpLoadFleINFO.begin(); 
			_it != m_lstUpLoadFleINFO.end() && m_bStopUploadingThread == FALSE; ++ _it, nCurIndex ++)
	{
		tagUpFileInfo _UpFileINFO;

		_UpFileINFO = (*_it);

		m_strProgressText.Format("已上传图片文件数/总数: %d/%d\0", nCurIndex + 1, nUploadlstCount);
		::SetDlgItemText(m_hWnd, IDC_DLG_UPLOADING_STATIC_PROMPT01, m_strProgressText);

		m_strCurPicName.Format("正在压缩图片: %s\0", _UpFileINFO.strDescription);
		::SetDlgItemText(m_hWnd, IDC_DLG_UPLOADING_STATIC_PROMP02, m_strCurPicName);
	
		if(!CompressFileIfNeed(&_UpFileINFO, g_AppConfigInfo.nLimittedMaxSideSize))
		{

			continue ;
		}
	
		m_strCurPicName.Format("当前图片: %s\0", _UpFileINFO.strDescription);
		::SetDlgItemText(m_hWnd, IDC_DLG_UPLOADING_STATIC_PROMP02, m_strCurPicName);


		CString strFile;

		if(_UpFileINFO.bIsChanged == FALSE)
			strFile = _UpFileINFO.strFileDir + "\\" + _UpFileINFO.strFileName;
		else
			strFile = _UpFileINFO.strGeneratedTmpFilePath;

		if(m_bStopUploadingThread)	break;
		
		CString strError;
		
		bool bResult = 	UploadFile( _UpFileINFO, strError);

		if(bResult == true)
		{
			(*_it).bRemoveIt = TRUE;
		}
		else
		{
			CString strErrorDes;
			strErrorDes.Format("图片%d - %s;", nCurIndex + 1, _UpFileINFO.strSourceFileName);

			if(strError.GetLength() > 0)
			{
				strTemp.Format("原因:%s.\r\n", strError);
				strErrorDes += strTemp;
			}
			else
			{
				strErrorDes += _T("\r\n");
			}
			
			strErrorInfoPrompt += strErrorDes;

		} // end if (UploadFile ...
	
			
		m_wndProgressCtrl.SetPos(nCurIndex + 1);
			
		if(m_bStopUploadingThread)				
			break;

	} // end for

	m_CursorType = CURSOR_TYPE_NORMAL;
	
	HWND hWnd = ::GetDlgItem(m_hWnd, IDC_DLG_UPLOADING_CANCEL);	
	if(::IsWindow(hWnd)) ::EnableWindow(hWnd, FALSE);
	
	if(m_bStopUploadingThread == FALSE)
	{
		SetForegroundWindow();
		if(strErrorInfoPrompt.GetLength() > 0)
		{
			strTemp.Format("对不起,下列图片上传失败,请您稍候再试!\r\n\r\n%s", strErrorInfoPrompt);
			MessageBox(strTemp, m_strWindowText, MB_ICONINFORMATION);
		}
		//else
		//{
		//	MessageBox("当前图片上传成功!\t\t", m_strWindowText, MB_ICONINFORMATION);
		//}
	} // end if
	
	m_CursorType = CURSOR_TYPE_WAIT;
	PostMessage(WM_UPLOADING_HTHREAD_MSG, UPLOADING_THREAD_NOTIFIY_UPLOAD_FINISHED, 0);
	return 0;
}

void CUploadingDlg::OnTimer(UINT nIDEvent)
{
	KillTimer(nIDEvent);
	CDialog::OnTimer(nIDEvent);

	DWORD dwThreadId = 0;

	HANDLE hUploadingThread	= CreateThread(NULL,0,	(LPTHREAD_START_ROUTINE)UploadThreadProc,this,
		0,&dwThreadId);
}

BOOL CUploadingDlg::OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message)
{
	if(m_CursorType == CURSOR_TYPE_WAIT)
	{
		//	SetCursor(AfxGetApp() ->LoadStandardCursor(MAKEINTRESOURCE(IDC_WAIT)));
		//	return TRUE;


		CRect rc;
		GetClientRect(&rc);

		CPoint pt;	GetCursorPos(&pt);
		ScreenToClient(&pt);
		CRect rcHalf(rc.top, rc.left, rc.right, rc.bottom -  rc.bottom / 3);

		if(rcHalf.PtInRect(pt))
		{
			SetCursor(AfxGetApp() ->LoadStandardCursor(MAKEINTRESOURCE(IDC_WAIT)));
			return TRUE;
		}
	}

	return CDialog::OnSetCursor(pWnd, nHitTest, message);
}


bool CUploadingDlg::UploadFile(const tagUpFileInfo & _UpFileINFO, CString & strError)
{
	CString strFile;
	CString strPictureUrl;

	if(_UpFileINFO.bIsChanged == FALSE)
		strFile.Format("%s\\%s\0",  _UpFileINFO.strFileDir, _UpFileINFO.strFileName);
	else
		strFile = _UpFileINFO.strGeneratedTmpFilePath;

	return UploadPicture(	strFile,
							_UpFileINFO.strDescription,
							g_AppConfigInfo.upfileprop.strTag,
							g_AppConfigInfo.upfileprop.strDescription,
							g_AppConfigInfo.upfileprop.iViewProp,
							g_AppConfigInfo.upfileprop.iAlbumDirId,
							strPictureUrl,
							strError
							);
}

BOOL CUploadingDlg::CompressFileIfNeed(tagUpFileInfo * pFile, const int nMaxSideSize)
{
	CString strSoruceFilePathName;
	strSoruceFilePathName.Format("%s\\%s", pFile ->strFileDir, pFile ->strFileName);

	if(g_AppConfigInfo.bCalledByShellExt == TRUE)
	{
		strSoruceFilePathName.Format("%s%s", pFile ->strFileDir, pFile ->strFileName);

		CComBSTR bst(strSoruceFilePathName);
		Bitmap image(bst.m_str);
		
		if(image.GetFlags() == ImageFlagsNone)
		{
			return FALSE;
		}
	
		pFile ->nSoruceImageHeight = image.GetHeight();
		pFile ->nSoruceImageWidth  = image.GetWidth();
	
	} // end if

	if(nMaxSideSize < (int)400)
		return TRUE;
	const int _nSoruceMaxSideSize =  max(pFile ->nSoruceImageHeight, pFile ->nSoruceImageWidth);
	if(_nSoruceMaxSideSize > nMaxSideSize)
	{
	//	PostMessage(WM_THREAD_NOTIFY, THREAD_NOTIFY_MSG_COMPRESSING_FILE, 0);
	
		const float fltScale =  (float)nMaxSideSize  / (float)_nSoruceMaxSideSize;

		const int nNewWidth  =  pFile ->nSoruceImageWidth * fltScale;
		const int nNewHeight = pFile ->nSoruceImageHeight * fltScale;

		CDC *pDC = CWnd::GetDesktopWindow() ->GetDC();

		CDC dcMem;
		CBitmap bmpMem;

		dcMem.CreateCompatibleDC(pDC);
		bmpMem.CreateCompatibleBitmap(pDC, nNewWidth, nNewHeight);

		CBitmap *pOldBitmap = dcMem.SelectObject(&bmpMem);

		Graphics g(dcMem.m_hDC);

		Bitmap * bitmap = Bitmap::FromFile(strSoruceFilePathName.AllocSysString());
		g.DrawImage(bitmap, 0, 0, nNewWidth, nNewHeight);
		delete bitmap;

		CImage image;
		image.Attach((HBITMAP)bmpMem.m_hObject);

		CString strFileName;
		CString strNewFilePathName;

		strFileName.Format("tmp_New_%s", pFile ->strFileName);
		strNewFilePathName.Format("%s\\%s\0", _get_tmp_directroy(), strFileName);

		::DeleteFile(strNewFilePathName);

		image.Save(strNewFilePathName);
		image.Detach();
		image.ReleaseGDIPlus();

		dcMem.SelectObject(pOldBitmap);
		CWnd::GetDesktopWindow() ->ReleaseDC (pDC);

		(*pFile).bIsChanged = TRUE;
		(*pFile).strGeneratedTmpFilePath = strNewFilePathName;

		(*pFile).nImageHeight = nNewHeight;
		(*pFile).nImageWidth  = nNewWidth;

		(*pFile).ulFileSize = GetFileSize(strNewFilePathName);

		CWnd::GetDesktopWindow() ->ReleaseDC(pDC);
	} // end if(_nSoruceMaxSideSize > nMaxSideSize)

	return TRUE;
}