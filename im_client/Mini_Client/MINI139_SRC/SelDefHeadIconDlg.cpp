// SelDefHeadIconDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "SelDefHeadIconDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "buttonext.h"
#include "RoundFaceCtrl.h"
// CSelDefHeadIconDlg 对话框

using namespace std;

// ID
#define c_idBgn 40001				// 图标控件起始ID
#define c_idEnd 41000				// 图标控件终止ID

IMPLEMENT_DYNAMIC(CSelDefHeadIconDlg, CDialog)

CSelDefHeadIconDlg::CSelDefHeadIconDlg(const std::vector<std::string> &vecHeadPath)
	: CDialog(IDD_DLG_SEL_DEF_HEAD, NULL)
{
	m_hBkGround = ::CreateSolidBrush(RGB(255, 255, 255));
	m_vecHeadPath = vecHeadPath;
}

CSelDefHeadIconDlg::~CSelDefHeadIconDlg()
{
	for(int i = 0; i < m_vecpFaceCtrl.size(); ++i)
	{
		if(m_vecpFaceCtrl[i] != NULL)
		{
			if(::IsWindow(m_vecpFaceCtrl[i]->GetSafeHwnd()))
			{
				m_vecpFaceCtrl[i]->DestroyWindow();
			}
			delete m_vecpFaceCtrl[i];
			m_vecpFaceCtrl[i] = NULL;
		}
	}
	m_vecpFaceCtrl.clear();
}

void CSelDefHeadIconDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BOOL CSelDefHeadIconDlg::Create(CWnd *pParent)
{
	return CDialog::Create(IDD_DLG_SEL_DEF_HEAD, pParent);
}

BEGIN_MESSAGE_MAP(CSelDefHeadIconDlg, CDialog)
	ON_WM_VSCROLL()
	ON_WM_ERASEBKGND()
	ON_WM_CTLCOLOR()
	ON_CONTROL_RANGE(BN_CLICKED, c_idBgn, c_idEnd, OnBnClickedHeadIcon)
END_MESSAGE_MAP()

BOOL CSelDefHeadIconDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	GetDlgItem(IDC_STATIC_SEL)->SetFont(&g_font12);

	// 创建图标控件
	CPoint ptBgn(10, 40);
	CRect rc;
	UINT idBgn = c_idBgn;				// 图标控件起始ID
	for(int i = 0; i < m_vecHeadPath.size(); ++i)
	{
		string strHeadPath = m_vecHeadPath[i];

		CRountFaceCtrl *pFaceCtrl = new CRountFaceCtrl(IDB_PNG_ROUND_NORMAL, IDB_PNG_ROUND_SEL, strHeadPath.c_str());
		pFaceCtrl->Create(this, idBgn++);
		CRect rcCtrl;
		pFaceCtrl->GetWindowRect(&rcCtrl);

		int nSpace = rcCtrl.Width() + 10;				// 图标之间的间距（左上角与左上角之间的距离）
		int nCountPreRow = 5;							// 每行图标个数		

		// 当前图标所处的行数
		int nRowNo = i / nCountPreRow;
		// 当前图标处于的列数
		int nColNo = i % nCountPreRow;

		// 计算图标的坐标
		rc.left = ptBgn.x + nColNo * nSpace;
		rc.top = ptBgn.y + nRowNo * nSpace;
		
		rc.right = rc.left + rcCtrl.Width();
		rc.bottom = rc.top + rcCtrl.Height();
		pFaceCtrl->MoveWindow(&rc);
		pFaceCtrl->ShowWindow(SW_SHOW);

		m_vecpFaceCtrl.push_back(pFaceCtrl);
	}

	// 添加滚动条
	SetScrollRange(SB_VERT, 0, 100);

	ShowScrollBar(SB_VERT, FALSE);

	return TRUE;
}

// CSelDefHeadIconDlg 消息处理程序
void CSelDefHeadIconDlg::OnVScroll(UINT nSBCode, UINT nPos, CScrollBar* pScrollBar) 
{
	CScrollBar *pBar = GetScrollBarCtrl(SB_VERT);

	SCROLLINFO scrollinfo;

	GetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
	switch (nSBCode)  
	{  
	case SB_BOTTOM:  
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMax)*10);  
		scrollinfo.nPos = scrollinfo.nMax;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case SB_TOP:  
		ScrollWindow(0,(scrollinfo.nPos-scrollinfo.nMin)*10);  
		scrollinfo.nPos = scrollinfo.nMin;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case SB_LINEUP:  
		scrollinfo.nPos -= 1;  
		if (scrollinfo.nPos)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,10);  
		break;  
	case SB_LINEDOWN:  
		scrollinfo.nPos += 1;  
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10);  
		break;  
	case SB_PAGEUP:  
		scrollinfo.nPos -= 5;  
		if (scrollinfo.nPos)
		{  
			scrollinfo.nPos = scrollinfo.nMin;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,10*5);  
		break;  
	case SB_PAGEDOWN:  
		scrollinfo.nPos += 5;  
		if (scrollinfo.nPos>scrollinfo.nMax)  
		{  
			scrollinfo.nPos = scrollinfo.nMax;  
			break;  
		}  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		ScrollWindow(0,-10*5);  
		break;  
	case SB_ENDSCROLL:  
		// MessageBox("SB_ENDSCROLL");  
		break;  
	case SB_THUMBPOSITION:  
		// ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		// scrollinfo.nPos = nPos;  
		// SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	case SB_THUMBTRACK:  
		ScrollWindow(0,(scrollinfo.nPos-nPos)*10);  
		scrollinfo.nPos = nPos;  
		SetScrollInfo(SB_VERT,&scrollinfo,SIF_ALL);  
		break;  
	}

	CDialog::OnVScroll(nSBCode, nPos, pScrollBar);
}

BOOL CSelDefHeadIconDlg::OnEraseBkgnd(CDC* pDC)
{
	CDialog::OnEraseBkgnd(pDC);

	// 背景刷为白色
	CRect rcDlg;
	GetClientRect(&rcDlg);
	pDC->FillSolidRect(&rcDlg, RGB(255, 255, 255));

	return FALSE;
}

void CSelDefHeadIconDlg::OnPaint()
{
	CDialog::OnPaint();

	CPaintDC dc(this);

}

HBRUSH CSelDefHeadIconDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
{
	HBRUSH hbr = CDialog::OnCtlColor(pDC, pWnd, nCtlColor);

	if ( nCtlColor==CTLCOLOR_STATIC )
	{
		pDC->SetBkMode(TRANSPARENT);
		hbr = m_hBkGround;
	}

	return hbr;
}

// 图标控件被点击的消息相应
void CSelDefHeadIconDlg::OnBnClickedHeadIcon(UINT uid)
{
	int nIndex = uid - c_idBgn;
	if(nIndex >= 0 && nIndex < m_vecHeadPath.size())
	{
		m_strHeadPath_CurSel = m_vecHeadPath[nIndex];
		::SendMessage(GetParent()->GetSafeHwnd(), WM_SELDEFHEADDLG_MSG, en_selhead_msg_type::en_selchange, 0);
	}
}