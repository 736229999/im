// FriendInfoDlg1.cpp : 实现文件
//

#include "stdafx.h"
#include "FriendInfoDlg.h"
#include "afxdialogex.h"
#include "resource.h"
#include "Im\WebProcess.h"
#include "RoundFaceCtrl.h"
#include "FriendInfoArchDlg.h"
#include "Im\FloorpageFriend.h"
#include "Im\P2PSendMsgDlg.h"
#include "Im\DuiP2PSendMsgDlg.h"
#include "../../../Common/Common.h"
#include "XmlAddress.h"
#include "MainFrame.h"

using  namespace  DuiLib;
extern CString g_cCareers[];
extern CWebUserHeadIconProcess g_webUserHeadIconProcess;
extern /*CP2PSendMsgDlg*/CDuiP2PSendMsgDlg * ShowMessageDlg(FRIEND_INFO *pFriend, StruExtraInfo struExtInfo = StruExtraInfo(), BOOL bIsShow = TRUE);
extern void UpdateBk(DuiLib::CControlUI* background, HWND hWnd);
static CFriendInfoDlg *pInstance = NULL;

map<int, std::tr1::shared_ptr<CFriendInfoDlg>> CFriendInfoDlg::g_mapDlg;

CFriendInfoDlg::CFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent)
{
	m_pFriendInfo = new FRIEND_INFO();
	memcpy(m_pFriendInfo, pFriendInfo, sizeof(FRIEND_INFO));
}

CFriendInfoDlg::~CFriendInfoDlg()
{
	if(m_pFriendInfo != NULL)
	{
		delete m_pFriendInfo;
		m_pFriendInfo = NULL;
	}
}

tr1::shared_ptr<CFriendInfoDlg> CFriendInfoDlg::NewDlg(FRIEND_INFO* pGroupFriend)
{
	if(g_mapDlg.find(pGroupFriend->GetId()) != g_mapDlg.end())
	{
		BringWindowToTop(g_mapDlg[pGroupFriend->GetId()]->GetHWND());

		return g_mapDlg[pGroupFriend->GetId()];
	}
	else
	{
		tr1::shared_ptr<CFriendInfoDlg> pDlg = tr1::shared_ptr<CFriendInfoDlg>(new CFriendInfoDlg(pGroupFriend,NULL));
		pDlg->Create(GetDesktopWindow(), _T("好友信息"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
		pDlg->CenterWindow();
		pDlg->ShowWindow(true);
		
		g_mapDlg[pGroupFriend->GetId()] = pDlg;
		return pDlg;
	}
}

void CFriendInfoDlg::OnFinalMessage(HWND hWnd)
{
	g_pProtocol->UnRegistCmd(hWnd);
	DuiLib::WindowImplBase::OnFinalMessage(hWnd);

	map<int, std::tr1::shared_ptr<CFriendInfoDlg>>::iterator it = g_mapDlg.find(m_pFriendInfo->GetId());
	if(it != g_mapDlg.end())
	{
		g_mapDlg.erase(it);
	}
}

//
////
/////*std::tr1::shared_ptr<CFriendInfoDlg>*/ 
///*std::tr1::shared_ptr<CFriendInfoDlg> */CFriendInfoDlg* CFriendInfoDlg::Show(FRIEND_INFO* pGroupFriend)
//{
//
//	if(pInstance != NULL && ::IsWindow(pInstance->GetHWND()))////出现过则到首层显示
//	{    
//		    pInstance->Refresh(pGroupFriend);
//			BringWindowToTop(pInstance->GetHWND());
//			::ShowWindow(pInstance->GetHWND(),SW_NORMAL);
//
//	}
//	else
//	{
//		pInstance = /*std::tr1::shared_ptr<CFriendInfoDlg>(*/new CFriendInfoDlg(pGroupFriend,NULL);
//		pInstance->Create(GetDesktopWindow(), _T("好友信息"), UI_WNDSTYLE_FRAME_DISMAX, WS_EX_WINDOWEDGE);
//		pInstance->CenterWindow();
//	    pInstance->ShowWindow(true);
//	}
//	return pInstance;
//}

void CFriendInfoDlg::InitWindow() 
{
	DuiLib::WindowImplBase::InitWindow();
	m_WndShadow.Create(m_hWnd);
	m_WndShadow.SetSize(5);
	m_WndShadow.SetPosition(0, 0);
	m_WndShadow.SetSharpness(10);
	m_WndShadow.SetColor(RGB(10,10,10));

	SetIcon(IDR_MAINFRAME);

	//////////获取控件变量
	m_pBtnClose  = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnclose"));////关闭
	m_pLabelHeadPhoto = static_cast<DuiLib::CButtonGifUI*>(m_PaintManager.FindControl("HeadPhotoLabel"));//头像
    m_pLabelnickName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("nickNameLabel"));//昵称

	m_pLevel    = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("level"));//等级
	m_pLabelLevel    = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelLabel"));//等级
	m_pLabelSec   = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelSec"));
	m_pLabelThird = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("levelThird"));

	btnViewRecord    = static_cast<DuiLib::CButtonUI*>(m_PaintManager.FindControl("btnViewRecord"));//发起会话

	m_pLabellabelName = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelName"));//姓名
	m_pEditBackName = static_cast<DuiLib:: CEditUI*>(m_PaintManager.FindControl("BackNameEdit"));//备注
	m_pComboFriendGroup = static_cast<DuiLib:: CComboUI*>(m_PaintManager.FindControl("FriGroupCombo"));//好友分组
	////////////////////////////////////////////////////////////////////////////////////////////
    m_pLabelSex = static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelSex"));//性别
	m_pLabelAddr= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelAddr"));//所在地 

	m_pLabelBirthDay= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelBirthDay"));//生日 

	m_pLabelCellPhone= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelCellPhone"));//手机
	m_pLabelTel= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelTel"));//电话    
	m_pLabelMail= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelMail"));//邮箱 
	m_pLlabelCareer= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelCareer"));//行业
	m_pLlabelCareerName= static_cast<DuiLib::CLabelUI*>(m_PaintManager.FindControl("labelCareerName"));//行业值
	UpdateBk();
	/////////////////////////////////////////////////////////////
	
