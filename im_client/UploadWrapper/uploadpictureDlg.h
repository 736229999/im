// uploadpictureDlg.h : 头文件
//

#pragma once


#import "..\..\HmgUploadPicture\HmgUploadPicture\Release\HmgUploadPicture.dll" named_guids no_namespace

// CuploadpictureDlg 对话框
class CuploadpictureDlg : public CDialog
{
// 构造
public:
	CuploadpictureDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_UPLOADPICTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	IHmgUploadPictureXPtr m_pUpload;

public:
	afx_msg void OnClose();
};
