// im\AddEmotionDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\addemotiondlg.h"
#include "imagefiledialog.h"
#include "localdata.h"
#include "emotiondlg.h"


// CAddEmotionDlg 对话框

IMPLEMENT_DYNAMIC(CAddEmotionDlg, CXSkinDlg)
CAddEmotionDlg::CAddEmotionDlg(int nGroup,const CString & strImageFile, CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CAddEmotionDlg::IDD, pParent)
{
	m_strImageFile = strImageFile;
	m_bModify      = false;
	m_bPic = false;
	m_nCurGroup = nGroup;
}

CAddEmotionDlg::~CAddEmotionDlg()
{
}

void CAddEmotionDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX,IDC_STATIC_PIC,m_picture);
	DDX_Control(pDX, IDC_EDIT_SHORTCUT, m_edtShortCut);
	DDX_Control(pDX, IDC_EDIT_INTRO, m_edtTip);
	DDX_Control(pDX, IDC_STATIC_LINE, m_wndLine);
	DDX_Control(pDX, IDC_BUTTON_OK, m_btnOk);
	DDX_Control(pDX, IDC_BUTTON_CANCEL, m_btnCancel);
}


BEGIN_MESSAGE_MAP(CAddEmotionDlg, CXSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_OK, OnBnClickedButtonOk)
	ON_BN_CLICKED(IDC_BUTTON_CANCEL, OnBnClickedButtonCancel)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
END_MESSAGE_MAP()



BOOL CAddEmotionDlg::OnInitDialog()
{
	int nIdx = 0;
	if(m_bPic)
	{
		m_bPic = false;
		m_bModify  = true;
		nIdx = AddEmotion(m_strImageFile);
		if(nIdx == -1)
		{
			OnCancel();
			return FALSE;
		}
		else
		{
			m_iEmotion = nIdx;
		}
	}

	CXSkinDlg::OnInitDialog();

	SetDlgStyle(CLOSE);
	SetWindowText("添加表情");

	if ( m_picture.Load(m_strImageFile) )
		m_picture.Draw();

	if ( m_bModify )
	{
		SetDlgItemText(IDC_EDIT_SHORTCUT,g_customEmotionDB[m_nCurGroup][m_iEmotion].szShortCut);
		SetDlgItemText(IDC_EDIT_INTRO,g_customEmotionDB[m_nCurGroup][m_iEmotion].szTip);
	}

	m_edtShortCut.LimitText(10);

	return TRUE; 
}

