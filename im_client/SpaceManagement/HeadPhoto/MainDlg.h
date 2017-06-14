#pragma once

#include "formprogress.h"

#include "explorer1.h"
#include "hpactxctrl.h"

class CMainDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CMainDlg)

public:
	virtual ~CMainDlg();

	static void Show();
	static void Close();

	enum { IDD = IDD_HEADPHOTO_DIALOG };

protected:
	virtual BOOL OnInitDialog();
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	LRESULT OnUploadFinished(WPARAM w,LPARAM l);
	LRESULT OnUploadCancel(WPARAM w,LPARAM l);

protected:
	CMainDlg(CWnd* pParent = NULL);   // 标准构造函数

	void OnUploadPictureHpactxctrlControl(signed char* szPictureFile);
	void OnUploadWebiconHpactxctrlControl(signed char* szPictureFile);
	void OnBeforeSaveFileHpactxctrlControl(signed char* szFile);
	void OnBeforePrintHpactxctrlControl(signed char* szFile);
	void BeforeNavigate2ExplorerWebResource(LPDISPATCH pDisp, VARIANT* URL, VARIANT* Flags, VARIANT* TargetFrameName, VARIANT* PostData, VARIANT* Headers, BOOL* Cancel);
	DECLARE_EVENTSINK_MAP()

	virtual void OnHelpClick();

protected:
	CExplorer1 m_wndIEFrame;
	CHeadPhotoOcxCtrl	m_wndHeadPhotoOcx;
	CFormProgress3 *    m_pWndProgress ;

	static CMainDlg *pInstance;

protected:
	struct tagPhotoFrame
	{
		float fPrice;
		char  szName[255];
	};

	void    AddPhotoFrameMap(int id, float fPrice, CString str);
	bool    GetPhotoFrame(int id, tagPhotoFrame &photoFrame);
	bool    BuyPhotoFrame();
	void	PayPhotoFrame();

	map<int,tagPhotoFrame>  m_mapFrame;//相框map<id,tagPhotoFrame>

protected:
	static UINT ThreadProc(LPVOID param);
	void   StopUpload();

	HANDLE      m_hThread;
	int         m_iUploadType;

	int			m_iHttpFileSize;
};