#ifdef _CS_PLAT_EX_  ///飞牛客服平台
	m_pLevel->SetVisible(false);	 ////取消"等级"
	m_pLabelLevel->SetVisible(false);
	m_pLabelSec->SetVisible(false);
	m_pLabelThird->SetVisible(false);
#endif

	
	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,GetHWND());
	g_pProtocol->RegistCmd(CMD_MOD_FCNAME_ACK,GetHWND());
	
	if(m_pFriendInfo != NULL && CMainFrame::IsWork(m_pFriendInfo->GetId()))
	{
		m_pLevel->SetVisible(false);	 ////取消"等级"
		m_pLabelLevel->SetVisible(false);
		m_pLabelSec->SetVisible(false);
		m_pLabelThird->SetVisible(false);
	}
	else
	{
		m_pLevel->SetVisible(true);	 ////取消"等级"
		m_pLabelLevel->SetVisible(true);
		m_pLabelSec->SetVisible(true);
		m_pLabelThird->SetVisible(true);
	}
	//更新好友资料
	XT_USERINFO_GET_REQ req;
	req.fid = m_pFriendInfo->GetId();
	g_pProtocol->SendReq(req);

}

// 更新所有背景
void CFriendInfoDlg::UpdateAllBk()
{
	map<int, std::tr1::shared_ptr<CFriendInfoDlg>>::iterator it = g_mapDlg.begin();
	for(; it != g_mapDlg.end(); ++it)
	{
		it->second->UpdateBk();
	}
}

void CFriendInfoDlg::CloseAll()
{
	map<int, std::tr1::shared_ptr<CFriendInfoDlg>>::iterator it = g_mapDlg.begin();
	for(; it != g_mapDlg.end(); ++it)
	{
		it->second->Close();
	}
}

void CFriendInfoDlg::UpdateBk()
{
	if(::IsWindow(GetHWND()) && m_PaintManager.GetRoot() != NULL)
	{
		DuiLib::CControlUI* background = m_PaintManager.FindControl("background");
		if (background != NULL)
		{
			string strBkImage = g_LocalData.GetBkImage(g_ImUserInfo.GetUserName().c_str());
			BOOL bSolid = strBkImage.find('c') != string::npos;

			if(bSolid)
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);

				TCHAR szBuf[MAX_PATH] = {0};
				_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,18,18'"), strBkImage.c_str());
				background->SetBkImage(szBuf);
			}
			else
			{
				RECT rc;
				GetWindowRect(GetHWND(), &rc);
				int nWidth = rc.right - rc.left;
				TCHAR szBuf[MAX_PATH] = {0};
				if(nWidth <= 600)
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,%d,180' dest='0,0,%d,180'"), strBkImage.c_str(), rc.right-rc.left, rc.right-rc.left);
				}
				else
				{
					_stprintf_s(szBuf, MAX_PATH - 1, _T("file='%s' source='0,0,600,180' dest='0,0,%d,180' corner='580,0,0,0'"), strBkImage.c_str(), nWidth);
				}
				background->SetBkImage(szBuf);
			}
		}
	}
}

// 消息
void CFriendInfoDlg::Notify(DuiLib::TNotifyUI& msg)
{
	DuiLib::WindowImplBase::Notify(msg);
	if (_tcsicmp(msg.sType, _T("click")) == 0)
	{
		if(msg.pSender == btnViewRecord)
		{
		    OnBnClickedSendMsg();/////发起会话
		}
		else if(msg.pSender == m_pBtnClose)
		{
			OnClose();////关闭
		}
	}
}

void CFriendInfoDlg::OnBnClickedSendMsg()
{
	FRIEND_INFO *pFriend = ::FindFriend(m_pFriendInfo->GetId());
	if(pFriend != NULL)
	{
		CMainFrame::OpenP2PMsgDlg(pFriend);
		//ShowMessageDlg(pFriend);
	}
}


