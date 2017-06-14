#include "StdAfx.h"
#include "DuiPicture.h"
#include "LocalData.h"
using namespace DuiLib;

CDuiPicture::CDuiPicture(void)
{
}


CDuiPicture::~CDuiPicture(void)
{
}

DuiLib::UILIB_RESOURCETYPE CDuiPicture::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiPicture::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

std::tr1::shared_ptr<CDuiPicture> CDuiPicture::pInstance = NULL;
std::tr1::shared_ptr<CDuiPicture> CDuiPicture::Show(std::string strFilePath)
{
	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))
	{
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->SetFilePath(strFilePath);
		pInstance->RecoverPic();
	}
	else
	{
		pInstance = tr1::shared_ptr<CDuiPicture>(new CDuiPicture());
		pInstance->Create(GetDesktopWindow(), _T("DuiPicture"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE | WS_EX_TOOLWINDOW, 0, 0, 200, 200);
		pInstance->CenterWindow();
		pInstance->ShowWindow(true);
		::BringWindowToTop(pInstance->GetHWND());
		pInstance->SetFilePath(strFilePath);
	}

	return pInstance;
}

void CDuiPicture::InitWindow()
{
	WindowImplBase::InitWindow();
	m_bIsLButtonDown = FALSE;
	m_bIsPicDrag = FALSE;
	m_pLabelPic = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelPic"));
	m_pHLBtn = static_cast<DuiLib::CHorizontalLayoutUI*>(m_PaintManager.FindControl("hlBtn"));
	m_pBtnClose = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtn"));
	m_nZoomIndex = 17;
	m_nImageAngle = 0;
}

