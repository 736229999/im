// SysCommonFile.cpp : 实现文件
//

#include "stdafx.h"
#include "SysCommonFile.h"
#include "afxdialogex.h"
#include "..\Im\LocalData.h"
using namespace DuiLib; 

// 计算文件大小(以MB为单位)
double GetFileSize_MB(const char *pFilePath)
{
	FILE * pFile;
	long size;

	pFile = fopen (pFilePath,"rb");
	if (pFile!=NULL)
	{
		fseek (pFile, 0, SEEK_END);   ///将文件指针移动文件结尾
		size=ftell (pFile); ///求出当前文件指针距离文件开始的字节数
		fclose (pFile);
		return size/1024.0/1024.0;
	}
	return 0;
}

// 获取目录下所有文件
void GetDirAllFile(CString strDir, vector<string> &vecAllFile)
{
	if(strDir == _T(""))
	{
		return;
	}
	else
	{
		if(strDir.Right(1) != _T("//"))
			strDir += L"//";
		strDir =strDir+_T("*.*");
	}
	CFileFind finder;
	CString strPath;
	BOOL bWorking = finder.FindFile(strDir);
	while(bWorking)
	{
		bWorking = finder.FindNextFile();
		strPath = finder.GetFilePath();
		if(finder.IsDirectory() && !finder.IsDots())
			GetDirAllFile(strPath, vecAllFile); //递归调用
		else if(!finder.IsDirectory() && !finder.IsDots())
		{
			vecAllFile.push_back((LPCTSTR)strPath);
		}
	}
}

void CSysCommonFile::InitWindow()
{
	WindowImplBase::InitWindow();
	m_pEditRecvPath = static_cast<CEditUI*>(m_PaintManager.FindControl("LogSavePath")); ////路径填充
	m_pEditRecvPath->SetReadOnly(TRUE);
	m_pBtnModDir=  static_cast<CButtonUI*>(m_PaintManager.FindControl("ChangeDir")); ////更改目录
	m_pBtnOPenFolder=  static_cast<CButtonUI*>(m_PaintManager.FindControl("OpenFolder"));   ////打开文件夹
	m_pBtnClear=  static_cast<CButtonUI*>(m_PaintManager.FindControl("clearbtn"));   ////清理垃圾

    m_pCheckBuffer =  static_cast<COptionUI*>(m_PaintManager.FindControl("FileBuf"));////缓存
	m_pCheckClusterPic=  static_cast<COptionUI*>(m_PaintManager.FindControl("ClusterPic"));  ////群
	m_pCheckContact=  static_cast<COptionUI*>(m_PaintManager.FindControl("CustPic"));  ////联系人
	m_pCheckRecv=  static_cast<COptionUI*>(m_PaintManager.FindControl("RecvFile"));  ////接收到

	/////查看
	//m_pBtnBuffer=  static_cast<CButtonUI*>(m_PaintManager.FindControl("viewbufferbtn")); ////查看缓存
	m_pBtnClusterPic=  static_cast<CButtonUI*>(m_PaintManager.FindControl("viewClusterBufbtn"));////查看群
	m_pBtnSysContact=  static_cast<CButtonUI*>(m_PaintManager.FindControl("viewCustPicbtn"));  ////查看联系人
	m_pBtnRecv= static_cast<CButtonUI*>(m_PaintManager.FindControl("viewRecvFilebtn")); ////查看接收到的文件 

	/////文件个数
	m_pLabelBufNum=  static_cast<CLabelUI*>(m_PaintManager.FindControl("FileBufNum")); ////缓存
	m_pLabelClusterPicNum=  static_cast<CLabelUI*>(m_PaintManager.FindControl("ClusterPicNum"));    ////群图片
	m_pLabelContactNum=  static_cast<CLabelUI*>(m_PaintManager.FindControl("CustPicNum"));  ////联系人
	m_pLabelRecvNum=  static_cast<CLabelUI*>(m_PaintManager.FindControl("RecvFileNum")); ////接受到的文件

	////垃圾大小
	m_pLabelBufSize=  static_cast<CLabelUI*>(m_PaintManager.FindControl("FileBufSize")); ////缓存垃圾
	m_pLabelClusterPicSize=  static_cast<CLabelUI*>(m_PaintManager.FindControl("ClusterPicSize"));   ////群图片垃圾
	m_pLabelContactSize=  static_cast<CLabelUI*>(m_PaintManager.FindControl("CustPicSize"));  ////联系人垃圾
	m_pLabelRecvSize=  static_cast<CLabelUI*>(m_PaintManager.FindControl("RecvFileSize"));  ////接受到的文件垃圾

	m_pCheckBuffer->Selected(True);  ///按钮选中
	///////系统配置
	UpdateMem();

	UpdateFace();
}

