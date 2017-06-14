// ModHeadIconDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "ModHeadIconDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "SelDefHeadIconDlg.h"
#include "SelCustomHeadIconDlg.h"
#include "RoundFaceCtrl.h"
#include "Im\WebProcess.h"
#include <string>
#include "im/xtpacket.h"
#include "im\SimpleFileTransSession.h"
#include "im/localdata.h"
//#include "im/RichEditCtrlEx.cpp"

using namespace std;

#define IDC_WORK_AREA 23362

extern CWebUserHeadIconProcess g_webUserHeadIconProcess;

// CModHeadIconDlg 对话框

IMPLEMENT_DYNAMIC(CModHeadIconDlg, CXSkinDlg)
IMPLEMENT_HMG_SCROLLBAR(CModHeadIconDlg)

CModHeadIconDlg::CModHeadIconDlg(CWnd* pParent /*=NULL*/)
	: CXSkinDlg(IDD_DLG_MOD_HEAD_ICON, pParent)
{
	m_colorBackground = RGB(255, 255, 255);
	m_colorBorder = CHAT_BKG_COLOR;
	DeleteObject(m_hBkGround);
	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
	m_strFilePath = "";
	m_nHeadType = HEAD_TYPE_DEFULT;
	SetDlgStyle(CXSkinDlg::CLOSE);


	m_font.CreateFont(
		19,                        // nHeight
		0,                         // nWidth
		0,                         // nEscapement
		0,                         // nOrientation
		FW_BOLD,                 // nWeight
		FALSE,                     // bItalic
		FALSE,                     // bUnderline
		0,                         // cStrikeOut
		ANSI_CHARSET,              // nCharSet
		OUT_DEFAULT_PRECIS,        // nOutPrecision
		CLIP_DEFAULT_PRECIS,       // nClipPrecision
		DEFAULT_QUALITY,           // nQuality
		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
		_T("微软雅黑"));

	// tab设置
	m_MainTab.SetDirectMode(CColumnCtrl::EN_X_DIRE);
	m_MainTab.SetFont(&m_font);
	m_MainTab.AddItem("默认头像");
	m_MainTab.AddItem("自定义头像");
	m_MainTab.SetBackgroundColor(RGB(212, 212, 212), RGB(255, 255, 255), RGB(235, 235, 235));
	m_MainTab.SetItemTextColor(RGB(0,0,0), RGB(0,0,0), RGB(0,0,0));
	m_MainTab.SetFrameModol(FRAME_DRAW_NULL);

	m_pSelDefHeadDlg = NULL;
	m_pSelCustomHeadDlg = NULL;
	m_pPreviewPic = NULL;

	// 下载默认头像
	for(int i = 1; i <= 16; ++i)
	{
		CString strUrl,strIcon;
		strIcon.Format("%s%d%s","icon%28",i,"%29.png");
		strUrl = HEADICON_URL + strIcon;
		CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)strUrl);
		if(!FileExist(strLocalPath))
		{
			DownLoadWebFile(strUrl, strLocalPath);
		}

		if(FileExist(strLocalPath))
		{
			m_mapDefHeadLocal_Url.insert(make_pair((LPTSTR)(LPCTSTR)strLocalPath, (LPTSTR)(LPCTSTR)strUrl));
		}
	}
}

CModHeadIconDlg::~CModHeadIconDlg()
{
	if(m_pSelDefHeadDlg != NULL)
	{
		if(::IsWindow(m_pSelDefHeadDlg->GetSafeHwnd()))
		{
			m_pSelDefHeadDlg->DestroyWindow();
		}
		delete m_pSelDefHeadDlg;
		m_pSelDefHeadDlg = NULL;
	}

	if(m_pSelCustomHeadDlg != NULL)
	{
		delete m_pSelCustomHeadDlg;
		m_pSelCustomHeadDlg = NULL;
	}
	if(m_pPreviewPic != NULL)
	{
		if(::IsWindow(m_pPreviewPic->GetSafeHwnd()))
		{
			m_pPreviewPic->DestroyWindow();
		}
		delete m_pPreviewPic;
		m_pPreviewPic = NULL;
	}
}

void CModHeadIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CXSkinDlg::DoDataExchange(pDX);
	DDX_Control(pDX, IDOK, m_btnOK);
	DDX_Control(pDX, IDCANCEL, m_btnCancel);
	DDX_Control(pDX, IDC_STATIC_TAB, m_MainTab);
}


