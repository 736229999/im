// im\AddEmotionDlg.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\addemotiondlg.h"
#include "imagefiledialog.h"
#include "localdata.h"
#include "emotiondlg.h"


// CAddEmotionDlg �Ի���

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
	SetWindowText("��ӱ���");

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
	// ���Ȼ���Զ�����������ļ�
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	CStdioFile file;
	int i,j;
	int nGroup;
	int nC;
	bool bOpenFlag;
	// ɾ�������ļ�
	//DeleteFile(strCustomConfigFile);
	// �ؽ������ļ�
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION face;
	// ����û�����Ķ�����Ϣ
	GetDlgItemText(IDC_EDIT_SHORTCUT,face.szShortCut,sizeof(face.szShortCut));
	GetDlgItemText(IDC_EDIT_INTRO,face.szTip,sizeof(face.szTip));
	// �ж϶���Ŀ�ݼ��Ƿ��Ѵ���
	if ( strcmp(face.szShortCut,"")!=0 )
	{
		// �ж��Ƿ��Ĭ�Ͽ�ݼ���ͻ
		for ( i=0; i<18; i++ )
		{
			if ( strcmp(CRichEditCtrlEx::emotions[i],face.szShortCut)==0 )
			{
				CString str;
				str.Format("��ݷ�ʽ \n  %s \n �Ѿ���ʹ�ã�����������һ����ݷ�ʽ��",
					face.szShortCut);
				ShowMessage(str,MB_OK|MB_ICONWARNING);
				return;
			}
		}
		// �ж��Ƿ����Ѵ��ڵĳ�ͻ
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
					str.Format("��ݷ�ʽ \n  %s \n �Ѿ���ʹ�ã�����������һ����ݷ�ʽ��",
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
					str.Format("�Ѿ������Զ������Ҳ�����˵��(%s)",face.szTip);
					ShowMessage(str,MB_OK|MB_ICONWARNING);
					return;
				}
			}		
		}
	}
	// ������޸�ģʽ���޸��滻
	if ( m_bModify )
	{
		strcpy(g_customEmotionDB[m_nCurGroup][m_iEmotion].szTip,face.szTip);
		strcpy(g_customEmotionDB[m_nCurGroup][m_iEmotion].szShortCut,face.szShortCut);
	}
	// ��������ģʽ���ļ����浽Ӳ�̺�ȫ�ֱ����б�
	else
	{
		CString strFaceFileName;
		g_LocalData.SaveImageFileToCustomFace(m_strImageFile,strFaceFileName);
		strcpy(face.szFile,strFaceFileName);
		g_customEmotionDB[m_nCurGroup].push_back(face);
	}
	// ���������ļ�
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
	// ���Ȼ���Զ�����������ļ�
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath + "customface.cfg.xml";
	CStdioFile file;
	bool bOpenFlag;
	// ɾ�������ļ�
	DeleteFile(strCustomConfigFile);
	// �ؽ������ļ�
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION	face;
	CString				strFaceFileName;

	// ���ļ����浽Ӳ�̺�ȫ�ֱ����б�
	g_LocalData.SaveImageFileToCustomFace(sEmotion,strFaceFileName);
	strcpy(face.szFile,strFaceFileName);
	if(nGroup >= g_customEmotionDB.size())
		nGroup = 0;
	g_customEmotionDB[nGroup].push_back(face);

	// ���������ļ�
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
		// �����ļ������ڻ���Ŀ¼��
		g_LocalData.SaveImageFileToCache(sFile,strCacheFile);
		// �õ������ļ���ȫ��
		sEmotion = g_LocalData.GetImageFileCachePath()+strCacheFile;
		GetMD5FileName(sEmotion,sMD5);
		nGroup = g_customEmotionDB.size();
		// �жϸû����ļ��Ƿ�����֪�����ļ�����
		for ( i = 0; i < nGroup; i++ )
		{
			nC = g_customEmotionDB[i].size();
			for(j = 0;j < nC; j++)
			{
				if ( sMD5 == g_customEmotionDB[i][j].szFile )
				{
					ShowMessage("����ӵı����ļ��Ѿ������ı����б���!");
					DeleteFile(sEmotion);
					return -1;
				}
			}	
		}
	}
	else
	{
		ShowMessage("�Բ����Զ�������ļ���С���ܳ����� 512K��",MB_ICONINFORMATION|MB_OK);
	}
	int nRt = AddEmotionFile(m_nCurGroup,sEmotion);
	DeleteFile(sEmotion);
	return nRt;
}
