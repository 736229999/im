// DlgSCreenCtrlColor.cpp : ʵ���ļ�
//

#include "stdafx.h"
#include "DlgSCreenCtrlColor.h"
#include "afxdialogex.h"
#include "SCreenCtrl.h"
// CDlgSCreenCtrlColor �Ի���

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


// CDlgSCreenCtrlColor ��Ϣ�������


BOOL CDlgSCreenCtrlColor::OnInitDialog()
{
	CDialog::OnInitDialog();

	// TODO:  �ڴ���Ӷ���ĳ�ʼ��

	m_btnSmall.SetPng(IDB_PNG_SCREEN_SHAPE_SMALL_HOVER,IDB_PNG_SCREEN_SHAPE_SMALL_NORMAL,IDB_PNG_SCREEN_SHAPE_SMALL_PRESSED,IDB_PNG_SCREEN_SHAPE_SMALL_NORMAL);
	m_btnNormal.SetPng(IDB_PNG_SCREEN_SHAPE_NORMAL_HOVER,IDB_PNG_SCREEN_SHAPE_NORMAL_NORMAL,IDB_PNG_SCREEN_SHAPE_NORMAL_PRESSED,IDB_PNG_SCREEN_SHAPE_NORMAL_NORMAL);
	m_btnBig.SetPng(IDB_PNG_SCREEN_SHAPE_BIG_HOVER,IDB_PNG_SCREEN_SHAPE_BIG_NORMAL,IDB_PNG_SCREEN_SHAPE_BIG_PRESSED,IDB_PNG_SCREEN_SHAPE_BIG_NORMAL);

	m_btnSmall.SetDown(TRUE);//���Ʊ߿�Ŀ��Ĭ��ΪС

	//�洢picture�ؼ�ID�����ڻ�����ɫ
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

	//�洢��ɫ
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

	//���ѡ��״̬����Ϊ��ѡ��
	for(int nPic = 0; nPic < 16; nPic++)
	{
		m_bIsInRect[nPic] = FALSE;
		m_bIsInRectOld[nPic] = FALSE;
	}
	return TRUE;  // return TRUE unless you set the focus to a control
	// �쳣: OCX ����ҳӦ���� FALSE
}


void CDlgSCreenCtrlColor::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	BOOL bIsInPicRect = FALSE;
	for(int nPic = 0; nPic < 16; nPic++)
	{
		UINT uPicId = m_vecPicId[nPic];
		CRect rc;
		GetDlgItem(uPicId)->GetWindowRect(&rc);
		ScreenToClient(&rc);

		//�����һ�������Ҳ���֮ǰ������������������ڵ���ɫ
		//ѡ��������Ʒ�ʽΪ
		//�߿��һ���ɫ,�߿�ڶ����ɫ,�ڲ�ΪԤ�����õ���ɫ
		if(rc.PtInRect(point))
		{
			bIsInPicRect = TRUE;
			m_bIsInRect[nPic] = TRUE;
			if(m_bIsInRect[nPic] != m_bIsInRectOld[nPic])
			{
				CRect rc;
				GetDlgItem(uPicId)->GetClientRect(&rc);
				CDC* pDC = GetDlgItem(uPicId)->GetDC();
				CPen Pen1(PS_SOLID, 2, RGB(255,255,255));//��ɫ�߿����ƣ����ڻ�ɫ�߿򸲸�
				pDC->SelectObject(&Pen1);   
				pDC->Rectangle(&rc); 
				Pen1.DeleteObject();
				CPen Pen2(PS_SOLID, 1, RGB(128,128,128));//��ɫ�߿�
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

				m_bIsInRectOld[nPic] = TRUE;//������һ��ѡ�е����������ж��Ƿ�ˢ�£���ֹ����
				SetRect(nPic, FALSE);//�����ؼ�������Ϊδѡ��
			}
		}
	}
	//���δѡ���κ�һ�������������һ��ѡ�е������Ϊδѡ�е���ɫ
	//δѡ�еĻ��Ʒ�ʽΪ
	//�߿�Ϊ��ɫ���ڲ�ΪԤ�����õ���ɫ
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
	//��һ��ѡ�е��������Ϊδѡ�е���ɫ
	//δѡ�еĻ��Ʒ�ʽΪ
	//�߿�Ϊ��ɫ���ڲ�ΪԤ�����õ���ɫ
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
	//���ó���ѡ����������������ȫ����Ϊδѡ��״̬
	for(int nPic = 0; nPic < 16; nPic++)
	{
		if(nPic != nPicRect)
			m_bIsInRect[nPic] = bIn;
	}
}

void CDlgSCreenCtrlColor::DrawAllRectColor()
{
	//�������е�����
	//���Ʒ�ʽΪ
	//�߿�Ϊ��ɫ���ڲ�ΪԤ�����õ���ɫ
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
	//����ѡ����ɫ������
	//���Ʒ�ʽΪ
	//�߿��һ���ɫ,�߿�ڶ����ɫ,�ڲ�Ϊѡ�е���ɫ
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
	//���Ʊ߿��Ⱥ���ɫ֮�������
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
	// TODO: �ڴ˴������Ϣ����������
	// ��Ϊ��ͼ��Ϣ���� CDialog::OnPaint()
	DrawGrayLine();//�߿��Ⱥ���ɫ֮�������
	DrawAllRectColor();//�����������ɫ
	DrawRectColor(m_vecColor[m_nColor]);//��ʾѡ����ɫ������
}



void CDlgSCreenCtrlColor::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: �ڴ������Ϣ�����������/�����Ĭ��ֵ
	//��갴��ѡ�е���ɫ����ı������ɫ
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
			m_pDlgSCreenCtrl->SetDrawColor(m_vecColor[m_nColor]);//�༭������ɫ
			break;
		}
	}
	CDialog::OnLButtonDown(nFlags, point);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnSmall()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�߿�������ΪС
	m_nSize = 1;
	m_btnSmall.SetDown(TRUE);
	m_btnNormal.SetDown(FALSE);
	m_btnBig.SetDown(FALSE);
	m_pDlgSCreenCtrl->SetDrawSize(1);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnNormal()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�߿�������Ϊ��
	m_nSize = 2;
	m_btnSmall.SetDown(FALSE);
	m_btnNormal.SetDown(TRUE);
	m_btnBig.SetDown(FALSE);
	m_pDlgSCreenCtrl->SetDrawSize(2);
}


void CDlgSCreenCtrlColor::OnBnClickedBtnBig()
{
	// TODO: �ڴ���ӿؼ�֪ͨ����������
	//�߿�������Ϊ��
	m_nSize = 3;
	m_btnSmall.SetDown(FALSE);
	m_btnNormal.SetDown(FALSE);
	m_btnBig.SetDown(TRUE);
	m_pDlgSCreenCtrl->SetDrawSize(3);
}


BOOL CDlgSCreenCtrlColor::PreTranslateMessage(MSG* pMsg)
{
	// TODO: �ڴ����ר�ô����/����û���
	if (pMsg->message == WM_KEYDOWN)
	{
		switch (pMsg->wParam)
		{
		case VK_ESCAPE://ESC����Ӧ�����ڵĹرձ༭���رմ���
			GetParent()->SendMessage( WM_COMMAND, IDC_BTN_CLOSE);
			SendMessage(WM_CLOSE);
			break;
		case VK_RETURN://�س�����Ӧ�����ڵ���ɱ༭���رմ���
			GetParent()->SendMessage(WM_COMMAND, IDC_BTN_DONE);
			SendMessage(WM_CLOSE);
			break;
		}
	}
	return CDialog::PreTranslateMessage(pMsg);
}
