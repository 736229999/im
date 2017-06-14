#pragma once
#include "..\HmgSkin\HmgSkin\ColumnCtrl.h"
#include "clientprotocol.h"
#include "im/improtocol.h"

#define HEAD_TYPE_DEFULT 1
#define HEAD_TYPE_CUSTOM 2
// �Զ�����Ϣ
#define WM_MOD_HEAD_DLG_MSG (WM_USER+412)
#define WM_PIC_PREVIEW_MSG (WM_USER+413)
#include <map>
#include <string>

// CModHeadIconDlg �Ի���
class CSelDefHeadIconDlg;
class CRountFaceCtrl;
class CSelCustomHeadIconDlg;
class CModHeadIconDlg : public CXSkinDlg
{
	DECLARE_DYNAMIC(CModHeadIconDlg)
	DECLARE_HMG_SCROLLBAR()

public:
	CModHeadIconDlg(CWnd* pParent = NULL);   // ��׼���캯��
	virtual ~CModHeadIconDlg();
	virtual BOOL OnInitDialog();
	void OnOK();
	void OnCancel();
	// ����
	BOOL Create(CWnd *pParentWnd);
	// ��ȡѡ���ͷ���url
	string GetHeadUrl();

protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��
	afx_msg void OnPaint();
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg HBRUSH OnCtlColor(CDC* pDC, CWnd* pWnd, UINT nCtlColor);
	// ѡ��Ĭ��ͷ��Ĵ��ڵ��Զ�����Ϣ��Ӧ
	afx_msg LRESULT OnSelDefHeadDlgMsg(WPARAM wp, LPARAM lp);
	afx_msg LRESULT OnPicPreviewMsg(WPARAM wP, LPARAM lP);
	DECLARE_MESSAGE_MAP()
	DECLARE_XT_MESSAGE_MAP()

private:
	CPngBtn m_btnOK;
	CPngBtn m_btnCancel;

	CColumnCtrl m_MainTab;
	CFont m_font;

	CRect m_rcTab;
	CRect m_rcBottonArea;
	CRect m_rcWorkArea;
	CRect m_rcSep;			// ��������Ԥ����֮��ķָ�������
	CRect m_rcPreview;

	CSelDefHeadIconDlg *m_pSelDefHeadDlg;
	CSelCustomHeadIconDlg *m_pSelCustomHeadDlg;
	CRountFaceCtrl *m_pPreviewPic;

	std::map<std::string, std::string> m_mapDefHeadLocal_Url;

	IMAGE_FILE_STRUCT m_imageFile;
	vector<SEND_FILE_STRUCT *>  m_vecSendFiles;
	CString m_strFilePath;
	int m_nHeadType;
public:
	afx_msg LRESULT OnColumnctrlSelChange(WPARAM wParam, LPARAM lParam);
	void OnHeadFileUploadAck(const char *data,WORD wRet);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
};
