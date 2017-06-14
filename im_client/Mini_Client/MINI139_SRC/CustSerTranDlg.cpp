#include "StdAfx.h"
#include "CustSerTranDlg.h"
#include "GdiPlus.h"
#include "Im\impublic.h"
#include "Im\WebProcess.h"
#include "../../../Common/Common.h"
#include "Im\LocalData.h"

extern  CLocalData	g_LocalData;
extern int GetEncoderClsid(const WCHAR* format, CLSID* pClsid);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);

// ͼƬת��ΪԲ��ͼƬ������ԭ��ͼƬ·����
string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE)
{	
	// ����GDI+
	unsigned long pGdiToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&pGdiToken,&gdiplusStartupInput,NULL);

	// ����ͼƬ
	WCHAR buf[1024];
	CStringW strWide = CT2CW(szImgPath); // ��T�ַ�ת���ɿ��ַ�
	wcscpy(buf, strWide); 
	Gdiplus::Image image(buf, FALSE);

	// ������ͼƬ
	UINT nOldWidth = image.GetWidth();
	UINT nOldHeight = image.GetHeight();
	Gdiplus::Bitmap bmp(nNewWidth, nNewHeight);
	Graphics *pGpBmp = Graphics::FromImage(&bmp);
	// ����ɫ��䱳��ɫ��������󣬽�������Ϊ͸��ɫ
	SolidBrush brush(0x89abcd);		// 
	pGpBmp->FillRectangle(&brush, 0, 0, nNewWidth, nNewHeight);
	pGpBmp->SetSmoothingMode(SmoothingModeAntiAlias);		// �����
	pGpBmp->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	// ���û�������
	GraphicsPath path;
	path.AddEllipse(1, 1, nNewWidth-3, nNewHeight-3); 
	pGpBmp->SetClip(&path);
	// ��ԭͼ��������ͼƬ��
	//pGpBmp->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
	pGpBmp->DrawImage(&image, 0, 0, nNewWidth, nNewHeight);
	pGpBmp->ResetClip();
	// ���ư�ΧȦ
	Pen pen(Color(255, 255, 255, 255), 2);  
	pen.SetDashStyle(DashStyleSolid);	// ʵ��  
	pGpBmp->DrawEllipse(&pen, 1, 1, nNewWidth-3, nNewHeight-3);
	// ����͸��ɫ
	ImageAttributes att;
	att.SetColorKey(0x89abcd, 0x89abcd);
	if(bGray)
	{
		ColorMatrix colormatrix = 
		{
			0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
			0.587f, 0.587f, 0.587f, 0.0f, 0.0f,
			0.144f, 0.144f, 0.144f, 0.0f, 0.0f,
			0.000f, 0.000f, 0.000f, 1.0f, 0.0f,
			0.000f, 0.000f, 0.000f, 0.0f, 1.0f
		};

		att.SetColorMatrix(&colormatrix, 
			ColorMatrixFlagsDefault, 
			ColorAdjustTypeBitmap);
	}
	RectF destRect(REAL(0),	REAL(0), REAL(nNewWidth), REAL(nNewHeight));
	pGpBmp->DrawImage(&bmp, destRect, REAL(0),	REAL(0), REAL(nNewWidth), REAL(nNewHeight), UnitPixel, &att);

	// ����ͼƬ
	CString strOutImgPath;
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	if(pngClsid != CLSID_NULL)
	{
		unsigned __int64 iCpuCycle = GetCpuCycle();
		CString strName;
		strName.Format("%llu.png", iCpuCycle);
		strOutImgPath = g_LocalData.GetRoundPath(strName).c_str();
		CStringW strOutImgPathW = CT2CW(strOutImgPath); // ��T�ַ�ת���ɿ��ַ�
		bmp.Save(strOutImgPathW, &pngClsid, NULL);
	}

	// ж��GDI+
	GdiplusShutdown(pGdiToken);
	if(strOutImgPath.IsEmpty())
		return szImgPath;
	else
		return (LPCTSTR)strOutImgPath;
}

string CutRectImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE)
{	
	// ����GDI+
	unsigned long pGdiToken;
	GdiplusStartupInput gdiplusStartupInput;
	GdiplusStartup(&pGdiToken,&gdiplusStartupInput,NULL);

	// ����ͼƬ
	WCHAR buf[1024];
	CStringW strWide = CT2CW(szImgPath); // ��T�ַ�ת���ɿ��ַ�
	wcscpy(buf, strWide); 
	Gdiplus::Image image(buf, FALSE);

	// ������ͼƬ
	UINT nOldWidth = image.GetWidth();
	UINT nOldHeight = image.GetHeight();
	Gdiplus::Bitmap bmp(nNewWidth, nNewHeight);
	Graphics *pGpBmp = Graphics::FromImage(&bmp);
	// ����ɫ��䱳��ɫ��������󣬽�������Ϊ͸��ɫ
	SolidBrush brush(0x89abcd);		// 
	pGpBmp->FillRectangle(&brush, 0, 0, nNewWidth, nNewHeight);
	pGpBmp->SetSmoothingMode(SmoothingModeAntiAlias);		// �����
	pGpBmp->SetInterpolationMode(InterpolationModeHighQualityBicubic);
	// ��ԭͼ��������ͼƬ��
	//pGpBmp->SetInterpolationMode(Gdiplus::InterpolationModeNearestNeighbor); 
	pGpBmp->DrawImage(&image, 0, 0, nNewWidth, nNewHeight);
	pGpBmp->ResetClip();
	// ���ư�ΧȦ
	Pen pen(Color(255, 255, 255, 255), 2);  
	pen.SetDashStyle(DashStyleSolid);	// ʵ��  
	pGpBmp->DrawRectangle(&pen, 1, 1, nNewWidth, nNewHeight);
	// ����͸��ɫ
	ImageAttributes att;
	att.SetColorKey(0x89abcd, 0x89abcd);
	if(bGray)
	{
		ColorMatrix colormatrix = 
		{
			0.299f, 0.299f, 0.299f, 0.0f, 0.0f,
			0.587f, 0.587f, 0.587f, 0.0f, 0.0f,
			0.144f, 0.144f, 0.144f, 0.0f, 0.0f,
			0.000f, 0.000f, 0.000f, 1.0f, 0.0f,
			0.000f, 0.000f, 0.000f, 0.0f, 1.0f
		};

		att.SetColorMatrix(&colormatrix, 
			ColorMatrixFlagsDefault, 
			ColorAdjustTypeBitmap);
	}
	RectF destRect(REAL(0),	REAL(0), REAL(nNewWidth), REAL(nNewHeight));
	pGpBmp->DrawImage(&bmp, destRect, REAL(0),	REAL(0), REAL(nNewWidth), REAL(nNewHeight), UnitPixel, &att);

	// ����ͼƬ
	CString strOutImgPath;
	CLSID pngClsid;
	GetEncoderClsid(L"image/png", &pngClsid);
	if(pngClsid != CLSID_NULL)
	{
		unsigned __int64 iCpuCycle = GetCpuCycle();
		CString strName;
		strName.Format("%llu.png", iCpuCycle);
		strOutImgPath = g_LocalData.GetRoundPath(strName).c_str();
		CStringW strOutImgPathW = CT2CW(strOutImgPath); // ��T�ַ�ת���ɿ��ַ�
		bmp.Save(strOutImgPathW, &pngClsid, NULL);
	}

	// ж��GDI+
	GdiplusShutdown(pGdiToken);
	if(strOutImgPath.IsEmpty())
		return szImgPath;
	else
		return (LPCTSTR)strOutImgPath;
}

bool ItemSortByState(FRIEND_INFO *pFriend1, FRIEND_INFO *pFriend2)
{
	return pFriend1->status < pFriend2->status;
}

