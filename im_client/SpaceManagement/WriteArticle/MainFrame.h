#pragma once

#include "WriteBlogDlg.h"
// CMainFrame �Ի���

class CMainFrame : public CDialog
{
	DECLARE_DYNAMIC(CMainFrame)

public:
	CMainFrame(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainFrame();

	static CMainFrame * Show();
	static void  Close();
// �Ի�������
	enum { IDD = IDD_MAINFRAME };

public:
	static CMainFrame		*	pInstance;
	void NewArticle();
	void EditLocalSavedArticle(const TLocalSavedArticleInfo &info, DWORD dwSavedID);
	void EditWebArticle(const TWebArticleInfo &info);

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual BOOL OnInitDialog();
	afx_msg void OnNcDestroy();
	afx_msg void OnTimer(UINT nIDEvent);
	DECLARE_MESSAGE_MAP()

	

	LRESULT OnInitWriteBlogDlg(WPARAM,LPARAM);
	LRESULT OnThreadNotify(WPARAM wParam, LPARAM lParam);

    bool  m_bStopCmdThread;
	static UINT  CmdScanProc(LPVOID lpParam);
	CCriticalSection    m_secCmd;

	struct tagCmdInfo
	{
		char * pCmdData;
		long   nDataLen;
		long   nParam;
		long   nCmdType;//0 new article, 1 edit web article , 2 edit local article;
		
		tagCmdInfo()
		{
			pCmdData = NULL;
			nDataLen = 0;
			nCmdType = 0;
		}

		~tagCmdInfo()
		{
			if ( pCmdData )
			{
				delete [] pCmdData;
			}
		}
	};


	list<tagCmdInfo *>  m_lstCmd;

	CWriteBlogDlg			*	m_pWriteBlogDlg; 
};
