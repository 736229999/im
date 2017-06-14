// PictureCtrlPic.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "PictureCtrlPic.h"
#include "afxdialogex.h"
#include "PictureCtrl.h"
#include "PictureCtrlClose.h"
#include <cstring>
// CPictureCtrlPic �Ի���

IMPLEMENT_DYNAMIC(CPictureCtrlPic, CDialog)

	CPictureCtrlPic::CPictureCtrlPic(CWnd* pParent /*=NULL*/, CString strFilePath)
	: CDialog(CPictureCtrlPic::IDD, pParent)
{
	m_strFilePath = strFilePath;
	m_pPictureCtrl = (CPictureCtrl*)pParent;
	m_x = -1;
	m_y = -1;
	m_bIsMove = FALSE;

	AfxEnableControlContainer();
	//װ��GDI+
	GdiplusStartupInput m_gdiplusStartupInput;
	GdiplusStartup(&m_pGdiToken,&m_gdiplusStartupInput,NULL);

}

CPictureCtrlPic::~CPictureCtrlPic()
{
	//ж��GDI+
	GdiplusShutdown(m_pGdiToken);
}

void CPictureCtrlPic::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(CPictureCtrlPic, CDialog)
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDBLCLK()
	ON_WM_LBUTTONDOWN()
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_MOUSEWHEEL()
	ON_WM_LBUTTONUP()
END_MESSAGE_MAP()


// CPictureCtrlPic ��Ϣ�������


BOOL CPictureCtrlPic::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��
	//SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	//SetLayeredWindowAttributes(0,255,LWA_ALPHA);

	//SetWindowLong(GetSafeHwnd(),GWL_EXSTYLE,GetWindowLong(GetSafeHwnd(),GWL_EXSTYLE)|WS_EX_LAYERED);
	//SetLayeredWindowAttributes(0,255,LWA_ALPHA);

	CRect rc;
	GetWindowRect(&rc);

	m_pPictureCtrl->SetFocus();
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}

void CPictureCtrlPic::SetFile(CString& strFilePath)
{
	m_strFilePath = strFilePath;
}

BOOL CPictureCtrlPic::OnEraseBkgnd(CDC* pDC)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	pDC->SetBkColor(TRANSPARENT);
	return FALSE;
	return CDialog::OnEraseBkgnd(pDC);
}


void CPictureCtrlPic::OnLButtonDblClk(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pPictureCtrl->SendMessage(WM_LBUTTONDBLCLK);
	return;
	CDialog::OnLButtonDblClk(nFlags, point);
}


void CPictureCtrlPic::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//if(!m_pPictureCtrl->m_bIsBig)
	//{
	m_pPictureCtrl->SendMessage(WM_LBUTTONDOWN);
	m_pPictureCtrl->SetFocus();
	return;
	//}
	CDialog::OnLButtonDown(nFlags, point);
}

void CPictureCtrlPic::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pPictureCtrl->SendMessage(WM_MOUSEMOVE);
	return;
	CDialog::OnMouseMove(nFlags, point);
}

void CPictureCtrlPic::SetPos(CRect rc)
{
	CRect rect;
	rect.left = 0;
	rect.top = 0;
	rect.right = rect.left + rc.Width();
	rect.bottom = rect.top + rc.Height();
	SetWindowPos(NULL, rc.left, rc.top, rc.Width(), rc.Height(), SWP_SHOWWINDOW);
	CenterWindow();
	//Invalidate();
	//CenterWindow();
	//MoveWindow(rect); 
	ShowWindow(SW_SHOW);
	m_rect = rc;

	m_nAddWidth = rc.Width()*0.1;
	m_nAddHight = rc.Height()*0.1;

	m_nTop = rc.top;
	m_nBottom = rc.bottom;
	m_nLeft = rc.left;
	m_nRight = rc.right;
}

