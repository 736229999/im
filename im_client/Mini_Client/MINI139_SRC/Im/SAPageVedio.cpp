// im\SAPageVedio.cpp : 实现文件
//

#include "stdafx.h"
#include "..\IMClient.h"
#include ".\SAPageVedio.h"


#include "LocalData.h"


IMPLEMENT_DYNAMIC(CSAPageVedio, CSABasePage)
CSAPageVedio::CSAPageVedio(CWnd* pParent /*=NULL*/)
: CSABasePage(CSAPageVedio::IDD, pParent)
{
	m_bInitVedioList = false;
	Create(CSAPageVedio::IDD,pParent);
}

CSAPageVedio::~CSAPageVedio()
{
}

void CSAPageVedio::DoDataExchange(CDataExchange* pDX)
{
	CSABasePage::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_COMBO_VEDIO, m_comboVedioList);
}


BEGIN_MESSAGE_MAP(CSAPageVedio, CSABasePage)
	ON_WM_SHOWWINDOW()
	ON_CBN_SELCHANGE(IDC_COMBO_VEDIO, OnCbnSelchangeComboVedio)
END_MESSAGE_MAP()


// CSAPageVedio 消息处理程序

BOOL CSAPageVedio::OnInitDialog()
{
	CSABasePage::OnInitDialog();
	return TRUE; 
}


int CSAPageVedio::SaveVedioSetting()
{
	int nSel;
	int nDefaultCameraIndex ;

	nSel = m_comboVedioList.GetCurSel();

	if ( nSel!=-1 )
	{
		nDefaultCameraIndex = m_comboVedioList.GetItemData(nSel);
		g_LocalData.SetDefaultCameraIndex(nDefaultCameraIndex);
	}
	

	return 1;
}

void CSAPageVedio::OnShowWindow(BOOL bShow, UINT nStatus)
{
	CSABasePage::OnShowWindow(bShow, nStatus);

	if ( bShow )
	{
		if ( m_bInitVedioList==false )
		{
			map<int,CString>::iterator it;

			int nDefaultCameraIndex = g_LocalData.GetDefaultCameraIndex();
			int nSel = -1;

			for ( it=g_mapVideoDriverIndex.begin(); it!=g_mapVideoDriverIndex.end(); it++ )
			{
				int nItem = m_comboVedioList.AddString(it->second);
				m_comboVedioList.SetItemData(nItem,it->first);

				if ( it->first == nDefaultCameraIndex )
				{
					nSel = nItem;
				}
			}

			if ( nSel!=-1 )
			{
				m_comboVedioList.SetCurSel(nSel);
			}

			OnCbnSelchangeComboVedio();
			m_bInitVedioList = true;
		}
	}
}

void CSAPageVedio::OnCbnSelchangeComboVedio()
{
	int nSel = m_comboVedioList.GetCurSel();

	m_videoCap.Close();

	if ( nSel!=-1 )
	{
		CWnd *pCapWnd = GetDlgItem(IDC_STATIC_VEDIO);
		CRect rcCapWnd;
		pCapWnd->GetClientRect(&rcCapWnd);
		rcCapWnd.InflateRect(-1,-1);
		m_videoCap.InitCap(rcCapWnd,pCapWnd->m_hWnd,m_comboVedioList.GetItemData(nSel));
		m_videoCap.Start(3);
	}
}
