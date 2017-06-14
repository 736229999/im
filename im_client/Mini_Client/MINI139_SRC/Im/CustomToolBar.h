#pragma once

#include "richeditctrlex.h"
#include "SendMsgActionDlg.h"
#include "HttpDownload.h"
#include "buttonext.h"
#include "impublic.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"

#define MAX_DOWNLOAD_TIME		1000*100	// 下载超时时间1分钟左右
#define CHECK_TIME				200			// 检测下载状态的间隔

class CP2PTalkToolBar : public CDialog
{
	DECLARE_DYNAMIC(CP2PTalkToolBar)

public:
	CP2PTalkToolBar(CRichEditCtrlEx *pEditor, XT_FONT *pFont, CWnd* pParent = NULL);   // 标准构造函数
	CP2PTalkToolBar(DuiLib::CRichEditUI *pEditor, XT_FONT *pFont, CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PTalkToolBar();

// 对话框数据
	enum { IDD = IDD_P2PTALK_TOOLBAR };

	void UpdateFontBtnTips();
    void SetFont(XT_FONT * pFont){m_pFont = pFont;}
	void SetRichEdit(DuiLib::CRichEditUI *pEditor);
	void SetRichEdit(CRichEditCtrlEx *pEditor);
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual BOOL OnInitDialog();
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	afx_msg void OnCbnSelchangeComboFontname();
	afx_msg void OnCbnSelchangeComboFontsize();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnFontBoldClicked();
	afx_msg void OnFontItalicClicked();
	afx_msg void OnFontUnderLineClicked();
	afx_msg void OnBnClickedBtnColor();
	DECLARE_MESSAGE_MAP()

private:
	CRichEditCtrlEx  * m_pRichEdit;
	DuiLib::CRichEditUI  * m_pRichEditDui;

	CGraphComboBox     m_comboFontName;
	CGraphComboBox     m_comboFontSize;
	CCheckButton      m_btnBold, m_btnItalic, m_btnUnderline,m_btnColor;

	CRichEditCtrlEx  * m_pEditor;
	XT_FONT			 * m_pFont;
public:
	virtual BOOL PreTranslateMessage(MSG* pMsg);
};



class CP2PToolBar : public CDialog
{
	DECLARE_DYNAMIC(CP2PToolBar)

public:
	CP2PToolBar(CWnd* pParent = NULL);   // 标准构造函数
	virtual ~CP2PToolBar();

	void	ShowActionButton(BOOL b)
	{ 

	};

	bool	StartDownloadFace();

	void	StopDownloadFace();

	bool	IsDownloading() const
	{ return m_download.IsDownloading(); };

	bool	IsDownloader() const
	{ return m_download.IsDownloader(); };

// 对话框数据
	enum { IDD = IDD_P2P_TOOLBAR };

	// 显示或隐藏评分按钮
	void ShowOrHideScoreBtn(BOOL bShow);

	//隐藏抖屏和评分按钮
	void HideScoreShakeBtn();
protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
	virtual void OnOK() {}
	virtual void OnCancel() {}
	DECLARE_MESSAGE_MAP()

	void	DownloadFaceOK(bool bDeleteCanche);

public:
	CBmpBtn		m_btnFont;
	CBmpBtn		m_btnFace;
	CBmpBtn		m_btnPic;
	CSplitBtn	m_btnScreen;
	CBmpBtn		m_btnShake;
	CBmpBtn		m_btnScore;
	CPngBtn		m_btnFastword;
	CFlatButton	m_btnHistory;

public:
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnBnClickedButtonFont();	
	afx_msg void OnBnClickedButtonAction();
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnSize(UINT nType, int cx, int cy);
	virtual BOOL PreTranslateMessage(MSG* pMsg);

	afx_msg void OnBnClickedButtonLink();

protected:
	CFaceDownload	m_download;
	int				m_nTime;
	int				m_nCount;
};
