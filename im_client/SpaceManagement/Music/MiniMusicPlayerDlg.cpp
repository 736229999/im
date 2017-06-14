// MiniMusicPlayerDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "MiniMusicPlayer.h"
#include "MiniMusicPlayerDlg.h"
#include ".\minimusicplayerdlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#define TIMER_GET_POS	1001
#define TIME_GET_POS	800
#define TIMER_HTTP_FILE 1002
#define TIME_HTTP_FILE	1000
#define WM_MYMESSAGE (WM_USER+100) 

#include <math.h>

// CMiniMusicPlayerDlg 对话框


CMiniMusicPlayerDlg::CMiniMusicPlayerDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CMiniMusicPlayerDlg::IDD, pParent)
	, m_chkRandomState(false)
{
	m_pMusicPlayer = new CMusicPlayer();

	m_bPlaying = false;
	m_bPause = false;
	m_bStop = false;

}
CMiniMusicPlayerDlg::~CMiniMusicPlayerDlg()
{
	delete m_pMusicPlayer;
}

void CMiniMusicPlayerDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Control(pDX, IDC_LIST_MUSIC, m_wndListMusic);
	DDX_Control(pDX, IDC_SLIDER_TIME, m_sldCtrlTime);
	DDX_Control(pDX, IDC_SLIDER_SOUND, m_sldCtrlSound);
	DDX_Control(pDX, IDC_STATIC_TOTALTIME, m_stcTotalTime);
	DDX_Control(pDX, IDC_STATIC_PRESENTTIME, m_stcCurTime);
	DDX_Control(pDX, IDC_STATIC_NAME, m_stcMusicName);
	DDX_Control(pDX, IDC_CHECK_RANDOM, m_chkRandom);
}

BEGIN_MESSAGE_MAP(CMiniMusicPlayerDlg, CDialog)
	ON_WM_PAINT()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON_PLAY, OnBnClickedButtonPlay)
	ON_BN_CLICKED(IDC_BUTTON_PAUSE, OnBnClickedButtonPause)
	ON_BN_CLICKED(IDC_BUTTON_STOP, OnBnClickedButtonStop)
	ON_BN_CLICKED(IDC_BUTTON_MY, OnBnClickedButtonMy)

	ON_WM_TIMER()
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_TIME, OnNMReleasedcaptureSliderTime)
	ON_BN_CLICKED(IDC_BUTTON_FRIEND, OnBnClickedButtonFriend)

	ON_MESSAGE(WM_MYMESSAGE,OnThreadNotify)//自定义的消息
	ON_NOTIFY(NM_DBLCLK, IDC_LIST_MUSIC, OnNMDblclkListMusic)

	ON_BN_CLICKED(IDC_BUTTON_NEXT, OnBnClickedButtonNext)
	ON_BN_CLICKED(IDC_BUTTON_PREVIOUS, OnBnClickedButtonPrevious)
	ON_MESSAGE(WM_SOUND_NOTIFY,OnSoundNotify)
	ON_NOTIFY(NM_RCLICK, IDC_LIST_MUSIC, OnNMRclickListMusic)
	ON_COMMAND(ID_MENU_PLAY, OnListRButtonPlay)
	ON_COMMAND(ID_MENU_DOWNLOAD, OnListRButtonDownLoad)
	//ON_NOTIFY(NM_CUSTOMDRAW, IDC_SLIDER_SOUND, OnNMCustomdrawSliderSound2)
	ON_NOTIFY(NM_RELEASEDCAPTURE, IDC_SLIDER_SOUND, OnNMReleasedcaptureSliderSound2)
	ON_WM_ERASEBKGND()
END_MESSAGE_MAP()


// CMiniMusicPlayerDlg 消息处理程序

BOOL CMiniMusicPlayerDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	m_pMusicPlayer->Init(m_hWnd);
	m_wndListMusic.SetExtendedStyle(LVS_EX_FULLROWSELECT);//设置列表风格为 FullRowSelect

	m_wndListMusic.InsertColumn( 0, "#", LVCFMT_LEFT, 40, 0);
	m_wndListMusic.InsertColumn( 1, "歌曲名", LVCFMT_LEFT, 90, 1);
	m_wndListMusic.InsertColumn( 2, "专辑", LVCFMT_LEFT, 60, 2);
	m_wndListMusic.InsertColumn( 3, "所有者", LVCFMT_LEFT, 80, 3);
	m_nCurrentListItem = 0;//默认播放列表的首行
	m_sldCtrlSound.SetRange( 0, 100);
	m_sldCtrlSound.SetPos(100);
	m_pMusicPlayer->SetVolume(0);//声音初始值设为最大
	
	//
	// 启动线程
	//
	
	AfxBeginThread(InitAudioListProc,this);

	return TRUE;  // 除非设置了控件的焦点，否则返回 TRUE
}


// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CMiniMusicPlayerDlg::OnPaint() 
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

void CMiniMusicPlayerDlg::OnTimer(UINT nIDEvent)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	CString s;
	int nM,nS;
	if (nIDEvent == TIMER_GET_POS)
	{
		if (m_bPlaying == true)
		{
			m_nCurTime = m_pMusicPlayer->GetCurTime();
			m_nCurTime /= 1000;
			nM = m_nCurTime / 60;
			nS = m_nCurTime % 60;
			m_sldCtrlTime.SetPos(m_nCurTime);
			s.Format("%d:%d",nM,nS);
			m_stcCurTime.SetWindowText(s);
		}
	}
	CDialog::OnTimer(nIDEvent);
}

		
void CMiniMusicPlayerDlg::OnBnClickedButtonPlay()
{
	// TODO: 在此添加控件通知处理程序代码
	int nM,nS;
	CString s;

	m_bPlaying = true;

	if(m_pMusicPlayer != NULL)
	{
		if (m_bPause == true)
		{
			m_bPause = false;
			m_pMusicPlayer->Play();
		}
		else
		{
			m_pMusicPlayer->Open();
			m_nTotalTime = m_pMusicPlayer->GetTotalTime();
			m_nTotalTime /= 1000;
			m_sldCtrlTime.SetRange(0, m_nTotalTime);

			nM = m_nTotalTime / 60;
			nS = m_nTotalTime % 60;
			s.Format("%d:%d",nM,nS);
			m_stcTotalTime.SetWindowText(s);
		}	
	}
	SetTimer(TIMER_GET_POS,TIME_GET_POS,NULL);
	m_pMusicPlayer->Play();
	
}

void CMiniMusicPlayerDlg::OnBnClickedButtonPause()
{
	// TODO: 在此添加控件通知处理程序代码
	m_bPause = true;
	m_pMusicPlayer->Pause();
}

void CMiniMusicPlayerDlg::OnBnClickedButtonStop()
{
	// TODO: 在此添加控件通知处理程序代码
	m_pMusicPlayer->Stop();
}

void CMiniMusicPlayerDlg::OnBnClickedButtonMy()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<TWebAudioInfo *>::iterator it;
	TWebAudioInfo *pInfo = NULL;
	CString s;
	int nCount = m_wndListMusic.GetItemCount();
	for (int j = nCount-1; j>=0; j-- )
	{
		m_wndListMusic.DeleteItem(j);
	}
	m_vecPlayingAudio = m_vecMyAudio;//当前播放的是自己的音乐
	m_vecRandAudio = m_vecMyAudio;
	int i = 0;
	for (it=m_vecMyAudio.begin(); it!=m_vecMyAudio.end(); it++, i++)
	{
		s.Format("%d",i + 1);
		m_wndListMusic.InsertItem( i,s);

		pInfo = *(it);
		m_wndListMusic.SetItemText( i, 1, pInfo->szName);
		m_wndListMusic.SetItemText( i, 2, pInfo->szSinger);
		m_wndListMusic.SetItemText( i, 3, pInfo->szLyric);

	}
	CMiniMusicPlayerDlg::SetRelate();//设置当前播放的音乐与列表框关联

}

void CMiniMusicPlayerDlg::OnNMReleasedcaptureSliderTime(NMHDR *pNMHDR, LRESULT *pResult)
{
	// TODO: 在此添加控件通知处理程序代码
	int nPos = m_sldCtrlTime.GetPos();
	m_pMusicPlayer->Seek(nPos * 1000);
	*pResult = 0;
}


