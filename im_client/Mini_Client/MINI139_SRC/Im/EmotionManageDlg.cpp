// im\EmotionManageDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\emotionmanagedlg.h"
#include "LocalData.h"
#include "imagefiledialog.h"
#include "addemotiondlg.h"
#include "emotiondlg.h"





// CEmotionManageDlg 对话框

IMPLEMENT_DYNAMIC(CEmotionManageDlg, CDialog)
CEmotionManageDlg::CEmotionManageDlg(int nGroup,CWnd* pParent /*=NULL*/)
	: CXSkinDlg(CEmotionManageDlg::IDD, pParent)
{
	m_nCurGroup = nGroup;
}

CEmotionManageDlg::~CEmotionManageDlg()
{
	CustomFaceMapIt it;
	for ( it=m_mapCustomFace.begin(); it!=m_mapCustomFace.end(); it++ )
	{
		delete (CBitmap *)it->second;
	}
	m_mapCustomFace.clear();
}

void CEmotionManageDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_EMOTION, m_lsEmotion);
	DDX_Control(pDX, IDC_STATIC_PIC, m_picture);
	DDX_Control(pDX, IDC_BUTTON_CLOSE,m_btnClose);
	DDX_Control(pDX, IDC_BUTTON_ADD,m_btnAdd);
	DDX_Control(pDX, IDC_BUTTON_DEL,m_btnDel);
	DDX_Control(pDX, IDC_BUTTON_MODIFY,m_btnModify);
}


BEGIN_MESSAGE_MAP(CEmotionManageDlg, CXSkinDlg)
	ON_BN_CLICKED(IDC_BUTTON_CLOSE, OnBnClickedButtonClose)
	ON_WM_PAINT()
	ON_BN_CLICKED(IDC_BUTTON_ADD, OnBnClickedButtonAdd)
	ON_NOTIFY(HDN_ITEMCHANGED, 0, OnHdnItemchangedListEmotion)
	ON_NOTIFY(LVN_ITEMCHANGED, IDC_LIST_EMOTION, OnLvnItemchangedListEmotion)
	ON_BN_CLICKED(IDC_BUTTON_DEL, OnBnClickedButtonDel)
	ON_BN_CLICKED(IDC_BUTTON_MODIFY, OnBnClickedButtonModify)
	ON_WM_SHOWWINDOW()
END_MESSAGE_MAP()


// CEmotionManageDlg 消息处理程序

void CEmotionManageDlg::OnBnClickedButtonClose()
{
	CXSkinDlg::OnCancel();
}

BOOL CEmotionManageDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();

	SetWindowText("表情管理");
	SetDlgStyle(CLOSE);

	m_imgEmotionList.Create( 20,
		20,
		ILC_COLOR32,
		0,
		1 );
	CRect rt;
	m_lsEmotion.GetClientRect(&rt);
	int nW=rt.Width();
	nW -= (130);
	if(nW < 30)
		nW = 30;
	m_lsEmotion.InsertColumn(0,"序号",  LVCFMT_LEFT,50);
	m_lsEmotion.InsertColumn(1,"快捷键", LVCFMT_LEFT,80);
	m_lsEmotion.InsertColumn(2,"说明", LVCFMT_LEFT,nW);

	m_lsEmotion.SetImageList(&m_imgEmotionList,LVSIL_SMALL );
	m_lsEmotion.SetExtendedStyle(LVS_EX_GRIDLINES|LVS_EX_FULLROWSELECT);

	RefreshEmotionList();

	
	

	return TRUE; 
}

void CEmotionManageDlg::InitCustomFace()
{
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();

	CDC *pDC = GetDC();

	int nGroup = g_customEmotionDB.size();
	int i,j;
	int nC;
	EMOTION_ID id;
	int type;

	for ( i = 0; i< nGroup; i++ )
	{
		CUSTOM_EMOTION_GROUP & group = g_customEmotionDB[i];
		nC = group.size();
		for(j = 0; j < nC; j++)
		{
			type = g_customEmotionDB[i][j].type;
			if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
				continue;
			CDC mDC;
			mDC.CreateCompatibleDC(pDC);
			CBitmap *pCustomFaceBmp = new CBitmap();
			pCustomFaceBmp->CreateCompatibleBitmap(pDC,20,20);
			CBitmap *pOldbmp = mDC.SelectObject(pCustomFaceBmp);
			// 初始化图片为白色
			mDC.FillSolidRect(0,0,20,20,RGB(255,255,255));
			CString strCustomFaceFile ;
			strCustomFaceFile.Format("%s%s",strCustomFacePath,group[j].szFile);
			Image    * image   = new Image(strCustomFaceFile.AllocSysString());
			Graphics * grahics = new Graphics(mDC.m_hDC);
			//DrawPicture(szCustomFaceFile,&mDC,20,20);

			Unit  units;
			RectF destRect(REAL(0),	REAL(0),	REAL(20),	REAL(20)),srcRect;
			image->GetBounds(&srcRect, &units);
			// 将自定义表情图片绘制到内存位图中
			grahics->DrawImage(image, destRect, srcRect.X, srcRect.Y, srcRect.Width,	srcRect.Height,	UnitPixel, NULL);
			delete image;
			delete grahics;
			mDC.SelectObject(pOldbmp);
			// 将绘制到的内存位图保存起来
			id.nGroup = i;
			id.nItem = j;
			m_mapCustomFace[id] = pCustomFaceBmp;
		}
	}
	ReleaseDC(pDC);
}

