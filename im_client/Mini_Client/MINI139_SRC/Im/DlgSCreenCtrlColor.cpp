// DlgSCreenCtrlColor.cpp : 实现文件
//

#include "stdafx.h"
#include "DlgSCreenCtrlColor.h"
#include "afxdialogex.h"
#include "SCreenCtrl.h"
// CDlgSCreenCtrlColor 对话框

IMPLEMENT_DYNAMIC(CDlgSCreenCtrlColor, CDialog)

CDlgSCreenCtrlColor::CDlgSCreenCtrlColor(CWnd* pParent /*=NULL*/, CScreenCtrl* pDlgSCreenCtrl)
	: CDialog(CDlgSCreenCtrlColor::IDD, pParent)
{
	m_pDlgSCreenCtrl = pDlgSCreenCtrl;
	m_nColor = 10;
	m_nSize = 1;
	memset(m_vecPicId, 0, sizeof(m_vecPicId));
	memset(m_vecColor, 0, sizeof(m_vecColor));
	memset(m_bIsInRect, 0, sizeof(m_bIsInRect));
	memset(m_bIsInRectOld, 0, sizeof(m_bIsInRectOld));

}

CDlgSCreenCtrlColor::~CDlgSCreenCtrlColor()
{
}

void CDlgSCreenCtrlColor::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_STATIC_COLOR, m_staticColor);
	DDX_Control(pDX, IDC_BTN_SMALL, m_btnSmall);
	DDX_Control(pDX, IDC_BTN_NORMAL, m_btnNormal);
	DDX_Control(pDX, IDC_BTN_BIG, m_btnBig);
}


BEGIN_MESSAGE_MAP(CDlgSCreenCtrlColor, CDialog)
	ON_WM_MOUSEMOVE()
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_BN_CLICKED(IDC_BTN_SMALL, &CDlgSCreenCtrlColor::OnBnClickedBtnSmall)
	ON_BN_CLICKED(IDC_BTN_NORMAL, &CDlgSCreenCtrlColor::OnBnClickedBtnNormal)
	ON_BN_CLICKED(IDC_BTN_BIG, &CDlgSCreenCtrlColor::OnBnClickedBtnBig)
END_MESSAGE_MAP()


// CDlgSCreenCtrlColor 消息处理程序