void CAddEmotionDlg::OnBnClickedButtonOk()
{
	// 首先获得自定义表情配置文件
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	CStdioFile file;
	int i,j;
	int nGroup;
	int nC;
	bool bOpenFlag;
	// 删除配置文件
	//DeleteFile(strCustomConfigFile);
	// 重建配置文件
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION face;
	// 获得用户输入的定义信息
	GetDlgItemText(IDC_EDIT_SHORTCUT,face.szShortCut,sizeof(face.szShortCut));
	GetDlgItemText(IDC_EDIT_INTRO,face.szTip,sizeof(face.szTip));
	// 判断定义的快捷键是否已存在
	if ( strcmp(face.szShortCut,"")!=0 )
	{
		// 判断是否和默认快捷键冲突
		for ( i=0; i<18; i++ )
		{
			if ( strcmp(CRichEditCtrlEx::emotions[i],face.szShortCut)==0 )
			{
				CString str;
				str.Format("快捷方式 \n  %s \n 已经被使用，请重新设置一个快捷方式。",
					face.szShortCut);
				ShowMessage(str,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		// 判断是否与已存在的冲突
		nGroup = g_customEmotionDB.size();
		for (  i = 0; i < nGroup; i++ )
		{
			nC = g_customEmotionDB[i].size();
			for( j = 0; j < nC; j++)
			{
				if ( j == m_iEmotion )
				{
					continue;
				}
				if ( stricmp(g_customEmotionDB[i][j].szShortCut,face.szShortCut) == 0 )
				{
					CString str;
					str.Format("快捷方式 \n  %s \n 已经被使用，请重新设置一个快捷方式。",
						face.szShortCut);
					ShowMessage(str,MB_OK|MB_ICONWARNING);
					return;
				}
			}
			
		}

	}

	if ( strcmp(face.szTip,"")!=0 )
	{
		nGroup = g_customEmotionDB.size();
		for ( i = 0; i < nGroup; i++ )
		{
			nC = g_customEmotionDB[i].size();
			for(j = 0; j < nC; j++)
			{
				if (j == m_iEmotion )
				{
					continue;
				}
				if ( stricmp(g_customEmotionDB[i][j].szTip,face.szTip)==0 )
				{
					CString str;
					str.Format("已经存在自定义表情也用这个说明(%s)",face.szTip);
					ShowMessage(str,MB_OK|MB_ICONWARNING);
					return;
				}
			}		
		}
	}
	// 如果是修改模式则修改替换
	if ( m_bModify )
	{
		strcpy(g_customEmotionDB[m_nCurGroup][m_iEmotion].szTip,face.szTip);
		strcpy(g_customEmotionDB[m_nCurGroup][m_iEmotion].szShortCut,face.szShortCut);
	}
	// 如果是添加模式则将文件保存到硬盘和全局表情列表
	else
	{
		CString strFaceFileName;
		g_LocalData.SaveImageFileToCustomFace(m_strImageFile,strFaceFileName);
		strcpy(face.szFile,strFaceFileName);
		g_customEmotionDB[m_nCurGroup].push_back(face);
	}
	// 保存配置文件
	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);
		file.Close();
		CXSkinDlg::OnOK();
	}
}

void CAddEmotionDlg::SetModifyEmotion(int nCurGroup,int iEmotion)
{
	m_bModify  = true;
	m_iEmotion = iEmotion;
	m_nCurGroup = nCurGroup;

}

void CAddEmotionDlg::OnBnClickedButtonCancel()
{
	CXSkinDlg::OnCancel();
}

BOOL CAddEmotionDlg::OnEraseBkgnd(CDC* pDC)
{
	CRect rcClient ;
	GetClientRect(&rcClient);
	
	pDC->FillSolidRect(rcClient,IM_SPACE_COLOR);

	if ( m_wndLine.m_hWnd )
	{
		CRect rcLine;
		m_wndLine.GetWindowRect(&rcLine);
		ScreenToClient(&rcLine);

		pDC->FillSolidRect(rcLine,0xFFFFFF);
		pDC->Draw3dRect(rcLine,IM_LINE_COLOR,IM_LINE_COLOR);
	}

	if ( m_picture.m_hWnd )
	{
		CRect rcLine;
		m_picture.GetWindowRect(&rcLine);
		ScreenToClient(&rcLine);
		rcLine.InflateRect(1,1);
		pDC->Draw3dRect(rcLine,IM_LINE_COLOR,IM_LINE_COLOR);
	}

	DrawBorder();
	DrawCaptionBar();

	return FALSE;
}

HBRUSH CAddEmotionDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( pWnd->GetDlgCtrlID()==IDC_STATIC )
	{
		pDC->SetTextColor(RGB(0,84,166));
		pDC->SetBkMode(TRANSPARENT);
		hbr = g_hWhiteBrush;
	}

	return hbr;
}

int AddEmotionFile(int nGroup,const CString sEmotion)
{
	if(sEmotion == "")
		return -1;
	// 首先获得自定义表情配置文件
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath + "customface.cfg.xml";
	CStdioFile file;
	bool bOpenFlag;
	// 删除配置文件
	DeleteFile(strCustomConfigFile);
	// 重建配置文件
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION	face;
	CString				strFaceFileName;

	// 将文件保存到硬盘和全局表情列表
	g_LocalData.SaveImageFileToCustomFace(sEmotion,strFaceFileName);
	strcpy(face.szFile,strFaceFileName);
	if(nGroup >= g_customEmotionDB.size())
		nGroup = 0;
	g_customEmotionDB[nGroup].push_back(face);

	// 保存配置文件
	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);
		file.Close();	
	}
	int nRt = (int)g_customEmotionDB[nGroup].size() - 1;
	return nRt;
}

int CAddEmotionDlg::AddEmotion(const CString & sFile)
{
	int nFileSize=::GetFileSize(sFile)/1024;
	if(nFileSize == -1)
		return -1;
	int nExist = 0;
	int nGroup;
	int nC;
	int i,j;
	CString sEmotion;
	if ( nFileSize < 512 )
	{
		CString strCacheFile;
		CString sMD5;
		// 将该文件保存在缓存目录中
		g_LocalData.SaveImageFileToCache(sFile,strCacheFile);
		// 得到缓存文件的全名
		sEmotion = g_LocalData.GetImageFileCachePath()+strCacheFile;
		GetMD5FileName(sEmotion,sMD5);
		nGroup = g_customEmotionDB.size();
		// 判断该缓存文件是否与已知表情文件重名
		for ( i = 0; i < nGroup; i++ )
		{
			nC = g_customEmotionDB[i].size();
			for(j = 0;j < nC; j++)
			{
				if ( sMD5 == g_customEmotionDB[i][j].szFile )
				{
					ShowMessage("待添加的表情文件已经在您的表情列表中!");
					DeleteFile(sEmotion);
					return -1;
				}
			}	
		}
	}
	else
	{
		ShowMessage("对不起，自定义表情文件大小不能超过于 512K！",MB_ICONINFORMATION|MB_OK);
	}
	int nRt = AddEmotionFile(m_nCurGroup,sEmotion);
	DeleteFile(sEmotion);
	return nRt;
}
