#pragma once
#include "DisplayPictures.h"
#include "stdafx.h"
#include "OptionDlg.h"

// CMainDlg 对话框

#define MAX_THREAD 5

static   BOOL   l_bWndTop=FALSE;
static   int    l_nInterval=3;
static   BOOL   l_bEmptyPictures=FALSE;
static   int    l_nUserID=0;

class CMainDlg : public CDialog
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CMainDlg();

// 对话框数据
	enum { IDD = IDD_DIALOG_MAIN };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

	DECLARE_MESSAGE_MAP()
	
public :
	static  void Show();
	static  void Close();
	static  void SetWindowTop(BOOL  bWndTop);
	static  void SetInterval(int nInterval);
	static  void IsEmptyPictures(BOOL   bEmptyPictures);

protected:
	static CMainDlg		 *pInstance;
	CRect				 m_rcMin,m_rcPopMenu,m_rcPictureBroadDlg;
	CRect				 m_rcPicture;

   	CDisplayPictures	* m_pDlgShowPics;

	CFont				m_fontWnd;
	HICON				m_hCaptionIcon;
	//COptionDlg			m_optionDlg;
	int					m_nInterval;   //图片显示的时间
	BOOL				m_bAlwaysTop;
	BOOL				m_bEmptyPictures;
	HCURSOR   m_hCursor;
	int		m_nPictureIndex;
	int		m_nBitmapIndex;
	int    m_buttonDownPos;    //鼠标按下的区域   1显示图片   2 m_rcPictureBroadDlg  3  m_rcPopMenu   4   m_rcMin
	int		m_nThreadRef;
	
	int    m_nBroadButtonState,m_nMenuButtonState,m_nCloseButtonState;
	int    m_nCurBtn;    //1  BroadButton   2  Menubutton   3  CloseButton;
    CToolTipCtrl   m_ToolTip;
	int    m_nDownPicturesCount;
	int   m_nUserCount;

	BOOL   m_bStopDown;
   BOOL   m_bPicturesListDownLoadEnd;
	
		CCriticalSection           m_secLockShow    ;
	CWebPictureProcess m_webPictureProcess;
	
protected:
	afx_msg void OnNcMouseMove(UINT nHitTest, CPoint point);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnClose();
	afx_msg void OnPaint();
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg LRESULT OnNcHitTest(CPoint point);
	
	virtual BOOL OnInitDialog();
	afx_msg void OnTaskbarExit();
	afx_msg void OnTaskbarMin();
   afx_msg void OnTaskbarShowdlg();
   afx_msg void OnTaskbarOption();

protected:
	void InitDlgInfo();
	int  HitTest(CPoint point);

	void DrawFrame(CDC *pDC);
	void DrawPicture(CDC *pDC);
	void InitDlgPosition();
	void ShowToolTip(CString strTip);
	void InitToolTip();

	HANDLE m_hThreadInit;

	//vector<HANDLE> m_vecWebPictureThreads;
	//int			   m_nNextUserIndex;

	//struct tagWebPictureProcParam
	//{
	//	int nUserIndex;
	//	CMainDlg * pDlg;
	//};

	static UINT	InitWebPictureListProc(LPVOID lpParam);

	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);   // lParam 表示用户的序号—即第几个用户,从0开始;
	LRESULT OnWebPictureNotify(WPARAM wParam, LPARAM lParam);
    LRESULT OnTrayClicked(WPARAM wParam, LPARAM lParam);
	void  ShowTray(BOOL bShow); 
	LRESULT OnDataUpdate(WPARAM wParam, LPARAM lParam);
	void  OnOpenWebPicture(CPoint pt);
	BOOL  IsPictureDownLoad(int nUserIndex);  //判断某个用户图片列表是否下载完成;
	BOOL  EmptyPictures(char * sDirName);
	static CString  GetLastMonth();
	static CString  GetLastTwoMonth();
	static CString  GetCurTime();

protected:
	afx_msg void OnTimer(UINT nIDEvent);		
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);	
	afx_msg BOOL OnSetCursor(CWnd* pWnd, UINT nHitTest, UINT message);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);

protected:
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	
public:
	afx_msg void OnDestroy();

	bool  m_bStopDownload;


};