BOOL CDlgSCreenCtrlColor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  在此添加额外的初始化

	m_btnSmall.SetPng(IDB_PNG_SCREEN_SHAPE_SMALL_HOVER,IDB_PNG_SCREEN_SHAPE_SMALL_NORMAL,IDB_PNG_SCREEN_SHAPE_SMALL_PRESSED,IDB_PNG_SCREEN_SHAPE_SMALL_NORMAL);
	m_btnNormal.SetPng(IDB_PNG_SCREEN_SHAPE_NORMAL_HOVER,IDB_PNG_SCREEN_SHAPE_NORMAL_NORMAL,IDB_PNG_SCREEN_SHAPE_NORMAL_PRESSED,IDB_PNG_SCREEN_SHAPE_NORMAL_NORMAL);
	m_btnBig.SetPng(IDB_PNG_SCREEN_SHAPE_BIG_HOVER,IDB_PNG_SCREEN_SHAPE_BIG_NORMAL,IDB_PNG_SCREEN_SHAPE_BIG_PRESSED,IDB_PNG_SCREEN_SHAPE_BIG_NORMAL);

	m_btnSmall.SetDown(TRUE);//绘制边框的宽度默认为小

	//存储picture控件ID，用于绘制颜色
	m_vecPicId[0] = IDC_STATIC_1;
	m_vecPicId[1] = IDC_STATIC_2;
	m_vecPicId[2] = IDC_STATIC_3;
	m_vecPicId[3] = IDC_STATIC_4;
	m_vecPicId[4] = IDC_STATIC_5;
	m_vecPicId[5] = IDC_STATIC_6;
	m_vecPicId[6] = IDC_STATIC_7;
	m_vecPicId[7] = IDC_STATIC_8;
	m_vecPicId[8] = IDC_STATIC_9;
	m_vecPicId[9] = IDC_STATIC_10;
	m_vecPicId[10] = IDC_STATIC_11;
	m_vecPicId[11] = IDC_STATIC_12;
	m_vecPicId[12] = IDC_STATIC_13;
	m_vecPicId[13] = IDC_STATIC_14;
	m_vecPicId[14] = IDC_STATIC_15;
	m_vecPicId[15] = IDC_STATIC_16;

	//存储颜色
	m_vecColor[0] = RGB(0x00, 0x00, 0x00);
	m_vecColor[1] = RGB(0x81,0x83,0x83);
	m_vecColor[2] = RGB(0x80,0x00,0x00);
	m_vecColor[3] = RGB(0xF7,0x88,0x3A);
	m_vecColor[4] = RGB(0x30,0x84,0x30);
	m_vecColor[5] = RGB(0x38,0x5A,0xD3);
	m_vecColor[6] = RGB(0x80,0x00,0x80);
	m_vecColor[7] = RGB(0x00,0x99,0x99);
	m_vecColor[8] = RGB(0xFF,0xFF,0xFF);
	m_vecColor[9] = RGB(0xC0,0xC0,0xC0);
	m_vecColor[10] = RGB(0xFB,0x38,0x38);
	m_vecColor[11] = RGB(0xFF,0xFF,0x00);
	m_vecColor[12] = RGB(0x99,0xCC,0x00);
	m_vecColor[13] = RGB(0x38,0x94,0xE4);
	m_vecColor[14] = RGB(0xF3,0x1B,0xF3);
	m_vecColor[15] = RGB(0x16,0xDC,0xDC);

	//鼠标选中状态都设为非选中
	for(int nPic = 0; nPic < 16; nPic++)
	{
		m_bIsInRect[nPic] = FALSE;
		m_bIsInRectOld[nPic] = FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// 异常: OCX 属性页应返回 FALSE
}