void CEmotionManageDlg::OnPaint()
{
	CPaintDC dc(this); 
	CXSkinDlg::OnPaint();
}

void CEmotionManageDlg::OnBnClickedButtonAdd()
{
	CImageFileDialog dlg("","",OFN_ALLOWMULTISELECT|OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT,this);
	int nFileSize;
	CString sPrompt;
	vector<string> sFileList;
	int nExist=0;
	int nBufSize = 0;
	int i,j;
	int nGroup;
	int nC;
	if ( IDOK==dlg.DoModal() )
	{
		POSITION pos=dlg.GetStartPosition();

		CString sFile;

		while(pos)
		{
			sFile=dlg.GetNextPathName(pos);
			nBufSize += sFile.GetLength();
			if(nBufSize > 20000)
			{
				ShowMessage("你选择的文件太多，部分文件将无法添加");
				break;
			}
			nFileSize=::GetFileSize(sFile)/1024;
			if ( dlg.GetSelFileSize()<512 )
			{
				CString strCacheFile;
				CString sMD5;
				// 将该文件保存在缓存目录中
				g_LocalData.SaveImageFileToCache(sFile,strCacheFile);
				// 得到缓存文件的全名
				CString strImageFile = g_LocalData.GetImageFileCachePath()+strCacheFile;

				bool bFlag=false;
				// 判断该缓存文件是否与已知表情文件重名
				nGroup = g_customEmotionDB.size();
				for ( i = 0; i < nGroup; i++ )
				{
					nC = g_customEmotionDB[i].size();
					for(j = 0; j < nC; j++)
					{
						GetMD5FileName(strImageFile,sMD5);
						if ( sMD5 == g_customEmotionDB[i][j].szFile )
						{
							bFlag=true;
							nExist++;
							break;
						}
					}
					if(bFlag)
						break;
				}
				if ( bFlag )
				{
					//sPrompt=sFile+"已经在您的表情列表中!";
					//MessageBox(sPrompt,IM_CAPTION,MB_OK|MB_ICONINFORMATION);
				}
				else
				{
					sFileList.push_back((LPCTSTR)strImageFile);
					/*CAddEmotionDlg dlgAddEmotion(strImageFile,this);

					if ( IDOK==dlgAddEmotion.DoModal() )
					{
						RefreshEmotionList();

						m_lsEmotion.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
					}*/
				}
			}
			else
			{
				ShowMessage("对不起，自定义表情文件大小不能超过于 512K！",MB_ICONINFORMATION|MB_OK);
			}
		}
	}
	if(nExist>0)
	{
		CString sPrompt;
		sPrompt.Format("当前有%d个文件已经在您的表情列表中!",nExist);
		//MessageBox(sPrompt,IM_CAPTION,MB_OK|MB_ICONINFORMATION);
		CMessageBox::Prompt(sPrompt);
	}
	if(sFileList.size()>0)
	{
		AddEmotions(sFileList);
	}
	for(int i=0;i<sFileList.size();i++)
	{
		DeleteFile(sFileList[i].c_str());
	}
}

void CEmotionManageDlg::RefreshEmotionList()
{
	int i;
	int j;
	int nC;
	nC = g_customEmotionDB[m_nCurGroup].size();
	m_imgEmotionList.SetImageCount(nC);
	CustomFaceMapIt it;
	for ( it=m_mapCustomFace.begin(); it!=m_mapCustomFace.end(); it++ )
	{
		delete (CBitmap *)it->second;
	}
	m_mapCustomFace.clear();
	
	InitCustomFace();
	EMOTION_ID id;
	id.nGroup = m_nCurGroup;
	
	
	m_lsEmotion.DeleteAllItems();
	int nIdx = 0;
	int type ;
	for (i = 0; i < nC; i++ )
	{
		type = g_customEmotionDB[m_nCurGroup][i].type;
		if(type == CUSTOM_EMOTION::SYS || type == CUSTOM_EMOTION::SYS_GIF)
			continue;
		m_lsEmotion.InsertItem(nIdx,IntToStr(nIdx+1),nIdx);
		m_lsEmotion.SetItemText(nIdx,1,g_customEmotionDB[m_nCurGroup][i].szShortCut);
		m_lsEmotion.SetItemText(nIdx,2,g_customEmotionDB[m_nCurGroup][i].szTip);
		m_lsEmotion.SetItemData(nIdx,(DWORD_PTR)i);
		nIdx++;
	}

	int nImages = m_lsEmotion.GetItemCount();
	// 重新加载位图列表
	for ( j=0; j< nImages; j++ )
	{
		id.nItem = (int)m_lsEmotion.GetItemData(j);
		it = m_mapCustomFace.find(id);
		if(it != m_mapCustomFace.end())
			m_imgEmotionList.Replace(j,it->second,NULL);
	}
}