bool ItemSortByText(FRIEND_INFO *pFriend1, FRIEND_INFO *pFriend2)
{
	bool bRet = ( strcmp(pFriend1->nickname,pFriend2->nickname) < 0 ) ? true : false;
	return bRet;
}

void SortTeam(vector<FRIEND_INFO*> &lsTeam)
{
	//��״̬����
	std::sort(lsTeam.begin(),lsTeam.end(),ItemSortByState);

	//����������
	vector<FRIEND_INFO*>::iterator it;
	int nOnlineCnt = 0;
	for(it=lsTeam.begin();it<lsTeam.end(); it++)
	{
		FRIEND_INFO* pFriend = (FRIEND_INFO*)(*it);
		if(pFriend->status == XTREAM_OFFLINE)
			break;
		nOnlineCnt++;
	}

	std::sort(lsTeam.begin(),it,ItemSortByText);
	std::sort(it,lsTeam.end(),ItemSortByText);
}

	//ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)//�޸ı�ע

DuiLib::UILIB_RESOURCETYPE CCustSerTranDlg::GetResourceType() const
{
	return DuiLib::UILIB_ZIPRESOURCE;
}

LPCTSTR CCustSerTranDlg::GetResourceID() const
{
	return MAKEINTRESOURCE(IDR_ZIPRES1);
}

void CCustSerTranDlg::InitWindow() 
{
	m_nSelFriendID = -1;
	m_strOrderID = "";
	m_strReason = "";

	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));
	m_pFriendList = static_cast<CFriendListUI*>(m_PaintManager.FindControl("friends"));
	m_pTab = static_cast<DuiLib::CTabLayoutUI*>(m_PaintManager.FindControl("tabs"));
	m_pButtonClose1 = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtnL1"));
	m_pButtonClose2 = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("closebtnL2"));
	m_pButtonOK = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("okbtn"));
	m_pLabelTranObject = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("tranobject"));
	m_pEditOrderId = static_cast<DuiLib::CEditUI*>(m_PaintManager.FindControl("orderid"));
	m_pEditReason = static_cast<DuiLib::CRichEditUI*>(m_PaintManager.FindControl("reason"));
	m_pBtnBack = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("backbtn"));

	UpdateBk();

	AddGroup("�ҵ��Ŷ�");

	// ����ҵ��Ŷӳ�Ա
	vector<FRIEND_INFO*> lsTeamFriend; 
	FRIEND_LIST::iterator it;
	for (it=g_lsFriend.begin(); it!=g_lsFriend.end();it++ )////���������Ϣ
	{
		FRIEND_INFO *pFriend = (*it);
		if( pFriend->group_index >= /*MY_TEAM_GROUP_ID*/FEINIU_GROUP_ID && pFriend->GetId() != g_ImUserInfo.GetId())
		{
			lsTeamFriend.push_back(pFriend);
		}
	}

	// ����
	SortTeam(lsTeamFriend);

	// ��ʾ������
	vector<FRIEND_INFO*>::iterator itTeam = lsTeamFriend.begin();
	for (; itTeam!=lsTeamFriend.end();itTeam++ )////���������Ϣ
	{
		CString str;
		FRIEND_INFO *pFriend = (*itTeam);
		
		str.Format("%s", UserInfoToStr(pFriend));

		if(pFriend->status == XTREAM_OFFLINE || pFriend->status == XTREAM_HIDE)
		{
			continue;
		}

		CString strFaceFile = g_webUserHeadIconProcess.GetFaceFile(pFriend);
		CString strGifFile = strFaceFile+".gif";
		//if(FileExist(strGifFile))
		//{
		//	string strOutImg = strGifFile.GetBuffer(strGifFile.GetLength());
		//	AddFriend(strOutImg.c_str(), str, pFriend->GetId(), pFriend->status == XTREAM_OFFLINE ? 0x727272 : 0x2E2E2E);
		//}
		//else
		//{
			string strOutImg = CutRoundImg(strFaceFile, 30, 30, pFriend->status == XTREAM_OFFLINE);
			AddFriend(strOutImg.c_str(), str, pFriend->GetId(), pFriend->status == XTREAM_OFFLINE ? 0x727272 : 0x2E2E2E);
		//}
	}
}

