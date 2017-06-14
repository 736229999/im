#include "StdAfx.h"
#include "DuiModHeadIconDlg.h"
#include "GdiPlus.h"
#include "Im\impublic.h"
#include "Im\WebProcess.h"
#include "../../../Common/Common.h"
#include "XmlAddress.h"
#include "ModHeadIconDlg.h"
#include "im/localdata.h"
#include "im/xtpacket.h"

using namespace DuiLib;
extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
// �ϴ�ͼƬ��ͼƬ�ռ�
extern BOOL upLoadPic(const char* szPic, string &strURL);

BEGIN_XT_MESSAGE_MAP(CDuiModHeadIconDlg)
	//ON_XT_COMMAND(CDM_HEAD_FILE_UPLOAD_ACK,OnHeadFileUploadAck)
END_XT_MESSAGE_MAP()

CDuiModHeadIconDlg::CDuiModHeadIconDlg(void)
{
	m_strChoseFilePath = _T("");
	m_bGifFlag = FALSE;
}


CDuiModHeadIconDlg::~CDuiModHeadIconDlg(void)
{
}

DuiLib::UILIB_RESOURCETYPE CDuiModHeadIconDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiModHeadIconDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

std::tr1::shared_ptr<CDuiModHeadIconDlg> CDuiModHeadIconDlg::pInstance = NULL;
std::tr1::shared_ptr<CDuiModHeadIconDlg> CDuiModHeadIconDlg::Show()
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiModHeadIconDlg>(new CDuiModHeadIconDlg());
		pInstance->Create(GetDesktopWindow(), _T("�޸�ͷ��"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
	}

	return pInstance;
}

void CDuiModHeadIconDlg::InitWindow()
{
	SetIcon(IDR_MAINFRAME);
	g_pProtocol->RegistCmd(CDM_HEAD_FILE_UPLOAD_ACK,m_hWnd);

	UpdateBk();

	//װ��GDI+
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

	//����߿��ʼ��
	m_rectTracker.m_nStyle = CRectTracker::resizeInside | CRectTracker::solidLine;
	m_rectTracker.SetTrackerColor(RGB(0x3F,0xB5,0xFF));
	m_rectTracker.SetTrackerType(TRACKER_TYPE_HEAD);

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
			m_mapDefHeadLocal_Url.insert(make_pair(/*(LPTSTR)(LPCTSTR)*/strLocalPath, /*(LPTSTR)(LPCTSTR)*/strUrl));
		}
	}
	int nDefaultHead = 0;
	map<CString, CString>::iterator it = m_mapDefHeadLocal_Url.begin();
 	for(; it != m_mapDefHeadLocal_Url.end(), nDefaultHead < 16; ++it, nDefaultHead++)
 	{
 		CString strPath = it->first/*.c_str()*/;
 
 		string strImg = CutRoundImg(strPath, 52, 52, FALSE);
 		strPath = strImg.c_str();
 
 		CString strHead;
 		strHead.Format("HLdefaulthead%d", nDefaultHead + 1);
 
 		CHorizontalLayoutUI* pHLHead = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T(strHead)));
 
 		pHLHead->SetBkImage(strPath);
 	}
	
	/////�����Զ���ؼ�
	m_pHLPreviewHead = static_cast<CButtonGifUI*>(m_PaintManager.FindControl("PreviewHead"));
	////
	m_tabSelfInfo = static_cast<CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_btnDefaultHead = static_cast<COptionUI*>(m_PaintManager.FindControl("defaulthead"));
	m_btnCustomHead = static_cast<COptionUI*>(m_PaintManager.FindControl("customhead"));
	m_btnUpload = static_cast<CButtonUI*>(m_PaintManager.FindControl("btnUpload"));
	m_labelPicture = static_cast<CLabelUI*>(m_PaintManager.FindControl("custompicture"));
	m_btnSave = static_cast<CButtonUI*>(m_PaintManager.FindControl("btnsave"));
	m_btnCancel = static_cast<CButtonUI*>(m_PaintManager.FindControl("btncancel"));
	m_nHandle = -1;
	m_nTrackerRectWidth = 88;//Ԥ��ͷ���ȣ�88
	m_nTrackerRectHeight = 88;//Ԥ��ͷ��߶ȣ�88
	m_strDefHead = "";
}