void CEmotionManageDlg::OnHdnItemchangedListEmotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMHEADER phdr = reinterpret_cast<LPNMHEADER>(pNMHDR);

	*pResult = 0;
}

void CEmotionManageDlg::OnLvnItemchangedListEmotion(NMHDR *pNMHDR, LRESULT *pResult)
{
	LPNMLISTVIEW pNMLV = reinterpret_cast<LPNMLISTVIEW>(pNMHDR);

	CString strImageFile ;

	POSITION pos = m_lsEmotion.GetFirstSelectedItemPosition();

	if ( pos==NULL )
		return ;

	int nIndex = m_lsEmotion.GetNextSelectedItem(pos);
	nIndex = (int)m_lsEmotion.GetItemData(nIndex);

	strImageFile.Format("%s%s",g_LocalData.GetCustomFacePath(),
		g_customEmotionDB[m_nCurGroup][nIndex].szFile);

	if ( m_picture.Load(strImageFile) )
		m_picture.Draw();

	*pResult = 0;
}

void CEmotionManageDlg::OnBnClickedButtonDel()
{
	POSITION pos = m_lsEmotion.GetFirstSelectedItemPosition();
	vector<int> arrSel;
	//vector<int> arrItems;
	int nItem ;
	int nIndex;
	if (pos == NULL)
		TRACE0("No items were selected!\n");
	else
	{
		while (pos)
		{
			nItem = m_lsEmotion.GetNextSelectedItem(pos);
			nIndex = (int)m_lsEmotion.GetItemData(nItem);
			arrSel.push_back(nIndex);	
			//arrItems.push_back(nItem);
		}
	}
	int nS = arrSel.size();
	CString strEmotionFile;
	// 一定要反序
	for(int i = nS-1;i >= 0;i--)
	{
		nIndex = arrSel[i];
		strEmotionFile = g_LocalData.GetCustomFacePath() + g_customEmotionDB[m_nCurGroup][nIndex].szFile;
		DeleteFile(strEmotionFile);
		g_customEmotionDB[m_nCurGroup].erase(g_customEmotionDB[m_nCurGroup].begin()+nIndex);
		//m_lsEmotion.DeleteItem(arrItems[i]);
	}
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	DeleteFile(strCustomConfigFile);
	CStdioFile file;
	bool bOpenFlag;

	if ( _access((LPCSTR)strCustomConfigFile,0)==-1 )
	{
		bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	}
	else
	{
		bOpenFlag = file.Open(strCustomConfigFile,CFile::modeWrite|CFile::typeText) ;
		file.SeekToBegin();
	}

	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);
		file.Close();
	}
	RefreshEmotionList();
}

void CEmotionManageDlg::OnBnClickedButtonModify()
{
	POSITION pos = m_lsEmotion.GetFirstSelectedItemPosition();
	if ( pos==NULL )
		return ;
	int nIndex = m_lsEmotion.GetNextSelectedItem(pos);
	nIndex = (int)m_lsEmotion.GetItemData(nIndex);
	CString strEmotionFile = g_LocalData.GetCustomFacePath() + g_customEmotionDB[m_nCurGroup][nIndex].szFile;
	CAddEmotionDlg dlg(m_nCurGroup,strEmotionFile,this);
	dlg.SetModifyEmotion(m_nCurGroup,nIndex);
	if (dlg.DoModal()==IDOK)
	{
		RefreshEmotionList();
	}
}

void CEmotionManageDlg::AddEmotions(const vector<string>& ems)
{
	int nS=ems.size();
	if(nS==0)
		return;
	// 首先获得自定义表情配置文件
	CString strCustomFacePath = g_LocalData.GetCustomFacePath();
	CString strCustomConfigFile = strCustomFacePath+"customface.cfg.xml";
	CStdioFile file;
	bool bOpenFlag;
	// 删除配置文件
	DeleteFile(strCustomConfigFile);
	// 重建配置文件
	bOpenFlag = file.Open(strCustomConfigFile,CFile::modeCreate|CFile::modeWrite|CFile::typeText);
	CUSTOM_EMOTION	face;
	CString				strFaceFileName;
	for(int i=0;i<nS;i++)
	{
		// 将文件保存到硬盘和全局表情列表
		g_LocalData.SaveImageFileToCustomFace(ems[i].c_str(),strFaceFileName);
		strcpy(face.szFile,strFaceFileName);
		g_customEmotionDB[m_nCurGroup].push_back(face);
	}

	// 保存配置文件
	if ( bOpenFlag )
	{
		WriteCustomEmotionConfig(file);
		file.Close();	
	}
	RefreshEmotionList();
	m_lsEmotion.PostMessage(WM_VSCROLL,SB_BOTTOM,0);
}

void CEmotionManageDlg::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CXSkinDlg::OnShowWindow(bShow, nStatus);

}
