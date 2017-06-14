#pragma once
#include "afxcmn.h"


// CVersionUpdatgeDlg �Ի���

class CVersionUpdatgeDlg : public CDialog
{
	DECLARE_DYNAMIC(CVersionUpdatgeDlg)

public:
	CVersionUpdatgeDlg(int nVersionFlag, CString strVersionUrl);   // ��׼���캯��
	virtual ~CVersionUpdatgeDlg();

// �Ի�������
	enum { IDD = IDD_DIALOG_DOWNLOAD };

public:
	CString GetDownloadFile() { return m_strDownloadFile;}

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	virtual void OnOK() {}
	virtual void OnCancel() {}

	DECLARE_MESSAGE_MAP()

private:
	static UINT DownloadProc(LPVOID param);
	LRESULT OnDownLoadNotify(WPARAM w,LPARAM l);
	
	void OnBnClickedInstall();

	bool StartDownload();
	bool TerminateDownload();

private:
	HANDLE  m_hThread;

private:
	CProgressCtrl m_wndProgress;

	CString m_strVersionUrl ; //�ͻ������ص�URL
	DWORD   m_dwPreTick;

	CString m_strVersion    ;  //�汾��
	CString m_strDate       ;  //��������
	CString m_strIssueName  ;  //�ļ�����
	CString m_strSize       ;  //�ļ���С
	CString m_strDownloadUrl;  //��������
	CString m_strDownloadFile; //�����ص��ļ�
	DWORD   m_dwRealFileSize;  //kb
	DWORD   m_dwDownLoadSize;  //b

	bool   m_bStopDownload  ;
	int    m_nVersionFlag   ;

public:
	afx_msg void OnTimer(UINT nIDEvent);
	afx_msg void OnNcDestroy();
	afx_msg void OnClose();
	afx_msg void OnBnClickedCancel();
};



#include "..\..\..\..\DuiLib\UIlib.h"
#include "..\..\..\..\Common\WndShadow.h"


class CVersionUpdateDlgDui : public DuiLib::WindowImplBase
{
		
public:
	CVersionUpdateDlgDui(int nVersionFlag, CString strVersionUrl);
	
	~CVersionUpdateDlgDui();
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("CVersionUpdateDlgDui");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("VersionUpdate.xml");    }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}
	
	static std::tr1::shared_ptr<CVersionUpdateDlgDui> Show(int nVersionFlag, CString strVersionUrl);
	
	virtual void InitWindow();
	LRESULT HandleMessage(UINT uMsg,WPARAM wParam,LPARAM lParam);
	// ��Ϣ
	void Notify(DuiLib::TNotifyUI& msg);

	static void UpdateBk();

	
private:
	static UINT DownloadProc(LPVOID param);
	LRESULT OnDownLoadNotify(WPARAM w,LPARAM l);
	bool TerminateDownload();
	bool StartDownload();
	void OnBnClickedInstall();
	
private:
	CWndShadow m_WndShadow;
	static std::tr1::shared_ptr<CVersionUpdateDlgDui> pInstance;
	DuiLib::CButtonUI *m_pBtnInstall;
	DuiLib::CLabelUI *m_pLblDownloadTips;
	DuiLib::CLabelUI *m_pLblDownloadRate;
	DuiLib::CProgressUI *m_pPrgDownload;

	CString m_strVersionUrl ; //�ͻ������ص�URL
	DWORD   m_dwPreTick;

	CString m_strVersion    ;  //�汾��
	CString m_strDate       ;  //��������
	CString m_strIssueName  ;  //�ļ�����
	CString m_strSize       ;  //�ļ���С
	CString m_strDownloadUrl;  //��������
	CString m_strDownloadFile; //�����ص��ļ�
	DWORD   m_dwRealFileSize;  //kb
	DWORD   m_dwDownLoadSize;  //b

	bool   m_bStopDownload  ;
	int    m_nVersionFlag   ;
	HANDLE  m_hThread;

	enum {
		
		DOWNLOAD_RECVING = 0,
		DOWNLOAD_FINISHED = 1,
		DOWNLOAD_FAILED = 2,
	};

};



