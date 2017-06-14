// MessageBox.cpp : 实现文件
//

#include "stdafx.h"
#include "MessageBox.h"
#include "impublic.h"

using namespace std;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

static int s_nID = 0;
std::map<int, std::tr1::shared_ptr<CMessageBox>> CMessageBox::g_mapDlg;

CMessageBox::CMessageBox(const char *pTitle, const char *pText, UINT nType, HWND hParent, BOOL bModel)
{
	m_strTitle = IM_CAPTION;
	if(pTitle != NULL)
		m_strTitle = pTitle;
	m_strText = pText;
	m_nID = s_nID++;
	m_hParent = hParent;
	if(hParent == NULL)
		m_hParent = GetDesktopWindow();
	m_nType = nType;
	m_bModel = bModel;
}

DuiLib::UILIB_RESOURCETYPE CMessageBox::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CMessageBox::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

int CMessageBox::Prompt( LPCTSTR lpszText, 
							LPCTSTR lpszCaption /*= NULL*/,
							UINT nType /*= MB_OK*/,
							HWND hParent /*= NULL*/,
							BOOL bModel /*= FALSE*/ )
{
	if(lpszCaption == NULL || strcmp(lpszCaption, "") == 0)
		lpszCaption = _T("提示");
	tr1::shared_ptr<CMessageBox> pDlg = tr1::shared_ptr<CMessageBox>(new CMessageBox(lpszCaption, lpszText, nType, hParent, bModel));
	pDlg->Create(hParent, lpszCaption, UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
	int nDlgID = pDlg->GetDlgID();
	g_mapDlg[nDlgID] = pDlg;
	pDlg->CenterWindow();
	FlashWindow(pDlg->GetHWND(), TRUE);
	if(bModel)
	{	// 模态对话框
		return pDlg->ShowModal();
	}
	else
	{
		pDlg->ShowWindow(true);
		return 1;
	}
}

// 弹出一个模态对话框
int CMessageBox::Model(HWND hWnd, LPCTSTR lpszText, LPCTSTR lpszCaption, UINT nType)
{
	return CMessageBox::Prompt(lpszText, lpszCaption, nType, hWnd, TRUE);
}

void CMessageBox::InitWindow()
{
	DuiLib::WindowImplBase::InitWindow();

	// 分析按钮
	DuiLib::CHorizontalLayoutUI *pHoreBtnArea = (DuiLib::CHorizontalLayoutUI*)m_PaintManager.FindControl("horiBtnArea");
	if((m_nType & 0x0000000F) == MB_OKCANCEL)
	{	
		DuiLib::CButtonUI *pBtn = NewBtn("btnok", "确认");
		pHoreBtnArea->Add(pBtn);
		DuiLib::CLabelUI *pLbl = NewLabel();
		pHoreBtnArea->Add(pLbl);

		DuiLib::CButtonUI *pBtnC = NewBtn("btncancel", "取消", FALSE);
		pHoreBtnArea->Add(pBtnC);
		DuiLib::CLabelUI *pLbl1 = NewLabel();
		pHoreBtnArea->Add(pLbl1);
	}
	else if((m_nType & 0x0000000F) == MB_YESNOCANCEL)
	{
		DuiLib::CButtonUI *pBtn = NewBtn("btnyes", "是");
		pHoreBtnArea->Add(pBtn);
		DuiLib::CLabelUI *pLbl = NewLabel();
		pHoreBtnArea->Add(pLbl);

		DuiLib::CButtonUI *pBtn1 = NewBtn("btnno", "否", FALSE);
		pHoreBtnArea->Add(pBtn1);
		DuiLib::CLabelUI *pLbl2 = NewLabel();
		pHoreBtnArea->Add(pLbl2);

		DuiLib::CButtonUI *pBtnC = NewBtn("btncancel", "取消", FALSE);
		pHoreBtnArea->Add(pBtnC);
		DuiLib::CLabelUI *pLbl1 = NewLabel();
		pHoreBtnArea->Add(pLbl1);
	}
	else if((m_nType & 0x0000000F) == MB_YESNO)
	{
		DuiLib::CButtonUI *pBtn = NewBtn("btnyes", "是");
		pHoreBtnArea->Add(pBtn);
		DuiLib::CLabelUI *pLbl = NewLabel();
		pHoreBtnArea->Add(pLbl);

		DuiLib::CButtonUI *pBtn1 = NewBtn("btnno", "否", FALSE);
		pHoreBtnArea->Add(pBtn1);
		DuiLib::CLabelUI *pLbl2 = NewLabel();
		pHoreBtnArea->Add(pLbl2);
	}
	else if((m_nType & 0x0000000F) == MB_RETRYCANCEL)
	{
		DuiLib::CButtonUI *pBtn = NewBtn("btretry", "重试");
		pHoreBtnArea->Add(pBtn);
		DuiLib::CLabelUI *pLbl = NewLabel();
		pHoreBtnArea->Add(pLbl);

		DuiLib::CButtonUI *pBtnC = NewBtn("btncancel", "取消", FALSE);
		pHoreBtnArea->Add(pBtnC);
		DuiLib::CLabelUI *pLbl1 = NewLabel();
		pHoreBtnArea->Add(pLbl1);
	}
	else
	{
		DuiLib::CButtonUI *pBtn = NewBtn("btnok", "确认");
		pHoreBtnArea->Add(pBtn);
		DuiLib::CLabelUI *pLbl = NewLabel();
		pHoreBtnArea->Add(pLbl);
	}
	
	// 加载文本
	DuiLib::CVerticalLayoutUI *pVertTextArea = (DuiLib::CVerticalLayoutUI*)m_PaintManager.FindControl("vertTextArea");
	DuiLib::CVerticalLayoutUI *pVertText = (DuiLib::CVerticalLayoutUI*)m_PaintManager.FindControl("vertText");
	DuiLib::CLabelUI *pLbl = NewLabel(210);
	pVertText->Add(pLbl);
	pLbl->SetText(m_strText.c_str());
	int nWidth = pLbl->CalcRealWidth();
	if(nWidth > 210)
	{
		// 分行添加
		char szWidth[16] = {0};
		sprintf_s(szWidth, "%d", 220);
		pVertTextArea->SetAttribute("width", szWidth);

		DuiLib::CDuiString tmp;
		int iBIndex = 0;
		int nCount = 1;			// 截取的字节数
		vector<string> vecStr;	// 分行后的字符串
		for (int i=0; i<(int)m_strText.size() ;i++, nCount++)
		{	
			int nBack = 1;				// 需要倒退的字节数，默认为1，当为汉字时，为2
			if (m_strText[i] < 0)
			{  			   
				tmp.Format("%c%c",m_strText[i],m_strText[i+1]);
				i++; 
				nCount++;
				nBack = 2;
			}
			else
			{
				tmp.Format("%c",m_strText[i]);
			}

			pLbl->SetText(m_strText.substr(iBIndex, nCount).c_str());
			if(pLbl->CalcRealWidth() > 210 && i<(int)m_strText.size()-1)
			{
				int nCountRel = nCount-1;
				if(m_strText[iBIndex+nCount-1] < 0)
				{
					nCountRel--;
				}
				string strTemp = m_strText.substr(iBIndex, nCountRel);
				nCount = 1;
				i -= nBack;
				iBIndex = i+1;
				vecStr.push_back(strTemp);
			}

			if(i == (int)m_strText.size()-1)
			{	// 末尾的字符特殊处理
				string strTemp = m_strText.substr(iBIndex, m_strText.size()-iBIndex);
				vecStr.push_back(strTemp);
			}
		}

		pLbl->SetAttribute("height", "17");
		pLbl->SetText(vecStr[0].c_str());
		for(int i = 1; i < (int)vecStr.size(); ++i)
		{
			DuiLib::CLabelUI *pLbl = NewLabel(210);
			pVertText->Add(pLbl);
			pLbl->SetAttribute("height", "17");
			pLbl->SetText(vecStr[i].c_str());
		}
	}
	else 
	{
		char szWidth[16] = {0};
		sprintf_s(szWidth, "%d", nWidth);
		pVertTextArea->SetAttribute("width", szWidth);
		pLbl->SetAttribute("width", szWidth);
		pLbl->SetAttribute("align", "center,vcenter");
		pLbl->SetAttribute("height", "17");
	}

	// 标题
	DuiLib::CLabelUI *pLblTitle = (DuiLib::CLabelUI*)m_PaintManager.FindControl("lbTitle");
	pLblTitle->SetText(m_strTitle.c_str());

	UpdateBk();
}

// 创建一个按钮
DuiLib::CButtonUI* CMessageBox::NewBtn(const char *pName, const char *pText, BOOL bBkBlue/*=TRUE*/)
{
	DuiLib::CButtonUI *pBtn = new DuiLib::CButtonUI();
	pBtn->SetNormalImage(bBkBlue ? "SysBtn\\blue_button_normal.png" : "SysBtn\\button_normal.png");
	pBtn->SetHotImage(bBkBlue ? "SysBtn\\blue_button_hover.png" : "SysBtn\\button_hover.png");
	pBtn->SetPushedImage(bBkBlue ? "SysBtn\\blue_button_pressed.png" : "SysBtn\\button_pressed.png");
	pBtn->SetTextColor(bBkBlue ? 0xFFFFFFFF : 0xFF2e2e2e);
	pBtn->SetAttribute("align", "center,vcenter");
	pBtn->SetAttribute("padding", "0,4,0,0");
	pBtn->SetAttribute("width", "63");
	pBtn->SetAttribute("height", "24");
	pBtn->SetName(pName);
	pBtn->SetText(pText);

	return pBtn;
}

// 创建一个占位符
DuiLib::CLabelUI* CMessageBox::NewLabel(int nWidth/*=10*/)
{
	DuiLib::CLabelUI *pLbl = new DuiLib::CLabelUI();
	char szWidth[8] = {0};
	sprintf_s(szWidth, "%d", nWidth);
	pLbl->SetAttribute("width", szWidth);
	pLbl->SetTextColor(0xFF2e2e2e);

	return pLbl;
}

// 关闭窗口时向外界发送消息
void CMessageBox::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if(msg.sType == "click")
	{
		if(msg.pSender->GetName() == "btnok")
		{
			Close(IDOK);
			if(!m_bModel && m_hParent != GetDesktopWindow())
			{	// 非模态对话框，发送消息给父窗口
				::SendMessage(m_hParent, WM_MSGBOX, IDOK, 0);
			}
		}
		else if(msg.pSender->GetName() == "btncancel" || msg.pSender->GetName() == "closebtn")
		{
			Close(IDCANCEL);
			if(!m_bModel && m_hParent != GetDesktopWindow())
			{	// 非模态对话框，发送消息给父窗口
				::SendMessage(m_hParent, WM_MSGBOX, IDCANCEL, 0);
			}
		}
		else if(msg.pSender->GetName() == "btnyes")
		{
			Close(IDYES);
			if(!m_bModel && m_hParent != GetDesktopWindow())
			{	// 非模态对话框，发送消息给父窗口
				::SendMessage(m_hParent, WM_MSGBOX, IDYES, 0);
			}
		}
		else if(msg.pSender->GetName() == "btnno")
		{
			Close(IDNO);
			if(!m_bModel && m_hParent != GetDesktopWindow())
			{	// 非模态对话框，发送消息给父窗口
				::SendMessage(m_hParent, WM_MSGBOX, IDNO, 0);
			}
		}
		else if(msg.pSender->GetName() == "btretry")
		{
			Close(IDRETRY);
			if(!m_bModel && m_hParent != GetDesktopWindow())
			{	// 非模态对话框，发送消息给父窗口
				::SendMessage(m_hParent, WM_MSGBOX, IDRETRY, 0);
			}
		}
	}
}

// 刷新背景
void CMessageBox::UpdateBk()
{
	DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
	if (background != NULL)
	{
		::UpdateBk(background, GetHWND());
	}
}

// 更新所有背景
void CMessageBox::UpdateAllBk()
{
	map<int, std::tr1::shared_ptr<CMessageBox>>::iterator it = g_mapDlg.begin();
	for(; it != g_mapDlg.end(); ++it)
	{
		it->second->UpdateBk();
	}
}