LRESULT CDuiModHeadIconDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}
	if(WM_MOUSEMOVE == uMsg)
	{
		DuiLib::CPoint point(lParam);
		OnMouseMove(wParam, point);
	}
	if(WM_SETCURSOR == uMsg)
	{
		if( m_rectTracker.SetCursor(CWnd::FromHandle(reinterpret_cast<HWND>(wParam)),LOWORD(lParam)))
			return 0;
	}
	if(WM_LBUTTONDOWN == uMsg)
	{
		DuiLib::CPoint point(lParam);
		if(m_rectTracker.HitTest(point)>=0)
		{
			m_nMoveX = 0;
			m_nMoveY = 0;
			m_nDownX = point.x;
			m_nDownY = point.y;
			m_rectLast = m_rect;
			m_nHandle = m_rectTracker.HitTestHandles(point);
			SetTimer(GetHWND(), 1, 100, NULL);
		}
	}
	if(WM_LBUTTONUP == uMsg)
	{
		DuiLib::CPoint point(lParam);
		m_nHandle = -1;
		m_nMoveX = 0;
		m_nMoveY = 0;
		m_nDownX = point.x;
		m_nDownY = point.y;
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

LRESULT CDuiModHeadIconDlg::HandleCustomMessage(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	if(uMsg == WM_TIMER)
	{
		OnTimer(uMsg, wParam, lParam, bHandled); 
	}
	return DuiLib::WindowImplBase::HandleCustomMessage(uMsg, wParam, lParam, bHandled);
}

void CDuiModHeadIconDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if(msg.sType == "click")
	{
		int nDefaultHead = 0;
		map<CString, CString>::iterator it = m_mapDefHeadLocal_Url.begin();
		for(; it != m_mapDefHeadLocal_Url.end(), nDefaultHead < 16; ++it, nDefaultHead++)
		{
			CString strHead;
			strHead.Format("defaulthead%d", nDefaultHead + 1);
			CButtonUI* pbtnHead = static_cast<CButtonUI*>(m_PaintManager.FindControl(_T(strHead)));

			if(msg.pSender == pbtnHead)
			{
				CString strPath = it->first/*.c_str()*/;
				m_strDefHead = it->second/*.c_str()*/;
				string strImg = CutRoundImg(strPath, 88, 88, FALSE);
				strPath = strImg.c_str();
				m_pHLPreviewHead->SetBkImage(strPath);
				m_pHLPreviewHead->SetNormalGifFile(_T(""));
			}
		}
		if(msg.pSender == m_btnDefaultHead)///Ĭ�ϼ���
		{
			m_tabSelfInfo->SelectItem(0);
		}
		else if(msg.pSender == m_btnCustomHead)///�Զ���ͷ��
		{
			m_tabSelfInfo->SelectItem(1);
		}
		else if(msg.pSender == m_btnUpload)
		{
			ClickedBtnSelectpic();
		}
		else if(msg.pSender == m_btnSave)
		{
			if(m_btnDefaultHead->IsSelected())///Ĭ��
			{
				if(m_strDefHead != "")
				{
					FRIEND_INFO user = g_ImUserInfo;
					strcpy(user.face_url, m_strDefHead);
					//strcpy(g_ImUserInfo.face_url, m_strDefHead);
					g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);
				}
				Close();
			}
			else if(m_btnCustomHead->IsSelected())////�Զ���
			{
				if(m_strFilePath != "")
				{
					/*CString strPath,strFileName,strExt;
					ExtractFilePath(m_strFilePath,strPath,strFileName,strExt);*/

					// ֱ���ϴ���ͼƬ�ռ�
					string strUrl;
					if(upLoadPic(m_strFilePath, strUrl))
					{
						FRIEND_INFO user = g_ImUserInfo;
						strcpy(user.face_url, strUrl.c_str());
						g_pProtocol->SendReq((XT_USERINFO_MOD_REQ)user);
						Close();
					}
					else
					{	// �ϴ�ͷ��ʧ��
						CMessageBox::Model(GetHWND(), "�޸�ͷ��ʧ��","��ʾ",MB_OK|MB_ICONWARNING);
					}
					/*static int nSessionId = ((rand()<<15)|(rand()<<2))|(rand()>>13);
					m_imageFile.file_session_id   = nSessionId++;
					m_imageFile.file_size         = GetFileSize(m_strFilePath);
					ASSERT(m_imageFile.file_size > 0);
					sprintf(m_imageFile.file_name, "%s%s", strFileName, strExt ); 
					g_pProtocol->SendHeadFileUploadReq(m_imageFile);*/
				}
			}
		}
		else if(msg.pSender == m_btnCancel)
		{
			Close();
		}
	}
}

