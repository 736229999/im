// uploadpictureDlg.h : ͷ�ļ�
//

#pragma once


#import "..\..\HmgUploadPicture\HmgUploadPicture\Release\HmgUploadPicture.dll" named_guids no_namespace

// CuploadpictureDlg �Ի���
class CuploadpictureDlg : public CDialog
{
// ����
public:
	CuploadpictureDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_UPLOADPICTURE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnPaint();
	afx_msg void OnActivate(UINT nState, CWnd* pWndOther, BOOL bMinimized);
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()

	IHmgUploadPictureXPtr m_pUpload;

public:
	afx_msg void OnClose();
};