BEGIN_XT_MESSAGE_MAP(CFriendInfoDlg)
	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
	//ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)//修改备注		也没有必要处理修改备注的请求
END_XT_MESSAGE_MAP()	


// CFriendInfoDlg 消息处理程序

LRESULT CFriendInfoDlg::HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam)
{
	// 禁止窗口最大化
	if(WM_NCLBUTTONDBLCLK == uMsg)
	{
		return 0;
	}

	// 处理后台交互消息
	if(WM_XT_CMD_NOTIFY == uMsg)
	{
		OnXtMessage(wParam, lParam);
	}

	return DuiLib::WindowImplBase::HandleMessage(uMsg,wParam,lParam);
}

extern string CutRoundImg(LPCTSTR szImgPath, int nNewWidth, int nNewHeight, BOOL bGray = FALSE);
void CFriendInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
{
	if ( wRet==0 )
	{
		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;

		if ( ack->GetId()==m_pFriendInfo->GetId() )
		{   
			// 保留备注名
			CString sCName=m_pFriendInfo->comment_name;
			//memcpy(m_pFriendInfo,ack,sizeof(XT_USER_INFO));
			UpdateUserFullInfo((XT_USER_INFO*)m_pFriendInfo, ack);
			strncpy(m_pFriendInfo->comment_name,sCName,sizeof(m_pFriendInfo->comment_name)-1);
			/*m_pArchDlg->Refresh(m_pFriendInfo);*/
			Refresh(m_pFriendInfo);
		}
	}
}

void CFriendInfoDlg::OnClose()
{
	string strRemarkName = m_pEditBackName->GetText().GetData();
	///*string strRemarkName = m_pArchDlg->GetRemarkName();*/
	if(strRemarkName != m_pFriendInfo->comment_name)
	{
		//发送服务器修改用户备注名称
		XT_MOD_FRIEND_COMMNAME_REQ req;
		req.fid = m_pFriendInfo->GetId();
		req.id=g_ImUserInfo.GetId();
		strcpy(req.commName,strRemarkName.c_str());
		// 发送操作
		g_pProtocol->SendReq(req);

		//修改主界面
		//CMainFrame::GetInstance()->SetCommName(m_pFriendInfo, strRemarkName.c_str());	// 没有必要调用
	}

	////int nGroupIndex = m_pArchDlg->GetGroupIndex();
    int nGroupIndex = GetGroupIndex(); 
	int nGroupItemIndex = g_GroupInfo.FindItemIndex(m_pFriendInfo->group_index);
	if(nGroupItemIndex != nGroupIndex && nGroupIndex != -1)
	{
		if(nGroupIndex > g_GroupInfo.count)
		{
			return;
		}

		//判断是否需要刷新好友树，如果是群成员则判断该成员是否在好友列表中
		FRIEND_INFO *pFriend = ::FindFriend(m_pFriendInfo->GetId());
		if(pFriend != NULL)
		{
			FRIEND_INFO *pFriend = FindFriend(m_pFriendInfo->GetId());
			CMainFrame::GetInstance()->SelItem(pFriend);
			::PostMessage( CMainFrame::GetInstance()->GetHWND(), WM_IMFRIEND_LIST_NOTIFY, NM_FL_DRAGDROP, MAKELPARAM(nGroupIndex,0));
		}
	}
	
	////更新好友资料
	//XT_USERINFO_GET_REQ req;
	//req.fid = m_pFriendInfo->GetId();
	//g_pProtocol->SendReq(req);
	Close();
    //ShowWindow(SW_HIDE);
}


void CFriendInfoDlg::OnModFCNameAck(const char* data,WORD wRet)
{
	XT_MOD_FRIEND_COMMNAME_ACK *pAck=(XT_MOD_FRIEND_COMMNAME_ACK*)data;
	if(wRet==0)///待修改的用户
	{
		if(pAck->fid == m_pFriendInfo->GetId())
		{
			strcpy(m_pFriendInfo->comment_name,pAck->commName);
			// 修改备注成功
			//CMessageBox::Prompt("修改备注成功！服务器已经接受您提交的信息。");
		}
	}
	else
	{
		// 修改备注失败
		//CMessageBox::Prompt(g_pProtocol->GetLastError(),"修改备注失败");
	}
}


string GetAddress(int nPid, int nCid, int nDid)
{
	// 合成地区
	map<int, string> mapP;
	GetAllProvince(mapP);
	map<int, string> mapC;
	map<int, string> mapD;
	string strAddress;
	int nP = 1;
	int nC = 1;
	if(nPid > 0)
	{
		strAddress = mapP[nPid];
		nP = nPid;
	}
	else
	{
		strAddress = mapP[nP];
	}
	strAddress += "  ";
	GetAllCity(nP, mapC);
	if(nCid > 0)
	{
		strAddress += mapC[nCid];
		nC = nCid;
	}
	else
	{
		strAddress += mapC[nC];
	}
	strAddress += "  ";
	GetAllDistrict(nP, nC, mapD);
	if(nDid > 0)
	{
		strAddress += mapD[nDid];
	}
	else
	{
		strAddress += mapD[1];
	}

	return strAddress;
}

