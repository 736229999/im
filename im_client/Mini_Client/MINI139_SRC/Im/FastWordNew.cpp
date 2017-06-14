#include "StdAfx.h"
#include "FastWordNew.h"
#include "LocalData.h"
using namespace DuiLib;
std::tr1::shared_ptr<CFastWordNew> CFastWordNew::pInstance = NULL;
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
CFastWordNew::CFastWordNew(void)
	: m_wndTalkToolBar(&m_reditSend, &m_font, CWnd::FromHandle(GetHWND()))
{
	//m_parent = NULL;
}


CFastWordNew::~CFastWordNew(void)
{
}

LRESULT CFastWordNew::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

void CFastWordNew::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();

	CRect rc;
	if(m_reditSend.GetSafeHwnd() == NULL)
		m_reditSend.Create(ES_MULTILINE | WS_CHILD | WS_VISIBLE | ES_WANTRETURN
		| WS_VSCROLL | ES_AUTOVSCROLL, rc, CWnd::FromHandle(GetHWND()), 10005);
	m_reditSend.SetLimitWordSize(400);//发送框限制输入400个字

	m_parent = NULL;
	CHorizontalLayoutUI* pHlTalkBar = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("hltalkbar")));
	pHlTalkBar->SetVisible(false);
	pHlTalkBar->SetVisible(true);
	SetIcon(IDR_MAINFRAME);

	m_pComboGroup = static_cast<CListUI*>(m_PaintManager.FindControl(_T("comboFastWordGroup")));
	m_editFastWordNum = static_cast<CEditUI*>(m_PaintManager.FindControl(_T("editFastWordNum")));
	m_editFastWordNum->SetMaxChar(4);
	m_reditSend.SetXFont(m_font);
	UpdateCombo();
	UpdateBk();
}

void CFastWordNew::UpdateBk()
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

void CFastWordNew::Notify(DuiLib::TNotifyUI& msg)
{
	if(msg.sType == _T("itemselect"))
	{
		if(m_pComboGroup == (DuiLib::CListUI *)msg.pSender)
		{
			int nSelect = m_pComboGroup->GetCurSel();
			CListLabelElementUI* pListElement = (CListLabelElementUI *)m_pComboGroup->GetItemAt(nSelect);
		
			if(nSelect == m_pComboGroup->GetCount() - 1)
			{
				if(pListElement->GetText() == "新建分组")
				{
					DuiLib::CDialogBuilder builder;
					CListContainerElementUI* pComboElement = static_cast</*CListContainerElementUI*/CListContainerElementUI*>(builder.Create(_T("FastWordGroupComboElement.xml"), (UINT)0, NULL));
					m_pComboGroup->Add(pComboElement);
					m_pComboGroup->RemoveAt(nSelect);
					m_pComboGroup->EndDown();
					DuiLib::CEditUI* pEdit = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindSubControlByName(pComboElement, _T("groupname")));
					pEdit->SetMaxChar(10);
					m_PaintManager.SetFocus(pEdit);
				}
			}
			else
			{
				m_pComboGroup->SetVisible(false);
				CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
				pHLayoutName->SetText(pListElement->GetText());
			}
		}
	}
	if(msg.sType == _T("click")) //点击咨询弹出菜单
	{
		if(msg.pSender->GetName() == _T("btnOtherUser"))
		{
			if(m_pComboGroup->IsVisible())
			{
				m_pComboGroup->SetVisible(false);
			}
			else
			{
				UpdateCombo();
				m_pComboGroup->SelectItem(-1);
				CHorizontalLayoutUI* pHLayoutName = static_cast<CHorizontalLayoutUI*>(m_PaintManager.FindControl(_T("HLayoutName")));
				CRect rc = pHLayoutName->GetPos();

				int nWidth = m_pComboGroup->GetFixedWidth();
				int nHeight = m_pComboGroup->GetFixedHeight();

				rc.left;
				rc.top = rc.bottom+1;
				rc.right = rc.left + nWidth;
				rc.bottom = rc.top + nHeight;

				m_pComboGroup->SetPos(rc);
				m_pComboGroup->SetVisible(true);
			}
		}
		else if(msg.pSender->GetName() == _T("fastwordconfig"))
		{
			CListContainerElementUI* pListElement = (CListContainerElementUI *)m_pComboGroup->GetItemAt(m_pComboGroup->GetCount()-1);
			CEditUI* pEditGroup = static_cast<CEditUI*>(m_PaintManager.FindSubControlByName(pListElement, _T("groupname")));
			CString strGroupCreate = pEditGroup->GetText();
			CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
			pHLayoutName->SetText(strGroupCreate);
			if(!AddFastWordGroup(strGroupCreate))
			{
				CMessageBox::Model(GetHWND(), "检测到重复的组名！", "提示", MB_OK);
				return;
			}
			else
				m_pComboGroup->SetVisible(false);
		}
		else if(msg.pSender->GetName() == _T("closebtn")) 
		{

		}
		else if(msg.pSender->GetName() == _T("btncancel"))
		{
			Close();
		}
		else if(msg.pSender->GetName() == _T("btnok"))
		{
			CString strText;
			m_reditSend.GetText(strText);
			if(strText == "")
			{
				CMessageBox::Model(GetHWND(), "快捷短语不能为空！", "提示", MB_OK);
				return;
			}
			m_pComboGroup->GetCurSel();
			if(m_nType == 1)//添加快捷短语
			{
				if(!AddFastWord())
				{
					CMessageBox::Model(GetHWND(), "检测到重复的快捷短语或快捷编码！", "提示", MB_OK);
					return;
				}
			}
			else if(m_nType == 2)//编辑快捷短语
			{
				if(!EditFastWord())
				{
					CMessageBox::Model(GetHWND(), "检测到重复的快捷短语或快捷编码！", "提示", MB_OK);
					return;
				}
			}
			Close();
		}
	}
	DuiLib::WindowImplBase::Notify(msg);
}