void CPictureCtrlPic::SetNewPos(BOOL bUp)
{
	CRect rc;
	if(bUp)
	{
		rc.left = m_rect.left = m_rect.left - m_nAddWidth;
		rc.right = m_rect.right = m_rect.right + m_nAddWidth;
		rc.top = m_rect.top = m_rect.top - m_nAddHight;
		rc.bottom = m_rect.bottom = m_rect.bottom + m_nAddHight;
	}
	else
	{
		rc.left = m_rect.left = m_rect.left + m_nAddWidth;
		rc.right = m_rect.right = m_rect.right - m_nAddWidth;
		rc.top = m_rect.top = m_rect.top + m_nAddHight;
		rc.bottom = m_rect.bottom = m_rect.bottom - m_nAddHight;
	}
	m_rect = rc;

	Invalidate();

	CRect rcChild;
	GetWindowRect(&rcChild);
	CRect rcParent;
	m_pPictureCtrl->GetWindowRect(&rcParent);
	if(rcParent.Width() < rc.Height() || rcParent.Height() < rc.Height())
	{
		CRect rcTrue;
		rcTrue = rcParent;
		rcTrue.left = m_rect.left > rcParent.left ? m_rect.left : rcParent.left;
		rcTrue.top = m_rect.top > rcParent.top ? m_rect.top : rcParent.top;
		rcTrue.right = m_rect.right < rcParent.right ? m_rect.right : rcParent.right;
		rcTrue.bottom = m_rect.bottom < rcParent.bottom ? m_rect.bottom : rcParent.bottom;
		MoveWindow(rcTrue);
	}
	else
	{
		MoveWindow(rc);
		CenterWindow(m_pPictureCtrl);
		CRect rcChild;
		GetWindowRect(&rcChild);
		m_rect = rcChild;
		Invalidate();
	}	
}

BOOL CPictureCtrlPic::ShowJpgGif(CDC* pDC,CString strPath, int x, int y)  
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

	int iWndWidth=m_rect.right-m_rect.left;  
	int iWndHeight=m_rect.bottom-m_rect.top;  

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

void CPictureCtrlPic::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	CRect rc;
	GetWindowRect(rc);
	int nX = m_rect.left - rc.left;
	int nY = m_rect.top - rc.top;
	//ole
	//if(m_strFilePath != "")
	//	ShowJpgGif(&dc, m_strFilePath, nX, nY);

	//gdi
	if(m_strFilePath != "")
	{
		CDC *pDC;
		pDC = GetDC();
		Graphics graphics( pDC->m_hDC);
		char *p=(LPSTR)(LPCSTR)m_strFilePath;

		WCHAR buf[1024];
		CStringW strWide = CT2CW(m_strFilePath); // ��T�ַ�ת���ɿ��ַ�
		wcscpy(buf, strWide); 

		Image image(buf, FALSE);

		graphics.DrawImage(&image, nX, nY, m_rect.Width(), m_rect.Height());  //����ͼƬ
	}

}


BOOL CPictureCtrlPic::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pPictureCtrl->SendMessage(WM_MOUSEWHEEL);
	return FALSE;
	return CDialog::OnMouseWheel(nFlags, zDelta, pt);
}

void CPictureCtrlPic::MoveNewPos(int x, int y)
{
	m_rect.left = m_rect.left + x;
	m_rect.top = m_rect.top + y;

	CRect rcParent;
	m_pPictureCtrl->GetWindowRect(&rcParent);
	if(rcParent.Width() < m_rect.Height() || rcParent.Height() < m_rect.Height())
	{
		MoveWindow(rcParent);
	}
	else
		MoveWindow(m_rect);
}

void CPictureCtrlPic::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	m_pPictureCtrl->m_bIsBig = FALSE;
	m_pPictureCtrl->SendMessage(WM_LBUTTONUP);
	m_pPictureCtrl->SetFocus();
	return;
	CDialog::OnLButtonUp(nFlags, point);
}
