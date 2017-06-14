#include "StdAfx.h"
#include "DuiGuidePage.h"

using namespace DuiLib;

#define GUIDEPAGE_POINT_HIGHTLIGHT "GuidePage\\icon_point_highligh.png" //引导页下方高亮点的图片
#define GUIDEPAGE_POINT_NORMAL "GuidePage\\icon_point_normal.png" //引导页下方普通点的图片

CDuiGuidePage::CDuiGuidePage(void)
{
	m_nIndexPage = 0;
}


CDuiGuidePage::~CDuiGuidePage(void)
{
}

DuiLib::UILIB_RESOURCETYPE CDuiGuidePage::GetResourceType() const
{
	return UILIB_ZIPRESOURCE;
}

LPCTSTR CDuiGuidePage::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CDuiGuidePage::Notify(DuiLib::TNotifyUI& msg) 
{
	if(msg.sType == _T("click"))
	{
		if(msg.pSender->GetName() == _T("btnClose")) //点击叉叉图标
		{
			Close();//关闭窗口
		}
		else if(msg.pSender->GetName() == _T("btnLeft")) //向左翻页
		{
			if(m_nIndexPage == 0)
			{
				return;
			}
			DuiLib::CVerticalLayoutUI* pLayout1 = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("Layout1")));
			m_nIndexPage--;
			DuiLib::CControlUI* pConPage = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("con1")));
			pConPage->SetBkImage(m_vecPage[m_nIndexPage]);

			CString strPage;
			for(int nPage = 0; nPage < m_vecPage.size(); nPage++)
			{
				strPage.Format("Page%d", nPage);
				DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, strPage));
				pBtnPage->SetBkImage(GUIDEPAGE_POINT_NORMAL);
			}
			strPage.Format("Page%d", m_nIndexPage);
			DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, strPage));
			pBtnPage->SetBkImage(GUIDEPAGE_POINT_HIGHTLIGHT);
		}
		else if(msg.pSender->GetName() == _T("btnRight")) //向右翻页
		{
			if(m_nIndexPage == m_vecPage.size()-1)
			{
				return;
			}
			DuiLib::CVerticalLayoutUI* pLayout1 = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("Layout1")));
			m_nIndexPage++;
			DuiLib::CControlUI* pConPage = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("con1")));
			pConPage->SetBkImage(m_vecPage[m_nIndexPage]);

			CString strPage;
			for(int nPage = 0; nPage < m_vecPage.size(); nPage++)
			{
				strPage.Format("Page%d", nPage);
				DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, strPage));
				pBtnPage->SetBkImage(_T(GUIDEPAGE_POINT_NORMAL));
			}
			strPage.Format("Page%d", m_nIndexPage);
			DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, strPage));
			pBtnPage->SetBkImage(GUIDEPAGE_POINT_HIGHTLIGHT);
		}

		//当前页面的点高亮显示，其他页面的点普通显示，选择点显示点对应的引导页
		for(int nPage = 0; nPage < m_vecPage.size(); nPage++)
		{
			DuiLib::CVerticalLayoutUI* pLayout1 = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("Layout1")));
			CDuiString str;
			str.Format("Page%d", nPage);
			if(msg.pSender->GetName() == str)
			{
				for(int nPageNormal = 0; nPageNormal < m_vecPage.size(); nPageNormal++)
				{
					CString strPage;
					strPage.Format("Page%d", nPageNormal);
					DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, strPage));
					
					pBtnPage->SetBkImage(GUIDEPAGE_POINT_NORMAL);
				}
				m_nIndexPage = nPage;
				DuiLib::CControlUI* pConPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("con1")));
				pConPage->SetBkImage(m_vecPage[m_nIndexPage]);
				DuiLib::CButtonUI* pBtnPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pLayout1, str));
				pBtnPage->SetBkImage(GUIDEPAGE_POINT_HIGHTLIGHT);
			}
		}
	}
}

void CDuiGuidePage::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();
	SetIcon(IDR_MAINFRAME);
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));
	if(m_vecPage.size() != 0)
	{
		//计算点的位置，根据引导页的数量计算起始点，两点之间相隔14
		int nFirstX = 0;
		int nY = 476;
		int nPageSize = m_vecPage.size();
		if(nPageSize == 1)
		{
			nFirstX = 350;
		}
		else if(nPageSize % 2 == 0)
		{
			nFirstX = (350 - 7) - (nPageSize / 2 - 1) * 14 - 5;
		}
		else if(nPageSize % 2 == 1)
		{
			nFirstX = 350 - (nPageSize / 2) * 14 - 5;
		}
		
		//绘制引导页和点
		for(int nPage = 0; nPage < m_vecPage.size(); nPage++)
		{
			DuiLib::CButtonUI *pBtnPoint = new DuiLib::CButtonUI;
			DuiLib::CVerticalLayoutUI* pParent = static_cast<DuiLib::CListUI*>(m_PaintManager.FindControl(_T("Layout1")));
			pParent->Add(pBtnPoint);

			RECT rc;
			rc.left = nFirstX + nPage * 14;
			rc.top = nY;
			rc.right =  nFirstX + nPage * 14 + 10;
			rc.bottom = nY + 10;
			CString str;
			str.Format("Page%d", nPage);
			pBtnPoint->SetName(str);
			pBtnPoint->SetFloat(true);
			pBtnPoint->SetVisible(true);
			pBtnPoint->SetPos(rc);
			pBtnPoint->SetBkImage(GUIDEPAGE_POINT_NORMAL);

			if(nPage == 0)
			{
				DuiLib::CControlUI* pConPage = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl(_T("con1")));
				pConPage->SetBkImage(m_vecPage[nPage]);
				pBtnPoint->SetBkImage(GUIDEPAGE_POINT_HIGHTLIGHT);
			}
			pBtnPoint->SetPos(rc);
		}
	}
	this->CenterWindow();
}

void CDuiGuidePage::SetPage(vector<CString> vecPage)
{
	m_vecPage.clear();
	m_vecPage = vecPage;
}