LRESULT CCustSerTranDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// ��ֹ�������
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

DuiLib::CControlUI* CCustSerTranDlg::CreateControl(LPCTSTR pstrClass)
{
	if (_tcsicmp(pstrClass, _T("FriendList")) == 0)
	{
		return new CFriendListUI(m_PaintManager);
	}

	return NULL;
}

void CCustSerTranDlg::AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor /*= 0x2E2E2E*/)
{
	m_pFriendList->AddFriend(szImgPath, szText, id, dwTextColor);
}

void CCustSerTranDlg::AddGroup(LPCTSTR szText)
{
	m_pFriendList->AddGroup(szText);
}

void CCustSerTranDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);

	if (_tcsicmp(msg.sType, DUI_MSGTYPE_ITEMSELECT) == 0)
	{
		DuiLib::CControlUI *pItem = m_pFriendList->GetItemAt(msg.wParam);
		if(pItem != NULL && pItem->GetTag() == -1)
		{
			if(msg.lParam > msg.wParam)
			{	// �������ϼ�
				pItem = m_pFriendList->GetItemAt(msg.wParam-1);
				if(pItem != NULL && pItem->GetTag() != -1)
				{
					msg.wParam -= 1;
				}
				else
				{
					msg.wParam = msg.lParam;
				}
				m_pFriendList->SelectItem(msg.wParam);
			}
			if(msg.lParam < msg.wParam)
			{	// �������¼�
				pItem = m_pFriendList->GetItemAt(msg.wParam+1);
				if(pItem != NULL && pItem->GetTag() != -1)
				{
					msg.wParam += 1;
				}
				else
				{
					msg.wParam = msg.lParam;
				}
				m_pFriendList->SelectItem(msg.wParam);
			}
		}
	}

	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		int nTag = (int)msg.pSender->GetTag();
		if(nTag == -1)
			m_pFriendList->CliekedGroup();

		if(msg.pSender == m_pButtonOK)
		{
			m_strOrderID = m_pEditOrderId->GetText().GetData();
			m_strReason = m_pEditReason->GetText().GetData();
			
			Close(0);
		}
		if(msg.pSender == m_pButtonClose1 || msg.pSender == m_pButtonClose2)
		{
			Close(1);
		}
		if(msg.pSender == m_pBtnBack)
		{
			// ��ת
			m_pTab->SelectItem(0);
		}
	}

	if(msg.sType == "itemactivate")
	{
		// ��ȡת�Ӷ���
		int nIndex = m_pFriendList->GetCurSel();
		DuiLib::CListContainerElementUI* pItem = static_cast<DuiLib::CListContainerElementUI*>(m_pFriendList->GetItemAt(nIndex));
		m_nSelFriendID = pItem->GetTag();	// ����ID
		FRIEND_INFO *pFriend = GetFriendInfo(m_nSelFriendID);
		if(pFriend == NULL)
			return;
#if defined (_BUSINESS_VESION) && !defined(_CS_PLAT_EX_) && !defined(_WORK_EX_)
		Close(0);
#elif defined _CS_PLAT_EX_
		Close(0);
#else
		// ��ת
		m_pTab->SelectItem(1);
		m_pLabelTranObject->SetText(pFriend->nickname);
#endif
	}
}

void CCustSerTranDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()))
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			::UpdateBk(background, GetHWND());
		}
	}
}


///////////////////////////////////////////////////////////////////////////////////////////////////////////
CFriendListUI::CFriendListUI(DuiLib::CPaintManagerUI& paint_manager)
	: paint_manager_(paint_manager)
{
}

CFriendListUI::~CFriendListUI()
{
}