// 更新用户信息
void CFriendInfoDlg::Refresh(FRIEND_INFO *pFriendInfo)
{
	if(m_pFriendInfo != pFriendInfo)
	{
		delete m_pFriendInfo;
		m_pFriendInfo = new FRIEND_INFO();
		memcpy(m_pFriendInfo, pFriendInfo, sizeof(FRIEND_INFO));
	}
	/////
	m_pEditBackName->SetMaxChar(8);
	if(CMainFrame::IsBuyer(m_pFriendInfo->GetId()))//用户ID大于5000000表示买家用户，不显示行业
	{
		m_pLlabelCareer->SetVisible(FALSE);
		m_pLlabelCareerName->SetVisible(FALSE);
	}
	else
	{
		m_pLlabelCareer->SetVisible(TRUE);
		m_pLlabelCareerName->SetVisible(TRUE);
	}
	////如果好友不在好友列表中则无法更改备注名
	FRIEND_INFO *pFriendF = ::FindFriend(m_pFriendInfo->GetId());
	if(pFriendF == NULL) ////不允许编辑
	{
		m_pEditBackName->SetEnabled(FALSE);     //备注 
		m_pComboFriendGroup->SetEnabled(FALSE);//好友分组 
	}
	///////////////////////////////////////////////////////////
	// 更新头像
	//CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)m_pFriendInfo->face_url);
	CString strLocalPath =  g_webUserHeadIconProcess.GetFaceFile(m_pFriendInfo);
	CString strGifFile =  "";
	CString strFile =  m_pFriendInfo->face_url;
	if(strFile.Find(".gif")!=-1)
	{
		strGifFile =  strLocalPath + ".gif";////文件改名
		if ( !FileExist(strGifFile) )////不存在则创建
		{
			CopyFile(strLocalPath,strGifFile,FALSE);
		}
	}
	if ( FileExist(strLocalPath) )
	{
		if ( FileExist(strGifFile) )
		{
		   m_pLabelHeadPhoto->SetNormalGifFile(strGifFile);
		   m_pLabelHeadPhoto->SetBkImage("");
		}
		else
		{
			string strImg = CutRoundImg(strLocalPath, 88, 88, FALSE);
			strLocalPath = strImg.c_str();
		   LPCSTR szHead = (LPSTR)(LPCSTR)strLocalPath;
		   m_pLabelHeadPhoto->SetBkImage(szHead);
		    m_pLabelHeadPhoto->SetNormalGifFile("");
		}
		/*m_pLabelHeadPhoto->SetBkImage("SysBtn\\icon_88x88_bg.png");*/
		//CVerticalLayoutUI* pVLhead = static_cast<CVerticalLayoutUI*>(m_PaintManager.FindControl(_T("BkLayout")));
		//pVLhead->SetBkImage(szHead);
	}
	//////系统设置
	m_pLabelnickName->SetText(UserInfoToStr(m_pFriendInfo)); 

#ifndef _CS_PLAT_EX_  ///飞牛客服平台
	// 等级
	int nNextTime = 0;
	int nLevel = CountLevel(m_pFriendInfo->online_time,nNextTime);
	SetLevelPic(nLevel);////设置图片
	CString strMsg = _T("");
	strMsg.Format("当前在线时长为%d,下一次升级需要时间为%d",m_pFriendInfo->online_time,nNextTime);
	m_pLabelLevel->SetToolTip(strMsg);
	m_pLabelSec->SetToolTip(strMsg);
	m_pLabelThird->SetToolTip(strMsg);
#endif

	m_pLabellabelName->SetText(m_pFriendInfo->nickname);
	m_pLabelSex->SetText(SexToStr(m_pFriendInfo->sex));
	// 备注名
	m_pEditBackName->SetText(m_pFriendInfo->comment_name);

	// 生日
	if(strcmp(m_pFriendInfo->birthday, "") != 0)
	{
		int nY, nM, nD;
		sscanf(m_pFriendInfo->birthday, "%4d%2d%2d", &nY, &nM, &nD); 
		CString strBirthday;
		strBirthday.Format("%d年%d月%d日", nY, nM, nD);
		m_pLabelBirthDay->SetText(strBirthday);
	}

	// 电话
	//SetDlgItemText(IDC_STATIC_TEL_VALUE, g_ImUserInfo.);
	// 行业
	CString strCareer = "";
	if(m_pFriendInfo->career > 0 && m_pFriendInfo->career <= 10)
	{
		strCareer = g_cCareers[m_pFriendInfo->career-1];
	}
	m_pLlabelCareerName->SetText(strCareer);

	///////////////////////////////////////////////////////////
	///////////////////////////////////////////////////////////
	//////分组信息
	////m_comboGroup.ResetContent();
	m_pComboFriendGroup->RemoveAll();
	FRIEND_INFO* pFriend = GetFriendInfo(m_pFriendInfo->GetId());
	//////飞牛网组织架构中的不允许移动
	if(pFriend->group_index < FEINIU_GROUP_ID)
	{
		int nSel = 0;
		int j = 0;
		for(int i=0; i<g_GroupInfo.count; i++)
		{
			if(g_GroupInfo.group[i].group_index < FEINIU_GROUP_ID)
			{

				DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
				pListElement->SetAttribute("text",g_GroupInfo.group[i].name);
				pListElement->SetTag(i);
				m_pComboFriendGroup->Add(pListElement);		
				if(g_GroupInfo.group[i].group_index == pFriend->group_index)
				{
					nSel = j;
				}
				j++;
			}			
		}
		m_pComboFriendGroup->SelectItem(nSel);	
	}
	else
	{	// 限定我的团队中的成员不能选择分组
		int nGroupItemIndex = g_GroupInfo.FindItemIndex(pFriend->group_index);
		ASSERT(nGroupItemIndex != -1);
		if(nGroupItemIndex != -1)
		{
			DuiLib::CListLabelElementUI* pListElement = new DuiLib::CListLabelElementUI;
			pListElement->SetAttribute("text",g_GroupInfo.group[nGroupItemIndex].name);
			pListElement->SetTag(nGroupItemIndex);
			m_pComboFriendGroup->Add(pListElement);	
		}
		m_pComboFriendGroup->SelectItem(0);	
	}

	// 私有
	if(m_pFriendInfo->linkpop == 0)
	{
		// 非陌生人或黑名单
		m_pLabelSex->SetText("保密");
		m_pLabelBirthDay->SetText("保密");
		m_pLabelAddr->SetText("保密");
		m_pLabelTel->SetText("保密");
		m_pLabelMail->SetText("保密");
		m_pLlabelCareerName->SetText("保密");
		m_pLabelCellPhone->SetText("保密");
	}
	// 好友可见
	else if(m_pFriendInfo->linkpop == 1)
	{
		if(pFriend->group_index != 1 && pFriend->group_index != 2)
		{
			// 非陌生人或黑名单
			string strAddress = GetAddress(m_pFriendInfo->province, m_pFriendInfo->city, m_pFriendInfo->country);
			m_pLabelAddr->SetText(strAddress.c_str());
			m_pLabelTel->SetText(m_pFriendInfo->mobile);
			m_pLabelMail->SetText(m_pFriendInfo->email);

			//电话(特殊)
#ifdef _VER_INTERNAL_
			string strJob, strExt;
			SplitExtAndJob(m_pFriendInfo->flash_url, strExt, strJob);
			m_pLabelTel->SetText(strExt.c_str());
#endif
		}
		else
		{
			m_pLabelSex->SetText("保密");
			m_pLabelBirthDay->SetText("保密");
			m_pLabelAddr->SetText("保密");
			m_pLabelTel->SetText("保密");
			m_pLabelMail->SetText("保密");
			m_pLlabelCareerName->SetText("保密");
			m_pLabelCellPhone->SetText("保密");
		}
	}
	// 公开
	else
	{
		string strAddress = GetAddress(m_pFriendInfo->province, m_pFriendInfo->city, m_pFriendInfo->country);
		m_pLabelAddr->SetText(strAddress.c_str());
		m_pLabelTel->SetText(m_pFriendInfo->mobile);
		m_pLabelMail->SetText(m_pFriendInfo->email);
	}
    //pInstance = NULL;
}