void CMiniMusicPlayerDlg::OnBnClickedButtonFriend()
{
	// TODO: 在此添加控件通知处理程序代码
	vector<TWebAudioInfo *>::iterator it;
	TWebAudioInfo *pInfo = NULL;
	CString s;
	int nCount = m_wndListMusic.GetItemCount();
	for (int j = nCount-1; j>=0; j-- )
	{
		m_wndListMusic.DeleteItem(j);
	}
	int i = 0;
	m_vecPlayingAudio = m_vecFriendAudio;//当前播放的是好友的音乐
	m_vecRandAudio = m_vecFriendAudio;
	for (it=m_vecFriendAudio.begin(); it!=m_vecFriendAudio.end(); it++, i++)
	{
		s.Format("%d",i + 1);
		m_wndListMusic.InsertItem( i,s);

		pInfo = *(it);
		m_wndListMusic.SetItemText( i, 1, pInfo->szName);
		m_wndListMusic.SetItemText( i, 2, pInfo->szSinger);
		m_wndListMusic.SetItemText( i, 3, pInfo->szLyric);

	}
	CMiniMusicPlayerDlg::SetRelate();//设置当前播放的音乐与列表框关联
}

UINT CMiniMusicPlayerDlg::InitAudioListProc(LPVOID pParam)
{
	CMiniMusicPlayerDlg * pDlg = (CMiniMusicPlayerDlg *)pParam;
	//循环遍历 g_FriendList 中的成员，获取每个成员的audio list
	//通知窗口，刷新列表控件
	//SendMessage(...)

	CWebInterfaceWrapper obj;
	CString strServerDate;
	list <tagFriendInfo>::iterator it;

	obj.InitInterface(9273109,"luchy129","luchy");

	//得到自己的音乐列表
	obj.GetUserAudioList(9273109,"","",pDlg->m_vecMyAudio,strServerDate);

	//得到好友的音乐列表
	for (it=g_FriendList.begin(); it!=g_FriendList.end(); it++ )
	{
		obj.GetUserAudioList(it->userId,"","",pDlg->m_vecFriendAudio,strServerDate);
	}

	pDlg->SendMessage(WM_MYMESSAGE,0,0);
	return 0;
}