void CDuiModHeadIconDlg::ClickedBtnSelectpic()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	CString sFilter = "ͷ��(*.bmp;*jpg;*png)|*.bmp;*jpg;*png||";
	CFileDialog fileDlg(TRUE, 
		NULL, 
		NULL, 
		OFN_HIDEREADONLY|OFN_OVERWRITEPROMPT|OFN_NOCHANGEDIR, 
		sFilter,
		CWnd::FromHandle(GetDesktopWindow()));
	fileDlg.m_ofn.lpstrTitle = "ѡ��ͷ���ļ�";
	char szBuf[256];
	memset(szBuf, 0, sizeof(szBuf));

	//���ļ�Ŀ¼��ΪĬ��Ŀ¼
	//GetCurrentDirectory(sizeof(szBuf), szBuf);
	//fileDlg.m_ofn.lpstrInitialDir = szBuf;	
	CString strPicFile = "";
	if(IDOK==fileDlg.DoModal())
	{
//  		m_nTrackerRectWidth = 88;
//  		m_nTrackerRectHeight = 88;
//  		CString strRecordInfo;
  		m_strChoseFilePath = fileDlg.GetPathName();
		CFile file;
	    if(file.Open(m_strChoseFilePath,CFile::modeRead))
		{
			double nFileSize = file.GetLength();
			if(nFileSize>200*1024)////200K���ϵ��ļ�������
			{
				CString str;
				str.Format("��ѡ���ͼƬ����200K,������ѡ��");
				CMessageBox::Model(GetHWND(), str,"��ʾ",MB_OK|MB_ICONWARNING);
				file.Close();
				return ;
			}
			else
			{
				file.Close();
			}
		}
		//������ѡͼƬ��ʾ��Ԥ����(.gif��ʾ������������)
		if(m_strChoseFilePath.Find(".gif") != string::npos)////�Ƿ�̬��ʾ
		{
			m_bGifFlag = TRUE;
		}
		else
		{
			m_bGifFlag = FALSE;
		}
		if(m_bGifFlag)
		{
			m_pHLPreviewHead->SetBkImage(_T(""));
			m_pHLPreviewHead->SetNormalGifFile(m_strChoseFilePath);
			strPicFile = m_strChoseFilePath;
		}

		///////
		CImage imagePic; //ʹ��ͼƬ��
		imagePic.Load( m_strChoseFilePath );
		CRect rc/*(31, 127, 281, 327);/ **/ = m_labelPicture->GetPos();
		rc.right = rc.left + 250;
		rc.bottom = rc.top + 200;
		//m_labelPicture->SetPos(rc);

		HDC hDC = GetDC(GetHWND());
		Graphics graphics3(hDC);

		Rect rcInit(rc.left,rc.top,rc.Width(),rc.Height());
		SolidBrush brush(Color(255,255,255));
		graphics3.FillRectangle(&brush,rcInit);

		CRect rcNew;
		rcNew.left = rc.left;
		rcNew.top = rc.top;

		if(imagePic.GetWidth() > rc.Width() || imagePic.GetHeight() > rc.Height())
		{
			if(imagePic.GetWidth() / (double)rc.Width() > imagePic.GetHeight() / (double)rc.Height())
			{
				rcNew.right = rcNew.left + rc.Width();
				rcNew.bottom = rcNew.top + imagePic.GetHeight() * (double)rc.Width() / imagePic.GetWidth();
			}
			else if(imagePic.GetWidth() / (double)rc.Width() < imagePic.GetHeight() / (double)rc.Height())
			{
				rcNew.bottom = rcNew.top + rc.Height();
				rcNew.right = rcNew.left + imagePic.GetWidth() * (double)rc.Height() / imagePic.GetHeight();
			}
			else if(imagePic.GetWidth() / (double)rc.Width() == imagePic.GetHeight() / (double)rc.Height())
			{
				rcNew.right = rcNew.left + rc.Width();
				rcNew.bottom = rcNew.top + rc.Height();
			}
		}
		else
		{
			rcNew.right = rcNew.left + imagePic.GetWidth();
			rcNew.bottom = rcNew.top + imagePic.GetHeight();
		}
		m_rcNew = rcNew;

		Graphics graphics(hDC);
		char *p=(LPSTR)(LPCSTR)m_strChoseFilePath;

		WCHAR buf[1024];
		CStringW strWide = CT2CW(m_strChoseFilePath); // ��T�ַ�ת���ɿ��ַ�
		wcscpy(buf, strWide); 

		Image image(buf, FALSE);
		graphics.DrawImage(&image, rcNew.left, rcNew.top, rcNew.Width(), rcNew.Height());  //����ͼƬ

		int nLeft = rcNew.left + (rcNew.Width() - m_nTrackerRectWidth) / 2;
		int nRight = nLeft + m_nTrackerRectWidth;
		int nTop = rcNew.top + (rcNew.Height() - m_nTrackerRectHeight) / 2;
		int nBottom = nTop + m_nTrackerRectHeight;

		if(rcNew.Width() < m_nTrackerRectWidth)
		{
			nLeft = 0;
			nRight = rcNew.Width();
		}
		if(rcNew.Height() < m_nTrackerRectHeight)
		{
			nTop = 0;
			nBottom = rcNew.Height();
		}
		m_rect.SetRect(nLeft, nTop, nRight, nBottom);
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);

		CDC *pDC = CDC::FromHandle(hDC);
		CBitmap bitmap;
		//bitmap.DeleteObject();
		bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
		CDC memDcInit;
		memDcInit.CreateCompatibleDC(pDC);


		CBitmap *oldmemBitmapInit = memDcInit.SelectObject(&bitmap);
		memDcInit.BitBlt(0, 0, m_rect.Width(),m_rect.Height(), pDC, m_rect.left, m_rect.top, SRCCOPY);  
		memDcInit.SelectObject(oldmemBitmapInit);
		memDcInit.DeleteDC();
		
		
	    /*if(PathFileExists(m_strFilePath))
		{
			DeleteFile(m_strFilePath);
		}*/
		CString strPath;
		strPath.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));

		CString strCacheFileName;
		g_LocalData.SaveImageFileToCache(strPath,strCacheFileName);
		m_strFilePath = g_LocalData.GetImageFileCachePath() + strCacheFileName;
		HBITMAP hbmp = (HBITMAP)bitmap.GetSafeHandle();
		CImage img;
		img.Attach(hbmp);
		img.Save(m_strFilePath);
		string strImg = CutRoundImg((LPSTR)(LPCSTR)m_strFilePath, 88, 88, FALSE);
		strPath = strImg.c_str();

		
		if(m_bGifFlag)
		{
			m_strFilePath = strPicFile;///֮ǰ�����·��
		}
		else
		{
			m_pHLPreviewHead->SetNormalGifFile(_T(""));
			m_pHLPreviewHead->SetBkImage(strPath);
			m_strFilePath = strPath;////���ؾ�̬ͼƬ
		}

		CRect rcParent;
		GetWindowRect(GetHWND(), &rcParent);

		m_bitmap.DeleteObject();
		m_bitmap.CreateCompatibleBitmap(pDC, rc.Width()+4, rc.Height()+4);

		CDC memDC;
		memDC.CreateCompatibleDC(pDC);


		CBitmap *oldmemBitmap = memDC.SelectObject(&m_bitmap);
		memDC.BitBlt(0, 0, rc.Width()+4,rc.Height()+4, pDC, rc.left-2, rc.top-2, SRCCOPY);  
		memDC.SelectObject(oldmemBitmap);

		memDC.DeleteDC();
		oldmemBitmap->DeleteObject();

		if(!m_bGifFlag) ///gif�����ư�ɫ����
		{
			//��Ӱ�ɫ���߳ʾŹ���
			Graphics graphics2(hDC);
			Pen pen(Color(255, 255, 255, 255), 1);
			pen.SetDashStyle(DashStyleDash);//����  
			graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3, m_rect.top, m_rect.left + m_rect.Width() / 3, m_rect.bottom);
			graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3 * 2, m_rect.top, m_rect.left + m_rect.Width() / 3 * 2, m_rect.bottom);
			graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3, m_rect.right, m_rect.top + m_rect.Height() / 3);
			graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3 * 2, m_rect.right, m_rect.top + m_rect.Height() / 3 * 2);
			CRect rect;
			m_rectTracker.GetTrueRect(&rect);
			pDC->SelectStockObject(NULL_BRUSH);  
			m_rectTracker.Draw(pDC/*, &pen*/);
			ReleaseDC(GetHWND(), hDC);
			pDC->DeleteDC();
		}
	}
}