string CFriendInfoDlg::GetRemarkName()
{
	CString strRemarkName;
	strRemarkName = m_pEditBackName->GetText();////备注
	strRemarkName.Trim();
	return (LPCTSTR)strRemarkName;
}

int CFriendInfoDlg::GetGroupIndex()
{
	int nIndex = 0;
	nIndex = m_pComboFriendGroup->GetCurSel();
	if(nIndex == -1)
	{
		return -1;
	}
	nIndex = m_pComboFriendGroup->GetItemAt(nIndex)->GetTag();
    return nIndex;
}


/*
等级计算公式：
time = (N`2+4*N)*a
a=18
其中N为当前用户等级，time为该等级需要的时间

则当前在线时长time可以计算等级 N = -2 + (√￣(16+time*(4/a)))/2 
*/
int CFriendInfoDlg::CountLevel(int nTime,int&nNextDelta)
{
	int nLevel;
	float fLevel;
	int nNextTime;	// 到下一等级需要的时间
	fLevel=-2.0+sqrt(16.0+nTime*(4.0/18))/2.0;
	nLevel=fLevel;
	if(nLevel==0)
	{
		nLevel=1;
	}
	if(fLevel >= nLevel)
	{
		nLevel++;
	}
	nNextTime=nLevel*nLevel+4*nLevel;
	nNextTime *= 18;
	nNextDelta=nNextTime-nTime;
	return nLevel;
}


