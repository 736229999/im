// PictureCtrl.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureCtrl.h"
#include "afxdialogex.h"
#include <GdiPlus.h>
#include "PictureCtrlPic.h"
#include "PictureCtrlClose.h"
#include <atlimage.h>
// CPictureCtrl �Ի���

IMPLEMENT_DYNAMIC(CPictureCtrl, CDialog)

	CPictureCtrl::CPictureCtrl(CWnd* pParent /*=NULL*/, CString strFilePath)
	: CDialog(CPictureCtrl::IDD, pParent)
{
	m_hCursor = ::LoadCursor(NULL,MAKEINTRESOURCE(IDC_HAND));
	m_brush.CreateSolidBrush(RGB(76,76,76));
	m_strFilePath = strFilePath;
	m_pPictureCtrlPic = NULL;
	m_pPictureCtrlClose = NULL;
	m_x = -1;
	m_y = -1;
	m_downX = -1;
	m_downY = -1;
	m_moveX = 0;
	m_moveY = 0;

	m_bIsBig = FALSE;
}

CPictureCtrl::~CPictureCtrl()
{
	if(m_pPictureCtrlPic != NULL)
	{
		delete m_pPictureCtrlPic;
		m_pPictureCtrlPic = NULL;
	}

	if(m_pPictureCtrlClose != NULL)
	{
		delete m_pPictureCtrlClose;
		m_pPictureCtrlClose = NULL;
	}
}

void CPictureCtrl::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureCtrl, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_MOUSEMOVE()
	ON_WM_MOVE()
	ON_WM_LBUTTONUP()
	ON_WM_TIMER()
END_MESSAGE_MAP()


// CPictureCtrl ��Ϣ�������


BOOL CPictureCtrl::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	pDC->SetBkColor(TRANSPARENT);
	// 	return TRUE; 
	// 	//return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}


BOOL CPictureCtrl::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	CenterWindow();

	SetTimer(1, 100, NULL);

	SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);

	SetLayeredWindowAttributes(RGB(77,77,77),128,LWA_COLORKEY);

	SetLayeredWindowAttributes(0,200,LWA_ALPHA);
	if(m_pPictureCtrlPic == NULL)
	{
		m_pPictureCtrlPic = new CPictureCtrlPic(this);
		m_pPictureCtrlPic->Create(CPictureCtrlPic::IDD, this);
	}
	if(m_pPictureCtrlClose == NULL)
	{
		m_pPictureCtrlClose = new CPictureCtrlClose(this);
		m_pPictureCtrlClose->Create(CPictureCtrlClose::IDD, this);
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPictureCtrl::SetPos()
{
	CRect rc;
	GetWindowRect(&rc);

	CImage image; //ʹ��ͼƬ��
	image.Load( m_strFilePath );

	if(image.GetWidth() > rc.Width())
	{
		rc.right = rc.left + image.GetWidth();
	}
	if(image.GetHeight() > rc.Height())
	{
		rc.bottom = rc.top + image.GetHeight();
	}
	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW); 
	CenterWindow();
	GetWindowRect(&rc);
	CRect rcPic;
	rcPic.left = rc.left;
	rcPic.top = rc.top;
	rcPic.right = rc.left + image.GetWidth();
	rcPic.bottom = rc.top + image.GetHeight();
	int nWidth = image.GetWidth();
	int nHight = image.GetHeight();
	rcPic.left = rc.left + (rc.Width() - nWidth) / 2;
	rcPic.right = rc.right - (rc.Width() - nWidth) / 2;
	rcPic.top = rc.top + (rc.Height() - nHight) / 2;
	rcPic.bottom = rc.bottom - (rc.Height() - nHight) / 2;
	m_pPictureCtrlPic->SetFile(m_strFilePath);
	m_pPictureCtrlPic->SetPos(rcPic);
	m_pPictureCtrlClose->m_bIsHover = FALSE;
	m_pPictureCtrlClose->SetPos(rc);
	CRect rcClose;
	m_pPictureCtrlClose->GetWindowRect(&rcClose);
	m_pPictureCtrlClose->SetWindowPos(NULL, rcClose.left, rcClose.top, rcClose.Width(), rcClose.Height(), SWP_SHOWWINDOW); 
}

