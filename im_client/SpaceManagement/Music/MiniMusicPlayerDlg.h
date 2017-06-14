// MiniMusicPlayerDlg.h : ͷ�ļ�
//

#pragma once
#include "afxcmn.h"
#include "MusicPlayer.h"
#include "afxwin.h"
#include "WebInterfaceWrapper.h"
#include "afxdlgs.h"

// CMiniMusicPlayerDlg �Ի���
class CMiniMusicPlayerDlg : public CDialog
{
// ����
public:
	CMiniMusicPlayerDlg(CWnd* pParent = NULL);	// ��׼���캯��
	~CMiniMusicPlayerDlg();
// �Ի�������
	enum { IDD = IDD_MINIMUSICPLAYER_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
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

	afx_msg LRESULT OnThreadNotify(WPARAM w,LPARAM l);//�Զ�����Ϣ
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
	vector<TWebAudioInfo*>  m_vecFriendAudio;//��������
	vector<TWebAudioInfo*>  m_vecMyAudio;//�ҵ�����
	vector<TWebAudioInfo*>  m_vecWokuAudio;//�ҿ��Ƽ�
	vector<TWebAudioInfo*>  m_vecPlayingAudio;//��ǰҪ���ŵ������б�
	vector<TWebAudioInfo*>  m_vecRandAudio;//���򲥷������б�

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