std::tr1::shared_ptr<CFastWordNew> CFastWordNew::NewDlg(HWND wnd, int nType, CString strGroupName, int nFastWordIndex)
{
	if(pInstance == NULL || pInstance->m_hWnd == NULL)
	{
		pInstance = tr1::shared_ptr<CFastWordNew>(new CFastWordNew());
		pInstance->Create(wnd, "快捷短语", UI_WNDSTYLE_FRAME, WS_EX_WINDOWEDGE);
		//pInstance->CenterWindow();
	}
	
	pInstance->SetType(nType);
	pInstance->SetGroupName(strGroupName);
	pInstance->SetFastWordIndex(nFastWordIndex);
	pInstance->UpdateInterface();
	pInstance->CenterWindow();
	::BringWindowToTop(*pInstance);
	pInstance->SetParentHwnd(wnd);
	return pInstance;
}

void CFastWordNew::SetParentHwnd(HWND pDlg)
{
	m_parent = pDlg;
}

void CFastWordNew::UpdateCombo()
{
	m_pComboGroup->RemoveAll();
	vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
	int nHeight = 0;
	for(int nGroup = 0; nGroup < vecFastWordGroupInfo.size(); nGroup++)
	{
		FastWordGroupInfo fastWordGroupInfo = vecFastWordGroupInfo[nGroup];
		CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
		pListElement->SetAttribute("text", fastWordGroupInfo.strGroupName);
		m_pComboGroup->Add(pListElement);
		nHeight += 26;
	}

	CListLabelElementUI* pListElementGroupNew = new DuiLib::CListLabelElementUI;
	pListElementGroupNew->SetAttribute("text", "新建分组");
	m_pComboGroup->Add(pListElementGroupNew);
	nHeight += 26;

	//DuiLib::CDialogBuilder builder;
	//CListContainerElementUI* pComboElement = static_cast</*CListContainerElementUI*/CListContainerElementUI*>(builder.Create(_T("FastWordGroupComboElement.xml"), (UINT)0, NULL));
	//m_pComboGroup->Add(pComboElement);
	//m_pComboGroup->SelectItem(-1);
	//nHeight += 26;

	if(nHeight > 76)
	{
		m_pComboGroup->SetFixedHeight(76);
		//DuiLib::CButtonUI* pBtnConfig = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindSubControlByName(pComboElement, _T("fastwordconfig")));
		//pBtnConfig->SetAttribute("pos", "72,0,125,24");
	}
	else
		m_pComboGroup->SetFixedHeight(nHeight);
}