HBRUSH CPictureCtrl::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	// TODO:  �ڴ˸��� DC ���κ�����
	pDC->SetBkMode(TRANSPARENT);
	return   m_brush;
	// TODO:  ���Ĭ�ϵĲ������軭�ʣ��򷵻���һ������
	return hbr;
}

void CPictureCtrl::SetFile(CString& strFilePath)
{
	m_strFilePath = strFilePath;
}

BOOL CPictureCtrl::ShowJpgGif(CDC* pDC,CString strPath, int x, int y)  

{  
	CFileStatus fstatus;    
	CFile file;    
	ULONGLONG cb;    

	// ���ļ�������ļ�����Ч��  
	if (!file.Open(strPath,CFile::modeRead))  
	{  
		return FALSE;  
	}  
	if (!file.GetStatus(strPath,fstatus))  
	{  
		return FALSE;  
	}  
	if ((cb =fstatus.m_size)<=0)  
	{  
		return FALSE;  
	}  

	// �����ļ���С�����ڴ�ռ�,�ǵ��ͷ��ڴ�  
	HGLOBAL hGlobal = GlobalAlloc(GMEM_MOVEABLE, (unsigned int)cb);    
	if (hGlobal== NULL)   
	{  
		return FALSE;  
	}  

	// �����ղŷ�����ڴ�ռ�  
	LPVOID pvData = NULL;    
	pvData = GlobalLock(hGlobal);  
	if (pvData == NULL)    
	{    
		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		return FALSE;  
	}   

	// ���ļ��ŵ�����  
	IStream *pStm;    
	file.Read(pvData,(unsigned int)cb);    
	GlobalUnlock(hGlobal);    
	CreateStreamOnHGlobal(hGlobal, TRUE, &pStm);    

	// �����м���ͼƬ  
	// ��ʾJPEG��GIF��ʽ��ͼƬ��GIFֻ����ʾһ֡����������ʾ������  
	// Ҫ��ʾ����GIF��ʹ��ACTIVE�ؼ���  
	IPicture *pPic;   
	if(OleLoadPicture(pStm,(LONG)fstatus.m_size,TRUE,IID_IPicture,(LPVOID*)&pPic)!=S_OK)   
	{   
		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		return FALSE;  
	}  

	//��ȡͼ���͸�,ע������Ŀ�͸߲���ͼ��ķֱ���  
	OLE_XSIZE_HIMETRIC hmWidth;    
	OLE_YSIZE_HIMETRIC hmHeight;    
	pPic->get_Width(&hmWidth);    
	pPic->get_Height(&hmHeight);    

	// ��ͼ���Ⱥ͸߶ȵ�λת��Ϊ���ص�λ  
	//#define HIMETRIC_PER_INCH 2540  
	//int nPicWidth =  MulDiv(hmWidth, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSX),2540);  
	//int nPicHeight = MulDiv(hmHeight, GetDeviceCaps(GetDC()->m_hDC, LOGPIXELSY),2540);  

	//  
	// HRESULT Render(  
	//     HDC hdc, //Handle of device context on which to render the image  
	//     long x,  //Horizontal position of image in hdc  
	//     long y,  //Vertical position of image in hdc  
	//     long cx, //Horizontal dimension of destination rectangle  
	//     long cy, //Vertical dimension of destination rectangle  
	//     OLE_XPOS_HIMETRIC xSrc,      //Horizontal offset in source picture  
	//     OLE_YPOS_HIMETRIC ySrc,      //Vertical offset in source picture  
	//     OLE_XSIZE_HIMETRIC cxSrc,    //Amount to copy horizontally in source picture  
	//     OLE_YSIZE_HIMETRIC cySrc,    //Amount to copy vertically in source picture  
	//     LPCRECT prcWBounds           //Pointer to position of destination for a metafile hdc  
	//     );  

	// //use render function display image  
	RECT rtWnd;  
	pDC->GetWindow()->GetWindowRect(&rtWnd);  
	int iWndWidth=rtWnd.right-rtWnd.left;  
	int iWndHeight=rtWnd.bottom-rtWnd.top;  

	//    RECT rect = { 0 };
	//    rect.bottom = image.GetWidth();
	//    rect.right = image.GetHeight();

	if(FAILED(pPic->Render(*pDC,x,y,iWndWidth,iWndHeight,0,hmHeight,hmWidth,-hmHeight,NULL)))    
	{  
		pPic->Release();  
		GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
		return false;  
	}  

	pPic->Release();   
	GlobalFree(hGlobal);  // �ǵ��ͷ��ڴ�  
	return true;  
}  