void CDuiPicture::Notify(DuiLib::TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("btn11"))//还原成1：1
		{
			RecoverPic();
		}
		else if(msg.pSender->GetName() == _T("btnAmplification")) //放大
		{
			AmplificationPic();
		}
		else if(msg.pSender->GetName() == _T("btnNarrow")) //缩小
		{
			NarrowPic();
		}
		else if(msg.pSender->GetName() == _T("btnRefresh")) //旋转
		{

			//m_nImageAngle = m_nImageAngle + 90 == 360 ? 0: m_nImageAngle + 90;
			//m_pLabelPic->SetImageAngle(m_nImageAngle);
			//m_pLabelPic->Invalidate();
			m_nImageAngle = m_nImageAngle + 90  == 360 ? 0: m_nImageAngle + 90;
			SetPicAngle(m_nImageAngle);
		}
		else if(msg.pSender->GetName() == _T("btnSave")) //保存
		{
			CFileDialog saveFile = CFileDialog(FALSE, ".*", "", OFN_CREATEPROMPT | OFN_PATHMUSTEXIST , "图片(*.*)|*.*||");  
			CString defaultFileName = m_strFilePath.c_str();  
			saveFile.m_ofn.lpstrFile = defaultFileName.GetBuffer(MAX_PATH);  
			saveFile.m_ofn.nMaxFile = MAX_PATH;  
			if(IDOK == saveFile.DoModal())
			{  
				if(FileExist(saveFile.GetPathName()))
				{
					if(IDYES == CMessageBox::Model(GetHWND(), "文件已存在，是否覆盖该文件？", "提示", MB_YESNO))
					{
						DeleteFile(saveFile.GetPathName());
						CopyFile(m_strFilePath.c_str(), saveFile.GetPathName(), FALSE);
					}
				}
				else
					CopyFile(m_strFilePath.c_str(), saveFile.GetPathName(), FALSE);
			}
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

LRESULT CDuiPicture::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	if(WM_LBUTTONDOWN == uMsg)
	{
		m_bIsLButtonDown = TRUE;
		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		m_pointLast = point;
	}
	else if(WM_LBUTTONUP == uMsg)
	{
		m_bIsLButtonDown = FALSE;
	}
	else if(WM_LBUTTONDBLCLK == uMsg)
	{
		POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
		CRect rcClose = m_pBtnClose->GetPos();
		CRect rcHL = m_pHLBtn->GetPos();
		if(!(rcClose.PtInRect(point) || rcHL.PtInRect(point)))
			Close(IDOK);
	}
	else if(WM_MOUSEMOVE == uMsg)
	{
		if(m_bIsLButtonDown && m_bIsPicDrag)
		{
			POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			int nWidth = point.x - m_pointLast.x;
			int nHeight = point.y - m_pointLast.y;

			CRect rcWindow;
			GetWindowRect(GetHWND(), rcWindow);

			CRect rcLabelPic = m_pLabelPic->GetPos();
			if(rcLabelPic.left + nWidth <= 0 && rcLabelPic.right + nWidth >= rcWindow.Width())
			{
				rcLabelPic.left = rcLabelPic.left + nWidth;
				rcLabelPic.right = rcLabelPic.right + nWidth;
			}
			if(rcLabelPic.top + nHeight <= 0 && rcLabelPic.bottom + nHeight >= rcWindow.Height())
			{
				rcLabelPic.top = rcLabelPic.top + nHeight;
				rcLabelPic.bottom = rcLabelPic.bottom + nHeight;
			}
			m_pLabelPic->SetPos(rcLabelPic);
			m_pointLast = point;
		}
		else if(m_bIsLButtonDown && !m_bIsPicDrag)
		{
			POINT point = { GET_X_LPARAM(lParam), GET_Y_LPARAM(lParam) };
			int nWidth = point.x - m_pointLast.x;
			int nHeight = point.y - m_pointLast.y;

			CRect rcWindow;
			GetWindowRect(GetHWND(), rcWindow);
			MoveWindow(GetHWND(), rcWindow.left + nWidth, rcWindow.top + nHeight, rcWindow.Width(), rcWindow.Height(), TRUE);
		}
	}
	else if(WM_MOUSEWHEEL == uMsg)
	{
		int zDelta = (int) (short) HIWORD(wParam);
		if(zDelta > 0)
			AmplificationPic();
		else
			NarrowPic();
	}
	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CDuiPicture::SetFilePath(std::string strFilePath)
{
	m_vecZoomRect.clear();
	m_strFilePath = strFilePath;
	CImage image; //使用图片类
	image.Load( strFilePath.c_str() );
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();

	CRect rcWindow;
	GetWindowRect(GetHWND(), rcWindow);
	if(nWidth > rcWindow.Width())
	{
		rcWindow.right = rcWindow.left + nWidth;
		MoveWindow(GetHWND(), rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), TRUE);
		CenterWindow();
	}
	if(nHeight > rcWindow.Height())
	{
		rcWindow.bottom = rcWindow.top + nHeight;
		MoveWindow(GetHWND(), rcWindow.left, rcWindow.top, rcWindow.Width(), rcWindow.Height(), TRUE);
		CenterWindow();
	}

	int nLeft = (rcWindow.Width() - nWidth) / 2;
	int nTop = (rcWindow.Height() - nHeight) / 2;

	CRect rcLabelPic(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	m_pLabelPic->SetPos(rcLabelPic);

	m_pLabelPic->SetAttribute("bkimage", strFilePath.c_str());

	int nWidthNarrow = nWidth;
	int nHeightNarrow = nHeight;
	for(int nZoomIndex = 0; nZoomIndex < 16; nZoomIndex++)
	{
		nWidthNarrow = nWidthNarrow * 0.8 > 0 ? nWidthNarrow * 0.8 : nWidthNarrow;
		nHeightNarrow = nHeightNarrow * 0.8 > 0 ? nHeightNarrow * 0.8 : nHeightNarrow;
		std::vector<pair<int, int> >::iterator itr = m_vecZoomRect.begin();
		m_vecZoomRect.insert(itr, make_pair(nWidthNarrow, nHeightNarrow));
	}
	m_vecZoomRect.push_back(make_pair(nWidth, nHeight));
	int nWidthAmplification = nWidth;
	int nHeightAmplification = nHeight;
	for(int nZoomIndex = 0; nZoomIndex < 16; nZoomIndex++)
	{
		nWidthAmplification = nWidthAmplification * 1.2;
		nHeightAmplification = nHeightAmplification * 1.2;
		m_vecZoomRect.push_back(make_pair(nWidthAmplification, nHeightAmplification));
	}
}

//还原成1:1
void CDuiPicture::RecoverPic()
{
	m_nZoomIndex = 17;
	CImage image; //使用图片类
	image.Load( m_strFilePath.c_str() );
	int nWidth = image.GetWidth();
	int nHeight = image.GetHeight();

	CRect rcWindow;
	GetWindowRect(GetHWND(), rcWindow);

	int nLeft = (rcWindow.Width() - nWidth) / 2;
	int nTop = (rcWindow.Height() - nHeight) / 2;

	CRect rcLabelPic(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	m_pLabelPic->SetPos(rcLabelPic);
}

//放大
void CDuiPicture::AmplificationPic()
{
	if(m_nZoomIndex == 33)
		return;
	m_nZoomIndex++;
	int nWidth = m_vecZoomRect[m_nZoomIndex-1].first;
	int nHeight = m_vecZoomRect[m_nZoomIndex-1].second;
	SetLabelPicPos(nWidth, nHeight);
}

//缩小
void CDuiPicture::NarrowPic()
{
	if(m_nZoomIndex == 1)
		return;
	m_nZoomIndex--;
	int nWidth = m_vecZoomRect[m_nZoomIndex-1].first;
	int nHeight = m_vecZoomRect[m_nZoomIndex-1].second;
	SetLabelPicPos(nWidth, nHeight);
}

void CDuiPicture::SetLabelPicPos(int nWidth, int nHeight)
{
	CRect rcWindow;
	GetWindowRect(GetHWND(), rcWindow);

	int nLeft = (rcWindow.Width() - nWidth) / 2;
	int nTop = (rcWindow.Height() - nHeight) / 2;

	if(nLeft < 0 || nTop < 0)
	{
		m_bIsPicDrag = TRUE;
	}
	else
	{
		m_bIsPicDrag = FALSE;
	}

	CRect rcLabelPic(nLeft, nTop, nLeft + nWidth, nTop + nHeight);
	m_pLabelPic->SetPos(rcLabelPic);
}

void CDuiPicture::SetPicAngle(int nAngle)
{
	m_nZoomIndex = 17;
	CImage* image = new CImage; //使用图片类
	image->Load( m_strFilePath.c_str() );
	int imageWidth=image->GetWidth();
	int imageHeight=image->GetHeight();

	if(nAngle == 0)
	{
		m_pLabelPic->SetAttribute("bkimage", m_strFilePath.c_str());
		SetLabelPicPos(imageWidth, imageHeight);
		delete image;
		image = NULL;
		return;
	}

	CImage* imageDes = new CImage; //使用图片类
	CString strPath,strName,strExt;
	ExtractFilePath(m_strFilePath.c_str(),strPath,strName,strExt);
	std::string strFilePath;

	POINT pt[3];
	if(nAngle == 90)
	{
		pt[0].x=imageHeight;pt[0].y=0;
		pt[1].x=imageHeight;pt[1].y=imageWidth;
		pt[2].x=0;pt[2].y=0;
		strFilePath = g_LocalData.GetImageFileCachePath() + strName + "(90)" + strExt;
		imageDes->Create(imageHeight, imageWidth, 32);
	}
	else if(nAngle == 180)
	{
		pt[0].x=imageWidth;pt[0].y=imageHeight;
		pt[1].x=0;pt[1].y=imageHeight-1;
		pt[2].x=imageWidth;pt[2].y=0;
		strFilePath = g_LocalData.GetImageFileCachePath() + strName + "(180)" + strExt;
		imageDes->Create(imageWidth, imageHeight, 32);
	}
	else if(nAngle == 270)
	{
		pt[0].x=0;pt[0].y=imageWidth;
		pt[1].x=0;pt[1].y=0;
		pt[2].x=imageHeight;pt[2].y=imageWidth;
		strFilePath = g_LocalData.GetImageFileCachePath() + strName + "(270)" + strExt;
		imageDes->Create(imageHeight, imageWidth, 32);
	}

	if(FileExist(strFilePath.c_str()))
	{
		DeleteFile(strFilePath.c_str());
	}
	image->PlgBlt(imageDes->GetDC(), pt, 0,0,imageWidth,imageHeight);
	imageDes->Save(strFilePath.c_str());
	m_pLabelPic->SetAttribute("bkimage", strFilePath.c_str());

	if(nAngle == 90 || nAngle == 270)
	{
		int nTemp = imageWidth;
		imageWidth = imageHeight;
		imageHeight = nTemp;
	}
	delete image;
	image = NULL;
	//delete imageDes;
	//imageDes = NULL;
	
	SetLabelPicPos(imageWidth, imageHeight);
}