bool CFriendListUI::AddGroup(LPCTSTR szText)
{
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(dlgBuilder.Create(_T("FriendListGroup.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return false;

	DuiLib::CButtonUI *pBtn = static_cast<DuiLib::CButtonUI*>(pListElement->FindSubControl("nicknameBtn"));
	if(pBtn == NULL)
		return false;

	CString strFile;
	strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_on_arrow.png");
	pBtn->SetBkImage(strFile);
	strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_on_arrow_hover.png");
	pBtn->SetHotImage(strFile);
	pBtn->SetPushedImage(strFile);

	DuiLib::CDuiRect rcTextPading(32, 0, 0, 0);
	pBtn->SetTextPadding(rcTextPading);
	pBtn->SetText(szText);
	pBtn->SetTag(-1);				// tagΪ-1��ʾitemΪ����

	m_pGroup = pListElement;
	m_bGroupExpend = TRUE;
	pListElement->SetTag(-1);		// tagΪ-1��ʾitemΪ����

	return CListUI::Add(pListElement);
}

bool CFriendListUI::AddFriend(LPCTSTR szImgPath, LPCTSTR szText, unsigned int id, DWORD dwTextColor /*= 0x2E2E2E*/)
{
	DuiLib::CListContainerElementUI* pListElement = NULL;
	DuiLib::CDialogBuilder dlgBuilder;
	pListElement = static_cast<DuiLib::CListContainerElementUI*>(dlgBuilder.Create(_T("FriendListItem.xml"), (UINT)0, NULL, &paint_manager_));
	if (pListElement == NULL)
		return false;

	DuiLib::CLabelUI *pLabelNick= static_cast<DuiLib::CButtonGifUI*>(pListElement->FindSubControl("labelfill"));
	DuiLib::CButtonGifUI *pHead = static_cast<DuiLib::CButtonGifUI*>(pListElement->FindSubControl("nickname"));
	if(pHead == NULL)
		return false;

	// ����������ɫ
	pLabelNick->SetTextColor(dwTextColor);

	CString strFile;
	strFile.Format("file='%s' dest='30,1,60,31'", szImgPath);
	CString strPicFile = szImgPath;
	BOOL bGifFlag = strPicFile.Find(".gif") != string::npos;////�Ƿ�Ϊgif��ʾ
	if(bGifFlag)
	{
		pHead->SetBkImage("");
		pHead->SetNormalGifFile(strPicFile);
	}
	else
	{
		pHead->SetBkImage(strPicFile);
		pHead->SetNormalGifFile("");
	}
	
	DuiLib::CDuiRect rcTextPading(70, 0, 0, 0);
	pLabelNick->SetTextPadding(rcTextPading);
	pLabelNick->SetText(szText);
	pListElement->SetTag(id);

	m_vecpFriendItem.push_back(pListElement);

	return CListUI::Add(pListElement);
}

void CFriendListUI::CliekedGroup()
{
	DuiLib::CButtonUI *pBtn = static_cast<DuiLib::CButtonUI*>(m_pGroup->FindSubControl("nicknameBtn"));
	if(pBtn == NULL)
		return;

	if(m_bGroupExpend)
	{
		CString strFile;
		strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_off_arrow.png");
		pBtn->SetBkImage(strFile);
		strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_off_arrow_hover.png");
		pBtn->SetHotImage(strFile);
		pBtn->SetPushedImage(strFile);
	}
	else
	{
		CString strFile;
		strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_on_arrow.png");
		pBtn->SetBkImage(strFile);
		strFile.Format("file='%s' dest='10,10,22,22'", "SysBtn\\gourp_on_arrow_hover.png");
		pBtn->SetHotImage(strFile);
		pBtn->SetPushedImage(strFile);
	}

	m_bGroupExpend = !m_bGroupExpend;
	for(int i = 0; i < m_vecpFriendItem.size(); ++i)
	{
		m_vecpFriendItem[i]->SetVisible(m_bGroupExpend);
	}
}