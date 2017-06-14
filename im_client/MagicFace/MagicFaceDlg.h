// MagicFaceDlg.h : ͷ�ļ�
//

#pragma once
#include "shockwaveflash1.h"
#include "showdlg.h"

// CMagicFaceDlg �Ի���
class CMagicFaceDlg : public CDialog
{
// ����
public:
	CMagicFaceDlg(CWnd* pParent = NULL);	// ��׼���캯��

// �Ի�������
	enum { IDD = IDD_MAGICFACE_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV ֧��


// ʵ��
protected:
	HICON m_hIcon;

	// ���ɵ���Ϣӳ�亯��
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	DECLARE_MESSAGE_MAP()

protected:
	void OnProgressShockwaveflash1(long percentDone);
	void FSCommandShockwaveflash1(LPCTSTR command, LPCTSTR args);
	void FlashCallShockwaveflash1(LPCTSTR request);
	void OnReadyStateChangeShockwaveflash1(long newState);
	DECLARE_EVENTSINK_MAP()

protected:
	CShockwaveflash1 m_FlashOcx;
	CShowDlg  *      m_pDlgShow;


public:
	afx_msg void OnDestroy();
};
