#pragma once
#include "..\resource.h"
#include "..\im\impublic.h"
#include "..\..\SpaceManagement\public\public_controls\ResGdiObj.h"
#include "afxwin.h"
#include "..\..\..\HmgSkin\HmgSkin\CheckButton.h"
// CSysCommonFile 对话框


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
	// 更新内存
	void UpdateMem();
	// 更新界面
	void UpdateFace();
	

private:

	DuiLib::CEditUI *m_pEditRecvPath;   ////保存文件路径填充

	DuiLib::CButtonUI *m_pBtnModDir;  ////更改目录
	DuiLib::CButtonUI *m_pBtnOPenFolder;    ////打开文件夹
	DuiLib::CButtonUI *m_pBtnClear;       ////清理垃圾

	DuiLib::COptionUI *m_pCheckBuffer;  ////缓存
	DuiLib::COptionUI *m_pCheckClusterPic;////群
	DuiLib::COptionUI *m_pCheckContact;   ////联系人
	DuiLib::COptionUI *m_pCheckRecv; ////接收到

	///////查看
	//DuiLib::CButtonUI *m_pBtnBuffer; ////查看缓存
	DuiLib::CButtonUI *m_pBtnClusterPic;////查看群
	DuiLib::CButtonUI *m_pBtnSysContact;   ////查看联系人
	DuiLib::CButtonUI *m_pBtnRecv; ////查看接收到的文件
	
	/////文件个数
	DuiLib::CLabelUI *m_pLabelBufNum; ////缓存
	DuiLib::CLabelUI *m_pLabelClusterPicNum;    ////群图片
	DuiLib::CLabelUI *m_pLabelContactNum;  ////联系人
	DuiLib::CLabelUI *m_pLabelRecvNum;  ////接受到的文件

	////垃圾大小
	DuiLib::CLabelUI *m_pLabelBufSize; ////缓存垃圾
	DuiLib::CLabelUI *m_pLabelClusterPicSize;    ////群图片垃圾
	DuiLib::CLabelUI *m_pLabelContactSize;  ////联系人垃圾
	DuiLib::CLabelUI *m_pLabelRecvSize;  ////接受到的文件垃圾

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
////	CSysCommonFile(CWnd* pParent = NULL);   // 标准构造函数
////	virtual ~CSysCommonFile();
////
////	// 保存
////	void Save();
////
////// 对话框数据
////	enum { IDD = IDD_WIZARD_SYS_COMMON_FILE };
////
////protected:
////	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持
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
////	// 更新内存
////	void UpdateMem();
////	// 更新界面
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