void CDuiModHeadIconDlg::OnMouseMove(UINT nFlags, DuiLib::CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	if(m_nHandle == -1)
		return;

	//��ȡ����ƶ��Ĵ�С
	m_nMoveX = m_nDownX - point.x;
	m_nMoveY = m_nDownY - point.y;

	CRect rcPic = m_rcNew;//m_labelPicture->GetPos();
	//m_staticPic.GetWindowRect(&rcPic);
	//ScreenToClient(&rcPic);
	//rcPic.right = rcPic.left + m_rectPic.Width();
	//rcPic.bottom = rcPic.top + m_rectPic.Height();

	if(m_nHandle == /*hitTopLeft*/0)//�����϶�
	{
		if( m_rectLast.left - m_nMoveX >= rcPic.left/*+1*/ && m_rectLast.top - m_nMoveY >= rcPic.top/*+1*/ )
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.bottom = m_rectLast.bottom;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitTopRight*/1)//�����϶�
	{
		if( m_rectLast.right - m_nMoveX <= rcPic.right/*-1*/ && m_rectLast.top - m_nMoveY >= rcPic.top/*+1*/ )
		{
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
		}

		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottomRight*/2)//�����϶�
	{
		if( m_rectLast.right - m_nMoveX <= rcPic.right/*-1*/ && m_rectLast.bottom - m_nMoveY <= rcPic.bottom/*-1*/ )
		{
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.left = m_rectLast.left;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottomLeft*/3)//�����϶�
	{
		if(m_rectLast.left - m_nMoveX >= rcPic.left/*+1*/ && m_rectLast.bottom - m_nMoveY <= rcPic.bottom/*-1*/)
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitTop*/4)//�����϶�
	{
		if( m_rectLast.top - m_nMoveY >= rcPic.top/*+1*/)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top - m_nMoveY;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitRight*/5)//�����϶�
	{
		if(m_rectLast.right - m_nMoveX <= rcPic.right/*-1*/)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitBottom*/6)//�����϶�
	{
		if(m_rectLast.bottom - m_nMoveY <= rcPic.bottom/*-1*/)
		{
			m_rect.left = m_rectLast.left;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitLeft*/7)//�����϶�
	{
		if(m_rectLast.left - m_nMoveX >= rcPic.left/*+1*/)
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom;
			m_rect.right = m_rectLast.right;
			m_rect.top = m_rectLast.top;
		}
		if(m_rect.Width() < m_nTrackerRectWidth || m_rect.Height() < m_nTrackerRectHeight)
		{
			m_rect = m_rectLast;
			return;
		}
		m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
	}
	if(m_nHandle == /*hitMiddle*/8)//�ƶ���������
	{
		if(m_rectLast.left - m_nMoveX <= rcPic.left/*+1*/ || m_rectLast.right - m_nMoveX >= rcPic.right/*-1*/)
		{
			if(m_rectLast.bottom - m_nMoveY <= rcPic.bottom/*-1*/ &&  m_rectLast.top - m_nMoveY >= rcPic.top/*+1*/)
			{
				m_rect.bottom = m_rectLast.bottom - m_nMoveY;
				m_rect.top = m_rectLast.top - m_nMoveY;
			}
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
		else if(m_rectLast.bottom - m_nMoveY >= rcPic.bottom/*-1*/ ||  m_rectLast.top - m_nMoveY <= rcPic.top/*+1*/)
		{
			if(m_rectLast.left - m_nMoveX >= rcPic.left/*+1*/ && m_rectLast.right - m_nMoveX <= rcPic.right/*-1*/)
			{
				m_rect.left = m_rectLast.left - m_nMoveX;
				m_rect.right = m_rectLast.right - m_nMoveX;
			}
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
		else
		{
			m_rect.left = m_rectLast.left - m_nMoveX;
			m_rect.bottom = m_rectLast.bottom - m_nMoveY;
			m_rect.right = m_rectLast.right - m_nMoveX;
			m_rect.top = m_rectLast.top - m_nMoveY;
			m_rectTracker.m_rect.SetRect(m_rect.left, m_rect.top, m_rect.right, m_rect.bottom);
		}
	}
	m_rectLast = m_rect;

	//ˢ�±���ͼƬ������֮ǰ���Ŀ�ѡ��
	Bitmap *pBmp = Bitmap::FromHBITMAP((HBITMAP)m_bitmap.m_hObject, NULL);
	if(pBmp == NULL)
		return;
	Graphics bmpGraphics(pBmp);
	bmpGraphics.SetSmoothingMode(SmoothingModeAntiAlias);

	CRect rc = m_labelPicture->GetPos();
	//m_staticPic.GetWindowRect(&rc);
	//ScreenToClient(&rc); //�ӿؼ���Ļ����ӳ�䵽�ؼ��ͻ�����

	HDC hDC = GetDC(GetHWND());
	CDC *pDC = CDC::FromHandle(hDC);
	/*Drawing on DC*/
	Graphics graphics(hDC);
	/*Important! Create a CacheBitmap object for quick drawing*/
	CachedBitmap cachedBmp(pBmp,&graphics);
	graphics.DrawCachedBitmap(&cachedBmp,rc.left-2,rc.top-2);

	CBitmap bitmap;
	//bitmap.DeleteObject();
	bitmap.CreateCompatibleBitmap(pDC, m_rect.Width(), m_rect.Height());
	CDC memDC;
	memDC.CreateCompatibleDC(pDC);


	CBitmap *oldmemBitmap = memDC.SelectObject(&bitmap);
	memDC.BitBlt(0, 0, m_rect.Width(),m_rect.Height(), pDC, m_rect.left, m_rect.top, SRCCOPY);  
	memDC.SelectObject(oldmemBitmap);
	memDC.DeleteDC();
	if(!m_bGifFlag)////ֻ�о�̬ͼƬ��ʾ��ɫ����
	{
	    //��ɫ���ߣ��ʾŹ���
		Graphics graphics2(pDC->m_hDC);
		Pen pen(Color(255, 255, 255, 255), 1);
		pen.SetDashStyle(DashStyleDash);//����  
		graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3, m_rect.top, m_rect.left + m_rect.Width() / 3, m_rect.bottom);
		graphics2.DrawLine(&pen, m_rect.left + m_rect.Width() / 3 * 2, m_rect.top, m_rect.left + m_rect.Width() / 3 * 2, m_rect.bottom);
		graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3, m_rect.right, m_rect.top + m_rect.Height() / 3);
		graphics2.DrawLine(&pen, m_rect.left, m_rect.top + m_rect.Height() / 3 * 2, m_rect.right, m_rect.top + m_rect.Height() / 3 * 2);
		m_rectTracker.Draw(pDC);
		//��¼����ƶ�����λ��,���ڼ����´�����ƶ��ľ���
		m_nDownX = point.x;
		m_nDownY = point.y;
	}
	
	//��ѡ������ʾ��Ԥ����
	//GetParent()->SendMessage(WM_PIC_PREVIEW_MSG, (DWORD)&bitmap);
	/*if(PathFileExists(m_strFilePath)) ////���������ļ�
	{
	DeleteFile(m_strFilePath);
	}*/
	CString strPath;
	strPath.Format("%stemp%d.jpg",g_LocalData.GetImageFileCachePath(),time(0));

	CString strCacheFileName;
	g_LocalData.SaveImageFileToCache(strPath,strCacheFileName);
	m_strFilePath = g_LocalData.GetImageFileCachePath() + strCacheFileName;
	HBITMAP hbmp = (HBITMAP)bitmap.GetSafeHandle();
	CImage img;
	img.Attach(hbmp);
	img.Save(m_strFilePath);
	string strImg = CutRoundImg((LPSTR)(LPCSTR)m_strFilePath, 88, 88, FALSE);
	strPath = strImg.c_str();
	if(m_bGifFlag)
	{
		m_pHLPreviewHead->SetBkImage(_T(""));
		m_pHLPreviewHead->SetNormalGifFile(m_strChoseFilePath);
		m_strFilePath = m_strChoseFilePath;
	}
	else
	{	
		m_pHLPreviewHead->SetBkImage(strPath);
		m_pHLPreviewHead->SetNormalGifFile(_T(""));
	}
	//if(PathFileExists(strPath))
	//	DeleteFile(strPath);

	ReleaseDC(GetHWND(), hDC);
	pDC->DeleteDC();
	oldmemBitmap->DeleteObject();
	DeleteObject(pBmp);
	delete pBmp;
	pBmp = NULL;
}

LRESULT CDuiModHeadIconDlg::OnTimer(UINT uMsg, WPARAM wParam, LPARAM lParam, BOOL& bHandled)
{
	switch (wParam)
	{
	case 1:
		{
			if(m_nHandle != -1)
			{
				DuiLib::CPoint ptCursor;
				GetCursorPos(&ptCursor);

				CRect rc = m_rcNew;//m_labelPicture->GetPos();
				CRect rcParent;
				::GetWindowRect(GetHWND(), rcParent);
				rc.left += rcParent.left;
				rc.right += rcParent.left;
				rc.top += rcParent.top;
				rc.bottom += rcParent.top;

				//m_staticPic.GetWindowRect(rc);
				if(ptCursor.x > rc.right || ptCursor.x < rc.left || ptCursor.y > rc.bottom || ptCursor.y < rc.top)
				{
					m_nMoveX = 0;
					m_nMoveY = 0;
					m_nDownX = -1;
					m_nDownY = -1;
					m_nHandle = -1;
					KillTimer(GetHWND(), 1);
				}
			}
		}
		break;
	}
	return 0;
}

void CDuiModHeadIconDlg::OnHeadFileUploadAck(const char *data,WORD wRet)
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

			char buffer[FILE_PACKET_LEN];////FILE_PACKET_LEN�������ֽ���
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
					CMessageBox::Model(NULL, "ͷ���ϴ�ʧ��!");
					return;
				}
				Close();
			}
		}
	}
	else
	{
		//�޷��ϴ�ͷ��
	}
}

void CDuiModHeadIconDlg::UpdateBk()
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