BEGIN_MESSAGE_MAP(CModHeadIconDlg, CXSkinDlg)
	ON_WM_PAINT()
	ON_WM_ERASEBKGND()
	SET_HMG_SCROLLBAR(IDC_WORK_AREA)
	ON_WM_CTLCOLOR()
	ON_MESSAGE(WM_SELDEFHEADDLG_MSG, OnSelDefHeadDlgMsg)
	ON_MESSAGE(WM_COLUMNCTRL_SEL_CHANGE, OnColumnctrlSelChange)
	ON_MESSAGE(WM_PIC_PREVIEW_MSG, OnPicPreviewMsg)
	ON_XT_MESSAGE()
	ON_WM_TIMER()
END_MESSAGE_MAP()

BEGIN_XT_MESSAGE_MAP(CModHeadIconDlg)
	ON_XT_COMMAND(CDM_HEAD_FILE_UPLOAD_ACK,OnHeadFileUploadAck)
END_XT_MESSAGE_MAP()

// CModHeadIconDlg 消息处理程序
BOOL CModHeadIconDlg::OnInitDialog()
{
	CXSkinDlg::OnInitDialog();
	g_pProtocol->RegistCmd(CDM_HEAD_FILE_UPLOAD_ACK,m_hWnd);
	// 设置控件字体
	CWnd* pChild = GetNextWindow(GW_CHILD);
	while (pChild != NULL)
	{
		pChild->SetFont(&g_font12);
		pChild = pChild->GetNextWindow(GW_HWNDNEXT);
	}

	m_btnOK.SetPng(IDB_PNG_SENIOR_BTN_HOVER, IDB_PNG_SENIOR_BTN_NORMAL, IDB_PNG_SENIOR_BTN_PRESSED, IDB_PNG_SENIOR_BTN_PRESSED);
	m_btnCancel.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
	m_btnCancel.SetTextColor(0x2E2E2E);

	GetDlgItem(IDC_STATIC_PREVIEW)->SetFont(&g_font12);

	SetWindowText("修改头像");

	// 调整tab栏的位置
	CRect rcDlg;
	GetClientRect(&rcDlg);
	CRect rcStatic;
	GetDlgItem(IDC_STATIC_TAB)->GetWindowRect(&rcStatic);
	ScreenToClient(&rcStatic);
	m_rcTab.top = TITLE_BAR_CY + 1;
	m_rcTab.bottom = m_rcTab.top + rcStatic.Height();
	m_rcTab.left = 1;
	m_rcTab.right = rcDlg.right - 1;
	m_MainTab.MoveWindow(&m_rcTab);
	m_MainTab.SetCurSel(0);

	// 计算按钮栏区域大小
	CRect rcOK;
	m_btnOK.GetWindowRect(&rcOK);
	ScreenToClient(&rcOK);
	m_rcBottonArea.left = 1; 
	m_rcBottonArea.right = rcDlg.right - 1;
	m_rcBottonArea.bottom = rcDlg.bottom - 1;
	m_rcBottonArea.top = rcOK.top - (rcDlg.bottom - rcOK.bottom);
	
	// 计算工作区的大小
	m_rcWorkArea.left = 1;
	m_rcWorkArea.right = rcDlg.Width()*2/3;
	m_rcWorkArea.top = m_rcTab.bottom;
	m_rcWorkArea.bottom = m_rcBottonArea.top;

	// 工作区与预览区之间的分割线所在区域
	m_rcSep.top = m_rcWorkArea.top;
	m_rcSep.bottom = m_rcWorkArea.bottom;
	m_rcSep.left = m_rcWorkArea.right;
	m_rcSep.right = m_rcSep.left + 5;

	// 计算预览区的大小
	m_rcPreview.left = m_rcSep.right;
	m_rcPreview.right = rcDlg.right - 1;
	m_rcPreview.top = m_rcTab.bottom;
	m_rcPreview.bottom = m_rcBottonArea.top;

	// 创建子窗口
	vector<string> vecHeadPath;
	map<string, string>::iterator it = m_mapDefHeadLocal_Url.begin();
	for(; it != m_mapDefHeadLocal_Url.end(); ++it)
	{
		vecHeadPath.push_back(it->first);
	}

	m_pSelDefHeadDlg = new CSelDefHeadIconDlg(vecHeadPath);
	m_pSelDefHeadDlg->Create(this);
	m_pSelDefHeadDlg->SetDlgCtrlID(IDC_WORK_AREA);
	m_pSelDefHeadDlg->MoveWindow(&m_rcWorkArea);
	m_pSelDefHeadDlg->ShowWindow(SW_SHOW);

	m_pSelCustomHeadDlg = new CSelCustomHeadIconDlg;
	m_pSelCustomHeadDlg->Create(CSelCustomHeadIconDlg::IDD, this);
	m_pSelCustomHeadDlg->MoveWindow(&m_rcWorkArea);
	m_pSelCustomHeadDlg->ShowWindow(SW_HIDE);

	AttachHMGScrollbar(m_pSelDefHeadDlg->m_hWnd);

	// 预览图片显示区域
	m_pPreviewPic = new CRountFaceCtrl(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL);
	m_pPreviewPic->Create(this, 50002);
	CRect rcPic;
	m_pPreviewPic->GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	CRect rcPicNew;
	rcPicNew.left = m_rcPreview.left + (m_rcPreview.Width()-rcPic.Width())/2;
	rcPicNew.right = rcPicNew.left + rcPic.Width();
	rcPicNew.top = m_rcPreview.top + (m_rcPreview.Height()*2/3-rcPic.Height())/2;
	rcPicNew.bottom = rcPicNew.top + rcPic.Height();
	m_pPreviewPic->MoveWindow(&rcPicNew);
	m_pPreviewPic->ShowWindow(SW_SHOW);

	return TRUE;
}