LRESULT CMiniMusicPlayerDlg::OnSoundNotify(WPARAM wParam, LPARAM lParam)
{
	m_chkRandomState = m_chkRandom.GetCheck();
	if (m_chkRandomState == false)
	{
		CMiniMusicPlayerDlg::OnBnClickedButtonNext();
		return 0;
	}
	//打开无序播放
	else
	{
		if (m_vecRandAudio.empty())
		{
			m_vecRandAudio = m_vecPlayingAudio;
		}
		else
		{
			//删除刚播放结束的那首
			//如果是在中间，把它后面的每个元素前移一位
			if (m_nCurrentListItem != m_vecRandAudio.size() - 1)
			{
				for (int i=m_nCurrentListItem; i<m_vecRandAudio.size() - 1; i++)
				{
					m_vecRandAudio[i] = m_vecRandAudio[i + 1];
				}
			}
			//删除
			m_vecRandAudio.pop_back();
		}
		//从没有播放过的里面重新得到随机数
		vector <TWebAudioInfo*>::size_type nRandNum = m_vecRandAudio.size();
		//得到随机播放位置
		srand((unsigned)time(NULL));
		int nRnd = rand() % nRandNum;

		//根据随机序列中的uID查找到对应lis对应中的位置
		DWORD uIdRand = m_vecRandAudio[nRnd]->dwAudioID;
		m_nCurrentListItem = GetListPosition(uIdRand);

		CString strUrl;
		CString strName;
		//当前播放歌曲的名字
		strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
		m_stcMusicName.SetWindowText(strName);

		//设置当前歌曲为选中状态
		m_wndListMusic.SetItemState(m_nCurrentListItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

		strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;
		BOOL bOpen;
		bOpen = m_pMusicPlayer->SetUrl(strUrl);//设置要播放的MP3的URL
		if (bOpen == false)
		{
			m_stcMusicName.SetWindowText("无法打开链接！");
			return 0;
		}
		OnBnClickedButtonPlay();
	}
}

void CMiniMusicPlayerDlg::SetRelate(void)
{
	int nCount = m_wndListMusic.GetItemCount();
	for (int i=0; i<nCount; i++)
	{
		m_wndListMusic.SetItemData(i, m_vecPlayingAudio[i]->dwAudioID);
	}
}

int CMiniMusicPlayerDlg::GetListPosition(DWORD dwAudioId)
{
	int nCount = m_wndListMusic.GetItemCount();
	//先关联当前播放的音乐列表与listCtrl控件中的列表
	for (int i=0; i<nCount; i++)
	{
		if (dwAudioId == m_wndListMusic.GetItemData(i))
		{
			return i;
		}
	}
		return 0;
}


LRESULT CMiniMusicPlayerDlg::OnThreadNotify(WPARAM wParam, LPARAM lParam)
{
	//接收线程消息，更新控件内容
	vector<TWebAudioInfo *>::iterator it;
	TWebAudioInfo *pInfo = NULL;
	CString s;
	int i = 0;
	m_vecPlayingAudio = m_vecFriendAudio;//当前播放的是好友的音乐
	m_vecRandAudio = m_vecFriendAudio;
	for (it=m_vecFriendAudio.begin(); it!=m_vecFriendAudio.end(); it++, i++)
	{
		s.Format("%d",i + 1);
		m_wndListMusic.InsertItem( i,s);

		pInfo = *(it);
		m_wndListMusic.SetItemText( i, 1, pInfo->szName);
		m_wndListMusic.SetItemText( i, 2, pInfo->szSinger);
		m_wndListMusic.SetItemText( i, 3, pInfo->szLyric);

	}
	CMiniMusicPlayerDlg::SetRelate();//设置当前播放的音乐与列表框关联
	return 0;
}
void CMiniMusicPlayerDlg::OnNMDblclkListMusic(NMHDR *pNMHDR, LRESULT *pResult)
{
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

	m_wndListMusic.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_wndListMusic.SubItemHitTest(&lvinfo);
	if(nItem != -1)
	{
		//CString strtemp;
		//strtemp.Format("双击的是第%d行第%d列", lvinfo.iItem + 1, lvinfo.iSubItem + 1);
		//AfxMessageBox(strtemp);
		//得到所选曲目的URL
		m_nCurrentListItem = lvinfo.iItem;
		CString strUrl;
		CString strName;
		//当前播放歌曲的名字
		strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
		m_stcMusicName.SetWindowText(strName);

		strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;
		BOOL bOpen;
		bOpen = m_pMusicPlayer->SetUrl(strUrl);//设置要播放的MP3的URL
		if (bOpen == false)
		{
			m_stcMusicName.SetWindowText("无法打开链接！");
			return;
		}
		OnBnClickedButtonPlay();
	}
	*pResult = 0;
}

void CMiniMusicPlayerDlg::OnBnClickedButtonNext()
{
	int nCount = m_wndListMusic.GetItemCount();
	if (m_nCurrentListItem < nCount-1)
	{
		m_nCurrentListItem ++; 
	}
	else
	{
		m_nCurrentListItem = 0;
	}
	CString strUrl;
	CString strName;
	//当前播放歌曲的名字
	strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
	m_stcMusicName.SetWindowText(strName);

	//设置当前歌曲为选中状态
	m_wndListMusic.SetItemState(m_nCurrentListItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;
	BOOL bOpen;
	bOpen = m_pMusicPlayer->SetUrl(strUrl);//设置要播放的MP3的URL
	if (bOpen == false)
	{
		m_stcMusicName.SetWindowText("无法打开链接！");
		return;
	}
	OnBnClickedButtonPlay();

}

void CMiniMusicPlayerDlg::OnBnClickedButtonPrevious()
{
	int nCount = m_wndListMusic.GetItemCount();
	if (m_nCurrentListItem != 0)
	{
		m_nCurrentListItem --; 
	}
	else
	{
		m_nCurrentListItem = nCount - 1;
	}
	CString strUrl;
	CString strName;
	//当前播放歌曲的名字
	strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
	m_stcMusicName.SetWindowText(strName);

	//设置当前歌曲为选中状态
	m_wndListMusic.SetItemState(m_nCurrentListItem,LVIS_SELECTED|LVIS_FOCUSED,LVIS_SELECTED|LVIS_FOCUSED);

	strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;
	BOOL bOpen;
	bOpen = m_pMusicPlayer->SetUrl(strUrl);//设置要播放的MP3的URL
	if (bOpen == false)
	{
		m_stcMusicName.SetWindowText("无法打开链接！");
		return;
	}
	OnBnClickedButtonPlay();
}

void CMiniMusicPlayerDlg::OnNMRclickListMusic(NMHDR *pNMHDR, LRESULT *pResult)
{
	//NM_LISTVIEW*pNMListView = (NM_LISTVIEW*)pNMHDR;

	LVHITTESTINFO lvinfo;
	DWORD dwPos = GetMessagePos();
	CPoint point(LOWORD(dwPos), HIWORD(dwPos));
	m_wndListMusic.ScreenToClient(&point);
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_wndListMusic.SubItemHitTest(&lvinfo);
	if(nItem != -1)
	{
		CMenu menu,*pSubMenu;
		menu.LoadMenu(IDR_MUSIC_POPMENU);
		pSubMenu = menu.GetSubMenu(0);
		ASSERT(pSubMenu != NULL);
		m_wndListMusic.ClientToScreen(&point);
		pSubMenu->TrackPopupMenu(TPM_LEFTALIGN | TPM_RIGHTBUTTON, point.x, point.y, this);
	
	}
	*pResult = 0;
}

void CMiniMusicPlayerDlg::OnListRButtonPlay()
{
	// TODO: 在此添加命令处理程序代码
	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

	m_wndListMusic.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_wndListMusic.SubItemHitTest(&lvinfo);
	if(nItem != -1)
	{
		m_nCurrentListItem = lvinfo.iItem;
		CString strUrl;
		CString strName;
		//当前播放歌曲的名字
		strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
		m_stcMusicName.SetWindowText(strName);

		strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;
		BOOL bOpen;
		bOpen = m_pMusicPlayer->SetUrl(strUrl);//设置要播放的MP3的URL
		if (bOpen == false)
		{
			m_stcMusicName.SetWindowText("无法打开链接！");
			return;
		}
		OnBnClickedButtonPlay();
	}
}

void CMiniMusicPlayerDlg::OnListRButtonDownLoad()
{
	// TODO: 在此添加命令处理程序代码

	DWORD dwPos = GetMessagePos();
	CPoint point( LOWORD(dwPos), HIWORD(dwPos) );

	m_wndListMusic.ScreenToClient(&point);

	LVHITTESTINFO lvinfo;
	lvinfo.pt = point;
	lvinfo.flags = LVHT_ABOVE;

	int nItem = m_wndListMusic.SubItemHitTest(&lvinfo);
	if(nItem != -1)
	{
		CString strUrl;
		CString strName;
		strName = m_vecPlayingAudio[m_nCurrentListItem]->szName;
		strUrl = m_vecPlayingAudio[m_nCurrentListItem]->szUrl;

		BOOL bOpen;
		bOpen = m_pMusicPlayer->SetUrl(strUrl);
		if (bOpen == false)
		{
			m_stcMusicName.SetWindowText("无法打开链接！");
			return;
		}
		OnBnClickedButtonPlay();

		CFileDialog dlg(FALSE, "mp3",NULL, OFN_HIDEREADONLY | OFN_OVERWRITEPROMPT,
			"(*.mp3)|*.mp3|", this);
		if(dlg.DoModal() == IDCANCEL)
		{
			return;
		}
	}
}

void CMiniMusicPlayerDlg::OnNMReleasedcaptureSliderSound2(NMHDR *pNMHDR, LRESULT *pResult)
{
	//AfxMessageBox("ok");
	// TODO: 在此添加控件通知处理程序代码
	int nPos = m_sldCtrlSound.GetPos();
	//映射声音滑块与音量
	if(nPos <= 0)
	{
		m_pMusicPlayer->SetVolume(-10000);
	}
	else
	{
		double dbPercent = nPos / 100.;
		double scale = log(dbPercent) / log(0.5);
		long nVolume = 0 -scale * 1500;
		//DSBVOLUME_MIN

		m_pMusicPlayer->SetVolume(nVolume);
	}


	*pResult = 0;
}

BOOL CMiniMusicPlayerDlg::OnEraseBkgnd(CDC* pDC)
{
	//CRect clientRect;
	//GetClientRect(clientRect);
	//pDC->FillSolidRect(clientRect, RGB(0, 125, 125));
	//return TRUE;

	return CDialog::OnEraseBkgnd(pDC);

}