void CPictureCtrl::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	CRect rect;
	m_pPictureCtrlPic->GetWindowRect(&rect);
	dc.FillSolidRect(rect.left, rect.top, rect.Width(), rect.Height(), RGB(77,77,77));
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
}

BOOL CPictureCtrl::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_moveX = 0;
	m_moveY = 0;
	if(zDelta > 0)
	{
		m_pPictureCtrlPic->SetNewPos(TRUE);
	}
	else if(zDelta < 0)
	{
		m_pPictureCtrlPic->SetNewPos(FALSE);
	}
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}


void CPictureCtrl::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	if(m_rcClose.PtInRect(ptCursor))
		CloseAll();

	CRect rc;
	GetWindowRect(&rc);
	if(rc.PtInRect(ptCursor))
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND

	if(m_pPictureCtrlPic->m_rect.Width() <= rc.Width() && m_pPictureCtrlPic->m_rect.Height() <= rc.Height())
	{
		m_bIsBig = FALSE;
		PostMessage(WM_NCLBUTTONDOWN,   HTCAPTION,   MAKELPARAM(point.x,   point.y));
	}
	else
	{
		m_bIsBig = TRUE;
		m_downX = ptCursor.x;
		m_downY = ptCursor.y;
		m_rcPic = m_pPictureCtrlPic->m_rect;
	}

	::SetWindowPos( m_pPictureCtrlClose->GetSafeHwnd(), HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION );
	::SetWindowPos( m_pPictureCtrlClose->GetSafeHwnd(), HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOREPOSITION );
	SetFocus();
	CDialog::OnLButtonDown(nFlags, point);
}


void CPictureCtrl::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	SendMessage(WM_CLOSE);
	m_pPictureCtrlPic->SendMessage(WM_CLOSE);
	m_pPictureCtrlClose->SendMessage(WM_CLOSE);
	CDialog::OnLButtonDblClk(nFlags, point);
}

void CPictureCtrl::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint ptCursor;
	GetCursorPos(&ptCursor);

	CRect rc;
	GetWindowRect(&rc);
	if(m_rcClose.PtInRect(ptCursor))
	{
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
		m_pPictureCtrlClose->DrawClose(TRUE);
	}
	else
	{
		::SetCursor(::LoadCursor(NULL,MAKEINTRESOURCE(32649)));//IDC HAND
		m_pPictureCtrlClose->DrawClose(FALSE);
	}

	if(m_bIsBig)
	{
		CRect rcParent;
		GetWindowRect(&rcParent);

		m_pPictureCtrlPic->m_rect.left = m_pPictureCtrlPic->m_rect.left + m_moveX;
		m_pPictureCtrlPic->m_rect.top = m_pPictureCtrlPic->m_rect.top + m_moveY;
		m_pPictureCtrlPic->m_rect.right = m_pPictureCtrlPic->m_rect.right + m_moveX;
		m_pPictureCtrlPic->m_rect.bottom = m_pPictureCtrlPic->m_rect.bottom + m_moveY;

		m_moveX = m_downX - ptCursor.x;
		m_moveY = m_downY - ptCursor.y;

		m_pPictureCtrlPic->m_rect.left = m_pPictureCtrlPic->m_rect.left - m_moveX;
		m_pPictureCtrlPic->m_rect.top = m_pPictureCtrlPic->m_rect.top - m_moveY;
		m_pPictureCtrlPic->m_rect.right = m_pPictureCtrlPic->m_rect.right - m_moveX;
		m_pPictureCtrlPic->m_rect.bottom = m_pPictureCtrlPic->m_rect.bottom - m_moveY;

		m_pPictureCtrlPic->Invalidate();

		CRect rcTrue;
		rcTrue = rcParent;
		rcTrue.left = m_pPictureCtrlPic->m_rect.left > rcParent.left ? m_pPictureCtrlPic->m_rect.left : rcParent.left;
		rcTrue.top = m_pPictureCtrlPic->m_rect.top > rcParent.top ? m_pPictureCtrlPic->m_rect.top : rcParent.top;
		rcTrue.right = m_pPictureCtrlPic->m_rect.right < rcParent.right ? m_pPictureCtrlPic->m_rect.right : rcParent.right;
		rcTrue.bottom = m_pPictureCtrlPic->m_rect.bottom < rcParent.bottom ? m_pPictureCtrlPic->m_rect.bottom : rcParent.bottom;
		m_pPictureCtrlPic->MoveWindow(rcTrue);
	}

	CDialog::OnMouseMove(nFlags, point);
}