void CModHeadIconDlg::OnOK()
{
	CXSkinDlg::OnOK();
	if(m_nHeadType == HEAD_TYPE_DEFULT)
		::SendMessage(GetParent()->GetSafeHwnd(), WM_MOD_HEAD_DLG_MSG, IDOK, 0);
	else if(m_nHeadType == HEAD_TYPE_CUSTOM)
	{
		SetTimer(1, 500, NULL);
	}
}

void CModHeadIconDlg::OnCancel()
{
	CXSkinDlg::OnCancel();
	::SendMessage(GetParent()->GetSafeHwnd(), WM_MOD_HEAD_DLG_MSG, IDCANCEL, 0);
}

void CModHeadIconDlg::OnPaint()
{
	CXSkinDlg::OnPaint();
}

BOOL CModHeadIconDlg::OnEraseBkgnd(CDC* pDC)
{
	CXSkinDlg::OnEraseBkgnd(pDC);

	// 绘制按钮栏
	pDC->FillSolidRect(&m_rcBottonArea, RGB(235, 235, 235));

	// 绘制图标选择区与预览区之间的分割线
	CPen pen(PS_SOLID, 1, RGB(215, 215, 215));
	CPen *pOldPen = pDC->SelectObject(&pen);
	pDC->MoveTo(m_rcSep.left + m_rcSep.Width()/2, m_rcSep.top);
	pDC->LineTo(m_rcSep.left + m_rcSep.Width()/2, m_rcSep.bottom);
	pDC->SelectObject(pOldPen);

	return FALSE;
}

HBRUSH CModHeadIconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}
	pDC->SetTextColor(0x2E2E2E);

	return hbr;
}

// 选择默认头像的窗口的自定义消息响应
LRESULT CModHeadIconDlg::OnSelDefHeadDlgMsg(WPARAM wp, LPARAM lp)
{
	en_selhead_msg_type msg_type = (en_selhead_msg_type)wp;
	if(msg_type == en_selhead_msg_type::en_selchange)
	{	// 选择改变
		string strHeadPath = m_pSelDefHeadDlg->GetSelHead();
		m_pPreviewPic->SetPicPath(strHeadPath.c_str());
	}

	return 0;
}

LRESULT CModHeadIconDlg::OnPicPreviewMsg(WPARAM wp, LPARAM lp)
{
	CBitmap* pBitmap = (CBitmap*)wp;

	CString str;
	str.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));
	m_strFilePath = str;

	CString strCacheFileName;
	g_LocalData.SaveImageFileToCache(str,m_strFilePath);
	m_strFilePath = g_LocalData.GetImageFileCachePath() + m_strFilePath;
	m_pPreviewPic->SetBitmap(pBitmap, m_strFilePath);
	return 0;
}
// 创建
BOOL CModHeadIconDlg::Create(CWnd *pParentWnd)
{
	BOOL bRlt = CXSkinDlg::Create(IDD_DLG_MOD_HEAD_ICON, pParentWnd);
	if(bRlt)
	{	// 居中显示窗口
		CenterWindow(pParentWnd);
		ShowWindow(SW_SHOW);
	}

	return bRlt;
}

// 获取选择的头像的url
string CModHeadIconDlg::GetHeadUrl()
{
	string strRet;
	string strLocal = m_pSelDefHeadDlg->GetSelHead();
	if(!strLocal.empty())
	{
		strRet =  m_mapDefHeadLocal_Url[strLocal];
	}
	return strRet;
}

