// MiniMusicPlayerDlg.h : 头文件
//

#pragma once
#include "afxcmn.h"
#include "MusicPlayer.h"
#include "afxwin.h"
#include "WebInterfaceWrapper.h"
#include "afxdlgs.h"

// CMiniMusicPlayerDlg 对话框
class CMiniMusicPlayerDlg : public CDialog
{
// 构造
public:
	CMiniMusicPlayerDlg(CWnd* pParent = NULL);	// 标准构造函数
	~CMiniMusicPlayerDlg();
// 对话框数据
	enum { IDD = IDD_MINIMUSICPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButtonPlay();
	afx_msg void OnBnClickedButtonPause();
	afx_msg void OnBnClickedButtonStop();
	afx_msg void OnBnClickedButtonMy();
	afx_msg void OnNMCustomdrawSlider1(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNMReleasedcaptureSliderTime(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonFriend();

	afx_msg LRESULT OnThreadNotify(WPARAM w,LPARAM l);//自定义消息
	LRESULT OnSoundNotify(WPARAM wParam, LPARAM lParam);

private:
	CListCtrl		m_wndListMusic;
	CMusicPlayer *  m_pMusicPlayer;

	int				m_nTotalTime;
	int				m_nCurTime;
	bool			m_bPlaying;
	bool			m_bPause;
	bool			m_bStop;

private:
	CSliderCtrl		m_sldCtrlTime;
	CSliderCtrl		m_sldCtrlSound;
	CStatic			m_stcTotalTime;
	CStatic			m_stcCurTime;
	CStatic			m_stcMusicName;
	CButton			m_chkRandom;
	bool			m_chkRandomState;

private:
	int						m_nCurrentListItem;
	vector<TWebAudioInfo*>  m_vecFriendAudio;//好友音乐
	vector<TWebAudioInfo*>  m_vecMyAudio;//我的音乐
	vector<TWebAudioInfo*>  m_vecWokuAudio;//我酷推荐
	vector<TWebAudioInfo*>  m_vecPlayingAudio;//当前要播放的音乐列表
	vector<TWebAudioInfo*>  m_vecRandAudio;//无序播放音乐列表

protected:
	static UINT  InitAudioListProc(LPVOID pParam);
public:
	afx_msg void OnNMDblclkListMusic(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnBnClickedButtonNext();
	afx_msg void OnBnClickedButtonPrevious();

	afx_msg void OnNMRclickListMusic(NMHDR *pNMHDR, LRESULT *pResult);

	afx_msg void OnListRButtonPlay();
	afx_msg void OnListRButtonDownLoad();
	//afx_msg void OnNMCustomdrawSliderSound2(NMHDR *pNMHDR, LRESULT *pResult);
	afx_msg void OnNMReleasedcaptureSliderSound2(NMHDR *pNMHDR, LRESULT *pResult);

	int GetListPosition(DWORD dwAudioId);
	void SetRelate(void);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
};
