#pragma once


// CMainToolBar �Ի���

class CMainToolBar : public CDialog
{
	DECLARE_DYNAMIC(CMainToolBar)

public:
	CMainToolBar(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CMainToolBar();

// �Ի�������
	enum { IDD = IDD_MAIN_TOOL };

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	virtual void OnOK() {}
	virtual void OnCancel() {}
	virtual BOOL PreTranslateMessage(MSG* pMsg);
	virtual BOOL OnInitDialog();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);


	DECLARE_MESSAGE_MAP()

protected:
	CFlatButton m_btnBigHead, m_btnMedia, m_btnUploadPic, m_btnWriteArticle, m_btnMyPicture, m_btnMyArticle;
	virtual LRESULT WindowProc(UINT message, WPARAM wParam, LPARAM lParam);
};