void CDlgSCreenCtrlColor::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	BOOL bIsInPicRect = FALSE;
	for(int nPic = 0; nPic < 16; nPic++)
	{
		UINT uPicId = m_vecPicId[nPic];
		CRect rc;
		GetDlgItem(uPicId)->GetWindowRect(&rc);
		ScreenToClient(&rc);

		//如果在一个区域并且不再之前的区域里则绘制区域内的颜色
		//选中区域绘制方式为
		//边框第一层灰色,边框第二层白色,内部为预先设置的颜色
		if(rc.PtInRect(point))
		{
			bIsInPicRect = TRUE;
			m_bIsInRect[nPic] = TRUE;
			if(m_bIsInRect[nPic] != m_bIsInRectOld[nPic])
			{
				CRect rc;
				GetDlgItem(uPicId)->GetClientRect(&rc);
				CDC* pDC = GetDlgItem(uPicId)->GetDC();
				CPen Pen1(PS_SOLID, 2, RGB(255,255,255));//白色边框会绘制，便于灰色边框覆盖
				pDC->SelectObject(&Pen1);   
				pDC->Rectangle(&rc); 
				Pen1.DeleteObject();
				CPen Pen2(PS_SOLID, 1, RGB(128,128,128));//灰色边框
				pDC->SelectObject(&Pen2);   
				pDC->Rectangle(&rc); 
				Pen2.DeleteObject();
				rc.InflateRect(-2,-2);
				CPen Pen3(PS_SOLID, 1, m_vecColor[nPic]);  
				pDC->SelectObject(&Pen3);   
				CBrush br;
				br.CreateSolidBrush(m_vecColor[nPic]);
				pDC->SelectObject(&br);
				pDC->Rectangle(&rc);
				Pen3.DeleteObject();
				br.DeleteObject();
				ReleaseDC(pDC);
				pDC->DeleteDC();

				m_bIsInRectOld[nPic] = TRUE;//设置上一次选中的区域，用于判断是否刷新，防止闪屏
				SetRect(nPic, FALSE);//其他控件都设置为未选中
			}
		}
	}
	//如果未选中任何一个区域则绘制上一次选中的区域变为未选中的颜色
	//未选中的绘制方式为
	//边框为灰色，内部为预先设置的颜色
	if(!bIsInPicRect)
	{
		for(int nPic = 0; nPic < 16; nPic++)
		{
			UINT uPicId = m_vecPicId[nPic];
			m_bIsInRect[nPic] = FALSE;
			if(m_bIsInRect[nPic] != m_bIsInRectOld[nPic])
			{
				CRect rc;
				GetDlgItem(uPicId)->GetClientRect(&rc);
				CDC* pDC = GetDlgItem(uPicId)->GetDC();
				CPen pen(PS_SOLID, 1, RGB(128,128,128));  
				pDC->SelectObject(&pen);   
				pDC->Rectangle(&rc);  
				rc.InflateRect(-1,-1);
				CBrush br;
				br.CreateSolidBrush(m_vecColor[nPic]);
				pDC->SelectObject(&br);
				pDC->Rectangle(&rc);
				pen.DeleteObject();
				br.DeleteObject();
				ReleaseDC(pDC);
				pDC->DeleteDC();
				m_bIsInRectOld[nPic] = FALSE;
			}
		}
	}
	//上一次选中的区域绘制为未选中的颜色
	//未选中的绘制方式为
	//边框为灰色，内部为预先设置的颜色
	for(int nPic = 0; nPic < 16; nPic++)
	{
		UINT uPicId = m_vecPicId[nPic];
		CRect rc;
		GetDlgItem(uPicId)->GetClientRect(&rc);
		ScreenToClient(&rc);

		if(m_bIsInRect[nPic] != m_bIsInRectOld[nPic])
		{
			UINT uPicId = m_vecPicId[nPic];
			CRect rc;
			GetDlgItem(uPicId)->GetClientRect(&rc);
			CDC* pDC = GetDlgItem(uPicId)->GetDC();

			CPen pen(PS_SOLID, 1, RGB(128,128,128));  
			pDC->SelectObject(&pen);   
			pDC->Rectangle(&rc);  
			rc.InflateRect(-1,-1);
			CBrush br;
			br.CreateSolidBrush(m_vecColor[nPic]);
			pDC->SelectObject(&br);
			pDC->Rectangle(&rc);
			pen.DeleteObject();
			br.DeleteObject();
			ReleaseDC(pDC);
			pDC->DeleteDC();

			m_bIsInRectOld[nPic] = FALSE;
		}
	}
	CDialog::OnMouseMove(nFlags, point);
}

void CDlgSCreenCtrlColor::SetRect(int nPicRect, BOOL bIn)
{
	//设置除了选中区域，其他的区域全部变为未选中状态
	for(int nPic = 0; nPic < 16; nPic++)
	{
		if(nPic != nPicRect)
			m_bIsInRect[nPic] = bIn;
	}
}

void CDlgSCreenCtrlColor::DrawAllRectColor()
{
	//绘制所有的区域
	//绘制方式为
	//边框为灰色，内部为预先设置的颜色
	for(int nPic = 0; nPic < 16; nPic++)
	{
		UINT uPicId = m_vecPicId[nPic];
		CRect rc;
		GetDlgItem(uPicId)->GetClientRect(&rc);
		CDC* pDC = GetDlgItem(uPicId)->GetDC();

		CPen pen(PS_SOLID, 1, RGB(128,128,128));  
		pDC->SelectObject(&pen);   
		pDC->Rectangle(&rc);  
		rc.InflateRect(-1,-1);
		CBrush br;
		br.CreateSolidBrush(m_vecColor[nPic]);
		pDC->SelectObject(&br);
		pDC->Rectangle(&rc);
		pen.DeleteObject();
		br.DeleteObject();
		ReleaseDC(pDC);
		pDC->DeleteDC();
		m_bIsInRectOld[nPic] = FALSE;
	}
}