void CPictureCtrl::OnMove(int x, int y)
{
	if(m_bIsBig)
		return;
	CRect rcParent;
	GetWindowRect(&rcParent);
	CDialog::OnMove(x, y);
	if(m_x == -1 || m_y == -1)
	{
		m_x = x;
		m_y = y;
		return;
	}
	if(m_x == x && m_y == y)
		return;
	if(m_pPictureCtrlPic != NULL)
	{
		m_pPictureCtrlPic->m_rect.left = m_pPictureCtrlPic->m_rect.left - m_x + x;
		m_pPictureCtrlPic->m_rect.top = m_pPictureCtrlPic->m_rect.top - m_y + y;
		m_pPictureCtrlPic->m_rect.right = m_pPictureCtrlPic->m_rect.right - m_x + x;
		m_pPictureCtrlPic->m_rect.bottom = m_pPictureCtrlPic->m_rect.bottom - m_y + y;

		if(rcParent.Width() < m_pPictureCtrlPic->m_rect.Height() || rcParent.Height() < m_pPictureCtrlPic->m_rect.Height())
		{
			m_pPictureCtrlPic->MoveWindow(rcParent);
		}
		else
			m_pPictureCtrlPic->MoveWindow(m_pPictureCtrlPic->m_rect);

		m_pPictureCtrlPic->Invalidate();
	}
	if(m_pPictureCtrlClose != NULL)
	{
		CRect rcChild;
		m_pPictureCtrlClose->GetWindowRect(&rcChild);
		rcChild.left = rcChild.left - m_x + x;
		rcChild.top = rcChild.top - m_y + y;
		rcChild.right = rcChild.right - m_x + x;
		rcChild.bottom = rcChild.bottom - m_y + y;
		m_pPictureCtrlClose->MoveWindow(rcChild.left, rcChild.top, rcChild.Width(), rcChild.Height());
		SetCloseRect(rcChild);
		//m_pPictureCtrlClose->Invalidate();
	}
	m_x = x;
	m_y = y;
}

void CPictureCtrl::CloseAll()
{
	SendMessage(WM_CLOSE);
	m_pPictureCtrlPic->SendMessage(WM_CLOSE);
	m_pPictureCtrlClose->SendMessage(WM_CLOSE);
}

void CPictureCtrl::SetCloseRect(CRect rc)
{
	m_rcClose.SetRect(rc.left, rc.top, rc.right, rc.bottom);
}

void CPictureCtrl::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_bIsBig = FALSE;
	m_downX = -1;
	m_downX = -1;
	m_moveX = 0;
	m_moveY = 0;
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	if(m_rcClose.PtInRect(ptCursor))
		CloseAll();
	CDialog::OnLButtonUp(nFlags, point);
}

void CPictureCtrl::OnTimer(UINT_PTR nIDEvent)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	CPoint ptCursor;
	GetCursorPos(&ptCursor);
	CRect rc;
	GetWindowRect(rc);
	if(ptCursor.x > rc.right || ptCursor.x < rc.left || ptCursor.y > rc.bottom || ptCursor.y < rc.top)
	{
		m_downX = -1;
		m_downY = -1;
		m_moveX = 0;
		m_moveY = 0;
		m_bIsBig = FALSE;
	}
	CDialog::OnTimer(nIDEvent);
}


BOOL CPictureCtrl::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if  (  pMsg->message  ==  WM_KEYDOWN  )  
	{  
		switch(  pMsg->wParam  )  
		{  
		case VK_RETURN:
			return TRUE;
		case VK_ESCAPE:
			CloseAll();
		default:
			;
		}  
	}
	return CDialog::PreTranslateMessage(pMsg);
}