// 消息
void CSysCommonFile::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == m_pBtnModDir) ////更改目录
		{
			OnBnClickedBtnDir();
		}
		else if(msg.pSender == m_pBtnOPenFolder) ////打开文件夹
		{
			OnBnClickedBtnFilefolder();
		}
		else if(msg.pSender == m_pBtnClear) ////清理垃圾
		{
			OnBnClickedBtnClear();
		}
		//else if(msg.pSender == m_pBtnBuffer) ////查看缓存
		//{
		//	////OnBnClickedBtnFilefolder();////此处不响应
		//}
		else if(msg.pSender == m_pBtnClusterPic) ////查看群图片
		{
			OnBnClickedBtnClusterpicView();
		}
		else if(msg.pSender == m_pBtnSysContact) ////查看联系人
		{
			OnBnClickedBtnFriendpicView();
		}
		else if(msg.pSender == m_pBtnRecv) ////查看接收到的文件
		{
			OnBnClickedBtnAcceptView();
		}
	}
}

// 保存
void CSysCommonFile::Save()
{
	if(!m_strNewAcceptPath.IsEmpty())
	{
		FILE_SETTING setting;
		strcpy(setting.szRecvFilePath, m_strNewAcceptPath);
		g_LocalData.SaveUserRecvFileSetting(setting);
	}
}

void CSysCommonFile::OnBnClickedBtnDir()
{
	// TODO: 在此添加控件通知处理程序代码
	BROWSEINFO	bi;
	memset(&bi,0,sizeof(bi));
	char pszDirName[256];//存放选择的目录名
	LPITEMIDLIST pidl;
	bi.hwndOwner = GetHWND();
	bi.pidlRoot = NULL;
	bi.pszDisplayName = pszDirName;
	bi.lpszTitle = _T("选择要备份文件所在的目录");
	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	char szCurDir[255];
	::GetCurrentDirectory(255,szCurDir);
	if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//调用选择目录对话框
	{
		if (::SHGetPathFromIDList(pidl, pszDirName))//获得所选择的目录
		{ 
			int nLength = strlen(pszDirName);
			if( nLength > sizeof(pszDirName))
			{
				return;
			}

			if( pszDirName[nLength-1] != '\\' )
			{
				strcat(pszDirName,"\\");
			}


			//测试一下该目录是否可写
			CString strTestFile;
			strTestFile.Format("%sMini_图片",pszDirName);

			::RemoveDirectory(strTestFile);

			if( 0 == ::CreateDirectory(strTestFile,NULL) )
			{
				CMessageBox::Model(GetHWND(),"该目录不可写，请重新选择一个可存放的目录","",MB_OK);
				return;
			}

			::RemoveDirectory(strTestFile);

			/*m_edtPath.SetWindowText(pszDirName);	*/
			m_pEditRecvPath->SetText(pszDirName);
			m_strAcceptPath = pszDirName;
		}
		::SetCurrentDirectory(szCurDir);
		m_strNewAcceptPath = pszDirName;
	}
	else
	{
		m_strAcceptPath = m_pEditRecvPath->GetText();
		::SetCurrentDirectory(m_strAcceptPath);
		m_strNewAcceptPath = m_strAcceptPath;
	}
}


//功能：  像CFileDialog浏览打开文件一样打开文件夹
//参数：
//        pParent：显示浏览窗口的父窗口句柄
//        wndTitle：浏览窗口显示的标题
//Example:
//        CString path = GetBrowseFolder(this->m_hWnd,"请选择要打开的目录");
//////////////////////////////////////////////////////////////////////////
CString CSysCommonFile::GetBrowseFolder(HWND pParent,CString wndTitle)
{
	char        szDir[MAX_PATH] = {0};
	BROWSEINFO    bi;
	ITEMIDLIST    *pidl = NULL;

	bi.hwndOwner = pParent;
	bi.pidlRoot = NULL;
	bi.pszDisplayName = szDir;
	bi.lpszTitle = wndTitle;
	bi.ulFlags = BIF_RETURNONLYFSDIRS;
	bi.lpfn = NULL;
	bi.lParam = 0;
	bi.iImage = 0;

	pidl = SHBrowseForFolder(&bi);
	if(NULL == pidl)
		return "";
	if(!SHGetPathFromIDList(pidl,szDir))  
		return "";

	return CString(szDir);
}