void CDlgSCreenCtrlColor::DrawRectColor(DWORD dwColor)
{
	//绘制选中颜色的区域
	//绘制方式为
	//边框第一层灰色,边框第二层白色,内部为选中的颜色
	CRect rc;
	m_staticColor.GetClientRect(&rc);
	CDC* pDC = m_staticColor.GetDC();
	CPen Pen1(PS_SOLID, 2, RGB(255,255,255));  
	pDC->SelectObject(&Pen1);   
	pDC->Rectangle(&rc); 
	Pen1.DeleteObject();
	CPen Pen2(PS_SOLID, 1, RGB(128,128,128));  
	pDC->SelectObject(&Pen2);   
	pDC->Rectangle(&rc); 
	Pen2.DeleteObject();
	rc.InflateRect(-2,-2);
	CPen Pen3(PS_SOLID, 1, dwColor);  
	pDC->SelectObject(&Pen3);   
	CBrush br;
	br.CreateSolidBrush(dwColor);
	pDC->SelectObject(&br);
	pDC->Rectangle(&rc);
	Pen3.DeleteObject();
	br.DeleteObject();
	ReleaseDC(pDC);
	pDC->DeleteDC();
}

void CDlgSCreenCtrlColor::DrawGrayLine()
{
	//绘制边框宽度和颜色之间的竖线
	CRect rcPic;
	m_staticColor.GetWindowRect(&rcPic);
	ScreenToClient(&rcPic);
	CPen pen(PS_SOLID, 1, RGB(128,128,128));
	CDC* pDC = GetDC();
	pDC->SelectObject(pen);
	pDC->MoveTo(rcPic.left - 7, rcPic.top);
	pDC->LineTo(rcPic.left - 7, rcPic.bottom);
	pen.DeleteObject();
	ReleaseDC(pDC);
	pDC->DeleteDC();
}
void CDlgSCreenCtrlColor::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	// TODO: 在此处添加消息处理程序代码
	// 不为绘图消息调用 CDialog::OnPaint()
	DrawGrayLine();//边框宽度和颜色之间的竖线
	DrawAllRectColor();//所有区域的颜色
	DrawRectColor(m_vecColor[m_nColor]);//显示选中颜色的区域
}



void CDlgSCreenCtrlColor::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//鼠标按下选中的颜色，则改变绘制颜色
	for(int nPic = 0; nPic < 16; nPic++)
	{
		UINT uPicId = m_vecPicId[nPic];
		CRect rc;
		GetDlgItem(uPicId)->GetWindowRect(&rc);
		ScreenToClient(&rc);

		if(rc.PtInRect(point))
		{
			m_nColor = nPic;
			DrawRectColor(m_vecColor[m_nColor]);
			m_pDlgSCreenCtrl->SetDrawColor(m_vecColor[m_nColor]);//编辑绘制颜色
			break;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnSmall()
{
	// TODO: 在此添加控件通知处理程序代码
	//边框宽度设置为小
	m_nSize = 1;
	m_btnSmall.SetDown(TRUE);
	m_btnNormal.SetDown(FALSE);
	m_btnBig.SetDown(FALSE);
	m_pDlgSCreenCtrl->SetDrawSize(1);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnNormal()
{
	// TODO: 在此添加控件通知处理程序代码
	//边框宽度设置为中
	m_nSize = 2;
	m_btnSmall.SetDown(FALSE);
	m_btnNormal.SetDown(TRUE);
	m_btnBig.SetDown(FALSE);
	m_pDlgSCreenCtrl->SetDrawSize(2);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnBig()
{
	// TODO: 在此添加控件通知处理程序代码
	//边框宽度设置为大
	m_nSize = 3;
	m_btnSmall.SetDown(FALSE);
	m_btnNormal.SetDown(FALSE);
	m_btnBig.SetDown(TRUE);
	m_pDlgSCreenCtrl->SetDrawSize(3);
}


BOOL CDlgSCreenCtrlColor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: 在此添加专用代码和/或调用基类
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC键响应父窗口的关闭编辑并关闭窗口
			GetParent()->SendMessage( WM_COMMAND, IDC_BTN_CLOSE);
			SendMessage(WM_CLOSE);
			break;
		case VK_RETURN://回车键响应父窗口的完成编辑并关闭窗口
			GetParent()->SendMessage(WM_COMMAND, IDC_BTN_DONE);
			SendMessage(WM_CLOSE);
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
