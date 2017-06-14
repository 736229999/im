#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"
#include "afxwin.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonFile �Ի���


#include "..\..\..\DuiLib\UIlib.h"

class CSysCommonFile : public DuiLib::WindowImplBase
{
public:
	virtual LPCTSTR    GetWindowClassName() const			{   return _T("UICommonFileManageCfg");  }
	virtual DuiLib::CDuiString GetSkinFile()                {   return _T("FileManage.xml");  }
	virtual DuiLib::CDuiString GetSkinFolder()              {   return DuiLib::CPaintManagerUI::GetInstancePath();  }
	DuiLib::UILIB_RESOURCETYPE GetResourceType() const		{	return DuiLib::UILIB_ZIPRESOURCE;	}
	LPCTSTR GetResourceID() const							{	return MAKEINTRESOURCE(IDR_ZIPRES1);	}

	virtual void InitWindow();
	virtual void Notify(DuiLib::TNotifyUI& msg);
	void    Save();

	afx_msg void OnBnClickedBtnClear();
	afx_msg void OnBnClickedBtnDir();
	CString GetBrowseFolder(HWND pParent,CString wndTitle);
	afx_msg void OnBnClickedBtnFilefolder();
	afx_msg void OnBnClickedBtnClusterpicView();
	afx_msg void OnBnClickedBtnFriendpicView();
	afx_msg void OnBnClickedBtnAcceptView();
	
private:
	// �����ڴ�
	void UpdateMem();
	// ���½���
	void UpdateFace();
	

private:

	DuiLib::CEditUI *m_pEditRecvPath;   ////�����ļ�·�����

	DuiLib::CButtonUI *m_pBtnModDir;  ////����Ŀ¼
	DuiLib::CButtonUI *m_pBtnOPenFolder;    ////���ļ���
	DuiLib::CButtonUI *m_pBtnClear;       ////��������

	DuiLib::COptionUI *m_pCheckBuffer;  ////����
	DuiLib::COptionUI *m_pCheckClusterPic;////Ⱥ
	DuiLib::COptionUI *m_pCheckContact;   ////��ϵ��
	DuiLib::COptionUI *m_pCheckRecv; ////���յ�

	///////�鿴
	//DuiLib::CButtonUI *m_pBtnBuffer; ////�鿴����
	DuiLib::CButtonUI *m_pBtnClusterPic;////�鿴Ⱥ
	DuiLib::CButtonUI *m_pBtnSysContact;   ////�鿴��ϵ��
	DuiLib::CButtonUI *m_pBtnRecv; ////�鿴���յ����ļ�
	
	/////�ļ�����
	DuiLib::CLabelUI *m_pLabelBufNum; ////����
	DuiLib::CLabelUI *m_pLabelClusterPicNum;    ////ȺͼƬ
	DuiLib::CLabelUI *m_pLabelContactNum;  ////��ϵ��
	DuiLib::CLabelUI *m_pLabelRecvNum;  ////���ܵ����ļ�

	////������С
	DuiLib::CLabelUI *m_pLabelBufSize; ////��������
	DuiLib::CLabelUI *m_pLabelClusterPicSize;    ////ȺͼƬ����
	DuiLib::CLabelUI *m_pLabelContactSize;  ////��ϵ������
	DuiLib::CLabelUI *m_pLabelRecvSize;  ////���ܵ����ļ�����

private:

		CString m_strPicPath;
		CString m_strAcceptPath;
	
		vector<string> m_vecTempFilePath;
		double m_dTempFileSize;
	
		vector<string> m_vecClusterPic;
		double m_dClusterPicSize;
	
		vector<string> m_vecFriendPic;
		double m_dFriendPicSize;
	
		vector<string> m_vecAllAcceptFilePath;
		double m_dAccepetdFileSize;
	
		DWORD    m_dwAllTrashCnt;
		double   m_dAllFileSize;
		CString m_strNewAcceptPath;

};
////
////class CSysCommonFile : public CDialog
////{
////	DECLARE_DYNAMIC(CSysCommonFile)
////
////public:
////	CSysCommonFile(CWnd* pParent = NULL);   // ��׼���캯��
////	virtual ~CSysCommonFile();
////
////	// ����
////	void Save();
////
////// �Ի�������
////	enum { IDD = IDD_WIZARD_SYS_COMMON_FILE };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
////	virtual   BOOL OnInitDialog();
////	void OnOK()	{}
////	void OnCancel() {}
////	DECLARE_MESSAGE_MAP()
////	afx_msg void OnBnClickedBtnClear();
////	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
////	afx_msg void OnBnClickedBtnDir();
////	CString GetBrowseFolder(HWND pParent,CString wndTitle);
////	afx_msg void OnBnClickedBtnFilefolder();
////	afx_msg void OnBnClickedBtnClusterpicView();
////	afx_msg void OnBnClickedBtnFriendpicView();
////	afx_msg void OnBnClickedBtnAcceptView();
////
////private:
////	// �����ڴ�
////	void UpdateMem();
////	// ���½���
////	void UpdateFace();
////
////private:
////	CFlatButton m_btnTemp;
////	CFlatButton m_btnClusterPic;
////	CFlatButton m_btnFriendPic;
////	CFlatButton m_btnAcceptFile;
////	CPngBtn m_btnDIR;
////	CPngBtn m_btnFileOpen;
////	CPngBtn m_btnDumpClear;
////	CFlatEdit m_edtPath;
////
////	CString m_strPicPath;
////	CString m_strAcceptPath;
////
////	vector<string> m_vecTempFilePath;
////	double m_dTempFileSize;
////
////	vector<string> m_vecClusterPic;
////	double m_dClusterPicSize;
////
////	vector<string> m_vecFriendPic;
////	double m_dFriendPicSize;
////
////	vector<string> m_vecAllAcceptFilePath;
////	double m_dAccepetdFileSize;
////
////	DWORD    m_dwAllTrashCnt;
////	double   m_dAllFileSize;
////	CString m_strNewAcceptPath;
////
////	CCheckButton m_checkTemp;
////	CCheckButton m_checkCluster;
////	CCheckButton m_checkFriend;
////	CCheckButton m_checkRecive;
////};