void CSysCommonFile::OnBnClickedBtnFilefolder()
{
	ShellExecute(NULL,"open",m_strAcceptPath, NULL,NULL,SW_SHOWNORMAL);  
}

void CSysCommonFile::OnBnClickedBtnClusterpicView()
{
	ShellExecute(NULL,"open",m_strPicPath, NULL,NULL,SW_SHOWNORMAL);  
}

void CSysCommonFile::OnBnClickedBtnFriendpicView()
{
	ShellExecute(NULL,"open",m_strPicPath, NULL,NULL,SW_SHOWNORMAL);  
}

void CSysCommonFile::OnBnClickedBtnAcceptView()
{
	ShellExecute(NULL,"open",m_strAcceptPath, NULL,NULL,SW_SHOWNORMAL);  
}

// 更新内存
void CSysCommonFile::UpdateMem()
{
	m_dClusterPicSize = 0;
	m_dAccepetdFileSize = 0;
	m_dFriendPicSize = 0;
	m_dTempFileSize = 0;

	m_strPicPath = g_LocalData.GetImageFileCachePath();

	FILE_SETTING setting;
	g_LocalData.GetUserRecvFileSetting(setting);
	m_pEditRecvPath->SetText( setting.szRecvFilePath);
    m_strAcceptPath = m_pEditRecvPath->GetText();////接收文件路径赋值

	// 获取群图片
	g_LocalData.GetAcceptedClusterPicList(m_vecClusterPic);
	for(int i = 0; i < m_vecClusterPic.size(); ++i)
	{
		CString strPath = m_strPicPath + m_vecClusterPic[i].c_str();
		m_dClusterPicSize += GetFileSize_MB(strPath);
		m_vecClusterPic[i] = (LPCTSTR)strPath;
	}

	// 获取好友图片
	g_LocalData.GetAcceptedFriendPicList(m_vecFriendPic);
	for(int i = 0; i < m_vecFriendPic.size(); ++i)
	{
		CString strPath = m_strPicPath + m_vecFriendPic[i].c_str();
		m_dFriendPicSize += GetFileSize_MB(strPath);
		m_vecFriendPic[i] = (LPCTSTR)strPath;
	}

	// 获取接收文件
	m_vecAllAcceptFilePath.clear();
	GetDirAllFile(m_strAcceptPath, m_vecAllAcceptFilePath);
	for(int i = 0; i < m_vecAllAcceptFilePath.size(); ++i)
	{
		m_dAccepetdFileSize += GetFileSize_MB(m_vecAllAcceptFilePath[i].c_str());
	}
}

// 更新界面
void CSysCommonFile::UpdateFace()
{
	// 更新界面数据
	// 临时文件

	CString strSize = "0MB";
	if(m_dTempFileSize > 0.00001)
	{
		strSize.Format("%.5fMB", m_dTempFileSize);
	}
	CString strCount;
	strCount.Format("%d个文件", m_vecTempFilePath.size());
	m_pLabelBufNum->SetText(strCount);
	m_pLabelBufSize->SetText(strSize);

	// 群图片文件
	strSize = "0MB";
	if(m_dClusterPicSize > 0.00001)
	{
		strSize.Format("%.5fMB", m_dClusterPicSize);
	}
	strCount.Format("%d个文件", m_vecClusterPic.size());
	m_pLabelClusterPicNum->SetText(strCount);
	m_pLabelClusterPicSize->SetText(strSize);

	// 好友图片文件
	strSize = "0MB";
	if(m_dFriendPicSize > 0.00001)
	{
		strSize.Format("%.5fMB", m_dFriendPicSize);
	}
	strCount.Format("%d个文件", m_vecFriendPic.size());
	m_pLabelContactNum->SetText(strCount);
	m_pLabelContactSize->SetText(strSize);

	// 接收文件
	strSize = "0MB";
	if(m_dAccepetdFileSize > 0.00001)
	{
		strSize.Format("%.5fMB", m_dAccepetdFileSize);
	}
	strCount.Format("%d个文件", m_vecAllAcceptFilePath.size());
	m_pLabelRecvNum->SetText(strCount);
	m_pLabelRecvSize->SetText(strSize);
}

