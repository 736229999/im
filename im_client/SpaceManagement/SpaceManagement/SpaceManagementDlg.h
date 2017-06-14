// SpaceManagementDlg.h : ͷ�ļ�
//

#pragma once
/*
#import "..\bin\UserInfo.dll" named_guids no_namespace
#import "..\bin\WriteArticle.dll" named_guids no_namespace
#import "..\bin\MyArticles.dll" named_guids no_namespace
#import "..\bin\MyPictures.dll" named_guids no_namespace
#import "..\bin\HeadPhoto.dll" named_guids no_namespace
#import "..\bin\WebInterface.dll" named_guids no_namespace
#import "..\bin\PictureLoadX.dll" named_guids no_namespace

#import "..\bin\MyArticles.dll" named_guids no_namespace
*/
#import "..\WebConfig\Debug\WebConfig.tlb" named_guids no_namespace

//#import "..\bin\Music.dll" named_guids no_namespace
// CSpaceManagementDlg �Ի���
class CSpaceManagementDlg : public CDialog
{
// ����
public:
	CSpaceManagementDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_SPACEMANAGEMENT_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	IWriteArticleXPtr  m_pWriteArticle;
	IMyArticlesXPtr    m_pMyArticles;
	IHeadPhotoXPtr     m_pHeadPhoto;
	IMyPicturesXPtr    m_pMyPictures;
	IWebInterfaceXPtr  m_pWebInterface;
	IMiniMusicPlayerPtr	   m_pMiniMusicPlayer;
	IIMiniPictureBroadCastXPtr m_pMiniPictureBroadCast;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnBnClickedButton1();
	afx_msg void OnBnClickedButton2();
	afx_msg void OnBnClickedButton3();
	afx_msg void OnBnClickedButton4();

	static UINT ThreadProc(LPVOID lpParam);

	afx_msg void OnDestroy();
	afx_msg void OnBnClickedButton5();
	afx_msg void OnBnClickedButton6();
	afx_msg void OnBnClickedButton7();
	afx_msg void OnBnClickedButton8();
	afx_msg void OnBnClickedButton9();
	afx_msg void OnBnClickedButton10();
	afx_msg void OnBnClickedButton11();
	afx_msg void OnBnClickedButton12();
	afx_msg void OnBnClickedButton13();
	afx_msg void OnBnClickedButton14();
	afx_msg void OnBnClickedButton16();
	afx_msg void OnBnClickedButton15();
	afx_msg void OnBnClickedButton17();
	afx_msg void OnBnClickedButton18();
};
