// ----------------------------------------------------------------
// �ļ�:FloorPage.h
// �汾: 
// ����: xy
// ����: 2006-7-6
// ----------------------------------------------------------------
// �汾��ʷ			ע��
// ----------------------------------------------------------------
// v0.1				��ʼ�汾
// 
// ----------------------------------------------------------------


#pragma once

#include "../resource.h"
#include "impublic.h"


//------------------------------------------------------
// �������л����������
//------------------------------------------------------

class CFloorPage : public CDialog
{
	DECLARE_DYNAMIC(CFloorPage)

public:
	CFloorPage(UINT nIDTemplate, CWnd* pParentWnd = NULL);
	virtual ~CFloorPage();

public:
	//------------------------------------------------------
	// ת����Ϣ�а�����������ʽ������ȡ��Ҫ���ص�����,���
	// û�б��ʽ������false
	//------------------------------------------------------
	static bool TranslateMsg(
		/*IN*/const CString & sMsg,
		/*OUT*/CString & sShowMsg,
		/*OUT*/vector<DOWNLOAD_OBJ> & arrDownloadTask
	);

protected:

	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV ֧��

	DECLARE_MESSAGE_MAP()
};