void CSysCommonFile::OnBnClickedBtnClear()
{
	m_dwAllTrashCnt = 0;
	m_dAllFileSize  = 0.0;
	if( m_pCheckBuffer->IsSelected() )
	{	
		if(m_dTempFileSize > 0.00001)
		{
			m_dAllFileSize += m_dTempFileSize;
		}
		m_dwAllTrashCnt=+m_vecTempFilePath.size();

		// 清理临时文件
		for(int i = 0; i < m_vecTempFilePath.size(); ++i)
		{
			DeleteFile(m_vecTempFilePath[i].c_str());

			//m_vecTempFilePath.erase(m_vecTempFilePath.begin());
		}
	}

	if( m_pCheckClusterPic->IsSelected())
	{
		if(m_dClusterPicSize > 0.00001)
		{
			m_dAllFileSize += m_dClusterPicSize;
		}
		m_dwAllTrashCnt=+m_vecClusterPic.size();

		// 清理群图片
		for(int i = 0; i < m_vecClusterPic.size(); ++i)
		{
			DeleteFile(m_vecClusterPic[i].c_str());
			//m_vecClusterPic.erase(m_vecClusterPic.begin());
		}
		g_LocalData.ClearAcceptedClusterPicList();
	}

	if( m_pCheckContact->IsSelected() )
	{
		if(m_dFriendPicSize > 0.00001)
		{
			m_dAllFileSize += m_dFriendPicSize;
		}
		m_dwAllTrashCnt=+m_vecFriendPic.size();
		
		// 清理联系人图片
		for(int i = 0; i < m_vecFriendPic.size(); ++i)
		{
			DeleteFile(m_vecFriendPic[i].c_str());
			//m_vecFriendPic.erase(m_vecFriendPic.begin());
		}
		g_LocalData.ClearAcceptedFriendPicList();
	}

	if( m_pCheckRecv->IsSelected() )
	{
		if(m_dAccepetdFileSize > 0.00001)
		{
			m_dAllFileSize += m_dAccepetdFileSize;
		}
	    m_dwAllTrashCnt=+m_vecAllAcceptFilePath.size();
	
		// 清理接收文件
		for(int i = 0; i < m_vecAllAcceptFilePath.size(); ++i)
		{
			DeleteFile(m_vecAllAcceptFilePath[i].c_str());
			//m_vecAllAcceptFilePath.erase(m_vecAllAcceptFilePath.begin());
		}
	}

    CString strMsg = _T("");
    strMsg.Format(_T("共清理 %d 个文件"),m_dwAllTrashCnt);
	if(m_dAllFileSize > 0.00001)
	{
	    strMsg.AppendFormat(_T(",成功释放 %.5f MB空间"),m_dAllFileSize);
	}
	else
	{
        strMsg.AppendFormat(_T(",成功释放 0MB 空间"));
	}
	CMessageBox::Prompt(strMsg);

	UpdateMem();
	UpdateFace();
}