void CFriendInfoDlg::SetLevelPic(int nLevel)
{
	switch(nLevel)
	{
	case 1:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 2:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 3:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	case 4:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 5:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(false);
		}
		break;
	case 6:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelThird->SetBkImage("SysBtn\\icon_moon.png");
			m_pLabelSec->SetVisible(true);
			m_pLabelThird->SetVisible(true);
		}
		break;
	default:
		{
			m_pLabelLevel->SetBkImage("SysBtn\\icon_star.png");
			m_pLabelSec->SetVisible(false);
			m_pLabelThird->SetVisible(false);
		}
		break;
	}
}
//////////////////////////////////////////////////////////////////////////////////////
// CFriendInfoDlg 对话框
////
////std::tr1::shared_ptr<CFriendInfoDlg> CFriendInfoDlg::pInstance = NULL;
////
////IMPLEMENT_DYNAMIC(CFriendInfoDlg, CXSkinDlg)
////
////CFriendInfoDlg::CFriendInfoDlg(FRIEND_INFO *pFriendInfo, CWnd* pParent)
////	: CXSkinDlg(IDD_DLG_FRIEND_INFO, pParent)
////{
////	m_colorBackground = RGB(255, 255, 255);
////	m_colorBorder = CHAT_BKG_COLOR;
////	DeleteObject(m_hBkGround);
////	m_hBkGround = ::CreateSolidBrush(m_colorBackground);
////	m_hBkCaptionBar = ::CreateSolidBrush(BACKGND_COLOR);
////	SetDlgStyle(CXSkinDlg::CLOSE);
////
////	m_staLevel.SetBkColor(BACKGND_COLOR);
////	m_staLevel.SetTextColor(RGB(255, 255, 255));
////	m_staLevel.SetFont(g_font12B);
////
////	m_pFriendInfo = pFriendInfo;
////
////	m_font.CreateFont(
////		19,                        // nHeight
////		0,                         // nWidth
////		0,                         // nEscapement
////		0,                         // nOrientation
////		FW_BOLD,                 // nWeight
////		FALSE,                     // bItalic
////		FALSE,                     // bUnderline
////		0,                         // cStrikeOut
////		ANSI_CHARSET,              // nCharSet
////		OUT_DEFAULT_PRECIS,        // nOutPrecision
////		CLIP_DEFAULT_PRECIS,       // nClipPrecision
////		DEFAULT_QUALITY,           // nQuality
////		DEFAULT_PITCH | FF_SWISS,  // nPitchAndFamily
////		_T("微软雅黑"));
////
////	// tab设置
////	m_MainTab.SetDirectMode(CColumnCtrl::EN_X_DIRE);
////	m_MainTab.SetFont(&m_font);
////	//m_MainTab.AddItem("店铺资料");
////	m_MainTab.AddItem("个人资料");
////	m_MainTab.SetBackgroundColor(RGB(96, 173, 221), RGB(50, 109, 146), RGB(96, 173, 221));
////	m_MainTab.SetItemTextColor(RGB(255, 255, 255), RGB(255, 255, 255), RGB(255, 255, 255));
////	m_MainTab.SetFrameModol(FRAME_DRAW_NULL);
////
////	m_pFacePic = NULL;
////	m_pArchDlg = NULL;
////}
////
////CFriendInfoDlg::~CFriendInfoDlg()
////{
////	if(g_pProtocol != NULL)
////	{
////		g_pProtocol->UnRegistCmd(m_hWnd);
////	}
////
////	if(m_pFacePic != NULL)
////	{
////		delete m_pFacePic;
////		m_pFacePic = NULL;
////	}
////
////	if(m_pArchDlg != NULL)
////	{
////		delete m_pArchDlg;
////		m_pArchDlg = NULL;
////	}
////
////	m_font.Detach();
////}
////
////void CFriendInfoDlg::DoDataExchange(CDataExchange* pDX)
////{
////	CXSkinDlg::DoDataExchange(pDX);
////	DDX_Control(pDX, IDC_STATIC_LEVEL, m_staLevel);
////	DDX_Control(pDX, IDC_STATIC_TAB, m_MainTab);
////	DDX_Control(pDX, IDC_BTN_SENDMSG, m_btnSendMsg);
////	DDX_Control(pDX, IDC_BTN_ENTERSHOP, m_btnEnterShop);
////}
////
////void CFriendInfoDlg::Show(FRIEND_INFO *pFriendInfo)
////{
////	pInstance = std::tr1::shared_ptr<CFriendInfoDlg>(new CFriendInfoDlg(pFriendInfo,GetDesktopWindow()));
////	pInstance->Create();
////	pInstance->ShowWindow(SW_SHOW);	
////}
////
////BEGIN_MESSAGE_MAP(CFriendInfoDlg, CXSkinDlg)
////	ON_XT_MESSAGE()
////	ON_WM_ERASEBKGND()
////	ON_WM_CTLCOLOR()
////	ON_WM_CLOSE()
////	ON_BN_CLICKED(IDC_BTN_SENDMSG, OnBnClickedSendMsg)
////	ON_WM_PAINT()
////END_MESSAGE_MAP()
////
////BEGIN_XT_MESSAGE_MAP(CFriendInfoDlg)
////	ON_XT_COMMAND(CMD_USERINFO_GET_ACK,OnUserInfoAck)
////	ON_XT_COMMAND(CMD_MOD_FCNAME_ACK,OnModFCNameAck)//修改备注
////END_XT_MESSAGE_MAP()
////
////
////// CFriendInfoDlg 消息处理程序
////BOOL CFriendInfoDlg::Create()
////{
////	BOOL bRlt = CXSkinDlg::Create(IDD_DLG_FRIEND_INFO, m_pParentWnd);
////	ShowWindow(SW_SHOW);
////
////	return bRlt;
////}
////
////BOOL CFriendInfoDlg::OnInitDialog()
////{
////	CXSkinDlg::OnInitDialog();
////
////	// 遍历所有子控件，设置控件字体
////	HWND hWndChild =::GetWindow(m_hWnd,GW_CHILD); 
////	while(hWndChild)  
////	{  
////		::SendMessage(hWndChild, WM_SETFONT, (WPARAM)g_font12.GetSafeHandle(), TRUE);
////		hWndChild=::GetWindow(hWndChild,GW_HWNDNEXT); 
////	}
////
////	// 头像
////	m_pFacePic = new CRountFaceCtrl(IDB_PNG_BIG_BROUND_NORMAL, IDB_PNG_BIG_BROUND_NORMAL);
////	m_pFacePic->Create(this, 50001);
////	CRect rcPic;
////	m_pFacePic->GetWindowRect(&rcPic);
////	ScreenToClient(&rcPic);
////	CRect rcPicNew;
////	rcPicNew.left = 10;
////	rcPicNew.right = rcPicNew.left + rcPic.Width();
////	rcPicNew.top = (150-rcPic.Height())/2;
////	rcPicNew.bottom = rcPicNew.top + rcPic.Height();
////	m_pFacePic->MoveWindow(&rcPicNew);
////	m_pFacePic->ShowWindow(SW_SHOW);
////
////	// 创建子窗口
////	CRect rcClient;
////	GetClientRect(&rcClient);
////	rcClient.top += 150;
////	rcClient.left += 1;
////	rcClient.right -= 1;
////	rcClient.bottom -= 1;
////
////	// 用户名设置字体
////	GetDlgItem(IDC_STATIC_NAME)->SetFont(&g_font12B);
////	GetDlgItem(IDC_STATIC_NAME)->SetWindowText(m_pFriendInfo->nickname);
////
	//// 等级
	//m_staLevel.SetOnlineTime(m_pFriendInfo->online_time);