BOOL CFastWordNew::AddFastWord()
{
	CString strFastWord;
	CString strNum;
	m_reditSend.GetText(strFastWord);
	strNum = m_editFastWordNum->GetText();
	CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
	CString strGroupName = pHLayoutName->GetText();
	if(strGroupName == "")
		strGroupName = "未分组";
	vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();

	vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), strGroupName);
	//添加快捷短语改为不去重
	FastWordGroupInfo &fastWordGroupInfo = *it;
	FastWordInfo fastWordInfo;
	fastWordInfo.strNum = strNum;
	fastWordInfo.strWord = strFastWord;
	fastWordInfo.font = m_font;
	fastWordGroupInfo.vecFastWordInfo.push_back(fastWordInfo);

	g_LocalData.SetFastWordInfo(vecFastWordGroupInfo);
	g_LocalData.SaveFastWordInfo();
	return TRUE;
}

BOOL CFastWordNew::AddFastWordGroup(CString strGroup)
{
	vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
	vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), strGroup);
	if(it != vecFastWordGroupInfo.end())
	{
		return FALSE;
	}
	FastWordGroupInfo fastWordGroupInfo;
	fastWordGroupInfo.strGroupName = strGroup;
	vecFastWordGroupInfo.push_back(fastWordGroupInfo);
	g_LocalData.SetFastWordInfo(vecFastWordGroupInfo);
	g_LocalData.SaveFastWordInfo();
	return TRUE;
}

BOOL CFastWordNew::EditFastWord()
{
	CString strFastWord;
	CString strNum;
	m_reditSend.GetText(strFastWord);
	strNum = m_editFastWordNum->GetText();
	CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
	CString strGroupName = pHLayoutName->GetText();
	vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();

	vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), m_strGroupName);
	if(it != vecFastWordGroupInfo.end())
	{
		FastWordGroupInfo& fastWordGroupInfo = *it;
		if(m_nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size())
		{
			FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[m_nFastWordIndex];
			CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
			CString strGroupNameChange = "";
			strGroupNameChange = pHLayoutName->GetText();
			if(strGroupNameChange == "")
				strGroupNameChange = "未分组";
			if(strGroupNameChange != fastWordGroupInfo.strGroupName)
			{
				g_LocalData.FastWordDelete(fastWordGroupInfo.strGroupName, m_nFastWordIndex);
				if(!AddFastWord())
					return FALSE;
				else
					return TRUE;
			}
			else
			{
				fastWordInfo.strNum = strNum;
				fastWordInfo.strWord = strFastWord;
				fastWordInfo.font = m_font;
			}
		}
	}

	g_LocalData.SetFastWordInfo(vecFastWordGroupInfo);
	g_LocalData.SaveFastWordInfo();
	return TRUE;
}

void CFastWordNew::SetType(int nType)
{
	m_nType = nType;
}

void CFastWordNew::SetGroupName(CString strGroupName)
{
	m_strGroupName = strGroupName;
}

void CFastWordNew::SetFastWordIndex(int nFastWordIndex)
{
	m_nFastWordIndex = nFastWordIndex;
}

void CFastWordNew::UpdateInterface()
{
	if(m_nType == 2)
	{
		CLabelUI* pLabelHead = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("HeadName")));
		pLabelHead->SetText("编辑快捷短语");
		vector<FastWordGroupInfo> vecFastWordGroupInfo = g_LocalData.GetFastWordInfo();
		vector<FastWordGroupInfo>::iterator it = std::find(vecFastWordGroupInfo.begin(), vecFastWordGroupInfo.end(), m_strGroupName);
		if(it != vecFastWordGroupInfo.end())
		{
			FastWordGroupInfo& fastWordGroupInfo = *it;
			if( m_nFastWordIndex < fastWordGroupInfo.vecFastWordInfo.size() )
			{
				FastWordInfo& fastWordInfo = fastWordGroupInfo.vecFastWordInfo[m_nFastWordIndex];
				CLabelUI* pHLayoutName = static_cast<CLabelUI*>(m_PaintManager.FindControl(_T("groupSelectName")));
				pHLayoutName->SetText(fastWordGroupInfo.strGroupName);
				m_reditSend.SetText(fastWordInfo.strWord);
				m_editFastWordNum->SetText(fastWordInfo.strNum);
				m_strFastWordNum = m_editFastWordNum->GetText();
				m_font.flags = fastWordInfo.font.flags;
				m_font.fontColor = fastWordInfo.font.fontColor;
				strcpy(m_font.fontName, fastWordInfo.font.fontName);
				m_font.fontSize = fastWordInfo.font.fontSize;
				m_wndTalkToolBar.SetFont(&m_font);
				m_wndTalkToolBar.UpdateFontBtnTips();
				m_reditSend.SetXFont(m_font);
			}
		}
	}
}