/////////////////////////////////////////////////////////////////////////////////
//////
//////// CSysCommonFile 对话框
//////
//////IMPLEMENT_DYNAMIC(CSysCommonFile, CDialog)
//////
//////CSysCommonFile::CSysCommonFile(CWnd* pParent /*=NULL*/)
//////	: CDialog(CSysCommonFile::IDD, pParent)
//////{
//////	UpdateMem();
//////}
//////
//////CSysCommonFile::~CSysCommonFile()
//////{
//////}
//////
//////void CSysCommonFile::DoDataExchange(CDataExchange* pDX)
//////{
//////	CDialog::DoDataExchange(pDX);
//////	DDX_Control(pDX, IDC_BTN_DIR, m_btnDIR);
//////	DDX_Control(pDX, IDC_BTN_FILEFOLDER, m_btnFileOpen);
//////	DDX_Control(pDX, IDC_BTN_CLEAR, m_btnDumpClear);
//////	DDX_Control(pDX, IDC_EDIT1, m_edtPath);
//////	DDX_Control(pDX, IDC_BTN_TEMP_VIEW, m_btnTemp);
//////	DDX_Control(pDX, IDC_BTN_CLUSTERPIC_VIEW, m_btnClusterPic);
//////	DDX_Control(pDX, IDC_BTN_FRIENDPIC_VIEW, m_btnFriendPic);
//////	DDX_Control(pDX, IDC_BTN_ACCEPT_VIEW, m_btnAcceptFile);
//////	DDX_Control(pDX, IDC_CHECK_TEMP, m_checkTemp);
//////	DDX_Control(pDX, IDC_CHECK_CLUSTER, m_checkCluster);
//////	DDX_Control(pDX, IDC_CHECK_FRIEND, m_checkFriend);
//////	DDX_Control(pDX, IDC_CHECK_RECIVE, m_checkRecive);
//////}
//////
//////
//////BEGIN_MESSAGE_MAP(CSysCommonFile, CDialog)
//////	ON_BN_CLICKED(IDC_BTN_CLEAR, &CSysCommonFile::OnBnClickedBtnClear)
//////	ON_WM_CTLCOLOR()
//////	ON_BN_CLICKED(IDC_BTN_DIR, &CSysCommonFile::OnBnClickedBtnDir)
//////	ON_BN_CLICKED(IDC_BTN_FILEFOLDER, &CSysCommonFile::OnBnClickedBtnFilefolder)
//////	ON_BN_CLICKED(IDC_BTN_CLUSTERPIC_VIEW, &CSysCommonFile::OnBnClickedBtnClusterpicView)
//////	ON_BN_CLICKED(IDC_BTN_FRIENDPIC_VIEW, &CSysCommonFile::OnBnClickedBtnFriendpicView)
//////	ON_BN_CLICKED(IDC_BTN_ACCEPT_VIEW, &CSysCommonFile::OnBnClickedBtnAcceptView)
//////END_MESSAGE_MAP()
//////
//////
//////// CSysCommonFile 消息处理程序
//////
//////BOOL CSysCommonFile::OnInitDialog()
//////{
//////	CDialog::OnInitDialog();
//////
//////	// 遍历所有子控件，设置控件字体
//////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
//////	while(hWndChild)  
//////	{  
//////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
//////		if(GetDlgItem(IDC_STATIC_FILE_MANAGE)->GetSafeHwnd() == hWndChild)
//////		{
//////			::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12B.GetSafeHandle(), TRUE);
//////		}
//////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
//////	}
//////
//////	m_edtPath.SetBorderColor(0xc8c8c8);
//////	m_edtPath.SetWindowText(m_strAcceptPath);
//////
//////	m_btnDIR.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
//////	m_btnDIR.SetTextColor(0x2e2e2e);
//////	m_btnFileOpen.SetPng(IDB_PNG_GRAYBTN_L_HOVER, IDB_PNG_GRAYBTN_L_NORMAL, IDB_PNG_GRAYBTN_L_PRESSED, IDB_PNG_GRAYBTN_L_PRESSED);
//////	m_btnFileOpen.SetTextColor(0x2e2e2e);
//////	m_btnDumpClear.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
//////	m_btnDumpClear.SetTextColor(0x2e2e2e);
//////
//////	m_btnTemp.SetTextColor(0x2e2e2e);
//////	m_btnClusterPic.SetTextColor(0x2e2e2e);
//////	m_btnFriendPic.SetTextColor(0x2e2e2e);
//////	m_btnAcceptFile.SetTextColor(0x2e2e2e);
//////
//////	((CButton*)GetDlgItem(IDC_CHECK_TEMP))->SetCheck(TRUE);
//////
//////	UpdateFace();
//////
//////	return TRUE;
//////}
//////
//////void CSysCommonFile::OnBnClickedBtnClear()
//////{
//////	m_dwAllTrashCnt = 0;
//////	m_dAllFileSize  = 0.0;
//////	if( ((CButton*)GetDlgItem(IDC_CHECK_TEMP))->GetCheck() )
//////	{	
//////		if(m_dTempFileSize > 0.00001)
//////		{
//////			m_dAllFileSize += m_dTempFileSize;
//////		}
//////		m_dwAllTrashCnt=+m_vecTempFilePath.size();
//////
//////		// 清理临时文件
//////		for(int i = 0; i < m_vecTempFilePath.size(); ++i)
//////		{
//////			DeleteFile(m_vecTempFilePath[i].c_str());
//////
//////			//m_vecTempFilePath.erase(m_vecTempFilePath.begin());
//////		}
//////	}
//////
//////	if( ((CButton*)GetDlgItem(IDC_CHECK_CLUSTER))->GetCheck() )
//////	{
//////		if(m_dClusterPicSize > 0.00001)
//////		{
//////			m_dAllFileSize += m_dClusterPicSize;
//////		}
//////		m_dwAllTrashCnt=+m_vecClusterPic.size();
//////
//////		// 清理群图片
//////		for(int i = 0; i < m_vecClusterPic.size(); ++i)
//////		{
//////			DeleteFile(m_vecClusterPic[i].c_str());
//////			//m_vecClusterPic.erase(m_vecClusterPic.begin());
//////		}
//////		g_LocalData.ClearAcceptedClusterPicList();
//////	}
//////
//////	if( ((CButton*)GetDlgItem(IDC_CHECK_FRIEND))->GetCheck() )
//////	{
//////		if(m_dFriendPicSize > 0.00001)
//////		{
//////			m_dAllFileSize += m_dFriendPicSize;
//////		}
//////		m_dwAllTrashCnt=+m_vecFriendPic.size();
//////		
//////		// 清理联系人图片
//////		for(int i = 0; i < m_vecFriendPic.size(); ++i)
//////		{
//////			DeleteFile(m_vecFriendPic[i].c_str());
//////			//m_vecFriendPic.erase(m_vecFriendPic.begin());
//////		}
//////		g_LocalData.ClearAcceptedFriendPicList();
//////	}
//////
//////	if( ((CButton*)GetDlgItem(IDC_CHECK_RECIVE))->GetCheck() )
//////	{
//////		if(m_dAccepetdFileSize > 0.00001)
//////		{
//////			m_dAllFileSize += m_dAccepetdFileSize;
//////		}
//////	    m_dwAllTrashCnt=+m_vecAllAcceptFilePath.size();
//////	
//////		// 清理接收文件
//////		for(int i = 0; i < m_vecAllAcceptFilePath.size(); ++i)
//////		{
//////			DeleteFile(m_vecAllAcceptFilePath[i].c_str());
//////			//m_vecAllAcceptFilePath.erase(m_vecAllAcceptFilePath.begin());
//////		}
//////	}
//////
//////    CString strMsg = _T("");
//////    strMsg.Format(_T("共清理 %d 个文件"),m_dwAllTrashCnt);
//////	if(m_dAllFileSize > 0.00001)
//////	{
//////	    strMsg.AppendFormat(_T(",成功释放 %.5f MB空间"),m_dAllFileSize);
//////	}
//////	else
//////	{
//////        strMsg.AppendFormat(_T(",成功释放 0MB 空间"));
//////	}
//////	CMessageBox::Prompt(strMsg);
//////
//////	UpdateMem();
//////	UpdateFace();
//////}
//////
//////
//////HBRUSH CSysCommonFile::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
//////{
//////	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);
//////
//////	if ( nCtlColor==CTLCOLOR_STATIC )
//////	{
//////		UINT nId = pWnd->GetDlgCtrlID();
//////
//////		if ( nId==IDC_EDIT_USERNAME || nId==IDC_EDIT_NICKNAME || nId==IDC_EDIT_EMAIL )
//////		{
//////
//////		}
//////		else
//////		{
//////			pDC->SetBkMode(TRANSPARENT);
//////			hbr = g_hWhiteBrush;
//////		}
//////
//////		if ( nId==IDC_STATIC_GROUP )
//////		{
//////			pDC->SetTextColor(RGB(255,0,0));//（255,0，0） //(74,123,211)
//////		}
//////		else
//////		{
//////			pDC->SetTextColor(0x2e2e2e);
//////		}
//////	}
//////
//////	if ( nCtlColor==CTLCOLOR_DLG  )
//////	{
//////		hbr = g_hWhiteBrush;
//////	}
//////
//////	return hbr;
//////}
//////
//////
//////void CSysCommonFile::OnBnClickedBtnDir()
//////{
//////	// TODO: 在此添加控件通知处理程序代码
//////	BROWSEINFO	bi;
//////	memset(&bi,0,sizeof(bi));
//////	char pszDirName[256];//存放选择的目录名
//////	LPITEMIDLIST pidl;
//////	bi.hwndOwner = GetSafeHwnd();
//////	bi.pidlRoot = NULL;
//////	bi.pszDisplayName = pszDirName;
//////	bi.lpszTitle = _T("选择要备份文件所在的目录");
//////	bi.ulFlags = BIF_RETURNFSANCESTORS | BIF_RETURNONLYFSDIRS;
//////	bi.lpfn = NULL;
//////	bi.lParam = 0;
//////	char szCurDir[255];
//////	::GetCurrentDirectory(255,szCurDir);
//////	if ((pidl = ::SHBrowseForFolder(&bi)) != NULL)//调用选择目录对话框
//////	{
//////		if (::SHGetPathFromIDList(pidl, pszDirName))//获得所选择的目录
//////		{ 
//////			int nLength = strlen(pszDirName);
//////			if( nLength > sizeof(pszDirName))
//////			{
//////				return;
//////			}
//////
//////			if( pszDirName[nLength-1] != '\\' )
//////			{
//////				strcat(pszDirName,"\\");
//////			}
//////
//////
//////			//测试一下该目录是否可写
//////			CString strTestFile;
//////			strTestFile.Format("%sMini_图片",pszDirName);
//////
//////			::RemoveDirectory(strTestFile);
//////
//////			if( 0 == ::CreateDirectory(strTestFile,NULL) )
//////			{
//////				MessageBox("该目录不可写，请重新选择一个可存放的目录");
//////				return;
//////			}
//////
//////			::RemoveDirectory(strTestFile);
//////
//////			m_edtPath.SetWindowText(pszDirName);	
//////		}
//////	}
//////	::SetCurrentDirectory(szCurDir);
//////
//////	m_strNewAcceptPath = pszDirName;
//////}
//////
//////
////////功能：  像CFileDialog浏览打开文件一样打开文件夹
////////参数：
////////        pParent：显示浏览窗口的父窗口句柄
////////        wndTitle：浏览窗口显示的标题
////////Example:
////////        CString path = GetBrowseFolder(this->m_hWnd,"请选择要打开的目录");
////////////////////////////////////////////////////////////////////////////////
//////CString CSysCommonFile::GetBrowseFolder(HWND pParent,CString wndTitle)
//////{
//////	char        szDir[MAX_PATH] = {0};
//////	BROWSEINFO    bi;
//////	ITEMIDLIST    *pidl = NULL;
//////
//////	bi.hwndOwner = pParent;
//////	bi.pidlRoot = NULL;
//////	bi.pszDisplayName = szDir;
//////	bi.lpszTitle = wndTitle;
//////	bi.ulFlags = BIF_RETURNONLYFSDIRS;
//////	bi.lpfn = NULL;
//////	bi.lParam = 0;
//////	bi.iImage = 0;
//////
//////	pidl = SHBrowseForFolder(&bi);
//////	if(NULL == pidl)
//////		return "";
//////	if(!SHGetPathFromIDList(pidl,szDir))  
//////		return "";
//////
//////	return CString(szDir);
//////}
//////
//////void CSysCommonFile::OnBnClickedBtnFilefolder()
//////{
//////	ShellExecute(NULL,"open",m_strAcceptPath, NULL,NULL,SW_SHOWNORMAL);  
//////}
//////
//////void CSysCommonFile::OnBnClickedBtnClusterpicView()
//////{
//////	ShellExecute(NULL,"open",m_strPicPath, NULL,NULL,SW_SHOWNORMAL);  
//////}
//////
//////void CSysCommonFile::OnBnClickedBtnFriendpicView()
//////{
//////	ShellExecute(NULL,"open",m_strPicPath, NULL,NULL,SW_SHOWNORMAL);  
//////}
//////
//////void CSysCommonFile::OnBnClickedBtnAcceptView()
//////{
//////	ShellExecute(NULL,"open",m_strAcceptPath, NULL,NULL,SW_SHOWNORMAL);  
//////}
//////
//////// 更新内存
//////void CSysCommonFile::UpdateMem()
//////{
//////	m_dClusterPicSize = 0;
//////	m_dAccepetdFileSize = 0;
//////	m_dFriendPicSize = 0;
//////	m_dTempFileSize = 0;
//////
//////	m_strPicPath = g_LocalData.GetImageFileCachePath();
//////
//////	FILE_SETTING setting;
//////	g_LocalData.GetUserRecvFileSetting(setting);
//////	m_strAcceptPath = setting.szRecvFilePath;
//////
//////	// 获取群图片
//////	g_LocalData.GetAcceptedClusterPicList(m_vecClusterPic);
//////	for(int i = 0; i < m_vecClusterPic.size(); ++i)
//////	{
//////		CString strPath = m_strPicPath + m_vecClusterPic[i].c_str();
//////		m_dClusterPicSize += GetFileSize_MB(strPath);
//////		m_vecClusterPic[i] = (LPCTSTR)strPath;
//////	}
//////
//////	// 获取好友图片
//////	g_LocalData.GetAcceptedFriendPicList(m_vecFriendPic);
//////	for(int i = 0; i < m_vecFriendPic.size(); ++i)
//////	{
//////		CString strPath = m_strPicPath + m_vecFriendPic[i].c_str();
//////		m_dFriendPicSize += GetFileSize_MB(strPath);
//////		m_vecFriendPic[i] = (LPCTSTR)strPath;
//////	}
//////
//////	// 获取接收文件
//////	m_vecAllAcceptFilePath.clear();
//////	GetDirAllFile(m_strAcceptPath, m_vecAllAcceptFilePath);
//////	for(int i = 0; i < m_vecAllAcceptFilePath.size(); ++i)
//////	{
//////		m_dAccepetdFileSize += GetFileSize_MB(m_vecAllAcceptFilePath[i].c_str());
//////	}
//////}
//////
//////// 更新界面
//////void CSysCommonFile::UpdateFace()
//////{
//////	// 更新界面数据
//////	// 临时文件
//////
//////	CString strSize = "0MB";
//////	if(m_dTempFileSize > 0.00001)
//////	{
//////		strSize.Format("%.5fMB", m_dTempFileSize);
//////	}
//////	CString strCount;
//////	strCount.Format("%d个文件", m_vecTempFilePath.size());
//////	SetDlgItemText(IDC_STATIC_TEMP_COUNT, strCount);
//////	SetDlgItemText(IDC_STATIC_TEMP_SIZE, strSize);
//////
//////	// 群图片文件
//////	strSize = "0MB";
//////	if(m_dClusterPicSize > 0.00001)
//////	{
//////		strSize.Format("%.5fMB", m_dClusterPicSize);
//////	}
//////	strCount.Format("%d个文件", m_vecClusterPic.size());
//////	SetDlgItemText(IDC_STATIC_CLUSTERPIC_COUNT, strCount);
//////	SetDlgItemText(IDC_STATIC_CLUSTERPIC_SIZE, strSize);
//////
//////	// 好友图片文件
//////	strSize = "0MB";
//////	if(m_dFriendPicSize > 0.00001)
//////	{
//////		strSize.Format("%.5fMB", m_dFriendPicSize);
//////	}
//////	strCount.Format("%d个文件", m_vecFriendPic.size());
//////	SetDlgItemText(IDC_STATIC_FRIENDPIC_COUNT, strCount);
//////	SetDlgItemText(IDC_STATIC_FRIENDPIC_SIZE, strSize);
//////
//////	// 接收文件
//////	strSize = "0MB";
//////	if(m_dAccepetdFileSize > 0.00001)
//////	{
//////		strSize.Format("%.5fMB", m_dAccepetdFileSize);
//////	}
//////	strCount.Format("%d个文件", m_vecAllAcceptFilePath.size());
//////	SetDlgItemText(IDC_STATIC_ACCEPT_COUNT, strCount);
//////	SetDlgItemText(IDC_STATIC_ACCEPT_SIZE, strSize);
//////
//////}
//////
//////// 保存
//////void CSysCommonFile::Save()
//////{
//////	if(!m_strNewAcceptPath.IsEmpty())
//////	{
//////		FILE_SETTING setting;
//////		strcpy(setting.szRecvFilePath, m_strNewAcceptPath);
//////		g_LocalData.SaveUserRecvFileSetting(setting);
//////	}
//////}