////
////	g_pProtocol->RegistCmd(CMD_USERINFO_GET_ACK,m_hWnd);
////	g_pProtocol->RegistCmd(CMD_MOD_FCNAME_ACK,m_hWnd);
////	//g_pProtocol->RegistCmd(CMD_USERINFO_MOD_ACK,m_hWnd);
////
////	CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)m_pFriendInfo->face_url);
////	m_pFacePic->SetPicPath(strLocalPath);
////
////	CRect rcStatic;
////	GetDlgItem(IDC_STATIC_TAB)->GetWindowRect(&rcStatic);
////	ScreenToClient(&rcStatic);
////	CRect rcTab;
////	rcTab.top = 150;
////	rcTab.bottom = rcTab.top + rcStatic.Height();
////	rcTab.left = 1;
////	rcTab.right = rcClient.right;
////	m_MainTab.MoveWindow(&rcTab);
////	m_MainTab.SetCurSel(0);
////
////	// 按钮
////	m_btnSendMsg.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
////	m_btnSendMsg.SetTextColor(RGB(0, 0, 0));
////	m_btnEnterShop.SetPng(IDB_PNG_GRAYBTN_S_HOVER, IDB_PNG_GRAYBTN_S_NORMAL, IDB_PNG_GRAYBTN_S_PRESSED, IDB_PNG_GRAYBTN_S_PRESSED);
////	m_btnEnterShop.SetTextColor(RGB(0, 0, 0));
////
////	// 创建子窗口
////	GetClientRect(&rcClient);
////	rcClient.top += 150 + rcStatic.Height();
////	rcClient.left += 1;
////	rcClient.right -= 1;
////	rcClient.bottom -= 1;
////	m_pArchDlg = new CFriendInfoArchDlg(m_pFriendInfo, this);
////	m_pArchDlg->Create();
////	m_pArchDlg->MoveWindow(rcClient);
////
////	//更新好友资料
////	XT_USERINFO_GET_REQ req;
////	req.fid = m_pFriendInfo->GetId();
////	g_pProtocol->SendReq(req);
////
////	m_btnEnterShop.ShowWindow(SW_HIDE);
////
////	return TRUE;
////}
////
////void CFriendInfoDlg::OnPaint()
////{
////	CXSkinDlg::OnPaint();
////
////	CPaintDC dc(this);
////}
////
////BOOL CFriendInfoDlg::OnEraseBkgnd(CDC* pDC)
////{
////	CXSkinDlg::OnEraseBkgnd(pDC);
////
////	CRect rcClient;
////	GetClientRect(&rcClient);
////	rcClient.top = TITLE_BAR_CY;
////	rcClient.bottom = 150;
////	rcClient.left += 1;
////	rcClient.right -= 1;
////	pDC->FillSolidRect(rcClient,BACKGND_COLOR);	
////
////	return FALSE;
////}
////
////// 设置控件颜色
////HBRUSH CFriendInfoDlg::OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor)
////{
////	HBRUSH hbr = CXSkinDlg::OnCtlColor(pDC, pWnd, nCtlColor);
////
////	if ( nCtlColor==CTLCOLOR_STATIC )
////	{
////		pDC->SetBkMode(TRANSPARENT);
////		pDC->SetTextColor(RGB(255,255,255));
////		hbr = m_hBkCaptionBar;
////	}
////
////	return hbr;
////}
////
////void CFriendInfoDlg::OnUserInfoAck(const char *data,WORD wRet)
////{
////	if ( wRet==0 )
////	{
////		LPXT_USERINFO_GET_ACK ack = (LPXT_USERINFO_GET_ACK)data;
////
////		if ( ack->GetId()==m_pFriendInfo->GetId() )
////		{   
////			// 保留备注名
////			CString sCName=m_pFriendInfo->comment_name;
////			//memcpy(m_pFriendInfo,ack,sizeof(XT_USER_INFO));
////			UpdateUserFullInfo((XT_USER_INFO*)m_pFriendInfo, ack);
////			strncpy(m_pFriendInfo->comment_name,sCName,sizeof(m_pFriendInfo->comment_name)-1);
////			m_pArchDlg->Refresh(m_pFriendInfo);
////
////			// 更新头像
////			CString strLocalPath = g_webUserHeadIconProcess.Url2File((LPTSTR)(LPCTSTR)m_pFriendInfo->face_url);
////			m_pFacePic->SetPicPath(strLocalPath);
////		}
////	}
////}
////
////void CFriendInfoDlg::OnClose()
////{
////	string strRemarkName = m_pArchDlg->GetRemarkName();
////	if(/*!strRemarkName.empty() &&*/ strRemarkName != m_pFriendInfo->comment_name)
////	{
////		//发送服务器修改用户备注名称
////		XT_MOD_FRIEND_COMMNAME_REQ req;
////		req.fid = m_pFriendInfo->GetId();
////		req.id=g_ImUserInfo.GetId();
////		strcpy(req.commName,strRemarkName.c_str());
////		// 发送操作
////		g_pProtocol->SendReq(req);
////
////		//修改主界面
////		g_FloorPageFriend->SetCommName(m_pFriendInfo, strRemarkName.c_str());
////	}
////
////	int nGroupIndex = m_pArchDlg->GetGroupIndex();
////	if(m_pFriendInfo->group_index != nGroupIndex && nGroupIndex != -1)
////	{
////		if(nGroupIndex > g_GroupInfo.count)
////			return;
////
////		if(g_FloorPageFriend != NULL)
////		{
////			//判断是否需要刷新好友树，如果是群成员则判断该成员是否在好友列表中
////			BOOL bFriend = FALSE;
////			FRIEND_LIST::iterator it;
////			for ( it=g_lsFriend.begin();it!=g_lsFriend.end(); it++ )
////			{
////				if ( (*it) == m_pFriendInfo )
////				{
////					bFriend = TRUE;
////					break;
////				}
////			}
////			if(bFriend)
////				::PostMessage( g_FloorPageFriend->m_hWnd, WM_IMFRIEND_LIST_NOTIFY, NM_FL_DRAGDROP, MAKELPARAM(nGroupIndex,0));
////		}
////	}
////	
////	//更新好友资料
////	XT_USERINFO_GET_REQ req;
////	req.fid = m_pFriendInfo->GetId();
////	g_pProtocol->SendReq(req);
////
////	CXSkinDlg::OnClose();
////	DestroyWindow();
////}
////
////
////void CFriendInfoDlg::OnModFCNameAck(const char* data,WORD wRet)
////{
////	XT_MOD_FRIEND_COMMNAME_ACK *pAck=(XT_MOD_FRIEND_COMMNAME_ACK*)data;
////	if(wRet==0 && pAck->fid == m_pFriendInfo->GetId())///待修改的用户
////	{
////		strcpy(m_pFriendInfo->comment_name,pAck->commName);
////		// 修改备注成功
////		CMessageBox::Prompt("修改备注成功！服务器已经接受您提交的信息。");
////	
////	}
////	else
////	{
////		// 修改备注失败
////		CMessageBox::Prompt(g_pProtocol->GetLastError(),"修改备注失败");
////	}
////}
////
////
////void CFriendInfoDlg::OnBnClickedSendMsg()
////{
////	ShowMessageDlg(m_pFriendInfo);
////}
////
////void CFriendInfoDlg::PostNcDestroy()
////{
////	CXSkinDlg::PostNcDestroy();
////}