// 菜单栏选着改变事件
LRESULT CModHeadIconDlg::OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam)
{
	CColumnCtrl *pCtrl = (CColumnCtrl*)wParam;
	int nSel = pCtrl->GetCurSel();
	switch(nSel)
	{
	case 0:
		{
			if(m_pSelDefHeadDlg != NULL)
				m_pSelDefHeadDlg->ShowWindow(SW_SHOW);
			if(m_pSelCustomHeadDlg != NULL)
				m_pSelCustomHeadDlg->ShowWindow(SW_HIDE);
			m_nHeadType = HEAD_TYPE_DEFULT;
		}
		break;
	case 1:
		{
			if(m_pSelDefHeadDlg != NULL)
				m_pSelDefHeadDlg->ShowWindow(SW_HIDE);
			if(m_pSelCustomHeadDlg != NULL)
				m_pSelCustomHeadDlg->ShowWindow(SW_SHOW);
			m_nHeadType = HEAD_TYPE_CUSTOM;
		}
		break;
	}

	return 0;
}

void CModHeadIconDlg::OnHeadFileUploadAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_HEAD_FILE_UPLOAD_ACK ack = (LPXT_HEAD_FILE_UPLOAD_ACK)data;

		if(ack->type == 1)
		{
			Sleep(100);
			SEND_FILE_STRUCT *pSend = new SEND_FILE_STRUCT();

			pSend->file_id   = m_imageFile.file_session_id;
			pSend->file_size = m_imageFile.file_size;
			pSend->window_size = 6;
			pSend->send_size = 0;
			sprintf(pSend->file_name,m_imageFile.file_name);

			char buffer[FILE_PACKET_LEN];
			memset(buffer, 0, sizeof(buffer));
			pSend->fileObject=new CStdioFile();
			m_vecSendFiles.push_back(pSend);
			int nRead  = 0;
			if ( pSend->fileObject->Open(m_strFilePath,CFile::modeRead|CFile::typeBinary|CFile::shareDenyNone) )
			{
				while( nRead=pSend->fileObject->Read(buffer,FILE_PACKET_LEN) )
				{
					char crypt[MAX_SEND_BUFFERS];
					CSendPacket spMsg(crypt);

					spMsg<<pSend->file_id<<pSend->last_packet_id<<pSend->last_file_pointer;
					spMsg.WriteData(buffer,nRead);
					int datalen=spMsg.GetSize();

					pSend->nPacketCount = nRead;
					pSend->last_packet_id++;
					pSend->last_file_pointer+=nRead ;
					pSend->send_size+=nRead;
					Sleep(50);
					g_pProtocol->SendHeadFileUploadPacketReq(*pSend, buffer, nRead);
				}
			}
		}
		else if(ack->type == 2)
		{
			Sleep(50);
			if(ack->uploadFinish == 1)
			{
				Sleep(50);
				FRIEND_INFO user = g_ImUserInfo;
				strcpy(user.face_url, ack->face_url);
				g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);

				for(int nFile = 0; nFile < m_vecSendFiles.size(); nFile++)
				{
					SEND_FILE_STRUCT *p = m_vecSendFiles[nFile];
					if ( p->fileObject )
					{
						try
						{
							if(p->fileObject->m_hFile != (HANDLE)0xFFFFFFFF)//zhangmin
							{
								p->fileObject->Close();
								delete p->fileObject;
								p->fileObject = NULL;
							}
						}
						catch(...)
						{
						}
					}
				}

				if(strcmp(ack->face_url, "") == 0)
				{
					AfxMessageBox("头像上传失败!");
					return;
				}
			}
		}
	}
	else
	{
		//无法上传头像
	}
}

void CModHeadIconDlg::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	if( nIDEvent == 1)
	{	
		if(GetFileSize(m_strFilePath) > 0)
		{
			CString strPath,strFileName,strExt;
			ExtractFilePath(m_strFilePath,strPath,strFileName,strExt);

			static int nSessionId = ((rand()<<15)|(rand()<<2))|(rand()>>13);
			m_imageFile.file_session_id   = nSessionId++;
			m_imageFile.file_size         = GetFileSize(m_strFilePath);
			ASSERT(m_imageFile.file_size > 0);
			sprintf(m_imageFile.file_name, "%s%s", strFileName, strExt ); 
			//AfxMessageBox("发送1");
			g_pProtocol->SendHeadFileUploadReq(m_imageFile);
			KillTimer(1);
		}
	}
	
	CXSkinDlg::OnTimer(nIDEvent);
}